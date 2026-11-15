#!/usr/bin/env python3
"""Progress dashboard: verified numbers only (objdiff report + symbols.txt).

    python tools/project.py dashboard [ver]      (or: python tools/dashboard.py [ver])

Writes docs/progress/dashboard.md, appends docs/progress/history.jsonl and
replaces the block between <!-- dashboard:start --> / <!-- dashboard:end -->
in README.md. A function counts as matched only when objdiff reports it at
100 % in a non-auto-generated unit (same rule as `project.py progress`).
"""

from __future__ import annotations

import json
import sys
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

MODULE_ORDER = ["main", "itcm", "ov000", "ov001"]
START, END = "<!-- dashboard:start -->", "<!-- dashboard:end -->"
SPARK = "▁▂▃▄▅▆▇█"


def _mtime(p: Path) -> datetime | None:
    return datetime.fromtimestamp(p.stat().st_mtime, timezone.utc) if p.exists() else None


def _iso(t: datetime | None) -> str:
    return t.strftime("%Y-%m-%d %H:%M:%SZ") if t else "never"


# --------------------------------------------------------------------------
# data
def collect(ver: str = "eur") -> dict:
    """Function table with match flags, per-module totals and build health."""
    import project  # tools/project.py (symbols/config parsing)

    funcs = [{"module": r["module"], "addr": int(r["address"], 16) & ~1,
              "size": r["size"], "name": r["name"], "matched": False, "mbytes": 0}
             for r in project.all_functions(ver)]
    by_name = {f["name"]: f for f in funcs}
    rep_p = ROOT / "build" / ver / "report.json"
    report_ok = rep_p.exists()
    if report_ok:
        rep = json.loads(rep_p.read_text())
        for unit in rep.get("units", []):
            if unit.get("metadata", {}).get("auto_generated"):
                continue
            for fn in unit.get("functions", []):
                f = by_name.get(fn["name"])
                if f and fn.get("fuzzy_match_percent", 0) >= 100.0:
                    f["matched"] = True
                    f["mbytes"] = int(fn.get("size", 0))
    mods = {}
    for f in funcs:
        m = mods.setdefault(f["module"], {"functions": 0, "code_bytes": 0,
                                          "matched_functions": 0, "matched_bytes": 0})
        m["functions"] += 1
        m["code_bytes"] += f["size"]
        if f["matched"]:
            m["matched_functions"] += 1
            m["matched_bytes"] += f["mbytes"]
    total = {k: sum(m[k] for m in mods.values()) for k in
             ("functions", "code_bytes", "matched_functions", "matched_bytes")}
    return {"version": ver, "funcs": funcs, "modules": mods, "total": total,
            "report": str(rep_p.relative_to(ROOT)) if report_ok else None,
            "report_time": _mtime(rep_p), "health": health(ver)}


def health(ver: str) -> dict:
    b = ROOT / "build" / ver
    rom, ok = b / f"rush_{ver}.nds", b / f"rush_{ver}.sha1.ok"
    rom_t, ok_t = _mtime(rom), _mtime(ok)
    if ok_t is None:
        rom_state = "NOT VERIFIED (no sha1.ok)"
    elif rom_t and rom_t > ok_t:
        rom_state = "STALE/FAILED (ROM newer than last SHA-1 OK)"
    else:
        rom_state = "OK"
    rl_p = b / "reports" / "gnu_relink.json"
    relink, bad = "not run", []
    if rl_p.exists():
        rl = json.loads(rl_p.read_text())
        bad = [f"{m['module']}{s['section']}" for m in rl.get("modules", [])
               for s in m.get("sections", []) if not s.get("match")]
        nsec = sum(len(m.get("sections", [])) for m in rl.get("modules", []))
        relink = (f"OK ({nsec}/{nsec} sections)" if rl.get("match") and not bad
                  else f"FAIL ({', '.join(bad) or 'match=false'})")
    return {"rom": rom_state, "rom_ok_time": ok_t, "rom_time": rom_t,
            "relink": relink, "relink_time": _mtime(rl_p)}


def range_stats(data: dict, module: str, start: int | None, end: int | None) -> dict:
    sel = [f for f in data["funcs"] if f["module"] == module
           and (start is None or f["addr"] >= start) and (end is None or f["addr"] < end)]
    return {"functions": len(sel), "code_bytes": sum(f["size"] for f in sel),
            "matched_functions": sum(f["matched"] for f in sel),
            "matched_bytes": sum(f["mbytes"] for f in sel)}


# --------------------------------------------------------------------------
# rendering
def pct(a: int, b: int) -> float:
    return a / b * 100 if b else 0.0


def bar(p: float, width: int = 20) -> str:
    n = round(p / 100 * width)
    return "`" + "█" * n + "░" * (width - n) + "`"


def sparkline(vals: list[int]) -> str:
    if not vals:
        return ""
    lo, hi = min(vals), max(vals)
    return "".join(SPARK[0 if hi == lo else int((v - lo) / (hi - lo) * (len(SPARK) - 1))]
                   for v in vals)


def load_history(path: Path) -> list[dict]:
    if not path.exists():
        return []
    out = []
    for line in path.read_text().splitlines():
        try:
            out.append(json.loads(line))
        except ValueError:
            pass
    return out


def module_table(data: dict) -> list[str]:
    rows = ["| Module | Matched code | Bytes | Functions | Remaining |",
            "| ------ | ------------ | ----- | --------- | --------- |"]
    order = [m for m in MODULE_ORDER if m in data["modules"]] + \
            sorted(m for m in data["modules"] if m not in MODULE_ORDER)
    for name in order + ["total"]:
        m = data["total"] if name == "total" else data["modules"][name]
        p = pct(m["matched_bytes"], m["code_bytes"])
        label = "**ARM9 total**" if name == "total" else name
        rows.append(f"| {label} | {bar(p)} {p:5.2f}% | {m['matched_bytes']:,} / "
                    f"{m['code_bytes']:,} | {m['matched_functions']:,} / {m['functions']:,} | "
                    f"{m['functions'] - m['matched_functions']:,} |")
    return rows


def milestone(data: dict) -> str:
    t = data["total"]
    p = pct(t["matched_bytes"], t["code_bytes"])
    nxt = min(100, (int(p) // 10 + 1) * 10)
    need = max(0, -(-t["code_bytes"] * nxt // 100) - t["matched_bytes"])
    return (f"Next milestone: **{nxt}%** matched code — {need:,} more bytes "
            f"(currently {p:.2f}%).")


def health_lines(data: dict) -> list[str]:
    h = data["health"]
    return [f"- ROM SHA-1 (`build/{data['version']}/rush_{data['version']}.sha1.ok`): "
            f"**{h['rom']}** — last OK {_iso(h['rom_ok_time'])}",
            f"- GNU relink (`build/{data['version']}/reports/gnu_relink.json`): "
            f"**{h['relink']}** — {_iso(h['relink_time'])}",
            f"- objdiff report: `{data['report'] or 'missing (run ninja report)'}` — "
            f"{_iso(data['report_time'])}"]


def task_lines(data: dict) -> list[str]:
    try:
        import tasks as tk
        board = tk.load()
    except Exception as e:  # board optional; never block the dashboard
        return [f"- task board unavailable: {e}"]
    ts = board["tasks"]
    count = {s: sum(t["status"] == s for t in ts) for s in ("open", "claimed", "done")}
    out = [f"- {count['claimed']} claimed · {count['open']} open · {count['done']} done "
           f"(`python tools/tasks.py list`)"]
    claimed = [t for t in ts if t["status"] == "claimed"]
    if claimed:
        out += ["", "| Task | Agent | Scope | Verified in scope |",
                "| ---- | ----- | ----- | ----------------- |"]
        for t in claimed:
            out.append(f"| {t['id']} | {t.get('agent') or '-'} | {tk.scope(t)} | "
                       f"{tk.scope_progress(t, data)} |")
    return out


def history_lines(hist: list[dict]) -> list[str]:
    if not hist:
        return []
    # keep the first record and every record whose numbers changed
    changed = [h for i, h in enumerate(hist)
               if i == 0 or (h["matched_bytes"], h["matched_functions"]) !=
               (hist[i - 1]["matched_bytes"], hist[i - 1]["matched_functions"])]
    vals = [h["matched_bytes"] for h in changed][-40:]
    out = [f"Matched bytes trend: {sparkline(vals)}  ({len(changed)} changes, "
           f"{len(hist)} runs)", "",
           "| UTC | Matched bytes | % code | Functions |",
           "| --- | ------------- | ------ | --------- |"]
    for h in changed[-10:][::-1]:
        out.append(f"| {h['time']} | {h['matched_bytes']:,} | "
                   f"{pct(h['matched_bytes'], h['code_bytes']):.2f}% | "
                   f"{h['matched_functions']:,} |")
    return out


def run(ver: str = "eur", write_readme: bool = True) -> int:
    data = collect(ver)
    now = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M:%SZ")
    t = data["total"]
    prog = ROOT / "docs" / "progress"
    prog.mkdir(parents=True, exist_ok=True)
    hist_p = prog / "history.jsonl"
    if data["report"]:
        rec = {"time": now, "version": ver, **t,
               "rom_sha1": data["health"]["rom"], "relink": data["health"]["relink"],
               "modules": {k: {"matched_bytes": v["matched_bytes"],
                               "matched_functions": v["matched_functions"]}
                           for k, v in sorted(data["modules"].items())}}
        with hist_p.open("a") as f:
            f.write(json.dumps(rec, sort_keys=True) + "\n")
    hist = load_history(hist_p)

    summary = module_table(data) + ["", milestone(data), ""] + health_lines(data)
    full = (["# Progress dashboard", "",
             f"Last updated: **{now}** · version `{ver}` · regenerate with "
             "`python tools/project.py dashboard`", "",
             "Numbers come only from the objdiff report (100 % matched functions in "
             "non-generated units) and `symbols.txt` sizes. Nothing estimated, nothing "
             "unverified.", "", "## Verified matched code (ARM9)", ""] + summary +
            ["", "## Task board", ""] + task_lines(data) +
            ["", "## History", ""] + (history_lines(hist) or ["(no history yet)"]) + [""])
    (prog / "dashboard.md").write_text("\n".join(full))

    if write_readme:
        readme = ROOT / "README.md"
        txt = readme.read_text()
        block = "\n".join([START, "", f"_Verified progress, updated {now} "
                           "([full dashboard](docs/progress/dashboard.md))._", ""] +
                          summary + ["", END])
        if START in txt and END in txt:
            a, b = txt.index(START), txt.index(END) + len(END)
            txt = txt[:a] + block + txt[b:]
        else:
            anchor = "## Current state"
            i = txt.find(anchor)
            j = txt.find("\n## ", i + 1) if i >= 0 else -1
            if j < 0:
                txt = txt.rstrip("\n") + "\n\n" + block + "\n"
            else:
                txt = txt[:j] + "\n" + block + "\n" + txt[j:]
        readme.write_text(txt)

    p = pct(t["matched_bytes"], t["code_bytes"])
    print(f"dashboard: {t['matched_bytes']}/{t['code_bytes']} B ({p:.2f}%), "
          f"{t['matched_functions']}/{t['functions']} funcs; ROM {data['health']['rom']}; "
          f"relink {data['health']['relink']} -> docs/progress/dashboard.md"
          + (", README.md" if write_readme else ""))
    return 0


if __name__ == "__main__":
    sys.exit(run(sys.argv[1] if len(sys.argv) > 1 else "eur"))

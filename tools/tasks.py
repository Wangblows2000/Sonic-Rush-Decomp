#!/usr/bin/env python3
"""Persistent task board (state/tasks.json, committed). No external deps for the board.

    python tools/tasks.py list [--status open|claimed|done] [--kind partition|research]
    python tools/tasks.py add "title" [--id ID] [--kind research] [--module ov000 --start 0x.. --end 0x..]
    python tools/tasks.py claim <id> <agent> [--force]   agent report = build/agents/<agent>.txt
    python tools/tasks.py release <id> [--note TEXT]
    python tools/tasks.py done <id> [--note TEXT]
    python tools/tasks.py note <id> TEXT
    python tools/tasks.py brief                           resume summary (SessionStart hook)

Progress shown per task is verified only (objdiff report, same rule as
`project.py progress`). Agent-reported `OK` lines not yet in the report are
shown as "pending integration" and never counted.
"""

from __future__ import annotations

import argparse
import contextlib
import fcntl
import json
import os
import re
import sys
import tempfile
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BOARD = Path(os.environ.get("TASKS_BOARD", ROOT / "state" / "tasks.json"))
LOCK = BOARD.parent / ".tasks.lock"
AGENTS = ROOT / "build" / "agents"


def now() -> str:
    return datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M:%SZ")


def load() -> dict:
    if not BOARD.exists():
        return {"version": 1, "tasks": []}
    return json.loads(BOARD.read_text())


def save(board: dict) -> None:
    BOARD.parent.mkdir(parents=True, exist_ok=True)
    fd, tmp = tempfile.mkstemp(dir=BOARD.parent, prefix=".tasks.", suffix=".tmp")
    with os.fdopen(fd, "w") as f:
        json.dump(board, f, indent=2)
        f.write("\n")
    os.replace(tmp, BOARD)


@contextlib.contextmanager
def locked():
    """Exclusive lock around read-modify-write (several agents may call this)."""
    LOCK.parent.mkdir(parents=True, exist_ok=True)
    with open(LOCK, "w") as lf:
        fcntl.flock(lf, fcntl.LOCK_EX)
        board = load()
        yield board
        save(board)


def find(board: dict, tid: str) -> dict:
    for t in board["tasks"]:
        if t["id"] == tid:
            return t
    sys.exit(f"error: no task '{tid}' (see `tasks.py list`)")


def scope(t: dict) -> str:
    if not t.get("module"):
        return "-"
    if t.get("start") is None:
        return f"{t['module']} (all)"
    return f"{t['module']} {t['start']}-{t['end']}"


def _int(x):
    return None if x is None else int(x, 16)


def scope_progress(t: dict, data: dict | None) -> str:
    if not data or not t.get("module"):
        return "-"
    import dashboard
    s = dashboard.range_stats(data, t["module"], _int(t.get("start")), _int(t.get("end")))
    p = dashboard.pct(s["matched_bytes"], s["code_bytes"])
    return (f"{s['matched_bytes']:,}/{s['code_bytes']:,} B ({p:.1f}%), "
            f"{s['matched_functions']}/{s['functions']} fn")


def agent_report(agent: str | None, data: dict | None) -> str:
    """OK lines in the agent's report; how many are not yet verified by objdiff."""
    if not agent:
        return ""
    p = AGENTS / f"{agent}.txt"
    if not p.exists():
        return "no report yet"
    syms = re.findall(r"^OK (\S+)", p.read_text(errors="replace"), re.M)
    if data is None:
        return f"{len(syms)} OK lines (unverified)"
    matched = {f["name"] for f in data["funcs"] if f["matched"]}
    pending = sum(s not in matched for s in set(syms))
    return f"{len(syms)} OK lines, {pending} not yet in objdiff report"


def _data():
    try:
        sys.path.insert(0, str(ROOT / "tools"))
        import dashboard
        return dashboard.collect("eur")
    except Exception as e:  # missing build/deps: board still usable
        print(f"(progress unavailable: {type(e).__name__}: {e})")
        return None


# --------------------------------------------------------------------------
def cmd_list(a) -> int:
    board = load()
    rows = [t for t in board["tasks"]
            if (not a.status or t["status"] == a.status) and (not a.kind or t["kind"] == a.kind)]
    data = _data() if a.progress else None
    for t in rows:
        line = f"{t['id']:14s} {t['status']:8s} {(t.get('agent') or '-'):10s} {scope(t):32s} {t['title']}"
        if data:
            line += f"  [{scope_progress(t, data)}]"
        print(line)
        if a.verbose:
            for n in t.get("notes", []):
                print(f"{'':16s}{n['time']} {n['text']}")
    print(f"{len(rows)} task(s)")
    return 0


def cmd_add(a) -> int:
    with locked() as board:
        ids = {t["id"] for t in board["tasks"]}
        tid = a.id or f"t{len(board['tasks']) + 1}"
        while tid in ids:
            tid += "x"
        if bool(a.start) != bool(a.end):
            sys.exit("error: give both --start and --end")
        board["tasks"].append({
            "id": tid, "title": a.title, "kind": a.kind, "module": a.module,
            "start": a.start and f"0x{int(a.start, 16):08x}",
            "end": a.end and f"0x{int(a.end, 16):08x}",
            "status": "open", "agent": None, "created": now(),
            "claimed_at": None, "done_at": None, "notes": []})
    print(f"added {tid}")
    return 0


def _note(t: dict, text: str | None) -> None:
    if text:
        t.setdefault("notes", []).append({"time": now(), "text": text})


def cmd_claim(a) -> int:
    with locked() as board:
        t = find(board, a.id)
        if t["status"] == "claimed" and t["agent"] != a.agent and not a.force:
            sys.exit(f"error: {a.id} already claimed by {t['agent']} (use --force)")
        if t["status"] == "done" and not a.force:
            sys.exit(f"error: {a.id} is done (use --force to reopen)")
        t.update(status="claimed", agent=a.agent, claimed_at=now(), done_at=None)
        _note(t, f"claimed by {a.agent}")
    print(f"{a.id} claimed by {a.agent} (report: build/agents/{a.agent}.txt)")
    return 0


def cmd_release(a) -> int:
    with locked() as board:
        t = find(board, a.id)
        _note(t, f"released by {t.get('agent')}" + (f": {a.note}" if a.note else ""))
        t.update(status="open", agent=None, claimed_at=None)
    print(f"{a.id} released")
    return 0


def cmd_done(a) -> int:
    with locked() as board:
        t = find(board, a.id)
        t.update(status="done", done_at=now())
        _note(t, "done" + (f": {a.note}" if a.note else ""))
    print(f"{a.id} done")
    return 0


def cmd_note(a) -> int:
    with locked() as board:
        _note(find(board, a.id), a.text)
    print(f"{a.id} noted")
    return 0


def cmd_brief(a) -> int:
    board = load()
    data = _data()
    print(f"== Sonic Rush decomp: resume brief ({now()}) ==")
    if data:
        import dashboard
        t, h = data["total"], data["health"]
        print(f"verified: {t['matched_bytes']:,}/{t['code_bytes']:,} B "
              f"({dashboard.pct(t['matched_bytes'], t['code_bytes']):.2f}%) · "
              f"{t['matched_functions']:,}/{t['functions']:,} funcs · report "
              f"{dashboard._iso(data['report_time'])}")
        print(f"health: ROM SHA-1 {h['rom']} ({dashboard._iso(h['rom_ok_time'])}) · "
              f"relink {h['relink']}")
        print(dashboard.milestone(data).replace("**", ""))
    hist = ROOT / "docs" / "progress" / "history.jsonl"
    if hist.exists() and hist.stat().st_size:
        last = json.loads(hist.read_text().splitlines()[-1])
        print(f"last dashboard run: {last['time']} ({last['matched_bytes']:,} B, "
              f"{last['matched_functions']:,} funcs)")
    claimed = [t for t in board["tasks"] if t["status"] == "claimed"]
    print(f"\nclaimed ({len(claimed)}):")
    for t in claimed:
        print(f"  {t['id']:13s} {t['agent']:10s} {scope(t):32s} verified "
              f"{scope_progress(t, data)} · agent: {agent_report(t['agent'], data)}")
    opn = [t for t in board["tasks"] if t["status"] == "open"]
    print(f"\nopen ({len(opn)}), next:")
    for t in opn[:a.n]:
        extra = f" [{scope_progress(t, data)}]" if t.get("module") else ""
        print(f"  {t['id']:13s} {scope(t):32s} {t['title']}{extra}")
    done = sum(t["status"] == "done" for t in board["tasks"])
    print(f"\ndone: {done}. Protocol: CLAUDE.md · agent brief: docs/agents/AGENT_BRIEF.md · "
          "integrate+commit: bash tools/utilities/checkpoint.sh (lead only)")
    return 0


def main() -> int:
    ap = argparse.ArgumentParser(description="Sonic Rush decomp task board")
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("list")
    p.add_argument("--status", choices=["open", "claimed", "done"])
    p.add_argument("--kind", choices=["partition", "research"])
    p.add_argument("-p", "--progress", action="store_true", help="verified bytes per scope")
    p.add_argument("-v", "--verbose", action="store_true", help="show notes")
    p = sub.add_parser("add")
    p.add_argument("title")
    p.add_argument("--id")
    p.add_argument("--kind", default="partition", choices=["partition", "research"])
    p.add_argument("--module")
    p.add_argument("--start")
    p.add_argument("--end")
    p = sub.add_parser("claim")
    p.add_argument("id")
    p.add_argument("agent")
    p.add_argument("--force", action="store_true")
    for name in ("release", "done"):
        p = sub.add_parser(name)
        p.add_argument("id")
        p.add_argument("--note")
    p = sub.add_parser("note")
    p.add_argument("id")
    p.add_argument("text")
    p = sub.add_parser("brief")
    p.add_argument("-n", type=int, default=8, help="open tasks to show")
    a = ap.parse_args()
    return globals()[f"cmd_{a.cmd}"](a)


if __name__ == "__main__":
    sys.exit(main())

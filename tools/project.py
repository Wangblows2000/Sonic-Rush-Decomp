#!/usr/bin/env python3
"""Sonic Rush decompilation project CLI.

    python tools/project.py doctor               check toolchain and inputs
    python tools/project.py analyze  [ver]       ROM header/layout report (JSON)
    python tools/project.py extract  [ver]       dsd rom extract + fix-ups
    python tools/project.py delink   [ver]       dsd delink (gap/TU objects)
    python tools/project.py verify   [ver]       ROM round-trip + GNU relink check
    python tools/project.py functions [ver]      export function table (CSV)
    python tools/project.py symbols  [ver]       symbol naming statistics
    python tools/project.py coverage [ver]       code not covered by functions
    python tools/project.py progress [ver]       matched bytes/functions report
    python tools/project.py dashboard [ver]      docs/progress/dashboard.md + README block + history
    python tools/project.py ghidra   [ver]       headless Ghidra import (needs Ghidra)

All commands are deterministic and read-only with respect to the base ROM.
Reports go to build/<ver>/reports/ (not tracked by Git).
"""

from __future__ import annotations

import argparse
import csv
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import yaml  # noqa: E402
from ndslib.rom import Rom  # noqa: E402

SYM_RE = re.compile(
    r"^(?P<name>\S+) kind:(?P<kind>\w+)(?:\((?P<args>[^)]*)\))? addr:0x(?P<addr>[0-9a-f]+)")
SEC_RE = re.compile(r"^\s+(\.\w+)\s+start:0x([0-9a-f]+)\s+end:0x([0-9a-f]+)\s+kind:(\w+)")
DEFAULT_NAME_RE = re.compile(r"^(func|data|bss|__sinit)_(ov\d{3}_)?[0-9a-f]{8}$|^\.L_")


# --------------------------------------------------------------------------
# helpers
def versions() -> dict:
    return yaml.safe_load((ROOT / "config" / "versions.yaml").read_text())


def tool(name: str) -> str | None:
    for cand in (ROOT / name, ROOT / "tools" / "bin" / name):
        if cand.exists():
            return str(cand)
    return shutil.which(name)


def sha1_file(p: Path) -> str:
    h = hashlib.sha1()
    with p.open("rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def run(cmd: list[str], **kw) -> int:
    print("+", " ".join(cmd), flush=True)
    return subprocess.run(cmd, **kw).returncode


def reports_dir(ver: str) -> Path:
    d = ROOT / "build" / ver / "reports"
    d.mkdir(parents=True, exist_ok=True)
    return d


def dsd_config(ver: str) -> Path:
    return ROOT / "config" / ver / "arm9" / "config.yaml"


def modules(ver: str) -> list[dict]:
    """Module descriptors from the dsd config: name, symbols, delinks paths."""
    cfgp = dsd_config(ver)
    cfg = yaml.safe_load(cfgp.read_text())
    out = [{"name": "main", **cfg["main_module"]}]
    out += [{"name": a["name"], **a} for a in cfg.get("autoloads", [])]
    out += [{"name": o["name"], **o} for o in cfg.get("overlays", [])]
    for m in out:
        m["symbols_path"] = cfgp.parent / m["symbols"]
        m["delinks_path"] = cfgp.parent / m["delinks"]
    return out


def parse_symbols(path: Path) -> list[dict]:
    syms = []
    for line in path.read_text().splitlines():
        m = SYM_RE.match(line)
        if not m:
            continue
        d = m.groupdict()
        args = d["args"] or ""
        mode, size = None, None
        for a in args.split(","):
            a = a.strip()
            if a in ("arm", "thumb"):
                mode = a
            elif a.startswith("size="):
                size = int(a[5:], 16)
        syms.append({"name": d["name"], "kind": d["kind"], "mode": mode,
                     "size": size, "addr": int(d["addr"], 16), "raw": args})
    return syms


def parse_sections(path: Path) -> list[tuple[str, int, int, str]]:
    out = []
    for line in path.read_text().splitlines():
        if line and not line[0].isspace():
            break  # stop at first TU entry; module header comes first
        m = SEC_RE.match(line)
        if m:
            out.append((m[1], int(m[2], 16), int(m[3], 16), m[4]))
    return out


def load_evidence(ver: str) -> dict:
    p = ROOT / "config" / ver / "symbol_evidence.yaml"
    return yaml.safe_load(p.read_text()) or {} if p.exists() else {}


def name_status(name: str, evidence: dict) -> str:
    if name in evidence:
        return evidence[name].get("confidence", "tentative")
    return "unknown" if DEFAULT_NAME_RE.match(name) else "tentative"


# --------------------------------------------------------------------------
# commands
def cmd_doctor(args) -> int:
    rows = []

    def check(label, ok, detail=""):
        rows.append((label, ok, detail))

    check("python >= 3.11", sys.version_info >= (3, 11), sys.version.split()[0])
    for mod in ("yaml", "elftools", "capstone"):
        try:
            __import__(mod)
            check(f"python module {mod}", True)
        except ImportError:
            check(f"python module {mod}", False, "pip install -r tools/requirements.txt")
    for exe, why in [
        ("git", "version control"), ("ninja", "build"),
        ("dsd", "ds-decomp: extract/delink/lcf/rom build"),
        ("objdiff-cli", "function diffing and progress"),
        ("arm-none-eabi-ld", "independent relink check"),
        ("arm-none-eabi-objdump", "disassembly"),
        ("wibo", "runs Metrowerks tools on Linux"),
    ]:
        p = tool(exe)
        detail = why
        if p and exe == "dsd":
            detail = subprocess.run([p, "--version"], capture_output=True, text=True).stdout.strip()
        check(exe, bool(p), detail if p else f"missing ({why})")
    mw = ROOT / "tools" / "mwccarm"
    check("tools/mwccarm (Metrowerks)", mw.is_dir(),
          "present" if mw.is_dir() else "missing: required for matching builds (INSTALL.md)")
    gh = os.environ.get("GHIDRA_INSTALL_DIR")
    check("Ghidra (GHIDRA_INSTALL_DIR)", bool(gh and Path(gh, "support", "analyzeHeadless").exists()),
          gh or "unset: optional, see docs/research/0007-ghidra.md")
    for ver, v in versions().items():
        rom = ROOT / v["baserom"]
        if rom.exists():
            s = sha1_file(rom)
            check(f"baserom {ver}", s == v["sha1"], s if s == v["sha1"] else f"SHA-1 mismatch: {s}")
        else:
            check(f"baserom {ver}", False, f"missing: {v['baserom']}")
        bios = ROOT / "arm7_bios.bin"
        check("arm7_bios.bin", bios.exists(),
              "present" if bios.exists() else "optional: header fix-up used instead")
    w = max(len(r[0]) for r in rows)
    bad = 0
    for label, ok, detail in rows:
        print(f"{'ok  ' if ok else 'MISS'} {label:<{w}}  {detail}")
        bad += not ok
    return 0


def cmd_analyze(args) -> int:
    v = versions()[args.version]
    rom = Rom.load(ROOT / v["baserom"])
    if rom.hashes()["sha1"] != v["sha1"]:
        print("error: base ROM SHA-1 does not match config/versions.yaml", file=sys.stderr)
        return 1
    info = rom.to_dict()
    h = rom.header
    regions = [
        ("header", 0, h.header_size),
        ("arm9", h.arm9_rom_offset, h.arm9_rom_offset + h.arm9_size),
        ("arm9_ovt", h.arm9_ovt_offset, h.arm9_ovt_offset + h.arm9_ovt_size),
        ("arm7", h.arm7_rom_offset, h.arm7_rom_offset + h.arm7_size),
        ("fnt", h.fnt_offset, h.fnt_offset + h.fnt_size),
        ("fat", h.fat_offset, h.fat_offset + h.fat_size),
        ("banner", h.banner_offset, h.banner_offset + 0x840),
    ]
    for o in rom.arm9_overlays:
        f = rom.files[o.file_id]
        regions.append((f"arm9_ov{o.id:03d}", f.start, f.end))
    info["rom_regions"] = [{"name": n, "start": f"0x{s:08x}", "end": f"0x{e:08x}"}
                           for n, s, e in sorted(regions, key=lambda r: r[1])]
    ext = Counter(f.path.rsplit(".", 1)[-1].lower() if "." in f.path.rsplit("/", 1)[-1] else ""
                  for f in rom.files if not f.path.startswith("<unnamed"))
    info["file_extensions"] = dict(ext.most_common())
    info["top_level_dirs"] = sorted({f.path.split("/")[1] for f in rom.files
                                     if f.path.count("/") > 1})
    out = reports_dir(args.version) / "rom.json"
    out.write_text(json.dumps(info, indent=2) + "\n")
    print(json.dumps({k: info[k] for k in ("hashes", "header_crc_ok", "logo_crc_ok",
                                           "secure_area_decrypted", "file_count")}, indent=2))
    print(f"wrote {out.relative_to(ROOT)}")
    return 0


def cmd_extract(args) -> int:
    v = versions()[args.version]
    rom = ROOT / v["baserom"]
    if not rom.exists():
        print(f"error: put your ROM at {v['baserom']} (see extract/README.md)", file=sys.stderr)
        return 1
    if sha1_file(rom) != v["sha1"]:
        print("error: ROM SHA-1 mismatch; refusing to use a different revision", file=sys.stderr)
        return 1
    dsd = tool("dsd")
    if not dsd:
        print("error: dsd not found (INSTALL.md)", file=sys.stderr)
        return 1
    out = ROOT / "extract" / args.version
    rc = run([dsd, "rom", "extract", "--rom", str(rom), "--output-path", str(out)])
    if rc:
        return rc
    return run([sys.executable, str(ROOT / "tools/extraction/fix_path_order.py"),
                str(rom), str(out / "path_order.txt")])


def cmd_delink(args) -> int:
    dsd = tool("dsd")
    return run([dsd, "delink", "--config-path", str(dsd_config(args.version))])


def cmd_verify(args) -> int:
    v = versions()[args.version]
    dsd = tool("dsd")
    ex = ROOT / "extract" / args.version / "config.yaml"
    out = ROOT / "build" / args.version / "roundtrip.nds"
    out.parent.mkdir(parents=True, exist_ok=True)
    ok = True
    if run([dsd, "rom", "build", "--config", str(ex), "--rom", str(out)]) == 0 and \
       run([sys.executable, str(ROOT / "tools/extraction/fix_header.py"), args.version, str(out)]) == 0:
        s = sha1_file(out)
        good = s == v["sha1"]
        ok &= good
        print(f"{'OK  ' if good else 'DIFF'} ROM round-trip from extract: {s}")
    else:
        ok = False
    rc = run([sys.executable, str(ROOT / "tools/matching/gnu_relink.py"), "--version",
              args.version, "--json", str(reports_dir(args.version) / "gnu_relink.json")])
    ok &= rc == 0
    print("VERIFY:", "PASS" if ok else "FAIL")
    return 0 if ok else 1


def all_functions(ver: str) -> list[dict]:
    ev = load_evidence(ver)
    rows = []
    for m in modules(ver):
        for s in parse_symbols(m["symbols_path"]):
            if s["kind"] != "function":
                continue
            rows.append({"module": m["name"], "address": f"0x{s['addr']:08x}",
                         "size": s["size"] or 0, "mode": s["mode"], "name": s["name"],
                         "name_status": name_status(s["name"], ev), "match_status": "asm"})
    rows.sort(key=lambda r: (r["module"], r["address"]))
    return rows


def cmd_functions(args) -> int:
    rows = all_functions(args.version)
    out = reports_dir(args.version) / "functions.csv"
    with out.open("w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0].keys()))
        w.writeheader()
        w.writerows(rows)
    print(f"{len(rows)} functions -> {out.relative_to(ROOT)}")
    return 0


def cmd_symbols(args) -> int:
    ev = load_evidence(args.version)
    table = []
    for m in modules(args.version):
        c = Counter()
        for s in parse_symbols(m["symbols_path"]):
            if s["name"].startswith(".L_"):
                continue
            c[(s["kind"], name_status(s["name"], ev))] += 1
        table.append((m["name"], c))
    statuses = ["confirmed", "strong", "tentative", "unknown"]
    print(f"{'module':8s} {'kind':9s} " + " ".join(f"{s:>9s}" for s in statuses))
    for name, c in table:
        for kind in ("function", "data", "bss"):
            print(f"{name:8s} {kind:9s} " + " ".join(f"{c[(kind, s)]:9d}" for s in statuses))
    return 0


def cmd_coverage(args) -> int:
    """Report byte ranges of each module's .text not covered by a function."""
    report = {}
    for m in modules(args.version):
        secs = parse_sections(m["delinks_path"])
        text = [s for s in secs if s[3] == "code"]
        funcs = sorted((s["addr"] & ~1, s["size"] or 0)
                       for s in parse_symbols(m["symbols_path"]) if s["kind"] == "function")
        gaps, covered, total = [], 0, 0
        for _, start, end, _ in text:
            total += end - start
            cur = start
            for a, sz in funcs:
                if a + sz <= start or a >= end:
                    continue
                if a > cur and a - cur >= args.min_gap:
                    gaps.append((cur, a))
                covered += max(0, min(a + sz, end) - max(a, cur))
                cur = max(cur, a + sz)
            if end - cur >= args.min_gap:
                gaps.append((cur, end))
        code_bytes = sum(e - s for _, s, e, k in secs if k != "bss")
        report[m["name"]] = {
            "text_bytes": total, "covered": covered, "module_bytes": code_bytes,
            "functions": len(funcs),
            "gaps": [{"start": f"0x{a:08x}", "end": f"0x{b:08x}", "size": b - a} for a, b in gaps],
        }
        pct = covered / total * 100 if total else 0
        print(f"{m['name']:6s} funcs={len(funcs):5d} text={total:#9x} covered={pct:5.1f}% "
              f"module={code_bytes:#9x} gaps>={args.min_gap:#x}: {len(gaps)}")
        for a, b in gaps[:args.show]:
            print(f"         gap 0x{a:08x}-0x{b:08x} ({b - a:#x})")
    out = reports_dir(args.version) / "coverage.json"
    out.write_text(json.dumps(report, indent=2) + "\n")
    return 0


def cmd_progress(args) -> int:
    """Progress counts only code proven to match (objdiff report), never names."""
    rows = all_functions(args.version)
    by_mod = defaultdict(lambda: {"functions": 0, "code_bytes": 0,
                                  "matched_functions": 0, "matched_bytes": 0})
    for r in rows:
        b = by_mod[r["module"]]
        b["functions"] += 1
        b["code_bytes"] += r["size"]
    objdiff = ROOT / "build" / args.version / "report.json"
    source = "none (no objdiff report: run `ninja report`)"
    if objdiff.exists():
        rep = json.loads(objdiff.read_text())
        source = str(objdiff.relative_to(ROOT))
        # map function name -> module via symbol tables; count only 100 % matches
        owner = {r["name"]: r["module"] for r in rows}
        for unit in rep.get("units", []):
            if unit.get("metadata", {}).get("auto_generated"):
                continue  # dsd gap objects: original bytes, not decompiled code
            for fn in unit.get("functions", []):
                mod = owner.get(fn["name"])
                if mod and fn.get("fuzzy_match_percent", 0) >= 100.0:
                    by_mod[mod]["matched_functions"] += 1
                    by_mod[mod]["matched_bytes"] += int(fn.get("size", 0))
    ev = load_evidence(args.version)
    named = sum(1 for r in rows if r["name_status"] != "unknown")
    tot = {k: sum(b[k] for b in by_mod.values()) for k in
           ("functions", "code_bytes", "matched_functions", "matched_bytes")}
    print(f"source of match data: {source}")
    print(f"{'module':8s} {'funcs':>6s} {'matched':>8s} {'code bytes':>11s} {'matched':>9s}")
    for mod, b in sorted(by_mod.items()):
        print(f"{mod:8s} {b['functions']:6d} {b['matched_functions']:8d} "
              f"{b['code_bytes']:11d} {b['matched_bytes']:9d}")
    pct = tot["matched_bytes"] / tot["code_bytes"] * 100 if tot["code_bytes"] else 0
    print(f"{'total':8s} {tot['functions']:6d} {tot['matched_functions']:8d} "
          f"{tot['code_bytes']:11d} {tot['matched_bytes']:9d}  ({pct:.2f}% matched code)")
    print(f"named functions (not counted as progress): {named}; evidence entries: {len(ev)}")
    out = reports_dir(args.version) / "progress.json"
    out.write_text(json.dumps({"source": source, "modules": by_mod, "total": tot,
                               "named_functions": named}, indent=2, default=dict) + "\n")
    return 0


def cmd_dashboard(args) -> int:
    import dashboard  # tools/dashboard.py
    return dashboard.run(args.version)


def cmd_ghidra(args) -> int:
    gh = os.environ.get("GHIDRA_INSTALL_DIR")
    if not gh:
        print("error: set GHIDRA_INSTALL_DIR (see docs/research/0007-ghidra.md)", file=sys.stderr)
        return 1
    return run(["bash", str(ROOT / "tools/ghidra/import.sh"), args.version])


def main() -> int:
    ap = argparse.ArgumentParser(description="Sonic Rush decomp project tool")
    sub = ap.add_subparsers(dest="cmd", required=True)
    for name in ("doctor", "analyze", "extract", "delink", "verify", "functions",
                 "symbols", "coverage", "progress", "dashboard", "ghidra"):
        p = sub.add_parser(name)
        if name != "doctor":
            p.add_argument("version", nargs="?", default="eur")
        if name == "coverage":
            p.add_argument("--min-gap", type=lambda x: int(x, 0), default=0x40)
            p.add_argument("--show", type=int, default=10)
    args = ap.parse_args()
    return globals()[f"cmd_{args.cmd}"](args)


if __name__ == "__main__":
    sys.exit(main())

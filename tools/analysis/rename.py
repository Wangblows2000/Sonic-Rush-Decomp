#!/usr/bin/env python3
"""Apply function/data renames everywhere, consistently (single writer).

Input: one or more CSV files with header `module,old,new,evidence`
(module = main | itcm | dtcm | ov000 | ov001; evidence = short reason).

For every accepted row:
  * the symbol line in config/<ver>/arm9/**/symbols.txt is renamed;
  * every whole-word use in src/**/*.c, src/**/*.h and include/**/*.h is rewritten;
  * a TU source file named after the old symbol is renamed (git mv when tracked)
    and its path in delinks.txt updated;
  * an evidence entry is appended to config/<ver>/symbol_evidence.yaml.

Rows are rejected (reported, never applied) when: the old symbol does not exist
in that module, the new name is not a valid C identifier, collides with any
existing symbol in any module, or is proposed twice.

    python3 tools/analysis/rename.py names/*.csv [--dry]
"""

from __future__ import annotations

import argparse
import csv
import datetime
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
IDENT = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")
# File-local (static) SDK/NNS helpers whose official name occurs in more than one
# source file. A bare name is ambiguous and would collide, so it must carry the
# source-file stem: `<OfficialName>_<stem>` (docs/naming-conventions.md, "Repeated
# library statics"), e.g. AlarmCallback_stream, AllocFromHead_frameheap.
REPEATED_STATICS = {
    "AlarmCallback", "AllocFromHead", "AllocFromTail", "BeginSleep", "EndSleep",
    "DisposeCallback", "ForceStopStrm", "StrmCallback", "ShutdownPlayer",
    "updateHintVec_", "texmtxCalc_flagT_", "texmtxCalc_flagS_", "texmtxCalc_flagR_",
    "texmtxCalc_flagTS_", "texmtxCalc_flagTR_", "texmtxCalc_flagSR_", "texmtxCalc_flagTSR_",
    "texmtxCalc_flagNone_", "calcTexMtx_",
}
SYMLINE = re.compile(r"^(\S+)( kind:.*)$")


def module_of(path: Path) -> str:
    parts = path.parts
    if "overlays" in parts:
        return parts[parts.index("overlays") + 1]
    for m in ("itcm", "dtcm"):
        if m in parts:
            return m
    return "main"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("csv", nargs="+", type=Path)
    ap.add_argument("--version", default="eur")
    ap.add_argument("--dry", action="store_true")
    args = ap.parse_args()

    cfg = ROOT / "config" / args.version
    symfiles = {module_of(p.relative_to(cfg)): p for p in (cfg / "arm9").rglob("symbols.txt")}
    names: dict[str, set[str]] = {}
    all_names: set[str] = set()
    # mwldarm runs under wibo (Windows semantics): object files are matched
    # case-insensitively, so names that differ only in case break the link.
    for mod, p in symfiles.items():
        s = set()
        for line in p.read_text().splitlines():
            m = SYMLINE.match(line)
            if m:
                s.add(m[1])
        names[mod] = s
        all_names |= s

    lower_names = {n.lower() for n in all_names}
    rows, seen_old, seen_new, rejected = [], set(), set(), []
    for f in args.csv:
        for r in csv.DictReader(f.open()):
            mod, old, new = r["module"].strip(), r["old"].strip(), r["new"].strip()
            ev = (r.get("evidence") or "").strip()
            why = None
            if mod not in names or old not in names[mod]:
                why = "old symbol not in module"
            elif not IDENT.match(new):
                why = "invalid identifier"
            elif new in all_names or new in seen_new:
                why = "name collision"
            elif new.lower() in lower_names and new.lower() != old.lower():
                why = "case-insensitive name collision"
            elif (mod, old) in seen_old:
                why = "duplicate proposal"
            elif not ev:
                why = "no evidence"
            elif new in REPEATED_STATICS:
                why = "repeated library static: use <OfficialName>_<file stem>"
            if why:
                rejected.append((f.name, mod, old, new, why))
                continue
            seen_old.add((mod, old))
            seen_new.add(new)
            lower_names.add(new.lower())
            rows.append((mod, old, new, ev))

    for rj in rejected:
        print("REJECT", *rj, file=sys.stderr)
    print(f"accepted {len(rows)}, rejected {len(rejected)}")
    if args.dry or not rows:
        return 0

    # 1. symbols.txt
    by_mod: dict[str, dict[str, str]] = {}
    for mod, old, new, _ in rows:
        by_mod.setdefault(mod, {})[old] = new
    for mod, mp in by_mod.items():
        p = symfiles[mod]
        out = []
        for line in p.read_text().splitlines():
            m = SYMLINE.match(line)
            if m and m[1] in mp:
                line = mp[m[1]] + m[2]
            out.append(line)
        p.write_text("\n".join(out) + "\n")

    # 2. sources (names are globally unique: overlay symbols carry their prefix)
    mp_all = {old: new for _, old, new, _ in rows}
    pat = re.compile(r"\b(" + "|".join(map(re.escape, sorted(mp_all, key=len, reverse=True))) + r")\b")
    files = [*ROOT.glob("src/**/*.c"), *ROOT.glob("src/**/*.h"), *ROOT.glob("include/**/*.h")]
    for f in files:
        t = f.read_text(errors="surrogateescape")
        n = pat.sub(lambda m: mp_all[m[1]], t)
        if n != t:
            f.write_text(n, errors="surrogateescape")

    # 3. file renames + delinks paths
    moves = {}
    for f in ROOT.glob("src/**/*.c"):
        if f.stem in mp_all:
            dst = f.with_name(mp_all[f.stem] + ".c")
            if dst.exists():
                print(f"warning: {dst} exists, not renaming {f}", file=sys.stderr)
                continue
            tracked = subprocess.run(["git", "ls-files", "--error-unmatch", str(f)], cwd=ROOT,
                                     capture_output=True).returncode == 0
            if tracked:
                subprocess.run(["git", "mv", str(f), str(dst)], cwd=ROOT, check=True)
            else:
                f.rename(dst)
            moves[f.relative_to(ROOT).as_posix()] = dst.relative_to(ROOT).as_posix()
    if moves:
        for dl in (cfg / "arm9").rglob("delinks.txt"):
            t = dl.read_text()
            n = t
            for a, b in moves.items():
                n = n.replace(f"\n{a}:\n", f"\n{b}:\n")
                if n.startswith(f"{a}:\n"):
                    n = f"{b}:\n" + n[len(a) + 2:]
            if n != t:
                dl.write_text(n)

    # 4. evidence (mapping keyed by symbol name). The new row's evidence replaces
    # any existing entry for the old name (block or flow style); the old name is
    # recorded in the text.
    ev_path = cfg / "symbol_evidence.yaml"
    text = ev_path.read_text()
    stamp = datetime.date.today().isoformat()
    for mod, old, new, ev in rows:
        text = re.sub(rf"(?m)^{re.escape(old)}:(?:[ \t]*\{{.*\}}[ \t]*\n|[ \t]*\n(?:[ \t]+.*\n)*)",
                      "", text, count=1)
        conf = ev.split()[0].lower() if ev.split() and ev.split()[0].lower() in ("confirmed", "strong") else "tentative"
        esc = ev.replace("\\", "\\\\").replace('"', '\\"')
        if not text.endswith("\n"):
            text += "\n"
        text += (f"{new}:\n  confidence: {conf}\n"
                 f"  evidence: \"{esc} (was {old}, {mod}, {stamp})\"\n")
    ev_path.write_text(text)
    print(f"renamed {len(rows)} symbols; moved {len(moves)} files")
    # Post-condition: the tree must stay consistent (no duplicates, no case-only
    # collisions, no stale references, valid evidence).
    rc = subprocess.run([sys.executable, str(ROOT / "tools/analysis/check_symbols.py"),
                         "--version", args.version], cwd=ROOT).returncode
    if rc:
        print("rename.py: check_symbols FAILED after applying; fix or revert this batch",
              file=sys.stderr)
    return rc


if __name__ == "__main__":
    sys.exit(main())

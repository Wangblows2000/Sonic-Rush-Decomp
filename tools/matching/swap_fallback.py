#!/usr/bin/env python3
"""Replace registered asm TUs with matching C (lead only).

For every `OK <symbol> <path>` line in the given agent reports whose C file
still verifies with match.py, the registered asm TU for the same symbol
(src/asm_fallback/<symbol>.c or src/asm/<symbol>.c) is removed (git rm) and
its delinks.txt entry is pointed at the C file. Nothing is changed for a
symbol whose C file fails to verify.

    python3 tools/matching/swap_fallback.py build/agents/c_*.txt [--dry]
"""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
ASM_DIRS = ("src/asm_fallback", "src/asm")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("reports", nargs="+", type=Path)
    ap.add_argument("--dry", action="store_true")
    a = ap.parse_args()
    dls = list((ROOT / "config/eur/arm9").rglob("delinks.txt"))
    texts = {p: p.read_text() for p in dls}
    done = 0
    for rep in a.reports:
        for line in rep.read_text().splitlines():
            m = re.match(r"^OK\s+(\S+)\s+(\S+\.c)\s*$", line)
            if not m:
                continue
            sym, cpath = m[1], m[2]
            c = ROOT / cpath
            if not c.exists():
                c = next(iter(ROOT.glob(f"src/*/{sym}.c")), None)
                if c is None or c.parts[-2] in ("asm_fallback", "asm"):
                    print(f"skip {sym}: C file missing")
                    continue
            old = next((f"{d}/{sym}.c" for d in ASM_DIRS if (ROOT / d / f"{sym}.c").exists()), None)
            if old is None:
                print(f"skip {sym}: no registered asm TU")
                continue
            r = subprocess.run([sys.executable, "tools/matching/match.py", str(c.relative_to(ROOT))],
                               cwd=ROOT, capture_output=True, text=True)
            if r.returncode != 0:
                print(f"skip {sym}: C does not verify")
                continue
            new = c.relative_to(ROOT).as_posix()
            hit = [p for p, t in texts.items() if f"\n{old}:\n" in "\n" + t]
            if not hit:
                print(f"skip {sym}: {old} not registered")
                continue
            print(f"swap {sym}: {old} -> {new}")
            done += 1
            if a.dry:
                continue
            for p in hit:
                t = "\n" + texts[p]
                texts[p] = t.replace(f"\n{old}:\n", f"\n{new}:\n", 1)[1:]
            subprocess.run(["git", "rm", "-q", old], cwd=ROOT, check=True)
    if not a.dry:
        for p, t in texts.items():
            if p.read_text() != t:
                p.write_text(t)
    print(f"swapped {done}")
    return 0


if __name__ == "__main__":
    sys.exit(main())

#!/usr/bin/env python3
"""Fast SHA-1 failure repair: map ROM byte differences to TUs (rom_diff.py),
move those TU sources to build/agents/rejected/, and re-run configure (which
prunes their delinks entries). Much faster than bisect_rom.py when only a
handful of TUs are wrong. Exit 0 if something was ejected, 1 if no TU could
be blamed (caller should fall back to bisect_rom.py).

Usage: python3 tools/matching/eject_diffs.py [--limit N]
"""
import argparse
import re
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--limit", type=int, default=200)
    a = ap.parse_args()
    r = subprocess.run([sys.executable, "tools/matching/rom_diff.py", "--limit", str(a.limit)],
                       cwd=ROOT, capture_output=True, text=True)
    print(r.stdout[-4000:])
    tus = sorted(set(re.findall(r"TU=(src/\S+\.c)", r.stdout)))
    if not tus:
        print("eject_diffs: no TU blamed", file=sys.stderr)
        return 1
    rej = ROOT / "build/agents/rejected"
    rej.mkdir(parents=True, exist_ok=True)
    for t in tus:
        src = ROOT / t
        if src.exists():
            shutil.move(str(src), str(rej / t.replace("/", "_")))
            print(f"ejected {t}")
    subprocess.run([sys.executable, "tools/configure.py", "eur"], cwd=ROOT, check=True)
    return 0


if __name__ == "__main__":
    sys.exit(main())

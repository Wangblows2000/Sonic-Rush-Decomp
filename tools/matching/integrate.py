#!/usr/bin/env python3
"""Integrate verified C files into the build (single writer).

For every src/**/*.c not yet listed in any delinks.txt: re-run match.py; if all
of its functions match, append a TU entry to the owning module's delinks.txt.
Then (unless --no-build) regenerate build.ninja, build the ROM, check SHA-1,
run the GNU relink check and the objdiff report. Files that fail stay out of
the build and are listed.
"""
from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


def listed() -> set[str]:
    out = set()
    for p in (ROOT / "config").rglob("delinks.txt"):
        for line in p.read_text().splitlines():
            if line and not line[0].isspace() and line.endswith(":"):
                out.add(line[:-1])
    return out


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--no-build", action="store_true")
    args = ap.parse_args()
    have = listed()
    new = [p for p in sorted((ROOT / "src").rglob("*.c")) if p.relative_to(ROOT).as_posix() not in have]
    ok, bad = 0, []
    for p in new:
        r = subprocess.run([sys.executable, str(ROOT / "tools/matching/match.py"), str(p), "--promote"],
                           cwd=ROOT, capture_output=True, text=True)
        if r.returncode == 0 and "promoted" in r.stdout:
            ok += 1
        else:
            bad.append((p.relative_to(ROOT).as_posix(), (r.stdout.splitlines() or ["?"])[0]))
    print(f"integrated {ok} new files; {len(bad)} not integrated")
    for f, why in bad:
        print(f"  skip {f}: {why}")
    if args.no_build:
        return 0
    steps = [[sys.executable, "tools/configure.py", "eur"], ["ninja"], ["ninja", "verify"], ["ninja", "report"],
             [sys.executable, "tools/project.py", "progress"]]
    for s in steps:
        r = subprocess.run(s, cwd=ROOT, capture_output=True, text=True)
        tail = [l for l in (r.stdout + r.stderr).splitlines() if "interworking" not in l][-3:]
        print("$", " ".join(s), "->", r.returncode, *tail, sep="\n  ")
        if r.returncode:
            return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())

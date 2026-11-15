#!/usr/bin/env python3
"""Re-verify every registered TU with match.py (parallel). Optional --cc.
Regression test: exit 0 only if all registered files still match."""
import argparse, re, glob, subprocess, sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
ap = argparse.ArgumentParser(); ap.add_argument("--cc"); ap.add_argument("-j", type=int, default=4)
a = ap.parse_args()
files = sorted({f for p in glob.glob(str(ROOT / "config/eur/arm9/**/delinks.txt"), recursive=True)
                for f in re.findall(r"^(src/\S+\.c):$", open(p).read(), re.M)})
def run(f):
    cmd = [sys.executable, str(ROOT / "tools/matching/match.py"), f] + (["--cc", a.cc] if a.cc else [])
    return f, subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True).returncode
with ThreadPoolExecutor(a.j) as ex:
    res = list(ex.map(run, files))
bad = [f for f, rc in res if rc]
print(f"{len(files) - len(bad)}/{len(files)} match" + (f" with {a.cc}" if a.cc else ""))
for f in bad[:40]: print("  FAIL", f)
sys.exit(1 if bad else 0)

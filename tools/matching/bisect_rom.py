#!/usr/bin/env python3
"""Find and eject TUs that break the ROM SHA-1 (regression guard).

Candidates = TU entries in config/**/delinks.txt that are not in git HEAD.
Binary search: disable half of the candidates, rebuild, check SHA-1. Each
culprit is removed from delinks.txt and its file moved to build/agents/rejected/.
Exit 0 when the ROM matches again.
"""
from __future__ import annotations

import re
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
ENTRY = re.compile(r"^(src/\S+\.c):\n(?:    .*\n?)*", re.M)


def entries(text: str) -> dict[str, str]:
    return {m.group(1): m.group(0) for m in ENTRY.finditer(text)}


def head_entries() -> set[str]:
    out = set()
    for p in (ROOT / "config").rglob("delinks.txt"):
        rel = p.relative_to(ROOT).as_posix()
        r = subprocess.run(["git", "show", f"HEAD:{rel}"], cwd=ROOT, capture_output=True, text=True)
        out |= set(entries(r.stdout))
    return out


def set_disabled(disabled: set[str], originals: dict[Path, str]) -> None:
    for p, text in originals.items():
        new = text
        for f in disabled:
            new = re.sub(rf"\n*^{re.escape(f)}:\n(?:    .*\n?)*", "\n", new, flags=re.M)
        p.write_text(new.rstrip("\n") + "\n")


def rom_ok() -> bool:
    subprocess.run([sys.executable, "tools/configure.py", "eur"], cwd=ROOT, capture_output=True)
    r = subprocess.run(["ninja"], cwd=ROOT, capture_output=True, text=True)
    return r.returncode == 0 and "OK" in r.stdout.splitlines()[-1] if r.stdout else False


def main() -> int:
    originals = {p: p.read_text() for p in (ROOT / "config").rglob("delinks.txt")}
    cur = set()
    for t in originals.values():
        cur |= set(entries(t))
    cand = sorted(cur - head_entries())
    print(f"{len(cand)} new TUs since HEAD")
    if rom_ok():
        print("ROM already matches")
        return 0
    bad: set[str] = set()
    pending = [cand]
    while pending:
        group = pending.pop()
        # does disabling this group (plus known-bad) fix the ROM?
        set_disabled(bad | set(group), originals)
        if not rom_ok():
            continue  # culprit is not (only) in this group
        if len(group) == 1:
            bad.add(group[0])
            print("culprit:", group[0])
            set_disabled(bad, originals)
            if rom_ok():
                break
            continue
        half = len(group) // 2
        pending += [group[half:], group[:half]]
    set_disabled(bad, originals)
    rej = ROOT / "build/agents/rejected"
    rej.mkdir(parents=True, exist_ok=True)
    for f in bad:
        if (ROOT / f).exists():
            shutil.move(str(ROOT / f), str(rej / Path(f).name))
    ok = rom_ok()
    print(f"ejected {len(bad)}; ROM {'matches' if ok else 'STILL BROKEN'}")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())

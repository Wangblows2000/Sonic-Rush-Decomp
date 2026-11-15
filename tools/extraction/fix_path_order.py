#!/usr/bin/env python3
"""Rewrite extract/<ver>/path_order.txt from the base ROM's FAT.

ds-rom (used by dsd) reconstructs the order in which NitroFS files are laid
out in the file image. Zero-length files share their FAT offset with the next
non-empty file, and ds-rom 0.8 orders some of them after that file (or not at
all), which moves their FAT offsets in the rebuilt ROM.

The original image order is fully determined by the base ROM's FAT: sort
named files by start offset, placing zero-length files before a non-empty
file at the same offset (observed rule; see docs/research/0005-rom-rebuild.md).
Overlay files (unnamed, ids < number of overlays) are excluded because ds-rom
places them itself.
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from ndslib.rom import Rom  # noqa: E402


def compute_order(rom: Rom) -> list[str]:
    named = [f for f in rom.files if not f.path.startswith("<unnamed")]
    named.sort(key=lambda f: (f.start, f.size != 0, f.id))
    return [f.path for f in named]


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("rom", type=Path)
    ap.add_argument("path_order", type=Path)
    ap.add_argument("--check", action="store_true",
                    help="only report whether the file would change")
    args = ap.parse_args()

    rom = Rom.load(args.rom)
    new = compute_order(rom)
    old = args.path_order.read_text().split("\n") if args.path_order.exists() else []
    old = [x for x in old if x]
    if old == new:
        print(f"{args.path_order}: already correct ({len(new)} entries)")
        return 0
    moved = [p for p in new if p not in old]
    print(f"{args.path_order}: {len(old)} -> {len(new)} entries; "
          f"{len(moved)} previously missing")
    if args.check:
        return 1
    args.path_order.write_text("\n".join(new) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())

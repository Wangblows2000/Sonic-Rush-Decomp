#!/usr/bin/env python3
"""Map differences between the built ROM and the base ROM to functions.

    python tools/matching/rom_diff.py [--limit 30]
Prints the functions (and owning TU, if any) containing differing bytes, in
address order, plus whether the module's code shifted (size mismatch).
"""
from __future__ import annotations

import argparse
import bisect
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from ndslib.rom import Rom  # noqa: E402
from project import modules, parse_symbols  # noqa: E402


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--limit", type=int, default=30)
    args = ap.parse_args()
    a = Rom.load(ROOT / "extract/baserom_rush_eur.nds")
    b = Rom.load(ROOT / "build/eur/rush_eur.nds")
    h = a.header
    regions = [("main", h.arm9_rom_offset, 0x02000000)]
    for o in a.arm9_overlays:
        regions.append((f"ov{o.id:03d}", a.files[o.file_id].start, o.ram_address))
    funcs, tus = {}, {}
    for m in modules("eur"):
        L = sorted((s["addr"], s["size"] or 0, s["name"]) for s in parse_symbols(m["symbols_path"])
                   if s["kind"] == "function")
        funcs[m["name"]] = L
        t = m["delinks_path"].read_text()
        tus[m["name"]] = [(int(x, 16), int(y, 16), f) for f, x, y in re.findall(
            r"^(\S+):\n    complete\n    \.text\s+start:0x([0-9a-f]+) end:0x([0-9a-f]+)", t, re.M)]
    hits = {}
    for mod, off, base in regions:
        L = funcs[mod]
        starts = [x[0] for x in L]
        size = (a.files[[o.file_id for o in a.arm9_overlays if f"ov{o.id:03d}" == mod][0]].end - off) \
            if mod != "main" else 0x7CFE0
        for i in range(off, off + size):
            if a.data[i] == b.data[i]:
                continue
            addr = base + i - off
            k = bisect.bisect_right(starts, addr) - 1
            fn = L[k][2] if k >= 0 and addr < L[k][0] + L[k][1] else "?"
            tu = next((f for x, y, f in tus[mod] if x <= addr < y), "-")
            hits.setdefault((mod, fn), [addr, 0, tu])[1] += 1
    other = sum(1 for i in range(0x200) if a.data[i] != b.data[i])
    print(f"header bytes differing: {other}; functions with differences: {len(hits)}")
    for (mod, fn), (addr, n, tu) in sorted(hits.items(), key=lambda x: x[1][0])[:args.limit]:
        print(f"  {mod:5s} 0x{addr:08x} {fn:28s} {n:5d} B  TU={tu}")
    return 0


if __name__ == "__main__":
    sys.exit(main())

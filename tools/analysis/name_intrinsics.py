#!/usr/bin/env python3
"""Name compiler runtime helpers (e.g. _s32_div_f) from compiled TUs.

When a matched TU calls a symbol that the compiler invented (division, 64-bit
math, float helpers), the original ROM has a BL to the real helper at the same
offset. The relocation in relocs.txt gives its address, so the helper's dsd
name can be replaced with the compiler's name: confirmed evidence.

    python tools/analysis/name_intrinsics.py   (after ninja has compiled the TUs)
"""
from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from project import modules, parse_symbols  # noqa: E402

REL = re.compile(r"from:0x([0-9a-f]+) kind:\S+ to:0x([0-9a-f]+)")


def main() -> int:
    mods = modules("eur")
    symaddr, addrsym, relocs = {}, {}, {}
    for m in mods:
        for s in parse_symbols(m["symbols_path"]):
            symaddr[s["name"]] = (m, s["addr"])
            if s["kind"] == "function":
                addrsym[s["addr"]] = (m, s["name"])
        for line in (m["symbols_path"].parent / "relocs.txt").read_text().splitlines():
            r = REL.match(line)
            if r:
                relocs[int(r[1], 16)] = int(r[2], 16)
    renames = {}
    for obj in (ROOT / "build/eur/src").rglob("*.o"):
        with obj.open("rb") as fh:
            e = ELFFile(fh)
            st = e.get_section_by_name(".symtab")
            syms = list(st.iter_symbols())
            funcs = {s.name: s for s in syms if s["st_info"]["type"] == "STT_FUNC" and s["st_shndx"] != "SHN_UNDEF"}
            for rs in e.iter_sections():
                if not isinstance(rs, RelocationSection):
                    continue
                for r in rs.iter_relocations():
                    tgt = syms[r["r_info_sym"]]
                    if tgt["st_shndx"] != "SHN_UNDEF" or tgt.name in symaddr or not tgt.name.startswith("_"):
                        continue
                    # which function of this object contains the reloc?
                    for fn in funcs.values():
                        start = fn["st_value"] & ~1
                        if start <= r["r_offset"] < start + fn["st_size"] and fn.name in symaddr:
                            rom = symaddr[fn.name][1] + (r["r_offset"] - start)
                            dest = relocs.get(rom)
                            if dest is not None and (dest & ~1) in addrsym:
                                m, old = addrsym[dest & ~1]
                                if re.match(r"^func_", old):
                                    renames[old] = (tgt.name, m, dest, fn.name)
    for old, (new, m, dest, via) in renames.items():
        p = m["symbols_path"]
        p.write_text(re.sub(rf"^{re.escape(old)} ", f"{new} ", p.read_text(), flags=re.M))
        for src in (ROOT / "src").rglob("*.c"):
            t = src.read_text()
            if re.search(rf"\b{old}\b", t):
                src.write_text(re.sub(rf"\b{old}\b", new, t))
        ev = ROOT / "config/eur/symbol_evidence.yaml"
        ev.write_text(ev.read_text().rstrip("\n") + f'\n{new}: {{confidence: confirmed, evidence: "compiler-generated call '
                      f'from matched {via} resolves to 0x{dest:08x} in the ROM"}}\n')
        print(f"{old} -> {new} (0x{dest:08x}, via {via})")
    return 0


if __name__ == "__main__":
    sys.exit(main())

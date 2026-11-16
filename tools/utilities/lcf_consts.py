#!/usr/bin/env python3
"""Add project-defined linker constants to the dsd-generated LCF.

relocs.txt entries `kind:link_time_const(NAME=0xVALUE)` (e.g. NitroSDK's
SDK_SYS_STACKSIZE) are referenced by C as absolute linker symbols; this defines
each one (`NAME = VALUE;`) next to dsd's own constants. Idempotent.

    python3 tools/utilities/lcf_consts.py build/eur/arm9.lcf [config dir]
"""
import re
import sys
from pathlib import Path

lcf = Path(sys.argv[1])
cfg = Path(sys.argv[2] if len(sys.argv) > 2 else "config")
consts = {}
for rel in cfg.rglob("relocs.txt"):
    for name, val in re.findall(r"link_time_const\(([A-Za-z_]\w*)=(0x[0-9a-fA-F]+|\d+)\)", rel.read_text()):
        consts[name] = int(val, 0)
text = lcf.read_text()
lines = [f"    {n} = 0x{v:08x};" for n, v in sorted(consts.items()) if not re.search(rf"\b{n}\s*=", text)]
if lines:
    m = re.search(r"(?m)^(\s*__OVERLAY_COUNT\s*=.*;)$", text)
    if not m:
        sys.exit("lcf_consts: __OVERLAY_COUNT anchor not found")
    text = text[:m.end()] + "\n" + "\n".join(lines) + text[m.end():]
    lcf.write_text(text)
print(f"lcf_consts: {len(consts)} constants")

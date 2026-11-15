#!/usr/bin/env python3
"""Apply recorded header fix-ups to a rebuilt ROM and recompute the header CRC.

Only fields listed under `header_fixups` in config/versions.yaml are touched.
Currently that is the secure-area CRC (0x6C), which cannot be recomputed
without the console's ARM7 BIOS (KEY1 tables). If the user supplies
arm7_bios.bin, prefer passing it to `dsd rom build --arm7-bios` instead and
skip this script.
"""

from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

import yaml

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from ndslib.rom import crc16_modbus  # noqa: E402


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("version")
    ap.add_argument("rom", type=Path)
    ap.add_argument("--versions", type=Path, default=Path("config/versions.yaml"))
    args = ap.parse_args()

    cfg = yaml.safe_load(args.versions.read_text())[args.version]
    fix = cfg.get("header_fixups", {}) or {}
    data = bytearray(args.rom.read_bytes())
    if "secure_area_crc" in fix:
        cur = struct.unpack_from("<H", data, 0x6C)[0]
        if cur not in (0, fix["secure_area_crc"]):
            print(f"refusing: secure-area CRC is {cur:#06x}, expected 0 or "
                  f"{fix['secure_area_crc']:#06x}", file=sys.stderr)
            return 1
        struct.pack_into("<H", data, 0x6C, fix["secure_area_crc"])
    struct.pack_into("<H", data, 0x15E, crc16_modbus(bytes(data[:0x15E])))
    args.rom.write_bytes(data)
    return 0


if __name__ == "__main__":
    sys.exit(main())

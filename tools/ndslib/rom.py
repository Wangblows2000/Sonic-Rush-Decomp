"""Minimal, dependency-free Nintendo DS ROM parser.

Covers the parts of the cartridge image a decompilation needs to reason about:
header, ARM9/ARM7 binaries, overlay tables (OVT), file name table (FNT),
file allocation table (FAT) and banner. Field layout follows GBATEK
("DS Cartridge Header", "DS Cartridge NitroROM and NitroARC File Systems").

This module never modifies a ROM. It is used both by the `project.py analyze`
command and by ad-hoc research scripts.
"""

from __future__ import annotations

import hashlib
import struct
from dataclasses import dataclass, field, asdict
from pathlib import Path


def crc16_modbus(data: bytes, crc: int = 0xFFFF) -> int:
    """CRC-16 used by the DS header (poly 0xA001 reflected, init 0xFFFF)."""
    for b in data:
        crc ^= b
        for _ in range(8):
            crc = (crc >> 1) ^ 0xA001 if crc & 1 else crc >> 1
    return crc


@dataclass
class Header:
    title: str
    game_code: str
    maker_code: str
    unit_code: int
    device_capacity: int
    rom_version: int
    autostart: int
    arm9_rom_offset: int
    arm9_entry: int
    arm9_ram_address: int
    arm9_size: int
    arm7_rom_offset: int
    arm7_entry: int
    arm7_ram_address: int
    arm7_size: int
    fnt_offset: int
    fnt_size: int
    fat_offset: int
    fat_size: int
    arm9_ovt_offset: int
    arm9_ovt_size: int
    arm7_ovt_offset: int
    arm7_ovt_size: int
    banner_offset: int
    secure_area_crc: int
    secure_transfer_timeout: int
    arm9_autoload_done: int
    arm7_autoload_done: int
    used_rom_size: int
    header_size: int
    logo_crc: int
    header_crc: int

    @classmethod
    def parse(cls, d: bytes) -> "Header":
        u32 = lambda o: struct.unpack_from("<I", d, o)[0]
        u16 = lambda o: struct.unpack_from("<H", d, o)[0]
        return cls(
            title=d[0:12].rstrip(b"\0").decode("ascii", "replace"),
            game_code=d[12:16].decode("ascii", "replace"),
            maker_code=d[16:18].decode("ascii", "replace"),
            unit_code=d[0x12],
            device_capacity=d[0x14],
            rom_version=d[0x1E],
            autostart=d[0x1F],
            arm9_rom_offset=u32(0x20), arm9_entry=u32(0x24),
            arm9_ram_address=u32(0x28), arm9_size=u32(0x2C),
            arm7_rom_offset=u32(0x30), arm7_entry=u32(0x34),
            arm7_ram_address=u32(0x38), arm7_size=u32(0x3C),
            fnt_offset=u32(0x40), fnt_size=u32(0x44),
            fat_offset=u32(0x48), fat_size=u32(0x4C),
            arm9_ovt_offset=u32(0x50), arm9_ovt_size=u32(0x54),
            arm7_ovt_offset=u32(0x58), arm7_ovt_size=u32(0x5C),
            banner_offset=u32(0x68),
            secure_area_crc=u16(0x6C), secure_transfer_timeout=u16(0x6E),
            arm9_autoload_done=u32(0x70), arm7_autoload_done=u32(0x74),
            used_rom_size=u32(0x80), header_size=u32(0x84),
            logo_crc=u16(0x15C), header_crc=u16(0x15E),
        )


@dataclass
class Overlay:
    id: int
    ram_address: int
    ram_size: int
    bss_size: int
    sinit_start: int
    sinit_end: int
    file_id: int
    compressed_size: int  # 24-bit; 0 when not compressed
    flags: int            # bit0 = compressed, bit1 = authenticated

    @property
    def is_compressed(self) -> bool:
        return bool(self.flags & 1)


@dataclass
class FileEntry:
    id: int
    path: str
    start: int
    end: int

    @property
    def size(self) -> int:
        return self.end - self.start


@dataclass
class Rom:
    path: Path
    data: bytes
    header: Header
    arm9_overlays: list[Overlay] = field(default_factory=list)
    arm7_overlays: list[Overlay] = field(default_factory=list)
    files: list[FileEntry] = field(default_factory=list)

    @classmethod
    def load(cls, path: str | Path) -> "Rom":
        path = Path(path)
        data = path.read_bytes()
        if len(data) < 0x200:
            raise ValueError(f"{path}: too small to be an NDS ROM")
        rom = cls(path=path, data=data, header=Header.parse(data))
        h = rom.header
        rom.arm9_overlays = rom._parse_ovt(h.arm9_ovt_offset, h.arm9_ovt_size)
        rom.arm7_overlays = rom._parse_ovt(h.arm7_ovt_offset, h.arm7_ovt_size)
        rom.files = rom._parse_filesystem()
        return rom

    # -- tables -----------------------------------------------------------
    def _parse_ovt(self, off: int, size: int) -> list[Overlay]:
        out = []
        for i in range(size // 32):
            f = struct.unpack_from("<8I", self.data, off + i * 32)
            out.append(Overlay(f[0], f[1], f[2], f[3], f[4], f[5], f[6],
                               f[7] & 0xFFFFFF, f[7] >> 24))
        return out

    def fat(self) -> list[tuple[int, int]]:
        h = self.header
        return [struct.unpack_from("<II", self.data, h.fat_offset + 8 * i)
                for i in range(h.fat_size // 8)]

    def _parse_filesystem(self) -> list[FileEntry]:
        h = self.header
        fnt = self.data[h.fnt_offset:h.fnt_offset + h.fnt_size]
        fat = self.fat()
        names: dict[int, str] = {}

        def walk(dir_id: int, prefix: str) -> None:
            ent = (dir_id & 0xFFF) * 8
            sub_off, first_id, _ = struct.unpack_from("<IHH", fnt, ent)
            p, fid = sub_off, first_id
            while True:
                t = fnt[p]
                p += 1
                if t == 0:
                    break
                n = t & 0x7F
                name = fnt[p:p + n].decode("shift_jis", "replace")
                p += n
                if t & 0x80:
                    sub = struct.unpack_from("<H", fnt, p)[0]
                    p += 2
                    walk(sub, f"{prefix}{name}/")
                else:
                    names[fid] = prefix + name
                    fid += 1

        walk(0xF000, "/")
        out = []
        for i, (s, e) in enumerate(fat):
            out.append(FileEntry(i, names.get(i, f"<unnamed:{i}>"), s, e))
        return out

    # -- helpers ----------------------------------------------------------
    def hashes(self) -> dict[str, str]:
        return {
            "sha1": hashlib.sha1(self.data).hexdigest(),
            "sha256": hashlib.sha256(self.data).hexdigest(),
            "size": len(self.data),
        }

    def header_crc_ok(self) -> bool:
        return crc16_modbus(self.data[:0x15E]) == self.header.header_crc

    def logo_crc_ok(self) -> bool:
        return crc16_modbus(self.data[0xC0:0x15C]) == self.header.logo_crc

    def secure_area_is_decrypted(self) -> bool:
        # A decrypted secure area begins with 0xE7FFDEFF twice
        # (GBATEK: "DS Encryption by Random Seed / KEY1").
        off = self.header.arm9_rom_offset
        return self.data[off:off + 8] == bytes.fromhex("ffdeffe7ffdeffe7")

    def to_dict(self) -> dict:
        return {
            "path": str(self.path.name),
            "hashes": self.hashes(),
            "header": asdict(self.header),
            "header_crc_ok": self.header_crc_ok(),
            "logo_crc_ok": self.logo_crc_ok(),
            "secure_area_decrypted": self.secure_area_is_decrypted(),
            "arm9_overlays": [asdict(o) for o in self.arm9_overlays],
            "arm7_overlays": [asdict(o) for o in self.arm7_overlays],
            "file_count": len(self.files),
        }

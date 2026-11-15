"""Unit tests for tools/ndslib and extraction helpers. No ROM required.

Run: python -m unittest discover -s tests
"""

import struct
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "extraction"))

from ndslib.rom import Rom, crc16_modbus  # noqa: E402
import fix_path_order  # noqa: E402


def make_rom() -> bytes:
    """Tiny synthetic NDS image: header, FNT with /a/{x,y,empty}, FAT."""
    d = bytearray(0x1000)
    d[0:12] = b"TESTROM\0\0\0\0\0"
    d[12:16] = b"TEST"
    d[16:18] = b"01"
    # FNT at 0x400: root dir entry + one subdir entry
    fnt = bytearray()
    root = struct.pack("<IHH", 16, 0, 2)        # subtable at 16, first file 0, 2 dirs
    sub = struct.pack("<IHH", 0, 0, 0xF000)     # filled below
    tbl_root = bytes([0x81]) + b"a" + struct.pack("<H", 0xF001) + b"\0"
    off_sub = 16 + len(tbl_root)
    sub = struct.pack("<IHH", off_sub, 0, 0xF000)
    tbl_sub = bytes([1]) + b"x" + bytes([1]) + b"y" + bytes([5]) + b"empty" + b"\0"
    fnt = root + sub + tbl_root + tbl_sub
    d[0x400:0x400 + len(fnt)] = fnt
    struct.pack_into("<II", d, 0x40, 0x400, len(fnt))
    # FAT at 0x600: x@0x800-0x810, y@0x900-0x920, empty@0x900 (zero length)
    fat = [(0x800, 0x810), (0x900, 0x920), (0x900, 0x900)]
    for i, (s, e) in enumerate(fat):
        struct.pack_into("<II", d, 0x600 + 8 * i, s, e)
    struct.pack_into("<II", d, 0x48, 0x600, 8 * len(fat))
    struct.pack_into("<H", d, 0x15E, crc16_modbus(bytes(d[:0x15E])))
    return bytes(d)


class TestRom(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.path = Path(self.tmp.name) / "t.nds"
        self.path.write_bytes(make_rom())

    def tearDown(self):
        self.tmp.cleanup()

    def test_crc16_known_vector(self):
        # CRC-16/MODBUS("123456789") = 0x4B37
        self.assertEqual(crc16_modbus(b"123456789"), 0x4B37)

    def test_header_and_fs(self):
        r = Rom.load(self.path)
        self.assertEqual(r.header.game_code, "TEST")
        self.assertTrue(r.header_crc_ok())
        self.assertEqual([f.path for f in r.files], ["/a/x", "/a/y", "/a/empty"])
        self.assertEqual(r.files[2].size, 0)

    def test_path_order_puts_empty_file_first(self):
        r = Rom.load(self.path)
        self.assertEqual(fix_path_order.compute_order(r), ["/a/x", "/a/empty", "/a/y"])


if __name__ == "__main__":
    unittest.main()

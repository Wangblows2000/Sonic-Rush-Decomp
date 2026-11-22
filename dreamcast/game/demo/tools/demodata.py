#!/usr/bin/env python3
"""Attract-demo data converter (BYOR: reads only the owner's extracted ROM).

Writes build/dreamcast/demo/:
  demotab.bin   "RDMO" v1: u32 magic, u16 version, u16 count, then count x 0x22-byte
                entries copied unchanged from the DemoPlay_Create table (arm9
                data_02073bd0): u8 zone, u8 act, u16 character, u16 start x,
                u16 start y, u16 frame limit (data_02073bd8), char key file[24].
  key/keyNN.bin the recorded pad input files (/dat/key/keyNN.bin), copied
                unchanged: u32 gRandSeed, then {u16 keys, u8 frames, u8 pad}
                records, frames 0 = end (DemoInput_Main).

Usage: python3 dreamcast/game/demo/tools/demodata.py [--out build/dreamcast/demo]
"""
import argparse
import shutil
import struct
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
EXT = ROOT / "extract/eur"
TABLE = 0x02073bd0      # data_02073bd0 (DemoPlay_Create: ENT(sym, id) = sym + id * 0x22)
ENTRY = 0x22
COUNT = 4               # TitleScreen_StartAttractDemo cycles gGameState.unk1E through 0..3


def build(out):
    a9 = (EXT / "arm9/arm9.bin").read_bytes()
    out.mkdir(parents=True, exist_ok=True)
    (out / "key").mkdir(exist_ok=True)
    blob = bytearray(struct.pack("<4sHH", b"RDMO", 1, COUNT))
    for i in range(COUNT):
        e = a9[TABLE - 0x02000000 + i * ENTRY:TABLE - 0x02000000 + (i + 1) * ENTRY]
        zone, act, chara, x, y, limit = struct.unpack("<BBHHHH", e[:10])
        path = e[10:].split(b"\0")[0].decode()
        src = EXT / "files" / path.lstrip("/")
        data = src.read_bytes()
        assert len(data) >= 8 and len(data) % 4 == 0, path
        # the record list must end with a zero-duration record (DemoInput_Main stops there)
        ends = [o for o in range(4, len(data), 4) if data[o + 2] == 0]
        assert ends, path
        shutil.copyfile(src, out / "key" / src.name)
        blob += e
        frames = sum(data[o + 2] for o in range(4, ends[0] + 4, 4))
        print("demo %d: zone %d act %d chara %d start (%d,%d) limit %d  %s: %d records, %d frames" %
              (i, zone, act, chara, x, y, limit, path, ends[0] // 4, frames))
    (out / "demotab.bin").write_bytes(bytes(blob))


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", default=str(ROOT / "build/dreamcast/demo"))
    build(Path(ap.parse_args().out))

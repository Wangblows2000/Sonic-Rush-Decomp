#!/usr/bin/env python3
"""Player tables for the native Dreamcast player module (bring-your-own-ROM).

Writes build/dreamcast/player/:
  physics.bin  gPlayerPhysicsTable (ov000 0x02368b7c), 2 entries x 0x30 bytes
               (Sonic, Blaze), copied from the owner's extracted overlay.
  trig.bin     8192 s16 sin/cos table (data_0206e470 layout: sin, cos
               interleaved, 4096 steps, 4.12 fixed) followed by 129 s16 atan
               table (data_02072470).  Both are COMPUTED here and then
               verified byte for byte against the owner's arm9.bin.
  tables.json  decoded physics values (for the docs / debugging).
  animtab.bin  the four Player_SetAnimation tables (ov000 0x02368bdc, 4 x 0x68
               bytes: joint animation, model resource 0 body / 1 ball,
               visibility animation, son.bac sprite animation), copied from
               the owner's overlay; read by dcp_model.c.

Usage: extract_physics.py [--extract extract/eur] [--out build/dreamcast/player]
"""
import argparse
import json
import math
import os
import struct
import sys

OV_BASE = 0x022C5C80
ARM9_BASE = 0x02000000
PHYS_ADDR = 0x02368B7C
SIN_ADDR = 0x0206E470
ATAN_ADDR = 0x02072470
N_CHARS = 2
FIELDS = ["acc", "top", "dec", "sd_base", "sd_add", "sd_max", "roll_dec", "hs_top",
          "boost_acc", "boost_top", "boost_dec", "hs_min", "slope_bonus", "p1a",
          "invuln_len", "fall_grace", "slope_acc", "gnd_max", "roll_slope", "jump",
          "gravity", "fall_max", "hs_time", "ride_max"]


def gen_sincos():
    out = []
    for i in range(4096):
        a = i * 2 * math.pi / 4096
        out += [round(math.sin(a) * 4096), round(math.cos(a) * 4096)]
    return out


def gen_atan():
    t = [round(math.atan(i / 128) * 65536 / (2 * math.pi)) for i in range(128)]
    return t + [0]  # entry 128 is never indexed (a/b < 1); the ROM has 0 there


ANIMTAB_ADDR = 0x02368BDC  # Player_SetAnimation tables (ov000)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--extract", default="extract/eur")
    ap.add_argument("--out", default="build/dreamcast/player")
    a = ap.parse_args()
    ov = open(os.path.join(a.extract, "arm9_overlays", "ov000.bin"), "rb").read()
    arm9 = open(os.path.join(a.extract, "arm9", "arm9.bin"), "rb").read()

    off = PHYS_ADDR - OV_BASE
    phys = ov[off:off + 0x30 * N_CHARS]
    if len(phys) != 0x30 * N_CHARS:
        sys.exit("ov000.bin too short")
    rows = []
    for c in range(N_CHARS):
        v = struct.unpack_from("<24h", phys, c * 0x30)
        rows.append(dict(zip(FIELDS, v)))
        # sanity: top speed and jump in a plausible range for this table
        if not (0x400 <= v[1] <= 0x1000 and 0x200 <= v[19] <= 0x800):
            sys.exit("physics entry %d does not look like gPlayerPhysicsTable" % c)

    sc = gen_sincos()
    at = gen_atan()
    rom_sc = struct.unpack_from("<8192h", arm9, SIN_ADDR - ARM9_BASE)
    rom_at = struct.unpack_from("<129h", arm9, ATAN_ADDR - ARM9_BASE)
    bad_sc = sum(1 for x, y in zip(sc, rom_sc) if x != y)
    bad_at = sum(1 for x, y in zip(at, rom_at) if x != y)
    if bad_sc or bad_at:
        sys.exit("generated trig tables differ from ROM: sincos %d, atan %d" % (bad_sc, bad_at))

    toff = ANIMTAB_ADDR - OV_BASE
    animtab = ov[toff:toff + 4 * 0x68]
    # sanity: the resource table holds only 0/1 and marks the ball ids 0x30..0x34
    if len(animtab) != 4 * 0x68 or any(b > 1 for b in animtab[0x68:0xd0]) or animtab[0x68 + 0x31] != 1:
        sys.exit("ov000.bin: Player_SetAnimation tables not found")

    os.makedirs(a.out, exist_ok=True)
    open(os.path.join(a.out, "animtab.bin"), "wb").write(animtab)
    open(os.path.join(a.out, "physics.bin"), "wb").write(phys)
    open(os.path.join(a.out, "trig.bin"), "wb").write(struct.pack("<8192h", *sc) + struct.pack("<129h", *at))
    json.dump({"chars": ["sonic", "blaze"], "physics": rows}, open(os.path.join(a.out, "tables.json"), "w"), indent=1)
    print("physics.bin %d bytes (%d chars), trig.bin %d bytes; sincos 8192/8192 and atan 129/129 match ROM"
          % (len(phys), N_CHARS, 8192 * 2 + 129 * 2))


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
"""Zone effect / camera tables from YOUR ROM's extracted binaries (extract/eur).

Writes build/dreamcast/zonefx/zonefx.bin ("RZFX" v1, little endian), read by
dreamcast/game/zonefx (zonefx_load_data) and dreamcast/game/camera (shake tables).
Nothing is copied into the source tree.

Layout (offsets fixed, see zonefx.c ZfxData):
  0x000 char magic[4] "RZFX", u16 version 1, u16 size
  0x008 s8 shake_3c[4]      arm9 data_0207323c  ScreenShake_Main modes 3/7/11
  0x00C s8 shake_40[12]     arm9 data_02073240  modes 1/5/9, 2/6/10
  0x018 s8 shake_4c[22]     arm9 data_0207324c  modes 4/8/12 (+2 pad)
  0x030 u8 z4_anim[24][8]   ov000 data_ov000_02364df0  Night Carnival palette animations
  0x0F0 u8 z5_anim[2][8]    ov000 data_ov000_02364dd0  Huge Crisis
  0x100 u8 z7_anim[2][8]    ov000 data_ov000_02364de0  Dead Line
  0x110 u8 z5_rot[8]        ov000 data_ov000_02364db4  Huge Crisis palette rotation
  0x118 u8 z7_rot[2][8]     ov000 data_ov000_02364dc4  Dead Line rotations
  0x128 u8 z5_add[4]        ov000 data_ov000_02364db0  Huge Crisis alarm ranges
  0x12C u8 z4_flash[60]     ov000 gZoneFxZone4BldyFlashTable (data_ov000_0236881c)
  0x168 u16 z3b_light0[3]   arm9 data_02074990  ZoneFx_Zone3Boss_Update light colours
  0x16E u16 z3b_light1[3]   ov000 data_ov000_02364dbc
  0x174 end
Palette animation entry (8 bytes): u8 buffer (0 = map tile palette, 1 = BG palette),
u8 event-archive file (the z4pnNN / z5pnNN / z7pnNN .bbg: alternate palette file - 3 in
bg.bin), u8 base, u8 frames, u8 dst, u8 count, u16 wait.
"""
import os
import struct
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../..'))
OV_BASE = 0x022C5C80
A9_BASE = 0x02000000


def main():
    out = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, 'build/dreamcast/zonefx')
    ov = open(os.path.join(ROOT, 'extract/eur/arm9_overlays/ov000.bin'), 'rb').read()
    a9 = open(os.path.join(ROOT, 'extract/eur/arm9/arm9.bin'), 'rb').read()

    def o(a, n):
        return ov[a - OV_BASE:a - OV_BASE + n]

    def m(a, n):
        return a9[a - A9_BASE:a - A9_BASE + n]

    body = bytearray()
    body += m(0x0207323c, 4) + m(0x02073240, 12) + m(0x0207324c, 22) + b'\0\0'
    body += o(0x02364df0, 24 * 8) + o(0x02364dd0, 16) + o(0x02364de0, 16)
    body += o(0x02364db4, 8) + o(0x02364dc4, 16) + o(0x02364db0, 4)
    body += o(0x0236881c, 60)
    body += m(0x02074990, 6) + o(0x02364dbc, 6)
    data = b'RZFX' + struct.pack('<HH', 1, 8 + len(body)) + bytes(body)
    assert len(data) == 0x174, hex(len(data))
    # sanity: the animation entries point at the alternate palettes (event archive files 4..)
    for e in range(24):
        buf, f, base, frames, dst, cnt = data[0x30 + e * 8:0x30 + e * 8 + 6]
        assert buf in (0, 1) and 4 <= f <= 0x13 and (frames - 1) * 16 + base + cnt <= 256, e
    os.makedirs(out, exist_ok=True)
    open(os.path.join(out, 'zonefx.bin'), 'wb').write(data)
    print('wrote %s (%d bytes)' % (os.path.join(out, 'zonefx.bin'), len(data)))


if __name__ == '__main__':
    main()

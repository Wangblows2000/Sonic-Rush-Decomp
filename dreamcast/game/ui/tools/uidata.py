#!/usr/bin/env python3
"""BYOR converter for the native UI module (Dreamcast port of Sonic Rush).

Reads the owner's extracted ROM (extract/eur: arm9/arm9.bin, base 0x02000000;
arm9_overlays/ov000.bin, base 0x022C5C80) and writes build/dreamcast/ui/uidata.bin,
a table of named sections the UI code reads at run time.  Every value comes from
the ROM at the address the decompiled code uses (symbol names below); nothing is
typed in by hand.

File "RUID" v1, little-endian:
  char magic[4] = "RUID"; u16 version = 1; u16 count;
  count x { char name[12] (NUL padded); u32 offset (from file start); u32 size }
  section data (each 4-byte aligned)

Usage: python3 dreamcast/game/ui/tools/uidata.py [--extract extract/eur] [--out build/dreamcast/ui]
"""
import argparse, os, struct, sys

ARM9_BASE = 0x02000000
OV0_BASE = 0x022C5C80


class Rom:
    def __init__(self, extract):
        self.arm9 = open(os.path.join(extract, "arm9", "arm9.bin"), "rb").read()
        self.ov0 = open(os.path.join(extract, "arm9_overlays", "ov000.bin"), "rb").read()

    def read(self, addr, n):
        if ARM9_BASE <= addr < ARM9_BASE + len(self.arm9):
            o = addr - ARM9_BASE
            if o + n <= len(self.arm9):
                return self.arm9[o:o + n]
        if OV0_BASE <= addr < OV0_BASE + len(self.ov0):
            o = addr - OV0_BASE
            if o + n <= len(self.ov0):
                return self.ov0[o:o + n]
        raise ValueError("address 0x%08x (+%d) not in arm9/ov000" % (addr, n))

    def u32s(self, addr, n):
        return list(struct.unpack("<%dI" % n, self.read(addr, 4 * n)))


def build(rom):
    S = []

    def raw(name, addr, n):
        S.append((name, rom.read(addr, n)))

    def deref_table(name, addr, count, each):
        """array of u32 pointers -> concatenated u32[each] arrays"""
        out = b""
        for p in rom.u32s(addr, count):
            out += rom.read(p, 4 * each)
        S.append((name, out))

    # HUD (ov000)
    raw("gauge_pal", 0x02364D60, 16)      # data_ov000_02364d60: u16[8] tension gauge colours (BGR555)
    raw("time_div", 0x02364D70, 28)       # data_ov000_02364d70: u32[7] Hud_FramesToTimeDigits divisors
    # ClearDemo score tally (arm9)
    deref_table("tb_thr", 0x020791D0, 27, 10)   # data_020791d0[zone][3] -> u32[10] time thresholds
    raw("tb_norm", 0x02078F90, 40)              # data_02078f90 u32[10] time bonus (normal acts)
    raw("tb_boss", 0x02079008, 40)              # data_02079008 u32[10] time bonus (boss acts)
    # data_02079198[zone][2]: the code indexes zone*2+act past the 14 entries for zone 7/8;
    # the table is followed by data_020791d0, so keep the whole pointer run (41 pointers).
    deref_table("sb_thr", 0x02079198, (0x0207923C - 0x02079198) // 4, 10)
    raw("sb_bonus", 0x02078EF0, 40)             # data_02078ef0 u32[10] trick (score) bonus
    raw("rank_boss", 0x02078B48, 16)            # data_02078b48 u32[4]
    raw("rank_norm", 0x02078B58, 16)            # data_02078b58 u32[4]
    raw("lbl_boss", 0x02078B30, 12)             # data_02078b30 u16[6] row label sprites (boss)
    raw("lbl_norm", 0x02078B3C, 12)             # data_02078b3c u16[6] row label sprites (normal)
    raw("cd_name_s", 0x020732E8, 18)            # data_020732e8 "SONIC THE HEDGEHOG" glyph string
    raw("cd_name_b", 0x020732B8, 13)            # data_020732b8 "BLAZE THE CAT"
    raw("cd_gw", 0x020733DC, 40)                # data_020733dc u16[20] glyph advance
    raw("cd_actx", 0x020732D8, 16)              # data_020732d8 s32[4] act text x
    raw("cd_act_ex", 0x020732C8, 14)            # data_020732c8
    raw("cd_act_bs", 0x02073294, 10)            # data_02073294
    raw("cd_act_1", 0x020732A0, 11)             # data_020732a0
    raw("cd_act_2", 0x020732AC, 11)             # data_020732ac
    raw("cd_model", 0x0207346C, 160)            # data_0207346c[2] (0x50 each): results model path / timing
    # Stage title card
    raw("tc_char_a", 0x02076B4C, 4)             # data_02076b4c u16[2]
    raw("tc_char_b", 0x02076B50, 4)             # data_02076b50 u16[2]
    raw("tc_text2", 0x02076B54, 6)              # data_02076b54 glyph string (Text404)
    raw("tc_text1", 0x02076B68, 12)             # data_02076b68 glyph string (Text3E8)
    raw("tc_lang", 0x02076B74, 12)              # data_02076b74 u16[6] act_com index of dm_stdm_<lang>
    raw("tc_gw", 0x02076B80, 16)                # data_02076b80 u8[16] glyph advance
    raw("tc_p180x", 0x02076B90, 36)             # data_02076b90 s32[9]
    raw("tc_p180y", 0x02076BB4, 72)             # data_02076bb4/bb8 {s32 a, b}[9]
    raw("zone_ord_s", 0x02074BC4, 9)            # data_02074bc4 story zone order (Sonic)
    raw("zone_ord_b", 0x02074BD0, 9)            # data_02074bd0 (Blaze)
    # Game over / time over
    raw("go_str", 0x0207A0CC, 120)              # data_0207a0cc s8[6][20]
    raw("go_w", 0x0207A09C, 24)                 # data_0207a09c s8[24]
    raw("to_str", 0x0207A144, 120)              # data_0207a144 s8[6][20]
    raw("to_w", 0x0207A0B4, 24)                 # data_0207a0b4 s8[24]
    raw("shake", 0x0207323C, 38)                # data_0207323c[4], data_02073240[12], data_0207324c[22] (ScreenShake_Main)
    # time-attack tally (ClearDemoTA_InitSprites / ClearDemoTA_Draw)
    S.append(("ta_anim", b"".join(rom.read(p, 30) for p in rom.u32s(0x02078B68, 6))))  # *data_02078b68[lang]: u16[15]
    raw("ta_dx", 0x02078B24, 10)                # data_02078b24: s16[5] time digit x offsets
    raw("ta_pal", 0x020732FC, 30)               # data_020732fc: u16[15] OBJ palette numbers
    # Pause
    raw("pause_lut", 0x0207BA24, 16)            # data_0207ba24 u8[16] dim LUT
    return S


def write(sections, path):
    n = len(sections)
    hdr = 8 + 20 * n
    off = (hdr + 3) & ~3
    dirs, blobs = [], b""
    for name, data in sections:
        assert len(name) < 12
        dirs.append(struct.pack("<12sII", name.encode(), off + len(blobs), len(data)))
        blobs += data
        blobs += b"\0" * ((-len(blobs)) & 3)
    out = b"RUID" + struct.pack("<HH", 1, n) + b"".join(dirs)
    out += b"\0" * (off - len(out)) + blobs
    os.makedirs(os.path.dirname(path), exist_ok=True)
    open(path, "wb").write(out)
    return len(out)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--extract", default="extract/eur")
    ap.add_argument("--out", default="build/dreamcast/ui")
    a = ap.parse_args()
    rom = Rom(a.extract)
    S = build(rom)
    size = write(S, os.path.join(a.out, "uidata.bin"))
    print("uidata.bin: %d sections, %d bytes" % (len(S), size))
    # sanity: decoded samples
    d = dict(S)
    print("  gauge_pal", [hex(x) for x in struct.unpack("<8H", d["gauge_pal"])])
    print("  rank_norm", struct.unpack("<4I", d["rank_norm"]), "tb_thr[0]", struct.unpack("<10I", d["tb_thr"][:40]))


if __name__ == "__main__":
    sys.exit(main())

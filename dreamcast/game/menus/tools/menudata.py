#!/usr/bin/env python3
"""Front-end data converter (BYOR: reads only the owner's extracted ROM).

Writes build/dreamcast/menus/:
  menudata.bin   "RMND" v1 section table (same layout as the UI module's RUID):
                 u32 magic, u16 version, u16 count, count x {char name[12],
                 u32 offset, u32 size}, then the sections. ROM tables read at
                 the addresses the decompiled menu code uses (see TABLES).
  bg/*.mnbg      menu backgrounds (menubg.py)
  ama/*.ama      KeyframeAnim banks, copied unchanged (#AMA, little-endian)
  preview/*.png  BG previews

Usage: python3 dreamcast/game/menus/tools/menudata.py [--out build/dreamcast/menus]
"""
import argparse
import shutil
import struct
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(HERE))
import menubg  # noqa: E402

EXT = ROOT / "extract/eur"
A9 = (EXT / "arm9/arm9.bin").read_bytes()
OV1 = (EXT / "arm9_overlays/ov001.bin").read_bytes()
OV0 = (EXT / "arm9_overlays/ov000.bin").read_bytes()
OV_BASE = 0x022C5C80


def mem(addr, n, ov=1):
    if 0x02000000 <= addr < 0x02000000 + len(A9):
        o = addr - 0x02000000
        return A9[o:o + n]
    src = OV1 if ov == 1 else OV0
    if OV_BASE <= addr < OV_BASE + len(src):
        o = addr - OV_BASE
        return src[o:o + n]
    raise ValueError("address %#x outside arm9/ov00%d" % (addr, ov))


def u32(addr):
    return struct.unpack("<I", mem(addr, 4))[0]


def cstr(addr):
    out = bytearray()
    while True:
        c = mem(addr, 1)[0]
        if c == 0:
            return out.decode("latin1")
        out.append(c)
        addr += 1


# name -> (address, size in bytes). Names are referenced by mn_data.c.
TABLES = {
    "save_sig": (0x02076b3c, 12),       # gSaveSignature
    "story_s": (0x02074bbc, 8),         # data_02074bbc: Sonic story step per zone
    "story_b": (0x02074bb4, 8),         # data_02074bb4: Blaze
    "zone_ord_s": (0x02074bc4, 9),      # data_02074bc4: StoryProgress zone order (Sonic)
    "zone_ord_b": (0x02074bd0, 9),      # data_02074bd0 (Blaze)
    "tm_slide": (0x0231616c, 7 * 8),    # data_ov001_0231616c..72: title menu BG slide colours
    "zm_adj": (0x0231649c, 2 * 17 * 4 * 4),   # data_ov001_0231649c[mode][node][dir]: zone map node graph
    "zm_rects": (0x02316358, 17 * 4),   # data_ov001_02316358: node hit rects (s8 x0,y0,x1,y1)
    "zm_icons": (0x023162f8, 8 * 4),    # data_ov001_023162f8: node of each zone name icon
    "as_rank1": (0x0231c3f4, 16),       # data_ov001_0231c3f4: act select rank thresholds (acts 1, 2)
    "as_rank2": (0x0231c404, 16),       # data_ov001_0231c404: (boss act)
    "st_widths": (0x02316028, 47 * 4),  # data_ov001_02316028: sound test glyph advance
}

# file tables resolved to names (path strings) -> runtime file ids.
FILE_TABLES = {
    "segalogo": (0x0231abe0, None, 2),        # gSegaLogoFileList
    "title": (0x0231b6f0, 6, 4),              # gTitleScreenFileTable[lang]
    "titlemenu": (0x0231bd18, 6, 9),          # gTitleMenuFileTable[lang]
}

BG_GLOBS = ["bg/sega_logo_*.bbg", "bg/title_logo_*.bbg", "bg/mnu_*.bbg", "bg2/zmap/*.bbg",
            "bg2/caution/*.bbg", "bg/title_demo_image_*.bbg"]
AMA = ["mnu_button.ama", "mnu_wipe.ama", "mnu_opti.ama", "ps_act.ama", "zmap_map_icon.ama",
       "zone_sel.ama"]


def file_tables():
    out = {}
    for key, (addr, nlang, n) in FILE_TABLES.items():
        if nlang is None:
            out[key] = [[cstr(u32(addr + 4 * i)) for i in range(n)]]
        else:
            out[key] = [[cstr(u32(u32(addr + 4 * l) + 4 * i)) for i in range(n)] for l in range(nlang)]
    return out


def vmu_icon():
    """DS banner icon (32x32, 16 colours) -> VMU icon: 16 x ARGB4444 + 32x32 4 bpp."""
    from PIL import Image
    bm = Image.open(EXT / "banner/bitmap.png").convert("RGBA")
    pal_im = Image.open(EXT / "banner/palette.png").convert("RGBA")
    pal = [pal_im.getpixel((i, 0)) for i in range(16)]
    vpal = []
    for i, (r, g, b, a) in enumerate(pal):
        al = 0 if i == 0 else 15
        vpal.append(al << 12 | (r >> 4) << 8 | (g >> 4) << 4 | (b >> 4))
    idx = []
    for y in range(32):
        for x in range(32):
            p = bm.getpixel((x, y))
            if p[3] == 0:
                idx.append(0)
                continue
            best = min(range(1, 16), key=lambda i: sum((p[k] - pal[i][k]) ** 2 for k in range(3)))
            idx.append(best)
    data = bytes((idx[i] << 4) | idx[i + 1] for i in range(0, 1024, 2))
    return struct.pack("<16H", *vpal) + data


def build(out):
    out.mkdir(parents=True, exist_ok=True)
    (out / "bg").mkdir(exist_ok=True)
    (out / "ama").mkdir(exist_ok=True)
    (out / "preview").mkdir(exist_ok=True)
    sections = []
    for name, (addr, size) in TABLES.items():
        sections.append((name, mem(addr, size)))
    sections.append(("vmu_icon", vmu_icon()))
    blob = bytearray(struct.pack("<4sHH", b"RMND", 1, len(sections)))
    off = 8 + 20 * len(sections)
    off = (off + 3) & ~3
    body = bytearray()
    table = bytearray()
    for name, data in sections:
        while (off + len(body)) % 4:
            body.append(0)
        table += struct.pack("<12sII", name.encode(), off + len(body), len(data))
        body += data
    blob += table
    while len(blob) < off:
        blob.append(0)
    blob += body
    (out / "menudata.bin").write_bytes(bytes(blob))
    print("menudata.bin: %d sections, %d bytes" % (len(sections), len(blob)))
    nbg = 0
    for g in BG_GLOBS:
        for src in sorted(EXT.glob("files/" + g)):
            menubg.convert(src, out / "bg", out / "preview")
            nbg += 1
    print("backgrounds: %d" % nbg)
    for a in AMA:
        shutil.copyfile(EXT / "files/ama" / a, out / "ama" / a)
    # the sound test track list (gOptionsFileTable entry 15), copied unchanged
    shutil.copyfile(EXT / "files/st/sound_test.sld", out / "sound_test.sld")
    print("ama: %d" % len(AMA))
    ft = file_tables()
    with open(out / "filetables.txt", "w") as f:
        for k, v in ft.items():
            for lang, names in enumerate(v):
                f.write("%s[%d] %s\n" % (k, lang, " ".join(names)))
    print("file tables: %s" % ", ".join(ft))


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", default=str(ROOT / "build/dreamcast/menus"))
    build(Path(ap.parse_args().out))

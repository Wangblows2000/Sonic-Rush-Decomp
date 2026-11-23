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
            "bg2/caution/*.bbg", "bg/title_demo_image_*.bbg", "bg/title_demo_rush_spin_*.bbg"]
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


def font8(out):
    """Records name font: bg/font_ipl_8x8.bbg character tiles + Font_GetGlyphIndex's tables.
    font8.bin "RFNT" v1: u32 magic, u16 version, u16 ranges (0x39), u16 tiles, u16 pad,
    u16 start[ranges] (gFontGlyphRangeStartTable), s32 base[ranges] (gFontGlyphRangeBaseTable),
    s32 len[ranges] (gFontGlyphRangeLengthTable), then tiles x 64 bytes (8x8, one 4-bit
    value per byte, row-major)."""
    sys.path.insert(0, str(ROOT / "tools/dreamcast/sprites"))
    from nds_archive import maybe_decompress
    d = (EXT / "files/bg/font_ipl_8x8.bbg").read_bytes()
    assert d[:4] == b"BBG\0"
    chr_off = struct.unpack_from("<I", d, 8)[0]
    tiles4 = maybe_decompress(d[chr_off:])
    n = len(tiles4) // 32
    nr = 0x39
    blob = bytearray(struct.pack("<4sHHHH", b"RFNT", 1, nr, n, 0))
    blob += mem(0x02072e70, 2 * nr)
    blob += mem(0x02072ee4, 4 * nr)
    blob += mem(0x02072fc8, 4 * nr)
    for t in range(n):
        for b in tiles4[t * 32:(t + 1) * 32]:
            blob += bytes((b & 15, b >> 4))
    (out / "font8.bin").write_bytes(bytes(blob))
    print("font8.bin: %d glyph tiles, %d ranges" % (n, nr))


def extraact(out):
    """ExtraAct (Zone/Extra_Zone/Results) tables + the palette of ex_cldm_bg_pal.bbg
    (member 5 of /narc/extra_act_lz7.narc). extraact.bin "REXA" v1, little-endian:
      u32 magic, u16 version, u16 size of the rest
      u16 spr[39]   data_ov001_02316afc (ac_ex_cldm anim of each sprite), u16 pad
      u16 gw[16]    data_ov001_02316abc (glyph advance)
      u16 txt_b[16] data_ov001_02316adc (bottom caption glyphs)
      u16 txt_t[14] data_ov001_02316aa0 (title caption glyphs, 13 + end)
      s32 rows[2]   data_ov001_02316a80 (TIME BONUS / RING BONUS label sprites)
      s32 bonus[10] gExtraActTimeBonusTable, u32 thr[10] gExtraActTimeBonusThresholds
      s32 cam_pos[3] data_ov001_0231d544, s32 cam_up[3] data_ov001_0231d538
      u16 bgpal[256] the BBG palette (BG1 letterbox colours 2 and 0x12)
      u32 rank_boss[4] data_02078b48, u32 rank_norm[4] data_02078b58 (ClearDemo_GetRank)"""
    sys.path.insert(0, str(ROOT / "tools/dreamcast/sprites"))
    from nds_archive import open_narc
    narc = open_narc(str(EXT / "files/narc/extra_act_lz7.narc"))
    name, bbg = narc.files[5]
    assert name.endswith("ex_cldm_bg_pal.bbg") and bbg[:4] == b"BBG\0", name
    pal_off = struct.unpack_from("<I", bbg, 0x10)[0]
    from nds_archive import maybe_decompress
    raw = bbg[pal_off:]
    h = struct.unpack_from("<I", raw, 0)[0]
    if h & 0xff == 0 and (h >> 8) + 4 <= len(raw):   # stored (Compress_GetData skips the header)
        pal = raw[4:4 + (h >> 8)][:512]
    else:
        pal = maybe_decompress(raw)[:512]
    pal = pal + bytes(512 - len(pal))
    body = bytearray()
    body += mem(0x02316afc, 78) + b"\0\0"
    body += mem(0x02316abc, 32) + mem(0x02316adc, 32) + mem(0x02316aa0, 28)
    body += mem(0x02316a80, 8) + mem(0x0231d550, 40) + mem(0x0231d578, 40)
    body += mem(0x0231d544, 12) + mem(0x0231d538, 12) + pal
    body += mem(0x02078b48, 16) + mem(0x02078b58, 16)   # ClearDemo_GetRank: boss / normal thresholds
    (out / "extraact.bin").write_bytes(struct.pack("<4sHH", b"REXA", 1, len(body)) + bytes(body))
    print("extraact.bin: %d bytes (tables + %s palette)" % (8 + len(body), name))


def rle(px):
    """NVFM v2 frames: c < 0x80: c+1 literal bytes; c >= 0x80: the next byte c-0x80+3 times."""
    out, i, n, lit = bytearray(), 0, len(px), bytearray()

    def flush():
        while lit:
            k = min(len(lit), 128)
            out.append(k - 1)
            out.extend(lit[:k])
            del lit[:k]
    while i < n:
        j = i
        while j < n and px[j] == px[i] and j - i < 130:
            j += 1
        if j - i >= 3:
            flush()
            out.append(0x80 + (j - i - 3))
            out.append(px[i])
            i = j
        else:
            lit.append(px[i])
            i += 1
    flush()
    return bytes(out)


def navi(out):
    """Zone-map navigator (NaviBg_Load / NaviMsg_Create mode 0, NaviMsgFace): sub-screen BGs.
    bg/<c>_m_b.mnbg      the navigator frame (sub BG2, chr 0x06211600 = tile 176)
    bg/m_bal.mnbg        the speech balloon (sub BG0, tile 376; <c>_m_b colours: m_bal_<c>.mnbg)
    bg/navi_<c>_<lang>_NN.mnbg  message panels data_022c4664[NN] (sub BG0 at tile (2,2), tile 400)
    navi/faces_<c>.bin   "NVFM" v2: u16 2, n, w, h, x0, y0, npal, 0 (crop in the 128x128 face frame),
                         14 x {u16 count, u16 flags, u8 [48][2] (duration, frame)} (data_0207b8d4),
                         npal x u16 pal[256] (the distinct face palettes),
                         n x {u16 pal, u16 0, u32 offset, u32 size} (size 0: frame not used by the
                         zone-map sequences 0..8; the boss sequences are the UI module's),
                         then the frames, RLE of the w*h indices: c < 0x80: c+1 literal bytes;
                         c >= 0x80: the next byte c-0x80+3 times."""
    sys.path.insert(0, str(ROOT / "dreamcast/game/ui/tools"))
    import uibg  # noqa: E402
    nd = EXT / "files/bg2/navi"
    (out / "navi").mkdir(exist_ok=True)
    langs = ["jpn", "eng", "fra", "deu", "ita", "spa"]
    total = 0

    def put(path, W, H, b, px, pal):
        nonlocal total
        data = (struct.pack("<4sHHHHHH", b"MNBG", W, H, b["type"], b["pofs"], b["pslot"], 0) +
                struct.pack("<256H", *pal) + bytes(px))
        path.write_bytes(data)
        total += len(data)

    for idx, c in enumerate("tc"):
        bb, W, H, px, bpal = uibg.bbg_load((nd / ("%s_m_b.bbg" % c)).read_bytes(), 176)
        sub = uibg.slot(bpal, bb["pofs"])
        put(out / "bg" / ("%s_m_b.mnbg" % c), W, H, bb, px, sub)
        lb, lW, lH, lpx, lpal = uibg.bbg_load((nd / "m_bal.bbg").read_bytes(), 376)
        lp = uibg.slot(lpal, lb["pofs"]) if lpal else sub
        put(out / "bg" / ("m_bal_%s.mnbg" % c), lW, lH, lb, lpx, lp)
        nmsg = u32(0x0207565C + 4 * idx)
        names = [cstr(u32(u32(0x0207B8B4 + 4 * idx) + 4 * i)) for i in range(nmsg)]
        for lang in langs:
            for i, n in enumerate(names):
                mb, mW, mH, mpx, mpal = uibg.bbg_load((nd / ("msg_" + c) / ("%s%s.bbg" % (lang, n))).read_bytes(), 400)
                put(out / "bg" / ("navi_%s_%s_%02d.mnbg" % (c, lang, i)), mW, mH, mb, mpx, uibg.slot(mpal, mb["pofs"]))
        # faces
        nf = u32(0x02075674 + 4 * idx)
        fnames = [cstr(u32(u32(0x0207B8CC + 4 * idx) + 4 * i)) for i in range(nf)]
        frames = []
        x0 = y0 = 1 << 30
        x1 = y1 = 0
        for n in fnames:
            fb, fW, fH, fpx, fpal = uibg.bbg_load((nd / ("act_" + c) / (n + ".bbg")).read_bytes(), 0)
            frames.append((fb, fW, fH, fpx, uibg.slot(fpal, fb["pofs"])))
            bx0, by0, bx1, by1 = uibg.bbox(fW, fH, fpx)
            if bx1 > bx0:
                x0, y0, x1, y1 = min(x0, bx0), min(y0, by0), max(x1, bx1), max(y1, by1)
        x0 &= ~7
        y0 &= ~7
        x1 = (x1 + 7) & ~7
        y1 = (y1 + 7) & ~7
        tbl = u32(0x0207B8D4 + 4 * idx)
        seqb, used = b"", set()
        for a in range(14):
            q = u32(tbl + 4 * a)
            cnt, fl, fp = struct.unpack("<HHI", mem(q, 8))
            fr = mem(fp, 2 * cnt)
            assert cnt <= 48
            seqb += struct.pack("<HH", cnt, fl) + fr + bytes(96 - 2 * cnt)
            if a <= 8:                       # NaviMsgFace idle/blink and the map sequences
                used.update(fr[1::2])
        pals, index, data = [], b"", b""
        for k, (fb, fW, fH, fpx, fpal) in enumerate(frames):
            if k not in used:
                index += struct.pack("<HHII", 0, 0, 0, 0)
                continue
            pb = struct.pack("<256H", *fpal)
            if pb not in pals:
                pals.append(pb)
            px = bytes(fpx[y * fW + x] if x < fW and y < fH else 0 for y in range(y0, y1) for x in range(x0, x1))
            r = rle(px)
            index += struct.pack("<HHII", pals.index(pb), 0, len(data), len(r))
            data += r
        blob = struct.pack("<4sHHHHHHHH", b"NVFM", 2, nf, x1 - x0, y1 - y0, x0, y0, len(pals), 0)
        blob += seqb + b"".join(pals) + index + data
        (out / "navi" / ("faces_%s.bin" % c)).write_bytes(blob)
        total += len(blob)
        print("navi %s: frame %dx%d, balloon %dx%d, %d messages x %d languages, %d faces (%d used, %d palettes, "
              "crop (%d,%d)-(%d,%d)), faces_%s.bin %d KB" % (
                  c, W, H, lW, lH, nmsg, len(langs), nf, len(used), len(pals), x0, y0, x1, y1, c, len(blob) // 1024))
    print("navi: %d KB" % (total // 1024))


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
    font8(out)
    extraact(out)
    navi(out)
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

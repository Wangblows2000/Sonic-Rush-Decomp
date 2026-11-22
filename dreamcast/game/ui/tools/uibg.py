#!/usr/bin/env python3
"""BYOR converter for the UI backgrounds and the navigator (Dreamcast port of Sonic Rush).

Reads the owner's extracted ROM (extract/eur: files/narc/act_com_lz7.narc, files/bg2/navi/,
arm9/arm9.bin) and writes, under build/dreamcast/ui/res/:

  tcbg_<n>/      title card backgrounds (StageTitleCard_InitBg / _InitBgBoss): act_com
                 file n = stdm_bg<zone> (6..14) or stdm_bs_bg<zone> (16..24) and the
                 zigzag plane of that kind (file 5 stdm_zigzag / 15 stdm_bs_zigzag)
  cldm/          act results backgrounds (ClearDemo_InitBg): cldm_far (0x1b), cldm_bg (0x1a)
  navi_<c>_<lang>/  the boss navigator panel (NaviBg_Load mode 1, NaviMsg_Create mode 1,
                 NaviMsg_LoadPanel): <c>_b_b.bbg box, b_bal.bbg balloon, the boss
                 message panels msg_<c>/<lang>15..19.bbg and the face frames act_<c>/*.bbg
                 used by the boss face sequences
  ../navidata.bin  "RUID" tables: face sequences (data_0207b8d4), voice ids
                 (data_0207566c, data_0207567c..02075690), NaviB icon table / positions
                 (data_02073264 / data_02073274), EnemyRoomHud shake (data_02077944),
                 EffEnemyRoomFix offsets (data_ov000_02367b0c), BossHelp palettes
                 (data_ov000_02364d8c)

Every resource directory uses the sprite format the UI already loads (frames.bin "DSPR"
v1 + atlas<i>.argb1555, SPRITES.md section 4): one animation of one frame per image.
Images are expanded to ARGB1555 with the palette the original code builds (palette
transfers included), so the runtime does no palette work.  Pages are power-of-two.

The BBG decoding reuses the menu converter (dreamcast/game/menus/tools/menubg.py,
MENUS.md section 4.1).  The tile base of each BG (the char VRAM address the original
Background_Init/InitEx uses) is passed explicitly: screen entries index VRAM tiles.

Usage: python3 dreamcast/game/ui/tools/uibg.py [--extract extract/eur] [--out build/dreamcast/ui]
"""
import argparse
import os
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT / "dreamcast/game/menus/tools"))
sys.path.insert(0, str(ROOT / "tools/dreamcast/sprites"))
import menubg  # noqa: E402
from nds_archive import open_narc  # noqa: E402

ARM9_BASE, OV0_BASE = 0x02000000, 0x022C5C80
LANGS = ["jpn", "eng", "fra", "deu", "ita", "spa"]   # data_0207b9bc (gGameLanguage order)


# ------------------------------------------------------------------ ROM
class Rom:
    def __init__(self, extract):
        self.arm9 = open(os.path.join(extract, "arm9", "arm9.bin"), "rb").read()
        self.ov0 = open(os.path.join(extract, "arm9_overlays", "ov000.bin"), "rb").read()

    def read(self, a, n):
        if ARM9_BASE <= a and a + n <= ARM9_BASE + len(self.arm9):
            return self.arm9[a - ARM9_BASE:a - ARM9_BASE + n]
        if OV0_BASE <= a and a + n <= OV0_BASE + len(self.ov0):
            return self.ov0[a - OV0_BASE:a - OV0_BASE + n]
        raise ValueError("0x%08x" % a)

    def u32(self, a):
        return struct.unpack("<I", self.read(a, 4))[0]

    def s32(self, a):
        return struct.unpack("<i", self.read(a, 4))[0]

    def cstr(self, a):
        b = self.read(a, 64)
        return b[:b.index(0)].decode("latin1")


# ------------------------------------------------------------------ BBG
def bbg_load(data, tile_base):
    """menubg.parse with a tile base and the 'raw, size 0' (no palette) case."""
    _, ochr, oscr, opal, flags, w, h, pofs, pslot = struct.unpack_from("<IIIIIHHHH", data, 4)

    def dz(o):
        if not o or (data[o] == 0 and struct.unpack_from("<I", data, o)[0] >> 8 == 0):
            return b""
        return menubg.decompress(data[o:])
    scr = dz(oscr)
    ents = struct.unpack("<%dH" % (len(scr) // 2), scr)
    # VRAM tile index -> index in this file's char data; out-of-range entries stay out of range
    ents = [(e & ~0x3FF) | (((e & 0x3FF) - tile_base) & 0x3FF) for e in ents]
    b = dict(type=flags & 0xF, flags=flags, w=w, h=h, pofs=pofs, pslot=pslot, chars=dz(ochr),
             screen=struct.pack("<%dH" % len(ents), *ents), pal=dz(opal))
    W, H, px = menubg.render(b)
    pal = [struct.unpack_from("<H", b["pal"], 2 * i)[0] for i in range(len(b["pal"]) // 2)]
    return b, W, H, px, pal


def argb(c):
    return 0x8000 | (c & 0x1F) << 10 | (c & 0x3E0) | (c >> 10 & 0x1F)


def expand(W, H, px, pal256, rect=None):
    """palette indices -> ARGB1555 rows (index 0 transparent), cropped to rect (x0,y0,x1,y1)."""
    x0, y0, x1, y1 = rect or (0, 0, W, H)
    out = []
    for y in range(y0, y1):
        row = []
        for x in range(x0, x1):
            p = px[y * W + x] if 0 <= x < W and 0 <= y < H else 0
            row.append(argb(pal256[p]) if p else 0)
        out.append(row)
    return out


def bbox(W, H, px, rect=None):
    x0, y0, x1, y1 = rect or (0, 0, W, H)
    xs, ys = [], []
    for y in range(y0, y1):
        for x in range(x0, x1):
            if px[y * W + x]:
                xs.append(x)
                ys.append(y)
    return (min(xs), min(ys), max(xs) + 1, max(ys) + 1) if xs else (x0, y0, x0, y0)


def slot(pal_list, pofs, extra=None):
    p = [0] * 256
    for i, c in enumerate(pal_list):
        if pofs + i < 256:
            p[pofs + i] = c
    for k, v in (extra or {}).items():
        p[k] = v
    return p


# ------------------------------------------------------------------ DSPR writer
def pack(images, max_page=512):
    """images: list of (rows, ox, oy) -> pages, placements. Shelf packing, pow2 pages."""
    order = sorted(range(len(images)), key=lambda i: -len(images[i][0]))
    pages, place = [], [None] * len(images)

    pw = 8
    while pw < max(len(r[0]) if r else 1 for r, _, _ in images):
        pw *= 2
    pw = max(pw, min(max_page, 256))

    def new_page():
        pages.append(dict(w=pw, h=max_page, x=0, y=0, rowh=0, used_w=0, used_h=0))
    new_page()
    for i in order:
        rows = images[i][0]
        h, w = len(rows), (len(rows[0]) if rows else 0)
        w, h = max(w, 1), max(h, 1)
        assert w <= max_page and h <= max_page, (w, h)
        pg = pages[-1]
        if pg["x"] + w > pg["w"]:
            pg["x"], pg["y"], pg["rowh"] = 0, pg["y"] + pg["rowh"] + 1, 0
        if pg["y"] + h > pg["h"]:
            new_page()
            pg = pages[-1]
        place[i] = (len(pages) - 1, pg["x"], pg["y"])
        pg["used_w"] = max(pg["used_w"], pg["x"] + w)
        pg["x"] += w + 1
        pg["rowh"] = max(pg["rowh"], h)
        pg["used_h"] = max(pg["used_h"], pg["y"] + h)
    assert len(pages) <= 4, "more than 4 atlas pages"
    for pg in pages:   # shrink to the smallest power of two that holds the used area
        for k, u in (("w", "used_w"), ("h", "used_h")):
            v = 8
            while v < pg[u]:
                v *= 2
            pg[k] = v
    return pages, place


def write_res(out_dir, images, preview=True):
    """images: list of (rows ARGB1555, ox, oy).  frames.bin + atlas pages."""
    pages, place = pack(images)
    os.makedirs(out_dir, exist_ok=True)
    n = len(images)
    hdr = struct.pack("<4sHHHHHH", b"DSPR", 1, len(pages), n, n, n, 0)
    body = b"".join(struct.pack("<HH", p["w"], p["h"]) for p in pages)
    for (rows, ox, oy), (pg, x, y) in zip(images, place):
        h, w = len(rows), (len(rows[0]) if rows else 0)
        body += struct.pack("<BBHHHHhhH", pg, 0, x, y, w, h, ox, oy, 0)
    for i in range(n):
        body += struct.pack("<HHhhBBH", i, 1, 0, -1, 0, 0, 0)
    for i in range(n):
        body += struct.pack("<HH", i, 1)
    open(os.path.join(out_dir, "frames.bin"), "wb").write(hdr + body)
    total = 0
    for k, p in enumerate(pages):
        buf = [0] * (p["w"] * p["h"])
        for (rows, _, _), (pg, x, y) in zip(images, place):
            if pg != k:
                continue
            for yy, row in enumerate(rows):
                buf[(y + yy) * p["w"] + x:(y + yy) * p["w"] + x + len(row)] = row
        open(os.path.join(out_dir, "atlas%d.argb1555" % k), "wb").write(struct.pack("<%dH" % len(buf), *buf))
        total += len(buf) * 2
        if preview:
            try:
                from PIL import Image
                im = Image.new("RGBA", (p["w"], p["h"]))
                im.putdata([(((v >> 10) & 31) * 255 // 31, ((v >> 5) & 31) * 255 // 31, (v & 31) * 255 // 31,
                             255 if v & 0x8000 else 0) for v in buf])
                im.save(os.path.join(out_dir, "atlas%d.png" % k))
            except ImportError:
                pass
    return pages, total


def write_ruid(sections, path):
    n = len(sections)
    off = (8 + 20 * n + 3) & ~3
    dirs, blobs = [], b""
    for name, data in sections:
        assert len(name) < 12
        dirs.append(struct.pack("<12sII", name.encode(), off + len(blobs), len(data)))
        blobs += data + b"\0" * ((-len(data)) & 3)
    out = b"RUID" + struct.pack("<HH", 1, n) + b"".join(dirs)
    out += b"\0" * (off - len(out)) + blobs
    open(path, "wb").write(out)
    return len(out)


# ------------------------------------------------------------------ periodic crops
def period(get, length, maxp):
    for p in range(1, maxp + 1):
        if all(get(i) == get(i + p) for i in range(length - p)):
            return p
    return None


def tile_piece(W, H, px, x0, x1):
    """columns x0..x1 of a plane, cropped to one vertical period (asserted)."""
    rows = [tuple(px[y * W + x0:y * W + x1]) for y in range(H)]
    p = period(lambda i: rows[i], H, 64)
    assert p, "no vertical period"
    return p


# ------------------------------------------------------------------ title card / results
def title_bgs(narc, out):
    """StageTitleCard_InitBg(Boss): bgN on sub BG3 (char base 0x06206b80 -> tile 430 for
    the 8 bpp normal BGs; boss: Background_Init default + 0x1c0 -> tile 14), zigzag on BG2
    (0x06006800 -> tile 416; boss: default -> tile 0).  The palette transfers copy 16
    colours from bgN's palette at data_02076b48[character] (0 / 0x20 bytes) to colour 0 of
    the bgN and zigzag palette banks, so the zigzag colours depend on the character."""
    files = {n: d for n, d in narc}
    report = []
    for boss in (0, 1):
        zz_name = "stdm_bs_zigzag.bbg" if boss else "stdm_zigzag.bbg"
        zb, zW, zH, zpx, _ = bbg_load(files[zz_name], 0 if boss else 416)
        # the zigzag plane: an edge band at the left, then a fill that repeats
        cols = lambda x: tuple(zpx[y * zW + x] for y in range(zH))   # noqa: E731
        edge_end = 64
        vp = tile_piece(zW, zH, zpx, 0, zW)
        fill_p = period(lambda i: cols(edge_end + i), zW - edge_end, 64) if not boss else None
        # the runtime stretches one fill column over the plane (ui_bg.c): it must be uniform
        assert boss or fill_p == 1, "zigzag fill columns differ (period %r)" % fill_p
        for zone in range(9):
            idx = (0x10 if boss else 6) + zone
            name = narc_names[idx]
            b, W, H, px, pal = bbg_load(files[name], 14 if boss else 430)
            imgs = []
            for chara in (0, 1):
                c0 = 16 * chara   # data_02076b48[chara] / 2
                p16 = {i: pal[c0 + i] for i in range(16) if c0 + i < len(pal)}
                bank = 0 if boss else 0
                ps = slot(pal, b["pofs"], {bank * 16 + k: v for k, v in p16.items()})
                if chara == 0:
                    if boss:   # 256x256 plane: keep the rows with pixels
                        x0, y0, x1, y1 = bbox(W, H, px)
                        rect = (0, 0, 256, y1)
                    else:
                        rect = (0, 0, 256, 192)
                    imgs.append((expand(W, H, px, ps, rect), 0, 0))
                zps = slot([], 0, p16)
                if boss:
                    zrect = (0, 0, 64, vp)
                else:
                    zrect = (0, 0, edge_end + fill_p * max(1, 8 // fill_p), vp)
                imgs.append((expand(zW, zH, zpx, zps, zrect), 0, 0))
            d = os.path.join(out, "tcbg_%d" % idx)
            pages, total = write_res(d, imgs)
            report.append("tcbg_%d (%s): bg %dx%d, zigzag piece %dx%d (period %d, fill %s), %d KB" % (
                idx, name, len(imgs[0][0][0]), len(imgs[0][0]), len(imgs[1][0][0]), len(imgs[1][0]), vp,
                fill_p, total // 1024))
    return report, dict(edge=64, fill=fill_p)


def results_bgs(narc, out):
    """ClearDemo_InitBg: cldm_far on main BG1 (type 2, tile base 0), cldm_bg on BG2/BG3
    (0x06006800 -> tile 416).  16 colours from each file's own palette at pal*32 bytes
    (pal = gGameState.unk0) go to colour 0 of its palette bank."""
    files = {n: d for n, d in narc}
    fb, fW, fH, fpx, fpal = bbg_load(files["cldm_far.bbg"], 0)
    bb, bW, bH, bpx, bpal = bbg_load(files["cldm_bg.bbg"], 416)
    vp = tile_piece(bW, bH, bpx, 0, bW)
    hp = period(lambda i: tuple(bpx[y * bW + i] for y in range(bH)), bW, 64)
    # the runtime draws cldm_bg as one stretched texel (ui_bg.c): it must be one colour
    assert hp == 1 and vp == 1 and len(set(bpx)) == 1 and bpx[0] != 0, (hp, vp, set(bpx))
    rep = []
    for chara in (0, 1):   # one resource per character: only one is loaded
        imgs = [(expand(fW, fH, fpx, slot(fpal, fb["pofs"], {i: fpal[16 * chara + i] for i in range(16)}),
                        (0, 0, 256, 192)), 0, 0)]
        ps = slot(bpal, bb["pofs"], {i: bpal[16 * chara + i] for i in range(16)})
        n = max(8, hp, vp)
        imgs.append((expand(bW, bH, bpx, ps, (0, 0, n, n)), 0, 0))
        pages, total = write_res(os.path.join(out, "cldm_%d" % chara), imgs)
        rep.append("cldm_%d: far %dx%d, bg pattern %dx%d (period %dx%d), %d KB" % (
            chara, fW, fH, n, n, hp, vp, total // 1024))
    return rep


# ------------------------------------------------------------------ navigator
# NaviB box and the face crop (DS sub-screen px).  The box is the BOSS frame of
# <c>_b_b.bbg (dark inner panel: digit slot, BOSS, boss icon, health gauge); the
# NaviB sprites are drawn over it at their original sub-screen positions.
PANEL_RECT = (16, 79, 121, 135)   # the inner BOSS box of <c>_b_b.bbg (DS sub-screen px)
FACE_RECT = (12, 0, 116, 80)     # head and gestures: this part of the 128x128 face frame (DS px)
FACE_POS = (128, 48)              # NaviMsg_Create bg1: the face frame at sub-screen (16, 6) tiles
BOSS_SEQS = (0, 1, 9, 10, 11, 12, 13)   # NaviMsgFace_StartIdle 0, Blink 1, boss SetFaceAnim 9/0xb/0xc/0xd, create 10
BOSS_MSGS = (15, 16, 17, 18, 19)        # NaviMsg_ShowMessage 0xf..0x13 (and the create panel 0xf)


def navi(rom, navi_dir, out):
    report, sections = [], []
    for idx, c in enumerate("tc"):
        # data_0207b8e4[idx][1] boss bg, data_0207b8bc[1] balloon
        bb, bW, bH, bpx, bpal = bbg_load(open(os.path.join(navi_dir, "%s_b_b.bbg" % c), "rb").read(), 176)
        lb, lW, lH, lpx, _ = bbg_load(open(os.path.join(navi_dir, "b_bal.bbg"), "rb").read(), 376)
        sub = slot(bpal, bb["pofs"])     # sub BG palette: <c>_b_b colours 224.. (b_bal has none)
        # inner box: the dark panel that holds the NaviB sprites (digit slot at 21,87,
        # gauge at 43..114, 104..128): grow from the gauge point while the colour is the box fill
        panel = PANEL_RECT
        bal = bbox(lW, lH, lpx, (0, 0, 256, 64))
        # face frames: union crop over the boss sequences
        tbl = rom.u32(0x0207B8D4 + 4 * idx)
        n_faces = rom.u32(0x02075674 + 4 * idx)
        names = [rom.cstr(rom.u32(rom.u32(0x0207B8CC + 4 * idx) + 4 * i)) for i in range(n_faces)]
        seqs, used = [], []
        for a in range(14):
            p = rom.u32(tbl + 4 * a)
            cnt, fl, fp = struct.unpack("<HHI", rom.read(p, 8))
            fr = list(rom.read(fp, 2 * cnt))
            pairs = list(zip(fr[0::2], fr[1::2]))
            seqs.append((cnt, fl, pairs))
            if a in BOSS_SEQS:
                for _, f in pairs:
                    if f not in used:
                        used.append(f)
        faces = []
        for f in used:
            fb, fW, fH, fpx, fpal = bbg_load(open(os.path.join(navi_dir, "act_%s" % c, names[f] + ".bbg"), "rb").read(), 0)
            faces.append((f, fW, fH, fpx, slot(fpal, fb["pofs"])))
        face_rect = FACE_RECT
        for lang in LANGS:
            imgs = [(expand(bW, bH, bpx, sub, panel), 0, 0), (expand(lW, lH, lpx, sub, bal), 0, 0)]
            for m in BOSS_MSGS:
                mb, mW, mH, mpx, mpal = bbg_load(open(os.path.join(navi_dir, "msg_%s" % c, "%s%02d.bbg" % (lang, m)), "rb").read(), 400)
                imgs.append((expand(mW, mH, mpx, slot(mpal, mb["pofs"])), 0, 0))
            # image 7: the <c>_b_b background behind the face crop (sub BG2 under BG1)
            fx0, fy0, fx1, fy1 = FACE_RECT
            imgs.append((expand(bW, bH, bpx, sub, (FACE_POS[0] + fx0, FACE_POS[1] + fy0, FACE_POS[0] + fx1,
                                                  FACE_POS[1] + fy1)), 0, 0))
            pages, total = write_res(os.path.join(out, "navi_%s_%s" % (c, lang)), imgs, preview=(lang == "eng"))
        # face frames (NaviMsg_ReadFaceFrameMap/Chr stream them per frame on the DS):
        # "NVFC" u16 count, u16 w, u16 h, u16 0, then count x {u16 palette[256] (DS BGR555),
        # u8 pixels[h][w] (0 transparent)}; the runtime expands the shown frame into one texture
        fx0, fy0, fx1, fy1 = FACE_RECT
        blob = struct.pack("<4sHHHH", b"NVFC", len(faces), fx1 - fx0, fy1 - fy0, 0)
        for f, W, H, px, ps in faces:
            blob += struct.pack("<256H", *ps)
            blob += bytes(px[y * W + x] for y in range(fy0, fy1) for x in range(fx0, fx1))
        open(os.path.join(out, "navifaces_%s.bin" % c), "wb").write(blob)
        report.append("navi_%s_<lang>: panel %s, balloon %s, %d messages, %d page(s) %dx%d; navifaces_%s.bin: %d faces %s, %d KB" % (
            c, panel, bal, len(BOSS_MSGS), len(pages), pages[0]["w"], pages[0]["h"], c, len(faces), FACE_RECT, len(blob) // 1024))
        # tables: face sequences remapped to navifaces_<c>.bin frame numbers
        first_face = 0
        blob = b""
        for a, (cnt, fl, pairs) in enumerate(seqs):
            if a not in BOSS_SEQS:
                cnt, pairs = 0, []
            blob += struct.pack("<HH", cnt, fl)
            pl = [(d, first_face + used.index(f)) for d, f in pairs][:40]
            pl += [(0, 0)] * (40 - len(pl))
            blob += b"".join(struct.pack("<BB", d, i) for d, i in pl)
        sections.append(("seq_%s" % c, blob))     # 14 x {u16 count, u16 flags, u8 [40][2] (duration, image)}
        sections.append(("geo_%s" % c, struct.pack("<16h", *(panel + bal + face_rect + FACE_POS + (16, 16)))))
    # voices: Sound_PlayVoice(handle, data_0207566c[idx], data_02075xxx[idx][0])
    v = b""
    for idx in range(2):
        v += struct.pack("<i", rom.s32(0x0207566C + 4 * idx))
        for a in (0x0207567C, 0x02075680, 0x02075684, 0x02075688, 0x0207568C, 0x02075690):
            v += struct.pack("<i", rom.s32(a + 24 * idx))
    sections.append(("navi_voice", v))           # [2] x {arc, 0xf, 0x10, 0x11, 0x12, 0x13, 0x13 zone 7}
    sections.append(("navib_icon", rom.read(0x02073264, 16)))   # u16[8]
    sections.append(("navib_pos", rom.read(0x02073274, 32)))    # s16[8][2]
    sections.append(("er_shake", rom.read(0x02077944, 8)))      # s8[4][2]
    sections.append(("bhelp_pal", rom.read(0x02364D8C, 4)))  # s16[2] (palette rows, kept for reference)
    return report, sections


narc_names = {}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--extract", default="extract/eur")
    ap.add_argument("--out", default="build/dreamcast/ui")
    a = ap.parse_args()
    rom = Rom(a.extract)
    narc = open_narc(os.path.join(a.extract, "files/narc/act_com_lz7.narc"))
    members = []
    for i, (name, data) in enumerate(narc_members(narc)):
        narc_names[i] = name
        members.append((name, data))
    res = os.path.join(a.out, "res")
    os.makedirs(res, exist_ok=True)
    rep, _ = title_bgs(members, res)
    rep += results_bgs(members, res)
    r2, sections = navi(rom, os.path.join(a.extract, "files/bg2/navi"), res)
    rep += r2
    size = write_ruid(sections, os.path.join(a.out, "navidata.bin"))
    rep.append("navidata.bin: %d sections, %d bytes" % (len(sections), size))
    print("\n".join(rep))


def narc_members(narc):
    """(name, data) in file-index order"""
    if hasattr(narc, "files"):
        f = narc.files
        if isinstance(f, dict):
            return list(f.items())
        return list(f)
    return list(narc)


if __name__ == "__main__":
    main()

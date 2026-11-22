#!/usr/bin/env python3
"""Cutscene data converter (BYOR: reads only the owner's extracted ROM and the
outputs of the other Dreamcast converters, writes build/dreamcast/cutscene/).

Outputs (all regenerated from the ROM, never committed):
  csdata.bin        "RCSD" v1 section table (layout below), the ROM tables the
                    cutscene code reads at the addresses of the decompiled code
                    (TalkDemo, DemoScript, PlayerDemo, TransformDemo, Ending,
                    StaffRoll); pointers are resolved to indices / file names
  talk/<name>_<lang>.btk   TalkDemo scripts, copied unchanged (#BTK)
  talk/img.csp      the 92 images of /bg2/talk/talk_m.bb (portraits 10x10,
                    frames 22x10, full images) as indexed images
  talk/msg_<lang>.csp  the message pages of /bg2/talk/talk_m_<lang>.bb
  pldm/<name>.bpd   PlayerDemo scripts, copied unchanged (#BPD)
  pldm/poly_m.csp   /bg2/poly_m.bb (caption palette, bottom-screen frame,
                    the per-language captions)
  bg/<name>.csi     the .bbg pictures of Ending / StaffRoll
  bg/<name>.csr     the 256x9600 staff roll credit maps, row-compacted (CSR1 below)
  sprites.txt       sprite ids (build/dreamcast/sprites/bac/<id>) the disc needs
  models.txt        .dcm/.dca/.dcv files (build/dreamcast/models/) the disc needs
  music.txt         sys-SDAT sequences the cutscenes stream (+ aliases of the
                    duplicate sequences) and the extra STRM/SDAT music
  preview/*.png     previews of every converted image

Indexed image "CSI1" (little-endian):
  0  "CSI1"  4 u16 w, h (px)  8 u16 type (BBG flags & 0xF), u16 bbg flags
  12 u16 npal, u16 pofs       16 u16 pal[npal] (DS BGR555, palette colours
  pofs..pofs+npal-1)          then u8 px[h][w]: palette index, 4 bpp tiles
  flattened to bank*16 + value, 0 = transparent (DS colour 0)
Row-compacted "CSR1": the CSI1 header (magic "CSR1") and palette, zero-padded
  to a multiple of 4, s32 rowoff[h] (-1 = empty row, else offset of the row's
  w pixels in the row data), then the non-empty rows.
Pack "CSP1": u32 count, u32 offset[count] (0 = no entry), CSI1 blobs.

csdata.bin "RCSD": u32 magic, u16 version 1, u16 count, count x
  {char name[12], u32 offset, u32 size}, sections 4-byte aligned. The section
  layouts are documented next to each `sec()` call and in cs_internal.h.

Usage: python3 dreamcast/game/cutscene/tools/csdata.py [--out build/dreamcast/cutscene] [--no-preview]
"""
import argparse
import json
import re
import shutil
import struct
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT / "tools/dreamcast/sprites"))
sys.path.insert(0, str(ROOT / "tools/dreamcast/audio"))
from nds_archive import decompress  # noqa: E402

EXT = ROOT / "extract/eur"
FILES = EXT / "files"
A9 = (EXT / "arm9/arm9.bin").read_bytes()
OV = {0: (EXT / "arm9_overlays/ov000.bin").read_bytes(), 1: (EXT / "arm9_overlays/ov001.bin").read_bytes()}
OV_BASE = 0x022C5C80
LANGS = ["jpn", "eng", "fra", "deu", "ita", "spa"]


def mem(addr, n, ov=1):
    if 0x02000000 <= addr < 0x02000000 + len(A9):
        return A9[addr - 0x02000000:addr - 0x02000000 + n]
    src = OV[ov]
    if OV_BASE <= addr < OV_BASE + len(src):
        return src[addr - OV_BASE:addr - OV_BASE + n]
    raise ValueError("address %#x outside arm9/ov00%d" % (addr, ov))


def u32(a, ov=1):
    return struct.unpack("<I", mem(a, 4, ov))[0]


def s32(a, ov=1):
    return struct.unpack("<i", mem(a, 4, ov))[0]


def s16(a, ov=1):
    return struct.unpack("<h", mem(a, 2, ov))[0]


def u16(a, ov=1):
    return struct.unpack("<H", mem(a, 2, ov))[0]


def valid(a):
    return 0x02000000 <= a < 0x02400000


def cstr(a, ov=1):
    out = bytearray()
    while True:
        c = mem(a, 1, ov)[0]
        if c == 0:
            return out.decode("latin1")
        out.append(c)
        a += 1


def fixstr(s, n):
    b = s.encode("latin1")[:n - 1]
    return b + b"\0" * (n - len(b))


SYMS = {}
for f in [ROOT / "config/eur/arm9/symbols.txt"] + sorted((ROOT / "config/eur/arm9/overlays").glob("*/symbols.txt")):
    for line in f.read_text().splitlines():
        m = re.match(r"(\S+) kind:\S+ addr:0x([0-9a-f]+)", line)
        if m:
            SYMS.setdefault(int(m.group(2), 16) & ~1, m.group(1))
            SYMS[m.group(1)] = int(m.group(2), 16)


def sym(name):
    return SYMS[name]


def fn_name(a):
    return SYMS.get(a & ~1, "0x%x" % a) if a else "NULL"


# ------------------------------------------------------------------ images
def parse_bbg(data):
    if data[:4] != b"BBG\0":
        raise ValueError("not a BBG")
    _, ochr, oscr, opal, flags, w, h, pofs, pslot = struct.unpack_from("<IIIIIHHHH", data, 4)

    def sub(o):
        if not o or o + 4 > len(data):
            return b""
        try:
            return decompress(data[o:])
        except Exception:
            return b""
    return dict(type=flags & 0xF, flags=flags, w=w, h=h, pofs=pofs, pslot=pslot,
                chars=sub(ochr), screen=sub(oscr), pal=sub(opal))


def render_bbg(b, tile_base=0):
    """-> (W, H, indices). 4 bpp entries flattened to bank*16+value. tile_base:
    char number of the first tile in `chars` (the message pages are loaded at
    tile 256 of BG1, TalkDemo_LoadMessageBg)."""
    W, H = b["w"] * 8, b["h"] * 8
    bpp8 = b["type"] != 1
    tsz = 64 if bpp8 else 32
    chars, scr = b["chars"], b["screen"]
    nchar = len(chars) // tsz
    out = bytearray(W * H)
    for ty in range(b["h"]):
        for tx in range(b["w"]):
            i = ty * b["w"] + tx
            if 2 * i + 2 > len(scr):
                continue
            e = struct.unpack_from("<H", scr, 2 * i)[0]
            c, hf, vf, bank = (e & 0x3FF) - tile_base, e >> 10 & 1, e >> 11 & 1, e >> 12
            if c < 0 or c >= nchar:
                continue
            base = c * tsz
            for y in range(8):
                sy = 7 - y if vf else y
                row = (ty * 8 + y) * W + tx * 8
                for x in range(8):
                    sx = 7 - x if hf else x
                    if bpp8:
                        v = chars[base + sy * 8 + sx]
                        if v:
                            out[row + x] = v
                    else:
                        byte = chars[base + sy * 4 + sx // 2]
                        v = byte >> 4 if sx & 1 else byte & 0xF
                        if v:
                            out[row + x] = (bank * 16 + v) & 0xFF
    return W, H, bytes(out)


def csi_blob(b, W, H, px):
    pal = b["pal"]
    npal = min(len(pal) // 2, 256 - b["pofs"]) if pal else 0
    return (struct.pack("<4sHHHHHH", b"CSI1", W, H, b["type"], b["flags"] & 0xFFFF, npal, b["pofs"]) +
            pal[:2 * npal] + px)


def csp_blob(blobs):
    hdr = 4 + 4 * len(blobs)
    out = bytearray(struct.pack("<I", len(blobs)) + b"\0" * (4 * len(blobs)))
    for i, bl in enumerate(blobs):
        if bl is None:
            continue
        out.extend(b"\0" * (-len(out) % 4))
        struct.pack_into("<I", out, 4 + 4 * i, len(out))
        out.extend(bl)
    assert hdr <= len(out)
    return bytes(out)


def bb_entries(path):
    d = (FILES / path.lstrip("/")).read_bytes()
    assert d[:2] == b"BB", path
    n = u32_le(d, 4)
    return [d[u32_le(d, 8 + 8 * i):u32_le(d, 8 + 8 * i) + u32_le(d, 12 + 8 * i)] for i in range(n)]


def u32_le(d, o):
    return struct.unpack_from("<I", d, o)[0]


def preview(path, W, H, px, pal16, pofs=0, base_pal=None):
    try:
        from PIL import Image
    except ImportError:
        return
    cols = list(base_pal) if base_pal else [0] * 256
    for i, c in enumerate(pal16):
        if pofs + i < 256:
            cols[pofs + i] = c
    im = Image.new("RGBA", (W, H))
    im.putdata([((cols[p] & 31) * 255 // 31, (cols[p] >> 5 & 31) * 255 // 31, (cols[p] >> 10 & 31) * 255 // 31,
                 255 if p else 0) for p in px])
    im.save(path)


def pal_list(b):
    return [struct.unpack_from("<H", b["pal"], 2 * i)[0] for i in range(len(b["pal"]) // 2)]


# ------------------------------------------------------------------ main
class Out:
    def __init__(self, root, prev):
        self.root = Path(root)
        self.prev = prev
        self.sections = []
        self.sprites = set()
        self.models = set()
        self.report = []

    def sec(self, name, data):
        assert len(name) < 12
        self.sections.append((name, bytes(data)))

    def write_rcsd(self):
        hdr = 8 + 20 * len(self.sections)
        out = bytearray(hdr)
        struct.pack_into("<4sHH", out, 0, b"RCSD", 1, len(self.sections))
        for i, (n, d) in enumerate(self.sections):
            out.extend(b"\0" * (-len(out) % 4))
            struct.pack_into("<12sII", out, 8 + 20 * i, fixstr(n, 12), len(out), len(d))
            out.extend(d)
        (self.root / "csdata.bin").write_bytes(out)
        return len(out)


def sprite_resolver():
    man = json.loads((ROOT / "build/dreamcast/sprites/manifest.json").read_text())
    m = {}
    for e in man:
        if e.get("status") == "ok":
            m[e["source"]] = e["converted_as"]
    return m


def model_resolver():
    man = json.loads((ROOT / "build/dreamcast/models/manifest.json").read_text())
    m = {}
    for e in man["entries"]:
        m[e["source"]] = e.get("outputs", [])
    return m


def conv_talk(o):
    d = o.root / "talk"
    d.mkdir(parents=True, exist_ok=True)
    # scripts: TalkDemo_LoadScript: "/talk/" + data_0207ca5c[id] + suffix + ".btk"
    names = []
    for i in range(0x3A):
        a = u32(0x0207CA5C + 4 * i)
        names.append(cstr(a) if a and valid(a) else "")
    pages = {}
    for i, n in enumerate(names):
        if not n:
            continue
        for li, lang in enumerate(LANGS):
            src = FILES / "talk" / ("%s_%s.btk" % (n, lang))
            if src.exists():
                shutil.copy(src, d / src.name)
                if lang == "eng":
                    b = src.read_bytes()
                    pages[i] = u32_le(b, 0x10)
    # section td_name: 58 x char[32]
    o.sec("td_name", b"".join(fixstr(n, 32) for n in names))
    # td_msg: s32[6][58]  data_0207ca10[lang][id] (first message of the scene)
    msg = b""
    for li in range(6):
        t = u32(0x0207CA10 + 4 * li)
        msg += b"".join(struct.pack("<i", s32(t + 4 * i)) for i in range(0x3A))
    o.sec("td_msg", msg)
    # td_port: s32[12][16]  data_0207ca28[a][b] (talk_m.bb index, -1 none)
    port = b""
    for a in range(12):
        t = u32(0x0207CA28 + 4 * a)
        port += b"".join(struct.pack("<i", s32(t + 4 * k) if t and valid(t) else -1) for k in range(16))
    o.sec("td_port", port)
    # td_frame: s32[16]  data_02076c20[i]
    o.sec("td_frame", b"".join(struct.pack("<i", s32(0x02076C20 + 4 * i)) for i in range(16)))
    # td_bgm: s16[10] data_02076c54 (TalkDemo_PlayBGM, sys SEQ ids)
    o.sec("td_bgm", mem(0x02076C54, 20))
    # td_snd: 58 x {s32 group, s32 seqarc, s32 se, u32 flags}  data_0207747c
    snd = b""
    for i in range(0x3A):
        p, f = struct.unpack("<II", mem(0x0207747C + 8 * i, 8))
        if p and valid(p):
            g, arc, se = struct.unpack("<iii", mem(p, 12))
            snd += struct.pack("<iiiI", g, arc, se, f | 0x80000000)
        else:
            snd += struct.pack("<iiiI", -1, -1, -1, f)
    o.sec("td_snd", snd)
    # td_psnd: 16 x {s32 id, s32 frame, u32 flags} data_02076e58; td_pmap s32[16] data_02076d60;
    # td_p37: data_02076ee8 (gTalkDemoId 0x37)
    o.sec("td_psnd", mem(0x02076E58, 16 * 12))
    o.sec("td_pmap", mem(0x02076D60, 16 * 4))
    o.sec("td_p37", mem(0x02076EE8, 12))
    # motions: {u32 fn kind, s16 v[8]} data_020776a8 / {u32 fn, s32 u4, s16 v[6]} data_02077770
    kinds = {"TalkDemo_UpdateBg2Motion": 1, "TalkDemo_ResetBg2Affine": 2}
    ent = b""
    for i in range(16):
        e = mem(0x020776A8 + 20 * i, 20)
        fn = struct.unpack_from("<I", e)[0]
        ent += struct.pack("<I", kinds.get(fn_name(fn), 0)) + e[4:]
    o.sec("td_enter", ent)
    ext = b""
    for i in range(14):
        e = mem(0x02077770 + 20 * i, 20)
        fn = struct.unpack_from("<I", e)[0]
        ext += struct.pack("<I", kinds.get(fn_name(fn), 0)) + e[4:]
    o.sec("td_exit", ext)
    o.sec("td_rock", mem(0x02077654, 6 * 14))
    # images
    imgs = bb_entries("/bg2/talk/talk_m.bb")
    blobs = []
    pv = o.root / "preview"
    for i, e in enumerate(imgs):
        b = parse_bbg(e)
        W, H, px = render_bbg(b)
        blobs.append(csi_blob(b, W, H, px))
        if o.prev:
            preview(pv / ("talk_img_%02d.png" % i), W, H, px, pal_list(b), b["pofs"])
    (d / "img.csp").write_bytes(csp_blob(blobs))
    o.report.append("talk: %d scripts, %d images" % (len([n for n in names if n]) * 6, len(blobs)))
    frame_pal = pal_list(parse_bbg(imgs[91]))
    for li, lang in enumerate(LANGS):
        es = bb_entries("/bg2/talk/talk_m_%s.bb" % lang)
        blobs = []
        for i, e in enumerate(es):
            b = parse_bbg(e)
            # TalkDemo_LoadMessageBg: chars at 0x06006000 = BG1 char base + 0x2000 (tile 256)
            ent = struct.unpack_from("<H", b["screen"], 0)[0] if len(b["screen"]) >= 2 else 0
            tb = 256 if any((struct.unpack_from("<H", b["screen"], 2 * k)[0] & 0x3FF) >= 256
                            for k in range(len(b["screen"]) // 2)) else 0
            del ent
            W, H, px = render_bbg(b, tb)
            blobs.append(csi_blob(b, W, H, px))
            if o.prev and lang == "eng" and i < 40:
                preview(pv / ("talk_msg_eng_%03d.png" % i), W, H, px, frame_pal, 0)
        (d / ("msg_%s.csp" % lang)).write_bytes(csp_blob(blobs))
        o.report.append("talk messages %s: %d" % (lang, len(blobs)))
    # sprites: TalkDemo_LoadCommonSprites "%s%s.bac" % ("/act/talk_demo_com", suffix); rocks
    sp = o.spr
    ids = b""
    for lang in LANGS:
        sid = sp["act/talk_demo_com_%s.bac" % lang]
        o.sprites.add(sid)
        ids += fixstr(sid, 48)
    rock = sp[cstr(0x0207CC60).lstrip("/")]
    o.sprites.add(rock)
    ids += fixstr(rock, 48)
    o.sec("td_spr", ids)
    return pages


def conv_script(o):
    # gDemoScriptTable: 0x45 x {u8 *ops, s32 n, fn, u32 flags, s32 progress}; ops {u8 op, u8 arg, u16 delay}
    tgt = {"SceneChange_ToStage": 1, "ClearDemo_Create": 2, "SceneChange_ToEnding": 3, "SceneChange_ToExtraBoss": 4,
           "SceneChange_ToZoneMap_1_0": 5, "SceneChange_ToExtraAct": 6}
    a = sym("gDemoScriptTable")
    out = b""
    for i in range(0x45):
        sc, n, fn, fl, pr = struct.unpack("<IIIIi", mem(a + 20 * i, 20))
        ops = mem(sc, 4 * n) if n else b""
        ops += b"\0" * (16 - len(ops))
        out += struct.pack("<iiIi", n, tgt[fn_name(fn)], fl, pr) + ops[:16]
    o.sec("ds_table", out)  # 0x45 x {s32 n, s32 next, u32 flags, s32 progress, u8 ops[4][4]}


def conv_player(o):
    d = o.root / "pldm"
    d.mkdir(parents=True, exist_ok=True)
    for f in sorted((FILES / "pldm").glob("*.bpd")):
        shutil.copy(f, d / f.name)
    names = [cstr(u32(0x0207C368 + 4 * i)) for i in range(14)]
    o.sec("pd_bpdn", b"".join(fixstr(n, 24) for n in names))
    o.sec("pd_bpd", mem(0x02076214, 0x2C * 4))
    # model table data_020768b4: {char *name, u32 flags} (PolyDemo_BuildModelPath / GetModelFlags)
    mod = o.mod
    mnames, mflags = [], []
    for i in range(80):
        p, f = struct.unpack("<II", mem(0x020768B4 + 8 * i, 8))
        if not valid(p):
            break
        nm = cstr(p)
        mnames.append(nm)
        mflags.append(f)
    # scenes: data_020766a4[44] {entry *scenes, s32 count, s8 light_row, s8 light_col, u16 pad}
    #   entry {char *arc, u32 *models, s32 count}
    scenes = []
    demo = b""
    for i in range(0x2C):
        tbl, cnt, row, col = struct.unpack("<IIbb", mem(0x020766A4 + 12 * i, 10))
        first = len(scenes)
        for k in range(cnt if tbl else 0):
            arc, mlist, n = struct.unpack("<IIi", mem(tbl + 12 * k, 12))
            arcname = cstr(u32(arc)) if False else cstr(arc)
            ids = [s32(mlist + 4 * j) for j in range(n)]
            scenes.append((arcname, n, ids))
        demo += struct.pack("<iibbH", first, cnt if tbl else 0, row, col, 0)
    o.sec("pd_demo", demo)  # 44 x {s32 first_scene, s32 count, s8 row, s8 col, u16 pad}
    # pd_scene: n x {char dca[48], char dcv[48], char cam[64], char int[64], s32 count,
    #   {char dcm[48], u32 flags} x 16}  (slot 0/1 = cam/int of the archive's mdl.nsbmd)
    sc = b""
    matx = b""     # pd_matx: n x {char dcc[48], char dcp[48], char dct[48]}: mdl.nsbma / .nsbtp / .nsbta
    for arcname, n, ids in scenes:
        base = arcname
        for ext, ofmt in ((".nsbma", ".dcc"), (".nsbtp", ".dcp"), (".nsbta", ".dct")):
            f = [x for x in mod.get(base + ":mdl" + ext, []) if x.endswith(ofmt)]
            if f:
                o.models.add(f[0])
            matx += fixstr(f[0] if f else "", 48)
        dca = mod.get(base + ":mdl.nsbca", [""])
        dcv = mod.get(base + ":mdl.nsbva", [""])
        mdl = mod.get(base + ":mdl.nsbmd", ["", ""])
        for f in dca[:1] + dcv[:1] + mdl[:2]:
            if f:
                o.models.add(f)
        rec = fixstr(dca[0] if dca else "", 48) + fixstr(dcv[0] if dcv else "", 48)
        rec += fixstr(mdl[0] if mdl else "", 64) + fixstr(mdl[1] if len(mdl) > 1 else "", 64)
        rec += struct.pack("<i", n)
        for j in range(16):
            if 2 <= j < n:
                nm = mnames[ids[j]]
                outs = mod.get("mod/%s.nsbmd" % nm, [])
                f = outs[0] if outs else ""
                if f:
                    o.models.add(f)
                rec += fixstr(f, 48) + struct.pack("<I", mflags[ids[j]])
            else:
                rec += fixstr("", 48) + struct.pack("<I", 5 if j < 2 else 0)
        sc += rec
    o.sec("pd_scene", sc)
    o.sec("pd_matx", matx)
    o.report.append("player demo: %d scenes" % len(scenes))
    # lights: data_0207c3a0[demo][act] {s32 vec_alt_idx? ...}: PlayerDemoLight_* tables
    # (colour rows data_02074960 [row][col] 3 x u16, vectors data_020748bc[row] 3 x s16[3],
    #  overrides data_020761b4 colours / data_020762c4 vectors indexed from data_0207c3a0)
    lt = b""
    for i in range(0x2C):
        p = u32(0x0207C3A0 + 4 * i)
        for k in range(8):
            lt += mem(p + 8 * k, 8) if p and valid(p) else b"\0" * 8
    o.sec("pd_lsel", lt)  # 44 x 8 x {s32 vec_idx, s32 col_idx}
    o.sec("pd_lcol", mem(0x02074960, 0x12 * 8))  # [row] x 3 cols x 3 u16
    o.sec("pd_lvec", mem(0x020748BC, 0x12 * 8))  # [row] 3 x s16[3]
    o.sec("pd_lcol2", mem(0x020761B4, 6 * 32))
    o.sec("pd_lvec2", mem(0x020762C4, 0x12 * 16))
    # sounds: data_02076544[44] {entry *e, u32 flags} e = {group, seqarc, ?}
    snd = b""
    for i in range(0x2C):
        p, f = struct.unpack("<II", mem(0x02076544 + 8 * i, 8))
        if p and valid(p):
            snd += struct.pack("<iiiI", s32(p), s32(p + 4), s32(p + 8), f | 0x80000000)
        else:
            snd += struct.pack("<iiiI", -1, -1, -1, f)
    o.sec("pd_snd", snd)
    o.sec("pd_bgm", mem(0x02075A34, 12 * 2))   # s16[12] PlayerDemoOp_PlayBGM
    o.sec("pd_se", mem(0x0207613C, 32 * 2))    # s16[32] PlayerDemoOp_PlaySE
    o.sec("pd_voice", mem(0x0207584C, 16 * 2))  # s16[16] PlayerDemoOp_PlayVoice
    o.sec("pd_talk", mem(0x02076AFC, 8 * 8))    # {s32 demo, s32 talk} PlayerDemoOp_StartTalkDemo
    o.sec("pd_fade", mem(0x02076ADC, 4 * 8))    # {u32 mode, s32 bright} PlayerDemo_StartFade
    # captions: data_020763e4[44] {u32 *offs, u32 count} + language base data_02075b6c[6]
    cap = b""
    for i in range(0x2C):
        p, c = struct.unpack("<II", mem(0x020763E4 + 8 * i, 8))
        offs = [u32(p + 4 * k) for k in range(c)] if p else []
        cap += struct.pack("<i", len(offs)) + b"".join(struct.pack("<i", x) for x in (offs + [0] * 12)[:12])
    o.sec("pd_cap", cap)   # 44 x {s32 count, s32 offs[12]}
    o.sec("pd_capl", mem(0x02075B6C, 24))
    es = bb_entries("/bg2/poly_m.bb")
    blobs = []
    cappal = pal_list(parse_bbg(es[0]))
    for i, e in enumerate(es):
        b = parse_bbg(e)
        W, H, px = render_bbg(b)
        blobs.append(csi_blob(b, W, H, px))
        if o.prev and (i < 4 or 37 <= i < 45):
            preview(o.root / "preview" / ("poly_m_%03d.png" % i), W, H, px, pal_list(b) or cappal, b["pofs"])
    (d / "poly_m.csp").write_bytes(csp_blob(blobs))
    # PolyDemoUnder2 sprites
    ids = fixstr(o.spr["act/poly_demo_under2.bac"], 48)
    o.sprites.add(o.spr["act/poly_demo_under2.bac"])
    for lang in LANGS:
        s = o.spr["act/poly_demo_under2_%s.bac" % lang]
        o.sprites.add(s)
        ids += fixstr(s, 48)
    o.sec("pd_spr", ids)


def conv_trans(o):
    mod = o.mod
    o.sec("tr_id", mem(0x023161EC, 12))           # data_ov001_023161ec [id] {u8 kind, u8 player}
    lst = sym("gTransformDemoModelList")
    files = []
    for i in range(16):
        nm = cstr(u32(lst + 4 * i)).lstrip("/")
        outs = mod.get(nm, [])
        files.append(outs[0] if outs else "")
        if outs:
            o.models.add(outs[0])
    o.sec("tr_mlist", b"".join(fixstr(f, 48) for f in files))
    o.sec("tr_models", mem(0x02316244, 3 * 24))   # [kind] s32[6] model list index
    for f in (mod.get("narc/pldm_trans_lz7.narc:mdl.nsbca", []) + mod.get("narc/pldm_trans_lz7.narc:mdl.nsbva", []) +
              mod.get("narc/pldm_trans_lz7.narc:mdl.nsbma", []) + mod.get("narc/pldm_trans_lz7.narc:mdl.nsbtp", []) +
              mod.get("narc/pldm_trans_lz7.narc:mdl.nsbta", [])):
        o.models.add(f)
    for f in ["narc__player_lz7__son.dca", "narc__player_lz7__son.dcv", "mod__son__son.dcm", "mod__blz__blz.dcm"]:
        o.models.add(f)
    o.sec("tr_light", mem(0x023161E4, 0x60))      # data_ov001_023161e4..0231623f: edge, ids, cols, vecs


def csr_blob(b, W, H, px):
    """Row-compacted picture "CSR1" for the tall credit maps (256x9600): the CSI1
    header and palette, padded to 4, then s32 rowoff[H] (-1 = empty row, else the
    offset of its W pixels in the row data), then the non-empty rows."""
    head = bytearray(csi_blob(b, W, H, b""))
    head[0:4] = b"CSR1"
    head.extend(b"\0" * (-len(head) % 4))
    offs, rows = [], bytearray()
    for y in range(H):
        r = px[y * W:(y + 1) * W]
        if any(r):
            offs.append(len(rows))
            rows.extend(r)
        else:
            offs.append(-1)
    return bytes(head) + struct.pack("<%di" % H, *offs) + bytes(rows)


def conv_bg(o, path):
    name = Path(path).stem
    out = o.root / "bg" / (name + ".csi")
    csr = o.root / "bg" / (name + ".csr")
    if out.exists() or csr.exists():
        return name
    out.parent.mkdir(parents=True, exist_ok=True)
    src = FILES / path.lstrip("/")
    b = parse_bbg(src.read_bytes())
    W, H, px = render_bbg(b)
    if H > 1024:
        csr.write_bytes(csr_blob(b, W, H, px))   # staff roll credit maps
    else:
        out.write_bytes(csi_blob(b, W, H, px))
    if o.prev:
        preview(o.root / "preview" / ("bg_%s.png" % name), W, H, px, pal_list(b), b["pofs"])
    return name


def conv_ending(o):
    son, blz, spr = sym("gEndingSonicPictureTable"), sym("gEndingBlazePictureTable"), sym("gEndingSpriteFileTable")
    names = b""
    for tbl in (son, blz):
        for li in range(6):
            t = u32(tbl + 4 * li)
            for i in range(19):
                names += fixstr(conv_bg(o, cstr(u32(t + 4 * i))), 32)
    o.sec("ed_pics", names)   # [char][lang][19] char[32]
    ids = b""
    for li in range(6):
        t = u32(spr + 4 * li)
        for i in range(2):
            s = o.spr[cstr(u32(t + 4 * i))]
            o.sprites.add(s)
            ids += fixstr(s, 48)
    o.sec("ed_spr", ids)      # [lang][2] char[48]
    o.sec("ed_tab", mem(0x02315EB4, 0x90))   # data_ov001_02315eb4..f44 (layers, sprite tables, bgm)
    o.sec("ed_timed", mem(0x02315F44, 2 * 5 * 3 * 4))
    # StaffRoll
    a, b = sym("gStaffRollBgFileList"), sym("gStaffRollExtraBgFileList")
    names = b""
    for tbl in (a, b):
        for i in range(15):
            p = u32(tbl + 4 * i)
            names += fixstr(conv_bg(o, cstr(p)) if p else "", 32)
    # language variants of ed_staff03
    for lang in LANGS:
        p = FILES / "bg" / ("ed_staff03_%s.bbg" % lang)
        if p.exists():
            conv_bg(o, "bg/ed_staff03_%s.bbg" % lang)
    o.sec("sr_bgs", names)    # [normal, extra][15] char[32]
    t = sym("gStaffRollSpriteFileTable")
    ids = b""
    for li in range(6):
        tt = u32(t + 4 * li)
        for i in range(3):
            s = o.spr[cstr(u32(tt + 4 * i))]
            o.sprites.add(s)
            ids += fixstr(s, 48)
    o.sec("sr_spr", ids)      # [lang][3] char[48]
    o.sec("sr_tab", mem(0x023160DC, 0x80))   # data_ov001_023160dc..0231611b


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", default=str(ROOT / "build/dreamcast/cutscene"))
    ap.add_argument("--no-preview", action="store_true")
    a = ap.parse_args()
    o = Out(a.out, not a.no_preview)
    o.root.mkdir(parents=True, exist_ok=True)
    (o.root / "preview").mkdir(exist_ok=True)
    o.spr = sprite_resolver()
    o.mod = model_resolver()
    o.sec("sincos", mem(0x0206E470, 4096 * 4))   # FX_SinCosTable_: 4096 x {s16 sin, s16 cos}
    conv_talk(o)
    conv_script(o)
    conv_player(o)
    conv_trans(o)
    conv_ending(o)
    n = o.write_rcsd()
    (o.root / "sprites.txt").write_text("# sprite ids under build/dreamcast/sprites/bac (copy to /cd/sprites)\n" +
                                        "\n".join(sorted(o.sprites)) + "\n")
    missing = [f for f in sorted(o.models) if not (ROOT / "build/dreamcast/models" / f).exists()]
    (o.root / "models.txt").write_text("# files under build/dreamcast/models (copy to /cd/models)\n" +
                                       "\n".join(sorted(o.models)) + "\n")
    # music: sys SDAT sequences used + duplicates resolved to the converted file
    man = json.loads((ROOT / "build/dreamcast/audio/manifest.json").read_text())
    byfile, seqs = {}, {}
    for e in man["entries"]:
        if e.get("kind") == "music" and e.get("sdat") == "sys":
            n_ = int(re.search(r"SEQ#(\d+)", e["source"]).group(1))
            f = int(re.search(r"file(\d+)", e["source"]).group(1))
            seqs[n_] = (e["status"], f)
            if e["status"] == "ok":
                byfile[f] = n_
    lines = ["# seq alias: <seq> <seq whose sNN.wav to play> (duplicate SSEQ files are converted once)"]
    for n_, (st, f) in sorted(seqs.items()):
        if st == "duplicate" and f in byfile:
            lines.append("%d %d" % (n_, byfile[f]))
    (o.root / "music.txt").write_text("\n".join(lines) + "\n")
    with open(o.root / "seqalias.txt", "w") as fp:
        for line in lines[1:]:
            fp.write(line + "\n")
    print("csdata.bin %d bytes, %d sections" % (n, len(o.sections)))
    for r in o.report:
        print(r)
    print("sprites %d, models %d (missing %d)" % (len(o.sprites), len(o.models), len(missing)))
    for m in missing:
        print("  missing model", m)
    return 0


if __name__ == "__main__":
    sys.exit(main())

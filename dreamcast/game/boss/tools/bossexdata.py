#!/usr/bin/env python3
"""Extra Zone boss (ExtraBoss, overlay ov001, stage 26) data for the native Dreamcast port.

Bring-your-own-ROM: reads the owner's extracted ROM (extract/eur) and the converted model / sprite
packages (build/dreamcast/models, build/dreamcast/sprites) and writes build/dreamcast/boss/extra/:

  bossex.bin    "BXDT" v1, little endian:
                  u32 magic, u16 version, u16 chunk count
                  { u32 ds_addr, u32 size, u32 file_offset } * count   ROM tables keyed by DS address
                  chunk data (each chunk starts at an offset with the address' alignment)
                  then 4-aligned sections, each "u32 count" + records:
                  models  { char file[96] }            0..4 = bsex_md models 0..4 (Model_SetResource
                                                        index), 5 e_son, 6 e_blz, 7 son_easy, 8 blz_easy
                  anims   { char file[96] }            0 bsex_ca (.dca), 1 bsex_va (.dcv),
                                                        2 ex_ply_ca (.dca), 3 ex_ply_va (.dcv)
                  joints  { u8 model, u8 pad, u16 node, char name[16] }  NNS_G3dGetResDictIdxByName
                  fx      { u32 flags; char model[96]; char anim[5][96];
                            u16 count[5]; u16 frames[5][8] }   gExtraBossFxResTable (27 kinds):
                                                        anim[0] joint (.dca), anim[4] visibility (.dcv);
                                                        slots 1..3 (material / pattern / SRT) are not
                                                        converted: their numFrame per animation index
                                                        is kept (timing only)
  sprites/<id>/{frames.bin,atlas0.argb1555,atlas0.pt4,palettes.bin}   ac_ex_fix_moji (HUD, archive file 0x26),
                                              ac_itm_ring (archive file 0x27) and ac_ex_vs (0x21)
  vs_son.bin / vs_blz.bin   the sub screen's VS splash BGs (extra_vs_bg_son / _blz.bbg, files 0x22 /
                0x23, 512x192), "EXBG" with npal = 0
  zex.bin       "EXBG" v1: u16 w, u16 h, u16 npal, u16 0, u16 base[256] (zex.bbg palette),
                u16 pal[npal][192] (archive files 0xc..0x20: zex_b0pal .. zex_t5r4pal, the colours
                PaletteTransfer_QueueCopy sends), u8 pixels[h][w] (bank * 16 + colour; 0 = transparent)
  assets.txt    the model packages the fight needs on the disc (/cd/models)

Nothing from the ROM is compiled into the C sources (dreamcast/game/boss/boss_extra*.c).

Usage: bossexdata.py [--extract extract/eur] [--models build/dreamcast/models]
                     [--sprites build/dreamcast/sprites] [--out build/dreamcast/boss/extra]
"""
import argparse
import json
import os
import shutil
import struct
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "../../../.."))
sys.path.insert(0, os.path.join(ROOT, "tools/dreamcast/models"))
sys.path.insert(0, os.path.join(ROOT, "tools/dreamcast/sprites"))

ARM9_BASE = 0x02000000
OV001_BASE = 0x022C5C80

CHUNKS = [
    # ov001 read-only tables of the Extra boss: flash palette tables (0231670c..14), node names
    # (02316718..023167a8), "/mod/bsex_md.nsbmd", boss hit points [2][5] (023167bc), voices, palette
    # tables (023167ec..02316804), wait timers (02316810), hurt offsets (02316820),
    # gExtraBossHandBossAnimTable (02316840), weapon tables (02316910..02316974), camera slots
    # (023169b4, 3 x 0x44), ring scroll tables (02316b9c / 02316cac), HUD tables (02316e84 / 02316e94),
    # background tables (02316ecc .. 02316f94)
    ("ov001_rodata", 0x0231670C, 0x02316FE4 - 0x0231670C),
    # ov001 data: trail matrix (0231d3e0), node name pointer tables (0231d404..0231d44c), palette name
    # tables (0231d4ac, 0231d518, 0231d408), sword animation table (0231d4c0), camera functions
    # (0231d52c), difficulty tables (0231d5d8..0231d68c), gExtraBossFxResTable (0231d6b4), player
    # parameters (0231da50) and state tables (0231da80 / 0231dc40), sub-screen tables (0231df2c..)
    ("ov001_data", 0x0231D3E0, 0x0231DF78 - 0x0231D3E0),
    # zone lights data_020748bc (vectors) / data_02074960 (colours), edge colours data_02074ba4
    ("zone_lights", 0x020748BC, 0x02074A04 - 0x020748BC),
    ("edge_colors", 0x02074BA4, 16),
]

CHECKS = [
    (0x023167A8, b"/mod/bsex_md.nsbmd\0"),
    (0x02316798, b"hand_catch\0"),
    (0x0231D9B0, b"/narc/extra_act_lz7.narc\0"),
    (0x0231DA14, b"/mod/e_son.nsbmd\0"),
]

FX_TABLE = 0x0231D6B4
FX_COUNT = 0x1B
ARCHIVE = "narc/extra_act_lz7.narc"

# names looked up with NNS_G3dGetResDictIdxByName by the node callbacks
# (ExtraBoss_WeaponNodeCallback data_ov001_0231d430, ExtraBoss_HandNodeCallback data_ov001_0231d440,
# ExtraBoss_HandCatchNodeCallback data_ov001_0231d404)
JOINTS = [
    (0, "p_body"), (0, "core"), (0, "weapon_l"), (0, "weapon_r"),
    (2, "p_body"), (2, "l_hand0a"), (2, "body_hand"), (2, "core"),
    (3, "hand_catch"),
]


def read_range(arm9, ov001, addr, size):
    if OV001_BASE <= addr < OV001_BASE + len(ov001):
        o = addr - OV001_BASE
        return ov001[o:o + size]
    if ARM9_BASE <= addr < ARM9_BASE + len(arm9):
        o = addr - ARM9_BASE
        return arm9[o:o + size]
    raise ValueError("address %08x not in arm9/ov001" % addr)


def anim_frame_counts(d):
    import g3d
    magic, blocks = g3d.file_blocks(d)
    blk = {b"BTA0": b"SRT0", b"BMA0": b"MAT0", b"BTP0": b"PAT0", b"BCA0": b"JNT0", b"BVA0": b"VIS0"}[magic]
    bo = blocks[blk]
    return [g3d.u16(d, bo + g3d.u32(ad, 0) + 4) for _, ad in g3d.read_dict(d, bo + 8)]


def bbg_parse(data):
    from nds_archive import decompress
    if data[:4] != b"BBG\0":
        raise ValueError("not a BBG")
    _, ochr, oscr, opal, flags, w, h, pofs, pslot = struct.unpack_from("<IIIIIHHHH", data, 4)
    return dict(type=flags & 0xF, w=w, h=h, pofs=pofs,
                chars=decompress(data[ochr:]) if ochr else b"",
                screen=decompress(data[oscr:]) if oscr else b"",
                pal=decompress(data[opal:]) if opal else b"")


def bbg_render_4bpp(b):
    """type 1: 4 bpp text BG, screen entries tile | hflip 0x400 | vflip 0x800 | bank << 12"""
    W, H = b["w"] * 8, b["h"] * 8
    px = bytearray(W * H)
    for ty in range(b["h"]):
        for tx in range(b["w"]):
            e = struct.unpack_from("<H", b["screen"], 2 * (ty * b["w"] + tx))[0]
            tile, hf, vf, bank = e & 0x3FF, e & 0x400, e & 0x800, e >> 12
            for y in range(8):
                for x in range(8):
                    sx, sy = (7 - x if hf else x), (7 - y if vf else y)
                    o = tile * 32 + sy * 4 + sx // 2
                    v = b["chars"][o] if o < len(b["chars"]) else 0
                    v = (v >> 4) if sx & 1 else (v & 15)
                    px[(ty * 8 + y) * W + tx * 8 + x] = bank * 16 + v if v else 0
    return W, H, px


def main(argv):
    ap = argparse.ArgumentParser()
    ap.add_argument("--extract", default=os.path.join(ROOT, "extract/eur"))
    ap.add_argument("--models", default=os.path.join(ROOT, "build/dreamcast/models"))
    ap.add_argument("--sprites", default=os.path.join(ROOT, "build/dreamcast/sprites"))
    ap.add_argument("--out", default=os.path.join(ROOT, "build/dreamcast/boss/extra"))
    a = ap.parse_args(argv)
    import g3d
    from nds_archive import open_narc, maybe_decompress

    arm9 = open(os.path.join(a.extract, "arm9", "arm9.bin"), "rb").read()
    ov001 = open(os.path.join(a.extract, "arm9_overlays", "ov001.bin"), "rb").read()
    for addr, want in CHECKS:
        if read_range(arm9, ov001, addr, len(want)) != want:
            print("ROM check failed at %08x (wrong ROM region?)" % addr)
            return 1

    # ---------------------------------------------------------------- ROM chunks
    hdr_size = 8 + 12 * len(CHUNKS)
    data = bytearray()
    table = []
    for name, addr, size in CHUNKS:
        while (hdr_size + len(data)) % 4 != addr % 4:
            data.append(0)
        table.append((addr, size, hdr_size + len(data)))
        data += read_range(arm9, ov001, addr, size)
    out = bytearray(struct.pack("<4sHH", b"BXDT", 1, len(CHUNKS)))
    for t in table:
        out += struct.pack("<III", *t)
    out += data
    while len(out) % 4:
        out.append(0)

    # ---------------------------------------------------------------- package names
    man = json.load(open(os.path.join(a.models, "manifest.json")))
    by_src = {e["source"]: e for e in man["entries"]}

    def resolve(src):
        e = by_src.get(src)
        while e is not None and "duplicate_of" in e:
            e = by_src.get(e["duplicate_of"])
        return e

    def outputs(src, ext=None):
        e = resolve(src)
        if e is None or not e.get("outputs"):
            return []
        return [o for o in e["outputs"] if ext is None or o.endswith(ext)]

    assets = []
    bsex = g3d.parse_bmd(open(os.path.join(a.extract, "files/mod/bsex_md.nsbmd"), "rb").read())
    mnames = [m["name"] for m in bsex["models"]]
    bsex_out = outputs("mod/bsex_md.nsbmd", ".dcm")
    models = []
    for n in mnames:   # Model_SetResource(a, data_ov001_0231f070, idx): idx = MDL0 dictionary order
        f = [o for o in bsex_out if o.endswith("__%s.dcm" % n)]
        models.append(f[0] if f else "")
    for m in ("e_son", "e_blz", "son_easy", "blz_easy"):
        f = outputs("mod/%s.nsbmd" % m, ".dcm")
        models.append(f[0] if f else "")
    anims = []
    for member, ext in (("bsex_ca.nsbca", ".dca"), ("bsex_va.nsbva", ".dcv"), ("ex_ply_ca.nsbca", ".dca"),
                        ("ex_ply_va.nsbva", ".dcv")):
        f = outputs("%s:%s" % (ARCHIVE, member), ext)
        anims.append(f[0] if f else "")
    out += struct.pack("<I", len(models))
    for m in models:
        out += struct.pack("<96s", m.encode())
    out += struct.pack("<I", len(anims))
    for m in anims:
        out += struct.pack("<96s", m.encode())
    assets += models + anims

    # ---------------------------------------------------------------- joints
    out += struct.pack("<I", len(JOINTS))
    for mi, node in JOINTS:
        names = [n["name"] for n in bsex["models"][mi]["nodes"]]
        out += struct.pack("<BBH16s", mi, 0, names.index(node) if node in names else 0xFFFF, node.encode())

    # ---------------------------------------------------------------- fx resources
    narc = open_narc(os.path.join(a.extract, "files", ARCHIVE))
    files = narc.files
    out += struct.pack("<I", FX_COUNT)
    nfx = 0
    for k in range(FX_COUNT):
        rec = read_range(arm9, ov001, FX_TABLE + 0x1C * k, 0x1C)
        flags = struct.unpack_from("<I", rec, 0)[0]
        ids = struct.unpack_from("<6i", rec, 4)
        mname = files[ids[0]][0]
        mfile = outputs("%s:%s" % (ARCHIVE, mname), ".dcm")
        mfile = mfile[0] if mfile else ""
        if mfile:
            assets.append(mfile)
            nfx += 1
        afile = [""] * 5
        counts = [0] * 5
        frames = [[0] * 8 for _ in range(5)]
        for s in range(5):
            fid = ids[s + 1]
            if fid < 0:
                continue
            member = files[fid][0]
            fr = anim_frame_counts(maybe_decompress(files[fid][1]))
            counts[s] = len(fr)
            frames[s][:min(8, len(fr))] = fr[:8]
            if s == 0 or s == 4:
                f = outputs("%s:%s" % (ARCHIVE, member), ".dca" if s == 0 else ".dcv")
                if f:
                    afile[s] = f[0]
                    assets.append(f[0])
        out += struct.pack("<I96s", flags, mfile.encode())
        for s in range(5):
            out += struct.pack("<96s", afile[s].encode())
        out += struct.pack("<5H", *counts)
        for s in range(5):
            out += struct.pack("<8H", *frames[s])
        out += b"\0\0"

    os.makedirs(a.out, exist_ok=True)
    with open(os.path.join(a.out, "bossex.bin"), "wb") as f:
        f.write(out)

    # ---------------------------------------------------------------- sprites
    smf = json.load(open(os.path.join(a.sprites, "manifest.json")))
    spr = {e["source"]: e for e in smf}
    for member, sid in (("ac_ex_fix_moji.bac", "ex_fix_moji"), ("ac_itm_ring.bac", "itm_ring"),
                        ("ac_ex_vs.bac", "ex_vs")):
        e = spr.get("%s:%s" % (ARCHIVE, member))
        if e is None or e.get("status") != "ok":
            print("sprite %s not converted (run tools/dreamcast/sprites/bac2dc.py)" % member)
            return 1
        src = os.path.join(a.sprites, "bac", e["converted_as"])
        dst = os.path.join(a.out, "sprites", sid)
        os.makedirs(dst, exist_ok=True)
        # + the PVR PAL4 page and its palettes (SPRITES.md section 4) when the converter made them
        for fn in ("frames.bin", "atlas0.argb1555", "atlas0.pt4", "palettes.bin"):
            if os.path.exists(os.path.join(src, fn)) or fn in ("frames.bin", "atlas0.argb1555"):
                shutil.copyfile(os.path.join(src, fn), os.path.join(dst, fn))

    # ---------------------------------------------------------------- background (zex.bbg + palettes)
    zex = bbg_parse(maybe_decompress(files[0xB][1]))
    if zex["type"] != 1:
        print("zex.bbg: unexpected type %d" % zex["type"])
        return 1
    W, H, px = bbg_render_4bpp(zex)
    base = [0] * 256
    for i in range(min(256, len(zex["pal"]) // 2)):
        base[i] = struct.unpack_from("<H", zex["pal"], 2 * i)[0]
    pals = []
    for fid in range(0xC, 0x21):
        p = bbg_parse(maybe_decompress(files[fid][1]))["pal"]
        pals.append([struct.unpack_from("<H", p, 2 * i)[0] if 2 * i + 1 < len(p) else 0 for i in range(192)])
    blob = struct.pack("<4sHHHH", b"EXBG", W, H, len(pals), 0)
    blob += struct.pack("<256H", *base)
    for p in pals:
        blob += struct.pack("<192H", *p)
    blob += bytes(px)
    with open(os.path.join(a.out, "zex.bin"), "wb") as f:
        f.write(blob)

    # ---------------------------------------------------------------- VS splash BGs (sub screen, 512x192)
    for fid, name in ((0x22, "vs_son.bin"), (0x23, "vs_blz.bin")):
        vb = bbg_parse(maybe_decompress(files[fid][1]))
        if vb["type"] != 1:
            print("%s: unexpected type %d" % (files[fid][0], vb["type"]))
            return 1
        vw, vh, vpx = bbg_render_4bpp(vb)
        vpal = [struct.unpack_from("<H", vb["pal"], 2 * i)[0] if 2 * i + 1 < len(vb["pal"]) else 0
                for i in range(256)]
        with open(os.path.join(a.out, name), "wb") as f:
            f.write(struct.pack("<4sHHHH", b"EXBG", vw, vh, 0, 0) + struct.pack("<256H", *vpal) + bytes(vpx))

    missing = 0
    with open(os.path.join(a.out, "assets.txt"), "w") as f:
        for fn in dict.fromkeys(x for x in assets if x):
            ok = os.path.exists(os.path.join(a.models, fn))
            missing += not ok
            f.write("models/%s%s\n" % (fn, "" if ok else "   # MISSING"))
        for fn in ("bossex.bin", "zex.bin", "sprites/ex_fix_moji/frames.bin", "sprites/ex_fix_moji/atlas0.argb1555",
                   "sprites/itm_ring/frames.bin", "sprites/itm_ring/atlas0.argb1555", "sprites/ex_vs/frames.bin",
                   "sprites/ex_vs/atlas0.argb1555", "vs_son.bin", "vs_blz.bin"):
            f.write("boss/extra/%s\n" % fn)
        for sid in ("ex_fix_moji", "itm_ring", "ex_vs"):
            for fn in ("atlas0.pt4", "palettes.bin"):
                if os.path.exists(os.path.join(a.out, "sprites", sid, fn)):
                    f.write("boss/extra/sprites/%s/%s\n" % (sid, fn))
    print("bossex.bin: %d bytes (%d chunks, %d models, %d anim sets, %d fx kinds with a model); zex.bin %dx%d, "
          "%d palettes; %d model packages missing" % (len(out), len(CHUNKS), len(models), len(anims), nfx, W, H,
                                                      len(pals), missing))
    for i, m in enumerate(models):
        print("  model %d: %s" % (i, m or "-"))
    return 0 if missing == 0 else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))

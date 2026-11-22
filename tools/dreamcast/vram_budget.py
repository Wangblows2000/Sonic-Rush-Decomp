#!/usr/bin/env python3
"""Offline PVR VRAM budget of every stage of the Dreamcast port (docs/dreamcast/GRAPHICS.md).

Computes, from the converted files in build/dreamcast, what each stage holds in PVR texture
memory with the allocation rules of the game:
  * texture RAM = 8 MB minus the TA/OPB/tile-matrix/frame buffers KOS reserves for the
    pvr_init_params_t of dreamcast/game/src/main.c (KOS pvr_allocate_buffers, 640x480 RGB565);
  * every pvr_mem_malloc of n bytes costs (n + 4) rounded up to 32 (KOS dlmalloc chunk:
    4-byte size field, 32-byte alignment); textures are power-of-two;
  * level: blocktex pages (512x512 PAL8) + the BG texture (engine/level.c);
  * objects: the atlas pages of the sprite resources objects_init preloads (the list comes
    from objects/test/test_coverage with COV_RES=1: every placed object and decoration
    created; no resource is loaded later);
  * UI: the ten resources ui_init loads, the title-card background of the stage (tcbg_N),
    the boss navigator (navi_c_eng, 128x128 face texture) on boss stages, the 256x256
    ARGB1555 variant cache (before: the HUD tension gauge allocates it on the first frame);
  * player: son.dcm textures; boss: the .dcm textures of boss/assets.txt for the stage.
"before": every sprite page ARGB1555 (the loaders before the PAL4 atlases).
"after": PAL4 pages where palettes.bin says so, palette rows simulated with the runtime
rules (engine/render.c): the level's PAL8 banks take only the rows their texels use;
object palettes take rows top-down at load, shared by content; a page whose palettes find
no row is expanded to ARGB1555; UI rows are taken on first draw (HUD + title card at the
stage start).

Usage: vram_budget.py [--root .] [--res res.txt] [--markdown]
  res.txt: output of  COV_RES=1 build/dreamcast/objects/test_coverage <player> <objects>
           <sprites/bac> <stages> <stage>...   (run automatically when missing)
"""
import argparse
import json
import os
import struct
import subprocess
import sys

STAGES = ["z11", "z12", "z13", "z21", "z22", "z23", "z31", "z32", "z33", "z41", "z42", "z43",
          "z51", "z52", "z53", "z61", "z62", "z63", "z71", "z72", "z73", "z81", "z82", "zf3", "z91", "z92"]
UI_STATIC = ["act__ac_fix_moji", "act__dm_cldm", "act__dm_stdm", "act__dm_stdm_eng", "act__ac_fix_pause",
             "act__ac_fix_pause_frame", "act__ac_fix_gameover", "act__navi_b", "act__ac_fix_bosshelp",
             "act__ac_eff_enemyroom_fix"]
UI_DRAWN_AT_START = ["act__ac_fix_moji", "act__dm_stdm", "act__dm_stdm_eng"]   # HUD + title card


def chunk(n):
    return max(32, (n + 4 + 31) // 32 * 32)


def kos_texture_ram(w=640, h=480, bpp=2, opb=(16, 0, 16, 0, 16), vbuf=512 * 1024, overflow=3):
    """KOS pvr_allocate_buffers: bytes left for textures."""
    tw, th = w // 32, h // 32
    opb_total = sum(4 * s * tw * th for s in opb)

    def al(x):
        return (x + 127) // 128 * 128
    a = al(vbuf)
    a += opb_total * (1 + overflow)
    a = al(a) + 0x48
    a += 4 * (6 + 6 * tw * th)
    a = al(a)
    a += w * h * bpp
    a = al(a)
    return 0x800000 - a * 2


class Rows:
    """engine/render.c palette RAM: 64 rows of 16 entries."""

    def __init__(self):
        self.kind = [None] * 64           # None, 'pal8', 'pal4'
        self.col = [[0] * 16 for _ in range(64)]
        self.mask = [0] * 64

    def pal8(self, rowmask):
        for b in range(4):
            if all(self.kind[b * 16 + r] is None for r in range(16) if rowmask >> r & 1):
                for r in range(16):
                    if rowmask >> r & 1:
                        self.kind[b * 16 + r] = "pal8"
                return b
        return -1

    def pal4(self, col, mask):
        mask &= 0xFFFE
        best = -1
        for r in range(64):
            if self.kind[r] == "pal4" and all(not (mask & self.mask[r]) >> k & 1 or self.col[r][k] == col[k]
                                              for k in range(1, 16)):
                if best < 0 or bin(mask & self.mask[r]).count("1") > bin(mask & self.mask[best]).count("1"):
                    best = r
        if best < 0:
            for r in range(63, -1, -1):
                if self.kind[r] is None:
                    best = r
                    self.kind[r] = "pal4"
                    self.mask[r] = 0
                    break
        if best < 0:
            return -1
        for k in range(1, 16):
            if mask >> k & 1:
                self.col[best][k] = col[k]
        self.mask[best] |= mask
        return best

    def used(self):
        return sum(1 for k in self.kind if k)


def atlas_info(d):
    """pages [(w, h, pal4)], images [(page, pal)], palettes [(col16, mask)] of a sprite dir."""
    fb = open(os.path.join(d, "frames.bin"), "rb").read()
    _, npages, nimg, nanim, _, _ = struct.unpack_from("<HHHHHH", fb, 4)
    pages = [struct.unpack_from("<HH", fb, 16 + 4 * i) for i in range(npages)]
    imgs = [fb[16 + 4 * npages + 16 * i] for i in range(nimg)]
    pal4 = [False] * npages
    image_pal = [None] * nimg
    pals = []
    pp = os.path.join(d, "palettes.bin")
    if os.path.exists(pp):
        b = open(pp, "rb").read()
        _, np_, ni, na, npal, _ = struct.unpack_from("<HHHHHH", b, 4)
        o = 16 + (np_ + 3) // 4 * 4
        pal4 = [b[16 + i] == 1 and os.path.exists(os.path.join(d, "atlas%d.pt4" % i)) for i in range(np_)]
        cols = [struct.unpack_from("<16H", b, o + 32 * k) for k in range(npal)]
        masks = struct.unpack_from("<%dH" % npal, b, o + 32 * npal)
        ip = struct.unpack_from("<%dH" % ni, b, o + 34 * npal)
        pals = list(zip(cols, masks))
        image_pal = [None if v == 0xFFFF else v for v in ip]
    return [(w, h, pal4[i]) for i, (w, h) in enumerate(pages)], list(zip(imgs, image_pal)), pals


def atlas_cost(d, mode, rows=None, lazy=False):
    """VRAM bytes of one sprite dir; rows: Rows (after) for the palette simulation."""
    if not os.path.exists(os.path.join(d, "frames.bin")):
        return 0, 0
    pages, imgs, pals = atlas_info(d)
    if mode == "before":
        return sum(chunk(w * h * 2) for w, h, _ in pages), 0
    total = expanded = 0
    for pn, (w, h, p4) in enumerate(pages):
        if not p4:
            total += chunk(w * h * 2)
            continue
        ok = True
        if rows is not None:
            for pg, pid in imgs:
                if pg == pn and pid is not None and rows.pal4(*pals[pid]) < 0:
                    ok = False
        if ok:
            total += chunk(w * h // 2)
        else:   # palette RAM full: the page goes ARGB1555 (static: at load; lazy: at first draw)
            total += chunk(w * h * 2)
            expanded += 1
    return total, expanded


def dcm_tex(path):
    if not os.path.exists(path):
        return 0
    d = open(path, "rb").read()
    nt = struct.unpack_from("<H", d, 40)[0]
    ot = struct.unpack_from("<I", d, 76)[0]
    return sum(chunk(struct.unpack_from("<I", d, ot + 16 * i + 12)[0]) for i in range(nt))


def level_cost(sd, rows=None):
    bt = open(os.path.join(sd, "blocktex.bin"), "rb").read()
    _, _, _, ps, _, npg, _, po = struct.unpack_from("<IHHHHHHI", bt, 0)
    lv = npg * chunk(ps * ps)
    lvmask = 0
    for i in range(npg):
        for v in set(bt[po + i * ps * ps:po + (i + 1) * ps * ps]):
            lvmask |= 1 << (v >> 4)
    bg = 0
    bgmask = 0
    bp = os.path.join(sd, "bg.bin")
    if os.path.exists(bp):
        b = open(bp, "rb").read()
        _, _, _, _, _, tw, th, _, _, _, to = struct.unpack_from("<IHHHHHHHHII", b, 0)
        bg = chunk(tw * th)
        for v in set(b[to:to + tw * th]):
            bgmask |= 1 << (v >> 4)
    if rows is not None:
        rows.pal8(lvmask)
        if bg:
            rows.pal8(bgmask)
    return lv, bg, bin(lvmask).count("1") + bin(bgmask).count("1")


def stage_zone_act(sd):
    h = open(os.path.join(sd, "level.bin"), "rb").read(16)
    return h[9], h[10]


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--root", default=".")
    ap.add_argument("--res", default=None)
    ap.add_argument("--json", default=None, help="write the numbers as JSON too")
    a = ap.parse_args(argv)
    B = os.path.join(a.root, "build/dreamcast")
    spr = os.path.join(B, "sprites/bac")
    res = {}
    lines = None
    if a.res and os.path.exists(a.res):
        lines = open(a.res).read().splitlines()
    else:
        exe = os.path.join(B, "objects/test_coverage")
        out = subprocess.run([exe, os.path.join(B, "player"), os.path.join(B, "objects"), spr,
                              os.path.join(B, "stages")] + STAGES, env=dict(os.environ, COV_RES="1"),
                             capture_output=True, text=True).stdout
        lines = out.splitlines()
        if a.res:
            open(a.res, "w").write(out)
    for ln in lines:
        f = ln.split()
        if len(f) >= 3 and f[0] == "res" and f[2] == "preload":
            res[f[1]] = f[3:]
    tex_ram = kos_texture_ram()
    player = dcm_tex(os.path.join(B, "models/mod__son__son.dcm"))
    boss = {}
    ap_ = os.path.join(B, "boss/assets.txt")
    if os.path.exists(ap_):
        seen = {}
        for ln in open(ap_):
            f = ln.split()
            if len(f) == 2 and f[1].endswith(".dcm") and f[1] not in seen.setdefault(f[0], set()):
                seen[f[0]].add(f[1])
                boss[f[0]] = boss.get(f[0], 0) + dcm_tex(os.path.join(B, f[1]))
    rows_out = []
    for st in STAGES:
        sd = os.path.join(B, "stages", st)
        if not os.path.exists(os.path.join(sd, "level.bin")):
            continue
        zone, act = stage_zone_act(sd)
        file = (0x10 if act == 2 else 6) + zone
        if zone > 8:
            file = 0x18 if act == 2 else 0xE
        r = {"stage": st}
        for mode in ("before", "after"):
            rows = Rows() if mode == "after" else None
            lv, bg, lvrows = level_cost(sd, rows)
            obj = exp = 0
            for i in res.get(st, []):
                c, e = atlas_cost(os.path.join(spr, i), mode, rows)
                obj += c
                exp += e
            ui = 0
            for i in UI_STATIC:
                c, e = atlas_cost(os.path.join(spr, i), mode, rows if i in UI_DRAWN_AT_START else None, True)
                ui += c
                exp += e
            c, e = atlas_cost(os.path.join(B, "ui/res", "tcbg_%d" % file), mode, rows, True)
            ui += c
            exp += e
            if act == 2:   # boss: NaviB box / messages + the streamed face
                c, e = atlas_cost(os.path.join(B, "ui/res", "navi_c_eng"), mode, rows, True)
                ui += c + chunk(128 * 128 * 2)
            if mode == "before":
                ui += chunk(256 * 256 * 2)   # variant cache (HUD gauge colours)
            bs = boss.get(st, 0) if act == 2 or st == "zf3" else 0
            tot = lv + bg + obj + ui + player + bs
            r[mode] = {"level": lv + bg, "objects": obj, "ui": ui, "player": player, "boss": bs,
                       "total": tot, "free": tex_ram - tot, "expanded_pages": exp,
                       "pal_rows": rows.used() if rows else None, "level_rows": lvrows}
        rows_out.append(r)
    # Extra Zone boss (no level): its models and HUD / VS sprites (boss/extra, loaded by the
    # boss module from atlas0.argb1555), and the cutscene sprite sets (/cd/cutscene)
    extra = {}
    xa = os.path.join(B, "boss/extra/assets.txt")
    if os.path.exists(xa):
        mdl = sum(dcm_tex(os.path.join(B, f.strip())) for f in sorted(set(open(xa).read().split()))
                  if f.endswith(".dcm"))
        xs = os.path.join(B, "boss/extra/sprites")
        sp = sum(atlas_cost(os.path.join(xs, d), "before")[0] for d in sorted(os.listdir(xs)))
        ui = sum(atlas_cost(os.path.join(spr, i), "after")[0] for i in UI_STATIC)
        extra = {"models": mdl, "sprites": sp, "ui": ui, "player": player,
                 "total": mdl + sp + ui + player, "free": tex_ram - (mdl + sp + ui + player)}
    cs = {}
    cl = os.path.join(B, "cutscene/sprites.txt")
    if os.path.exists(cl):
        for ln in open(cl):
            i = ln.strip()
            if i and not i.startswith("#"):
                cs[i] = (atlas_cost(os.path.join(spr, i), "before")[0], atlas_cost(os.path.join(spr, i), "after")[0])
    if a.json:
        json.dump({"texture_ram": tex_ram, "stages": rows_out, "extra_boss": extra, "cutscene_sprites": cs},
                  open(a.json, "w"), indent=1)
    K = lambda v: "%d" % ((v + 512) // 1024)   # noqa: E731
    print("PVR texture RAM after the KOS buffers: %d bytes (%s KB)\n" % (tex_ram, K(tex_ram)))
    print("| stage | level | objects before | objects after | UI before | UI after | player | boss "
          "| total before | free before | total after | free after | palette rows after |")
    print("|---|---|---|---|---|---|---|---|---|---|---|---|---|")
    for r in rows_out:
        b, f = r["before"], r["after"]
        print("| %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %d%s |" % (
            r["stage"], K(b["level"]), K(b["objects"]), K(f["objects"]), K(b["ui"]), K(f["ui"]), K(b["player"]),
            K(b["boss"]), K(b["total"]), K(b["free"]) if b["free"] >= 0 else "**%s**" % K(b["free"]),
            K(f["total"]), K(f["free"]), f["pal_rows"],
            " (%d pages ARGB1555)" % f["expanded_pages"] if f["expanded_pages"] else ""))
    if extra:
        print("\nExtra Zone boss (after): models %s, boss/extra sprites %s (ARGB1555, boss module), UI %s, player %s"
              " = %s KB, free %s KB" % (K(extra["models"]), K(extra["sprites"]), K(extra["ui"]), K(extra["player"]),
                                       K(extra["total"]), K(extra["free"])))
    if cs:
        print("Cutscene sprite sets (/cd/cutscene, all of them): before %s KB, after %s KB; largest: %s"
              % (K(sum(v[0] for v in cs.values())), K(sum(v[1] for v in cs.values())),
                 ", ".join("%s %s->%s" % (k, K(v[0]), K(v[1])) for k, v in
                           sorted(cs.items(), key=lambda t: -t[1][0])[:3])))
    return 0


if __name__ == "__main__":
    sys.exit(main())

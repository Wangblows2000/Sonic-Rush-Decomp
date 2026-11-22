#!/usr/bin/env python3
"""Object-module data for the native Dreamcast port (bring-your-own-ROM).

Reads the owner's extracted ROM (extract/eur) and the sprite converter output
(build/dreamcast/sprites, tools/dreamcast/sprites/bac2dc.py) and writes
build/dreamcast/objects/:

  objdata.bin   "ROBJ" v1: small ROM tables the object code reads at runtime
                (copied from extract/eur/arm9_overlays/ov000.bin), plus the
                resolved sprite id of every object BAC slot for every stage.
  hbx/<id>.hbx  "HBOX" v1: the BAC collision-box commands (command 7,
                Player_SpriteCallback / MapObject_SpriteCallback_Default) of
                every frame, in frames.bin frame order.  frames.bin (DSPR v1)
                has no events, so they come from frames.json.
  sprites.txt   which sprite directories the disc needs (per stage + union).

Nothing is compiled into C sources; see docs/dreamcast/translation/OBJECTS.md.

Usage: objdata.py [--extract extract/eur] [--sprites build/dreamcast/sprites]
                  [--out build/dreamcast/objects]
"""
import argparse
import json
import os
import struct
import sys

OV_BASE = 0x022C5C80

# ROM tables (ov000 addresses, symbol names from config/eur/arm9/overlays/ov000/symbols.txt)
TABLES = [
    # name, address, size
    ("margin", 0x023686E0, 0xF2),        # data_ov000_023686e0: per object type spawn/despawn margin
    ("ring_gravity", 0x023690E4, 2),     # data_ov000_023690e4: s16 ring gravity
    ("itembox_rings", 0x02366B44, 8),    # data_ov000_02366b44: random ring item amounts
    ("itembox_vs", 0x02366B3C, 8),       # data_ov000_02366b3c: VS item cycle
    ("player_spr_anim", 0x02368D14, 0x68),  # data_ov000_02368d14: Player_SetAnimation sprite anim map
    ("needle_shape", 0x023661C0, 12 * 8),   # data_ov000_023661c0: needle collision tiles (w*h)[8]
    ("animal", 0x023661A8, 24),          # data_ov000_023661a8: freed animal kinds per zone [8][3]
    ("shake", 0x02077944, 16),           # data_02077944 (arm9): StageObj_Main hit-stop shake offsets (s8)
]
ARM9_BASE = 0x02000000

# Object BAC slots. Order == enum ObjBac in obj_types.h.
# (archive, name inside archive, loose fallback) -- archive "zone" = z??_act_lz7.narc of the stage
# (data_ov000_02369ba0), "act_com" = act_com_lz7.narc (gActComArchive), "player" = player_lz7.narc.
SLOTS = [
    ("RING", "act_com", "ac_itm_ring.bac", None),                 # data_ov000_02369100 "/ac_itm_ring.bac"
    ("SPRING", "zone", "act/ac_gmk_spring.bac", "act/ac_gmk_spring.bac"),
    ("DASH_P", "zone", "act/ac_gmk_dash_p.bac", "act/ac_gmk_dash_p.bac"),
    ("DASH_C", "zone", "act/ac_gmk_dash_c.bac", "act/ac_gmk_dash_c.bac"),
    ("DASH_CT", "zone", "act/ac_gmk_dash_ct.bac", "act/ac_gmk_dash_ct.bac"),
    ("ITEMBOX", "zone", "act/ac_itm_box.bac", "act/ac_itm_box.bac"),
    ("NEEDLE", "zone", "act/ac_gmk_needle.bac", "act/ac_gmk_needle.bac"),
    ("GOALRING", "zone", "act/ac_gmk_goalring.bac", "act/ac_gmk_goalring.bac"),
    ("MARKER", "zone", "act/ac_gmk_marker.bac", "act/ac_gmk_marker.bac"),
    ("EGGPAWN", "zone", "act/ac_ene_eggpawn.bac", "act/ac_ene_eggpawn.bac"),
    ("EGGPAWN_M", "zone", "act/ac_ene_eggpawn_m.bac", "act/ac_ene_eggpawn_m.bac"),
    ("ZAKO_BOMB", "act_com", "act/ac_eff_zako_bomb.bac", "act/ac_eff_zako_bomb.bac"),
    ("HAHEN_ZAKO", "act_com", "act/ac_eff_hahen_zako.bac", "act/ac_eff_hahen_zako.bac"),
    ("PLAYER", "player", "act/son.bac", None),                    # hitboxes only (data-only BAC)
]
ID_LEN = 48
N_STAGES = 27


def stage_name(idx):
    if idx == 23:
        return "zf3"
    if idx == 26:
        return None
    return "z%d%d" % (idx // 3 + 1, idx % 3 + 1)


def resolve(manifest, stage, slot):
    _, arc, name, loose = slot
    if arc == "zone":
        arc_file = "narc/%s_act_lz7.narc" % stage
    elif arc == "act_com":
        arc_file = "narc/act_com_lz7.narc"
    else:
        arc_file = "narc/player_lz7.narc"
    key = "%s:%s" % (arc_file, name)
    if key in manifest:
        return manifest[key]
    if loose and loose in manifest:
        return manifest[loose]
    return None


def write_hbx(fj, out_path, dspr):
    anims = fj["animations"]
    frames = []
    events = []
    anim_tab = []
    for a in anims:
        anim_tab.append((len(frames), len(a["frames"])))
        for f in a["frames"]:
            evs = [e for e in f.get("events", []) if e.get("cmd") == 7]
            frames.append((len(events), len(evs)))
            for e in evs:
                l, t, r, b = e["rect"]
                events.append((e["slot"], e.get("flags", 0), l, t, r, b))
    # consistency with frames.bin (same animation / frame order)
    if dspr is not None:
        n_an, n_fr = dspr
        if n_an != len(anims) or n_fr != len(frames):
            raise SystemExit("%s: frames.json (%d anims, %d frames) != frames.bin (%d, %d)"
                             % (out_path, len(anims), len(frames), n_an, n_fr))
    b = bytearray(b"HBOX")
    b += struct.pack("<HHHHI", 1, len(anims), len(frames), len(events), 0)
    for ff, n in anim_tab:
        b += struct.pack("<HH", ff, n)
    for fe, n in frames:
        b += struct.pack("<HH", fe, n)
    for slot, fl, l, t, r, bb in events:
        b += struct.pack("<BBHhhhh", slot, 0, fl & 0xFFFF, l, t, r, bb)
    with open(out_path, "wb") as f:
        f.write(b)
    return len(anims), len(frames), len(events), len(b)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--extract", default="extract/eur")
    ap.add_argument("--sprites", default="build/dreamcast/sprites")
    ap.add_argument("--out", default="build/dreamcast/objects")
    a = ap.parse_args()

    ov = open(os.path.join(a.extract, "arm9_overlays/ov000.bin"), "rb").read()
    arm9 = open(os.path.join(a.extract, "arm9/arm9.bin"), "rb").read()
    man_list = json.load(open(os.path.join(a.sprites, "manifest.json")))
    manifest = {e["source"]: e["converted_as"] for e in man_list if e.get("status") == "ok"}

    os.makedirs(os.path.join(a.out, "hbx"), exist_ok=True)
    blob = bytearray(b"ROBJ")
    blob += struct.pack("<HHHH", 1, len(SLOTS), N_STAGES, ID_LEN)
    for name, addr, size in TABLES:
        if addr < OV_BASE:
            data = arm9[addr - ARM9_BASE:addr - ARM9_BASE + size]
        else:
            data = ov[addr - OV_BASE:addr - OV_BASE + size]
        if len(data) != size:
            raise SystemExit("table %s out of range" % name)
        blob += data
        while len(blob) % 4:
            blob += b"\0"
    needed = {}
    per_stage = {}
    missing = 0
    for s in range(N_STAGES):
        st = stage_name(s)
        ids = []
        for slot in SLOTS:
            sid = resolve(manifest, st, slot) if st else None
            ids.append(sid)
            if sid:
                needed.setdefault(sid, set()).add(st)
            elif st:
                missing += 1
            enc = (sid or "").encode()
            if len(enc) >= ID_LEN:
                raise SystemExit("id too long: %s" % sid)
            blob += enc + b"\0" * (ID_LEN - len(enc))
        per_stage[st] = ids
    with open(os.path.join(a.out, "objdata.bin"), "wb") as f:
        f.write(blob)

    report = []
    for sid in sorted(needed):
        d = os.path.join(a.sprites, "bac", sid)
        fj = json.load(open(os.path.join(d, "frames.json")))
        fb = open(os.path.join(d, "frames.bin"), "rb").read()
        hdr = struct.unpack_from("<HHHHHH", fb, 4)
        dspr = (hdr[3], hdr[4])
        r = write_hbx(fj, os.path.join(a.out, "hbx", sid + ".hbx"), dspr)
        report.append("%-40s anims %3d frames %4d boxes %4d (%d bytes)" % ((sid,) + r))

    with open(os.path.join(a.out, "sprites.txt"), "w") as f:
        f.write("# sprite directories (build/dreamcast/sprites/bac/<id>/) used by the object module\n")
        f.write("# disc: /cd/sprites/<id>/{frames.bin,atlas0.argb1555}  /cd/objects/{objdata.bin,hbx/<id>.hbx}\n")
        for sid in sorted(needed):
            f.write("%s\t%s\n" % (sid, " ".join(sorted(needed[sid]))))
    print("objdata.bin: %d bytes, %d slots x %d stages, %d unresolved (stage, slot) pairs"
          % (len(blob), len(SLOTS), N_STAGES, missing))
    for line in report:
        print("hbx " + line)
    return 0


if __name__ == "__main__":
    sys.exit(main())

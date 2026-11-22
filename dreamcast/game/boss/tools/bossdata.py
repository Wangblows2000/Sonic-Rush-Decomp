#!/usr/bin/env python3
"""Boss-module data for the native Dreamcast port (bring-your-own-ROM).

Reads the owner's extracted ROM (extract/eur) and writes build/dreamcast/boss/:

  bossdata.bin  "BOSD" v1: the small ROM tables the translated boss code reads
                at runtime, copied byte for byte from arm9.bin / ov000.bin and
                keyed by their original DS address, so the C code can keep
                the original symbol (data_ov000_02362d64 -> BOSS_DATA(0x02362d64)).
  assets.txt    the model / animation packages (build/dreamcast/models) each
                boss stage needs on the disc.

Nothing from the ROM is compiled into C sources; see
docs/dreamcast/translation/BOSSES.md.

File layout (little endian):
  u32 magic 'BOSD', u16 version 1, u16 chunk count
  chunk table: { u32 ds_addr, u32 size, u32 file_offset } * count
  chunk data, each placed at a file offset with the same (addr & 3) as the DS
  address so 16/32-bit tables stay naturally aligned.
  then (4-aligned) u32 joint count and { char model[16], char node[16], u32 index }
  for the nodes the boss code looks up by name (NNS_G3dGetResDictIdxByName),
  read from the NSBMD node dictionaries (tools/dreamcast/models/g3d.py).
  then u32 effect count and { u8 stage, u8 pad[3], char name[24], char model[96],
  char anim[96] }: for every boss stage (act 3, zone archive z<N>3_act_lz7.narc =
  gZoneActArchive) the converted package of each BossEffect_Create / EffPebble
  name that the archive holds (duplicates resolved through models/manifest.json).

Usage: bossdata.py [--extract extract/eur] [--out build/dreamcast/boss]
"""
import argparse
import os
import struct
import sys

ARM9_BASE = 0x02000000
OV000_BASE = 0x022C5C80

# (name, address, size). Addresses/types from config/eur/arm9/**/symbols.txt and the
# extern declarations in src/game/Boss/Boss1/*.c.
CHUNKS = [
    # Boss1 (Egg Hammer): data_ov000_02362b58 .. data_ov000_02362dc4 (one block of tables:
    # palette slots/indices, missile tables, hammer-roll counts [2][4] s8, node names
    # "hammer"/"neck_ham"/"base_battery01/02", attack patterns [4][8] u8, roll counts [2][8] s16,
    # impact anim speeds [2][8] s16, slam frame thresholds [2][8] u16, slam/big-slam speeds [2][8] s16)
    ("boss1_tables", 0x02362B58, 0x02362DC4 - 0x02362B58),
    # ScreenShake_Main tables data_0207323c (4 x s8), data_02073240 (12 x s8), data_0207324c (22 x s8)
    ("screenshake", 0x0207323C, 4 + 12 + 22),
]

# sanity checks against known contents (decoded once from the ROM, not shipped)
CHECKS = [
    (0x02362BB4, b"hammer\0"),
    (0x02362C54, b"neck_ham\0"),
    (0x02362BC4, b"base_battery01\0"),
    (0x02362BE4, b"base_battery02\0"),
]

# (model file under files/mod, node name): Boss1Neck_NodeCallback_NeckHam, Boss1Hammer_NodeCallback_Hammer,
# Boss1_NodeCallback_Batteries
JOINTS = [
    ("bs1_neck", "neck_ham"),
    ("bs1_hammer", "hammer"),
    ("bs1_base", "base_battery01"),
    ("bs1_base", "base_battery02"),
]

# BossEffect_Create<Name> resource names (data_ov000_02367c5c..02367e24) + EffPebble (02367c2c)
EFFECTS = ["eff_base_dest", "eff_beam_fin_b", "eff_blast", "eff_blast1", "eff_blast3", "eff_blast4", "eff_blast5",
           "eff_blastf", "eff_bomb_ball", "eff_boost", "eff_catch", "eff_clash", "eff_dmg_hand", "eff_hit",
           "eff_homing", "eff_impact_big", "eff_impact_pr", "eff_impact_sl", "eff_impact_small", "eff_impact_st",
           "eff_jump", "eff_land", "eff_laser_fin_b", "eff_launch", "eff_parts", "eff_pick", "eff_pop", "eff_produce",
           "eff_shoot_bomb", "eff_strike_big", "eff_strike_fin", "eff_strike_hit", "eff_suction", "eff_suction2",
           "eff_tackle", "eff_pebble"]
BOSS_ZONES = [1, 2, 3, 4, 5, 6, 7]   # z13 .. z73 (zone index 0..6), stage = zone_index * 3 + 2


def effect_table(models_dir):
    import json
    man = json.load(open(os.path.join(models_dir, "manifest.json")))
    by_src = {e["source"]: e for e in man["entries"]}

    def resolve(src):
        e = by_src.get(src)
        while e is not None and "duplicate_of" in e:
            e = by_src.get(e["duplicate_of"])
        return e

    out = []
    for z in BOSS_ZONES:
        arc = "narc/z%d3_act_lz7.narc" % z
        for name in EFFECTS:
            m = resolve("%s:%s.nsbmd" % (arc, name))
            if m is None or not m.get("outputs"):
                continue
            a = resolve("%s:%s.nsbca" % (arc, name))
            out.append(((z - 1) * 3 + 2, name, m["outputs"][0], a["outputs"][0] if a and a.get("outputs") else ""))
    return out


ASSETS = {
    # stage id -> packages under build/dreamcast/models (BossStage1_OnModelLoaded data_02074308,
    # Boss1_Create "bs1" archive file 8, BossEffect_Create / EffPebble_Create names)
    "z13": [
        "mod__bs1_base__bs1_base.dcm",
        "mod__bs1_neck__bs1_neck.dcm",
        "mod__bs1_hammer__bs1_hammer.dcm",
        "mod__bs1_sky__bs1_sky.dcm",
        "narc__z13_act_lz7__bs1.dca",
        "narc__z13_act_lz7__eff_hit__eff_hit.dcm",
        "narc__z13_act_lz7__eff_hit.dca",
        "narc__z13_act_lz7__eff_impact_small__eff_impact_small.dcm",
        "narc__z13_act_lz7__eff_impact_small.dca",
        "narc__z13_act_lz7__eff_impact_big__eff_impact_big.dcm",
        "narc__z13_act_lz7__eff_impact_big.dca",
        "narc__z13_act_lz7__eff_blast1__eff_blast1.dcm",
        "narc__z13_act_lz7__eff_blast1.dca",
        "narc__z13_act_lz7__eff_pebble__eff_pebble01.dcm",
    ],
}


def read_range(arm9, ov000, addr, size):
    if OV000_BASE <= addr < OV000_BASE + len(ov000):
        o = addr - OV000_BASE
        return ov000[o:o + size]
    if ARM9_BASE <= addr < ARM9_BASE + len(arm9):
        o = addr - ARM9_BASE
        return arm9[o:o + size]
    raise ValueError("address %08x not in arm9/ov000" % addr)


def main(argv):
    ap = argparse.ArgumentParser()
    ap.add_argument("--extract", default="extract/eur")
    ap.add_argument("--out", default="build/dreamcast/boss")
    ap.add_argument("--models", default="build/dreamcast/models")
    a = ap.parse_args(argv)
    arm9 = open(os.path.join(a.extract, "arm9", "arm9.bin"), "rb").read()
    ov000 = open(os.path.join(a.extract, "arm9_overlays", "ov000.bin"), "rb").read()
    for addr, want in CHECKS:
        got = read_range(arm9, ov000, addr, len(want))
        if got != want:
            print("ROM check failed at %08x: %r != %r (wrong ROM region?)" % (addr, got, want))
            return 1
    hdr_size = 8 + 12 * len(CHUNKS)
    table = []
    data = bytearray()
    off = hdr_size
    for name, addr, size in CHUNKS:
        while (off + len(data)) % 4 != addr % 4:
            data.append(0)
        foff = off + len(data)
        blob = read_range(arm9, ov000, addr, size)
        if len(blob) != size:
            print("short read %s" % name)
            return 1
        data += blob
        table.append((addr, size, foff))
    out = struct.pack("<4sHH", b"BOSD", 1, len(CHUNKS))
    for t in table:
        out += struct.pack("<III", *t)
    out += bytes(data)
    sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../../../tools/dreamcast/models"))
    sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../../../tools/dreamcast/sprites"))
    import g3d
    joints = []
    for model, node in JOINTS:
        res = g3d.parse_bmd(open(os.path.join(a.extract, "files", "mod", model + ".nsbmd"), "rb").read())
        names = [n["name"] for n in res["models"][0]["nodes"]]
        idx = names.index(node) if node in names else 0xFFFFFFFF  # bs1_base has no batteries (zone 5 only)
        joints.append((model, node, idx))
    while len(out) % 4:
        out += b"\0"
    out += struct.pack("<I", len(joints))
    for model, node, idx in joints:
        out += struct.pack("<16s16sI", model.encode(), node.encode(), idx)
    effects = effect_table(a.models)
    out += struct.pack("<I", len(effects))
    for stage, name, mfile, afile in effects:
        out += struct.pack("<B3x24s96s96s", stage, name.encode(), mfile.encode(), afile.encode())
    print("effects: %d (stage, name) packages, %d on z13" % (len(effects), sum(1 for e in effects if e[0] == 2)))
    os.makedirs(a.out, exist_ok=True)
    with open(os.path.join(a.out, "bossdata.bin"), "wb") as f:
        f.write(out)
    missing = 0
    with open(os.path.join(a.out, "assets.txt"), "w") as f:
        for stage, files in ASSETS.items():
            for fn in files:
                ok = os.path.exists(os.path.join(a.models, fn))
                missing += not ok
                f.write("%s models/%s%s\n" % (stage, fn, "" if ok else "   # MISSING (run nsbmd2dc.py batch)"))
    print("bossdata.bin: %d bytes, %d chunks; assets.txt: %d files, %d missing" %
          (len(out), len(CHUNKS), sum(len(v) for v in ASSETS.values()), missing))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))

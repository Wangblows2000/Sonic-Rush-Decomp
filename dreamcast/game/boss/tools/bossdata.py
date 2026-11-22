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
    # Boss2 (Egg Turtle) read-only tables data_ov000_02362dc4 .. data_ov000_0236306c: palette slots, light
    # colours/vectors (02362df4 / 02362ef8), hitbox index lists, voice ids, palette tables per player
    # (02362e1c .. 02362e90), idle/dash timers (02362e40 / 02362e50), node names "body0" "body_neck" "prop"
    # "neck_head" "head_01", defeat camera tables (02362ed0 / 02362ee4 / 02362f0c), hitbox kinds (02362f30),
    # attack patterns [2][16] (02362f58 / 02362f98), gBoss2ModelColorsDefault (02362fd8), hitbox rects (0236301c)
    ("boss2_rodata", 0x02362DC4, 0x0236306C - 0x02362DC4),
    # Boss2 data data_ov000_023650fc .. 02365414: palette-name pointer tables, node-name pointers
    # (02365100 / 02365104 / 0236511c), palette names "boss02_0x_pl", strike speeds (023651d8), defeat player
    # positions [3][3] (023651f8), hitbox callbacks (0236521c, ROM function addresses), gBoss2ModelColorsAlt
    # (02365244), "bs2" archive / resource names
    ("boss2_data", 0x023650FC, 0x02365414 - 0x023650FC),
    # gEffSplash3DModelNames / gEffRipple3DModelNames (+ their strings)
    ("eff3d_names", 0x02367B38, 0x02367B9C - 0x02367B38),
    # zone light vectors data_020748bc [zone][3] VecFx16 and colours data_02074960 [zone][act][3] GXRgb
    ("zone_lights", 0x020748BC, 0x02074A04 - 0x020748BC),
    # Boss3 read-only tables data_ov000_0236306c .. 02363504: ball palette slots / indices (0236306c..74),
    # catch thresholds (02363078), wait-after-kick (02363080), push levels per health band (02363088/90/98),
    # throw counts (023630a0), body palette slots (023630ac), kick speeds (023630b8), wait masks (023630c4),
    # throw levels (023630d0), "body_ball" (023630e0), kick levels (023630f0/02363100/02363120), throw levels
    # (02363110), ball cycle periods (02363130), body palettes (02363148/60), attack patterns [4][8]
    # (02363178/98/b8), push durations [2][9] (023631d8), push max speeds (023631fc), aim times [9][5]
    # (02363220), throw speeds [9][5] (0236327c), throw vertical speeds [9][5] (02363330),
    # push harmful/hittable patterns [2][9][16] (023633e4)
    ("boss3_rodata", 0x0236306C, 0x02363504 - 0x0236306C),
    # Boss3 data data_ov000_02365414 .. 023655e8: palette name pointer tables (02365414 / 02365488) and the
    # "boss03_0x_pl" names, "exc"/"bs3" archive names, effect resource names
    ("boss3_data", 0x02365414, 0x023655E8 - 0x02365414),
    # Boss4 read-only tables data_ov000_02363504 .. 023636e8: palette slots / indices per player, voices
    # (02363538), attack / electric masks per health (02363548 / 02363550), electric on-times (02363570),
    # attack timers (02363580), node names "rod_arm" "body_exit" "rod_body" "arm_gun", shot counts / intervals
    # (023635a0 / 023635c0), spin scales (02363668, floats), attack patterns [8][4] (02363688),
    # electric off-times [2][8][2] (023636a8)
    ("boss4_rodata", 0x02363504, 0x023636E8 - 0x02363504),
    # Boss4 data data_ov000_023655e8 .. 023657e0: palette name pointer tables and the "boss04_xx_pl" names,
    # "exc"/"bs4" archive and effect resource names
    ("boss4_data", 0x023655E8, 0x023657E0 - 0x023655E8),
    # Boss6 read-only tables data_ov000_023636e8 .. 02363b18: palette slots / indices per player, voices (02363718),
    # hit box index lists (02363720 / 02363750 / 023637dc), defeat camera tables (0236372c / 02363744 / 023637ec), cut-in
    # BBG file indices (02363738), plate spawn frames (0236375c), node names "body_pro_r" "pelvis" "body_core"
    # "body_pro_l", walk times (023637ac), aim speeds (023637bc), aim times / edge jump counts / jump track times per
    # health (02363804 / 02363824 / 02363844), piece files [4][4] (02363864), body palettes per player (02363884 /
    # 023638a4), the RotY(180) mirror MtxFx33 (023638c4), plate positions (023638e8), attack patterns [4][8] per health
    # band (02363918 / 02363998 / 023639d8), landing anim speeds [2][8] (02363958), pull-out speeds [2][8][4] (02363a18)
    ("boss6_rodata", 0x023636E8, 0x02363B18 - 0x023636E8),
    # Boss6 data data_ov000_023657e0 .. 023659c4: palette name pointer tables and the "boss06_xx_pl" / "bult00_pl"
    # names, piece states (023658ac, ROM function addresses), node name pointers (023658bc), defeat player positions
    # (023658cc), "bs6" archive and effect resource names
    ("boss6_data", 0x023657E0, 0x023659C4 - 0x023657E0),
    # Boss7 read-only tables data_ov000_02363b18 .. 02363da4: clash mash base per level (02363b18), special /
    # air-attack strength picks per health band (02363b1c / b2c / b34 / b4c / b54 / b5c), pillar counts / intervals /
    # lifetimes (02363b24 / b44 / b6c), orbit counts (02363b3c), flame anim speeds (02363b64), air dash speeds
    # (02363b74), special chances [2][4] (02363b84 / b94), reposition / rolling / jump-attack checks [2][8] (02363ba4 /
    # bc4 / be4), tornado lifetimes / pull limits / pull accelerations [4][4] (02363c04 / c24 / c64), attack interval
    # [2][8] (02363c44), attack tables [4][8] x 9 (02363c84 .. 02363d84)
    ("boss7_rodata", 0x02363B18, 0x02363DA4 - 0x02363B18),
    # gPlayerPhysicsTable (0x02368b7c, Sonic / Blaze, the rival reads top / dec / sd_max / jump / gravity / fall_max)
    # and the Player_SetAnimation tables 02368bdc (joint anim), 02368c44 (model), 02368cac (visibility anim),
    # 02368d14 (son.bac anim), 0x68 entries each
    ("player_tables", 0x02368B7C, 0x02368D7C - 0x02368B7C),
    # BossF (zf3) read-only tables data_ov000_02363da4 .. 023643d4: attack variants / patterns per health band
    # (02363da4 .. 02363e40, 02363eac .. 02363eec), palette slots / indices per player (02363dac, 02363dd0 / ddc,
    # 02363de8 / df8, 02363e6c .. e90, 02363f60, 023640b8 / 023640ec), timers per level and health (02363f7c ..
    # 02364344), wave / ball speeds, beam widths, node names "body_core" "body_sh_l" "body_sh_r" "body_fist_l"
    # "body_fist_r" (02363efc .. 02363f3c)
    ("bossf_rodata", 0x02363DA4, 0x023643D4 - 0x02363DA4),
    # BossF data data_ov000_02365c54 .. 023661a4: laser chase timers (02365c54), volley counts / widths / speeds /
    # intervals [2][3][8] (02365c5c, 02365da0 .. 02365e60), palette name tables, "/mod/bsf_sky_b.nsbmd",
    # "bsf" / "exc" archive and effect resource names
    ("bossf_data", 0x02365C54, 0x023661A4 - 0x02365C54),
]

# zone archive files the boss code reads as data, stored like the ROM tables at pseudo addresses outside the DS
# address space: (pseudo address, archive, member)
ARCHIVE_CHUNKS = [
    # BossFPartD8_Create: "bsf" files 12 / 13, the arm's ObjCollisionMap (0x56 x 0x28 tiles of 8 x u8 height /
    # width codes, and the tile angles)
    (0x7F000000, "narc/zf3_act_lz7.narc", "bsf_st_r.df"),
    (0x7F010000, "narc/zf3_act_lz7.narc", "bsf_st_r.di"),
]

# sanity checks against known contents (decoded once from the ROM, not shipped)
CHECKS = [
    (0x02362BB4, b"hammer\0"),
    (0x02362C54, b"neck_ham\0"),
    (0x02362BC4, b"base_battery01\0"),
    (0x02362BE4, b"base_battery02\0"),
    (0x02362E60, b"body0\0"),
    (0x02362E80, b"body_neck\0"),
    (0x02362EA0, b"prop\0"),
    (0x02362EB0, b"neck_head\0"),
    (0x02362EC0, b"head_01\0"),
    (0x02365128, b"boss02_00_pl\0"),
    (0x02367B6C, b"eff_ripple_s\0"),
    (0x023630E0, b"body_ball\0"),
    (0x02365418, b"boss03_04_pl\0"),
    (0x02365544, b"bs3\0bs3\0bs3\0exc\0"),
    (0x02363590, b"rod_arm\0"),
    (0x023635B0, b"body_exit\0"),
    (0x023635F0, b"rod_body\0"),
    (0x02363610, b"arm_gun\0"),
    (0x02365600, b"boss04_09_pl\0"),
    # Boss2 hitbox callback table data_ov000_0236521c: NULL x4, Boss2_OnHit_DamageAttack2 (022cdbb0),
    # Boss2_OnHit_DamageAttack3 (022cd880), Boss2_OnHit_PushPlayer (022cd738) x4
    (0x0236521C, bytes(16) + bytes.fromhex("b0db2c0280d82c0238d72c0238d72c0238d72c0238d72c02")),
    (0x0236376C, b"body_pro_r\0"),
    (0x0236378C, b"pelvis\0"),
    (0x0236379C, b"body_core\0"),
    (0x023637CC, b"body_pro_l\0"),
    (0x02365904, b"bs6\0"),
    # Boss6 piece states data_ov000_023658ac: Boss6_State_PieceWaitSpinFly (022dfcbc), Boss6_State_PieceStartSlide
    # (022dfb68), Boss6_State_PieceDestroyOnAnimEnd (022dfd30) x2; the mirror matrix data_ov000_023638c4
    (0x023658AC, bytes.fromhex("bcfc2d0268fb2d0230fd2d0230fd2d02")),
    (0x023638C4, bytes.fromhex("00f0ffff0000000000000000000000000010000000000000000000000000000000f0ffff")),
    # Boss7: the clash mash bases {0x78, 0xa0}, "bs7" archive and the rival's sprite name
    (0x02363B18, bytes.fromhex("7800a000")),
    (0x02365B20, b"/act/son.bac"),
    # BossF: BossFPartD8_NodeCallback_Body node names, BossFPartD8_State_LoadSkyModel file
    (0x02363EFC, b"body_core\0"),
    (0x02363F0C, b"body_sh_l\0"),
    (0x02363F1C, b"body_sh_r\0"),
    (0x02363F2C, b"body_fist_l\0"),
    (0x02363F3C, b"body_fist_r\0"),
    (0x02365F20, b"/mod/bsf_sky_b.nsbmd\0"),
]

# (model file under files/mod, node name): Boss1Neck_NodeCallback_NeckHam, Boss1Hammer_NodeCallback_Hammer,
# Boss1_NodeCallback_Batteries
JOINTS = [
    ("bs1_neck", "neck_ham"),
    ("bs1_hammer", "hammer"),
    ("bs1_base", "base_battery01"),
    ("bs1_base", "base_battery02"),
    # Boss2_NodeCallback_Body / _NeckHead / _Head (Boss2_StoreNodeMatrix)
    ("bs2_body", "body0"),
    ("bs2_body", "body_neck"),
    ("bs2_body", "prop"),
    ("bs2_neck", "neck_head"),
    ("bs2_head", "head_01"),
    # Boss3PartCC_NodeCallback_BodyBall (data_ov000_023630e0)
    ("bs3_body", "body_ball"),
    # Boss1_NodeCallback_Batteries on the zone-5 base
    ("bs5_base", "base_battery01"),
    ("bs5_base", "base_battery02"),
    # Boss4_NodeCallback_Rods / _ArmGun / _BodyExit
    ("bs4_rod", "rod_arm"),
    ("bs4_rod", "rod_body"),
    ("bs4_arm", "arm_gun"),
    ("bs4_body", "body_exit"),
    # Boss6_StoreNodeMatrix (data_ov000_023658bc)
    ("bs6_body", "body_pro_r"),
    ("bs6_body", "body_pro_l"),
    ("bs6_body", "pelvis"),
    ("bs6_body", "body_core"),
    # BossFPartD8_NodeCallback_Body (data_ov000_02363efc .. 02363f3c)
    ("bsf_body", "body_core"),
    ("bsf_body", "body_sh_l"),
    ("bsf_body", "body_sh_r"),
    ("bsf_body", "body_fist_l"),
    ("bsf_body", "body_fist_r"),
]

# animation files that are not converted but whose frame counts the boss code
# needs (timing-only animation objects, see Model_SetAnimation in boss_model.c):
# (archive, member) -> written as name "<member>"
ANIM_LENGTHS = [
    ("narc/z23_act_lz7.narc", "bs2.nsbta"),   # Boss2 file 9 (water / shower SRT animations, slot 3)
    ("narc/z33_act_lz7.narc", "bs3.nsbta"),   # Boss3 file 9 (arena SRT animation, slot 3)
    ("narc/z53_act_lz7.narc", "eff_bomb_up.nsbma"),   # Boss5PartD3 explosion (slots 1 / 3 / 2)
    ("narc/z53_act_lz7.narc", "eff_bomb_up.nsbta"),
    ("narc/z53_act_lz7.narc", "eff_bomb_up.nsbtp"),
    ("narc/z43_act_lz7.narc", "bs4.nsbma"),   # Boss4 file 9 (electric arm material animation, slot 1)
    ("narc/z43_act_lz7.narc", "bs4.nsbtp"),   # Boss4 file 10 (base pattern animation, slot 2)
    ("narc/z43_act_lz7.narc", "eff_ele_stay.nsbma"),    # Boss4PartD0 (slot 1: the shot ends with it)
    ("narc/z43_act_lz7.narc", "eff_ele_flash.nsbma"),   # Boss4PartD0 flash (slot 1)
    ("narc/z43_act_lz7.narc", "eff_wave_disap.nsbma"),  # Boss4PartD2 vanish (slot 1)
    ("narc/z63_act_lz7.narc", "eff_plate.nsbma"),       # Boss6 piece 0 (slot 1)
    ("narc/z63_act_lz7.narc", "eff_shoot.nsbma"),       # Boss6 piece 2 (slot 1: the flash ends with it)
    ("narc/z63_act_lz7.narc", "eff_shoot_bomb.nsbma"),  # Boss6 piece 3 (slots 1 / 2)
    ("narc/z63_act_lz7.narc", "eff_shoot_bomb.nsbtp"),
    # Boss7PartD6 (tornado / fire pillar: slots 1 material, 2 pattern; appear 01, loop 02, vanish 03) and
    # Boss7PartD7 (clash aura: slot 1 material next to the converted 01/02/03 .nsbca)
    ("narc/z73_act_lz7.narc", "eff_tornado01.nsbma"),
    ("narc/z73_act_lz7.narc", "eff_tornado02.nsbma"),
    ("narc/z73_act_lz7.narc", "eff_tornado03.nsbma"),
    ("narc/z73_act_lz7.narc", "eff_tornado01.nsbtp"),
    ("narc/z73_act_lz7.narc", "eff_tornado02.nsbtp"),
    ("narc/z73_act_lz7.narc", "eff_tornado03.nsbtp"),
    ("narc/z73_act_lz7.narc", "eff_flame01.nsbma"),
    ("narc/z73_act_lz7.narc", "eff_flame02.nsbma"),
    ("narc/z73_act_lz7.narc", "eff_flame03.nsbma"),
    ("narc/z73_act_lz7.narc", "eff_flame01.nsbtp"),
    ("narc/z73_act_lz7.narc", "eff_flame02.nsbtp"),
    ("narc/z73_act_lz7.narc", "eff_flame03.nsbtp"),
    ("narc/z73_act_lz7.narc", "eff_aura_son01.nsbma"),
    ("narc/z73_act_lz7.narc", "eff_aura_son02.nsbma"),
    ("narc/z73_act_lz7.narc", "eff_aura_son03.nsbma"),
    ("narc/z73_act_lz7.narc", "eff_aura_blz01.nsbma"),
    ("narc/z73_act_lz7.narc", "eff_aura_blz02.nsbma"),
    ("narc/z73_act_lz7.narc", "eff_aura_blz03.nsbma"),
    # BossF (zf3): the sky's material / SRT animations (bsf files 11 / 10), BossFPartDD and the laser-fin effect
    # (eff_laser_fin_b: slot 1 material, slot 2 pattern; DD vanishes when both ended), BossFPartDE beam ends
    # (eff_beam_fin.nsbtp, looping)
    ("narc/zf3_act_lz7.narc", "bsf.nsbta"),
    ("narc/zf3_act_lz7.narc", "bsf.nsbma"),
    ("narc/zf3_act_lz7.narc", "eff_laser_fin_b.nsbma"),
    ("narc/zf3_act_lz7.narc", "eff_laser_fin_b.nsbtp"),
    ("narc/zf3_act_lz7.narc", "eff_beam_fin.nsbtp"),
]

# BossEffect_Create<Name> resource names (data_ov000_02367c5c..02367e24) + EffPebble (02367c2c)
EFFECTS = ["eff_base_dest", "eff_beam_fin_b", "eff_blast", "eff_blast1", "eff_blast3", "eff_blast4", "eff_blast5",
           "eff_blastf", "eff_bomb_ball", "eff_boost", "eff_catch", "eff_clash", "eff_dmg_hand", "eff_hit",
           "eff_homing", "eff_impact_big", "eff_impact_pr", "eff_impact_sl", "eff_impact_small", "eff_impact_st",
           "eff_jump", "eff_land", "eff_laser_fin_b", "eff_launch", "eff_parts", "eff_pick", "eff_pop", "eff_produce",
           "eff_shoot_bomb", "eff_strike_big", "eff_strike_fin", "eff_strike_hit", "eff_suction", "eff_suction2",
           "eff_tackle", "eff_pebble",
           # Water Palace 3D water effects (gEffSplash3DModelNames / gEffRipple3DModelNames)
           "eff_splash_s", "eff_splash_b", "eff_ripple_s", "eff_ripple_m", "eff_ripple_b"]
BOSS_ZONES = [1, 2, 3, 4, 5, 6, 7]   # z13 .. z73 (zone index 0..6), stage = zone_index * 3 + 2
EXTRA_EFFECT_ARCS = [("narc/zf3_act_lz7.narc", 23)]   # BossF: zone 7 act 2 (stage 23)


def manifest_resolver(models_dir):
    """source -> manifest entry of build/dreamcast/models (following duplicate_of)."""
    import json
    man = json.load(open(os.path.join(models_dir, "manifest.json")))
    by_src = {e["source"]: e for e in man["entries"]}

    def resolve(src):
        e = by_src.get(src)
        while e is not None and "duplicate_of" in e:
            e = by_src.get(e["duplicate_of"])
        return e
    return resolve


def effect_table(models_dir):
    resolve = manifest_resolver(models_dir)

    out = []
    for z in BOSS_ZONES:
        arc = "narc/z%d3_act_lz7.narc" % z
        for name in EFFECTS:
            m = resolve("%s:%s.nsbmd" % (arc, name))
            if m is None or not m.get("outputs"):
                continue
            a = resolve("%s:%s.nsbca" % (arc, name))
            out.append(((z - 1) * 3 + 2, name, m["outputs"][0], a["outputs"][0] if a and a.get("outputs") else ""))
    for arc, stage in EXTRA_EFFECT_ARCS:
        for name in EFFECTS:
            m = resolve("%s:%s.nsbmd" % (arc, name))
            if m is None or not m.get("outputs"):
                continue
            a = resolve("%s:%s.nsbca" % (arc, name))
            out.append((stage, name, m["outputs"][0], a["outputs"][0] if a and a.get("outputs") else ""))
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
    # BossStage2_OnModelLoaded data_020747a0 (11 models), Boss2_Create "bs2" files 8 (bs2.nsbca;
    # file 9 bs2.nsbta is timing-only, in bossdata.bin), BossEffect names of z23_act_lz7.narc
    "z23": [
        "mod__bs2_base__bs2_base.dcm",
        "mod__bs2_lotus__bs2_lotus.dcm",
        "mod__bs2_water__bs2_water.dcm",
        "mod__bs2_shower__bs2_shower.dcm",
        "mod__bs2_shower_s__bs2_shower_s.dcm",
        "mod__bs2_shower_e__bs2_shower_e.dcm",
        "mod__bs2_sky__bs2_sky.dcm",
        "mod__bs2_body__bs2_body.dcm",
        "mod__bs2_neck__bs2_neck.dcm",
        "mod__bs2_head__bs2_head.dcm",
        "mod__bs2_prop__bs2_prop.dcm",
        "narc__z23_act_lz7__bs2.dca",
        "narc__z23_act_lz7__eff_blast__eff_blast.dcm",
        "narc__z23_act_lz7__eff_blast.dca",
        "narc__z13_act_lz7__eff_hit__eff_hit.dcm",
        "narc__z13_act_lz7__eff_hit.dca",
        "narc__z13_act_lz7__eff_impact_small__eff_impact_small.dcm",
        "narc__z13_act_lz7__eff_impact_small.dca",
        "narc__extra_act__eff_catch__eff_catch.dcm",
        "narc__extra_act__eff_catch.dca",
        "narc__z23_act_lz7__eff_suction2__eff_suction2.dcm",
        "narc__z23_act_lz7__eff_splash_s__eff_splash_s.dcm",
        "narc__z23_act_lz7__eff_splash_s.dca",
        "narc__z23_act_lz7__eff_splash_b__eff_splash_b.dcm",
        "narc__z23_act_lz7__eff_splash_b.dca",
        "narc__z23_act_lz7__eff_ripple_s__eff_ripple_s.dcm",
        "narc__z23_act_lz7__eff_ripple_s.dca",
        "narc__z23_act_lz7__eff_ripple_m__eff_ripple_m.dcm",
        "narc__z23_act_lz7__eff_ripple_m.dca",
        "narc__z23_act_lz7__eff_ripple_b__eff_ripple_b.dcm",
        "narc__z23_act_lz7__eff_ripple_b.dca",
    ],
    # BossStage3_OnModelLoaded data_02074408 (5 models), Boss3_Create "bs3" file 8 (bs3.nsbca; file 9
    # bs3.nsbta is timing-only, in bossdata.bin), BossEffect names of z33_act_lz7.narc (eff_hit, eff_catch,
    # eff_clash, eff_pop, eff_jump, eff_land, eff_shoot_bomb set up by Boss3_Create, eff_blast3 at the defeat)
    "z33": [
        "mod__bs3_base__bs3_base.dcm",
        "mod__bs3_body__bs3_body.dcm",
        "mod__bs3_ball__bs3_ball.dcm",
        "mod__bs3_thorn__bs3_thorn.dcm",
        "mod__bs3_sky__bs3_sky.dcm",
        "narc__z33_act_lz7__bs3.dca",
        "narc__z33_act_lz7__eff_blast3__eff_blast3.dcm",
        "narc__z33_act_lz7__eff_blast3.dca",
        "narc__z13_act_lz7__eff_hit__eff_hit.dcm",
        "narc__z13_act_lz7__eff_hit.dca",
        "narc__extra_act__eff_catch__eff_catch.dcm",
        "narc__extra_act__eff_catch.dca",
        "narc__extra_act__eff_clash__eff_clash.dcm",
        "narc__extra_act__eff_clash.dca",
        "narc__z33_act_lz7__eff_pop__eff_pop.dcm",
        "narc__z33_act_lz7__eff_pop.dca",
        "narc__extra_act__eff_jump__eff_jump.dcm",
        "narc__extra_act__eff_jump.dca",
        "narc__z33_act_lz7__eff_land__eff_land.dcm",
        "narc__z33_act_lz7__eff_land.dca",
        "narc__ss_act_lz7__eff_shoot_bomb__eff_shoot_bomb.dcm",
        "narc__ss_act_lz7__eff_shoot_bomb.dca",
    ],
    # BossStage4_OnModelLoaded data_02074538 (7 models), Boss4_Create "bs4" files 8 (bs4.nsbca), 9/10
    # (bs4.nsbma / bs4.nsbtp: timing only), 0x51..0x5e (eff_ele_atk, eff_ele_stay, eff_ele_flash,
    # eff_ele_wave, eff_wave_disap), BossEffect names (eff_hit, eff_clash, eff_shoot_bomb, eff_impact_small,
    # eff_blast4 at the defeat)
    # Boss5 = Boss1 zone-5 variant: BossStage5_OnModelLoaded data_02074388 (bs5_base, bs1_neck, bs1_hammer,
    # bs5_sky), "bs1" files 8 (bs1.nsbca, identical to z13's) and 9 (bs5.nsbca), Boss5PartD3 files 0x3d,
    # 0x41..0x45 (eff_missile, eff_bomb_up), BossEffect names of z53_act_lz7.narc
    "z53": [
        "mod__bs5_base__bs5_base.dcm",
        "mod__bs1_neck__bs1_neck.dcm",
        "mod__bs1_hammer__bs1_hammer.dcm",
        "mod__bs5_sky__bs5_sky.dcm",
        "narc__z13_act_lz7__bs1.dca",
        "narc__z53_act_lz7__bs5.dca",
        "narc__z53_act_lz7__eff_missile__eff_missile.dcm",
        "narc__z53_act_lz7__eff_bomb_up__eff_bomb_up.dcm",
        "narc__z53_act_lz7__eff_bomb_up.dca",
        "narc__z53_act_lz7__eff_launch__eff_launch.dcm",
        "narc__z53_act_lz7__eff_launch.dca",
        "narc__z13_act_lz7__eff_hit__eff_hit.dcm",
        "narc__z13_act_lz7__eff_hit.dca",
        "narc__z13_act_lz7__eff_impact_small__eff_impact_small.dcm",
        "narc__z13_act_lz7__eff_impact_small.dca",
        "narc__z13_act_lz7__eff_impact_big__eff_impact_big.dcm",
        "narc__z13_act_lz7__eff_impact_big.dca",
        "narc__z23_act_lz7__eff_blast__eff_blast.dcm",
        "narc__z23_act_lz7__eff_blast.dca",
        "narc__z13_act_lz7__eff_pebble__eff_pebble01.dcm",
    ],
    "z43": [
        "mod__bs4_base__bs4_base.dcm",
        "mod__bs4_body__bs4_body.dcm",
        "mod__bs4_arm__bs4_arm.dcm",
        "mod__bs4_ele__bs4_ele.dcm",
        "mod__bs4_rod__bs4_rod.dcm",
        "mod__bs4_enemy__bs4_enemy.dcm",
        "mod__bs4_sky__bs4_sky.dcm",
        "narc__z43_act_lz7__bs4.dca",
        "narc__z43_act_lz7__eff_ele_atk__eff_ele_atk.dcm",
        "narc__z43_act_lz7__eff_ele_stay__eff_ele_stay.dcm",
        "narc__z43_act_lz7__eff_ele_flash__eff_flash.dcm",
        "narc__z43_act_lz7__eff_ele_flash.dca",
        "narc__z43_act_lz7__eff_ele_wave__eff_wave.dcm",
        "narc__z43_act_lz7__eff_ele_wave.dca",
        "narc__z43_act_lz7__eff_wave_disap__eff_wave_disap.dcm",
        "narc__z43_act_lz7__eff_wave_disap.dca",
        "narc__z43_act_lz7__eff_blast4__eff_blast4.dcm",
        "narc__z43_act_lz7__eff_blast4.dca",
        "narc__z13_act_lz7__eff_hit__eff_hit.dcm",
        "narc__z13_act_lz7__eff_hit.dca",
        "narc__extra_act__eff_clash__eff_clash.dcm",
        "narc__extra_act__eff_clash.dca",
        "narc__z13_act_lz7__eff_impact_small__eff_impact_small.dcm",
        "narc__z13_act_lz7__eff_impact_small.dca",
        "narc__ss_act_lz7__eff_shoot_bomb__eff_shoot_bomb.dcm",
        "narc__ss_act_lz7__eff_shoot_bomb.dca",
    ],
    # BossStage6_OnModelLoaded data_020746c0 (10 models), Boss6_Create "bs6" file 8 (bs6.nsbca), pieces 0x35..0x3d
    # (eff_plate, eff_wind, eff_shoot, eff_shoot_bomb; their .nsbma / .nsbtp are timing only), BossEffect names of
    # z63_act_lz7.narc (eff_pick, eff_parts, eff_impact_small, eff_land, eff_hit, eff_blast at the defeat)
    "z63": [
        "mod__bs6_base__bs6_base.dcm",
        "mod__bs6_rcloud01__bs6_rcloud01.dcm",
        "mod__bs6_rcloud02__bs6_rcloud02.dcm",
        "mod__bs6_tcloud01__bs6_tcloud01.dcm",
        "mod__bs6_tcloud02__bs6_tcloud02.dcm",
        "mod__bs6_sky__bs6_sky.dcm",
        "mod__bs6_body__bs6_body.dcm",
        "mod__bs6_prop01__bs6_prop01.dcm",
        "mod__bs6_prop02__bs6_prop02.dcm",
        "mod__bs6_bult__bs6_bult.dcm",
        "narc__z63_act_lz7__bs6.dca",
        "narc__z63_act_lz7__eff_plate__eff_plate.dcm",
        "narc__z63_act_lz7__eff_wind__eff_wind.dcm",
        "narc__z63_act_lz7__eff_shoot__eff_shoot.dcm",
        "narc__ss_act_lz7__eff_shoot_bomb__eff_shoot_bomb.dcm",
        "narc__ss_act_lz7__eff_shoot_bomb.dca",
        "narc__z63_act_lz7__eff_pick__eff_pick.dcm",
        "narc__z63_act_lz7__eff_pick.dca",
        "narc__z63_act_lz7__eff_parts__eff_parts.dcm",
        "narc__z63_act_lz7__eff_parts.dca",
        "narc__z13_act_lz7__eff_impact_small__eff_impact_small.dcm",
        "narc__z13_act_lz7__eff_impact_small.dca",
        "narc__z33_act_lz7__eff_land__eff_land.dcm",
        "narc__z33_act_lz7__eff_land.dca",
        "narc__z13_act_lz7__eff_hit__eff_hit.dcm",
        "narc__z13_act_lz7__eff_hit.dca",
        "narc__z23_act_lz7__eff_blast__eff_blast.dcm",
        "narc__z23_act_lz7__eff_blast.dca",
    ],
    # BossStage7_OnModelLoaded data_02074164 (bs7_base, bs7_sky), "bs7" files 8 / 9 (bs7.nsbca / bs7.nsbva), the
    # rival (StageObj_LoadModel / LoadMotion of player_lz7: blz.nsbmd against Sonic, son.nsbmd against Blaze, both
    # listed; son.nsbca / son.nsbva), D6 0x12 / 0x19 (eff_tornado / eff_flame; .nsbma / .nsbtp timing only), D7
    # 0x20..0x2d (eff_aura_son / eff_aura_blz + 01..03 .nsbca), BossEffect names of z73_act_lz7.narc (eff_hit,
    # eff_homing, eff_boost, eff_strike_big / _hit / _fin, eff_clash, eff_suction); boss7bac.bin (son.bac boxes)
    # is written next to bossdata.bin
    "z73": [
        "mod__bs7_base__bs7_base.dcm",
        "mod__bs7_sky__bs7_sky.dcm",
        "narc__z73_act_lz7__bs7.dca",
        "narc__z73_act_lz7__bs7.dcv",
        "mod__son__son.dcm",
        "mod__son__son_ball.dcm",
        "mod__blz__blz.dcm",
        "mod__blz__blz_spin.dcm",
        "narc__player_lz7__son.dca",
        "narc__player_lz7__son.dcv",
        "narc__z73_act_lz7__eff_tornado__eff_tornado.dcm",
        "narc__z73_act_lz7__eff_flame__eff_flame.dcm",
        "narc__z73_act_lz7__eff_aura_son__eff_aura_son.dcm",
        "narc__z73_act_lz7__eff_aura_son01.dca",
        "narc__z73_act_lz7__eff_aura_son02.dca",
        "narc__z73_act_lz7__eff_aura_son03.dca",
        "narc__z73_act_lz7__eff_aura_blz__eff_aura_blz.dcm",
        "narc__z73_act_lz7__eff_aura_blz01.dca",
        "narc__z73_act_lz7__eff_aura_blz02.dca",
        "narc__z73_act_lz7__eff_aura_blz03.dca",
        "narc__z13_act_lz7__eff_hit__eff_hit.dcm",
        "narc__z13_act_lz7__eff_hit.dca",
        "narc__z73_act_lz7__eff_homing__eff_homing.dcm",
        "narc__z73_act_lz7__eff_homing.dca",
        "narc__z73_act_lz7__eff_boost__eff_boost.dcm",
        "narc__z73_act_lz7__eff_boost.dca",
        "narc__z73_act_lz7__eff_strike_big__eff_strike_big.dcm",
        "narc__z73_act_lz7__eff_strike_big.dca",
        "narc__z73_act_lz7__eff_strike_hit__eff_strike_hit.dcm",
        "narc__z73_act_lz7__eff_strike_hit.dca",
        "narc__z73_act_lz7__eff_strike_fin__eff_strike_fin.dcm",
        "narc__z73_act_lz7__eff_strike_fin.dca",
        "narc__extra_act__eff_clash__eff_clash.dcm",
        "narc__extra_act__eff_clash.dca",
        "narc__z73_act_lz7__eff_suction__eff_suction.dcm",
    ],
    # BossStageF_OnModelLoaded data_02074498 (bsf_base, bsf_body, bsf_ball, bsf_shock, bsf_wave_sl, bsf_pas_dust01),
    # BossF_Create /mod/bsf_sky + /mod/bsf_passage (4 models), BossFPartD8_State_LoadSkyModel /mod/bsf_sky_b, "bsf"
    # files 8 / 9 (bsf.nsbca / bsf.nsbva; 10 / 11 timing only), 0x6e / 0x6f / 0x72 (eff_laser_fin, eff_laser_fin_b,
    # eff_beam_fin; their .nsbma / .nsbtp timing only), BossEffect names of zf3_act_lz7.narc; bsf_st_r.df / .di are
    # in bossdata.bin
    "zf3": [
        "mod__bsf_base__bsf_base.dcm",
        "mod__bsf_body__bsf_body.dcm",
        "mod__bsf_ball__bsf_ball.dcm",
        "mod__bsf_shock__bsf_shock.dcm",
        "mod__bsf_wave_sl__bsf_wave_sl.dcm",
        "mod__bsf_pas_dust01__bsf_pas_dust01.dcm",
        "mod__bsf_sky__bsf_sky.dcm",
        "mod__bsf_sky_b__bsf_sky_b.dcm",
        "mod__bsf_passage__bsf_pas_sate01.dcm",
        "mod__bsf_passage__bsf_pas_peel01.dcm",
        "mod__bsf_passage__bsf_pas_clo00.dcm",
        "mod__bsf_passage__bsf_pas_clo01.dcm",
        "narc__zf3_act_lz7__bsf.dca",
        "narc__zf3_act_lz7__bsf.dcv",
        "narc__zf3_act_lz7__eff_laser_fin__eff_laser_fin.dcm",
        "narc__zf3_act_lz7__eff_laser_fin_b__eff_laser_fin_b.dcm",
        "narc__zf3_act_lz7__eff_beam_fin__eff_beam_fin.dcm",
        "narc__z13_act_lz7__eff_hit__eff_hit.dcm",
        "narc__z13_act_lz7__eff_hit.dca",
        "narc__zf3_act_lz7__eff_impact_sl__eff_impact_sl.dcm",
        "narc__zf3_act_lz7__eff_impact_sl.dca",
        "narc__zf3_act_lz7__eff_impact_st__eff_impact_st.dcm",
        "narc__zf3_act_lz7__eff_impact_st.dca",
        "narc__zf3_act_lz7__eff_impact_pr__eff_impact_pr.dcm",
        "narc__zf3_act_lz7__eff_impact_pr.dca",
        "narc__zf3_act_lz7__eff_dmg_hand__eff_dmg_hand.dcm",
        "narc__zf3_act_lz7__eff_dmg_hand.dca",
        "narc__zf3_act_lz7__eff_produce__eff_produce.dcm",
        "narc__zf3_act_lz7__eff_produce.dca",
        "narc__zf3_act_lz7__eff_bomb_ball__eff_bomb_ball.dcm",
        "narc__zf3_act_lz7__eff_bomb_ball.dca",
        "narc__zf3_act_lz7__eff_tackle__eff_tackle.dcm",
        "narc__zf3_act_lz7__eff_base_dest__eff_base_dest.dcm",
        "narc__zf3_act_lz7__eff_base_dest.dca",
        "narc__zf3_act_lz7__eff_beam_fin_b__eff_beam_fin_b.dcm",
        "narc__zf3_act_lz7__eff_beam_fin_b.dca",
        "narc__zf3_act_lz7__eff_blastf__eff_blastf.dcm",
        "narc__zf3_act_lz7__eff_blastf.dca",
    ],
}


def write_boss7_bac(extract, out_dir):
    """boss7bac.bin "B7BC" v1: the son.bac (player_lz7.narc act/son.bac) timing the Boss7 rival's AnimatorSprite
    needs: per animation {u16 first_frame, u16 frame_count, s16 loop_frame, u16 0}, per frame {u16 duration, u16
    first_event, u16 count, u16 0}, per command-7 event the 16 command bytes {u16 7, u16 16, u16 slot, u16 flags,
    s16 l, t, r, b} (Boss7PartD5_SpriteCallback reads them as on the DS).  Header: magic, u16 1, u16 anims,
    u16 frames, u16 events, u32 0."""
    import bac_format
    from nds_archive import open_narc, maybe_decompress
    files = dict(open_narc(os.path.join(extract, "files", "narc", "player_lz7.narc")).files)
    bac = bac_format.Bac(maybe_decompress(files["act/son.bac"]))
    anims, frames, events = [], [], []
    for an in bac.animations:
        if an.loop_anim is not None or an.next_anim is not None:
            raise SystemExit("son.bac anim %d jumps to another animation (not handled)" % an.index)
        anims.append((len(frames), len(an.frames), an.loop_frame if an.loop_frame is not None else -1))
        for fr in an.frames:
            evs = [e for e in fr.events if e.get("kind") == "hitbox"]
            frames.append((fr.duration, len(events), len(evs)))
            for e in evs:
                events.append((e["slot"], e["flags"]) + tuple(e["rect"]))
    b = struct.pack("<4sHHHHI", b"B7BC", 1, len(anims), len(frames), len(events), 0)
    for ff, n, lp in anims:
        b += struct.pack("<HHhH", ff, n, lp, 0)
    for du, fe, n in frames:
        b += struct.pack("<HHHH", du, fe, n, 0)
    for slot, fl, l, t, r, bb in events:
        b += struct.pack("<HHHHhhhh", 7, 16, slot, fl & 0xFFFF, l, t, r, bb)
    with open(os.path.join(out_dir, "boss7bac.bin"), "wb") as f:
        f.write(b)
    print("boss7bac.bin: son.bac %d animations, %d frames, %d hit box events (%d bytes)" %
          (len(anims), len(frames), len(events), len(b)))


def anim_frame_counts(d):
    """numFrame of every animation in an NNS animation file (BTA0 SRT0 / BMA0 MAT0 / BTP0 PAT0 / BCA0
    JNT0): the dictionary at block + 8 points to animations whose u16 at +4 is numFrame."""
    import g3d
    magic, blocks = g3d.file_blocks(d)
    blk = {b"BTA0": b"SRT0", b"BMA0": b"MAT0", b"BTP0": b"PAT0", b"BCA0": b"JNT0"}[magic]
    bo = blocks[blk]
    return [g3d.u16(d, bo + g3d.u32(ad, 0) + 4) for _, ad in g3d.read_dict(d, bo + 8)]


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
    arc_blobs = {}
    if ARCHIVE_CHUNKS:
        sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../../../tools/dreamcast/sprites"))
        from nds_archive import open_narc as _open_narc, maybe_decompress as _maybe_decompress
        for addr, arc, member in ARCHIVE_CHUNKS:
            files = dict(_open_narc(os.path.join(a.extract, "files", arc)).files)
            arc_blobs[addr] = _maybe_decompress(files[member])
    all_chunks = list(CHUNKS) + [(member, addr, len(arc_blobs[addr])) for addr, arc, member in ARCHIVE_CHUNKS]
    hdr_size = 8 + 12 * len(all_chunks)
    table = []
    data = bytearray()
    off = hdr_size
    for name, addr, size in all_chunks:
        while (off + len(data)) % 4 != addr % 4:
            data.append(0)
        foff = off + len(data)
        blob = arc_blobs[addr] if addr in arc_blobs else read_range(arm9, ov000, addr, size)
        if len(blob) != size:
            print("short read %s" % name)
            return 1
        data += blob
        table.append((addr, size, foff))
    out = struct.pack("<4sHH", b"BOSD", 1, len(all_chunks))
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
    print("effects: %d (stage, name) packages; per stage %s" % (
        len(effects), {st: sum(1 for e in effects if e[0] == st) for st in sorted(set(e[0] for e in effects))}))
    from nds_archive import open_narc, maybe_decompress
    out += struct.pack("<I", len(ANIM_LENGTHS))
    for arc, member in ANIM_LENGTHS:
        files = dict(open_narc(os.path.join(a.extract, "files", arc)).files)
        frames = anim_frame_counts(maybe_decompress(files[member]))
        blob = struct.pack("<24sI", member.encode(), len(frames)) + struct.pack("<%dH" % len(frames), *frames)
        while len(blob) % 4:
            blob += b"\0"
        out += blob
        print("timing-only %s:%s: %d animations, frames %s" % (arc, member, len(frames), frames))
    # converted material animations (.dcc colour / .dcp texture pattern / .dct texture SRT, nsbmd2dc.py):
    # section 5 = the timing-table names above -> package, section 6 = the effects' slots 1..3
    # (BossEffect_Create <name>.nsbma / .nsbtp / .nsbta); boss_core.c reads both, older readers stop before
    mat_ext = {".nsbma": ".dcc", ".nsbtp": ".dcp", ".nsbta": ".dct"}
    resolve = manifest_resolver(a.models)

    def converted(src, ext):
        e = resolve(src)
        f = [o for o in (e or {}).get("outputs", []) if o.endswith(ext)]
        return f[0] if f else ""

    def stage_key(arc):
        return arc.split("/")[1][:3]      # narc/z43_act_lz7.narc -> z43
    out += struct.pack("<I", len(ANIM_LENGTHS))
    for arc, member in ANIM_LENGTHS:
        f = converted("%s:%s" % (arc, member), mat_ext[member[member.rindex("."):]])
        out += struct.pack("<24s96s", member.encode(), f.encode())
        if f:
            ASSETS.setdefault(stage_key(arc), []).append(f)
    effmats = []
    for arc, stage in [("narc/z%d3_act_lz7.narc" % z, (z - 1) * 3 + 2) for z in BOSS_ZONES] + EXTRA_EFFECT_ARCS:
        for name in EFFECTS:
            if not converted("%s:%s.nsbmd" % (arc, name), ".dcm"):
                continue
            fs = [converted("%s:%s%s" % (arc, name, x), mat_ext[x]) for x in (".nsbma", ".nsbtp", ".nsbta")]
            if any(fs):
                effmats.append((stage, name, fs))
                ASSETS.setdefault(stage_key(arc), []).extend(f for f in fs if f)
    out += struct.pack("<I", len(effmats))
    for stage, name, fs in effmats:
        out += struct.pack("<B3x24s96s96s96s", stage, name.encode(), *(f.encode() for f in fs))
    print("material animations: %d timing-table packages, %d effects with material / pattern / SRT sets" %
          (sum(1 for arc, m in ANIM_LENGTHS if converted("%s:%s" % (arc, m), mat_ext[m[m.rindex("."):]])),
           len(effmats)))
    os.makedirs(a.out, exist_ok=True)
    with open(os.path.join(a.out, "bossdata.bin"), "wb") as f:
        f.write(out)
    write_boss7_bac(a.extract, a.out)
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

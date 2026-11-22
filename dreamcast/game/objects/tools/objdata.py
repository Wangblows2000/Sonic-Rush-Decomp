#!/usr/bin/env python3
"""Object-module data for the native Dreamcast port (bring-your-own-ROM).

Reads the owner's extracted ROM (extract/eur) and the sprite converter output
(build/dreamcast/sprites, tools/dreamcast/sprites/bac2dc.py) and writes
build/dreamcast/objects/:

  objdata.bin   "ROBJ" v3: small ROM tables the object code reads at runtime
                (copied from extract/eur/arm9_overlays/ov000.bin), the
                resolved sprite id of every object BAC slot for every stage,
                and per stage the /dat/gmk/*.df collision maps (ObjData_Load:
                zone archive first, then the loose file).
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
    ("solid", 0x023661C0, 48 * 8),       # data_ov000_023661c0: all-solid collision tiles (Needle: first 12)
    ("animal", 0x023661A8, 24),          # data_ov000_023661a8: freed animal kinds per zone [8][3]
    ("shake", 0x02077944, 16),           # data_02077944 (arm9): StageObj_Main hit-stop shake offsets (s8)
    # v2
    ("col_attr", 0x023661A4, 4),         # data_ov000_023661a4: collision map attribute (top-solid)
    ("bend_wobble", 0x023643D4, 28),     # data_ov000_023643d4/_d8/_dc: BendStick wobble offsets (s8)
    ("pedestal", 0x02364400, 48),        # data_ov000_02364400: JumpPedestal launch speeds s16[4][6]
    ("dec_margin", 0x023685EC, 0xF2),    # data_ov000_023685ec: per decoration type spawn margin
    # v4 (zones 4..7)
    ("valve_ofs", 0x02364444, 16),       # data_ov000_02364444: Valve steam sprite offsets s16[4][2]
    ("valve_box", 0x02364454, 32),       # data_ov000_02364454: Valve hit box s16[4][4]
    ("valve_puff", 0x02364474, 32),      # data_ov000_02364474: Valve puff area s16[4][4]
    ("valve_anim", 0x02364494, 48),      # data_ov000_02364494: Valve steam anim active/end + flags u32[4][3]
]
# /dat/gmk/*.df collision maps (ObjData_Load with gZoneActArchive)
DF_FILES = ["gmk_jump30.df", "gmk_jump45.df", "gmk_jump60.df", "gmk_cargo.df", "gmk_jet.df",
            "gmk_switch.df", "gmk_switch_tate.df"]
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
    # v2: Leaf Storm + shared objects, decorations, effects (string table near data_ov000_02366bf0..)
    ("TRAMPOLINE", "zone", "act/ac_gmk_trampoline.bac", "act/ac_gmk_trampoline.bac"),
    ("BEND_STICK", "zone", "act/ac_gmk_bend_stick.bac", "act/ac_gmk_bend_stick.bac"),
    ("ROCK_BREAK", "zone", "act/ac_gmk_rock_break.bac", "act/ac_gmk_rock_break.bac"),
    ("ROPE", "zone", "act/ac_gmk_rope.bac", "act/ac_gmk_rope.bac"),
    ("ROPE_C_LAND", "zone", "act/ac_gmk_rope_c_land.bac", "act/ac_gmk_rope_c_land.bac"),
    ("ROPE_C_SPRING", "zone", "act/ac_gmk_rope_c_spring.bac", "act/ac_gmk_rope_c_spring.bac"),
    ("ROPE_C", "zone", "act/ac_gmk_rope_c.bac", "act/ac_gmk_rope_c.bac"),
    ("JUMP_PEDESTAL", "zone", "act/ac_gmk_jump_pedestal.bac", "act/ac_gmk_jump_pedestal.bac"),
    ("FLOOR_BREAK", "zone", "act/ac_gmk_floor_braek01.bac", "act/ac_gmk_floor_braek01.bac"),
    ("LAND", "zone", "act/ac_gmk_land.bac", "act/ac_gmk_land.bac"),
    ("WALL_BREAK", "zone", "act/ac_gmk_wall_braek01.bac", "act/ac_gmk_wall_braek01.bac"),
    ("IRON_BALL", "zone", "act/ac_gmk_iron_ball.bac", "act/ac_gmk_iron_ball.bac"),
    ("SPECIAL", "zone", "act/ac_gmk_special.bac", "act/ac_gmk_special.bac"),
    ("CARGO", "zone", "act/ac_gmk_cargo.bac", "act/ac_gmk_cargo.bac"),
    ("FLAPPER", "zone", "act/ac_ene_flapper.bac", "act/ac_ene_flapper.bac"),
    ("EGG_DIVER", "zone", "act/ac_ene_egg_diver.bac", "act/ac_ene_egg_diver.bac"),
    ("DEC_FLOWER", "zone", "act/ac_dec_flower.bac", "act/ac_dec_flower.bac"),
    ("DEC_VINE", "zone", "act/ac_dec_vine.bac", "act/ac_dec_vine.bac"),
    ("DEC_BIRD_NEST", "zone", "act/ac_dec_bird_nest.bac", "act/ac_dec_bird_nest.bac"),
    ("DEC_BIRD", "zone", "act/ac_dec_bird.bac", "act/ac_dec_bird.bac"),
    ("DEC_LEAF", "zone", "act/ac_dec_leaf.bac", "act/ac_dec_leaf.bac"),
    ("EFF_FOUND", "act_com", "act/ac_eff_found.bac", "act/ac_eff_found.bac"),
    ("EFF_BRAKE", "act_com", "act/ac_eff_kemuri_brake.bac", "act/ac_eff_kemuri_brake.bac"),
] + [("ANIMAL%02d" % n, "zone", "act/ac_dec_animal%02d.bac" % n, "act/ac_dec_animal%02d.bac" % n)
     for n in range(1, 17)                                        # DecAnimal: data_ov000_023672a0
]
# objdata v3: Water Palace (zone 2) and Mirage Road (zone 3) (string table data_ov000_02366378..02367fa8)
SLOTS += [
    ("LAND_UNREST", "zone", "act/ac_gmk_land_unrest.bac", "act/ac_gmk_land_unrest.bac"),
    ("WHEEL2D", "zone", "act/ac_gmk_wheel2d.bac", "act/ac_gmk_wheel2d.bac"),
    ("MINE", "zone", "act/ac_gmk_mine.bac", "act/ac_gmk_mine.bac"),
    ("BUBBLE", "zone", "act/ac_gmk_bubble.bac", "act/ac_gmk_bubble.bac"),
    ("WATER_JET", "zone", "act/ac_gmk_water_jet.bac", "act/ac_gmk_water_jet.bac"),
    ("KURAGEN_BOMB", "zone", "act/ac_ene_kuragen_bomb.bac", "act/ac_ene_kuragen_bomb.bac"),
    ("KURAGEN_BLITZ", "zone", "act/ac_ene_kuragen_blitz.bac", "act/ac_ene_kuragen_blitz.bac"),
    ("EGG_LANCER", "zone", "act/ac_ene_egg_lancer.bac", "act/ac_ene_egg_lancer.bac"),
    ("EFF_WATER", "zone", "act/ac_eff_water.bac", "act/ac_eff_water.bac"),
    ("EFF_WATER_GMK", "zone", "act/ac_eff_water_gmk.bac", "act/ac_eff_water_gmk.bac"),
    ("FIX_MOJI", "act_com", "ac_fix_moji.bac", "act/ac_fix_moji.bac"),  # data_ov000_02367fa8 (MineDigit: HUD digits)
    ("DEC_FOUNTAIN", "zone", "act/ac_dec_fountain.bac", "act/ac_dec_fountain.bac"),
    ("WATER_FALL", "zone", "act/ac_gmk_water_fall.bac", "act/ac_gmk_water_fall.bac"),
    ("GMK_WHEEL", "zone", "act/ac_gmk_wheel.bac", "act/ac_gmk_wheel.bac"),
    ("DEC_ALGAE", "zone", "act/ac_dec_algae.bac", "act/ac_dec_algae.bac"),
    ("DEC_CORAL", "zone", "act/ac_dec_coral.bac", "act/ac_dec_coral.bac"),
    ("DEC_FISH", "zone", "act/ac_dec_fish.bac", "act/ac_dec_fish.bac"),
    ("EGG_BISHOP", "zone", "act/ac_ene_egg_bishop.bac", "act/ac_ene_egg_bishop.bac"),
    ("EGG_HAMMER", "zone", "act/ac_ene_egg_hammer.bac", "act/ac_ene_egg_hammer.bac"),
    ("HAND2D", "zone", "act/ac_gmk_hand2d.bac", "act/ac_gmk_hand2d.bac"),
    ("ROCK", "zone", "act/ac_gmk_rock.bac", "act/ac_gmk_rock.bac"),
    ("SAND_SLIDER", "zone", "act/ac_gmk_sand_slider.bac", "act/ac_gmk_sand_slider.bac"),
    ("EFF_SAND", "zone", "act/ac_eff_sand.bac", "act/ac_eff_sand.bac"),
    ("EFF_SHADOW", "zone", "act/ac_eff_shadow.bac", "act/ac_eff_shadow.bac"),
    ("GMK_RAIL", "zone", "act/ac_gmk_rail.bac", "act/ac_gmk_rail.bac"),
    ("GMK_WALL", "zone", "act/ac_gmk_wall.bac", "act/ac_gmk_wall.bac"),
    ("GMK_HAND", "zone", "act/ac_gmk_hand.bac", "act/ac_gmk_hand.bac"),
    ("DEC_SAND", "zone", "act/ac_dec_sand.bac", "act/ac_dec_sand.bac"),
    ("DEC_DECORA", "zone", "act/ac_dec_decora.bac", "act/ac_dec_decora.bac"),
    ("DEC_RELIEF", "zone", "act/ac_dec_relief.bac", "act/ac_dec_relief.bac"),
    ("DEC_STATUE", "zone", "act/ac_dec_statue.bac", "act/ac_dec_statue.bac"),
    ("DEC_TORCH", "zone", "act/ac_dec_torch.bac", "act/ac_dec_torch.bac"),
    ("DEC_CANOPS", "zone", "act/ac_dec_canops.bac", "act/ac_dec_canops.bac"),
    ("DEC_BED", "zone", "act/ac_dec_bed.bac", "act/ac_dec_bed.bac"),
    ("DEC_BOX", "zone", "act/ac_dec_box.bac", "act/ac_dec_box.bac"),
    ("DEC_TABLE_WATER", "zone", "act/ac_dec_table_water.bac", "act/ac_dec_table_water.bac"),
    ("EFF_ENTRANCE", "zone", "act/ac_eff_entrance.bac", "act/ac_eff_entrance.bac"),
    ("GMK_RAIL6", "zone", "act/ac_gmk_rail6.bac", "act/ac_gmk_rail6.bac"),
]
# objdata v4: zones 4..7 (Night Carnival, Huge Crisis, Altitude Limit, Dead Line)
SLOTS_V4 = [
    ("CRANE_SLIDE", "zone", "act/ac_gmk_crane_slide.bac", "act/ac_gmk_crane_slide.bac"),
    ("NEON_ROUTE", "zone", "act/ac_gmk_neon_route.bac", "act/ac_gmk_neon_route.bac"),
    ("SHOOTER", "zone", "act/ac_gmk_shooter.bac", "act/ac_gmk_shooter.bac"),
    ("SWING_SKY", "zone", "act/ac_gmk_swing_sky.bac", "act/ac_gmk_swing_sky.bac"),
    ("SWITCH", "zone", "act/ac_gmk_switch.bac", "act/ac_gmk_switch.bac"),
    ("FIX_INFORMATION", "zone", "act/ac_fix_information.bac", "act/ac_fix_information.bac"),
    ("DEC_SONIC_NEON", "zone", "act/ac_dec_sonic_neon.bac", "act/ac_dec_sonic_neon.bac"),
    ("DEC_NEON_FLOOR", "zone", "act/ac_dec_neon_floor.bac", "act/ac_dec_neon_floor.bac"),
    ("DEC_NEON_LAND", "zone", "act/ac_dec_neon_land.bac", "act/ac_dec_neon_land.bac"),
    ("DEC_SWING_SKY_HANG", "zone", "act/ac_dec_swing_sky_hang.bac", "act/ac_dec_swing_sky_hang.bac"),
    ("DEC_NEON_BALL", "zone", "act/ac_dec_neon_ball.bac", "act/ac_dec_neon_ball.bac"),
    ("DEC_NEON_PILLAR", "zone", "act/ac_dec_neon_pillar.bac", "act/ac_dec_neon_pillar.bac"),
    ("DEC_LIGHT4", "zone", "act/ac_dec_light4.bac", "act/ac_dec_light4.bac"),
    ("DEC_NEON_BALL2", "zone", "act/ac_dec_neon_ball2.bac", "act/ac_dec_neon_ball2.bac"),
    ("DEC_NEON_STAR", "zone", "act/ac_dec_neon_star.bac", "act/ac_dec_neon_star.bac"),
    ("GLIDER", "zone", "act/ac_gmk_glider.bac", "act/ac_gmk_glider.bac"),
    ("NEEDLE_G", "zone", "act/ac_gmk_needle_g.bac", "act/ac_gmk_needle_g.bac"),
    ("PARACHUTE", "zone", "act/ac_gmk_parachute.bac", "act/ac_gmk_parachute.bac"),
    ("SAUCER", "zone", "act/ac_gmk_saucer.bac", "act/ac_gmk_saucer.bac"),
    ("CLOUD_3", "zone", "act/ac_gmk_cloud_3.bac", "act/ac_gmk_cloud_3.bac"),
    ("RING_GRAIDER", "zone", "act/ac_itm_ring_graider.bac", "act/ac_itm_ring_graider.bac"),
    ("FLAPPER3D", "zone", "act/ac_ene_flapper3d.bac", "act/ac_ene_flapper3d.bac"),
    ("DEC_CLOUD", "zone", "act/ac_dec_cloud.bac", "act/ac_dec_cloud.bac"),
    ("DEC_CLOUD_W", "zone", "act/ac_dec_cloud_w.bac", "act/ac_dec_cloud_w.bac"),
    ("DEC_INNER_PILLAR", "zone", "act/ac_dec_inner_pillar.bac", "act/ac_dec_inner_pillar.bac"),
    ("DEC_PROPELLER", "zone", "act/ac_dec_propeller.bac", "act/ac_dec_propeller.bac"),
    ("DEC_JET", "zone", "act/ac_dec_jet.bac", "act/ac_dec_jet.bac"),
    ("DEC_LIGHT", "zone", "act/ac_dec_light.bac", "act/ac_dec_light.bac"),
    ("DEC_LASER", "zone", "act/ac_dec_laser.bac", "act/ac_dec_laser.bac"),
    ("CRANE", "zone", "act/ac_gmk_crane.bac", "act/ac_gmk_crane.bac"),
    ("PIPE", "zone", "act/ac_gmk_pipe.bac", "act/ac_gmk_pipe.bac"),
    ("PISTON", "zone", "act/ac_gmk_piston.bac", "act/ac_gmk_piston.bac"),
    ("VALVE", "zone", "act/ac_gmk_valve.bac", "act/ac_gmk_valve.bac"),
    ("JET_CATAPULT", "zone", "act/ac_gmk_jet_catapult.bac", "act/ac_gmk_jet_catapult.bac"),
    ("EFF_JET", "zone", "act/ac_eff_jet.bac", "act/ac_eff_jet.bac"),
    ("EFF_STEAM", "zone", "act/ac_eff_steam.bac", "act/ac_eff_steam.bac"),
    ("DEC_PIPE", "zone", "act/ac_dec_pipe.bac", "act/ac_dec_pipe.bac"),
    ("DEC_PLANE", "zone", "act/ac_dec_plane.bac", "act/ac_dec_plane.bac"),
    ("AIR", "zone", "act/ac_gmk_air.bac", "act/ac_gmk_air.bac"),
    ("CANNON", "zone", "act/ac_gmk_cannon.bac", "act/ac_gmk_cannon.bac"),
    ("DUCT", "zone", "act/ac_gmk_duct.bac", "act/ac_gmk_duct.bac"),
    ("ROCKET", "zone", "act/ac_gmk_rocket.bac", "act/ac_gmk_rocket.bac"),
    ("SWING", "zone", "act/ac_gmk_swing.bac", "act/ac_gmk_swing.bac"),
    ("DEC_ARROW", "zone", "act/ac_dec_arrow.bac", "act/ac_dec_arrow.bac"),
    ("EGG_SF", "zone", "act/ac_ene_egg_sf.bac", "act/ac_ene_egg_sf.bac"),
    ("EGG_SOLID", "zone", "act/ac_ene_egg_solid.bac", "act/ac_ene_egg_solid.bac"),
    ("GUN_HUNTER", "zone", "act/ac_ene_gun_hunter.bac", "act/ac_ene_gun_hunter.bac"),
    ("RHINO", "zone", "act/ac_ene_rhino.bac", "act/ac_ene_rhino.bac"),
    ("EGG_BUNNY", "zone", "act/ac_ene_egg_bunny.bac", "act/ac_ene_egg_bunny.bac"),
    ("EGG_FLAPPER_LASER", "zone", "act/ac_ene_egg_flapper_laser.bac", "act/ac_ene_egg_flapper_laser.bac"),
    ("BOMB_HAWK", "zone", "act/ac_ene_bomb_hawk.bac", "act/ac_ene_bomb_hawk.bac"),
    ("FLAPPER_BAT", "zone", "act/ac_ene_flapper_bat.bac", "act/ac_ene_flapper_bat.bac"),
    ("FLAPPER_CANNON", "zone", "act/ac_ene_flapper_cannon.bac", "act/ac_ene_flapper_cannon.bac"),
    ("FALCO", "zone", "act/ac_ene_falco.bac", "act/ac_ene_falco.bac"),
]
SLOTS += SLOTS_V4
# player effects (src/game/Effects: EffBoost*, EffHighSpeedStart, EffBlazeBoostFlame, EffBoostJet,
# EffSpinSmoke1/2): act_com /act/ac_eff_boost.bac (data_ov000_02368460..02368520),
# ac_eff_kemuri_spin01/02.bac
SLOTS += [
    ("EFF_BOOST", "act_com", "act/ac_eff_boost.bac", "act/ac_eff_boost.bac"),
    ("EFF_SPIN1", "act_com", "act/ac_eff_kemuri_spin01.bac", "act/ac_eff_kemuri_spin01.bac"),
    ("EFF_SPIN2", "act_com", "act/ac_eff_kemuri_spin02.bac", "act/ac_eff_kemuri_spin02.bac"),
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


# PaletteAnim_Create sequence tables {u16 anim; u8 frames; u8 flags (1 = end)} (ov000):
# DecGroupAF neon ball / pillar / star (zone 4), DecArrow (zone 7), DecGroup95 laser lights (zone 6)
PALANIM_TABLES = [0x02364568, 0x0236454C, 0x02364558, 0x0236457C, 0x0236459C]


def write_palanim(ov, out_path):
    """palanim.bin "RPAL" v1: u16 version, u16 count; count x {u32 address; u16 n; u16 pad;
    n x {u16 anim; u8 time; u8 flags}} (the entries up to and including the end marker)."""
    b = bytearray(b"RPAL") + struct.pack("<HH", 1, len(PALANIM_TABLES))
    for addr in PALANIM_TABLES:
        o = addr - OV_BASE
        ents = []
        while True:
            col, t, fl = struct.unpack_from("<HBB", ov, o + 4 * len(ents))
            ents.append((col, t, fl))
            if fl & 1 or t == 0 or len(ents) >= 64:
                break
        b += struct.pack("<IHH", addr, len(ents), 0)
        for e in ents:
            b += struct.pack("<HBB", *e)
    with open(out_path, "wb") as f:
        f.write(b)


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
    blob += struct.pack("<HHHH", 4, len(SLOTS), N_STAGES, ID_LEN)
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
    # per stage .df blobs: u32 offsets[stages + 1] (absolute), then records
    # {char name[24]; u32 size; data padded to 4}
    sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../../../tools/dreamcast/sprites"))
    import nds_archive  # noqa: E402  (converter helper: NARC + LZ decoding)
    dir_off = len(blob)
    blob += b"\0" * (4 * (N_STAGES + 1))
    df_report = {}
    for s_ in range(N_STAGES):
        struct.pack_into("<I", blob, dir_off + 4 * s_, len(blob))
        st = stage_name(s_)
        files = {}
        if st:
            narc_path = os.path.join(a.extract, "files/narc/%s_act_lz7.narc" % st)
            if os.path.exists(narc_path):
                for name, data in nds_archive.open_narc(narc_path).files:
                    if name.startswith("dat/gmk/") and name[8:] in DF_FILES:
                        files[name[8:]] = data
            for n in DF_FILES:
                lp = os.path.join(a.extract, "files/dat/gmk", n)
                if n not in files and os.path.exists(lp):
                    files[n] = open(lp, "rb").read()
        for n in DF_FILES:
            if n in files:
                d = files[n]
                blob += n.encode().ljust(24, b"\0") + struct.pack("<I", len(d)) + d
                while len(blob) % 4:
                    blob += b"\0"
                df_report[n] = df_report.get(n, 0) + 1
    struct.pack_into("<I", blob, dir_off + 4 * N_STAGES, len(blob))
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

    write_palanim(ov, os.path.join(a.out, "palanim.bin"))

    with open(os.path.join(a.out, "sprites.txt"), "w") as f:
        f.write("# sprite directories (build/dreamcast/sprites/bac/<id>/) used by the object module\n")
        f.write("# disc: /cd/sprites/<id>/{frames.bin,atlas0.argb1555}  /cd/objects/{objdata.bin,hbx/<id>.hbx}\n")
        for sid in sorted(needed):
            f.write("%s\t%s\n" % (sid, " ".join(sorted(needed[sid]))))
    print("objdata.bin: %d bytes, %d slots x %d stages, %d unresolved (stage, slot) pairs"
          % (len(blob), len(SLOTS), N_STAGES, missing))
    print("df files per stage: %s" % ", ".join("%s x%d" % kv for kv in sorted(df_report.items())))
    for line in report:
        print("hbx " + line)
    return 0


if __name__ == "__main__":
    sys.exit(main())

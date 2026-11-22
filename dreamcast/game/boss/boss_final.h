/*
 * Final boss (BossF, stage zf3 = stage id 23, DS zone 7 act 2): public API of
 * the native translation in boss_final.c.  The common boss API (boss.h)
 * drives it like the other bosses (boss_init(23, ...), boss_start,
 * boss_update_pre + boss_update, boss_draw, boss_health / boss_phase); this
 * header adds what only the final fight has.  See
 * docs/dreamcast/translation/BOSSES.md section "BossF".
 */
#ifndef BOSS_FINAL_H
#define BOSS_FINAL_H

#include "boss.h"

/* gBossStatus.unk4 (boss_phase) values written by the BossF code */
enum {
    BOSSF_PHASE_IDLE = 0x2f,      /* BossF_Create / BossF_PartD8StartWaitNextAttack */
    BOSSF_PHASE_HANDSLAM = 0x30,  /* BossFPartD8_Action_StartHandSlam: one fist slams (wave sl objects) */
    BOSSF_PHASE_PRESS = 0x31,     /* BossFPartD8_Action_StartDoublePress: both fists press */
    BOSSF_PHASE_THROW = 0x32,     /* BossFPartD8_Action_StartPartDAThrow: the arm rolls shock waves (0xda) */
    BOSSF_PHASE_RIDE = 0x33,      /* BossFPartD8_Action_StartHandRide: the arm lies on the floor, ride it */
    BOSSF_PHASE_RIDE_MOVE = 0x34, /* BossFPartD8_State_StartHandRideMove: the hand swings (crouch to stay) */
    BOSSF_PHASE_RIDE_END = 0x35,  /* BossFPartD8_State_HandRideEnd */
    BOSSF_PHASE_LASER = 0x36,     /* BossFPartD8_Action_StartPartDCVolley: laser volley (0xdc / 0xdd) */
    BOSSF_PHASE_BEAM = 0x37,      /* BossFPartD8_Action_StartPartDEAttack: beam balls (0xde) */
    BOSSF_PHASE_SKY = 0x38,       /* BossFPartD8_Action_StartSkyCharge: last hit point, sky charges */
    BOSSF_PHASE_HAND_HURT = 0x39, /* a fist was hit (hand slam / press / throw) */
    BOSSF_PHASE_HURT = 0x3a,      /* BossFPartD8_Action_Hurt / HurtInSky: the body was hit */
    BOSSF_PHASE_DEFEAT = 0x3b     /* BossFPartD8_Action_Defeat */
};

/* g_boss_hooks.help kinds besides BOSS_HELP_CREATE / BOSS_HELP_DESTROY (boss.h).  BossF uses
 * BossHelp_Create(1) (the hand-ride warning) and, when the hand ride starts,
 * EffEnemyRoomFix_Create(0xb4, 0x30, 0, 0xb4): two HUD sprites (act/ac_eff_enemyroom_fix.bac, animations
 * 0xb / 0xc) at screen (a, 0x30) for b frames.  A help handler that does not know the kind ignores it. */
enum { BOSS_HELP_ENEMY_ROOM_FIX = 2 };

/* gDisplayMain.unk20 / unk24 (BLDCNT / BLDY) as the BossF code sets them: the 3D layer
 * (BG0) is brightened towards white by evy/16 (mode 2) - laser glow, impact flashes,
 * the white-out before the sky phase.  mode 0: off. */
void bossf_blend(s32 *mode, s32 *evy);

/* Boss_LoadModelPalettes state (the palettes themselves are not swapped natively): set 0 all of bsf_body,
 * 1 fist L, 2 fist R, 3 one bsf_body palette.  bossf_palette: the first palette index loaded last (-1 none);
 * bossf_palette_flash: 1 while the flash (hurt blink) palettes are loaded.  An integrator can tint the
 * body model while bossf_palette_flash(0) is set. */
s32 bossf_palette(int set);
int bossf_palette_flash(int set);

/* state for tests / debug */
typedef struct BossFInfo {
    s32 x, y, z;                 /* BossFPartD8 position (24.8) */
    s32 core[3], fist_l[3], fist_r[3], sh_r[3]; /* node translations (fx32 px, world) */
    u16 rot;                     /* unk45E: RotY of the body */
    int boxes_armed;             /* bit i: coll[i] armed (0 body, 1/2 fist L attack, 3 fist L damage,
                                    4/5 fist R attack, 6 fist R damage) */
    ObjBox box[7];
    int cmap_on;                 /* the arm's collision map (bsf_st_r) is registered */
    int riding;                  /* BossFPartD8_IsPlayerRiding */
    int ride_draw;               /* the player is drawn on the hand (BossFPartD8_Draw_RidingPlayer) */
    s32 variant;                 /* unk458 */
    u16 timer;                   /* unk434 (u16): the current state's timer (e.g. the ride warning) */
    int balls, shocks, waves, lasers, flashes, beams; /* D9, DA, DB, DC, DD, DE alive */
    int sky_model;               /* 0 bsf_sky, 1 bsf_sky_b (after the white-out) */
    u32 flags20C;                /* gBossF->unk20C */
    s32 map_right;               /* gStageMapCollision.unk24 */
} BossFInfo;
int bossf_info(BossFInfo *out);
/* per-kind counters (spawned) */
typedef struct BossFStats {
    u32 hand_slams, presses, throws, rides, ride_moves, lasers, beams, sky_charges;
    u32 hand_hits, body_hits, sky_hits, knock_offs, balls, shocks, waves, laser_objs, flashes, beam_objs;
    u32 talks, helps, help_destroys, white_outs;
} BossFStats;
const BossFStats *bossf_stats(void);

#endif

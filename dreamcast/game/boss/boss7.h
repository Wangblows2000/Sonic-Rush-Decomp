/*
 * Boss7 (Dead Line act 3, stage z73 = stage id 20): public API of the native
 * translation in boss7.c (the rival fight: Blaze when the story character is
 * Sonic, Sonic when it is Blaze, then the button-mash clash).  The common boss
 * API (boss.h) drives it like the other bosses: boss_init(20, ...),
 * boss_start, boss_update, boss_draw, boss_health / boss_phase.  This header
 * adds what only this fight has.  See docs/dreamcast/translation/BOSSES.md,
 * section "Boss7".
 *
 * Clash input (Boss7_State_ClashStruggle reads gPad.trg & 3 = DS A | B):
 * on the Dreamcast pad that is A or B (engine/input.c: CONT_A -> IN_JUMP ->
 * DS A, CONT_B -> IN_TRICK -> DS B; main.c ds_pad).  The boss module reads the
 * raw pressed keys the player module stored this frame (DcPlayer.pad_raw[1],
 * filled by dcp_player_frame), so nothing extra has to be wired.  Each new
 * press of A or B counts; holding does not.
 */
#ifndef BOSS7_H
#define BOSS7_H

#include "boss.h"

/* gBossStatus.unk4 (boss_phase) values written by the Boss7 code */
enum {
    BOSS7_PHASE_IDLE = 0x24,      /* Boss7_Create / Boss7PartD5_MoveState_StartIdle */
    BOSS7_PHASE_RUN = 0x25,       /* Boss7PartD5_MoveState_StartRun */
    BOSS7_PHASE_BACKHOP = 0x26,   /* Boss7PartD5_MoveState_StartBackHop */
    BOSS7_PHASE_CROUCH = 0x27,    /* Boss7PartD5_MoveState_StartCrouch */
    BOSS7_PHASE_JUMP = 0x28,      /* Boss7PartD5_MoveState_StartJump */
    BOSS7_PHASE_SPINDASH = 0x29,  /* Boss7PartD5_MoveState_StartSpindashCharge */
    BOSS7_PHASE_ROLL = 0x2a,      /* Boss7PartD5_MoveState_SpindashRelease */
    BOSS7_PHASE_AIRATTACK = 0x2b, /* Boss7PartD5_MoveState_StartAirAttackCharge (homing / fire dash) */
    BOSS7_PHASE_SPECIAL = 0x2c,   /* Boss7PartD5_StartSpecialAttack (tornado / fire pillars) */
    BOSS7_PHASE_HURT = 0x2d       /* Boss7PartD5_MoveState_StartHurt / StartDefeatExit */
};

/* BossHelp_Create(0) at the clash start, BossHelp_Destroy at its end: g_boss_hooks.help
 * (BOSS_HELP_CREATE / BOSS_HELP_DESTROY, boss.h) and boss_help(). */

/* gDisplayMain.unk20 / unk24 (BLDCNT / BLDY) as the Boss7 code sets them: the
 * 3D layer (BG0) is brightened towards white by evy/16 (mode 2): the clash
 * flash, the air-dash flash.  mode 0: off.  (Master brightness, the win
 * cut-ins, stays in boss_brightness().) */
void boss7_blend(s32 *mode, s32 *evy);

/* TalkDemo_Create (gTalkDemoId 0xa when the story character is Sonic, 0xb for
 * Blaze) after the rival is beaten.  On the DS it freezes the stage
 * (System_Freeze) until the dialogue ends: the integrator should stop calling
 * boss_update / the player / objects while the talk runs (cutscene module,
 * CS_TALK).  Called through g_boss_hooks.log("talk", id, 0) as well. */
typedef void (*Boss7TalkFn)(void *ctx, s32 talk_id);
void boss7_set_talk_hook(Boss7TalkFn fn, void *ctx);

typedef struct Boss7Info {
    int rival_chara;              /* unk3D0: 0 Sonic, 1 Blaze */
    s32 rival_x, rival_y, rival_z;/* Boss7PartD5 position (24.8) */
    s32 rival_vx, rival_vy;
    s32 rival_anim;               /* unk3D4 (Boss7_SetRivalAnimation index, before the Blaze offset) */
    u16 rival_disp;               /* disp flags (1 facing left, 0x20 hidden / blinking) */
    int rival_box_armed[2];       /* unk378 (body, OnHit) / unk3A4 (attack) armed */
    ObjBox rival_box[2];
    u16 rival_pad_held, rival_pad_pressed; /* the AI's virtual pad (unk3D8 / unk3DC) */
    const char *rival_move, *rival_input;  /* current move / input state names */
    s32 hurt_timer;               /* unk1F0 */
    u32 flags20C;                 /* gBoss7->unk20C: 2 defeat scene done, 0x10 after-image, 0x20 tornado pull */
    s32 camera_mode;              /* gBoss7->unk210 (StageZoneScript_Zone7Boss case) */
    s32 balance;                  /* unk384: > 0 the player wins the clash (+-0x4000 ends it) */
    s32 power_player, power_rival;/* unk390 / unk38C mash power (0..0x80) */
    s32 pull;                     /* unk22A tornado pull speed */
    int clash;                    /* the clash struggle runs (Boss7_State_ClashStruggle) */
    int won, lost;                /* Boss7_State_ClashWon / Boss7_State_ClashLost ran */
    int pillars, auras;           /* Boss7PartD6 / D7 alive */
    s32 player_anim_speed;        /* gPlayers->unk124->unk114 set by Boss7_State_ClashStart (0x4000 while anim 0x42), else 0x1000 */
    s32 blend_mode, blend_evy;    /* see boss7_blend */
    int base_nodes, base_hidden;  /* bs7_base nodes / hidden by bs7.nsbva (anim 4 negapod for Sonic, 3 eggpod for Blaze) */
    u32 base_diffuse;             /* first bs7_base material diffuse (0xAARRGGBB): grey while the clash darkens the arena */
    int rival_posed, afterimage;  /* the rival model / its after-image were posed this frame */
} Boss7Info;
int boss7_info(Boss7Info *out);

typedef struct Boss7Stats {
    u32 decides, attacks, spindashes, air_attacks, specials, jumps, backhops, crouches, evades, jump_overs, repositions;
    u32 hurts, clash_bounces, pushes, pillars, auras, pull_frames, afterimage_frames;
    u32 mash_player, mash_rival, sparks, talks, helps, help_destroys, fx3d;
} Boss7Stats;
const Boss7Stats *boss7_stats(void);

#endif

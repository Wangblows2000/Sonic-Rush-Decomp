/*
 * Boss1 "Egg Hammer" (Leaf Storm act 3, stage z13): native translation of
 * src/game/Boss/Boss1/ (Boss1_*, Boss1Neck_*, Boss1Hammer_*, EffPebble_*)
 * and src/game/Zone/Leaf_Storm/Boss/BossStage1_*.
 *
 * Every function keeps its original name and body; the per-file Unk structs
 * of the decompilation are merged into Boss1 / Boss1Neck / Boss1Hammer with
 * the original offsets in the field names.  DS services are replaced:
 *   - Task_Create / StageObj_Main      -> three static parts run by boss1_update
 *                                         through boss_obj_main (ids 0x500..0x502)
 *   - Model_* / NNS_G3d*               -> boss_model.c (converted .dcm/.dca)
 *   - node callbacks + NNS_G3dGetCurrentMtx (hammer position from the neck's
 *     "neck_ham" node and the hammer's "hammer" node) -> boss_model_joint_world
 *   - ObjRect_*                         -> dreamcast/game/objects (obj_rect.c)
 *   - Sound / NaviMsg / StageShutdown   -> g_boss_hooks
 *   - palette swaps (Boss1_SetPalettes, Boss1_SetFlashPalette) -> state only
 *     (textures are pre-decoded; see BOSSES.md)
 * The zone-5 variant (Boss1_Unk_ov000_022cc6d4: gGameState.unk1C == 4, the
 * missile batteries) is translated where it shares code; its missile object
 * (Boss5PartD3, map object 0xd3) goes out through g_boss_hooks.missile.
 */
#include <stdio.h>
#include <string.h>
#include "boss_internal.h"


/* ------------------------------------------------------------- parts */
typedef struct Boss1Neck Boss1Neck;
typedef struct Boss1Hammer Boss1Hammer;

typedef struct Boss1 {        /* 0x374 bytes (Boss1_Create) */
    DcPlayer h;               /* StageObj header @0x00..@0x8C */
    BossFn func;              /* @0x68 */
    u32 unk20C;               /* @0x20C bit0 hammer released, bit1 pinch (health 2) */
    Boss1Neck *unk210;        /* @0x210 */
    Boss1Hammer *unk214;      /* @0x214 */
    u16 unk218;               /* @0x218 attack pattern column */
    u16 unk21A;               /* @0x21A attack pattern row */
    s16 unk21C, unk21E;       /* @0x21C @0x21E battery angles (zone 5) */
    u16 unk220, unk222;       /* @0x220 @0x222 timers */
    s16 unk228;               /* @0x228 hammer-roll passes left (sign = direction) */
    u16 unk22A;               /* @0x22A attack speed */
    const BossModelRes *unk22C; /* @0x22C bs1_base (data_ov000_02369ad0.unk0) */
    const BossAnimRes *unk230, *unk234; /* @0x230 bs1.nsbca, @0x234 bs5.nsbca (zone 5) */
    const BossModelRes *unk238; /* @0x238 bs1_sky (data_ov000_02369ad0.unkC) */
    BossModel unk23C;         /* @0x23C model (speed @0x350) */
    int alive;
} Boss1;

struct Boss1Neck {            /* 0x360 bytes (Boss1Neck_Create) */
    DcPlayer h;
    BossFn func;              /* @0x68 */
    u16 unk20C;               /* @0x20C counter / hitbox-done flag / camera shake */
    s16 unk20E;               /* @0x20E */
    u16 unk210;               /* @0x210 */
    s32 unk214;               /* @0x214 missile target x (zone 5) */
    u16 unk218;               /* @0x218 flash counter */
    u16 unk21A;               /* @0x21A MoveTowardX cooldown */
    const BossModelRes *unk21C; /* @0x21C bs1_neck */
    const BossAnimRes *unk220, *unk224; /* @0x220 @0x224 */
    BossModel unk228;         /* @0x228 model: anm @0x30C, flags @0x334, speed @0x33C */
    int alive;
};

struct Boss1Hammer {          /* 0x444 bytes (Boss1Hammer_Create) */
    DcPlayer h;
    BossFn func;              /* @0x68 */
    ObjBox unk20C[4];         /* @0x20C damage box, @0x238 / @0x264 attack boxes, @0x290 push box */
    u16 unk2BC;               /* @0x2BC passes rolled */
    u16 unk2BE;               /* @0x2BE debris counter */
    u16 unk2C0, unk2C2;       /* @0x2C0 slide t, @0x2C2 slide step */
    u16 unk2CC;               /* @0x2CC spin angle */
    s16 unk2CE;               /* @0x2CE spin speed */
    dcm_mtx_t unk2D0;         /* @0x2D0 MtxFx43 read back from the geometry engine */
    const BossModelRes *unk300; /* @0x300 bs1_hammer */
    const BossAnimRes *unk304, *unk308; /* @0x304 @0x308 */
    BossModel unk30C;         /* @0x30C model: rot @0x330, anm @0x3F0, flags @0x418, speed @0x420 */
    int alive;
};
/* box flag fields by original offset */
#define unk216 unk20C[0].flags
#define unk242 unk20C[1].flags
#define unk26E unk20C[2].flags
#define unk29A unk20C[3].flags

static Boss1 s_boss1;
static Boss1Neck s_neck;
static Boss1Hammer s_hammer;
static Boss1 *gBoss1;

/* BossStage1 resources: data_ov000_02369ad0[4] (bs1_base, bs1_neck, bs1_hammer, bs1_sky)
 * and the zone act archive's bs1.nsbca */
static BossModelRes data_ov000_02369ad0[4];
static BossAnimRes s_bs1_anim;
static dcm_mtx_t s_mtx_slot30;  /* geometry-engine matrix stack slot 0x1e (MTX_STORE in the node callbacks) */
static int s_joint_neck_ham = -1, s_joint_hammer = -1;
static int s_pal_flash, s_pal_hurt; /* native: Boss1_SetFlashPalette / Boss1_SetPalettes state */

/* ROM tables (bossdata.bin) */
#define data_ov000_02362b64 BOSS_DATA(0x02362b64, const u8 *)
#define data_ov000_02362b68 BOSS_DATA(0x02362b68, const u8 *)
#define data_ov000_02362b84 BOSS_DATA(0x02362b84, const u16 *)
#define data_ov000_02362b8c BOSS_DATA(0x02362b8c, const u16 *)
#define data_ov000_02362b94 BOSS_DATA(0x02362b94, const s16 *)
#define data_ov000_02362b9c BOSS_DATA(0x02362b9c, const s8(*)[4])
#define data_ov000_02362c64 BOSS_DATA(0x02362c64, const u8(*)[8])
#define data_ov000_02362c84 BOSS_DATA(0x02362c84, const u8(*)[8])
#define data_ov000_02362ca4 BOSS_DATA(0x02362ca4, const u8(*)[8])
#define data_ov000_02362cc4 BOSS_DATA(0x02362cc4, const s16(*)[8])
#define data_ov000_02362ce4 BOSS_DATA(0x02362ce4, const s16(*)[8])
#define data_ov000_02362d04 BOSS_DATA(0x02362d04, const s16(*)[8])
#define data_ov000_02362d24 BOSS_DATA(0x02362d24, const u16(*)[8])
#define data_ov000_02362d44 BOSS_DATA(0x02362d44, const u16(*)[8])
#define data_ov000_02362d64 BOSS_DATA(0x02362d64, const s16(*)[8])
#define data_ov000_02362d84 BOSS_DATA(0x02362d84, const s16(*)[8])
#define data_ov000_02362da4 BOSS_DATA(0x02362da4, const u8(*)[8])

/* forward declarations (original names) */
static void Boss1_State_WaitForStart(void *p);
static void Boss1_State_Idle(void *p);
static void Boss1_State_Hurt(void *p);
static void Boss1_State_Defeat(void *p);
static void Boss1_State_DefeatFade_ov000_022cab0c(void *p);
static void Boss1_State_DefeatCamera2(void *p);
static void Boss1_State_DefeatFade_ov000_022ca9cc(void *p);
static void Boss1_State_DefeatCamera3(void *p);
static void Boss1_State_DefeatFade_ov000_022ca890(void *p);
static void Boss1_State_DefeatExplode(void *p);
static void Boss1_State_DefeatFinish(void *p);
static void Boss1_State_WaitSlam(void *p);
static void Boss1_State_WaitTripleSlam(void *p);
static void Boss1_State_WaitBigSlam(void *p);
static void Boss1_State_WaitHammerRoll(void *p);
static void Boss1_State_WaitMissileAttack(void *p);
static void Boss1_Action_IdleAll(void);
static void Boss1_Action_HurtAll(void);
static void Boss1_Action_DefeatAll(void);
static void Boss1_ResetCamera(void);
static void Boss1_SetCameraSide(s32 dir);
static void Boss1_SetPalettes(s32 idx, s32 flag);
static void Boss1_SetFlashPalette(s32 idx, s32 flag);
static void Boss1_UpdateCameraShake(u16 *p, s32 a, s32 sh);
static dcp_bool Boss1_Unk_ov000_022cc6d4(void);

static void Boss1Neck_State_Idle(void *p);
static void Boss1Neck_State_FollowPlayer(void *p);
static void Boss1Neck_State_Hurt(void *p);
static void Boss1Neck_State_Defeated(void *p);
static void Boss1Neck_State_SlamApproach(void *p);
static void Boss1Neck_State_SlamWindUp(void *p);
static void Boss1Neck_State_SlamHold(void *p);
static void Boss1Neck_State_SlamRaise(void *p);
static void Boss1Neck_State_SlamStrike(void *p);
static void Boss1Neck_State_SlamDown(void *p);
static void Boss1Neck_State_SlamImpact(void *p);
static void Boss1Neck_State_SlamImpactShake(void *p);
static void Boss1Neck_State_SlamRecover(void *p);
static void Boss1Neck_State_SlamRecoverWait(void *p);
static void Boss1Neck_State_SlamDone(void *p);
static void Boss1Neck_State_TripleSlamApproach(void *p);
static void Boss1Neck_State_TripleSlamWindUp(void *p);
static void Boss1Neck_State_TripleSlamHold(void *p);
static void Boss1Neck_State_TripleSlamRaise(void *p);
static void Boss1Neck_State_TripleSlamStrike(void *p);
static void Boss1Neck_State_TripleSlamDown(void *p);
static void Boss1Neck_State_TripleSlamAfterImpact(void *p);
static void Boss1Neck_State_TripleSlamImpactShake(void *p);
static void Boss1Neck_State_TripleSlamStartRewind(void *p);
static void Boss1Neck_State_TripleSlamRewind(void *p);
static void Boss1Neck_State_TripleSlamStartReraiseShake(void *p);
static void Boss1Neck_State_TripleSlamReraiseShake(void *p);
static void Boss1Neck_State_TripleSlamFinalImpact(void *p);
static void Boss1Neck_State_TripleSlamFinalShake(void *p);
static void Boss1Neck_State_TripleSlamRecover(void *p);
static void Boss1Neck_State_TripleSlamRecoverWait(void *p);
static void Boss1Neck_State_TripleSlamDone(void *p);
static void Boss1Neck_State_BigSlamApproach(void *p);
static void Boss1Neck_State_BigSlamWindUp(void *p);
static void Boss1Neck_State_BigSlamHold(void *p);
static void Boss1Neck_State_BigSlamRaise(void *p);
static void Boss1Neck_State_BigSlamStrike(void *p);
static void Boss1Neck_State_BigSlamDown(void *p);
static void Boss1Neck_State_BigSlamImpact(void *p);
static void Boss1Neck_State_BigSlamImpactShake(void *p);
static void Boss1Neck_State_BigSlamRecover(void *p);
static void Boss1Neck_State_BigSlamRecoverWait(void *p);
static void Boss1Neck_State_BigSlamDone(void *p);
static void Boss1Neck_State_RollStart(void *p);
static void Boss1Neck_State_RollMoveToEdge(void *p);
static void Boss1Neck_State_RollReleaseAnim(void *p);
static void Boss1Neck_State_RollReleaseHammer(void *p);
static void Boss1Neck_State_RollStartWait(void *p);
static void Boss1Neck_State_WaitTimer_ov000_022c8b90(void *p);
static void Boss1Neck_State_RollAnim2(void *p);
static void Boss1Neck_State_RollWaitAnim2(void *p);
static void Boss1Neck_State_RollAnim3(void *p);
static void Boss1Neck_State_RollWaitAnim3(void *p);
static void Boss1Neck_State_RollWaitHammer(void *p);
static void Boss1Neck_State_RollReturnAnim1(void *p);
static void Boss1Neck_State_RollReturnWait1(void *p);
static void Boss1Neck_State_RollReturnAnim2(void *p);
static void Boss1Neck_State_RollReturnWait2(void *p);
static void Boss1Neck_State_RollRegrabHammer(void *p);
static void Boss1Neck_State_RollReturnAnim3(void *p);
static void Boss1Neck_State_RollReturnWait3(void *p);
static void Boss1Neck_State_HammerRollDone(void *p);
static void Boss1Neck_State_MissileWaitStart(void *p);
static void Boss1Neck_State_MissileIntroAnim(void *p);
static void Boss1Neck_State_MissileWaitIntro(void *p);
static void Boss1Neck_State_MissileSetPose(void *p);
static void Boss1Neck_State_MissilePoseAnim(void *p);
static void Boss1Neck_State_MissileReadyAnim(void *p);
static void Boss1Neck_State_MissileWaitReady(void *p);
static void Boss1Neck_State_MissileSelectPattern(void *p);
static void Boss1Neck_State_PickRandomTarget(void *p);
static void Boss1Neck_State_AimAtTarget(void *p);
static void Boss1Neck_State_FireAtTarget(void *p);
static void Boss1Neck_State_StartTrackPlayer(void *p);
static void Boss1Neck_State_TrackPlayer(void *p);
static void Boss1Neck_State_FireAtPlayer(void *p);
static void Boss1Neck_State_MissileEndAnim(void *p);
static void Boss1Neck_State_MissileWaitEndAnim(void *p);
static void Boss1Neck_State_MissileRetractAnim(void *p);
static void Boss1Neck_State_MissileWaitRetract(void *p);
static void Boss1Neck_State_MissileResetPose(void *p);
static void Boss1Neck_State_MissileWaitResetPose(void *p);
static void Boss1Neck_State_MissileDone(void *p);

static void Boss1Hammer_State_Idle(void *p);
static void Boss1Hammer_State_Idle_ov000_022c5f90(void *p);
static void Boss1Hammer_State_Idle_ov000_022c5fa4(void *p);
static void Boss1Hammer_State_Idle_ov000_022c5fb8(void *p);
static void Boss1Hammer_State_Idle_ov000_022c6940(void *p);
static void Boss1Hammer_State_Idle_ov000_022c6954(void *p);
static void Boss1Hammer_State_Idle_ov000_022c6968(void *p);
static void Boss1Hammer_State_Hurt(void *p);
static void Boss1Hammer_State_Defeated(void *p);
static void Boss1Hammer_State_WaitRelease(void *p);
static void Boss1Hammer_State_StartFall(void *p);
static void Boss1Hammer_State_Fall(void *p);
static void Boss1Hammer_State_Land(void *p);
static void Boss1Hammer_State_WaitTimer_ov000_022c67ec(void *p);
static void Boss1Hammer_State_StartSpin(void *p);
static void Boss1Hammer_State_SpinUp(void *p);
static void Boss1Hammer_State_LaunchRoll(void *p);
static void Boss1Hammer_State_Roll(void *p);
static void Boss1Hammer_State_TurnAtEdge(void *p);
static void Boss1Hammer_State_SlideToEdge(void *p);
static void Boss1Hammer_State_FinishRoll(void *p);
static void Boss1Hammer_State_SlideBackSpinDown(void *p);
static void Boss1Hammer_State_EndRoll(void *p);

/* ================================================================ Boss1 */

/* Boss1_Unk_ov000_022cc6d4 (copied): zone 5 variant (gGameState.unk1C == 4) */
static dcp_bool Boss1_Unk_ov000_022cc6d4(void) { return g_dcp_world.zone == 4; }

/* Boss1_Unk_ov000_022cc110 (empty in the original) */
static void Boss1_Unk_ov000_022cc110(s32 v) { (void)v; }

/* Boss1_SetPartAnimation (copied) */
static void Boss1_SetPartAnimation(BossModel *a, const BossAnimRes *b, s32 c, s32 d, s32 e) {
    Model_SetAnimation(a, 0, b, (u32)c);
    a->speed = d;          /* unk114 */
    a->flags[0] &= ~4;     /* unk10C */
    if (e)
        a->flags[0] |= 4;
}

/* Boss1_SetPalettes / Boss1_SetFlashPalette: Boss_LoadModelPalettes copies
 * bs1_{neck,ham}_0x_1p[d].nbfp into the models' texture palettes (hurt blink,
 * impact flash).  The Dreamcast textures are pre-decoded ARGB, so only the
 * selection is kept (boss1 state, BOSSES.md "deviations"). */
static void Boss1_SetPalettes(s32 idx, s32 flag) {
    (void)idx;
    s_pal_hurt = flag;
}
static void Boss1_SetFlashPalette(s32 idx, s32 flag) {
    (void)idx;
    s_pal_flash = flag;
}

/* Boss1_UpdateCameraShake (copied) */
static void Boss1_UpdateCameraShake(u16 *p, s32 a, s32 sh) {
    data_ov000_0236a444.unk3C.x = FX_Mul(a * (ScreenShake_GetOffsetX() << 12), (*p << 12) >> sh);
    data_ov000_0236a444.unk3C.y = FX_Mul(a * (ScreenShake_GetOffsetY() << 12), (*p << 12) >> sh);
    if (*p != 0)
        (*p)--;
}

/* Boss1_ResetCamera (copied; c = &data_ov000_0236a48c = &t[0]) */
static void Boss1_ResetCamera(void) {
    BossCamera *c = &data_ov000_0236a444;
    /* *(vu16 *)0x4000304 |= 0x8000: POWCNT display swap, nothing to do */
    c->unkDC = 2;
    c->unkC = 0x1a66000;
    c->unk14 = FX_Div(0x1000, 0x34cc);
    c->t[0].ox = c->t[0].oy = c->t[0].oz = NULL;          /* c->unk0..unk8 */
    c->t[0].offx = c->t[0].offy = c->t[0].offz = 0;       /* c->unkC..unk14 */
    c->t[0].lerp = 0x200;                                  /* c->unk30 */
    c->t[1].ox = gPlayers;                                 /* c->unk3C */
    c->t[1].oy = NULL;                                     /* c->unk40 */
    c->t[1].oz = gPlayers;                                 /* c->unk44 */
    c->t[1].tgt.y = -0x1c0000;                             /* c->unk58 */
    c->t[1].offx = 0;                                      /* c->unk48 */
    c->t[1].offy = 0x28000;                                /* c->unk4C */
    c->t[1].offz = 0;                                      /* c->unk50 */
    c->t[1].lerp = 0x400;                                  /* c->unk6C */
    c->unkC0 = c->unkC4 = 0xa0000;                         /* c->unk78, unk7C */
    c->unkC8 = 0x200;                                      /* c->unk80 */
    c->unkCC = c->unkD0 = -0x14000;                        /* c->unk84, unk88 */
    c->unkD4 = 0x80;                                       /* c->unk8C */
    c->unkD6 = c->unkD8 = 0;                               /* c->unk8E, unk90 */
    c->unkDA = 0x100;                                      /* c->unk92 */
    gPlayers->map_up = 0x40;                               /* unk430 */
    gPlayers->map_down = 0x80;                             /* unk432 */
}

/* Boss1_SetCameraSide (copied) */
static void Boss1_SetCameraSide(s32 dir) {
    data_ov000_0236a444.t[1].offx = 0;      /* unk90 */
    data_ov000_0236a444.t[1].offy = 0;      /* unk94 */
    data_ov000_0236a444.t[1].offz = 0;      /* unk98 */
    data_ov000_0236a444.unkC0 = 0x8c000;
    data_ov000_0236a444.unkCC = 0xf000;
    if (dir > 0) {
        data_ov000_0236a444.unkD6 = 0xcaaa;
        gPlayers->map_up = 0x10;
        gPlayers->map_down = 0x20;
    } else {
        data_ov000_0236a444.unkD6 = 0x3555;
        gPlayers->map_up = 0x20;
        gPlayers->map_down = 0x10;
    }
}

/* defeat cameras: shared body of Boss1_State_Defeat / DefeatCamera2 / DefeatCamera3 / DefeatExplode */
static void defeat_camera(fx32 tx, fx32 ty, fx32 tz, fx32 dist, fx32 h, u16 ang) {
    BossCamera *c = &data_ov000_0236a444;
    c->unkDC = 2;
    c->t[0].lerp = 0x1000;                                         /* c->unk30 */
    c->t[1].ox = c->t[1].oy = c->t[1].oz = NULL;                   /* c->unk3C..44 */
    c->t[1].offx = c->t[1].offy = c->t[1].offz = 0;                /* c->unk48..50 */
    c->t[1].tgt.x = tx;                                            /* c->unk54 */
    c->t[1].tgt.y = ty;                                            /* c->unk58 */
    c->t[1].tgt.z = tz;                                            /* c->unk5C */
    c->t[1].lerp = 0x1000;                                         /* c->unk6C */
    c->unkC0 = dist;                                               /* c->unk78 */
    c->unkC8 = 0x1000;                                             /* c->unk80 */
    c->unkCC = h;                                                  /* c->unk84 */
    c->unkD4 = 0x1000;                                             /* c->unk8C */
    c->unkD6 = ang;                                                /* c->unk8E */
    c->unkDA = 0x1000;                                             /* c->unk92 */
}

/* Boss1_Draw: Model_Draw of bs1_base (no animation) */
static void Boss1_Draw(void *p) {
    Boss1 *s = (Boss1 *)p;
    if (s->h.disp_flags & 0x20)
        return;
    boss_model_pose(&s->unk23C, NULL);
}

static void Boss1_Destructor(void *p) {
    Boss1 *s = (Boss1 *)p;
    boss_model_release(&s->unk23C);
}

/* Boss1_IsXDone (copied) */
static dcp_bool Boss1_IsBigSlamDone(void) { return (BossFn)Boss1Neck_State_BigSlamDone == gBoss1->unk210->func; }
static dcp_bool Boss1_IsHammerRollDone(void) { return (BossFn)Boss1Neck_State_HammerRollDone == gBoss1->unk210->func; }
static dcp_bool Boss1_IsMissileAttackDone(void) { return (BossFn)Boss1Neck_State_MissileDone == gBoss1->unk210->func; }
static dcp_bool Boss1_IsSlamDone(void) { return (BossFn)Boss1Neck_State_SlamDone == gBoss1->unk210->func; }
static dcp_bool Boss1_IsTripleSlamDone(void) { return (BossFn)Boss1Neck_State_TripleSlamDone == gBoss1->unk210->func; }

/* Boss1_Action_Wait* (copied) */
static void Boss1_Action_WaitBigSlam(Boss1 *s) {
    gBossStatus.unk4 = 3;
    s->func = Boss1_State_WaitBigSlam;
}
static void Boss1_Action_WaitHammerRoll(Boss1 *s) {
    gBossStatus.unk4 = 4;
    s->func = Boss1_State_WaitHammerRoll;
}
static void Boss1_Action_WaitMissileAttack(Boss1 *s) {
    gBossStatus.unk4 = 0x1b;
    s->func = Boss1_State_WaitMissileAttack;
}
static void Boss1_Action_WaitSlam(Boss1 *s) {
    gBossStatus.unk4 = 2;
    s->func = Boss1_State_WaitSlam;
}
static void Boss1_Action_WaitTripleSlam(Boss1 *s) {
    gBossStatus.unk4 = 2;
    s->func = Boss1_State_WaitTripleSlam;
}

/* part actions used by the Boss1_Action_* fan-outs (copied) */
static void Boss1Neck_Action_Defeat(Boss1Neck *s) { s->func = Boss1Neck_State_Defeated; }
static void Boss1Neck_Action_FollowPlayer(Boss1Neck *s) { s->func = Boss1Neck_State_FollowPlayer; }
static void Boss1Neck_Action_Hurt(Boss1Neck *s) { s->func = Boss1Neck_State_Hurt; }
static void Boss1Neck_Action_StartBigSlam(Boss1Neck *s) { s->func = Boss1Neck_State_BigSlamApproach; }
static void Boss1Neck_Action_StartHammerRoll(Boss1Neck *s) { s->func = Boss1Neck_State_RollStart; }
static void Boss1Neck_Action_StartMissileAttack(Boss1Neck *s) {
    s->h.vel_x = 0;
    s->func = Boss1Neck_State_MissileWaitStart;
}
static void Boss1Neck_Action_StartSlam(Boss1Neck *s) { s->func = Boss1Neck_State_SlamApproach; }
static void Boss1Neck_Action_StartTripleSlam(Boss1Neck *s) {
    s->unk210 = 0;
    s->func = Boss1Neck_State_TripleSlamApproach;
}
static void Boss1Hammer_Action_Defeat(Boss1Hammer *s) { s->func = Boss1Hammer_State_Defeated; }
static void Boss1Hammer_Action_Hurt(Boss1Hammer *s) { s->func = Boss1Hammer_State_Hurt; }
static void Boss1Hammer_Action_Idle_ov000_022c5f94(Boss1Hammer *s) { s->func = Boss1Hammer_State_Idle_ov000_022c5f90; }
static void Boss1Hammer_Action_Idle_ov000_022c5fa8(Boss1Hammer *s) { s->func = Boss1Hammer_State_Idle_ov000_022c5fa4; }
static void Boss1Hammer_Action_Idle_ov000_022c6944(Boss1Hammer *s) { s->func = Boss1Hammer_State_Idle_ov000_022c6940; }
static void Boss1Hammer_Action_Idle_ov000_022c6958(Boss1Hammer *s) { s->func = Boss1Hammer_State_Idle_ov000_022c6954; }
static void Boss1Hammer_Action_Idle_ov000_022c696c(Boss1Hammer *s) { s->func = Boss1Hammer_State_Idle_ov000_022c6968; }
static void Boss1Hammer_Action_StartRoll(Boss1Hammer *s) {
    s->unk2BC = 0;
    s->func = Boss1Hammer_State_WaitRelease;
}

/* Boss1_Action_Start* (copied) */
static void Boss1_Action_StartBigSlam(u16 v) {
    gBoss1->unk22A = v;
    Boss1_Action_WaitBigSlam(gBoss1);
    Boss1Neck_Action_StartBigSlam(gBoss1->unk210);
    Boss1Hammer_Action_Idle_ov000_022c6944(gBoss1->unk214);
    g_boss_stats.big_slams++;
    boss_log("attack_bigslam", v, gBossStatus.unk8);
}
static void Boss1_Action_StartHammerRoll(s32 p0) {
    gBoss1->unk228 = (s16)p0;
    Boss1_Action_WaitHammerRoll(gBoss1);
    Boss1Neck_Action_StartHammerRoll(gBoss1->unk210);
    Boss1Hammer_Action_StartRoll(gBoss1->unk214);
    g_boss_stats.rolls++;
    boss_log("attack_roll", p0, gBossStatus.unk8);
}
static void Boss1_Action_StartMissileAttack(u16 v) {
    gBoss1->unk22A = v;
    Boss1_Action_WaitMissileAttack(gBoss1);
    Boss1Neck_Action_StartMissileAttack(gBoss1->unk210);
    Boss1Hammer_Action_Idle_ov000_022c5f94(gBoss1->unk214);
    g_boss_stats.missiles++;
    boss_log("attack_missile", v, gBossStatus.unk8);
}
static void Boss1_Action_StartSlam(u16 v) {
    gBoss1->unk22A = v;
    Boss1_Action_WaitSlam(gBoss1);
    Boss1Neck_Action_StartSlam(gBoss1->unk210);
    Boss1Hammer_Action_Idle_ov000_022c6958(gBoss1->unk214);
    g_boss_stats.slams++;
    boss_log("attack_slam", v, gBossStatus.unk8);
}
static void Boss1_Action_StartTripleSlam(u16 v) {
    gBoss1->unk22A = v;
    Boss1_Action_WaitTripleSlam(gBoss1);
    Boss1Neck_Action_StartTripleSlam(gBoss1->unk210);
    Boss1Hammer_Action_Idle_ov000_022c5fa8(gBoss1->unk214);
    g_boss_stats.triple_slams++;
    boss_log("attack_tripleslam", v, gBossStatus.unk8);
}

/* Boss1_Action_Idle (copied) */
static void Boss1_Action_Idle(Boss1 *s) {
    Boss1Hammer *p = s->unk214;
    Boss1_SetPartAnimation(&s->unk210->unk228, s->unk210->unk220, 0xd, 0x1000, 1);
    Boss1_SetPartAnimation(&p->unk30C, p->unk304, 0, 0x1000, 1);
    s->unk220 = (u16)((Rand() & 0x3f) + 0x1e);
    Boss1_ResetCamera();
    p->unk216 &= ~4;
    p->unk242 &= ~4;
    p->unk26E &= ~4;
    p->unk29A &= ~4;
    gBossStatus.unk4 = 1;
    s->func = Boss1_State_Idle;
}
static void Boss1_Action_IdleAll(void) {
    Boss1_Action_Idle(gBoss1);
    Boss1Neck_Action_FollowPlayer(gBoss1->unk210);
    Boss1Hammer_Action_Idle_ov000_022c696c(gBoss1->unk214);
}

/* Boss1_Action_Hurt (copied) */
static void Boss1_Action_Hurt(Boss1 *s) {
    gBossStatus.unk4 = 5;
    Boss1_SetPartAnimation(&s->unk210->unk228, s->unk210->unk220, 0x1e, 0x1000, 0);
    Boss1_SetPartAnimation(&s->unk214->unk30C, s->unk214->unk304, 0xb, 0x1000, 0);
    s->unk214->unk216 &= ~4;
    s->unk214->unk29A &= ~4;
    s->unk214->unk242 &= ~4;
    Boss1_ResetCamera();
    ScreenShake_Start(0);
    Boss1_Unk_ov000_022cc110(0x1000);
    Sound_PlaySeqArc(0, 0, 0x7b);
    Sound_PlayVoice(0, 0, (Rand() & 1) ? 0xce : 0xcf);
    s->unk220 = 0;
    s->unk222 = 0;
    s->func = Boss1_State_Hurt;
}
static void Boss1_Action_HurtAll(void) {
    Boss1_Action_Hurt(gBoss1);
    Boss1Neck_Action_Hurt(gBoss1->unk210);
    Boss1Hammer_Action_Hurt(gBoss1->unk214);
}

/* Player_State_Blank / Player_Action_Blank (player module lacks them) */
static void Player_State_Blank(DcPlayer *p) { (void)p; }
static void Player_Action_Blank(DcPlayer *s) {
    dcp_player_init_state(s);
    dcp_reset_gimmick_state(s);
    s->state = Player_State_Blank;
}

/* Boss1_Action_Defeat (copied) */
static void Boss1_Action_Defeat(Boss1 *s) {
    Boss1Neck *p = s->unk210;
    gBossStatus.unk4 = 6;
    s->unk214->unk216 &= ~4;
    s->unk214->unk29A &= ~4;
    s->unk214->unk242 &= ~4;
    ScreenShake_Start(0);
    s->h.obj_flags |= 0x20;
    s->unk210->h.obj_flags |= 0x20;
    s->unk214->h.obj_flags |= 0x20;
    s->unk23C.speed = 0;            /* unk350 */
    s->unk210->unk228.speed = 0;    /* unk33C */
    s->unk214->unk30C.speed = 0;    /* unk420 */
    StageObj_SetPause();
    g_dcp_world.flags &= ~1u;       /* gGameState.unk18 &= ~1: stop the timer */
    p->h.x = 0xe100;
    Boss1_SetFlashPalette(g_boss_game.unk0, 0);
    Player_Action_Blank(gPlayers);
    dcp_set_anim(gPlayers, 0x30);   /* Player_SetAnimation */
    gPlayers->invuln = 0;           /* unk3B2 */
    gPlayers->x = 0xe100;
    gPlayers->y = 0x1a800;
    if (g_boss_hooks.ring_flags)    /* RingManager_SetFlags(RingManager_GetFlags() | 1) */
        g_boss_hooks.ring_flags(g_boss_hooks.ctx, 1);
    if (g_boss_hooks.music_stop)    /* NNS_SndPlayerStopSeqAll(0) */
        g_boss_hooks.music_stop(g_boss_hooks.ctx);
    s->func = Boss1_State_Defeat;
    boss_log("defeat", 0, 0);
}
static void Boss1_Action_DefeatAll(void) {
    Boss1_Action_Defeat(gBoss1);
    Boss1Neck_Action_Defeat(gBoss1->unk210);
    Boss1Hammer_Action_Defeat(gBoss1->unk214);
}

/* Boss1_SelectNextAttack (copied) */
static inline s32 GetIdx(void) {
    dcp_bool b = DCP_TRUE;
    if (g_dcp_world.mode != 2 && g_dcp_world.mode != 3)
        b = DCP_FALSE;
    if (b)
        return 1;
    return g_boss_game.unk14;
}
static void Boss1_SelectNextAttack(void) {
    Boss1 *g = gBoss1;
    g_boss_stats.attacks++;
    if (gBossStatus.unk8 >= 6) {
        switch (data_ov000_02362da4[g->unk21A][g->unk218]) {
        case 0:
            if (Boss1_Unk_ov000_022cc6d4()) {
                if ((Rand() & 0xf) > 4)
                    Boss1_Action_StartSlam((u16)data_ov000_02362d64[GetIdx()][Rand() & 7]);
                else
                    Boss1_Action_StartTripleSlam((u16)data_ov000_02362d64[GetIdx()][Rand() & 7]);
            } else {
                Boss1_Action_StartSlam((u16)data_ov000_02362d64[GetIdx()][Rand() & 7]);
            }
            break;
        case 1:
            Boss1_Action_StartBigSlam((u16)data_ov000_02362d84[GetIdx()][Rand() & 7]);
            break;
        }
    } else if (gBossStatus.unk8 >= 3) {
        u8 k;
        if (Boss1_Unk_ov000_022cc6d4())
            k = data_ov000_02362c84[g->unk21A][g->unk218];
        else
            k = data_ov000_02362c64[g->unk21A][g->unk218];
        switch (k) {
        case 0:
            if (Boss1_Unk_ov000_022cc6d4()) {
                if ((Rand() & 0xf) > 8)
                    Boss1_Action_StartSlam((u16)data_ov000_02362d64[GetIdx()][Rand() & 7]);
                else
                    Boss1_Action_StartTripleSlam((u16)data_ov000_02362d64[GetIdx()][Rand() & 7]);
            } else {
                Boss1_Action_StartSlam((u16)data_ov000_02362d64[GetIdx()][Rand() & 7]);
            }
            break;
        case 1:
            Boss1_Action_StartBigSlam((u16)data_ov000_02362d84[GetIdx()][Rand() & 7]);
            break;
        case 2:
            if (Boss1_Unk_ov000_022cc6d4())
                Boss1_Action_StartMissileAttack(data_ov000_02362b64[Rand() & 3]);
            else
                Boss1_Action_StartHammerRoll(data_ov000_02362b9c[GetIdx()][Rand() & 3]);
            break;
        }
    } else {
        switch (data_ov000_02362ca4[g->unk21A][g->unk218]) {
        case 0:
            if (Boss1_Unk_ov000_022cc6d4()) {
                if ((Rand() & 0xf) > 10)
                    Boss1_Action_StartSlam((u16)data_ov000_02362d64[GetIdx()][Rand() & 7]);
                else
                    Boss1_Action_StartTripleSlam((u16)data_ov000_02362d64[GetIdx()][Rand() & 7]);
            } else {
                Boss1_Action_StartSlam((u16)data_ov000_02362d64[GetIdx()][Rand() & 7]);
            }
            break;
        case 1:
            Boss1_Action_StartBigSlam((u16)data_ov000_02362d84[GetIdx()][Rand() & 7]);
            break;
        case 2:
            if (Boss1_Unk_ov000_022cc6d4())
                Boss1_Action_StartMissileAttack(data_ov000_02362b68[Rand() & 3]);
            else
                Boss1_Action_StartHammerRoll(data_ov000_02362cc4[GetIdx()][Rand() & 7]);
            break;
        }
    }
    g->unk218++;
    if (g->unk218 >= 8) {
        g->unk218 = 0;
        g->unk21A = Rand() & 3;
    }
}

/* ---------------------------------------------------- Boss1 states */
static void Boss1_State_WaitForStart(void *p) {
    Boss1 *s = (Boss1 *)p;
    BossModel *a;
    BossCamera *c = &data_ov000_0236a444;
    Boss1Neck *p1;
    Boss1Hammer *p2;
    if (!(gBossStatus.unk0 & 1))
        return;
    a = &data_ov000_0236a444.unkE0;   /* data_ov000_0236a524 */
    p1 = s->unk210;
    p2 = s->unk214;
    s->h.disp_flags &= ~0x20;
    p1->h.disp_flags &= ~0x20;
    p2->h.disp_flags &= ~0x20;
    data_ov000_0236a444.unk0 |= 1;
    Model_Init(a, 0);
    Model_SetResource(a, s->unk238);
    a->scale[0] = a->scale[1] = a->scale[2] = 0x1355b50;
    Boss1_ResetCamera();
    c->t[0].cur.x = 0xc0000;       /* c->unk24 */
    c->t[0].cur.y = -0x1d4000;
    c->t[0].cur.z = 0xa0000;
    c->t[0].tgt = c->t[0].cur;     /* c->unk18 */
    c->t[1].cur.x = 0xc0000;       /* c->unk60 */
    c->t[1].cur.y = -0x1c0000;
    c->t[1].cur.z = 0;
    c->t[1].tgt = c->t[1].cur;     /* c->unk54 */
    NaviMsg_SetFaceAnim(9);
    NaviMsg_ShowMessage(0xf, 0xf0);
    Boss1_Action_IdleAll();
    boss_log("start", gBossStatus.unk8, 0);
}

static void Boss1_State_Idle(void *p) {
    Boss1 *s = (Boss1 *)p;
    if (s->unk220 != 0) {
        s->unk220--;
        return;
    }
    Boss1_SelectNextAttack();
}

static void Boss1_State_WaitBigSlam(void *p) {
    (void)p;
    if (Boss1_IsBigSlamDone() == 0)
        return;
    Boss1_Action_IdleAll();
}
static void Boss1_State_WaitHammerRoll(void *p) {
    (void)p;
    if (Boss1_IsHammerRollDone() == 0)
        return;
    Boss1_Action_IdleAll();
}
static void Boss1_State_WaitMissileAttack(void *p) {
    (void)p;
    if (Boss1_IsMissileAttackDone() == 0)
        return;
    Boss1_Action_IdleAll();
}
static void Boss1_State_WaitSlam(void *p) {
    (void)p;
    if (Boss1_IsSlamDone() == 0)
        return;
    Boss1_Action_IdleAll();
}
static void Boss1_State_WaitTripleSlam(void *p) {
    (void)p;
    if (Boss1_IsTripleSlamDone() == 0)
        return;
    Boss1_Action_IdleAll();
}

static void Boss1_State_Hurt(void *p) {
    Boss1 *s = (Boss1 *)p;
    if (s->unk222 < 0x2d || (s->unk220 & 0x8000)) {
        s->unk222++;
        s->unk220++;
        if (!(s->unk220 & 1)) {
            if (s->unk220 & 0x8000)
                Boss1_SetPalettes(g_boss_game.unk0, 0);
            else
                Boss1_SetPalettes(g_boss_game.unk0, 1);
            s->unk220 &= ~1;
            s->unk220 ^= 0x8000;
        }
    }
    if (s->unk210->unk228.flags[0] & 2) /* unk334 & 2 */
        Boss1_Action_IdleAll();
}

static void Boss1_State_Defeat(void *p) {
    Boss1 *s = (Boss1 *)p;
    defeat_camera(gPlayers->x << 4, -gPlayers->y << 4, gPlayers->z << 4, 0x60000, 0x20000, 0x2000);
    Boss_DrawExcBackground(6);
    g_boss_game.bright = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7c);
    s->unk220 = 0;
    s->func = Boss1_State_DefeatFade_ov000_022cab0c;
}
static void Boss1_State_DefeatCamera2(void *p) {
    Boss1 *s = (Boss1 *)p;
    defeat_camera(gPlayers->x << 4, -gPlayers->y << 4, gPlayers->z << 4, 0xa0000, -0x10000, 0xe000);
    Boss_DrawExcBackground(7);
    g_boss_game.bright = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7c);
    s->unk220 = 0;
    s->func = Boss1_State_DefeatFade_ov000_022ca9cc;
}
static void Boss1_State_DefeatCamera3(void *p) {
    Boss1 *s = (Boss1 *)p;
    defeat_camera(gPlayers->x << 4, -gPlayers->y << 4, gPlayers->z << 4, 0x60000, 0, 0);
    Boss_DrawExcBackground(6);
    g_boss_game.bright = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7c);
    s->unk220 = 0;
    s->func = Boss1_State_DefeatFade_ov000_022ca890;
}
/* Boss1_State_DefeatFade_* (copied, gDisplayMain.unk48 -> g_boss_game.bright) */
static void Boss1_State_DefeatFade_ov000_022ca890(void *p) {
    Boss1 *s = (Boss1 *)p;
    s32 t1;
    if (g_boss_game.bright > 0)
        g_boss_game.bright -= 2;
    t1 = s->unk220++;
    if ((u32)t1 > 0x1e)
        s->func = Boss1_State_DefeatExplode;
}
static void Boss1_State_DefeatFade_ov000_022ca9cc(void *p) {
    Boss1 *s = (Boss1 *)p;
    s32 t1;
    if (g_boss_game.bright > 0)
        g_boss_game.bright -= 2;
    t1 = s->unk220++;
    if ((u32)t1 > 0x1e)
        s->func = Boss1_State_DefeatCamera3;
}
static void Boss1_State_DefeatFade_ov000_022cab0c(void *p) {
    Boss1 *s = (Boss1 *)p;
    s32 t1;
    if (g_boss_game.bright > 0)
        g_boss_game.bright -= 2;
    t1 = s->unk220++;
    if ((u32)t1 > 0x1e)
        s->func = Boss1_State_DefeatCamera2;
}

static void Boss1_State_DefeatExplode(void *p) {
    Boss1 *s = (Boss1 *)p;
    Boss1_SetPartAnimation(&s->unk210->unk228, s->unk210->unk220, 0x1f, 0x1000, 0);
    Boss1_SetPartAnimation(&s->unk214->unk30C, s->unk214->unk304, 0xc, 0x1000, 0);
    s->unk20C |= 1;
    MTX_Identity33(&s->unk214->unk30C.rot);   /* unk330 */
    defeat_camera(0xe1000, -0x180000, 0, 0x12c000, -0x80000, 0xfaab);
    g_boss_game.exc_bg = -1;                  /* REG_DISPCNT &= ~BG2 */
    s->h.obj_flags &= ~0x20;
    s->unk210->h.obj_flags &= ~0x20;
    s->unk214->h.obj_flags &= ~0x20;
    s->unk23C.speed = 0x1000;
    s->unk210->unk228.speed = 0x1000;
    s->unk214->unk30C.speed = 0x1000;
    StageObj_ClearPause();
    gPlayers->disp_flags |= 0x20;
    gPlayers->pf |= 0x200000;
    gPlayers->held = gPlayers->pressed = gPlayers->released = 0;
    if (Boss1_Unk_ov000_022cc6d4()) {
        /* BossEffect_CreateBlast5 (zone 5 archive): not loaded */
        boss_log("effect_blast5", s->unk210->h.x >> 8, s->unk210->h.y >> 8);
    } else {
        BossEffect *obj = BossEffect_CreateBlast1(0, 0xe100, 0x20000, 0);
        obj->unk134.speed = 0x800;            /* obj + 0x248 */
    }
    NaviMsg_SetFaceAnim(0xd);
    NaviMsg_ShowMessage(0x13, 0xf0);
    g_boss_game.bright = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7d);
    s->unk220 = 0;
    s->func = Boss1_State_DefeatFinish;
}

static void Boss1_State_DefeatFinish(void *p) {
    Boss1 *s = (Boss1 *)p;
    Boss1Neck *n;
    s->unk220++;
    if ((s->unk220 & 3) == 0) {
        if (g_boss_game.bright > 0)
            g_boss_game.bright--;
    }
    n = s->unk210;
    if (!(n->unk228.flags[0] & 2))
        return;
    if (s->unk220 <= 0x12c)
        return;
    n->h.obj_flags |= 4;
    s->unk214->h.obj_flags |= 4;
    g_obj_stage.ring_bonus = gPlayers->rings * 1000; /* data_022c4590.unk10 */
    if (!g_boss_game.cleared) {                      /* StageShutdown_Create(Stage_OnBossCleared) */
        g_boss_game.cleared = 1;
        boss_log("cleared", gPlayers->rings, 0);
        if (g_boss_hooks.cleared)
            g_boss_hooks.cleared(g_boss_hooks.ctx);
    }
}

/* ============================================================ Boss1Neck */

/* Boss1Neck_IsPlayerInSlamRange (copied) */
static dcp_bool Boss1Neck_IsPlayerInSlamRange(s32 x, Boss1Hammer *s) {
    dcp_bool r = DCP_FALSE;
    s32 d = x - s->h.z;
    if (d < 0)
        d = -d;
    if (d < 0x10000) {
        d = 0x20000 - s->h.y;
        if (d < 0)
            d = -d;
        if (d < 0xb400)
            r = DCP_TRUE;
    }
    return r;
}

/* Boss1Neck_UpdateSlamHitboxes (copied; the ARM code returns the range test
 * in r0 and the slam states read it) */
static dcp_bool Boss1Neck_UpdateSlamHitboxes(u32 mask) {
    Boss1Hammer *o = gBoss1->unk214;
    dcp_bool r = Boss1Neck_IsPlayerInSlamRange(gPlayers->z, o);
    u32 i;
    ObjBox *p;
    for (i = 0, p = o->unk20C; i < 4; i++, p++) {
        if (mask & (1u << i)) {
            if (r)
                p->flags |= 4;
            else
                p->flags &= ~4;
        }
    }
    return r;
}

/* Boss1Neck_MoveTowardX (copied) */
static void Boss1Neck_MoveTowardX(Boss1Neck *w, s32 target, s32 range, s32 max, s16 accel, s32 decel, s16 lim) {
    s32 d;
    s32 v = w->h.vel_x;
    s32 x = w->h.x;
    d = target - x;
    if (max > range)
        range = max;
    if (v == 0) {
        if (w->unk21A == 0) {
            if (d < 0)
                d = -d;
            if (range < d) {
                if (x < target)
                    w->h.vel_x = (s16)max;
                else
                    w->h.vel_x = (s16)-max;
            }
        }
    } else {
        if (!((x < target && v < 0) || (x > target && v > 0))) {
            if (d < 0)
                d = -d;
            if (range < d)
                goto accelerate;
        }
        if (v < 0) {
            if (decel < -v)
                w->h.vel_x += decel;
            else
                w->h.vel_x = 0;
        } else if (v > 0) {
            if (decel < v)
                w->h.vel_x -= decel;
            else
                w->h.vel_x = 0;
        }
        if (w->h.vel_x == 0)
            w->unk21A = (u16)((Rand() & 0x1f) + 0x1e);
        goto end;
    accelerate:
        if (v > 0)
            w->h.vel_x += accel;
        else
            w->h.vel_x -= accel;
        w->h.vel_x = w->h.vel_x < -lim ? -lim : (w->h.vel_x > lim ? lim : w->h.vel_x);
    }
end:
    if (w->h.x < 0x400) {
        w->h.x = 0x400;
        w->h.vel_x = 0;
    }
    if (w->h.x > 0x1be00) {
        w->h.x = 0x1be00;
        w->h.vel_x = 0;
    }
    if (w->unk21A != 0)
        w->unk21A--;
}

/* Boss1Neck_MoveToX / TrackPlayerFast / Medium / Slow (copied) */
static void Boss1Neck_MoveToX(Boss1Neck *p0, s32 p1) {
    p0->unk21A = 0;
    Boss1Neck_MoveTowardX(p0, p1, 0xc00, 0x180, 0x60, 0x500, 0x500);
}
static void Boss1Neck_TrackPlayerFast(Boss1Neck *p0) {
    Boss1Neck_MoveTowardX(p0, gPlayers->x, 0x2000, 0x100, 0x20, 0x40, 0x400);
}
static void Boss1Neck_TrackPlayerMedium(Boss1Neck *p0) {
    p0->unk21A = 0;
    Boss1Neck_MoveTowardX(p0, gPlayers->x, 0x1000, 0x100, 0x20, 0x40, 0x400);
}
static void Boss1Neck_TrackPlayerSlow(Boss1Neck *p0) {
    p0->unk21A = 0;
    Boss1Neck_MoveTowardX(p0, gPlayers->x, 0x800, 0x180, 0x40, 0x40, 0x500);
}

/* frames left in the neck's animation: ((anm->res->numFrame << 12) - anm->frame) >> 12 */
static inline s32 anim_left(const BossModel *m) {
    const BossAnmObj *q = m->anm[0];
    return q ? ((q->numFrames << 12) - q->frame) >> 12 : 0;
}
static inline s32 anim_left_fx(const BossModel *m) {
    const BossAnmObj *q = m->anm[0];
    return q ? (q->numFrames << 12) - q->frame : 0;
}
/* Raise states: anm->frame = (numFrame << 12) * k * k (loop "n = 1; do .. while (n-- != 0)") */
static void set_raise_frame(BossModel *m, s32 k) {
    BossAnmObj *q = m->anm[0];
    s32 r, n = 1;
    if (q == NULL)
        return;
    r = q->numFrames << 12;
    do {
        r = (s32)(((s64)r * k + 0x800) >> 12);
    } while (n-- != 0);
    q->frame = r;
}
/* data_ov000_02362d04 / 02362ce4 [GetIdx()][health - 1] and 02362d44 / 02362d24 */
static s32 impact_speed(void) {
    if (Boss1_Unk_ov000_022cc6d4())
        return data_ov000_02362d04[GetIdx()][gBossStatus.unk8 - 1];
    return data_ov000_02362ce4[GetIdx()][gBossStatus.unk8 - 1];
}
static u32 slam_track_frames(void) {
    if (Boss1_Unk_ov000_022cc6d4())
        return data_ov000_02362d44[GetIdx()][gBossStatus.unk8 - 1];
    return data_ov000_02362d24[GetIdx()][gBossStatus.unk8 - 1];
}

static void Boss1Neck_State_Idle(void *p) { (void)p; }
static void Boss1Neck_State_Hurt(void *p) { (void)p; }
static void Boss1Neck_State_Defeated(void *p) { (void)p; }
static void Boss1Neck_State_HammerRollDone(void *p) { (void)p; }
static void Boss1Neck_State_MissileDone(void *p) { (void)p; }
static void Boss1Neck_State_RollWaitHammer(void *p) { (void)p; }
static void Boss1Neck_State_FollowPlayer(void *p) { Boss1Neck_TrackPlayerFast((Boss1Neck *)p); }

/* ------------------------------------------------------------- slam */
static void Boss1Neck_State_SlamApproach(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    Boss1Neck_TrackPlayerFast(s);
    if (!(s->unk228.flags[0] & 2))
        return;
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0xe, 0x1000, 0);
    Boss1_SetPartAnimation(&h->unk30C, h->unk304, 1, 0x1000, 0);
    Sound_PlaySeqArc(0, 0, 0x7e);
    Sound_PlayVoice(0, 0, 0xd0);
    s->func = Boss1Neck_State_SlamWindUp;
}
static void Boss1Neck_State_SlamWindUp(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Neck_TrackPlayerMedium(s);
    if (s->unk228.flags[0] & 2)
        s->func = Boss1Neck_State_SlamHold;
}
static void Boss1Neck_State_SlamHold(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0xf, 0x1000, 0);
    s->unk228.flags[0] |= 1;   /* unk334 |= 1 */
    Boss1_SetPartAnimation(&h->unk30C, h->unk304, 2, 0x1000, 0);
    h->unk30C.flags[0] |= 1;   /* unk418 |= 1 */
    s->unk20C = 0;
    s->func = Boss1Neck_State_SlamRaise;
    s->func(s);
}
static void Boss1Neck_State_SlamRaise(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    Boss1Neck_TrackPlayerMedium(s);
    s->unk20C += gBoss1->unk22A;
    if ((s32)s->unk20C >= 0x1000) {
        s->h.vel_x = 0;
        s->unk20C = 0x1000;
        s->func = Boss1Neck_State_SlamStrike;
    }
    set_raise_frame(&s->unk228, (s16)s->unk20C);
    set_raise_frame(&h->unk30C, (s16)s->unk20C);
}
static void Boss1Neck_State_SlamStrike(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0x10, 0x1000, 0);
    Boss1_SetPartAnimation(&h->unk30C, h->unk304, 3, 0x1000, 0);
    Sound_PlaySeqArc(0, 0, 0x81);
    s->unk20C = 0;
    s->func = Boss1Neck_State_SlamDown;
    s->func(s);
}
static void Boss1Neck_State_SlamDown(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    u32 v;
    BossEffect *obj;
    Boss1_Unk_ov000_022cc6d4();
    v = slam_track_frames();
    if ((s32)v >= anim_left(&s->unk228))
        s->h.vel_x = 0;
    else
        Boss1Neck_TrackPlayerMedium(s);
    if (s->unk20C == 0) {
        if (Boss1Neck_UpdateSlamHitboxes(2))
            s->unk20C = 1;
    }
    if (!(s->unk228.flags[0] & 2))
        return;
    ScreenShake_Start(0xb);
    obj = BossEffect_CreateImpactSmall(1, gBoss1->unk214->h.x, 0x20000, 0);
    obj->unk134.speed = 0x1000;
    obj->unk134.scale[0] = 0x34cc;
    obj->unk134.scale[1] = 0x34cc;
    obj->unk134.scale[2] = 0x34cc;
    Sound_PlaySeqArc(0, 0, 0x7f);
    s->func = Boss1Neck_State_SlamImpact;
    boss_log("slam_impact", gBoss1->unk214->h.x >> 8, gPlayers->x >> 8);
}
static void Boss1Neck_State_SlamImpact(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    s32 v = impact_speed();
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0x11, v, 0);
    Boss1_SetPartAnimation(&h->unk30C, h->unk304, 4, v, 0);
    h->unk216 |= 4;
    h->unk29A |= 4;
    h->unk242 &= ~4;
    s->unk20C = 0x40;
    s->unk218 = 0;
    s->func = Boss1Neck_State_SlamImpactShake;
}
static void Boss1Neck_State_SlamImpactShake(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1_UpdateCameraShake(&s->unk20C, 2, 6);
    s->unk218++;
    if ((s->unk218 & 3) == 0) {
        if (s->unk218 & 0x8000)
            Boss1_SetFlashPalette(g_boss_game.unk0, 0);
        else
            Boss1_SetFlashPalette(g_boss_game.unk0, 1);
        s->unk218 &= ~3;
        s->unk218 ^= 0x8000;
    }
    if (!(s->unk228.flags[0] & 2))
        return;
    if (s->unk218 & 0x8000)
        return;
    ScreenShake_Start(0);
    s->func = Boss1Neck_State_SlamRecover;
}
static void Boss1Neck_State_SlamRecover(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0x12, 0x1000, 0);
    Boss1_SetPartAnimation(&h->unk30C, h->unk304, 5, 0x1000, 0);
    s->unk20C = 0;
    s->func = Boss1Neck_State_SlamRecoverWait;
}
static void Boss1Neck_State_SlamRecoverWait(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    if (s->unk20C == 0) {
        if (Boss1Neck_UpdateSlamHitboxes(9) == 0)
            s->unk20C = 1;
    }
    if (s->unk228.flags[0] & 2)
        s->func = Boss1Neck_State_SlamDone;
}
static void Boss1Neck_State_SlamDone(void *p) {
    Boss1Hammer *h = gBoss1->unk214;
    (void)p;
    h->unk216 &= ~4;
    h->unk242 &= ~4;
    h->unk29A &= ~4;
}

/* ------------------------------------------------------ triple slam */
static void Boss1Neck_State_TripleSlamApproach(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    Boss1Neck_TrackPlayerFast(s);
    if (!(s->unk228.flags[0] & 2))
        return;
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0xe, 0x1000, 0);
    Boss1_SetPartAnimation(&h->unk30C, h->unk304, 1, 0x1000, 0);
    Sound_PlaySeqArc(0, 0, 0x7e);
    Sound_PlayVoice(0, 0, 0xd0);
    s->func = Boss1Neck_State_TripleSlamWindUp;
}
static void Boss1Neck_State_TripleSlamWindUp(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Neck_TrackPlayerMedium(s);
    if (s->unk228.flags[0] & 2)
        s->func = Boss1Neck_State_TripleSlamHold;
}
static void Boss1Neck_State_TripleSlamHold(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0xf, 0x1000, 0);
    s->unk228.flags[0] |= 1;
    Boss1_SetPartAnimation(&h->unk30C, h->unk304, 2, 0x1000, 0);
    h->unk30C.flags[0] |= 1;
    s->unk20C = 0;
    s->func = Boss1Neck_State_TripleSlamRaise;
    s->func(s);
}
static void Boss1Neck_State_TripleSlamRaise(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    Boss1Neck_TrackPlayerMedium(s);
    s->unk20C += gBoss1->unk22A;
    if ((s32)s->unk20C >= 0x1000) {
        s->h.vel_x = 0;
        s->unk20C = 0x1000;
        s->func = Boss1Neck_State_TripleSlamStrike;
    }
    set_raise_frame(&s->unk228, (s16)s->unk20C);
    set_raise_frame(&h->unk30C, (s16)s->unk20C);
}
static void Boss1Neck_State_TripleSlamStrike(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    if (s->unk210 == 0) {
        Boss1_SetPartAnimation(&s->unk228, s->unk220, 0x10, 0x1000, 0);
        Boss1_SetPartAnimation(&h->unk30C, h->unk304, 3, 0x1000, 0);
    } else {
        s->unk228.speed = 0x1000;     /* unk33C */
        s->unk228.flags[0] &= ~1;
        h->unk30C.speed = 0x1000;     /* unk420 */
        h->unk30C.flags[0] &= ~1;
    }
    Sound_PlaySeqArc(0, 0, 0x81);
    s->unk20C = 0;
    s->func = Boss1Neck_State_TripleSlamDown;
    s->func(s);
}
static void Boss1Neck_State_TripleSlamDown(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    u32 v;
    BossEffect *obj;
    Boss1_Unk_ov000_022cc6d4();
    v = slam_track_frames();
    if ((s32)v >= anim_left(&s->unk228))
        s->h.vel_x = 0;
    else
        Boss1Neck_TrackPlayerMedium(s);
    if (s->unk20C == 0) {
        if (Boss1Neck_UpdateSlamHitboxes(2))
            s->unk20C = 1;
    }
    if (!(s->unk228.flags[0] & 2))
        return;
    ScreenShake_Start(0xb);
    obj = BossEffect_CreateImpactSmall(1, gBoss1->unk214->h.x, 0x20000, 0);
    obj->unk134.speed = 0x1000;
    obj->unk134.scale[0] = 0x34cc;
    obj->unk134.scale[1] = 0x34cc;
    obj->unk134.scale[2] = 0x34cc;
    Sound_PlaySeqArc(0, 0, 0x7f);
    s->unk210++;
    if (s->unk210 < 3)
        s->func = Boss1Neck_State_TripleSlamAfterImpact;
    else
        s->func = Boss1Neck_State_TripleSlamFinalImpact;
}
static void Boss1Neck_State_TripleSlamAfterImpact(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    h->unk242 &= ~4;
    s->unk20C = 0x40;
    s->unk20E = 0;
    s->func = Boss1Neck_State_TripleSlamImpactShake;
    s->func(s);
}
static void Boss1Neck_State_TripleSlamImpactShake(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1_UpdateCameraShake(&s->unk20C, 2, 6);
    s->unk20E++;
    if ((u32)(u16)s->unk20E > 4)
        s->func = Boss1Neck_State_TripleSlamStartRewind;
}
static void Boss1Neck_State_TripleSlamStartRewind(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    s->unk228.flags[0] &= ~2;     /* unk334 */
    s->unk228.speed = -0x1000;    /* unk33C */
    h->unk30C.flags[0] &= ~2;     /* unk418 */
    h->unk30C.speed = -0x1000;    /* unk420 */
    s->func = Boss1Neck_State_TripleSlamRewind;
}
static void Boss1Neck_State_TripleSlamRewind(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    Boss1_UpdateCameraShake(&s->unk20C, 2, 6);
    if (s->unk228.anm[0] != NULL && s->unk228.anm[0]->frame > 0xa000)
        return;
    s->unk228.flags[0] |= 1;
    h->unk30C.flags[0] |= 1;
    s->func = Boss1Neck_State_TripleSlamStartReraiseShake;
}
static void Boss1Neck_State_TripleSlamStartReraiseShake(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    s->unk20E = 0;
    s->func = Boss1Neck_State_TripleSlamReraiseShake;
    s->func(s);
}
static void Boss1Neck_State_TripleSlamReraiseShake(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1_UpdateCameraShake(&s->unk20C, 2, 6);
    s->unk20E++;
    if ((u32)(u16)s->unk20E > 6)
        s->func = Boss1Neck_State_TripleSlamStrike;
}
static void Boss1Neck_State_TripleSlamFinalImpact(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    s32 v = impact_speed();
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0x11, v, 0);
    Boss1_SetPartAnimation(&h->unk30C, h->unk304, 4, v, 0);
    h->unk216 |= 4;
    h->unk29A |= 4;
    h->unk242 &= ~4;
    s->unk20C = 0x40;
    s->unk218 = 0;
    s->func = Boss1Neck_State_TripleSlamFinalShake;
}
static void Boss1Neck_State_TripleSlamFinalShake(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1_UpdateCameraShake(&s->unk20C, 2, 6);
    s->unk218++;
    if ((s->unk218 & 3) == 0) {
        if (s->unk218 & 0x8000)
            Boss1_SetFlashPalette(g_boss_game.unk0, 0);
        else
            Boss1_SetFlashPalette(g_boss_game.unk0, 1);
        s->unk218 &= ~3;
        s->unk218 ^= 0x8000;
    }
    if (!(s->unk228.flags[0] & 2))
        return;
    if (s->unk218 & 0x8000)
        return;
    ScreenShake_Start(0);
    s->func = Boss1Neck_State_TripleSlamRecover;
}
static void Boss1Neck_State_TripleSlamRecover(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0x12, 0x1000, 0);
    Boss1_SetPartAnimation(&h->unk30C, h->unk304, 5, 0x1000, 0);
    s->unk20C = 0;
    s->func = Boss1Neck_State_TripleSlamRecoverWait;
}
static void Boss1Neck_State_TripleSlamRecoverWait(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    if (s->unk20C == 0) {
        if (Boss1Neck_UpdateSlamHitboxes(9) == 0)
            s->unk20C = 1;
    }
    if (s->unk228.flags[0] & 2)
        s->func = Boss1Neck_State_TripleSlamDone;
}
static void Boss1Neck_State_TripleSlamDone(void *p) {
    Boss1Hammer *h = gBoss1->unk214;
    (void)p;
    ScreenShake_Start(0);
    h->unk216 &= ~4;
    h->unk242 &= ~4;
    h->unk29A &= ~4;
}

/* --------------------------------------------------------- big slam */
static void Boss1Neck_State_BigSlamApproach(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    BossCamera *c = &data_ov000_0236a444;
    Boss1Neck_TrackPlayerFast(s);
    if (!(s->unk228.flags[0] & 2))
        return;
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0x13, 0x1000, 0);
    Boss1_SetPartAnimation(&h->unk30C, h->unk304, 6, 0x1000, 0);
    s->unk20E = 0;
    Sound_PlaySeqArc(0, 0, 0x7e);
    Sound_PlayVoice(0, 0, 0xd2);
    c->t[1].ox = &h->h;          /* unk84 */
    c->t[1].oy = &h->h;          /* unk88 */
    c->t[1].oz = &h->h;          /* unk8C */
    c->t[0].oy = NULL;           /* unk4C */
    c->t[0].oz = NULL;           /* unk50 */
    c->t[0].ox = gPlayers;       /* unk48 */
    c->unkDC = 1;
    c->t[1].lerp = 0x40;         /* unkB4 */
    c->t[0].tgt.y = -0x200000;   /* unk64 */
    c->t[0].tgt.z = 0xc8000;     /* unk68 */
    c->t[0].lerp = 0x100;        /* unk78 */
    s->func = Boss1Neck_State_BigSlamWindUp;
}
static void Boss1Neck_State_BigSlamWindUp(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Neck_TrackPlayerSlow(s);
    if (s->unk228.flags[0] & 2)
        s->func = Boss1Neck_State_BigSlamHold;
}
static void Boss1Neck_State_BigSlamHold(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    if (!Boss1_Unk_ov000_022cc6d4())
        s->h.vel_x = 0;
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0x14, 0x1000, 0);
    Boss1_SetPartAnimation(&h->unk30C, h->unk304, 7, 0x1000, 0);
    data_ov000_0236a444.t[0].tgt.y = -0x210000;    /* unk64 */
    data_ov000_0236a444.t[1].offy = 0x180000;      /* unk94 */
    data_ov000_0236a444.t[0].tgt.z -= 0x50000;     /* unk68 */
    s->unk20C = 0;
    s->func = Boss1Neck_State_BigSlamRaise;
    s->func(s);
}
static void Boss1Neck_State_BigSlamRaise(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    if (Boss1_Unk_ov000_022cc6d4())
        Boss1Neck_TrackPlayerSlow(s);
    s->unk20C += gBoss1->unk22A;
    if ((s32)s->unk20C >= 0x1000) {
        s->unk20C = 0x1000;
        s->func = Boss1Neck_State_BigSlamStrike;
    }
    set_raise_frame(&s->unk228, (s16)s->unk20C);
    set_raise_frame(&h->unk30C, (s16)s->unk20C);
}
static void Boss1Neck_State_BigSlamStrike(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0x15, 0x1000, 0);
    Boss1_SetPartAnimation(&h->unk30C, h->unk304, 8, 0x1000, 0);
    data_ov000_0236a444.t[1].offy = -0x28000;      /* unk94 */
    data_ov000_0236a444.t[1].lerp = 0x80;          /* unkB4 */
    data_ov000_0236a444.t[0].tgt.z = 0xa0000;      /* unk68 */
    data_ov000_0236a444.t[0].lerp = 0x55;          /* unk78 */
    Sound_PlaySeqArc(0, 0, 0x81);
    s->unk20C = 0;
    s->func = Boss1Neck_State_BigSlamDown;
    s->func(s);
}
static void Boss1Neck_State_BigSlamDown(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    BossEffect *obj;
    if (Boss1_Unk_ov000_022cc6d4()) {
        if (anim_left(&s->unk228) <= 0x14)
            s->h.vel_x = 0;
        else
            Boss1Neck_TrackPlayerSlow(s);
    }
    if (s->unk20C == 0) {
        if (Boss1Neck_UpdateSlamHitboxes(4))
            s->unk20C = 1;
    }
    if (!(s->unk228.flags[0] & 2))
        return;
    ScreenShake_Start(0xc);
    obj = BossEffect_CreateImpactBig(1, gBoss1->unk214->h.x, 0x20000, 0);
    obj->unk134.speed = 0x1000;
    obj->unk134.scale[0] = 0x34cc;
    obj->unk134.scale[1] = 0x34cc;
    obj->unk134.scale[2] = 0x34cc;
    Sound_PlaySeqArc(0, 0, 0x80);
    s->func = Boss1Neck_State_BigSlamImpact;
    boss_log("bigslam_impact", gBoss1->unk214->h.x >> 8, gPlayers->x >> 8);
}
static void Boss1Neck_State_BigSlamImpact(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    BossCamera *c = &data_ov000_0236a444;
    s32 v = impact_speed();
    DcPlayer *t;
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0x16, v, 0);
    Boss1_SetPartAnimation(&h->unk30C, h->unk304, 9, v, 0);
    h->unk216 |= 4;
    h->unk29A |= 4;
    h->unk26E &= ~4;
    t = gPlayers;
    c->t[1].tgt.y = -0x1c0000;   /* unkA0 */
    c->t[1].ox = t;              /* unk84 */
    c->t[1].oy = NULL;           /* unk88 */
    c->t[1].oz = t;              /* unk8C */
    c->t[1].offx = 0;            /* unk90 */
    c->t[1].offy = 0x28000;      /* unk94 */
    c->t[1].offz = 0;            /* unk98 */
    c->t[1].lerp = 0x200;        /* unkB4 */
    c->t[0].tgt.y = -0x1d8000;   /* unk64 */
    c->t[0].offx = 0;            /* unk54 */
    c->t[0].offy = 0x28000;      /* unk58 */
    c->t[0].offz = 0xa0000;      /* unk5C */
    c->t[0].lerp = 0x100;        /* unk78 */
    s->unk20C = 0x80;
    s->unk20E = 0;
    s->unk218 = 0;
    s->func = Boss1Neck_State_BigSlamImpactShake;
}
static void Boss1Neck_State_BigSlamImpactShake(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1_UpdateCameraShake(&s->unk20C, 2, 7);
    s->unk218++;
    if ((s->unk218 & 3) == 0) {
        if (s->unk218 & 0x8000)
            Boss1_SetFlashPalette(g_boss_game.unk0, 0);
        else
            Boss1_SetFlashPalette(g_boss_game.unk0, 1);
        s->unk218 &= ~3;
        s->unk218 ^= 0x8000;
    }
    if (!(s->unk228.flags[0] & 2))
        return;
    if (s->unk218 & 0x8000)
        return;
    ScreenShake_Start(0);
    s->func = Boss1Neck_State_BigSlamRecover;
}
static void Boss1Neck_State_BigSlamRecover(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1Hammer *h = gBoss1->unk214;
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0x17, 0x1000, 0);
    Boss1_SetPartAnimation(&h->unk30C, h->unk304, 0xa, 0x1000, 0);
    s->unk20C = 0;
    s->func = Boss1Neck_State_BigSlamRecoverWait;
}
static void Boss1Neck_State_BigSlamRecoverWait(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    if (s->unk20C == 0) {
        if (Boss1Neck_UpdateSlamHitboxes(9) == 0)
            s->unk20C = 1;
    }
    if (s->unk228.flags[0] & 2)
        s->func = Boss1Neck_State_BigSlamDone;
}
static void Boss1Neck_State_BigSlamDone(void *p) {
    Boss1Hammer *h = gBoss1->unk214;
    (void)p;
    h->unk216 &= ~4;
    h->unk26E &= ~4;
    h->unk29A &= ~4;
}

/* ------------------------------------------------------ hammer roll */
static void Boss1Neck_State_RollStart(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    s->unk20C = 0;
    s->func = Boss1Neck_State_RollMoveToEdge;
    s->func(s);
}
static void Boss1Neck_State_RollMoveToEdge(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    if (gBoss1->unk228 > 0)
        Boss1Neck_MoveToX(s, 0x1aa00);
    else
        Boss1Neck_MoveToX(s, 0x1800);
    s->unk20C++;
    if ((s16)s->h.vel_x != 0)
        s->unk20C = 0;
    if (s->unk20C > 10)
        s->func = Boss1Neck_State_RollReleaseAnim;
}
static void Boss1Neck_State_RollReleaseAnim(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    if ((s->unk228.flags[0] & 2) == 0)
        return;
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0x18, 0x1000, 0);
    s->func = Boss1Neck_State_RollReleaseHammer;
}
static void Boss1Neck_State_RollReleaseHammer(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    if (anim_left(&s->unk228) < 0x2d && !(gBoss1->unk20C & 1)) {
        Boss1_SetCameraSide(gBoss1->unk228);
        gBoss1->unk20C |= 1;
        boss_log("hammer_release", gBoss1->unk214->h.x >> 8, gBoss1->unk228);
    }
    if (s->unk228.flags[0] & 2)
        s->func = Boss1Neck_State_RollStartWait;
}
static void Boss1Neck_State_RollStartWait(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    s->unk20C = 0;
    s->func = Boss1Neck_State_WaitTimer_ov000_022c8b90;
}
static void Boss1Neck_State_WaitTimer_ov000_022c8b90(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    if (s->unk20C++ > 5)
        s->func = Boss1Neck_State_RollAnim2;
}
static void Boss1Neck_State_RollAnim2(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0x19, 0x1000, 0);
    s->func = Boss1Neck_State_RollWaitAnim2;
}
static void Boss1Neck_State_RollWaitAnim2(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    if (s->unk228.flags[0] & 2)
        s->func = Boss1Neck_State_RollAnim3;
}
static void Boss1Neck_State_RollAnim3(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0x1a, 0x1000, 0);
    s->func = Boss1Neck_State_RollWaitAnim3;
}
static void Boss1Neck_State_RollWaitAnim3(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    if (s->unk228.flags[0] & 2) {
        s->h.disp_flags |= 0x20;
        s->func = Boss1Neck_State_RollWaitHammer;
    }
}
static void Boss1Neck_State_RollReturnAnim1(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    s->h.disp_flags &= ~0x20;
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0x1b, 0x1000, 0);
    s->func = Boss1Neck_State_RollReturnWait1;
}
static void Boss1Neck_State_RollReturnWait1(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    if (s->unk228.flags[0] & 2)
        s->func = Boss1Neck_State_RollReturnAnim2;
}
static void Boss1Neck_State_RollReturnAnim2(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0x1c, 0x1000, 0);
    s->func = Boss1Neck_State_RollReturnWait2;
}
static void Boss1Neck_State_RollReturnWait2(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    if (s->unk228.flags[0] & 2)
        s->func = Boss1Neck_State_RollRegrabHammer;
}
static void Boss1Neck_State_RollRegrabHammer(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    gBoss1->unk20C &= ~1u;
    gBoss1->unk214->unk29A &= ~4;
    Sound_PlaySeqArc(0, 0, 0xea);
    s->func = Boss1Neck_State_RollReturnAnim3;
    boss_log("hammer_regrab", gBoss1->unk214->h.x >> 8, 0);
}
static void Boss1Neck_State_RollReturnAnim3(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1_SetPartAnimation(&s->unk228, s->unk220, 0x1d, 0x1000, 0);
    s->func = Boss1Neck_State_RollReturnWait3;
}
static void Boss1Neck_State_RollReturnWait3(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    if (s->unk228.flags[0] & 2)
        s->func = Boss1Neck_State_HammerRollDone;
}

/* ------------------------------------------- missiles (zone 5 variant) */
/* Boss1Neck_CalcTurretAngle (copied; returns the FX_Atan2 result) */
static s16 Boss1Neck_CalcTurretAngle(s32 x, s32 unused, s32 y, s32 flag) {
    s32 px = x << 4;
    s32 py = y << 4;
    s32 ox, oy;
    (void)unused;
    if (flag != 0) {
        ox = -0xaa8da;
        oy = 0x2a1290;
    } else {
        ox = -0x117726;
        oy = 0x2a1290;
    }
    return (s16)Boss_Atan2(px + ox, py + oy);
}
/* Boss1Neck_GetTurretMuzzlePos (copied) */
static void Boss1Neck_GetTurretMuzzlePos(BossVec *out, s32 flag) {
    Boss1 *g = gBoss1;
    if (flag) {
        out->x = FX_Mul(SIN(g->unk21C), 0x46f22) + 0xaa8da;
        out->z = FX_Mul(COS(g->unk21C), 0x46f22) - 0x2a1290;
        out->y = -0x1c14dc;
    } else {
        out->x = FX_Mul(SIN(g->unk21E), 0x48988) + 0x117726;
        out->z = FX_Mul(COS(g->unk21E), 0x48988) - 0x2a1290;
        out->y = -0x1c14dc;
    }
}
/* Boss5PartD3_Action_Launch: the missile object is not translated */
static void launch_missile(const BossVec *v, s32 tx, s32 ty, s32 tz, s32 speed) {
    boss_log("missile", tx >> 12, speed);
    if (g_boss_hooks.missile)
        g_boss_hooks.missile(g_boss_hooks.ctx, v->x, v->y, v->z, tx, ty, tz, speed);
}

static void Boss1Neck_State_MissileWaitStart(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    if ((s->unk228.flags[0] & 2) == 0)
        return;
    s->func = Boss1Neck_State_MissileIntroAnim;
    s->func(s);
}
static void Boss1Neck_State_MissileIntroAnim(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1_SetPartAnimation(&s->unk228, s->unk224, 0, 0x1000, 0);
    data_ov000_0236a444.unkCC = -0x3c000;
    Sound_PlayVoice(0, 0, 0x7e);
    s->func = Boss1Neck_State_MissileWaitIntro;
}
static void Boss1Neck_State_MissileWaitIntro(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    if (s->unk228.flags[0] & 2)
        s->func = Boss1Neck_State_MissileSetPose;
}
static void Boss1Neck_State_MissileSetPose(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1_SetPartAnimation(&s->unk228, s->unk224, 1, 0x1000, 0);
    s->h.x = -0x7c00;
    s->h.y = 0x20000;
    s->h.z = -0x16800;
    s->unk20C = 0;
    s->func = Boss1Neck_State_MissilePoseAnim;
}
static void Boss1Neck_State_MissilePoseAnim(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1 *g = gBoss1;
    if (anim_left_fx(&s->unk228) < 0x14000 && s->unk20C == 0) {
        Sound_PlaySeqArc(0, 0, 0x9e);
        s->unk20C = 1;
    }
    if (!(s->unk228.flags[0] & 2))
        return;
    g->unk20C |= 1;
    MTX_Identity33(&g->unk214->unk30C.rot);
    s->func = Boss1Neck_State_MissileReadyAnim;
}
static void Boss1Neck_State_MissileReadyAnim(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1_SetPartAnimation(&s->unk228, s->unk224, 2, 0x1000, 0);
    s->func = Boss1Neck_State_MissileWaitReady;
}
static void Boss1Neck_State_MissileWaitReady(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    if ((s->unk228.flags[0] & 2) == 0)
        return;
    s->h.disp_flags |= 0x20;
    Sound_PlayVoice(0, 0, 0xd1);
    s->func = Boss1Neck_State_MissileSelectPattern;
}
static void Boss1Neck_State_MissileSelectPattern(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    s->unk20C = 0;
    switch (gBoss1->unk22A) {
    case 2:
    case 3:
        s->func = Boss1Neck_State_StartTrackPlayer;
        return;
    }
    s->func = Boss1Neck_State_PickRandomTarget;
}
static void Boss1Neck_State_PickRandomTarget(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    s->unk214 = (Rand() & 0xff) * 0x1c2;
    s->unk20E = Boss1Neck_CalcTurretAngle(s->unk214, 0x20000, 0, s->unk20C & 1);
    s->unk210 = 0;
    s->func = Boss1Neck_State_AimAtTarget;
}
static void Boss1Neck_State_AimAtTarget(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    s16 *pa;
    s16 t;
    Boss1 *g = gBoss1;
    if (s->unk20C & 1)
        pa = &g->unk21C;
    else
        pa = &g->unk21E;
    t = s->unk20E;
    if (*pa < t) {
        if (t - *pa > 0x100)
            *pa += 0x100;
        else
            *pa = t;
    } else if (*pa > t) {
        if (*pa - t > 0x100)
            *pa -= 0x100;
        else
            *pa = t;
    }
    if (data_ov000_02362b8c[gBoss1->unk22A] < s->unk210++)
        s->func = Boss1Neck_State_FireAtTarget;
}
static void Boss1Neck_State_FireAtTarget(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    BossVec v;
    Boss1Neck_GetTurretMuzzlePos(&v, s->unk20C & 1);
    launch_missile(&v, s->unk214 << 4, -0x200000, 0, data_ov000_02362b94[gBoss1->unk22A]);
    BossEffect_CreateLaunch(1, v.x >> 4, -v.y >> 4, v.z >> 4);
    Sound_PlaySeqArc(0, 0, 0x9c);
    s->unk20C++;
    if (data_ov000_02362b84[gBoss1->unk22A] <= s->unk20C)
        s->func = Boss1Neck_State_MissileEndAnim;
    else
        s->func = Boss1Neck_State_PickRandomTarget;
}
static void Boss1Neck_State_StartTrackPlayer(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    s->unk210 = 0;
    s->func = Boss1Neck_State_TrackPlayer;
}
static void Boss1Neck_State_TrackPlayer(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1 *g = gBoss1;
    s16 t;
    u16 lim;
    t = Boss1Neck_CalcTurretAngle(gPlayers->x, gPlayers->y, gPlayers->z, 1);
    if (g->unk21C < t) {
        if (t - g->unk21C > 0x100)
            g->unk21C += 0x100;
        else
            g->unk21C = t;
    } else if (g->unk21C > t) {
        if (g->unk21C - t > 0x100)
            g->unk21C -= 0x100;
        else
            g->unk21C = t;
    }
    t = Boss1Neck_CalcTurretAngle(gPlayers->x, gPlayers->y, gPlayers->z, 0);
    if (g->unk21E < t) {
        if (t - g->unk21E > 0x100)
            g->unk21E += 0x100;
        else
            g->unk21E = t;
    } else if (g->unk21E > t) {
        if (g->unk21E - t > 0x100)
            g->unk21E -= 0x100;
        else
            g->unk21E = t;
    }
    lim = data_ov000_02362b8c[gBoss1->unk22A];
    if (gBoss1->unk22A == 3 && (s->unk20C & 7) == 0)
        lim += 0x32;
    if (lim < s->unk210++)
        s->func = Boss1Neck_State_FireAtPlayer;
}
static void Boss1Neck_State_FireAtPlayer(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    BossVec v;
    s32 x;
    Boss1Neck_GetTurretMuzzlePos(&v, s->unk20C & 1);
    x = gPlayers->x << 4;
    x += ((Rand() & 0x7f) - 0x40) << 12;
    if (x < 0)
        x = 0;
    else if (x > 0x1c2000)
        x = 0x1c2000;
    launch_missile(&v, x, -0x200000, 0, data_ov000_02362b94[gBoss1->unk22A]);
    BossEffect_CreateLaunch(1, v.x >> 4, -v.y >> 4, v.z >> 4);
    Sound_PlaySeqArc(0, 0, 0x9c);
    s->unk20C++;
    if (data_ov000_02362b84[gBoss1->unk22A] <= s->unk20C)
        s->func = Boss1Neck_State_MissileEndAnim;
    else
        s->func = Boss1Neck_State_StartTrackPlayer;
}
static void Boss1Neck_State_MissileEndAnim(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1_SetPartAnimation(&s->unk228, s->unk224, 3, 0x1000, 0);
    s->h.disp_flags &= ~0x20;
    s->func = Boss1Neck_State_MissileWaitEndAnim;
}
static void Boss1Neck_State_MissileWaitEndAnim(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    if ((s->unk228.flags[0] & 2) == 0)
        return;
    gBoss1->unk20C &= ~1u;
    Sound_PlayVoice(0, 0, 0xea);
    s->func = Boss1Neck_State_MissileRetractAnim;
}
static void Boss1Neck_State_MissileRetractAnim(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1_SetPartAnimation(&s->unk228, s->unk224, 4, 0x1000, 0);
    s->func = Boss1Neck_State_MissileWaitRetract;
}
static void Boss1Neck_State_MissileWaitRetract(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    if (s->unk228.flags[0] & 2)
        s->func = Boss1Neck_State_MissileResetPose;
}
static void Boss1Neck_State_MissileResetPose(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    Boss1_SetPartAnimation(&s->unk228, s->unk224, 5, 0x1000, 0);
    s->h.x = 0xe100;
    s->h.y = 0x20000;
    s->h.z = -0x8e00;
    Boss1_ResetCamera();
    Sound_PlayVoice(0, 0, 0x7e);
    s->func = Boss1Neck_State_MissileWaitResetPose;
}
static void Boss1Neck_State_MissileWaitResetPose(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    if (s->unk228.flags[0] & 2)
        s->func = Boss1Neck_State_MissileDone;
}

/* Boss1Neck_Draw (+ Boss1Neck_NodeCallback_NeckHam: MTX_STORE 0x1e at node "neck_ham") */
static void Boss1Neck_Draw(void *p) {
    Boss1Neck *o = (Boss1Neck *)p;
    BossModel *m = &o->unk228;
    if (o->h.disp_flags & 0x20)
        return;
    m->trans[0] = o->h.x << 4;
    m->trans[1] = -(o->h.y << 4);
    m->trans[2] = o->h.z << 4;
    Model_Animate(m);
    boss_model_pose(m, NULL);
    boss_model_joint_world(m, s_joint_neck_ham, &s_mtx_slot30);
}

static void Boss1Neck_Destructor(void *p) {
    Boss1Neck *s = (Boss1Neck *)p;
    boss_model_release(&s->unk228);
}

/* ========================================================== Boss1Hammer */

/* Boss1Hammer_ApplySpinRotation (copied): rot = RotY(spin) * RotX(90 deg) */
static void Boss1Hammer_ApplySpinRotation(Boss1Hammer *s) {
    MtxFx33 m;
    BossModel *p = &s->unk30C;
    MTX_RotY33(&p->rot, SIN(s->unk2CC), COS(s->unk2CC));
    MTX_RotX33(&m, SIN(0x4000), COS(0x4000)); /* data_0206f470[0], [1] */
    MTX_Concat33(&p->rot, &m, &p->rot);
}

/* Boss1Hammer_SpawnDebris (copied; the three Rand() of the EffPebble_Create
 * call are drawn in the ARM code's order: y velocity, z velocity, z) */
static void Boss1Hammer_SpawnDebris(Boss1Hammer *s) {
    s16 v;
    s32 a;
    s16 b;
    s16 c;
    s32 z;
    BossEffect *obj;
    if (s->unk2BE++ & 3)
        return;
    v = s->unk2CE >> 4;
    if (v > 0)
        v += (Rand() & 0xff) - 0x80;
    else if (v < 0)
        v -= (Rand() & 0xff) - 0x80;
    a = s->unk2CE;
    if (a < 0)
        a = -a;
    b = (s16)-(a >> 3);
    b = (s16)(b - (s16)(Rand() & 0xff));
    c = (s16)(0x100 - (Rand() & 0x1ff));
    z = 0x1000 - (Rand() & 0x1fff);
    obj = EffPebble_Create(s->h.x, 0x20000, z, v, b, c, 0x3c);
    obj->unk134.scale[0] += (Rand() & 0x3fff) - 0x1000;
    obj->unk134.scale[1] += (Rand() & 0x3fff) - 0x1000;
    obj->unk134.scale[2] += (Rand() & 0x3fff) - 0x1000;
    switch ((s->unk2BE >> 2) & 3) {
    case 0:
        MTX_RotX33(&obj->unk134.rot, SIN(s->unk2CC), COS(s->unk2CC));
        break;
    case 1:
        MTX_RotY33(&obj->unk134.rot, SIN(s->unk2CC), COS(s->unk2CC));
        break;
    case 2:
    default:
        MTX_RotZ33(&obj->unk134.rot, SIN(s->unk2CC), COS(s->unk2CC));
        break;
    }
}

static void Boss1Hammer_State_Idle(void *p) { (void)p; }
static void Boss1Hammer_State_Idle_ov000_022c5f90(void *p) { (void)p; }
static void Boss1Hammer_State_Idle_ov000_022c5fa4(void *p) { (void)p; }
static void Boss1Hammer_State_Idle_ov000_022c5fb8(void *p) { (void)p; }
static void Boss1Hammer_State_Idle_ov000_022c6940(void *p) { (void)p; }
static void Boss1Hammer_State_Idle_ov000_022c6954(void *p) { (void)p; }
static void Boss1Hammer_State_Idle_ov000_022c6968(void *p) { (void)p; }
static void Boss1Hammer_State_Hurt(void *p) { (void)p; }
static void Boss1Hammer_State_Defeated(void *p) { (void)p; }

static void Boss1Hammer_State_WaitRelease(void *p) {
    Boss1Hammer *s = (Boss1Hammer *)p;
    if (gBoss1->unk20C & 1)
        s->func = Boss1Hammer_State_StartFall;
}
static void Boss1Hammer_State_StartFall(void *p) {
    Boss1Hammer *s = (Boss1Hammer *)p;
    s->unk30C.cb[0] = MOP_FLUSHP;   /* unk316 = 3 */
    s->unk30C.cb[1] = 0;            /* unk317 = 0 */
    MTX_RotX33(&s->unk30C.rot, SIN(0x4000), COS(0x4000));
    s->unk29A |= 4;
    s->func = Boss1Hammer_State_Fall;
}
static void Boss1Hammer_State_Fall(void *p) {
    Boss1Hammer *s = (Boss1Hammer *)p;
    s->h.vel_y += 0x100;
    if (s->h.y + s->h.vel_y < 0x1dc00)
        return;
    s->h.y = 0x1dc00;
    s->h.vel_y = 0;
    Sound_PlaySeqArc(0, 0, 0x7b);
    s->func = Boss1Hammer_State_Land;
}
static void Boss1Hammer_State_Land(void *p) {
    Boss1Hammer *s = (Boss1Hammer *)p;
    s->unk2C0 = 0;
    Boss1_SetCameraSide(gBoss1->unk228);
    s->func = Boss1Hammer_State_WaitTimer_ov000_022c67ec;
}
static void Boss1Hammer_State_WaitTimer_ov000_022c67ec(void *p) {
    Boss1Hammer *s = (Boss1Hammer *)p;
    if (s->unk2C0++ > 0x1e)
        s->func = Boss1Hammer_State_StartSpin;
}
static void Boss1Hammer_State_StartSpin(void *p) {
    Boss1Hammer *s = (Boss1Hammer *)p;
    s->unk2CC = 0;
    if (gBoss1->unk228 > 0)
        s->unk2CE = 0x1800;
    else
        s->unk2CE = -0x1800;
    s->func = Boss1Hammer_State_SpinUp;
}
static void Boss1Hammer_State_SpinUp(void *p) {
    Boss1Hammer *s = (Boss1Hammer *)p;
    u16 n;
    s16 k;
    s32 v;
    n = (s->unk2BC > 4) ? 4 : s->unk2BC;
    k = (s16)(((n * 0x80) >> 2) + 0x80);
    Boss1Hammer_SpawnDebris(s);
    if (gBoss1->unk228 > 0)
        s->unk2CE += k;
    else
        s->unk2CE -= k;
    v = s->unk2CE;
    if (v < -0x3800)
        v = -0x3800;
    else if (v > 0x3800)
        v = 0x3800;
    s->unk2CE = (s16)v;
    s->unk2CC += s->unk2CE;
    Boss1Hammer_ApplySpinRotation(s);
    v = s->unk2CE;
    if (v < 0)
        v = -v;
    if (v != 0x3800)
        return;
    Sound_PlayVoice(0, 0, 0xd1);
    s->func = Boss1Hammer_State_LaunchRoll;
}
static void Boss1Hammer_State_LaunchRoll(void *p) {
    Boss1Hammer *s = (Boss1Hammer *)p;
    u16 n;
    s16 v;
    n = (s->unk2BC > 4) ? 4 : s->unk2BC;
    v = (s16)(((n * 0x600) >> 2) + 0x600);
    if (gBoss1->unk228 > 0)
        s->h.vel_x = (s16)-v;
    else
        s->h.vel_x = v;
    Sound_PlaySeqArc(0, 0, 0x82);
    s->func = Boss1Hammer_State_Roll;
    s->func(s);
}
static void Boss1Hammer_State_Roll(void *p) {
    Boss1Hammer *s = (Boss1Hammer *)p;
    dcp_bool b = DCP_FALSE;
    s32 v;
    if (s->unk2CE > 0)
        s->unk2CE -= 0xa0;
    else if (s->unk2CE < 0)
        s->unk2CE += 0xa0;
    s->unk2CC += s->unk2CE;
    Boss1Hammer_SpawnDebris(s);
    v = gBoss1->unk228;
    if (v > 0) {
        if (s->h.x <= 0x9000)
            b = DCP_TRUE;
    } else {
        if (s->h.x >= 0x13200)
            b = DCP_TRUE;
    }
    if (b) {
        if (v < 0)
            v = -v;
        if (v == 1)
            s->func = Boss1Hammer_State_FinishRoll;
        else
            s->func = Boss1Hammer_State_TurnAtEdge;
    }
    Boss1Hammer_ApplySpinRotation(s);
}
static void Boss1Hammer_State_TurnAtEdge(void *p) {
    Boss1Hammer *s = (Boss1Hammer *)p;
    s32 v;
    if (gBoss1->unk228 > 0)
        s->unk2CE = -0xc00;
    else
        s->unk2CE = 0xc00;
    Boss1_SetCameraSide(-gBoss1->unk228);
    s->unk2C0 = 0;
    v = (s16)s->h.vel_x;
    if (v < 0)
        v = -v;
    s->unk2C2 = (u16)FX_Div(v << 4, 0x78000);
    s->unk2C2 >>= 1;
    s->func = Boss1Hammer_State_SlideToEdge;
    s->func(s);
}
/* slide target: x approaches 0x18000 (or 0x1aa000) with weight t, 3 iterations (fx32 world) */
static s32 slide_target(s32 x, s32 tgt, s32 t) {
    s32 n = 2;
    do {
        x += (s32)(((s64)(tgt - x) * t + 0x800) >> 12);
    } while (n-- != 0);
    return x;
}
static void Boss1Hammer_State_SlideToEdge(void *p) {
    Boss1Hammer *s = (Boss1Hammer *)p;
    s32 v;
    if (gBoss1->unk228 > 0)
        s->unk2CE -= 0x80;
    else
        s->unk2CE += 0x80;
    v = s->unk2CE;
    if (v < -0x3800)
        v = -0x3800;
    else if (v > 0x3800)
        v = 0x3800;
    s->unk2CE = (s16)v;
    s->unk2CC += s->unk2CE;
    Boss1Hammer_SpawnDebris(s);
    s->unk2C0 += s->unk2C2;
    if ((s16)s->unk2C0 > 0x1000)
        s->unk2C0 = 0x1000;
    if (gBoss1->unk228 > 0)
        s->h.vel_x = (s16)((slide_target(0x90000, 0x18000, (s16)s->unk2C0) >> 4) - s->h.x);
    else
        s->h.vel_x = (s16)((slide_target(0x132000, 0x1aa000, (s16)s->unk2C0) >> 4) - s->h.x);
    if ((s16)s->unk2C0 >= 0x1000) {
        if (gBoss1->unk228 > 0)
            gBoss1->unk228--;
        else
            gBoss1->unk228++;
        gBoss1->unk228 = (s16)-gBoss1->unk228;
        s->unk2BC++;
        s->func = Boss1Hammer_State_SpinUp;
    }
    Boss1Hammer_ApplySpinRotation(s);
}
static void Boss1Hammer_State_FinishRoll(void *p) {
    Boss1Hammer *s = (Boss1Hammer *)p;
    s32 v;
    s->unk2CE = (s16)-s->unk2CE;
    Boss1_ResetCamera();
    s->unk2C0 = 0;
    v = (s16)s->h.vel_x;
    if (v < 0)
        v = -v;
    s->unk2C2 = (u16)FX_Div(v << 4, 0x78000);
    s->unk2C2 >>= 1;
    gBoss1->unk210->h.x = s->h.x;
    gBoss1->unk210->func = Boss1Neck_State_RollReturnAnim1;
    s->func = Boss1Hammer_State_SlideBackSpinDown;
    s->func(s);
}
static void Boss1Hammer_State_SlideBackSpinDown(void *p) {
    Boss1Hammer *s = (Boss1Hammer *)p;
    dcp_bool pos;
    s32 v;
    gBoss1->unk210->h.x = s->h.x;
    s->unk2C0 += s->unk2C2;
    if ((s16)s->unk2C0 > 0x1000)
        s->unk2C0 = 0x1000;
    if (gBoss1->unk228 > 0)
        s->h.vel_x = (s16)((slide_target(0x90000, 0x18000, (s16)s->unk2C0) >> 4) - s->h.x);
    else
        s->h.vel_x = (s16)((slide_target(0x132000, 0x1aa000, (s16)s->unk2C0) >> 4) - s->h.x);
    Boss1Hammer_SpawnDebris(s);
    pos = DCP_TRUE;
    if (s->unk2CE < 0) {
        s->unk2CE = (s16)-s->unk2CE;
        pos = DCP_FALSE;
    }
    v = s->unk2CE;
    if (v > 0) {
        if (s->unk2CC >= 0xfc00 && v <= 0x400) {
            s->unk2CE = 0;
            s->unk2CC = 0;
        } else if (v > 0) {
            s->unk2CE -= 0xa0;
            if (s->unk2CE < 0x400)
                s->unk2CE = 0x400;
        }
    }
    if (!pos)
        s->unk2CE = (s16)-s->unk2CE;
    s->unk2CC += s->unk2CE;
    if (s->unk2CE == 0 && (s16)s->unk2C0 >= 0x1000) {
        s->h.vel_x = 0;
        s->func = Boss1Hammer_State_EndRoll;
    }
    Boss1Hammer_ApplySpinRotation(s);
}
static void Boss1Hammer_State_EndRoll(void *p) {
    Boss1Hammer *s = (Boss1Hammer *)p;
    gBoss1->unk210->h.x = s->h.x;
    s->func = Boss1Hammer_State_Idle_ov000_022c5fb8;
}

/* Player_Action_KnockbackIntangible (player module lacks it; copied) */
static void Player_Action_KnockbackIntangible(DcPlayer *w, s16 vx, s16 vy) {
    dcp_player_init_state(w);
    dcp_set_anim(w, 0xf);
    w->vel_x = vx;
    w->vel_y = vy;
    w->gnd_vel = 0;
    if (vx < 0)
        w->disp_flags &= ~1;
    else
        w->disp_flags |= 1;
    w->obj_flags |= 2;
    w->move_flags |= 0x10;
    w->move_flags |= 0x8000;
    w->move_flags &= ~0x80000u;
    w->move_flags &= ~1u;
    w->state = dcp_state_hurt;
}

/* Boss1Hammer_OnHit_Damage (copied): box 0 hit by a player attack box */
static void Boss1Hammer_OnHit_Damage(ObjBox *a, ObjBox *b) {
    MtxFx33 mtx;
    DcPlayer *p = obj_box_player(b); /* b->unk18 with p->unk0 == 1 */
    BossEffect *obj;
    u16 ang;
    (void)a;
    if (gBossStatus.unk8 == 0)
        return;
    gBossStatus.unk8--;
    g_boss_stats.hits++;
    boss_log("hit", gBossStatus.unk8, b->x);
    if (gBossStatus.unk8 != 0) {
        Boss1_Action_HurtAll();
        obj = BossEffect_CreateHit(1, b->x << 8, b->y << 8, 0);
        ang = (u16)((Rand() & 0xfff) + 0xaaa);
        MTX_RotX33(&obj->unk134.rot, SIN(ang), COS(ang));
        ang = Rand();
        MTX_RotY33(&mtx, SIN(ang), COS(ang));
        MTX_Concat33(&obj->unk134.rot, &mtx, &obj->unk134.rot);
        obj->unk134.speed = 0x1000;
        if (gBossStatus.unk8 == 2) {
            NaviMsg_SetFaceAnim(0xb);
            NaviMsg_ShowMessage(0x12, 0xf0);
            if (g_boss_hooks.music) /* Sound_PlaySeq(0, 0xf) */
                g_boss_hooks.music(g_boss_hooks.ctx, 0xf);
            gBoss1->unk20C |= 2;
        } else {
            NaviMsg_SetFaceAnim(0xb);
            NaviMsg_ShowMessage(0x10, 0xf0);
        }
        if (p == NULL)
            return;
        obj_player_attack_bounce(p);
    } else {
        Boss1_Action_DefeatAll();
    }
}

/* Boss1Hammer_OnHit_PushPlayer (copied): box 3 (solid sides) */
static void Boss1Hammer_OnHit_PushPlayer(ObjBox *a, ObjBox *b) {
    DcPlayer *p = obj_box_player(b);
    Boss1Hammer *o = (Boss1Hammer *)a->owner;
    s32 vel;
    if (p == NULL)
        return;
    if (p->move_flags & 0x8000)
        vel = p->vel_x;
    else
        vel = p->gnd_vel;
    if (o->h.x < p->x) {
        if (o->h.vel_x == 0 && o->unk2CE == 0) {
            if (vel > 0)
                return;
            p->push_x = (s16)(((a->x + a->r - b->l) << 8) - p->x);
            p->move_flags |= 4;
            p->vel_x = 0;
            p->gnd_vel = 0;
            return;
        }
        if (p->x >= 0x1aa00) {
            if (p->anim == 0xf)
                return;
            Player_Action_KnockbackIntangible(p, 0x180, -0x600);
            boss_log("knockback", p->x >> 8, 1);
            return;
        }
        Sound_PlaySeqArc(0, 0, 0x7b);
        p->push_x = (s16)(((a->x + a->r - b->l) << 8) - p->x);
        if (p->move_flags & 0x8000)
            p->vel_x = 0xa00;
        else
            p->gnd_vel = 0xa00;
    } else {
        if (o->h.vel_x == 0 && o->unk2CE == 0) {
            if (vel < 0)
                return;
            p->push_x = (s16)(((a->x + a->l - b->r) << 8) - p->x);
            p->move_flags |= 4;
            p->vel_x = 0;
            p->gnd_vel = 0;
            return;
        }
        if (p->x <= 0x1800) {
            if (p->anim == 0xf)
                return;
            Player_Action_KnockbackIntangible(p, -0x180, -0x600);
            boss_log("knockback", p->x >> 8, -1);
            return;
        }
        Sound_PlaySeqArc(0, 0, 0x7b);
        p->push_x = (s16)(((a->x + a->l - b->r) << 8) - p->x);
        if (p->move_flags & 0x8000)
            p->vel_x = -0xa00;
        else
            p->gnd_vel = -0xa00;
    }
}

/* Boss1Hammer_Collide (copied) */
static void Boss1Hammer_Collide(void *p) {
    Boss1Hammer *o = (Boss1Hammer *)p;
    int i;
    if (o->h.obj_flags & 0xc)
        return;
    for (i = 0; i < 4; i++)
        if (o->unk20C[i].owner != NULL)
            obj_handle_collider(&o->h, o, OBJ_OWNER_OBJECT, &o->unk20C[i], 0);
}

/* Boss1Hammer_Draw (+ Boss1Hammer_NodeCallback_Hammer) */
static void Boss1Hammer_Draw(void *p) {
    Boss1Hammer *o = (Boss1Hammer *)p;
    BossModel *m = &o->unk30C;
    if (o->h.disp_flags & 0x20)
        return;
    if (gBoss1->unk20C & 1) {
        m->cb[0] = MOP_FLUSHP;
        m->cb[1] = 0;
        m->trans[0] = o->h.x << 4;
        m->trans[1] = -(o->h.y << 4);
        m->trans[2] = o->h.z << 4;
        Model_Animate(m);
        boss_model_pose(m, NULL);
        boss_model_joint_world(m, s_joint_hammer, &s_mtx_slot30); /* node callback (unused here) */
        return;
    }
    m->cb[0] = MOP_FLUSHVP;
    m->cb[1] = MOP_RESTORE;
    m->cb[2] = 0;
    m->trans[0] = 0;
    m->trans[1] = 0;
    m->trans[2] = 0;
    Model_Animate(m);
    boss_model_pose(m, &s_mtx_slot30);                        /* MTX_RESTORE 0x1e (neck_ham) */
    boss_model_joint_world(m, s_joint_hammer, &s_mtx_slot30); /* MTX_STORE 0x1e at node "hammer" */
    o->unk2D0 = s_mtx_slot30;                                 /* NNS_G3dGetCurrentMtx */
    {
        s32 tx = (s32)(o->unk2D0.m[3][0] * 4096.0f + (o->unk2D0.m[3][0] >= 0 ? 0.5f : -0.5f));
        s32 ty = (s32)(o->unk2D0.m[3][1] * 4096.0f + (o->unk2D0.m[3][1] >= 0 ? 0.5f : -0.5f));
        s32 tz = (s32)(o->unk2D0.m[3][2] * 4096.0f + (o->unk2D0.m[3][2] >= 0 ? 0.5f : -0.5f));
        o->h.x = tx >> 4;       /* unk2F4 >> 4 */
        o->h.y = -ty >> 4;      /* -unk2F8 >> 4 */
        o->h.z = tz >> 4;       /* unk2FC >> 4 */
    }
}

static void Boss1Hammer_Destructor(void *p) {
    Boss1Hammer *s = (Boss1Hammer *)p;
    int i;
    for (i = 0; i < 4; i++)
        rect_forget(&s->unk20C[i]);
    boss_model_release(&s->unk30C);
}

/* ============================================================ creation */
static void MapObject_Init_parts(DcPlayer *h, ObjBox *coll, const ObjParam *prm, s32 x, s32 y) {
    /* MapObject_Init (obj_manager.c obj_mapobject_init) fields the boss parts use */
    int i;
    h->obj_flags |= 0x40;
    h->x = x;
    h->y = y;
    h->type = (prm != NULL && prm->type < 0x40) ? 2 : 3;
    h->gravity = 0x2a;
    h->fall_max = 0xf00;
    h->obj_flags |= 1;
    h->disp_flags |= 1;
    h->move_flags |= 0x200;
    if (coll != NULL) {
        for (i = 0; i < 3; i++)
            coll[i].mask = 0x12;
        coll[0].cb = NULL; /* MapObject_OnHit_Default: replaced by the copies below */
        rect_set_atkdef(&coll[0], 9, 0xa);
        rect_set_atkdef(&coll[1], 0x13, 0x13);
        rect_set_atkdef(&coll[2], 0x13, 0x13);
        for (i = 0; i < 3; i++)
            coll[i].flags &= ~4;
    }
}

/* Boss1Neck_Create (copied) */
static Boss1Neck *Boss1Neck_Create(void) {
    Boss1Neck *s = &s_neck;
    Boss1 *g = gBoss1;
    BossModel *p;
    memset(s, 0, sizeof(*s));
    s->alive = 1;
    s->func = Boss1Neck_State_Idle;
    s->unk220 = &s_bs1_anim;            /* NNS_FndGetArchiveFileByIndex(ctx, 8) */
    s->unk224 = NULL;                   /* file 9: zone 5 only */
    s->unk21C = &data_ov000_02369ad0[1];
    MapObject_Init_parts(&s->h, NULL, NULL, 0, 0);
    s->h.obj_flags |= 0x12;
    s->h.move_flags |= 0x8300;
    s->h.disp_flags |= 0x20;
    s->h.x = g->h.x;
    s->h.y = g->h.y;
    s->h.z = g->h.z - 0x8e00;
    p = &s->unk228;
    Model_Init(p, 0);
    Model_SetResource(p, s->unk21C);
    p->trans[0] = g->h.x << 4;
    p->trans[1] = -(g->h.y << 4);
    p->trans[2] = (g->h.z - 0x8e00) << 4;
    p->scale[0] = p->scale[1] = p->scale[2] = 0x34cc;
    p->cb[0] = MOP_FLUSHVP;             /* p->unkA = 4 */
    return s;
}

/* Boss1Hammer_Create (copied) */
static Boss1Hammer *Boss1Hammer_Create(void) {
    Boss1Hammer *s = &s_hammer;
    ObjBox coll[3];
    ObjBox *h;
    BossModel *p;
    memset(s, 0, sizeof(*s));
    memset(coll, 0, sizeof(coll));
    s->alive = 1;
    s->func = Boss1Hammer_State_Idle;
    s->unk304 = &s_bs1_anim;
    s->unk308 = NULL;
    s->unk300 = &data_ov000_02369ad0[2];
    MapObject_Init_parts(&s->h, coll, NULL, 0, 0);
    s->h.obj_flags |= 0x10;
    s->h.move_flags |= 0x8300;
    s->h.disp_flags |= 0x20;
    h = &s->unk20C[0];
    *h = coll[0];                               /* s->unk134 */
    rect_set_box3d(h, -0x14, -0x30, 0x14, -0x20, -0x7d00, 0x7d00);
    h->owner = s;
    h->owner_kind = OBJ_OWNER_OBJECT;
    h->cb = Boss1Hammer_OnHit_Damage;
    h->flags &= ~4;
    h = &s->unk20C[1];
    *h = coll[1];                               /* s->unk160 */
    rect_set_box3d(h, -0x1c, -0x78, 0x1c, 0x20, -0x7d00, 0x7d00);
    h->owner = s;
    h->owner_kind = OBJ_OWNER_OBJECT;
    h->flags &= ~4;
    h = &s->unk20C[2];
    *h = coll[1];
    rect_set_box3d(h, -0x1c, -0x78, 0x1c, 0x20, -0x7d00, 0x7d00);
    h->atk |= 0x20;
    h->owner = s;
    h->owner_kind = OBJ_OWNER_OBJECT;
    h->flags &= ~4;
    h = &s->unk20C[3];
    rect_set_box3d(h, -0x1c, -0x20, 0x1c, 0x20, -0x7d00, 0x7d00);
    rect_set_atkdef(h, 0, (u32)-0x10);
    h->flags |= 0xc0;
    h->mask = 0x12;
    h->owner = s;
    h->owner_kind = OBJ_OWNER_OBJECT;
    h->cb = Boss1Hammer_OnHit_PushPlayer;
    h->flags &= ~4;
    s->h.x = 0;
    s->h.y = 0;
    s->h.z = 0;
    p = &s->unk30C;
    Model_Init(p, 0);
    Model_SetResource(p, s->unk300);
    p->scale[0] = p->scale[1] = p->scale[2] = 0x34cc;
    p->unk8 = 0x1e;
    p->cb[0] = MOP_FLUSHVP;   /* unkA = 4 */
    p->cb[1] = MOP_RESTORE;   /* unkB = 2 */
    return s;
}

/* Boss1_Create (copied) */
void *Boss1_Create(const ObjParam *a, s32 x, s32 y) {
    Boss1 *s;
    BossModel *p;
    if (a->x0 == 0xff && a->y0 == 0xff)
        return NULL;
    s = &s_boss1;
    memset(s, 0, sizeof(*s));
    s->alive = 1;
    gBoss1 = s;
    s->func = Boss1_State_WaitForStart;
    s->unk21A = Rand() & 3;
    s->unk21C = 0xaaa;
    memset(&gBossStatus, 0, sizeof(gBossStatus)); /* MI_CpuFill16(&gBossStatus, 0, 0xc) */
    gBossStatus.unk4 = 1;
    gBossStatus.unk8 = (u16)Boss_GetStartHealth();
    gBossStatus.unkA = (u16)Boss_GetStartHealth();
    s->unk230 = &s_bs1_anim;
    s->unk234 = NULL;
    s->unk22C = &data_ov000_02369ad0[0];
    s->unk238 = &data_ov000_02369ad0[3];
    MapObject_Init_parts(&s->h, NULL, a, x, y);
    s->h.obj_flags |= 0x12;
    s->h.move_flags |= 0xa300;
    s->h.disp_flags |= 0x20;
    s->h.x = 0xe100;
    s->h.y = 0x20000;
    s->h.z = 0;
    p = &s->unk23C;
    Model_Init(p, 0);
    Model_SetResource(p, s->unk22C);
    p->trans[0] = s->h.x << 4;
    p->trans[1] = -(s->h.y << 4);
    p->trans[2] = s->h.z << 4;
    p->scale[0] = p->scale[1] = p->scale[2] = 0x34cc;
    s->unk210 = Boss1Neck_Create();     /* MapObject_Spawn(0xca, ...) */
    s->unk214 = Boss1Hammer_Create();   /* MapObject_Spawn(0xcb, ...) */
    Boss1_SetPalettes(g_boss_game.unk0, 0);
    /* NNS_G3dResDefaultSetup(exc:eff_hit/eff_impact_small/eff_impact_big.nsbmd): boss_effects_load */
    boss_log("create", gBossStatus.unk8, s->unk21A);
    return s;
}

/* ============================================================ module glue */
/* BossStage1_OnModelLoaded: data_02074308 = bs1_base, bs1_neck, bs1_hammer, bs1_sky */
int boss1_load(void) {
    static const char *const files[4] = {"mod__bs1_base__bs1_base.dcm", "mod__bs1_neck__bs1_neck.dcm",
                                         "mod__bs1_hammer__bs1_hammer.dcm", "mod__bs1_sky__bs1_sky.dcm"};
    int i, r = 0;
    for (i = 0; i < 4; i++)
        if (!data_ov000_02369ad0[i].loaded)
            r |= boss_res_load_model(&data_ov000_02369ad0[i], files[i]);
    if (!s_bs1_anim.loaded)
        r |= boss_res_load_anim(&s_bs1_anim, "narc__z13_act_lz7__bs1.dca");
    s_joint_neck_ham = boss_joint_index("bs1_neck", "neck_ham");
    s_joint_hammer = boss_joint_index("bs1_hammer", "hammer");
    if (s_joint_neck_ham < 0 || s_joint_hammer < 0)
        r = -1;
    return r ? -1 : 0;
}

/* BossStage1_FreeModels */
void boss1_unload(void) {
    int i;
    for (i = 0; i < 4; i++)
        boss_res_free_model(&data_ov000_02369ad0[i]);
    boss_res_free_anim(&s_bs1_anim);
}

void boss1_destroy(void) {
    if (s_hammer.alive)
        Boss1Hammer_Destructor(&s_hammer);
    if (s_neck.alive)
        Boss1Neck_Destructor(&s_neck);
    if (s_boss1.alive)
        Boss1_Destructor(&s_boss1);
    s_hammer.alive = s_neck.alive = s_boss1.alive = 0;
    gBoss1 = NULL;
}

static const BossObjOps s_ops_boss1 = {Boss1_Draw, NULL, Boss1_Destructor};
static const BossObjOps s_ops_neck = {Boss1Neck_Draw, NULL, Boss1Neck_Destructor};
static const BossObjOps s_ops_hammer = {Boss1Hammer_Draw, Boss1Hammer_Collide, Boss1Hammer_Destructor};

void boss1_update(void) {
    if (s_boss1.alive)
        boss_obj_main(&s_boss1.h, &s_boss1.func, &s_ops_boss1, &s_boss1, &s_boss1.alive);
    if (s_neck.alive)
        boss_obj_main(&s_neck.h, &s_neck.func, &s_ops_neck, &s_neck, &s_neck.alive);
    if (s_hammer.alive)
        boss_obj_main(&s_hammer.h, &s_hammer.func, &s_ops_hammer, &s_hammer, &s_hammer.alive);
}

int boss1_draw(int list) {
    int n = 0;
    g_boss_debug_color = 0x808080;
    if (s_boss1.alive && !(s_boss1.h.disp_flags & 0x20))
        n += boss_model_draw(&s_boss1.unk23C, list);
    g_boss_debug_color = 0x40ff40;
    if (s_neck.alive && !(s_neck.h.disp_flags & 0x20))
        n += boss_model_draw(&s_neck.unk228, list);
    g_boss_debug_color = 0xff4040;
    if (s_hammer.alive && !(s_hammer.h.disp_flags & 0x20))
        n += boss_model_draw(&s_hammer.unk30C, list);
    g_boss_debug_color = 0xffff40;
    return n;
}

void boss1_hammer_pos(s32 *x, s32 *y, s32 *z) {
    *x = s_hammer.h.x;
    *y = s_hammer.h.y;
    *z = s_hammer.h.z;
}
int boss1_hammer_box_armed(int box) { return box >= 0 && box < 4 && (s_hammer.unk20C[box].flags & 4) != 0; }

/* state names for the test log */
#define N(f) {(BossFn)f, #f}
static const struct { BossFn f; const char *n; } s_names[] = {
    N(Boss1_State_WaitForStart), N(Boss1_State_Idle), N(Boss1_State_Hurt), N(Boss1_State_Defeat),
    N(Boss1_State_DefeatFade_ov000_022cab0c), N(Boss1_State_DefeatCamera2), N(Boss1_State_DefeatFade_ov000_022ca9cc),
    N(Boss1_State_DefeatCamera3), N(Boss1_State_DefeatFade_ov000_022ca890), N(Boss1_State_DefeatExplode),
    N(Boss1_State_DefeatFinish), N(Boss1_State_WaitSlam), N(Boss1_State_WaitTripleSlam), N(Boss1_State_WaitBigSlam),
    N(Boss1_State_WaitHammerRoll), N(Boss1_State_WaitMissileAttack),
    N(Boss1Neck_State_Idle), N(Boss1Neck_State_FollowPlayer), N(Boss1Neck_State_Hurt), N(Boss1Neck_State_Defeated),
    N(Boss1Neck_State_SlamApproach), N(Boss1Neck_State_SlamWindUp), N(Boss1Neck_State_SlamHold),
    N(Boss1Neck_State_SlamRaise), N(Boss1Neck_State_SlamStrike), N(Boss1Neck_State_SlamDown),
    N(Boss1Neck_State_SlamImpact), N(Boss1Neck_State_SlamImpactShake), N(Boss1Neck_State_SlamRecover),
    N(Boss1Neck_State_SlamRecoverWait), N(Boss1Neck_State_SlamDone), N(Boss1Neck_State_TripleSlamApproach),
    N(Boss1Neck_State_TripleSlamWindUp), N(Boss1Neck_State_TripleSlamHold), N(Boss1Neck_State_TripleSlamRaise),
    N(Boss1Neck_State_TripleSlamStrike), N(Boss1Neck_State_TripleSlamDown), N(Boss1Neck_State_TripleSlamAfterImpact),
    N(Boss1Neck_State_TripleSlamImpactShake), N(Boss1Neck_State_TripleSlamStartRewind),
    N(Boss1Neck_State_TripleSlamRewind), N(Boss1Neck_State_TripleSlamStartReraiseShake),
    N(Boss1Neck_State_TripleSlamReraiseShake), N(Boss1Neck_State_TripleSlamFinalImpact),
    N(Boss1Neck_State_TripleSlamFinalShake), N(Boss1Neck_State_TripleSlamRecover),
    N(Boss1Neck_State_TripleSlamRecoverWait), N(Boss1Neck_State_TripleSlamDone), N(Boss1Neck_State_BigSlamApproach),
    N(Boss1Neck_State_BigSlamWindUp), N(Boss1Neck_State_BigSlamHold), N(Boss1Neck_State_BigSlamRaise),
    N(Boss1Neck_State_BigSlamStrike), N(Boss1Neck_State_BigSlamDown), N(Boss1Neck_State_BigSlamImpact),
    N(Boss1Neck_State_BigSlamImpactShake), N(Boss1Neck_State_BigSlamRecover), N(Boss1Neck_State_BigSlamRecoverWait),
    N(Boss1Neck_State_BigSlamDone), N(Boss1Neck_State_RollStart), N(Boss1Neck_State_RollMoveToEdge),
    N(Boss1Neck_State_RollReleaseAnim), N(Boss1Neck_State_RollReleaseHammer), N(Boss1Neck_State_RollStartWait),
    N(Boss1Neck_State_WaitTimer_ov000_022c8b90), N(Boss1Neck_State_RollAnim2), N(Boss1Neck_State_RollWaitAnim2),
    N(Boss1Neck_State_RollAnim3), N(Boss1Neck_State_RollWaitAnim3), N(Boss1Neck_State_RollWaitHammer),
    N(Boss1Neck_State_RollReturnAnim1), N(Boss1Neck_State_RollReturnWait1), N(Boss1Neck_State_RollReturnAnim2),
    N(Boss1Neck_State_RollReturnWait2), N(Boss1Neck_State_RollRegrabHammer), N(Boss1Neck_State_RollReturnAnim3),
    N(Boss1Neck_State_RollReturnWait3), N(Boss1Neck_State_HammerRollDone), N(Boss1Neck_State_MissileWaitStart),
    N(Boss1Neck_State_MissileIntroAnim), N(Boss1Neck_State_MissileWaitIntro), N(Boss1Neck_State_MissileSetPose),
    N(Boss1Neck_State_MissilePoseAnim), N(Boss1Neck_State_MissileReadyAnim), N(Boss1Neck_State_MissileWaitReady),
    N(Boss1Neck_State_MissileSelectPattern), N(Boss1Neck_State_PickRandomTarget), N(Boss1Neck_State_AimAtTarget),
    N(Boss1Neck_State_FireAtTarget), N(Boss1Neck_State_StartTrackPlayer), N(Boss1Neck_State_TrackPlayer),
    N(Boss1Neck_State_FireAtPlayer), N(Boss1Neck_State_MissileEndAnim), N(Boss1Neck_State_MissileWaitEndAnim),
    N(Boss1Neck_State_MissileRetractAnim), N(Boss1Neck_State_MissileWaitRetract), N(Boss1Neck_State_MissileResetPose),
    N(Boss1Neck_State_MissileWaitResetPose), N(Boss1Neck_State_MissileDone),
    N(Boss1Hammer_State_Idle), N(Boss1Hammer_State_Idle_ov000_022c5f90), N(Boss1Hammer_State_Idle_ov000_022c5fa4),
    N(Boss1Hammer_State_Idle_ov000_022c5fb8), N(Boss1Hammer_State_Idle_ov000_022c6940),
    N(Boss1Hammer_State_Idle_ov000_022c6954), N(Boss1Hammer_State_Idle_ov000_022c6968), N(Boss1Hammer_State_Hurt),
    N(Boss1Hammer_State_Defeated), N(Boss1Hammer_State_WaitRelease), N(Boss1Hammer_State_StartFall),
    N(Boss1Hammer_State_Fall), N(Boss1Hammer_State_Land), N(Boss1Hammer_State_WaitTimer_ov000_022c67ec),
    N(Boss1Hammer_State_StartSpin), N(Boss1Hammer_State_SpinUp), N(Boss1Hammer_State_LaunchRoll),
    N(Boss1Hammer_State_Roll), N(Boss1Hammer_State_TurnAtEdge), N(Boss1Hammer_State_SlideToEdge),
    N(Boss1Hammer_State_FinishRoll), N(Boss1Hammer_State_SlideBackSpinDown), N(Boss1Hammer_State_EndRoll),
};
#undef N

const char *boss1_state_name(int part) {
    BossFn f = part == 0 ? s_boss1.func : part == 1 ? s_neck.func : s_hammer.func;
    unsigned i;
    for (i = 0; i < sizeof(s_names) / sizeof(s_names[0]); i++)
        if (s_names[i].f == f)
            return s_names[i].n + (part == 0 ? 6 : part == 1 ? 10 : 12);
    return "?";
}

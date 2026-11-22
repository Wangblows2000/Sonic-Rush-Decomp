/*
 * Boss3 (Mirage Road act 3, stage z33): native translation of
 * src/game/Zone/Mirage_Road/Boss/ (Boss3_*, Boss3PartCC_*, Boss3PartCD_*,
 * BossStage3_*): all 153 functions.
 *
 *   Boss3        (task 0x500, map object 0x3d)  the arena (bs3_base) and the fight director
 *   Boss3PartCC  (task 0x501, MapObject_Spawn(0xcc))  the boss body (bs3_body)
 *   Boss3PartCD  (task 0x502, MapObject_Spawn(0xcd))  the ball (bs3_ball + bs3_thorn);
 *                several can live at once (thrown balls + the held one)
 *
 * Every function keeps its original name and body; the per-file Unk structs
 * of the decompilation are merged into Boss3 / Boss3PartCC / Boss3PartCD with
 * the original offsets in the field names (Model fields: unk2C4.unk10C =
 * unk2C4.flags[0], .unk114 = .speed, .unk12E = .blendSpeeds[0], .unk24 = .rot,
 * .unk48/4C/50 = .trans, .unkE4 = .anm[0]; box flags unk13E = unk134.flags,
 * unk16A = unk160.flags).  DS services are replaced:
 *   - Task_Create / StageObj_Main   -> static parts (CC) and a ball pool run by
 *                                      boss3_update through boss_obj_main
 *   - Model_* / NNS_G3d*            -> boss_model.c (converted .dcm/.dca;
 *                                      bs3.nsbta = timing-only animations)
 *   - Boss3PartCC_NodeCallback_BodyBall (MTX_STORE of "body_ball") + MTX_RESTORE +
 *     NNS_G3dGetCurrentMtx             -> boss_model_joint_world into g_boss_mtx_slot[0x1e]
 *   - ObjRect_*                      -> dreamcast/game/objects (obj_rect.c)
 *   - palette / material swaps (Boss_LoadModelPalettes, NNSi_G3dModifyMatFlag,
 *     NNS_G3dGlbMaterialColorSpecEmi) -> selection state only (textures are pre-decoded)
 *   - Sound handles / NaviMsg / StageShutdown / RingManager -> g_boss_hooks
 *   - display registers (BG2 cut-in, master brightness) -> g_boss_game.exc_bg / bright
 * The player walks on a circle (radius 0xf42f8, circumference 0x5fe435 fx32 = 1534 px):
 * gPlayers->unk70 = Boss3_Draw_PlayerOnArena (g_boss_game.draw_player).
 */
#include <stdio.h>
#include <string.h>
#include "boss_internal.h"

/* object-system entry points (obj_rect.c) not declared in boss_internal.h */
void rect_set_box2d(ObjBox *b, s16 l, s16 t, s16 r, s16 bb); /* ObjRect_SetBox2D */
void rect_no_hit(ObjBox *a, ObjBox *b);                      /* ObjRect_FuncNoHit */
void obj_player_on_hurt(ObjBox *body, ObjBox *attacker);     /* Player_OnHurt */

typedef struct Boss3 Boss3;
typedef struct Boss3PartCC Boss3PartCC;
typedef struct Boss3PartCD Boss3PartCD;

struct Boss3 {                /* 0x374 bytes (Boss3_Create) */
    DcPlayer h;               /* StageObj header */
    BossFn func;              /* @0x68 */
    ObjBox unk134, unk160;    /* MapObject_Init default boxes (no owner: never registered) */
    u32 unk20C;               /* @0x20C bit0 pinch (health 2), bit1 throw attack (body off the
                                 arena circle), bit2 player chases a reflected ball, bit3 push turn */
    s32 unk210;               /* @0x210 */
    Boss3PartCC *unk214;      /* @0x214 body */
    Boss3PartCD *unk218;      /* @0x218 ball held / kicked (NULL: none) */
    u16 unk21C;               /* @0x21C attack pattern column */
    u16 unk21E;               /* @0x21E attack pattern row */
    u16 pad220;
    u16 unk222;               /* @0x222 attack level (table index) */
    s32 unk224;               /* @0x224 timer */
    const BossModelRes *unk22C; /* @0x22C bs3_base (data_ov000_02369b18[0]) */
    const BossAnimRes *unk230;  /* @0x230 bs3.nsbca (archive file 8) */
    const BossAnimRes *unk234;  /* @0x234 bs3.nsbta (archive file 9, timing only) */
    const BossModelRes *unk238; /* @0x238 bs3_sky (data_ov000_02369b18[4]) */
    BossModel unk23C;         /* @0x23C arena model (speed = unk350) */
    int alive;
};

struct Boss3PartCC {          /* 0x438 bytes (Boss3PartCC_Create) */
    DcPlayer h;               /* x @0x10 = arc position (24.8), vel_x @0x2E, vel_y @0x30, disp @0x5C */
    BossFn func;              /* @0x68 */
    ObjBox unk134;            /* @0x134 damage box (hit by a reflected ball), flags = unk13E */
    ObjBox unk160;            /* @0x160 body box (hurts the player), flags = unk16A */
    s32 unk20C, unk210, unk214; /* @0x20C draw offset (stagger shake), cleared after each draw */
    s32 unk218, unk21C, unk220; /* @0x218 jump start / target (fx32 world) */
    u16 unk224;               /* @0x224 facing angle (throw attack) */
    fx32 unk228[12];          /* @0x228 MtxFx43 of the "body_ball" node (unk24C/250/254 = translation) */
    u16 unk258, unk25A, unk25C, unk25E; /* @0x258 timers / angles; unk25E = throw count */
    u16 unk264;               /* @0x264 camera shake timer */
    u32 unk2C0;               /* @0x2C0 sound handle */
    BossModel unk2C4;         /* @0x2C4 body model (flags[0] = unk3D0, speed = unk3D8) */
    const BossModelRes *unk430; /* @0x430 unk2C4.unk16C */
    const BossAnimRes *unk434;  /* @0x434 unk2C4.unk170 */
    int alive;
};

struct Boss3PartCD {          /* 0x578 bytes (Boss3PartCD_Create) */
    DcPlayer h;
    BossFn func;              /* @0x68 */
    ObjBox unk134;            /* @0x134 hittable box (atk 0x11 def 0), flags = unk13E */
    ObjBox unk160;            /* @0x160 harmful box (atk 0x13), flags = unk16A */
    u32 unk20C;               /* @0x20C bit0 free (rolling), bit1 thrown (flying), bit2 thorns out,
                                 bit3 harmful/hittable cycle, bit4 harmful, bit6 reflected by the
                                 player, bit7 being summoned */
    u16 unk210;               /* @0x210 roll angle */
    u16 unk212;               /* @0x212 scale (0x1000 = 1) */
    BossVec unk214;           /* @0x214 throw velocity (fx32 world) */
    u16 unk220;               /* @0x220 arena angle at the throw */
    u16 unk222;               /* @0x222 throw direction */
    u16 unk224;               /* @0x224 cycle period */
    u16 unk226;               /* @0x226 cycle counter */
    u16 unk228;
    u16 unk22A;               /* @0x22A hittable blink */
    s16 unk22C;               /* @0x22C explosion counter */
    u16 unk22E;
    u16 pad230, pad232;
    u16 unk234;               /* @0x234 camera shake timer */
    ObjBox unk238[2];         /* @0x238 the boxes one arena length to the side (wrap) */
    BossModel unk290;         /* @0x290 ball (rot unk2B4, trans unk2D8, speed unk3A4) */
    BossModel unk404;         /* @0x404 thorns (speed unk518) */
    int alive;
    u32 seq;                  /* native: creation order (task order within priority 0x502) */
};

#define BOSS3_BALLS 8
static Boss3 s_boss3;
static Boss3PartCC s_cc;
static Boss3PartCD s_cd[BOSS3_BALLS];
static u32 s_cd_seq;
static Boss3 *gBoss3;

/* BossStage3 resources: data_ov000_02369b18[5] = bs3_base, bs3_body, bs3_ball, bs3_thorn,
 * bs3_sky (data_02074408) and the zone act archive's bs3.nsbca / bs3.nsbta */
static BossModelRes data_ov000_02369b18[5];
static BossAnimRes s_bs3_anim, s_bs3_srt;
static int s_joint_body_ball;
static s32 s_pal[5];          /* native: Boss_LoadModelPalettes state (first palette index per model) */
static int s_ball_matflag;    /* native: NNSi_G3dModifyMatFlag(ball, 0x400) state (1 = hittable white) */

/* ROM tables (bossdata.bin) */
#define data_ov000_0236306c BOSS_DATA(0x0236306c, const u16 *)
#define data_ov000_02363070 BOSS_DATA(0x02363070, const u16 *)
#define data_ov000_02363074 BOSS_DATA(0x02363074, const u16 *)
#define data_ov000_02363078 BOSS_DATA(0x02363078, const u8 *)
#define data_ov000_02363080 BOSS_DATA(0x02363080, const u8 *)
#define data_ov000_02363088 BOSS_DATA(0x02363088, const u8 *)
#define data_ov000_02363090 BOSS_DATA(0x02363090, const u8 *)
#define data_ov000_02363098 BOSS_DATA(0x02363098, const u8 *)
#define data_ov000_023630a0 BOSS_DATA(0x023630a0, const u8 *)
#define data_ov000_023630ac BOSS_DATA(0x023630ac, const u16 *)
#define data_ov000_023630b8 BOSS_DATA(0x023630b8, const s16 *)
#define data_ov000_023630c4 BOSS_DATA(0x023630c4, const u16 *)
#define data_ov000_023630d0 BOSS_DATA(0x023630d0, const u8(*)[8])
#define data_ov000_023630e0 BOSS_DATA(0x023630e0, const char *)
#define data_ov000_023630f0 BOSS_DATA(0x023630f0, const u8(*)[8])
#define data_ov000_02363100 BOSS_DATA(0x02363100, const u8(*)[8])
#define data_ov000_02363110 BOSS_DATA(0x02363110, const u8(*)[8])
#define data_ov000_02363120 BOSS_DATA(0x02363120, const u8(*)[8])
#define data_ov000_02363130 BOSS_DATA(0x02363130, const u16(*)[6])
#define data_ov000_02363148 BOSS_DATA(0x02363148, const u16(*)[6])
#define data_ov000_02363160 BOSS_DATA(0x02363160, const u16(*)[6])
#define data_ov000_02363178 BOSS_DATA(0x02363178, const u8(*)[8])
#define data_ov000_02363198 BOSS_DATA(0x02363198, const u8(*)[8])
#define data_ov000_023631b8 BOSS_DATA(0x023631b8, const u8(*)[8])
#define data_ov000_023631d8 BOSS_DATA(0x023631d8, const u16(*)[9])
#define data_ov000_023631fc BOSS_DATA(0x023631fc, const s16(*)[9])
#define data_ov000_02363220 BOSS_DATA(0x02363220, const u16(*)[5])
#define data_ov000_0236327c BOSS_DATA(0x0236327c, const s32(*)[5])
#define data_ov000_02363330 BOSS_DATA(0x02363330, const s32(*)[5])
#define data_ov000_023633e4 BOSS_DATA(0x023633e4, const u8(*)[9][16])
#define data_ov000_02365414 BOSS_DATA(0x02365414, const u32 *)
#define data_ov000_02365488 BOSS_DATA(0x02365488, const u32 *)

#define CAM data_ov000_0236a444
#define gGameState (g_boss_game.unk0) /* gGameState.unk0: player slot (palette tables) */

static inline s32 GetIdx(void) {
    dcp_bool b = DCP_TRUE;
    if (g_dcp_world.mode != 2 && g_dcp_world.mode != 3)
        b = DCP_FALSE;
    if (b)
        return 1;
    return g_boss_game.unk14;
}
static inline s32 GetIdx2(void) {
    switch (g_dcp_world.mode) {
    case 2:
    case 3:
        return 1;
    }
    return g_boss_game.unk14;
}
static inline fx32 FX_Inv(fx32 x) { return FX_Div(0x1000, x); }

/* VEC_Mag (NitroSDK: CP sqrt of 4 * |v|^2, rounded) */
static u64 isqrt64(u64 v) {
    u64 r = 0, b = (u64)1 << 62;
    while (b > v)
        b >>= 2;
    while (b != 0) {
        if (v >= r + b) {
            v -= r + b;
            r = (r >> 1) + b;
        } else
            r >>= 1;
        b >>= 2;
    }
    return r;
}
static fx32 VEC_Mag(const BossVec *v) {
    s64 l2 = (s64)v->x * v->x + (s64)v->y * v->y + (s64)v->z * v->z;
    return (fx32)((isqrt64((u64)l2 * 4) + 1) >> 1);
}

/* float joint matrix -> MtxFx43 (NNS_G3dGetCurrentMtx) */
static void mtx_to_fx43(const dcm_mtx_t *m, fx32 *out) {
    int r, c;
    for (r = 0; r < 4; r++)
        for (c = 0; c < 3; c++) {
            float v = m->m[r][c] * 4096.0f;
            out[r * 3 + c] = (fx32)(v >= 0 ? v + 0.5f : v - 0.5f);
        }
}

static void Player_State_Blank(DcPlayer *p) { (void)p; }
/* Player_Action_Blank (player module lacks it) */
static void Player_Action_Blank(DcPlayer *s) {
    dcp_player_init_state(s);
    dcp_reset_gimmick_state(s);
    s->state = Player_State_Blank;
}

/* Boss_LoadModelPalettes(model, names, indices, slots, n): the .dcm textures are
 * pre-decoded, so only the selection is kept (BOSSES.md) */
static void Boss_LoadModelPalettes(int model, const u32 *names, const u16 *idx, const u16 *slots, u16 n) {
    (void)names;
    (void)slots;
    (void)n;
    if (model >= 0 && model < 5 && idx != NULL)
        s_pal[model] = idx[0];
}

/* sound handle helpers: Sound_PlaySeqArc(handle, 0, id), NNS_SndPlayerStopSeq + ReleaseSeq */
#define SndPlayHandle(h, id) boss_sfx_handle((h), (id))
#define SndStopHandle(h) boss_sfx_stop(h)

/* forward declarations */
#define DECL(f) static void f(void *p);
DECL(Boss3_State_WaitForStart) DECL(Boss3_State_Idle) DECL(Boss3_State_WaitHurt) DECL(Boss3_State_WaitKickBall)
DECL(Boss3_State_WaitPushBall) DECL(Boss3_State_WaitSummonBall) DECL(Boss3_State_WaitThrowAttack)
DECL(Boss3_State_StartDefeat) DECL(Boss3_State_DefeatCamera1) DECL(Boss3_State_DefeatCamera2)
DECL(Boss3_State_DefeatCamera3) DECL(Boss3_State_DefeatFade_ov000_022d94fc) DECL(Boss3_State_DefeatFade_ov000_022d96d8)
DECL(Boss3_State_DefeatFade_ov000_022d98b0) DECL(Boss3_State_DefeatExplode) DECL(Boss3_State_DefeatFinish)
DECL(Boss3PartCC_State_Idle) DECL(Boss3PartCC_State_StartIdle) DECL(Boss3PartCC_State_Idle_ov000_022d8bb8)
DECL(Boss3PartCC_State_Defeated) DECL(Boss3PartCC_State_StartHurt) DECL(Boss3PartCC_State_HurtKnockback)
DECL(Boss3PartCC_State_HurtDone) DECL(Boss3PartCC_State_KickWindUp) DECL(Boss3PartCC_State_KickWindUpWait)
DECL(Boss3PartCC_State_KickBall) DECL(Boss3PartCC_State_KickFollowThrough) DECL(Boss3PartCC_State_StartWaitAfterKick)
DECL(Boss3PartCC_State_WaitAfterKick) DECL(Boss3PartCC_State_StartTurn) DECL(Boss3PartCC_State_Turn)
DECL(Boss3PartCC_State_SelectWatchOrCatch) DECL(Boss3PartCC_State_WatchBall) DECL(Boss3PartCC_State_JumpOverBall)
DECL(Boss3PartCC_State_JumpTakeoff) DECL(Boss3PartCC_State_JumpRise) DECL(Boss3PartCC_State_JumpFallStart)
DECL(Boss3PartCC_State_JumpFall) DECL(Boss3PartCC_State_JumpLandTurn) DECL(Boss3PartCC_State_StartWaitCatchBall)
DECL(Boss3PartCC_State_WaitCatchBall) DECL(Boss3PartCC_State_CatchBall) DECL(Boss3PartCC_State_CatchBallWait)
DECL(Boss3PartCC_State_CatchBallDone) DECL(Boss3PartCC_State_PushSelectDirection) DECL(Boss3PartCC_State_PushTurn)
DECL(Boss3PartCC_State_PushTurnWait) DECL(Boss3PartCC_State_StartPushBall) DECL(Boss3PartCC_State_PushBall)
DECL(Boss3PartCC_State_PushEnd) DECL(Boss3PartCC_State_PushDone) DECL(Boss3PartCC_State_StartStagger)
DECL(Boss3PartCC_State_Stagger) DECL(Boss3PartCC_State_SummonBall) DECL(Boss3PartCC_State_SummonBallGrow)
DECL(Boss3PartCC_State_SummonBallDone) DECL(Boss3PartCC_State_ThrowAttackJump) DECL(Boss3PartCC_State_ThrowAttackTurn)
DECL(Boss3PartCC_State_StartMoveToCentre) DECL(Boss3PartCC_State_MoveToCentre) DECL(Boss3PartCC_State_StartThrows)
DECL(Boss3PartCC_State_StartThrowAim) DECL(Boss3PartCC_State_ThrowAim) DECL(Boss3PartCC_State_ThrowWindUp)
DECL(Boss3PartCC_State_ThrowWindUpTurn) DECL(Boss3PartCC_State_ThrowBall) DECL(Boss3PartCC_State_ThrowFollowThrough)
DECL(Boss3PartCC_State_SpawnBall) DECL(Boss3PartCC_State_WaitBallSpawned) DECL(Boss3PartCC_State_EndThrows)
DECL(Boss3PartCC_State_WaitTimer_ov000_022d7754) DECL(Boss3PartCC_State_PickEdgeTarget)
DECL(Boss3PartCC_State_TurnToEdgeTarget) DECL(Boss3PartCC_State_StartJumpToEdge) DECL(Boss3PartCC_State_JumpToEdge)
DECL(Boss3PartCC_State_LandAtEdge) DECL(Boss3PartCC_State_LandAtEdgeShake) DECL(Boss3PartCC_State_FacePlayerSide)
DECL(Boss3PartCC_State_TurnToSide) DECL(Boss3PartCC_State_ReturnDone)
DECL(Boss3PartCD_State_Idle) DECL(Boss3PartCD_State_Defeated) DECL(Boss3PartCD_State_StartInert)
DECL(Boss3PartCD_State_Inert) DECL(Boss3PartCD_State_StartHarmful) DECL(Boss3PartCD_State_Harmful)
DECL(Boss3PartCD_State_StartHittable) DECL(Boss3PartCD_State_Hittable) DECL(Boss3PartCD_State_ThrowStart)
DECL(Boss3PartCD_State_Fly) DECL(Boss3PartCD_State_Explode) DECL(Boss3PartCD_State_Exploding)
#undef DECL
static void Boss3_ResetCamera(void);
static void Boss3_Action_DefeatAll(void);
static void Boss3_Action_HurtAll(void);
static void Boss3_MakeBallHarmful(void);
static void Boss3_MakeBallHittable(void);
static void Boss3_SetBallHarmful(Boss3PartCD *s);
static void Boss3_SetBallHittable(Boss3PartCD *s);
static void Boss3_SetBallInert(Boss3PartCD *s);
static void Boss3_LoadBallPalette(s32 k, dcp_bool f);
static void Boss3PartCD_Action_Throw(Boss3PartCD *s);
static Boss3PartCD *Boss3PartCD_Create(const ObjParam *a, s32 x, s32 y);

/* ============================================================ helpers */
/* Boss3_ArenaDistance (copied) */
static s32 Boss3_ArenaDistance(s32 a, s32 b, dcp_bool dir) {
    if (dir) {
        if (a >= b)
            return a - b;
        return 0x5fe43 - (b - a);
    }
    if (a <= b)
        return b - a;
    return 0x5fe43 - (a - b);
}

/* Boss3_StepAngleToward (copied) */
static u16 Boss3_StepAngleToward(u16 cur, u16 target, u16 step) {
    u16 d;
    if (target == cur)
        return cur;
    d = target - cur;
    if (d < 0x8000) {
        if (d < step)
            step = d;
        return cur + step;
    }
    d = cur - target;
    if (d < step)
        step = d;
    return cur - step;
}

/* Boss3_UpdateCameraShake (copied; data_ov000_0236a444.unk3C/40/44 = camera shake offset) */
static void Boss3_UpdateCameraShake(u16 *p, s32 a, s32 b) {
    s32 t;
    t = (*p << 12) >> b;
    CAM.unk3C.x = FX_Mul((ScreenShake_GetOffsetX() << 12) * a, t);
    t = (*p << 12) >> b;
    CAM.unk3C.y = FX_Mul((ScreenShake_GetOffsetY() << 12) * a, t);
    t = (*p << 12) >> b;
    CAM.unk3C.z = FX_Mul((ScreenShake_GetOffsetY() << 12) * a, t);
    if (*p != 0)
        (*p)--;
}

/* Boss3_LoadPalettes (modified: palette selection state only) */
static void Boss3_LoadPalettes(s32 k, dcp_bool f) {
    const u16 *t;
    if (f)
        t = data_ov000_02363160[k];
    else
        t = data_ov000_02363148[k];
    Boss_LoadModelPalettes(1, data_ov000_02365488, t, data_ov000_023630ac, 6);
}

/* Boss3_LoadBallPalette (modified: NNS_G3dGlbMaterialColorSpecEmi / NNSi_G3dModifyMatFlag and the
 * palette load become selection state) */
static void Boss3_LoadBallPalette(s32 k, dcp_bool f) {
    const u16 *t;
    if (f) {
        s_ball_matflag = 1; /* NNS_G3dGlbMaterialColorSpecEmi(0x2108, 0x2108, 0); ModifyMatFlag(q, 0, 0x400) */
        t = &data_ov000_02363070[k];
    } else {
        s_ball_matflag = 0; /* NNSi_G3dModifyMatFlag(q, 1, 0x400) */
        t = &data_ov000_02363074[k];
    }
    Boss_LoadModelPalettes(2, data_ov000_02365414, t, data_ov000_0236306c, 1);
}

/* Boss3_ResetCamera (copied; c = camera + 0x48) */
static void Boss3_ResetCamera(void) {
    gBoss3->unk210 = 0;
    CAM.unkDC = 2;
    CAM.unkC = 0x1a66000;
    CAM.unk14 = FX_Div(0x1000, 0x34cc);
    CAM.t[0].ox = CAM.t[0].oy = CAM.t[0].oz = NULL;          /* c->unk0..8 */
    CAM.t[0].offx = CAM.t[0].offy = CAM.t[0].offz = 0;       /* c->unkC..14 */
    CAM.t[0].lerp = 0x200;                                   /* c->unk30 */
    CAM.t[0].accel = 0;                                      /* c->unk38 */
    CAM.t[1].ox = NULL;                                      /* c->unk3C */
    CAM.t[1].oy = NULL;                                      /* c->unk40 */
    CAM.t[1].oz = NULL;                                      /* c->unk44 */
    CAM.t[1].tgt.x = 0;                                      /* c->unk54 */
    CAM.t[1].tgt.y = -0x200000;                              /* c->unk58 */
    CAM.t[1].tgt.z = 0;                                      /* c->unk5C */
    CAM.t[1].offx = 0;                                       /* c->unk48 */
    CAM.t[1].offy = 0x28000;                                 /* c->unk4C */
    CAM.t[1].offz = 0;                                       /* c->unk50 */
    CAM.t[1].lerp = 0x200;                                   /* c->unk6C */
    CAM.t[1].accel = 0;                                      /* c->unk74 */
    CAM.unkC0 = CAM.unkC4 = 0x1a82f8;                        /* c->unk78 / unk7C */
    CAM.unkC8 = 0x10;                                        /* c->unk80 */
    CAM.unkCC = CAM.unkD0 = 0x30000;                         /* c->unk84 / unk88 */
    CAM.unkD4 = 0x10;                                        /* c->unk8C */
    CAM.unkD6 = CAM.unkD8 = 0;                               /* c->unk8E / unk90 */
    CAM.unkDA = 0xccc;                                       /* c->unk92 */
}

/* Boss3PartCC_IsOtherOnNegativeSide (copied) */
static dcp_bool Boss3PartCC_IsOtherOnNegativeSide(const DcPlayer *a, const DcPlayer *b) {
    u16 a1;
    u16 a2;
    BossArena_ArcToCirclePos(0xf42f8, 0x5fe435, a->x << 4, NULL, NULL, &a1);
    BossArena_ArcToCirclePos(0xf42f8, 0x5fe435, b->x << 4, NULL, NULL, &a2);
    return (u16)(a2 - a1) >= 0x8000;
}

/* Boss3PartCC_CanCatchBall (copied; c->unk0 / unk8 = camera eye track objects x / z) */
static dcp_bool Boss3PartCC_CanCatchBall(Boss3PartCC *s) {
    s32 v;
    if (CAM.unkDC == 1) {
        Boss3PartCD *r = gBoss3->unk218;
        if (Boss3_ArenaDistance(r->h.x, s->h.x, r->h.vel_x <= 0) < 0xc800) {
            CAM.t[0].ox = NULL;
            CAM.t[0].oz = NULL;
        } else {
            CAM.t[0].ox = &gBoss3->unk218->h;
            CAM.t[0].oz = &gBoss3->unk218->h;
        }
    }
    BossArena_CirclePosToArc(0xf42f8, 0x5fe435, s->unk228[9], s->unk228[11], &v); /* unk24C / unk254 */
    v >>= 4;
    if (Boss3_ArenaDistance(v, gBoss3->unk218->h.x, !(s->h.disp_flags & 1)) < 0x800) {
        if (s->h.disp_flags & 1) {
            if (gBoss3->unk218->h.vel_x < 0)
                return DCP_TRUE;
        } else if (gBoss3->unk218->h.vel_x > 0) {
            return DCP_TRUE;
        }
    }
    return DCP_FALSE;
}

/* Boss3PartCC_MakeBallInert (copied) */
static void Boss3PartCC_MakeBallInert(void) { Boss3_SetBallInert(gBoss3->unk218); }

/* Boss3_MakeBallHarmful / Boss3_MakeBallHittable (copied) */
static void Boss3_MakeBallHarmful(void) { Boss3_SetBallHarmful(gBoss3->unk218); }
static void Boss3_MakeBallHittable(void) { Boss3_SetBallHittable(gBoss3->unk218); }

/* Boss3_SetBallHarmful (copied) */
static void Boss3_SetBallHarmful(Boss3PartCD *s) {
    Boss3_LoadBallPalette(gGameState, 0);
    if (!(s->unk20C & 4))
        s->func = Boss3PartCD_State_StartHarmful;
    else
        s->func = Boss3PartCD_State_Harmful;
}

/* Boss3_SetBallHittable (copied) */
static void Boss3_SetBallHittable(Boss3PartCD *s) {
    s->unk134.flags |= 4;  /* unk13E */
    s->unk160.flags &= ~4; /* unk16A */
    s->unk22A = 0;
    if (s->unk20C & 4)
        s->func = Boss3PartCD_State_StartHittable;
    else
        s->func = Boss3PartCD_State_Hittable;
}

/* Boss3_SetBallInert (copied) */
static void Boss3_SetBallInert(Boss3PartCD *s) {
    Boss3_LoadBallPalette(gGameState, 0);
    s->unk134.flags &= ~4;
    s->unk160.flags &= ~4;
    s->unk20C &= ~0x10;
    if (!(s->unk20C & 4))
        s->func = Boss3PartCD_State_StartInert;
    else
        s->func = Boss3PartCD_State_Inert;
}

/* ============================================================ Boss3PartCD (ball) */
/* Boss3PartCD_UpdateModeCycle (copied) */
static void Boss3PartCD_UpdateModeCycle(Boss3PartCD *s) {
    if (!(s->unk20C & 8))
        return;
    if (s->unk224 > s->unk226++)
        return;
    if (s->unk20C & 0x10)
        Boss3_MakeBallHittable();
    else
        Boss3_MakeBallHarmful();
    s->unk226 = 0;
}

/* Boss3PartCD_State_Idle / _Defeated (copied, empty) */
static void Boss3PartCD_State_Idle(void *p) { (void)p; }
static void Boss3PartCD_State_Defeated(void *p) { (void)p; }

/* Boss3PartCD_State_StartInert (copied) */
static void Boss3PartCD_State_StartInert(void *p) {
    Boss3PartCD *s = (Boss3PartCD *)p;
    s->unk20C |= 4;
    s->func = Boss3PartCD_State_Inert;
}

/* Boss3PartCD_State_Inert (copied: the ARM code passes the object in r0) */
static void Boss3PartCD_State_Inert(void *p) { Boss3PartCD_UpdateModeCycle((Boss3PartCD *)p); }

/* Boss3PartCD_State_StartHarmful (copied) */
static void Boss3PartCD_State_StartHarmful(void *p) {
    Boss3PartCD *s = (Boss3PartCD *)p;
    s->unk20C |= 4;
    if (!(s->unk20C & 0x80))
        Sound_PlaySeqArc(0, 0, 0x94);
    s->func = Boss3PartCD_State_Harmful;
}

/* Boss3PartCD_State_Harmful (copied) */
static void Boss3PartCD_State_Harmful(void *p) {
    Boss3PartCD *s = (Boss3PartCD *)p;
    s->unk134.flags &= ~4;
    s->unk160.flags |= 4;
    s->unk20C |= 0x10;
    Boss3PartCD_UpdateModeCycle(s);
}

/* Boss3PartCD_State_StartHittable (copied) */
static void Boss3PartCD_State_StartHittable(void *p) {
    Boss3PartCD *s = (Boss3PartCD *)p;
    s->unk20C &= ~4;
    if (!(s->unk20C & 0x80))
        Sound_PlaySeqArc(0, 0, 0xe6);
    s->func = Boss3PartCD_State_Hittable;
}

/* Boss3PartCD_State_Hittable (copied) */
static void Boss3PartCD_State_Hittable(void *p) {
    Boss3PartCD *s = (Boss3PartCD *)p;
    s->unk20C &= ~0x10;
    s->unk22A++;
    if (!(s->unk22A & 3)) {
        if (s->unk22A & 0x8000)
            Boss3_LoadBallPalette(gGameState, 0);
        else
            Boss3_LoadBallPalette(gGameState, 1);
        s->unk22A &= ~3;
        s->unk22A ^= 0x8000;
    }
    Boss3PartCD_UpdateModeCycle(s);
}

/* Boss3PartCD_State_ThrowStart (copied) */
static void Boss3PartCD_State_ThrowStart(void *p) {
    Boss3PartCD *s = (Boss3PartCD *)p;
    s->unk20C |= 4;
    s->func = Boss3PartCD_State_Harmful;
}

/* Boss3PartCD_State_Fly (copied; unk2D8 = ball model translation, unk2B4 = its rotation) */
static void Boss3PartCD_State_Fly(void *p) {
    Boss3PartCD *s = (Boss3PartCD *)p;
    MtxFx33 mtx;
    BossVec v;
    u16 ang;
    dcp_bool flag = DCP_FALSE;
    fx32 *t = s->unk290.trans;
    t[0] += s->unk214.x; /* VEC_Add(&s->unk2D8, &s->unk214, &s->unk2D8) */
    t[1] += s->unk214.y;
    t[2] += s->unk214.z;
    s->unk214.y -= 0x199;
    ang = (u16)Boss_Atan2(s->unk214.x, s->unk214.z);
    s->unk210 += (u16)(VEC_Mag(&s->unk214) >> 4);
    MTX_RotX33(&s->unk290.rot, SIN(s->unk210), COS(s->unk210));
    MTX_RotY33(&mtx, SIN(ang), COS(ang));
    MTX_Concat33(&s->unk290.rot, &mtx, &s->unk290.rot);
    v.x = t[0];
    v.y = 0;
    v.z = t[2];
    if (VEC_Mag(&v) > 0xf42f8 || t[1] < -0x200000)
        flag = DCP_TRUE;
    if (flag)
        s->func = Boss3PartCD_State_Explode;
}

/* Boss3PartCD_State_Explode (copied) */
static void Boss3PartCD_State_Explode(void *p) {
    Boss3PartCD *s = (Boss3PartCD *)p;
    fx32 *t = s->unk290.trans;
    s->unk134.flags &= ~4;
    s->unk160.flags |= 4;
    s->unk20C |= 0x10;
    s->h.disp_flags |= 0x20;
    BossArena_CirclePosToArc(0xf42f8, 0x5fe435, t[0], t[2], &s->h.x);
    s->h.x >>= 4;
    s->h.y = 0x1e000;
    s->unk20C &= ~2;
    s->unk22C = 0;
    BossEffect_CreateShootBomb(1, t[0] >> 4, -t[1] >> 4, t[2] >> 4);
    Sound_PlaySeqArc(0, 0, 0x96);
    s->unk234 = 0x20;
    s->func = Boss3PartCD_State_Exploding;
    g_boss_stats.ball_explosions++;
    boss_log("ball_explode", s->h.x >> 8, 0);
}

/* Boss3PartCD_State_Exploding (copied) */
static void Boss3PartCD_State_Exploding(void *p) {
    Boss3PartCD *s = (Boss3PartCD *)p;
    Boss3_UpdateCameraShake(&s->unk234, 2, 6);
    if (s->unk22C++ > 10)
        s->unk160.flags &= ~4;
    if (s->unk234 == 0)
        s->h.obj_flags |= 4;
}

/* Boss3PartCD_Action_Defeat (copied) */
static void Boss3PartCD_Action_Defeat(Boss3PartCD *s) {
    s->h.vel_x = 0;
    s->func = Boss3PartCD_State_Defeated;
}

/* Boss3PartCD_Action_Throw (copied) */
static void Boss3PartCD_Action_Throw(Boss3PartCD *s) {
    Boss3 *p = gBoss3;
    s->unk214.y = data_ov000_02363330[p->unk222][p->unk214->unk25E];
    BossArena_ArcToCirclePos(0xf42f8, 0x5fe435, s->h.x << 4, NULL, NULL, &s->unk220);
    s->unk22E = 0;
    if (!(s->unk20C & 4))
        s->func = Boss3PartCD_State_ThrowStart;
    else
        s->func = Boss3PartCD_State_Fly;
}

/* Boss3PartCD_OnHit (modified: owner type check -> obj_box_player) */
static void Boss3PartCD_OnHit(ObjBox *a, ObjBox *b) {
    DcPlayer *p = obj_box_player(b);
    Boss3PartCD *o = (Boss3PartCD *)a->owner;
    if (p == NULL) /* p->unk0 != 1 */
        return;
    if (p->anim == 0x30) { /* unk328: jump (ball) animation */
        if (gBossStatus.unk4 == 0xe) {
            gBoss3->unk214->func = Boss3PartCC_State_StartStagger; /* unk68 */
            p->hitstop = 4;                                          /* unk62 */
            g_boss_stats.staggers++;
            boss_log("stagger", o->h.x >> 8, 0);
        } else if (gBossStatus.unk4 == 0x10) {
            p->hitstop = 4;
            o->h.hitstop = 4;
            if ((o->unk20C & 1) && !(o->unk20C & 0x40)) {
                o->unk20C |= 0x40;
                if (o->h.vel_x > 0)
                    o->h.vel_x = -0x600;
                else
                    o->h.vel_x = 0x600;
                o->unk20C &= ~8;
                Boss3_MakeBallHittable(); /* (o): the ARM code ignores the argument */
                gPlayers->pf |= 0x200000;     /* unk32C */
                gPlayers->released = 0;       /* unk424 */
                gPlayers->pressed = gPlayers->released;
                gPlayers->held = gPlayers->pressed;
                gBoss3->unk20C |= 4;
                gBoss3->unk210 = 1;
                g_boss_stats.reflects++;
                boss_log("reflect", o->h.x >> 8, o->h.vel_x);
            }
        }
        p->push_y = (s16)(((a->y + a->t - b->b) << 8) - p->y); /* unk38 */
        obj_player_attack_bounce(p);
        BossEffect_CreateClash(1, g_boss_game.player_trans[0] >> 4, -g_boss_game.player_trans[1] >> 4,
                               g_boss_game.player_trans[2] >> 4); /* unk17C / unk180 / unk184 */
        Sound_PlaySeqArc(0, 0, 0xcb);
    } else if (p->invuln == 0) { /* unk3B2 */
        obj_player_on_hurt(b, a);
    }
}

/* Boss3PartCD_Collide (modified: StageObj_HandleCollider = obj_handle_collider) */
static void Boss3PartCD_Collide(void *p) {
    Boss3PartCD *o = (Boss3PartCD *)p;
    ObjBox *box[2];
    s32 i;
    if (o->h.obj_flags & 0xc)
        return;
    box[0] = &o->unk134;
    box[1] = &o->unk160;
    for (i = 0; i < 2; i++) {
        if (box[i]->owner != NULL) {
            obj_handle_collider(&o->h, o, OBJ_OWNER_OBJECT, box[i], 0);
            o->unk238[i] = *box[i];
            obj_handle_collider(&o->h, o, OBJ_OWNER_OBJECT, &o->unk238[i], 0);
            if (o->unk238[i].x + 0x40 < 0x2ff)
                o->unk238[i].x += 0x5fe;
            else
                o->unk238[i].x -= 0x5fe;
        }
    }
    /* o->unk1B8 (ObjCollisionMap) is never set for the ball */
}

/* Boss3PartCD_Draw (modified: Model_Draw -> boss_model_pose; a->unk24 = MtxFx43 rot + trans) */
static void Boss3PartCD_Draw(void *p) {
    Boss3PartCD *o = (Boss3PartCD *)p;
    BossModel *a = &o->unk290;
    BossModel *b = &o->unk404;
    s32 sc = FX_Mul(o->unk212, 0x34cc);
    u16 ang;
    MtxFx33 m1;
    MtxFx33 m2;
    if (o->unk20C & 1) {
        a->cb[0] = MOP_FLUSHP; /* unkA = 3 */
        a->cb[1] = 0;          /* unkB = 0 */
        if (!(o->unk20C & 2)) {
            if (o->h.x < 0x4000)
                o->h.x += 0x5fe43;
            else if (o->h.x > 0x63e43)
                o->h.x -= 0x5fe43;
            BossArena_ArcToCirclePos(0xf42f8, 0x5fe435, o->h.x << 4, &a->trans[0], &a->trans[2], &ang);
            a->trans[1] = -o->h.y << 4;
            if (!(o->h.disp_flags & 0x10))
                o->unk210 -= o->h.vel_x;
            MTX_RotZ33(&a->rot, SIN(o->unk210), COS(o->unk210));
            MTX_RotY33(&m1, SIN(ang), COS(ang));
            MTX_Concat33(&a->rot, &m1, &a->rot);
        }
    } else {
        s32 i;
        a->cb[0] = MOP_FLUSHP;
        a->cb[1] = 0;
        for (i = 0; i < 9; i++) /* a->unk24 = gBoss3->unk214->unk228 */
            a->rot.m[i] = gBoss3->unk214->unk228[i];
        for (i = 0; i < 3; i++)
            a->trans[i] = gBoss3->unk214->unk228[9 + i];
        BossArena_CirclePosToArc(0xf42f8, 0x5fe435, a->trans[0], a->trans[2], &o->h.x);
        o->h.x >>= 4;
        BossArena_ArcToCirclePos(0xf42f8, 0x5fe435, o->h.x << 4, NULL, NULL, &ang);
        MTX_RotZ33(&a->rot, SIN(o->unk210), COS(o->unk210));
        MTX_RotY33(&m2, SIN(ang), COS(ang));
        MTX_Concat33(&a->rot, &m2, &a->rot);
    }
    a->scale[2] = sc;
    a->scale[1] = a->scale[2];
    a->scale[0] = a->scale[1];
    b->trans[0] = a->trans[0];
    b->trans[1] = a->trans[1];
    b->trans[2] = a->trans[2];
    b->rot = a->rot;
    b->scale[2] = sc;
    b->scale[1] = b->scale[2];
    b->scale[0] = b->scale[1];
    if (o->h.disp_flags & 0x20) {
        a->posed = b->posed = 0;
        return;
    }
    Model_Animate(a);
    boss_model_pose(a, NULL);
    b->posed = 0;
    if (o->unk20C & 4) {
        Model_Animate(b);
        boss_model_pose(b, NULL);
    }
}

/* Boss3PartCD_Destructor (modified: resource release, boxes forgotten) */
static void Boss3PartCD_Destructor(void *p) {
    Boss3PartCD *o = (Boss3PartCD *)p;
    boss_model_release(&o->unk290);
    boss_model_release(&o->unk404);
    rect_forget(&o->unk134);
    rect_forget(&o->unk160);
    rect_forget(&o->unk238[0]);
    rect_forget(&o->unk238[1]);
    boss_obj_set_model(&o->h, NULL);
    if (gBoss3 != NULL && gBoss3->unk218 == o)
        gBoss3->unk218 = NULL; /* native: the DS task memory is freed; nothing reads it afterwards */
}

/* MapObject_Init (obj_manager.c obj_mapobject_init) fields the boss parts use */
static void MapObject_Init_parts(DcPlayer *h, ObjBox *coll0, ObjBox *coll1, const ObjParam *prm, s32 x, s32 y) {
    h->obj_flags |= 0x40;
    h->x = x;
    h->y = y;
    h->type = (prm != NULL && prm->type < 0x40) ? 2 : 3;
    h->gravity = 0x2a;
    h->fall_max = 0xf00;
    h->obj_flags |= 1;
    h->disp_flags |= 1;
    h->move_flags |= 0x200;
    coll0->mask = 0x12;
    coll1->mask = 0x12;
    coll0->cb = NULL; /* MapObject_OnHit_Default (replaced by every caller) */
    rect_set_atkdef(coll0, 9, 0xa);
    rect_set_atkdef(coll1, 0x13, 0x13);
    coll0->flags &= ~4;
    coll1->flags &= ~4;
}

/* Boss3PartCD_Create (modified: pool slot instead of Task_Create; models from BossStage3) */
static Boss3PartCD *Boss3PartCD_Create(const ObjParam *a, s32 x, s32 y) {
    Boss3PartCD *s = NULL;
    Boss3 *b = gBoss3;
    ObjBox *h;
    BossModel *p;
    int i;
    for (i = 0; i < BOSS3_BALLS; i++)
        if (!s_cd[i].alive) {
            s = &s_cd[i];
            break;
        }
    if (s == NULL) { /* MapObject_AllocSpawnEntry / Task_Create failure */
        boss_log("ball_pool_full", 0, 0);
        return NULL;
    }
    boss_model_release(&s->unk290);
    boss_model_release(&s->unk404);
    memset(s, 0, sizeof(*s)); /* MI_CpuFill16(s, 0, 0x578) */
    s->alive = 1;
    s->seq = ++s_cd_seq;
    s->func = Boss3PartCD_State_Idle;
    s->unk212 = 0x1000;
    /* s->unk290.unk170 = s->unk404.unk170 = bs3.nsbca (archive file 8): no ball animation is set */
    MapObject_Init_parts(&s->h, &s->unk134, &s->unk160, a, x, y);
    s->h.obj_flags |= 0x10;
    s->h.move_flags |= 0x8300;
    s->h.disp_flags |= 0x20;
    boss_obj_set_model(&s->h, &s->unk290); /* unk124 = &unk290 */
    s->h.x = x;
    s->h.y = b->h.y - 0x2000;
    s->h.z = b->h.z;
    h = &s->unk134;
    rect_set_box2d(h, -0x20, -0x20, 0x20, 0x20);
    rect_set_atkdef(h, 0x11, 0);
    h->owner = s;
    h->owner_kind = OBJ_OWNER_OBJECT;
    h->cb = Boss3PartCD_OnHit;
    h->flags &= ~4;
    h->flags |= 0x80;
    h = &s->unk160;
    rect_set_box2d(h, -0x20, -0x20, 0x20, 0x20);
    h->owner = s;
    h->owner_kind = OBJ_OWNER_OBJECT;
    h->flags &= ~4;
    p = &s->unk290;
    Model_Init(p, 0);
    Model_SetResource(p, &data_ov000_02369b18[2]);
    p->trans[0] = b->h.x << 4;
    p->trans[1] = -(b->h.y << 4);
    p->trans[2] = b->h.z << 4;
    p->scale[2] = 0x34cc;
    p->scale[1] = p->scale[2];
    p->scale[0] = p->scale[1];
    p->cb[0] = MOP_FLUSHVP; /* unkA = 4 */
    p = &s->unk404;
    Model_Init(p, 0);
    Model_SetResource(p, &data_ov000_02369b18[3]);
    p->trans[0] = b->h.x << 4;
    p->trans[1] = -(b->h.y << 4);
    p->trans[2] = b->h.z << 4;
    p->scale[2] = 0x34cc;
    p->scale[1] = p->scale[2];
    p->scale[0] = p->scale[1];
    p->cb[0] = MOP_FLUSHVP;
    g_boss_stats.balls_spawned++;
    return s;
}

/* ============================================================ Boss3PartCC (body) */
/* Boss3PartCC_State_Idle / _Defeated / _HurtDone / _CatchBallDone / _SummonBallDone /
 * _ReturnDone (copied, empty: the Boss3_Is*Done tests compare against them) */
static void Boss3PartCC_State_Idle(void *p) { (void)p; }
static void Boss3PartCC_State_Defeated(void *p) { (void)p; }
static void Boss3PartCC_State_HurtDone(void *p) { (void)p; }
static void Boss3PartCC_State_CatchBallDone(void *p) { (void)p; }
static void Boss3PartCC_State_SummonBallDone(void *p) { (void)p; }
static void Boss3PartCC_State_ReturnDone(void *p) { (void)p; }

/* Boss3PartCC_State_Idle_ov000_022d8bb8 (copied) */
static void Boss3PartCC_State_Idle_ov000_022d8bb8(void *p) { Boss3_UpdateCameraShake(&((Boss3PartCC *)p)->unk264, 1, 5); }

/* Boss3PartCC_State_StartIdle (copied) */
static void Boss3PartCC_State_StartIdle(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    s->func = Boss3PartCC_State_Idle_ov000_022d8bb8;
    s->func(s);
}

/* Boss3PartCC_Action_Idle (copied) */
static void Boss3PartCC_Action_Idle(Boss3PartCC *s) {
    Boss3PartCD *r = gBoss3->unk218;
    s->unk134.flags |= 4;
    s->unk160.flags |= 4;
    if (r != NULL) {
        if (r->unk20C & 0x10)
            Boss3_MakeBallHarmful();
        else
            Boss3_MakeBallHittable();
    }
    s->func = Boss3PartCC_State_StartIdle;
}

/* Boss3PartCC_Action_Defeat (modified: sound handle via hooks) */
static void Boss3PartCC_Action_Defeat(Boss3PartCC *s) {
    SndStopHandle(s->unk2C0);
    s->func = Boss3PartCC_State_Defeated;
}

/* Boss3PartCC_Action_Hurt (modified: sound handle via hooks) */
static void Boss3PartCC_Action_Hurt(Boss3PartCC *s) {
    s->h.vel_y = 0;
    s->h.y = 0x20000;
    SndStopHandle(s->unk2C0);
    s->func = Boss3PartCC_State_StartHurt;
}

/* Boss3PartCC_Action_StartKickBall / StartPushBall / StartSummonBall / StartThrowAttack (copied) */
static void Boss3PartCC_Action_StartKickBall(Boss3PartCC *s) {
    s->unk258 = 0;
    s->unk264 = 0;
    s->func = Boss3PartCC_State_KickWindUp;
}
static void Boss3PartCC_Action_StartPushBall(Boss3PartCC *s) {
    s->h.vel_x = 0;
    s->unk258 = 0;
    s->unk25A = data_ov000_023633e4[GetIdx()][gBoss3->unk222][0];
    if (s->unk25A != 0)
        Boss3_MakeBallHarmful();
    else
        Boss3_MakeBallHittable();
    s->func = Boss3PartCC_State_PushSelectDirection;
}
static void Boss3PartCC_Action_StartSummonBall(Boss3PartCC *s) { s->func = Boss3PartCC_State_SummonBall; }
static void Boss3PartCC_Action_StartThrowAttack(Boss3PartCC *s) { s->func = Boss3PartCC_State_ThrowAttackJump; }

/* ---------------------------------------------------------------- hurt */
/* Boss3PartCC_State_StartHurt (copied) */
static void Boss3PartCC_State_StartHurt(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 0x15);
    s->unk2C4.flags[0] &= ~4;
    s->unk2C4.speed = 0x1000;
    if (s->h.disp_flags & 1)
        s->h.vel_x = 0x400;
    else
        s->h.vel_x = -0x400;
    Sound_PlaySeqArc(0, 0, 0x7b);
    Sound_PlayVoice(0, 0, (Rand() & 1) ? 0xce : 0xcf);
    s->unk258 = 0;
    s->unk25A = 0;
    s->unk264 = 0x40;
    s->func = Boss3PartCC_State_HurtKnockback;
}

/* Boss3PartCC_State_HurtKnockback (modified: palette blink -> selection state) */
static void Boss3PartCC_State_HurtKnockback(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    s16 v;
    Boss3_UpdateCameraShake(&s->unk264, 2, 6);
    if (s->unk25A < 0x2d || (s->unk258 & 0x8000)) {
        s->unk25A++;
        s->unk258++;
        if (!(s->unk258 & 1)) {
            if (s->unk258 & 0x8000)
                Boss_LoadModelPalettes(1, data_ov000_02365488, data_ov000_02363148[gGameState], data_ov000_023630ac, 6);
            else
                Boss_LoadModelPalettes(1, data_ov000_02365488, data_ov000_02363160[gGameState], data_ov000_023630ac, 6);
            s->unk258 &= ~1;
            s->unk258 ^= 0x8000;
        }
    }
    v = s->h.vel_x;
    if (v < 0) {
        if (-v > 0xc)
            s->h.vel_x = v + 0xc;
        else
            s->h.vel_x = 0;
    } else if (v > 0) {
        if (v > 0xc)
            s->h.vel_x = v - 0xc;
        else
            s->h.vel_x = 0;
    }
    if (s->unk2C4.flags[0] & 2) {
        Boss3_ResetCamera();
        s->func = Boss3PartCC_State_HurtDone;
    }
}

/* ---------------------------------------------------------------- kick */
/* Boss3PartCC_State_KickWindUp (copied) */
static void Boss3PartCC_State_KickWindUp(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 0xd);
    s->unk2C4.flags[0] &= ~4;
    s->unk2C4.speed = 0x1000;
    Sound_PlayVoice(0, 0, 0xd1);
    s->func = Boss3PartCC_State_KickWindUpWait;
}

/* Boss3PartCC_State_KickWindUpWait (copied) */
static void Boss3PartCC_State_KickWindUpWait(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    if (!(s->unk2C4.flags[0] & 2))
        return;
    s->func = Boss3PartCC_State_KickBall;
    s->func(s);
}

/* Boss3PartCC_State_KickBall (copied) */
static void Boss3PartCC_State_KickBall(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Boss3PartCD *r = gBoss3->unk218;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 0xe);
    s->unk2C4.flags[0] &= ~4;
    s->unk2C4.speed = 0x1000;
    r->unk20C |= 1;
    if (s->h.disp_flags & 1)
        r->h.vel_x = data_ov000_023630b8[gBoss3->unk222];
    else
        r->h.vel_x = -data_ov000_023630b8[gBoss3->unk222];
    r->unk20C |= 8;
    r->unk224 = data_ov000_02363130[GetIdx()][gBoss3->unk222];
    SndPlayHandle(s->unk2C0, 0x93);
    s->func = Boss3PartCC_State_KickFollowThrough;
    boss_log("kick", r->h.vel_x, r->unk224);
}

/* Boss3PartCC_State_KickFollowThrough (copied) */
static void Boss3PartCC_State_KickFollowThrough(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    if (Boss3PartCC_CanCatchBall(s)) {
        s->func = Boss3PartCC_State_CatchBall;
        return;
    }
    if (s->unk2C4.flags[0] & 2)
        s->func = Boss3PartCC_State_StartWaitAfterKick;
}

/* Boss3PartCC_State_StartWaitAfterKick (copied) */
static void Boss3PartCC_State_StartWaitAfterKick(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 1);
    s->unk2C4.flags[0] |= 4;
    s->unk2C4.speed = 0x1000;
    s->unk25A = data_ov000_02363080[gBoss3->unk222] + (data_ov000_023630c4[gBoss3->unk222] & Rand());
    s->func = Boss3PartCC_State_WaitAfterKick;
    s->func(s);
}

/* Boss3PartCC_State_WaitAfterKick (copied) */
static void Boss3PartCC_State_WaitAfterKick(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    if (Boss3PartCC_CanCatchBall(s)) {
        s->func = Boss3PartCC_State_CatchBall;
        return;
    }
    if (s->unk25A-- == 0)
        s->func = Boss3PartCC_State_StartTurn;
}

/* Boss3PartCC_State_StartTurn (copied) */
static void Boss3PartCC_State_StartTurn(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 0xc);
    s->unk2C4.flags[0] &= ~4; /* unk3D0 */
    s->unk2C4.speed = 0x1000; /* unk3D8 */
    s->unk264 = 0x20;
    s->unk25A = 0;
    s->func = Boss3PartCC_State_Turn;
}

/* Boss3PartCC_State_Turn (copied) */
static void Boss3PartCC_State_Turn(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    if (s->unk25A != 0) {
        Boss3_UpdateCameraShake(&s->unk264, 1, 5);
    } else {
        BossAnmObj *c = s->unk2C4.anm[0]; /* unkE4 */
        if (c != NULL && (c->numFrames << 12) - c->frame < 0x5a)
            s->unk25A = 1;
    }
    if (!(s->unk2C4.flags[0] & 2))
        return;
    s->h.disp_flags ^= 1;
    s->func = Boss3PartCC_State_SelectWatchOrCatch;
    s->func(s);
}

/* Boss3PartCC_State_SelectWatchOrCatch (copied) */
static void Boss3PartCC_State_SelectWatchOrCatch(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    if (data_ov000_02363078[gBoss3->unk222] <= s->unk258) {
        s->func = Boss3PartCC_State_StartWaitCatchBall;
        s->func(s);
        return;
    }
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 1);
    s->unk2C4.flags[0] &= ~4;
    s->unk2C4.speed = 0x1000;
    s->func = Boss3PartCC_State_WatchBall;
    s->func(s);
}

/* Boss3PartCC_State_WatchBall (copied) */
static void Boss3PartCC_State_WatchBall(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Boss3_UpdateCameraShake(&s->unk264, 1, 5);
    if (Boss3PartCC_CanCatchBall(s)) {
        s->unk264 = 0;
        s->func = Boss3PartCC_State_CatchBall;
        return;
    }
    if (Boss3_ArenaDistance(s->h.x, gBoss3->unk218->h.x, (s->h.disp_flags & 1) ? 1 : 0) < 0xfa00)
        s->func = Boss3PartCC_State_JumpOverBall;
}

/* Boss3PartCC_State_JumpOverBall (copied) */
static void Boss3PartCC_State_JumpOverBall(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 6);
    s->unk2C4.flags[0] &= ~4;
    s->unk2C4.speed = 0x1000;
    gBoss3->unk218->unk20C &= ~0x40;
    BossEffect_CreateJump(1, s->unk2C4.trans[0] >> 4, -s->unk2C4.trans[1] >> 4, s->unk2C4.trans[2] >> 4);
    Sound_PlaySeqArc(0, 0, 0x8f);
    s->func = Boss3PartCC_State_JumpTakeoff;
    g_boss_stats.jumps++;
    boss_log("jump_over_ball", s->h.x >> 8, gBoss3->unk218->h.x >> 8);
}

/* Boss3PartCC_State_JumpTakeoff (copied) */
static void Boss3PartCC_State_JumpTakeoff(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    if (!(s->unk2C4.flags[0] & 2))
        return;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 7);
    s->unk2C4.flags[0] |= 4;
    s->unk2C4.speed = 0x1000;
    s->h.vel_y = -0xa00;
    s->func = Boss3PartCC_State_JumpRise;
}

/* Boss3PartCC_State_JumpRise (copied) */
static void Boss3PartCC_State_JumpRise(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    s->h.vel_y += 0x60;
    if (s->h.vel_y < 0)
        return;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 8);
    s->unk2C4.flags[0] &= ~4;
    s->unk2C4.speed = 0x1000;
    Boss3_ResetCamera();
    CAM.t[0].lerp = 0x199; /* c->unk30 */
    CAM.t[1].lerp = 0xcc;  /* c->unk6C */
    s->func = Boss3PartCC_State_JumpFallStart;
}

/* Boss3PartCC_State_JumpFallStart (copied) */
static void Boss3PartCC_State_JumpFallStart(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    s->h.vel_y += 0x60;
    if (s->h.y > 0x20000) {
        s->h.y = 0x20000;
        s->h.vel_y = 0;
    }
    if (!(s->unk2C4.flags[0] & 2))
        return;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 9);
    s->unk2C4.flags[0] |= 4;
    s->unk2C4.speed = 0x1000;
    s->func = Boss3PartCC_State_JumpFall;
}

/* Boss3PartCC_State_JumpFall (copied) */
static void Boss3PartCC_State_JumpFall(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    s->h.vel_y += 0x60;
    if (s->h.y > 0x20000) {
        s->h.y = 0x20000;
        s->h.vel_y = 0;
    }
    if (s->h.y < 0x20000)
        return;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 0xa);
    s->unk2C4.flags[0] &= ~4;
    s->unk2C4.speed = 0x1000;
    s->h.vel_y = 0;
    BossEffect_CreateLand(1, s->unk2C4.trans[0] >> 4, -s->unk2C4.trans[1] >> 4, s->unk2C4.trans[2] >> 4);
    Sound_PlaySeqArc(0, 0, 0x90);
    s->unk264 = 0x40;
    s->func = Boss3PartCC_State_JumpLandTurn;
}

/* Boss3PartCC_State_JumpLandTurn (copied) */
static void Boss3PartCC_State_JumpLandTurn(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Boss3_UpdateCameraShake(&s->unk264, 2, 6);
    if (!(s->unk2C4.flags[0] & 2))
        return;
    if (s->unk264 != 0)
        return;
    s->h.disp_flags ^= 1;
    s->func = Boss3PartCC_State_StartWaitCatchBall;
    s->func(s);
}

/* Boss3PartCC_State_StartWaitCatchBall (copied) */
static void Boss3PartCC_State_StartWaitCatchBall(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 1);
    s->unk2C4.flags[0] |= 4;
    s->unk2C4.speed = 0x1000;
    s->func = Boss3PartCC_State_WaitCatchBall;
    s->func(s);
}

/* Boss3PartCC_State_WaitCatchBall (copied) */
static void Boss3PartCC_State_WaitCatchBall(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Boss3_UpdateCameraShake(&s->unk264, 1, 5);
    if (Boss3PartCC_CanCatchBall(s)) {
        s->unk264 = 0;
        s->func = Boss3PartCC_State_CatchBall;
        return;
    }
}

/* Boss3PartCC_State_CatchBall (modified: sound handle via hooks) */
static void Boss3PartCC_State_CatchBall(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Boss3PartCD *r = gBoss3->unk218;
    BossEffect *q;
    MtxFx33 mtx;
    u16 ang;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 2);
    s->unk2C4.flags[0] &= ~4;
    s->unk2C4.speed = 0x1000;
    r->unk20C &= ~1;
    r->h.vel_x = 0;
    r->unk20C &= ~0x48;
    q = BossEffect_CreateCatch(1, r->unk290.trans[0] >> 4, -r->unk290.trans[1] >> 4, r->unk290.trans[2] >> 4);
    ang = (u16)((Rand() & 0xfff) + 0xaaa);
    MTX_RotX33(&q->unk134.rot, SIN(ang), COS(ang)); /* q->unk158 */
    ang = Rand();
    MTX_RotY33(&mtx, SIN(ang), COS(ang));
    MTX_Concat33(&q->unk134.rot, &mtx, &q->unk134.rot);
    gPlayers->pf &= ~0x200000;
    gBoss3->unk20C &= ~4;
    SndStopHandle(s->unk2C0);
    Sound_PlaySeqArc(0, 0, 0xcc);
    s->func = Boss3PartCC_State_CatchBallWait;
    g_boss_stats.catches++;
    boss_log("catch", r->h.x >> 8, 0);
}

/* Boss3PartCC_State_CatchBallWait (copied) */
static void Boss3PartCC_State_CatchBallWait(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    if (!(s->unk2C4.flags[0] & 2))
        return;
    if (CAM.unkDC != 2)
        Boss3_ResetCamera();
    s->func = Boss3PartCC_State_CatchBallDone;
}

/* ---------------------------------------------------------------- push */
/* Boss3PartCC_State_PushSelectDirection (copied) */
static void Boss3PartCC_State_PushSelectDirection(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    dcp_bool f = Boss3PartCC_IsOtherOnNegativeSide(&s->h, gPlayers) == 0;
    if (((s->h.disp_flags & 1) && f) || (!(s->h.disp_flags & 1) && !f)) {
        s->func = Boss3PartCC_State_StartPushBall;
        return;
    }
    s->func = Boss3PartCC_State_PushTurn;
}

/* Boss3PartCC_State_PushTurn (copied) */
static void Boss3PartCC_State_PushTurn(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Boss3 *b = gBoss3;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 0xb);
    s->unk2C4.flags[0] &= ~4;
    s->unk2C4.speed = 0x1000;
    s->h.vel_x = 0;
    b->unk20C |= 8;
    s->func = Boss3PartCC_State_PushTurnWait;
}

/* Boss3PartCC_State_PushTurnWait (copied) */
static void Boss3PartCC_State_PushTurnWait(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Boss3 *b = gBoss3;
    if (!(s->unk2C4.flags[0] & 2))
        return;
    s->h.disp_flags ^= 1;
    b->unk20C &= ~8;
    s->func = Boss3PartCC_State_StartPushBall;
    s->func(s);
}

/* Boss3PartCC_State_StartPushBall (copied) */
static void Boss3PartCC_State_StartPushBall(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    s->unk2C4.flags[0] &= ~0x40;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 5);
    s->unk2C4.flags[0] |= 4;
    s->unk2C4.speed = 0x1000;
    s->h.vel_x = 0;
    s->unk25E = 0;
    s->unk25C = 0;
    s->unk264 = 0;
    Sound_PlayVoice(0, 0, 0xd1);
    s->func = Boss3PartCC_State_PushBall;
    s->func(s);
}

/* Boss3PartCC_State_PushBall (copied) */
#define MAXSPD data_ov000_023631fc[GetIdx()][gBoss3->unk222]
#define MAXSPD2 data_ov000_023631fc[GetIdx2()][gBoss3->unk222]
static void Boss3PartCC_State_PushBall(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    if (s->h.disp_flags & 1) {
        if (s->h.vel_x < 0x100)
            s->h.vel_x = 0x100;
        else
            s->h.vel_x += 8;
    } else {
        if (s->h.vel_x > -0x100)
            s->h.vel_x = -0x100;
        else
            s->h.vel_x -= 8;
    }
    s->h.vel_x = (s->h.vel_x < -MAXSPD) ? -MAXSPD2 : ((s->h.vel_x > MAXSPD2) ? MAXSPD2 : s->h.vel_x);
    s->unk2C4.speed = FX_Mul((s->h.vel_x << 4) < 0 ? -(s->h.vel_x << 4) : (s->h.vel_x << 4), 0x666);
    gBoss3->unk218->unk210 -= s->h.vel_x;
    s->unk25C += (u16)(s->h.vel_x < 0 ? -s->h.vel_x : s->h.vel_x);
    if (s->unk25C > 0x6000) {
        s->unk25C -= 0x6000;
        Sound_PlaySeqArc(0, 0, 0x8e);
        s->unk264 = 0x20;
    }
    Boss3_UpdateCameraShake(&s->unk264, 1, 5);
    {
        u8 k = data_ov000_023633e4[GetIdx()][gBoss3->unk222][s->unk258 >> 5];
        if (s->unk25A != k) {
            s->unk25A = k;
            if (s->unk25A != 0)
                Boss3_MakeBallHarmful();
            else
                Boss3_MakeBallHittable();
        }
    }
    if (data_ov000_023631d8[GetIdx()][gBoss3->unk222] < s->unk258++)
        s->func = Boss3PartCC_State_PushEnd;
}
#undef MAXSPD
#undef MAXSPD2

/* Boss3PartCC_State_PushEnd (copied) */
static void Boss3PartCC_State_PushEnd(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    s->unk2C4.flags[0] |= 0x40;
    s->unk2C4.blendSpeeds[0] = 0x51; /* unk12E */
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 0);
    s->unk2C4.flags[0] |= 4;
    s->unk2C4.speed = 0x1000;
    s->h.vel_x = 0;
    s->func = Boss3PartCC_State_PushDone;
}

/* Boss3PartCC_State_PushDone (copied) */
static void Boss3PartCC_State_PushDone(void *p) { Boss3_UpdateCameraShake(&((Boss3PartCC *)p)->unk264, 1, 5); }

/* Boss3PartCC_State_StartStagger (copied) */
static void Boss3PartCC_State_StartStagger(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    s->unk2C4.flags[0] |= 1; /* unk3D0: animation paused */
    s->unk25A = 0;
    s->h.vel_x = 0;
    s->func = Boss3PartCC_State_Stagger;
}

/* Boss3PartCC_State_Stagger (copied) */
static void Boss3PartCC_State_Stagger(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Boss3 *b = gBoss3;
    u16 r;
    s32 m;
    s16 d;
    if (s->unk25A & 1) {
        r = Rand();
        m = FX_Mul(FX_Div((0x3c - s->unk25A) << 12, 0x3c000), 0x8000);
        s->unk20C = FX_Mul(m, COS(r));
        s->unk214 = FX_Mul(m, SIN(r));
    }
    if (s->unk25A++ <= 0x3c)
        return;
    d = (s16)(data_ov000_023631d8[GetIdx()][gBoss3->unk222] - s->unk258);
    s->unk2C4.flags[0] &= ~1;
    if (d > 0x78) {
        if (b->unk20C & 8)
            s->func = Boss3PartCC_State_PushTurnWait;
        else
            s->func = Boss3PartCC_State_PushSelectDirection;
    } else {
        s->func = Boss3PartCC_State_PushEnd;
    }
}

/* -------------------------------------------------------------- summon */
/* Boss3PartCC_State_SummonBall (copied; MapObject_Spawn(0xcd) = Boss3PartCD_Create) */
static void Boss3PartCC_State_SummonBall(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    ObjParam prm;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 3);
    s->unk2C4.flags[0] &= ~4;
    s->unk2C4.speed = 0x1000;
    memset(&prm, 0, sizeof(prm));
    prm.x0 = prm.y0 = 0xff;
    prm.type = 0xcd;
    gBoss3->unk218 = Boss3PartCD_Create(&prm, 0, 0);
    gBoss3->unk218->h.disp_flags &= ~0x20;
    gBoss3->unk218->unk212 = 0;
    gBoss3->unk218->unk20C |= 0x80;
    Boss3_MakeBallHarmful();
    BossEffect_CreatePop(1, s->unk228[9] >> 4, -s->unk228[10] >> 4, s->unk228[11] >> 4); /* unk24C/250/254 */
    Sound_PlaySeqArc(0, 0, 0x95);
    ScreenShake_Start(0xb);
    s->unk264 = 0x20;
    s->unk258 = 0;
    s->func = Boss3PartCC_State_SummonBallGrow;
    g_boss_stats.summons++;
    boss_log("summon", s->h.x >> 8, 0);
}

/* Boss3PartCC_State_SummonBallGrow (copied) */
static void Boss3PartCC_State_SummonBallGrow(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Boss3PartCD *r;
    if (s->unk258 != 0) {
        Boss3_UpdateCameraShake(&s->unk264, 1, 5);
    } else {
        BossAnmObj *c = s->unk2C4.anm[0]; /* unkE4 */
        if (c != NULL && (c->numFrames << 12) - c->frame < 0x3c)
            s->unk258 = 1;
    }
    gBoss3->unk218->unk212 += 0x40;
    if (gBoss3->unk218->unk212 > 0x1000)
        gBoss3->unk218->unk212 = 0x1000;
    if (!(s->unk2C4.flags[0] & 2))
        return;
    r = gBoss3->unk218;
    if (r->unk212 != 0x1000)
        return;
    if (s->unk264 == 0) {
        r->unk20C &= ~0x80;
        s->func = Boss3PartCC_State_SummonBallDone;
    }
}

/* -------------------------------------------------------------- throw attack */
/* Boss3PartCC_State_ThrowAttackJump (copied) */
static void Boss3PartCC_State_ThrowAttackJump(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Boss3PartCD *r = gBoss3->unk218;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 0xf);
    s->unk2C4.flags[0] &= ~4;
    s->unk2C4.speed = 0x1000;
    gBoss3->unk20C |= 2;
    Boss3_MakeBallHarmful();
    r->unk20C &= ~8;
    s->unk134.flags &= ~4;
    s->unk160.flags &= ~4;
    Boss3PartCC_MakeBallInert();
    BossArena_ArcToCirclePos(0xf42f8, 0x5fe435, s->h.x << 4, NULL, NULL, &s->unk258);
    if (s->h.disp_flags & 1)
        s->unk224 = s->unk258 - 0x4000;
    else
        s->unk224 = s->unk258 + 0x4000;
    s->unk258 += 0x8000;
    Boss3_ResetCamera();
    CAM.t[1].tgt.y = -0x1a0000;            /* c->unk58 */
    CAM.t[1].lerp = 0x400;                 /* c->unk6C */
    CAM.t[1].accel = 0x4cc;                /* c->unk74 */
    CAM.t[0].lerp = 0x80;                  /* c->unk30 */
    CAM.t[0].accel = 0xccc;                /* c->unk38 */
    CAM.unkC0 = CAM.unkC4 = 0x18a2f8;      /* c->unk78 / unk7C */
    CAM.unkC8 = 0x80;                      /* c->unk80 */
    CAM.unkCC = CAM.unkD0 = -0x50000;      /* c->unk84 / unk88 */
    CAM.unkD4 = 0x100;                     /* c->unk8C */
    BossEffect_CreateJump(1, s->unk2C4.trans[0] >> 4, -s->unk2C4.trans[1] >> 4, s->unk2C4.trans[2] >> 4);
    Sound_PlaySeqArc(0, 0, 0x91);
    s->func = Boss3PartCC_State_ThrowAttackTurn;
}

/* Boss3PartCC_State_ThrowAttackTurn (copied) */
static void Boss3PartCC_State_ThrowAttackTurn(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    s->unk224 = Boss3_StepAngleToward(s->unk224, s->unk258, 0x200);
    MTX_RotY33(&s->unk2C4.rot, SIN(s->unk224), COS(s->unk224));
    if (s->unk224 == s->unk258) {
        CAM.t[0].lerp = 0x200; /* data_ov000_0236a444.unk78 */
        s->func = Boss3PartCC_State_StartMoveToCentre;
    }
}

/* Boss3PartCC_State_StartMoveToCentre (modified: sound handle via hooks) */
static void Boss3PartCC_State_StartMoveToCentre(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 0x10);
    s->unk2C4.flags[0] |= 4;
    s->unk2C4.speed = 0x1000;
    s->unk218 = s->unk2C4.trans[0]; /* *(VecFx32 *)&s->unk218 = *(VecFx32 *)&s->unk2C4.unk48 */
    s->unk21C = s->unk2C4.trans[1];
    s->unk220 = s->unk2C4.trans[2];
    s->unk258 = 0;
    SndPlayHandle(s->unk2C0, 0x92);
    Sound_PlayVoice(0, 0, 0xd2);
    s->func = Boss3PartCC_State_MoveToCentre;
}

/* Boss3PartCC_State_MoveToCentre (copied) */
static void Boss3PartCC_State_MoveToCentre(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    BossModel *a = &s->unk2C4;
    s->unk258 += 0x7a;
    if ((s32)s->unk258 >= 0x1000) {
        s->unk258 = 0x1000;
        s->func = Boss3PartCC_State_StartThrows;
    }
    a->trans[0] = s->unk218 + FX_Mul(-s->unk218, (s16)s->unk258);
    a->trans[1] = s->unk21C + FX_Mul(-0x188000 - s->unk21C, (s16)s->unk258);
    a->trans[2] = s->unk220 + FX_Mul(-s->unk220, (s16)s->unk258);
}

/* Boss3PartCC_State_StartThrows (copied) */
static void Boss3PartCC_State_StartThrows(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    gBoss3->unk214->unk25E = 0;
    s->func = Boss3PartCC_State_StartThrowAim;
    s->func(s);
}

/* Boss3PartCC_State_StartThrowAim (copied) */
static void Boss3PartCC_State_StartThrowAim(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 0x12);
    s->unk2C4.flags[0] &= ~4;
    s->unk2C4.speed = 0x1000;
    s->unk258 = 0;
    s->func = Boss3PartCC_State_ThrowAim;
}

/* Boss3PartCC_State_ThrowAim (copied; gPlayers->unk17C / unk184 = player model translation) */
static void Boss3PartCC_State_ThrowAim(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    s->unk224 = Boss3_StepAngleToward(
        s->unk224, (u16)(Boss_Atan2(g_boss_game.player_trans[0], g_boss_game.player_trans[2]) + 0x8000), 0x400);
    MTX_RotY33(&s->unk2C4.rot, SIN(s->unk224), COS(s->unk224));
    if (data_ov000_02363220[gBoss3->unk222][gBoss3->unk214->unk25E] < s->unk258++)
        s->func = Boss3PartCC_State_ThrowWindUp;
}

/* Boss3PartCC_State_ThrowWindUp (copied) */
static void Boss3PartCC_State_ThrowWindUp(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 0x13);
    s->unk2C4.flags[0] &= ~4;
    s->unk2C4.speed = 0x1000;
    s->func = Boss3PartCC_State_ThrowWindUpTurn;
    s->func(s);
}

/* Boss3PartCC_State_ThrowWindUpTurn (copied) */
static void Boss3PartCC_State_ThrowWindUpTurn(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    s->unk224 = Boss3_StepAngleToward(
        s->unk224, (u16)(Boss_Atan2(g_boss_game.player_trans[0], g_boss_game.player_trans[2]) + 0x8000), 0x400);
    MTX_RotY33(&s->unk2C4.rot, SIN(s->unk224), COS(s->unk224));
    if (s->unk2C4.flags[0] & 2)
        s->func = Boss3PartCC_State_ThrowBall;
}

/* Boss3PartCC_ThrowBall (copied) */
static void Boss3PartCC_ThrowBall(u16 ang, s32 v) {
    Boss3PartCD *r = gBoss3->unk218;
    r->unk214.x = FX_Mul(v, SIN(ang));
    r->unk214.z = FX_Mul(v, COS(ang));
    gBoss3->unk218->unk222 = ang;
    Boss3PartCD_Action_Throw(gBoss3->unk218);
}

/* Boss3PartCC_State_ThrowBall (copied) */
static void Boss3PartCC_State_ThrowBall(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    s32 v;
    u16 ang;
    s32 m;
    DcPlayer *pl;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 0x14);
    s->unk2C4.flags[0] &= ~4;
    s->unk2C4.speed = 0x1000;
    v = data_ov000_0236327c[gBoss3->unk222][gBoss3->unk214->unk25E];
    gBoss3->unk218->unk20C |= 3;
    ang = (u16)Boss_Atan2(g_boss_game.player_trans[0], g_boss_game.player_trans[2]);
    switch (gBoss3->unk222) {
    case 3:
    case 4:
    case 5:
        pl = gPlayers;
        if (pl->move_flags & 0x8000)
            m = pl->vel_x;
        else
            m = pl->gnd_vel;
        {
            u16 k = (u16)(FX_Inv(data_ov000_0236327c[gBoss3->unk222][gBoss3->unk214->unk25E]) * 15 >> 12);
            ang += (u16)(k * (m * 4));
        }
        break;
    case 6:
    case 7:
    case 8:
        ang = (u16)(ang - 0x1fff + (Rand() & 0x3fff));
        break;
    }
    Boss3PartCC_ThrowBall(ang, v);
    gBoss3->unk218 = NULL;
    Sound_PlaySeqArc(0, 0, 0x97);
    s->func = Boss3PartCC_State_ThrowFollowThrough;
    g_boss_stats.balls_thrown++;
    boss_log("throw", ang, v);
    s->func(s);
}

/* Boss3PartCC_State_ThrowFollowThrough (copied) */
static void Boss3PartCC_State_ThrowFollowThrough(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    s->unk224 = Boss3_StepAngleToward(
        s->unk224, (u16)(Boss_Atan2(g_boss_game.player_trans[0], g_boss_game.player_trans[2]) + 0x8000), 0x400);
    MTX_RotY33(&s->unk2C4.rot, SIN(s->unk224), COS(s->unk224));
    if (s->unk2C4.flags[0] & 2)
        s->func = Boss3PartCC_State_SpawnBall;
}

/* Boss3PartCC_State_SpawnBall (copied; MapObject_Spawn(0xcd)) */
static void Boss3PartCC_State_SpawnBall(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    ObjParam prm;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 4);
    s->unk2C4.flags[0] &= ~4;
    s->unk2C4.speed = 0x1000;
    memset(&prm, 0, sizeof(prm));
    prm.x0 = prm.y0 = 0xff;
    prm.type = 0xcd;
    gBoss3->unk218 = Boss3PartCD_Create(&prm, 0, 0);
    gBoss3->unk218->h.disp_flags &= ~0x20;
    Boss3PartCC_MakeBallInert();
    BossEffect_CreatePop(1, s->unk228[9] >> 4, -s->unk228[10] >> 4, s->unk228[11] >> 4);
    s->func = Boss3PartCC_State_WaitBallSpawned;
}

/* Boss3PartCC_State_WaitBallSpawned (copied) */
static void Boss3PartCC_State_WaitBallSpawned(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    if (!(s->unk2C4.flags[0] & 2))
        return;
    gBoss3->unk214->unk25E++;
    if (data_ov000_023630a0[gBoss3->unk222] > gBoss3->unk214->unk25E)
        s->func = Boss3PartCC_State_StartThrowAim;
    else
        s->func = Boss3PartCC_State_EndThrows;
}

/* Boss3PartCC_State_EndThrows (copied) */
static void Boss3PartCC_State_EndThrows(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 0x10);
    s->unk2C4.flags[0] |= 4;
    s->unk2C4.speed = 0x1000;
    s->unk258 = 0;
    s->func = Boss3PartCC_State_WaitTimer_ov000_022d7754;
}

/* Boss3PartCC_State_WaitTimer_ov000_022d7754 (copied) */
static void Boss3PartCC_State_WaitTimer_ov000_022d7754(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    if (s->unk258++ != 0)
        s->func = Boss3PartCC_State_PickEdgeTarget;
}

/* Boss3PartCC_State_PickEdgeTarget (copied; FX_Atan2 = Boss_Atan2) */
static void Boss3PartCC_State_PickEdgeTarget(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    s->unk258 = (u16)Boss_Atan2(g_boss_game.player_trans[0], g_boss_game.player_trans[2]);
    s->unk258 += (Rand() & 0xf) * 0x2aa - 0x1555;
    s->func = Boss3PartCC_State_TurnToEdgeTarget;
}

/* Boss3PartCC_State_TurnToEdgeTarget (copied) */
static void Boss3PartCC_State_TurnToEdgeTarget(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    s32 d;
    s->unk224 = Boss3_StepAngleToward(s->unk224, s->unk258, 0x200);
    MTX_RotY33(&s->unk2C4.rot, SIN(s->unk224), COS(s->unk224));
    d = s->unk224 - s->unk258;
    if (d < 0)
        d = -d;
    if (d <= 0x40)
        s->func = Boss3PartCC_State_StartJumpToEdge;
}

/* Boss3PartCC_State_StartJumpToEdge (copied) */
static void Boss3PartCC_State_StartJumpToEdge(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    s->unk218 = FX_Mul(SIN(s->unk224), 0xf42f8);
    s->unk21C = -0x200000;
    s->unk220 = FX_Mul(COS(s->unk224), 0xf42f8);
    s->unk258 = 0;
    s->func = Boss3PartCC_State_JumpToEdge;
}

/* Boss3PartCC_State_JumpToEdge (copied) */
static void Boss3PartCC_State_JumpToEdge(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    BossModel *a = &s->unk2C4;
    s->unk258 += 0x7a;
    if ((s32)s->unk258 >= 0x1000) {
        s->unk258 = 0x1000;
        s->func = Boss3PartCC_State_LandAtEdge;
    }
    a->trans[0] = FX_Mul(s->unk218, (s16)s->unk258);
    a->trans[1] = FX_Mul(s->unk21C + 0x188000, (s16)s->unk258) - 0x188000;
    a->trans[2] = FX_Mul(s->unk220, (s16)s->unk258);
}

/* Boss3PartCC_State_LandAtEdge (modified: sound handle via hooks) */
static void Boss3PartCC_State_LandAtEdge(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    BossArena_CirclePosToArc(0xf42f8, 0x5fe435, s->unk218, s->unk220, &s->h.x);
    s->h.x >>= 4;
    s->unk134.flags |= 4;
    s->unk160.flags |= 4;
    Boss3_MakeBallHarmful();
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 0x11);
    s->unk2C4.flags[0] &= ~4;
    s->unk2C4.speed = 0x1000;
    BossEffect_CreateJump(1, s->unk2C4.trans[0] >> 4, -s->unk2C4.trans[1] >> 4, s->unk2C4.trans[2] >> 4);
    SndStopHandle(s->unk2C0);
    Sound_PlaySeqArc(0, 0, 0x90);
    s->unk264 = 0x40;
    s->func = Boss3PartCC_State_LandAtEdgeShake;
}

/* Boss3PartCC_State_LandAtEdgeShake (copied) */
static void Boss3PartCC_State_LandAtEdgeShake(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Boss3_UpdateCameraShake(&s->unk264, 2, 6);
    if (!(s->unk2C4.flags[0] & 2))
        return;
    if (s->unk264 == 0)
        s->func = Boss3PartCC_State_FacePlayerSide;
}

/* Boss3PartCC_State_FacePlayerSide (copied) */
static void Boss3PartCC_State_FacePlayerSide(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    Boss3_ResetCamera();
    CAM.t[1].accel = 0xccc; /* c->unk74 */
    CAM.t[0].lerp = 0x80;   /* c->unk30 */
    CAM.t[0].accel = 0x4cc; /* c->unk38 */
    if (!Boss3PartCC_IsOtherOnNegativeSide(&s->h, gPlayers)) {
        s->h.disp_flags |= 1;
        s->unk258 = s->unk224 - 0x4000;
    } else {
        s->h.disp_flags &= ~1;
        s->unk258 = s->unk224 + 0x4000;
    }
    s->func = Boss3PartCC_State_TurnToSide;
}

/* Boss3PartCC_State_TurnToSide (copied) */
static void Boss3PartCC_State_TurnToSide(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    s->unk224 = Boss3_StepAngleToward(s->unk224, s->unk258, 0x200);
    MTX_RotY33(&s->unk2C4.rot, SIN(s->unk224), COS(s->unk224));
    if (s->unk224 != s->unk258)
        return;
    gBoss3->unk20C &= ~2;
    CAM.t[0].lerp = 0x200; /* data_ov000_0236a444.unk78 */
    s->func = Boss3PartCC_State_ReturnDone;
}

/* Boss3PartCC_OnHit (modified: owner type check -> obj_box_player; NaviMsg / music via hooks) */
static void Boss3PartCC_OnHit(ObjBox *a, ObjBox *b) {
    Boss3PartCC *o = (Boss3PartCC *)a->owner;
    Boss3PartCD *p = (Boss3PartCD *)b->owner;
    BossEffect *q;
    MtxFx33 mtx;
    u16 ang;
    if ((void *)o == (void *)p) {
        rect_no_hit(a, b);
        return;
    }
    if (obj_box_player(b) != NULL) { /* p->unk0 == 1 */
        rect_no_hit(a, b);
        return;
    }
    if (!(p->unk20C & 0x40)) {
        rect_no_hit(a, b);
        return;
    }
    b->flags |= 0x1000;
    gBossStatus.unk8--;
    g_boss_stats.hits++;
    boss_log("hit", gBossStatus.unk8, p->h.x >> 8);
    if (gBossStatus.unk8 != 0) {
        if (p->h.vel_x > 0)
            o->h.disp_flags |= 1;
        else
            o->h.disp_flags &= ~1;
        Boss3_Action_HurtAll();
        q = BossEffect_CreateHit(1, p->unk290.trans[0] >> 4, -p->unk290.trans[1] >> 4, p->unk290.trans[2] >> 4);
        ang = (u16)((Rand() & 0xfff) + 0xaaa);
        MTX_RotX33(&q->unk134.rot, SIN(ang), COS(ang));
        ang = Rand();
        MTX_RotY33(&mtx, SIN(ang), COS(ang));
        MTX_Concat33(&q->unk134.rot, &mtx, &q->unk134.rot);
        if (gBossStatus.unk8 == 2) {
            NaviMsg_SetFaceAnim(0xb);
            NaviMsg_ShowMessage(0x12, 0xf0);
            if (g_boss_hooks.music) /* Sound_PlaySeq(0, 0xf): pinch */
                g_boss_hooks.music(g_boss_hooks.ctx, 0xf);
            gBoss3->unk20C |= 1;
        } else {
            NaviMsg_SetFaceAnim(0xb);
            NaviMsg_ShowMessage(0x10, 0xf0);
        }
    } else {
        Boss3_Action_DefeatAll();
    }
}

/* Boss3PartCC_Collide = MapObject_Collide_Default (both boxes have an owner) */
static void Boss3PartCC_Collide(void *p) {
    Boss3PartCC *o = (Boss3PartCC *)p;
    if (o->h.obj_flags & 0xc)
        return;
    if (o->unk134.owner != NULL)
        obj_handle_collider(&o->h, o, OBJ_OWNER_OBJECT, &o->unk134, 0);
    if (o->unk160.owner != NULL)
        obj_handle_collider(&o->h, o, OBJ_OWNER_OBJECT, &o->unk160, 0);
}

/* Boss3PartCC_Draw (+ Boss3PartCC_NodeCallback_BodyBall) (modified: Model_Draw -> boss_model_pose;
 * the "body_ball" node matrix (MTX_STORE 0x1e) -> boss_model_joint_world; NNS_G3dGetCurrentMtx after
 * MTX_RESTORE 0x1e -> unk228) */
static void Boss3PartCC_Draw(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    BossModel *a = &s->unk2C4;
    u16 ang;
    fx32 saved[3];
    if (!(gBoss3->unk20C & 2)) {
        if (s->h.x < 0x4000)
            s->h.x += 0x5fe43;
        else if (s->h.x > 0x63e43)
            s->h.x -= 0x5fe43;
        BossArena_ArcToCirclePos(0xf42f8, 0x5fe435, s->h.x << 4, &a->trans[0], &a->trans[2], &ang);
        a->trans[1] = -s->h.y << 4;
        {
            u16 r;
            if (s->h.disp_flags & 1)
                r = 0xc000;
            else
                r = 0x4000;
            r += ang;
            MTX_RotY33(&a->rot, SIN(r), COS(r));
        }
    }
    if (s->h.disp_flags & 0x20) {
        a->posed = 0;
        return;
    }
    saved[0] = a->trans[0];
    saved[1] = a->trans[1];
    saved[2] = a->trans[2];
    a->trans[0] += s->unk20C;
    a->trans[1] += s->unk210;
    a->trans[2] += s->unk214;
    Model_Animate(a);
    boss_model_pose(a, NULL);
    /* Boss3PartCC_NodeCallback_BodyBall: MTX_STORE(0x1e) at the "body_ball" node */
    boss_model_joint_world(a, s_joint_body_ball, &g_boss_mtx_slot[0x1e]);
    s->unk214 = 0;
    s->unk210 = s->unk214;
    s->unk20C = s->unk210;
    a->trans[0] = saved[0];
    a->trans[1] = saved[1];
    a->trans[2] = saved[2];
    mtx_to_fx43(&g_boss_mtx_slot[0x1e], s->unk228); /* MTX_MODE(2); MTX_RESTORE(0x1e); GetCurrentMtx */
}

/* Boss3PartCC_Destructor (modified) */
static void Boss3PartCC_Destructor(void *p) {
    Boss3PartCC *s = (Boss3PartCC *)p;
    boss_sfx_stop(s->unk2C0); /* Sound_FreeHandle */
    boss_model_release(&s->unk2C4);
    rect_forget(&s->unk134);
    rect_forget(&s->unk160);
    boss_obj_set_model(&s->h, NULL);
}

/* Boss3PartCC_Create (modified: static part; MapObject_Spawn(0xcc) from Boss3_Create) */
static Boss3PartCC *Boss3PartCC_Create(const ObjParam *a, s32 x, s32 y) {
    Boss3PartCC *s = &s_cc;
    Boss3 *b = gBoss3;
    ObjBox *h;
    BossModel *p;
    boss_model_release(&s->unk2C4);
    memset(s, 0, sizeof(*s)); /* MI_CpuFill16(s, 0, 0x438) */
    s->alive = 1;
    s->func = Boss3PartCC_State_Idle;
    s->unk2C0 = 1; /* Sound_AllocHandle() */
    s->unk434 = &s_bs3_anim; /* unk2C4.unk170 = archive file 8 (bs3.nsbca) */
    s->unk430 = &data_ov000_02369b18[1];
    MapObject_Init_parts(&s->h, &s->unk134, &s->unk160, a, x, y);
    s->h.obj_flags |= 0x10;
    s->h.move_flags |= 0x8300;
    s->h.disp_flags |= 0x20;
    boss_obj_set_model(&s->h, &s->unk2C4); /* unk124 = &unk2C4 */
    s->h.x = x;
    s->h.y = b->h.y;
    s->h.z = b->h.z;
    h = &s->unk134;
    rect_set_box2d(h, -0x18, -0x20, -0x20, 0);
    rect_set_atkdef(h, 0x13, 8);
    h->mask = 0x33;
    h->owner = s;
    h->owner_kind = OBJ_OWNER_OBJECT;
    h->cb = Boss3PartCC_OnHit;
    h->flags |= 0xc4;
    h = &s->unk160;
    rect_set_box2d(h, -0x18, -0x60, 0x20, 0);
    h->owner = s;
    h->owner_kind = OBJ_OWNER_OBJECT;
    h->flags |= 4;
    p = &s->unk2C4;
    Model_Init(p, 0);
    Model_SetResource(p, s->unk430);
    /* NNS_G3dRenderObjSetCallBack(p->unk90, Boss3PartCC_NodeCallback_BodyBall, 0, 6, 3): see Boss3PartCC_Draw */
    p->trans[0] = b->h.x << 4;
    p->trans[1] = -(b->h.y << 4);
    p->trans[2] = b->h.z << 4;
    p->scale[2] = 0x34cc;
    p->scale[1] = p->scale[2];
    p->scale[0] = p->scale[1];
    p->cb[0] = MOP_FLUSHVP; /* unkA = 4 */
    return s;
}

/* ============================================================ Boss3 (director) */
/* Boss3_Action_Idle / Hurt / WaitKickBall / WaitPushBall / WaitSummonBall / WaitThrowAttack (copied) */
static void Boss3_Action_Idle(Boss3 *s) {
    gBossStatus.unk4 = 0xd;
    s->unk224 = 0;
    s->func = Boss3_State_Idle;
}
static void Boss3_Action_Hurt(Boss3 *s) {
    gBossStatus.unk4 = 0x12;
    s->func = Boss3_State_WaitHurt;
}
static void Boss3_Action_WaitKickBall(Boss3 *s) {
    gBossStatus.unk4 = 0x10;
    s->func = Boss3_State_WaitKickBall;
}
static void Boss3_Action_WaitPushBall(Boss3 *s) {
    gBossStatus.unk4 = 0xe;
    s->func = Boss3_State_WaitPushBall;
}
static void Boss3_Action_WaitSummonBall(Boss3 *s) {
    gBossStatus.unk4 = 0xf;
    s->func = Boss3_State_WaitSummonBall;
}
static void Boss3_Action_WaitThrowAttack(Boss3 *s) {
    gBossStatus.unk4 = 0x11;
    s->func = Boss3_State_WaitThrowAttack;
}

/* Boss3_IdleAll (copied) */
static void Boss3_IdleAll(void) {
    Boss3_Action_Idle(gBoss3);
    Boss3PartCC_Action_Idle(gBoss3->unk214);
}

/* Boss3_Is*Done (copied) */
static dcp_bool Boss3_IsHurtDone(void) { return gBoss3->unk214->func == Boss3PartCC_State_HurtDone; }
static dcp_bool Boss3_IsKickBallDone(void) { return gBoss3->unk214->func == Boss3PartCC_State_CatchBallDone; }
static dcp_bool Boss3_IsPushBallDone(void) { return gBoss3->unk214->func == Boss3PartCC_State_PushDone; }
static dcp_bool Boss3_IsSummonBallDone(void) { return gBoss3->unk214->func == Boss3PartCC_State_SummonBallDone; }
static dcp_bool Boss3_IsThrowAttackDone(void) { return gBoss3->unk214->func == Boss3PartCC_State_ReturnDone; }

/* Boss3_StartKickBall / StartPushBall / StartSummonBall / StartThrowAttack (copied) */
static void Boss3_StartKickBall(s32 p0) {
    gBoss3->unk222 = (u16)p0;
    Boss3_Action_WaitKickBall(gBoss3);
    Boss3PartCC_Action_StartKickBall(gBoss3->unk214);
    g_boss_stats.attacks++;
    g_boss_stats.kicks++;
    boss_log("attack_kick", p0, gBoss3->unk214->h.x >> 8);
}
static void Boss3_StartPushBall(s32 p0) {
    gBoss3->unk222 = (u16)p0;
    Boss3_Action_WaitPushBall(gBoss3);
    Boss3PartCC_Action_StartPushBall(gBoss3->unk214);
    g_boss_stats.attacks++;
    g_boss_stats.pushes++;
    boss_log("attack_push", p0, gBoss3->unk214->h.x >> 8);
}
static void Boss3_StartSummonBall(void) {
    Boss3_Action_WaitSummonBall(gBoss3);
    Boss3PartCC_Action_StartSummonBall(gBoss3->unk214);
}
static void Boss3_StartThrowAttack(s32 p0) {
    gBoss3->unk222 = (u16)p0;
    Boss3_Action_WaitThrowAttack(gBoss3);
    Boss3PartCC_Action_StartThrowAttack(gBoss3->unk214);
    g_boss_stats.attacks++;
    g_boss_stats.throw_attacks++;
    boss_log("attack_throw", p0, gBoss3->unk214->h.x >> 8);
}

/* Boss3_SelectNextAttack (copied) */
static void Boss3_SelectNextAttack(void) {
    Boss3 *c = gBoss3;
    u16 r;
    if (gBossStatus.unk8 >= 6) {
        switch (data_ov000_02363178[c->unk21E][c->unk21C]) {
        case 0:
            Boss3_StartPushBall(data_ov000_02363098[Rand() & 7]);
            break;
        case 1:
            r = Rand();
            Boss3_StartKickBall(data_ov000_023630f0[GetIdx()][r & 3]);
            break;
        }
    } else if (gBossStatus.unk8 >= 3) {
        switch (data_ov000_02363198[c->unk21E][c->unk21C]) {
        case 0:
            Boss3_StartPushBall(data_ov000_02363090[Rand() & 7]);
            break;
        case 1:
            r = Rand();
            Boss3_StartKickBall(data_ov000_02363120[GetIdx()][r & 7]);
            break;
        case 2:
            r = Rand();
            Boss3_StartThrowAttack(data_ov000_023630d0[GetIdx()][r & 7]);
            break;
        }
    } else {
        switch (data_ov000_023631b8[c->unk21E][c->unk21C]) {
        case 0:
            Boss3_StartPushBall(data_ov000_02363088[Rand() & 7]);
            break;
        case 1:
            r = Rand();
            Boss3_StartKickBall(data_ov000_02363100[GetIdx()][r & 7]);
            break;
        case 2:
            r = Rand();
            Boss3_StartThrowAttack(data_ov000_02363110[GetIdx()][r & 7]);
            break;
        }
    }
    c->unk21C++;
    if (c->unk21C >= 8) {
        c->unk21C = 0;
        c->unk21E = Rand() & 3;
    }
}

/* Boss3_Action_HurtAll (copied) */
static void Boss3_Action_HurtAll(void) {
    Boss3PartCD *r = gBoss3->unk218;
    if (r != NULL) {
        r->h.obj_flags |= 8;
        gBoss3->unk218 = NULL;
    }
    Boss3_ResetCamera();
    CAM.t[0].lerp = 0; /* c->unk30 */
    CAM.t[1].lerp = 0; /* c->unk6C */
    CAM.unkC8 = 0;     /* c->unk80 */
    CAM.unkDA = 0;     /* c->unk92 */
    Boss3_Action_Hurt(gBoss3);
    Boss3PartCC_Action_Hurt(gBoss3->unk214);
}

/* Boss3_Action_Defeat (modified: RingManager / music / timer via hooks; a NULL ball is skipped) */
static void Boss3_Action_Defeat(Boss3 *s) {
    Boss3PartCC *a = s->unk214;
    Boss3PartCD *b = s->unk218;
    gBossStatus.unk4 = 0x13;
    a->unk134.flags &= ~4;
    a->unk160.flags &= ~4;
    if (b != NULL) {
        b->unk134.flags &= ~4;
        b->unk160.flags &= ~4;
    }
    s->h.obj_flags |= 0x20;
    a->h.obj_flags |= 0x20;
    if (b != NULL)
        b->h.obj_flags |= 0x20;
    s->unk23C.speed = 0; /* unk350 */
    a->unk2C4.speed = 0;
    if (b != NULL) {
        b->unk290.speed = 0; /* unk3A4 */
        b->unk404.speed = 0; /* unk518 */
    }
    StageObj_SetPause();
    g_dcp_world.flags &= ~1u; /* gGameState.unk18 &= ~1 */
    Model_SetAnimation(&a->unk2C4, 0, a->unk434, 1);
    a->unk2C4.flags[0] |= 4;
    a->h.x = 0;
    a->h.disp_flags |= 1;
    a->h.vel_y = 0;
    if (b != NULL) {
        b->h.x = -0x4600;
        b->h.vel_x = 0;
    }
    Player_Action_Blank(gPlayers);
    gPlayers->invuln = 0;            /* unk3B2 */
    gPlayers->disp_flags |= 0x20;
    gPlayers->pf |= 0x200000;
    gPlayers->released = 0;          /* unk424 */
    gPlayers->pressed = gPlayers->released;
    gPlayers->held = gPlayers->pressed;
    if (g_boss_hooks.ring_flags)     /* RingManager_SetFlags(RingManager_GetFlags() | 1) */
        g_boss_hooks.ring_flags(g_boss_hooks.ctx, 1);
    if (g_boss_hooks.music_stop)     /* NNS_SndPlayerStopSeqAll(0) */
        g_boss_hooks.music_stop(g_boss_hooks.ctx);
    s->func = Boss3_State_StartDefeat;
    boss_log("defeat", 0, 0);
}

/* Boss3_Action_DefeatAll (copied) */
static void Boss3_Action_DefeatAll(void) {
    Boss3PartCD *b = gBoss3->unk218;
    Boss3_Action_Defeat(gBoss3);
    Boss3PartCC_Action_Defeat(gBoss3->unk214);
    if (b != NULL)
        Boss3PartCD_Action_Defeat(b);
}

/* Boss3_State_WaitForStart (modified: sky Model on the camera; gPlayers->unk70 -> g_boss_game.draw_player) */
static void Boss3_Draw_PlayerOnArena(void);
static void Boss3_State_WaitForStart(void *p) {
    Boss3 *s = (Boss3 *)p;
    Boss3PartCC *a;
    Boss3PartCD *b;
    BossModel *m;
    if (!(gBossStatus.unk0 & 1))
        return;
    a = s->unk214;
    b = s->unk218;
    s->h.disp_flags &= ~0x20;
    a->h.disp_flags &= ~0x20;
    if (b != NULL)
        b->h.disp_flags &= ~0x20;
    m = &CAM.unkE0; /* data_ov000_0236a524 */
    g_boss_game.draw_player = Boss3_Draw_PlayerOnArena; /* gPlayers->unk70 */
    gPlayers->disp_flags |= 0x2040;
    CAM.unk0 |= 6;
    CAM.unk0 |= 1;
    Model_Init(m, 0);
    Model_SetResource(m, s->unk238);
    m->scale[2] = 0x1355b50;
    m->scale[1] = m->scale[2];
    m->scale[0] = m->scale[1];
    Boss3_ResetCamera();
    CAM.t[0].cur.x = (s32)0xffedc800; /* c->unk24 */
    CAM.t[0].cur.y = -0x1d0000;
    CAM.t[0].cur.z = -0x134000;
    CAM.t[0].tgt = CAM.t[0].cur;      /* c->unk18 */
    CAM.t[1].cur.x = 0;               /* c->unk60 */
    CAM.t[1].cur.y = -0x200000;
    CAM.t[1].cur.z = 0;
    CAM.t[1].tgt = CAM.t[1].cur;      /* c->unk54 */
    NaviMsg_SetFaceAnim(9);
    NaviMsg_ShowMessage(0xf, 0xf0);
    Boss3_IdleAll();
    boss_log("start", gBossStatus.unk8, 0);
}

/* Boss3_State_Idle (copied) */
static void Boss3_State_Idle(void *p) {
    Boss3 *s = (Boss3 *)p;
    if (s->unk224++ <= 0x1e)
        return;
    s->unk210 = 0;
    if (s->unk218 == NULL)
        Boss3_StartSummonBall();
    else
        Boss3_SelectNextAttack();
}

/* Boss3_State_Wait* (copied) */
static void Boss3_State_WaitHurt(void *p) {
    (void)p;
    if (Boss3_IsHurtDone() == 0)
        return;
    Boss3_IdleAll();
}
static void Boss3_State_WaitKickBall(void *p) {
    (void)p;
    if (Boss3_IsKickBallDone() == 0)
        return;
    Boss3_IdleAll();
}
static void Boss3_State_WaitPushBall(void *p) {
    (void)p;
    if (Boss3_IsPushBallDone() == 0)
        return;
    Boss3_IdleAll();
}
static void Boss3_State_WaitSummonBall(void *p) {
    (void)p;
    if (Boss3_IsSummonBallDone() == 0)
        return;
    Boss3_IdleAll();
}
static void Boss3_State_WaitThrowAttack(void *p) {
    (void)p;
    if (Boss3_IsThrowAttackDone() == 0)
        return;
    Boss3_IdleAll();
}

/* Boss3_State_StartDefeat (copied) */
static void Boss3_State_StartDefeat(void *p) { ((Boss3 *)p)->func = Boss3_State_DefeatCamera1; }

/* the defeat cut-in camera of Boss3_State_DefeatCamera1/2/3 (copied; c = camera + 0x48) */
static void Boss3_DefeatCameraCommon(Boss3PartCC *a, s32 dx, s32 dist, s32 height, u16 ang) {
    CAM.t[0].lerp = 0x1000;                                 /* c->unk30 */
    CAM.t[1].ox = CAM.t[1].oy = CAM.t[1].oz = NULL;         /* c->unk3C..44 */
    CAM.t[0].ox = CAM.t[0].oy = CAM.t[0].oz = NULL;         /* c->unk0..8 */
    CAM.t[1].offx = CAM.t[1].offy = CAM.t[1].offz = 0;      /* c->unk48..50 */
    CAM.t[1].tgt.x = a->unk2C4.trans[0] + dx;               /* c->unk54 */
    CAM.t[1].tgt.y = a->unk2C4.trans[1] + 0x20000;          /* c->unk58 */
    CAM.t[1].tgt.z = a->unk2C4.trans[2];                    /* c->unk5C */
    CAM.t[1].lerp = 0x1000;                                 /* c->unk6C */
    CAM.unkC0 = dist;                                       /* c->unk78 */
    CAM.unkC8 = 0x1000;                                     /* c->unk80 */
    CAM.unkCC = height;                                     /* c->unk84 */
    CAM.unkD4 = 0x1000;                                     /* c->unk8C */
    CAM.unkD6 = ang;                                        /* c->unk8E */
    CAM.unkDA = 0x1000;                                     /* c->unk92 */
}

/* Boss3_State_DefeatCamera1 (modified: BG2 cut-in (concentrated_line_01.bbg, archive file 6) ->
 * Boss_DrawExcBackground; gDisplayMain.unk48 -> g_boss_game.bright) */
static void Boss3_State_DefeatCamera1(void *p) {
    Boss3 *s = (Boss3 *)p;
    Boss3_ResetCamera();
    CAM.unkDC = 2;
    Boss3_DefeatCameraCommon(s->unk214, -0x20000, 0x50000, 0x20000, 0xf555);
    Boss_DrawExcBackground(6); /* BG2CNT, Background_Init(file 6), DISPCNT |= BG2 */
    g_boss_game.bright = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7c);
    s->unk224 = 0;
    s->func = Boss3_State_DefeatFade_ov000_022d98b0;
}

/* Boss3_State_DefeatCamera2 (modified: as DefeatCamera1, file 7) */
static void Boss3_State_DefeatCamera2(void *p) {
    Boss3 *s = (Boss3 *)p;
    Boss3_DefeatCameraCommon(s->unk214, -0x30000, 0x19000, 0x40000, 0x4000);
    Boss_DrawExcBackground(7);
    g_boss_game.bright = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7c);
    s->unk224 = 0;
    s->func = Boss3_State_DefeatFade_ov000_022d96d8;
}

/* Boss3_State_DefeatCamera3 (modified: as DefeatCamera1, file 6) */
static void Boss3_State_DefeatCamera3(void *p) {
    Boss3 *s = (Boss3 *)p;
    Boss3_DefeatCameraCommon(s->unk214, -0x20000, 0x78000, 0x20000, 0xf555);
    Boss_DrawExcBackground(6);
    g_boss_game.bright = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7c);
    s->unk224 = 0;
    s->func = Boss3_State_DefeatFade_ov000_022d94fc;
}

/* Boss3_State_DefeatFade_* (copied; gDisplayMain.unk48 -> g_boss_game.bright) */
static void Boss3_State_DefeatFade_ov000_022d94fc(void *p) {
    Boss3 *s = (Boss3 *)p;
    if (g_boss_game.bright > 0)
        g_boss_game.bright -= 2;
    if (s->unk224++ > 0x1e)
        s->func = Boss3_State_DefeatExplode;
}
static void Boss3_State_DefeatFade_ov000_022d96d8(void *p) {
    Boss3 *s = (Boss3 *)p;
    if (g_boss_game.bright > 0)
        g_boss_game.bright -= 2;
    if (s->unk224++ > 0x1e)
        s->func = Boss3_State_DefeatCamera3;
}
static void Boss3_State_DefeatFade_ov000_022d98b0(void *p) {
    Boss3 *s = (Boss3 *)p;
    if (g_boss_game.bright > 0)
        g_boss_game.bright -= 2;
    if (s->unk224++ > 0x1e)
        s->func = Boss3_State_DefeatCamera2;
}

/* Boss3_State_DefeatExplode (modified: DISPCNT BG2 off -> exc_bg = -1) */
static void Boss3_State_DefeatExplode(void *p) {
    Boss3 *s = (Boss3 *)p;
    Boss3PartCC *a = s->unk214;
    Boss3PartCD *b = s->unk218;
    BossEffect *q;
    Model_SetAnimation(&a->unk2C4, 0, a->unk434, 0x16);
    a->unk2C4.flags[0] &= ~4;
    a->unk2C4.speed = 0x1000;
    CAM.unkDC = 1;
    CAM.t[0].ox = CAM.t[0].oy = CAM.t[0].oz = NULL;   /* c->unk0..8 */
    CAM.t[0].tgt.x = 0;                               /* c->unk18 */
    CAM.t[0].tgt.y = -0x1b0000;                       /* c->unk1C */
    CAM.t[0].tgt.z = a->unk2C4.trans[2] + 0x10e000;   /* c->unk20 */
    CAM.t[0].lerp = 0x1000;                           /* c->unk30 */
    CAM.t[1].ox = CAM.t[1].oy = CAM.t[1].oz = NULL;   /* c->unk3C..44 */
    CAM.t[1].offx = CAM.t[1].offy = CAM.t[1].offz = 0; /* c->unk48..50 */
    CAM.t[1].tgt.x = a->unk2C4.trans[0];              /* c->unk54 */
    CAM.t[1].tgt.y = a->unk2C4.trans[1] + 0x50000;
    CAM.t[1].tgt.z = a->unk2C4.trans[2];
    CAM.t[1].lerp = 0x1000;                           /* c->unk6C */
    Boss_DrawExcBackground((u32)-1);                  /* GX_SetVisiblePlane(.. & ~4) */
    g_boss_game.exc_bg = -1;
    if (b != NULL)
        b->h.disp_flags |= 0x20;
    s->h.obj_flags &= ~0x20;
    a->h.obj_flags &= ~0x20;
    s->unk23C.speed = 0x1000; /* unk350 */
    a->unk2C4.speed = 0x1000;
    StageObj_ClearPause();
    q = BossEffect_CreateBlast3(0, a->unk2C4.trans[0] >> 4, -a->unk2C4.trans[1] >> 4, a->unk2C4.trans[2] >> 4);
    q->unk134.cb[0] = 0x13;   /* unk13E */
    q->unk134.cb[1] = 3;      /* unk13F */
    q->unk134.cb[2] = 0;      /* unk140 */
    q->unk134.speed = 0x800;  /* unk248 */
    NaviMsg_SetFaceAnim(0xd);
    NaviMsg_ShowMessage(0x13, 0xf0);
    g_boss_game.bright = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7d);
    s->unk224 = 0;
    s->func = Boss3_State_DefeatFinish;
}

/* Boss3_State_DefeatFinish (modified: StageShutdown_Create -> g_boss_hooks.cleared (once);
 * data_022c4590.unk10 -> g_obj_stage.ring_bonus) */
static void Boss3_State_DefeatFinish(void *p) {
    Boss3 *s = (Boss3 *)p;
    Boss3PartCC *a = s->unk214;
    Boss3PartCD *b = s->unk218;
    s->unk224++;
    if (!(s->unk224 & 3)) {
        if (g_boss_game.bright > 0)
            g_boss_game.bright--;
    }
    if (!(a->unk2C4.flags[0] & 2)) /* unk3D0 */
        return;
    if (s->unk224 <= 0x12c)
        return;
    a->h.obj_flags |= 4;
    if (b != NULL)
        b->h.obj_flags |= 4;
    g_obj_stage.ring_bonus = gPlayers->rings * 1000; /* unk3AE */
    if (!g_boss_game.cleared) {
        g_boss_game.cleared = 1;
        boss_log("cleared", gPlayers->rings, 0);
        if (g_boss_hooks.cleared)
            g_boss_hooks.cleared(g_boss_hooks.ctx);
    }
}

/* Boss3_Draw_PlayerOnArena (modified: the player model transform is kept for boss_player_model()) */
static void Boss3_Draw_PlayerOnArena(void) {
    DcPlayer *p = gPlayers;
    u16 ang;
    dcp_bool f;
    if (p->disp_flags & 0x20)
        return;
    if (p->x < 0x4000)
        p->x += 0x5fe43;
    else if (p->x > 0x63e43)
        p->x -= 0x5fe43;
    BossArena_ArcToCirclePos(0xf42f8, 0x5fe435, p->x << 4, &g_boss_game.player_trans[0], &g_boss_game.player_trans[2],
                             &ang);                       /* unk17C / unk184 */
    g_boss_game.player_trans[1] = -p->y << 4;            /* unk180 */
    {
        u16 r;
        if (p->disp_flags & 1)
            r = 0xc000;
        else
            r = 0x4000;
        r += ang;
        MTX_RotY33(&g_boss_game.player_rot, SIN(r), COS(r)); /* unk158 */
    }
    g_boss_game.player_trans_valid = 1;
    CAM.unkD6 = ang;
    if (!(gBoss3->unk20C & 4))
        return;
    if (gBoss3->unk218 != NULL) {
        f = DCP_FALSE;
        p->held = 0; /* unk420 */
        if (!(p->move_flags & 1))
            return;
        if (gBoss3->unk218->h.vel_x <= 0)
            f = DCP_TRUE;
        if (Boss3_ArenaDistance(gBoss3->unk214->h.x, p->x, f) < 0x1f400) {
            if (f)
                p->held |= 0x20;
            else
                p->held |= 0x10;
            return;
        }
        p->pf &= ~0x200000;
        gBoss3->unk20C &= ~4;
        return;
    }
    p->pf &= ~0x200000;
    gBoss3->unk20C &= ~4;
}

/* Boss3_Draw_Model (modified: Model_Draw -> boss_model_pose) */
static void Boss3_Draw_Model(void *p) {
    Boss3 *s = (Boss3 *)p;
    if (s->h.disp_flags & 0x20) {
        s->unk23C.posed = 0;
        return;
    }
    Model_Animate(&s->unk23C);
    boss_model_pose(&s->unk23C, NULL);
}

/* Boss3_Destructor (modified: effect resources released at boss_shutdown) */
static void Boss3_Destructor(void *p) {
    Boss3 *s = (Boss3 *)p;
    boss_model_release(&s->unk23C);
    g_boss_game.draw_player = NULL;
}

/* Boss3_Create (modified: resources from BossStage3 / bossdata.bin) */
void *Boss3_Create(const ObjParam *a, s32 x, s32 y) {
    Boss3 *s;
    BossModel *p;
    ObjParam prm;
    if (a->x0 == 0xff && a->y0 == 0xff)
        return NULL;
    s = &s_boss3;              /* Task_Create(StageObj_Main, Boss3_Destructor, 0, 0x500, 2, 0x374) */
    boss_model_release(&s->unk23C);
    memset(s, 0, sizeof(*s));  /* MI_CpuFill16(s, 0, 0x374) */
    s->alive = 1;
    gBoss3 = s;
    s->func = Boss3_State_WaitForStart;
    memset(&gBossStatus, 0, sizeof(gBossStatus));
    gBossStatus.unk8 = (u16)Boss_GetStartHealth();
    gBossStatus.unkA = (u16)Boss_GetStartHealth();
    s->unk230 = &s_bs3_anim; /* NNS_FndGetArchiveFileByIndex("bs3", 8) */
    s->unk234 = &s_bs3_srt;  /* file 9 (bs3.nsbta, timing only) */
    s->unk22C = &data_ov000_02369b18[0];
    s->unk238 = &data_ov000_02369b18[4];
    MapObject_Init_parts(&s->h, &s->unk134, &s->unk160, a, x, y);
    /* s->unk70 = Boss3_Draw_Model */
    s->h.obj_flags |= 0x12;
    s->h.move_flags |= 0xa300;
    s->h.disp_flags |= 0x20;
    s->unk21E = Rand() & 3;
    s->h.x = 0;
    s->h.y = 0x20000;
    s->h.z = 0;
    p = &s->unk23C;
    Model_Init(p, 0);
    Model_SetResource(p, s->unk22C);
    Model_SetAnimation(p, 3, s->unk234, 0);
    p->flags[3] |= 4; /* unk10F */
    p->trans[0] = s->h.x << 4;
    p->trans[1] = -(s->h.y << 4);
    p->trans[2] = s->h.z << 4;
    p->scale[2] = 0x34cc;
    p->scale[1] = p->scale[2];
    p->scale[0] = p->scale[1];
    memset(&prm, 0, sizeof(prm));
    prm.x0 = prm.y0 = 0xff;
    prm.type = 0xcc;
    s->unk214 = Boss3PartCC_Create(&prm, x, 0); /* MapObject_Spawn(0xcc, x, 0, ...) */
    Boss3_LoadPalettes(gGameState, 0);
    Boss3_LoadBallPalette(gGameState, 0);
    /* NNS_G3dResDefaultSetup("exc:eff_hit/catch/clash/pop/jump/land/shoot_bomb.nsbmd"): boss_effects_load */
    ScreenShake_Start(0xb);
    boss_log("create", gBossStatus.unk8, s->unk21E);
    return s;
}

/* ============================================================ module glue */
/* BossStage3_OnModelLoaded: data_02074408 = bs3_base, bs3_body, bs3_ball, bs3_thorn, bs3_sky
 * (NaviBg_Load / NaviB_Load after the last one: HUD, not here) */
int boss3_load(void) {
    static const char *const files[5] = {"mod__bs3_base__bs3_base.dcm", "mod__bs3_body__bs3_body.dcm",
                                         "mod__bs3_ball__bs3_ball.dcm", "mod__bs3_thorn__bs3_thorn.dcm",
                                         "mod__bs3_sky__bs3_sky.dcm"};
    int i, r = 0;
    for (i = 0; i < 5; i++)
        if (!data_ov000_02369b18[i].loaded)
            r |= boss_res_load_model(&data_ov000_02369b18[i], files[i]);
    if (!s_bs3_anim.loaded)
        r |= boss_res_load_anim(&s_bs3_anim, "narc__z33_act_lz7__bs3.dca");
    memset(&s_bs3_srt, 0, sizeof(s_bs3_srt));
    if (boss_anim_lengths("bs3.nsbta", &s_bs3_srt.nframes, &s_bs3_srt.count) != 0) {
        printf("boss: bs3.nsbta frame counts missing in bossdata.bin\n");
        r = -1;
    }
    s_joint_body_ball = boss_joint_index("bs3_body", data_ov000_023630e0); /* "body_ball" */
    if (s_joint_body_ball < 0)
        r = -1;
    return r ? -1 : 0;
}

/* BossStage3_FreeModels */
void boss3_unload(void) {
    int i;
    for (i = 0; i < 5; i++)
        boss_res_free_model(&data_ov000_02369b18[i]);
    boss_res_free_anim(&s_bs3_anim);
    memset(&s_bs3_srt, 0, sizeof(s_bs3_srt));
}

void boss3_destroy(void) {
    int i;
    for (i = 0; i < BOSS3_BALLS; i++) {
        if (s_cd[i].alive)
            Boss3PartCD_Destructor(&s_cd[i]);
        s_cd[i].alive = 0;
    }
    if (s_cc.alive)
        Boss3PartCC_Destructor(&s_cc);
    s_cc.alive = 0;
    if (s_boss3.alive)
        Boss3_Destructor(&s_boss3);
    s_boss3.alive = 0;
    gBoss3 = NULL;
}

static const BossObjOps s_ops_boss3 = {Boss3_Draw_Model, NULL, Boss3_Destructor};
static const BossObjOps s_ops_cc = {Boss3PartCC_Draw, Boss3PartCC_Collide, Boss3PartCC_Destructor};
static const BossObjOps s_ops_cd = {Boss3PartCD_Draw, Boss3PartCD_Collide, Boss3PartCD_Destructor};

void boss3_update(void) {
    u32 last = 0;
    if (s_boss3.alive)
        boss_obj_main(&s_boss3.h, &s_boss3.func, &s_ops_boss3, &s_boss3, &s_boss3.alive); /* 0x500 */
    if (s_cc.alive)
        boss_obj_main(&s_cc.h, &s_cc.func, &s_ops_cc, &s_cc, &s_cc.alive);                 /* 0x501 */
    /* 0x502: balls in creation order (also the ones created during this loop) */
    for (;;) {
        Boss3PartCD *next = NULL;
        int i;
        for (i = 0; i < BOSS3_BALLS; i++)
            if (s_cd[i].alive && s_cd[i].seq > last && (next == NULL || s_cd[i].seq < next->seq))
                next = &s_cd[i];
        if (next == NULL)
            break;
        last = next->seq;
        boss_obj_main(&next->h, &next->func, &s_ops_cd, next, &next->alive);
    }
}

int boss3_draw(int list) {
    int n = 0, i;
    if (!s_boss3.alive && !s_cc.alive)
        return 0;
    g_boss_debug_color = 0x4080ff;
    if (s_boss3.alive)
        n += boss_model_draw(&s_boss3.unk23C, list);
    g_boss_debug_color = 0x40ff40;
    if (s_cc.alive)
        n += boss_model_draw(&s_cc.unk2C4, list);
    g_boss_debug_color = 0xff8040;
    for (i = 0; i < BOSS3_BALLS; i++)
        if (s_cd[i].alive) {
            n += boss_model_draw(&s_cd[i].unk290, list);
            n += boss_model_draw(&s_cd[i].unk404, list);
        }
    g_boss_debug_color = 0xffff40;
    return n;
}

/* ------------------------------------------------------------ test / debug accessors */
void boss3_body_pos(s32 *x, s32 *y, s32 *z) {
    *x = s_cc.unk2C4.trans[0] >> 4;
    *y = -s_cc.unk2C4.trans[1] >> 4;
    *z = s_cc.unk2C4.trans[2] >> 4;
}
int boss3_box_armed(int box) {
    if (box == 0)
        return (s_cc.unk134.flags & 4) != 0;
    if (box == 1)
        return (s_cc.unk160.flags & 4) != 0;
    return 0;
}
s32 boss3_arc_x(void) { return s_cc.h.x; }
int boss3_ball(Boss3BallInfo *out) {
    const Boss3PartCD *b = gBoss3 != NULL ? gBoss3->unk218 : NULL;
    memset(out, 0, sizeof(*out));
    if (b == NULL || !b->alive)
        return 0;
    out->x = b->h.x;
    out->y = b->h.y;
    out->vel_x = b->h.vel_x;
    out->flags = b->unk20C;
    out->hittable = (b->unk134.flags & 4) != 0;
    out->harmful = (b->unk160.flags & 4) != 0;
    out->cycle_period = b->unk224;
    out->cycle_count = b->unk226;
    return 1;
}
int boss3_balls_alive(void) {
    int i, n = 0;
    for (i = 0; i < BOSS3_BALLS; i++)
        n += s_cd[i].alive;
    return n;
}
s32 boss3_palette(int model) { return model >= 0 && model < 5 ? s_pal[model] : -1; }
int boss3_ball_white(void) { return s_ball_matflag; }

/* state names for the test log */
#define N(f) {(BossFn)f, #f}
static const struct { BossFn f; const char *n; } s_names[] = {
    N(Boss3_State_WaitForStart), N(Boss3_State_Idle), N(Boss3_State_WaitHurt), N(Boss3_State_WaitKickBall),
    N(Boss3_State_WaitPushBall), N(Boss3_State_WaitSummonBall), N(Boss3_State_WaitThrowAttack),
    N(Boss3_State_StartDefeat), N(Boss3_State_DefeatCamera1), N(Boss3_State_DefeatCamera2),
    N(Boss3_State_DefeatCamera3), N(Boss3_State_DefeatFade_ov000_022d94fc), N(Boss3_State_DefeatFade_ov000_022d96d8),
    N(Boss3_State_DefeatFade_ov000_022d98b0), N(Boss3_State_DefeatExplode), N(Boss3_State_DefeatFinish),
    N(Boss3PartCC_State_Idle), N(Boss3PartCC_State_StartIdle), N(Boss3PartCC_State_Idle_ov000_022d8bb8),
    N(Boss3PartCC_State_Defeated), N(Boss3PartCC_State_StartHurt), N(Boss3PartCC_State_HurtKnockback),
    N(Boss3PartCC_State_HurtDone), N(Boss3PartCC_State_KickWindUp), N(Boss3PartCC_State_KickWindUpWait),
    N(Boss3PartCC_State_KickBall), N(Boss3PartCC_State_KickFollowThrough), N(Boss3PartCC_State_StartWaitAfterKick),
    N(Boss3PartCC_State_WaitAfterKick), N(Boss3PartCC_State_StartTurn), N(Boss3PartCC_State_Turn),
    N(Boss3PartCC_State_SelectWatchOrCatch), N(Boss3PartCC_State_WatchBall), N(Boss3PartCC_State_JumpOverBall),
    N(Boss3PartCC_State_JumpTakeoff), N(Boss3PartCC_State_JumpRise), N(Boss3PartCC_State_JumpFallStart),
    N(Boss3PartCC_State_JumpFall), N(Boss3PartCC_State_JumpLandTurn), N(Boss3PartCC_State_StartWaitCatchBall),
    N(Boss3PartCC_State_WaitCatchBall), N(Boss3PartCC_State_CatchBall), N(Boss3PartCC_State_CatchBallWait),
    N(Boss3PartCC_State_CatchBallDone), N(Boss3PartCC_State_PushSelectDirection), N(Boss3PartCC_State_PushTurn),
    N(Boss3PartCC_State_PushTurnWait), N(Boss3PartCC_State_StartPushBall), N(Boss3PartCC_State_PushBall),
    N(Boss3PartCC_State_PushEnd), N(Boss3PartCC_State_PushDone), N(Boss3PartCC_State_StartStagger),
    N(Boss3PartCC_State_Stagger), N(Boss3PartCC_State_SummonBall), N(Boss3PartCC_State_SummonBallGrow),
    N(Boss3PartCC_State_SummonBallDone), N(Boss3PartCC_State_ThrowAttackJump), N(Boss3PartCC_State_ThrowAttackTurn),
    N(Boss3PartCC_State_StartMoveToCentre), N(Boss3PartCC_State_MoveToCentre), N(Boss3PartCC_State_StartThrows),
    N(Boss3PartCC_State_StartThrowAim), N(Boss3PartCC_State_ThrowAim), N(Boss3PartCC_State_ThrowWindUp),
    N(Boss3PartCC_State_ThrowWindUpTurn), N(Boss3PartCC_State_ThrowBall), N(Boss3PartCC_State_ThrowFollowThrough),
    N(Boss3PartCC_State_SpawnBall), N(Boss3PartCC_State_WaitBallSpawned), N(Boss3PartCC_State_EndThrows),
    N(Boss3PartCC_State_WaitTimer_ov000_022d7754), N(Boss3PartCC_State_PickEdgeTarget),
    N(Boss3PartCC_State_TurnToEdgeTarget), N(Boss3PartCC_State_StartJumpToEdge), N(Boss3PartCC_State_JumpToEdge),
    N(Boss3PartCC_State_LandAtEdge), N(Boss3PartCC_State_LandAtEdgeShake), N(Boss3PartCC_State_FacePlayerSide),
    N(Boss3PartCC_State_TurnToSide), N(Boss3PartCC_State_ReturnDone),
    N(Boss3PartCD_State_Idle), N(Boss3PartCD_State_Defeated), N(Boss3PartCD_State_StartInert),
    N(Boss3PartCD_State_Inert), N(Boss3PartCD_State_StartHarmful), N(Boss3PartCD_State_Harmful),
    N(Boss3PartCD_State_StartHittable), N(Boss3PartCD_State_Hittable), N(Boss3PartCD_State_ThrowStart),
    N(Boss3PartCD_State_Fly), N(Boss3PartCD_State_Explode), N(Boss3PartCD_State_Exploding),
};
#undef N

static const char *name_of(BossFn f) {
    unsigned i;
    for (i = 0; i < sizeof(s_names) / sizeof(s_names[0]); i++)
        if (s_names[i].f == f)
            return strchr(s_names[i].n, '_') + 1;
    return "?";
}

const char *boss3_state_name(int part) {
    if (part == 0)
        return s_boss3.alive ? name_of(s_boss3.func) : "-";
    if (part == 1)
        return s_cc.alive ? name_of(s_cc.func) : "-";
    if (gBoss3 != NULL && gBoss3->unk218 != NULL)
        return name_of(gBoss3->unk218->func);
    return "-";
}

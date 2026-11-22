/*
 * Boss2 "Egg Turtle" (Water Palace act 3, stage z23): native translation of
 * src/game/Zone/Water_Palace/Boss/ (Boss2_*, Boss2Water_*, Boss2WaterLight_*,
 * BossStage2_*) and of the Water Palace 3D water effects the boss spawns
 * (src/game/Zone/Water_Palace/Effects/EffRipple3D_*, EffSplash3D_*).
 *
 * Every function keeps its original name and body; the per-file Unk structs
 * of the decompilation are merged into Boss2 / Boss2Water with the original
 * offsets in the field names (unk214[i] = the 11 Models at 0x214 + i*0x138:
 * unkA9C = body (7), unkBD4 = neck (8), unkD0C = head (9), unkE44 = prop (10)).
 * DS services are replaced:
 *   - Task_Create / StageObj_Main   -> static parts run by boss2_update: the
 *                                      water controller (task 0x4ff), Boss2
 *                                      through boss_obj_main (0x500) and the
 *                                      water-light task (0x500, created later)
 *   - Model_* / NNS_G3d*            -> boss_model.c (converted .dcm/.dca;
 *                                      bs2.nsbta = timing-only animations)
 *   - node callbacks (Boss2_StoreNodeMatrix: MTX_STORE of the "body0",
 *     "body_neck", "prop", "neck_head", "head_01" joints) + MTX_RESTORE +
 *     NNS_G3dGetCurrentMtx            -> g_boss_mtx_slot[] from
 *                                      boss_model_joint_world
 *   - ObjRect_*                      -> dreamcast/game/objects (obj_rect.c)
 *   - gCamera.unk62 (water level)    -> g_obj_camera[0].water
 *   - gCamera.unkE0 / unkE4          -> g_boss_game.arena_len / arena_circ
 *   - lights (NNS_G3dGlbLight*)      -> g_boss_lights (view space) for dcm_draw
 *   - material colours (NNS_G3dMdlSetMdl{Diff,Amb,Emi}All) -> boss_model_set_colors
 *   - palette swaps (Boss_LoadModelPalettes) -> state only (textures are pre-decoded)
 *   - Sound handles / NaviMsg / StageShutdown -> g_boss_hooks
 *   - EffWater3D_Create (3D BAC sprite)        -> g_boss_hooks.water_fx (not translated)
 *   - display registers (BG2 cut-in, master brightness) -> g_boss_game.exc_bg / bright
 * The player walks on a circle: gPlayers->unk70 = Boss2_Draw_PlayerOnArena
 * (g_boss_game.draw_player) wraps the player's arc position every frame and
 * builds the player model transform (boss_player_model()).
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "boss_internal.h"

/* ------------------------------------------------------------- parts */
typedef struct Boss2 Boss2;
typedef struct Boss2Water Boss2Water;
typedef void (*Boss2WaterFn)(Boss2Water *t);

struct Boss2Water {           /* 0x18 bytes (Boss2_CreateWaterController, task 0x4ff) */
    Boss2 *unk0;              /* @0x00 */
    Boss2WaterFn func;        /* @0x04 */
    s32 unk8;                 /* @0x08 water height offset (fx32): level = (unk8 >> 12) + 0x200 */
    s32 unkC;                 /* @0x0C timer */
    s16 unk10;                /* @0x10 spray cooldown */
    s16 unk12;                /* @0x12 frames above the shallow-water band */
    s16 unk14;                /* @0x14 player ripple counter */
    s16 unk16;                /* @0x16 boss ripple counter */
    int alive;
};

struct Boss2 {                /* 0x1478 bytes (Boss2_Create) */
    DcPlayer h;               /* StageObj header: x @0x10 (arc position, 24.8), y @0x14, z @0x18,
                                 shake @0x06/@0x07, obj_flags @0x54, move @0x58, disp @0x5C */
    BossFn func;              /* @0x68 */
    ObjBox unk134, unk160;    /* @0x134 / @0x160 MapObject_Init default boxes */
    u32 unk20C;               /* @0x20C bit0 hidden before start, bit1 off the arena circle (centre mode),
                                 bit2 neck follows the body rotation, bit3 boss above the water,
                                 bit4 player in shallow water, bit5 camera follows the player angle,
                                 bit6 catch effect spawned */
    Boss2Water *unk210;       /* @0x210 */
    BossModel unk214[11];     /* @0x214 base, lotus, water, shower, shower_s, shower_e, sky, body, neck, head, prop */
    const BossAnimRes *unkF7C; /* @0xF7C bs2.nsbca (archive file 8) */
    const BossAnimRes *unkF80; /* @0xF80 bs2.nsbta (archive file 9, timing only) */
    ObjBox unkF84[10];        /* @0xF84 hit boxes */
    ObjBox unk113C[10];       /* @0x113C the same boxes one arena length to the side (wrap) */
    s32 unk12F4;              /* @0x12F4 timer */
    s32 unk12F8;              /* @0x12F8 dash count (health 1..2 pattern) */
    s16 unk12FC;              /* @0x12FC pattern column */
    s16 unk12FE;              /* @0x12FE pattern row */
    u16 unk1300;              /* @0x1300 next attack (1 dash, 2 attack2, 3 attack3) */
    u16 unk1302;              /* @0x1302 last Boss2_SelectNextAttack mode */
    u16 unk1304;              /* @0x1304 facing angle */
    s32 unk1308, unk130C, unk1310, unk1314; /* @0x1308 prop spin accel / speed / max / min */
    u16 unk1318;              /* @0x1318 prop spin angle */
    u16 unk1320;              /* @0x1320 Boss2_NodeCallback_Body step */
    BossEffect *unk131C;      /* @0x131C suction effect */
    fx32 unk1324[12];         /* @0x1324 MtxFx43 "body0" (unk1348/134C/1350 = translation) */
    fx32 unk1378[12];         /* @0x1378 MtxFx43 "head_01" (unk139C/13A0/13A4) */
    fx32 unk13CC[12];         /* @0x13CC MtxFx43 "prop" (unk13F0/13F4/13F8) */
    BossModel *unk1420;       /* @0x1420 shower model being animated */
    const u16 *unk1424, *unk1428, *unk142C, *unk1430, *unk1434, *unk1438, *unk143C, *unk1440, *unk1444,
        *unk1448;             /* @0x1424.. palette index tables (gGameState.unk0) */
    s32 unk144C;              /* @0x144C flash counter */
    u32 unk1450, unk1454, unk1458; /* @0x1450 sound handles */
    union { s32 unk145C; u16 unk145C_h; };  /* @0x145C speed / angle speed / counter (u16 view in the
                                               attack2/attack3 files) */
    union { s32 unk1460; u16 unk1460_h; };  /* @0x1460 accel / defeat pose */
    union { s32 unk1464; u16 unk1464_h; };  /* @0x1464 max speed / distance */
    s32 unk1468;              /* @0x1468 */
    union { s16 unk146C; s32 unk146C_w; };  /* @0x146C (32-bit view in DashHop / DashBrake) */
    s32 unk1470;              /* @0x1470 */
    s32 unk1474;              /* @0x1474 */
    int alive;
};
#define unk1348 unk1324[9]
#define unk134C unk1324[10]
#define unk1350 unk1324[11]
#define unk139C unk1378[9]
#define unk13A0 unk1378[10]
#define unk13A4 unk1378[11]
#define unk13F0 unk13CC[9]
#define unk13F4 unk13CC[10]
#define unk13F8 unk13CC[11]
#define unkA9C unk214[7]
#define unkBD4 unk214[8]
#define unkD0C unk214[9]
#define unkE44 unk214[10]
#define unkBA8 unk214[7].flags[0]

static Boss2 s_boss2;
static Boss2 *gBoss2;
static Boss2Water s_water;
/* Boss2WaterLight task (gBoss2WaterLightTask) */
static void (*gBoss2WaterLightTask)(void);
/* head node callback installed (NNS_G3dRenderObjSetCallBack on unkD0C.unk90) */
static int s_head_cb;

/* BossStage2 resources: gBossStage2Models[11] (data_020747a0) and the zone act
 * archive's bs2.nsbca / bs2.nsbta */
static BossModelRes gBossStage2Models[11];
static BossAnimRes s_bs2_anim, s_bs2_srt;
static int s_joint[5];        /* body0, body_neck, prop, neck_head, head_01 */
static s32 s_pal[11];         /* native: Boss_LoadModelPalettes state (first palette index per model) */

/* lights (NNS_G3dGlbLightColor / NNS_G3dGlbLightVector) set this frame */
static u16 s_light_col[3];
static s16 s_light_vec[3][3];
static int s_light_set;
static dcm_lights_t s_lights;

/* ROM tables (bossdata.bin) */
#define data_ov000_02362dc4 BOSS_DATA(0x02362dc4, const u16 *)
#define data_ov000_02362dc8 BOSS_DATA(0x02362dc8, const u16 *)
#define data_ov000_02362dcc BOSS_DATA(0x02362dcc, const u16 *)
#define data_ov000_02362dd0 BOSS_DATA(0x02362dd0, const u16 *)
#define data_ov000_02362dd4 BOSS_DATA(0x02362dd4, const u16 *)
#define data_ov000_02362dd8 BOSS_DATA(0x02362dd8, const u16 *)
#define data_ov000_02362ddc BOSS_DATA(0x02362ddc, const u16 *)
#define data_ov000_02362de0 BOSS_DATA(0x02362de0, const u16 *)
#define data_ov000_02362de4 BOSS_DATA(0x02362de4, const u16 *)
#define data_ov000_02362de8 BOSS_DATA(0x02362de8, const u16 *)
#define data_ov000_02362dec BOSS_DATA(0x02362dec, const u16 *)
#define data_ov000_02362df4 BOSS_DATA(0x02362df4, const u16 *)
#define data_ov000_02362dfc BOSS_DATA(0x02362dfc, const u16 *)
#define data_ov000_02362e04 BOSS_DATA(0x02362e04, const u16 *)
#define data_ov000_02362e0c BOSS_DATA(0x02362e0c, const u16 *)
#define data_ov000_02362e14 BOSS_DATA(0x02362e14, const s32 *)
#define data_ov000_02362e1c BOSS_DATA(0x02362e1c, const u16(*)[3])
#define data_ov000_02362e28 BOSS_DATA(0x02362e28, const u16(*)[3])
#define data_ov000_02362e34 BOSS_DATA(0x02362e34, const u32 *)
#define data_ov000_02362e40 BOSS_DATA(0x02362e40, const s32 *)
#define data_ov000_02362e50 BOSS_DATA(0x02362e50, const s32 *)
#define data_ov000_02362e70 BOSS_DATA(0x02362e70, const u16(*)[4])
#define data_ov000_02362e90 BOSS_DATA(0x02362e90, const u16(*)[4])
#define data_ov000_02362ed0 BOSS_DATA(0x02362ed0, const s16(*)[3])
#define data_ov000_02362ee4 BOSS_DATA(0x02362ee4, const u16(*)[3])
#define data_ov000_02362ef8 BOSS_DATA(0x02362ef8, const s16(*)[3])
#define data_ov000_02362f0c BOSS_DATA(0x02362f0c, const s32(*)[3])
#define data_ov000_02362f30 BOSS_DATA(0x02362f30, const s32 *)
#define data_ov000_02362f58 BOSS_DATA(0x02362f58, const u16(*)[16])
#define data_ov000_02362f98 BOSS_DATA(0x02362f98, const u16(*)[16])
#define gBoss2ModelColorsDefault BOSS_DATA(0x02362fd8, const u16(*)[3])
#define data_ov000_0236301c BOSS_DATA(0x0236301c, const s16(*)[4])
#define data_ov000_023650fc BOSS_DATA(0x023650fc, const u32 *)
#define data_ov000_02365100 (*BOSS_DATA(0x02365100, const u32 *))
#define data_ov000_02365104 (*BOSS_DATA(0x02365104, const u32 *))
#define data_ov000_02365108 BOSS_DATA(0x02365108, const u32 *)
#define data_ov000_0236510c BOSS_DATA(0x0236510c, const u32 *)
#define data_ov000_02365110 BOSS_DATA(0x02365110, const u32 *)
#define data_ov000_0236511c BOSS_DATA(0x0236511c, const u32 *)
#define data_ov000_023651c8 BOSS_DATA(0x023651c8, const u32 *)
#define data_ov000_023651d8 BOSS_DATA(0x023651d8, const s32 *)
#define data_ov000_023651f8 BOSS_DATA(0x023651f8, const s32(*)[3])
#define data_ov000_023651fc BOSS_DATA(0x023651fc, const s32(*)[3])
#define data_ov000_02365200 BOSS_DATA(0x02365200, const s32(*)[3])
#define data_ov000_0236521c BOSS_DATA(0x0236521c, const u32 *)
#define gBoss2ModelColorsAlt BOSS_DATA(0x02365244, const u16(*)[3])
#define gEffSplash3DModelNames BOSS_DATA(0x02367b38, const u32 *)
#define gEffRipple3DModelNames BOSS_DATA(0x02367b40, const u32 *)
#define data_020748bc BOSS_DATA(0x020748bc, const s16(*)[3][3])
#define data_02074960 BOSS_DATA(0x02074960, const u16(*)[3][3])
/* a pointer stored in a ROM table -> the string it points to (also in bossdata.bin) */
#define ROM_STR(ptr) BOSS_DATA((ptr), const char *)

/* gCamera fields */
#define gCamera_unk62 (g_obj_camera[0].water)
#define gCamera_unkE0 (g_boss_game.arena_len)
#define gCamera_unkE4 (g_boss_game.arena_circ)
#define CAM data_ov000_0236a444

static inline s32 FX_DivS32(s32 a, s32 b) { return b != 0 ? a / b : 0; }
/* FX_Sqrt: fx32 square root */
static s32 FX_Sqrt(s32 x) {
    u64 v, r = 0, b = (u64)1 << 62;
    if (x <= 0)
        return 0;
    v = (u64)(u32)x << 12;
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
    return (s32)r;
}
static inline s32 Lerp(s32 a, s32 b) {
    return FX_DivS32(gBossStatus.unk8 * a + (gBossStatus.unkA - gBossStatus.unk8) * b, gBossStatus.unkA);
}
static inline s32 GetIdx(void) {
    dcp_bool b = DCP_TRUE;
    if (g_dcp_world.mode != 2 && g_dcp_world.mode != 3)
        b = DCP_FALSE;
    if (b)
        return 1;
    return g_boss_game.unk14;
}
#define SndHandle(h) (h)

/* forward declarations: states */
#define DECL(f) static void f(void *p);
DECL(Boss2_State_WaitForStart) DECL(Boss2_State_IdleCountdown) DECL(Boss2_State_IdleFinish)
DECL(Boss2_State_IdleAfterRaise) DECL(Boss2_State_IdleAfterRaiseWaitLoop) DECL(Boss2_State_IdleAfterRaiseCountdown)
DECL(Boss2_State_IdleAfterRaiseFinish) DECL(Boss2_State_IdleBeforeAttack3) DECL(Boss2_State_IdleBeforeAttack3WaitLoop)
DECL(Boss2_State_IdleBeforeAttack3Countdown) DECL(Boss2_State_IdleBeforeAttack3Finish)
DECL(Boss2_State_DashTurn) DECL(Boss2_State_DashStart) DECL(Boss2_State_Dash) DECL(Boss2_State_DashHop)
DECL(Boss2_State_DashBrake) DECL(Boss2_State_DashStop) DECL(Boss2_State_JumpRise) DECL(Boss2_State_JumpAirborne)
DECL(Boss2_State_JumpStartFall) DECL(Boss2_State_JumpFall) DECL(Boss2_State_JumpLandShake)
DECL(Boss2_State_WaitAnimThenSelectMode1) DECL(Boss2_State_WaitAnimThenSelectMode2)
DECL(Boss2_State_Attack2Turn) DECL(Boss2_State_Attack2EnterTurnMode) DECL(Boss2_State_Attack2StartAim)
DECL(Boss2_State_Attack2Aim) DECL(Boss2_State_Attack2AimEnd) DECL(Boss2_State_Attack2Charge)
DECL(Boss2_State_Attack2Strike) DECL(Boss2_State_Attack2Recover) DECL(Boss2_State_Attack2End)
DECL(Boss2_State_Attack2Hurt) DECL(Boss2_State_Attack3Start) DECL(Boss2_State_Attack3Suck)
DECL(Boss2_State_Attack3Hold) DECL(Boss2_State_Attack3Close) DECL(Boss2_State_Attack3Catch)
DECL(Boss2_State_Attack3Recover) DECL(Boss2_State_Attack3Hurt) DECL(Boss2_State_Attack3End)
DECL(Boss2_State_Defeat) DECL(Boss2_State_DefeatFinish)
#undef DECL
static void Boss2_SelectNextAttack(u16 mode);
static void Boss2_Action_Defeat(void);
static void Boss2Water_State_LowerShake(Boss2Water *t);
static void Boss2Water_State_LowerWait(Boss2Water *t);
static void Boss2Water_State_Lower(Boss2Water *t);
static void Boss2Water_State_LowerEndShake(Boss2Water *t);
static void Boss2Water_State_RaiseWaitAnim(Boss2Water *t);
static void Boss2Water_State_Raise(Boss2Water *t);
static void Boss2Water_State_RaiseEnd(Boss2Water *t);
static void Boss2WaterLight_Main_ApplyAltColors(void);
static void Boss2WaterLight_Main_RestoreColors(void);

/* ------------------------------------------------- native services */
/* Boss_LoadModelPalettes(model, names, indices, slots, n): loads the
 * boss02_0x_pl palettes into the model's texture palette slots.  The .dcm
 * textures are pre-decoded, so only the selection is kept (BOSSES.md). */
static void Boss_LoadModelPalettes(int model, const u32 *names, const u16 *idx, const u16 *slots, u16 n) {
    (void)names;
    (void)slots;
    (void)n;
    if (model >= 0 && model < 11 && idx != NULL)
        s_pal[model] = idx[0];
}

/* NNS_G3dGlbLightColor / NNS_G3dGlbLightVector for lights 0..2 */
static void Boss2_SetLights(int boss_lights) {
    s32 i;
    if (boss_lights) {
        for (i = 0; i <= 2; i++) {
            s_light_col[i] = data_ov000_02362df4[i];
            s_light_vec[i][0] = data_ov000_02362ef8[i][0];
            s_light_vec[i][1] = data_ov000_02362ef8[i][1];
            s_light_vec[i][2] = data_ov000_02362ef8[i][2];
        }
    } else {
        for (i = 0; i <= 2; i++) {
            s_light_col[i] = data_02074960[g_dcp_world.zone][g_dcp_world.act][i];
            s_light_vec[i][0] = data_020748bc[g_dcp_world.zone][i][0];
            s_light_vec[i][1] = data_020748bc[g_dcp_world.zone][i][1];
            s_light_vec[i][2] = data_020748bc[g_dcp_world.zone][i][2];
        }
    }
    s_light_set = 1;
}

/* NNS_G3dMdlSetMdlDiffAll / AmbAll / EmiAll for the 11 stage models */
static void Boss2_SetModelColors(const u16 (*tbl)[3]) {
    s32 i;
    for (i = 0; i < 11; i++)
        boss_model_set_colors(&gBossStage2Models[i], tbl[i][0], tbl[i][1], tbl[i][2]);
}

static void Player_State_Blank(DcPlayer *p) { (void)p; }
/* Player_Action_Blank (player module lacks it) */
static void Player_Action_Blank(DcPlayer *s) {
    dcp_player_init_state(s);
    dcp_reset_gimmick_state(s);
    s->state = Player_State_Blank;
}

static void set_anim3(Boss2 *s, u32 ix, u32 iy, u32 iz) {
    Model_SetAnimation(&s->unkA9C, 0, s->unkF7C, ix);
    Model_SetAnimation(&s->unkBD4, 0, s->unkF7C, iy);
    Model_SetAnimation(&s->unkD0C, 0, s->unkF7C, iz);
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

/* ============================================================ effects */
/* EffRipple3D_State_Main (copied) */
static void EffRipple3D_State_Main(void *p) {
    BossEffect *s = (BossEffect *)p;
    s->h.y = s->unk2B0 + (gCamera_unk62 << 8);
    BossEffect_State_Main(s);
}

/* EffRipple3D_Spawn (copied) */
static BossEffect *EffRipple3D_Spawn(DcPlayer *p, s32 dx, s32 b, s32 k, s32 c, s32 d, s32 e) {
    u16 ang;
    s32 ox;
    s32 oz;
    BossEffect *o = BossEffect_Create(ROM_STR(gEffRipple3DModelNames[k]), 3, 1, 0, 0, 0);
    o->h.x = p->x + (dx << 8);
    o->h.y = (c << 8) + b * 256;
    o->h.move_flags |= 0x2100;
    o->h.obj_flags |= 2;
    o->unk2B0 = b;
    o->state = EffRipple3D_State_Main;
    BossArena_ArcToCirclePos(d, e, o->h.x << 4, &ox, &oz, &ang);
    o->h.x = ox >> 4;
    o->h.z = oz >> 4;
    g_boss_stats.ripples++;
    return o;
}

/* EffRipple3D_SpawnForPlayer (copied) */
static void EffRipple3D_SpawnForPlayer(DcPlayer *s) {
    dcp_bool on;
    if (s->pid != 0)
        return;
    if (g_dcp_world.zone < 9 && g_dcp_world.act < 2)
        return;
    if (gCamera_unkE0 != 0 || gCamera_unkE4 != 0)
        on = DCP_TRUE;
    else
        on = DCP_FALSE;
    if (on)
        EffRipple3D_Spawn(s, 0, 1, 0, gCamera_unk62, gCamera_unkE0, gCamera_unkE4);
}

/* EffSplash3D_State_Main (copied) */
static void EffSplash3D_State_Main(void *p) {
    BossEffect *s = (BossEffect *)p;
    s->h.y = (gCamera_unk62 << 8) + (s->unk2B0 << 8);
    BossEffect_State_Main(s);
}

/* EffSplash3D_Spawn (copied) */
static BossEffect *EffSplash3D_Spawn(DcPlayer *p, s32 dx, s32 b, s32 c, u16 k, s32 d, s32 e) {
    u16 ang;
    s32 ox;
    s32 oz;
    BossEffect *o = BossEffect_Create(ROM_STR(gEffSplash3DModelNames[k]), 3, 1, 0, 0, 0);
    o->h.x = p->x + (dx << 8);
    o->h.y = (c << 8) + b * 256;
    o->h.move_flags |= 0x2100;
    o->h.obj_flags |= 2;
    o->unk2B0 = b;
    o->state = EffSplash3D_State_Main;
    BossArena_ArcToCirclePos(d, e, o->h.x << 4, &ox, &oz, &ang);
    o->h.x = ox >> 4;
    o->h.z = oz >> 4;
    g_boss_stats.splashes++;
    return o;
}

/* EffWater3D_Create: a 3D BAC sprite (ac_eff_water_3d.bac, StageObj_Load3dBAC)
 * placed on the arena circle.  Not translated: the position goes to the hook. */
static void EffWater3D_Create(DcPlayer *arg, s32 x, s32 y, s32 unused, s32 a, s32 b) {
    s32 o10, o14;
    u16 o0C;
    (void)unused;
    BossArena_ArcToCirclePos(a, b, (arg->x + (x << 8)) << 4, &o10, &o14, &o0C);
    boss_log("water_fx", o10 >> 12, o14 >> 12);
    if (g_boss_hooks.water_fx)
        g_boss_hooks.water_fx(g_boss_hooks.ctx, o10 >> 4, arg->y + (y << 8), o14 >> 4);
}

/* ======================================================= water light */
/* Boss2WaterLight_Destructor (copied) */
static void Boss2WaterLight_Destructor(void) { gBoss2WaterLightTask = NULL; }

/* Boss2WaterLight_Main_ApplyAltColors (copied) */
static void Boss2WaterLight_Main_ApplyAltColors(void) {
    if ((gCamera_unk62 << 12) < -CAM.t[0].cur.y) /* c->unk28 */
        return;
    Boss2_SetModelColors(gBoss2ModelColorsAlt);
    CAM.unkC8 = 0x200;  /* c->unk80 */
    CAM.unkD4 = 0x80;   /* c->unk8C */
    Boss2WaterLight_Destructor(); /* Task_Destroy(gTaskCurrent) */
}

/* Boss2WaterLight_Main_RestoreColors (copied) */
static void Boss2WaterLight_Main_RestoreColors(void) {
    if ((gCamera_unk62 << 12) > -CAM.t[0].cur.y)
        return;
    Boss2_SetModelColors(gBoss2ModelColorsDefault);
    CAM.unkC8 = 0x200;
    CAM.unkD4 = 0x80;
    Boss2WaterLight_Destructor();
}

/* Boss2WaterLight_CreateApplyAlt (copied; c = &data_ov000_0236a48c = camera + 0x48) */
static void Boss2WaterLight_CreateApplyAlt(void) {
    CAM.t[0].lerp = 0x800;    /* c->unk30 */
    CAM.unkC0 = 0x1fb2f0;     /* c->unk78 */
    CAM.unkC8 = 0x800;        /* c->unk80 */
    CAM.unkCC = 0x32000;      /* c->unk84 */
    CAM.unkD4 = 0x800;        /* c->unk8C */
    gBoss2WaterLightTask = Boss2WaterLight_Main_ApplyAltColors;
}

/* Boss2WaterLight_CreateRestore (copied) */
static void Boss2WaterLight_CreateRestore(void) {
    CAM.t[0].lerp = 0x800;
    CAM.unkC0 = 0x1dd2f0;
    CAM.unkC8 = 0x800;
    CAM.unkCC = 0;
    CAM.unkD4 = 0x800;
    gBoss2WaterLightTask = Boss2WaterLight_Main_RestoreColors;
}

/* ============================================================ water */
/* Boss2Water_Action_StartLower (copied) */
static void Boss2Water_Action_StartLower(Boss2 *p0) {
    Boss2Water *t0 = p0->unk210;
    t0->func = Boss2Water_State_LowerShake;
    ScreenShake_Start(0xc);
    t0->unkC = 0xf;
    boss_sfx_handle(p0->unk1454, 0x9f);
    g_boss_stats.water_lowers++;
    boss_log("water_lower", gCamera_unk62, 0);
}

/* Boss2Water_Action_StartRaise (copied) */
static void Boss2Water_Action_StartRaise(Boss2 *s) {
    BossModel *b;
    s->unk210->func = Boss2Water_State_RaiseWaitAnim;
    b = &s->unk214[4]; /* unk6F4 */
    Model_SetAnimation(b, 0, s->unkF7C, 1);
    b->flags[0] &= ~4;
    Model_SetAnimation(b, 3, s->unkF80, 1);
    b->flags[3] &= ~4;
    b->speed = 0x1000;
    s->unk1420 = b;
    boss_sfx_handle(s->unk1454, 0x9f);
    g_boss_stats.water_raises++;
    boss_log("water_raise", gCamera_unk62, 0);
}

/* Boss2Water_Destructor (copied: empty) */
static void Boss2Water_Destructor(void) {}

/* Boss2Water_State_LowerShake (copied, including the y = x >> 12 slip) */
static void Boss2Water_State_LowerShake(Boss2Water *t) {
    s32 k = t->unkC << 12;
    CAM.unk3C.x = FX_Div(k * ScreenShake_GetOffsetX(), 0xf000);
    CAM.unk3C.x = CAM.unk3C.x * 0x1800;
    CAM.unk3C.x = CAM.unk3C.x >> 12;
    CAM.unk3C.y = FX_Div(k * ScreenShake_GetOffsetY(), 0xf000);
    CAM.unk3C.y = CAM.unk3C.y * 0x1800;
    CAM.unk3C.y = CAM.unk3C.x >> 12;
    t->unkC--;
    if (t->unkC != 0)
        return;
    t->func = Boss2Water_State_LowerWait;
    ScreenShake_Start(0);
    t->unkC = 0xf;
}

/* Boss2Water_State_LowerWait (copied) */
static void Boss2Water_State_LowerWait(Boss2Water *p0) {
    p0->unkC = p0->unkC - 1;
    if (p0->unkC != 0)
        return;
    p0->func = Boss2Water_State_Lower;
    ScreenShake_Start(9);
    p0->unkC = 0x78;
}

/* Boss2Water_State_Lower (copied) */
static void Boss2Water_State_Lower(Boss2Water *t) {
    t->unk8 += 0x1b33;
    if (t->unk8 > 0x66000)
        t->unk8 = 0x66000;
    CAM.unk3C.x = ScreenShake_GetOffsetX() * 0xccc;
    CAM.unk3C.y = ScreenShake_GetOffsetY() * 0xccc;
    t->unkC--;
    if (t->unkC == 0) {
        t->func = Boss2Water_State_LowerEndShake;
        ScreenShake_Start(0xc);
        t->unkC = 0xf;
    } else if (t->unkC == 0x76) {
        Boss2WaterLight_CreateApplyAlt();
    }
}

/* Boss2Water_State_LowerEndShake (copied) */
static void Boss2Water_State_LowerEndShake(Boss2Water *t) {
    s32 k = t->unkC << 12;
    CAM.unk3C.x = FX_Div(k * ScreenShake_GetOffsetX(), 0xf000);
    CAM.unk3C.x = CAM.unk3C.x * 0x1800;
    CAM.unk3C.x = CAM.unk3C.x >> 12;
    CAM.unk3C.y = FX_Div(k * ScreenShake_GetOffsetY(), 0xf000);
    CAM.unk3C.y = CAM.unk3C.y * 0x1800;
    CAM.unk3C.y = CAM.unk3C.x >> 12;
    t->unkC--;
    if (t->unkC != 0)
        return;
    t->func = NULL;
    ScreenShake_Start(0);
    boss_sfx_stop(t->unk0->unk1454);
}

/* Boss2Water_State_RaiseWaitAnim (copied) */
static void Boss2Water_State_RaiseWaitAnim(Boss2Water *t) {
    Boss2 *s = t->unk0;
    BossModel *a = s->unk1420;
    BossModel *b;
    s32 k;
    if (!(a->flags[0] & 2))
        return;
    if (!(a->flags[3] & 2))
        return;
    t->func = Boss2Water_State_Raise;
    b = &s->unk214[3]; /* unk5BC */
    Model_SetAnimation(b, 3, s->unkF80, 0);
    b->flags[3] |= 4;
    b->speed = 0x1000;
    k = (t->unk8 >> 12) + 0xd3;
    b->scale[1] = FX_DivS32(k * 0x34cc, 0xd3);
    b->trans[1] = -(k + 0x12c) << 12;
    s->unk1420 = b;
    t->unkC = 0x78;
}

/* Boss2Water_State_Raise (copied) */
static void Boss2Water_State_Raise(Boss2Water *t) {
    BossModel *a = t->unk0->unk1420;
    s32 k;
    t->unk8 += -0x1b33;
    if (t->unk8 < -0x66000)
        t->unk8 = -0x66000;
    k = (t->unk8 >> 12) + 0xd3;
    a->scale[1] = FX_DivS32(k * 0x34cc, 0xd3);
    a->trans[1] = -(k + 0x12c) << 12;
    t->unkC--;
    if (t->unkC <= 0) {
        Boss2 *s = t->unk0;
        BossModel *b;
        t->func = Boss2Water_State_RaiseEnd;
        b = &s->unk214[5]; /* unk82C */
        b->scale[1] = 0x34cc;
        Model_SetAnimation(b, 0, s->unkF7C, 2);
        b->flags[0] &= ~4;
        Model_SetAnimation(b, 3, s->unkF80, 2);
        b->flags[3] &= ~4;
        b->speed = 0x1000;
        s->unk1420 = b;
    } else if (t->unkC == 0xf) {
        Boss2WaterLight_CreateRestore();
    }
}

/* Boss2Water_State_RaiseEnd (copied) */
static void Boss2Water_State_RaiseEnd(Boss2Water *p0) {
    Boss2 *t0 = p0->unk0;
    BossModel *t1 = t0->unk1420;
    if ((t1->flags[0] & 2) == 0)
        return;
    if ((t1->flags[3] & 2) == 0)
        return;
    p0->func = NULL;
    t0->unk1420 = NULL;
    boss_sfx_stop(t0->unk1454);
}

/* Boss2Water_Main (copied) */
static void Boss2Water_Main(void) {
    Boss2Water *t;
    Boss2 *s;
    DcPlayer *pl;
    u16 h;
    pl = gPlayers;
    t = &s_water;
    s = t->unk0;
    if (t->func != NULL)
        t->func(t);
    Boss2_SetLights((gCamera_unk62 << 12) > -CAM.t[0].cur.y); /* data_ov000_0236a444.unk70 */
    gCamera_unk62 = (u16)((t->unk8 >> 12) + 0x200);
    {
        s32 v = -(gCamera_unk62 << 12);
        s->unk214[2].trans[1] = v; /* unk4D0 */
        s->unk214[1].trans[1] = v; /* unk398 */
    }
    h = gCamera_unk62;
    if (h <= 0x200) {
        s32 k = (-s->unk134C >> 12) + 0x34;
        if (h < k && h > k / 128.7) {
            if (t->unk16 == 0x18) {
                BossEffect *o = EffRipple3D_Spawn(&s->h, 0, 1, 2, h, gCamera_unkE0, gCamera_unkE4);
                if (s->unk20C & 2) {
                    o->h.x = s->unk1348 >> 4;
                    o->h.z = s->unk1350 >> 4;
                }
                t->unk16 = 0;
            }
            t->unk16++;
        }
    }
    if (s->unk20C & 8) {
        if (gCamera_unk62 > (-s->unk134C >> 12) + 0x34) {
            s->unk20C &= ~8;
            EffSplash3D_Spawn(&s->h, 0, 0, gCamera_unk62, 1, gCamera_unkE0, gCamera_unkE4);
            Sound_PlaySeqArc(0, 0, 0xa1);
        }
    } else {
        if (gCamera_unk62 <= (-s->unk134C >> 12) + 0x34) {
            s->unk20C |= 8;
            EffSplash3D_Spawn(&s->h, 0, 0, gCamera_unk62, 1, gCamera_unkE0, gCamera_unkE4);
            Sound_PlaySeqArc(0, 0, 0xa1);
        }
    }
    if (s->unk20C & 0x10) {
        s32 vel;
        if (pl->move_flags & 0x8000)
            vel = pl->vel_x;
        else
            vel = pl->gnd_vel;
        if (pl->move_flags & 0x10) {
            t->unk10 = 0;
        } else {
            if (t->unk10 == 0 && vel != 0) {
                EffWater3D_Create(pl, 0, 0, gCamera_unk62, gCamera_unkE0, gCamera_unkE4);
                t->unk10 = 0xc;
                Sound_PlaySeqArc(0, 0, 0x51);
            }
            t->unk10--;
        }
        if (gCamera_unk62 > 0x200) {
            t->unk12--;
            if (t->unk12 <= 0)
                s->unk20C &= ~0x10;
        } else if (gCamera_unk62 < 502.1) {
            s->unk20C &= ~0x10;
        }
    } else {
        h = gCamera_unk62;
        if (h >= 502.1 && h <= 0x200) {
            s->unk20C |= 0x10;
            t->unk10 = 0;
            t->unk12 = 0x3c;
        }
    }
    h = gCamera_unk62;
    if (!(h >= 459.2))
        return;
    if (h > 0x200)
        return;
    if (pl->move_flags & 0x10)
        return;
    if ((t->unk14 & 7) == 0)
        EffRipple3D_SpawnForPlayer(pl);
    t->unk14++;
}

/* Boss2_CreateWaterController (copied) */
static void Boss2_CreateWaterController(Boss2 *s) {
    Boss2Water *p = &s_water; /* Task_Create(Boss2Water_Main, Boss2Water_Destructor, 0, 0x4ff, 2, 0x18) */
    memset(p, 0, sizeof(*p));
    p->alive = 1;
    s->unk210 = p;
    p->unk0 = s;
    p->unk8 = -0x66000;
}

/* ====================================================== Boss2 helpers */
/* Boss2_SetTargetAngle (copied): prop spin target */
static void Boss2_SetTargetAngle(Boss2 *s, s32 v) {
    if (s->unk130C < v) {
        s->unk1310 = v;
        s->unk1308 = 0xa3000;
    } else {
        s->unk1314 = v;
        s->unk1308 = -0xa3000;
    }
}

/* Boss2_SetPalettes (copied; Boss_LoadModelPalettes keeps the selection only) */
static void Boss2_SetPalettes(u32 flags) {
    if (flags & 3)
        return;
    if (flags & 4) {
        Boss_LoadModelPalettes(7, data_ov000_02365110, gBoss2->unk1424, data_ov000_02362e04, 3);
        Boss_LoadModelPalettes(9, data_ov000_023651c8, gBoss2->unk142C, data_ov000_02362e0c, 4);
        Boss_LoadModelPalettes(8, data_ov000_023650fc, gBoss2->unk1434, data_ov000_02362dc8, 1);
        Boss_LoadModelPalettes(10, data_ov000_02365108, gBoss2->unk143C, data_ov000_02362dc4, 1);
    } else {
        Boss_LoadModelPalettes(7, data_ov000_02365110, gBoss2->unk1428, data_ov000_02362e04, 3);
        Boss_LoadModelPalettes(9, data_ov000_023651c8, gBoss2->unk1430, data_ov000_02362e0c, 4);
        Boss_LoadModelPalettes(8, data_ov000_023650fc, gBoss2->unk1438, data_ov000_02362dc8, 1);
        Boss_LoadModelPalettes(10, data_ov000_02365108, gBoss2->unk1440, data_ov000_02362dc4, 1);
    }
}

/* Boss2_UpdateArenaSpeed (copied) */
static void Boss2_UpdateArenaSpeed(Boss2 *s, s32 mode) {
    if (mode == 0) {
        s->unk145C += s->unk1460;
        if (s->unk1464 < 0) {
            if (s->unk145C < s->unk1464)
                s->unk145C = s->unk1464;
        } else {
            if (s->unk145C > s->unk1464)
                s->unk145C = s->unk1464;
        }
    } else if (mode == 1) {
        s->unk145C -= s->unk1460;
        if (s->unk1464 < 0) {
            if (s->unk145C > s->unk1468)
                s->unk145C = s->unk1468;
        } else {
            if (s->unk145C < s->unk1468)
                s->unk145C = s->unk1468;
        }
    }
    s->h.x += s->unk145C;
}

/* Boss2_StoreNodeMatrix (modified): in the node callback of `model`, if the
 * current node is `name`, MTX_STORE its matrix into slot v */
static dcp_bool Boss2_StoreNodeMatrix(const BossModel *m, int node, u32 v, int joint) {
    if (node != joint)
        return DCP_FALSE;
    boss_model_joint_world(m, joint, &g_boss_mtx_slot[v & 31]);
    return DCP_TRUE;
}

/* Boss2_NodeCallback_Body (copied; called for every node of bs2_body in
 * NODEDESC order, the render state's callback cleared after "prop") */
static void Boss2_NodeCallback_Body(const BossModel *m) {
    int node, n = m->res != NULL ? m->res->mdl.hdr->num_joints : 0;
    for (node = 0; node < n; node++) {
        switch (gBoss2->unk1320) {
        case 0:
            if (Boss2_StoreNodeMatrix(m, node, 0x1e, s_joint[0])) /* data_ov000_0236511c[0] "body0" */
                gBoss2->unk1320++;
            break;
        case 1:
            if (Boss2_StoreNodeMatrix(m, node, 0x1d, s_joint[1])) /* "body_neck" */
                gBoss2->unk1320++;
            break;
        case 2:
            if (Boss2_StoreNodeMatrix(m, node, 0x1c, s_joint[2])) { /* "prop" */
                gBoss2->unk1320 = 0;
                return; /* s->unk24 = 0; s->unk92 = 0 */
            }
            break;
        }
    }
}

/* Boss2_NodeCallback_NeckHead (copied) */
static void Boss2_NodeCallback_NeckHead(const BossModel *m) {
    int node, n = m->res != NULL ? m->res->mdl.hdr->num_joints : 0;
    for (node = 0; node < n; node++)
        if (Boss2_StoreNodeMatrix(m, node, 0x1b, s_joint[3])) /* data_ov000_02365100 "neck_head" */
            return;
}

/* Boss2_NodeCallback_Head (copied) */
static void Boss2_NodeCallback_Head(const BossModel *m) {
    int node, n = m->res != NULL ? m->res->mdl.hdr->num_joints : 0;
    for (node = 0; node < n; node++)
        if (Boss2_StoreNodeMatrix(m, node, 0x1a, s_joint[4])) /* data_ov000_02365104 "head_01" */
            return;
}

/* ============================================================ hit boxes */
/* register a box and its copy one arena length to the side */
static void Boss2_RegisterBoxPair(Boss2 *s, ObjBox *h1, ObjBox *h2, s16 x, s16 xb, s16 y, s16 z) {
    h1->x = x;
    h2->x = xb;
    h1->y = h2->y = y;
    h1->z = h2->z = z;
    h1->flags &= ~3;
    h2->flags &= ~3;
    if (!(s->h.disp_flags & 1)) {
        h1->flags |= 1;
        h2->flags |= 1;
    }
    rect_register(h1);
    rect_register(h2);
}

/* Boss2_UpdateBodyHitboxes (copied) */
static void Boss2_UpdateBodyHitboxes(Boss2 *s) {
    s32 i;
    ObjBox *h1;
    ObjBox *h2;
    s16 x;
    s16 xb;
    s16 y;
    s16 z;
    x = (s16)(s->h.x >> 8);
    y = (s16)-(s->unk134C >> 12);
    z = (s16)(s->h.z >> 8);
    if (x < 0x366)
        xb = x + 0x6cd;
    else
        xb = x - 0x6cd;
    for (i = 0; i < 2; i++) {
        h1 = &s->unkF84[data_ov000_02362ddc[i]];
        h2 = &s->unk113C[data_ov000_02362ddc[i]];
        if (h1->flags & 4)
            Boss2_RegisterBoxPair(s, h1, h2, x, xb, y, z);
    }
    if (s->unk144C != 0) {
        Boss_LoadModelPalettes(9, data_ov000_0236510c, gBoss2->unk1444, data_ov000_02362dcc, 1);
        s->unk144C = 0;
    }
}

/* Boss2_UpdateAttack2Hitboxes (copied) */
static void Boss2_UpdateAttack2Hitboxes(Boss2 *s) {
    s32 d;
    s32 m;
    s32 i;
    ObjBox *h1;
    ObjBox *h2;
    s16 x;
    s16 xb;
    s16 y;
    s16 z;
    {
        s32 az = s->unk139C >> 12;
        s32 ax = s->unk13A4 >> 12;
        d = FX_Sqrt((az * az + ax * ax) << 12);
    }
    m = (s32)(((s64)((s->unk1304 << 12) >> 16) * 0x6cd98 + 0x800) >> 12);
    if (m < 0x4000)
        m += 0x6cd98;
    else if (m > 0x70d98)
        m -= 0x6cd98;
    x = (s16)(m >> 8);
    y = (s16)-(s->unk13A0 >> 12);
    z = (s16)((-(0x1152f0 - d) >> 12) + 0x23);
    if (x < 0x366)
        xb = x + 0x6cd;
    else
        xb = x - 0x6cd;
    for (i = 0; i < 3; i++) {
        h1 = &s->unkF84[data_ov000_02362dfc[i]];
        h2 = &s->unk113C[data_ov000_02362dfc[i]];
        if (h1->flags & 4)
            Boss2_RegisterBoxPair(s, h1, h2, x, xb, y, z);
    }
    if (s->unkF84[4].flags & 4) { /* unk1034 */
        s->unk144C++;
        if (s->unk144C == 3) {
            Boss_LoadModelPalettes(9, data_ov000_0236510c, gBoss2->unk1448, data_ov000_02362dcc, 1);
        } else if (s->unk144C == 7) {
            Boss_LoadModelPalettes(9, data_ov000_0236510c, gBoss2->unk1444, data_ov000_02362dcc, 1);
            s->unk144C = 0;
        }
    } else if (s->unk144C != 0) {
        Boss_LoadModelPalettes(9, data_ov000_0236510c, gBoss2->unk1444, data_ov000_02362dcc, 1);
        s->unk144C = 0;
    }
}

/* Boss2_UpdateAttack3Hitboxes (copied) */
static void Boss2_UpdateAttack3Hitboxes(Boss2 *s) {
    s32 out;
    s32 i;
    ObjBox *h1;
    ObjBox *h2;
    s16 x;
    s16 xb;
    s16 y;
    s16 z;
    BossArena_CirclePosToArc(0x1152f0, 0x6cd988, s->unk1348, s->unk1350, &out);
    {
        s16 x1;
        s16 xb1;
        s16 y1;
        x1 = (s16)(out >> 12);
        y1 = (s16)(s->h.y >> 8);
        if (x1 < 0x366)
            xb1 = x1 + 0x6cd;
        else
            xb1 = x1 - 0x6cd;
        h1 = &s->unkF84[8];
        h2 = &s->unk113C[8];
        if (h1->flags & 4)
            Boss2_RegisterBoxPair(s, h1, h2, x1, xb1, y1, 0);
    }
    BossArena_CirclePosToArc(0x1152f0, 0x6cd988, s->unk139C, s->unk13A4, &out);
    z = 0;
    x = (s16)(out >> 12);
    y = (s16)-(s->unk13A0 >> 12);
    if (x < 0x366)
        xb = x + 0x6cd;
    else
        xb = x - 0x6cd;
    for (i = 0; i < 3; i++) {
        h1 = &s->unkF84[data_ov000_02362dec[i]];
        h2 = &s->unk113C[data_ov000_02362dec[i]];
        if (h1->flags & 4)
            Boss2_RegisterBoxPair(s, h1, h2, x, xb, y, z);
    }
    if (s->unkF84[5].flags & 4) {
        s->unk144C++;
        if (s->unk144C == 3) {
            Boss_LoadModelPalettes(9, data_ov000_0236510c, gBoss2->unk1448, data_ov000_02362dcc, 1);
        } else if (s->unk144C == 7) {
            Boss_LoadModelPalettes(9, data_ov000_0236510c, gBoss2->unk1444, data_ov000_02362dcc, 1);
            s->unk144C = 0;
        }
    } else if (s->unk144C != 0) {
        Boss_LoadModelPalettes(9, data_ov000_0236510c, gBoss2->unk1444, data_ov000_02362dcc, 1);
        s->unk144C = 0;
    }
}

/* box arm / disarm (unkA |= 4, &= ~0x1000 / unkA &= ~4, |= 0x1000) */
static inline void box_on(ObjBox *b) {
    b->flags |= 4;
    b->flags &= ~0x1000;
}
static inline void box_off(ObjBox *b) {
    b->flags &= ~4;
    b->flags |= 0x1000;
}

/* ============================================================ draw */
/* Boss2_Draw_ModelsAndHitboxes (modified: Model_Draw -> boss_model_pose; the
 * matrix stack reads go through g_boss_mtx_slot) */
static void Boss2_Draw_ModelsAndHitboxes(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *a;
    ObjBox *h1;
    ObjBox *h2;
    s32 out;
    s16 x;
    s16 xb;
    s16 y;
    if (!(s->unk20C & 1)) {
        s32 i;
        for (i = 0; i <= 2; i++) {
            Model_Animate(&s->unk214[i]);
            boss_model_pose(&s->unk214[i], NULL);
        }
    }
    if (s->unk1420 != NULL) {
        Model_Animate(s->unk1420);
        boss_model_pose(s->unk1420, NULL);
    }
    if (s->h.disp_flags & 0x20)
        return;
    a = &s->unkA9C;
    if (!(s->unk20C & 2)) {
        u16 ang;
        if (s->h.x < 0x4000)
            s->h.x += 0x6cd98;
        else if (s->h.x > 0x70d98)
            s->h.x -= 0x6cd98;
        BossArena_ArcToCirclePos(0x1152f0, 0x6cd988, s->h.x << 4, &a->trans[0], &a->trans[2], &ang);
        a->trans[1] = -s->h.y << 4;
        {
            u16 r = (u16)(ang + 0xc000 + s->unk1304);
            MTX_RotY33(&a->rot, SIN(r), COS(r));
        }
    } else {
        a->trans[0] = s->h.x << 4;
        a->trans[1] = -s->h.y << 4;
        a->trans[2] = s->h.z << 4;
        MTX_RotY33(&a->rot, SIN(s->unk1304), COS(s->unk1304));
    }
    a->trans[0] += s->h.shake_x << 12;
    a->trans[1] += s->h.shake_y << 12;
    Model_Animate(a);
    boss_model_pose(a, NULL);
    Boss2_NodeCallback_Body(a);
    /* MTX_RESTORE 0x1e, NNS_G3dGetCurrentMtx(unk1324, unk1354) */
    mtx_to_fx43(&g_boss_mtx_slot[0x1e], s->unk1324);
    a = &s->unkBD4;
    if (s->unk20C & 4)
        a->rot = s->unkA9C.rot;
    Model_Animate(a);
    boss_model_pose(a, NULL);
    Boss2_NodeCallback_NeckHead(a);
    a = &s->unkD0C;
    Model_Animate(a);
    boss_model_pose(a, NULL);
    if (s_head_cb)
        Boss2_NodeCallback_Head(a);
    switch (gBossStatus.unk4) {
    case 9:
    case 10:
        mtx_to_fx43(&g_boss_mtx_slot[0x1a], s->unk1378); /* MTX_RESTORE 0x1a */
        break;
    }
    s->unk130C += s->unk1308;
    if (s->unk130C > s->unk1310)
        s->unk130C = s->unk1310;
    else if (s->unk130C < s->unk1314)
        s->unk130C = s->unk1314;
    s->unk1318 += (u16)(s->unk130C >> 12);
    a = &s->unkE44;
    MTX_RotZ33(&a->rot, SIN(s->unk1318), COS(s->unk1318));
    Model_Animate(a);
    boss_model_pose(a, NULL);
    mtx_to_fx43(&g_boss_mtx_slot[0x1c], s->unk13CC); /* MTX_RESTORE 0x1c */
    switch (gBossStatus.unk4) {
    case 7:
    case 8:
        Boss2_UpdateBodyHitboxes(s);
        break;
    case 9:
        Boss2_UpdateAttack2Hitboxes(s);
        break;
    case 10:
        Boss2_UpdateAttack3Hitboxes(s);
        break;
    }
    if ((s->unk20C & 2) || (s->unk1304 != 0 && s->unk1304 != 0x8000)) {
        if (gBossStatus.unk4 != 10)
            return;
    }
    BossArena_CirclePosToArc(0x1152f0, 0x6cd988, s->unk13F0, s->unk13F8, &out);
    x = (s16)(out >> 12);
    y = (s16)-(s->unk13F4 >> 12);
    if (x < 0x366)
        xb = x + 0x6cd;
    else
        xb = x - 0x6cd;
    h1 = &s->unkF84[2];
    h2 = &s->unk113C[2];
    if (!(h1->flags & 4))
        return;
    Boss2_RegisterBoxPair(s, h1, h2, x, xb, y, 0);
}

/* Boss2_Draw_PlayerOnArena (modified: the player model transform is kept
 * for boss_player_model(); the lights go to Boss2_SetLights) */
static void Boss2_Draw_PlayerOnArena(void) {
    DcPlayer *p = gPlayers;
    u16 ang;
    u16 off;
    if (p->disp_flags & 0x20)
        return;
    if (p->x < 0x4000)
        p->x += 0x6cd98;
    else if (p->x > 0x70d98)
        p->x -= 0x6cd98;
    BossArena_ArcToCirclePos(0x1152f0, 0x6cd988, p->x << 4, &g_boss_game.player_trans[0],
                             &g_boss_game.player_trans[2], &ang);  /* unk17C / unk184 */
    g_boss_game.player_trans[1] = -p->y << 4;                     /* unk180 */
    if (p->disp_flags & 1)
        off = 0xc000;
    else
        off = 0x4000;
    MTX_RotY33(&g_boss_game.player_rot, SIN((u16)(off + ang)), COS((u16)(off + ang))); /* unk158 */
    g_boss_game.player_trans_valid = 1;
    if (gBoss2->unk20C & 0x20)
        CAM.unkD6 = ang;
    Boss2_SetLights((gCamera_unk62 << 12) > -CAM.t[0].cur.y);
}

/* Boss2_DrawDefeatBackground (modified: the BBG goes to boss_exc_background) */
static void Boss2_DrawDefeatBackground(s32 a, s32 b) {
    Boss_DrawExcBackground(data_ov000_02362e34[b]); /* concentrated_line_0x.bbg into BG2 */
    CAM.unkC0 = CAM.unkC4 = data_ov000_02362f0c[a][b];  /* p->unk78 / unk7C */
    CAM.unkCC = CAM.unkD0 = data_ov000_02362ed0[a][b];  /* p->unk84 / unk88 */
    CAM.unkD6 = CAM.unkD8 = data_ov000_02362ee4[a][b];  /* p->unk8E / unk90 */
}

/* Boss2_InitCamera (copied; c = camera + 0x48) */
static void Boss2_InitCamera(void) {
    CAM.unkDC = 2;
    CAM.unkC = 0x1a66000;
    CAM.unk14 = FX_Div(0x1000, 0x34cc);
    CAM.t[0].ox = CAM.t[0].oy = CAM.t[0].oz = NULL;           /* c->unk0..8 */
    CAM.t[0].offx = CAM.t[0].offy = CAM.t[0].offz = 0;        /* c->unkC..14 */
    CAM.t[0].tgt.x = CAM.t[0].tgt.y = CAM.t[0].tgt.z = 0;     /* c->unk18..20 */
    CAM.t[0].lerp = 0x200;                                    /* c->unk30 */
    CAM.t[1].ox = NULL;                                       /* c->unk3C */
    CAM.t[1].oy = NULL;
    CAM.t[1].oz = NULL;
    CAM.t[1].tgt.x = 0;                                       /* c->unk54 */
    CAM.t[1].tgt.y = -0x1c0000;
    CAM.t[1].tgt.z = 0;
    CAM.t[1].offx = 0;                                        /* c->unk48 */
    CAM.t[1].offy = 0xa000;
    CAM.t[1].offz = 0;
    CAM.t[1].lerp = 0x400;                                    /* c->unk6C */
    CAM.unkC0 = CAM.unkC4 = 0x1dd2f0;                         /* c->unk78 / unk7C */
    CAM.unkC8 = 0x200;
    CAM.unkCC = CAM.unkD0 = 0;
    CAM.unkD4 = 0x80;
    CAM.unkD6 = CAM.unkD8 = 0;
    CAM.unkDA = 0x100;
    gPlayers->map_up = 0x40;     /* unk430 */
    gPlayers->map_down = 0x80;   /* unk432 */
    gPlayers->map_right = 0x10;  /* unk42C */
    gPlayers->map_left = 0x20;   /* unk42E */
}

/* ============================================================ actions */
/* Boss2_Action_Idle (copied) */
static void Boss2_Action_Idle(void) {
    Boss2 *s = gBoss2;
    BossModel *x = &s->unkA9C;
    BossModel *y = &s->unkBD4;
    BossModel *z = &s->unkD0C;
    s->unk12F4 = data_ov000_02362e40[Rand() & 3];
    if (s->unk1302 == 3) {
        s->func = Boss2_State_IdleAfterRaise;
        set_anim3(s, 3, 0x23, 0x3f);
        s->unk12F4 += 0x3c;
        Boss2_SetTargetAngle(s, 0x38e000);
    } else if (s->unk1300 == 3) {
        s->func = Boss2_State_IdleBeforeAttack3;
        set_anim3(s, 4, 0x24, 0x40);
        s->unk145C = 0x1e;
        Boss2_SetTargetAngle(s, 0);
    } else {
        s->func = Boss2_State_IdleCountdown;
        set_anim3(s, 4, 0x24, 0x40);
        Boss2_SetTargetAngle(s, 0x38e000);
    }
    x->flags[0] |= 4;
    y->flags[0] |= 4;
    z->flags[0] |= 4;
    x->speed = 0x1000;
    y->speed = 0x1000;
    z->speed = 0x1000;
    box_on(&s->unkF84[6]);
    box_on(&s->unkF84[2]);
}

/* Boss2_Action_DashAtPlayer (copied) */
static void Boss2_Action_DashAtPlayer(void) {
    Boss2 *s = gBoss2;
    s32 d = gPlayers->x;
    BossModel *x = &s->unkA9C;
    BossModel *y = &s->unkBD4;
    BossModel *z = &s->unkD0C;
    s32 a;
    s32 b;
    if (d < s->h.x) {
        a = s->h.x - d;
        b = d + 0x6cd98 - s->h.x;
    } else {
        a = s->h.x + 0x6cd98 - d;
        b = d - s->h.x;
    }
    if (s->unk1304 == 0) {
        if (b < a) {
            s->func = Boss2_State_DashTurn;
            s->unk146C = 0x27d;
            set_anim3(s, 8, 0x28, 0x44);
            s->unk1460 = Lerp(0xc, 0x14);
            s->unk1464 = Lerp(0x780, 0xa00);
        } else {
            s->func = Boss2_State_DashStart;
            s->unk146C = 0;
            set_anim3(s, 5, 0x25, 0x41);
            s->unk1460 = -Lerp(0xc, 0x14);
            s->unk1464 = -Lerp(0x780, 0xa00);
        }
    } else if (s->unk1304 != 0) {
        if (a < b) {
            s->func = Boss2_State_DashTurn;
            s->unk146C = -0x27d;
            set_anim3(s, 5, 0x25, 0x41);
            s->unk1460 = -Lerp(0xc, 0x14);
            s->unk1464 = -Lerp(0x780, 0xa00);
        } else {
            s->func = Boss2_State_DashStart;
            s->unk146C = 0;
            set_anim3(s, 8, 0x28, 0x44);
            s->unk1460 = Lerp(0xc, 0x14);
            s->unk1464 = Lerp(0x780, 0xa00);
        }
    }
    x->flags[0] &= ~4;
    y->flags[0] &= ~4;
    z->flags[0] &= ~4;
    x->speed = 0x1000;
    y->speed = 0x1000;
    z->speed = 0x1000;
    Boss2_SetTargetAngle(s, 0x1c71000);
    box_off(&s->unkF84[6]);
    s->unk12F4 = data_ov000_02362e50[Rand() & 3];
    s->unk1470 = 0;
    s->unk145C = 0;
    s->unk1468 = 0;
    s->unk1474 = s->unk1464;
}

/* Boss2_Action_Attack2 (copied) */
static void Boss2_Action_Attack2(void) {
    Boss2 *s = gBoss2;
    BossModel *x = &s->unkA9C;
    BossModel *y = &s->unkBD4;
    BossModel *z = &s->unkD0C;
    s32 r;
    s32 k;
    if (s->unk1304 == 0) {
        s->func = Boss2_State_Attack2Turn;
        s->unk146C = 0x27d;
    } else {
        s->func = Boss2_State_Attack2EnterTurnMode;
        box_off(&s->unkF84[6]);
        boss_sfx_handle(s->unk1450, 0xa0);
    }
    set_anim3(s, 8, 0x28, 0x44);
    x->flags[0] &= ~4;
    y->flags[0] &= ~4;
    z->flags[0] &= ~4;
    x->speed = 0x1000;
    y->speed = 0x1000;
    z->speed = 0x1000;
    s->unk145C = 0;
    s->unk1460 = 0x80;
    s->unk1464 = 0x200;
    Boss2_SetTargetAngle(s, 0x1555000);
    r = Rand() & 0xff;
    k = gBossStatus.unk8;
    s->unk1470 = 1;
    if ((k >= 6 && r < 0x40) || (k >= 3 && r < 0x80) || (k > 0 && r < 0xc0))
        s->unk1470++;
}

/* Boss2_Action_Attack3 (copied; unk145C is a u16 in this file) */
static void Boss2_Action_Attack3(void) {
    Boss2 *s = gBoss2;
    BossModel *x = &s->unkA9C;
    BossModel *y = &s->unkBD4;
    BossModel *z = &s->unkD0C;
    u16 ang;
    s_head_cb = 1; /* NNS_G3dRenderObjSetCallBack(z->unk90, Boss2_NodeCallback_Head, 0, 6, 3) */
    if (s->unk1304) {
        set_anim3(s, 0x17, 0x35, 0x53);
    } else {
        set_anim3(s, 0x11, 0x31, 0x4d);
    }
    x->flags[0] &= ~4;
    y->flags[0] &= ~4;
    z->flags[0] &= ~4;
    x->speed = 0x1000;
    y->speed = 0x1000;
    z->speed = 0x1000;
    s->func = Boss2_State_Attack3Start;
    s->unk145C_h = s->unk1304;
    s->unk1460 = s->h.x;
    BossArena_ArcToCirclePos(0x1152f0, 0x6cd988, s->h.x << 4, NULL, NULL, &ang);
    s->unk1304 = ang + 0x4000;
    s->unk20C |= 2;
    s->h.x = 0;
    s->unk20C &= ~0x40;
    box_on(&s->unkF84[8]);
    Boss2_SetTargetAngle(s, 0x1c71000);
}

/* Boss2_Action_Defeat (modified: display registers -> boss state) */
static void Boss2_Action_Defeat(void) {
    BossModel *x;
    BossModel *y;
    Boss2 *s = gBoss2;
    s32 i;
    BossModel *z;
    s->func = Boss2_State_Defeat;
    x = &s->unkA9C;
    y = &s->unkBD4;
    z = &s->unkD0C;
    if (g_boss_hooks.music_stop) /* NNS_SndPlayerStopSeqAll(0) */
        g_boss_hooks.music_stop(g_boss_hooks.ctx);
    StageObj_SetPause();
    s->h.obj_flags |= 0x20;
    g_dcp_world.flags &= ~1u; /* gGameState.unk18 &= ~1 */
    s->unk214[1].speed = s->unk214[2].speed = 0; /* unk460 = unk598 = 0 */
    s->unk1308 = 0;
    s->unk130C = 0;
    s->unk1310 = 0;
    s->unk1314 = 0;
    s->unk1318 = 0;
    if (gBossStatus.unk4 == 9) {
        set_anim3(s, 0xe, 0x2e, 0x4a);
        z->anm[0]->frame = 0;
        y->anm[0]->frame = z->anm[0]->frame;
        x->anm[0]->frame = y->anm[0]->frame;
        s->unk1460 = 0;
        s->unk1304 = 0;
    } else {
        if (s->unk145C_h) {
            set_anim3(s, 0x1a, 0x37, 0x56);
            s->unk1460 = 2;
            z->anm[0]->frame = 0x9a000;
            y->anm[0]->frame = z->anm[0]->frame;
            x->anm[0]->frame = y->anm[0]->frame;
        } else {
            set_anim3(s, 0x14, 0x33, 0x50);
            s->unk1460 = 1;
            z->anm[0]->frame = 0x9a000;
            y->anm[0]->frame = z->anm[0]->frame;
            x->anm[0]->frame = y->anm[0]->frame;
        }
        s->unk1304 = 0x4000;
    }
    s->h.x = 0;
    s->h.y = 0x20000;
    s->h.z = 0;
    x->speed = 0;
    y->speed = 0;
    z->speed = 0;
    gPlayers->x = data_ov000_023651f8[s->unk1460][0] << 8;
    gPlayers->y = data_ov000_023651fc[s->unk1460][0] << 8;
    gPlayers->z = data_ov000_02365200[s->unk1460][0] << 8;
    Player_Action_Blank(gPlayers);
    gPlayers->invuln = 0;              /* unk3B2 */
    gPlayers->disp_flags &= ~0x20;
    dcp_set_anim(gPlayers, 0x30);      /* Player_SetAnimation */
    for (i = 0; i < 10; i++)
        box_off(&s->unkF84[i]);
    gBossStatus.unk4 = 0xc;
    ScreenShake_Start(0);
    if (g_boss_hooks.ring_flags)       /* RingManager_SetFlags(RingManager_GetFlags() | 1) */
        g_boss_hooks.ring_flags(g_boss_hooks.ctx, 1);
    s->unk12F4 = 0x1e;
    s->unk145C = 0;
    g_boss_game.bright = 0x10;         /* gDisplayMain.unk48 */
    /* BG2CNT / BG2 offsets / gDisplayMain.unk8, unkA = 0: the cut-in background layer */
    Boss2_DrawDefeatBackground(s->unk1460, 0);
    /* DISPCNT |= BG2 (shown through boss_exc_background) */
    CAM.unkDC = 2;
    s->unk20C &= ~0x20;
    CAM.t[1].ox = CAM.t[1].oy = CAM.t[1].oz = NULL;       /* c->unk3C..44 */
    CAM.t[1].offx = CAM.t[1].offy = CAM.t[1].offz = 0;    /* c->unk48..50 */
    BossArena_ArcToCirclePos(0x1152f0, 0x6cd988, data_ov000_023651f8[s->unk1460][0] << 12, &CAM.t[1].cur.x,
                             &CAM.t[1].cur.z, NULL);
    CAM.t[1].tgt.x = CAM.t[1].cur.x;
    CAM.t[1].tgt.y = CAM.t[1].cur.y = -data_ov000_023651fc[s->unk1460][0] << 12;
    CAM.t[1].tgt.z = CAM.t[1].cur.z;
    CAM.t[1].lerp = 0x1000;   /* c->unk6C */
    CAM.unkC8 = 0x1000;       /* c->unk80 */
    CAM.unkD4 = 0x1000;       /* c->unk8C */
    CAM.unkDA = 0x1000;       /* c->unk92 */
    CAM.t[0].lerp = 0x1000;   /* c->unk30 */
    Sound_PlaySeqArc(0, 0, 0x7c);
    Boss_LoadModelPalettes(9, data_ov000_0236510c, gBoss2->unk1444, data_ov000_02362dcc, 1);
    boss_log("defeat", 0, 0);
}

/* Boss2_SelectNextAttack (copied) */
static void Boss2_SelectNextAttack(u16 mode) {
    Boss2 *s = gBoss2;
    s32 k = gBossStatus.unk8;
    s->unk1302 = mode;
    switch (mode) {
    case 0:
        switch (s->unk1300) {
        case 1:
            Boss2_Action_DashAtPlayer();
            gBossStatus.unk4 = 8;
            s->unk1300 = 0;
            g_boss_stats.attacks++;
            g_boss_stats.dashes++;
            boss_log("attack_dash", s->h.x >> 8, gPlayers->x >> 8);
            return;
        case 2:
            Boss2_Action_Attack2();
            gBossStatus.unk4 = 9;
            s->unk1300 = 0;
            g_boss_stats.attacks++;
            g_boss_stats.strikes++;
            boss_log("attack_strike", s->h.x >> 8, s->unk1470);
            return;
        case 3:
            Boss2_Action_Attack3();
            gBossStatus.unk4 = 10;
            s->unk1300 = 0;
            g_boss_stats.attacks++;
            g_boss_stats.catches++;
            boss_log("attack_catch", gCamera_unk62, 0);
            return;
        }
        return;
    case 1:
    case 2:
        if (k >= 6) {
            if (mode == 1) {
                s->unk1300 = 2;
            } else if ((Rand() & 0xf) < 2) {
                s->unk1300 = 2;
            } else {
                s->unk1300 = 1;
            }
        } else if (k >= 3) {
            s->unk1300 = data_ov000_02362f58[s->unk12FE][s->unk12FC];
            s->unk12FC++;
            if (s->unk12FC >= 16) {
                s->unk12FE = Rand() & 1;
                s->unk12FC = 0;
            }
            if (s->unk1300 == 3)
                Boss2Water_Action_StartLower(s);
        } else {
            s->unk1300 = data_ov000_02362f98[s->unk12FE][s->unk12FC];
            s->unk12FC++;
            if (s->unk12FC >= 16) {
                s->unk12FE = Rand() & 1;
                s->unk12FC = 0;
            }
            switch (s->unk1300) {
            case 1:
                if (++s->unk12F8 <= 2)
                    break;
                s->unk1300 = 3;
                /* fallthrough */
            case 3:
                Boss2Water_Action_StartLower(s);
                /* fallthrough */
            case 2:
                s->unk12F8 = 0;
                break;
            }
        }
        Boss2_Action_Idle();
        gBossStatus.unk4 = 7;
        break;
    case 3:
        Boss2Water_Action_StartRaise(s);
        if (k >= 6) {
            s->unk1300 = 1;
        } else if (k >= 3) {
            if ((Rand() & 0xff) < 0x20)
                s->unk1300 = 2;
            else
                s->unk1300 = 1;
        } else {
            if ((Rand() & 0xff) < 0x40)
                s->unk1300 = 2;
            else
                s->unk1300 = 1;
        }
        Boss2_Action_Idle();
        gBossStatus.unk4 = 7;
        break;
    }
}

/* ============================================================ hit callbacks */
/* Boss2_OnHit_DamageAttack2 (copied): box 4 (head, attack 2) hit by a player attack */
static void Boss2_OnHit_DamageAttack2(ObjBox *a, ObjBox *b) {
    Boss2 *s = (Boss2 *)a->owner;
    BossModel *x = &s->unkA9C;
    BossModel *y = &s->unkBD4;
    BossModel *z = &s->unkD0C;
    DcPlayer *p;
    if (gBossStatus.unk8 != 0) {
        gBossStatus.unk8--;
        g_boss_stats.hits++;
        boss_log("hit", gBossStatus.unk8, b->x);
        box_off(&s->unkF84[1]); /* unkFB0[0] */
        box_off(&s->unkF84[4]); /* unkFB0[3] */
        box_off(&s->unkF84[7]); /* unkFB0[6] */
        box_off(&s->unkF84[2]); /* unkFB0[1] */
        if (gBossStatus.unk8 != 0) {
            s->func = Boss2_State_Attack2Hurt;
            set_anim3(s, 0x10, 0x30, 0x4c);
            x->flags[0] &= ~4;
            y->flags[0] &= ~4;
            z->flags[0] &= ~4;
            x->speed = 0x1000;
            y->speed = 0x1000;
            z->speed = 0x1000;
            Boss2_SetPalettes(0);
            s->unk1474 = 0;
            gBossStatus.unk4 = 0xb;
            BossEffect_CreateHit(0, s->unk139C >> 4, (-s->unk13A0 >> 4) - 0x3200, s->unk13A4 >> 4);
            p = obj_box_player(b);
            if (p != NULL)
                obj_player_attack_bounce(p);
            if (gBossStatus.unk8 == 2) {
                NaviMsg_SetFaceAnim(0xb);
                NaviMsg_ShowMessage(0x12, 0xf0);
                if (g_boss_hooks.music) /* Sound_PlaySeq(0, 0xf) */
                    g_boss_hooks.music(g_boss_hooks.ctx, 0xf);
            } else {
                NaviMsg_SetFaceAnim(0xb);
                NaviMsg_ShowMessage(0x10, 0xf0);
            }
            Sound_PlaySeqArc(0, 0, 0x7b);
            Sound_PlayVoice(0, 0, data_ov000_02362e14[Rand() & 1]);
        } else {
            Boss2_Action_Defeat();
        }
    } else {
        Boss2_Action_Defeat();
    }
}

/* Boss2_OnHit_DamageAttack3 (copied): box 5 (head, attack 3) */
static void Boss2_OnHit_DamageAttack3(ObjBox *a, ObjBox *b) {
    Boss2 *s = (Boss2 *)a->owner;
    BossModel *x = &s->unkA9C;
    BossModel *y = &s->unkBD4;
    BossModel *z = &s->unkD0C;
    DcPlayer *p;
    s32 v;
    if (gBossStatus.unk8 != 0) {
        gBossStatus.unk8--;
        g_boss_stats.hits++;
        boss_log("hit", gBossStatus.unk8, b->x);
        box_off(&s->unkF84[3]); /* unk1008 */
        box_off(&s->unkF84[9]); /* unk1110 */
        box_off(&s->unkF84[5]); /* unk1060 */
        box_off(&s->unkF84[2]); /* unkFDC */
        if (gBossStatus.unk8 != 0) {
            v = y->anm[0]->frame;
            if (s->func == Boss2_State_Attack3Catch)
                v = 0x12d000 - v;
            s->func = Boss2_State_Attack3Hurt;
            if (s->unk145C_h) {
                set_anim3(s, 0x1c, 0x37, 0x58);
            } else {
                set_anim3(s, 0x16, 0x33, 0x52);
            }
            x->flags[0] &= ~4;
            y->flags[0] &= ~4;
            z->flags[0] &= ~4;
            x->speed = 0x1000;
            y->speed = 0x4000;
            z->speed = 0x1000;
            y->anm[0]->frame = v;
            Boss2_SetPalettes(0);
            s->unk1474 = 0;
            gBossStatus.unk4 = 0xb;
            Boss2_SetTargetAngle(s, 0);
            BossEffect_CreateHit(0, s->unk139C >> 4, (-s->unk13A0 >> 4) - 0x3200, s->unk13A4 >> 4);
            boss_sfx_handle(s->unk1458, 0xa4);
            p = obj_box_player(b);
            if (p != NULL)
                obj_player_attack_bounce(p);
            if (gBossStatus.unk8 == 2) {
                NaviMsg_SetFaceAnim(0xb);
                NaviMsg_ShowMessage(0x12, 0xf0);
                if (g_boss_hooks.music)
                    g_boss_hooks.music(g_boss_hooks.ctx, 0xf);
            } else {
                NaviMsg_SetFaceAnim(0xb);
                NaviMsg_ShowMessage(0x10, 0xf0);
            }
            Sound_PlaySeqArc(0, 0, 0x7b);
            Sound_PlayVoice(0, 0, data_ov000_02362e14[Rand() & 1]);
        } else {
            Boss2_Action_Defeat();
        }
    } else {
        Boss2_Action_Defeat();
    }
}

/* Boss2_OnHit_PushPlayer (copied): boxes 6..9 (solid body) */
static void Boss2_OnHit_PushPlayer(ObjBox *a, ObjBox *b) {
    DcPlayer *p = obj_box_player(b);
    Boss2 *o = (Boss2 *)a->owner;
    s32 vel;
    s32 px;
    if (p == NULL)
        return;
    if (p->move_flags & 0x8000)
        vel = p->vel_x;
    else
        vel = p->gnd_vel;
    px = p->x;
    if (a->x < b->x) {
        if (vel > 0)
            return;
        if (o->h.disp_flags & 1)
            p->push_x = (s16)(((a->x + a->r - b->l) << 8) - px);
        else
            p->push_x = (s16)(((a->x - a->l - b->l) << 8) - px);
        p->move_flags |= 4;
        p->vel_x = 0;
        p->gnd_vel = 0;
    } else {
        if (vel < 0)
            return;
        if (o->h.disp_flags & 1)
            p->push_x = (s16)(((a->x + a->l - b->r + 2) << 8) - px);
        else
            p->push_x = (s16)(((a->x - a->r - b->r + 2) << 8) - px);
        p->move_flags |= 4;
        p->vel_x = 0;
        p->gnd_vel = 0;
    }
}

/* data_ov000_0236521c: ROM function addresses -> native callbacks */
static void (*Boss2_HitCallback(u32 rom))(ObjBox *, ObjBox *) {
    switch (rom) {
    case 0x022cdbb0:
        return Boss2_OnHit_DamageAttack2;
    case 0x022cd880:
        return Boss2_OnHit_DamageAttack3;
    case 0x022cd738:
        return Boss2_OnHit_PushPlayer;
    }
    return NULL;
}

/* ============================================================ states */
/* Boss2_State_WaitForStart (modified: POWCNT / sky model / colours) */
static void Boss2_State_WaitForStart(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *m;
    if (!(gBossStatus.unk0 & 1))
        return;
    s->h.disp_flags &= ~0x20;
    m = &CAM.unkE0; /* data_ov000_0236a524 */
    s->unk20C &= ~1;
    /* *(vu16 *)0x4000304 |= 0x8000: POWCNT display swap (main screen on the top LCD) */
    CAM.unk0 |= 1;
    Model_Init(m, 0);
    Model_SetResource(m, &gBossStage2Models[6]);
    m->scale[2] = 0x1355b50;
    m->scale[1] = m->scale[2];
    m->scale[0] = m->scale[1];
    Boss2_InitCamera();
    s->unk20C |= 0x20;
    Boss2_SetModelColors(gBoss2ModelColorsDefault);
    Boss2_Action_Idle();
    g_boss_game.draw_player = Boss2_Draw_PlayerOnArena; /* gPlayers->unk70 */
    gPlayers->disp_flags |= 0x2040;
    NaviMsg_SetFaceAnim(9);
    NaviMsg_ShowMessage(0xf, 0xf0);
    Boss2_CreateWaterController(s);
    gCamera_unk62 = 0x19a;
    boss_log("start", gBossStatus.unk8, 0);
}

/* Boss2_State_IdleCountdown (copied) */
static void Boss2_State_IdleCountdown(void *p) {
    Boss2 *s = (Boss2 *)p;
    s->unk12F4--;
    if (s->unk12F4 != 0)
        return;
    s->func = Boss2_State_IdleFinish;
    s->unkA9C.flags[0] &= ~4;
    s->unkBD4.flags[0] &= ~4;
    s->unkD0C.flags[0] &= ~4;
}

/* Boss2_State_IdleFinish (copied) */
static void Boss2_State_IdleFinish(void *p) {
    Boss2 *s = (Boss2 *)p;
    if (!(s->unkBA8 & 2))
        return;
    Boss2_SelectNextAttack(0);
}

/* the cross-fade into the idle loop (IdleAfterRaise / IdleBeforeAttack3) */
static void Boss2_StartIdleFade(Boss2 *s, u32 ix, u32 iy, u32 iz) {
    BossModel *x = &s->unkA9C, *y = &s->unkBD4, *z = &s->unkD0C;
    x->flags[0] |= 0x40;
    y->flags[0] |= 0x40;
    z->flags[0] |= 0x40;
    set_anim3(s, ix, iy, iz);
    z->blendSpeed = 0x8c;
    y->blendSpeed = z->blendSpeed;
    x->blendSpeed = y->blendSpeed;
    if (x->anm2[0] != NULL) { /* *x->unkE4 = *x->unkF8 */
        x->anm[0]->frame = x->anm2[0]->frame;
        y->anm[0]->frame = x->anm2[0]->frame;
        z->anm[0]->frame = x->anm2[0]->frame;
    }
}

/* Boss2_State_IdleAfterRaise (copied) */
static void Boss2_State_IdleAfterRaise(void *p) {
    Boss2 *s = (Boss2 *)p;
    if (!(gCamera_unk62 <= 459.5))
        return;
    s->func = Boss2_State_IdleAfterRaiseWaitLoop;
    Boss2_StartIdleFade(s, 4, 0x24, 0x40);
}

/* Boss2_State_IdleAfterRaiseWaitLoop (copied) */
static void Boss2_State_IdleAfterRaiseWaitLoop(void *p) {
    Boss2 *s = (Boss2 *)p;
    if (s->unkBA8 & 0x80)
        s->func = Boss2_State_IdleAfterRaiseCountdown;
}

/* Boss2_State_IdleAfterRaiseCountdown (copied) */
static void Boss2_State_IdleAfterRaiseCountdown(void *p) {
    Boss2 *s = (Boss2 *)p;
    s->unk12F4--;
    if (s->unk12F4 != 0)
        return;
    s->func = Boss2_State_IdleAfterRaiseFinish;
    s->unkA9C.flags[0] &= ~4;
    s->unkBD4.flags[0] &= ~4;
    s->unkD0C.flags[0] &= ~4;
}

/* Boss2_State_IdleAfterRaiseFinish (copied) */
static void Boss2_State_IdleAfterRaiseFinish(void *p) {
    Boss2 *s = (Boss2 *)p;
    if (!(s->unkBA8 & 2))
        return;
    Boss2_SelectNextAttack(0);
}

/* Boss2_State_IdleBeforeAttack3 (copied) */
static void Boss2_State_IdleBeforeAttack3(void *p) {
    Boss2 *s = (Boss2 *)p;
    s->unk145C--;
    if (s->unk145C > 0)
        return;
    s->func = Boss2_State_IdleBeforeAttack3WaitLoop;
    Boss2_StartIdleFade(s, 3, 0x23, 0x3f);
}

/* Boss2_State_IdleBeforeAttack3WaitLoop (copied) */
static void Boss2_State_IdleBeforeAttack3WaitLoop(void *p) {
    Boss2 *s = (Boss2 *)p;
    if (s->unkBA8 & 0x80)
        s->func = Boss2_State_IdleBeforeAttack3Countdown;
}

/* Boss2_State_IdleBeforeAttack3Countdown (copied) */
static void Boss2_State_IdleBeforeAttack3Countdown(void *p) {
    Boss2 *s = (Boss2 *)p;
    s->unk12F4--;
    if (s->unk12F4 != 0)
        return;
    s->func = Boss2_State_IdleBeforeAttack3Finish;
    s->unkA9C.flags[0] &= ~4;
    s->unkBD4.flags[0] &= ~4;
    s->unkD0C.flags[0] &= ~4;
}

/* Boss2_State_IdleBeforeAttack3Finish (copied) */
static void Boss2_State_IdleBeforeAttack3Finish(void *p) {
    Boss2 *s = (Boss2 *)p;
    if (!(s->unkBA8 & 2))
        return;
    Boss2_SelectNextAttack(0);
}

/* ---------------------------------------------------------- dash */
/* Boss2_State_DashTurn (copied) */
static void Boss2_State_DashTurn(void *p) {
    Boss2 *s = (Boss2 *)p;
    s->unk1304 += s->unk146C;
    if (s->unk146C > 0) {
        if (s->unk1304 >= 0x8000) {
            s->func = Boss2_State_DashStart;
            s->unk146C = 0;
            s->unk1304 = 0x8000;
            s->h.disp_flags &= ~1;
        } else if (s->unk1304 >= 0x4000) {
            s->unk145C += s->unk1460;
            if (s->unk145C > s->unk1464)
                s->unk145C = s->unk1464;
            s->h.x += s->unk145C;
        }
    } else {
        if (s->unk1304 > 0x8000) {
            s->func = Boss2_State_DashStart;
            s->unk146C = 0;
            s->unk1304 = 0;
            s->h.disp_flags |= 1;
        } else if (s->unk1304 <= 0x4000) {
            s->unk145C += s->unk1460;
            if (s->unk145C < s->unk1464)
                s->unk145C = s->unk1464;
            s->h.x += s->unk145C;
        }
    }
}

/* Boss2_State_DashStart (copied) */
static inline s32 Ease_DashStart(s32 t) {
    s32 v;
    s32 n;
    n = 1;
    v = 0x2000;
    do {
        v = FX_Mul(v, t);
    } while (n-- != 0);
    return v;
}
static void Boss2_State_DashStart(void *p) {
    Boss2 *s = (Boss2 *)p;
    Boss2_UpdateArenaSpeed(s, 0);
    s->unk146C += 0x5b;
    s->unk146C = (s->unk146C >= 0x1000) ? 0x1000 : s->unk146C;
    s->h.y = Ease_DashStart(s->unk146C) + 0x20000;
    if (s->unk146C < 0x1000)
        return;
    s->func = Boss2_State_Dash;
    box_on(&s->unkF84[0]);
    boss_sfx_handle(s->unk1450, 0xa0);
}

/* Boss2_State_Dash (copied) */
static inline s32 GetDist(Boss2 *s) {
    s32 d = gPlayers->x;
    s32 a;
    s32 b;
    if (d < s->h.x) {
        a = s->h.x - d;
        b = d + 0x6cd98 - s->h.x;
    } else {
        a = s->h.x + 0x6cd98 - d;
        b = d - s->h.x;
    }
    if (s->unk1460 < 0)
        return a;
    return b;
}
static void Boss2_State_Dash(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x = &s->unkA9C;
    BossModel *y = &s->unkBD4;
    BossModel *z = &s->unkD0C;
    DcPlayer *pl;
    s32 a;
    s32 v;
    s32 m;
    s32 t;
    Boss2_UpdateArenaSpeed(s, 0);
    s->unk12F4--;
    if (s->unk12F4 <= 0) {
        s->func = Boss2_State_DashHop;
        s->unk146C = 0;
        boss_sfx_stop(s->unk1450);
        return;
    }
    if (s->unk1470 == 0 && (x->flags[0] & 2)) {
        if (s->unk1460 < 0) {
            set_anim3(s, 6, 0x26, 0x42);
        } else {
            set_anim3(s, 9, 0x29, 0x45);
        }
        x->flags[0] |= 4;
        y->flags[0] |= 4;
        z->flags[0] |= 4;
        x->speed = 0x1000;
        y->speed = 0x1000;
        z->speed = 0x1000;
        s->unk1470 = 1;
        return;
    }
    if (s->unk1470 == 0)
        return;
    pl = gPlayers;
    if (pl->move_flags & 0x8000)
        v = pl->vel_x;
    else
        v = pl->gnd_vel;
    s->unk1464 = s->unk1474;
    if (v >= 0) {
        t = (v < 0x47a) ? 0 : v - 0x47a;
        s->unk1464 += t;
    } else {
        t = (v > -0x47a) ? 0 : v + 0x47a;
        s->unk1464 += t;
    }
    if ((s->unk1460 > 0 && v > 0) || (s->unk1460 < 0 && v < 0)) {
        a = GetDist(s);
        if (s->unk1460 > 0)
            m = v * Lerp(-0x28, -10);
        else
            m = v * Lerp(0x28, 10);
        m += ((s->unk145C < 0) ? -s->unk145C : s->unk145C) * 10 + 0x19000;
        if (m < 0)
            m = 0;
        if (a > m)
            return;
        s->func = Boss2_State_JumpRise;
        x = &s->unkA9C;
        x->flags[0] &= ~4;
        y = &s->unkBD4;
        y->flags[0] &= ~4;
        z = &s->unkD0C;
        z->flags[0] &= ~4;
        z->speed = 0x2000;
        y->speed = z->speed;
        x->speed = y->speed;
        s->unk1470 = 0;
        s->unk146C = 0;
        if (s->unk1460 < 0)
            s->unk1474 = -Lerp(0x180, 0x2a0);
        else
            s->unk1474 = Lerp(0x180, 0x2a0);
        s->unk1468 = s->unk1474 + v;
        s->unk1460 <<= 2;
        g_boss_stats.jumps++;
        boss_log("jump", s->h.x >> 8, gPlayers->x >> 8);
    }
}

/* Boss2_State_DashHop (copied) */
static inline s32 Decay(s32 t) {
    s32 v;
    s32 n;
    n = 1;
    v = 0x2000;
    do {
        v += FX_Mul(-v, t);
    } while (n-- != 0);
    return v;
}
static void Boss2_State_DashHop(void *p) {
    Boss2 *s = (Boss2 *)p;
    Boss2_UpdateArenaSpeed(s, 0);
    s->unk146C += 0x5b;
    s->unk146C = (s->unk146C >= 0x1000) ? 0x1000 : s->unk146C;
    s->h.y = Decay(s->unk146C) + 0x20000;
    if (s->unk146C < 0x1000)
        return;
    s->func = Boss2_State_DashBrake;
    s->h.y = 0x20000;
    s->unk146C_w = (s->unk1460 + 1) * 0x1f;
    s->unk1470 = 0;
    box_off(&s->unkF84[0]);
    box_on(&s->unkF84[6]);
}

/* Boss2_State_DashBrake (copied; unk146C read as s32 here) */
static void Boss2_State_DashBrake(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x;
    BossModel *y;
    BossModel *z;
    s->unk145C -= s->unk1460;
    x = &s->unkA9C;
    if (s->unk1464 < 0) {
        if (s->unk145C > s->unk146C_w) {
            s->unk145C = s->unk146C_w;
            if (x->flags[0] & 2) {
                y = &s->unkBD4;
                z = &s->unkD0C;
                set_anim3(s, 7, 0x27, 0x43);
                x->flags[0] &= ~4;
                y->flags[0] &= ~4;
                z->flags[0] &= ~4;
                x->speed = 0x1000;
                y->speed = 0x1000;
                z->speed = 0x1000;
                s->func = Boss2_State_DashStop;
            }
        }
    } else {
        if (s->unk145C < s->unk146C_w) {
            s->unk145C = s->unk146C_w;
            if (x->flags[0] & 2) {
                y = &s->unkBD4;
                z = &s->unkD0C;
                set_anim3(s, 0xa, 0x2a, 0x46);
                x->flags[0] &= ~4;
                y->flags[0] &= ~4;
                z->flags[0] &= ~4;
                x->speed = 0x1000;
                y->speed = 0x1000;
                z->speed = 0x1000;
                s->func = Boss2_State_DashStop;
            }
        }
    }
    s->h.x += s->unk145C;
}

/* Boss2_State_DashStop (copied) */
static void Boss2_State_DashStop(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x = &s->unkA9C;
    BossModel *y;
    BossModel *z;
    Boss2_UpdateArenaSpeed(s, 1);
    if (s->unk1470 == 0 && (x->flags[0] & 2)) {
        y = &s->unkBD4;
        z = &s->unkD0C;
        set_anim3(s, 4, 0x24, 0x40);
        x->flags[0] &= ~4;
        y->flags[0] &= ~4;
        z->flags[0] &= ~4;
        x->speed = 0x1000;
        y->speed = 0x1000;
        z->speed = 0x1000;
        s->unk1470 = 1;
    }
    if (s->unk145C == 0 && (x->flags[0] & 2) && s->unk1470 != 0)
        Boss2_SelectNextAttack(1);
}

/* ---------------------------------------------------------- jump */
/* Boss2_State_JumpRise (copied) */
static inline s32 Ease_JumpRise(s32 t) {
    s32 v;
    s32 n;
    v = 0;
    n = 1;
    do {
        v = FX_Mul(v - 0x2000, t) + 0x2000;
    } while (n-- != 0);
    return v;
}
static void Boss2_State_JumpRise(void *p) {
    Boss2 *s = (Boss2 *)p;
    DcPlayer *pl = gPlayers;
    BossModel *x = &s->unkA9C;
    BossModel *y = &s->unkBD4;
    BossModel *z = &s->unkD0C;
    s32 v;
    s32 t;
    if (pl->move_flags & 0x8000)
        v = pl->vel_x;
    else
        v = pl->gnd_vel;
    t = s->unk1474 + v;
    if (s->unk1474 > 0) {
        if (t < 0)
            t = 0;
    } else {
        if (t > 0)
            t = 0;
    }
    s->unk1468 = t;
    Boss2_UpdateArenaSpeed(s, 1);
    s->unk146C += 0xcc;
    s->unk146C = (s->unk146C >= 0x1000) ? 0x1000 : s->unk146C;
    s->h.y = Ease_JumpRise(s->unk146C) + 0x20000;
    if (!(x->flags[0] & 2))
        return;
    if (s->unk1470 == 0) {
        if (s->unk1460 < 0) {
            set_anim3(s, 7, 0x27, 0x43);
        } else {
            set_anim3(s, 0xa, 0x2a, 0x46);
        }
        s->unk1470 = 1;
    } else {
        s->func = Boss2_State_JumpAirborne;
        s->h.y = 0x20000;
        set_anim3(s, 0x1d, 0x39, 0x59);
        z->speed = Lerp(0xccc, 0x1333);
        y->speed = z->speed;
        x->speed = y->speed;
        boss_sfx_stop(s->unk1450);
        Sound_PlayVoice(0, 0, 0xd0);
    }
}

/* Boss2_State_JumpAirborne (copied) */
static void Boss2_State_JumpAirborne(void *p) {
    Boss2 *s = (Boss2 *)p;
    DcPlayer *pl = gPlayers;
    BossModel *x = &s->unkA9C;
    s32 v;
    s32 t;
    if (pl->move_flags & 0x8000)
        v = pl->vel_x;
    else
        v = pl->gnd_vel;
    t = s->unk1474 + v;
    if (s->unk1474 > 0) {
        if (t < 0)
            t = 0;
    } else {
        if (t > 0)
            t = 0;
    }
    s->unk1468 = t;
    Boss2_UpdateArenaSpeed(s, 1);
    if (!(x->flags[0] & 2))
        return;
    s->func = Boss2_State_JumpStartFall;
    set_anim3(s, 0x1e, 0x3a, 0x5a);
}

/* Boss2_State_JumpStartFall (copied) */
static void Boss2_State_JumpStartFall(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x = &s->unkA9C;
    s32 a;
    s32 b;
    s32 d;
    Boss2_UpdateArenaSpeed(s, 1);
    if (!(x->flags[0] & 2))
        return;
    s->func = Boss2_State_JumpFall;
    set_anim3(s, 0x1f, 0x3b, 0x5b);
    d = gPlayers->x;
    if (d < s->h.x) {
        a = s->h.x - d;
        b = d + 0x6cd98 - s->h.x;
    } else {
        a = s->h.x + 0x6cd98 - d;
        b = d - s->h.x;
    }
    if (s->unk1460 >= 0)
        a = b;
    if (s->unk1460 < 0) {
        s->unk145C = -FX_DivS32(a, 10);
        if (s->unk145C < s->unk1464)
            s->unk145C = s->unk1464;
    } else {
        s->unk145C = FX_DivS32(a, 10);
        if (s->unk145C > s->unk1464)
            s->unk145C = s->unk1464;
    }
    s->unk1460 = 0;
}

/* Boss2_State_JumpFall (copied) */
static void Boss2_State_JumpFall(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x = &s->unkA9C;
    Boss2_UpdateArenaSpeed(s, 2);
    if (!(x->flags[0] & 2))
        return;
    s->func = Boss2_State_JumpLandShake;
    set_anim3(s, 0x20, 0x3c, 0x5c);
    box_off(&s->unkF84[0]);
    box_on(&s->unkF84[6]);
    ScreenShake_Start(0xc);
    s->unk12F4 = 0xf;
    BossEffect_CreateImpactSmall(1, s->unkA9C.trans[0] >> 4, 0x20000, s->unkA9C.trans[2] >> 4);
    Sound_PlaySeqArc(0, 0, 0xa2);
}

/* Boss2_State_JumpLandShake (copied, including the y = x >> 12 slip) */
static void Boss2_State_JumpLandShake(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x = &s->unkA9C;
    BossModel *y = &s->unkBD4;
    BossModel *z = &s->unkD0C;
    s32 k;
    if (s->unk12F4 >= 0) {
        k = s->unk12F4 << 12;
        CAM.unk3C.x = FX_Div(k * ScreenShake_GetOffsetX(), 0xf000);
        CAM.unk3C.x = CAM.unk3C.x << 12;
        CAM.unk3C.x = CAM.unk3C.x >> 12;
        CAM.unk3C.y = FX_Div(k * ScreenShake_GetOffsetY(), 0xf000);
        CAM.unk3C.y = CAM.unk3C.y << 12;
        CAM.unk3C.y = CAM.unk3C.x >> 12;
        s->unk12F4--;
    }
    if (!(x->flags[0] & 2))
        return;
    s->func = Boss2_State_WaitAnimThenSelectMode1;
    set_anim3(s, 0x21, 0x3d, 0x5d);
    x->speed = 0x1000;
    y->speed = 0x1000;
    z->speed = 0x1000;
    ScreenShake_Start(0);
}

/* Boss2_State_WaitAnimThenSelectMode1 (copied) */
static void Boss2_State_WaitAnimThenSelectMode1(void *p) {
    Boss2 *s = (Boss2 *)p;
    if (!(s->unkBA8 & 2))
        return;
    Boss2_SelectNextAttack(1);
}

/* Boss2_State_WaitAnimThenSelectMode2 (copied) */
static void Boss2_State_WaitAnimThenSelectMode2(void *p) {
    Boss2 *s = (Boss2 *)p;
    if (!(s->unkBA8 & 2))
        return;
    Boss2_SelectNextAttack(2);
}

/* ---------------------------------------------------------- attack 2 */
/* Boss2_State_Attack2Turn (copied) */
static void Boss2_State_Attack2Turn(void *p) {
    Boss2 *s = (Boss2 *)p;
    s->unk1304 += s->unk146C;
    if (s->unk1304 >= 0x8000) {
        s->func = Boss2_State_Attack2EnterTurnMode;
        s->unk1304 = 0x8000;
        box_off(&s->unkF84[6]);
        s->h.disp_flags &= ~1;
        boss_sfx_handle(s->unk1450, 0xa0);
    } else if (s->unk1304 >= 0x4000) {
        Boss2_UpdateArenaSpeed(s, 0);
    }
}

/* Boss2_State_Attack2EnterTurnMode (copied) */
static void Boss2_State_Attack2EnterTurnMode(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x;
    BossModel *y;
    BossModel *z;
    u16 ang;
    Boss2_UpdateArenaSpeed(s, 0);
    if (!(s->unkA9C.flags[0] & 2))
        return;
    s->func = Boss2_State_Attack2StartAim;
    BossArena_ArcToCirclePos(0x1152f0, 0x6cd988, s->h.x << 4, NULL, NULL, &ang);
    s->unk1304 = ang + 0x4000;
    x = &s->unkA9C;
    s->unk20C |= 2;
    s->h.x = 0;
    y = &s->unkBD4;
    z = &s->unkD0C;
    set_anim3(s, 0xb, 0x2b, 0x47);
    x->flags[0] &= ~4;
    y->flags[0] &= ~4;
    z->flags[0] &= ~4;
    x->speed = 0x1000;
    y->speed = 0x1000;
    z->speed = 0x1000;
}

/* the aim start shared by Attack2StartAim and Attack2Recover (u16 views) */
static void Boss2_StartAim(Boss2 *s) {
    BossModel *x = &s->unkA9C, *y = &s->unkBD4, *z = &s->unkD0C;
    s_head_cb = 1; /* NNS_G3dRenderObjSetCallBack(s->unkD0C.unk90, Boss2_NodeCallback_Head, 0, 6, 3) */
    x->flags[0] |= 0x40;
    y->flags[0] |= 0x40;
    z->flags[0] |= 0x40;
    set_anim3(s, 3, 0x23, 0x3f);
    x->flags[0] |= 4;
    y->flags[0] |= 4;
    z->flags[0] |= 4;
    z->blendSpeed = 0xcc;
    y->blendSpeed = z->blendSpeed;
    x->blendSpeed = y->blendSpeed;
    z->speed = 0x1000;
    y->speed = z->speed;
    x->speed = y->speed;
    s->unk145C_h = 0;
    s->unk1460_h = (u16)Lerp(0x12, 0x12);
    s->unk1464_h = (u16)Lerp(0xda, 0x16c);
    s->unk1474 = (u16)Lerp(0xb4, 0x78);
}

/* Boss2_State_Attack2StartAim (copied) */
static void Boss2_State_Attack2StartAim(void *p) {
    Boss2 *s = (Boss2 *)p;
    if (!(s->unkA9C.flags[0] & 2))
        return;
    s->func = Boss2_State_Attack2Aim;
    Boss2_StartAim(s);
    boss_sfx_stop(s->unk1450);
}

/* the head-turn shared by Attack2Aim and Attack2AimEnd (u16 views) */
static void Boss2_AimAtPlayer(Boss2 *s) {
    u16 ang;
    BossArena_ArcToCirclePos(0x1152f0, 0x6cd988, gPlayers->x << 4, NULL, NULL, &ang);
    if (s->unk145C_h >= 0x8000) {
        if ((u16)(ang - s->unk1304 + 0x71c) < 0x4000) {
            s->unk145C_h += s->unk1464_h >> 3;
            if (s->unk145C_h > 0)
                s->unk145C_h = 0;
        }
    } else if (s->unk145C_h != 0) {
        if ((u16)(ang - s->unk1304 - 0x71c) > 0xc000) {
            s->unk145C_h -= s->unk1464_h >> 3;
            if ((s16)s->unk145C_h < 0)
                s->unk145C_h = 0;
        }
    }
    if ((u16)(ang - s->unk1304 - 0x38e) < 0x8000) {
        s->unk145C_h += s->unk1460_h;
        if (s->unk145C_h > s->unk1464_h)
            s->unk145C_h = s->unk1464_h;
    } else if ((u16)(ang - s->unk1304 + 0x38e) > 0x8000) {
        s->unk145C_h -= s->unk1460_h;
        if (s->unk145C_h < (u16)-s->unk1464_h)
            s->unk145C_h = (u16)-s->unk1464_h;
    }
    s->unk1304 += s->unk145C_h;
}

/* Boss2_State_Attack2Aim (copied) */
static void Boss2_State_Attack2Aim(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x;
    BossModel *y;
    BossModel *z;
    Boss2_AimAtPlayer(s);
    s->unk1474--;
    if (s->unk1474 > 0)
        return;
    s->func = Boss2_State_Attack2AimEnd;
    x = &s->unkA9C;
    x->flags[0] |= 0x40;
    y = &s->unkBD4;
    y->flags[0] |= 0x40;
    z = &s->unkD0C;
    z->flags[0] |= 0x40;
    set_anim3(s, 0xc, 0x2c, 0x48);
    x->flags[0] &= ~4;
    y->flags[0] &= ~4;
    z->flags[0] &= ~4;
    z->blendSpeed = 0xcc;
    y->blendSpeed = z->blendSpeed;
    x->blendSpeed = y->blendSpeed;
    z->speed = Lerp(0x3000, 0x5000);
    y->speed = z->speed;
    x->speed = y->speed;
}

/* Boss2_State_Attack2AimEnd (copied) */
static void Boss2_State_Attack2AimEnd(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *q;
    BossModel *x;
    BossModel *y;
    BossModel *z;
    Boss2_AimAtPlayer(s);
    if (!(s->unkA9C.flags[0] & 2))
        return;
    s->func = Boss2_State_Attack2Charge;
    y = &s->unkBD4;
    z = &s->unkD0C;
    x = &s->unkA9C;
    z->speed = 0;
    y->speed = z->speed;
    x->speed = y->speed;
    s->unk131C = BossEffect_CreateSuction2(1, s->unk139C >> 4, -s->unk13A0 >> 4, s->unk13A4 >> 4);
    q = &s->unk131C->unk134;
    s->unk131C->h.disp_flags |= 4;
    s->unk131C->h.disp_flags &= ~0x30;
    s->unk131C->state = NULL;
    q->unk8 = 0x1a;
    q->cb[0] = MOP_FLUSHVP; /* unkA = 4 */
    q->cb[1] = MOP_RESTORE; /* unkB = 2 */
    s->unk1474 = 5;
}

/* Boss2_State_Attack2Charge (copied) */
static void Boss2_State_Attack2Charge(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x;
    BossModel *y;
    BossModel *z;
    s->unk1474--;
    if (s->unk1474 > 0)
        return;
    s->func = Boss2_State_Attack2Strike;
    x = &s->unkA9C;
    y = &s->unkBD4;
    z = &s->unkD0C;
    set_anim3(s, 0xd, 0x2d, 0x49);
    x->flags[0] &= ~4;
    y->flags[0] &= ~4;
    z->flags[0] &= ~4;
    z->speed = Lerp(0x1000, 0x1800);
    y->speed = z->speed;
    x->speed = y->speed;
    box_on(&s->unkF84[1]);
    Sound_PlayVoice(0, 0, 0xd0);
}

/* Boss2_State_Attack2Strike (copied) */
static void Boss2_State_Attack2Strike(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x;
    BossModel *y;
    BossModel *z;
    s32 v;
    if (s->unkA9C.flags[0] & 2) {
        s->func = Boss2_State_Attack2Recover;
        x = &s->unkA9C;
        y = &s->unkBD4;
        z = &s->unkD0C;
        set_anim3(s, 0xe, 0x2e, 0x4a);
        x->flags[0] &= ~4;
        y->flags[0] &= ~4;
        z->flags[0] &= ~4;
        box_off(&s->unkF84[1]);
        box_on(&s->unkF84[7]);
        return;
    }
    if (s->unkA9C.anm[0]->frame < 0xd000)
        return;
    if (s->unkF84[4].flags & 4)
        return;
    box_on(&s->unkF84[4]);
    v = (GetIdx() == 1) ? Lerp(0x38e, 0x666) : data_ov000_023651d8[gBossStatus.unk8 - 1];
    s->unkD0C.speed = v;
    s->unkBD4.speed = s->unkD0C.speed;
    s->unkA9C.speed = s->unkBD4.speed;
    Sound_PlaySeqArc(0, 0, 0xa3);
    s->unk131C->h.obj_flags |= 4;
}

/* Boss2_State_Attack2Recover (copied) */
static void Boss2_State_Attack2Recover(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x;
    BossModel *y;
    BossModel *z;
    if (s->unkA9C.flags[0] & 2) {
        s->unk1470--;
        if (s->unk1470 > 0) {
            s->func = Boss2_State_Attack2Aim;
            Boss2_StartAim(s);
            return;
        }
        s->func = Boss2_State_Attack2End;
        s_head_cb = 0; /* NNS_G3dRenderObjResetCallBack(s->unkD0C.unk90) */
        x = &s->unkA9C;
        y = &s->unkBD4;
        z = &s->unkD0C;
        set_anim3(s, 0xf, 0x2f, 0x4b);
        x->flags[0] &= ~4;
        y->flags[0] &= ~4;
        z->flags[0] &= ~4;
        x->speed = 0x1000;
        y->speed = 0x1000;
        z->speed = 0x1000;
        boss_sfx_handle(s->unk1450, 0xa0);
        return;
    }
    if (s->unkA9C.anm[0]->frame <= 0x21000)
        return;
    box_off(&s->unkF84[4]);
    box_off(&s->unkF84[7]);
    s->unkA9C.speed = 0x1000;
    s->unkBD4.speed = 0x1000;
    s->unkD0C.speed = 0x1000;
}

/* Boss2_State_Attack2End (copied) */
static void Boss2_State_Attack2End(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x;
    BossModel *y;
    BossModel *z;
    if (!(s->unkA9C.flags[0] & 2))
        return;
    s->func = Boss2_State_WaitAnimThenSelectMode2;
    x = &s->unkA9C;
    y = &s->unkBD4;
    z = &s->unkD0C;
    set_anim3(s, 0xa, 0x2a, 0x46);
    x->flags[0] &= ~4;
    y->flags[0] &= ~4;
    z->flags[0] &= ~4;
    x->speed = 0x1000;
    y->speed = 0x1000;
    z->speed = 0x1000;
    s->unk20C &= ~2;
    s->h.x = FX_Mul((s16)((u16)(s->unk1304 - 0x4000) >> 4), 0x6cd98);
    s->unk1304 = 0x8000;
    box_on(&s->unkF84[6]);
    boss_sfx_stop(s->unk1450);
}

/* Boss2_State_Attack2Hurt (copied) */
static void Boss2_State_Attack2Hurt(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x;
    BossModel *y;
    BossModel *z;
    if (s->unkA9C.flags[0] & 2) {
        s->func = Boss2_State_Attack2End;
        s_head_cb = 0; /* NNS_G3dRenderObjResetCallBack */
        x = &s->unkA9C;
        y = &s->unkBD4;
        z = &s->unkD0C;
        set_anim3(s, 0xf, 0x2f, 0x4b);
        x->flags[0] &= ~4;
        y->flags[0] &= ~4;
        z->flags[0] &= ~4;
        x->speed = 0x1000;
        y->speed = 0x1000;
        z->speed = 0x1000;
        Boss2_SetPalettes(4);
    } else {
        s->unk1474++;
        Boss2_SetPalettes((u32)s->unk1474);
    }
}

/* ---------------------------------------------------------- attack 3 */
/* Boss2_State_Attack3Start (copied) */
static void Boss2_State_Attack3Start(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x;
    BossModel *y;
    BossModel *z;
    BossModel *q;
    if (!(s->unkA9C.flags[0] & 2))
        return;
    s->func = Boss2_State_Attack3Suck;
    x = &s->unkA9C;
    y = &s->unkBD4;
    z = &s->unkD0C;
    if (s->unk145C_h) {
        set_anim3(s, 0x18, 0x36, 0x54);
    } else {
        set_anim3(s, 0x12, 0x32, 0x4e);
    }
    x->flags[0] &= ~4;
    y->flags[0] &= ~4;
    z->flags[0] &= ~4;
    z->speed = FX_DivS32(gBossStatus.unk8 * 0xb33 + (gBossStatus.unkA - gBossStatus.unk8) * 0x1800, gBossStatus.unkA);
    y->speed = z->speed;
    x->speed = y->speed;
    s->unk20C |= 4;
    y->cb[1] = 0; /* y->unkB = 0 */
    box_on(&s->unkF84[3]);
    box_on(&s->unkF84[9]);
    boss_sfx_handle(s->unk1458, 0xa4);
    s->unk131C = BossEffect_CreateSuction2(1, s->unk139C >> 4, -s->unk13A0 >> 4, s->unk13A4 >> 4);
    q = &s->unk131C->unk134;
    s->unk131C->h.disp_flags |= 4;
    s->unk131C->h.disp_flags &= ~0x30;
    s->unk131C->state = NULL;
    q->unk8 = 0x1b;
    q->cb[0] = MOP_FLUSHVP;
    q->cb[1] = MOP_RESTORE;
}

/* Boss2_State_Attack3Suck (copied) */
static void Boss2_State_Attack3Suck(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x = &s->unkA9C;
    s32 pp;
    s32 a;
    s32 d1;
    BossModel *y = &s->unkBD4;
    BossModel *z = &s->unkD0C;
    s32 d2;
    if (y->flags[0] & 2) {
        s->func = Boss2_State_Attack3Catch;
        if (s->unk145C_h) {
            Model_SetAnimation(x, 0, s->unkF7C, 0x19);
            Model_SetAnimation(z, 0, s->unkF7C, 0x55);
        } else {
            Model_SetAnimation(x, 0, s->unkF7C, 0x13);
            Model_SetAnimation(z, 0, s->unkF7C, 0x4f);
        }
        x->flags[0] &= ~4;
        z->flags[0] &= ~4;
        x->speed = 0x1000;
        z->speed = 0x1000;
        boss_sfx_stop(s->unk1458);
        Sound_PlayVoice(0, 0, 0xd2);
        s->unk131C->h.obj_flags |= 4;
        return;
    }
    BossArena_CirclePosToArc(0x1152f0, 0x6cd988, s->unk139C, s->unk13A4, &a);
    pp = gPlayers->x << 4;
    if (pp < a) {
        d1 = a - pp;
        d2 = pp + 0x6cd988 - a;
    } else {
        d2 = pp - a;
        d1 = a + 0x6cd988 - pp;
    }
    if (s->unk145C_h == 0)
        d2 = d1;
    if (d2 > FX_DivS32(gBossStatus.unk8 * 0xfa000 + (gBossStatus.unkA - gBossStatus.unk8) * 0xb4000, gBossStatus.unkA))
        return;
    s->func = Boss2_State_Attack3Hold;
    y->speed = 0;
    s->unk1464 = d2;
    s->unk12F4 = FX_DivS32(gBossStatus.unk8 * 0x14 + (gBossStatus.unkA - gBossStatus.unk8) * 0xa, gBossStatus.unkA);
    boss_sfx_stop(s->unk1458);
}

/* Boss2_State_Attack3Hold (copied) */
static void Boss2_State_Attack3Hold(void *p) {
    Boss2 *s = (Boss2 *)p;
    s32 v;
    BossModel *y;
    s->unk12F4--;
    if (s->unk12F4 > 0)
        return;
    v = s->unk1464;
    s->func = Boss2_State_Attack3Close;
    y = &s->unkBD4;
    y->speed = FX_DivS32(gBossStatus.unk8 * 0x1666 + (gBossStatus.unkA - gBossStatus.unk8) * 0x3000, gBossStatus.unkA);
    if (v < 0)
        v = 0;
    s->unk12F4 = FX_DivS32(v * 300, 0x51a326);
    if (300 - (FX_DivS32(gBossStatus.unk8 * 8 + (gBossStatus.unkA - gBossStatus.unk8) * 4, gBossStatus.unkA) +
               (y->anm[0]->frame >> 12)) < s->unk12F4)
        s->unk12F4 = 300 - (FX_DivS32(gBossStatus.unk8 * 8 + (gBossStatus.unkA - gBossStatus.unk8) * 4, gBossStatus.unkA) +
                            (y->anm[0]->frame >> 12));
    s->unk12F4 = FX_Div(s->unk12F4, y->speed);
    Sound_PlayVoice(0, 0, 0xd2);
}

/* Boss2_State_Attack3Close (copied) */
static void Boss2_State_Attack3Close(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x;
    BossModel *y;
    BossModel *z;
    s->unk12F4--;
    if (s->unk12F4 > 0 && !(s->unkBD4.flags[0] & 2))
        return;
    s->func = Boss2_State_Attack3Catch;
    x = &s->unkA9C;
    y = &s->unkBD4;
    z = &s->unkD0C;
    if (s->unk145C_h) {
        Model_SetAnimation(x, 0, s->unkF7C, 0x19);
        Model_SetAnimation(z, 0, s->unkF7C, 0x55);
    } else {
        Model_SetAnimation(x, 0, s->unkF7C, 0x13);
        Model_SetAnimation(z, 0, s->unkF7C, 0x4f);
    }
    x->flags[0] &= ~4;
    z->flags[0] &= ~4;
    x->speed = z->speed = 0x1000;
    y->speed = 0;
    s->unk12F4 = 0;
    s->unk131C->h.obj_flags |= 4;
}

/* Boss2_State_Attack3Catch (copied) */
static void Boss2_State_Attack3Catch(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x;
    BossModel *y;
    BossModel *z;
    s32 v;
    if (s->unkA9C.flags[0] & 2) {
        s->func = Boss2_State_Attack3Recover;
        y = &s->unkBD4;
        x = &s->unkA9C;
        z = &s->unkD0C;
        v = 0x12d000 - y->anm[0]->frame;
        if (s->unk145C_h) {
            set_anim3(s, 0x1a, 0x37, 0x56);
        } else {
            set_anim3(s, 0x14, 0x33, 0x50);
        }
        x->flags[0] &= ~4;
        y->flags[0] &= ~4;
        z->flags[0] &= ~4;
        z->speed = FX_DivS32(gBossStatus.unk8 * 0x666 + (gBossStatus.unkA - gBossStatus.unk8) * 0xb33, gBossStatus.unkA);
        y->speed = z->speed;
        x->speed = y->speed;
        y->anm[0]->frame = v;
        s->unk12F4 = 200;
        boss_sfx_handle(s->unk1458, 0xa4);
        return;
    }
    if (s->unk12F4 > 0) {
        s->unk12F4--;
        if (s->unk12F4 > 0)
            return;
        s->unk12F4 = -1;
        s->unkA9C.speed = s->unkBD4.speed = s->unkD0C.speed = 0x1000;
        return;
    }
    if (s->unkD0C.anm[0]->frame < 0x9000)
        return;
    if (s->unk12F4 != 0)
        return;
    box_on(&s->unkF84[5]);
    box_off(&s->unkF84[3]);
    if (!(s->unk20C & 0x40)) {
        s32 a;
        s32 b;
        BossEffect *obj;
        s->unk20C |= 0x40;
        BossArena_CirclePosToArc(0x1152f0, 0x6cd988, s->unk139C, s->unk13A4, &a);
        if (s->unk145C_h == 0)
            a -= 0x50000;
        else
            a += 0x50000;
        if (a < 0x40000)
            a = 0x70d988 - (0x40000 - a);
        else if (a > 0x70d988)
            a += -0x6cd988;
        BossArena_ArcToCirclePos(0x1152f0, 0x6cd988, a, &a, &b, NULL);
        obj = BossEffect_CreateCatch(1, a >> 4, -s->unk13A0 >> 4, b >> 4);
        if (s->unk145C_h == 0) /* MTX_RotZ33_(obj + 0x158, data_0206f470[0], [1]): angle 0x4000 */
            MTX_RotZ33(&obj->unk134.rot, SIN(0x4000), COS(0x4000));
        else                   /* data_02071470: angle 0xc000 */
            MTX_RotZ33(&obj->unk134.rot, SIN(0xc000), COS(0xc000));
        Sound_PlaySeqArc(0, 0, 0xa3);
    }
    s->unkA9C.speed = s->unkBD4.speed = s->unkD0C.speed = 0;
    s->unk12F4 = FX_DivS32(gBossStatus.unk8 * 0x3c + (gBossStatus.unkA - gBossStatus.unk8) * 0x1e, gBossStatus.unkA);
}

/* Boss2_State_Attack3Recover (copied) */
static void Boss2_State_Attack3Recover(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x;
    BossModel *y;
    BossModel *z;
    if (s->unkBD4.flags[0] & 2) {
        s->func = Boss2_State_Attack3End;
        y = &s->unkBD4;
        s->unk20C &= ~4;
        y->cb[1] = MOP_RESTORE; /* y->unkB = 2 */
        x = &s->unkA9C;
        z = &s->unkD0C;
        MTX_RotY33(&y->rot, SIN(0), COS(0));
        if (s->unk145C_h) {
            set_anim3(s, 0x1b, 0x38, 0x57);
        } else {
            set_anim3(s, 0x15, 0x34, 0x51);
        }
        x->flags[0] &= ~4;
        y->flags[0] &= ~4;
        z->flags[0] &= ~4;
        x->speed = 0x1000;
        y->speed = 0x1000;
        z->speed = 0x1000;
        Boss2_SetTargetAngle(s, 0);
        box_off(&s->unkF84[5]);
        box_off(&s->unkF84[9]);
        boss_sfx_stop(s->unk1458);
    } else {
        s->unk12F4--;
        if (s->unk12F4 == 0)
            s->unkBD4.speed = 0x4000;
    }
}

/* Boss2_State_Attack3Hurt (copied) */
static void Boss2_State_Attack3Hurt(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x;
    BossModel *y = &s->unkBD4;
    BossModel *z;
    if ((s->unkA9C.flags[0] & 2) && (y->flags[0] & 2)) {
        s->func = Boss2_State_Attack3End;
        s->unk20C &= ~4;
        y->cb[1] = MOP_RESTORE; /* y->unkB = 2 */
        MTX_RotY33(&y->rot, SIN(0), COS(0));
        x = &s->unkA9C;
        y = &s->unkBD4;
        z = &s->unkD0C;
        if (s->unk145C_h) {
            set_anim3(s, 0x1b, 0x38, 0x57);
        } else {
            set_anim3(s, 0x15, 0x34, 0x51);
        }
        x->flags[0] &= ~4;
        y->flags[0] &= ~4;
        z->flags[0] &= ~4;
        x->speed = 0x1000;
        y->speed = 0x1000;
        z->speed = 0x1000;
        Boss2_SetPalettes(4);
        boss_sfx_stop(s->unk1458);
    } else {
        s->unk1474++;
        Boss2_SetPalettes((u32)s->unk1474);
    }
}

/* Boss2_State_Attack3End (copied) */
static void Boss2_State_Attack3End(void *p) {
    Boss2 *s = (Boss2 *)p;
    if (!(s->unkA9C.flags[0] & 2))
        return;
    s_head_cb = 0; /* NNS_G3dRenderObjResetCallBack(s->unkD0C.unk90) */
    s->unk20C &= ~2;
    s->unk1304 = s->unk145C_h;
    s->h.x = s->unk1460;
    Boss2_SelectNextAttack(3);
    box_off(&s->unkF84[8]);
}

/* ---------------------------------------------------------- defeat */
/* Boss2_State_Defeat (modified: display registers -> boss state) */
static void Boss2_State_Defeat(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossModel *x;
    BossModel *y;
    BossModel *z;
    BossEffect *obj;
    s->unk12F4--;
    if (g_boss_game.bright > 0)
        g_boss_game.bright -= 2;
    if (s->unk12F4 > 0)
        return;
    s->unk145C++;
    if (s->unk145C >= 3) {
        y = &s->unkBD4;
        s->func = Boss2_State_DefeatFinish;
        z = &s->unkD0C;
        y->cb[1] = 0; /* y->unkB = 0 */
        z->cb[1] = 0;
        y->trans[1] = -0x200000;
        z->trans[1] = -0x200000;
        Boss_DrawExcBackground((u32)-1); /* DISPCNT &= ~BG2: the cut-in background off */
        g_boss_game.exc_bg = -1;
        x = &s->unkA9C;
        set_anim3(s, 0x22, 0x3e, 0x5e);
        x->flags[0] &= ~4;
        y->flags[0] &= ~4;
        z->flags[0] &= ~4;
        x->speed = y->speed = z->speed = 0x1000;
        s->unk1304 = 0;
        s->h.x = 0;
        s->h.y = 0x20000;
        s->h.z = 0;
        StageObj_ClearPause();
        s->h.obj_flags &= ~0x20;
        s->unk214[1].speed = s->unk214[2].speed = 0x1000; /* unk460 = unk598 */
        gPlayers->disp_flags |= 0x20;
        gPlayers->pf |= 0x200000;
        g_boss_game.bright = 0x10;
        CAM.t[1].tgt.x = 0;                                          /* c->unk54 */
        CAM.t[1].tgt.y = -data_ov000_023651fc[s->unk1460][0] << 12;  /* c->unk58 */
        CAM.t[1].tgt.z = 0;                                          /* c->unk5C */
        CAM.unkC0 = CAM.unkC4 = 0x12c000;
        CAM.unkCC = CAM.unkD0 = -0x80000;
        CAM.unkD6 = CAM.unkD8 = 0;
        obj = BossEffect_CreateBlast(0, 0, data_ov000_023651fc[s->unk1460][0] << 8, 0);
        obj->unk134.speed = 0x800; /* obj + 0x248 */
        Sound_PlaySeqArc(0, 0, 0x7d);
        s->unk12F4 = 0x12c;
        NaviMsg_SetFaceAnim(0xd);
        NaviMsg_ShowMessage(0x13, 0xf0);
    } else {
        s->unk12F4 = 0x1e;
        Boss2_DrawDefeatBackground(s->unk1460, s->unk145C);
        g_boss_game.bright = 0x10;
        Sound_PlaySeqArc(0, 0, 0x7c);
    }
}

/* Boss2_State_DefeatFinish (copied) */
static void Boss2_State_DefeatFinish(void *p) {
    Boss2 *s = (Boss2 *)p;
    BossEffect *a;
    BossEffect *b;
    u16 h;
    s->unk12F4--;
    if ((s->unk12F4 & 3) == 0) {
        if (g_boss_game.bright > 0)
            g_boss_game.bright--;
    }
    if (s->unk12F4 <= 0) {
        g_obj_stage.ring_bonus = gPlayers->rings * 1000; /* data_022c4590.unk10 */
        if (!g_boss_game.cleared) {                      /* StageShutdown_Create(Stage_OnBossCleared) */
            g_boss_game.cleared = 1;
            boss_log("cleared", gPlayers->rings, 0);
            if (g_boss_hooks.cleared)
                g_boss_hooks.cleared(g_boss_hooks.ctx);
        }
        return;
    }
    if (s->unk12F4 != 0xd7)
        return;
    h = gCamera_unk62;
    if (!(h <= 502.1))
        return;
    a = EffRipple3D_Spawn(&s->h, 0, 1, 1, h, gCamera_unkE0, gCamera_unkE4);
    b = EffSplash3D_Spawn(&s->h, 0, 0, gCamera_unk62, 1, gCamera_unkE0, gCamera_unkE4);
    a->h.x = b->h.x = -0x6400;
    a->h.z = b->h.z = 0;
    a = EffRipple3D_Spawn(&s->h, 0, 1, 1, gCamera_unk62, gCamera_unkE0, gCamera_unkE4);
    b = EffSplash3D_Spawn(&s->h, 0, 0, gCamera_unk62, 1, gCamera_unkE0, gCamera_unkE4);
    a->h.x = b->h.x = 0x6400;
    a->h.z = b->h.z = 0;
    a = EffRipple3D_Spawn(&s->h, 0, 1, 2, gCamera_unk62, gCamera_unkE0, gCamera_unkE4);
    b = EffSplash3D_Spawn(&s->h, 0, 0, gCamera_unk62, 1, gCamera_unkE0, gCamera_unkE4);
    a->h.x = b->h.x = 0x6400;
    a->h.z = b->h.z = 0x6400;
}

/* ============================================================ creation */
/* MapObject_Init (obj_manager.c obj_mapobject_init): the fields and default
 * boxes (unk134 = coll[0], unk160 = coll[1]) Boss2_Create uses */
static void MapObject_Init_boss2(Boss2 *s, const ObjParam *prm, s32 x, s32 y) {
    DcPlayer *h = &s->h;
    h->obj_flags |= 0x40;
    h->x = x;
    h->y = y;
    h->type = (prm != NULL && prm->type < 0x40) ? 2 : 3;
    h->gravity = 0x2a;
    h->fall_max = 0xf00;
    h->obj_flags |= 1;
    h->disp_flags |= 1;
    h->move_flags |= 0x200;
    s->unk134.mask = 0x12;
    s->unk160.mask = 0x12;
    s->unk134.cb = NULL; /* MapObject_OnHit_Default: replaced by data_ov000_0236521c */
    rect_set_atkdef(&s->unk134, 9, 0xa);
    rect_set_atkdef(&s->unk160, 0x13, 0x13);
    s->unk134.flags &= ~4;
    s->unk160.flags &= ~4;
}

/* Boss2_Create (modified: resources from BossStage2 / bossdata.bin) */
void *Boss2_Create(const ObjParam *a, s32 x, s32 y) {
    s32 i;
    Boss2 *s;
    ObjBox *h;
    ObjBox *h2;
    BossModel *p;
    u32 k;
    if (a->x0 == 0xff && a->y0 == 0xff)
        return NULL;
    s = &s_boss2;              /* Task_Create(StageObj_Main, Boss2_Destructor, 0, 0x500, 2, 0x1478) */
    memset(s, 0, sizeof(*s));  /* MI_CpuFill16(s, 0, 0x1478) */
    s->alive = 1;
    gBoss2 = s;
    s->unk20C |= 9;
    s->unk12FE = Rand() & 1;
    memset(&gBossStatus, 0, sizeof(gBossStatus));
    gBossStatus.unk4 = 7;
    gBossStatus.unk8 = (u16)Boss_GetStartHealth();
    gBossStatus.unkA = (u16)Boss_GetStartHealth();
    MapObject_Init_boss2(s, a, x, y);
    s->func = Boss2_State_WaitForStart;        /* unk68; unk70 = Boss2_Draw_ModelsAndHitboxes */
    s->h.disp_flags |= 0x20;
    s->h.obj_flags |= 0x12;
    s->h.move_flags |= 0xa300;
    s->h.x = 0;
    s->h.y = 0x20000;
    s->h.z = 0;
    h = s->unkF84;
    h2 = s->unk113C;
    for (i = 0; i < 10; i++, h2++, h++) {
        switch (data_ov000_02362f30[i]) {
        case 0:
            *h = s->unk160;
            break;
        case 1:
            *h = s->unk160;
            h->atk |= 0x20;
            break;
        case 2:
            *h = s->unk134;
            break;
        case 3:
            rect_set_atkdef(h, 0, (u32)-0x10);
            h->flags |= 0xc0;
            h->mask = 0x12;
            break;
        }
        rect_set_box3d(h, data_ov000_0236301c[i][0], data_ov000_0236301c[i][1], data_ov000_0236301c[i][2],
                       data_ov000_0236301c[i][3], -0x7d00, 0x7d00);
        h->owner = s;
        h->owner_kind = OBJ_OWNER_OBJECT;
        h->flags &= ~4;
        h->cb = Boss2_HitCallback(data_ov000_0236521c[i]);
        *h2 = *h;
    }
    s->unk113C[1].zn = -0x10;
    s->unkF84[1].zn = s->unk113C[1].zn;
    s->unk113C[1].zf = 0x23;
    s->unkF84[1].zf = s->unk113C[1].zf;
    /* s->unk78 = 0 (no collide callback) */
    s->unkF7C = &s_bs2_anim;   /* NNS_FndGetArchiveFileByIndex(ctx, 8) */
    s->unkF80 = &s_bs2_srt;    /* file 9 */
    /* NNS_G3dResDefaultSetup(bs2:eff_*.nsbmd): boss_effects_load */
    p = s->unk214;
    for (i = 0; i < 11; i++) {
        Model_Init(p, 0);
        Model_SetResource(p, &gBossStage2Models[i]);
        p->scale[2] = 0x34cc;
        p->scale[1] = p->scale[2];
        p->scale[0] = p->scale[1];
        p++;
    }
    p = &s->unk214[0];
    p->trans[0] = 0;
    p->trans[1] = -0x200000;
    p->trans[2] = 0;
    p = &s->unk214[7];
    p->trans[0] = 0;
    p->trans[1] = -0x200000;
    p->trans[2] = 0;
    /* NNS_G3dRenderObjSetCallBack(p->unk90, Boss2_NodeCallback_Body, 0, 6, 3) */
    p->cb[0] = MOP_FLUSHVP;
    p = &s->unk214[8];
    p->trans[0] = 0;
    p->trans[1] = -0x200000;
    p->trans[2] = 0;
    p->unk8 = 0x1d;
    p->cb[0] = MOP_FLUSHVP;
    p->cb[1] = MOP_RESTORE;
    /* NNS_G3dRenderObjSetCallBack(p->unk90, Boss2_NodeCallback_NeckHead, 0, 6, 3) */
    p = &s->unk214[9];
    p->trans[0] = 0;
    p->trans[1] = 0;
    p->trans[2] = 0;
    p->unk8 = 0x1b;
    p->cb[0] = MOP_FLUSHVP;
    p->cb[1] = MOP_RESTORE;
    p = &s->unk214[10];
    p->trans[0] = 0;
    p->trans[1] = 0;
    p->trans[2] = 0;
    p->unk8 = 0x1c;
    p->cb[0] = MOP_FLUSHVP;
    p->cb[1] = MOP_RESTORE;
    p->cb[2] = MOP_ROTATE;
    p = &s->unk214[1];
    Model_SetAnimation(p, 0, gBoss2->unkF7C, 0);
    p->flags[0] |= 4;
    p->trans[0] = 0;
    p->trans[1] = (s32)0xffe66000;
    p->trans[2] = 0x32000;
    p = &s->unk214[2];
    Model_SetAnimation(p, 3, gBoss2->unkF80, 3);
    p->flags[3] |= 4;
    p->trans[0] = 0;
    p->trans[1] = (s32)0xffe66000;
    p->trans[2] = 0;
    p = &s->unk214[4];
    Model_SetAnimation(p, 3, gBoss2->unkF80, 1);
    Model_SetAnimation(p, 0, gBoss2->unkF7C, 1);
    p->flags[3] |= 4;
    p->trans[0] = 0;
    p->trans[1] = -0x200000;
    p->trans[2] = 0;
    p = &s->unk214[3];
    Model_SetAnimation(p, 3, gBoss2->unkF80, 0);
    p->flags[3] |= 4;
    p->trans[0] = 0;
    p->trans[1] = -0x200000;
    p->trans[2] = 0;
    p = &s->unk214[5];
    Model_SetAnimation(p, 3, gBoss2->unkF80, 2);
    Model_SetAnimation(p, 0, gBoss2->unkF7C, 2);
    p->flags[3] |= 4;
    p->trans[0] = 0;
    p->trans[1] = -0x200000;
    p->trans[2] = 0;
    s->unk1300 = 1;
    s->unk1302 = 0xffff;
    k = (u32)g_boss_game.unk0; /* gGameState (player slot) */
    s->unk1424 = data_ov000_02362e28[k];
    s->unk1428 = data_ov000_02362e1c[k];
    s->unk142C = data_ov000_02362e70[k];
    s->unk1430 = data_ov000_02362e90[k];
    s->unk1434 = &data_ov000_02362dd8[k];
    s->unk1438 = &data_ov000_02362de8[k];
    s->unk143C = &data_ov000_02362dd4[k];
    s->unk1440 = &data_ov000_02362de4[k];
    s->unk1444 = &data_ov000_02362dd0[k];
    s->unk1448 = &data_ov000_02362de0[k];
    Boss_LoadModelPalettes(7, data_ov000_02365110, gBoss2->unk1424, data_ov000_02362e04, 3);
    Boss_LoadModelPalettes(9, data_ov000_023651c8, gBoss2->unk142C, data_ov000_02362e0c, 4);
    Boss_LoadModelPalettes(8, data_ov000_023650fc, gBoss2->unk1434, data_ov000_02362dc8, 1);
    Boss_LoadModelPalettes(10, data_ov000_02365108, gBoss2->unk143C, data_ov000_02362dc4, 1);
    s->unk1450 = 1; /* Sound_AllocHandle() */
    s->unk1454 = 2;
    s->unk1458 = 3;
    s_head_cb = 0;
    gBoss2WaterLightTask = NULL;
    boss_log("create", gBossStatus.unk8, s->unk12FE);
    return s;
}

/* Boss2_Destructor (modified) */
static void Boss2_Destructor(void *p) {
    Boss2 *s = (Boss2 *)p;
    s32 i;
    for (i = 0; i < 11; i++)
        boss_model_release(&s->unk214[i]);
    /* NNS_G3dResDefaultRelease(bs2:eff_*.nsbmd): boss_effects_shutdown */
    for (i = 0; i < 10; i++) {
        rect_forget(&s->unkF84[i]);
        rect_forget(&s->unk113C[i]);
    }
    if (s->unk210 != NULL) { /* Task_Destroy(s->unk210) */
        s->unk210->alive = 0;
        Boss2Water_Destructor();
    }
    ScreenShake_Start(0);
    boss_sfx_stop(s->unk1450); /* Sound_FreeHandle */
    boss_sfx_stop(s->unk1454);
    boss_sfx_stop(s->unk1458);
    gBoss2 = NULL;
    g_boss_game.draw_player = NULL;
}

/* ============================================================ module glue */
/* BossStage2_OnModelLoaded: data_020747a0 = bs2_base, bs2_lotus, bs2_water, bs2_shower,
 * bs2_shower_s, bs2_shower_e, bs2_sky, bs2_body, bs2_neck, bs2_head, bs2_prop
 * (NaviBg_Load / NaviB_Load after the last one: HUD, not here) */
int boss2_load(void) {
    static const char *const files[11] = {
        "mod__bs2_base__bs2_base.dcm",       "mod__bs2_lotus__bs2_lotus.dcm",   "mod__bs2_water__bs2_water.dcm",
        "mod__bs2_shower__bs2_shower.dcm",   "mod__bs2_shower_s__bs2_shower_s.dcm",
        "mod__bs2_shower_e__bs2_shower_e.dcm", "mod__bs2_sky__bs2_sky.dcm",     "mod__bs2_body__bs2_body.dcm",
        "mod__bs2_neck__bs2_neck.dcm",       "mod__bs2_head__bs2_head.dcm",     "mod__bs2_prop__bs2_prop.dcm"};
    int i, r = 0;
    for (i = 0; i < 11; i++)
        if (!gBossStage2Models[i].loaded)
            r |= boss_res_load_model(&gBossStage2Models[i], files[i]);
    if (!s_bs2_anim.loaded)
        r |= boss_res_load_anim(&s_bs2_anim, "narc__z23_act_lz7__bs2.dca");
    memset(&s_bs2_srt, 0, sizeof(s_bs2_srt));
    if (boss_anim_lengths("bs2.nsbta", &s_bs2_srt.nframes, &s_bs2_srt.count) != 0) {
        printf("boss: bs2.nsbta frame counts missing in bossdata.bin\n");
        r = -1;
    }
    s_joint[0] = boss_joint_index("bs2_body", ROM_STR(data_ov000_0236511c[0]));
    s_joint[1] = boss_joint_index("bs2_body", ROM_STR(data_ov000_0236511c[1]));
    s_joint[2] = boss_joint_index("bs2_body", ROM_STR(data_ov000_0236511c[2]));
    s_joint[3] = boss_joint_index("bs2_neck", ROM_STR(data_ov000_02365100));
    s_joint[4] = boss_joint_index("bs2_head", ROM_STR(data_ov000_02365104));
    for (i = 0; i < 5; i++)
        if (s_joint[i] < 0)
            r = -1;
    return r ? -1 : 0;
}

/* BossStage2_FreeModels */
void boss2_unload(void) {
    int i;
    for (i = 0; i < 11; i++)
        boss_res_free_model(&gBossStage2Models[i]);
    boss_res_free_anim(&s_bs2_anim);
    memset(&s_bs2_srt, 0, sizeof(s_bs2_srt));
}

void boss2_destroy(void) {
    if (s_boss2.alive)
        Boss2_Destructor(&s_boss2);
    s_boss2.alive = 0;
    s_water.alive = 0;
    gBoss2WaterLightTask = NULL;
    gBoss2 = NULL;
}

static const BossObjOps s_ops_boss2 = {Boss2_Draw_ModelsAndHitboxes, NULL, Boss2_Destructor};

void boss2_update(void) {
    if (s_water.alive && gBoss2 != NULL)
        Boss2Water_Main();                    /* 0x4ff */
    if (s_boss2.alive)
        boss_obj_main(&s_boss2.h, &s_boss2.func, &s_ops_boss2, &s_boss2, &s_boss2.alive);
    if (gBoss2WaterLightTask != NULL)
        gBoss2WaterLightTask();               /* 0x500, created after the boss */
}

/* light vectors are world space; NNS sends them through the camera matrix */
void boss2_build_lights(void) {
    dcm_camera_t cam;
    int i, c;
    if (!s_light_set) {
        g_boss_lights = NULL;
        return;
    }
    boss_camera(&cam);
    memset(&s_lights, 0, sizeof(s_lights));
    s_lights.count = 3;
    for (i = 0; i < 3; i++) {
        float v[3];
        v[0] = s_light_vec[i][0] / 4096.0f;
        v[1] = s_light_vec[i][1] / 4096.0f;
        v[2] = s_light_vec[i][2] / 4096.0f;
        for (c = 0; c < 3; c++)
            s_lights.dir[i][c] = v[0] * cam.view.m[0][c] + v[1] * cam.view.m[1][c] + v[2] * cam.view.m[2][c];
        s_lights.col[i][0] = (s_light_col[i] & 31) / 31.0f;
        s_lights.col[i][1] = ((s_light_col[i] >> 5) & 31) / 31.0f;
        s_lights.col[i][2] = ((s_light_col[i] >> 10) & 31) / 31.0f;
    }
    g_boss_lights = &s_lights;
}

int boss2_draw(int list) {
    int n = 0, i;
    Boss2 *s = &s_boss2;
    if (!s->alive)
        return 0;
    boss2_build_lights();
    g_boss_debug_color = 0x4080ff;
    if (!(s->unk20C & 1))
        for (i = 0; i <= 2; i++)
            n += boss_model_draw(&s->unk214[i], list);
    if (s->unk1420 != NULL)
        n += boss_model_draw(s->unk1420, list);
    if (!(s->h.disp_flags & 0x20)) {
        g_boss_debug_color = 0x40ff40;
        for (i = 7; i <= 10; i++)
            n += boss_model_draw(&s->unk214[i], list);
    }
    g_boss_debug_color = 0xffff40;
    return n;
}

void boss2_body_pos(s32 *x, s32 *y, s32 *z) {
    *x = s_boss2.unk1348 >> 4;
    *y = -s_boss2.unk134C >> 4;
    *z = s_boss2.unk1350 >> 4;
}
int boss2_box_armed(int box) { return box >= 0 && box < 10 && (s_boss2.unkF84[box].flags & 4) != 0; }
int boss2_box(int box, s32 *x, s32 *y, s32 *z, s16 rect[4]) {
    const ObjBox *b;
    if (!s_boss2.alive || box < 0 || box >= 10)
        return 0;
    b = &s_boss2.unkF84[box];
    *x = b->x;
    *y = b->y;
    *z = b->z;
    rect[0] = b->l;
    rect[1] = b->t;
    rect[2] = b->r;
    rect[3] = b->b;
    return (b->flags & 4) != 0;
}
s32 boss2_arc_x(void) { return s_boss2.h.x; }
s32 boss2_palette(int model) { return model >= 0 && model < 11 ? s_pal[model] : -1; }

/* state names for the test log */
#define N(f) {(BossFn)f, #f}
static const struct { BossFn f; const char *n; } s_names[] = {
    N(Boss2_State_WaitForStart), N(Boss2_State_IdleCountdown), N(Boss2_State_IdleFinish),
    N(Boss2_State_IdleAfterRaise), N(Boss2_State_IdleAfterRaiseWaitLoop), N(Boss2_State_IdleAfterRaiseCountdown),
    N(Boss2_State_IdleAfterRaiseFinish), N(Boss2_State_IdleBeforeAttack3), N(Boss2_State_IdleBeforeAttack3WaitLoop),
    N(Boss2_State_IdleBeforeAttack3Countdown), N(Boss2_State_IdleBeforeAttack3Finish), N(Boss2_State_DashTurn),
    N(Boss2_State_DashStart), N(Boss2_State_Dash), N(Boss2_State_DashHop), N(Boss2_State_DashBrake),
    N(Boss2_State_DashStop), N(Boss2_State_JumpRise), N(Boss2_State_JumpAirborne), N(Boss2_State_JumpStartFall),
    N(Boss2_State_JumpFall), N(Boss2_State_JumpLandShake), N(Boss2_State_WaitAnimThenSelectMode1),
    N(Boss2_State_WaitAnimThenSelectMode2), N(Boss2_State_Attack2Turn), N(Boss2_State_Attack2EnterTurnMode),
    N(Boss2_State_Attack2StartAim), N(Boss2_State_Attack2Aim), N(Boss2_State_Attack2AimEnd),
    N(Boss2_State_Attack2Charge), N(Boss2_State_Attack2Strike), N(Boss2_State_Attack2Recover),
    N(Boss2_State_Attack2End), N(Boss2_State_Attack2Hurt), N(Boss2_State_Attack3Start), N(Boss2_State_Attack3Suck),
    N(Boss2_State_Attack3Hold), N(Boss2_State_Attack3Close), N(Boss2_State_Attack3Catch),
    N(Boss2_State_Attack3Recover), N(Boss2_State_Attack3Hurt), N(Boss2_State_Attack3End), N(Boss2_State_Defeat),
    N(Boss2_State_DefeatFinish),
};
#undef N
static const struct { Boss2WaterFn f; const char *n; } s_wnames[] = {
    {Boss2Water_State_LowerShake, "LowerShake"}, {Boss2Water_State_LowerWait, "LowerWait"},
    {Boss2Water_State_Lower, "Lower"},           {Boss2Water_State_LowerEndShake, "LowerEndShake"},
    {Boss2Water_State_RaiseWaitAnim, "RaiseWaitAnim"}, {Boss2Water_State_Raise, "Raise"},
    {Boss2Water_State_RaiseEnd, "RaiseEnd"},
};

const char *boss2_state_name(int part) {
    unsigned i;
    if (part == 0) {
        for (i = 0; i < sizeof(s_names) / sizeof(s_names[0]); i++)
            if (s_names[i].f == s_boss2.func)
                return s_names[i].n + 6;
        return "?";
    }
    if (part == 1) {
        if (!s_water.alive || s_water.func == NULL)
            return "-";
        for (i = 0; i < sizeof(s_wnames) / sizeof(s_wnames[0]); i++)
            if (s_wnames[i].f == s_water.func)
                return s_wnames[i].n;
        return "?";
    }
    return gBoss2WaterLightTask == Boss2WaterLight_Main_ApplyAltColors   ? "Light_ApplyAlt"
           : gBoss2WaterLightTask == Boss2WaterLight_Main_RestoreColors ? "Light_Restore"
                                                                        : "-";
}

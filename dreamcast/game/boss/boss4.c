/*
 * Boss4 (Night Carnival act 3, stage z43): native translation of
 * src/game/Zone/Night_Carnival/Boss/ (Boss4_*, Boss4PartCE..D2_*, BossStage4_*)
 * and of the two Boss4 functions kept as ARM listings in src/asm_fallback/
 * (Boss4_State_WaitForStart, Boss4_SubState_SlamStopSpin).
 *
 * The boss is a seesaw that spins round the arena circle (radius 0x128fb8,
 * circumference 0x749fed fx32 = 1865 px): the rod model (bs4_rod) turns by
 * unkBB8 and tilts by unkBBC; the body (bs4_body, part 0xce) hangs at the
 * "rod_body" node, the arm (bs4_arm, part 0xcf) at "rod_arm".
 *
 *   Boss4        (task 0x500, map object 0x3c)  rod, arena, electric arm (bs4_ele), director
 *   Boss4PartCE  (0x501, MapObject_Spawn(0xce))  body: damage box (a jumping player hurts it)
 *   Boss4PartCF  (0x502, 0xcf)  arm: clash box, or harmful while electrified / slamming
 *   Boss4PartD0  (0x503, 0xd0)  electric shot from the arm gun (eff_ele_atk / eff_ele_stay / eff_ele_flash)
 *   Boss4PartD1  (0x504, 0xd1)  enemy dropped from the body exit (bs4_enemy): chases, bounce on it
 *   Boss4PartD2  (0x505, 0xd2)  shock waves along the arena after a slam (eff_ele_wave / eff_wave_disap)
 *
 * Every function keeps its original name and body.  The per-file Unk structs are
 * merged with the original offsets in the field names; the models of Boss4 are
 * unk228[7] (0x138 each): [0] base @0x228, [1] body @0x360, [2] arm @0x498,
 * [3] ele @0x5D0, [4] rod @0x708 (flags[0] = unk814, speeds[0] = unk820),
 * [5] enemy @0x840, [6] sky @0x978.  DS services are replaced as in boss3.c:
 *   - node callbacks (Rods / ArmGun / BodyExit + Boss4_CaptureNodeMatrix) ->
 *     boss_model_joint_world after the pose, same fx32 scale (MTX_Scale43 x)
 *   - palette swaps (Boss4_LoadModelPalettes*) -> selection state
 *   - material / pattern / SRT animations (bs4.nsbma/.nsbtp, eff_*.nsbma) -> timing-only
 *     animation objects (frame counts in bossdata.bin)
 *   - the player's hit boxes (gPlayers->unk2AA / unk2D6) -> g_boss_hooks.player_box_flags
 */
#include <stdio.h>
#include <string.h>
#include "boss_internal.h"

void rect_set_box2d(ObjBox *b, s16 l, s16 t, s16 r, s16 bb); /* ObjRect_SetBox2D */
void obj_player_on_hurt(ObjBox *body, ObjBox *attacker);     /* Player_OnHurt */

typedef struct Boss4 Boss4;
typedef struct Boss4Part Boss4Part;
typedef struct Boss4PartD0 Boss4PartD0;
typedef struct Boss4PartD1 Boss4PartD1;
typedef struct Boss4PartD2 Boss4PartD2;
typedef void (*Boss4SubFn)(Boss4 *s);

struct Boss4 {                /* 0xBF4 bytes (Boss4_Create) */
    DcPlayer h;
    BossFn func;              /* @0x68 */
    BossFn unk78;             /* @0x78 collide (none) */
    ObjBox unk134, unk160, unk18C; /* MapObject_Init boxes (no owner) */
    Boss4Part *unk20C;        /* @0x20C body part (0xce) */
    Boss4Part *unk210;        /* @0x210 arm part (0xcf) */
    s32 unk214;               /* @0x214 attack pattern column */
    s32 unk218;               /* @0x218 frame counter */
    u32 unk21C;               /* @0x21C bit0 hit at least once, bit1 flash palette shown */
    Boss4SubFn unk220;        /* @0x220 sub-state */
    s32 unk224;               /* @0x224 sub-state timer */
    BossModel unk228[7];      /* @0x228 base, body, arm, ele, rod, enemy, sky */
    const void *unkAB0[17];   /* @0xAB0 archive files (see Boss4_Create) */
    u32 unkAF4;               /* @0xAF4 node matrices captured this frame */
    fx32 unkAF8[12];          /* @0xAF8 "rod_body" node (MtxFx43, scale removed) */
    fx32 unkB28[12];          /* @0xB28 "rod_arm" node */
    fx32 unkB58[12];          /* @0xB58 "arm_gun" node (unkB7C/B80/B84 = translation) */
    fx32 unkB88[12];          /* @0xB88 "body_exit" node (unkBAC/BB0/BB4) */
    s32 unkBB8;               /* @0xBB8 spin angle */
    s32 unkBBC;               /* @0xBBC tilt angle */
    s32 unkBC0;               /* @0xBC0 spin speed */
    s32 unkBC4;               /* @0xBC4 tilt speed */
    s32 unkBC8, unkBCC;       /* @0xBC8 tilt limits / target */
    s32 unkBD0;               /* @0xBD0 arm electrified (ele model shown) */
    s32 unkBD4;               /* @0xBD4 electric cycle timer */
    s32 unkBD8;               /* @0xBD8 electric warning */
    s32 unkBDC;               /* @0xBDC enemies alive */
    s32 unkBE0;               /* @0xBE0 shot burst running */
    s32 unkBE4;               /* @0xBE4 shots left */
    s32 unkBE8;               /* @0xBE8 attack timer */
    s32 unkBEC;               /* @0xBEC fast slam */
    s32 unkBF0;               /* @0xBF0 slam coming down (arm box crushes) */
    int alive;
};

struct Boss4Part {            /* 0x214 bytes (Boss4PartCE_Create / Boss4PartCF_Create) */
    DcPlayer h;
    BossFn func;              /* @0x68 (NULL) */
    BossFn unk78;             /* @0x78 collide */
    ObjBox coll0, coll1, coll2; /* @0x134 / @0x160 / @0x18C (coll2: wrapped copy) */
    BossModel *unk20C;        /* @0x20C the Boss4 model it draws */
    int alive;
};

struct Boss4PartD0 {          /* Boss4PartD0_Create */
    DcPlayer h;
    BossFn func;              /* @0x68 = Boss4PartD0_State_Update */
    BossFn unk78;
    ObjBox unk134, unk160, unk18C; /* unk18C: wrapped copy of unk160 */
    BossModel unk20C;         /* @0x20C shot (scale y unk228, trans unk254/258/25C) */
    BossModel unk344;         /* @0x344 flash (trans unk38C, flags[0] unk450) */
    s32 unk47C;               /* @0x47C lifetime */
    s32 unk480;               /* @0x480 flash shown */
    s32 unk484;               /* @0x484 shot length */
    void (*unk488)(Boss4PartD0 *s); /* @0x488 sub-state */
    s32 unk48C;               /* @0x48C sub-state timer */
    int alive;
};

struct Boss4PartD1 {          /* Boss4PartD1_Create */
    DcPlayer h;
    BossFn func;
    BossFn unk78;
    ObjBox unk134, unk160, unk18C;
    BossModel unk20C;         /* @0x20C enemy (rot unk230, trans unk254) */
    s32 unk344;               /* @0x344 lifetime */
    s32 unk348;               /* @0x348 moving right */
    u16 unk34C;               /* @0x34C facing */
    ObjBox unk350, unk37C;    /* @0x350 wrapped copies */
    s32 unk3A8, unk3AC, unk3B0; /* @0x3A8 speed, accel, max */
    s32 unk3B4, unk3B8, unk3BC; /* @0x3B4 fall speed, accel, max */
    void (*unk3C0)(Boss4PartD1 *s); /* @0x3C0 sub-state */
    s32 unk3C4;               /* @0x3C4 sub-state timer */
    int alive;
};

struct Boss4PartD2 {          /* Boss4PartD2_Create */
    DcPlayer h;
    BossFn func;
    BossFn unk78;
    ObjBox unk134, unk160, unk18C;
    BossModel unk20C;         /* @0x20C wave (drawn twice) */
    BossModel unk344;         /* @0x344 vanish */
    s32 unk47C;               /* @0x47C lifetime */
    ObjBox unk480, unk4AC;    /* @0x480 wrapped copies */
    s32 unk4D8;               /* @0x4D8 waves met: vanish */
    s32 unk4DC;               /* @0x4DC spread speed */
    s32 unk4E0, unk4E4;       /* @0x4E0 arc positions of the two waves */
    u16 unk4E8, unk4EA, unk4EC; /* @0x4E8 start angle, wave angles */
    void (*unk4F0)(Boss4PartD2 *s); /* @0x4F0 sub-state */
    s32 unk4F4;
    dcm_mtx_t wave2;          /* native: model matrix of the second Model_Draw of unk20C */
    int alive;
};

#define B4_D0 8
#define B4_D1 2
#define B4_D2 4
static Boss4 s_boss4;
static Boss4Part s_ce, s_cf;
static Boss4PartD0 s_d0[B4_D0];
static Boss4PartD1 s_d1[B4_D1];
static Boss4PartD2 s_d2[B4_D2];
static Boss4 *gBoss4;

/* BossStage4 resources: data_ov000_02369b30[7] = bs4_base, bs4_body, bs4_arm, bs4_ele, bs4_rod,
 * bs4_enemy, bs4_sky (data_02074538); archive files of z43_act_lz7.narc */
static BossModelRes data_ov000_02369b30[7];
static BossAnimRes s_bs4_ca, s_bs4_ma, s_bs4_tp;           /* files 8, 9, 10 */
static BossModelRes s_ele_atk, s_ele_stay, s_ele_flash, s_ele_wave, s_wave_disap; /* 0x51 0x52 0x57 0x5a 0x5c */
static BossAnimRes s_stay_ma, s_flash_ca, s_flash_ma, s_wave_ca, s_disap_ca, s_disap_ma;
static int s_joint[4];        /* rod_arm, rod_body (bs4_rod), arm_gun (bs4_arm), body_exit (bs4_body) */
static int s_cb_off[3];       /* NNS_G3dRenderObjResetCallBack on the body / arm / rod (defeat) */
static s32 s_pal[7];          /* native: Boss_LoadModelPalettes state (first palette index per model) */
static int s_pal_flash[7];

#define data_ov000_02363504 BOSS_DATA(0x02363504, const u16 *)
#define data_ov000_02363508 BOSS_DATA(0x02363508, const u16 *)
#define data_ov000_0236350c BOSS_DATA(0x0236350c, const u16 *)
#define data_ov000_02363510 BOSS_DATA(0x02363510, const u16 *)
#define data_ov000_02363514 BOSS_DATA(0x02363514, const u16 *)
#define data_ov000_02363518 BOSS_DATA(0x02363518, const u16(*)[2])
#define data_ov000_02363520 BOSS_DATA(0x02363520, const u16(*)[2])
#define data_ov000_02363528 BOSS_DATA(0x02363528, const u16(*)[2])
#define data_ov000_02363530 BOSS_DATA(0x02363530, const u16(*)[2])
#define data_ov000_02363538 BOSS_DATA(0x02363538, const s32 *)
#define data_ov000_02363540 BOSS_DATA(0x02363540, const u16 *)
#define data_ov000_02363548 BOSS_DATA(0x02363548, const u8 *)
#define data_ov000_02363550 BOSS_DATA(0x02363550, const u8 *)
#define data_ov000_02363558 BOSS_DATA(0x02363558, const u16 *)
#define data_ov000_02363564 BOSS_DATA(0x02363564, const u16 *)
#define data_ov000_02363570 BOSS_DATA(0x02363570, const s16 *)
#define data_ov000_02363580 BOSS_DATA(0x02363580, const s16 *)
#define data_ov000_02363590 BOSS_DATA(0x02363590, const char *)
#define data_ov000_023635a0 BOSS_DATA(0x023635a0, const s16 *)
#define data_ov000_023635b0 BOSS_DATA(0x023635b0, const char *)
#define data_ov000_023635c0 BOSS_DATA(0x023635c0, const s16 *)
#define data_ov000_023635d0 BOSS_DATA(0x023635d0, const u16(*)[4])
#define data_ov000_023635e0 BOSS_DATA(0x023635e0, const u16(*)[4])
#define data_ov000_023635f0 BOSS_DATA(0x023635f0, const char *)
#define data_ov000_02363600 BOSS_DATA(0x02363600, const u16(*)[4])
#define data_ov000_02363610 BOSS_DATA(0x02363610, const char *)
#define data_ov000_02363620 BOSS_DATA(0x02363620, const u16(*)[6])
#define data_ov000_02363638 BOSS_DATA(0x02363638, const u16(*)[6])
#define data_ov000_02363650 BOSS_DATA(0x02363650, const u16(*)[6])
#define data_ov000_02363668 BOSS_DATA(0x02363668, const float *)
#define data_ov000_02363688 BOSS_DATA(0x02363688, const s8(*)[4])
#define data_ov000_023636a8 BOSS_DATA(0x023636a8, const s16(*)[8][2])
#define data_ov000_023655e8 BOSS_DATA(0x023655e8, const u32 *)
#define data_ov000_023655ec BOSS_DATA(0x023655ec, const u32 *)
#define data_ov000_023655f0 BOSS_DATA(0x023655f0, const u32 *)
#define data_ov000_023655f8 BOSS_DATA(0x023655f8, const u32 *)
#define data_ov000_02365700 BOSS_DATA(0x02365700, const u32 *)
#define data_ov000_02365710 BOSS_DATA(0x02365710, const u32 *)

#define CAM data_ov000_0236a444
#define gGameState (g_boss_game.unk0)
#define AB0_MDL(i) ((const BossModelRes *)gBoss4->unkAB0[i])
#define AB0_ANM(i) ((const BossAnimRes *)gBoss4->unkAB0[i])
/* VecFx32 views of the node matrix translations */
#define unkB7C unkB58[9]
#define unkB84 unkB58[11]

static inline s32 GetIdx(void) {
    dcp_bool b = DCP_TRUE;
    if (g_dcp_world.mode != 2 && g_dcp_world.mode != 3)
        b = DCP_FALSE;
    if (b)
        return 1;
    return g_boss_game.unk14;
}
static inline fx32 FX_Inv(fx32 x) { return FX_Div(0x1000, x); }

static void mtx_to_fx43(const dcm_mtx_t *m, fx32 *out) {
    int r, c;
    for (r = 0; r < 4; r++)
        for (c = 0; c < 3; c++) {
            float v = m->m[r][c] * 4096.0f;
            out[r * 3 + c] = (fx32)(v >= 0 ? v + 0.5f : v - 0.5f);
        }
}
/* model->unk24 = MtxFx43 (rot + trans) */
static void model_set_mtx43(BossModel *m, const fx32 *v) {
    int i;
    for (i = 0; i < 9; i++)
        m->rot.m[i] = v[i];
    for (i = 0; i < 3; i++)
        m->trans[i] = v[9 + i];
}

static void Player_State_Blank(DcPlayer *p) { (void)p; }
static void Player_Action_Blank(DcPlayer *s) {
    dcp_player_init_state(s);
    dcp_reset_gimmick_state(s);
    s->state = Player_State_Blank;
}
/* Player_Action_AttackBounceVel (copied; the player module lacks it) */
static void Player_Action_AttackBounceVel(DcPlayer *w, s32 v) {
    if (w->move_flags & 0x10) {
        s16 vx = w->vel_x;
        s16 g = w->gnd_vel;
        dcp_player_init_state(w);
        w->disp_flags |= 4;
        w->move_flags |= 0x10;
        w->move_flags |= 0x8000;
        w->state = dcp_state_in_air;
        if (g_dcp_hooks.atkdef) /* ObjRect_SetAtkDefFlags(coll2CC, 0x13, 0) */
            g_dcp_hooks.atkdef(g_dcp_hooks.ctx, w, 1, 0x13, 0);
        w->vel_y = (s16)v;
        w->vel_x = vx;
        w->gnd_vel = g;
        w->pf &= ~0xfu;
        w->pf |= 1;
    }
    w->timer = 0;
    w->trick_input = 0;
    w->charge = 0;
}

/* Boss_LoadModelPalettes: selection state only (pre-decoded textures) */
static void Boss_LoadModelPalettes(int model, const u32 *names, const u16 *idx, const u16 *slots, u16 n) {
    (void)names;
    (void)slots;
    (void)n;
    if (model >= 0 && model < 7 && idx != NULL)
        s_pal[model] = idx[0];
}

#define DECL(f) static void f(void *p);
DECL(Boss4_State_WaitForStart) DECL(Boss4_State_Fight) DECL(Boss4_State_DefeatCamera1)
DECL(Boss4_State_DefeatCamera2) DECL(Boss4_State_DefeatCamera3) DECL(Boss4_State_DefeatFade_ov000_022dd410)
DECL(Boss4_State_DefeatFade_ov000_022dd4d0) DECL(Boss4_State_DefeatFade_ov000_022dd590)
DECL(Boss4_State_DefeatExplode) DECL(Boss4_State_DefeatFinish)
DECL(Boss4PartD0_State_Update) DECL(Boss4PartD1_State_Update) DECL(Boss4PartD2_State_Update)
#undef DECL
static void Boss4_SubState_Spin(Boss4 *s);
static void Boss4_SubState_SpinUp(Boss4 *s);
static void Boss4_SubState_TiltSpin(Boss4 *s);
static void Boss4_SubState_HurtFlash(Boss4 *s);
static void Boss4_SubState_ClashFlash(Boss4 *s);
static void Boss4_SubState_SlamStopSpin(Boss4 *s);
static void Boss4_SubState_SlamWaitAnim(Boss4 *s);
static void Boss4_SubState_SlamTiltUp(Boss4 *s);
static void Boss4_SubState_SlamHold(Boss4 *s);
static void Boss4_SubState_SlamDown(Boss4 *s);
static void Boss4_SubState_SlamShake(Boss4 *s);
static void Boss4_SubState_SlamRecover(Boss4 *s);
static void Boss4_OnHit_Damage(ObjBox *a, ObjBox *h);
static void *MapObject_Spawn4(u16 type);

/* ============================================================ helpers */
/* Boss4_WrapArenaX (copied) */
static void Boss4_WrapArenaX(s32 *p0) {
    s32 t0 = *p0;
    if (t0 < 0x4000) {
        *p0 = t0 + 0x749fe;
        return;
    }
    if (t0 > 0x789fe)
        *p0 = t0 - 0x749fe;
}

/* Boss4_ArenaPosToWorld (copied; p0 = VecFx32 or NULL) */
static void Boss4_ArenaPosToWorld(fx32 *p0, u16 *p1, s32 p2, s32 p3) {
    if (p0 != NULL) {
        BossArena_ArcToCirclePos(0x128fb8, 0x749fed, p2 << 4, &p0[0], &p0[2], p1);
        p0[1] = -p3 << 4;
        return;
    }
    BossArena_ArcToCirclePos(0x128fb8, 0x749fed, p2 << 4, NULL, NULL, p1);
}

/* Boss4_ObjArenaPosToWorld (copied) */
static void Boss4_ObjArenaPosToWorld(fx32 *a, u16 *b, const DcPlayer *s) { Boss4_ArenaPosToWorld(a, b, s->x, s->y); }

/* Boss4_ArenaToStagePos (copied) */
static void Boss4_ArenaToStagePos(const fx32 *p0, DcPlayer *p1) {
    BossArena_CirclePosToArc(0x128fb8, 0x749fed, p0[0], p0[2], &p1->x);
    p1->x = p1->x >> 4;
    p1->y = -p0[1] >> 4;
}

/* Boss4_HandleColliderWrapped (modified: StageObj_HandleCollider = obj_handle_collider) */
static void Boss4_HandleColliderWrapped(ObjBox *h, ObjBox *out) {
    DcPlayer *o = (DcPlayer *)h->owner;
    *out = *h;
    obj_handle_collider(o, h->owner, OBJ_OWNER_OBJECT, out, 0);
    if (out->x < 0x3a4)
        out->x += 0x749;
    else
        out->x -= 0x749;
}

/* Boss4_GetHealthSpinScale (copied; the table holds floats) */
static float Boss4_GetHealthSpinScale(void) { return data_ov000_02363668[gBossStatus.unk8 - 1]; }

/* Boss4_SetSpinSpeed (copied: _ffix(_fmul(_fflt(p), scale))) */
static void Boss4_SetSpinSpeed(Boss4 *s, s32 p) { s->unkBC0 = (s32)((float)p * Boss4_GetHealthSpinScale()); }

/* Boss4_RandomBitInMask (copied) */
static dcp_bool Boss4_RandomBitInMask(u32 mask, s32 n) {
    if (mask & (1u << (Rand() % n)))
        return DCP_TRUE;
    return DCP_FALSE;
}

/* Boss4_IsTiltPastLimits (copied) */
static dcp_bool Boss4_IsTiltPastLimits(Boss4 *s) {
    if (s->unkBBC >= s->unkBC8 && s->unkBC4 > 0)
        return DCP_TRUE;
    if (s->unkBBC <= s->unkBCC && s->unkBC4 < 0)
        return DCP_TRUE;
    return DCP_FALSE;
}

/* Boss4_StepTiltToward (copied) */
static void Boss4_StepTiltToward(Boss4 *s, s32 t) {
    if (s->unkBBC > t) {
        s->unkBBC -= s->unkBC4;
        if (s->unkBBC <= t) {
            s->unkBBC = t;
            s->unkBC4 = 0;
        }
    } else {
        s->unkBBC += s->unkBC4;
        if (s->unkBBC >= t) {
            s->unkBBC = t;
            s->unkBC4 = 0;
        }
    }
}

/* Boss4_LoadModelPalettes1..5, 1Clash, 2Warn (modified: palette selection state) */
static void Boss4_LoadModelPalettes1(dcp_bool f) {
    s32 k = gGameState;
    const u16 *t = f ? data_ov000_02363638[k] : data_ov000_02363620[k];
    Boss_LoadModelPalettes(1, data_ov000_02365710, t, data_ov000_02363564, 6);
    s_pal_flash[1] = f;
}
static void Boss4_LoadModelPalettes1Clash(dcp_bool f) {
    s32 k = gGameState;
    const u16 *t = f ? data_ov000_02363650[k] : data_ov000_02363620[k];
    Boss_LoadModelPalettes(1, data_ov000_023655ec, t, data_ov000_02363558, 1);
    s_pal_flash[1] = f ? 2 : 0;
}
static void Boss4_LoadModelPalettes2(dcp_bool flag) {
    s32 i = gGameState;
    const u16 *p = flag ? data_ov000_023635e0[i] : data_ov000_023635d0[i];
    Boss_LoadModelPalettes(2, data_ov000_02365700, p, data_ov000_02363540, 4);
    s_pal_flash[2] = flag;
}
static void Boss4_LoadModelPalettes2Warn(dcp_bool flag) {
    s32 i = gGameState;
    const u16 *p = flag ? data_ov000_02363600[i] : data_ov000_023635d0[i];
    Boss_LoadModelPalettes(2, data_ov000_02365700, p, data_ov000_02363540, 4);
    s_pal_flash[2] = flag ? 2 : 0;
}
static void Boss4_LoadModelPalettes3(dcp_bool flag) {
    s32 i = gGameState;
    const u16 *p = flag ? data_ov000_02363530[i] : data_ov000_02363518[i];
    Boss_LoadModelPalettes(3, data_ov000_023655f8, p, data_ov000_02363508, 2);
    s_pal_flash[3] = flag;
}
static void Boss4_LoadModelPalettes4(dcp_bool flag) {
    s32 i = gGameState;
    const u16 *p = flag ? data_ov000_02363528[i] : data_ov000_02363520[i];
    Boss_LoadModelPalettes(4, data_ov000_023655f0, p, data_ov000_02363510, 2);
    s_pal_flash[4] = flag;
}
static void Boss4_LoadModelPalettes5(dcp_bool flag) {
    s32 i = gGameState;
    const u16 *p = flag ? &data_ov000_02363514[i] : &data_ov000_0236350c[i];
    Boss_LoadModelPalettes(5, data_ov000_023655e8, p, data_ov000_02363504, 1);
    s_pal_flash[5] = flag;
}

/* ============================================================ camera */
/* Boss4_ResetCamera (copied; c = camera + 0x48) */
static void Boss4_ResetCamera(void) {
    CAM.unkDC = 2;
    CAM.unkC = 0x1a66000;
    CAM.unk14 = FX_Div(0x1000, 0x34cc);
    CAM.unk4 = 0x1555;
    CAM.t[0].ox = CAM.t[0].oy = CAM.t[0].oz = NULL;          /* c->unk0..8 */
    CAM.t[0].offx = CAM.t[0].offy = CAM.t[0].offz = 0;       /* c->unkC..14 */
    CAM.t[0].lerp = 0x200;                                   /* c->unk30 */
    CAM.t[0].accel = 0;                                      /* c->unk38 */
    CAM.t[1].ox = CAM.t[1].oy = CAM.t[1].oz = NULL;          /* c->unk3C..44 */
    CAM.t[1].tgt.x = 0;                                      /* c->unk54 */
    CAM.t[1].tgt.y = -0x200000;
    CAM.t[1].tgt.z = 0;
    CAM.t[1].offx = 0;                                       /* c->unk48 */
    CAM.t[1].offy = 0x28000;
    CAM.t[1].offz = 0;
    CAM.t[1].lerp = 0x200;                                   /* c->unk6C */
    CAM.t[1].accel = 0;                                      /* c->unk74 */
    CAM.unkC0 = CAM.unkC4 = 0x1befb8;                        /* c->unk78 / unk7C */
    CAM.unkC8 = 0x80;                                        /* c->unk80 */
    CAM.unkCC = CAM.unkD0 = 0x50000;                         /* c->unk84 / unk88 */
    CAM.unkD4 = 0x80;                                        /* c->unk8C */
    CAM.unkD6 = CAM.unkD8 = 0;                               /* c->unk8E / unk90 */
    CAM.unkDA = 0xccc;                                       /* c->unk92 */
}

/* Boss4_SetCameraDefaultDistance (copied; c->unk80 is written as a u16) */
static void Boss4_SetCameraDefaultDistance(void) {
    CAM.unkCC = 0x50000;                                     /* c->unk84 */
    CAM.unkC0 = 0x1befb8;                                    /* c->unk78 */
    CAM.unkC8 = (CAM.unkC8 & ~0xffff) | 0x80;                /* c->unk80 (u16) */
    CAM.unkD4 = 0x80;                                        /* c->unk8C */
}

/* Boss4_SetCameraZoomOut (copied) */
static void Boss4_SetCameraZoomOut(void) {
    CAM.unkCC = 0xdc000;
    CAM.unkC0 = 0x1dcfb8;
    CAM.unkC8 = (CAM.unkC8 & ~0xffff) | 0x400;
    CAM.unkD4 = 0x400;
}

/* ============================================================ electric arm */
/* Boss4_ActivateAuxModel (copied) */
static void Boss4_ActivateAuxModel(Boss4 *s) {
    if (s->unkBD0 == 0) {
        BossModel *a = &s->unk228[3]; /* unk5D0 */
        Sound_PlaySeqArc(0, 0, 0x88);
        Model_SetAnimation(a, 0, AB0_ANM(0), 0);
        Model_SetAnimation(a, 1, AB0_ANM(1), 0); /* unkAB4: bs4.nsbma (timing only) */
        a->flags[0] |= 4;
        a->flags[1] |= 4;
        g_boss_stats.electrified++;
    }
    s->unkBD0 = 1;
    s->unkBD4 = data_ov000_02363570[gBossStatus.unk8 - 1];
    s->unkBD8 = 0;
    Boss4_LoadModelPalettes2Warn(0);
}

/* Boss4_DeactivateAuxModel (copied) */
static void Boss4_DeactivateAuxModel(Boss4 *s) {
    u16 r;
    s->unkBD0 = 0;
    r = Rand();
    s->unkBD4 = data_ov000_023636a8[GetIdx()][gBossStatus.unk8 - 1][r & 1];
    s->unkBD4 -= 0x3c;
}

/* Boss4_StartAuxModelCycle (copied: the ARM code passes the object in r0) */
static void Boss4_StartAuxModelCycle(Boss4 *s) { Boss4_ActivateAuxModel(s); }

/* Boss4_UpdateAuxModelCycle (copied) */
static void Boss4_UpdateAuxModelCycle(Boss4 *s) {
    s->unkBD4--;
    if (s->unkBD8 == 1) {
        if (s->unkBD4 & 6)
            Boss4_LoadModelPalettes2Warn(1);
        else
            Boss4_LoadModelPalettes2Warn(0);
    }
    if (s->unkBD4 > 0)
        return;
    if (s->unkBD8 == 1) {
        Boss4_LoadModelPalettes2Warn(0);
        Boss4_ActivateAuxModel(s);
        return;
    }
    if (s->unkBD0 == 1) {
        if (Boss4_RandomBitInMask((s->unk21C & 1) ? 0xff : data_ov000_02363550[gBossStatus.unk8 - 1], 8)) {
            Boss4_DeactivateAuxModel(s);
            return;
        }
        Boss4_ActivateAuxModel(s);
        return;
    }
    s->unkBD8 = 1;
    s->unkBD4 = 0x3c;
}

/* ============================================================ actions */
/* Boss4_Action_StartSpin (copied) */
static void Boss4_Action_StartSpin(Boss4 *p0) {
    gBossStatus.unk4 = 0x14;
    p0->unk220 = Boss4_SubState_Spin;
    p0->unk224 = 0;
    p0->unkBC4 = 0x10;
    p0->unkBC8 = -0x600;
    p0->unkBCC = -0x600;
}

/* Boss4_Action_StartSlam (copied) */
static void Boss4_Action_StartSlam(Boss4 *p0) {
    gBossStatus.unk4 = 0x18;
    p0->unk220 = Boss4_SubState_SlamStopSpin;
    p0->unk224 = 0;
    Boss4_SetSpinSpeed(p0, 0x60);
    p0->unkBC4 = 0x80;
    p0->unkBC8 = -0x600;
    p0->unkBCC = -0x600;
    p0->unkBEC = Rand() & 1;
    g_boss_stats.attacks++;
    g_boss_stats.slams++;
    boss_log("attack_slam", p0->unkBEC, p0->unk210->h.x >> 8);
    if ((p0->unk21C & 2) == 0)
        return;
    Boss4_LoadModelPalettes1(0);
    Boss4_LoadModelPalettes2(0);
    Boss4_LoadModelPalettes4(0);
    Boss4_LoadModelPalettes3(0);
}

/* Boss4_Action_StartTiltSpin (copied) */
static void Boss4_Action_StartTiltSpin(Boss4 *p0) {
    gBossStatus.unk4 = 0x19;
    p0->unk220 = Boss4_SubState_TiltSpin;
    p0->unk224 = 0;
    Boss4_SetSpinSpeed(p0, 0x60);
    p0->unkBC4 = 0x20;
    p0->unkBC8 = -0xc00;
    p0->unkBCC = -0xc00;
    g_boss_stats.attacks++;
    g_boss_stats.tilt_spins++;
    boss_log("attack_tiltspin", 0, 0);
    if ((p0->unk21C & 2) == 0)
        return;
    Boss4_LoadModelPalettes1(0);
    Boss4_LoadModelPalettes2(0);
    Boss4_LoadModelPalettes4(0);
    Boss4_LoadModelPalettes3(0);
}

/* Boss4_Action_Clash (copied) */
static void Boss4_Action_Clash(Boss4 *s) {
    gBossStatus.unk4 = 0x15;
    s->unk220 = Boss4_SubState_ClashFlash;
    s->unk224 = 0;
    Boss4_SetSpinSpeed(s, 0x20);
    s->unkBC4 = 0x180;
    s->unkBC8 = 0xd80;
    s->unkBCC = 0x900;
    Sound_PlaySeqArc(0, 0, 0x8d);
    g_boss_stats.clashes++;
    boss_log("clash", 0, 0);
}

/* Boss4_Action_Hurt (copied) */
static void Boss4_Action_Hurt(Boss4 *s) {
    gBossStatus.unk4 = 0x16;
    s->unk220 = Boss4_SubState_HurtFlash;
    s->unk224 = 0;
    Boss4_SetSpinSpeed(s, 0x20);
    s->unkBC4 = 0x180;
    s->unkBC8 = -0x600;
    s->unkBCC = -0xa00;
    Sound_PlaySeqArc(0, 0, 0x7b);
    Sound_PlayVoice(0, 0, data_ov000_02363538[Rand() & 1]);
}

/* Boss4_Action_Defeat (modified: RingManager / music / display via hooks and state; the player's
 * hit boxes (unk2AA / unk2D6) via g_boss_hooks.player_box_flags) */
static void Boss4_Action_Defeat(Boss4 *s) {
    Boss4Part *a = s->unk20C;
    Boss4Part *b = s->unk210;
    s32 i;
    gBossStatus.unk4 = 0x1a;
    s->func = Boss4_State_DefeatCamera1;
    s->unk220 = NULL;
    s->unk224 = 0;
    g_dcp_world.flags &= ~1u; /* gGameState.unk18 &= ~1 */
    ScreenShake_Start(0);
    if (g_boss_hooks.music_stop) /* NNS_SndPlayerStopSeqAll(0) */
        g_boss_hooks.music_stop(g_boss_hooks.ctx);
    Boss4_DeactivateAuxModel(s);
    for (i = 0; i < 7; i++)
        s->unk228[i].flag |= 2; /* unk214[i].unk18: animations frozen */
    Boss4_LoadModelPalettes1(0);
    Boss4_LoadModelPalettes2(0);
    Boss4_LoadModelPalettes4(0);
    Boss4_LoadModelPalettes3(0);
    if (g_boss_hooks.ring_flags) /* RingManager_SetFlags(RingManager_GetFlags() | 1) */
        g_boss_hooks.ring_flags(g_boss_hooks.ctx, 1);
    g_boss_game.bright = 0x10; /* gDisplayMain.unk48 */
    Sound_PlaySeqArc(0, 0, 0x7c);
    Boss_DrawExcBackground(6);
    Player_Action_Blank(gPlayers);
    dcp_set_anim(gPlayers, 0x30);
    gPlayers->invuln = 0;
    gPlayers->disp_flags &= ~0x20;
    if (g_boss_hooks.player_box_flags) {
        g_boss_hooks.player_box_flags(g_boss_hooks.ctx, gPlayers, 0, 4, 0x1000); /* unk2AA &= ~4; |= 0x1000 */
        g_boss_hooks.player_box_flags(g_boss_hooks.ctx, gPlayers, 1, 4, 0x1000); /* unk2D6 */
    }
    s->unk78 = NULL;
    a->unk78 = NULL;
    b->unk78 = NULL;
    boss_log("defeat", 0, 0);
}

/* Boss4_OnHit_Damage (modified: owner type check -> obj_box_player; NaviMsg / music via hooks) */
static void Boss4_OnHit_Damage(ObjBox *a, ObjBox *h) {
    DcPlayer *p = obj_box_player(h);
    Boss4 *b = gBoss4;
    BossEffect *fx;
    const fx32 *pt = g_boss_game.player_trans; /* p->unk17C / unk180 / unk184 */
    if (p == NULL) /* p->unk0 != 1 */
        return;
    if (p->anim != 0x30)
        return;
    if (a->owner == b->unk20C) {
        gBossStatus.unk8--;
        b->unk21C |= 1;
        g_boss_stats.hits++;
        boss_log("hit", gBossStatus.unk8, a->x);
        if (gBossStatus.unk8 == 0) {
            Boss4_Action_Defeat(b);
            return;
        }
        Boss4_Action_Hurt(b);
        if (gBossStatus.unk8 == 2) {
            NaviMsg_SetFaceAnim(0xb);
            NaviMsg_ShowMessage(0x12, 0xf0);
            if (g_boss_hooks.music) /* Sound_PlaySeq(0, 0xf) */
                g_boss_hooks.music(g_boss_hooks.ctx, 0xf);
        } else {
            NaviMsg_SetFaceAnim(0xb);
            NaviMsg_ShowMessage(0x10, 0xf0);
        }
        BossEffect_CreateHit(1, pt[0] >> 4, -pt[1] >> 4, pt[2] >> 4);
    } else {
        Boss4_Action_Clash(b);
        fx = BossEffect_CreateClash(1, pt[0] >> 4, -pt[1] >> 4, pt[2] >> 4);
        fx->unk134.cb[0] = 0x13; /* unk13E */
        fx->unk134.cb[1] = 3;
        fx->unk134.cb[2] = 0;
    }
    obj_player_attack_bounce(p);
}

/* Boss4_SpawnPartD0 (copied) */
static void Boss4_SpawnPartD0(Boss4 *p0) {
    p0->unkBE4 = p0->unkBE4 - 1;
    p0->unkBE0 = 1;
    p0->unkBE8 = data_ov000_023635c0[gBossStatus.unk8 - 1];
    MapObject_Spawn4(0xd0);
}

/* Boss4_StartAttack (copied) */
static void Boss4_StartAttack(Boss4 *s, s32 n) {
    switch (n) {
    case 0:
        s->unkBE4 = data_ov000_023635a0[gBossStatus.unk8 - 1];
        g_boss_stats.attacks++;
        g_boss_stats.shot_bursts++;
        boss_log("attack_shots", s->unkBE4, 0);
        Boss4_SpawnPartD0(s);
        break;
    case 1:
        Boss4_Action_StartSlam(s);
        break;
    case 2:
        Boss4_Action_StartTiltSpin(s);
        break;
    }
}

/* Boss4_UpdateAttackTimer (copied) */
static void Boss4_UpdateAttackTimer(Boss4 *s) {
    s32 k;
    if (gBossStatus.unk4 == 0x16)
        return;
    s->unkBE8--;
    if (s->unkBE8 > 0)
        return;
    if (s->unkBE0 == 1) {
        if (s->unkBE4 > 0) {
            Boss4_SpawnPartD0(s);
            return;
        }
        s->unkBE0 = 0;
        s->unkBE8 = data_ov000_02363580[gBossStatus.unk8 - 1];
        return;
    }
    if (Boss4_RandomBitInMask(data_ov000_02363548[gBossStatus.unk8 - 1], 8)) {
        if (gBossStatus.unk4 == 0x15) {
            k = 0;
        } else {
            k = data_ov000_02363688[gBossStatus.unk8 - 1][s->unk214];
            s->unk214++;
            s->unk214 %= 4;
        }
        Boss4_StartAttack(s, k);
        return;
    }
    s->unkBE8 = data_ov000_02363580[gBossStatus.unk8 - 1];
}

/* ============================================================ sub-states */
/* Boss4_SubState_Spin (copied) */
static void Boss4_SubState_Spin(Boss4 *s) {
    s32 lim = (s32)(96.0f * Boss4_GetHealthSpinScale());
    if (s->unkBC0 > lim) {
        s->unkBC0 -= 8;
        if (s->unkBC0 < lim)
            s->unkBC0 = lim;
    } else {
        s->unkBC0 += 8;
        if (s->unkBC0 > lim)
            s->unkBC0 = lim;
    }
    s->unkBB8 += s->unkBC0;
    Boss4_StepTiltToward(s, s->unkBC8);
}

/* Boss4_SubState_SpinUp (copied) */
static void Boss4_SubState_SpinUp(Boss4 *p0) {
    s32 t1 = (s32)(96.0f * Boss4_GetHealthSpinScale());
    p0->unkBC0 = p0->unkBC0 + 8;
    if (p0->unkBC0 < t1) {
        p0->unkBB8 = p0->unkBB8 + p0->unkBC0;
        return;
    }
    p0->unkBC0 = t1;
    Boss4_Action_StartSpin(p0);
}

/* Boss4_SubState_TiltSpin (copied) */
static void Boss4_SubState_TiltSpin(Boss4 *p) {
    p->unkBB8 += p->unkBC0;
    Boss4_StepTiltToward(p, p->unkBC8);
    if (p->unk224 <= 0xb4)
        return;
    Boss4_Action_StartSpin(p);
}

/* Boss4_SubState_HurtFlash (copied) */
static void Boss4_SubState_HurtFlash(Boss4 *s) {
    s32 d;
    if (s->unk224 > 0x3c) {
        Boss4_Action_StartSpin(s);
        s->unk21C &= ~2;
        Boss4_LoadModelPalettes1(0);
        Boss4_LoadModelPalettes2(0);
        Boss4_LoadModelPalettes4(0);
        Boss4_LoadModelPalettes3(0);
        return;
    }
    s->unkBB8 += s->unkBC0;
    s->unkBBC += s->unkBC4;
    if (s->unk224 & 2) {
        s->unk21C &= ~2;
        Boss4_LoadModelPalettes1(0);
        Boss4_LoadModelPalettes2(0);
        Boss4_LoadModelPalettes4(0);
        Boss4_LoadModelPalettes3(0);
    } else {
        s->unk21C |= 2;
        Boss4_LoadModelPalettes1(1);
        Boss4_LoadModelPalettes2(1);
        Boss4_LoadModelPalettes4(1);
        Boss4_LoadModelPalettes3(1);
    }
    if (!Boss4_IsTiltPastLimits(s))
        return;
    d = s->unkBCC - s->unkBC8;
    if (d < -0x40)
        d = -0x40;
    if (d == 0) {
        s->unkBC8 = s->unkBCC;
        s->unkBC4 = 0;
        return;
    }
    s->unkBC8 += d;
    s->unkBC4 *= -1;
}

/* Boss4_SubState_ClashFlash (copied) */
static void Boss4_SubState_ClashFlash(Boss4 *s) {
    s32 lim;
    s32 d;
    lim = (s->unk21C & 1) ? 0x2d0 : 0x1e0;
    if (s->unk224 > lim) {
        Boss4_Action_StartSpin(s);
        s->unk21C &= ~2;
        Boss4_LoadModelPalettes1Clash(0);
        return;
    }
    if (s->unk224 == 10 && s->unkBDC < 1)
        MapObject_Spawn4(0xd1);
    s->unkBB8 += s->unkBC0;
    s->unkBBC += s->unkBC4;
    if (s->unk224 & 4) {
        s->unk21C &= ~2;
        Boss4_LoadModelPalettes1Clash(0);
    } else {
        s->unk21C |= 2;
        Boss4_LoadModelPalettes1Clash(1);
    }
    if (!Boss4_IsTiltPastLimits(s))
        return;
    d = s->unkBC8 - s->unkBCC;
    if (d > 0x40)
        d = 0x40;
    if (d == 0) {
        s->unkBCC = s->unkBC8;
        s->unkBC4 = 0;
        return;
    }
    s->unkBCC += d;
    s->unkBC4 *= -1;
}

/* Boss4_SubState_SlamStopSpin (from the ARM listing in src/asm_fallback/) */
static void Boss4_SubState_SlamStopSpin(Boss4 *r4) {
    BossModel *r5;
    if (r4->unk224 == 1)
        Boss4_SetCameraZoomOut();
    r4->unkBC0 = r4->unkBC0 - 8;
    if (r4->unkBC0 < 0)
        r4->unkBC0 = 0;
    r4->unkBB8 = r4->unkBB8 + r4->unkBC0;
    Boss4_StepTiltToward(r4, r4->unkBC8);
    if (r4->unkBC0 != 0)
        return;
    if (r4->unkBC4 != 0)
        return;
    r5 = &r4->unk228[4]; /* r4 + 0x708 */
    Model_SetAnimation(r5, 0, AB0_ANM(0), 4);
    r5->speeds[0] = 0x1000; /* +0x118 */
    r4->unk220 = Boss4_SubState_SlamWaitAnim;
    r4->unk224 = 0;
    Sound_PlayVoice(0, 0, 0xd2);
}

/* Boss4_SubState_SlamWaitAnim (copied; unk814 = rod flags[0]) */
static void Boss4_SubState_SlamWaitAnim(Boss4 *s) {
    if (!(s->unk228[4].flags[0] & 2))
        return;
    s->unk220 = Boss4_SubState_SlamTiltUp;
    s->unk224 = 0;
    s->unkBF0 = 1;
}

/* Boss4_SubState_SlamTiltUp (copied; float easing as on the DS (softfloat single)) */
static void Boss4_SubState_SlamTiltUp(Boss4 *s) {
    s32 n;
    float t;
    if (s->unkBEC != 0)
        n = 0x14;
    else
        n = 0x32;
    t = 1.0f - (float)s->unk224 / (float)n;
    t = 1.0f - t * (t * t);
    s->unkBBC = -0x600;
    s->unkBBC = (s32)((float)s->unkBBC + 8448.0f * t);
    if (s->unkBBC >= 0x1b00) {
        s->unkBBC = 0x1b00;
        s->unk220 = Boss4_SubState_SlamHold;
        s->unk224 = 0;
    }
}

/* Boss4_SubState_SlamHold (copied) */
static void Boss4_SubState_SlamHold(Boss4 *p) {
    s32 lim;
    if (p->unkBEC != 0)
        lim = 0;
    else
        lim = 0x1e;
    if (p->unk224 <= lim)
        return;
    p->unk220 = Boss4_SubState_SlamDown;
    p->unk224 = 0;
    Sound_PlaySeqArc(0, 0, 0x8b);
}

/* Boss4_SubState_SlamDown (copied) */
static void Boss4_SubState_SlamDown(Boss4 *s) {
    s->unkBBC -= 0x700;
    if (s->unkBBC > -0x1300)
        return;
    s->unkBBC = -0x1300;
    s->unk220 = Boss4_SubState_SlamShake;
    s->unk224 = 0;
    MapObject_Spawn4(0xd2);
    BossEffect_CreateImpactSmall(1, s->unkB7C >> 4, 0x20000, s->unkB84 >> 4);
    s->unkBF0 = 0;
    ScreenShake_Start(0xc);
}

/* Boss4_SubState_SlamShake (copied; unk820 = rod speeds[0]) */
static void Boss4_SubState_SlamShake(Boss4 *p0) {
    if (p0->unk224 > 0x6e) {
        p0->unk228[4].speeds[0] = p0->unk228[4].speeds[0] * -1;
        p0->unk220 = Boss4_SubState_SlamRecover;
        p0->unk224 = 0;
        ScreenShake_Start(0);
        CAM.unk3C.x = 0;
        CAM.unk3C.y = 0;
        return;
    }
    CAM.unk3C.x = ScreenShake_GetOffsetX() << 0xd;
    CAM.unk3C.y = ScreenShake_GetOffsetY() << 0xd;
}

/* Boss4_SubState_SlamRecover (copied) */
static void Boss4_SubState_SlamRecover(Boss4 *s) {
    float t;
    t = (float)s->unk224 / 45.0f;
    if (t > 1.0f)
        t = 1.0f;
    t = 1.0f - t;
    t = 1.0f - t * (t * t);
    s->unkBBC = -0x1300;
    s->unkBBC = (s32)((float)s->unkBBC + 3328.0f * t);
    if (s->unk224 > 0x2d) {
        s->unk220 = Boss4_SubState_SpinUp;
        s->unk224 = 0;
        Boss4_SetCameraDefaultDistance();
    }
}

/* ============================================================ draw */
/* Boss4_CaptureNodeMatrix (modified: NNS_G3dGetCurrentMtx -> boss_model_joint_world; the same
 * MTX_Scale43(1/3.3) x MTX_Concat43 in fx32) */
static void Boss4_CaptureNodeMatrix(fx32 *dst, const BossModel *m, int joint, u32 bit) {
    fx32 s = FX_Inv(0x34cc);
    dcm_mtx_t w;
    int i;
    boss_model_joint_world(m, joint, &w);
    mtx_to_fx43(&w, dst);
    gBoss4->unkAF4 |= bit;
    for (i = 0; i < 9; i++) /* rows 0..2 scaled, translation kept */
        dst[i] = (fx32)(((s64)s * dst[i]) >> 12);
}

/* Boss4_NodeCallback_Rods / _ArmGun / _BodyExit (substantially rewritten: per joint after the pose) */
static void Boss4_NodeCallback_Rods(const BossModel *m) {
    Boss4 *b = gBoss4;
    Boss4_CaptureNodeMatrix(b->unkB28, m, s_joint[0], 2); /* "rod_arm" */
    Boss4_CaptureNodeMatrix(b->unkAF8, m, s_joint[1], 1); /* "rod_body" */
}
static void Boss4_NodeCallback_ArmGun(const BossModel *m) { Boss4_CaptureNodeMatrix(gBoss4->unkB58, m, s_joint[2], 4); }
static void Boss4_NodeCallback_BodyExit(const BossModel *m) { Boss4_CaptureNodeMatrix(gBoss4->unkB88, m, s_joint[3], 0x10); }

/* Boss4_DrawSpinTiltModel (modified: Model_Draw -> boss_model_pose + node callback) */
static void Boss4_DrawSpinTiltModel(void) {
    MtxFx33 m1;
    MtxFx33 m2;
    Boss4 *w = gBoss4;
    BossModel *a = &w->unk228[4]; /* unk708 */
    u16 a1 = (u16)w->unkBB8;
    u16 a2 = (u16)w->unkBBC;
    MTX_RotY33(&m1, SIN(a1), COS(a1));
    MTX_RotZ33(&m2, SIN(a2), COS(a2));
    MTX_Concat33(&m2, &m1, &a->rot);
    Model_Animate(a);
    boss_model_pose(a, NULL);
    if (!s_cb_off[2])
        Boss4_NodeCallback_Rods(a);
}

/* Boss4_DrawAuxModel (modified) */
static void Boss4_DrawAuxModel(void) {
    MtxFx33 mtx;
    Boss4 *w = gBoss4;
    BossModel *a = &w->unk228[3]; /* unk5D0 */
    u16 ang;
    model_set_mtx43(a, w->unkB28);
    ang = (u16)-w->unkBBC;
    MTX_RotZ33(&mtx, SIN(ang), COS(ang));
    MTX_Concat33(&mtx, &a->rot, &a->rot);
    Model_Animate(a);
    boss_model_pose(a, NULL);
}

/* Boss4_Draw_Body (modified) */
static void Boss4_Draw_Body(void *p) {
    Boss4 *o = (Boss4 *)p;
    BossModel *cam;
    if (o->h.disp_flags & 0x20)
        return; /* boss4_draw skips the base / rod / ele models while hidden */
    o->unkAF4 = 0;
    Boss4_DrawSpinTiltModel();
    o->unk228[3].posed = 0;
    if (o->unkBD0 != 0)
        Boss4_DrawAuxModel();
    cam = &o->unk228[0];
    Model_Animate(cam);
    boss_model_pose(cam, NULL);
}

/* Boss4_Draw_PlayerOnArena (modified: player model transform -> g_boss_game) */
static void Boss4_Draw_PlayerOnArena(void) {
    u16 ang[4];
    DcPlayer *p;
    s32 st;
    u16 off;
    p = gPlayers;
    if (p->disp_flags & 0x20)
        return;
    Boss4_WrapArenaX(&p->x);
    Boss4_ObjArenaPosToWorld(g_boss_game.player_trans, ang, p); /* unk17C */
    if (p->disp_flags & 1)
        off = 0xc000;
    else
        off = 0x4000;
    MTX_RotY33(&g_boss_game.player_rot, SIN((u16)(off + ang[0])), COS((u16)(off + ang[0]))); /* unk158 */
    g_boss_game.player_trans_valid = 1;
    st = (s32)gBossStatus.unk4;
    if (st == 0x1a)
        return;
    CAM.unkD6 = ang[0];
    if (st == 0x18)
        return;
    if (p->anim == 0x30)
        return;
    Boss4_SetCameraDefaultDistance();
}

/* ============================================================ body / arm parts */
static void Boss4Part_CollideTrampoline(void *p) {
    Boss4Part *o = (Boss4Part *)p;
    if (o->unk78 != NULL)
        o->unk78(o);
}

/* Boss4PartCE_Collide (modified: StageObj_HandleCollider = obj_handle_collider) */
static void Boss4PartCE_Collide(void *p) {
    Boss4 *m = gBoss4;
    Boss4Part *o = m->unk20C;
    ObjBox *c;
    (void)p;
    Boss4_ArenaToStagePos(m->unk228[1].trans, &o->h); /* unk3A8 */
    if (gBossStatus.unk4 == 0x18)
        return;
    c = &o->coll0;
    rect_set_box2d(c, -0x28, 0, 0x50, 0x64);
    c->owner = o;
    c->owner_kind = OBJ_OWNER_OBJECT;
    c->cb = Boss4_OnHit_Damage;
    c->flags |= 4;
    obj_handle_collider(&o->h, o, OBJ_OWNER_OBJECT, c, 0);
    Boss4_HandleColliderWrapped(c, &o->coll2);
    o->coll2.y = c->y;
}

/* Boss4PartCF_Collide (modified: as above) */
static void Boss4PartCF_Collide(void *p) {
    Boss4 *m = gBoss4;
    Boss4Part *o = m->unk210;
    ObjBox *c;
    (void)p;
    Boss4_ArenaToStagePos(m->unk228[2].trans, &o->h); /* unk4E0 */
    if (m->unkBD0 != 0 || gBossStatus.unk4 == 0x18) {
        c = &o->coll1;
        rect_set_box2d(c, -0x4b, -0x28, 0x4b, 0x28);
        c->cb = NULL;
        if (m->unkBF0 != 0)
            c->atk |= 0x20;
        else
            c->atk &= ~0x20;
    } else {
        c = &o->coll0;
        rect_set_box2d(c, -0x32, -0x23, 0x32, 0x23);
        c->cb = Boss4_OnHit_Damage;
    }
    c->owner = o;
    c->owner_kind = OBJ_OWNER_OBJECT;
    c->flags |= 4;
    obj_handle_collider(&o->h, o, OBJ_OWNER_OBJECT, c, 0);
    c->y += 0x64;
    Boss4_HandleColliderWrapped(c, &o->coll2);
    o->coll2.y = c->y;
}

/* Boss4PartCE_Draw (modified: the body takes the "rod_body" node; BodyExit node callback) */
static void Boss4PartCE_Draw(void *p) {
    Boss4Part *s = (Boss4Part *)p;
    BossModel *m = s->unk20C;
    model_set_mtx43(m, gBoss4->unkAF8);
    Model_Animate(m);
    boss_model_pose(m, NULL);
    if (!s_cb_off[0])
        Boss4_NodeCallback_BodyExit(m);
}

/* Boss4PartCF_Draw (modified: the arm takes the "rod_arm" node; ArmGun node callback) */
static void Boss4PartCF_Draw(void *p) {
    Boss4Part *s = (Boss4Part *)p;
    MtxFx33 mtx;
    Boss4 *w = gBoss4;
    BossModel *a = s->unk20C;
    u16 ang;
    model_set_mtx43(a, w->unkB28);
    ang = (u16)-w->unkBBC;
    MTX_RotZ33(&mtx, SIN(ang), COS(ang));
    MTX_Concat33(&mtx, &a->rot, &a->rot);
    Model_Animate(a);
    boss_model_pose(a, NULL);
    if (!s_cb_off[1])
        Boss4_NodeCallback_ArmGun(a);
}

/* MapObject_Init (obj_manager.c obj_mapobject_init) fields used */
static void MapObject_Init_parts(DcPlayer *h, ObjBox *coll, const ObjParam *prm, s32 x, s32 y) {
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
    for (i = 0; i < 3; i++)
        coll[i].mask = 0x12;
    coll[0].cb = NULL; /* MapObject_OnHit_Default (replaced by the callers) */
    rect_set_atkdef(&coll[0], 9, 0xa);
    rect_set_atkdef(&coll[1], 0x13, 0x13);
    rect_set_atkdef(&coll[2], 0x13, 0x13);
    for (i = 0; i < 3; i++)
        coll[i].flags &= ~4;
}

static void part_forget(ObjBox *b, int n) {
    int i;
    for (i = 0; i < n; i++)
        rect_forget(&b[i]);
}

/* Boss4PartCE_Destructor / Boss4PartCF_Destructor (MapObject_Destructor) */
static void Boss4Part_Destructor(void *p) {
    Boss4Part *s = (Boss4Part *)p;
    rect_forget(&s->coll0);
    rect_forget(&s->coll1);
    rect_forget(&s->coll2);
}

/* Boss4PartCE_Create / Boss4PartCF_Create (copied) */
static Boss4Part *Boss4PartCE_Create(const ObjParam *p0, s32 p1, s32 p2) {
    Boss4Part *t2 = &s_ce;
    memset(t2, 0, sizeof(*t2)); /* MI_CpuFill8(.., 0, 0x214) */
    t2->alive = 1;
    MapObject_Init_parts(&t2->h, &t2->coll0, p0, p1, p2);
    t2->func = NULL;
    t2->unk78 = Boss4PartCE_Collide;
    t2->h.obj_flags |= 0x10;
    t2->h.move_flags |= 0xa300;
    t2->unk20C = &gBoss4->unk228[1]; /* t0 + 0x360 */
    return t2;
}
static Boss4Part *Boss4PartCF_Create(const ObjParam *p0, s32 p1, s32 p2) {
    Boss4Part *t2 = &s_cf;
    memset(t2, 0, sizeof(*t2));
    t2->alive = 1;
    MapObject_Init_parts(&t2->h, &t2->coll0, p0, p1, p2);
    t2->func = NULL;
    t2->unk78 = Boss4PartCF_Collide;
    t2->h.obj_flags |= 0x10;
    t2->h.move_flags |= 0xa300;
    t2->unk20C = &gBoss4->unk228[2]; /* t0 + 0x498 */
    return t2;
}

/* ============================================================ D0: electric shot */
static void Boss4PartD0_SubState_DestroyOnAnimEnd(Boss4PartD0 *s);
static void Boss4PartD0_SubState_Launch(Boss4PartD0 *s);

/* Boss4PartD0_StartBurst (copied) */
static void Boss4PartD0_StartBurst(Boss4PartD0 *s) {
    Boss4 *w = gBoss4;
    BossModel *a;
    s->unk488 = Boss4PartD0_SubState_DestroyOnAnimEnd;
    a = &s->unk20C;
    s->unk48C = 0;
    s->unk480 = 1;
    s->h.y = 0x20000;
    a->trans[1] = -0x200000;
    a->scale[1] = 0x34cc;
    Model_SetResource(a, (const BossModelRes *)w->unkAB0[4]); /* unkAC0: eff_ele_stay */
    Model_SetAnimation(a, 1, (const BossAnimRes *)w->unkAB0[8], 0); /* unkAD0: eff_ele_stay.nsbma */
    s->unk344.trans[0] = s->unk20C.trans[0];
    s->unk344.trans[1] = s->unk20C.trans[1];
    s->unk344.trans[2] = s->unk20C.trans[2];
}

/* Boss4PartD0_StartLaunch (copied) */
static void Boss4PartD0_StartLaunch(Boss4PartD0 *s) {
    s->unk488 = Boss4PartD0_SubState_Launch;
    s->unk48C = 0;
    s->h.y = 0x20000;
    s->unk20C.trans[1] = -0x200000; /* unk258 */
}

/* Boss4PartD0_SubState_Launch (copied) */
static void Boss4PartD0_SubState_Launch(Boss4PartD0 *s) {
    float f = s->unk48C / 5.0f;
    if (f >= 1.0f)
        f = 1.0f;
    f = 1.0f - f * (f * f);
    s->unk20C.scale[1] = (s32)(f * s->unk484); /* unk228 */
    if (s->unk48C < 5)
        return;
    Boss4PartD0_StartBurst(s);
}

/* Boss4PartD0_SubState_DestroyOnAnimEnd (copied; unk319 = flags[1]) */
static void Boss4PartD0_SubState_DestroyOnAnimEnd(Boss4PartD0 *s) {
    if (!(s->unk20C.flags[1] & 2))
        return;
    s->h.obj_flags |= 8;
    s->h.obj_flags |= 2;
}

/* Boss4PartD0_State_Update (copied) */
static void Boss4PartD0_State_Update(void *p) {
    Boss4PartD0 *s = (Boss4PartD0 *)p;
    u16 ang;
    if (gBossStatus.unk4 == 0x1a) {
        s->h.obj_flags |= 8;
        s->h.obj_flags |= 2;
        return;
    }
    s->unk47C++;
    s->unk48C++;
    if (s->unk488 != NULL)
        s->unk488(s);
    Boss4_WrapArenaX(&s->h.x);
    Boss4_ObjArenaPosToWorld(s->unk20C.trans, &ang, &s->h); /* unk254 */
    if (s->unk480 != 0) {
        if (s->unk344.flags[0] & 2) /* unk450 */
            s->unk480 = 0;
        else {
            s->unk344.trans[0] = s->unk20C.trans[0]; /* unk38C = unk254 */
            s->unk344.trans[1] = s->unk20C.trans[1];
            s->unk344.trans[2] = s->unk20C.trans[2];
        }
    }
    if (s->unk47C > 0x258) {
        s->h.obj_flags |= 8;
        s->h.obj_flags |= 2;
    }
}

/* Boss4PartD0_Collide (modified) */
static void Boss4PartD0_Collide(void *p) {
    Boss4PartD0 *s = (Boss4PartD0 *)p;
    ObjBox *h = &s->unk160;
    rect_set_box2d(h, -0xb, -0x16, 0xb, 0x16);
    h->owner = s;
    h->owner_kind = OBJ_OWNER_OBJECT;
    h->cb = NULL;
    h->flags |= 4;
    obj_handle_collider(&s->h, s, OBJ_OWNER_OBJECT, h, 0);
    Boss4_HandleColliderWrapped(&s->unk160, &s->unk18C);
}

/* Boss4PartD0_Draw (modified: Model_Draw -> boss_model_pose) */
static void Boss4PartD0_Draw(void *p) {
    Boss4PartD0 *s = (Boss4PartD0 *)p;
    BossModel *a;
    s->unk20C.posed = s->unk344.posed = 0;
    if (s->h.disp_flags & 0x20)
        return;
    a = &s->unk20C;
    Model_Animate(a);
    boss_model_pose(a, NULL);
    if (s->unk480 == 0)
        return;
    a = &s->unk344;
    Model_Animate(a);
    boss_model_pose(a, NULL);
}

/* Boss4PartD0_Destructor (modified) */
static void Boss4PartD0_Destructor(void *p) {
    Boss4PartD0 *s = (Boss4PartD0 *)p;
    boss_model_release(&s->unk20C);
    boss_model_release(&s->unk344);
    part_forget(&s->unk134, 3);
}

/* Boss4PartD0_Create (modified: pool slot) */
static Boss4PartD0 *Boss4PartD0_Create(const ObjParam *p, s32 b, s32 c) {
    Boss4 *boss = gBoss4;
    Boss4PartD0 *w = NULL;
    BossModel *sub;
    int i;
    for (i = 0; i < B4_D0; i++)
        if (!s_d0[i].alive) {
            w = &s_d0[i];
            break;
        }
    if (w == NULL)
        return NULL;
    boss_model_release(&w->unk20C);
    boss_model_release(&w->unk344);
    memset(w, 0, sizeof(*w));
    w->alive = 1;
    MapObject_Init_parts(&w->h, &w->unk134, p, b, c);
    w->func = Boss4PartD0_State_Update;
    w->unk78 = Boss4PartD0_Collide;
    w->h.obj_flags |= 0x10;
    w->h.move_flags |= 0xa300;
    sub = &w->unk20C;
    Model_Init(sub, 0);
    Model_SetResource(sub, (const BossModelRes *)boss->unkAB0[3]); /* eff_ele_atk */
    sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
    sub->trans[0] = boss->unkB58[9]; /* unk48 = unkB7C */
    sub->trans[1] = boss->unkB58[10];
    sub->trans[2] = boss->unkB58[11];
    Boss4_ArenaToStagePos(sub->trans, &w->h);
    sub = &w->unk344;
    Model_Init(sub, 0);
    Model_SetResource(sub, (const BossModelRes *)boss->unkAB0[9]);  /* eff_ele_flash */
    Model_SetAnimation(sub, 0, (const BossAnimRes *)boss->unkAB0[10], 0);
    Model_SetAnimation(sub, 1, (const BossAnimRes *)boss->unkAB0[11], 0);
    sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
    sub->trans[0] = boss->unkB58[9];
    sub->trans[1] = boss->unkB58[10];
    sub->trans[2] = boss->unkB58[11];
    w->unk484 = boss->unkB58[10] + 0x200000; /* unkB7C.y */
    Sound_PlaySeqArc(0, 0, 0x89);
    Boss4PartD0_StartLaunch(w);
    g_boss_stats.shots++;
    return w;
}

/* ============================================================ D1: enemy */
static void Boss4PartD1_SubState_Fall(Boss4PartD1 *s);
static void Boss4PartD1_SubState_TurnToPlayer(Boss4PartD1 *s);
static void Boss4PartD1_SubState_Chase(Boss4PartD1 *s);
static void Boss4PartD1_SubState_Brake(Boss4PartD1 *s);

/* Boss4PartD1_GetPlayerDirection (copied) */
static dcp_bool Boss4PartD1_GetPlayerDirection(const Boss4PartD1 *o) {
    s32 d = gPlayers->x - o->h.x;
    dcp_bool f = d > 0;
    if ((d < 0 ? -d : d) <= 0x384ff)
        return f;
    return !f;
}

/* Boss4PartD1_StartFall (copied) */
static void Boss4PartD1_StartFall(Boss4PartD1 *p0) {
    p0->unk3C0 = Boss4PartD1_SubState_Fall;
    p0->unk3C4 = 0;
    p0->unk3A8 = 0;
    p0->unk3AC = 0;
    p0->unk3B0 = 0;
    p0->unk3B4 = 0x1400;
    p0->unk3B8 = 0x80;
    p0->unk3BC = 0x1800;
}

/* Boss4PartD1_StartTurnToPlayer (copied) */
static void Boss4PartD1_StartTurnToPlayer(Boss4PartD1 *p0) {
    p0->unk348 = Boss4PartD1_GetPlayerDirection(p0);
    p0->unk3C0 = Boss4PartD1_SubState_TurnToPlayer;
    p0->unk3C4 = 0;
    p0->unk3A8 = 0;
    p0->unk3AC = 0;
    p0->unk3B0 = 0;
    p0->unk3B4 = 0;
    p0->unk3B8 = 0;
    p0->unk3BC = 0;
}

/* Boss4PartD1_StartChase (copied) */
static void Boss4PartD1_StartChase(Boss4PartD1 *s) {
    s->unk3C0 = Boss4PartD1_SubState_Chase;
    s->unk3C4 = 0;
    s->unk3A8 = 0;
    s->unk3AC = 0x20;
    s->unk3B0 = 0x400;
    s->unk3B4 = 0;
    s->unk3B8 = 0;
    s->unk3BC = 0;
}

/* Boss4PartD1_StartBrake (copied) */
static void Boss4PartD1_StartBrake(Boss4PartD1 *s) {
    s->unk3C0 = Boss4PartD1_SubState_Brake;
    s->unk3C4 = 0;
    s->unk3AC = -16;
}

/* Boss4PartD1_SubState_* (copied) */
static void Boss4PartD1_SubState_Fall(Boss4PartD1 *s) {
    if (s->h.y < 0x20000)
        return;
    s->h.y = 0x20000;
    Boss4PartD1_StartTurnToPlayer(s);
}
static void Boss4PartD1_SubState_TurnToPlayer(Boss4PartD1 *s) {
    u16 t;
    u16 old = s->unk34C;
    if (s->unk348)
        t = 0x4000;
    else
        t = 0xc000;
    s->unk34C += 0x300;
    if (t > old && t <= s->unk34C) {
        s->unk34C = t;
        Boss4PartD1_StartChase(s);
    }
}
static void Boss4PartD1_SubState_Chase(Boss4PartD1 *s) {
    if (s->unk348 == Boss4PartD1_GetPlayerDirection(s))
        return;
    Boss4PartD1_StartBrake(s);
}
static void Boss4PartD1_SubState_Brake(Boss4PartD1 *s) {
    if (s->unk3AC * s->unk3A8 < 0)
        return;
    Boss4PartD1_StartTurnToPlayer(s);
}

/* Boss4PartD1_State_Update (copied) */
static void Boss4PartD1_State_Update(void *p) {
    Boss4PartD1 *s = (Boss4PartD1 *)p;
    u16 ang;
    if (gBossStatus.unk4 == 0x1a) {
        s->h.obj_flags |= 8;
        s->h.obj_flags |= 2;
        return;
    }
    s->unk344++;
    s->unk3C4++;
    s->unk3A8 += s->unk3AC;
    if (s->unk3A8 > s->unk3B0)
        s->unk3A8 = s->unk3B0;
    s->unk3B4 += s->unk3B8;
    if (s->unk3B4 > s->unk3BC)
        s->unk3B4 = s->unk3BC;
    if (s->unk348)
        s->h.x += s->unk3A8;
    else
        s->h.x -= s->unk3A8;
    s->h.y += s->unk3B4;
    if (s->unk3C0 != NULL)
        s->unk3C0(s);
    Boss4_WrapArenaX(&s->h.x);
    Boss4_ObjArenaPosToWorld(s->unk20C.trans, &ang, &s->h); /* unk254 */
    {
        u16 r = s->unk34C + ang;
        MTX_RotY33(&s->unk20C.rot, SIN(r), COS(r)); /* unk230 */
    }
}

/* Boss4PartD1_OnHit (modified: owner type check -> obj_box_player) */
static void Boss4PartD1_OnHit(ObjBox *a, ObjBox *b) {
    Boss4 *w = gBoss4;
    Boss4PartD1 *o = (Boss4PartD1 *)a->owner;
    DcPlayer *p = obj_box_player(b);
    if (p == NULL)
        return;
    if (p->anim == 0x30) {
        w->unkBDC--;
        if (w->unkBDC < 0)
            w->unkBDC = 0;
        Sound_PlaySeqArc(0, 0, 0x33);
        BossEffect_CreateShootBomb(1, o->unk20C.trans[0] >> 4, -o->unk20C.trans[1] >> 4, o->unk20C.trans[2] >> 4);
        Player_Action_AttackBounceVel(p, -0x900);
        Boss4_SetCameraZoomOut();
        o->h.obj_flags |= 8;
        o->unk134.flags |= 0x1000; /* unk13E */
        o->unk160.flags |= 0x1000; /* unk16A */
        o->unk18C.flags |= 0x1000; /* unk196 */
        g_boss_stats.enemy_bounces++;
        boss_log("enemy_bounce", o->h.x >> 8, 0);
    } else if (p->invuln == 0) {
        obj_player_on_hurt(b, a);
    }
}

/* Boss4PartD1_Collide (modified) */
static void Boss4PartD1_Collide(void *p) {
    Boss4PartD1 *s = (Boss4PartD1 *)p;
    ObjBox *h;
    ObjBox *h2;
    h = &s->unk134;
    rect_set_box2d(h, -0x18, -0x1b, 0x18, 0x1b);
    h->owner = s;
    h->owner_kind = OBJ_OWNER_OBJECT;
    h->cb = Boss4PartD1_OnHit;
    h->flags |= 4;
    obj_handle_collider(&s->h, s, OBJ_OWNER_OBJECT, h, 0);
    h2 = &s->unk160;
    rect_set_box2d(h2, -0x20, -0x19, 0x20, 0x19);
    h2->owner = s;
    h2->owner_kind = OBJ_OWNER_OBJECT;
    h2->cb = NULL;
    h2->flags |= 4;
    obj_handle_collider(&s->h, s, OBJ_OWNER_OBJECT, h2, 0);
    Boss4_HandleColliderWrapped(&s->unk134, &s->unk350);
    Boss4_HandleColliderWrapped(&s->unk160, &s->unk37C);
}

/* Boss4PartD1_Draw (modified) */
static void Boss4PartD1_Draw(void *p) {
    Boss4PartD1 *s = (Boss4PartD1 *)p;
    BossModel *a = &s->unk20C;
    a->posed = 0;
    if (s->h.disp_flags & 0x20)
        return;
    Model_Animate(a);
    boss_model_pose(a, NULL);
}

/* Boss4PartD1_Destructor (modified) */
static void Boss4PartD1_Destructor(void *p) {
    Boss4PartD1 *s = (Boss4PartD1 *)p;
    boss_model_release(&s->unk20C);
    part_forget(&s->unk134, 3);
    rect_forget(&s->unk350);
    rect_forget(&s->unk37C);
}

/* Boss4PartD1_Create (modified: pool slot) */
static Boss4PartD1 *Boss4PartD1_Create(const ObjParam *p, s32 b, s32 c) {
    Boss4 *boss = gBoss4;
    Boss4PartD1 *w = NULL;
    BossModel *sub;
    int i;
    if (boss->unkBDC >= 1)
        return NULL;
    for (i = 0; i < B4_D1; i++)
        if (!s_d1[i].alive) {
            w = &s_d1[i];
            break;
        }
    if (w == NULL)
        return NULL;
    boss->unkBDC++;
    boss_model_release(&w->unk20C);
    memset(w, 0, sizeof(*w));
    w->alive = 1;
    MapObject_Init_parts(&w->h, &w->unk134, p, b, c);
    w->func = Boss4PartD1_State_Update;
    w->unk78 = Boss4PartD1_Collide;
    w->h.obj_flags |= 0x10;
    w->h.move_flags |= 0xa300;
    sub = &w->unk20C;
    Model_Init(sub, 0);
    Model_SetResource(sub, &data_ov000_02369b30[5]);
    sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
    sub->trans[0] = boss->unkB88[9]; /* unk48 = unkBAC */
    sub->trans[1] = boss->unkB88[10];
    sub->trans[2] = boss->unkB88[11];
    Boss4_ArenaToStagePos(sub->trans, &w->h);
    w->unk34C = 0;
    Sound_PlaySeqArc(0, 0, 0x8a);
    Boss4PartD1_StartFall(w);
    g_boss_stats.enemies++;
    boss_log("enemy", w->h.x >> 8, w->h.y >> 8);
    return w;
}

/* ============================================================ D2: shock waves */
/* Boss4PartD2_SubState_Spread (copied) */
static void Boss4PartD2_SubState_Spread(Boss4PartD2 *p0) {
    if (p0->unk4D8 != 0)
        return;
    p0->unk4DC = p0->unk4DC - 0x40;
    if (p0->unk4DC < 0x700)
        p0->unk4DC = 0x700;
    p0->unk4E0 = p0->unk4E0 + p0->unk4DC;
    p0->unk4E4 = p0->unk4E4 - p0->unk4DC;
}

/* Boss4PartD2_StartSpread (copied) */
static void Boss4PartD2_StartSpread(Boss4PartD2 *s) {
    s->unk4F0 = Boss4PartD2_SubState_Spread;
    s->unk4F4 = 0;
    s->unk4DC = (Rand() & 1) ? 0x1600 : 0xa00;
}

/* Boss4PartD2_State_Update (copied; data_02070470[0x70] = the sine table entry of angle 0x8380) */
static void Boss4PartD2_State_Update(void *p) {
    Boss4PartD2 *s = (Boss4PartD2 *)p;
    if (gBossStatus.unk4 == 0x1a) {
        s->h.obj_flags |= 8;
        s->h.obj_flags |= 2;
        return;
    }
    s->unk47C++;
    s->unk4F4++;
    if (s->unk4F0 != NULL)
        s->unk4F0(s);
    Boss4_WrapArenaX(&s->unk4E0);
    Boss4_WrapArenaX(&s->unk4E4);
    Boss4_ArenaPosToWorld(NULL, &s->unk4EA, s->unk4E0, 0);
    Boss4_ArenaPosToWorld(NULL, &s->unk4EC, s->unk4E4, 0);
    if (s->unk4D8 == 0) {
        u16 d = s->unk4EA - s->unk4E8;
        if (SIN(d) < SIN(0x8380)) {
            s->unk4D8 = 1;
            Boss4_ArenaPosToWorld(s->unk344.trans, NULL, gBoss4->unk20C->h.x, s->h.y);
        }
    } else if (s->unk4D8 != 0 && (s->unk344.flags[0] & 2)) {
        s->h.obj_flags |= 8;
        s->h.obj_flags |= 2;
    }
    if (s->unk47C > 0x258) {
        s->h.obj_flags |= 8;
        s->h.obj_flags |= 2;
    }
}

/* Boss4PartD2_Collide (modified) */
static void Boss4PartD2_Collide(void *p) {
    Boss4PartD2 *s = (Boss4PartD2 *)p;
    ObjBox *h;
    s->h.x = s->unk4E0;
    h = &s->unk160;
    rect_set_box2d(h, -0x50, -0x41, 0, 10);
    h->owner = s;
    h->owner_kind = OBJ_OWNER_OBJECT;
    h->cb = NULL;
    h->flags |= 4;
    obj_handle_collider(&s->h, s, OBJ_OWNER_OBJECT, h, 0);
    Boss4_HandleColliderWrapped(&s->unk160, &s->unk480);
    s->h.x = s->unk4E4;
    h = &s->unk18C;
    rect_set_box2d(h, 0, -0x41, 0x50, 10);
    h->owner = s;
    h->owner_kind = OBJ_OWNER_OBJECT;
    h->cb = NULL;
    h->flags |= 4;
    obj_handle_collider(&s->h, s, OBJ_OWNER_OBJECT, h, 0);
    Boss4_HandleColliderWrapped(&s->unk18C, &s->unk4AC);
}

/* Boss4PartD2_Draw (modified: the wave model is drawn twice; the second model matrix is kept in wave2) */
static void Boss4PartD2_Draw(void *p) {
    Boss4PartD2 *s = (Boss4PartD2 *)p;
    BossModel *a;
    dcm_mtx_t first;
    s->unk20C.posed = s->unk344.posed = 0;
    if (s->h.disp_flags & 0x20)
        return;
    if (s->unk4D8 != 0) {
        a = &s->unk344;
        Model_Animate(a);
        boss_model_pose(a, NULL);
        return;
    }
    a = &s->unk20C;
    Model_Animate(a);
    Boss4_ArenaPosToWorld(a->trans, &s->unk4EA, s->unk4E0, s->h.y);
    s->unk4EA += 0x4000;
    MTX_RotY33(&a->rot, SIN(s->unk4EA), COS(s->unk4EA));
    boss_model_pose(a, NULL);
    first = a->mdl_mtx;
    Boss4_ArenaPosToWorld(a->trans, &s->unk4EC, s->unk4E4, s->h.y);
    s->unk4EC -= 0x4000;
    MTX_RotY33(&a->rot, SIN(s->unk4EC), COS(s->unk4EC));
    boss_model_pose(a, NULL);
    s->wave2 = a->mdl_mtx;
    a->mdl_mtx = first;
}

/* Boss4PartD2_Destructor (modified) */
static void Boss4PartD2_Destructor(void *p) {
    Boss4PartD2 *s = (Boss4PartD2 *)p;
    boss_model_release(&s->unk20C);
    boss_model_release(&s->unk344);
    part_forget(&s->unk134, 3);
    rect_forget(&s->unk480);
    rect_forget(&s->unk4AC);
}

/* Boss4PartD2_Create (modified: pool slot) */
static Boss4PartD2 *Boss4PartD2_Create(const ObjParam *p, s32 b, s32 c) {
    Boss4 *boss = gBoss4;
    Boss4PartD2 *w = NULL;
    BossModel *sub;
    int i;
    for (i = 0; i < B4_D2; i++)
        if (!s_d2[i].alive) {
            w = &s_d2[i];
            break;
        }
    if (w == NULL)
        return NULL;
    boss_model_release(&w->unk20C);
    boss_model_release(&w->unk344);
    memset(w, 0, sizeof(*w));
    w->alive = 1;
    MapObject_Init_parts(&w->h, &w->unk134, p, b, c);
    w->func = Boss4PartD2_State_Update;
    w->unk78 = Boss4PartD2_Collide;
    w->h.obj_flags |= 0x10;
    w->h.move_flags |= 0xa300;
    sub = &w->unk20C;
    Model_Init(sub, 0);
    Model_SetResource(sub, (const BossModelRes *)boss->unkAB0[12]); /* eff_ele_wave */
    Model_SetAnimation(sub, 0, (const BossAnimRes *)boss->unkAB0[13], 0);
    sub->flags[0] |= 4;
    sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
    sub->trans[0] = 0;
    sub->trans[1] = -0x200000;
    sub->trans[2] = 0;
    w->h.x = boss->unk210->h.x;
    w->h.y = 0x20000;
    w->h.z = 0;
    w->unk4E0 = w->h.x;
    w->unk4E4 = w->h.x;
    Boss4_ArenaPosToWorld(NULL, &w->unk4E8, w->unk4E0, 0);
    sub = &w->unk344;
    Model_Init(sub, 0);
    Model_SetResource(sub, (const BossModelRes *)boss->unkAB0[14]); /* eff_wave_disap */
    Model_SetAnimation(sub, 0, (const BossAnimRes *)boss->unkAB0[15], 0);
    Model_SetAnimation(sub, 1, (const BossAnimRes *)boss->unkAB0[16], 0);
    sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
    Sound_PlaySeqArc(0, 0, 0x8c);
    Boss4PartD2_StartSpread(w);
    g_boss_stats.waves++;
    return w;
}

/* MapObject_Spawn(type, 0, 0, ...) for the Boss4 parts */
static void *MapObject_Spawn4(u16 type) {
    ObjParam prm;
    memset(&prm, 0, sizeof(prm));
    prm.x0 = prm.y0 = 0xff;
    prm.type = type;
    switch (type) {
    case 0xce:
        return Boss4PartCE_Create(&prm, 0, 0);
    case 0xcf:
        return Boss4PartCF_Create(&prm, 0, 0);
    case 0xd0:
        return Boss4PartD0_Create(&prm, 0, 0);
    case 0xd1:
        return Boss4PartD1_Create(&prm, 0, 0);
    case 0xd2:
        return Boss4PartD2_Create(&prm, 0, 0);
    }
    return NULL;
}

/* ============================================================ Boss4 states */
/* Boss4_State_WaitForStart (from the ARM listing in src/asm_fallback/) */
static void Boss4_State_WaitForStart(void *p) {
    Boss4 *r6 = (Boss4 *)p;
    Boss4 *r4;
    BossModel *r5;
    if (!(gBossStatus.unk0 & 1))
        return;
    r4 = gBoss4;
    r4->h.disp_flags &= ~0x20;
    g_boss_game.draw_player = Boss4_Draw_PlayerOnArena; /* gPlayers->unk70 */
    gPlayers->disp_flags |= 0x2040;
    CAM.unk0 |= 6;
    CAM.unk0 |= 1;
    r5 = &CAM.unkE0; /* data_ov000_0236a524 */
    Model_Init(r5, 0);
    Model_SetResource(r5, &data_ov000_02369b30[6]); /* [0x18 / 4] */
    r5->scale[2] = 0x1355b50;
    r5->scale[1] = r5->scale[2];
    r5->scale[0] = r5->scale[1];
    Boss4_ResetCamera();
    r4->unk20C = (Boss4Part *)MapObject_Spawn4(0xce);
    r4->unk210 = (Boss4Part *)MapObject_Spawn4(0xcf);
    Boss4_StartAuxModelCycle(r4);
    NaviMsg_SetFaceAnim(9);
    NaviMsg_ShowMessage(0xf, 0xf0);
    r6->func = Boss4_State_Fight;
    Boss4_Action_StartSpin(r4);
    boss_log("start", gBossStatus.unk8, 0);
}

/* Boss4_State_Fight (copied) */
static void Boss4_State_Fight(void *p) {
    Boss4 *t0 = gBoss4;
    (void)p;
    t0->unk218 = t0->unk218 + 1;
    t0->unk224 = t0->unk224 + 1;
    if ((u32)(gBossStatus.unk4 - 0x18) <= 1u) {
        Boss4_ActivateAuxModel(t0);
        t0->unkBE8 = data_ov000_02363580[gBossStatus.unk8 - 1];
        t0->unkBE4 = 0;
        t0->unkBE0 = 0;
    } else {
        Boss4_UpdateAuxModelCycle(t0);
        Boss4_UpdateAttackTimer(t0);
    }
    if (t0->unk220 == NULL)
        return;
    t0->unk220(t0);
}

/* Boss4_State_DefeatCamera1 (modified: Player_SetAnimation -> dcp_set_anim) */
static void Boss4_State_DefeatCamera1(void *p) {
    Boss4 *t = (Boss4 *)p;
    Boss4 *w = gBoss4;
    w->unk224++;
    if (w->unk224 == 1) {
        Player_Action_Blank(gPlayers);
        w->unkBB8 = 0x7200;
        w->unkBBC = 0xd80;
        gPlayers->x = 0x1c200;
        gPlayers->y = 0x1a100;
        dcp_set_anim(gPlayers, 0x30);
        return;
    }
    Boss4_ResetCamera();
    CAM.unkDC = 2;
    CAM.t[0].lerp = 0x1000;                                  /* c->unk30 */
    CAM.t[1].ox = CAM.t[1].oy = CAM.t[1].oz = NULL;          /* c->unk3C..44 */
    CAM.t[0].ox = CAM.t[0].oy = CAM.t[0].oz = NULL;          /* c->unk0..8 */
    CAM.t[1].offx = CAM.t[1].offy = CAM.t[1].offz = 0;       /* c->unk48..50 */
    CAM.t[1].tgt.x = g_boss_game.player_trans[0];            /* c->unk54 = gPlayers->unk17C */
    CAM.t[1].tgt.y = g_boss_game.player_trans[1];
    CAM.t[1].tgt.z = g_boss_game.player_trans[2];
    CAM.t[1].lerp = 0x1000;                                  /* c->unk6C */
    CAM.unkC8 = 0x1000;                                      /* c->unk80 */
    CAM.unkD4 = 0x1000;                                      /* c->unk8C */
    CAM.unkDA = 0x1000;                                      /* c->unk92 */
    CAM.unkC0 = 0x50000;                                     /* c->unk78 */
    CAM.unkCC = 0x20000;                                     /* c->unk84 */
    CAM.unkD6 = 0x3000;                                      /* c->unk8E */
    StageObj_SetPause();
    w->h.obj_flags |= 0x20;
    t->func = Boss4_State_DefeatFade_ov000_022dd590;
    w->unk224 = 0;
}

/* Boss4_State_DefeatCamera2 / 3 (modified: BG2 cut-in -> Boss_DrawExcBackground; brightness state) */
static void Boss4_State_DefeatCamera2(void *p) {
    Boss4 *w = gBoss4;
    (void)p;
    w->func = Boss4_State_DefeatFade_ov000_022dd4d0;
    w->unk224 = 0;
    g_boss_game.bright = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7c);
    Boss_DrawExcBackground(7);
    CAM.unkC0 = 0x60000; /* c->unk78 */
    CAM.unkCC = 0x80000; /* c->unk84 */
    CAM.unkD6 = 0x5000;  /* c->unk8E */
}
static void Boss4_State_DefeatCamera3(void *p) {
    Boss4 *w = gBoss4;
    (void)p;
    w->func = Boss4_State_DefeatFade_ov000_022dd410;
    w->unk224 = 0;
    g_boss_game.bright = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7c);
    Boss_DrawExcBackground(6);
    CAM.unkC0 = 0x40000;
    CAM.unkCC = 0x10000;
    CAM.unkD6 = 0x4000;
}

/* Boss4_State_DefeatFade_* (copied) */
static void Boss4_State_DefeatFade_ov000_022dd410(void *p0) {
    Boss4 *t0 = gBoss4;
    t0->unk224 = t0->unk224 + 1;
    if (g_boss_game.bright > 0)
        g_boss_game.bright = (s16)(g_boss_game.bright - 2);
    if (t0->unk224 > 0x1e)
        ((Boss4 *)p0)->func = Boss4_State_DefeatExplode;
}
static void Boss4_State_DefeatFade_ov000_022dd4d0(void *p0) {
    Boss4 *t0 = gBoss4;
    t0->unk224 = t0->unk224 + 1;
    if (g_boss_game.bright > 0)
        g_boss_game.bright = (s16)(g_boss_game.bright - 2);
    if (t0->unk224 > 0x1e)
        ((Boss4 *)p0)->func = Boss4_State_DefeatCamera3;
}
static void Boss4_State_DefeatFade_ov000_022dd590(void *p0) {
    Boss4 *t0 = gBoss4;
    t0->unk224 = t0->unk224 + 1;
    if (g_boss_game.bright > 0)
        g_boss_game.bright = (s16)(g_boss_game.bright - 2);
    if (t0->unk224 > 0x1e)
        ((Boss4 *)p0)->func = Boss4_State_DefeatCamera2;
}

/* Boss4_State_DefeatExplode (modified: NNS_G3dRenderObjResetCallBack -> s_cb_off; DISPCNT -> exc_bg) */
static void Boss4_State_DefeatExplode(void *p) {
    Boss4 *t = (Boss4 *)p;
    Boss4 *w = gBoss4;
    BossModel *a;
    s32 i;
    BossEffect *e;
    CAM.unkC0 = 0x1f0fb8;    /* c->unk78 */
    CAM.unkCC = 0x64000;     /* c->unk84 */
    CAM.unkD6 = 0;           /* c->unk8E */
    CAM.t[1].tgt.x = 0;      /* c->unk54 */
    CAM.t[1].tgt.y = -0x1d8000;
    CAM.t[1].tgt.z = 0;
    w->unkBB8 = 0;
    w->unkBBC = 0;
    a = &w->unk228[2];
    Model_SetAnimation(a, 0, AB0_ANM(0), 1);
    s_cb_off[1] = 1;
    a = &w->unk228[1];
    Model_SetAnimation(a, 0, AB0_ANM(0), 3);
    s_cb_off[0] = 1;
    a = &w->unk228[4];
    Model_SetAnimation(a, 0, AB0_ANM(0), 2);
    a->speeds[0] = 0x1000; /* unk118 */
    s_cb_off[2] = 1;
    a->trans[1] = -0x200000;
    MTX_Identity33((MtxFx33 *)w->unkAF8);
    w->unkAF8[9] = 0;
    w->unkAF8[10] = -0x200000;
    w->unkAF8[11] = 0;
    MTX_Identity33((MtxFx33 *)w->unkB28);
    w->unkB28[9] = 0;
    w->unkB28[10] = -0x200000;
    w->unkB28[11] = 0;
    Boss_DrawExcBackground((u32)-1); /* GX_SetVisiblePlane(.. & ~4) */
    g_boss_game.exc_bg = -1;
    w->h.obj_flags &= ~0x20;
    StageObj_ClearPause();
    gPlayers->disp_flags |= 0x20;
    gPlayers->pf |= 0x200000;
    e = BossEffect_CreateBlast4(0, 0, 0x1ce00, 0xfa00);
    e->unk134.speed = 0x800; /* unk248 */
    g_boss_game.bright = 0x10;
    NaviMsg_SetFaceAnim(0xd);
    NaviMsg_ShowMessage(0x13, 0xf0);
    Sound_PlaySeqArc(0, 0, 0x7d);
    for (i = 0; i < 7; i++)
        w->unk228[i].flag &= ~2u;
    t->func = Boss4_State_DefeatFinish;
    w->unk224 = 0;
}

/* Boss4_State_DefeatFinish (modified: StageShutdown_Create -> g_boss_hooks.cleared (once)) */
static void Boss4_State_DefeatFinish(void *p) {
    Boss4 *w = gBoss4;
    (void)p;
    w->unk224++;
    gPlayers->invuln = 0;
    gPlayers->disp_flags |= 0x20;
    if (!(w->unk224 & 3)) {
        if (g_boss_game.bright > 0)
            g_boss_game.bright--;
    }
    if (w->unk224 <= 0x12c)
        return;
    g_obj_stage.ring_bonus = gPlayers->rings * 1000; /* data_022c4590.unk10 */
    if (!g_boss_game.cleared) {
        g_boss_game.cleared = 1;
        boss_log("cleared", gPlayers->rings, 0);
        if (g_boss_hooks.cleared)
            g_boss_hooks.cleared(g_boss_hooks.ctx);
    }
}

/* Boss4_Destructor (modified) */
static void Boss4_Destructor(void *p) {
    Boss4 *s = (Boss4 *)p;
    s32 i;
    for (i = 0; i < 7; i++)
        boss_model_release(&s->unk228[i]);
    /* NNS_G3dResDefaultRelease of the effect / eff_ele_* resources: boss4_unload / boss_effects_shutdown */
    g_boss_game.draw_player = NULL;
    gBoss4 = NULL;
}

/* Boss4_Create (modified: resources from BossStage4 / the zone act archive; callbacks native) */
void *Boss4_Create(const ObjParam *p, s32 b, s32 c) {
    Boss4 *w;
    s32 i;
    BossModel *sub;
    if (p->x0 == 0xff && p->y0 == 0xff)
        return NULL;
    w = &s_boss4;
    for (i = 0; i < 7; i++)
        boss_model_release(&w->unk228[i]);
    memset(w, 0, sizeof(*w));
    w->alive = 1;
    gBoss4 = w;
    memset(&gBossStatus, 0, sizeof(gBossStatus));
    gBossStatus.unk8 = (u16)Boss_GetStartHealth();
    gBossStatus.unkA = (u16)Boss_GetStartHealth();
    w->unkBE0 = 0;
    w->unkBE8 = data_ov000_02363580[gBossStatus.unk8 - 1];
    MapObject_Init_parts(&w->h, &w->unk134, p, b, c);
    w->func = Boss4_State_WaitForStart;
    w->unk78 = NULL;
    w->h.obj_flags |= 0x12;
    w->h.move_flags |= 0xa300;
    w->h.disp_flags |= 0x20;
    w->h.x = 0;
    w->h.y = 0x20000;
    w->h.z = 0;
    w->unkAB0[0] = &s_bs4_ca; /* "bs4" files 8, 9, 10 */
    w->unkAB0[1] = &s_bs4_ma;
    w->unkAB0[2] = &s_bs4_tp;
    sub = w->unk228;
    for (i = 0; i < 7; i++) {
        Model_Init(sub, 0);
        Model_SetResource(sub, &data_ov000_02369b30[i]);
        sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
        sub->trans[0] = w->h.x << 4;
        sub->trans[1] = -(w->h.y << 4);
        sub->trans[2] = w->h.z << 4;
        sub->cb[0] = MOP_FLUSHVP; /* unkA = 4 */
        sub++;
    }
    Model_SetAnimation(&w->unk228[0], 2, AB0_ANM(2), 0); /* pattern animation (bs4.nsbtp, timing only) */
    w->unk228[0].flags[2] |= 4; /* unk10E */
    w->unk228[4].trans[1] = (s32)0xffec6b72;
    /* NNS_G3dRenderObjSetCallBack: Rods (rod), ArmGun (arm), BodyExit (body): run after each pose */
    s_cb_off[0] = s_cb_off[1] = s_cb_off[2] = 0;
    w->unkAB0[3] = &s_ele_atk;     /* 0x51 eff_ele_atk.nsbmd */
    w->unkAB0[4] = &s_ele_stay;    /* 0x52 eff_ele_stay.nsbmd */
    w->unkAB0[5] = NULL;           /* 0x53 eff_ele_stay.nsbca (not used) */
    w->unkAB0[6] = NULL;           /* 0x54 eff_ele_stay.nsbta (not used) */
    w->unkAB0[7] = NULL;           /* 0x55 eff_ele_stay.nsbtp (not used) */
    w->unkAB0[8] = &s_stay_ma;     /* 0x56 eff_ele_stay.nsbma */
    w->unkAB0[9] = &s_ele_flash;   /* 0x57 eff_ele_flash.nsbmd */
    w->unkAB0[10] = &s_flash_ca;   /* 0x59 eff_ele_flash.nsbca */
    w->unkAB0[11] = &s_flash_ma;   /* 0x58 eff_ele_flash.nsbma */
    w->unkAB0[12] = &s_ele_wave;   /* 0x5a eff_ele_wave.nsbmd */
    w->unkAB0[13] = &s_wave_ca;    /* 0x5b eff_ele_wave.nsbca */
    w->unkAB0[14] = &s_wave_disap; /* 0x5c eff_wave_disap.nsbmd */
    w->unkAB0[15] = &s_disap_ca;   /* 0x5e eff_wave_disap.nsbca */
    w->unkAB0[16] = &s_disap_ma;   /* 0x5d eff_wave_disap.nsbma */
    /* NNS_G3dResDefaultSetup("bs4:eff_hit/clash/shoot_bomb/impact_small.nsbmd"): boss_effects_load */
    Boss4_LoadModelPalettes1(0);
    Boss4_LoadModelPalettes2(0);
    Boss4_LoadModelPalettes4(0);
    Boss4_LoadModelPalettes3(0);
    Boss4_LoadModelPalettes5(0);
    boss_log("create", gBossStatus.unk8, 0);
    return w;
}

/* ============================================================ module glue */
static int load_lengths(BossAnimRes *r, const char *name) {
    memset(r, 0, sizeof(*r));
    if (boss_anim_lengths(name, &r->nframes, &r->count) != 0) {
        printf("boss: %s frame counts missing in bossdata.bin\n", name);
        return -1;
    }
    return 0;
}

/* BossStage4_OnModelLoaded: data_02074538 = bs4_base, bs4_body, bs4_arm, bs4_ele, bs4_rod,
 * bs4_enemy, bs4_sky (NaviBg_Load / NaviB_Load: HUD, not here); plus the z43 archive files */
int boss4_load(void) {
    static const char *const files[7] = {"mod__bs4_base__bs4_base.dcm", "mod__bs4_body__bs4_body.dcm",
                                         "mod__bs4_arm__bs4_arm.dcm",   "mod__bs4_ele__bs4_ele.dcm",
                                         "mod__bs4_rod__bs4_rod.dcm",   "mod__bs4_enemy__bs4_enemy.dcm",
                                         "mod__bs4_sky__bs4_sky.dcm"};
    int i, r = 0;
    for (i = 0; i < 7; i++)
        if (!data_ov000_02369b30[i].loaded)
            r |= boss_res_load_model(&data_ov000_02369b30[i], files[i]);
    if (!s_bs4_ca.loaded)
        r |= boss_res_load_anim(&s_bs4_ca, "narc__z43_act_lz7__bs4.dca");
    if (!s_ele_atk.loaded)
        r |= boss_res_load_model(&s_ele_atk, "narc__z43_act_lz7__eff_ele_atk__eff_ele_atk.dcm");
    if (!s_ele_stay.loaded)
        r |= boss_res_load_model(&s_ele_stay, "narc__z43_act_lz7__eff_ele_stay__eff_ele_stay.dcm");
    if (!s_ele_flash.loaded)
        r |= boss_res_load_model(&s_ele_flash, "narc__z43_act_lz7__eff_ele_flash__eff_flash.dcm");
    if (!s_flash_ca.loaded)
        r |= boss_res_load_anim(&s_flash_ca, "narc__z43_act_lz7__eff_ele_flash.dca");
    if (!s_ele_wave.loaded)
        r |= boss_res_load_model(&s_ele_wave, "narc__z43_act_lz7__eff_ele_wave__eff_wave.dcm");
    if (!s_wave_ca.loaded)
        r |= boss_res_load_anim(&s_wave_ca, "narc__z43_act_lz7__eff_ele_wave.dca");
    if (!s_wave_disap.loaded)
        r |= boss_res_load_model(&s_wave_disap, "narc__z43_act_lz7__eff_wave_disap__eff_wave_disap.dcm");
    if (!s_disap_ca.loaded)
        r |= boss_res_load_anim(&s_disap_ca, "narc__z43_act_lz7__eff_wave_disap.dca");
    r |= load_lengths(&s_bs4_ma, "bs4.nsbma");
    r |= load_lengths(&s_bs4_tp, "bs4.nsbtp");
    r |= load_lengths(&s_stay_ma, "eff_ele_stay.nsbma");
    r |= load_lengths(&s_flash_ma, "eff_ele_flash.nsbma");
    r |= load_lengths(&s_disap_ma, "eff_wave_disap.nsbma");
    s_joint[0] = boss_joint_index("bs4_rod", data_ov000_02363590);  /* "rod_arm" */
    s_joint[1] = boss_joint_index("bs4_rod", data_ov000_023635f0);  /* "rod_body" */
    s_joint[2] = boss_joint_index("bs4_arm", data_ov000_02363610);  /* "arm_gun" */
    s_joint[3] = boss_joint_index("bs4_body", data_ov000_023635b0); /* "body_exit" */
    for (i = 0; i < 4; i++)
        if (s_joint[i] < 0)
            r = -1;
    return r ? -1 : 0;
}

/* BossStage4_FreeModels (+ the archive files) */
void boss4_unload(void) {
    int i;
    for (i = 0; i < 7; i++)
        boss_res_free_model(&data_ov000_02369b30[i]);
    boss_res_free_anim(&s_bs4_ca);
    boss_res_free_model(&s_ele_atk);
    boss_res_free_model(&s_ele_stay);
    boss_res_free_model(&s_ele_flash);
    boss_res_free_anim(&s_flash_ca);
    boss_res_free_model(&s_ele_wave);
    boss_res_free_anim(&s_wave_ca);
    boss_res_free_model(&s_wave_disap);
    boss_res_free_anim(&s_disap_ca);
}

static const BossObjOps s_ops_boss4 = {Boss4_Draw_Body, Boss4Part_CollideTrampoline, Boss4_Destructor};
static const BossObjOps s_ops_ce = {Boss4PartCE_Draw, Boss4Part_CollideTrampoline, Boss4Part_Destructor};
static const BossObjOps s_ops_cf = {Boss4PartCF_Draw, Boss4Part_CollideTrampoline, Boss4Part_Destructor};
static void D0_collide(void *p) { if (((Boss4PartD0 *)p)->unk78) ((Boss4PartD0 *)p)->unk78(p); }
static void D1_collide(void *p) { if (((Boss4PartD1 *)p)->unk78) ((Boss4PartD1 *)p)->unk78(p); }
static void D2_collide(void *p) { if (((Boss4PartD2 *)p)->unk78) ((Boss4PartD2 *)p)->unk78(p); }
static const BossObjOps s_ops_d0 = {Boss4PartD0_Draw, D0_collide, Boss4PartD0_Destructor};
static const BossObjOps s_ops_d1 = {Boss4PartD1_Draw, D1_collide, Boss4PartD1_Destructor};
static const BossObjOps s_ops_d2 = {Boss4PartD2_Draw, D2_collide, Boss4PartD2_Destructor};

void boss4_destroy(void) {
    int i;
    for (i = 0; i < B4_D0; i++)
        if (s_d0[i].alive) {
            Boss4PartD0_Destructor(&s_d0[i]);
            s_d0[i].alive = 0;
        }
    for (i = 0; i < B4_D1; i++)
        if (s_d1[i].alive) {
            Boss4PartD1_Destructor(&s_d1[i]);
            s_d1[i].alive = 0;
        }
    for (i = 0; i < B4_D2; i++)
        if (s_d2[i].alive) {
            Boss4PartD2_Destructor(&s_d2[i]);
            s_d2[i].alive = 0;
        }
    if (s_ce.alive)
        Boss4Part_Destructor(&s_ce);
    if (s_cf.alive)
        Boss4Part_Destructor(&s_cf);
    s_ce.alive = s_cf.alive = 0;
    if (s_boss4.alive)
        Boss4_Destructor(&s_boss4);
    s_boss4.alive = 0;
    gBoss4 = NULL;
}

void boss4_update(void) {
    int i;
    if (s_boss4.alive)
        boss_obj_main(&s_boss4.h, &s_boss4.func, &s_ops_boss4, &s_boss4, &s_boss4.alive); /* 0x500 */
    if (s_ce.alive)
        boss_obj_main(&s_ce.h, &s_ce.func, &s_ops_ce, &s_ce, &s_ce.alive);                 /* 0x501 */
    if (s_cf.alive)
        boss_obj_main(&s_cf.h, &s_cf.func, &s_ops_cf, &s_cf, &s_cf.alive);                 /* 0x502 */
    for (i = 0; i < B4_D0; i++)                                                            /* 0x503 */
        if (s_d0[i].alive)
            boss_obj_main(&s_d0[i].h, &s_d0[i].func, &s_ops_d0, &s_d0[i], &s_d0[i].alive);
    for (i = 0; i < B4_D1; i++)                                                            /* 0x504 */
        if (s_d1[i].alive) {
            boss_obj_main(&s_d1[i].h, &s_d1[i].func, &s_ops_d1, &s_d1[i], &s_d1[i].alive);
        }
    for (i = 0; i < B4_D2; i++)                                                            /* 0x505 */
        if (s_d2[i].alive)
            boss_obj_main(&s_d2[i].h, &s_d2[i].func, &s_ops_d2, &s_d2[i], &s_d2[i].alive);
}

int boss4_draw(int list) {
    int n = 0, i;
    Boss4 *s = &s_boss4;
    if (!s->alive)
        return 0;
    g_boss_debug_color = 0x4080ff;
    if (!(s->h.disp_flags & 0x20)) {
        n += boss_model_draw(&s->unk228[0], list);
        n += boss_model_draw(&s->unk228[4], list);
        if (s->unkBD0)
            n += boss_model_draw(&s->unk228[3], list);
    }
    g_boss_debug_color = 0x40ff40;
    if (s_ce.alive)
        n += boss_model_draw(&s->unk228[1], list);
    g_boss_debug_color = 0xff4040;
    if (s_cf.alive)
        n += boss_model_draw(&s->unk228[2], list);
    g_boss_debug_color = 0xff8040;
    for (i = 0; i < B4_D0; i++)
        if (s_d0[i].alive) {
            n += boss_model_draw(&s_d0[i].unk20C, list);
            if (s_d0[i].unk480)
                n += boss_model_draw(&s_d0[i].unk344, list);
        }
    for (i = 0; i < B4_D1; i++)
        if (s_d1[i].alive)
            n += boss_model_draw(&s_d1[i].unk20C, list);
    for (i = 0; i < B4_D2; i++)
        if (s_d2[i].alive) {
            BossModel *a = &s_d2[i].unk20C;
            if (s_d2[i].unk4D8 == 0 && a->posed) {
                dcm_mtx_t first = a->mdl_mtx;
                n += boss_model_draw(a, list);
                a->mdl_mtx = s_d2[i].wave2;
                n += boss_model_draw(a, list);
                a->mdl_mtx = first;
            } else
                n += boss_model_draw(&s_d2[i].unk344, list);
        }
    g_boss_debug_color = 0xffff40;
    return n;
}

/* ------------------------------------------------------------ test / debug accessors */
void boss4_body_pos(s32 *x, s32 *y, s32 *z) {
    *x = s_boss4.unk228[1].trans[0] >> 4;
    *y = -s_boss4.unk228[1].trans[1] >> 4;
    *z = s_boss4.unk228[1].trans[2] >> 4;
}
int boss4_box_armed(int box) {
    if (box == 0)
        return s_ce.alive && (s_ce.coll0.flags & 4) && gBossStatus.unk4 != 0x18;
    if (box == 1)
        return s_cf.alive && (s_cf.coll0.flags & 4);
    return 0;
}
int boss4_info(Boss4Info *o) {
    int i;
    memset(o, 0, sizeof(*o));
    if (!s_boss4.alive)
        return 0;
    o->body_x = s_ce.h.x;
    o->body_y = s_ce.h.y;
    o->arm_x = s_cf.h.x;
    o->arm_y = s_cf.h.y;
    o->spin = s_boss4.unkBB8;
    o->tilt = s_boss4.unkBBC;
    o->electrified = s_boss4.unkBD0;
    o->body_box = s_ce.coll0;
    o->body_box_valid = s_ce.alive && gBossStatus.unk4 != 0x18 && gBossStatus.unk4 != 0x1a;
    for (i = 0; i < B4_D1; i++)
        if (s_d1[i].alive && !(s_d1[i].h.obj_flags & 0xc)) {
            o->enemy = 1;
            o->enemy_x = s_d1[i].h.x;
            o->enemy_y = s_d1[i].h.y;
        }
    for (i = 0; i < B4_D0; i++)
        o->shots += s_d0[i].alive;
    for (i = 0; i < B4_D2; i++)
        o->waves += s_d2[i].alive;
    return 1;
}
s32 boss4_palette(int model) { return model >= 0 && model < 7 ? s_pal[model] : -1; }
int boss4_palette_flash(int model) { return model >= 0 && model < 7 ? s_pal_flash[model] : 0; }

#define N(f) {(void *)f, #f}
static const struct { void *f; const char *n; } s_names[] = {
    N(Boss4_State_WaitForStart), N(Boss4_State_Fight), N(Boss4_State_DefeatCamera1), N(Boss4_State_DefeatCamera2),
    N(Boss4_State_DefeatCamera3), N(Boss4_State_DefeatFade_ov000_022dd410), N(Boss4_State_DefeatFade_ov000_022dd4d0),
    N(Boss4_State_DefeatFade_ov000_022dd590), N(Boss4_State_DefeatExplode), N(Boss4_State_DefeatFinish),
    N(Boss4_SubState_Spin), N(Boss4_SubState_SpinUp), N(Boss4_SubState_TiltSpin), N(Boss4_SubState_HurtFlash),
    N(Boss4_SubState_ClashFlash), N(Boss4_SubState_SlamStopSpin), N(Boss4_SubState_SlamWaitAnim),
    N(Boss4_SubState_SlamTiltUp), N(Boss4_SubState_SlamHold), N(Boss4_SubState_SlamDown), N(Boss4_SubState_SlamShake),
    N(Boss4_SubState_SlamRecover),
};
#undef N
static const char *name_of(const void *f) {
    unsigned i;
    if (f == NULL)
        return "-";
    for (i = 0; i < sizeof(s_names) / sizeof(s_names[0]); i++)
        if (s_names[i].f == f)
            return strchr(s_names[i].n, '_') + 1;
    return "?";
}
const char *boss4_state_name(int part) {
    if (!s_boss4.alive)
        return "-";
    if (part == 0)
        return name_of((const void *)s_boss4.func);
    if (part == 1)
        return name_of((const void *)s_boss4.unk220);
    return "-";
}

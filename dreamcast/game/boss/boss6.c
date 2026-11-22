/*
 * Boss6 (Altitude Limit act 3, stage z63, stage id 17): native translation of
 * src/game/Zone/Altitude_Limit/Boss/ (Boss6_*, Boss6PartD4_*, BossStage6_*):
 * all 100 functions (none is kept as an ARM listing).
 *
 * The fight takes place on a flying deck (x 0..832 px, deck surface y = 512,
 * the base model bs6_base at (416, -512, 0)).  The boss (bs6_body, map object
 * 0x3a) walks on the deck and picks one attack after a short walk:
 *
 *   1 (phase 0x1e) PartD4: it fires bullets (Boss6PartD4, map object 0xd4,
 *     bs6_bult) at the camera while walking; the bullets' 2D hit boxes sit
 *     beside / above it.
 *   2 (phase 0x1f) pick: it stops near the player, raises and aims its pick,
 *     swings it into the deck (stuns a grounded player), and pulls it out.
 *     While pulling out, its core (node "body_core") is the damage box: a hit
 *     turns it round (Boss6_OnHit_DamageFlip, the camera goes to the other
 *     side, unkD5E toggles).
 *   3 (phase 0x20) tilt: it jumps to the middle and lands on one end; the deck
 *     tilts and the player slides towards the low end.  The player escapes by
 *     mashing A / B (Boss6_ApplyTiltToPlayer reads gPad directly and drives
 *     the player through Player_Action_GroundMove / KnockbackIntangible).
 *     The BossHelp prompt (g_boss_hooks.help) is shown meanwhile.
 *   4 (phase 0x21) edge jumps: it jumps to one edge, then hops 3 times at
 *     the player; after each landing its core is the damage box
 *     (Boss6_OnHit_Damage).
 *
 * Every function keeps its original name and body.  The per-file Unk structs
 * are merged with the original offsets in the field names; the models are
 * unk210[9] (0x138 each): [0] base @0x210, [1] rcloud01, [2] rcloud02,
 * [3] tcloud01, [4] tcloud02, [5] sky, [6] body @0x960 (rot @0x984,
 * render object @0x9F0, flags[0] @0xA6C), [7] prop01, [8] prop02.
 * DS services are replaced as in boss4.c:
 *   - node callbacks (Boss6_NodeCallback_*, Boss6_StoreNodeMatrix) run per joint in
 *     the SBC NODEDESC order after the pose; G3_MultMtx33 of the mirror matrix
 *     at the first NODEDESC becomes a joint rotation of joint 0
 *     (boss_model_set_joint_rot); MTX_STORE -> g_boss_mtx_slot
 *   - palette swaps (Boss_LoadModelPalettes) -> selection state
 *   - NNS_G3dMdlSetMdlAlphaAll -> material alpha of the converted model
 *   - material / pattern animations of the pieces (eff_plate / eff_shoot /
 *     eff_shoot_bomb .nsbma / .nsbtp) -> timing-only animation objects
 *   - BossHelp_Create / BossHelp_Destroy -> g_boss_hooks.help
 *   - gPad.trg (the raw pressed keys) -> gPlayers->pad_raw[1] (what the player
 *     module received this frame)
 *   - display registers (POWCNT swap, BG2 cut-in) -> boss_exc_background() /
 *     boss_brightness() state; the POWCNT screen swap is dropped (single screen)
 */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "boss_internal.h"

void rect_set_box2d(ObjBox *b, s16 l, s16 t, s16 r, s16 bb); /* ObjRect_SetBox2D */

typedef struct Boss6 Boss6;
typedef struct Boss6PartD4 Boss6PartD4;
typedef struct Boss6Piece Boss6Piece;

/* NNSG3dRS subset seen by the node callbacks: current node (@0xAE, valid with
 * flag 0x10), the render object, and the callback reset (@0x24 / @0x92) */
typedef struct Boss6NodeRS {
    BossModel *m;
    int node;
    int done;                 /* rs->cbFunc / cbTiming cleared for the rest of this draw */
} Boss6NodeRS;
typedef void (*Boss6NodeCb)(Boss6NodeRS *rs);

struct Boss6 {                /* 0x1130 bytes (Boss6_Create) */
    DcPlayer h;
    BossFn func;              /* @0x68 */
    ObjBox coll0, coll1, coll2; /* @0x134 MapObject_Init boxes (templates of unkD60) */
    u32 unk20C;               /* @0x20C 1 arena not shown yet, 4 pick boxes, 8 tilt boxes, 0x10 jump boxes,
                                 0x100 tilt towards the left end, 0x200 hurt, 0x1000 / 0x2000 / 0x4000 pick box
                                 requests */
    BossModel unk210[9];      /* @0x210 base, rcloud01, rcloud02, tcloud01, tcloud02, sky, body, prop01, prop02 */
    const BossAnimRes *unkD08;    /* @0xD08 bs6.nsbca (file 8) */
    const BossModelRes *unkD0C[4]; /* @0xD0C piece models (data_ov000_02363864[i][0]) */
    const BossAnimRes *unkD1C[4];  /* @0xD1C piece animation slot 0 */
    const BossAnimRes *unkD2C[4];  /* @0xD2C slot 1 */
    const BossAnimRes *unkD3C[4];  /* @0xD3C slot 2 */
    s16 unkD4C;               /* @0xD4C attack pattern column */
    s16 unkD4E;               /* @0xD4E attack pattern row */
    u16 unkD50, unkD52;       /* @0xD50 rcloud01 / rcloud02 angles */
    s32 unkD54[2];            /* @0xD54 tcloud scroll speeds (unkD54 / unkD58) */
    u16 unkD5C;               /* @0xD5C tcloud shown (0 / 1) */
    u16 unkD5E;               /* @0xD5E turned round (camera on the other side) */
    ObjBox unkD60[11];        /* @0xD60 hit boxes (Boss6_InitHitboxes) */
    u16 unkF44;               /* @0xF44 walks */
    u16 unkF46;               /* @0xF46 next attack (1..4) */
    u16 unkF48;               /* @0xF48 Boss6_SelectNextAttack mode */
    s16 unkF4A;               /* @0xF4A lean angle (RotZ) */
    s16 unkF4C;               /* @0xF4C turn angle (RotY) */
    s32 unkF50;               /* @0xF50 timer */
    s32 unkF54, unkF58, unkF5C; /* @0xF54 walk acceleration / speed / target speed */
    s16 unkF60, unkF64;       /* @0xF60 lean step / target */
    s16 unkF66, unkF68, unkF6A, unkF6C; /* @0xF66 propeller angle, speed, speed step, target speed */
    Boss6PartD4 *unkF70[10];  /* @0xF70 bullets fired (ring) */
    s32 unkF98;               /* @0xF98 ring index */
    fx32 unk1044[12];         /* @0x1044 "pelvis" matrix (MtxFx43; unk1068 / unk106C / unk1070 translation) */
    fx32 unk1074[9];          /* @0x1074 its direction matrix */
    fx32 unk1098[12];         /* @0x1098 "body_core" matrix (unk10BC / unk10C0 / unk10C4) */
    fx32 unk10C8[9];          /* @0x10C8 */
    u16 unk10EC;              /* @0x10EC node callback state */
    const u16 *unk10F0;       /* @0x10F0 palette indices (per player slot, see Boss6_Create) */
    const u16 *unk10F4, *unk10F8, *unk10FC, *unk1100, *unk1104, *unk1108, *unk110C, *unk1110;
    s32 unk1114;              /* @0x1114 core flash counter */
    s32 unk1118, unk111C, unk1120, unk1124, unk1128, unk112C; /* @0x1118 state work (s16 in some states) */
    DcpFn unk111C_fn;         /* @0x111C the player state saved by Boss6_Action_StunPlayer (shares the s32) */
    Boss6NodeCb cb6;          /* NNS_G3dRenderObjSetCallBack of the body (unk9F0) */
    int alive;
};

struct Boss6PartD4 {          /* 0x36C bytes (Boss6PartD4_Create) */
    DcPlayer h;
    BossFn func;              /* @0x68 = Boss6PartD4_State_Fly */
    ObjBox coll0, coll1, coll2; /* @0x134 / @0x160 / @0x18C */
    u32 unk1F4;               /* @0x1F4 StageObj flags (|= 0x10 at the end; no native reader) */
    BossModel unk20C;         /* @0x20C bullet (bs6_bult), rot @0x230 */
    const BossAnimRes *unk344; /* @0x344 "bs6" file 8 (not used) */
    s32 unk348, unk34C, unk350; /* @0x348 z speed, acceleration, maximum */
    s32 unk354;               /* @0x354 y speed */
    s32 unk360;               /* @0x360 z offset of the hit test */
    s32 unk364;               /* @0x364 frames flown */
    u32 unk368;               /* @0x368 bit0: hit the player */
    int alive;
};

struct Boss6Piece {           /* Boss6_CreatePiece (task 0xa00) */
    DcPlayer h;               /* unk8 = h.trick_input (speed), unkC = h.timer (frames) */
    BossFn func;              /* @0x68 data_ov000_023658ac[k] */
    const BossModel *unk124;  /* @0x124 = &unk134 */
    BossModel unk134;         /* @0x134 model (rot @0x158) */
    int alive;
};

#define B6_D4 8
#define B6_PIECES 24
static Boss6 s_boss6;
static Boss6PartD4 s_d4[B6_D4];
static Boss6Piece s_piece[B6_PIECES];
static Boss6 *gBoss6;

/* BossStage6 resources: data_ov000_02369b50[10] = bs6_base, rcloud01, rcloud02, tcloud01, tcloud02, sky,
 * body, prop01, prop02, bult (data_020746c0); archive files of z63_act_lz7.narc */
static BossModelRes data_ov000_02369b50[10];
static BossAnimRes s_bs6_ca;                              /* file 8 bs6.nsbca */
static BossModelRes s_plate, s_wind, s_shoot, s_bomb;     /* files 0x35 0x37 0x38 0x3a */
static BossAnimRes s_plate_ma, s_shoot_ma, s_bomb_ca, s_bomb_ma, s_bomb_tp; /* 0x36 0x39 0x3b 0x3c 0x3d */
static int s_joint[4];        /* body_pro_r, body_pro_l, pelvis, body_core (bs6_body) */
static s32 s_pal[10];         /* native: Boss_LoadModelPalettes state (first palette file per model) */
static s32 s_pal_core;        /* the body's single-slot palette (bod_07: normal / white flash) */
static dcm_mtx_t s_prop_mtx[2]; /* native: the two Model_Draw of the propeller (slots 0x1e / 0x1d) */
static int s_prop_n, s_base_shown, s_body_shown, s_cloud_shown;
/* native statistics for the host test */
static Boss6Info s_info;

#define data_ov000_023636e8 BOSS_DATA(0x023636e8, const u16 *)
#define data_ov000_023636ec BOSS_DATA(0x023636ec, const u16 *)
#define data_ov000_023636f0 BOSS_DATA(0x023636f0, const u16 *)
#define data_ov000_023636f4 BOSS_DATA(0x023636f4, const u16 *)
#define data_ov000_023636f8 BOSS_DATA(0x023636f8, const u16 *)
#define data_ov000_023636fc BOSS_DATA(0x023636fc, const u16 *)
#define data_ov000_02363700 BOSS_DATA(0x02363700, const u16 *)
#define data_ov000_02363704 BOSS_DATA(0x02363704, const u16 *)
#define data_ov000_02363708 BOSS_DATA(0x02363708, const u16 *)
#define data_ov000_0236370c BOSS_DATA(0x0236370c, const u16 *)
#define data_ov000_02363710 BOSS_DATA(0x02363710, const u16 *)
#define data_ov000_02363714 BOSS_DATA(0x02363714, const u16 *)
#define data_ov000_02363718 BOSS_DATA(0x02363718, const s32 *)
#define data_ov000_02363720 BOSS_DATA(0x02363720, const s32 *)
#define data_ov000_0236372c BOSS_DATA(0x0236372c, const s16(*)[3])
#define data_ov000_02363738 BOSS_DATA(0x02363738, const u32 *)
#define data_ov000_02363744 BOSS_DATA(0x02363744, const u16(*)[3])
#define data_ov000_02363750 BOSS_DATA(0x02363750, const s32 *)
#define data_ov000_0236375c BOSS_DATA(0x0236375c, const s32 *)
#define data_ov000_0236377c BOSS_DATA(0x0236377c, const u16 *)
#define data_ov000_023637ac BOSS_DATA(0x023637ac, const u32 *)
#define data_ov000_023637bc BOSS_DATA(0x023637bc, const s32 *)
#define data_ov000_023637dc BOSS_DATA(0x023637dc, const s32 *)
#define data_ov000_023637ec BOSS_DATA(0x023637ec, const s32(*)[3])
#define data_ov000_02363804 BOSS_DATA(0x02363804, const s32 *)
#define data_ov000_02363824 BOSS_DATA(0x02363824, const s32 *)
#define data_ov000_02363844 BOSS_DATA(0x02363844, const s32 *)
#define data_ov000_02363864 BOSS_DATA(0x02363864, const s16(*)[4])
#define data_ov000_02363884 BOSS_DATA(0x02363884, const u16(*)[8])
#define data_ov000_023638a4 BOSS_DATA(0x023638a4, const u16(*)[8])
#define data_ov000_023638c4 BOSS_DATA(0x023638c4, const MtxFx33 *)
#define data_ov000_023638e8 BOSS_DATA(0x023638e8, const s32(*)[3])
#define data_ov000_023638ec BOSS_DATA(0x023638ec, const s32(*)[3])
#define data_ov000_023638f0 BOSS_DATA(0x023638f0, const s32(*)[3])
#define data_ov000_02363918 BOSS_DATA(0x02363918, const u16(*)[8])
#define data_ov000_02363958 BOSS_DATA(0x02363958, const s32(*)[8])
#define data_ov000_02363998 BOSS_DATA(0x02363998, const u16(*)[8])
#define data_ov000_023639d8 BOSS_DATA(0x023639d8, const u16(*)[8])
#define data_ov000_02363a18 BOSS_DATA(0x02363a18, const s32(*)[8][4])
#define data_ov000_023657e0 BOSS_DATA(0x023657e0, const u32 *)
#define data_ov000_023657e4 BOSS_DATA(0x023657e4, const u32 *)
#define data_ov000_023657e8 BOSS_DATA(0x023657e8, const u32 *)
#define data_ov000_023657ec BOSS_DATA(0x023657ec, const u32 *)
#define data_ov000_023658ac BOSS_DATA(0x023658ac, const u32 *)
#define data_ov000_023658bc BOSS_DATA(0x023658bc, const u32 *)
#define data_ov000_023658cc BOSS_DATA(0x023658cc, const s32(*)[3])
#define data_ov000_023658d0 BOSS_DATA(0x023658d0, const s32(*)[3])
#define data_ov000_023658d4 BOSS_DATA(0x023658d4, const s32(*)[3])
#define data_ov000_023658e4 BOSS_DATA(0x023658e4, const u32 *)
#define ROM_STR(ptr) BOSS_DATA((ptr), const char *)

#define CAM data_ov000_0236a444
#define gGameState (g_boss_game.unk0) /* gGameState.unk0: player slot (palette tables) */
/* data_ov000_0236a48c = &CAM.t[0]: c->unk30 / unk6C / unk80 are u16 (low half of the fx32 field) */
#define CAM_LO16(f, v) ((f) = (s32)(((u32)(f) & ~0xffffu) | (u16)(v)))
/* merged field views (original offsets) */
#define unk960 unk210[6]
#define unk984 unk210[6].rot
#define unkA6C unk210[6].flags[0]
#define unkD58 unkD54[1]
#define unkE68 unkD60[6]
#define unkEC0 unkD60[8]
#define unkD6A unkD60[0].flags
#define unkD96 unkD60[1].flags
#define unkDC2 unkD60[2].flags
#define unkDEE unkD60[3].flags
#define unkE1A unkD60[4].flags
#define unkE46 unkD60[5].flags
#define unkE72 unkD60[6].flags
#define unkE9E unkD60[7].flags
#define unkECA unkD60[8].flags
#define unkEF6 unkD60[9].flags
#define unkF22 unkD60[10].flags
#define unk1068 unk1044[9]
#define unk106C unk1044[10]
#define unk1070 unk1044[11]
#define unk10BC unk1098[9]
#define unk10C0 unk1098[10]
#define unk10C4 unk1098[11]

static inline s32 GetIdx(void) {
    dcp_bool b = DCP_TRUE;
    if (g_dcp_world.mode != 2 && g_dcp_world.mode != 3)
        b = DCP_FALSE;
    if (b)
        return 1;
    return g_boss_game.unk14;
}

static void mtx_to_fx43(const dcm_mtx_t *m, fx32 *out) {
    int r, c;
    for (r = 0; r < 4; r++)
        for (c = 0; c < 3; c++) {
            float v = m->m[r][c] * 4096.0f;
            out[r * 3 + c] = (fx32)(v >= 0 ? v + 0.5f : v - 0.5f);
        }
}

/* NNS_G3dGetCurrentMtx after MTX_RESTORE of a stored node matrix: position matrix (MtxFx43) and
 * direction matrix (rotation rows normalised; no native reader) */
static void GetCurrentMtx(int slot, fx32 *pos, fx32 *dir) {
    int r, c;
    mtx_to_fx43(&g_boss_mtx_slot[slot & 31], pos);
    for (r = 0; r < 3; r++) {
        const float *v = g_boss_mtx_slot[slot & 31].m[r];
        float l = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
        float inv = l > 0 ? 1.0f / sqrtf(l) : 0;
        for (c = 0; c < 3; c++)
            dir[r * 3 + c] = (fx32)(v[c] * inv * 4096.0f);
    }
}

/* MTX_MultVec33 (NitroSDK: row vector, fx64 sum >> 12) */
static void MTX_MultVec33(const BossVec *v, const MtxFx33 *m, BossVec *out) {
    s32 x = (s32)(((s64)v->x * m->m[0] + (s64)v->y * m->m[3] + (s64)v->z * m->m[6]) >> 12);
    s32 y = (s32)(((s64)v->x * m->m[1] + (s64)v->y * m->m[4] + (s64)v->z * m->m[7]) >> 12);
    s32 z = (s32)(((s64)v->x * m->m[2] + (s64)v->y * m->m[5] + (s64)v->z * m->m[8]) >> 12);
    out->x = x;
    out->y = y;
    out->z = z;
}

/* MTX_Inverse33 (NitroSDK: cofactors, determinant, FX_InvAsync / FX_GetDivResult) */
static int MTX_Inverse33(const MtxFx33 *s, MtxFx33 *d) {
    const fx32 *a = s->m;
    fx32 det00, det10, det20, det, t01, t02, t11, t12, t21, t22, w;
    MtxFx33 p;
    det00 = (fx32)(((s64)a[4] * a[8] - (s64)a[5] * a[7] + 0x800) >> 12);
    det10 = (fx32)(((s64)a[3] * a[8] - (s64)a[5] * a[6] + 0x800) >> 12);
    det20 = (fx32)(((s64)a[3] * a[7] - (s64)a[4] * a[6] + 0x800) >> 12);
    det = (fx32)(((s64)a[0] * det00 - (s64)a[1] * det10 + (s64)a[2] * det20 + 0x800) >> 12);
    if (det == 0)
        return -1;
    t01 = (fx32)(((s64)a[1] * a[8] - (s64)a[7] * a[2]) >> 12);
    t02 = (fx32)(((s64)a[1] * a[5] - (s64)a[4] * a[2]) >> 12);
    t11 = (fx32)(((s64)a[0] * a[8] - (s64)a[6] * a[2]) >> 12);
    t12 = (fx32)(((s64)a[0] * a[5] - (s64)a[3] * a[2]) >> 12);
    t21 = (fx32)(((s64)a[0] * a[7] - (s64)a[6] * a[1]) >> 12);
    t22 = (fx32)(((s64)a[0] * a[4] - (s64)a[3] * a[1]) >> 12);
    w = (fx32)((((s64)0x1000 << 32) / det + (1 << 19)) >> 20);
    p.m[0] = (fx32)(((s64)w * det00) >> 12);
    p.m[1] = -(fx32)(((s64)w * t01) >> 12);
    p.m[2] = (fx32)(((s64)w * t02) >> 12);
    p.m[3] = -(fx32)(((s64)w * det10) >> 12);
    p.m[4] = (fx32)(((s64)w * t11) >> 12);
    p.m[5] = -(fx32)(((s64)w * t12) >> 12);
    p.m[6] = (fx32)(((s64)w * det20) >> 12);
    p.m[7] = -(fx32)(((s64)w * t21) >> 12);
    p.m[8] = (fx32)(((s64)w * t22) >> 12);
    *d = p;
    return 0;
}

/* NNS_G3dMdlSetMdlAlphaAll on a converted model: every material's polygon alpha (0..31).  A value
 * below 31 sends the materials to the translucent list (the DS draws alpha 0 as wireframe; here
 * it is fully transparent). */
static void boss6_set_mdl_alpha(const BossModelRes *res, u32 alpha) {
    dcm_material_t *mat;
    u32 i;
    if (res == NULL || !res->loaded)
        return;
    mat = (dcm_material_t *)res->mdl.materials;
    for (i = 0; i < res->mdl.hdr->num_materials; i++) {
        mat[i].alpha = (u8)(alpha & 31);
        if (mat[i].alpha < 31)
            mat[i].flags |= DCM_MF_TRANSLUCENT;
    }
}

/* Boss_LoadModelPalettes: selection state only (pre-decoded textures) */
static void Boss_LoadModelPalettes(int model, const u32 *names, const u16 *idx, const u16 *slots, u16 n) {
    (void)names;
    (void)slots;
    if (idx == NULL || model < 0 || model >= 10)
        return;
    if (model == 6 && n == 1)
        s_pal_core = idx[0];
    else
        s_pal[model] = idx[0];
}

/* Player_State_Blank / Player_Action_Blank (player module lacks them; copied) */
static void Player_State_Blank(DcPlayer *p) { (void)p; }
static void Player_Action_Blank(DcPlayer *s) {
    dcp_player_init_state(s);
    dcp_reset_gimmick_state(s);
    s->state = Player_State_Blank;
}
/* Player_Action_KnockbackIntangible (player module lacks it; copied as in boss1.c) */
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

#define DECL(f) static void f(void *p);
DECL(Boss6_State_WaitForStart) DECL(Boss6_State_Move) DECL(Boss6_State_PartD4Attack) DECL(Boss6_State_PickApproach)
DECL(Boss6_State_PickStop) DECL(Boss6_State_PickRaise) DECL(Boss6_State_PickAim) DECL(Boss6_State_PickSwing)
DECL(Boss6_State_PickImpact) DECL(Boss6_State_PickPullOut) DECL(Boss6_State_PickTurnAround)
DECL(Boss6_State_TiltStop) DECL(Boss6_State_TiltWaitJump) DECL(Boss6_State_TiltJump) DECL(Boss6_State_TiltLand)
DECL(Boss6_State_TiltStart) DECL(Boss6_State_Tilting) DECL(Boss6_State_TiltReturn) DECL(Boss6_State_TiltEnd)
DECL(Boss6_State_WaitSelectAttack3) DECL(Boss6_State_EdgeJumpStop) DECL(Boss6_State_EdgeJumpLeap)
DECL(Boss6_State_EdgeJumpLand) DECL(Boss6_State_EdgeJumpRecover) DECL(Boss6_State_StartJump)
DECL(Boss6_State_JumpRise) DECL(Boss6_State_JumpFall) DECL(Boss6_State_LandShake) DECL(Boss6_State_AfterLand)
DECL(Boss6_State_EndJumps) DECL(Boss6_State_WaitSelectAttack) DECL(Boss6_State_Hurt) DECL(Boss6_State_HurtRecover)
DECL(Boss6_State_HurtWaitSelectAttack) DECL(Boss6_State_HurtFlip) DECL(Boss6_State_Defeat)
DECL(Boss6_State_DefeatFinish) DECL(Boss6PartD4_State_Fly)
DECL(Boss6_State_PieceWaitSpinFly) DECL(Boss6_State_PieceSpinFly) DECL(Boss6_State_PieceStartSlide)
DECL(Boss6_State_PieceSlide) DECL(Boss6_State_PieceDestroyOnAnimEnd)
#undef DECL
static void Boss6_NodeCallback_Store2Nodes_ov000_022e62fc(Boss6NodeRS *p);
static void Boss6_NodeCallback_Store2Nodes_ov000_022e620c(Boss6NodeRS *p);
static void Boss6_NodeCallback_Store4Nodes_ov000_022e5df0(Boss6NodeRS *p);
static void Boss6_NodeCallback_Store4Nodes_ov000_022e60a4(Boss6NodeRS *p);
static void Boss6_NodeCallback_PelvisAndPro(Boss6NodeRS *w);
static void Boss6_OnHit_Damage(ObjBox *a, ObjBox *b);
static void Boss6_OnHit_DamageFlip(ObjBox *a, ObjBox *b);
static void Boss6_OnHit_PushPlayer_ov000_022e08b0(ObjBox *a, ObjBox *b);
static void Boss6_OnHit_PushPlayer_ov000_022e0998(ObjBox *a, ObjBox *b);
static void Boss6_OnHit_PushPlayer_ov000_022e0af8(ObjBox *a, ObjBox *b);
static void Boss6_OnHit_PushPlayer_ov000_022e0ba8(ObjBox *a, ObjBox *b);
static void Boss6_State_PlayerStunned(DcPlayer *s);
static Boss6Piece *Boss6_CreatePiece(s32 k, s32 x, s32 y, s32 z);
static Boss6PartD4 *Boss6PartD4_Create(const ObjParam *p, s32 b, s32 c);
static void Boss6_SelectNextAttack(s32 mode);
static void Boss6_Action_Defeat(void);

/* NNS_G3dRenderObjSetCallBack(body render object, cb, 0, NNS_G3D_SBC_NODEDESC, NNS_G3D_SBC_CALLBACK_TIMING_C) */
static void SetBodyCallBack(Boss6NodeCb cb) { gBoss6->cb6 = cb; }

/* ============================================================ camera */
/* Boss6_ResetCamera (copied; c = data_ov000_0236a48c = &CAM.t[0]) */
static void Boss6_ResetCamera(void) {
    CAM.unkDC = 2;
    CAM.unkC = 0x1a66000;
    CAM.unk14 = FX_Div(0x1000, 0x34cc);
    CAM.t[0].ox = CAM.t[0].oy = CAM.t[0].oz = NULL;        /* c->unk0..8 */
    CAM.t[0].offx = CAM.t[0].offy = CAM.t[0].offz = 0;     /* c->unkC..14 */
    CAM.t[0].tgt.x = CAM.t[0].tgt.y = CAM.t[0].tgt.z = 0;  /* c->unk18..20 */
    CAM_LO16(CAM.t[0].lerp, 0x200);                        /* c->unk30 */
    CAM.t[1].ox = gPlayers;                                /* c->unk3C */
    CAM.t[1].oy = NULL;                                    /* c->unk40 */
    CAM.t[1].oz = gPlayers;                                /* c->unk44 */
    CAM.t[1].tgt.x = 0;                                    /* c->unk54 */
    CAM.t[1].tgt.y = -0x1c0000;
    CAM.t[1].tgt.z = 0;
    CAM.t[1].offx = 0;                                     /* c->unk48 */
    CAM.t[1].offy = 0x28000;
    CAM.t[1].offz = 0;
    CAM_LO16(CAM.t[1].lerp, 0x400);                        /* c->unk6C */
    CAM.unkC0 = CAM.unkC4 = 0xa0000;                       /* c->unk78 / unk7C */
    CAM_LO16(CAM.unkC8, 0x200);                            /* c->unk80 */
    CAM.unkCC = CAM.unkD0 = -0x14000;                      /* c->unk84 / unk88 */
    CAM.unkD4 = 0x80;                                      /* c->unk8C */
    CAM.unkD6 = CAM.unkD8 = 0;                             /* c->unk8E / unk90 */
    CAM.unkDA = 0x100;                                     /* c->unk92 */
    gPlayers->map_up = 0x40;                               /* unk430 */
    gPlayers->map_down = 0x80;                             /* unk432 */
    gPlayers->map_right = 0x10;                            /* unk42C */
    gPlayers->map_left = 0x20;                             /* unk42E */
}

/* Boss6_SetCameraFollowPlayer (copied) */
static void Boss6_SetCameraFollowPlayer(Boss6 *s) {
    CAM.t[1].ox = gPlayers;
    CAM.t[1].oy = NULL;
    CAM.t[1].oz = gPlayers;
    CAM.t[1].tgt.x = 0;
    CAM.t[1].tgt.y = -0x1c0000;
    CAM.t[1].tgt.z = 0;
    CAM_LO16(CAM.t[1].lerp, 0x400);
    CAM.unkC0 = 0xa0000;
    CAM_LO16(CAM.unkC8, 0x200);
    CAM.unkCC = -0x14000;
    CAM.unkD4 = 0x80;
    CAM.unkD6 = 0;
    CAM.unkDA = 0x100;
    if (s->unkD5E != 0)
        CAM.unkD6 += 0x8000;
}

/* Boss6_SetCameraJump (copied; c->unk40 = gPlayers) */
static void Boss6_SetCameraJump(Boss6 *s, s32 a) {
    CAM.t[1].ox = gPlayers;
    CAM.t[1].oy = gPlayers;
    CAM.t[1].oz = gPlayers;
    CAM.unkCC = -0x14000;
    CAM.unkD4 = 0x100;
    CAM.unkC0 = 0xa0000;
    CAM_LO16(CAM.unkC8, 0x100);
    if (a != 0)
        CAM.unkD6 = 0x1c71;
    else
        CAM.unkD6 = 0xe38f;
    if (s->unkD5E != 0)
        CAM.unkD6 += 0x8000;
    CAM.unkDA = 0x200;
}

/* Boss6_SetCameraJumpStart (copied) */
static void Boss6_SetCameraJumpStart(Boss6 *s) {
    (void)s;
    CAM.t[1].ox = gPlayers;
    CAM.t[1].oy = NULL;
    CAM.t[1].oz = gPlayers;
    CAM.t[1].tgt.y = -0xdc000;
    CAM_LO16(CAM.t[1].lerp, 0x100);
    CAM.unkCC = -0x15e000;
    CAM.unkD4 = 0x100;
    CAM.unkC0 = 0x258000;
    CAM_LO16(CAM.unkC8, 0x100);
}

/* Boss6_SetCameraEdgeJump (copied) */
static void Boss6_SetCameraEdgeJump(Boss6 *s, s32 a) {
    CAM.t[1].ox = gPlayers;
    CAM.t[1].oy = gPlayers;
    CAM.t[1].oz = gPlayers;
    CAM.t[1].tgt.x = 0;
    CAM.t[1].tgt.y = -0x1c0000;
    CAM.t[1].tgt.z = 0;
    CAM.t[1].offx = 0;
    CAM.t[1].offy = 0x28000;
    CAM.t[1].offz = 0;
    CAM_LO16(CAM.t[1].lerp, 0x400);
    CAM.unkCC = -0x14000;
    if (a != 0)
        CAM.unkD6 = 0x1c71;
    else
        CAM.unkD6 = 0xe38f;
    if (s->unkD5E != 0)
        CAM.unkD6 += 0x8000;
    CAM.unkDA = 0x200;
}

/* Boss6_SetCameraAngle (copied) */
static void Boss6_SetCameraAngle(Boss6 *p, s32 b) {
    if (b)
        CAM.unkD6 = 0x1c71;
    else
        CAM.unkD6 = 0xe38f;
    if (p->unkD5E != 0)
        CAM.unkD6 += 0x8000;
    CAM.unkDA = 0x200;
}

/* Boss6_StartTiltCamera (modified: BossHelp_Create via the hook) */
static void Boss6_StartTiltCamera(Boss6 *s) {
    if (s->unk20C & 0x100)
        CAM.unkD6 += 0x38e3;
    else
        CAM.unkD6 -= 0x38e3;
    CAM.unkDA = 0x200;
    gPlayers->map_a = 0;       /* unk434 */
    gPlayers->map_b = 0;       /* unk436 */
    gPlayers->map_up = 0;      /* unk430 */
    gPlayers->map_down = 0;    /* unk432 */
    gPlayers->map_right = 0;   /* unk42C */
    gPlayers->map_left = 0;    /* unk42E */
    gPlayers->released = 0;    /* unk424 */
    gPlayers->pressed = gPlayers->released;
    gPlayers->held = gPlayers->pressed;
    BossHelp_Create(0);
}

/* Boss6_EndTiltCamera (modified: BossHelp_Destroy via the hook) */
static void Boss6_EndTiltCamera(Boss6 *s) {
    if (s->unk20C & 0x100)
        CAM.unkD6 -= 0x38e3;
    else
        CAM.unkD6 += 0x38e3;
    gPlayers->map_a = 1;
    gPlayers->map_b = 2;
    gPlayers->map_up = 0x40;
    gPlayers->map_down = 0x80;
    if (s->unkD5E != 0) {
        gPlayers->map_right = 0x20;
        gPlayers->map_left = 0x10;
    } else {
        gPlayers->map_right = 0x10;
        gPlayers->map_left = 0x20;
    }
    CAM.unkDA = 0x100;
    BossHelp_Destroy();
}

/* Boss6_SetCameraPickStart (copied: camera mode 1, eye follows the player, 'at' the boss) */
static void Boss6_SetCameraPickStart(Boss6 *s) {
    CAM.unkDC = 1;
    CAM.t[0].ox = gPlayers;                          /* c->unk0 */
    CAM.t[0].tgt.y = -0x1f0000;                      /* c->unk1C */
    CAM_LO16(CAM.t[0].lerp, 0x200);
    CAM.t[1].oz = NULL;                              /* c->unk3C = (c->unk40 = (c->unk44 = 0)) */
    CAM.t[1].oy = NULL;
    CAM.t[1].ox = &s->h;                             /* c->unk3C = s */
    CAM.t[1].tgt.y = -0x19c000;                      /* c->unk58 */
    CAM_LO16(CAM.t[1].lerp, 0x200);
    if (s->unkD5E != 0) {
        CAM.t[0].tgt.z = -0x64000;                   /* c->unk20 */
        CAM.t[1].tgt.z = 0xc8000;                    /* c->unk5C */
    } else {
        CAM.t[0].tgt.z = 0x64000;
        CAM.t[1].tgt.z = -0xc8000;
    }
}

/* Boss6_SetCameraPickSwing (copied) */
static void Boss6_SetCameraPickSwing(Boss6 *s) {
    (void)s;
    CAM.unkDC = 2;
    CAM.t[0].ox = CAM.t[0].oy = CAM.t[0].oz = NULL;
    CAM.t[0].tgt.x = CAM.t[0].tgt.y = CAM.t[0].tgt.z = 0;
    CAM.t[1].ox = gPlayers;
    CAM.t[1].oy = NULL;
    CAM.t[1].oz = gPlayers;
    CAM.t[1].tgt.x = 0;
    CAM.t[1].tgt.y = -0x1c0000;
    CAM.t[1].tgt.z = 0;
    CAM_LO16(CAM.t[0].lerp, 0x400);
    CAM_LO16(CAM.t[1].lerp, 0x400);
}

/* Boss6_SetCameraPickPullOut (copied) */
static void Boss6_SetCameraPickPullOut(Boss6 *s) {
    CAM.unkDC = 2;
    CAM.t[0].ox = CAM.t[0].oy = CAM.t[0].oz = NULL;
    CAM.t[0].tgt.x = CAM.t[0].tgt.y = CAM.t[0].tgt.z = 0;
    CAM_LO16(CAM.t[0].lerp, 0x200);
    CAM.t[1].ox = gPlayers;
    CAM.t[1].oy = NULL;
    CAM.t[1].oz = gPlayers;
    CAM.t[1].tgt.x = 0;
    CAM.t[1].tgt.y = -0x1c0000;
    CAM.t[1].tgt.z = 0;
    CAM_LO16(CAM.t[1].lerp, 0x400);
    CAM.unkD6 = (u16)((s->unkD5E ^ 1) << 15);
    CAM.unkDA = 0x400;
    if (s->unkD5E != 0) {
        gPlayers->map_right = 0x10;
        gPlayers->map_left = 0x20;
    } else {
        gPlayers->map_right = 0x20;
        gPlayers->map_left = 0x10;
    }
}

/* Boss6_SetCameraSideShot (copied) */
static void Boss6_SetCameraSideShot(Boss6 *s) {
    CAM.unkDC = 1;
    CAM.t[0].ox = gPlayers;
    CAM.t[0].tgt.y = -0x200000;
    CAM_LO16(CAM.t[0].lerp, 0x200);
    CAM.t[1].oz = NULL;
    CAM.t[1].oy = NULL;
    CAM.t[1].ox = gPlayers;
    CAM.t[1].tgt.y = -0x174000;
    CAM_LO16(CAM.t[1].lerp, 0x200);
    if (s->unkD5E == 0) {
        CAM.t[0].tgt.z = -0x64000;
        CAM.t[1].tgt.z = 0xc8000;
    } else {
        CAM.t[0].tgt.z = 0x64000;
        CAM.t[1].tgt.z = -0xc8000;
    }
}

/* Boss6_SetCameraAfterFlip (copied) */
static void Boss6_SetCameraAfterFlip(Boss6 *s) {
    (void)s;
    CAM.unkDC = 2;
    CAM.t[0].ox = CAM.t[0].oy = CAM.t[0].oz = NULL;
    CAM.t[0].tgt.x = CAM.t[0].tgt.y = CAM.t[0].tgt.z = 0;
    CAM_LO16(CAM.t[0].lerp, 0x100);
    CAM.t[1].ox = gPlayers;
    CAM.t[1].oy = NULL;
    CAM.t[1].oz = gPlayers;
    CAM.t[1].tgt.x = 0;
    CAM.t[1].tgt.y = -0x1c0000;
    CAM.t[1].tgt.z = 0;
    CAM_LO16(CAM.t[1].lerp, 0x400);
    CAM_LO16(CAM.t[1].lerp, 0x400);
    CAM.unkDA = 0x100;
}

/* Boss6_SetDefeatCameraShot (modified: Background_Init / Background_Draw of the cut-in BBG
 * (bs6 file data_ov000_02363738[b]) -> Boss_DrawExcBackground state) */
static void Boss6_SetDefeatCameraShot(s32 a, s32 b) {
    Boss_DrawExcBackground(data_ov000_02363738[b]);
    CAM.unkC0 = CAM.unkC4 = data_ov000_023637ec[a][b];        /* p->unk78 / unk7C */
    CAM.unkCC = CAM.unkD0 = data_ov000_0236372c[a][b];        /* p->unk84 / unk88 */
    CAM.unkD6 = CAM.unkD8 = data_ov000_02363744[a][b];        /* p->unk8E / unk90 */
}

/* ============================================================ palettes */
/* Boss6_UpdateFlashPalettes (copied; templates.py pointer code written with the field names) */
static void Boss6_UpdateFlashPalettes(s32 p0) {
    if ((p0 & 3) != 0)
        return;
    if ((p0 & 4) != 0) {
        Boss_LoadModelPalettes(6, data_ov000_023658e4, gBoss6->unk10F4, data_ov000_0236377c, 8);
        Boss_LoadModelPalettes(7, data_ov000_023657e8, gBoss6->unk10FC, data_ov000_023636e8, 1);
        Boss_LoadModelPalettes(8, data_ov000_023657ec, gBoss6->unk1104, data_ov000_023636f0, 1);
        return;
    }
    Boss_LoadModelPalettes(6, data_ov000_023658e4, gBoss6->unk10F8, data_ov000_0236377c, 8);
    Boss_LoadModelPalettes(7, data_ov000_023657e8, gBoss6->unk1100, data_ov000_023636e8, 1);
    Boss_LoadModelPalettes(8, data_ov000_023657ec, gBoss6->unk1108, data_ov000_023636f0, 1);
}

/* ============================================================ movement helpers */
/* Boss6_UpdateMoveSpeed (copied) */
static void Boss6_UpdateMoveSpeed(Boss6 *w) {
    w->unkF4A = (s16)(w->unkF4A + w->unkF60);
    if (w->unkF60 >= 0) {
        if (w->unkF64 < w->unkF4A)
            w->unkF4A = w->unkF64;
    } else {
        if (w->unkF64 > w->unkF4A)
            w->unkF4A = w->unkF64;
    }
    w->unkF58 += w->unkF54;
    if (w->unkF54 >= 0) {
        if (w->unkF58 > w->unkF5C)
            w->unkF58 = w->unkF5C;
    } else {
        if (w->unkF58 < w->unkF5C)
            w->unkF58 = w->unkF5C;
    }
}

/* Boss6_ScrollFadeModel (modified: NNS_G3dMdlSetMdlAlphaAll -> boss6_set_mdl_alpha) */
static void Boss6_ScrollFadeModel(Boss6 *w) {
    u16 i = w->unkD5C;
    BossModel *s = &w->unk210[i + 3];
    s32 d;
    s->trans[0] += w->unkD54[i];
    if (w->unkD54[i] > 0) {
        if (s->trans[0] > 0x1a0000) {
            s->trans[0] = -0x23df10;
            w->unkD5C ^= 1;
        }
    } else if (s->trans[0] < -0x23df10) {
        s->trans[0] = 0x1a0000;
        w->unkD5C ^= 1;
    }
    d = s->trans[0] - 0x1a0000;
    d += 0x1eef88;
    if (d < 0)
        d = -d;
    d = 0x1eef88 - d;
    if (d < 0x10df4c)
        boss6_set_mdl_alpha(&data_ov000_02369b50[3 + i], (u32)((d * 0x1f / 0x10df4c) & 0x1f) * 6 / 10);
}

/* Boss6_Unk_ov000_022e1f10 (copied): cloud scroll speed from the propeller speed */
static void Boss6_Unk_ov000_022e1f10(Boss6 *p) {
    s32 v;
    s32 d;
    v = p->unkF68;
    if (v < 0)
        v = -v;
    d = v * 2;
    if (p->unkD5E != 0) {
        if (gBoss6->unk20C & 0x100)
            d = -d;
    } else {
        if (!(gBoss6->unk20C & 0x100))
            d = -d;
    }
    p->unkD54[0] = d - 0x17ff;
    p->unkD58 = d + 0x17ff;
}

/* ============================================================ hit boxes */
/* MapObject_Init (obj_manager.c obj_mapobject_init) fields used */
static void MapObject_Init6(DcPlayer *h, ObjBox *coll, const ObjParam *prm, s32 x, s32 y) {
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

/* Boss6_InitHitboxes (copied; boxes are owned by the boss, ObjRect_Register from Boss6_Draw) */
static void Boss6_InitHitboxes(Boss6 *w) {
    ObjBox *c;
    int i;
    w->h.x = 0x1a000;
    w->h.y = 0x20000;
    w->h.z = 0;
    c = &w->unkD60[0];
    *c = w->coll0;
    rect_set_box3d(c, -13, -10, 13, 12, -0x7d00, 0x7d00);
    c->owner = w;
    c->cb = Boss6_OnHit_DamageFlip;
    c->flags &= ~4;
    c = &w->unkD60[1];
    *c = w->coll0;
    rect_set_box3d(c, -28, -10, 18, 12, -0x7d00, 0x7d00);
    c->owner = w;
    c->cb = Boss6_OnHit_Damage;
    c->flags &= ~4;
    c = &w->unkD60[2];
    *c = w->coll1;
    rect_set_box3d(c, -20, 0, 20, 48, -0x7d00, 0x7d00);
    c->owner = w;
    c->flags &= ~4;
    c = &w->unkD60[3];
    *c = w->coll1;
    rect_set_box3d(c, -60, -20, 60, 48, -0x7d00, 0x7d00);
    c->owner = w;
    c->flags &= ~4;
    c = &w->unkD60[4];
    *c = w->coll1;
    rect_set_box3d(c, -28, 30, 48, 100, -0x7d00, 0x7d00);
    c->owner = w;
    c->flags &= ~4;
    c = &w->unkD60[5];
    *c = w->coll1;
    rect_set_box3d(c, -40, 6, 28, 28, -0x7d00, 0x7d00);
    c->owner = w;
    c->flags &= ~4;
    c = &w->unkD60[6];
    *c = w->coll1;
    rect_set_box3d(c, -30, 0, 48, 112, -0x7d00, 0x7d00);
    c->atk |= 0x20;
    c->owner = w;
    c->flags &= ~4;
    c = &w->unkD60[7];
    rect_set_box3d(c, -18, 8, 18, 50, -0x7d00, 0x7d00);
    rect_set_atkdef(c, 0, (u32)-16);
    c->flags |= 0xc0;
    c->mask = 0x12;
    c->owner = w;
    c->cb = Boss6_OnHit_PushPlayer_ov000_022e0ba8;
    c->flags &= ~4;
    c = &w->unkD60[8];
    rect_set_box3d(c, -35, -16, -20, 92, -0x7d00, 0x7d00);
    rect_set_atkdef(c, 0, (u32)-16);
    c->flags |= 0xc0;
    c->mask = 0x12;
    c->owner = w;
    c->cb = Boss6_OnHit_PushPlayer_ov000_022e0af8;
    c->flags &= ~4;
    c = &w->unkD60[9];
    rect_set_box3d(c, -28, -30, 48, 55, -0x7d00, 0x7d00);
    rect_set_atkdef(c, 0, (u32)-16);
    c->flags |= 0xc0;
    c->mask = 0x12;
    c->owner = w;
    c->cb = Boss6_OnHit_PushPlayer_ov000_022e0998;
    c->flags &= ~4;
    c = &w->unkD60[10];
    rect_set_box3d(c, -45, 5, 30, 24, -0x7d00, 0x7d00);
    rect_set_atkdef(c, 0, (u32)-16);
    c->flags |= 0xc0;
    c->mask = 0x12;
    c->owner = w;
    c->cb = Boss6_OnHit_PushPlayer_ov000_022e08b0;
    c->flags &= ~4;
    for (i = 0; i < 11; i++)
        w->unkD60[i].owner_kind = OBJ_OWNER_OBJECT; /* native: owner type for obj_box_player */
    /* w->base.unk78 = 0: no StageObj collide step */
}

/* Boss6_UpdateAttackHitboxes (copied) */
static void Boss6_UpdateAttackHitboxes(Boss6 *w) {
    s32 x;
    if (gBossStatus.unk4 == 0x22) {
        w->unkD6A &= ~4;
        w->unkD6A |= 0x1000;
        w->unkDC2 &= ~4;
        w->unkDC2 |= 0x1000;
        w->unkDEE &= ~4;
        w->unkDEE |= 0x1000;
        return;
    }
    if (w->unkD5E != 0)
        x = 0x14000;
    else
        x = -0x14000;
    if (w->unk20C & 0x4000) {
        w->unkD6A |= 4;
        w->unkD6A &= ~0x1000;
        w->unkE9E |= 4;
        w->unkE9E &= ~0x1000;
        w->unk20C &= ~0x4000;
    } else {
        w->unkD6A &= ~4;
        w->unkE9E &= ~4;
    }
    if (w->unk20C & 0x1000) {
        x = (gPlayers->z << 4) - (w->unk10C4 + x);
        if (x < 0)
            x = -x;
        if (x <= 0x64000) {
            w->unkDC2 |= 4;
            w->unkDC2 &= ~0x1000;
            w->unk20C &= ~0x1000;
            goto next;
        }
    }
    w->unkDC2 &= ~4;
next:
    if (w->unk20C & 0x2000) {
        w->unkDEE |= 4;
        w->unkDEE &= ~0x1000;
        w->unk20C &= ~0x2000;
    } else {
        w->unkDEE &= ~4;
    }
}

/* Boss6_RegisterHitboxes4 (copied): pick attack boxes 0, 7, 2, 3 at "body_core" */
static void Boss6_RegisterHitboxes4(Boss6 *s) {
    s32 i;
    ObjBox *h;
    s16 x1;
    s16 y1;
    x1 = (s16)(s->unk10BC >> 12);
    y1 = (s16)-(s->unk10C0 >> 12);
    for (i = 0; i < 4; i++) {
        h = &s->unkD60[data_ov000_023637dc[i]];
        if (h->flags & 4) {
            h->x = x1;
            h->y = y1;
            h->flags &= ~3;
            if (!(s->h.disp_flags & 1))
                h->flags |= 1;
            rect_register(h);
        }
    }
    if (s->unkD60[0].flags & 4) {
        s->unk1114++;
        if (s->unk1114 == 3) {
            Boss_LoadModelPalettes(6, data_ov000_023657e0, gBoss6->unk1110, data_ov000_023636f4, 1);
        } else if (s->unk1114 == 7) {
            Boss_LoadModelPalettes(6, data_ov000_023657e0, gBoss6->unk110C, data_ov000_023636f4, 1);
            s->unk1114 = 0;
        }
    } else if (s->unk1114 != 0) {
        Boss_LoadModelPalettes(6, data_ov000_023657e0, gBoss6->unk110C, data_ov000_023636f4, 1);
        s->unk1114 = 0;
    }
}

/* Boss6_RegisterHitboxes2 (copied): tilt boxes 6 and 8 at "pelvis" */
static void Boss6_RegisterHitboxes2(Boss6 *w) {
    ObjBox *n = &w->unkE68;
    s16 x = (s16)(w->unk1068 >> 12);
    s16 y = (s16)-(w->unk106C >> 12);
    if (n->flags & 4) {
        n->x = x;
        n->y = y;
        n->flags &= ~3;
        if ((w->unkD5E == 0 && (w->unk20C & 0x100)) || (w->unkD5E != 0 && !(w->unk20C & 0x100)))
            n->flags |= 1;
        rect_register(n);
    }
    n = &w->unkEC0;
    if (n->flags & 4) {
        n->x = x;
        n->y = y;
        n->flags &= ~3;
        if ((w->unkD5E == 0 && (w->unk20C & 0x100)) || (w->unkD5E != 0 && !(w->unk20C & 0x100)))
            n->flags |= 1;
        rect_register(n);
    }
    if (w->unk1114 == 0)
        return;
    Boss_LoadModelPalettes(6, data_ov000_023657e0, gBoss6->unk110C, data_ov000_023636f4, 1);
    w->unk1114 = 0;
}

/* Boss6_RegisterHitboxes3x2 (copied): jump boxes 1, 10, 5 at "body_core", 4, 6, 9 at "pelvis" */
static void Boss6_RegisterHitboxes3x2(Boss6 *s) {
    s32 i;
    ObjBox *h;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    x1 = (s16)(s->unk10BC >> 12);
    y1 = (s16)-(s->unk10C0 >> 12);
    x2 = (s16)(s->unk1068 >> 12);
    y2 = (s16)-(s->unk106C >> 12);
    for (i = 0; i < 3; i++) {
        h = &s->unkD60[data_ov000_02363750[i]];
        if (h->flags & 4) {
            h->x = x1;
            h->y = y1;
            h->flags &= ~3;
            if ((s->unkD5E == 0 && s->unkF4C != 0) || (s->unkD5E != 0 && s->unkF4C == 0))
                h->flags |= 1;
            rect_register(h);
        }
    }
    for (i = 0; i < 3; i++) {
        h = &s->unkD60[data_ov000_02363720[i]];
        if (h->flags & 4) {
            h->x = x2;
            h->y = y2;
            h->flags &= ~3;
            if ((s->unkD5E == 0 && s->unkF4C != 0) || (s->unkD5E != 0 && s->unkF4C == 0))
                h->flags |= 1;
            rect_register(h);
        }
    }
    if (s->unkD60[1].flags & 4) {
        s->unk1114++;
        if (s->unk1114 == 3) {
            Boss_LoadModelPalettes(6, data_ov000_023657e0, gBoss6->unk1110, data_ov000_023636f4, 1);
        } else if (s->unk1114 == 7) {
            Boss_LoadModelPalettes(6, data_ov000_023657e0, gBoss6->unk110C, data_ov000_023636f4, 1);
            s->unk1114 = 0;
        }
    } else if (s->unk1114 != 0) {
        Boss_LoadModelPalettes(6, data_ov000_023657e0, gBoss6->unk110C, data_ov000_023636f4, 1);
        s->unk1114 = 0;
    }
}

static void disarm_all(Boss6 *w) {
    s32 i;
    for (i = 0; i < 11; i++) {
        w->unkD60[i].flags &= ~4;
        w->unkD60[i].flags |= 0x1000;
    }
}

/* Boss6_OnHit_PushPlayer_ov000_022e08b0 (copied; box 10) */
static void Boss6_OnHit_PushPlayer_ov000_022e08b0(ObjBox *a, ObjBox *b) {
    Boss6 *w;
    DcPlayer *p;
    s32 spd;
    p = obj_box_player(b); /* b->unk18 with p->unk0 == 1 */
    w = (Boss6 *)a->owner;
    if (p == NULL)
        return;
    if (p->move_flags & 0x8000)
        spd = p->vel_x;
    else
        spd = p->gnd_vel;
    if ((w->unkF4C != 0 && w->unkD5E == 0) || (w->unkF4C == 0 && w->unkD5E != 0))
        p->push_x = (s16)(((a->x - a->l - b->l + 1) << 8) - p->x);
    else
        p->push_x = (s16)(((a->x + a->l - b->r - 1) << 8) - p->x);
    if (spd == 0)
        return;
    p->move_flags |= 4;
    p->gnd_vel = 0;
    p->vel_x = p->gnd_vel;
}

/* Boss6_OnHit_PushPlayer_ov000_022e0998 (copied; box 9) */
static void Boss6_OnHit_PushPlayer_ov000_022e0998(ObjBox *a, ObjBox *b) {
    Boss6 *w;
    DcPlayer *p;
    s32 spd;
    p = obj_box_player(b);
    w = (Boss6 *)a->owner;
    if (p == NULL)
        return;
    if (p->move_flags & 0x8000)
        spd = p->vel_x;
    else
        spd = p->gnd_vel;
    if ((w->unkF4C != 0 && w->unkD5E == 0) || (w->unkF4C == 0 && w->unkD5E != 0)) {
        if (w->h.x < p->x && w->h.x < 0x2d8)
            p->push_x = (s16)(((a->x - a->l - b->l + 1) << 8) - p->x);
        else
            p->push_x = (s16)(((a->x - a->r - b->r - 1) << 8) - p->x);
    } else {
        if (w->h.x < p->x && w->h.x > 0x68)
            p->push_x = (s16)(((a->x + a->r - b->l + 1) << 8) - p->x);
        else
            p->push_x = (s16)(((a->x + a->l - b->r - 1) << 8) - p->x);
    }
    if (spd == 0)
        return;
    p->move_flags |= 4;
    p->gnd_vel = 0;
    p->vel_x = p->gnd_vel;
}

/* Boss6_OnHit_PushPlayer_ov000_022e0af8 (copied; box 8) */
static void Boss6_OnHit_PushPlayer_ov000_022e0af8(ObjBox *a, ObjBox *b) {
    DcPlayer *p;
    s32 spd;
    p = obj_box_player(b);
    if (p == NULL)
        return;
    if (p->move_flags & 0x8000)
        spd = p->vel_x;
    else
        spd = p->gnd_vel;
    if (p->x > 0x1a000)
        p->push_x = (s16)(((a->x + a->l - b->r - 1) << 8) - p->x);
    else
        p->push_x = (s16)(((a->x - a->l - b->l + 1) << 8) - p->x);
    if (spd == 0)
        return;
    p->move_flags |= 4;
    p->gnd_vel = 0;
    p->vel_x = p->gnd_vel;
}

/* Boss6_OnHit_PushPlayer_ov000_022e0ba8 (copied; box 7) */
static void Boss6_OnHit_PushPlayer_ov000_022e0ba8(ObjBox *a, ObjBox *b) {
    DcPlayer *p;
    Boss6 *w;
    s32 spd;
    p = obj_box_player(b);
    w = (Boss6 *)a->owner;
    if (p == NULL)
        return;
    if (p->move_flags & 0x8000)
        spd = p->vel_x;
    else
        spd = p->gnd_vel;
    if (w->h.x < p->x) {
        if (spd > 0)
            return;
        p->push_x = (s16)(((a->x + a->r - b->l) << 8) - p->x);
        p->move_flags |= 4;
        p->vel_x = 0;
        p->gnd_vel = 0;
    } else {
        if (spd < 0)
            return;
        p->push_x = (s16)(((a->x + a->l - b->r) << 8) - p->x);
        p->move_flags |= 4;
        p->vel_x = 0;
        p->gnd_vel = 0;
    }
}

/* Boss6_OnHit_Damage (modified: owner type check -> obj_box_player; NaviMsg / music via hooks) */
static void Boss6_OnHit_Damage(ObjBox *a, ObjBox *b) {
    Boss6 *w = (Boss6 *)a->owner;
    BossModel *m = &w->unk960;
    DcPlayer *p = obj_box_player(b);
    if (gBossStatus.unk8 != 0) {
        gBossStatus.unk8--;
        g_boss_stats.hits++;
        s_info.hits_jump++;
        boss_log("hit", gBossStatus.unk8, b->x);
        disarm_all(w);
        w->unk20C &= ~0x10;
        if (gBossStatus.unk8 != 0) {
            w->func = Boss6_State_Hurt;
            Boss6_UpdateFlashPalettes(0);
            w->unk1118 = 0;
            Model_SetAnimation(m, 0, w->unkD08, 0x18);
            m->flags[0] &= ~4;
            m->speed = 0x1000;
            gBossStatus.unk4 = 0x22;
            w->unk20C |= 0x200;
            BossEffect_CreateHit(0, b->x << 8, b->y << 8, 0);
            if (gBossStatus.unk8 == 2) {
                NaviMsg_SetFaceAnim(0xb);
                NaviMsg_ShowMessage(0x12, 0xf0);
                if (g_boss_hooks.music) /* Sound_PlaySeq(0, 0xf) */
                    g_boss_hooks.music(g_boss_hooks.ctx, 0xf);
            } else {
                NaviMsg_SetFaceAnim(0xb);
                NaviMsg_ShowMessage(0x10, 0xf0);
            }
            if (p != NULL) /* b->unk18 != NULL && b->unk18->unk0 == 1 */
                obj_player_attack_bounce(p);
            Sound_PlaySeqArc(0, 0, 0x7b);
            Sound_PlayVoice(0, 0, data_ov000_02363718[Rand() & 1]);
        } else {
            Boss6_Action_Defeat();
        }
    } else {
        Boss6_Action_Defeat();
    }
}

/* Boss6_OnHit_DamageFlip (modified: as Boss6_OnHit_Damage) */
static void Boss6_OnHit_DamageFlip(ObjBox *a, ObjBox *b) {
    Boss6 *w = (Boss6 *)a->owner;
    BossModel *m = &w->unk960;
    DcPlayer *p = obj_box_player(b);
    if (gBossStatus.unk8 != 0) {
        gBossStatus.unk8--;
        g_boss_stats.hits++;
        s_info.hits_pick++;
        boss_log("hit", gBossStatus.unk8, b->x);
        disarm_all(w);
        w->unk20C &= ~4;
        if (gBossStatus.unk8 != 0) {
            w->func = Boss6_State_HurtFlip;
            Boss6_SetCameraPickPullOut(w);
            w->unk111C = 8;
            Boss6_UpdateFlashPalettes(0);
            w->unk1118 = 0;
            Model_SetAnimation(m, 0, w->unkD08, 6);
            m->flags[0] &= ~4;
            m->speed = 0x1000;
            gBossStatus.unk4 = 0x22;
            w->unk20C |= 0x200;
            BossEffect_CreateHit(0, b->x << 8, b->y << 8, 0);
            if (gBossStatus.unk8 == 2) {
                NaviMsg_SetFaceAnim(0xb);
                NaviMsg_ShowMessage(0x12, 0xf0);
                if (g_boss_hooks.music)
                    g_boss_hooks.music(g_boss_hooks.ctx, 0xf);
            } else {
                NaviMsg_SetFaceAnim(0xb);
                NaviMsg_ShowMessage(0x10, 0xf0);
            }
            if (p != NULL)
                obj_player_attack_bounce(p);
            Sound_PlaySeqArc(0, 0, 0x7b);
            Sound_PlayVoice(0, 0, data_ov000_02363718[Rand() & 1]);
        } else {
            Boss6_Action_Defeat();
        }
    } else {
        Boss6_Action_Defeat();
    }
}

/* ============================================================ player (the deck) */
/* Boss6_Action_StunPlayer (copied; templates.py pointer code written with the field names) */
static void Boss6_Action_StunPlayer(Boss6 *p0) {
    DcPlayer *t0 = gPlayers;
    t0->map_a = 0;      /* +0x434 */
    t0->map_b = 0;      /* +0x436 */
    t0->map_up = 0;     /* +0x430 */
    t0->map_down = 0;   /* +0x432 */
    t0->map_right = 0;  /* +0x42C */
    t0->map_left = 0;   /* +0x42E */
    t0->gnd_vel = 0;    /* +0x34 */
    p0->unk111C_fn = t0->state; /* p0->unk111C = t0->unk68 */
    t0->state = Boss6_State_PlayerStunned;
    dcp_set_anim(t0, 0xf);
    s_info.stuns++;
    boss_log("stun", t0->x >> 8, 0);
}

/* Boss6_State_PlayerStunned (modified: a player state; the model's "ended" bit (unk240 & 2) is the
 * player module's DCP_DF_ANIM_DONE) */
static void Boss6_State_PlayerStunned(DcPlayer *s) {
    if (!(s->disp_flags & DCP_DF_ANIM_DONE))
        return;
    s->state = gBoss6 != NULL && gBoss6->unk111C_fn != NULL ? gBoss6->unk111C_fn : dcp_state_ground_idle;
    dcp_set_anim(s, 0x3c);
    s->map_a = 1;
    s->map_b = 2;
    s->map_up = 0x40;
    s->map_down = 0x80;
    if (gBoss6 != NULL && gBoss6->unkD5E != 0) {
        s->map_right = 0x20;
        s->map_left = 0x10;
    } else {
        s->map_right = 0x10;
        s->map_left = 0x20;
    }
}

/* Boss6_ApplyTiltToPlayer (modified: gPad.trg -> gPlayers->pad_raw[1]; player routines of the player
 * module (unk338 = act_move) and the copied Player_Action_KnockbackIntangible) */
static void Boss6_ApplyTiltToPlayer(Boss6 *s) {
    s32 i;
    dcp_bool found;
    dcp_bool b;
    s32 idx;
    s32 x;
    s32 d;
    s32 v;
    if (gPlayers->pf & 0x400)
        return;
    i = 0;
    found = DCP_FALSE;
    for (; i < 2; i++) {
        if (gPlayers->pad_raw[1] & data_ov000_02363710[i]) { /* gPad.unk4 (trg) & A / B */
            s->unk111C += 0x200;
            found = DCP_TRUE;
        }
    }
    if (s->unk112C > 0 && found) {
        s->unk112C--;
        s->unk111C += 0x200;
    }
    b = DCP_TRUE;
    if (g_dcp_world.mode != 2 && g_dcp_world.mode != 3)
        b = DCP_FALSE;
    if (b)
        idx = 1;
    else
        idx = g_boss_game.unk14;
    if (idx == 1)
        s->unk111C -= 0x20;
    else
        s->unk111C -= 0x15;
    if (s->unk111C < 0)
        s->unk111C = 0;
    else if (s->unk111C > 0x2000)
        s->unk111C = 0x2000;
    if (found) {
        s->unk1120 = 0;
        s_info.mash_presses++;
    } else
        s->unk1120 = (s32)((u32)s->unk1120 + 1);
    if ((u32)s->unk1120 >= 0xf)
        s->unk111C = 0;
    x = gPlayers->x;
    if (x > 0x34000)
        return;
    if (x < 0)
        return;
    if ((s->unkD5E == 0 && !(s->unk20C & 0x100)) || (s->unkD5E != 0 && (s->unk20C & 0x100)))
        d = 0x31e00 - x;
    else
        d = x - 0x2200;
    if (d < 0)
        d = 0;
    v = FX_Div(((-((0x2fc00 - d) << 8)) / 0x2fc00 - 0x400) * s->unkF68, 0x549f) >> 12;
    gPlayers->gnd_vel = (s16)(s->unk111C * 0x500 / 0x2000);
    if (s->unk20C & 0x100) {
        v *= -1;
        gPlayers->gnd_vel = (s16)(gPlayers->gnd_vel * -1);
    }
    if (s->unkD5E != 0) {
        v *= -1;
        gPlayers->gnd_vel = (s16)(gPlayers->gnd_vel * -1);
    }
    gPlayers->x += v;
    if (gPlayers->gnd_vel != 0) {
        if (gPlayers->anim < 0x3d || gPlayers->anim > 0x42)
            gPlayers->act_move(gPlayers); /* unk338: Player_Action_GroundMove */
    }
    if (gPlayers->anim == 0xf)
        return;
    if (gPlayers->x > 0x33800) {
        Player_Action_KnockbackIntangible(gPlayers, 0x500, -0x300);
        gPlayers->vel_y = (s16)(gPlayers->vel_y * 2);
        s_info.edge_knockbacks++;
        boss_log("edge_knockback", gPlayers->x >> 8, 1);
        return;
    }
    if (gPlayers->x < 0x800) {
        Player_Action_KnockbackIntangible(gPlayers, -0x500, -0x300);
        gPlayers->vel_y = (s16)(gPlayers->vel_y * 2);
        s_info.edge_knockbacks++;
        boss_log("edge_knockback", gPlayers->x >> 8, -1);
    }
}

/* ============================================================ attack selection / actions */
/* Boss6_SelectNextAttack (copied) */
static inline void Pick(Boss6 *b, const u16 (*tbl)[8]) {
    b->unkF46 = tbl[b->unkD4E][b->unkD4C];
    b->unkD4C++;
    if (b->unkD4C >= 8) {
        b->unkD4E = (s16)(Rand() & 3);
        b->unkD4C = 0;
    }
}
static void Boss6_Action_StartPartD4Attack(void);
static void Boss6_Action_StartPickAttack(void);
static void Boss6_Action_StartTilt(void);
static void Boss6_Action_StartEdgeJump(void);
static void Boss6_Action_StartMove(void);
static void Boss6_SelectNextAttack(s32 mode) {
    Boss6 *b = gBoss6;
    s32 hp = gBossStatus.unk8;
    b->unkF48 = (u16)mode;
    switch (mode) {
    case 0:
        switch (b->unkF46) {
        case 1:
            gBossStatus.unk4 = 0x1e;
            Boss6_Action_StartPartD4Attack();
            return;
        case 2:
            gBossStatus.unk4 = 0x1f;
            Boss6_Action_StartPickAttack();
            return;
        case 3:
            gBossStatus.unk4 = 0x20;
            Boss6_Action_StartTilt();
            return;
        case 4:
            gBossStatus.unk4 = 0x21;
            Boss6_Action_StartEdgeJump();
            return;
        }
        break;
    case 1:
        b->unkF44++;
        if (hp >= 6)
            Pick(b, data_ov000_02363918);
        else if (hp >= 3)
            Pick(b, data_ov000_02363998);
        else
            Pick(b, data_ov000_023639d8);
        gBossStatus.unk4 = 0x1c;
        Boss6_Action_StartMove();
        break;
    default:
        if (Rand() & 1) {
            b->unkF46 = 1;
        } else if (hp >= 6) {
            Pick(b, data_ov000_02363918);
        } else if (hp >= 3) {
            Pick(b, data_ov000_02363998);
        } else {
            Pick(b, data_ov000_023639d8);
        }
        gBossStatus.unk4 = 0x1c;
        Boss6_Action_StartMove();
        break;
    }
}

static void turn_to_stop(Boss6 *w) {
    if (w->unkF54 >= 0) {
        if (w->unkF58 > 0) {
            w->unkF54 = -w->unkF54;
            w->unkF60 = (s16)-w->unkF60;
        }
    } else {
        if (w->unkF58 < 0) {
            w->unkF54 = -w->unkF54;
            w->unkF60 = (s16)-w->unkF60;
        }
    }
    w->unkF64 = 0;
    w->unkF5C = 0;
}

/* Boss6_Action_StartMove (modified: NNS_G3dRenderObjSetCallBack -> SetBodyCallBack) */
static void Boss6_Action_StartMove(void) {
    Boss6 *w = gBoss6;
    BossModel *m = &w->unk960;
    s32 i;
    SetBodyCallBack(Boss6_NodeCallback_Store2Nodes_ov000_022e62fc);
    Model_SetAnimation(m, 0, w->unkD08, 0);
    m->flags[0] |= 4;
    m->speed = 0x1000;
    for (i = 0; i < 11; i++) {
        w->unkD60[i].flags &= ~4;
        w->unkD60[i].flags |= 0x1000;
    }
    w->unkF50 = (s32)data_ov000_023637ac[Rand() & 3];
    if (w->unkF48 != 1 && w->unkF48 != 0) {
        if (gPlayers->x < w->h.x) {
            w->unkF54 = -0x17;
            w->unkF5C = -0x580;
            w->unkF60 = 0x5b;
            w->unkF64 = 0x1555;
        } else {
            w->unkF54 = 0x17;
            w->unkF5C = 0x580;
            w->unkF60 = -0x5b;
            w->unkF64 = -0x1555;
        }
        w->unkF58 = 0;
    }
    w->unkF68 = 0x549f;
    w->unkF6A = 0;
    w->unkF6C = 0x549f;
    w->func = Boss6_State_Move;
}

/* Boss6_Action_StartPartD4Attack (modified: callback, voice hook) */
static void Boss6_Action_StartPartD4Attack(void) {
    Boss6 *p = gBoss6;
    SetBodyCallBack(Boss6_NodeCallback_Store2Nodes_ov000_022e620c);
    p->unk1118 = 0;
    p->func = Boss6_State_PartD4Attack;
    Sound_PlayVoice(0, 0, 0xd1);
    g_boss_stats.attacks++;
    s_info.attacks_d4++;
    boss_log("attack_bullets", gBossStatus.unk8, 0);
}

/* Boss6_Action_StartPickAttack (modified: callback) */
static void Boss6_Action_StartPickAttack(void) {
    Boss6 *w = gBoss6;
    s32 x = gPlayers->x;
    SetBodyCallBack(Boss6_NodeCallback_Store4Nodes_ov000_022e60a4);
    g_boss_stats.attacks++;
    s_info.attacks_pick++;
    boss_log("attack_pick", gBossStatus.unk8, 0);
    if (x < w->h.x + 0x3200 && x > w->h.x - 0x3200) {
        w->func = Boss6_State_PickStop;
        turn_to_stop(w);
        return;
    }
    w->func = Boss6_State_PickApproach;
}

/* Boss6_Action_StartTilt (copied) */
static void Boss6_Action_StartTilt(void) {
    Boss6 *w = gBoss6;
    g_boss_stats.attacks++;
    s_info.attacks_tilt++;
    boss_log("attack_tilt", gBossStatus.unk8, 0);
    turn_to_stop(w);
    w->func = Boss6_State_TiltStop;
}

/* Boss6_Action_StartEdgeJump (modified: callback) */
static void Boss6_Action_StartEdgeJump(void) {
    Boss6 *w = gBoss6;
    SetBodyCallBack(Boss6_NodeCallback_Store4Nodes_ov000_022e5df0);
    turn_to_stop(w);
    w->func = Boss6_State_EdgeJumpStop;
    w->unk1128 = data_ov000_02363824[gBossStatus.unk8 - 1];
    g_boss_stats.attacks++;
    s_info.attacks_jump++;
    boss_log("attack_edge_jump", gBossStatus.unk8, w->unk1128);
}

/* Boss6_Action_Defeat (modified: music / ring manager via hooks; display registers -> state) */
static void Boss6_Action_Defeat(void) {
    Boss6 *s = gBoss6;
    BossModel *x;
    s32 i;
    s->func = Boss6_State_Defeat;
    x = &s->unk960;
    if (g_boss_hooks.music_stop) /* NNS_SndPlayerStopSeqAll(0) */
        g_boss_hooks.music_stop(g_boss_hooks.ctx);
    StageObj_SetPause();
    x->speed = 0;
    s->h.obj_flags |= 0x20;
    g_dcp_world.flags &= ~1u; /* gGameState.unk18 &= ~1 */
    if (gBossStatus.unk4 == 0x1f) {
        Model_SetAnimation(x, 0, s->unkD08, 4);
        x->flags[0] &= ~4;
        x->speed = 0;
        if (x->anm[0] != NULL)
            x->anm[0]->frame = 0x28000; /* *x->unkE4 */
        s->unk111C = 0;
    } else {
        Model_SetAnimation(x, 0, s->unkD08, 0x14);
        x->flags[0] &= ~4;
        x->speed = 0;
        if (x->anm[0] != NULL)
            x->anm[0]->frame = 0x12000;
        s->unk111C = 1;
    }
    s->h.x = 0x1a000;
    s->h.y = 0x20000;
    s->h.z = 0;
    gPlayers->x = data_ov000_023658cc[s->unk111C][0] << 8;
    gPlayers->y = data_ov000_023658d0[s->unk111C][0] << 8;
    gPlayers->z = data_ov000_023658d4[s->unk111C][0] << 8;
    s->unkF4C = 0;
    Player_Action_Blank(gPlayers);
    gPlayers->invuln = 0;          /* unk3B2 */
    gPlayers->disp_flags &= ~0x20;
    dcp_set_anim(gPlayers, 0x30);  /* Player_SetAnimation */
    s->unk20C &= ~0x14;
    for (i = 0; i < 11; i++) {
        s->unkD60[i].flags &= ~4;
        s->unkD60[i].flags |= 0x1000;
    }
    s->unkD5E = 0;
    ScreenShake_Start(0);
    if (g_boss_hooks.ring_flags) /* RingManager_SetFlags(RingManager_GetFlags() | 1) */
        g_boss_hooks.ring_flags(g_boss_hooks.ctx, 1);
    gBossStatus.unk4 = 0x23;
    s->unkF50 = 0x1e;
    s->unk1118 = 0;
    g_boss_game.bright = 0x10; /* gDisplayMain.unk48 */
    /* BG2CNT / BG2 offset / gDisplayMain.unk8, unkA: the cut-in background, shown by
     * Boss6_SetDefeatCameraShot (DISPCNT |= BG2) */
    Boss6_SetDefeatCameraShot(s->unk111C, 0);
    CAM.unkDC = 2;
    CAM.t[1].ox = CAM.t[1].oy = CAM.t[1].oz = NULL;      /* c->unk3C..44 */
    CAM.t[1].offx = CAM.t[1].offy = CAM.t[1].offz = 0;   /* c->unk48..50 */
    CAM.t[1].tgt.x = CAM.t[1].cur.x = data_ov000_023658cc[s->unk111C][0] << 12;  /* c->unk54 / unk60 */
    CAM.t[1].tgt.y = CAM.t[1].cur.y = -data_ov000_023658d0[s->unk111C][0] << 12;
    CAM.t[1].tgt.z = CAM.t[1].cur.z = data_ov000_023658d4[s->unk111C][0] << 12;
    CAM_LO16(CAM.t[1].lerp, 0x1000);
    CAM_LO16(CAM.unkC8, 0x1000);
    CAM.unkD4 = 0x1000;
    CAM.unkDA = 0x1000;
    CAM_LO16(CAM.t[0].lerp, 0x1000);
    Sound_PlaySeqArc(0, 0, 0x7c);
    Boss_LoadModelPalettes(6, data_ov000_023657e0, gBoss6->unk110C, data_ov000_023636f4, 1);
    s->unk1114 = 0;
    boss_log("defeat", 0, 0);
}

/* ============================================================ states: walk / bullets */
static void reverse_at_limits(Boss6 *w, s32 d, int use_player) {
    if (w->unkF54 >= 0) {
        if (w->h.x > 0x2aa00 || (use_player && d < -0x8000)) {
            w->unkF54 = -w->unkF54;
            w->unkF60 = (s16)-w->unkF60;
            w->unkF64 = (s16)-w->unkF64;
            w->unkF5C = -w->unkF5C;
        }
    } else {
        if (w->h.x < 0x9600 || (use_player && d > 0x8000)) {
            w->unkF54 = -w->unkF54;
            w->unkF60 = (s16)-w->unkF60;
            w->unkF64 = (s16)-w->unkF64;
            w->unkF5C = -w->unkF5C;
        }
    }
}

/* Boss6_State_WaitForStart (modified: POWCNT screen swap dropped (single screen); sky model) */
static void Boss6_State_WaitForStart(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *a;
    if (!(gBossStatus.unk0 & 1))
        return;
    a = &CAM.unkE0; /* data_ov000_0236a524 */
    w->h.disp_flags &= ~0x20;
    w->unk20C &= ~1;
    /* *(vu16 *)0x4000304 |= 0x8000: POWCNT display swap, not done (one 640x480 view) */
    CAM.unk0 |= 1;
    Model_Init(a, 0);
    Model_SetResource(a, &data_ov000_02369b50[5]); /* data_ov000_02369b50.unk14 */
    a->scale[0] = a->scale[1] = a->scale[2] = 0x1355b50;
    Boss6_ResetCamera();
    Boss6_Action_StartMove();
    w->unkF50 = 0xb4;
    NaviMsg_SetFaceAnim(9);
    NaviMsg_ShowMessage(0xf, 0xf0);
    boss_log("start", gBossStatus.unk8, 0);
}

/* Boss6_State_Move (copied) */
static void Boss6_State_Move(void *p) {
    Boss6 *w = (Boss6 *)p;
    s32 d;
    w->unkF50--;
    Boss6_UpdateMoveSpeed(w);
    d = gPlayers->x - w->h.x;
    reverse_at_limits(w, d, 1);
    w->h.x += w->unkF58;
    if (w->unkF50 > 0)
        return;
    Boss6_SelectNextAttack(0);
}

/* MapObject_Spawn(0xd4, 0, 0, ...) */
static Boss6PartD4 *MapObject_Spawn6(u16 type) {
    ObjParam prm;
    memset(&prm, 0, sizeof(prm));
    prm.x0 = prm.y0 = 0xff;
    prm.type = type;
    return Boss6PartD4_Create(&prm, 0, 0);
}

/* Boss6_State_PartD4Attack (modified: MapObject_Spawn(0xd4) -> pool; the bullet's matrix / boxes
 * with the fx32 MTX_Inverse33 / MTX_MultVec33) */
static void Boss6_State_PartD4Attack(void *pp) {
    Boss6 *p = (Boss6 *)pp;
    BossVec v1;
    BossVec v2;
    MtxFx33 mtx;
    Boss6PartD4 *e;
    Boss6Piece *pc;
    s32 d;
    Boss6_UpdateMoveSpeed(p);
    d = gPlayers->x - p->h.x;
    reverse_at_limits(p, d, 1);
    p->h.x += p->unkF58;
    if (p->unk1118 % 15 == 0) {
        e = MapObject_Spawn6(0xd4);
        p->unkF70[p->unkF98] = e;
        if (p->unkD5E != 0) {
            MTX_RotY33(&e->unk20C.rot, SIN(0x8000), COS(0x8000)); /* data_02070470[0] / [1] */
            MTX_Concat33(&e->unk20C.rot, &p->unk984, &e->unk20C.rot);
        } else {
            e->unk20C.rot = p->unk984; /* MIi_CpuCopy32(p->unk984, e + 0x230, 0x24) */
        }
        e->h.x = p->h.x;
        e->h.y = p->h.y;
        e->h.z = p->h.z;
        MTX_Inverse33(&e->unk20C.rot, &mtx);
        if (p->unkD5E != 0)
            MTX_Concat33(&mtx, data_ov000_023638c4, &mtx);
        v1.x = 0x33000;
        v1.y = -0x97000;
        v1.z = 0;
        MTX_MultVec33(&v1, &mtx, &v1);
        v1.x >>= 12;
        v1.y >>= 12;
        v2.x = -0x33000;
        v2.y = -0x97000;
        v2.z = 0;
        MTX_MultVec33(&v2, &mtx, &v2);
        v2.x >>= 12;
        v2.y >>= 12;
        rect_set_box3d(&e->coll0, (s16)(v1.x - 13), (s16)(v1.y - 13), (s16)(v1.x + 13), (s16)(v1.y + 13), -32000, 32000);
        rect_set_box3d(&e->coll1, (s16)(v2.x - 13), (s16)(v2.y - 13), (s16)(v2.x + 13), (s16)(v2.y + 13), -32000, 32000);
        pc = Boss6_CreatePiece(2, e->h.x, e->h.y, e->h.z);
        if (p->unkD5E != 0) {
            MTX_RotY33(&pc->unk134.rot, SIN(0x8000), COS(0x8000));
            MTX_Concat33(&pc->unk134.rot, &p->unk984, &pc->unk134.rot);
        } else {
            pc->unk134.rot = p->unk984;
        }
        p->unkF98++;
        if (p->unkF98 >= 10)
            p->unkF98 = 0;
        Sound_PlaySeqArc(0, 0, 0x83);
    }
    p->unk1118++;
    if (p->unk1118 < 0x1c2)
        return;
    Boss6_SelectNextAttack(1);
}

/* ============================================================ states: pick */
static inline s32 Mul64(s64 a, s64 b) { return (s32)((a * b + 0x800) >> 12); }

/* Boss6_State_PickApproach (copied) */
static void Boss6_State_PickApproach(void *p) {
    Boss6 *s = (Boss6 *)p;
    s32 px = gPlayers->x;
    Boss6_UpdateMoveSpeed(s);
    reverse_at_limits(s, 0, 0);
    s->h.x += s->unkF58;
    if (px >= s->h.x + 0x3200)
        return;
    if (px <= s->h.x - 0x3200)
        return;
    s->func = Boss6_State_PickStop;
    turn_to_stop(s);
}

static s32 clamp_target(s32 x) {
    if (x < 0x3000)
        x = 0x3000;
    else if (x > 0x31000)
        x = 0x31000;
    return x;
}

/* Boss6_State_PickStop (copied; unk1118 written as u16) */
static void Boss6_State_PickStop(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    Boss6_UpdateMoveSpeed(w);
    w->h.x += w->unkF58;
    if (w->unkF58 != 0)
        return;
    if (w->unkF4A != 0)
        return;
    w->func = Boss6_State_PickRaise;
    Model_SetAnimation(m, 0, w->unkD08, 1);
    m->flags[0] &= ~4;
    m->speed = 0x1000;
    Boss6_SetCameraPickStart(w);
    w->unkF54 = 0;
    w->unkF58 = 0;
    w->unkF5C = 0;
    w->unkF60 = 0;
    w->unkF64 = 0;
    w->unkF4A = 0;
    w->unk1118 = (s32)(((u32)w->unk1118 & ~0xffffu)); /* u16 store of 0 */
    w->unk111C = w->h.x;
    w->unk1120 = clamp_target(gPlayers->x);
    w->unk1124 = data_ov000_02363804[gBossStatus.unk8 - 1];
    Sound_PlaySeqArc(0, 0, 0x84);
}

/* the s16 accesses of unk1118 in PickRaise / PickAim / PickSwing (strh / ldrsh) */
static s32 follow_target(Boss6 *w) {
    s32 v;
    s32 k;
    s32 cur;
    s32 tgt;
    s32 i;
    v = (s16)(w->unk1118 + 0x11);
    if (v < 0)
        v = 0;
    else if (v > 0x1000)
        v = 0x1000;
    w->unk1118 = (s32)(((u32)w->unk1118 & ~0xffffu) | (u16)v);
    k = (s16)w->unk1118;
    tgt = w->unk1120;
    cur = w->unk111C;
    i = 2;
    do {
        cur += Mul64(tgt - cur, k);
    } while (i-- != 0);
    return cur;
}

/* Boss6_State_PickRaise (copied) */
static void Boss6_State_PickRaise(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    w->unk1124--;
    if (w->unk1124 >= 0)
        w->unk1120 = clamp_target(gPlayers->x);
    w->h.x = follow_target(w);
    if (!(m->flags[0] & 2))
        return;
    w->func = Boss6_State_PickAim;
    Model_SetAnimation(m, 0, w->unkD08, 2);
    m->speed = data_ov000_023637bc[Rand() & 3];
}

/* Boss6_State_PickAim (copied) */
static void Boss6_State_PickAim(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    w->unk1124--;
    if (w->unk1124 >= 0)
        w->unk1120 = clamp_target(gPlayers->x);
    w->h.x = follow_target(w);
    w->unkF50--;
    if (!(m->flags[0] & 2))
        return;
    w->func = Boss6_State_PickSwing;
    Model_SetAnimation(m, 0, w->unkD08, 3);
    m->speed = 0x1000;
    w->unk20C |= 4;
    Boss6_SetCameraPickSwing(w);
    w->unk1128 = 0;
    Sound_PlaySeqArc(0, 0, 0x85);
    Sound_PlayVoice(0, 0, 0xd0);
}

/* Boss6_State_PickSwing (copied) */
static void Boss6_State_PickSwing(void *pp) {
    Boss6 *w = (Boss6 *)pp;
    BossModel *m = &w->unk960;
    DcPlayer *p;
    s32 x;
    s32 d;
    w->unk1124--;
    if (w->unk1124 >= 0) {
        p = gPlayers;
        x = p->x;
        if (p->vel_x > 0) {
            w->unk1128++;
            if (w->unk1128 > 0x40)
                w->unk1128 = 0x40;
            x += w->unk1128 << 8;
        } else if (p->vel_x < 0) {
            w->unk1128--;
            if (w->unk1128 < -0x40)
                w->unk1128 = -0x40;
            x += w->unk1128 << 8;
        }
        w->unk1120 = clamp_target(x);
    }
    w->h.x = follow_target(w);
    if (!(m->flags[0] & 2))
        return;
    w->func = Boss6_State_PickImpact;
    Model_SetAnimation(m, 0, w->unkD08, 4);
    m->speed = 0x1000;
    ScreenShake_Start(0xc);
    w->unkF50 = 0x1e;
    w->unk1118 = 0x28; /* *(s32 *)&w->unk1118 */
    if (w->unkD5E != 0)
        d = -0x2100;
    else
        d = 0x2100;
    BossEffect_CreateImpactSmall(1, w->unk1068 >> 4, 0x20000, d + (w->unk1070 >> 4));
    p = gPlayers;
    if (!(p->pf & 0x400) && (p->move_flags & 1))
        Boss6_Action_StunPlayer(w);
    Sound_PlaySeqArc(0, 0, 0x86);
}

/* camera shake from the screen shake offsets (inline in PickImpact / PickPullOut / LandShake / ...);
 * the original writes unk40 from unk3C again (copied as is) */
static void shake_camera(s32 k, s32 div, int mode) {
    CAM.unk3C.x = FX_Div(k * ScreenShake_GetOffsetX(), div);
    if (mode == 0)
        CAM.unk3C.x = CAM.unk3C.x * 0x3000;
    else
        CAM.unk3C.x = CAM.unk3C.x << mode;
    CAM.unk3C.x = CAM.unk3C.x >> 12;
    CAM.unk3C.y = FX_Div(k * ScreenShake_GetOffsetY(), div);
    if (mode == 0)
        CAM.unk3C.y = CAM.unk3C.y * 0x3000;
    else
        CAM.unk3C.y = CAM.unk3C.y << mode;
    CAM.unk3C.y = CAM.unk3C.x >> 12;
}

/* Boss6_State_PickImpact (copied) */
static void Boss6_State_PickImpact(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    s32 k;
    u16 r;
    s32 i;
    s32 d;
    k = w->unkF50 << 12;
    shake_camera(k, 0x1e000, 14);
    w->unkF50--;
    if (w->unkF50 < 0)
        w->unkF50 = 0;
    w->unk20C |= 0x1000;
    w->unk1118--;
    if (w->unk1118 > 0)
        return;
    w->func = Boss6_State_PickPullOut;
    r = Rand();
    i = GetIdx();
    m->speed = data_ov000_02363a18[i][gBossStatus.unk8 - 1][r & 3];
    w->unk1120 = 4;
    w->unkF50 = 0x3c;
    if (w->unkD5E != 0)
        d = -0x4200;
    else
        d = 0x4200;
    BossEffect_CreatePick(1, w->unk1068 >> 4, 0x20000, d + (w->unk1070 >> 4));
    BossEffect_CreateParts(1, w->unk1068 >> 4, 0x20000, d + (w->unk1070 >> 4));
    Sound_PlaySeqArc(0, 0, 0x8d);
}

/* Boss6_State_PickPullOut (copied) */
static void Boss6_State_PickPullOut(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    s32 k;
    w->unkF50--;
    if (w->unkF50 < 0)
        w->unkF50 = 0;
    k = w->unkF50 << 12;
    shake_camera(k, 0x3c000, 12);
    w->unk1120--;
    if (w->unk1120 >= 0)
        w->unk20C |= 0x2000;
    w->unk20C |= 0x4000;
    if (!(m->flags[0] & 2))
        return;
    w->func = Boss6_State_PickTurnAround;
    Model_SetAnimation(m, 0, w->unkD08, 5);
    m->speed = 0x1000;
    ScreenShake_Start(0);
    gPlayers->map_a = 1;
    gPlayers->map_b = 2;
    gPlayers->map_up = 0x40;
    gPlayers->map_down = 0x80;
    Boss6_SetCameraPickPullOut(w);
    if (gPlayers->state == Boss6_State_PlayerStunned) /* the DS keeps the saved state in the same s32 */
        boss_log("stun_overwritten", 0, 0);
    w->unk111C = 8;
    Sound_PlaySeqArc(0, 0, 0x84);
}

/* Boss6_State_PickTurnAround (copied) */
static void Boss6_State_PickTurnAround(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    w->unk111C--;
    if (w->unk111C == 0)
        Boss6_SetCameraSideShot(w);
    if (!(m->flags[0] & 2))
        return;
    Boss6_SetCameraAfterFlip(w);
    w->unkD5E ^= 1;
    s_info.turns++;
    Boss6_SelectNextAttack(2);
    w->unk20C &= ~4;
}

/* ============================================================ states: tilt */
/* Boss6_State_TiltStop (copied) */
static void Boss6_State_TiltStop(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    Boss6_UpdateMoveSpeed(w);
    w->h.x += w->unkF58;
    if (w->unkF58 != 0)
        return;
    if (w->unkF4A != 0)
        return;
    w->func = Boss6_State_TiltWaitJump;
    Model_SetAnimation(m, 0, w->unkD08, 7);
    m->flags[0] &= ~4;
    m->speed = 0x1000;
    Sound_PlaySeqArc(0, 0, 0x84);
}

/* Boss6_State_TiltWaitJump (copied) */
static void Boss6_State_TiltWaitJump(void *p) {
    Boss6 *w = (Boss6 *)p;
    if (!(w->unkA6C & 2))
        return;
    w->func = Boss6_State_TiltJump;
    w->unkF50 = 0;
    w->unkF66 = 0;
    w->unkF68 = 0;
    w->unkF6A = 0;
    w->unkF6C = 0;
    w->unk1118 = w->h.x;
    w->h.x = 0x1a000;
    w->unk20C &= ~0x100;
    if (Rand() & 1)
        w->unk20C |= 0x100;
}

/* Boss6_State_TiltJump (modified: callback) */
static void Boss6_State_TiltJump(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    w->unkF50--;
    if (w->unkF50 > 0)
        return;
    SetBodyCallBack(Boss6_NodeCallback_PelvisAndPro);
    w->func = Boss6_State_TiltLand;
    Model_SetAnimation(m, 0, w->unkD08, 8);
    m->speed = 0x1000;
    w->unkE72 |= 4;
    w->unkE72 &= ~0x1000;
    w->unkECA |= 4;
    w->unkECA &= ~0x1000;
    w->unk20C |= 8;
}

/* Boss6_State_TiltLand (modified: unk338 = act_move of the player module) */
static void Boss6_State_TiltLand(void *pp) {
    Boss6 *w = (Boss6 *)pp;
    BossModel *m = &w->unk960;
    DcPlayer *p;
    if (!(m->flags[0] & 2))
        return;
    w->func = Boss6_State_TiltStart;
    Model_SetAnimation(m, 0, w->unkD08, 9);
    m->speed = 0x1000;
    ScreenShake_Start(0xc);
    w->unkF50 = 0x5a;
    BossEffect_CreateLand(1, w->unk1068 >> 4, 0x20000, w->unk1070 >> 4);
    Boss6_StartTiltCamera(w);
    p = gPlayers;
    if (!(p->pf & 0x400)) {
        if ((w->unkD5E != 0 && (w->unk20C & 0x100)) || (w->unkD5E == 0 && !(w->unk20C & 0x100))) {
            p->held = 0x10; /* unk420 */
            gPlayers->act_move(gPlayers);
        } else {
            p->held = 0x20;
            gPlayers->act_move(gPlayers);
        }
    }
    Sound_PlaySeqArc(0, 0, 0x86);
    s_info.tilts++;
    boss_log("tilt", (w->unk20C & 0x100) ? -1 : 1, 0);
}

/* Boss6_State_TiltStart (copied) */
static void Boss6_State_TiltStart(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    s32 k;
    k = w->unkF50 << 12;
    shake_camera(k, 0x5a000, 0);
    w->unkF50--;
    if (w->unkF50 < 0)
        w->unkF50 = 0;
    if (!(m->flags[0] & 2))
        return;
    w->func = Boss6_State_Tilting;
    ScreenShake_Start(0);
    Model_SetAnimation(m, 0, w->unkD08, 0xa);
    m->speed = 0x1000;
    w->unkF6A = 0x169;
    w->unkF6C = 0x549f;
    w->unkF50 = 0x12c;
    w->unkE72 &= ~4;
    w->unk1124 = 0;
    w->unk1128 = 0;
    w->unk111C = 0;
    w->unk1120 = 0;
    w->unk112C = 10;
    Sound_PlaySeqArc(0, 0, 0x87);
    Sound_PlayVoice(0, 0, 0xd2);
}

/* Boss6_SpawnDebrisTimed (copied) */
static void Boss6_SpawnDebrisTimed(Boss6 *s) {
    Boss6 *b;
    s32 r;
    s32 x;
    s32 a;
    s32 z;
    s->unk1128--;
    if (s->unk1128 > 0)
        return;
    s->unk1128 = (0x549f - s->unkF68) * 10 / 0x549f + 5;
    r = Rand() & 0x1f;
    b = gBoss6;
    if (b->unkD5E != 0) {
        if (b->unk20C & 0x100)
            x = 0x328 - r;
        else
            x = r + 0x18;
    } else {
        if (b->unk20C & 0x100)
            x = r + 0x18;
        else
            x = 0x328 - r;
    }
    a = (0x1ec - (Rand() & 0x3f)) << 8;
    z = ((Rand() & 0x3f) + 0x1e) << 8;
    if (Rand() & 1)
        z *= -1;
    Boss6_CreatePiece(1, x << 8, a, z);
}

/* Boss6_State_Tilting (copied) */
static void Boss6_State_Tilting(void *p) {
    Boss6 *w = (Boss6 *)p;
    s32 i;
    w->unkF50--;
    Boss6_ApplyTiltToPlayer(w);
    if (w->unkF50 == data_ov000_0236375c[w->unk1124]) {
        i = Rand() & 3;
        w->unk1124++;
        Boss6_CreatePiece(0, data_ov000_023638e8[i][0], data_ov000_023638ec[i][0], data_ov000_023638f0[i][0]);
    }
    Boss6_SpawnDebrisTimed(w);
    Boss6_Unk_ov000_022e1f10(w);
    if (w->unkF50 > 0)
        return;
    w->func = Boss6_State_TiltReturn;
    w->unkF6A = -0x79;
    w->unkF6C = 0;
}

/* Boss6_State_TiltReturn (copied) */
static void Boss6_State_TiltReturn(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    Boss6_ApplyTiltToPlayer(w);
    Boss6_SpawnDebrisTimed(w);
    Boss6_Unk_ov000_022e1f10(w);
    if (w->unkF68 != w->unkF6C)
        return;
    w->func = Boss6_State_TiltEnd;
    Model_SetAnimation(m, 0, w->unkD08, 0xb);
    m->speed = 0x1000;
    w->unkECA &= ~4;
    w->unkECA |= 0x1000;
    w->unk20C &= ~8;
    Boss6_EndTiltCamera(w);
    w->unkD54[0] = -0x17ff;
    w->unkD58 = 0x17ff;
}

/* Boss6_State_TiltEnd (modified: callback) */
static void Boss6_State_TiltEnd(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    if (!(m->flags[0] & 2))
        return;
    SetBodyCallBack(Boss6_NodeCallback_Store2Nodes_ov000_022e62fc);
    w->func = Boss6_State_WaitSelectAttack3;
    Model_SetAnimation(m, 0, w->unkD08, 0xc);
    m->speed = 0x1000;
    w->unkF68 = 0x549f;
    w->unkF6A = 0;
    w->unkF6C = 0x549f;
    w->h.x = w->unk1118;
}

/* Boss6_State_WaitSelectAttack3 (copied) */
static void Boss6_State_WaitSelectAttack3(void *p) {
    Boss6 *s = (Boss6 *)p;
    if (!(s->unkA6C & 2))
        return;
    Boss6_SelectNextAttack(3);
    s->unk111C = 0;
    s->unk1120 = 0;
}

/* ============================================================ states: edge jumps */
/* Boss6_State_EdgeJumpStop (copied) */
static void Boss6_State_EdgeJumpStop(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    Boss6_UpdateMoveSpeed(w);
    w->h.x += w->unkF58;
    if (w->unkF58 != 0)
        return;
    if (w->unkF4A != 0)
        return;
    w->func = Boss6_State_EdgeJumpLeap;
    Model_SetAnimation(m, 0, w->unkD08, 0xd);
    m->flags[0] &= ~4;
    m->speed = 0x1000;
    Sound_PlaySeqArc(0, 0, 0x84);
}

/* Boss6_State_EdgeJumpLeap (copied) */
static void Boss6_State_EdgeJumpLeap(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    if (!(m->flags[0] & 2))
        return;
    w->func = Boss6_State_EdgeJumpLand;
    Model_SetAnimation(m, 0, w->unkD08, 0xe);
    m->flags[0] &= ~4;
    m->speed = 0x1000;
    w->unkF50 = 0;
    w->unkF66 = 0;
    w->unkF68 = 0;
    w->unkF6A = 0;
    w->unkF6C = 0;
    w->unk1118 = w->h.x;
    if (Rand() & 1) {
        w->h.x = 0x1800;
        if (w->unkD5E != 0)
            w->unkF4C = 0;
        else
            w->unkF4C = -0x8000;
    } else {
        w->h.x = 0x32800;
        if (w->unkD5E != 0)
            w->unkF4C = -0x8000;
        else
            w->unkF4C = 0;
    }
    w->unkE72 |= 4;
    w->unkE72 &= ~0x1000;
    w->unkEF6 |= 4;
    w->unkEF6 &= ~0x1000;
    w->unk20C |= 0x10;
    Boss6_SetCameraAngle(w, w->unkF4C);
}

/* Boss6_State_EdgeJumpLand (copied) */
static void Boss6_State_EdgeJumpLand(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    if (m->flags[0] & 2) {
        w->func = Boss6_State_EdgeJumpRecover;
        Model_SetAnimation(m, 0, w->unkD08, 0xf);
        m->flags[0] &= ~4;
        m->speed = 0x1000;
        ScreenShake_Start(0xc);
        w->unkF50 = 0x14;
        BossEffect_CreateLand(1, w->unk1068 >> 4, 0x20000, w->unk1070 >> 4);
        w->unkE72 &= ~4;
        w->unkE72 |= 0x1000;
        Sound_PlaySeqArc(0, 0, 0x86);
    }
    if (gPlayers->pf & 0x400) {
        CAM_LO16(CAM.t[0].lerp, 0x10);
        CAM_LO16(CAM.t[1].lerp, 0x10);
    }
}

/* Boss6_State_EdgeJumpRecover (copied) */
static void Boss6_State_EdgeJumpRecover(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    s32 k;
    k = w->unkF50 << 12;
    shake_camera(k, 0x14000, 0);
    w->unkF50--;
    if (w->unkF50 < 0)
        w->unkF50 = 0;
    if (m->flags[0] & 2) {
        w->func = Boss6_State_StartJump;
        Model_SetAnimation(m, 0, w->unkD08, 0x10);
        m->flags[0] &= ~4;
        m->speed = 0x1000;
        ScreenShake_Start(0);
        Boss6_SetCameraEdgeJump(w, w->unkF4C);
    }
    if (gPlayers->pf & 0x400) {
        CAM_LO16(CAM.t[0].lerp, 0x10);
        CAM_LO16(CAM.t[1].lerp, 0x10);
    }
}

/* Boss6_State_StartJump (copied; unk111C / unk1124 are s16 here) */
static void Boss6_State_StartJump(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    if (!(m->flags[0] & 2))
        return;
    w->func = Boss6_State_JumpRise;
    Model_SetAnimation(m, 0, w->unkD08, 0x11);
    m->flags[0] &= ~4;
    m->speed = 0x1000;
    w->unk111C = (s16)-0x1e00;
    w->unk1120 = data_ov000_02363844[gBossStatus.unk8 - 1];
    if (gPlayers->x - w->h.x < 0) {
        if (w->unkD5E != 0)
            w->unk1124 = 0x38e;
        else
            w->unk1124 = (s16)-0x38e;
    } else {
        if (w->unkD5E != 0)
            w->unk1124 = (s16)-0x38e;
        else
            w->unk1124 = 0x38e;
    }
    Boss6_SetCameraJumpStart(w);
    s_info.jumps++;
}

static void jump_track(Boss6 *w) {
    s32 d;
    if (w->unk1120 != 0) {
        d = gPlayers->x - w->h.x;
        w->unk1120--;
        if (d < -0x1000) {
            w->h.x -= 0xa00;
            if (w->h.x < 0x7700)
                w->h.x = 0x7700;
        } else if (d > 0x1000) {
            w->h.x += 0xa00;
            if (w->h.x > 0x2c800)
                w->h.x = 0x2c800;
        }
    }
    w->unkF4C = (s16)(w->unkF4C + (s16)w->unk1124);
    if ((s16)w->unk1124 < 0) {
        if (w->unkF4C < 0)
            w->unkF4C = 0;
    } else if ((u16)w->unkF4C > 0x8000) {
        w->unkF4C = -0x8000;
    }
}

/* Boss6_State_JumpRise (copied) */
static void Boss6_State_JumpRise(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    w->unk111C = (s16)(w->unk111C + 0xd0);
    w->h.y += (s16)w->unk111C;
    if ((s16)w->unk111C >= 0) {
        w->func = Boss6_State_JumpFall;
        Model_SetAnimation(m, 0, w->unkD08, 0x12);
        m->flags[0] &= ~4;
        m->speed = 0x1000;
        if (gPlayers->x - w->h.x < 0) {
            if (w->unkD5E != 0) {
                w->unk1124 = 0x38e;
                Boss6_SetCameraJump(w, 1);
            } else {
                w->unk1124 = (s16)-0x38e;
                Boss6_SetCameraJump(w, 0);
            }
        } else {
            if (w->unkD5E != 0) {
                w->unk1124 = (s16)-0x38e;
                Boss6_SetCameraJump(w, 0);
            } else {
                w->unk1124 = 0x38e;
                Boss6_SetCameraJump(w, 1);
            }
        }
        w->unkE1A |= 4;
        w->unkE1A &= ~0x1000;
        w->unkEF6 &= ~4;
        w->unkEF6 |= 0x1000;
        Sound_PlayVoice(0, 0, 0xd0);
    }
    jump_track(w);
}

/* Boss6_State_JumpFall (copied) */
static void Boss6_State_JumpFall(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    w->unk111C = (s16)(w->unk111C + 0xd0);
    w->h.y += (s16)w->unk111C;
    if (w->h.y >= 0x20000) {
        w->h.y = 0x20000;
        w->func = Boss6_State_LandShake;
        Model_SetAnimation(m, 0, w->unkD08, 0x14);
        m->flags[0] &= ~4;
        m->speed = data_ov000_02363958[GetIdx()][gBossStatus.unk8 - 1];
        ScreenShake_Start(0xc);
        w->unkF50 = 0x14;
        BossEffect_CreateLand(1, w->unk1068 >> 4, 0x20000, w->unk1070 >> 4);
        w->unkD96 |= 4;
        w->unkD96 &= ~0x1000;
        w->unkF22 |= 4;
        w->unkF22 &= ~0x1000;
        w->unkE46 |= 4;
        w->unkE46 &= ~0x1000;
        w->unkEF6 |= 4;
        w->unkEF6 &= ~0x1000;
        Sound_PlaySeqArc(0, 0, 0x86);
        s_info.landings++;
        boss_log("land", w->h.x >> 8, 0);
    } else if (m->flags[0] & 2) {
        Model_SetAnimation(m, 0, w->unkD08, 0x13);
        m->flags[0] &= ~4;
        m->speed = 0x1000;
    }
    jump_track(w);
    if (gPlayers->pf & 0x400) {
        CAM_LO16(CAM.t[0].lerp, 0x10);
        CAM_LO16(CAM.t[1].lerp, 0x10);
    }
}

/* Boss6_State_LandShake (copied) */
static void Boss6_State_LandShake(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    s32 k;
    w->unkF50--;
    if (w->unkF50 < 0)
        w->unkF50 = 0;
    k = w->unkF50 << 12;
    shake_camera(k, 0x14000, 13);
    if (m->flags[0] & 2) {
        w->func = Boss6_State_AfterLand;
        Model_SetAnimation(m, 0, w->unkD08, 0x15);
        m->flags[0] &= ~4;
        m->speed = 0x1000;
        ScreenShake_Start(0);
        w->unkE1A &= ~4;
        w->unkE1A |= 0x1000;
        w->unkD96 &= ~4;
        w->unkD96 |= 0x1000;
        w->unkE46 &= ~4;
        w->unkE46 |= 0x1000;
    }
    if (gPlayers->pf & 0x400) {
        CAM_LO16(CAM.t[0].lerp, 0x10);
        CAM_LO16(CAM.t[1].lerp, 0x10);
    }
}

/* Boss6_State_AfterLand (copied) */
static void Boss6_State_AfterLand(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    if (!(m->flags[0] & 2))
        return;
    w->unk1128--;
    if (w->unk1128 != 0) {
        w->func = Boss6_State_StartJump;
        Model_SetAnimation(m, 0, w->unkD08, 0x10);
        m->flags[0] &= ~4;
        m->speed = 0x1000;
    } else {
        w->func = Boss6_State_EndJumps;
        Model_SetAnimation(m, 0, w->unkD08, 0x16);
        m->flags[0] &= ~4;
        m->speed = 0x1000;
        w->h.y = 0x20000;
        Boss6_SetCameraFollowPlayer(w);
        Sound_PlaySeqArc(0, 0, 0x84);
    }
    w->unkF22 &= ~4;
    w->unkF22 |= 0x1000;
}

/* Boss6_State_EndJumps (copied) */
static void Boss6_State_EndJumps(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    s32 i;
    if (!(m->flags[0] & 2))
        return;
    w->func = Boss6_State_WaitSelectAttack;
    Model_SetAnimation(m, 0, w->unkD08, 0x17);
    m->flags[0] &= ~4;
    m->speed = 0x1000;
    w->h.x = w->unk1118;
    w->unkF4C = 0;
    for (i = 0; i < 11; i++) {
        w->unkD60[i].flags &= ~4;
        w->unkD60[i].flags |= 0x1000;
    }
    w->unk20C &= ~0x10;
    w->unkF68 = 0x549f;
    w->unkF6A = 0;
    w->unkF6C = 0x549f;
}

/* Boss6_State_WaitSelectAttack (copied) */
static void Boss6_State_WaitSelectAttack(void *p) {
    Boss6 *s = (Boss6 *)p;
    if (!(s->unkA6C & 2))
        return;
    Boss6_SelectNextAttack(4);
}

/* ============================================================ states: hurt / defeat */
/* Boss6_State_Hurt (copied) */
static void Boss6_State_Hurt(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    if (m->flags[0] & 2) {
        w->func = Boss6_State_HurtRecover;
        Model_SetAnimation(m, 0, w->unkD08, 0x16);
        m->flags[0] &= ~4;
        m->speed = 0x1000;
        Boss6_SetCameraFollowPlayer(w);
        Sound_PlaySeqArc(0, 0, 0x84);
    }
    w->unk1118++;
    Boss6_UpdateFlashPalettes(w->unk1118);
}

/* Boss6_State_HurtRecover (copied) */
static void Boss6_State_HurtRecover(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    if (m->flags[0] & 2) {
        w->func = Boss6_State_HurtWaitSelectAttack;
        Model_SetAnimation(m, 0, w->unkD08, 0x17);
        m->flags[0] &= ~4;
        m->speed = 0x1000;
        w->h.x = w->unk1118;
        w->unkF4C = 0;
        Boss6_UpdateFlashPalettes(4);
        w->unkF68 = 0x549f;
        w->unkF6A = 0;
        w->unkF6C = 0x549f;
        return;
    }
    w->unk1118++;
    Boss6_UpdateFlashPalettes(w->unk1118);
}

/* Boss6_State_HurtWaitSelectAttack (copied) */
static void Boss6_State_HurtWaitSelectAttack(void *p) {
    Boss6 *s = (Boss6 *)p;
    if (!(s->unkA6C & 2))
        return;
    Boss6_SelectNextAttack(4);
}

/* Boss6_State_HurtFlip (copied) */
static void Boss6_State_HurtFlip(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    w->unk111C--;
    if (w->unk111C == 0)
        Boss6_SetCameraSideShot(w);
    if (m->flags[0] & 2) {
        Boss6_SetCameraAfterFlip(w);
        w->unkD5E ^= 1;
        w->unk20C &= ~4;
        Boss6_UpdateFlashPalettes(4);
        s_info.turns++;
        Boss6_SelectNextAttack(2);
        return;
    }
    w->unk1118++;
    Boss6_UpdateFlashPalettes(w->unk1118);
}

/* Boss6_State_Defeat (modified: DISPCNT BG2 off -> exc_bg -1; brightness state) */
static void Boss6_State_Defeat(void *p) {
    Boss6 *w = (Boss6 *)p;
    BossModel *m = &w->unk960;
    BossEffect *fx;
    w->unkF50--;
    if (g_boss_game.bright > 0)
        g_boss_game.bright = (s16)(g_boss_game.bright - 2);
    if (w->unkF50 > 0)
        return;
    w->unk1118++;
    if (w->unk1118 >= 3) {
        w->func = Boss6_State_DefeatFinish;
        g_boss_game.exc_bg = -1; /* SetVisiblePlane(GetVisiblePlane() & ~4) */
        boss_log("exc_bg", -1, 0);
        Model_SetAnimation(m, 0, w->unkD08, 0x19);
        m->flags[0] &= ~4;
        m->speed = 0x1000;
        StageObj_ClearPause();
        w->h.obj_flags &= ~0x20;
        gPlayers->disp_flags |= 0x20;
        gPlayers->pf |= 0x200000;
        g_boss_game.bright = 0x10;
        CAM.t[1].tgt.x = data_ov000_023658cc[w->unk111C][0] << 12;    /* cam->unk54 */
        CAM.t[1].tgt.y = -data_ov000_023658d0[w->unk111C][0] << 12;
        CAM.t[1].tgt.z = data_ov000_023658d4[w->unk111C][0] << 12;
        CAM.unkC0 = CAM.unkC4 = 0x12c000;                             /* cam->unk78 / unk7C */
        CAM.unkCC = CAM.unkD0 = -0x80000;                             /* cam->unk84 / unk88 */
        CAM.unkD6 = CAM.unkD8 = 0;                                    /* cam->unk8E / unk90 */
        fx = BossEffect_CreateBlast(0, data_ov000_023658cc[w->unk111C][0] << 8, data_ov000_023658d0[w->unk111C][0] << 8,
                                    data_ov000_023658d4[w->unk111C][0] << 8);
        fx->unk134.speed = 0x800; /* unk248 */
        Sound_PlaySeqArc(0, 0, 0x7d);
        w->unkF50 = 300;
        NaviMsg_SetFaceAnim(0xd);
        NaviMsg_ShowMessage(0x13, 0xf0);
    } else {
        w->unkF50 = 0x1e;
        Boss6_SetDefeatCameraShot(w->unk111C, w->unk1118);
        g_boss_game.bright = 0x10;
        Sound_PlaySeqArc(0, 0, 0x7c);
    }
}

/* Boss6_State_DefeatFinish (modified: StageShutdown_Create -> g_boss_hooks.cleared (once)) */
static void Boss6_State_DefeatFinish(void *p) {
    Boss6 *w = (Boss6 *)p;
    w->unkF50--;
    if ((w->unkF50 & 3) == 0) {
        if (g_boss_game.bright > 0)
            g_boss_game.bright--;
    }
    if (w->unkF50 > 0)
        return;
    g_obj_stage.ring_bonus = gPlayers->rings * 1000; /* data_022c4590.unk10 = gPlayers->unk3AE * 1000 */
    if (!g_boss_game.cleared) {
        g_boss_game.cleared = 1;
        boss_log("cleared", gPlayers->rings, 0);
        if (g_boss_hooks.cleared) /* StageShutdown_Create(Stage_OnBossCleared) */
            g_boss_hooks.cleared(g_boss_hooks.ctx);
    }
}

/* ============================================================ node callbacks / draw */
/* G3_MultMtx33 from a node callback: the matrix multiplies the current (node) matrix; natively a
 * joint rotation of the node being processed (the first NODEDESC = joint 0: the whole body) */
static void G3_MultMtx33(Boss6NodeRS *rs, const MtxFx33 *m) { boss_model_set_joint_rot(rs->m, 0, rs->node, m); }

/* Boss6_StoreNodeMatrix (modified: NNS_G3dGetResDictIdxByName -> bossdata.bin joint table; MTX_STORE ->
 * g_boss_mtx_slot[v] = joint world matrix) */
static dcp_bool Boss6_StoreNodeMatrix(Boss6NodeRS *p, u32 v, u32 k) {
    s32 r = -1;
    s32 expect;
    int i;
    for (i = 0; i < 4; i++)
        if (data_ov000_023658bc[i] == k)
            r = s_joint[i];
    expect = p->node; /* rs->flag & 0x10 (current node valid) is always set in a NODEDESC callback */
    if (r != expect)
        return DCP_FALSE;
    boss_model_joint_world(p->m, p->node, &g_boss_mtx_slot[v & 31]);
    return DCP_TRUE;
}

/* Boss6_NodeCallback_PelvisAndPro (substantially rewritten: per joint after the pose; see SBC order) */
static void Boss6_NodeCallback_PelvisAndPro(Boss6NodeRS *w) {
    u16 st = gBoss6->unk10EC;
    switch (st) {
    case 1:
        if (!Boss6_StoreNodeMatrix(w, 0x1c, data_ov000_023658bc[2]))
            return;
        gBoss6->unk10EC++;
        return;
    case 2:
        if (!Boss6_StoreNodeMatrix(w, 0x1d, data_ov000_023658bc[1]))
            return;
        gBoss6->unk10EC++;
        return;
    case 3:
        if (!Boss6_StoreNodeMatrix(w, 0x1e, data_ov000_023658bc[0]))
            return;
        gBoss6->unk10EC = 0;
        w->done = 1; /* w->unk24 = 0; w->unk92 = 0 */
        return;
    case 0:
        gBoss6->unk10EC = (u16)(st + 1);
        if ((gBoss6->unkD5E != 0 && !(gBoss6->unk20C & 0x100)) || (gBoss6->unkD5E == 0 && (gBoss6->unk20C & 0x100)))
            G3_MultMtx33(w, data_ov000_023638c4);
        break;
    }
}

/* Boss6_NodeCallback_Store2Nodes_ov000_022e620c / _022e62fc (substantially rewritten, as above) */
static void Store2Nodes(Boss6NodeRS *p) {
    switch (gBoss6->unk10EC) {
    case 1:
        if (!Boss6_StoreNodeMatrix(p, 0x1d, data_ov000_023658bc[1]))
            return;
        gBoss6->unk10EC++;
        return;
    case 2:
        if (!Boss6_StoreNodeMatrix(p, 0x1e, data_ov000_023658bc[0]))
            return;
        gBoss6->unk10EC = 0;
        p->done = 1;
        return;
    case 0:
        gBoss6->unk10EC++;
        if (gBoss6->unkD5E == 0)
            return;
        G3_MultMtx33(p, data_ov000_023638c4);
        break;
    }
}
static void Boss6_NodeCallback_Store2Nodes_ov000_022e620c(Boss6NodeRS *p) { Store2Nodes(p); }
static void Boss6_NodeCallback_Store2Nodes_ov000_022e62fc(Boss6NodeRS *p) { Store2Nodes(p); }

/* Boss6_NodeCallback_Store4Nodes_ov000_022e5df0 / _022e60a4 (substantially rewritten, as above) */
static void Store4Nodes(Boss6NodeRS *p) {
    switch (gBoss6->unk10EC) {
    case 1:
        if (!Boss6_StoreNodeMatrix(p, 0x1c, data_ov000_023658bc[2]))
            return;
        gBoss6->unk10EC++;
        return;
    case 2:
        if (!Boss6_StoreNodeMatrix(p, 0x1d, data_ov000_023658bc[1]))
            return;
        gBoss6->unk10EC++;
        return;
    case 3:
        if (!Boss6_StoreNodeMatrix(p, 0x1b, data_ov000_023658bc[3]))
            return;
        gBoss6->unk10EC++;
        return;
    case 4:
        if (!Boss6_StoreNodeMatrix(p, 0x1e, data_ov000_023658bc[0]))
            return;
        gBoss6->unk10EC = 0;
        p->done = 1;
        return;
    case 0:
        gBoss6->unk10EC++;
        if (gBoss6->unkD5E == 0)
            return;
        G3_MultMtx33(p, data_ov000_023638c4);
        break;
    }
}
static void Boss6_NodeCallback_Store4Nodes_ov000_022e5df0(Boss6NodeRS *p) { Store4Nodes(p); }
static void Boss6_NodeCallback_Store4Nodes_ov000_022e60a4(Boss6NodeRS *p) { Store4Nodes(p); }

/* Model_Draw of the body with its NODEDESC callback (timing C): bs6_body's SBC runs NODEDESC for
 * joints 0..25 in index order (checked in the NSBMD).  The first call happens at joint 0 before
 * any matrix is read, so a G3_MultMtx33 there is applied as a joint-0 rotation before the pose;
 * the other calls store the posed joint matrices. */
static void boss6_draw_body(Boss6 *w, BossModel *sub) {
    Boss6NodeRS rs;
    int j, n;
    boss_model_set_joint_rot(sub, 0, -1, NULL);
    rs.m = sub;
    rs.node = 0;
    rs.done = 0;
    if (w->cb6 != NULL)
        w->cb6(&rs);
    boss_model_pose(sub, NULL);
    n = (sub->res != NULL && sub->res->loaded) ? sub->res->mdl.hdr->num_joints : 0;
    for (j = 1; j < n && w->cb6 != NULL && !rs.done; j++) {
        rs.node = j;
        w->cb6(&rs);
    }
}

/* Boss6_Draw (modified: Model_Draw -> boss_model_pose; MTX_RESTORE + NNS_G3dGetCurrentMtx -> the stored
 * slot matrices; the float angle steps are done in double as on the DS (softfloat _dadd) and truncated
 * through u32 like _dfixu) */
static void Boss6_Draw(void *p) {
    MtxFx33 mtx;
    Boss6 *w = (Boss6 *)p;
    s32 i;
    BossModel *sub;
    u16 idx;
    u32 flags;
    if (!(g_dcp_world.objmgr & 1)) { /* gObjManagerFlags & 1 */
        w->unkD50 = (u16)(u32)((double)w->unkD50 + 1.5170370370370372);
        w->unkD52 = (u16)(u32)((double)w->unkD52 + 6.674962962962963);
        sub = &w->unk210[1];
        idx = w->unkD50;
        MTX_RotY33(&sub->rot, SIN(idx), COS(idx));
        sub = &w->unk210[2];
        idx = w->unkD52;
        MTX_RotY33(&sub->rot, SIN(idx), COS(idx));
        Boss6_ScrollFadeModel(w);
        w->unkF68 = (s16)(w->unkF68 + w->unkF6A);
        if (w->unkF6A >= 0) {
            if (w->unkF68 > w->unkF6C)
                w->unkF68 = w->unkF6C;
        } else {
            if (w->unkF68 < w->unkF6C)
                w->unkF68 = w->unkF6C;
        }
        w->unkF66 = (s16)(w->unkF66 + w->unkF68);
    }
    s_base_shown = 0;
    if (!(w->unk20C & 1)) {
        sub = w->unk210;
        for (i = 0; i <= 2; i++) {
            Model_Animate(sub);
            boss_model_pose(sub, NULL);
            sub++;
        }
        s_base_shown = 1;
    }
    boss_model_pose(&w->unk210[w->unkD5C + 3], NULL);
    s_cloud_shown = w->unkD5C + 3;
    s_body_shown = 0;
    s_prop_n = 0;
    if (w->h.disp_flags & 0x20)
        return;
    sub = &w->unk210[6];
    sub->trans[0] = w->h.x << 4;
    sub->trans[1] = -w->h.y << 4;
    sub->trans[2] = w->h.z << 4;
    idx = (u16)w->unkF4A;
    MTX_RotZ33(&sub->rot, SIN(idx), COS(idx));
    if (w->unkF4C != 0) {
        idx = (u16)w->unkF4C;
        MTX_RotY33(&mtx, SIN(idx), COS(idx));
        MTX_Concat33(&sub->rot, &mtx, &sub->rot);
    }
    Model_Animate(sub);
    boss6_draw_body(w, sub);
    s_body_shown = 1;
    switch (gBossStatus.unk4) {
    case 0x1f:
    case 0x21:
        GetCurrentMtx(0x1b, w->unk1098, w->unk10C8); /* MTX_RESTORE 0x1b */
        /* fallthrough */
    case 0x20:
        GetCurrentMtx(0x1c, w->unk1044, w->unk1074); /* MTX_MODE 2, MTX_RESTORE 0x1c */
        break;
    }
    if (w->unkF68 < 0x238e) {
        sub = &w->unk210[7];
        idx = (u16)w->unkF66;
        MTX_RotY33(&sub->rot, SIN(idx), COS(idx));
    } else {
        sub = &w->unk210[8];
        idx = (u16)(w->unkF66 >> 1);
        MTX_RotY33(&sub->rot, SIN(idx), COS(idx));
    }
    sub->unk8 = 0x1e;
    Model_Animate(sub);
    boss_model_pose(sub, NULL);
    s_prop_mtx[0] = sub->mdl_mtx;
    sub->unk8 = 0x1d;
    Model_Animate(sub);
    boss_model_pose(sub, NULL);
    s_prop_mtx[1] = sub->mdl_mtx;
    s_prop_n = sub == &w->unk210[7] ? 7 : 8;
    flags = w->unk20C;
    if (flags & 4) {
        Boss6_UpdateAttackHitboxes(w);
        Boss6_RegisterHitboxes4(w);
        return;
    }
    if (flags & 8) {
        Boss6_RegisterHitboxes2(w);
        return;
    }
    if (flags & 0x10) {
        Boss6_RegisterHitboxes3x2(w);
        return;
    }
    if (w->unk1114 == 0)
        return;
    Boss_LoadModelPalettes(6, data_ov000_023657e0, gBoss6->unk110C, data_ov000_023636f4, 1);
    w->unk1114 = 0;
}

/* ============================================================ pieces (Boss6_CreatePiece) */
/* Boss6_State_PieceSlide (copied) */
static void Boss6_State_PieceSlide(void *p) {
    Boss6Piece *s = (Boss6Piece *)p;
    s->h.x += s->h.trick_input; /* unk10 += unk8 */
    s->h.timer++;               /* unkC */
    if (s->h.timer >= 0x14)
        s->h.obj_flags |= 4;
}

/* Boss6_State_PieceStartSlide (copied) */
static void Boss6_State_PieceStartSlide(void *pp) {
    Boss6Piece *p = (Boss6Piece *)pp;
    s32 v;
    Boss6 *g;
    v = -0x3200;
    g = gBoss6;
    if (g->unkD5E != 0)
        v *= -1;
    if (g->unk20C & 0x100)
        v *= -1;
    p->h.trick_input = v;
    p->h.timer = 0;
    p->func = Boss6_State_PieceSlide;
    Boss6_State_PieceSlide(p);
}

/* Boss6_State_PieceSpinFly (copied) */
static void Boss6_State_PieceSpinFly(void *pp) {
    Boss6Piece *p = (Boss6Piece *)pp;
    MtxFx33 mtx;
    u16 a;
    BossModel *cam;
    a = 0;
    p->h.timer++;
    p->h.x += p->h.trick_input;
    cam = &p->unk134;
    p->h.y -= 0x100;
    if (p->h.trick_input > 0)
        a = 0x8000;
    MTX_RotY33(&mtx, SIN(a), COS(a));
    MTX_RotZ33(&cam->rot, SIN((u16)(p->h.timer * 0x2aaa)), COS((u16)(p->h.timer * 0x2aaa)));
    MTX_Concat33(&cam->rot, &mtx, &cam->rot);
    if (p->h.timer >= 0x78)
        p->h.obj_flags |= 4;
}

/* Boss6_State_PieceWaitSpinFly (copied) */
static void Boss6_State_PieceWaitSpinFly(void *pp) {
    Boss6Piece *p = (Boss6Piece *)pp;
    s32 v;
    Boss6 *g;
    p->h.timer++;
    if (p->h.timer < 10)
        return;
    p->func = Boss6_State_PieceSpinFly;
    p->h.timer = 0;
    v = -0xa00;
    g = gBoss6;
    if (g->unkD5E != 0)
        v *= -1;
    if (g->unk20C & 0x100)
        v *= -1;
    p->h.trick_input = v;
}

/* Boss6_State_PieceDestroyOnAnimEnd (copied) */
static void Boss6_State_PieceDestroyOnAnimEnd(void *pp) {
    Boss6Piece *p = (Boss6Piece *)pp;
    BossModel *c = &p->unk134;
    s32 i;
    for (i = 0; i < 5; i++) {
        if (c->anm[i] != NULL && !(c->flags[i] & 2))
            return;
    }
    p->h.obj_flags |= 4;
}

/* data_ov000_023658ac: ROM addresses of the piece states (checked by tools/bossdata.py) */
static BossFn Boss6_PieceState(u32 rom) {
    switch (rom) {
    case 0x022dfcbc:
        return Boss6_State_PieceWaitSpinFly;
    case 0x022dfb68:
        return Boss6_State_PieceStartSlide;
    case 0x022dfd30:
        return Boss6_State_PieceDestroyOnAnimEnd;
    }
    return NULL;
}

/* Boss6_CreatePiece (modified: Task_Create -> pool; resources are the native packages) */
static Boss6Piece *Boss6_CreatePiece(s32 k, s32 x, s32 y, s32 z) {
    static Boss6Piece dummy; /* pool full (Task_Create failure): the callers write into it */
    Boss6Piece *w = NULL;
    BossModel *sub;
    int i;
    for (i = 0; i < B6_PIECES; i++)
        if (!s_piece[i].alive) {
            w = &s_piece[i];
            break;
        }
    if (w == NULL) {
        boss_log("piece_pool_full", k, 0);
        memset(&dummy, 0, sizeof(dummy));
        return &dummy;
    }
    boss_model_release(&w->unk134);
    memset(w, 0, sizeof(*w)); /* MIi_CpuClear16 */
    w->alive = 1;
    sub = &w->unk134;
    w->unk124 = sub;
    Model_Init(sub, 0);
    Model_SetResource(sub, gBoss6->unkD0C[k]);
    if (gBoss6->unkD1C[k] != NULL)
        Model_SetAnimation(sub, 0, gBoss6->unkD1C[k], 0);
    if (gBoss6->unkD2C[k] != NULL)
        Model_SetAnimation(sub, 1, gBoss6->unkD2C[k], 0);
    if (gBoss6->unkD3C[k] != NULL)
        Model_SetAnimation(sub, 2, gBoss6->unkD3C[k], 0); /* pattern animation (timing only) */
    sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
    w->h.x = x;
    w->h.y = y;
    w->h.z = z;
    w->func = Boss6_PieceState(data_ov000_023658ac[k]);
    w->h.move_flags |= 0x300;
    w->h.obj_flags |= 2;
    w->h.timer = 0;
    w->h.trick_input = 0;
    s_info.pieces[k & 3]++;
    return w;
}

/* Boss6_Destructor_Piece (StageObj_Destructor) */
static void Boss6_Destructor_Piece(void *p) { boss_model_release(&((Boss6Piece *)p)->unk134); }

/* StageObj_Draw3D step of the pieces (as for the boss effects in boss_effect.c) */
static void Boss6Piece_Draw(void *self) {
    Boss6Piece *w = (Boss6Piece *)self;
    BossModel *m = &w->unk134;
    u16 flags = w->h.disp_flags;
    u32 i;
    m->flag &= ~2u;
    for (i = 0; i < 5; i++)
        m->flags[i] &= ~5;
    if (flags & 4)
        for (i = 0; i < 5; i++)
            m->flags[i] |= 4;
    m->trans[0] = (w->h.x >> 8) << 12;
    m->trans[1] = -(w->h.y >> 8) << 12;
    m->trans[2] = (w->h.z >> 8) << 12;
    if (flags & 0x10)
        m->flag |= 2;
    if (!(flags & 0x1000))
        Model_Animate(m);
    if (flags & 0x20) {
        m->posed = 0;
        return;
    }
    boss_model_pose(m, NULL);
}

/* ============================================================ Boss6PartD4 (bullet) */
/* Boss6PartD4_State_Fly (copied) */
static inline s32 Abs(s32 x) {
    if (x < 0)
        x = -x;
    return x;
}
static void Boss6PartD4_State_Fly(void *p) {
    Boss6PartD4 *s = (Boss6PartD4 *)p;
    s32 v;
    s32 a;
    s->unk348 += s->unk34C;
    a = s->unk350 < 0 ? -s->unk350 : s->unk350;
    v = s->unk348 < 0 ? -s->unk348 : s->unk348;
    if (v > a)
        s->unk348 = s->unk350;
    s->h.z += s->unk348 >> 4;
    s->h.y += s->unk354 >> 4;
    if ((s->unk368 & 1) || Abs(gPlayers->z - (s->h.z + s->unk360)) > 0x3200) {
        s->coll0.flags &= ~4;
        s->coll1.flags &= ~4;
    } else {
        s->coll0.flags |= 4;
        s->coll1.flags |= 4;
    }
    s->unk364++;
    if (s->unk364 < 0x1f)
        return;
    s->h.obj_flags |= 8;
    s->h.obj_flags |= 2;
    s->unk1F4 |= 0x10;
    s->coll0.flags |= 0x1000;
    s->coll1.flags |= 0x1000;
}

/* Boss6PartD4_OnHit (copied; templates.py pointer code written with the field names): the bullet
 * (attacker, flag 8) touched the player */
static void Boss6PartD4_OnHit(ObjBox *p0, ObjBox *p1) {
    Boss6PartD4 *t1 = (Boss6PartD4 *)p0->owner;
    if (obj_box_player(p1) == NULL) /* *(u8 *)p1->unk18 != 1 */
        return;
    Boss6_CreatePiece(3, gPlayers->x, gPlayers->y, gPlayers->z);
    t1->unk368 |= 1;
    s_info.bullet_hits++;
    boss_log("bullet_hit", gPlayers->x >> 8, 0);
}

/* Boss6PartD4_Draw (modified: Model_Draw -> boss_model_pose; templates.py pointer code) */
static void Boss6PartD4_Draw(void *p) {
    Boss6PartD4 *t0 = (Boss6PartD4 *)p;
    BossModel *t1 = &t0->unk20C;
    t1->trans[0] = t0->h.x << 4;
    t1->trans[1] = -t0->h.y << 4;
    t1->trans[2] = t0->h.z << 4;
    Model_Animate(t1);
    boss_model_pose(t1, NULL);
}

/* MapObject_Collide_Default (the bullet's coll0 / coll1; coll2 has no owner) */
static void Boss6PartD4_Collide(void *p) {
    Boss6PartD4 *o = (Boss6PartD4 *)p;
    if (o->h.obj_flags & 0xc)
        return;
    if (o->coll0.owner != NULL)
        obj_handle_collider(&o->h, o, OBJ_OWNER_OBJECT, &o->coll0, 0);
    if (o->coll1.owner != NULL)
        obj_handle_collider(&o->h, o, OBJ_OWNER_OBJECT, &o->coll1, 0);
    if (o->coll2.owner != NULL)
        obj_handle_collider(&o->h, o, OBJ_OWNER_OBJECT, &o->coll2, 0);
}

/* Boss6PartD4_Destructor (modified: Model_Release + MapObject_Destructor -> rect_forget) */
static void Boss6PartD4_Destructor(void *p) {
    Boss6PartD4 *s = (Boss6PartD4 *)p;
    boss_model_release(&s->unk20C);
    rect_forget(&s->coll0);
    rect_forget(&s->coll1);
    rect_forget(&s->coll2);
}

/* Boss6PartD4_Create (modified: Task_Create -> pool; "bs6" file 8 and bs6_bult from BossStage6) */
static Boss6PartD4 *Boss6PartD4_Create(const ObjParam *p, s32 b, s32 c) {
    static Boss6PartD4 dummy;
    Boss6PartD4 *w = NULL;
    BossModel *sub;
    int i;
    for (i = 0; i < B6_D4; i++)
        if (!s_d4[i].alive) {
            w = &s_d4[i];
            break;
        }
    if (w == NULL) {
        boss_log("bullet_pool_full", 0, 0);
        memset(&dummy, 0, sizeof(dummy));
        return &dummy;
    }
    boss_model_release(&w->unk20C);
    memset(w, 0, sizeof(*w)); /* MIi_CpuClear16 */
    w->alive = 1;
    MapObject_Init6(&w->h, &w->coll0, p, b, c);
    w->func = Boss6PartD4_State_Fly;
    w->h.disp_flags |= 0x20;
    w->h.obj_flags |= 0x10;
    w->h.move_flags |= 0xa300;
    w->unk344 = &s_bs6_ca; /* NNS_FndGetArchiveFileByIndex(ctx, 8) */
    sub = &w->unk20C;
    Model_Init(sub, 0);
    Model_SetResource(sub, &data_ov000_02369b50[9]);
    sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
    w->coll0.owner = w;
    rect_set_atkdef(&w->coll0, 0x13, (u32)-1);
    w->coll0.flags &= ~4;
    w->coll0.flags |= 0x28;
    w->coll0.cb = Boss6PartD4_OnHit;
    w->coll1 = w->coll0;
    w->coll2.flags |= 0x1000;
    if (gBoss6->unkD5E != 0) {
        w->unk348 = -0x8000;
        w->unk34C = -0x1000;
        w->unk350 = -0xf000;
        w->unk360 = 0x17800;
    } else {
        w->unk348 = 0x8000;
        w->unk34C = 0x1000;
        w->unk350 = 0xf000;
        w->unk360 = -0x17800;
    }
    w->unk354 = 0x36db;
    s_info.bullets++;
    return w;
}

/* ============================================================ create / destroy */
/* Boss6_Destructor (modified: resources are released by boss6_unload / boss_effects_shutdown) */
static void Boss6_Destructor(void *p) {
    Boss6 *s = (Boss6 *)p;
    s32 i;
    for (i = 0; i < 9; i++)
        boss_model_release(&s->unk210[i]);
    /* NNS_G3dResDefaultRelease of the piece models and of bs6:eff_pick / eff_parts / eff_impact_small /
     * eff_land: boss6_unload / boss_effects_shutdown */
    gBoss6 = NULL;
}

/* piece resources by archive file index (data_ov000_02363864) */
static const void *piece_file(s16 idx) {
    switch (idx) {
    case 0x35:
        return &s_plate;
    case 0x36:
        return &s_plate_ma;
    case 0x37:
        return &s_wind;
    case 0x38:
        return &s_shoot;
    case 0x39:
        return &s_shoot_ma;
    case 0x3a:
        return &s_bomb;
    case 0x3b:
        return &s_bomb_ca;
    case 0x3c:
        return &s_bomb_ma;
    case 0x3d:
        return &s_bomb_tp;
    }
    return NULL;
}

/* Boss6_Create (modified: archive files -> converted packages / timing-only animations; callbacks,
 * palettes and model alpha native) */
void *Boss6_Create(const ObjParam *p, s32 b, s32 c) {
    Boss6 *w;
    BossModel *sub;
    s32 i;
    s32 idx;
    if (p->x0 == 0xff && p->y0 == 0xff)
        return NULL;
    w = &s_boss6;
    for (i = 0; i < 9; i++)
        boss_model_release(&w->unk210[i]);
    memset(w, 0, sizeof(*w));
    memset(&s_info, 0, sizeof(s_info));
    memset(s_pal, 0, sizeof(s_pal));
    w->alive = 1;
    gBoss6 = w;
    w->unk20C |= 1;
    w->unkD4E = (s16)(Rand() & 3);
    memset(&gBossStatus, 0, sizeof(gBossStatus));
    gBossStatus.unk4 = 0x1c;
    gBossStatus.unk8 = (u16)Boss_GetStartHealth();
    gBossStatus.unkA = (u16)Boss_GetStartHealth();
    MapObject_Init6(&w->h, &w->coll0, p, b, c);
    w->func = Boss6_State_WaitForStart;
    w->h.disp_flags |= 0x20;
    w->h.obj_flags |= 0x12;
    w->h.move_flags |= 0xa300;
    w->unkD08 = &s_bs6_ca; /* "bs6" file 8 */
    for (i = 0; i < 4; i++) {
        if (data_ov000_02363864[i][0] != -1)
            w->unkD0C[i] = (const BossModelRes *)piece_file(data_ov000_02363864[i][0]); /* + NNS_G3dResDefaultSetup */
        if (data_ov000_02363864[i][1] != -1)
            w->unkD1C[i] = (const BossAnimRes *)piece_file(data_ov000_02363864[i][1]);
        if (data_ov000_02363864[i][2] != -1)
            w->unkD2C[i] = (const BossAnimRes *)piece_file(data_ov000_02363864[i][2]);
        if (data_ov000_02363864[i][3] != -1)
            w->unkD3C[i] = (const BossAnimRes *)piece_file(data_ov000_02363864[i][3]);
    }
    /* NNS_G3dResDefaultSetup("bs6:eff_pick / eff_parts / eff_impact_small / eff_land.nsbmd"): boss_effects_load */
    sub = w->unk210;
    for (i = 0; i < 9; i++) {
        Model_Init(sub, 0);
        Model_SetResource(sub, &data_ov000_02369b50[i]);
        sub->scale[0] = sub->scale[1] = sub->scale[2] = 0x34cc;
        sub++;
    }
    sub = &w->unk210[0];
    sub->trans[0] = 0x1a0000;
    sub->trans[1] = -0x200000;
    sub->trans[2] = 0;
    sub = &w->unk210[1];
    sub->trans[0] = 0x1a0000;
    sub->trans[1] = -0x200000;
    sub->trans[2] = 0;
    sub = &w->unk210[2];
    sub->trans[0] = 0x1a0000;
    sub->trans[1] = -0x200000;
    sub->trans[2] = 0;
    sub = &w->unk210[3];
    sub->trans[0] = 0x1a0000;
    sub->trans[1] = -0x200000;
    sub->trans[2] = -0x32000;
    boss6_set_mdl_alpha(&data_ov000_02369b50[3], 0); /* NNS_G3dMdlSetMdlAlphaAll(.., 0) */
    sub = &w->unk210[4];
    sub->trans[0] = (s32)0xffdc20f0;
    sub->trans[1] = -0x200000;
    sub->trans[2] = 0x32000;
    boss6_set_mdl_alpha(&data_ov000_02369b50[4], 0);
    sub = &w->unk210[6];
    sub->trans[0] = 0x1a0000;
    sub->trans[1] = -0x200000;
    sub->trans[2] = 0;
    w->cb6 = Boss6_NodeCallback_Store2Nodes_ov000_022e62fc; /* NNS_G3dRenderObjSetCallBack(sub->unk90, ..) */
    sub->cb[0] = MOP_FLUSHVP; /* unkA = 4 */
    sub = &w->unk210[7];
    sub->trans[0] = 0;
    sub->trans[1] = 0;
    sub->trans[2] = 0;
    sub->unk8 = 0x1e;
    sub->cb[0] = MOP_FLUSHVP; /* unkA = 4 */
    sub->cb[1] = MOP_RESTORE; /* unkB = 2 */
    sub->cb[2] = MOP_ROTATE;  /* unkC = 0xb */
    sub = &w->unk210[8];
    sub->trans[0] = 0;
    sub->trans[1] = 0;
    sub->trans[2] = 0;
    sub->unk8 = 0x1e;
    sub->cb[0] = MOP_FLUSHVP;
    sub->cb[1] = MOP_RESTORE;
    sub->cb[2] = MOP_ROTATE;
    Boss6_InitHitboxes(w);
    w->unkF46 = 1;
    w->unkF48 = 0xffff;
    w->unkD54[0] = -0x17ff;
    w->unkD58 = 0x17ff;
    idx = gGameState;
    w->unk10F0 = &data_ov000_0236370c[idx];
    w->unk10F4 = data_ov000_02363884[idx];
    w->unk10F8 = data_ov000_023638a4[idx];
    w->unk10FC = &data_ov000_02363714[idx];
    w->unk1100 = &data_ov000_023636f8[idx];
    w->unk1104 = &data_ov000_02363708[idx];
    w->unk1108 = &data_ov000_02363700[idx];
    w->unk110C = &data_ov000_02363704[idx];
    w->unk1110 = &data_ov000_023636fc[idx];
    Boss_LoadModelPalettes(6, data_ov000_023658e4, gBoss6->unk10F4, data_ov000_0236377c, 8);
    Boss_LoadModelPalettes(7, data_ov000_023657e8, gBoss6->unk10FC, data_ov000_023636e8, 1);
    Boss_LoadModelPalettes(8, data_ov000_023657ec, gBoss6->unk1104, data_ov000_023636f0, 1);
    Boss_LoadModelPalettes(9, data_ov000_023657e4, gBoss6->unk10F0, data_ov000_023636ec, 1);
    s_pal_core = *gBoss6->unk110C;
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

/* BossStage6_OnModelLoaded: data_020746c0 = the 10 bs6 models (NaviBg_Load / NaviB_Load: HUD, not
 * here); plus the z63 archive files Boss6_Create / Boss6PartD4_Create read */
int boss6_load(void) {
    static const char *const files[10] = {
        "mod__bs6_base__bs6_base.dcm",         "mod__bs6_rcloud01__bs6_rcloud01.dcm",
        "mod__bs6_rcloud02__bs6_rcloud02.dcm", "mod__bs6_tcloud01__bs6_tcloud01.dcm",
        "mod__bs6_tcloud02__bs6_tcloud02.dcm", "mod__bs6_sky__bs6_sky.dcm",
        "mod__bs6_body__bs6_body.dcm",         "mod__bs6_prop01__bs6_prop01.dcm",
        "mod__bs6_prop02__bs6_prop02.dcm",     "mod__bs6_bult__bs6_bult.dcm"};
    int i, r = 0;
    for (i = 0; i < 10; i++)
        if (!data_ov000_02369b50[i].loaded)
            r |= boss_res_load_model(&data_ov000_02369b50[i], files[i]);
    if (!s_bs6_ca.loaded)
        r |= boss_res_load_anim(&s_bs6_ca, "narc__z63_act_lz7__bs6.dca");
    if (!s_plate.loaded)
        r |= boss_res_load_model(&s_plate, "narc__z63_act_lz7__eff_plate__eff_plate.dcm");
    if (!s_wind.loaded)
        r |= boss_res_load_model(&s_wind, "narc__z63_act_lz7__eff_wind__eff_wind.dcm");
    if (!s_shoot.loaded)
        r |= boss_res_load_model(&s_shoot, "narc__z63_act_lz7__eff_shoot__eff_shoot.dcm");
    if (!s_bomb.loaded) /* z63 eff_shoot_bomb is identical to ss_act_lz7's (deduplicated) */
        r |= boss_res_load_model(&s_bomb, "narc__ss_act_lz7__eff_shoot_bomb__eff_shoot_bomb.dcm");
    if (!s_bomb_ca.loaded)
        r |= boss_res_load_anim(&s_bomb_ca, "narc__ss_act_lz7__eff_shoot_bomb.dca");
    r |= load_lengths(&s_plate_ma, "eff_plate.nsbma");
    r |= load_lengths(&s_shoot_ma, "eff_shoot.nsbma");
    r |= load_lengths(&s_bomb_ma, "eff_shoot_bomb.nsbma");
    r |= load_lengths(&s_bomb_tp, "eff_shoot_bomb.nsbtp");
    for (i = 0; i < 4; i++) {
        s_joint[i] = boss_joint_index("bs6_body", ROM_STR(data_ov000_023658bc[i])); /* pro_r, pro_l, pelvis, core */
        if (s_joint[i] < 0)
            r = -1;
    }
    return r ? -1 : 0;
}

/* BossStage6_FreeModels (+ the archive files) */
void boss6_unload(void) {
    int i;
    for (i = 0; i < 10; i++)
        boss_res_free_model(&data_ov000_02369b50[i]);
    boss_res_free_anim(&s_bs6_ca);
    boss_res_free_model(&s_plate);
    boss_res_free_model(&s_wind);
    boss_res_free_model(&s_shoot);
    boss_res_free_model(&s_bomb);
    boss_res_free_anim(&s_bomb_ca);
}

static const BossObjOps s_ops_boss6 = {Boss6_Draw, NULL, Boss6_Destructor};
static const BossObjOps s_ops_d4 = {Boss6PartD4_Draw, Boss6PartD4_Collide, Boss6PartD4_Destructor};
static const BossObjOps s_ops_piece = {Boss6Piece_Draw, NULL, Boss6_Destructor_Piece};

void boss6_destroy(void) {
    int i;
    for (i = 0; i < B6_D4; i++)
        if (s_d4[i].alive) {
            Boss6PartD4_Destructor(&s_d4[i]);
            s_d4[i].alive = 0;
        }
    for (i = 0; i < B6_PIECES; i++)
        if (s_piece[i].alive) {
            Boss6_Destructor_Piece(&s_piece[i]);
            s_piece[i].alive = 0;
        }
    if (s_boss6.alive)
        Boss6_Destructor(&s_boss6);
    s_boss6.alive = 0;
    gBoss6 = NULL;
}

void boss6_update(void) {
    int i;
    if (s_boss6.alive)
        boss_obj_main(&s_boss6.h, &s_boss6.func, &s_ops_boss6, &s_boss6, &s_boss6.alive); /* 0x500 */
    for (i = 0; i < B6_D4; i++)                                                           /* 0x502 */
        if (s_d4[i].alive)
            boss_obj_main(&s_d4[i].h, &s_d4[i].func, &s_ops_d4, &s_d4[i], &s_d4[i].alive);
    for (i = 0; i < B6_PIECES; i++)                                                       /* 0xa00 */
        if (s_piece[i].alive)
            boss_obj_main(&s_piece[i].h, &s_piece[i].func, &s_ops_piece, &s_piece[i], &s_piece[i].alive);
}

int boss6_draw(int list) {
    int n = 0, i;
    Boss6 *s = &s_boss6;
    if (!s->alive)
        return 0;
    g_boss_debug_color = 0x4080ff;
    if (s_base_shown)
        for (i = 0; i <= 2; i++)
            n += boss_model_draw(&s->unk210[i], list);
    g_boss_debug_color = 0xc0c0c0;
    if (s_cloud_shown >= 3)
        n += boss_model_draw(&s->unk210[s_cloud_shown], list);
    if (s_body_shown) {
        g_boss_debug_color = 0xff4040;
        n += boss_model_draw(&s->unk210[6], list);
        if (s_prop_n) {
            BossModel *a = &s->unk210[s_prop_n];
            dcm_mtx_t keep = a->mdl_mtx;
            g_boss_debug_color = 0x40ff40;
            a->mdl_mtx = s_prop_mtx[0];
            n += boss_model_draw(a, list);
            a->mdl_mtx = s_prop_mtx[1];
            n += boss_model_draw(a, list);
            a->mdl_mtx = keep;
        }
    }
    g_boss_debug_color = 0xff8040;
    for (i = 0; i < B6_D4; i++) /* Boss6PartD4_Draw draws even with disp 0x20 (set by Boss6PartD4_Create) */
        if (s_d4[i].alive && s_d4[i].unk20C.posed)
            n += boss_model_draw(&s_d4[i].unk20C, list);
    g_boss_debug_color = 0xffff40;
    for (i = 0; i < B6_PIECES; i++)
        if (s_piece[i].alive && s_piece[i].unk134.posed)
            n += boss_model_draw(&s_piece[i].unk134, list);
    return n;
}

/* ------------------------------------------------------------ test / debug accessors */
void boss6_body_pos(s32 *x, s32 *y, s32 *z) {
    *x = s_boss6.h.x;
    *y = s_boss6.h.y;
    *z = s_boss6.h.z;
}
int boss6_box_armed(int box) {
    if (box < 0 || box >= 11 || !s_boss6.alive)
        return 0;
    return (s_boss6.unkD60[box].flags & 4) && !(s_boss6.unkD60[box].flags & 0x1000);
}
int boss6_info(Boss6Info *o) {
    int i;
    Boss6 *s = &s_boss6;
    s_info.help_shown = boss_help() >= 0;
    *o = s_info;
    if (!s->alive)
        return 0;
    o->x = s->h.x;
    o->y = s->h.y;
    o->turned = s->unkD5E;
    o->tilt_left = (s->unk20C & 0x100) != 0;
    o->prop_speed = s->unkF68;
    o->core_x = s->unk10BC >> 12;
    o->core_y = -(s->unk10C0 >> 12);
    o->pelvis_x = s->unk1068 >> 12;
    o->pelvis_y = -(s->unk106C >> 12);
    for (i = 0; i < 11; i++) {
        o->box[i] = s->unkD60[i];
        o->armed[i] = (u8)boss6_box_armed(i);
    }
    o->bullets_alive = 0;
    for (i = 0; i < B6_D4; i++)
        if (s_d4[i].alive) {
            if (o->bullets_alive < 8) {
                o->bullet_box[o->bullets_alive][0] = s_d4[i].coll0;
                o->bullet_box[o->bullets_alive][1] = s_d4[i].coll1;
            }
            o->bullets_alive++;
        }
    o->pieces_alive = 0;
    for (i = 0; i < B6_PIECES; i++)
        o->pieces_alive += s_piece[i].alive;
    o->palette_body = s_pal[6];
    o->palette_core = s_pal_core;
    return 1;
}

#define N(f) {(void *)f, #f}
static const struct { void *f; const char *n; } s_names[] = {
    N(Boss6_State_WaitForStart), N(Boss6_State_Move), N(Boss6_State_PartD4Attack), N(Boss6_State_PickApproach),
    N(Boss6_State_PickStop), N(Boss6_State_PickRaise), N(Boss6_State_PickAim), N(Boss6_State_PickSwing),
    N(Boss6_State_PickImpact), N(Boss6_State_PickPullOut), N(Boss6_State_PickTurnAround), N(Boss6_State_TiltStop),
    N(Boss6_State_TiltWaitJump), N(Boss6_State_TiltJump), N(Boss6_State_TiltLand), N(Boss6_State_TiltStart),
    N(Boss6_State_Tilting), N(Boss6_State_TiltReturn), N(Boss6_State_TiltEnd), N(Boss6_State_WaitSelectAttack3),
    N(Boss6_State_EdgeJumpStop), N(Boss6_State_EdgeJumpLeap), N(Boss6_State_EdgeJumpLand),
    N(Boss6_State_EdgeJumpRecover), N(Boss6_State_StartJump), N(Boss6_State_JumpRise), N(Boss6_State_JumpFall),
    N(Boss6_State_LandShake), N(Boss6_State_AfterLand), N(Boss6_State_EndJumps), N(Boss6_State_WaitSelectAttack),
    N(Boss6_State_Hurt), N(Boss6_State_HurtRecover), N(Boss6_State_HurtWaitSelectAttack), N(Boss6_State_HurtFlip),
    N(Boss6_State_Defeat), N(Boss6_State_DefeatFinish), N(Boss6_State_PlayerStunned),
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
const char *boss6_state_name(int part) {
    if (!s_boss6.alive)
        return "-";
    if (part == 0)
        return name_of((const void *)s_boss6.func);
    if (part == 1)
        return gPlayers != NULL ? name_of((const void *)gPlayers->state) : "-";
    return "-";
}

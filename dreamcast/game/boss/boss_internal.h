/* Internal interfaces of the native boss module (not for the integrator).
 *
 * Field names keep the original work-struct offset (unk20C = @0x20C) so the
 * translated functions read like the decompilation in src/game/Boss/.  Where
 * a DS struct is replaced by a native one the offset is in the comment. */
#ifndef BOSS_INTERNAL_H
#define BOSS_INTERNAL_H

#include "boss.h"
#include "../objects/objects.h" /* ObjBox, ObjParam (obj_types.h), g_obj_stage */

/* Original function names are kept in the source; the link symbols get a
 * boss_ prefix so they cannot clash with other modules that translate the
 * same DS functions (ScreenShake, Model, NaviMsg, StageObj pause). */
#define FX_Div boss_FX_Div
#define MTX_Identity33 boss_MTX_Identity33
#define MTX_RotX33 boss_MTX_RotX33
#define MTX_RotY33 boss_MTX_RotY33
#define MTX_RotZ33 boss_MTX_RotZ33
#define MTX_Concat33 boss_MTX_Concat33
#define Model_Init boss_Model_Init
#define Model_SetResource boss_Model_SetResource
#define Model_SetAnimation boss_Model_SetAnimation
#define Model_Animate boss_Model_Animate
#define gBossStatus boss_gBossStatus
#define Boss_GetStartHealth boss_Boss_GetStartHealth
#define Boss_DrawExcBackground boss_Boss_DrawExcBackground
#define Boss_Atan2 boss_Boss_Atan2
#define BossArena_ArcToCirclePos boss_BossArena_ArcToCirclePos
#define BossArena_CirclePosToArc boss_BossArena_CirclePosToArc
#define ScreenShake_Create boss_ScreenShake_Create
#define ScreenShake_Start boss_ScreenShake_Start
#define ScreenShake_Stop boss_ScreenShake_Stop
#define ScreenShake_Main boss_ScreenShake_Main
#define ScreenShake_GetOffsetX boss_ScreenShake_GetOffsetX
#define ScreenShake_GetOffsetY boss_ScreenShake_GetOffsetY
#define data_ov000_0236a444 boss_data_ov000_0236a444
#define BossCamera_Create boss_BossCamera_Create
#define BossCamera_Main boss_BossCamera_Main
#define BossCamera_Destructor boss_BossCamera_Destructor
#define BossEffect_Create boss_BossEffect_Create
/* BossEffect_Create<Name> wrappers keep their names (boss_ prefix through BOSSEFFECT_WRAPPERS below) */
#define EffPebble_Create boss_EffPebble_Create
#define NaviMsg_SetFaceAnim boss_NaviMsg_SetFaceAnim
#define NaviMsg_ShowMessage boss_NaviMsg_ShowMessage
#define StageObj_SetPause boss_StageObj_SetPause
#define StageObj_ClearPause boss_StageObj_ClearPause
#define Boss1_Create boss_Boss1_Create
#define Boss2_Create boss_Boss2_Create
#define Boss3_Create boss_Boss3_Create
#define Boss4_Create boss_Boss4_Create

/* Object-system entry points the boss parts use (dreamcast/game/objects,
 * declared here instead of including obj_internal.h). */
void rect_set_box3d(ObjBox *b, s16 l, s16 t, s16 r, s16 bb, s16 zn, s16 zf); /* ObjRect_SetBox3D */
void rect_set_atkdef(ObjBox *b, u32 atk, u32 def);                           /* ObjRect_SetAtkDefFlags */
void rect_forget(ObjBox *b);                                                 /* drop a freed box */
void obj_handle_collider(DcPlayer *h, void *owner, int kind, ObjBox *b, int has_sprite); /* StageObj_HandleCollider */
dcp_bool obj_view_out(s32 x, s32 y, s16 ofs, s16 l, s16 t, s16 r, s16 b);    /* StageObj_ViewOutCheck */
dcp_bool obj_pause_check(u32 f);                                             /* StageObj_PauseCheck */
DcPlayer *obj_box_player(const ObjBox *b);                                   /* box owner if it is a player */
void obj_player_attack_bounce(DcPlayer *p);                                  /* Player_Action_AttackBounce */
void rect_register(ObjBox *b);                                               /* ObjRect_Register */

typedef s32 fx32;
typedef struct { fx32 x, y, z; } BossVec;   /* VecFx32 */

/* ------------------------------------------------------- ROM data */
/* bossdata.bin (tools/bossdata.py): ROM tables keyed by DS address */
const void *boss_data(u32 ds_addr);
#define BOSS_DATA(addr, type) ((type)boss_data(addr))

/* ---------------------------------------------------- fixed point */
static inline fx32 FX_Mul(fx32 a, fx32 b) { return (fx32)(((s64)a * b + 0x800) >> 12); }
fx32 FX_Div(fx32 n, fx32 d);                /* (n << 12) / d, DS divider */
#define SIN(a) dcp_sin((u16)(a))            /* data_0206e470[((a & 0xffff) >> 4) * 2] */
#define COS(a) dcp_cos((u16)(a))
#define Rand() dcp_rand()                   /* gRandSeed LCG */
typedef struct { fx32 m[9]; } MtxFx33;      /* row-major, row vectors (NitroSDK) */
void MTX_Identity33(MtxFx33 *m);
void MTX_RotX33(MtxFx33 *m, fx32 s, fx32 c);
void MTX_RotY33(MtxFx33 *m, fx32 s, fx32 c);
void MTX_RotZ33(MtxFx33 *m, fx32 s, fx32 c);
void MTX_Concat33(const MtxFx33 *a, const MtxFx33 *b, MtxFx33 *ab); /* ab = a * b */

/* ------------------------------------------------- 3D model runtime */
/* Converted G3D resource: a .dcm model (+ textures) */
typedef struct BossModelRes {
    dcm_model_t mdl;
    void *bytes;
    int loaded;
} BossModelRes;
typedef struct BossAnimRes {
    dca_set_t set;
    void *bytes;
    int loaded;
    /* timing-only resource (an animation file that is not converted, e.g.
     * .nsbta): frame count per animation index, from bossdata.bin, so the
     * "ended" flag protocol of Model_AnimateMdl keeps the original timing */
    const u16 *nframes;
    u32 count;
} BossAnimRes;

/* NNSG3dAnmObj subset: frame @0x00, ratio @0x04, resource @0x08 (numFrame @+4) */
typedef struct BossAnmObj {
    fx32 frame;
    fx32 ratio;
    const BossAnimRes *res;
    int idx;               /* animation index in res */
    u16 numFrames;         /* res->anims[idx].num_frames */
} BossAnmObj;

/* Model (src/game/Graphics/Model, Model_Init: 0x134 bytes) */
typedef struct BossModel {
    u32 type;              /* @0x00 0 = joint model, 1 = shape */
    u32 flag;              /* @0x04 bit0 hidden, bit1 do not advance animations (ctx flags) */
    u8 unk8;               /* @0x08 matrix stack slot (MTX_RESTORE) */
    u8 pad9;
    u8 cb[8];              /* @0x0A matrix op list (gModelMatrixOpTable index), unkA/unkB/unkC.. */
    fx32 scale[3];         /* @0x18 */
    MtxFx33 rot;           /* @0x24 */
    fx32 trans[3];         /* @0x48 */
    BossAnmObj anm_obj[5][2]; /* storage: NNS_G3dAllocAnmObj (two per slot for the cross-fade) */
    BossAnmObj *anm[5];    /* @0xE4 */
    BossAnmObj *anm2[5];   /* @0xF8 cross-fade source (flags 0x40) */
    u8 flags[5];           /* @0x10C bit0 paused, bit1 ended, bit2 loop, 0x40 cross-fade, 0x80 fade done */
    fx32 speed;            /* @0x114 */
    fx32 speeds[5];        /* @0x118 */
    s16 blendSpeed;        /* @0x12C */
    s16 blendSpeeds[5];    /* @0x12E */
    const BossModelRes *res;
    /* native: pose of the last draw step (Model_Draw), used by boss_draw */
    dcm_mtx_t mdl_mtx;
    dcm_mtx_t *palette;    /* [num_palette] */
    int posed;
    /* native: node callbacks that multiply a joint's matrix (MTX_MULT_3x3 at a NODEDESC, e.g.
     * Boss1_NodeCallback_Batteries): rotation applied to the joint's local transform at the pose */
    int jr_n;
    int jr_joint[2];
    MtxFx33 jr_rot[2];
} BossModel;

enum { MOP_NONE = 0, MOP_IDENTITY, MOP_RESTORE, MOP_FLUSHP, MOP_FLUSHVP, MOP_FLUSHWVP };
#define MOP_ROTATE 0x0b       /* Model_MatrixOp_Rotate: MTX_MULT_3x3 with the model rot (@0x24) */
#define MOP_SETCAMERAROT 0x13
/* geometry-engine matrix stack (MTX_STORE / MTX_RESTORE slots 0..30): the node
 * callbacks store joint matrices here, models with MOP_RESTORE read slot unk8 */
extern dcm_mtx_t g_boss_mtx_slot[32];
/* lights for boss_model_draw (NULL: dcm stage defaults); view space */
extern const dcm_lights_t *g_boss_lights;
/* NNS_G3dMdlSetMdlDiffAll / AmbAll / EmiAll on a loaded model (GXRgb colours) */
void boss_model_set_colors(const BossModelRes *r, u16 diff, u16 amb, u16 emi);

void Model_Init(BossModel *s, u32 a);
void Model_SetResource(BossModel *s, const BossModelRes *res);
void Model_SetAnimation(BossModel *o, u32 slot, const BossAnimRes *res, u32 idx);
void Model_Animate(BossModel *s);
/* native Model_Draw: build the model matrix from cb/scale/rot/trans (or the
 * restored matrix slot) and pose the joints; keeps the result for boss_draw */
void boss_model_pose(BossModel *m, const dcm_mtx_t *restored);
/* world matrix of joint `joint` after boss_model_pose (NNS_G3dGetCurrentMtx in a node callback) */
void boss_model_joint_world(const BossModel *m, int joint, dcm_mtx_t *out);
/* node index by name (NNS_G3dGetResDictIdxByName), from the bossdata.bin joint table; -1 if absent */
int boss_joint_index(const char *model, const char *node);
/* i-th effect package of a boss stage (bossdata.bin effect table); -1 past the end */
int boss_effect_file(int stage, int i, const char **name, const char **model, const char **anim);
int boss_model_draw(const BossModel *m, int list);
/* set (or clear with r = NULL) a joint rotation for the next boss_model_pose (see BossModel.jr_*) */
void boss_model_set_joint_rot(BossModel *m, int slot, int joint, const MtxFx33 *r);
extern u8 *g_boss_debug_fb;   /* host debug view (boss_model.c) */
extern u32 g_boss_debug_color;
void boss_model_release(BossModel *m);
int boss_res_load_model(BossModelRes *r, const char *file);
int boss_res_load_anim(BossAnimRes *r, const char *file);
void boss_res_free_model(BossModelRes *r);
void boss_res_free_anim(BossAnimRes *r);

/* ------------------------------------------------------ boss status */
typedef struct BossStatus {   /* gBossStatus (0xC bytes) */
    u16 unk0;                 /* bit0: fight started (StageTitleCard) */
    u32 unk4;                 /* phase (BOSS_PHASE_*) */
    u16 unk8;                 /* health */
    u16 unkA;                 /* start health */
} BossStatus;
extern BossStatus gBossStatus;

typedef void (*BossDrawPlayerFn)(void);
/* gGameState / gDisplayMain fields the boss code reads or writes */
typedef struct BossGame {
    s32 unk0;                 /* gGameState.unk0 (palette table index) */
    s32 unk14;                /* gGameState.unk14 (difficulty index) */
    s16 bright;               /* gDisplayMain.unk48 (master brightness, 16 = white) */
    s32 exc_bg;               /* Boss_DrawExcBackground index shown, -1 = none */
    int cleared;
    DcPlayer *player;         /* gPlayers */
    s32 arena_len, arena_circ; /* gCamera.unkE0 / unkE4 (Camera_Create: boss arena radius / circumference) */
    /* gPlayers->unk70 = Boss2_Draw_PlayerOnArena: 3D transform of the player model */
    BossDrawPlayerFn draw_player;
    s32 player_trans[3];      /* @0x17C model translation (fx32 px) */
    MtxFx33 player_rot;       /* @0x158 model rotation */
    int player_trans_valid;
} BossGame;
extern BossGame g_boss_game;
#define gPlayers (g_boss_game.player)
s32 Boss_GetStartHealth(void);
void Boss_DrawExcBackground(u32 id);
s32 Boss_Atan2(s32 y, s32 x);
void BossArena_ArcToCirclePos(s32 len, s32 b, s32 a, s32 *outX, s32 *outY, u16 *outAng);
void BossArena_CirclePosToArc(s32 unused, s32 len, s32 x, s32 y, s32 *out);

/* ------------------------------------------------------ ScreenShake */
void ScreenShake_Create(void);
void ScreenShake_Start(u8 v);
void ScreenShake_Stop(void);
void ScreenShake_Main(void);
s8 ScreenShake_GetOffsetX(void);
s8 ScreenShake_GetOffsetY(void);

/* ------------------------------------------------------- BossCamera */
/* data_ov000_0236a444 (0xE0 bytes + the sky Model at 0xE0).
 * One track (0x3C bytes) per camera point: t[0] @0x48 eye, t[1] @0x84 at. */
typedef struct BossCamTrack {
    DcPlayer *ox, *oy, *oz;   /* +0x00 objects followed per axis (StageObj header) */
    fx32 offx, offy, offz;    /* +0x0C */
    BossVec tgt;              /* +0x18 target */
    BossVec cur;              /* +0x24 current */
    fx32 lerp;                /* +0x30 */
    fx32 speed;               /* +0x34 */
    fx32 accel;               /* +0x38 */
} BossCamTrack;
typedef struct BossCamera {
    u32 unk0;                 /* @0x00 bit0 sky model, bit1/2 track the model translation */
    fx32 unk4;                /* @0x04 half fovy (angle) */
    fx32 unk8;                /* @0x08 near */
    fx32 unkC;                /* @0x0C far */
    fx32 unk10;               /* @0x10 aspect */
    fx32 unk14;               /* @0x14 W scale */
    BossVec unk18;            /* @0x18 eye */
    BossVec unk24;            /* @0x24 at */
    BossVec unk30;            /* @0x30 up */
    BossVec unk3C;            /* @0x3C shake offset (cleared every frame) */
    BossCamTrack t[2];        /* @0x48 eye track, @0x84 at track */
    fx32 unkC0, unkC4, unkC8; /* @0xC0 orbit distance target / current / lerp */
    fx32 unkCC, unkD0;        /* @0xCC orbit height target / current */
    s16 unkD4;                /* @0xD4 height lerp */
    u16 unkD6, unkD8;         /* @0xD6 orbit angle target / current */
    s16 unkDA;                /* @0xDA angle lerp */
    u32 unkDC;                /* @0xDC mode: 0 fixed, 1 track0, 2 track1 */
    BossModel unkE0;          /* @0xE0 sky model (data_ov000_0236a524) */
    int active;
} BossCamera;
extern BossCamera data_ov000_0236a444;
void BossCamera_Create(s32 mode);
void BossCamera_Main(void);
void BossCamera_Destructor(void);

/* -------------------------------------------------- objects (parts) */
typedef struct BossObj BossObj;
typedef void (*BossFn)(void *self);
/* StageObj_Main for the boss parts and effects (obj_manager.c obj_main subset) */
typedef struct BossObjOps {
    BossFn draw;              /* @0x70 */
    BossFn collide;           /* @0x78 */
    BossFn dtor;
} BossObjOps;
void boss_obj_main(DcPlayer *h, BossFn *state, const BossObjOps *ops, void *self, int *alive);
/* StageObj.unk124 (the object's Model): BossCamera tracks its translation when camera
 * flags 2/4 are set.  m = NULL removes the entry.  boss_obj_model_trans returns the
 * model translation (fx32), the player model transform for gPlayers, or NULL. */
void boss_obj_set_model(const DcPlayer *o, const struct BossModel *m);
const fx32 *boss_obj_model_trans(const DcPlayer *o);

/* ----------------------------------------------------- BossEffect */
typedef struct BossEffRes BossEffRes;
typedef struct BossEffect {
    DcPlayer h;               /* StageObj header */
    BossFn state;             /* @0x68 */
    s32 unk8, unkC;           /* @0x08 delay frames, @0x0C lifetime (0 = until all animations end) */
    BossModel unk134;         /* @0x134 model (scale @0x14C, rot @0x158, speed @0x248) */
    u32 unk2A0;               /* @0x2A0 BossEffect_Create flags */
    BossFn unk2A4;            /* @0x2A4 per-frame callback */
    int unk2AC;               /* @0x2AC .nsbtp resource present (pattern animation, bound after the delay) */
    s32 unk2B0;               /* @0x2B0 EffRipple3D / EffSplash3D: height above the water level */
    const BossEffRes *res;
    int used;
} BossEffect;
/* BossEffect_Create(name, animation mask, flags, x, y, z): name as in the ROM ("eff_hit") */
BossEffect *BossEffect_Create(const char *name, u32 anim_mask, u32 flags, s32 x, s32 y, s32 z);
#define BossEffect_State_Main boss_BossEffect_State_Main
void BossEffect_State_Main(void *self);
#define BOSSEFFECT_WRAPPERS(X) X(BaseDest) X(BeamFinB) X(Blast) X(Blast1) X(Blast3) X(Blast4) X(Blast5) X(BlastF) \
    X(BombBall) X(Boost) X(Catch) X(Clash) X(DmgHand) X(Hit) X(Homing) X(ImpactBig) X(ImpactPr) X(ImpactSl)          \
    X(ImpactSmall) X(ImpactSt) X(Jump) X(Land) X(LaserFinB) X(Launch) X(Parts) X(Pick) X(Pop) X(Produce)             \
    X(ShootBomb) X(StrikeBig) X(StrikeFin) X(StrikeHit) X(Suction) X(Suction2) X(Tackle)
#define BOSSEFFECT_DECL(N) BossEffect *BossEffect_Create##N(s32 a, s32 b, s32 c, s32 d);
BOSSEFFECT_WRAPPERS(BOSSEFFECT_DECL)
BossEffect *EffPebble_Create(s32 x, s32 y, s32 z, s16 a, s16 b, s16 c, s32 d);
void boss_effects_init(void);
int boss_effects_load(int stage);
void boss_effects_update(void);
void boss_effects_shutdown(void);
int boss_effects_draw(int list);
int boss_effects_live(void);

/* --------------------------------------------------------- hooks */
void boss_sfx(s32 id);
void boss_sfx_handle(u32 handle, s32 id);   /* Sound_PlaySeqArc(handle, 0, id) */
void boss_sfx_stop(u32 handle);             /* NNS_SndPlayerStopSeq(handle, 0) */
void boss_sfx_volume(s32 vol);              /* NNS_SndPlayerSetVolume(gSESoundHandle, vol) */
void boss_voice(s32 id);
void boss_log(const char *ev, s32 a, s32 b);
#define Sound_PlaySeqArc(a, b, id) boss_sfx(id)
#define Sound_PlayVoice(a, b, id) boss_voice(id)
void NaviMsg_SetFaceAnim(s32 face);
void NaviMsg_ShowMessage(s32 msg, s32 time);
void StageObj_SetPause(void);
void StageObj_ClearPause(void);
extern BossStats g_boss_stats;

/* ------------------------------------------------------- Boss1 */
int boss1_load(void);
void boss1_unload(void);
void *Boss1_Create(const ObjParam *a, s32 x, s32 y);
void boss1_update(void);
int boss1_draw(int list);
void boss1_destroy(void);
const char *boss1_state_name(int part);
void boss1_hammer_pos(s32 *x, s32 *y, s32 *z);
int boss1_hammer_box_armed(int box);

/* -------------------------------------------- Boss5 (Boss1 variant) */
/* Boss5PartD3 (missile, map object 0xd3): created by Boss1Neck_State_FireAt* in zone 5 */
void *Boss5PartD3_Create(const ObjParam *a, s32 x, s32 y);
void Boss5PartD3_Action_Launch(void *s, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s16 g);
int boss5_load(void);
void boss5_unload(void);
void boss5_update(void);
int boss5_draw(int list);
void boss5_destroy(void);
int boss5_missiles_alive(void);

/* ------------------------------------------------------- Boss2 */
/* timing-only animation table of bossdata.bin ("bs2.nsbta" -> frame counts) */
int boss_anim_lengths(const char *name, const u16 **frames, u32 *count);
/* converted material animation of a timing-table entry (see boss_core.c), NULL if none */
const dca_set_t *boss_matanim_for_frames(const u16 *frames);
/* converted .dcc / .dcp / .dct of a stage effect (files[0..2], "" if absent); -1 if none */
int boss_effect_matanims(int stage, const char *name, const char *files[3]);
/* the material states of a model's slot 1..3 animation objects at their integer frames (static buffer,
 * valid until the next call), NULL when none is bound: pass to dcm_draw_ex */
const dcm_matstate_t *boss_model_matstates(const BossModel *m);
extern u32 g_boss_matanim_draws;   /* draws that had material animations bound (tests) */
int boss2_load(void);
void boss2_unload(void);
void *Boss2_Create(const ObjParam *a, s32 x, s32 y);
void boss2_update(void);
int boss2_draw(int list);
void boss2_destroy(void);
const char *boss2_state_name(int part);
void boss2_body_pos(s32 *x, s32 *y, s32 *z);
int boss2_box_armed(int box);
void boss2_build_lights(void);   /* g_boss_lights from this frame's Boss2 lights + camera */

/* ------------------------------------------------------- Boss3 */
int boss3_load(void);
void boss3_unload(void);
void *Boss3_Create(const ObjParam *a, s32 x, s32 y);
void boss3_update(void);
int boss3_draw(int list);
void boss3_zone_script(void);    /* StageZoneScript_Zone3Boss (0x3eff) */
void boss3_destroy(void);
const char *boss3_state_name(int part);
void boss3_body_pos(s32 *x, s32 *y, s32 *z);
int boss3_box_armed(int box);

/* ------------------------------------------------------- Boss4 */
int boss4_load(void);
void boss4_unload(void);
void *Boss4_Create(const ObjParam *a, s32 x, s32 y);
void boss4_update(void);
int boss4_draw(int list);
void boss4_destroy(void);
const char *boss4_state_name(int part);
void boss4_body_pos(s32 *x, s32 *y, s32 *z);
int boss4_box_armed(int box);

/* ------------------------------------------------------- Boss6 */
#define Boss6_Create boss_Boss6_Create
#define BossHelp_Create boss_BossHelp_Create
#define BossHelp_Destroy boss_BossHelp_Destroy
void BossHelp_Create(u32 idx);   /* -> g_boss_hooks.help(BOSS_HELP_CREATE, idx, 0) */
void BossHelp_Destroy(void);     /* -> g_boss_hooks.help(BOSS_HELP_DESTROY, idx, 0) if one is shown */
int boss6_load(void);
void boss6_unload(void);
void *Boss6_Create(const ObjParam *a, s32 x, s32 y);
void boss6_update(void);
int boss6_draw(int list);
void boss6_destroy(void);
const char *boss6_state_name(int part);
void boss6_body_pos(s32 *x, s32 *y, s32 *z);
int boss6_box_armed(int box);

/* ------------------------------------------------------- Boss7 (boss7.c, public part in boss7.h) */
#define Boss7_Create boss_Boss7_Create
const char *boss_data_dir(void); /* boss_set_paths data_dir (boss7bac.bin lives there too) */
int boss7_load(void);
void boss7_unload(void);
void *Boss7_Create(const ObjParam *a, s32 x, s32 y);
void boss7_update(void);
int boss7_draw(int list);
void boss7_destroy(void);
int boss7_defeated(void);
const char *boss7_state_name(int part);
void boss7_body_pos(s32 *x, s32 *y, s32 *z);
int boss7_box_armed(int box);

/* ------------------------------------------------------- BossF (boss_final.c, public part in boss_final.h) */
#define BossF_Create boss_BossF_Create
int bossf_load(void);
void bossf_unload(void);
void bossf_init(void);
void *BossF_Create(const ObjParam *a, s32 x, s32 y);
void bossf_update_pre(void);     /* BossFPartD8 (task 0xf0) */
void bossf_update(void);         /* BossF 0x500, parts 0x501, ZoneFx_ZoneFBoss */
void bossf_zone_script(void);    /* StageZoneScript_ZoneFBoss (0x3eff) */
int bossf_draw(int list);
void bossf_destroy(void);
const char *bossf_state_name(int part);
void bossf_body_pos(s32 *x, s32 *y, s32 *z);
int bossf_box_armed(int box);
void bossf_set_map_width(s32 blocks);

#endif

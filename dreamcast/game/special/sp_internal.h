/*
 * sp_internal.h - shared definitions of the native special-stage module.
 *
 * The decompilation uses one anonymous UnkStruct per file; they are merged here
 * into named structs.  Field names keep the original offsets (unkNN / comments
 * "@0xNN") so every statement of the C in src/game/SpecialStage/ maps 1:1.
 * Pointers are native (the host build is 64-bit), so the structs are not
 * byte-compatible with the DS layout; the offsets are documentation only.
 */
#ifndef SP_INTERNAL_H
#define SP_INTERNAL_H

/* Names the menus / objects modules also define: the special-stage copies are
 * renamed so every module links into one binary. */
#define AnimatorSprite_Init sp_AnimatorSprite_Init
#define AnimatorSprite_ProcessAnimation sp_AnimatorSprite_ProcessAnimation
#define AnimatorSprite_SetAnimation sp_AnimatorSprite_SetAnimation
#define FX_Sqrt sp_FX_Sqrt
#define MTX_Concat33 sp_MTX_Concat33
#define MTX_Identity33_ sp_MTX_Identity33_
#define MTX_RotX33_ sp_MTX_RotX33_
#define MTX_RotY33_ sp_MTX_RotY33_
#define MTX_RotZ33_ sp_MTX_RotZ33_
#define gPad sp_gPad
#define gRandSeed sp_gRandSeed

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "special.h"
#include "../model/model.h"

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;
typedef s32 fx32;
typedef int BOOL;
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

typedef struct { fx32 x, y, z; } VecFx32;
typedef struct { fx32 m[9]; } MtxFx33;   /* _00 _01 _02 _10 .. _22 */
typedef struct { fx32 m[12]; } MtxFx43;  /* + _30 _31 _32 */

/* ------------------------------------------------------------------ sp_math.c */
static inline fx32 FX_Mul(fx32 a, fx32 b) { return (fx32)(((s64)a * b + 0x800) >> 12); }
extern const s16 *g_sp_sincos;   /* FX_SinCosTable_ (data_0206e470): {sin, cos} x 4096 */
extern const s16 *g_sp_atan;     /* data_02072470 */
#define SIN(a) g_sp_sincos[(((a) & 0xffff) >> 4) * 2]
#define COS(a) g_sp_sincos[(((a) & 0xffff) >> 4) * 2 + 1]
#define SIN_IDX(i) g_sp_sincos[(u32)((i) << 1)]
#define COS_IDX(i) g_sp_sincos[(u32)(((i) << 1) + 1)]
extern u32 gRandSeed;
static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}
fx32 FX_Div(fx32 n, fx32 d);
u32 FX_Sqrt(s32 v);
u16 FX_Atan2(fx32 y, fx32 x);
fx32 VEC_Mag(const VecFx32 *v);
fx32 VEC_Distance(const VecFx32 *a, const VecFx32 *b);
void VEC_Normalize(const VecFx32 *s, VecFx32 *d);
void VEC_Add(const VecFx32 *a, const VecFx32 *b, VecFx32 *ab);
void VEC_Subtract(const VecFx32 *a, const VecFx32 *b, VecFx32 *ab);
void VEC_MultAdd(fx32 a, const VecFx32 *v1, const VecFx32 *v2, VecFx32 *d);
void MTX_Identity33_(MtxFx33 *m);
void MTX_Identity43_(MtxFx43 *m);
void MTX_RotX33_(MtxFx33 *m, fx32 s, fx32 c);
void MTX_RotY33_(MtxFx33 *m, fx32 s, fx32 c);
void MTX_RotZ33_(MtxFx33 *m, fx32 s, fx32 c);
void MTX_RotX43_(MtxFx43 *m, fx32 s, fx32 c);
void MTX_RotY43_(MtxFx43 *m, fx32 s, fx32 c);
void MTX_RotZ43_(MtxFx43 *m, fx32 s, fx32 c);
void MTX_Concat33(const MtxFx33 *a, const MtxFx33 *b, MtxFx33 *ab);
void MTX_Concat43(const MtxFx43 *a, const MtxFx43 *b, MtxFx43 *ab);
void MTX_Copy33To43_(const MtxFx33 *a, MtxFx43 *b);
void MTX_TransApply43(const MtxFx43 *s, MtxFx43 *d, fx32 x, fx32 y, fx32 z);
void MTX_MultVec43(const VecFx32 *v, const MtxFx43 *m, VecFx32 *d);
void Model_TransposeRot(const MtxFx43 *src, MtxFx33 *dst);
/* G3i_LookAt_ (NitroSDK G3i_LookAt_: camPos, camUp, target) */
void G3i_LookAt_(const VecFx32 *pos, const VecFx32 *up, const VecFx32 *target, MtxFx43 *out);
/* _ffix: float -> s32 truncation (ARM soft-float) */
static inline s32 _ffix(float f) { return (s32)f; }

/* ------------------------------------------------------------------ sp_data.c */
/* ROM table access: the converted table bytes keyed by DS address (spdata.bin) */
const void *sp_rom(u32 addr, u32 size);
#define SP_ROM(addr, type) ((const type *)sp_rom((addr), sizeof(type)))
#define SP_ROMN(addr, type, n) ((const type *)sp_rom((addr), sizeof(type) * (n)))
static inline s32 sp_rom_s32(u32 a) { return *SP_ROM(a, s32); }
static inline u32 sp_rom_u32(u32 a) { return *SP_ROM(a, u32); }
static inline s16 sp_rom_s16(u32 a) { return *SP_ROM(a, s16); }
static inline u16 sp_rom_u16(u32 a) { return *SP_ROM(a, u16); }
static inline u8 sp_rom_u8(u32 a) { return *SP_ROM(a, u8); }
const void *sp_blob(const char *name, u32 *size);
/* native function of a ROM function address (tables hold DS code pointers) */
typedef void (*SpFn)(void *);
SpFn sp_func(u32 ds_addr);
const char *sp_func_name(u32 ds_addr);
void *sp_file_load(const char *rel, u32 *size);       /* <path>/<rel>, malloc'ed, NULL if missing */
extern char g_sp_path[128];
extern int g_sp_language;
int sp_data_load(void);
void sp_data_free(void);

/* ------------------------------------------------------------------ sprites (sp_sprite.c) */
typedef struct {
    u16 first_frame, frame_count;
    s16 loop_frame, next_anim;
    u8 format, pad;
    u16 pad2;
} SpDsprAnim;
typedef struct {
    u8 page, pad;
    u16 x, y, w, h;
    s16 ox, oy;
    u16 pad2;
} SpDsprImage;
typedef struct SpSprRes {
    u8 *frames;               /* frames.bin bytes */
    const SpDsprImage *images;
    const SpDsprAnim *anims;
    const u16 *frametab;      /* {image, duration} pairs */
    u16 n_images, n_anims, n_frames;
    u16 page_w, page_h;
    u16 *pixels;              /* atlas0 ARGB1555 (kept in RAM for the host renderer) */
    void *tex;                /* pvr_ptr_t (Dreamcast, without palettes.bin) */
    void *atlas;              /* Dreamcast SprAtlas: PAL4 page + palette rows (engine/render.h) */
    int loaded;
} SpSprRes;
/* AnimatorSprite subset (0x48 bytes on the DS) */
typedef struct SpSprite {
    const SpSprRes *res;      /* @0x0 resource (header + anim table) */
    s16 x, y;                 /* @0x4 / @0x6 */
    u16 anim;                 /* @0x8 animation index */
    u16 frameNo;              /* @0xA 1-based frame number (unkA) */
    u16 frameCount;           /* @0xC (unkC) */
    s16 speed;                /* @0x2A 0x10 per game frame */
    s32 wait;                 /* duration << 4 */
    s32 frame;                /* index into anim frames */
    u32 flags;                /* @0x2C 1 stopped, 2 ended, 4 loop, 0x80 hflip, 0x100 vflip, 0x80000 hidden */
    u16 unk46;                /* @0x46 OAM priority */
    s16 cur_image;            /* image of the frame shown (FrameAssembly) */
    u8 engine;                /* 0 main (3D/touch screen), 1 sub (top screen) */
} SpSprite;
int sp_sprres_load(SpSprRes *r, const char *id);
void sp_sprres_free(SpSprRes *r);
void AnimatorSprite_Init(SpSprite *s, const SpSprRes *res, u16 anim, u32 flags, int engine);
void AnimatorSprite_SetAnimation(SpSprite *s, u16 anim);
void AnimatorSprite_ProcessAnimation(SpSprite *s, s32 a, s32 b);
int sp_sprite_image(const SpSprite *s);       /* current image index, -1 = blank */

/* ------------------------------------------------------------------ models (sp_model.c) */
typedef struct SpMdlRes {          /* one converted .dcm (NNSG3dResMdl) */
    u8 *bytes;
    dcm_model_t mdl;
    int loaded;
} SpMdlRes;
typedef struct SpAnmRes {          /* converted animation set, or a timing-only resource */
    u8 *bytes;
    dca_set_t set;
    int kind;                      /* 0 joint .dca, 1 visibility .dcv, 2 material colour .dcc, 3 pattern .dcp,
                                    * 4 texture SRT .dct (MODELS.md 4.1) */
    int loaded;
    u16 nframes_fixed;             /* frame count for timing-only resources */
} SpAnmRes;
typedef struct SpAnmObj {
    fx32 frame;
    fx32 ratio;
    const SpAnmRes *res;
    int idx;
    u16 numFrames;
} SpAnmObj;
/* Model (DS 0x138 bytes, Model_InitSprite3D 0xF4) */
typedef struct SpModel {
    u32 type;               /* @0x00 0 model, 2 sprite 3D */
    u32 flag;               /* @0x04 bit0 hidden */
    u16 unk8;               /* @0x08 matrix stack slot for RestoreMtx */
    u8 cb[8];               /* @0x0A matrix op list (gModelMatrixOpTable) */
    fx32 scale[3];          /* @0x18 */
    MtxFx33 rot;            /* @0x24 */
    fx32 trans[3];          /* @0x48 */
    fx32 trans2[3];         /* @0x54 Model_MatrixOp_Translate2 */
    MtxFx43 unk60;          /* @0x60 Model_MatrixOp_MultMtx43 */
    const SpMdlRes *res;    /* NNS render object resource */
    SpAnmObj anm_obj[5][2];
    SpAnmObj *anm[5];       /* @0xE4 */
    SpAnmObj *anm2[5];      /* @0xF8 */
    u8 flags[5];            /* @0x10C 1 paused, 2 ended, 4 loop, 0x40 cross-fade, 0x80 fade done */
    fx32 speed;             /* @0x114 */
    fx32 speeds[5];         /* @0x118 */
    s16 blendSpeed;         /* @0x12C */
    s16 blendSpeeds[5];     /* @0x12E */
    /* sprite 3D (Model_InitSprite3D): AnimatorSprite @0x90, flags @0xBC, poly attr @0xDC */
    SpSprite spr;
    u32 unkDC;
    u32 unkEC, unkF0;
} SpModel;
#define SP_MDL_FLAGS(m) ((m)->flags[0])     /* @0x10C byte */
void Model_Init(SpModel *s, u32 a);
void Model_InitSprite3D(SpModel *o, const SpSprRes *res, u16 anim);
void Model_SetResource(SpModel *s, const SpMdlRes *res);
void Model_SetAnimation(SpModel *o, u32 slot, const SpAnmRes *res, u32 idx);
void Model_Animate(SpModel *s);
/* matrix stack slot 0x1e (MTX_STORE in SpecialStageMap_Draw) */
extern MtxFx43 g_sp_mtx_slot[32];

/* ------------------------------------------------------------------ objects */
typedef struct { fx32 x, y, z, r; } SpCircle;          /* collision circle (data_ov001_0231ea..) */
struct SpObjList;
/* SpecialStage object: the player (0x230 bytes), gimmicks and act objects (0xE4 bytes)
 * share the header up to 0xE4. */
typedef struct SpObj {
    s32 kind;               /* @0x00 1 act object, 2 gimmick */
    VecFx32 scale;          /* @0x04 */
    u16 unk10;              /* @0x10 rot x */
    u16 unk12;              /* @0x12 rot y */
    u16 unk14;              /* @0x14 rot z */
    u16 unk16;              /* @0x16 pipe angle */
    s32 unk18;              /* @0x18 height above the pipe */
    s32 unk1C;              /* @0x1C distance along the segment */
    VecFx32 pos;            /* @0x20 */
    VecFx32 mvel;           /* @0x2C own movement (gimmick states) */
    VecFx32 macc;           /* @0x38 */
    VecFx32 vel;            /* @0x44 (angle, height, distance) speed */
    VecFx32 acc;            /* @0x50 */
    s16 unk5C;              /* @0x5C hit points */
    u16 unk5E;              /* @0x5E ring reward */
    u16 unk60;              /* @0x60 1 destroyed, 2 frozen, 4 hidden, 8 no touch, 0x10 no anim, 0x20 free, 0x40 keep matrix, 0x100 invulnerable */
    u16 unk62;              /* @0x62 */
    u16 unk64;              /* @0x64 type flags (gimmick table +0x10) */
    u16 unk66;
    s16 unk68;              /* @0x68 type */
    s16 unk6A;              /* @0x6A argument */
    s32 unk6C;              /* @0x6C frame counter */
    s32 unk70;              /* @0x70 life (99999) */
    s32 unk74;              /* @0x74 invulnerability / hit timer */
    void *unk78;            /* @0x78 map segment (SpSeg*) */
    MtxFx43 unk7C;          /* @0x7C pipe matrix */
    SpModel *unkAC;         /* @0xAC 3D model */
    SpModel *unkB0;         /* @0xB0 3D sprite */
    const SpCircle *unkB4;  /* @0xB4 hurts the player / player attack */
    s32 unkB8;
    const SpCircle *unkBC;  /* @0xBC touch (player body vs gimmick) */
    s32 unkC0;
    const SpCircle *unkC4;  /* @0xC4 tap target circle (screen space) */
    s32 unkC8;
    void (*unkCC)(struct SpObj *);  /* @0xCC update */
    s32 unkD0;              /* @0xD0 state timer */
    struct SpObj *unkD4;    /* @0xD4 link (parent) */
    struct SpObj *unkD8;    /* @0xD8 root */
    struct SpObj *next;     /* @0xDC */
    struct SpObj *prev;     /* @0xE0 */
    /* ---- player only (SpecialStagePlayer, 0x230) */
    VecFx32 unkE4;          /* @0xE4 camera focus point */
    MtxFx33 mtx;            /* @0xF0 */
    s32 unk114;             /* @0x114 steering enabled */
    struct SpObj *unk118;   /* @0x118 attack target */
    struct SpObj *unk11C;   /* @0x11C emerald to grab */
    s32 unk120;             /* @0x120 rings dropped */
    s32 unk124;             /* @0x124 attacking */
    s32 unk128;             /* @0x128 trick input active */
    const SpCircle *unk12C; /* @0x12C */
    s32 unk130;
    s32 unk134;             /* @0x134 target pipe angle */
    s32 unk138;             /* @0x138 turn speed */
    s32 unk13C;
    s32 unk140;             /* @0x140 target at a wall */
    s32 unk144;             /* @0x144 dash timer */
    s32 unk148;             /* @0x148 rings */
    s16 unk14C;             /* @0x14C trick: next point */
    s16 unk14E;             /* @0x14E trick: frames */
    s16 unk150;             /* @0x150 trick sequence */
    s16 unk152;             /* @0x152 chained sequence */
    s32 unk154;             /* @0x154 reward rings to spawn */
    s32 unk158;             /* @0x158 rating */
    s32 unk15C;             /* @0x15C spin counter */
    VecFx32 unk160, unk16C, unk178;   /* @0x160 position history */
    MtxFx33 unk184, unk1A8, unk1CC;   /* @0x184 rotation history */
    s32 unk1F0;             /* @0x1F0 after-images */
    s32 unk1F4;             /* @0x1F4 after-image timer */
    MtxFx43 unk1F8;         /* @0x1F8 right hand matrix (RHandMtxCallback) */
    s32 unk228;             /* @0x228 state */
    void (*unk22C)(struct SpObj *);   /* @0x22C state function */
    u16 unk68id;            /* @0x68 for the player: id (F16(0x68)) */
} SpObj;

typedef struct SpObjList {   /* SpecialStageObjList (0x1E4) */
    u16 count;               /* @0x0 capacity */
    u16 unk2;                /* @0x2 used */
    u16 unk4;                /* @0x4 read */
    u16 unk6;                /* @0x6 write */
    SpObj *ents;             /* @0x8 */
    SpObj **ptrs;            /* @0xC */
    SpObj head;              /* @0x10 head sentinel (next @0xEC) */
    SpObj tail;              /* @0xF4 tail sentinel */
    void (*unk1D8)(SpObj *); /* @0x1D8 pre-update */
    void (*unk1DC)(SpObj *); /* @0x1DC post-update */
    void (*unk1E0)(SpObj *); /* @0x1E0 destructor */
} SpObjList;

/* map segment (0x40) */
typedef struct SpSeg {
    s32 type;                /* @0x00 */
    MtxFx43 mtx;             /* @0x04 */
    u16 flags;               /* @0x34 bit0 passed */
    u16 pad;
    struct SpSeg *next;      /* @0x38 */
    struct SpSeg *prev;      /* @0x3C */
} SpSeg;

/* gSpecialStageMap (0xAC8) */
typedef struct SpMap {
    s32 unk0;                /* @0x000 scroll distance in the current segment */
    s32 unk4;                /* @0x004 scroll speed */
    MtxFx43 mtx;             /* @0x008 */
    s32 unk38;               /* @0x038 curve length 0x12c05e */
    SpModel models[7];       /* @0x03C segment models (0x138 each) */
    SpModel sky;             /* @0x8C4 */
    SpMdlRes res[4];         /* @0x9FC map_all_00 / st_mid / st_gl / sky */
    SpMdlRes res_all[5];     /* the five models of ss_map_all_00 (one .dcm each) */
    SpAnmRes nsbta;          /* @0xA0C ss.nsbta */
    s32 unkA10;
    MtxFx33 unkA14;
    u16 cap, count, rd, wr;  /* @0xA38 */
    SpSeg *pool;             /* @0xA40 */
    SpSeg **slots;           /* @0xA44 */
    SpSeg head;              /* @0xA48 head sentinel; type @0xA48 = last passed type, mtx @0xA4C */
    SpSeg tail;              /* @0xA88 */
} SpMap;

/* gSpecialStageData (0x40) */
typedef struct SpSst {       /* SST file header */
    char magic[4];
    u32 pad;
    u32 offset;              /* relocated */
    s32 count;
    u16 quota1;              /* +0x10 SpecialStage_GetRingQuota */
    u16 quota2;              /* +0x12 SpecialStageHud_Unk_ov001_02314368 */
    s32 unk14;               /* +0x14 first-half segment count (02314214 start) */
    s32 unk18;               /* +0x18 end */
} SpSst;
typedef struct SpSstCmd { s32 type; u8 a, b; u16 c; s32 d, e; } SpSstCmd;
typedef struct SpData {
    s32 idx;                 /* @0x0 */
    s32 unk4;                /* @0x4 frame */
    s32 unk8;                /* @0x8 steps */
    s32 unkC;                /* @0xC segments passed */
    SpSeg *unk10;            /* @0x10 last added segment */
    u8 *unk14;               /* @0x14 SST file */
    s32 unk18;               /* @0x18 command cursor */
} SpData;

/* SSD trick sequence (0x28) */
typedef struct SpTrickSeq {
    struct { u16 x, y; } pts[8];
    u16 unk20;               /* time */
    u16 count;
    s32 unk24;               /* chained sequence (-1 none) */
} SpTrickSeq;
typedef struct SpTrick {     /* gSpecialStageTrick */
    u8 unk0[0x100];
    u16 unk100[3];
    SpTrickSeq *unk10C;
    s32 unk110;
} SpTrick;

/* camera list entry (0x58) */
typedef struct SpCamEnt {
    char name[0x20];
    VecFx32 pos;             /* @0x20 */
    VecFx32 target;          /* @0x2C */
    VecFx32 up;              /* @0x38 */
    u16 lat, lon;            /* @0x44 */
    u16 rot;                 /* @0x48 perspective half angle */
    s32 x, y, z;             /* @0x4C aspect, near, far */
} SpCamEnt;
typedef struct SpCamera {    /* gSpecialStageCamera (0x3C) */
    s32 state;               /* @0x00 mode */
    s32 unk4;                /* @0x04 setup request */
    s32 unk8;                /* @0x08 0 speed-limited, 1 lerp */
    float unkC;              /* @0x0C lerp factor */
    VecFx32 pos;             /* @0x10 */
    VecFx32 tgt;             /* @0x1C */
    s32 unk28, unk2C, unk30; /* @0x28 speed, thresholds */
    s32 unk34;               /* @0x34 handle */
    s32 unk38;               /* @0x38 orbit counter */
} SpCamera;

/* gSpecialStagePlayerResources (0x148) */
typedef struct SpPlayerRes {
    SpModel model;           /* @0x004 */
    SpMdlRes mdl[2];         /* @0x13C son.nsbmd (son, son_ball) */
    SpAnmRes bca;            /* @0x140 son.nsbca */
    SpAnmRes bva;            /* @0x144 son.nsbva */
    int r_hand;              /* node index of "r_hand" */
    int rhand_cb;            /* SpecialStagePlayer_SetRHandCallback done */
} SpPlayerRes;

/* gSpecialStageGimmickModels (0xEC8) */
typedef struct SpGimmickModels {
    SpSprRes *unk0;          /* @0x000 ac_spst_gmk.bac */
    SpModel objs[9];         /* @0x004 3D sprites (0xF4 each) */
    SpMdlRes res[5];         /* @0x898 dash_p, switch01, switch02, eme_light, emeXX */
    SpModel anims[5];        /* @0x8AC models */
    SpAnmRes unkEC4;         /* @0xEC4 ss.nsbtp (eme_light pattern) */
} SpGimmickModels;

/* data_ov001_0231f5b8: act objects (0x46E0) */
typedef struct SpActWork {
    SpModel obj[32];         /* sprites */
    u32 used1[32];
    SpModel obj2[32];        /* models */
    u32 used2[32];
    SpMdlRes res0, res3;     /* eff_clash, eff_shoot_bomb (its pattern textures are in the model) */
    SpAnmRes res1, res2, res4, res5, res6;
    SpObjList *task;         /* @0x46A0 */
} SpActWork;

/* gSpecialStageHud (0xCEC) */
typedef struct SpBg {        /* Background (BBG) */
    int res;                 /* bg index 0..6 into the resource table */
    int engine;              /* 0 main, 1 sub */
    int layer;
    int drawn;               /* Background_Draw done */
} SpBg;
typedef struct SpHud {
    void *unk0, *unk4;       /* @0x000 tasks (FlushPaletteNow, 023095e0) */
    SpBg bg[4];              /* @0x008 fix_base, fix_touch, cldm_base, cldm_caption */
    SpSprite spr[23];        /* @0x108 */
    SpSprite spr0;           /* @0x780 language captions */
    SpSprite spr1;           /* @0x7C8 ac_spst_ann */
    SpSprite spr2[10];       /* @0x810 trick circles, pen */
    s32 unkAE0;              /* @0xAE0 frames */
    s32 unkAE4;              /* @0xAE4 shown quota */
    s32 unkAE8;              /* @0xAE8 play sprites created */
    s32 unkAEC;              /* @0xAEC last trick point */
    s32 unkAF0;              /* @0xAF0 rating text timer */
    float a[7], b[7], c[7];  /* @0xAF4 emerald slide */
    u16 pal0[16], pal1[16], pal2[16]; /* @0xB48 */
    s32 unkBA8;              /* @0xBA8 show big emerald */
    s32 unkBAC;              /* @0xBAC palette lerp timer */
    s32 unkBB0;              /* @0xBB0 show marker */
    s32 unkBB4;              /* @0xBB4 marker at the end */
    s32 unkBB8;              /* @0xBB8 head blink */
    s32 unkBBC;              /* @0xBBC */
    s32 unkBC0;              /* @0xBC0 START! scale */
    s32 unkBC4;              /* @0xBC4 start sequence timer */
    s32 unkBC8;              /* @0xBC8 checkpoint timer */
    s32 unkBCC;              /* @0xBCC goal timer */
    s32 unkBD0;              /* @0xBD0 fail timer */
    s32 unkBD4;              /* @0xBD4 clear timer */
    s32 unkBD8[7];           /* @0xBD8 emerald circle timers */
    void *unkBF4;            /* @0xBF4 task */
    s16 unkBF8, unkBFA;      /* @0xBF8 */
    SpSprite spr3[3];        /* @0xBFC sparkles */
    s32 timers[3];           /* @0xCD4 */
    struct { s16 x, y; } pos[3];  /* @0xCE0 */
} SpHud;

/* gSpecialStageWork (0x1D8) */
typedef struct SpWork {
    u8 *unk0;                /* sound heap (unused) */
    s32 unk4;                /* @0x04 stage */
    u32 unk8;                /* @0x08 emerald flags (gGameState.unk28) */
    int taskC;               /* @0x0C update task: 0 intro, 1 play, 2 exit */
    int task10;              /* @0x10 draw task */
    int task14;              /* @0x14 intro BG task alive */
    SpObj *unk18;            /* @0x18 player */
    SpObjList **unk1C;       /* @0x1C gimmick manager */
    s32 unk20;               /* @0x20 frozen */
    s32 unk24;               /* @0x24 cleared */
    s32 unk28;               /* @0x28 play frames */
    s32 timer;               /* @0x2C intro timer */
    s32 unk30;               /* @0x30 */
    s32 unk34;               /* @0x34 */
    SpBg obj[3];             /* @0x38 intro BGs */
    SpSprite spr[3];         /* @0xF8 intro sprites */
} SpWork;

/* resources (data_ov001_0231f5c4) */
typedef struct SpRes {
    u8 *bg[7];               /* mnbg files (res[7]) */
    SpSprRes spr[5];         /* ac_spst, ac_spst_<lang>, ac_spst_ann, ac_spst_efct; [4] ac_spst_pad (DC
                              * controller art: button targets, PRESS START prompt, legend lines) */
    u8 *unk2C;               /* ss.ssd */
    u32 bgsize[7];
} SpRes;

/* display state the original keeps in registers / gDisplayMain / gDisplaySub */
typedef struct SpDisplay {
    s16 bright;              /* gDisplayX.unk48 (index 0x24): +16 white */
    struct { s16 x, y; } bg[4];
    u32 bg_enable;           /* DISPCNT bits 8..12 >> 8 */
    u8 blend_on;             /* BLDCNT alpha blending active */
    u8 eva, evb;             /* BLDALPHA */
} SpDisplay;
extern SpDisplay gSpDisplayMain, gSpDisplaySub;

/* globals (original names) */
extern SpWork *gSpecialStageWork;
extern SpRes *data_ov001_0231f5c4;
extern SpData *gSpecialStageData;
extern SpMap *gSpecialStageMap;
extern SpTrick *gSpecialStageTrick;
extern SpCamera gSpecialStageCamera;
extern SpCamEnt *gSpecialStageCameraList;
extern s32 data_ov001_0231f6a8;
extern SpPlayerRes *gSpecialStagePlayerResources;
extern SpGimmickModels *gSpecialStageGimmickModels;
extern SpActWork *data_ov001_0231f5b8;
extern SpHud *gSpecialStageHud;
extern s32 data_ov001_0231f698;
extern u32 gGameState_unk28;

/* input (sp_input.c): the Dreamcast controller scheme (SPECIAL.md section 4).  The original
 * reads the touch screen (gTouchState); here the mechanics read physical inputs directly:
 * steering target, homing-attack press, Trick Spring button presses, begin press. */
typedef struct SpPad {
    int steer;               /* a steering target this frame (stick out of its dead zone, D-pad, stick release) */
    s32 steer_angle;         /* its pipe angle, -0x4000 (left wall) .. 0x4000 (right wall) */
    int attack;              /* A pressed outside a Trick Spring sequence: homing attack */
    u32 qte;                 /* Trick Spring inputs pressed this frame: bit k = target k+1 (SP_QTE_*) */
    int begin;               /* intro "PRESS START TO BEGIN": START or A pressed */
} SpPad;
extern SpPad g_sp_pad;
extern u16 gPad[3];          /* gPad[0] held / gPad[2] pressed (DS bits: START pause, A / B pause menu) */
enum { SP_QTE_COUNT = 10 };  /* 1 A, 2 B, 3 X, 4 Y, 5 L, 6 R, 7 Up, 8 Right, 9 Down, 10 Left */
extern const char *const sp_qte_names[SP_QTE_COUNT];
/* analogue trigger: press at >= SP_TRIG_ON, released after 2 frames <= SP_TRIG_OFF */
enum { SP_TRIG_ON = 160, SP_TRIG_OFF = 64 };
typedef struct { u8 down, low; } SpTrig;
int sp_trigger_step(SpTrig *t, int value);           /* 1 = press event */
s32 sp_stick_to_angle(int ax);                       /* stick x (after the engine dead zone) -> pipe angle */
s32 sp_qte_step(s32 cur, s32 count, u32 events);      /* SpecialStageTrick_CheckTouch rule on button events */
void sp_input_update(const InputState *in);
void sp_input_reset(void);
int sp_legend_pending(void);                          /* the controller legend shows at the intro prompt */
/* screen position of a world point through the current camera (NNS_G3dWorldPosToScrPos) */
BOOL NNS_G3dWorldPosToScrPos(const VecFx32 *v, s32 *x, s32 *y);

/* audio */
void Sound_PlaySeqArc_SS(int arc, int id);
void Sound_PlaySeq_SS(int seq);
void Sound_StopBGM_SS(int fade);

/* ------------------------------------------------------------------ functions */
/* core */
void SpecialStage_Start(void);
void SpecialStage_Init(s32 arg);
void SpecialStage_State_IntroUpdate(void);
void SpecialStage_State_IntroDraw(void);
void SpecialStage_StartPlay(void);
void SpecialStage_State_PlayUpdate(void);
void SpecialStage_State_PlayDraw(void);
void SpecialStage_State_Exit(void);
void SpecialStage_RequestExit(s32 v);
void SpecialStage_Shutdown(void);
void SpecialStage_DrawFadeQuad(s32 n);
s32 SpecialStage_GetStageIndex(void);
u32 SpecialStage_GetEmeraldFlags(void);
SpObj *SpecialStage_GetPlayer(void);
SpObjList **SpecialStage_GetGimmickManager(void);
u16 SpecialStage_GetRingQuota(void);
s32 SpecialStage_PipeAngleToX(u16 ang);
u16 SpecialStage_PipeXToAngle(s32 x);
s32 SpecialStage_TouchToPipeAngle(s32 x, s32 y);
u16 SpecialStage_ArcLengthToAngle(s32 a, s32 b);
/* a = screen circle {x, y, r} (12 bytes: data_ov001_02317e80, the gimmick tap circles @0xC4) */
BOOL SpecialStage_IsPointInCircle(const s32 *a, const VecFx32 *b, s32 x, s32 y);
void SpecialStage_RotateMtxXAroundPivot(MtxFx43 *m, s32 d, u16 ang);
void SpecialStage_RotateMtxYAroundPivot(MtxFx43 *m, s32 d, u16 ang);
BOOL SpecialStage_TestCollision(const SpCircle *a, const VecFx32 *pa, u16 angA, const SpCircle *b, const VecFx32 *pb, u16 angB);
void SpecialStage_CheckGimmickCollisions(void);
void SpecialStage_UpdateObject(SpObjList *c, SpObj *w);
void SpecialStage_UpdateObjects(void);
void SpecialStage_Unk_ov001_02312dd0(SpObj *p, s32 v);
BOOL SpecialStage_Unk_ov001_02309894(void);
void SpecialStage_Unk_ov001_023099ec(void);
void SpecialStage_Unk_ov001_0230e3c8(SpObj *p, void *q, u16 ang);
void SpecialStage_Unk_ov001_0230ea6c(SpObj *s);
void SpecialStage_Unk_ov001_0230f284(SpObj *p, void *q);
void SpecialStage_ActObjBounceMove(SpObj *p);
void SpecialStage_ActObjDestroyWhenDone(SpObj *p);
void SpecialStage_ActObjFollowParent(SpObj *p);
void SpecialStage_ActObjReleaseSlots(SpObj *o);
void SpecialStage_ActObjSpawnGimmick(SpObj *p);
/* data */
void SpecialStage_LoadStageData(s32 idx);
void SpecialStage_ReleaseStageData(void);
void SpecialStageData_AddStep(void);
BOOL SpecialStageData_RunCommand(SpData *p);
void SpecialStageData_Update(void);
/* map */
void SpecialStage_LoadMapModels(void);
void SpecialStage_ReleaseMapModels(void);
SpSeg *SpecialStageMap_AllocSegment(void);
SpSeg *SpecialStageMap_AddSegment(u8 type);
void SpecialStageMap_FreeSegment(SpSeg *n);
SpSeg *SpecialStageMap_GetCurrentSegment(void);
s32 SpecialStageMap_GetScrollDistance(void);
s32 SpecialStageMap_GetScrollSpeed(void);
void SpecialStageMap_SetScrollSpeed(s32 v);
BOOL SpecialStageMap_IsSegmentListEmpty(void);
void SpecialStageMap_Update(void);
void SpecialStageMap_Draw(void);
s32 sp_seg_length(s32 type);
/* gimmicks / object lists / act objects */
void SpecialStage_LoadGimmickModels(void);
void SpecialStage_ReleaseGimmickModels(void);
void SpecialStage_LoadActArchive(void);
void SpecialStage_ReleaseActArchive(void);
SpObjList **SpecialStageGimmick_CreateManager(s32 a);
void SpecialStageGimmick_DestroyManager(SpObjList **w);
SpObj *SpecialStageGimmick_Spawn(SpObjList **g, void *parent, s32 type, s32 arg, SpObj *link, u16 ang, s32 y, s32 z);
void SpecialStageGimmick_Update(SpObjList **p);
void SpecialStageGimmick_Draw(SpObjList **p);
void SpecialStageGimmick_Destroy(SpObj *p);
void SpecialStageGimmick_OnAttacked(SpObj *p);
SpObj *SpecialStageGimmick_FindTouched(SpObj *a, SpObjList *list, s32 x, s32 y);
SpObj *SpecialStageGimmick_FindAttackTarget(SpObj *a, SpObjList *list);
void SpecialStageGimmick_SpawnTrickReward(SpObjList **a, s32 x, s32 y, s32 z);
SpObj *SpecialStageGimmick_SpawnActObjAt(void *parent, s32 type, u16 ang, s32 y, s32 z);
void SpecialStageGimmick_CalcPipeMatrix(MtxFx43 *m, s32 mode, s32 flag, s32 force, u32 ang, s32 y, s32 z);
BOOL SpecialStageGimmick_Unk_ov001_02310144(SpObj *p);
void SpecialStageGimmick_Unk_ov001_02312dc8(SpObj *p, SpObj *v);
SpObjList *SpecialStageObjList_Create(s32 n);
void SpecialStageObjList_Destroy(SpObjList *g);
SpObj *SpecialStageObjList_Alloc(SpObjList *g);
void SpecialStageObjList_Free(SpObjList *g, SpObj *n);
void SpecialStageObjList_Update(SpObjList *g);
void SpecialStageObjList_Draw(SpObjList *p);
BOOL SpecialStageObjList_IsCulled(SpObj *p);
BOOL SpecialStageObj_IsInReach(SpObj *unused, SpObj *p);
void SpecialStageObj_MarkDestroyed(SpObj *p);
SpObj *SpecialStageActObj_CreateByType(void *arg, u32 type);
SpObj *SpecialStageActObj_SpawnAt(s32 type, s32 x, s32 y, s32 z);
void SpecialStageActObj_Destroy(SpObj *p);
void SpecialStage_DrawObjects(void);
void SpecialStageTrickReward_Update(SpObj *p);
/* player */
void SpecialStage_LoadPlayerArchive(void);
void SpecialStage_ReleasePlayerArchive(void);
SpObj *SpecialStagePlayer_Create(u16 id);
void SpecialStagePlayer_Destroy(SpObj *p);
void SpecialStagePlayer_ChangeState(SpObj *w, s32 state);
void SpecialStagePlayer_Update(SpObj *w);
void SpecialStagePlayer_Draw(SpObj *p);
void SpecialStagePlayer_Action_Start(SpObj *p);
void SpecialStagePlayer_Action_Celebrate(SpObj *p);
void SpecialStagePlayer_Action_Fail(SpObj *p);
void SpecialStagePlayer_Action_StartTrick(SpObj *p, s32 seq);
void SpecialStagePlayer_OnTouchGimmick(SpObj *p, SpObj *e);
void SpecialStagePlayer_OnHurt(SpObj *p, SpObj *o);
void SpecialStagePlayer_OnAttackHit(SpObj *p);
void SpecialStagePlayer_SetRHandCallback(void);
SpFn sp_player_state_fn(const char *name);
/* camera */
void SpecialStageCamera_Init(void);
void SpecialStageCamera_Shutdown(void);
void SpecialStageCamera_SetMode(s32 v);
void SpecialStageCamera_Update(void);
void SpecialStageCamera_Apply(void);
void SpecialStageCamera_GetVectors(VecFx32 *pos, VecFx32 *target, VecFx32 *up);
/* the camera of the last SpecialStageCamera_Apply (fx32 world units) */
extern MtxFx43 g_sp_view;          /* data_02084d84 (G3i_LookAt_ output) */
extern fx32 g_sp_proj[5];          /* sin, cos, aspect, near, far */
void sp_camera_dcm(dcm_camera_t *c);
/* trick */
void SpecialStageTrick_Init(void);
void SpecialStageTrick_Shutdown(void);
void SpecialStageTrick_Reset(void);
void SpecialStageTrick_SetSequences(SpTrickSeq *a, s32 b);
SpTrickSeq *SpecialStageTrick_GetSequence(s32 idx);
s32 SpecialStageTrick_GetRating(s32 idx, s32 n);
s32 SpecialStageTrick_CheckTouch(s32 idx, s32 n, s32 cur);
s32 SpecialStagePlayer_Unk_ov001_02314694(s32 i);
/* HUD */
void SpecialStage_LoadHudAssets(SpRes *o);
void SpecialStage_ReleaseHudAssets(SpRes *o);
void SpecialStage_CreateIntroTasks(void);
void SpecialStageHud_Create(void);
void SpecialStageHud_Destroy(void);
void SpecialStageHud_Update(void);
void SpecialStageHud_Draw(void);
void SpecialStageHud_ShowTrickRating(s32 n);
void SpecialStageHud_EndTrickRating(void);
void SpecialStageHud_Unk_ov001_02312dd8(SpObj *p, s32 v);
void SpecialStageGimmick_Unk_ov001_02309994(void);
void SpecialStageGimmick_Unk_ov001_023099c4(void);
void sp_hud_tasks(void);           /* the HUD's display tasks (0xf200 .. 0xf400) */
u16 SpecialStageHud_Unk_ov001_02314368(void);

/* ------------------------------------------------------------------ drawing (sp_draw.c) */
/* draw-command capture: the game code "draws" by recording commands during the draw
 * task; special_draw() replays them on the PVR (or the host rasteriser in tests). */
enum { SPD_MODEL, SPD_SPRITE2D, SPD_FADEQUAD, SPD_RATINGBAR, SPD_BG };
void sp_draw_reset(void);
void sp_draw_model(const SpModel *m, int light_mode);     /* Model_Draw */
void sp_draw_sprite(const SpSprite *s, s32 scale_x, s32 scale_y, int rotscale); /* AnimatorSprite_Draw(RotScale) */
void sp_draw_fadequad(s32 alpha);                         /* SpecialStage_DrawFadeQuad */
void sp_draw_ratingbar(float f, int color_idx, int blink);
void sp_draw_bg(int res, int engine, int layer, s16 sx, s16 sy, int alpha16);
int sp_draw_count(void);
extern int g_sp_light_mode;        /* SpecialStagePlayer_DrawModels light colour 0x6182 */
void sp_render_frame(void);        /* KOS: emit lists; host: optional raster */
int sp_render_init(void);
void sp_render_shutdown(void);
/* host test raster (sp_draw.c, host only) */
extern u8 *g_sp_host_fb;           /* 640x480 RGB if set */

/* trace counters for tests */
typedef struct {
    int sfx[32][64];
    int music_last;
    int rings_collected;
    int hurt;
    int attacks;
    int tricks_ok, tricks_fail;
} SpTrace;
extern SpTrace g_sp_trace;

#endif

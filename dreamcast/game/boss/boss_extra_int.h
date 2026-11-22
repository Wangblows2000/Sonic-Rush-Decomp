/* Internal interfaces of the Extra Zone boss (boss_extra*.c).  Not for the integrator.
 *
 * Structures keep the original work-struct offsets in their field names (unk1C8 = @0x1C8), like
 * the other boss translations; where a DS structure is replaced by a native one the offset is in
 * the comment.  Original function names are kept (ExtraBoss*, ExtraBossObj*, ExtraBossPlayer*);
 * they are unique to this module.  The Model runtime (Model_Init / Model_SetAnimation / ...) and
 * the fx32 helpers come from boss_model.c through boss_internal.h. */
#ifndef BOSS_EXTRA_INT_H
#define BOSS_EXTRA_INT_H

#include "boss_internal.h"
#include "boss_extra.h"

/* original names kept in the sources; the linker sees module-private ones (the objects, special, menu
 * and other boss modules define their own FX_Sqrt / gTaskCurrent) */
#define FX_Sqrt ex_FX_Sqrt
#define gTaskCurrent ex_gTaskCurrent
#define BrightnessLerp_Create ex_BrightnessLerp_Create

#ifndef TRUE
typedef int BOOL;
#define TRUE 1
#define FALSE 0
#endif

/* ------------------------------------------------------------- ROM data */
/* bossex.bin (tools/bossexdata.py): ov001 / arm9 tables keyed by their DS address */
const void *ex_data(u32 ds_addr);
#define EX_DATA(addr, type) ((type)ex_data(addr))
#define EX_S32(addr, i) (EX_DATA(addr, const s32 *)[i])
#define EX_U16(addr, i) (EX_DATA(addr, const u16 *)[i])
/* ov001 address of a C string (pointer tables keep DS values) */
const char *ex_rom_str(u32 ptr);

/* --------------------------------------------------------- math helpers */
static inline s32 FX_DivS32(s32 a, s32 b) { return b ? a / b : 0; }
static inline s32 FX_ModS32(s32 a, s32 b) { return b ? a % b : 0; }
s32 FX_Sqrt(s32 v);                        /* NitroSDK FX_Sqrt (hardware 64-bit sqrt, rounded) */
#define FX_Atan2(y, x) dcp_atan2((y), (x))
#define SIN_IDX(i) dcp_sin((u16)((i) << 4))
#define COS_IDX(i) dcp_cos((u16)((i) << 4))
void ex_mtx_mult_vec33(const fx32 *v, const MtxFx33 *m, fx32 *out); /* MTX_MultVec33 */

/* --------------------------------------------------------------- tasks */
/* Task_Create / Task_Destroy subset: tasks run once per frame in (id, creation) order, as the DS
 * task list does.  gTaskCurrent is the task being run. */
typedef struct ExTask ExTask;
typedef void (*ExTaskFn)(void);
struct ExTask {
    ExTaskFn fn;             /* @0x08 main function (changed by the tasks themselves) */
    void *work;              /* @0x10 */
    u16 id;
    u32 seq;
    int used, dead;
};
extern ExTask *gTaskCurrent;
ExTask *ex_task_create(ExTaskFn fn, u16 id, void *work);
void ex_task_destroy(ExTask *t);
void ex_tasks_run(void);
void ex_tasks_clear(void);

/* ------------------------------------------------------------- objects */
typedef struct ExObj ExObj;
typedef struct ExCollider ExCollider;
typedef void (*ExFn)(ExObj *self);
typedef void (*ExHitFn)(ExCollider *self, ExCollider *other);

/* hit box (0x38 bytes, ExtraBoss_InitCollider) */
struct ExCollider {
    ExObj *owner;            /* @0x00 */
    ExHitFn unk4;            /* @0x04 on-hit callback (fn(attacker, defender)) */
    u32 unk8;                /* @0x08 bits 0-2 groups; 0x100 attacker list; 0x200 off; 0x1000 own position;
                                0x2000 own rotation; 0x10 / 0x20 / 0x40: player hit kinds */
    fx32 unkC[3];            /* @0x0C centre offset */
    fx32 unk18[3];           /* @0x18 half size */
    u16 unk24;               /* @0x24 rotation */
    fx32 unk28[3];           /* @0x28 position (owner position, copied every frame) */
    u16 unk34;               /* @0x34 owner rotation (0x10000 - rotZ) */
};

/* ExtraBossObj (0x1E0 bytes, ExtraBossObj_Create) */
struct ExObj {
    ExTask *unk0;            /* @0x00 task */
    s32 unk4;                /* @0x04 kind: 0 player, 1 effect / shot, 2 boss part, 3 (unused) */
    u16 unk8;                /* @0x08 index in gExtraBossPlayerManager->all[] */
    u16 unkA;                /* @0x0A index in the kind's slot array */
    u32 unk14;               /* @0x14 flags: 1 off-axis camera, 8 destroy, 0x10 follow parent destroy,
                                0x20 facing (0x40 left), 0x100/0x200/0x400 stop at zero velocity,
                                0x1000 no hit boxes, 0x10000 no animation, 0x20000 hidden,
                                0x40000 draws in its own task, 0x80000 no motion, 0x100000 no parent
                                check, 0x200000 no position motion, 0x400000 no rotation motion */
    fx32 unk18, unk1C, unk20; /* @0x18 position (fx32 px, y down) */
    fx32 unk24, unk28, unk2C; /* @0x24 previous position */
    fx32 unk30, unk34, unk38; /* @0x30 draw offset */
    u16 unk3C, unk3E, unk40;  /* @0x3C rotation X / Y / Z */
    fx32 unk44, unk48, unk4C; /* @0x44 sprite scale */
    fx32 unk50, unk54, unk58; /* @0x50 velocity */
    fx32 unk5C, unk60, unk64; /* @0x5C acceleration */
    u16 unk68, unk6A, unk6C;  /* @0x68 rotation velocity */
    u16 unk6E, unk70, unk72;  /* @0x6E rotation acceleration */
    ExFn unk74;              /* @0x74 state */
    ExFn unk78;              /* @0x78 draw */
    ExObj *unk7C;            /* @0x7C parent */
    ExObj *unk80;            /* @0x80 target (the player) */
    BossModel *unk84;        /* @0x84 model drawn */
    BossModel *unk88;        /* @0x88 player model 0 (e_son / e_blz) */
    BossModel *unk8C;        /* @0x8C player model 1 (son_easy / blz_easy) */
    void *unk90;             /* @0x90 sprite (unused by the fight) */
    BossModel *unk94;        /* @0x94 shape model (unused by the fight) */
    u16 unk98;               /* @0x98 hit points (0xffff = indestructible) */
    ExCollider *unk9C[5];    /* @0x9C hit boxes registered every frame */
    ExCollider unkB0[5];     /* @0xB0 / 0xE8 / 0x120 / 0x158 / 0x190 hit box storage */
    s32 unk1C8;              /* @0x1C8 fx kind / timer (some states use the low 16 bits) */
    s32 unk1CC;              /* @0x1CC */
    s32 unk1D0;              /* @0x1D0 */
    s32 unk1D4;              /* @0x1D4 */
    s32 unk1D8;              /* @0x1D8 */
    void *unk1DC;            /* @0x1DC extra work (player / boss / trail) */
    int used;
};
#define EX_COLL_B0 (0)
#define EX_COLL_E8 (1)
#define EX_COLL_120 (2)
#define EX_COLL_158 (3)

/* gExtraBossPlayerManager (0x174 bytes) */
typedef struct ExObjManager {
    ExObj *unk0;
    ExObj *unk4[1];          /* @0x04 kind 0: the player */
    ExObj *unk8[5];          /* @0x08 kind 2: boss parts */
    ExObj *unk1C[10];        /* @0x1C kind 3 */
    ExObj *unk44[0x1e];      /* @0x44 kind 1: effects / shots */
    ExObj *unkBC[0x2e];      /* @0xBC all objects */
} ExObjManager;
extern ExObjManager *gExtraBossPlayerManager;

/* player parameters data_ov001_0231da50 (0x18 per character) */
typedef struct ExPlayerCfg {
    u16 unk0;                /* dash buttons */
    u16 unk2;                /* special buttons (Blaze: fire) */
    s32 unk4;                /* accel */
    s32 unk8;                /* decel */
    s32 unkC;                /* max speed */
    s32 unk10;               /* dash deceleration */
    s32 unk14;               /* dash speed */
} ExPlayerCfg;
/* player state entry (data_ov001_0231da80 / 0231dc40, 0x38 bytes) */
typedef struct ExPlayerStateDef {
    u16 idx, anim0, anim1, flags;
    s32 v8[3], v14[3];
    s16 a20[3], a26[3];
    u8 pad2C[8];
    u32 fn;                  /* @0x34 ROM address of the state function */
} ExPlayerStateDef;
/* player extra work (0x14) */
typedef struct ExPlayerWork {
    u32 unk0;                /* 1 clamp to the field, 2 invincible, 4 held by the suction */
    s32 unk4;                /* current state (ExtraBossPlayer_ChangeState) */
    const ExPlayerCfg *unk8;
    const ExPlayerStateDef *unkC;
    s32 unk10;               /* invincibility timer */
} ExPlayerWork;

/* boss extra work: hand boss (0x1C4) / weapon boss (0x168); node matrices as fx32 4x3 + 3x3 */
typedef struct { fx32 m[12]; } ExMtx43;
typedef struct ExBossWork {
    s32 unk0;                /* current attack state */
    s32 unk4;                /* next attack */
    s32 unk8;                /* previous state */
    s32 unkC;                /* attack count */
    union {
        s32 unk10;           /* hand: count reload */
        ExObj *sword;        /* weapon: unk10 sword effect */
    } u10;
    u16 unk14, unk16;        /* hand: catch hand out / player grabbed */
    ExObj *unk18;            /* hand: the catch hand object */
    ExMtx43 mtx[5];          /* native: node matrices (MTX_RESTORE slots read back) */
    s32 flash;               /* hand unk1C0 / weapon unk164: core palette flash counter */
} ExBossWork;
/* node matrix slots used by the draw functions: weapon 0x1b p_body, 0x1c core, 0x1d weapon_l,
 * 0x1e weapon_r; hand 0x1d p_body, 0x1b core, 0x1c l_hand0a, 0x1e body_hand; hand catch 0x1d hand_catch */
enum { EXM_SLOT_1B = 0, EXM_SLOT_1C, EXM_SLOT_1D, EXM_SLOT_1E, EXM_CATCH };

/* player trail work (0x2C8) */
typedef struct ExTrailNode {
    fx32 a[3];               /* @0x00 */
    fx32 b[3];               /* @0x0C */
    struct ExTrailNode *next; /* @0x18 */
    struct ExTrailNode *prev; /* @0x1C */
} ExTrailNode;
typedef struct ExTrailWork {
    s32 unk0;                /* width */
    ExTrailNode nodes[0x15];
    ExTrailNode *unk2A4;     /* current */
    s32 unk2A8;              /* length */
    s32 unk2AC;              /* max length */
    u16 unk2B0, unk2B2;      /* colours */
    s32 unk2B4, unk2B8, unk2BC, unk2C0, unk2C4;
} ExTrailWork;

/* --------------------------------------------------------- AnimatorSprite */
typedef struct ExSprImage { u8 page, pad; u16 x, y, w, h; s16 ox, oy; u16 pad2; } ExSprImage;
typedef struct ExSprAnim { u16 first_frame, frame_count; s16 loop_frame, next_anim; u8 format, pad; u16 pad2; } ExSprAnim;
typedef struct ExSprRes {
    u8 *frames;
    const ExSprImage *images;
    const ExSprAnim *anims;
    const u16 *frametab;
    u16 n_images, n_anims, n_frames, page_w, page_h;
    u16 *pixels;             /* ARGB1555 atlas (host renderer, texture upload without palettes.bin) */
    void *tex;               /* pvr_ptr_t */
    int loaded;
    void *atlas;             /* KOS SprAtlas: PAL4 page + palette rows (engine/render.h) */
} ExSprRes;
typedef struct ExSprite {    /* AnimatorSprite subset (0x48 bytes on the DS) */
    const ExSprRes *res;     /* @0x00 */
    s16 x, y;                /* @0x04 / @0x06 */
    u16 anim;                /* @0x08 */
    u16 frameNo, frameCount; /* @0x0A / @0x0C */
    s16 speed;
    s32 wait, frame;
    u32 flags;               /* @0x2C 1 stopped, 2 ended, 4 loop, 0x80 h flip, 0x100 v flip */
    u16 unk44;               /* @0x44 palette slot */
    s16 cur_image;
    s32 sx, sy;              /* native: DrawRotScale scale (0x1000 = 1) */
    u16 rot;
} ExSprite;
void ex_sprite_init(ExSprite *s, const ExSprRes *res, u16 anim, u32 flags);  /* AnimatorSprite_Init */
void ex_sprite_process(ExSprite *s);                                         /* AnimatorSprite_ProcessAnimation */
void ex_sprite_draw(const ExSprite *s);                                      /* AnimatorSprite_Draw (main OAM) */
void ex_sprite_draw_rs(const ExSprite *s, s32 sx, s32 sy, u16 rot);          /* AnimatorSprite_DrawRotScale */
int ex_sprres_load(ExSprRes *r, const char *id);
void ex_sprres_free(ExSprRes *r);

/* ------------------------------------------------------------ rings (0x1388) */
typedef struct ExRing {
    fx32 x, y;               /* @0x00 */
    fx32 ax, ay;             /* @0x08 */
    fx32 vx, vy;             /* @0x10 */
    s32 unk18;               /* @0x18 timer / kind */
    s32 unk1C;               /* @0x1C pickup delay */
    s32 unk20;               /* @0x20 collected sparkle time */
    struct ExRing *prev;     /* @0x24 */
    struct ExRing *next;     /* @0x28 */
} ExRing;
typedef struct ExRingWork {
    ExSprite unk0;           /* @0x00 ring sprite */
    ExSprite unk48;          /* @0x48 sparkle sprite */
    ExTaskFn unk90;          /* @0x90 update function */
    u32 unk94;               /* @0x94 1 scroll table spawn, 2 pickup */
    ExRing *unk98, *unk9C;   /* @0x98 placed list */
    ExRing *unkA0, *unkA4;   /* @0xA0 collected list */
    ExRing *unkA8, *unkAC;   /* @0xA8 loose list */
    ExRing *unkB0, *unkB4;   /* @0xB0 attracted list */
    s32 unkB8;               /* @0xB8 stack pointer */
    ExRing *unkBC[100];      /* @0xBC free stack */
    ExRing unk24C[100];      /* @0x24C */
    s32 unk137C;             /* scroll table index */
    s32 unk1380;             /* scroll position */
    u32 unk1384;             /* scroll table (DS address: 02316cac / 02316b9c) */
} ExRingWork;
extern ExRingWork *gExtraBossRingWork;

/* ---------------------------------------------------------- background (0xB54) */
typedef struct ExBgWork {
    u32 unk0;                /* bit 0: frozen */
    s16 unk584, unk586;
    s32 unk588;              /* scroll (16.16 px) */
    s32 unk58C;
    s32 unk590;              /* scroll speed */
    s32 unk594;              /* last scroll step */
    ExTaskFn unk598;         /* ExtraBoss_State_ScrollSpawn / ScrollDone */
    s32 unk59C;              /* palette frame timer */
    s32 unk5A0;              /* palette frame */
    u32 unk5A4;              /* palette frame table (DS address) */
    s32 unk5A8;              /* palette frames */
    s16 unk5AC[2][0xc0];     /* @0x5AC BG1 HOFS per line (double buffer: unk5AC / unk72C) */
    s16 unk8AC[0x140];       /* @0x8AC wave table */
    u16 unkB2C;              /* buffer shown */
    s32 unkB30, unkB34, unkB38;
    s32 unkB3C;              /* meteor timer */
    s32 unkB40;              /* meteor count */
    s32 unkB44;              /* big beam timer */
    s32 unkB48;              /* big beam period */
    s32 unkB4C, unkB50;      /* shake time / amplitude */
    s16 vofs;                /* gDisplayMain BG1 VOFS */
    u16 pal[256];            /* native: BG palette RAM (PaletteTransfer) */
    int pal_dirty;
} ExBgWork;
extern ExBgWork *data_ov001_0231f54c;
typedef struct ExBgImage {   /* zex.bin */
    u16 w, h, npal;
    const u16 *base;         /* zex.bbg palette (256) */
    const u16 *pals;         /* [npal][192] */
    const u8 *px;            /* [h][w] */
    u8 *bytes;
} ExBgImage;
extern ExBgImage g_ex_bg;

/* ----------------------------------------------------- sub screen (0x5F0) */
typedef struct ExSubWork {
    u32 unk0;                /* 1 names shown, 2 "VS" text shown */
    ExTask *unk4[5];         /* @0x04 tasks: draw, window, name scale, HDMA, text */
    s32 bg;                  /* @0x18 BG shown (Background_Draw bgs[unk3C]) */
    ExSprite unk98[8];       /* @0x98 sprites (ac_ex_vs.bac) */
    s16 unk2D8[2][0xc0];     /* @0x2D8 BG HOFS per line (double buffer) */
    s32 unk5D8;
    s16 unk5DC;
    u16 unk5DE;
    s32 unk5E0;
    s16 unk5E4, unk5E6;
    s32 unk5E8, unk5EC;
    u8 win[16];              /* data_02087d14: window 0/1 rectangles, enables */
    int hdma_on;             /* gSystemFlags 0x400 */
    s32 name_scale;          /* native: scale of the name sprites (ExtraBoss_Unk_ov001_02305bb8) */
    s32 text_scale;          /* native: zoom of the text sprites (ExtraBoss_Unk_ov001_023059b4) */
    int drawn;               /* native: the draw task (ExtraBoss_Unk_ov001_023062f0) ran this frame */
    s32 cutin_t;             /* native: frames since the sequence ended (unk0 & 2), for the cut-in fade */
} ExSubWork;
extern ExSubWork *data_ov001_0231f5a8;

/* ------------------------------------------------------------- camera slots */
typedef struct ExCamSlot {   /* data_ov001_023169b4 (3 x 0x44) */
    u16 unk0;                /* half fovy */
    u16 pad2;
    fx32 unk4, unk8, unkC, unk10; /* near, far, aspect, W scale */
    fx32 unk14[3];           /* eye */
    fx32 unk20[3];           /* target */
    fx32 unk2C[3];           /* up */
    fx32 unk38[3];           /* offset (shake) */
} ExCamSlot;
extern ExCamSlot *data_ov001_0231f080;

/* ------------------------------------------------------------- main work (0x340) */
typedef struct ExWork {
    u32 unk0;                /* 1 dual-screen 3D, 2 round intro, 4 HUD, 8 no pause, 0x10 player down,
                                0x20 faded out, 0x40 suction, 0x100 cleared, 0x200 time over,
                                0x400 next round */
    s32 unk4;                /* camera slot (0 screen fit, 1 top, 2 bottom) */
    ExTask *unk8, *unkC, *unk10, *unk14;
    u16 unk18, unk1A, unk1C, unk1E, unk20; /* gPad[0..4] remapped (held, -, pressed, -, repeat) */
    u16 unk22[12];           /* pad bit map (ExtraBoss_InitPadMap) */
    s32 unk3C;               /* character: 0 Super Sonic, 1 Burning Blaze */
    s32 unk40;               /* round 0..4 */
    s32 unk44;               /* time (frames) */
    s32 unk48;               /* time at the start of the pair */
    s32 unk4C;               /* rings */
    s32 unk50, unk54, unk58, unk5C; /* field top, left, right, bottom */
    s8 unk60[2];             /* lives (Sonic, Blaze) */
    s32 unk308;              /* boss hit points */
    s32 unk30C;              /* boss start hit points */
    s32 unk310;              /* suction height */
    s32 unk314;              /* player-down delay */
    s32 unk316;              /* fade-out done (BrightnessLerp target) */
    s32 unk318;              /* camera: 0 follow the player, 1 track the boss */
    fx32 unk31C, unk320;     /* camera centre */
    fx32 unk324, unk328, unk32C, unk330, unk334; /* follow margins (up, left, right, down), speed */
    u16 unk338;
    u16 unk304;
} ExWork;
extern ExWork *gExtraBossWork;
extern fx32 data_ov001_0231f520, data_ov001_0231f524; /* scroll: -(camera y - 96), -(camera x - 128) */
extern s32 gExtraBossResultTime, gExtraBossResultRings;
extern u8 *data_ov001_0231f094;  /* [0] beam units out (1, 2), [1] sword cancel */
extern s32 data_ov001_0231f064;  /* node callback step */

/* game state the fight reads */
typedef struct ExGame {
    s32 unk0;                /* gGameState.unk0 (player slot) */
    s32 unk10;               /* gGameState.unk10 (mode: 2 / 3 time attack, VS) */
    s32 unk14;               /* gGameState.unk14 (difficulty) */
    int time_unlimited;      /* gSaveData.unk8 & 2 */
    s16 bright_main;         /* gDisplayMain.unk48 */
    s16 bright_sub;
    int paused;
    int result;              /* BOSS_EXTRA_* */
    int title_timer;
    int character_at_entry;
    int lives_in[2];
    InputState in;
} ExGame;
extern ExGame g_ex;
s32 ex_level(void);          /* (mode 2 / 3) ? 1 : gGameState.unk14 */

/* --------------------------------------------------------------- resources */
typedef struct ExFxRes {     /* gExtraBossFxRes (0x18 per kind) + gExtraBossFxResTable */
    u32 flags;
    BossModelRes mdl;
    BossAnimRes anm[5];
    u16 frames[5][8];
} ExFxRes;
extern ExFxRes gExtraBossFxRes[0x1b];
extern BossModelRes g_ex_bsex[5];      /* data_ov001_0231f070: bsex_md.nsbmd models */
extern BossAnimRes data_ov001_0231f068; /* bsex_ca.nsbca (archive file 0x28) */
extern BossAnimRes data_ov001_0231f074; /* bsex_va.nsbva (0x29) */
extern BossAnimRes data_ov001_0231f534; /* ex_ply_ca.nsbca (0x24) */
extern BossAnimRes data_ov001_0231f53c; /* ex_ply_va.nsbva (0x25) */
extern BossModelRes data_ov001_0231f540; /* e_son / e_blz */
extern BossModelRes data_ov001_0231f538; /* son_easy / blz_easy */
extern ExSprRes data_ov001_0231f320;   /* ac_ex_fix_moji.bac (0x26) */
extern ExSprRes gExtraBossRingSprite;  /* ac_itm_ring.bac (0x27) */
extern ExSprite *data_ov001_0231f324;  /* HUD sprites (0x1b) */
int ex_joint(int model, const char *name);
const char *ex_model_file(int i);   /* bossex.bin models: 0..4 bsex_md, 5 e_son, 6 e_blz, 7 son_easy, 8 blz_easy */
int ExtraBoss_LoadAssets(void);
void ExtraBoss_ReleaseAssets(void);
void ex_vsbg_load(void);   /* boss_extra_draw.c: vs_son.bin / vs_blz.bin */
void ex_vsbg_free(void);
extern char g_ex_model_dir[96], g_ex_data_dir[96];

/* ----------------------------------------------------------------- draw list */
typedef struct ExDrawItem {
    const BossModel *m;
    dcm_mtx_t mtx;
    int offaxis;             /* ExtraBossObj_DrawModel flag 1: frustum shifted to the model */
    float fr[4];             /* off-axis frustum (top, bottom, left, right at near) */
    int slot;                /* camera slot */
    u8 vis[DCM_MAX_JOINTS];
    int has_vis;
} ExDrawItem;
void ex_draw_reset(void);
void ex_draw_model(BossModel *m, int offaxis, const float fr[4]); /* Model_Draw: pose + queue */
void ex_draw_trail(const ExObj *p);                               /* ExtraBossObj_Draw_PlayerTrail */
void ex_camera(int slot, dcm_camera_t *out);
void ex_hud_sprite(const ExSprite *s);                            /* queued 2D sprite (OAM) */
void ex_draw_frame(int list);
void ex_draw_shutdown(void);
void BrightnessLerp_Create(s32 total, s16 from, s16 to, u32 flags, s32 *done);
void ex_brightness_run(void);

/* ------------------------------------------------- functions shared across files */
/* boss_extra.c */
ExWork *ExtraBoss_GetWork(void);
void ExtraBoss_Start(void);
void ExtraBoss_Shutdown(void);
void ExtraBoss_Main(void);
void ExtraBoss_State_Battle(void);
void ExtraBoss_PostUpdate_Battle(void);
void ExtraBoss_Draw_Battle(void);
void ExtraBoss_StartBattle(void);
void ExtraBoss_ResetRound(void);
void ExtraBoss_AdvancePhase(void);
void ExtraBoss_StartCharacterSwitch(void);
void ExtraBoss_ResetCamera(void);
void ExtraBoss_UpdateCamera(void);
void ExtraBoss_SetCameraParams(s32 a, s32 b, s32 c, s32 d, s32 e);
ExCamSlot *ExtraBoss_GetCurrentCharacterSlot(void);
void ExtraBoss_SetTimeAndRings(s32 t, s32 r);
void ExtraBoss_SpawnBoss(void);
/* boss_extra_obj.c */
ExObj *ExtraBossObj_Create(s32 kind, u16 id);
void ExtraBossObj_Main(void);
void ExtraBossObj_Destroy(ExObj *w);
void ExtraBossObj_DestroyFlagged(void);
void ExtraBossObj_DestroyKind1(void);
void ExtraBossObj_DestroyKind2(void);
void ExtraBossObj_DestroyKind2And3(void);
void ExtraBossObj_DestroyPlayer(void);
void ExtraBossObj_DrawAll(void);
void ExtraBossObj_Draw_Default(ExObj *p);
void ExtraBossObj_DrawModel(ExObj *w, BossModel *a);
void ExtraBossObj_UpdateMotionAll(void);
void ExtraBossObj_PropagateParentDestroy(void);
BOOL ExtraBossObj_IsAlive(const ExObj *p);
BOOL ExtraBossObj_IsOutOfArena(s32 x, s32 y, s32 w, s32 h);
void ExtraBossObj_State_None(ExObj *p);
void ExtraBossObj_State_Blink(ExObj *p);
void ExtraBossObj_State_Blink2(ExObj *p);
void ExtraBossObj_State_DestroyAfterTimer(ExObj *p);
void ExtraBossObj_State_DestroyWhenAnimsDone(ExObj *p);
void ExtraBossObj_State_DestroyWhenOutOfArena(ExObj *p);
void ExtraBossObj_ReleaseFxModelRes(ExObj *p);
void ExtraBoss_InitCollider(ExCollider *c, ExObj *owner, u32 mask, s32 mode, s32 a, s32 b, s32 d, s32 e, s32 f,
                            s32 g, u16 h);
void ExtraBoss_ClearColliderLists(void);
void ExtraBoss_CheckAllCollisions(void);
ExObj *ExtraBossPlayer_CreateFxObj(s32 kind, ExObj *parent, s32 x, s32 y, s32 z);
BossModel *ExtraBossPlayer_SetupFxModel(BossModel *o, s32 n);
void ExtraBoss_SwapObjModel(ExObj *p, s32 n);
void ExtraBoss_SetObjModelAnims(ExObj *p, s32 idx);
void ExtraBoss_CopyParentPosition(ExObj *p);
void ExtraBoss_SnapObjToCameraX(ExObj *p);
s32 ExtraBoss_CountActiveBeamUnits(void);
void ExtraBoss_ResetFxFlags(void);
void ExtraBoss_CancelSwordFx(void);
void ExtraBoss_SpawnMeteor(ExObj *p, s32 x, s32 y, s32 z);
void ExtraBoss_SpawnMeteorR(ExObj *p, s32 x, s32 y, s32 z);
void ExtraBoss_SpawnBigBeam(ExObj *p, s32 y, s32 z);
void ExtraBoss_SpawnBeamUnit(ExObj *parent, s32 x, s32 y, s32 z, u16 ang);
void ExtraBoss_SpawnHomingBeam(ExObj *parent, s32 x, s32 y, s32 z);
void ExtraBoss_SpawnHomingMissile(ExObj *parent, s32 x, s32 y, s32 z, u16 ang);
void ExtraBoss_SpawnBombFx(ExObj *a, s32 x, s32 y, s32 z, BOOL flag);
void ExtraBoss_SpawnClawFx(ExObj *a, s32 x, s32 y, s32 z, s16 big);
void ExtraBoss_SpawnCatchFx(ExObj *a, s32 x, s32 y, s32 z);
void ExtraBoss_SpawnJumpFx(ExObj *a, s32 x, s32 y, s32 z);
void ExtraBoss_SpawnDamageFx(ExObj *a, s32 x, s32 y, s32 z);
void ExtraBoss_SpawnSpacebFx(ExObj *a, s32 y, s32 z);
ExObj *ExtraBoss_SpawnSwordFx(ExObj *p);
void ExtraBoss_AdvanceSwordFxStep(ExObj *p);
void ExtraBossPlayer_SpawnBlzShotFx(ExObj *p, s32 x, s32 y, s32 z);
void ExtraBossPlayer_SpawnSpecialShot(ExObj *p, s32 x, s32 y, s32 z);
void ExtraBossPlayer_SpawnClashFx(ExObj *p, s32 x, s32 y, s32 z);
/* boss_extra_player.c */
ExObj *ExtraBoss_GetPlayer(void);
BOOL ExtraBoss_IsPlayerSpawned(void);
void ExtraBossPlayerManager_Create(void);
void ExtraBossPlayerManager_Destroy(void);
void ExtraBoss_LoadPlayerModel(s32 idx);
void ExtraBossPlayer_ChangeState(ExObj *p, s32 n);
void ExtraBossPlayer_Action_Defeat(void);
void ExtraBossPlayer_Action_Hurt(void);
void ExtraBossPlayer_Action_LockControl(void);
void ExtraBossPlayer_Action_UnlockControl(ExObj *p);
void ExtraBoss_ResetPlayerForBattle(void);
void ExtraBoss_Unk_ov001_02302658(void);
ExFn ex_player_state_fn(u32 rom_addr);
/* boss_extra_hand.c / boss_extra_weapon.c */
ExObj *ExtraBoss_CreateHandBoss(void);
ExObj *ExtraBoss_CreateWeaponBoss(void);
void ExtraBossObj_State_HandBossCatchWaitHand(ExObj *p);
void ExtraBoss_StopAndFaceCenter(ExObj *p);
void ExtraBoss_StopAndFaceCenter2(ExObj *p);
void ExtraBoss_StepXTowardCameraTarget(ExObj *p);
void ExtraBoss_StepXTowardRightEdge(ExObj *p);
void ExtraBoss_LoadModelPalettes(u32 names, u32 idx, u32 slots, s32 n);
void ExtraBoss_HandNodeCallback(BossModel *m);
void ExtraBossObj_State_BeamUnitDeflected(ExObj *p);
/* boss_extra_stage.c */
void ExtraBoss_InitRings(void);
void ExtraBoss_FreeSpawnWork(void);
void ExtraBoss_ResetSpawnWork(void);
void ExtraBoss_ClearSpawnLists(void);
void ExtraBoss_UpdateSpawnWork(void);
void ExtraBoss_CollectRings(void);
void ExtraBoss_DrawRings(void);
void ExtraBoss_ScatterPlayerRings(s32 x, s32 y, s32 n);
void ExtraBoss_SpawnRingBurst(s32 x, s32 y, s32 count);
void ExtraBoss_StartRingSuction(void);
void ExtraBoss_EndRingSuction(void);
void ExtraBoss_SetRingPickupEnabled(s32 on);
void ExtraBoss_SetScrollSpawnEnabled(s32 on);
void ExtraBoss_AllocBgWork(void);
void ExtraBoss_FreeBgWork(void);
void ExtraBoss_Unk_ov001_02304fb4(void);
void ExtraBoss_UpdateBgScrollPalette(void);
void ExtraBoss_UpdateBgWaveScroll(void);
void ExtraBoss_StartBgShake(s32 a, s32 b);
void ExtraBoss_Unk_ov001_022fdbc0(void);
void ExtraBoss_FreeObjVramSlots(void);
void ExtraBoss_DrawHud(void);
void ExtraBoss_AllocSubScreenWork(void);
void ExtraBoss_FreeSubScreenWork(void);
void ExtraBoss_StopSubScreenTasks(void);
void ExtraBoss_SetupSubDisplay(BOOL on);
void ExtraBoss_ResetSubDisplay(void);
void ExtraBoss_Unk_ov001_02306424(void);
void ExtraBoss_Unk_ov001_022ffcf8(void);
void ExtraBoss_AllocFlagBlock(void);
void ExtraBoss_FreeFlagBlock(void);
extern ExSprRes g_ex_vs_sprites;

/* trace / statistics for the host test */
typedef struct BossExStats {
    u32 rounds_started, rounds_won, boss_hits, player_hurts, player_downs, rings_collected, rings_lost;
    u32 dashes, shots, deflects, meteors, meteors_broken, big_beams, homing_beams, beam_units, missiles,
        swords, claws, catches, grabs, suctions, switches;
    u32 boss_spawned[2]; /* weapon, hand */
} BossExStats;
extern BossExStats g_bossex_stats;
const char *ex_state_name(ExFn fn);         /* object states, NULL if unknown */
const char *ex_weapon_state_name(ExFn fn);  /* weapon boss states */
const char *ex_hand_state_name(ExFn fn);    /* hand boss states */

#endif

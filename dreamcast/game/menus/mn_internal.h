/* Front-end internals: the DS engine services the menu code calls, rebuilt on
 * native data (task list, AnimatorSprite on converted BAC frames, menu BGs,
 * KeyframeAnim on .ama banks, master brightness, gPad, touch -> pad mapping).
 * Names follow the decompilation so the translated menu code reads the same. */
#ifndef SRDC_MN_INTERNAL_H
#define SRDC_MN_INTERNAL_H
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "menus.h"
#include "../save/save.h"

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef s32 fx32;
typedef int BOOL;
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

static inline fx32 FX_Mul(fx32 a, fx32 b) { return (fx32)(((s64)a * b + 0x800) >> 12); }
/* FX_Div: 20.12 / 20.12 -> 20.12 (CP1 divider, truncating) */
static inline fx32 FX_Div(fx32 a, fx32 b) { return b ? (fx32)(((s64)a << 12) / b) : 0; }

/* ------------------------------------------------------------- tasks */
typedef struct MnTask {
    void (*func)(void);   /* +8 on the DS (state function) */
    void (*dtor)(void);
    u16 prio, tag;
    void *work;           /* +0x10 */
    u32 size;
    u8 used, dead, fresh;
} MnTask;
#define MN_MAX_TASKS 48
extern MnTask *gTaskCurrent;
MnTask *Task_Create(void (*fn)(void), void (*dtor)(void), u16 flags, u16 prio, u16 tag, u32 size);
void Task_Destroy(MnTask *t);
void Task_DestroyAll(void);
void Task_DestroyRange(u16 lo, u16 hi);
void Task_DestroyByTag(u16 tag);
void Task_RunAll(void);
int Task_Count(void);
#define TASK_WORK(T) ((T *)gTaskCurrent->work)

/* ------------------------------------------------- pad (Pad_Update) */
enum { PAD_A = 1, PAD_B = 2, PAD_SELECT = 4, PAD_START = 8, PAD_RIGHT = 0x10, PAD_LEFT = 0x20,
       PAD_UP = 0x40, PAD_DOWN = 0x80, PAD_R = 0x100, PAD_L = 0x200, PAD_X = 0x400, PAD_Y = 0x800 };
typedef struct {
    u16 cur, prev, trg, rel, rep;
    u16 cnt[12], delay[12], interval[12];
} MnPad;
extern MnPad gPad;
void Pad_Reset(MnPad *p, const u16 *delay, const u16 *interval);
void Pad_Update(MnPad *p, u16 keys);
u16 mn_pad_from_input(const InputState *in);

/* --------------------------------------------------- display state */
typedef struct { u16 x, y; } MnBgOfs;
extern MnBgOfs gDisplayMain[4], gDisplaySub[4];   /* BG scroll per engine (gDisplayMain/Sub ofs[]) */
extern s16 g_mn_bright[2];   /* data_0208926c (main) / data_02087d44 (sub): -16 black .. 16 white */
#define BRIGHT_MAIN g_mn_bright[0]
#define BRIGHT_SUB g_mn_bright[1]

/* ------------------------------------------------------ BG images (.mnbg) */
typedef struct MnBgImg {
    u16 w, h, type, pofs, pslot;
    u16 pal[256];          /* DS BGR555, the BG palette slot */
    u8 *idx;               /* w*h palette indices, 0 = transparent */
    u16 *argb;             /* expanded texture (tex_w*tex_h), rebuilt when the palette changes */
    u16 tw, th;
    void *tex;             /* KOS Texture */
    u8 dirty;
    char name[64];
} MnBgImg;
MnBgImg *mn_bg_load(const char *bbg_path);   /* "bg/x.bbg" -> <menus>/bg/x.mnbg */
void mn_bg_free(MnBgImg *b);
void mn_bg_set_colors(MnBgImg *b, int start, int count, const u16 *bgr);
void mn_bg_expand(MnBgImg *b);              /* palette -> argb (host + KOS) */

/* one BG layer of an engine (Background_Init + Background_Draw) */
typedef struct {
    MnBgImg *img;
    u8 visible, prio;
    s16 alpha;      /* 0..16 (BLDALPHA), 16 = opaque */
    u16 wrap_w, wrap_h;   /* BG plane size (BGxCNT size bits); 0 = 256x256 or the image size */
} MnBgLayer;
void Background_SetSize(int engine, int layer, int w, int h);
extern MnBgLayer g_mn_layer[2][4];
/* Background_Init(bg, res, flags, engine, layer, w, h) + Background_Draw */
void Background_Show(int engine, int layer, MnBgImg *img);
void Background_Hide(int engine, int layer);
void Background_SetPriority(int engine, int layer, int prio);
void mn_display_reset(void);   /* the *_ResetDisplay functions: BGs off, scroll 0 */

/* ------------------------------------------------------ sprite resources */
typedef struct { u16 w, h; } MnPage;
typedef struct { u8 page, pad; u16 x, y, w, h; s16 ox, oy; u16 pad2; } MnImage;
typedef struct { u16 first, count; s16 loop, next; u8 fmt, pad; u16 pad2; } MnAnimDef;
typedef struct { u16 image, duration; } MnFrame;
typedef struct MnRes {
    char id[64];
    u8 *blob;
    const MnPage *pages;
    const MnImage *images;
    const MnAnimDef *anims;
    const MnFrame *frames;
    u16 n_pages, n_images, n_anims, n_frames;
    u16 *pixels[4];
    void *tex[4];
} MnRes;
MnRes *mn_res_load(const char *bac_path);   /* "act/title.bac" -> <sprites>/act__title/ */
void mn_res_free(MnRes *r);

/* AnimatorSprite (Graphics/Sprite) subset. flags: DS bits */
enum { SPR_STOPPED = 1, SPR_ENDED = 2, SPR_LOOP = 4, SPR_HFLIP = 0x80, SPR_VFLIP = 0x100,
       SPR_BLEND = 0x400, SPR_HIDDEN = 0x80000, SPR_PRIO_MASK = 0x60000 };
typedef struct MnSpr {
    const MnRes *res;
    s16 x, y;
    u16 anim, frame;
    s32 wait;
    s16 speed;       /* 0x1000 = 1 frame per frame (AnimatorSprite unk2A analogue, DS: 0x10 << 8) */
    u32 flags;
    u8 engine;       /* 0 main (top), 1 sub (bottom) */
    u16 alpha;       /* 0..16, 16 opaque (blend effects) */
} MnSpr;
void AnimatorSprite_Init(MnSpr *s, const MnRes *r, u16 anim, u32 flags, int engine);
void AnimatorSprite_SetAnimation(MnSpr *s, u16 anim);
void AnimatorSprite_ProcessAnimation(MnSpr *s, s32 a, s32 b);
void AnimatorSprite_Draw(MnSpr *s);
/* DrawRotScale: scale 0x1000 = 1.0 about the sprite origin (rotation not used by the menus here) */
void AnimatorSprite_DrawScaled(MnSpr *s, fx32 sx, fx32 sy);
int AnimatorSprite_AnimCount(const MnSpr *s);

/* ---------------------------------------------------------- quads */
typedef struct {
    float x, y, w, h;        /* DS screen px of the engine */
    float u0, v0, u1, v1;
    const MnRes *res;        /* sprite texture (page in .page) or NULL */
    const MnBgImg *bg;       /* BG texture or NULL; both NULL = flat colour */
    u8 page, engine, prio, kind;
    u32 argb;
} MnQuad;
enum { MNQ_SPRITE, MNQ_BG, MNQ_FLAT };
#define MN_MAX_QUADS 768
int mn_quads(const MnQuad **out);
void mn_quads_reset(void);
void mn_emit_flat(int engine, int prio, float x, float y, float w, float h, u32 argb);
/* Single-screen composition. The two DS engines are not shown as two
 * screens: a scene places rectangles of an engine's DS frame (source rect in
 * DS px) into the one 640x480 frame with a uniform scale. Regions are drawn
 * in the order added (back to front); each region draws its engine's BG
 * layers (unless hidden for the composition) and sprites in DS order,
 * clipped to the source rect. One brightness (the base engine's) covers the
 * whole frame. */
typedef struct {
    float sx0, sy0, sx1, sy1;   /* source rect, DS px of the engine */
    float dx, dy, scale;        /* destination of (sx0, sy0) on 640x480, uniform scale */
    u8 engine;
    u8 hide_bg;                 /* bit l: BG layer l left out of this region */
    u8 hide_spr;                /* 1: the engine's sprites left out of this region */
} MnRegion;
#define MN_MAX_REGIONS 8
typedef struct {
    MnRegion r[MN_MAX_REGIONS];
    int n;
    u8 base;           /* engine whose brightness fades the frame */
    u32 fill;          /* backdrop colour under everything (0 = none) */
} MnComp;
extern MnComp g_mn_comp;
void mn_comp_reset(int base_engine);
/* src rect (x0,y0)-(x1,y1) of `engine` to (dx,dy) at `scale`; hide_bg = BG
 * layer mask left out, hide_spr = leave the sprites out */
MnRegion *mn_comp_add(int engine, float x0, float y0, float x1, float y1, float dx, float dy, float scale, int hide_bg,
                      int hide_spr);
/* the whole 256x192 frame of `engine` at 2.5x */
MnRegion *mn_comp_full(int engine);
typedef struct { float x, y, w, h, u0, v0, u1, v1; } MnOut;
int mn_project(const MnQuad *q, const MnRegion *r, MnOut *o);   /* DS quad -> 640x480, clipped to the region */
/* the final sorted list (DS compositing order), used by the KOS drawer and the host test */
typedef struct { const MnQuad *q; const MnRegion *r; } MnDraw;
int mn_sorted(const MnDraw **out);
/* texture size for a w x h image: power of two, 8..1024 (PVR rule) */
int mn_tex_pow2(int v);

/* ----------------------------------------------------- KeyframeAnim (.ama) */
typedef struct {
    s16 x, y;
    s32 unk4, unk8;    /* scale x/y */
    s16 unkc;          /* rotation */
    u8 pade[2];
    u32 flags;         /* 1 stopped, 2 loop, 8 reverse, 0x20 dirty, 0x40 finished */
    u32 seq;
    s32 frame, speed;
    s32 unk20, unk24;  /* cached key index (position / scale) */
    const u8 *hdr;
} KeyframeAnim;
void *mn_ama_load(const char *path);   /* "/ama/x.ama" */
void KeyframeAnim_Init(KeyframeAnim *a, const void *hdr, u32 seq);
void KeyframeAnim_Update(KeyframeAnim *a);
BOOL KeyframeAnim_IsFinished(const KeyframeAnim *a);
void KeyframeAnim_PlayReverse(KeyframeAnim *a);
void KeyframeAnim_SetFrame(KeyframeAnim *a, s32 frame);
void KeyframeAnim_SetToLastFrame(KeyframeAnim *a);
void KeyframeAnim_SetAnim(KeyframeAnim *a, u32 seq);
s32 KeyframeAnim_SeqFrames(const void *hdr, u32 seq);

/* -------------------------------------------------------------- data */
typedef struct {
    u8 save_sig[12];
    u8 story_s[8], story_b[8];      /* data_02074bbc / data_02074bb4 */
    u8 zone_ord[2][9];              /* data_02074bc4 / data_02074bd0 */
    u16 tm_slide[7][4];             /* data_ov001_0231616c */
    s32 zm_adj[2][17][4];           /* data_ov001_0231649c */
    s8 zm_rects[17][4];             /* data_ov001_02316358 */
    s32 zm_icons[8];                /* data_ov001_023162f8 */
    s32 as_rank1[4], as_rank2[4];   /* data_ov001_0231c3f4 / 0231c404 */
    s32 st_widths[47];              /* data_ov001_02316028 */
    u16 vmu_pal[16];
    u8 vmu_icon[512];
    int ok;
} MnData;
extern MnData g_mn_data;
int mn_data_load(const u8 *buf, u32 size);
int mn_load_file(const char *path, u8 **out, u32 *size);
extern char g_mn_dir[96], g_mn_sprite_dir[96];
int mn_lang(void);   /* the GetLang inline: 0..5, else 1 */
extern u8 gGameLanguage;
extern u32 gRandSeed;
static inline u16 mn_rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

/* ----------------------------------------------------- game state */
/* gGameState fields the front end reads/writes (0x022c4560) */
typedef struct {
    s32 unk0;      /* character: 0 Sonic, 1 Blaze */
    u16 unkE;      /* map step (StoryProgress_CalcMapStep) */
    s32 unk10;     /* mode */
    s32 unk14;     /* difficulty (1 = normal: SaveData flag 1 clear) */
    u32 unk18;     /* flags (0x10, 0x20: VS) */
    u8 unk1C, unk1D;   /* zone, act */
    u16 unk1E;     /* attract demo index (gGameState[0xf]) */
    u16 unk28;     /* emeralds */
    u8 lives;      /* data_022c4590.unk14 */
    u32 score;     /* data_022c4590.unk4 */
    u16 new_flags; /* data_022c45f8 (bit 0: a new stage was cleared) */
} MnGameState;
extern MnGameState gGameState;
extern s32 gSaveDataBusy_dummy;

/* story progression (GameSystem/StoryProgress_*) */
u8 StoryProgress_CalcMapStep(u16 t);
u8 StoryProgress_GetZoneAt(u32 i);
/* The game modules (ui/, sound/) define these names with their own tables and
 * NNS signatures; the menu copies are renamed so both link into one binary. */
#define StoryProgress_GetZoneIndex MnStoryProgress_GetZoneIndex
#define Sound_PlaySeqArc MnSound_PlaySeqArc
#define Sound_PlayVoice MnSound_PlayVoice
#define Sound_PlayVoiceAlt MnSound_PlayVoiceAlt
#define Sound_PlayVoiceArc MnSound_PlayVoiceArc
u8 StoryProgress_GetZoneIndex(u8 v);
u8 StoryProgress_GetActIndex(u8 act);
u8 StoryProgress_GetActAt(u8 act);
BOOL StoryProgress_IsStageNew(void);
void StoryProgress_RestoreFromSave(void);
void StoryProgress_Update(void);
void Game_StartStoryStage(s32 zoneIndex, s32 act);

/* scene changes (the request main.c receives) */
void mn_request_stage(void);         /* SceneChange_ToStage with gGameState */
void mn_request_demo(void);          /* SceneChange_ToDemoPlay */
void mn_request_unavailable(const char *scene);
extern MenuRequest g_mn_request;

/* sound (MenuHooks) */
void SysSound_PlaySE(s32 id);
void SysSound_PlayBGM(s32 id);
void Sound_PlaySeq(s32 seq);
void Sound_PlaySeqArc(s32 arc, s32 id);
void Sound_PlayVoice(s32 id);
void Sound_PlayVoiceAlt(s32 id);
void Sound_PlayVoiceArc(s32 arc, s32 id);
void NNS_SndStopSoundAll(void);

/* scenes */
void SegaLogo_Start(void);
void TitleScreen_Start(void);
void TitleScreen_StartFromDemo(void);
void TitleMenu_Start(void);
void TitleMenu_StartFromOptions(void);
void TitleMenu_StartAtPlayerSelectItem0(void);
void TitleMenu_StartAtPlayerSelectItem1(void);
void PlayerSelectMenu_StartMode(int mode);
void ZoneMap_StartDefault(void);          /* SceneChange_ToZoneMap */
void ZoneMap_StartFromStage(void);        /* SceneChange_ToZoneMap_1_1: straight to the act select */
void ZoneMap_StartFromStageWithFade(void); /* SceneChange_ToZoneMap_1_0 */
void ZoneActSelectMode_Start(void);
void Options_StartFromTitleMenu(void);
/* scene ids for the host test */
const char *mn_scene_name(void);
extern const char *g_mn_scene;
#endif

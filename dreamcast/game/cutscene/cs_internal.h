/* Cutscene module internals: DS services the translated code calls, named as
 * on the DS where they are kept (AnimatorSprite_*, brightness, tasks are plain
 * state machines here), the RCSD tables of csdata.bin and the draw list.
 * CUTSCENES.md section 2. */
#ifndef CS_INTERNAL_H
#define CS_INTERNAL_H

/* Names the menus / UI modules also define: the cutscene copies are renamed so
 * every module links into one binary. */
#define AnimatorSprite_Draw cs_AnimatorSprite_Draw
#define AnimatorSprite_Init cs_AnimatorSprite_Init
#define AnimatorSprite_ProcessAnimation cs_AnimatorSprite_ProcessAnimation
#define AnimatorSprite_SetAnimation cs_AnimatorSprite_SetAnimation
#define BrightnessFade_Create cs_BrightnessFade_Create
#define BrightnessFade_Destroy cs_BrightnessFade_Destroy
#define BrightnessFade_IsDone cs_BrightnessFade_IsDone
#define BrightnessFade_Main cs_BrightnessFade_Main
#define gPad cs_gPad
#define gRandSeed cs_gRandSeed
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "cutscene.h"
#include "../model/model.h"

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef s32 fx32;
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
typedef int BOOL;

#define FX_Mul(a, b) ((s32)(((s64)(a) * (b) + 0x800) >> 12))
static inline s32 FX_Div(s32 a, s32 b) { return b ? (s32)(((s64)a << 12) / b) : 0; }

/* DS pad bits (gPad) as the translated code tests them */
enum { PAD_A = 1, PAD_B = 2, PAD_SELECT = 4, PAD_START = 8, PAD_RIGHT = 0x10, PAD_LEFT = 0x20, PAD_UP = 0x40,
       PAD_DOWN = 0x80, PAD_R = 0x100, PAD_L = 0x200, PAD_X = 0x400, PAD_Y = 0x800 };
typedef struct {
    u16 held, trg;
} CsPad;
extern CsPad gPad;

extern CsStory g_cs_story;
extern CsResult g_cs_result;
extern char g_cs_dir[96], g_cs_sprite_dir[96], g_cs_model_dir[96], g_cs_music_dir[96];
extern u32 gRandSeed;
static inline u16 cs_rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}
s16 cs_sin(u16 a);   /* FX_SinIdx table (1.12) */
s16 cs_cos(u16 a);

/* ------------------------------------------------------------- files / data */
void *cs_load_file(const char *path, u32 *size);
int cs_data_load(void);       /* csdata.bin, once */
const void *cs_sec(const char *name, u32 *size);
int cs_lang(void);            /* gGameLanguage clamped 0..5 */
extern const char *const cs_lang_suffix[6];
void cs_log(const char *fmt, ...);

/* --------------------------------------------------------------- images */
typedef struct {
    u16 w, h, type, flags, npal, pofs;
    const u16 *pal;
    const u8 *px;
} CsImg;
typedef struct {
    u8 *blob;
    u32 size, count;
} CsPack;
int cs_img_parse(CsImg *img, const u8 *blob, u32 size);
int cs_pack_load(CsPack *p, const char *path);
int cs_pack_get(const CsPack *p, int i, CsImg *out);
void cs_pack_free(CsPack *p);
/* single picture (bg/<name>.csi) */
typedef struct {
    u8 *blob;
    CsImg img;
} CsPic;
int cs_pic_load(CsPic *p, const char *name);
int cs_pack_read(CsPic *p, const char *path, int i);
void cs_pic_free(CsPic *p);

/* Texture made from an index buffer + a 256-colour DS palette (index 0 =
 * transparent). Re-expanded when cs_tex_set marks it dirty. tw x th is the
 * power-of-two size the PVR needs (8..1024). */
typedef struct {
    int w, h, tw, th;
    u16 *argb;       /* tw*th ARGB1555 */
    void *pvr;       /* pvr_ptr_t (KOS) */
    int dirty;
    int dy0, dy1;    /* rows changed since the last upload (dy1 > dy0) */
    u16 fill;        /* colour for index 0 when nonzero (opaque backdrops) */
} CsTex;
int cs_tex_init(CsTex *t, int w, int h);
/* expand idx (stride w) with pal; dst at (x,y) inside the texture */
void cs_tex_put(CsTex *t, int x, int y, int w, int h, const u8 *idx, int stride, const u16 *pal);
void cs_tex_clear(CsTex *t);
void cs_tex_free(CsTex *t);
u16 cs_bgr_to_argb(u16 c);

/* --------------------------------------------------------- sprites (BAC) */
typedef struct { u16 w, h; } CsPage;
typedef struct { u8 page, pad; u16 x, y, w, h; s16 ox, oy; u16 pad2; } CsImage;
typedef struct { u16 first, count; s16 loop, next; u8 format, pad; u16 pad2; } CsAnimDef;
typedef struct { u16 image, duration; } CsFrame;
typedef struct {
    u8 *blob;
    int n_pages, n_images, n_anims, n_frames;
    const CsPage *pages;
    const CsImage *images;
    const CsAnimDef *anims;
    const CsFrame *frames;
    CsTex tex[4];
} CsRes;
/* AnimatorSprite flags (DS values) */
enum { SPR_STOPPED = 1, SPR_ENDED = 2, SPR_LOOP = 4, SPR_HFLIP = 0x80, SPR_VFLIP = 0x100, SPR_HIDDEN = 0x80000 };
typedef struct {
    const CsRes *res;
    u16 anim, frame;
    s32 wait, speed;
    u32 flags;
    s16 x, y;
    u8 alpha;    /* 0..16 */
    u8 bright;   /* 0..255 colour scale (palette fades of the staff roll) */
} CsSpr;
CsRes *cs_res_load(const char *id);
void cs_res_free(CsRes *r);
void AnimatorSprite_Init(CsSpr *s, const CsRes *r, u16 anim, u32 flags);
void AnimatorSprite_SetAnimation(CsSpr *s, u16 anim);
void AnimatorSprite_ProcessAnimation(CsSpr *s);
void AnimatorSprite_Draw(CsSpr *s);        /* adds a quad at the current view */
int AnimatorSprite_FrameBox(const CsSpr *s, s16 *l, s16 *t, s16 *r, s16 *b);

/* ---------------------------------------------------------------- draw list */
enum { CSQ_FLAT = 0, CSQ_TEX = 1, CSQ_SPR = 2 };
typedef struct {
    u8 kind, page;
    CsTex *tex;
    CsRes *res;
    float x[4], y[4], u[4], v[4];   /* screen px (640x480), texels */
    u32 argb;
} CsQuad;
/* current view: DS pixel -> screen px = (p - src) * scale + dst */
typedef struct { float sx, sy, scale, dx, dy; float clip_x0, clip_y0, clip_x1, clip_y1; } CsView;
extern CsView g_cs_view;
void cs_view_full(void);                       /* 256x192 at x2.5 = 640x480 */
void cs_view_set(float sx, float sy, float scale, float dx, float dy);
void cs_quads_reset(void);
int cs_quad_count(void);
const CsQuad *cs_quads(void);
/* DS-space rectangle through the view (clipped to the view's clip rect) */
void cs_quad_tex(CsTex *t, float x, float y, float w, float h, float u0, float v0, float u1, float v1, u32 argb);
void cs_quad_flat(float x, float y, float w, float h, u32 argb);
/* affine BG: the DS mapping screen -> texture: tex = m * (scr - center) + center + ofs
 * (G2_SetBGxAffine), drawn as one quad over the texture's extent */
void cs_quad_affine(CsTex *t, const s32 m[4], s32 cx, s32 cy, s32 ofsx, s32 ofsy, u32 argb);
/* full-frame brightness overlay, DS master brightness -16..16 */
extern s16 g_cs_bright, g_cs_bright_sub;
void BrightnessFade_Create(u16 flags, u16 v);
BOOL BrightnessFade_IsDone(void);
void BrightnessFade_Destroy(void);
void BrightnessFade_Main(void);
extern u32 g_cs_backdrop;     /* opaque backdrop for full-screen cutscenes (0 = none) */

/* ---------------------------------------------------------------- 3D */
typedef struct {
    dcm_model_t mdl;
    u8 *blob;
    int ok;
} CsModel;
int cs_model_load(CsModel *m, const char *file);
void cs_model_free(CsModel *m);
typedef struct {
    dca_set_t set;
    u8 *blob;
} CsAnimSet;
int cs_anim_load(CsAnimSet *a, const char *file);
void cs_anim_free(CsAnimSet *a);
typedef struct {
    float eye[3], at[3], up[3];
    u16 fovy;          /* DS angle of the half fovy (G3_Perspective) */
    float aspect, near_, far_;
} CsCam;
/* one model instance as Model_Draw draws it (scale, rot, trans, anims) */
typedef struct {
    CsModel *model;
    const CsAnimSet *jnt, *vis;
    int jnt_idx, vis_idx;
    float frame;           /* Model_Animate frame (loops over the animation) */
    float scale[3], rot[9], trans[3];
    u8 alpha;
    u8 hidden;
} CsInst;
void cs3d_begin(const CsCam *cam, const dcm_lights_t *lights);
void cs3d_add(const CsInst *inst);  /* records the instance for this frame */
void cs3d_draw_list(int pvr_list);
int cs3d_count(void);
void cs3d_reset(void);
/* world matrix of joint `name` (or joint 0) of an instance (camera nodes) */
int cs3d_joint_pos(const CsInst *inst, const char *name, float out[3]);
void cs_lights_from_ds(dcm_lights_t *l, const u16 col[3], const s16 vec[3][3], const CsCam *cam);

/* ---------------------------------------------------------------- sound */
typedef struct NNSSndHandle CsSndHandle;
void cs_snd_bank(int ssar);            /* sys_aNN bank (NNS_SndArcLoadGroup), -1 = restore */
void cs_bgm_play(int seq);             /* Sound_PlaySeq: /cd/music/sNN.wav */
void cs_bgm_play_file(const char *name, int loop); /* STRM / extra_end music */
void cs_bgm_stop(int fade);            /* NNS_SndPlayerStopSeq(bgm, fade) */
void cs_bgm_update(void);
int cs_bgm_playing(void);
void cs_se(int handle, int arc, int id);    /* Sound_PlaySeqArc(handle h, arc, id); handle 0 = SE handle */
void cs_voice(int arc, int id);             /* Sound_PlayVoice(NULL, arc, id) */
void cs_snd_stop(int handle, int fade);     /* NNS_SndPlayerStopSeq(handle, fade) */
int cs_snd_active(int handle);
void cs_snd_stop_all(void);
void cs_snd_shutdown(void);
enum { CSH_SE = 0, CSH_TALK_PAGE = 1, CSH_PD_SE = 2, CSH_PD_REPEAT = 3, CSH_TRANS = 4, CSH_COUNT = 5 };

/* ---------------------------------------------------------------- modules */
/* TalkDemo (cs_talk.c) */
void TalkDemo_Create(void);
void TalkDemo_CreateForZoneMap(void);
BOOL TalkDemo_IsActive(void);
void TalkDemo_RunFrame(void);
void TalkDemo_Draw(void);
void TalkDemo_Kill(void);
int TalkDemo_PageCount(int id);
extern s16 gTalkDemoId;
/* PlayerDemo + PolyDemoUnder2 (cs_player.c) */
void PlayerDemo_Start(void);
BOOL PlayerDemo_IsActive(void);
void PlayerDemo_RunFrame(void);
void PlayerDemo_Draw(void);
void PlayerDemo_Kill(void);
u32 *PlayerDemo_Flags(void);   /* PlayerDemo_GetWork()->flags (NULL when inactive) */
void PlayerDemo_StopBGMForTalk(int seq);
extern s16 gPlayerDemoId;
/* TransformDemo (cs_trans.c) */
void TransformDemo_Start(void);
BOOL TransformDemo_IsActive(void);
void TransformDemo_RunFrame(void);
void TransformDemo_Draw(void);
void TransformDemo_Kill(void);
extern s16 gTransformDemoId;
/* DemoScript (cs_script.c) */
void DemoScript_Create(s32 mode);
BOOL DemoScript_IsActive(void);
void DemoScript_RunFrame(void);
void DemoScript_Kill(void);
void DemoScript_StartAfterBoss(void);
/* Ending / ExtraEnding / StaffRoll / PostStaffRoll (cs_ending.c) */
void Ending_Create(void);
void ExtraEnding_Create(void);
void StaffRoll_Create(s32 mode);
void PostStaffRoll_Start(void);
BOOL Ending_IsActive(void);
void Ending_RunFrame(void);
void Ending_Draw(void);
void Ending_Kill(void);

/* SceneChange_* targets the cutscenes reach (cs_main.c records them) */
void cs_scene_change(CsNext next);
#endif

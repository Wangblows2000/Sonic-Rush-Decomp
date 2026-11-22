/* UI module internals: sprite resources (converted BAC frames.bin), the
 * AnimatorSprite subset the UI needs, ROM tables (uidata.bin), draw commands. */
#ifndef SRDC_UI_INTERNAL_H
#define SRDC_UI_INTERNAL_H

#include <stdint.h>
#include <string.h>
#include "ui.h"
#include "../objects/objects.h"

#define W g_dcp_world   /* gGameState / data_022c4590 fields owned by the player module */
#define G g_ui_game     /* the rest */

/* gGameState.unk1C < 9 && gGameState.unk1D < 2 */
#define UI_NORMAL_STAGE() (W.zone < 9 && W.act < 2)

static inline s32 FX_Mul(s32 a, s32 b) { return (s32)(((int64_t)a * b + 0x800) >> 12); }
/* the inline "Lerp_xxxx" / "Lerp2_xxxx" helpers repeated in the title card and results */
static inline s32 UiLerpN(s32 v, s32 to, s32 t, s32 n) {
    do {
        v += FX_Mul(to - v, t);
    } while (n--);
    return v;
}
static inline s32 UiLerp2N(s32 v, s32 base, s32 t, s32 n) {
    do {
        v = FX_Mul(v - base, t) + base;
    } while (n--);
    return v;
}
/* gRandSeed LCG (Rand) - the player module owns the seed (g_dcp_world.rand_seed) */
static inline u16 UiRand(void) {
    W.rand_seed = W.rand_seed * 0x196225 + 0x3c6ef35f;
    return (u16)(W.rand_seed >> 16);
}

/* ------------------------------------------------------------ resources */
/* DSPR v1 records (tools/dreamcast/sprites/bac2dc.py, SPRITES.md section 4) */
typedef struct { u16 w, h; } UiPage;
typedef struct { u8 page, pad; u16 x, y, w, h; s16 ox, oy; u16 pad2; } UiImage;
typedef struct { u16 first, count; s16 loop, next; u8 fmt, pad; u16 pad2; } UiAnim;
typedef struct { u16 image, duration; } UiFrame;

enum {
    UI_RES_MOJI,        /* act_com file 0 ac_fix_moji: HUD, ScorePopup (Hud_Create)           */
    UI_RES_CLDM,        /* act_com file 0x19 dm_cldm: act results (ClearDemo_Create)          */
    UI_RES_STDM,        /* act_com file 2 dm_stdm: title card (StageTitleCard_Create res[0])  */
    UI_RES_STDM_LANG,   /* act_com file data_02076b74[lang]: dm_stdm_<lang> (res[1])          */
    UI_RES_PAUSE,       /* /act/ac_fix_pause.bac (PauseMenu_Create)                           */
    UI_RES_PAUSE_FRAME, /* /act/ac_fix_pause_frame.bac                                       */
    UI_RES_GAMEOVER,    /* /act/ac_fix_gameover.bac (GameOver_Create)                         */
    UI_RES_COUNT
};
typedef struct UiRes {
    char id[48];
    u8 *blob;          /* frames.bin */
    const UiPage *pages;
    const UiImage *images;
    const UiAnim *anims;
    const UiFrame *frames;
    u16 n_pages, n_images, n_anims, n_frames;
    u16 *pixels[4];    /* atlas pages kept in RAM (KOS: for recoloured variants) */
    void *tex[4];      /* KOS Texture* */
} UiRes;
extern UiRes g_ui_res[UI_RES_COUNT];

/* AnimatorSprite subset (Graphics/Sprite/AnimatorSprite_*) on frames.bin */
enum { UI_SPR_LOOP = 4, UI_SPR_ENDED = 2, UI_SPR_STOPPED = 1 };
typedef struct UiSpr {
    const UiRes *res;
    u16 anim;          /* unk8  */
    u16 frame;
    s32 wait;          /* frame wait (duration << 4) */
    s16 speed;         /* unk2A (0x10 per game frame = 1x) */
    u16 flags;         /* UI_SPR_* (bit 4 loop, 2 ended, 1 stopped) */
    s16 x, y;          /* screen position (AnimatorSprite x/y) */
} UiSpr;
void UiSpr_Init(UiSpr *s, int res, u16 anim, u16 flags);
void UiSpr_SetAnimation(UiSpr *s, u16 anim);         /* AnimatorSprite_SetAnimation */
void UiSpr_ProcessAnimation(UiSpr *s);               /* AnimatorSprite_ProcessAnimation */
const UiImage *UiSpr_Image(const UiSpr *s, int *index);

/* ---------------------------------------------------------- draw commands */
enum {
    UI_SP_SCREEN,   /* DS screen 256x192, mapped to the full 640x480 (x2.5, like the world view) */
    UI_SP_HUD_TL,   /* HUD art, integer scale, anchored to the safe area's top-left     */
    UI_SP_HUD_BL,   /* ... bottom-left (DS y 192 = safe bottom)                         */
    UI_SP_HUD_TR,   /* ... top-right (DS x 256 = safe right)                            */
    UI_SP_WORLD     /* world px (ScorePopup), camera from ui_set_camera                  */
};
enum { UI_VAR_NONE = 0, UI_VAR_GAUGE0 = 1 /* .. +6: white -> gauge_pal[i] */, UI_VAR_DIM = 8 };
enum { UI_FLIP_H = 1, UI_FLIP_V = 2 };
/* AnimatorSprite_Draw / _DrawRotScale: scale 0x1000 = 1.0, about the sprite origin */
void ui_emit(const UiSpr *s, int space, s32 scale_x, s32 scale_y, int flip, u32 argb, int variant);
static inline void ui_emit_spr(const UiSpr *s, int space) { ui_emit(s, space, 0x1000, 0x1000, 0, 0xffffffff, 0); }
/* draw order (stable within a layer): world popups < HUD < title card < results
 * < pause < game over < fade overlay */
enum { UI_LAYER_POPUP, UI_LAYER_HUD, UI_LAYER_TITLE, UI_LAYER_RESULTS, UI_LAYER_PAUSE,
       UI_LAYER_GAMEOVER, UI_LAYER_FADE };
extern int g_ui_layer;
void ui_emit_flat(int space, float x, float y, float w, float h, u32 argb);
void ui_cmd_reset(void);
/* quads emitted between these are dropped (sprites hidden behind a BG the DC lacks) */
void ui_cmd_mark_hidden_begin(void);
void ui_cmd_mark_hidden_end(void);

/* --------------------------------------------------------------- ROM tables */
typedef struct UiData {
    u16 gauge_pal[8];            /* data_ov000_02364d60 */
    u32 time_div[7];             /* data_ov000_02364d70 */
    u32 tb_thr[27][10];          /* *data_020791d0[zone][act] */
    u32 tb_norm[10], tb_boss[10];/* data_02078f90, data_02079008 */
    u32 sb_thr[41][10];          /* *data_02079198[zone][act] (flat zone*2+act) */
    u32 sb_bonus[10];            /* data_02078ef0 */
    u32 rank_boss[4], rank_norm[4]; /* data_02078b48, data_02078b58 */
    u16 lbl_boss[6], lbl_norm[6];/* data_02078b30, data_02078b3c */
    u8 cd_name_s[18], cd_name_b[13]; /* data_020732e8, data_020732b8 */
    u16 cd_gw[20];               /* data_020733dc */
    s32 cd_actx[4];              /* data_020732d8 */
    u8 cd_act_ex[14], cd_act_bs[10], cd_act_1[11], cd_act_2[11];
    s32 cd_model[2][20];         /* data_0207346c: +0x20/+0x28 move step, s16 +0x4C/+0x4E delay */
    u16 tc_char_a[2], tc_char_b[2]; /* data_02076b4c, data_02076b50 */
    u8 tc_text2[6], tc_text1[12];   /* data_02076b54, data_02076b68 */
    u16 tc_lang[6];              /* data_02076b74 */
    u8 tc_gw[16];                /* data_02076b80 */
    s32 tc_p180x[9];             /* data_02076b90 */
    s32 tc_p180y[9][2];          /* data_02076bb4 / data_02076bb8 */
    u8 zone_ord[2][9];           /* data_02074bc4, data_02074bd0 */
    s8 go_str[6][20], go_w[24];  /* data_0207a0cc, data_0207a09c */
    s8 to_str[6][20], to_w[24];  /* data_0207a144, data_0207a0b4 */
    u8 pause_lut[16];            /* data_0207ba24 */
    s8 shake[38];                /* data_0207323c[4] + data_02073240[12] + data_0207324c[22] */
    int ok;
} UiData;
extern UiData g_ui_data;
int ui_data_load(const u8 *buf, u32 size);   /* parses "RUID" v1 */
int ui_load_file(const char *path, u8 **out, u32 *size);
extern char g_ui_dir[96], g_ui_sprite_dir[96];
int ui_res_load(int idx, const char *id);    /* frames.bin (+ atlas on KOS) */
void ui_res_free(int idx);
/* KOS: upload the atlas (ui_draw.c); host: no-op */
int ui_res_upload(UiRes *r);
void ui_res_release(UiRes *r);

/* -------------------------------------------------- shared helpers */
/* ObjShift_Set / _SetFromRef: defined by the objects module (obj_fx.c) */
s32 ObjShift_Set(s32 cur, s32 target, s32 shift, s32 maxStep, s32 minStep);
s32 ObjShift_SetFromRef(s32 cur, s32 target, s32 ref, s32 shift, u16 maxStep, s32 minStep);
s32 Math_LerpFx32(s32 a, s32 b, s32 t);
u8 StoryProgress_GetZoneIndex(u8 v);
int ui_lang(void);   /* the "ClampLang" inline: languages 0..5, else 1 */

/* BrightnessFade (Graphics/Screen) */
void *BrightnessFade_Create(u16 flags, u16 v);
void BrightnessFade_Main(void);
int BrightnessFade_IsDone(void);
void BrightnessFade_Destroy(void);
/* gDisplayMain.brightness / gDisplaySub.brightness (-16 black .. +16 white) */
extern s16 g_ui_bright_main, g_ui_bright_sub;

/* --------------------------------------------------------------- HUD (ui_hud.c) */
void Hud_Create(void);
void Hud_Destroy(void);
void Hud_Main(void);
void Hud_SetLifeCount(u16 v);
void Hud_SetRingCount(s32 v);
void Hud_SetTime(s32 v);
void Hud_SetFlags(s32 v);
s32 Hud_GetFlags(void);
void Hud_SetTensionGauge(s32 v, s32 mode);
void Hud_ToDecimalDigits(u32 *out, u32 v, u32 n);
void Hud_FramesToTimeDigits(u32 *out, u32 t);
s32 Hud_TensionToGaugeValue(s32 v);
void Hud_SetScore(u32 v);            /* native: score counter */
void Hud_SetTensionMax(s16 v);       /* gPlayers[0]->unk3B6 for Hud_DrawTensionGauge */
void Hud_SetOamPriority(u32 prio);
void ScorePopup_Create(s32 x, s32 y, s32 n);
void ui_score_follow(const DcPlayer *p, s32 n);
int ScorePopup_IsActive(void);
void ScorePopup_Main(void);
void ScorePopup_Destroy(void);

/* --------------------------------------------------------- title card, pause, ... */
void StageTitleCard_Create(DcPlayer *p);
void StageTitleCard_Main(void);
void StageTitleCard_Destroy(void);
int StageTitleCard_IsActive(void);
void StageTitleCard_HoldPlayer(DcPlayer *p);  /* fn478 */

void PauseMenu_Create(int alt);
void PauseMenu_Main(const InputState *in);
int PauseMenu_IsActive(void);
void PauseMenu_Destroy(void);

void ClearDemo_Create(void);
void ClearDemo_Main(const InputState *in);
int ClearDemo_IsActive(void);
void ClearDemo_Destroy(void);
u32 ClearDemo_GetTimeBonus(void);
u32 ClearDemo_GetScoreBonus(void);
s16 ClearDemo_GetRank(u32 v);

void GameOverSeq_StartGameOver(void);
void GameOverSeq_StartTimeOver(void);
void GameOverSeq_Main(const InputState *in);
int GameOverSeq_IsActive(void);
void GameOverSeq_Destroy(void);

/* scene exits (ui_state.c) */
void ui_request_exit(int kind);   /* StageExit_Create(cb) -> UI_EXIT with this kind */
void Stage_ExitAfterFail(void);

/* DS pad bits from InputState (gPad: A 1, B 2, SELECT 4, START 8, R 0x10.. UP 0x40, DOWN 0x80) */
u16 ui_pad_bits(u32 b);
extern u16 g_ui_trig, g_ui_held;   /* this frame's DS pad bits (gPad.trg / held) */
void ui_world_to_screen(s32 wx, s32 wy, s16 *sx, s16 *sy); /* Camera_WorldToScreen (DS px) */
int ui_titlecard_player_view(s32 *scale, s32 *x, s32 *y);
#endif

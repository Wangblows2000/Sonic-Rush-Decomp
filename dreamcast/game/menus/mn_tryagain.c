/*
 * TryAgain_* (src/game/Menus/TryAgain/, 31 functions): the menu after a
 * time-attack clear (ClearDemo_GoToNextScene -> SceneChange_ToTryAgain):
 * TRY AGAIN / CHANGE CHARACTER / CHANGE ACT (CHANGE BOSS) / QUIT.
 *
 * DS display (TryAgain_InitDisplay / _InitBackgrounds / _InitSprites):
 *   main: BG0 mnu_time_tryagain_image (character art), BG1 mnu_time_tryagain_bg
 *     (zigzag, scrolls 2 px/frame), sprite: the "TIME ATTACK" plate (slides in)
 *   sub: BG0 mnu_time_tryagain_bg_b (the panel; opens from its middle line with a
 *     per-line BG0VOFS HDMA, TryAgain_InitSplitScroll / _UpdateSplitScroll), BG1 the
 *     zigzag, sprites: the 4 items and the cursor frame
 * Single screen: TryAgain_Compose (MENUS.md 3).
 *
 * DS service                          here
 * HDMA BG0VOFS of the sub screen      the panel as row bands (mn_emit_bg_part), same rows / wrap
 * DISPCNT sub BG0 on at v >= 0x30     the bands appear from that frame
 * gTouchState (item rectangles)       pad only (UP / DOWN + A, the original's pad path)
 * TryAgain_Destructor Touch_Disable,
 *   ObjVram / heap frees              mn_res_free / mn_bg_free
 */
#include <stdlib.h>
#include "mn_internal.h"

typedef struct {
    s32 unk0;            /* timer */
    s32 unk4;            /* selection */
    s32 unk8;            /* frames since the cursor moved */
    MnSpr spr[6];        /* +0xc: plate, cursor, 4 items */
    s32 anim[6];
    MnBgImg *bg[3];      /* +0x5e8: tryagain_bg, tryagain_bg_b, tryagain_image */
    MnRes *res[2];       /* +0x5e0: mnu_time_tryagain, mnu_time_tryagain_<lang> */
    s32 unk5d4;          /* split scroll running (data_02087cdc.reg) */
    s32 unk5d8, unk5dc;  /* split scroll step / speed */
    s32 sub_bg0_on;      /* DISPCNT sub BG0 */
    u32 unk5f4[4];       /* item anims */
    void (*fns[4])(void);
} TryAgainWork;

static TryAgainWork *s_tryagain;

/* ---------------------------------------------------------------- fade */
typedef struct {
    u32 t;
    s16 from, to;
    u32 dur, delay;
} TryAgainFadeWork;

static void TryAgainFadeTask_Main(void) {
    TryAgainFadeWork *w = TASK_WORK(TryAgainFadeWork);
    fx32 a, b;
    s32 v;
    if (w->t < w->delay) {
        w->t++;
        BRIGHT_SUB = w->from;
        BRIGHT_MAIN = BRIGHT_SUB;
        return;
    }
    a = w->from << 12;
    b = w->to << 12;
    a += a >= 0 ? 0x800 : -0x800;
    b += b >= 0 ? 0x800 : -0x800;
    v = (a + FX_Div(FX_Mul((fx32)((w->t - w->delay) << 12), b - a), (fx32)(w->dur << 12))) >> 12;
    if (v < -16)
        v = -16;
    if (v > 16)
        v = 16;
    BRIGHT_SUB = (s16)v;
    BRIGHT_MAIN = BRIGHT_SUB;
    w->t++;
    if (w->t - w->delay >= w->dur) {
        BRIGHT_SUB = w->to;
        BRIGHT_MAIN = BRIGHT_SUB;
        Task_Destroy(gTaskCurrent);
    }
}

static void TryAgain_CreateFadeTask(s32 a, s32 b, s32 c, s32 d) {
    TryAgainFadeWork *w;
    Task_DestroyRange(0x2000, 0x2000);
    if (a < -16)
        a = -16;
    if (a > 16)
        a = 16;
    if (b < -16)
        b = -16;
    if (b > 16)
        b = 16;
    w = (TryAgainFadeWork *)Task_Create(TryAgainFadeTask_Main, NULL, 0, 0x2000, 0x1000, sizeof(TryAgainFadeWork))
            ->work;
    w->t = 0;
    w->from = (s16)a;
    w->to = (s16)b;
    w->dur = (u32)c;
    w->delay = (u32)d;
}

static void TryAgainBgScroll_Main(void) {
    gDisplayMain[1].x += 2;   /* gDisplayMain[2] */
    gDisplaySub[1].x += 2;
}
static void TryAgain_CreateBgScrollTask(void) { Task_Create(TryAgainBgScroll_Main, NULL, 0, 0x3000, 0x1000, 0); }

/* ------------------------------------------------------------ helpers */
static s32 TryAgain_Interpolate(s32 from, s32 to, s32 len, s32 cur, s32 e) {
    s32 t;
    if (e < 0)
        e = 0;
    if (e > 0x2000)
        e = 0x2000;
    if (cur > len)
        cur = len;
    t = FX_Div(cur << 12, len << 12);
    t = FX_Mul(t, e) + FX_Mul(FX_Mul(t, t), 0x1000 - e);
    return from + ((FX_Mul((to - from) << 12, t) + 0x800) >> 12);
}

/* TryAgain_Unk_ov001_022d344c: an act (not a boss) was played */
static BOOL TryAgain_Unk_ov001_022d344c(void) { return gGameState.unk1D <= 1 ? TRUE : FALSE; }

/* the split: sub BG0VOFS = r - 0x60 on lines 0..0x5f, 0x60 - r on 0x60..0xbf */
static s32 s_split_r = 0;
static void TryAgain_InitSplitScroll(TryAgainWork *p) {
    p->sub_bg0_on = 0;   /* DISPCNT sub 0x1200 */
    p->unk5dc = 8;
    p->unk5d4 = 1;
    p->unk5d8 = 0;
    s_split_r = 0;
}
static void TryAgain_UpdateSplitScroll(TryAgainWork *p) {
    s32 v = p->unk5dc * p->unk5d8;
    p->sub_bg0_on = v >= 0x30;
    if (v >= 0x60) {
        gDisplaySub[0].x = 0;
        gDisplaySub[0].y = 0;
        p->unk5d4 = 0;
        return;
    }
    s_split_r = TryAgain_Interpolate(0, 0x60, 0x60, v, 0);
    p->unk5d8++;
}
static BOOL TryAgain_IsSplitScrollDone(TryAgainWork *p) { return p->unk5d4 == 0 ? TRUE : FALSE; }

/* sub BG0 while the HDMA runs: the rows each line shows (256-line plane, the panel
 * image is 192 rows, the rest empty) */
static void TryAgain_DrawSplit(TryAgainWork *p) {
    MnBgImg *img = p->bg[1];
    s32 r = s_split_r;
    g_mn_layer[1][0].visible = (u8)(p->sub_bg0_on && !p->unk5d4);
    if (!img || !p->sub_bg0_on || !p->unk5d4)
        return;
    /* lines 0..0x5f: row (y + r - 0x60) & 0xff */
    if (160 + r < 192)
        mn_emit_bg_part(1, 0, img, 0, 0, 0, (float)(160 + r), 256, (float)(192 - 160 - r));
    mn_emit_bg_part(1, 0, img, 0, (float)(0x60 - r), 0, 0, 256, (float)r);
    /* lines 0x60..0xbf: row (y + 0x60 - r) & 0xff */
    mn_emit_bg_part(1, 0, img, 0, 0x60, 0, (float)(192 - r), 256, (float)r);
    if (r < 32)
        mn_emit_bg_part(1, 0, img, 0, (float)(160 + r), 0, 0, 256, (float)(32 - r));
}

static void spr_set(TryAgainWork *w, s32 i, s32 id) {
    if (w->anim[i] != id) {
        AnimatorSprite_SetAnimation(&w->spr[i], (u16)id);
        w->anim[i] = id;
    }
}

/* ------------------------------------------------------------- exits */
static void TryAgain_ExitToRetry(void) {
    NNS_SndStopSoundAll();   /* SysSound_Release */
    Game_StartTimeAttackStage(-1, -1);
}
static void TryAgain_ExitToPlayerSelect(void) {
    if (gGameState.unk1D < 2)
        PlayerSelectMenu_StartMode(3);   /* SceneChange_ToPlayerSelectMenuMode3 */
    else
        PlayerSelectMenu_StartMode(4);
}
static void TryAgain_ExitToTimeAttackMenuMode0(void) { TimeAttackMenu_StartMode0(); }
static void TryAgain_ExitToTimeAttackMenuMode1(void) { TimeAttackMenu_StartMode1(); }
static void TryAgain_ExitToTitleScreen(void) {
    NNS_SndStopSoundAll();
    TitleScreen_Start();
}

/* ------------------------------------------------------------- states */
static void TryAgain_State_Select(void);
static void TryAgain_State_Confirm(void);
static void TryAgain_State_FadeOut(void);

static void TryAgain_State_Enter(void) {
    TryAgainWork *w = TASK_WORK(TryAgainWork);
    MnSpr *s;
    TryAgain_UpdateSplitScroll(w);
    s = &w->spr[0];
    spr_set(w, 0, 0);
    s->flags &= ~4u;
    s->x = (s16)TryAgain_Interpolate(-0xa0, 0, 0xc, w->unk0, 0x2000);
    s->y = 0;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
    TryAgain_DrawSplit(w);
    w->unk0++;
    if (TryAgain_IsSplitScrollDone(w)) {
        w->unk0 = 0;
        w->unk4 = 0;
        w->unk8 = 7;
        gTaskCurrent->func = TryAgain_State_Select;
    }
}

/* the items: anim unk5f4[i], +1 selected (Select), +2 flashing (Confirm / FadeOut) */
static void TryAgain_DrawItems(TryAgainWork *w, int confirm) {
    MnSpr *s;
    s32 i;
    u16 id;
    s = &w->spr[0];
    spr_set(w, 0, 0);
    s->flags &= ~4u;
    if (confirm == 2) {
        s->x = 0;
        s->y = 0;
    }
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
    for (i = 0; i < 4; i++) {
        if (w->unk4 == i) {
            s = &w->spr[1];
            spr_set(w, 1, (!confirm && w->unk8 < 7) ? 1 : 0);
            s->flags &= ~4u;
            s->x = 0x40;
            s->y = (s16)(i * 0x20 + 0x24);
            AnimatorSprite_ProcessAnimation(s, 0, 0);
            AnimatorSprite_Draw(s);
        }
        s = &w->spr[2 + i];
        id = (u16)w->unk5f4[i];
        if (w->unk4 == i) {
            id = (u16)(id + (confirm ? 2 : 1));
            if (confirm)
                s->flags |= 4;
            else
                s->flags &= ~4u;
        } else {
            s->flags &= ~4u;
        }
        spr_set(w, 2 + i, id);
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_Draw(s);
    }
}

static void TryAgain_State_Select(void) {
    TryAgainWork *w = TASK_WORK(TryAgainWork);
    BOOL done = FALSE;
    TryAgain_DrawItems(w, 0);
    w->unk0++;
    w->unk8++;
    if (gPad.rep & PAD_UP) {
        w->unk4--;
        if (w->unk4 < 0)
            w->unk4 = 3;
        w->unk8 = 0;
        SysSound_PlaySE(4);
    } else if (gPad.rep & PAD_DOWN) {
        w->unk4++;
        if (w->unk4 >= 4)
            w->unk4 = 0;
        w->unk8 = 0;
        SysSound_PlaySE(4);
    } else if (gPad.trg & PAD_A) {
        done = TRUE;
    }
    if (done) {
        SysSound_PlaySE(0);
        w->unk0 = 0;
        gTaskCurrent->func = TryAgain_State_Confirm;
    }
}

static void TryAgain_State_Confirm(void) {
    TryAgainWork *w = TASK_WORK(TryAgainWork);
    TryAgain_DrawItems(w, 1);
    w->unk0++;
    if (w->unk0 >= 0x20) {
        w->unk0 = 0;
        TryAgain_CreateFadeTask(0, 0x10, 0x10, 0);
        gTaskCurrent->func = TryAgain_State_FadeOut;
    }
}

static void TryAgain_State_FadeOut(void) {
    TryAgainWork *w = TASK_WORK(TryAgainWork);
    TryAgain_DrawItems(w, 2);
    w->unk0++;
    if (w->unk0 >= 0x10) {
        void (*fn)(void) = w->fns[w->unk4];
        Task_DestroyByTag(0x1000);
        fn();
    }
}

/* -------------------------------------------------------------- setup */
static void TryAgain_Destructor(void) {
    TryAgainWork *w = TASK_WORK(TryAgainWork);
    s32 i;
    /* Touch_Disable; gSystemFlags &= ~0x400 (HDMA off); TryAgain_Dummy;
     * TryAgain_ReleaseSprites; TryAgain_ReleaseAssets; TryAgain_ResetDisplay */
    w->unk5d4 = 0;
    mn_display_reset();
    for (i = 0; i < 3; i++)
        mn_bg_free(w->bg[i]);
    for (i = 0; i < 2; i++)
        mn_res_free(w->res[i]);
    if (s_tryagain == w)
        s_tryagain = NULL;
}

static TryAgainWork *TryAgain_Create(void) {
    TryAgainWork *w =
        (TryAgainWork *)Task_Create(TryAgain_State_Enter, TryAgain_Destructor, 0, 0x1000, 0x1000, sizeof(TryAgainWork))
            ->work;
    w->unk0 = 0;
    w->unk4 = 0;
    w->unk8 = 0;
    w->unk5f4[0] = 1;
    w->unk5f4[1] = 4;
    w->unk5f4[2] = TryAgain_Unk_ov001_022d344c() ? 7 : 10;
    w->unk5f4[3] = 0xd;
    w->fns[0] = TryAgain_ExitToRetry;
    w->fns[1] = TryAgain_ExitToPlayerSelect;
    w->fns[2] = TryAgain_Unk_ov001_022d344c() ? TryAgain_ExitToTimeAttackMenuMode0 : TryAgain_ExitToTimeAttackMenuMode1;
    w->fns[3] = TryAgain_ExitToTitleScreen;
    return w;
}

/* gTryAgainFileTable[lang] */
static const char *const k_tryagain_lang[6] = {
    "act/mnu_time_tryagain_jpn.bac", "act/mnu_time_tryagain_eng.bac", "act/mnu_time_tryagain_fra.bac",
    "act/mnu_time_tryagain_deu.bac", "act/mnu_time_tryagain_ita.bac", "act/mnu_time_tryagain_spa.bac",
};
static void TryAgain_LoadAssets(TryAgainWork *p) {
    p->res[0] = mn_res_load("act/mnu_time_tryagain.bac");
    p->res[1] = mn_res_load(k_tryagain_lang[mn_lang()]);
    p->bg[0] = mn_bg_load("bg/mnu_time_tryagain_bg.bbg");
    p->bg[1] = mn_bg_load("bg/mnu_time_tryagain_bg_b.bbg");
    p->bg[2] = mn_bg_load("bg/mnu_time_tryagain_image.bbg");
}
static void TryAgain_InitSprites(TryAgainWork *w) {
    s32 i;
    /* TryAgain_InitMainSprite (main, prio 0) / _InitSubSprite (sub, prio 0) */
    AnimatorSprite_Init(&w->spr[0], w->res[1], 0, 0, 0);
    AnimatorSprite_Init(&w->spr[1], w->res[0], 0, 0, 1);
    for (i = 0; i < 4; i++)
        AnimatorSprite_Init(&w->spr[2 + i], w->res[1], (u16)w->unk5f4[0], 0, 1);
    for (i = 0; i < 6; i++)
        w->anim[i] = 0;
    w->anim[2] = w->anim[3] = w->anim[4] = w->anim[5] = (s32)w->unk5f4[0];
}
static void TryAgain_InitBackgrounds(TryAgainWork *p) {
    Background_Show(0, 1, p->bg[0]);
    Background_Show(1, 1, p->bg[0]);
    Background_Show(1, 0, p->bg[1]);
    Background_Show(0, 0, p->bg[2]);
    g_mn_layer[1][0].visible = 0;   /* DISPCNT sub 0x1200 until the split opens */
}

/* One frame: the main screen (zigzag, the character art on the right, the
 * "TIME ATTACK" plate sliding in at the bottom left) at 2.5x; the sub screen's
 * yellow panel with the four items and the cursor (its zigzag left out) at 1.4x
 * in the top left, between the plate and the art (MENUS.md 3). */
static void TryAgain_Compose(void) {
    mn_comp_reset(0);
    mn_comp_full(0);
    mn_comp_add(1, 8, 10, 252, 182, 10, 70, 1.4f, 1 << 1, 0);
}

void TryAgain_Start(void) {
    TryAgainWork *w = TryAgain_Create();
    s_tryagain = w;
    mn_display_reset();   /* TryAgain_InitDisplay */
    TryAgain_Compose();
    g_mn_scene = "TryAgain";
    TryAgain_LoadAssets(w);
    TryAgain_InitSprites(w);
    TryAgain_InitBackgrounds(w);
    TryAgain_CreateBgScrollTask();
    TryAgain_CreateFadeTask(BRIGHT_MAIN, 0, 0x10, 2);
    TryAgain_InitSplitScroll(w);
    SysSound_PlayBGM(0);   /* SysSound_Init + SysSound_PlayBGM(0) */
}

int TryAgain_Selected(void) { return s_tryagain ? s_tryagain->unk4 : -1; }

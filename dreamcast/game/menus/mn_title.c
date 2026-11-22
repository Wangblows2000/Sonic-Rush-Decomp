/*
 * SegaLogo_* (src/game/Menus/SegaLogo/) and TitleScreen_* (Menus/TitleScreen/),
 * plus the unlock notices of PostStaffRoll (Cutscenes/Ending) that the title
 * screen starts. Same states, timers and constants; DS display setup replaced
 * by the native layer state (mn_core.c).
 */
#include <stdlib.h>
#include "mn_internal.h"

/* ================================================================ SegaLogo */
typedef struct {
    s32 unk0;            /* timer */
    s32 bright;          /* +4: brightness at create */
    MnBgImg *res[2];     /* +0x88 */
} SegaLogoWork;

static void SegaLogo_State_Init(void);
static void SegaLogo_State_FadeIn(void);
static void SegaLogo_State_Wait(void);
static void SegaLogo_State_FadeOut(void);
static void SegaLogo_State_End(void);

/* SegaLogo_Interpolate */
s32 SegaLogo_Interpolate(s32 from, s32 to, s32 n, s32 i, fx32 ease) {
    fx32 t;
    if (ease < 0)
        ease = 0;
    if (ease > 0x2000)
        ease = 0x2000;
    if (i > n - 1)
        i = n - 1;
    if (i == 0)
        return from;
    if (i == n - 1)
        return to;
    if (n <= 1)
        return from;
    t = FX_Div(i << 12, (n - 1) << 12);
    t = FX_Mul(t, ease) + FX_Mul(FX_Mul(t, t), 0x1000 - ease);
    return from + ((FX_Mul((to - from) << 12, t) + 0x800) >> 12);
}

/* One 640x480 composition (MENUS.md section 3): both logos side by side on
 * the white the two DS screens share, 1.5x. */
static void SegaLogo_Compose(void) {
    mn_comp_reset(0);
    g_mn_comp.fill = 0xffffffff;
    mn_comp_add(0, 32, 44, 232, 142, 16, 166, 1.5f, 0, 0);    /* PRESENTED BY SEGA */
    mn_comp_add(1, 40, 44, 232, 134, 336, 172, 1.5f, 0, 0);   /* CREATED BY SONIC TEAM */
}

static void SegaLogo_InitDisplay(void) {
    mn_display_reset();
    if (BRIGHT_MAIN == 0x10) {
        BRIGHT_MAIN = 0x10;
        BRIGHT_SUB = 0x10;
    } else {
        BRIGHT_MAIN = -16;
        BRIGHT_SUB = -16;
    }
    SegaLogo_Compose();
    g_mn_scene = "SegaLogo";
}

static void SegaLogo_LoadAssets(SegaLogoWork *p) {
    /* gSegaLogoFileList */
    p->res[0] = mn_bg_load("bg/sega_logo_sega.bbg");
    p->res[1] = mn_bg_load("bg/sega_logo_sonicteam.bbg");
}

static void SegaLogo_ShowBackgrounds(SegaLogoWork *p) {
    Background_Show(0, 3, p->res[0]);
    Background_Show(1, 3, p->res[1]);
}

static void SegaLogo_ReleaseAssets(SegaLogoWork *p) {
    int i;
    for (i = 0; i < 2; i++) {
        mn_bg_free(p->res[i]);
        p->res[i] = NULL;
    }
}

static void SegaLogo_ResetDisplay(void) {
    BRIGHT_MAIN = 0x10;
    BRIGHT_SUB = 0x10;
    mn_display_reset();
}

static void SegaLogo_Destructor(void) {
    SegaLogoWork *w = TASK_WORK(SegaLogoWork);
    SegaLogo_ReleaseAssets(w);
    SegaLogo_ResetDisplay();
}

static SegaLogoWork *SegaLogo_Create(void) {
    SegaLogoWork *w = (SegaLogoWork *)Task_Create(SegaLogo_State_Init, SegaLogo_Destructor, 0, 0x1000, 0,
                                                  sizeof(SegaLogoWork))->work;
    w->unk0 = 0;
    w->bright = BRIGHT_MAIN;
    return w;
}

void SegaLogo_Start(void) {
    SegaLogo_InitDisplay();
    SegaLogo_Create();
    gSaveDataBusy = 0;
}

static void SegaLogo_State_Init(void) {
    SegaLogoWork *w = TASK_WORK(SegaLogoWork);
    w->unk0++;
    if (w->unk0 >= 2) {
        SegaLogo_LoadAssets(w);
        SegaLogo_ShowBackgrounds(w);
        w->unk0 = 0;
        gTaskCurrent->func = SegaLogo_State_FadeIn;
    }
}

static void SegaLogo_State_FadeIn(void) {
    SegaLogoWork *w = TASK_WORK(SegaLogoWork);
    s16 v = (s16)SegaLogo_Interpolate(w->bright, 0, 0x20, w->unk0, 0x1000);
    BRIGHT_MAIN = v;
    BRIGHT_SUB = v;
    w->unk0++;
    if (w->unk0 >= 0x20) {
        w->unk0 = 0;
        BRIGHT_MAIN = 0;
        BRIGHT_SUB = 0;
        gTaskCurrent->func = SegaLogo_State_Wait;
    }
}

static void SegaLogo_State_Wait(void) {
    SegaLogoWork *w = TASK_WORK(SegaLogoWork);
    w->unk0++;
    if (w->unk0 >= 0x78) {
        w->unk0 = 0;
        gTaskCurrent->func = SegaLogo_State_FadeOut;
    }
}

static void SegaLogo_State_FadeOut(void) {
    SegaLogoWork *w = TASK_WORK(SegaLogoWork);
    s16 v = (s16)SegaLogo_Interpolate(0, 0x10, 0x20, w->unk0, 0x1000);
    BRIGHT_MAIN = v;
    BRIGHT_SUB = v;
    w->unk0++;
    if (w->unk0 >= 0x20) {
        w->unk0 = 0;
        BRIGHT_MAIN = 0x10;
        BRIGHT_SUB = 0x10;
        gTaskCurrent->func = SegaLogo_State_End;
    }
}

static void SegaLogo_State_End(void) {
    SegaLogoWork *w = TASK_WORK(SegaLogoWork);
    w->unk0++;
    if (w->unk0 >= 0x1e) {
        w->unk0 = 0;
        Task_DestroyAll();
        /* TitleDemo_Start: the 3D intro (TitleDemo_*) is not part of the port;
         * its end (scene 6 / skip) leads to TitleScreen_Start. */
        TitleScreen_Start();
    }
}

/* ============================================================== TitleScreen */
typedef struct {
    s32 timer;
    MnSpr sprB;          /* +4  resB (title_<lang>.bac) */
    MnSpr sprA;          /* +0x4C resA (title.bac) */
    u32 animB, animA;    /* +0x94, +0x98 */
    MnRes *resA, *resB;  /* +0x15C, +0x160 */
    MnBgImg *bg164, *bg168;
} TitleScreenWork;

/* TitleScreenFadeTask (TitleScreen_CreateFadeTask) */
typedef struct {
    u32 t;
    s16 from, to;
    u32 dur, delay;
} TitleFadeWork;

static void TitleScreenFadeTask_Main(void) {
    TitleFadeWork *w = TASK_WORK(TitleFadeWork);
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

void TitleScreen_CreateFadeTask(s32 a, s32 b, s32 c, s32 d) {
    TitleFadeWork *w;
    Task_DestroyRange(0x2000, 0x2000);
    if (a < -16)
        a = -16;
    if (a > 16)
        a = 16;
    if (b < -16)
        b = -16;
    if (b > 16)
        b = 16;
    w = (TitleFadeWork *)Task_Create(TitleScreenFadeTask_Main, NULL, 0, 0x2000, 0, sizeof(TitleFadeWork))->work;
    w->t = 0;
    w->from = (s16)a;
    w->to = (s16)b;
    w->dur = (u32)c;
    w->delay = (u32)d;
}

/* TitleScreen_PaletteCycle: colours start..start+count of a BG palette cycle
 * A -> C -> B -> C -> A (C = per-component minimum) over `duration` frames, and
 * the BG scrolls 2 px per frame. */
typedef struct {
    s32 timer;
    MnBgImg *img;
    int engine, layer;
    s32 duration, offset, count;
    u16 palA[16], palB[16], palC[16], out[16];
} PalCycleWork;

static void TitleScreen_PaletteCycle_Main(void) {
    PalCycleWork *w = TASK_WORK(PalCycleWork);
    s32 i, half, q, range, t, pos, b;
    BOOL dir, flag;
    const u16 *src, *dst;
    if (w->engine == 0)
        gDisplayMain[w->layer].x += 2;
    else
        gDisplaySub[w->layer].x += 2;
    half = w->duration / 2;
    q = half * 12 / 16;
    range = half - q;
    t = w->timer;
    if (t >= half) {
        t -= half;
        dir = FALSE;
    } else {
        dir = TRUE;
    }
    pos = t < q ? 0 : t - q;
    if (pos < range / 2) {
        flag = TRUE;
        range = range / 2;
    } else {
        flag = FALSE;
        pos -= range / 2;
        range -= range / 2;
    }
    if (dir) {
        src = flag ? w->palA : w->palC;
        dst = flag ? w->palC : w->palB;
    } else {
        src = flag ? w->palB : w->palC;
        dst = flag ? w->palC : w->palA;
    }
    for (i = 0; i < w->count; i++) {
        s32 r, g;
        u16 d;
        r = *src & 0x1f;
        g = (*src >> 5) & 0x1f;
        b = (*src >> 10) & 0x1f;
        d = *dst;
        r += pos * ((d & 0x1f) - r) / range;
        g += pos * (((d >> 5) & 0x1f) - g) / range;
        b += pos * (((d >> 10) & 0x1f) - b) / range;
        w->out[i] = (u16)((r & 0x1f) | ((g & 0x1f) << 5) | ((b & 0x1f) << 10));
        src++;
        dst++;
    }
    mn_bg_set_colors(w->img, w->offset, w->count, w->out);
    w->timer++;
    if (w->timer >= w->duration)
        w->timer = 0;
}

static void TitleScreen_CreatePaletteCycle(MnBgImg *img, int engine, int layer, s32 a, s32 b, s32 count,
                                           s32 extra) {
    PalCycleWork *w;
    s32 i;
    u16 r0, g0, b0, r1, g1, b1;
    if (!img)
        return;
    w = (PalCycleWork *)Task_Create(TitleScreen_PaletteCycle_Main, NULL, 0, 0x3000, 0, sizeof(PalCycleWork))->work;
    w->img = img;
    w->engine = engine;
    w->layer = layer;
    w->duration = extra;
    w->offset = a;
    w->count = count;
    for (i = 0; i < count; i++) {
        w->palA[i] = img->pal[a + i];
        w->palB[i] = img->pal[b + i];
        r0 = w->palA[i] & 0x1f;
        g0 = (w->palA[i] >> 5) & 0x1f;
        b0 = (w->palA[i] >> 10) & 0x1f;
        r1 = w->palB[i] & 0x1f;
        g1 = (w->palB[i] >> 5) & 0x1f;
        b1 = (w->palB[i] >> 10) & 0x1f;
        if (r0 > r1)
            r0 = r1;
        if (g0 > g1)
            g0 = g1;
        if (b0 > b1)
            b0 = b1;
        w->palC[i] = (u16)((b0 << 10) | (r0 | (g0 << 5)));
    }
}

static const char *const s_title_lang_bac[6] = { "act/title_jpn.bac", "act/title_eng.bac", "act/title_fra.bac",
                                                 "act/title_deu.bac", "act/title_ita.bac", "act/title_spa.bac" };

static void TitleScreen_LoadAssets(TitleScreenWork *p) {
    /* gTitleScreenFileTable[lang]: title.bac, title_<lang>.bac, title_logo_eng.bbg
     * (every language), title_logo_base.bbg */
    p->resA = mn_res_load("act/title.bac");
    p->resB = mn_res_load(s_title_lang_bac[mn_lang()]);
    p->bg164 = mn_bg_load("bg/title_logo_eng.bbg");
    p->bg168 = mn_bg_load("bg/title_logo_base.bbg");
}

static void TitleScreen_InitBackgrounds(TitleScreenWork *p) {
    Background_Show(0, 3, p->bg168);
    Background_Show(1, 3, p->bg168);
    Background_Show(0, 2, p->bg164);
}

static void TitleScreen_InitSprites(TitleScreenWork *p) {
    AnimatorSprite_Init(&p->sprA, p->resA, 0, 0, 1);
    p->sprA.flags &= ~0x60000u;
    p->sprA.flags |= 0x40000;
    AnimatorSprite_Init(&p->sprB, p->resB, 0, 0, 1);
    p->sprB.flags &= ~0x60000u;
    p->sprB.flags |= 0x40000;
    p->animA = (u32)-1;
    p->animB = (u32)-1;
}

/* The main screen (logo over the zigzag BG) is the frame at 2.5x; the
 * PRESS START sprite of the other screen sits in the gap between the logo
 * and the copyright line, 2x, without its BG. */
static void TitleScreen_Compose(void) {
    mn_comp_reset(0);
    mn_comp_full(0);
    mn_comp_add(1, 40, 78, 216, 106, 144, 318, 2.0f, 0xf, 0);
}

static void TitleScreen_InitDisplay(void) {
    mn_display_reset();
    TitleScreen_Compose();
    g_mn_scene = "TitleScreen";
}

static void TitleScreen_Destructor(void) {
    TitleScreenWork *p = TASK_WORK(TitleScreenWork);
    NNS_SndStopSoundAll();   /* TitleScreen_ReleaseSound */
    mn_display_reset();      /* TitleScreen_ResetDisplay (BGs off before the images go) */
    mn_res_free(p->resA);
    mn_res_free(p->resB);
    mn_bg_free(p->bg164);
    mn_bg_free(p->bg168);
    p->resA = p->resB = NULL;
    p->bg164 = p->bg168 = NULL;
}

/* TitleScreen_CheckStartPressed: touch inside (0x28..0xd8, 0x50..0x6a) of the
 * bottom screen ("touch screen" caption) or START. DC: START or A. */
static BOOL TitleScreen_CheckStartPressed(void) {
    if (gPad.trg & PAD_A)
        return TRUE;
    if (gPad.trg & PAD_START)
        return TRUE;
    return FALSE;
}

static void TitleScreen_State_Init(void);
static void TitleScreen_State_WaitForStart(void);
static void TitleScreen_State_StartPressed(void);

static TitleScreenWork *TitleScreen_Create(void) {
    TitleScreenWork *w = (TitleScreenWork *)Task_Create(TitleScreen_State_Init, TitleScreen_Destructor, 0, 0x1000, 0,
                                                        sizeof(TitleScreenWork))->work;
    w->timer = 0;
    return w;
}

void TitleScreen_Start(void) {
    if (BRIGHT_MAIN != 0x10 && BRIGHT_MAIN != -0x10) {
        BRIGHT_MAIN = -0x10;
        BRIGHT_SUB = -0x10;
    }
    TitleScreen_Create();
    /* TitleScreen_LoadSound + TitleScreen_PlayMusic */
    Sound_PlaySeq(0);
    gSaveDataBusy = 0;
}

void TitleScreen_StartFromDemo(void) {
    TitleScreen_Create();
    gSaveDataBusy = 0;
}

static void TitleScreen_State_Init(void) {
    TitleScreenWork *w = TASK_WORK(TitleScreenWork);
    w->timer++;
    if (w->timer >= 2) {
        TitleScreen_InitDisplay();
        TitleScreen_LoadAssets(w);
        TitleScreen_InitBackgrounds(w);
        TitleScreen_InitSprites(w);
        TitleScreen_CreateFadeTask(BRIGHT_MAIN, 0, 8, 0);
        TitleScreen_CreatePaletteCycle(w->bg168, 0, 3, 1, 0x11, 4, 0x1e0);
        /* the same BBG is on both engines here: one image, one cycle is enough
         * for the colours; the sub engine scroll is driven separately */
        TitleScreen_CreatePaletteCycle(w->bg168, 1, 3, 1, 0x11, 4, 0x1e0);
        Sound_PlayVoice(0);   /* TitleScreen_PlayTitleVoice: Sound_PlayVoice(0, 0, 5) */
        w->timer = 0;
        gTaskCurrent->func = TitleScreen_State_WaitForStart;
    }
}

static void title_draw(MnSpr *s, u32 *a, u32 anim, BOOL loop) {
    if (*a != anim) {
        *a = anim;
        AnimatorSprite_SetAnimation(s, (u16)anim);
    }
    if (loop)
        s->flags |= SPR_LOOP;
    else
        s->flags &= ~(u32)SPR_LOOP;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}

static void TitleScreen_State_WaitForStart(void) {
    TitleScreenWork *w = TASK_WORK(TitleScreenWork);
    s32 v;
    BOOL done;
    if (w->timer >= 0x44) {
        if ((w->timer - 0x44) % 0x3c < 0x28)
            title_draw(&w->sprB, &w->animB, 0, FALSE);
        title_draw(&w->sprA, &w->animA, 0, FALSE);
    }
    if (w->timer >= 0x4a0) {
        v = 0x10 - (0x4b0 - w->timer);
        if (v > 0x10)
            v = 0x10;
        if (v < -0x10)
            v = -0x10;
        BRIGHT_MAIN = (s16)v;
        BRIGHT_SUB = (s16)v;
    }
    done = FALSE;
    if (w->timer >= 0x44)
        done = TitleScreen_CheckStartPressed();
    w->timer++;
    if (done == TRUE) {
        Sound_PlaySeqArc(0, 0);   /* TitleScreen_PlayStartSound */
        w->timer = 0;
        BRIGHT_MAIN = 0;
        BRIGHT_SUB = 0;
        gTaskCurrent->func = TitleScreen_State_StartPressed;
        return;
    }
    if (w->timer >= 0x4b0) {
        w->timer = 0;
        BRIGHT_MAIN = 0x10;
        BRIGHT_SUB = 0x10;
        Task_DestroyAll();
        /* TitleScreen_StartAttractDemo */
        gGameState.unk1E++;
        if (gGameState.unk1E >= 4)
            gGameState.unk1E = 0;
        mn_request_demo();
    }
}

static void PostStaffRoll_ShowUnlockNotices(void);

static void TitleScreen_State_StartPressed(void) {
    TitleScreenWork *w = TASK_WORK(TitleScreenWork);
    BOOL flag;
    title_draw(&w->sprB, &w->animB, 1, TRUE);
    title_draw(&w->sprA, &w->animA, 1, TRUE);
    if ((gSaveData.unkC >= 0x1a || gSaveData.unk64 >= 0x1c) &&
        (!(gSaveData.unk8 & 0x100) || !(gSaveData.unk8 & 0x200)))
        flag = FALSE;
    else
        flag = TRUE;
    if (w->timer == 0x10) {
        if (flag)
            TitleScreen_CreateFadeTask(0, 0x10, 0x10, 0);
        else
            TitleScreen_CreateFadeTask(0, -0x10, 0x10, 0);
    }
    w->timer++;
    if (w->timer >= 0x20) {
        w->timer = 0;
        if (flag) {
            BRIGHT_MAIN = 0x10;
            BRIGHT_SUB = 0x10;
        } else {
            BRIGHT_MAIN = -0x10;
            BRIGHT_SUB = -0x10;
        }
        Task_DestroyAll();
        Pad_Reset(&gPad, NULL, NULL);
        if (!flag) {
            PostStaffRoll_ShowUnlockNotices();   /* TitleScreen_GoToUnlockNotice */
            return;
        }
        TitleMenu_Start();   /* TitleScreen_GoToTitleMenu */
    }
}

/* ===================================================== unlock notices */
/* PostStaffRoll_ShowPlayerSelectUnlockNotice / ..SoundTest..: a message box
 * (MsgBox 0x14 / 0x15) closes at frame 0xf0, then the flag is set and saved.
 * The message box text art is not converted: a dark screen stands for it and
 * A/START closes it early (the DS box closes on its own). */
typedef struct {
    s32 timer;
    s32 which;   /* 0 player select page, 1 sound test */
} UnlockWork;

static void PostStaffRollFade_Main(void);
static void PostStaffRoll_State_UnlockNotice(void) {
    UnlockWork *w = TASK_WORK(UnlockWork);
    if (w->timer == 0xa)
        SysSound_PlaySE(0x15);
    if (w->timer == 0x28)
        SysSound_PlaySE(0x16);
    w->timer++;
    BRIGHT_MAIN = BRIGHT_SUB = -8;
    if (w->timer >= 0xf0 || (w->timer > 0x28 && (gPad.trg & (PAD_A | PAD_START)))) {
        if (w->which == 0) {
            SaveData_UnlockPlayerSelectPage();
            SaveData_CommitOrShowError(NULL);
            w->which = 1;
            w->timer = 0;
            return;
        }
        SaveData_UnlockSoundTest();
        SaveData_CommitOrShowError(NULL);
        gTaskCurrent->func = PostStaffRollFade_Main;
        w->timer = 0;
    }
}

/* PostStaffRoll_State_GoToSegaLogo: 0xb4 frames, then SceneChange_ToSegaLogo */
static void PostStaffRollFade_Main(void) {
    UnlockWork *w = TASK_WORK(UnlockWork);
    BRIGHT_MAIN = BRIGHT_SUB = -16;
    w->timer++;
    if (w->timer >= 0xb4) {
        Task_Destroy(gTaskCurrent);
        SegaLogo_Start();
    }
}

static void PostStaffRoll_ShowUnlockNotices(void) {
    UnlockWork *w;
    mn_display_reset();
    mn_comp_reset(0);
    g_mn_comp.fill = 0xff000000;
    g_mn_scene = "UnlockNotice";
    w = (UnlockWork *)Task_Create(PostStaffRoll_State_UnlockNotice, NULL, 0, 0x1000, 0, sizeof(UnlockWork))->work;
    w->which = (gSaveData.unk8 & 0x100) ? 1 : 0;
    gSaveDataBusy = 0;
}

/*
 * TitleDemo_* (src/game/Menus/TitleScreen/TitleDemo_*, 52 functions): the intro between the SEGA logo
 * and the title screen.  Six scenes (TitleDemo_State_Scene1..6, 258 frames):
 *   1  (0x20)  white; the bottom screen fades in from white with "SONIC" (title_demo_eng anim 0)
 *   2  (0x48)  Blaze's picture (main BG3 title_demo_image_blz) scrolls up (0 -> -0x20 -> -0x40) while the
 *              top screen fades in from white and out again; the blue band (title_demo anim 1) slides in
 *              from x 0x100 at 0x38; SE 1 at 0, SE 2 at 0x38
 *   3  (0x48)  the same with Sonic's picture (main BG2) and his blue band (anim 0)
 *   4  (0x20)  "RUSH" (sub BG3 title_demo_rush_spin_eng, affine around (0xa8, 0x68)) spins in, scale
 *              0x800 -> 0x1000, 2.5 turns -> 0; Sonic (son.dcm, menu_ex anim 1, mirrored, scale 14,
 *              z -120) and Blaze (blz.dcm, menu_ex anim 3, scale 13.125, z -80) slide from x +-0x80 to
 *              +-0x28 at y -0x40 under an ortho camera; the top screen fades in; SE 3 at 0
 *   5  (10)    the models' animations start; "RUSH" becomes the sprite (title_demo_eng anim 2: shine
 *              for 16 frames, then 1); SE 4 at 0
 *   6  (0x28)  both screens fade to white from 0x18, then TitleScreen_StartFromDemo (the music goes on)
 * START or A (TitleDemo_CheckSkipInput; the touch screen is not mapped) fades both screens to white
 * over 0x20 frames and goes to the title screen.  The music is Sound_PlaySeq(0) at the start.
 *
 * Single screen (MENUS.md section 3): the top screen (pictures, models) at x1.875 across the top
 * 480x360 of the frame, the logo part of the bottom screen ((8, 24)-(248, 128): the bands, SONIC,
 * RUSH) at x1.15 under it; white around them (both screens' BG1 is a white plane).  The two screens'
 * brightnesses are kept apart (MnComp.split_bright): the top screen fades while the logo stays.
 * Not done: the DS edge marking (outline) of the models; the affine RUSH is not clipped to the
 * bottom screen while it is magnified.
 */
#include <stdlib.h>
#include "mn_internal.h"

typedef struct {
    s32 timer;            /* +0 (the Action functions' step) */
    s32 state, val;       /* +8 / +0xc: skip requested, the white it fades to */
    s32 timer2;           /* the skip fade */
    s32 scene;
    MnRes *res0, *res1;   /* act/title_demo.bac, act/title_demo_eng.bac */
    MnBgImg *img_son, *img_blz, *spin;
    MnSpr spr[4];         /* 0 blue band (anim 0, palette 0), 1 purple band (anim 1, palette 1), 2 SONIC, 3 RUSH */
    s32 anim[4];
    s32 rush_timer;       /* TitleDemo_Unk_ov001_022d5a18's own timer */
    s32 model_frame;      /* Model_Animate of both models (stopped until scene 5) */
    BOOL models_run;
} TitleDemoWork;

static void TitleDemo_State(void);

/* TitleDemo_Interpolate: from -> to over n steps, i clamped; ease 0..0x2000 (0x1000 linear, 0 ease-in
 * t^2, 0x2000 ease-out 2t - t^2) */
s32 TitleDemo_Interpolate(s32 from, s32 to, s32 n, s32 i, fx32 ease) {
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
    t = (fx32)(((s64)t * ease + 0x800) >> 12) + (fx32)(((s64)(fx32)(((s64)t * t + 0x800) >> 12) * (0x1000 - ease) + 0x800) >> 12);
    return from + (s32)(((((s64)((to - from) << 12) * t + 0x800) >> 12) + 0x800) >> 12);
}

static void td_bright(TitleDemoWork *w, int engine, s32 v) {   /* TitleDemo_SetMain/SubBrightness */
    if (v < -0x10)
        v = -0x10;
    if (v > 0x10)
        v = 0x10;
    if (w->state != 0 && v < w->val)
        v = w->val;
    g_mn_bright[engine] = (s16)v;
}

static void td_spr(TitleDemoWork *w, int k, s32 anim, s32 x) {
    MnSpr *s = &w->spr[k];
    if (w->anim[k] != anim) {
        w->anim[k] = anim;
        AnimatorSprite_SetAnimation(s, (u16)anim);
    }
    s->x = (s16)x;
    s->y = 0;
    s->flags &= ~(u32)SPR_LOOP;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}
/* TitleDemo_Unk_ov001_022d5be4 / 5b44 / 5b94 / 5a18 / 5af0 */
static void td_sonic(TitleDemoWork *w) { td_spr(w, 2, 0, 0); }
static void td_purple(TitleDemoWork *w, s32 x) { td_spr(w, 1, 1, x); }   /* Blaze's band */
static void td_blue(TitleDemoWork *w, s32 x) { td_spr(w, 0, 0, x); }     /* Sonic's band */
static void td_rush_shine(TitleDemoWork *w) {
    s32 id = w->rush_timer >= 0x10 ? 1 : 2;
    if (w->anim[3] != id)
        w->rush_timer = 0;
    td_spr(w, 3, id, 0);
    w->rush_timer++;
}
static void td_rush(TitleDemoWork *w) {
    td_spr(w, 3, 1, 0);
    w->rush_timer = 0;
}

/* TitleDemo_ShowBg2At / ShowBg3At: the picture shown, scrolled */
static void td_show_bg(int layer, MnBgImg *img, s32 y) {
    Background_Show(0, layer, img);
    gDisplayMain[layer].x = 0;
    gDisplayMain[layer].y = (u16)(-y);
}

/* TitleDemo_SetOrthoProjection + SetupCamera + SetupModelLighting + DrawSonicModel / DrawBlazeModel */
static void td_model(TitleDemoWork *w, int chara, s32 x, s32 y) {
    MnModelDesc d;
    memset(&d, 0, sizeof(d));
    d.mdl = chara;
    d.set = 2;                         /* menu_ex.narc */
    d.jidx = d.vidx = chara ? 3 : 1;
    d.frame = w->model_frame << 12;
    d.t = 95.0f;
    d.b = -96.0f;
    d.l = -128.0f;
    d.r = 127.0f;
    d.n = 1.0f;
    d.f = 200.0f;
    d.at[2] = -50.0f;                  /* LookAt (0, 0, 0) -> (0, 0, -50) */
    d.pos[0] = (float)x;
    d.pos[1] = (float)y;
    d.pos[2] = chara ? -80.0f : -120.0f;
    if (chara) {
        d.scale[0] = d.scale[1] = d.scale[2] = 0xd200 / 4096.0f;
        d.cull = 2;
        d.lvec[0][0] = -1672; d.lvec[0][1] = -1672; d.lvec[0][2] = -3344;   /* VEC_Fx16Normalize(-1, -1, -2) */
    } else {
        d.scale[0] = -0xe000 / 4096.0f;   /* mirrored */
        d.scale[1] = d.scale[2] = 0xe000 / 4096.0f;
        d.cull = 1;
        d.lvec[0][0] = 1672; d.lvec[0][1] = -1672; d.lvec[0][2] = -3344;
    }
    d.diff = 0x7fff;
    d.amb = 0x6f7b;
    d.nlights = 1;
    d.lcol[0] = 0x7fff;                /* data_ov001_02316154[0] */
    mn_3d_add(&d);
}

/* white on both screens, Task_DestroyAll, TitleDemo_SkipToTitleScreen.  This frame's quads go first:
 * they draw from the files the destructor frees (the frame is white anyway) */
static void td_end(void) {
    BRIGHT_MAIN = BRIGHT_SUB = 0x10;
    mn_quads_reset();
    Task_DestroyAll();
    TitleScreen_StartFromDemo();
}

static void td_skip_check(TitleDemoWork *w, BOOL allowed) {   /* TitleDemo_CheckSkipInput */
    if (w->state == 0 && allowed && (gPad.trg & (PAD_A | PAD_START))) {
        w->state = 1;
        w->val = 0;
    }
}
/* the skip fade: 1 when the title screen has been started */
static BOOL td_skip_fade(TitleDemoWork *w) {
    if (w->state != 1)
        return FALSE;
    if (w->timer2 >= 0)
        w->val = TitleDemo_Interpolate(0, 0x10, 0x20, w->timer2, 0x1000);
    w->timer2++;
    if (w->timer2 >= 0x20) {
        td_end();
        return TRUE;
    }
    return FALSE;
}

static void td_next(TitleDemoWork *w) {   /* TitleDemo_Action_SceneN: the step restarts */
    w->scene++;
    w->timer = 0;
}

/* the scene's planes (the Action functions' DISPCNT): main BG1 (white) and the sub BG1 / OBJ are
 * the white fill and the sprites here */
static void td_planes(int scene) {
    Background_Hide(0, 2);
    Background_Hide(0, 3);
    Background_Hide(1, 3);
    g_mn_3d.on = scene >= 4;
    g_mn_3d.engine = 0;
}

static void td_scene(TitleDemoWork *w) {
    s32 t, v;
    switch (w->scene) {
    case 1:
        td_sonic(w);
        if (w->timer >= 0) {
            td_bright(w, 1, TitleDemo_Interpolate(0x10, 0, 0x20, w->timer, 0x1000));
            td_bright(w, 0, 0x10);
        }
        td_skip_check(w, w->timer > 0);
        if (td_skip_fade(w))
            return;
        if (++w->timer >= 0x20) {
            td_bright(w, 1, 0);
            td_bright(w, 0, 0x10);
            td_next(w);
            td_planes(2);
        }
        break;
    case 2:
    case 3:
        td_sonic(w);
        if (w->scene == 3)
            td_purple(w, 0);
        t = w->timer - 0x38;
        if (t >= 0) {
            v = TitleDemo_Interpolate(0x100, 0, 0x10, t, 0x2000);
            if (w->scene == 2)
                td_purple(w, v);
            else
                td_blue(w, v);
            if (t == 0)
                Sound_PlaySeqArc(0, 2);   /* TitleDemo_PlaySeq2 */
        }
        t = w->timer;
        v = t < 0x38 ? TitleDemo_Interpolate(0, -0x20, 0x38, t, 0x1800) : TitleDemo_Interpolate(-0x20, -0x40, 0x10, t - 0x38, 0x800);
        if (w->scene == 2)
            td_show_bg(3, w->img_blz, v);
        else
            td_show_bg(2, w->img_son, v);
        if (t < 0x38) {
            td_bright(w, 1, 0);
            td_bright(w, 0, TitleDemo_Interpolate(0x10, 0, 0x18, t, 0x1000));
            if (t == 0)
                Sound_PlaySeqArc(0, 1);   /* TitleDemo_PlaySeq1 */
        } else {
            td_bright(w, 1, 0);
            td_bright(w, 0, TitleDemo_Interpolate(0, 0x10, 0x10, t - 0x38, 0x1000));
        }
        td_skip_check(w, TRUE);
        if (td_skip_fade(w))
            return;
        if (++w->timer >= 0x48) {
            td_bright(w, 1, 0);
            td_bright(w, 0, 0x10);
            td_next(w);
            td_planes(w->scene);
        }
        break;
    case 4:
        td_sonic(w);
        td_purple(w, 0);
        td_blue(w, 0);
        t = w->timer;
        mn_emit_bg_affine(1, 0, w->spin, 0xa8, 0x68, TitleDemo_Interpolate(0x800, 0x1000, 0x20, t, 0x2000),
                          TitleDemo_Interpolate(0x28000, 0, 0x20, t, 0x2000));
        if (t == 0)
            Sound_PlaySeqArc(0, 3);   /* TitleDemo_PlaySeq3 */
        td_bright(w, 1, 0);
        td_bright(w, 0, TitleDemo_Interpolate(0x10, 0, 0x20, t, 0x1000));
        v = TitleDemo_Interpolate(0x80, 0x28, 0x20, t, 0x2000);
        td_model(w, 0, v, -0x40);
        td_model(w, 1, -v, -0x40);
        td_skip_check(w, TRUE);
        if (td_skip_fade(w))
            return;
        if (++w->timer >= 0x20) {
            td_bright(w, 1, 0);
            td_bright(w, 0, 0);
            td_next(w);
            td_planes(5);
            w->models_run = TRUE;   /* TitleDemo_Action_Scene5: the models' stop flags cleared */
        }
        break;
    case 5:
    case 6:
        td_sonic(w);
        td_purple(w, 0);
        td_blue(w, 0);
        if (w->scene == 5) {
            if (w->timer >= 0)
                td_rush_shine(w);
            if (w->timer == 0)
                Sound_PlaySeqArc(0, 4);   /* TitleDemo_PlaySeq4 */
        } else {
            td_rush(w);
            if (w->timer - 0x18 >= 0) {
                v = TitleDemo_Interpolate(0, 0x10, 0x10, w->timer - 0x18, 0x1000);
                td_bright(w, 1, v);
                td_bright(w, 0, v);
            } else {
                td_bright(w, 1, 0);
                td_bright(w, 0, 0);
            }
        }
        td_model(w, 0, 0x28, -0x40);
        td_model(w, 1, -0x28, -0x40);
        td_skip_check(w, TRUE);
        if (td_skip_fade(w))
            return;
        if (w->scene == 5) {
            td_bright(w, 1, 0);
            td_bright(w, 0, 0);
            if (++w->timer >= 10) {
                td_next(w);
                td_planes(6);
            }
        } else if (++w->timer >= 0x28) {
            td_end();
            return;
        }
        break;
    }
}

static void TitleDemo_State(void) {
    TitleDemoWork *w = TASK_WORK(TitleDemoWork);
    if (w->models_run && w->scene >= 5)
        w->model_frame++;   /* Model_Animate in DrawSonicModel / DrawBlazeModel (once per frame for both) */
    td_scene(w);
}

static void TitleDemo_Destructor(void) {
    TitleDemoWork *w = TASK_WORK(TitleDemoWork);
    mn_display_reset();   /* TitleDemo_ResetDisplay */
    g_mn_3d.on = 0;
    mn_3d_free();         /* TitleDemo_ReleaseModels */
    mn_res_free(w->res0);
    mn_res_free(w->res1);
    mn_bg_free(w->img_son);
    mn_bg_free(w->img_blz);
    mn_bg_free(w->spin);
}

/* the single-screen composition (see the top) */
static void TitleDemo_Compose(void) {
    mn_comp_reset(1);
    g_mn_comp.fill = 0xffffffff;
    g_mn_comp.split_bright = 1;
    mn_comp_add(0, 0, 0, 256, 192, 80, 0, 1.875f, 1 << 1, 0);
    mn_comp_add(1, 8, 24, 248, 128, 320 - 120 * 1.15f, 360, 1.15f, (1 << 0) | (1 << 1) | (1 << 2), 0);
}

void TitleDemo_Start(void) {
    TitleDemoWork *w = (TitleDemoWork *)Task_Create(TitleDemo_State, TitleDemo_Destructor, 0, 0x1000, 0,
                                                    sizeof(TitleDemoWork))->work;
    int k;
    memset(w, 0, sizeof(*w));
    /* TitleDemo_InitDisplay: both screens white (master brightness 0x10), BG2 priority 1, BG3 2 */
    mn_display_reset();
    Background_SetPriority(0, 0, 0);
    Background_SetPriority(0, 2, 1);
    Background_SetPriority(0, 3, 2);
    Background_SetSize(0, 2, 256, 256);
    Background_SetSize(0, 3, 256, 256);
    BRIGHT_MAIN = BRIGHT_SUB = 0x10;
    TitleDemo_Compose();
    g_mn_scene = "TitleDemo";
    /* TitleDemo_LoadAssets: gTitleDemoFileTable[lang] (every language: the _eng files) */
    w->res0 = mn_res_load("act/title_demo.bac");
    w->res1 = mn_res_load("act/title_demo_eng.bac");
    w->img_son = mn_bg_load("bg/title_demo_image_son.bbg");
    w->img_blz = mn_bg_load("bg/title_demo_image_blz.bbg");
    w->spin = mn_bg_load("bg/title_demo_rush_spin_eng.bbg");
    /* TitleDemo_InitSprites: sub OBJ, priority 1 */
    AnimatorSprite_Init(&w->spr[0], w->res0, 0, 0, 1);
    AnimatorSprite_Init(&w->spr[1], w->res0, 1, 0, 1);
    AnimatorSprite_Init(&w->spr[2], w->res1, 0, 0, 1);
    AnimatorSprite_Init(&w->spr[3], w->res1, 1, 0, 1);
    for (k = 0; k < 4; k++) {
        static const u8 oam[4] = {0xc, 0x10, 8, 4};   /* TitleDemo_InitSubSprite's unk46 (oamPrio) */
        w->spr[k].flags = (w->spr[k].flags & ~SPR_PRIO_MASK) | (1u << 17);
        w->spr[k].oam = oam[k];
        w->anim[k] = k == 1 || k == 3 ? 1 : 0;
    }
    w->scene = 1;   /* TitleDemo_Action_Scene1 */
    td_planes(1);
    Sound_PlaySeq(0);   /* TitleDemo_PlayMusic: Sound_PlaySeq(0, 0) */
    gSaveDataBusy = 0;
}

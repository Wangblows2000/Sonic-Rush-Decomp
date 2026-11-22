/*
 * PlayerSelectMenu_* (src/game/Menus/PlayerSelect/, overlay ov000): Sonic /
 * Blaze (/ extra) choice before the story (mode 0) and time attack (3, 4).
 * VS modes 1/2 (wireless partner sync) are not part of the port.
 *
 * Kept: the state machine (Opening, Intro, Select, ScrollOut/ScrollIn,
 * Confirm/Confirmed, FadeOutBack), its timers, key-frame positions
 * (ps_act.ama), the character illustration BG on the top screen with its
 * scroll, the sprites, voices and the exits (ExitToGame / ExitBack).
 * Not drawn: the 3D character models of the bottom screen
 * (PlayerSelectMenu_*Model*, camera orbit) and the palette animation task.
 * Touch -> pad: OK button = A, arrows = LEFT/RIGHT (both also on the pad in
 * the original), back button = B.
 */
#include <math.h>
#include <stdlib.h>
#include "mn_internal.h"

typedef struct {
    s32 unk0, unk4, unk8;
    s32 unkC;              /* mode */
    s32 unk10;             /* number of choices (2, 3 when fully unlocked) */
    s32 unk14;             /* choice: 0 Sonic, 1 Blaze, 2 extra */
    s32 unk18, unk1C;      /* next / previous choice */
    s32 unk20;             /* scroll direction (1 = right) */
    s32 unk24;
    s32 unk32C;            /* BG shown */
    s32 unk330, unk334, unk338, unk33C, unk340;
    KeyframeAnim posB68;
    KeyframeAnim pos[8];
    MnSpr spr[19];         /* +0x5c4 */
    s32 anim[19];          /* +0xb1c */
    MnBgImg *bg[10];       /* bis_son/blz/extra, scname x3, base_b, base_a, circle_ext, circle_text */
    MnRes *res_psel, *res_lang, *res_titl;
    void *ama;
} PselWork;

static void PlayerSelectMenu_Main(void);
static void PlayerSelectMenu_State_Intro(void);
static void PlayerSelectMenu_State_Select(void);
static void PlayerSelectMenu_State_ScrollOut(void);
static void PlayerSelectMenu_State_ScrollIn(void);
static void PlayerSelectMenu_State_Confirmed(void);
static void PlayerSelectMenu_State_FadeOutBack(void);

#define PW TASK_WORK(PselWork)

static BOOL PlayerSelectMenu_IsFullyUnlocked(void) {
    return gSaveData.unkC >= 0x1a && gSaveData.unk64 >= 0x1c && (gSaveData.unkF & 0x7f) == 0x7f;
}

static s32 PlayerSelectMenu_EaseValue(s32 a, s32 b, s32 n, s32 m, s32 t) {
    fx32 r;
    if (t < 0)
        t = 0;
    if (t > 0x2000)
        t = 0x2000;
    if (m > n)
        m = n;
    r = FX_Div(m << 12, n << 12);
    r = FX_Mul(r, t) + FX_Mul(FX_Mul(r, r), 0x1000 - t);
    return a + (FX_Mul((b - a) << 12, r) >> 12);
}

/* PlayerSelectMenu_StartBrightnessFade / _BrightnessFade_Main */
typedef struct { s32 t, from, to, dur, delay; } PselFade;
static void PlayerSelectMenu_BrightnessFade_Main(void) {
    PselFade *w = TASK_WORK(PselFade);
    s32 v;
    if (w->t < w->delay) {
        w->t++;
        return;
    }
    v = w->from + (w->to - w->from) * (w->t - w->delay) / (w->dur > 0 ? w->dur : 1);
    BRIGHT_MAIN = BRIGHT_SUB = (s16)v;
    w->t++;
    if (w->t - w->delay >= w->dur) {
        BRIGHT_MAIN = BRIGHT_SUB = (s16)w->to;
        Task_Destroy(gTaskCurrent);
    }
}
static void PlayerSelectMenu_StartBrightnessFade(s32 from, s32 to, s32 dur, s32 delay) {
    PselFade *w;
    Task_DestroyRange(0x3000, 0x3000);
    w = (PselFade *)Task_Create(PlayerSelectMenu_BrightnessFade_Main, NULL, 0, 0x3000, 0x1000, sizeof(PselFade))->work;
    w->from = from;
    w->to = to;
    w->dur = dur;
    w->delay = delay;
}

/* ---------------------------------------------------------- sprites */
static void spr_set(PselWork *w, int i, s32 anim) {
    if (w->anim[i] != anim) {
        AnimatorSprite_SetAnimation(&w->spr[i], (u16)anim);
        w->anim[i] = anim;
    }
}
static void spr_draw_at(PselWork *w, int i, s32 x, s32 y) {
    w->spr[i].x = (s16)x;
    w->spr[i].y = (s16)y;
    AnimatorSprite_ProcessAnimation(&w->spr[i], 0, 0);
    AnimatorSprite_Draw(&w->spr[i]);
}

static void SetSprite60CAnim3(PselWork *w) {   /* left arrow */
    AnimatorSprite_SetAnimation(&w->spr[1], 3);
    AnimatorSprite_ProcessAnimation(&w->spr[1], 0, 0);
    w->spr[1].flags &= ~(u32)SPR_LOOP;
    w->spr[1].flags |= SPR_STOPPED;
}
static void SetSprite60CAnim4(PselWork *w) {
    AnimatorSprite_SetAnimation(&w->spr[1], 4);
    w->spr[1].flags &= ~(u32)SPR_LOOP;
}
static void SetSprite654Anim3(PselWork *w) {   /* right arrow */
    AnimatorSprite_SetAnimation(&w->spr[2], 3);
    AnimatorSprite_ProcessAnimation(&w->spr[2], 0, 0);
    w->spr[2].flags &= ~(u32)SPR_LOOP;
    w->spr[2].flags |= SPR_STOPPED | SPR_HFLIP;
}
static void SetSprite654Anim4(PselWork *w) {
    AnimatorSprite_SetAnimation(&w->spr[2], 4);
    w->spr[2].flags &= ~(u32)SPR_LOOP;
    w->spr[2].flags |= SPR_HFLIP;
}
static void SetSprite9B4Anim(PselWork *w, BOOL confirm) {   /* character name */
    s32 a = w->unk14 * 2 + (confirm ? 3 : 2);
    if (w->anim[14] != a) {
        AnimatorSprite_SetAnimation(&w->spr[14], (u16)a);
        if (confirm)
            w->spr[14].flags |= SPR_LOOP;
        else
            w->spr[14].flags &= ~(u32)SPR_LOOP;
        w->anim[14] = a;
    }
}
static void SetSprite804(PselWork *w, BOOL confirm) {   /* OK button */
    AnimatorSprite_SetAnimation(&w->spr[8], (u16)(3 * w->unk14 + (confirm ? 0xa : 9)));
    w->spr[8].flags |= SPR_LOOP;
    AnimatorSprite_ProcessAnimation(&w->spr[8], 0, 0);
}
static void SetSpriteA8C(PselWork *w, s32 a) {   /* back button */
    spr_set(w, 17, a);
    w->spr[17].flags &= ~(u32)SPR_LOOP;
}
static void SetSprite5C4(PselWork *w, s32 a) {   /* top screen title */
    if (a == 1 && w->anim[0] != 1 && w->unk14 >= 0 && w->unk14 <= 2) {
        /* SetSprite5C4Anim1: the name banner (scname BBG) on sub BG2 */
        Background_Show(1, 2, w->bg[3 + w->unk14]);
        gDisplaySub[2].x = 0;
        gDisplaySub[2].y = 0x20;
    }
    spr_set(w, 0, a);
    w->spr[0].flags &= ~(u32)SPR_LOOP;
    if (a != 1)
        gDisplaySub[2].y = (u16)-8;
}

/* PlayerSelectMenu_SetBackgroundSelection: the illustration on sub BG0 */
static void PlayerSelectMenu_SetBackgroundSelection(PselWork *w, s32 sel, s32 scroll) {
    if (sel != w->unk32C) {
        if (sel >= 0 && sel <= 2)
            Background_Show(1, 0, w->bg[sel]);
        w->unk32C = sel;
    }
    if (sel == 4) {
        gDisplaySub[0].x = 0x100;
    } else {
        scroll = -scroll;
        if (scroll < 0)
            scroll += ((-scroll) / 0x200 + 1) << 9;
        scroll = scroll % 0x200;
        gDisplaySub[0].x = (u16)scroll;
    }
}

static void DrawOrbitSprite(PselWork *w, s32 n, s32 ang) {
    /* icons of the other characters around the selection circle */
    float a = (u16)ang * (6.28318530718f / 65536.0f);
    s32 x = (s32)(sinf(a) * 0x41) + 1 + 0x80, y = (s32)(cosf(a) * 0x41) + 1 + 0x58;
    MnSpr *s = &w->spr[n + 5];
    s->x = (s16)x;
    s->y = (s16)y;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}

static void draw_orbit(PselWork *w, s32 base, s32 first) {
    s32 i, step = w->unk10 == 2 ? 0x8000 : 0x5555;
    for (i = first; i < w->unk10; i++)
        DrawOrbitSprite(w, (w->unk14 + i) % w->unk10, base + step * i);
}

static void draw_common(PselWork *w, BOOL a8c) {
    spr_draw_at(w, 1, w->pos[0].x, w->pos[0].y);
    spr_draw_at(w, 2, w->pos[1].x, w->pos[1].y);
    spr_draw_at(w, 3, w->pos[2].x, w->pos[2].y);
    spr_draw_at(w, 4, w->pos[3].x, w->pos[3].y);
    spr_draw_at(w, 14, w->pos[4].x, w->pos[4].y);
    if (a8c && w->unkC != 2)
        spr_draw_at(w, 17, w->pos[5].x, w->pos[5].y);
}

static void DrawBottomSpriteScaled(PselWork *w, s32 x, s32 scale) {
    MnSpr *s = &w->spr[13];
    if (scale >= 0 && scale <= 0x1000) {
        s->x = (s16)(x + 0x80);
        s->y = 0xa8;
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_DrawScaled(s, 0x1000, scale);
    }
}
static void DrawSubTitle(PselWork *w, s32 x, BOOL flag) {
    spr_draw_at(w, 0, x, 0x10);
    gDisplaySub[2].y = flag ? (u16)-8 : 0x20;
}

/* ----------------------------------------------------------- actions */
static void PlayerSelectMenu_Action_Select(void) {
    PselWork *w = PW;
    w->unk0 = 0;
    SetSprite9B4Anim(w, FALSE);
    SetSpriteA8C(w, 0);
    SetSprite804(w, FALSE);
    SetSprite5C4(w, 1);
    gTaskCurrent->func = PlayerSelectMenu_State_Select;
}

static void PlayerSelectMenu_Action_Intro(void) {
    PselWork *w = PW;
    w->unk0 = 0;
    w->unk20 = 1;
    w->unk18 = 4;
    KeyframeAnim_SetAnim(&w->pos[6], 9);
    KeyframeAnim_SetAnim(&w->pos[7], 0xb);
    KeyframeAnim_SetAnim(&w->pos[0], 1);
    KeyframeAnim_SetAnim(&w->pos[1], 2);
    KeyframeAnim_SetAnim(&w->pos[2], 3);
    KeyframeAnim_SetAnim(&w->pos[3], 4);
    KeyframeAnim_SetAnim(&w->pos[4], 5);
    KeyframeAnim_SetAnim(&w->pos[5], 7);
    SetSprite60CAnim3(w);
    SetSprite654Anim3(w);
    spr_set(w, 3, 6);
    spr_set(w, 4, 7);
    SetSprite9B4Anim(w, FALSE);
    SetSpriteA8C(w, 0);
    w->unk334 = 1;
    w->unk338 = 1;
    gTaskCurrent->func = PlayerSelectMenu_State_Intro;
}

static void PlayerSelectMenu_Action_ScrollOut(void) {
    PselWork *w = PW;
    w->unk0 = 0;
    KeyframeAnim_SetAnim(&w->pos[4], 6);
    KeyframeAnim_SetAnim(&w->pos[5], 8);
    KeyframeAnim_SetAnim(&w->pos[6], 0xa);
    KeyframeAnim_SetAnim(&w->pos[7], 0xc);
    if (w->unk20) {
        SetSprite60CAnim3(w);
        SetSprite654Anim4(w);
    } else {
        SetSprite60CAnim4(w);
        SetSprite654Anim3(w);
    }
    SetSprite9B4Anim(w, FALSE);
    SetSpriteA8C(w, 0);
    SetSprite5C4(w, 2);
    w->unk334 = 0;
    if (w->unkC == 0 || (u32)(w->unkC - 3) <= 1)
        SysSound_PlaySE(5);
    gTaskCurrent->func = PlayerSelectMenu_State_ScrollOut;
}

static void PlayerSelectMenu_Action_ScrollIn(void) {
    PselWork *w = PW;
    w->unk0 = 0;
    w->unk1C = w->unk14;
    w->unk14 = w->unk18;
    w->unk18 = 4;
    KeyframeAnim_SetAnim(&w->pos[4], 5);
    KeyframeAnim_SetAnim(&w->pos[5], 7);
    KeyframeAnim_SetAnim(&w->pos[6], 9);
    KeyframeAnim_SetAnim(&w->pos[7], 0xb);
    if (w->unk20) {
        SetSprite60CAnim3(w);
        SetSprite654Anim4(w);
    } else {
        SetSprite60CAnim4(w);
        SetSprite654Anim3(w);
    }
    SetSprite9B4Anim(w, FALSE);
    SetSpriteA8C(w, 0);
    w->unk334 = 1;
    gTaskCurrent->func = PlayerSelectMenu_State_ScrollIn;
}

static void PlayerSelectMenu_Action_Confirm(void) {
    PselWork *w = PW;
    w->unk0 = 0;
    w->unk4 = 0;
    SetSprite9B4Anim(w, TRUE);
    SetSprite60CAnim3(w);
    SetSprite654Anim3(w);
    SetSprite804(w, TRUE);
    switch (w->unk14) {
    case 0:
        Sound_PlayVoice(2);   /* SysSound_PlayMenuVoice */
        break;
    case 1:
        Sound_PlayVoice(3);
        break;
    case 2:
        Sound_PlayVoice(4);
        Sound_PlayVoiceAlt(5);   /* SysSound_PlayMenuVoiceAlt */
        break;
    }
    w->unk334 = 0;
    w->unk8 = 0;
    gTaskCurrent->func = PlayerSelectMenu_State_Confirmed;
}

static void PlayerSelectMenu_Action_FadeOutBack(void) {
    PselWork *w = PW;
    w->unk0 = 0;
    SetSpriteA8C(w, w->unk33C ? 1 : 0);
    SetSprite60CAnim3(w);
    SetSprite654Anim3(w);
    if (w->unk33C) {
        PlayerSelectMenu_StartBrightnessFade(0, 0x10, 0x10, 0x18);
    } else {
        PlayerSelectMenu_StartBrightnessFade(0, 0x10, 0x10, 0);
        w->unk0 = 0x18;
    }
    w->unk334 = 0;
    gTaskCurrent->func = PlayerSelectMenu_State_FadeOutBack;
}

/* ------------------------------------------------------------- exits */
static void PlayerSelectMenu_ExitBack(s32 mode) {
    switch (mode) {
    case 3:
        TitleMenu_StartAtPlayerSelectItem0();   /* SceneChange_ToTitleMenu_1_0 */
        break;
    case 4:
        TitleMenu_StartAtPlayerSelectItem1();   /* SceneChange_ToTitleMenu_1_1 */
        break;
    default:
        TitleMenu_Start();                      /* SceneChange_ToTitleMenu */
        break;
    }
}

static void PlayerSelectMenu_ExitToGame(s32 mode, s32 sub) {
    gGameState.unk0 = sub == 1 ? 1 : 0;
    switch (mode) {
    case 0:
        if (sub == 2) {
            /* Game_StartDemoScript3AtZone8Act2: the extra story script, then zone 8 act 2 */
            gGameState.unk1C = 8;
            gGameState.unk1D = 2;
            gGameState.unk10 = 0;
            StoryProgress_RestoreFromSave();
            mn_request_stage();
            return;
        }
        ZoneMap_StartDefault();   /* SceneChange_ToZoneMap */
        return;
    case 3:
    case 4:
        gGameState.unk1C = StoryProgress_GetZoneAt(0);
        gGameState.unk1D = 0;
        /* SceneChange_ToTimeAttackMenuMode0 / 1 */
        if (mode == 3)
            TimeAttackMenu_StartMode0();
        else
            TimeAttackMenu_StartMode1();
        return;
    default:
        TitleMenu_Start();
        return;
    }
}

/* ------------------------------------------------------------- states */
static void PlayerSelectMenu_Main(void) {   /* the opening */
    PselWork *w = PW;
    s32 x;
    if ((gPad.trg & PAD_A) && w->unk0 >= 0x15)
        w->unk340 = 1;
    if (w->unk340) {
        w->unk0 = 0x1e;
        KeyframeAnim_SetToLastFrame(&w->posB68);
    }
    KeyframeAnim_Update(&w->posB68);
    if (w->unk0 >= 0x26)
        w->unk330 += 0x20;
    DrawSubTitle(w, w->unk330, FALSE);
    x = (s32)(w->unk0 << 9) / 0x1e - 0x200;
    if (x > 0)
        x = 0;
    if (x < -0x100)
        x = -0x100;
    DrawBottomSpriteScaled(w, x, 0x1000);
    w->unk0++;
    if (KeyframeAnim_IsFinished(&w->posB68))
        PlayerSelectMenu_Action_Intro();
}

static void PlayerSelectMenu_State_Intro(void) {
    PselWork *w = PW;
    BOOL done = FALSE;
    s32 x, i;
    if (gPad.trg & PAD_A)
        w->unk340 = 1;
    if (w->unk340) {
        w->unk0 = 0x10;
        for (i = 0; i < 6; i++)
            KeyframeAnim_SetToLastFrame(&w->pos[i]);
    }
    if (w->unk0 >= 6) {
        KeyframeAnim_Update(&w->pos[6]);
        KeyframeAnim_Update(&w->pos[7]);
        x = w->pos[7].x;
        if (!w->unk20)
            x = -x;
        PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, x);
        for (i = 0; i < 6; i++)
            KeyframeAnim_Update(&w->pos[i]);
        done = TRUE;
        draw_common(w, TRUE);
        for (i = 0; i < 6; i++)
            if (!KeyframeAnim_IsFinished(&w->pos[i]))
                done = FALSE;
    }
    DrawBottomSpriteScaled(w, 0, 0x1000);
    w->unk330 += 0x20;
    if (w->unk330 > 0)
        w->unk330 = 0;
    DrawSubTitle(w, w->unk330, FALSE);
    w->unk0++;
    if (done && w->unk0 >= 0x10) {
        PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, 0);
        w->unk340 = 0;
        PlayerSelectMenu_Action_Select();
    }
}

static void PlayerSelectMenu_State_Select(void) {
    PselWork *w = PW;
    s32 n, scale;
    PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, 0);
    spr_draw_at(w, 1, w->pos[0].x, w->pos[0].y);
    spr_draw_at(w, 2, w->pos[1].x, w->pos[1].y);
    spr_draw_at(w, 3, w->pos[2].x, w->pos[2].y);
    spr_draw_at(w, 4, w->pos[3].x, w->pos[3].y);
    spr_draw_at(w, 14, w->pos[4].x, w->pos[4].y);
    if (w->unk0 != 0 || w->unk338 == 1) {
        spr_draw_at(w, 17, w->pos[5].x, w->pos[5].y);
        w->unk338 = 0;
    }
    draw_orbit(w, 0, 1);
    if (w->unk334 == 0 || w->unk0 >= 4)
        scale = 0x1000;
    else
        scale = (s32)(w->unk0 * 0x600) / 4 + 0xa00;
    w->spr[8].x = 0x80;
    w->spr[8].y = 0x98;
    AnimatorSprite_ProcessAnimation(&w->spr[8], 0, 0);
    AnimatorSprite_DrawScaled(&w->spr[8], scale, scale);
    DrawBottomSpriteScaled(w, 0, 0x1000);
    if (w->unk334 == 1 && w->unk0 < 5) {
        DrawSubTitle(w, 0, FALSE);
    } else {
        gDisplaySub[2].x++;
        DrawSubTitle(w, 0, TRUE);
    }
    w->unk0++;
    if (w->unkC == 0)
        mn_automap_choice(&gPad.cur, &gPad.trg, w->unk14, (u32)w->unk0);   /* test builds: debug/automap.txt */
    if (gPad.trg & PAD_A) {   /* PlayerSelectMenu_IsConfirmInput */
        PlayerSelectMenu_Action_Confirm();
        return;
    }
    if (gPad.cur & PAD_LEFT) {   /* PlayerSelectMenu_IsLeftInput */
        n = w->unk14 - 1;
        if (n < 0)
            n = w->unk10 - 1;
        w->unk20 = 0;
        w->unk18 = n;
        PlayerSelectMenu_Action_ScrollOut();
        return;
    }
    if (gPad.cur & PAD_RIGHT) {
        n = w->unk14 + 1;
        if (n >= w->unk10)
            n = 0;
        w->unk20 = 1;
        w->unk18 = n;
        PlayerSelectMenu_Action_ScrollOut();
        return;
    }
    if (gPad.trg & PAD_B) {   /* PlayerSelectMenu_IsBackPressed */
        w->unk33C = 0;
        SysSound_PlaySE(1);   /* SysSound_PlayCancelSE */
        PlayerSelectMenu_Action_FadeOutBack();
        return;
    }
    if (w->unk0 == 1) {
        w->spr[1].flags |= SPR_LOOP;
        w->spr[2].flags |= SPR_LOOP;
    }
}

static void PlayerSelectMenu_State_ScrollOut(void) {
    PselWork *w = PW;
    s32 x, step, base, t;
    KeyframeAnim_Update(&w->pos[4]);
    KeyframeAnim_Update(&w->pos[5]);
    draw_common(w, FALSE);
    KeyframeAnim_Update(&w->pos[7]);
    x = w->pos[7].x;
    if (!w->unk20)
        x = -x;
    PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, x);
    step = w->unk10 == 2 ? 0x8000 : 0x5555;
    base = PlayerSelectMenu_EaseValue(0, step, 0x20, w->unk0, 0x2000);
    if (w->unk20)
        base = -base;
    draw_orbit(w, base, 0);
    if (w->unk14 == 2 || w->unk18 == 2) {
        t = (s32)(w->unk0 << 12) / 10;
        if (t < 0x1000)
            DrawBottomSpriteScaled(w, 0, 0x1000 - t);
    } else {
        DrawBottomSpriteScaled(w, 0, 0x1000);
    }
    if (w->unk0 < 9) {
        if (w->unk0 < 5) {
            gDisplaySub[2].x++;
            DrawSubTitle(w, 0, TRUE);
        } else {
            DrawSubTitle(w, 0, FALSE);
        }
    } else {
        SetSprite5C4(w, 0);
        DrawSubTitle(w, 0, FALSE);
    }
    w->unk0++;
    if (w->unk0 >= 0x10)
        PlayerSelectMenu_Action_ScrollIn();
}

static void PlayerSelectMenu_State_ScrollIn(void) {
    PselWork *w = PW;
    s32 x, t, step, base;
    spr_draw_at(w, 1, w->pos[0].x, w->pos[0].y);
    spr_draw_at(w, 2, w->pos[1].x, w->pos[1].y);
    spr_draw_at(w, 3, w->pos[2].x, w->pos[2].y);
    spr_draw_at(w, 4, w->pos[3].x, w->pos[3].y);
    if (w->unk0 >= 6) {
        KeyframeAnim_Update(&w->pos[4]);
        KeyframeAnim_Update(&w->pos[5]);
        spr_draw_at(w, 14, w->pos[4].x, w->pos[4].y);
        KeyframeAnim_Update(&w->pos[6]);
        KeyframeAnim_Update(&w->pos[7]);
        x = w->pos[7].x;
        if (!w->unk20)
            x = -x;
        PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, x);
    }
    if (w->unk14 == 2 || w->unk1C == 2)
        t = (s32)((w->unk0 - 6) << 12) / 10;
    else
        t = 0x1000;
    DrawBottomSpriteScaled(w, 0, t);
    step = w->unk10 == 2 ? 0x8000 : 0x5555;
    base = PlayerSelectMenu_EaseValue(0, step, 0x20, w->unk0 + 0x10, 0x2000);
    if (w->unk20)
        base = -base;
    {
        s32 i;
        for (i = 0; i < w->unk10; i++)
            DrawOrbitSprite(w, (w->unk1C + i) % w->unk10, base + step * i);
    }
    DrawSubTitle(w, 0, FALSE);
    w->unk0++;
    if (w->unk0 >= 0x10) {
        PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, 0);
        PlayerSelectMenu_Action_Select();
    }
}

static void PlayerSelectMenu_State_Confirmed(void) {
    PselWork *w = PW;
    s32 limit = w->unk14 == 2 ? 0x78 : 0xe2;
    if (limit - w->unk0 > 0x10 && w->unk0 >= 0x20 && (gPad.trg & PAD_A))
        w->unk0 = limit - 0x10;
    if (limit - w->unk0 == 0x10)
        PlayerSelectMenu_StartBrightnessFade(0, 0x10, 0x10, 0);
    if (w->unk0 >= limit) {
        s32 mode = w->unkC, sel = w->unk14;
        Task_DestroyByTag(0x1000);
        PlayerSelectMenu_ExitToGame(mode, sel);
        return;
    }
    PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, 0);
    draw_common(w, TRUE);
    spr_draw_at(w, 15, 0, 0);
    draw_orbit(w, 0, 1);
    spr_draw_at(w, 8, 0x80, 0x98);
    DrawBottomSpriteScaled(w, 0, 0x1000);
    gDisplaySub[2].x++;
    DrawSubTitle(w, 0, TRUE);
    w->unk0++;
}

static void PlayerSelectMenu_State_FadeOutBack(void) {
    PselWork *w = PW;
    if (w->unk0 >= 0x28) {
        s32 mode = w->unkC;
        Task_DestroyByTag(0x1000);
        PlayerSelectMenu_ExitBack(mode);
        return;
    }
    PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, 0);
    draw_common(w, TRUE);
    draw_orbit(w, 0, 1);
    spr_draw_at(w, 8, 0x80, 0x98);
    DrawBottomSpriteScaled(w, 0, 0x1000);
    gDisplaySub[2].x++;
    DrawSubTitle(w, 0, TRUE);
    w->unk0++;
}

/* ---------------------------------------------------------- create */
static const char *const s_psel_lang[6] = { "act/mnu_psel_jpn.bac", "act/mnu_psel_eng.bac", "act/mnu_psel_fra.bac",
                                            "act/mnu_psel_deu.bac", "act/mnu_psel_ita.bac", "act/mnu_psel_spa.bac" };
static const char *const s_psel_bg[10] = { "bg/mnu_psel_bis_son.bbg",    "bg/mnu_psel_bis_blz.bbg",
                                           "bg/mnu_psel_bis_extra.bbg",  "bg/mnu_psel_scname_son.bbg",
                                           "bg/mnu_psel_scname_blz.bbg", "bg/mnu_psel_scname_extra.bbg",
                                           "bg/mnu_psel_base_b.bbg",     "bg/mnu_psel_base_a.bbg",
                                           "bg/mnu_psel_circle_ext.bbg", "bg/mnu_psel_circle_text.bbg" };

static void PlayerSelectMenu_Destructor(void) {
    PselWork *w = PW;
    int i;
    mn_display_reset();
    for (i = 0; i < 10; i++)
        mn_bg_free(w->bg[i]);
    mn_res_free(w->res_psel);
    mn_res_free(w->res_lang);
    mn_res_free(w->res_titl);
    free(w->ama);
}

static void init_spr(PselWork *w, int i, const MnRes *res, s32 anim, s32 prio, int engine) {
    AnimatorSprite_Init(&w->spr[i], res, (u16)anim, 0, engine);
    w->spr[i].flags &= ~0x60000u;
    w->spr[i].flags |= (u32)prio << 17;
}

/* The character screen (sub engine: illustration, name banner, "please
 * select") fills the frame at 2.5x. The selection UI of the touch screen
 * (circle with the other characters' icons, arrows, OK plate, back button)
 * is laid over it without the touch screen's base BG. */
static void PlayerSelect_Compose(void) {
    mn_comp_reset(1);
    mn_comp_full(1);
    /* main screen dial (circle BG3, character dots, plate with L/R arrows) as one widget, right of the art */
    mn_comp_add(0, 0x36, 0x08, 0xca, 0xc0, 478, 72, 1.0f, 1 << 2, 0);
    /* main screen B/back button, under the dial */
    mn_comp_add(0, 0x06, 0x4a, 0x2a, 0x6e, 580, 258, 1.5f, 0xf, 0);
}

void PlayerSelectMenu_StartMode(int mode) {
    MnTask *t = Task_Create(PlayerSelectMenu_Main, PlayerSelectMenu_Destructor, 0, 0x1000, 0x1000, sizeof(PselWork));
    PselWork *w = (PselWork *)t->work;
    int i;
    w->unkC = mode;
    w->unk14 = 0;
    w->unk18 = 4;
    w->unk1C = 4;
    w->unk24 = 4;
    w->unk10 = (mode == 0 && PlayerSelectMenu_IsFullyUnlocked()) ? 3 : 2;
    w->unk32C = 4;
    w->unk338 = 1;
    /* PlayerSelectMenu_InitDisplay: main engine (bottom) = models + buttons, sub (top) = illustration */
    mn_display_reset();
    PlayerSelect_Compose();
    g_mn_scene = "PlayerSelect";
    /* PlayerSelectMenu_LoadAssets: data_ov000_02369094 / data_ov000_02369074[lang] */
    for (i = 0; i < 10; i++)
        w->bg[i] = mn_bg_load(s_psel_bg[i]);
    w->res_psel = mn_res_load("act/mnu_psel.bac");
    w->res_lang = mn_res_load(s_psel_lang[mn_lang()]);
    w->res_titl = mn_res_load("act/mnu_titl.bac");
    w->ama = mn_ama_load("ama/ps_act.ama");
    /* PlayerSelectMenu_InitBackgrounds */
    Background_Show(0, 2, w->bg[6]);
    Background_Show(0, 3, w->bg[8]);
    Background_Show(1, 3, w->bg[7]);
    Background_SetPriority(1, 0, 1);
    Background_SetPriority(1, 2, 0);
    Background_SetPriority(1, 3, 3);
    Background_SetSize(1, 0, 512, 256);
    /* PlayerSelectMenu_InitSprites */
    init_spr(w, 0, w->res_psel, 1, 2, 1);
    init_spr(w, 1, w->res_psel, 3, 1, 0);
    init_spr(w, 2, w->res_psel, 3, 1, 0);
    w->spr[2].flags |= SPR_HFLIP;
    init_spr(w, 3, w->res_psel, 6, 1, 0);
    init_spr(w, 4, w->res_psel, 7, 1, 0);
    init_spr(w, 5, w->res_psel, 8, 2, 0);
    init_spr(w, 6, w->res_psel, 0xb, 2, 0);
    init_spr(w, 7, w->res_psel, 0xe, 2, 0);
    init_spr(w, 8, w->res_psel, 9, 1, 0);
    for (i = 9; i <= 12; i++)
        init_spr(w, i, w->res_psel, 0x11 + (i - 9), 3, 0);
    init_spr(w, 15, w->res_psel, 0x1d, 1, 0);
    init_spr(w, 16, w->res_psel, 0x1e, 1, 0);
    init_spr(w, 13, w->res_lang, 0, 0, 1);
    init_spr(w, 14, w->res_lang, 2, 1, 0);
    init_spr(w, 17, w->res_titl, 0, 1, 0);
    for (i = 0; i < 19; i++)
        w->anim[i] = -1;
    KeyframeAnim_Init(&w->posB68, w->ama, 0);
    for (i = 0; i < 8; i++)
        KeyframeAnim_Init(&w->pos[i], w->ama, 0);
    /* fade in, BGM, voice */
    if (BRIGHT_MAIN >= 0)
        BRIGHT_MAIN = BRIGHT_SUB = 0x10;
    else
        BRIGHT_MAIN = BRIGHT_SUB = -0x10;
    PlayerSelectMenu_StartBrightnessFade(BRIGHT_MAIN, 0, 0x10, 0);
    SysSound_PlayBGM(0);
    Sound_PlayVoice(0);
    /* PlayerSelectMenu_Action_Opening */
    w->unk0 = 0;
    KeyframeAnim_SetAnim(&w->posB68, 0);
    PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, 0x100);
    w->unk330 = -0x100;
    SetSprite5C4(w, 0);
    gSaveDataBusy = 0;
}

/*
 * PlayerSelectMenu_* (src/game/Menus/PlayerSelect/, overlay ov000): Sonic /
 * Blaze (/ extra) choice before the story (mode 0) and time attack (3, 4),
 * and Battle Play (modes 1 host / 2 guest). Battle Play on one Dreamcast: two
 * instances run side by side, player 1 = mode 1 on engines 0 / 1 with gPad,
 * player 2 = mode 2 on engines 2 / 3 (MN_ENGINE_P2) with gPad2; the wireless
 * send / partner receive records (WirelessSession_*) are the two players'
 * records in mn_vs.c, so the partner logic (same choice blocked, both decided ->
 * TimeSelect, cancels) is the original's. Vs CPU: player 2's instance is driven by the computer
 * (PlayerSelectMenu_CpuInput) and its column is tagged "CPU" (PlayerSelectMenu_DrawCpuTag).
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
    MnBgImg *bg[10];       /* bis_son/blz/extra, scname x3, base_b, base_a, circle_ext, circle_text */
    MnRes *res_psel, *res_lang, *res_titl;
    void *ama;
} PselAssets;

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
    PselAssets *as;        /* the files (Battle Play: one set for both players) */
    PselAssets own;
    s32 eng;               /* engine of the DS main screen: 0, or MN_ENGINE_P2 for player 2 */
    s32 player;            /* 0 / 1: whose pad and wireless record */
    MnPad *pad;            /* gPad / gPad2 */
    s32 cpu_t;             /* Vs CPU: frames since player 2 (the computer) could decide */
} PselWork;

static void PlayerSelectMenu_Main(void);
static void PlayerSelectMenu_State_Intro(void);
static void PlayerSelectMenu_State_Select(void);
static void PlayerSelectMenu_State_ScrollOut(void);
static void PlayerSelectMenu_State_ScrollIn(void);
static void PlayerSelectMenu_State_Confirmed(void);
static void PlayerSelectMenu_State_FadeOutBack(void);
static void PlayerSelectMenu_State_QuitPrompt(void);
static void PlayerSelectMenu_State_PartnerLeft(void);
static PselWork *s_ps_vs[2];   /* the two players' instances (Battle Play) */

#define PW TASK_WORK(PselWork)
#define PS_SUB(w) ((w)->eng + 1)
#define DSUB(w) mn_bg_ofs(PS_SUB(w))
#define PS_VS(w) ((u32)((w)->unkC - 1) <= 1)

/* ------------------------------------------------- partner records (VS) */
/* WirelessSession_GetSendBuffer / GetPartnerRecvBuffer: [0] choice, [1] decided, [2] quit, [3] sync */
static void PlayerSelectMenu_SetLocalChoice(PselWork *w, u8 v) {
    u8 *q = WirelessSession_GetSendBuffer(w->player);
    q[0] = v;
    q[1] = 1;
}
static void PlayerSelectMenu_SetLocalQuit(PselWork *w) {
    if (w->unkC == 1)
        WirelessSession_GetSendBuffer(w->player)[2] = 1;
}
static s32 PlayerSelectMenu_GetPartnerChoice(PselWork *w) {
    u8 *p = WirelessSession_GetPartnerRecvBuffer(w->player);
    if (p[1] == 0)
        return 4;
    return p[0];
}
static BOOL PlayerSelectMenu_IsSameChoiceAsPartner(PselWork *w) {
    u8 *a = WirelessSession_GetSendBuffer(w->player), *b = WirelessSession_GetPartnerRecvBuffer(w->player);
    if (a[1] == 0 || b[1] == 0)
        return FALSE;
    return a[0] == b[0];
}
static BOOL PlayerSelectMenu_IsDifferentChoiceFromPartner(PselWork *w) {
    u8 *a = WirelessSession_GetSendBuffer(w->player), *b = WirelessSession_GetPartnerRecvBuffer(w->player);
    if (a[1] == 0 || b[1] == 0)
        return FALSE;
    return a[0] != b[0];
}
static BOOL PlayerSelectMenu_HasPartnerCancelled(PselWork *w) {
    if (w->unkC != 2)
        return FALSE;
    return WirelessSession_GetPartnerRecvBuffer(w->player)[2] != 0;
}

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
typedef struct { s32 t, from, to, dur, delay, eng; } PselFade;
static void PlayerSelectMenu_BrightnessFade_Main(void) {
    PselFade *w = TASK_WORK(PselFade);
    s32 v;
    if (w->t < w->delay) {
        w->t++;
        return;
    }
    v = w->from + (w->to - w->from) * (w->t - w->delay) / (w->dur > 0 ? w->dur : 1);
    g_mn_bright[w->eng] = g_mn_bright[w->eng + 1] = (s16)v;
    w->t++;
    if (w->t - w->delay >= w->dur) {
        g_mn_bright[w->eng] = g_mn_bright[w->eng + 1] = (s16)w->to;
        Task_Destroy(gTaskCurrent);
    }
}
/* the fade task of player 2's instance has priority 0x3001 (each player's DS has its own) */
static void PlayerSelectMenu_StartBrightnessFade(PselWork *o, s32 from, s32 to, s32 dur, s32 delay) {
    PselFade *w;
    u16 prio = (u16)(0x3000 + o->player);
    Task_DestroyRange(prio, prio);
    w = (PselFade *)Task_Create(PlayerSelectMenu_BrightnessFade_Main, NULL, 0, prio, 0x1000, sizeof(PselFade))->work;
    w->from = from;
    w->to = to;
    w->dur = dur;
    w->delay = delay;
    w->eng = o->eng;
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
        Background_Show(PS_SUB(w), 2, w->as->bg[3 + w->unk14]);
        DSUB(w)[2].x = 0;
        DSUB(w)[2].y = 0x20;
    }
    spr_set(w, 0, a);
    w->spr[0].flags &= ~(u32)SPR_LOOP;
    if (a != 1)
        DSUB(w)[2].y = (u16)-8;
}

/* PlayerSelectMenu_SetBackgroundSelection: the illustration on sub BG0 */
static void PlayerSelectMenu_SetBackgroundSelection(PselWork *w, s32 sel, s32 scroll) {
    if (sel != w->unk32C) {
        if (sel >= 0 && sel <= 2)
            Background_Show(PS_SUB(w), 0, w->as->bg[sel]);
        w->unk32C = sel;
    }
    if (sel == 4) {
        DSUB(w)[0].x = 0x100;
    } else {
        scroll = -scroll;
        if (scroll < 0)
            scroll += ((-scroll) / 0x200 + 1) << 9;
        scroll = scroll % 0x200;
        DSUB(w)[0].x = (u16)scroll;
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
    DSUB(w)[2].y = flag ? (u16)-8 : 0x20;
}

/* PlayerSelectMenu_DrawSpriteA8C: the back button (not on the guest's screen) */
static void DrawSpriteA8C(PselWork *w, s32 x, s32 y) {
    if (w->unkC != 2)
        spr_draw_at(w, 17, x, y);
}

/* ----------------------------------------------------------- actions */
static void PlayerSelectMenu_Action_Select(void) {
    PselWork *w = PW;
    u8 *s;
    w->unk0 = 0;
    SetSprite9B4Anim(w, FALSE);
    SetSpriteA8C(w, 0);
    SetSprite804(w, FALSE);
    SetSprite5C4(w, 1);
    s = WirelessSession_GetSendBuffer(w->player);
    s[0] = (u8)w->unk14;
    s[2] = 0;
    s[1] = 0;
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
    if (PS_VS(w))
        PlayerSelectMenu_SetLocalChoice(w, (u8)w->unk14);
    if (!PS_VS(w)) {
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
        PlayerSelectMenu_StartBrightnessFade(w, 0, 0x10, 0x10, 0x18);
    } else {
        PlayerSelectMenu_StartBrightnessFade(w, 0, 0x10, 0x10, 0);
        w->unk0 = 0x18;
    }
    w->unk334 = 0;
    gTaskCurrent->func = PlayerSelectMenu_State_FadeOutBack;
}

/* PlayerSelectMenu_Action_OpenQuitPrompt (host, B): MsgBox 9 (YES / NO) on the host's main screen BG1 */
static void PlayerSelectMenu_Action_OpenQuitPrompt(void) {
    PselWork *w = PW;
    w->unk0 = 0;
    SetSpriteA8C(w, w->unk33C ? 1 : 0);
    SetSprite60CAnim3(w);
    SetSprite654Anim3(w);
    MsgBox_Create(9, 3, 1, w->eng, 1, 0, 0xc, 0x100010);
    w->unk334 = 0;
    gTaskCurrent->func = PlayerSelectMenu_State_QuitPrompt;
}

/* PlayerSelectMenu_Action_PartnerLeft (guest): the host left the select. (Task_DestroyRange(0x4000)
 * is the link icon, WirelessSession_Stop the wireless session: neither exists here; the guest's own
 * fade task stops.) */
static void PlayerSelectMenu_Action_PartnerLeft(void) {
    PselWork *w = PW;
    u16 prio = (u16)(0x3000 + w->player);
    w->unk0 = 0;
    SetSprite60CAnim3(w);
    SetSprite654Anim3(w);
    Task_DestroyRange(prio, prio);
    w->unk334 = 0;
    gTaskCurrent->func = PlayerSelectMenu_State_PartnerLeft;
}

/* PlayerSelectMenu_Action_Opening */
static void PlayerSelectMenu_Action_Opening(MnTask *t) {
    PselWork *w = (PselWork *)t->work;
    w->unk0 = 0;
    KeyframeAnim_SetAnim(&w->posB68, 0);
    PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, 0x100);
    w->unk330 = -0x100;
    SetSprite5C4(w, 0);
    t->func = PlayerSelectMenu_Main;
}

/* ------------------------------------------------------------- exits */
static void PlayerSelectMenu_ExitBack(s32 mode) {
    switch (mode) {
    case 1:   /* SysSound_Release; WirelessSession_EndWhenAlone(SceneChange_ToTitleMenu) */
    case 2:   /* SysSound_Release; SceneChange_ToTitleMenu */
        SysSound_Release();
        TitleMenu_Start();
        break;
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
    case 1:   /* host: unk4 = the host's character, unk8 = the guest's */
        if (sub == 0) {
            gGameState.unk4 = 0;
            gGameState.unk8 = 1;
        } else if (sub == 1) {
            gGameState.unk4 = 1;
            gGameState.unk8 = 0;
        }
        TimeSelectMenu_StartMode0();
        return;
    case 2:   /* guest (TimeSelectMenu_StartMode1: the follower; on one Dreamcast player 1's
               * TimeSelect is the screen of both, PlayerSelectMenu_VsGuestDone) */
        if (sub == 0) {
            gGameState.unk4 = 1;
            gGameState.unk8 = 0;
        } else if (sub == 1) {
            gGameState.unk4 = 0;
            gGameState.unk8 = 1;
        }
        TimeSelectMenu_StartMode0();
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

/* Battle Play on one Dreamcast: the scene changes of the select are player 1's (the host's
 * Task_DestroyByTag(0x1000) ends both players' tasks). The guest reaching an exit first leaves
 * its screen (already faded) and waits for the host's exit. */
static void PlayerSelectMenu_State_VsGuestWait(void) {}
static BOOL PlayerSelectMenu_VsGuestDone(PselWork *w) {
    if (w->unkC != 2)
        return FALSE;
    gTaskCurrent->func = PlayerSelectMenu_State_VsGuestWait;
    return TRUE;
}

/* Vs CPU (native): player 2's select played by the computer on gPad2 (released: mn_vs_pad_update). It
 * keeps its character until player 1 decides; on player 1's character the select itself moves it on
 * (the original skip of the partner's decided character, above in State_Select), on the other one it
 * presses A 0x20 frames later (player 1's confirm voice first; player 1 may still cancel with B). */
static void PlayerSelectMenu_CpuInput(PselWork *w, MnPad *pad) {
    s32 p1 = PlayerSelectMenu_GetPartnerChoice(w);
    if (p1 == 4 || p1 == w->unk14) {
        w->cpu_t = 0;
        return;
    }
    if (++w->cpu_t >= 0x20) {
        w->cpu_t = 0;
        pad->trg |= PAD_A;
    }
}

/* Vs CPU (native): player 2's column is tagged "CPU" in the name plates' font (font8.bin, the original
 * font_ipl_8x8 glyphs: mn_name_strips), twice its size, white on a dark box, on player 2's touch screen
 * where player 1 has the back button (the guest has none). A task of the scene (tag 0x1000). */
typedef struct {
    MnBgImg *img;
} PselCpuTag;
static void PlayerSelectMenu_DrawCpuTag(void) {
    PselCpuTag *t = TASK_WORK(PselCpuTag);
    if (!t->img)
        return;
    mn_emit_bg_part_scaled(MN_ENGINE_P2, 0, t->img, 6, 0x58, 0, 0, 24, 8, 2.0f);
    mn_emit_flat(MN_ENGINE_P2, 0, 2, 0x54, 54, 24, 0xc0000000u);
}
static void PlayerSelectMenu_CpuTag_Destructor(void) {
    PselCpuTag *t = TASK_WORK(PselCpuTag);
    mn_bg_free(t->img);
    t->img = NULL;
}
static void PlayerSelectMenu_CreateCpuTag(void) {
    PselCpuTag *t = (PselCpuTag *)Task_Create(PlayerSelectMenu_DrawCpuTag, PlayerSelectMenu_CpuTag_Destructor, 0,
                                              0x1002, 0x1000, sizeof(PselCpuTag))->work;
    const u16 *nm;
    int len = mn_vs_player_name(1, &nm);
    u16 ink = 0x7fff;
    t->img = mn_name_strips(&nm, &len, 1, &ink);
}

/* ------------------------------------------------------------- states */
/* PlayerSelectMenu_State_StartSync / _State_Sync: the two units' handshake on record byte 3 */
static void PlayerSelectMenu_State_Sync(void) {
    PselWork *w = PW;
    u8 *a = WirelessSession_GetSendBuffer(w->player);
    u8 *b = WirelessSession_GetPartnerRecvBuffer(w->player);
    u8 *c = WirelessSession_GetSendBuffer(w->player);   /* WirelessSession_GetOwnRecvBuffer */
    BOOL go = FALSE;
    if (w->unkC == 1) {
        if (a[3] == 0 && b[3] == 0) {
            a[3] = 1;
        } else if (a[3] == 1) {
            if (b[3] == 1)
                a[3] = 2;
        } else if (a[3] == 2) {
            if (b[3] == 2)
                a[3] = 3;
        } else if (c[3] >= 3) {
            go = TRUE;
        }
    } else {
        if (b[3] == 0)
            a[3] = 0;
        if (a[3] == 0 && b[3] == 1)
            a[3] = 1;
        if (a[3] == 1) {
            if (b[3] == 2)
                a[3] = 2;
        } else if (a[3] == 2 && b[3] == 3) {
            go = TRUE;
        }
    }
    if (go) {
        PlayerSelectMenu_StartBrightnessFade(w, 0x10, 0, 0x10, 0);
        SysSound_PlayBGM(2);
        PlayerSelectMenu_Action_Opening(gTaskCurrent);
    }
}
static void PlayerSelectMenu_State_StartSync(void) {
    PselWork *w = PW;
    WirelessSession_GetSendBuffer(w->player)[3] = 0;
    gTaskCurrent->func = PlayerSelectMenu_State_Sync;
}

static void PlayerSelectMenu_Main(void) {   /* the opening */
    PselWork *w = PW;
    s32 x;
    if (!PS_VS(w) && (w->pad->trg & PAD_A) && w->unk0 >= 0x15)
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
    if (!PS_VS(w) && (w->pad->trg & PAD_A))
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
    MnPad *pad = w->pad;
    s32 n, scale;
    PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, 0);
    spr_draw_at(w, 1, w->pos[0].x, w->pos[0].y);
    spr_draw_at(w, 2, w->pos[1].x, w->pos[1].y);
    spr_draw_at(w, 3, w->pos[2].x, w->pos[2].y);
    spr_draw_at(w, 4, w->pos[3].x, w->pos[3].y);
    spr_draw_at(w, 14, w->pos[4].x, w->pos[4].y);
    if (w->unk0 != 0 || w->unk338 == 1) {
        DrawSpriteA8C(w, w->pos[5].x, w->pos[5].y);
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
        DSUB(w)[2].x++;
        DrawSubTitle(w, 0, TRUE);
    }
    if (PS_VS(w) && w->unk14 == w->unk24)
        spr_draw_at(w, 16, 0, 0);   /* PlayerSelectMenu_DrawSpriteA44: the partner's character */
    w->unk0++;
    if (PS_VS(w) && w->unk14 == w->unk24) {
        /* the partner has decided on this character: on to the next one */
        n = w->unk14;
        if (w->unk20) {
            if (++n >= w->unk10)
                n = 0;
        } else {
            if (--n < 0)
                n = w->unk10 - 1;
        }
        w->unk18 = n;
        PlayerSelectMenu_Action_ScrollOut();
        return;
    }
    if (w->unkC == 0)
        mn_automap_choice(&pad->cur, &pad->trg, w->unk14, (u32)w->unk0);   /* test builds: debug/automap.txt */
    if (w->unkC == 2 && mn_vs_cpu())
        PlayerSelectMenu_CpuInput(w, pad);
    if (pad->trg & PAD_A) {   /* PlayerSelectMenu_IsConfirmInput */
        PlayerSelectMenu_Action_Confirm();
        return;
    }
    if (pad->cur & PAD_LEFT) {   /* PlayerSelectMenu_IsLeftInput */
        n = w->unk14 - 1;
        if (n < 0)
            n = w->unk10 - 1;
        w->unk20 = 0;
        w->unk18 = n;
        PlayerSelectMenu_Action_ScrollOut();
        return;
    }
    if (pad->cur & PAD_RIGHT) {
        n = w->unk14 + 1;
        if (n >= w->unk10)
            n = 0;
        w->unk20 = 1;
        w->unk18 = n;
        PlayerSelectMenu_Action_ScrollOut();
        return;
    }
    if (PS_VS(w)) {
        n = PlayerSelectMenu_GetPartnerChoice(w);
        if (w->unk24 != n)
            w->unk24 = n;
    }
    if (w->unkC != 2) {
        if (pad->trg & PAD_B) {   /* PlayerSelectMenu_IsBackPressed */
            w->unk33C = 0;
            SysSound_PlaySE(1);   /* SysSound_PlayCancelSE */
            if (w->unkC == 1)
                PlayerSelectMenu_Action_OpenQuitPrompt();
            else
                PlayerSelectMenu_Action_FadeOutBack();
            return;
        }
    } else if (PlayerSelectMenu_HasPartnerCancelled(w)) {
        SysSound_PlaySE(1);
        PlayerSelectMenu_Action_PartnerLeft();
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
            DSUB(w)[2].x++;
            DrawSubTitle(w, 0, TRUE);
        } else {
            DrawSubTitle(w, 0, FALSE);
        }
    } else {
        SetSprite5C4(w, 0);
        DrawSubTitle(w, 0, FALSE);
    }
    if (PS_VS(w) && w->unk14 == w->unk24)
        spr_draw_at(w, 16, 0, 0);
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
    if (PS_VS(w) && w->unk14 == w->unk24)
        spr_draw_at(w, 16, 0, 0);
    w->unk0++;
    if (w->unk0 >= 0x10) {
        PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, 0);
        PlayerSelectMenu_Action_Select();
    }
}

static void PlayerSelectMenu_DrawConfirmed(PselWork *w) {
    PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, 0);
    draw_common(w, TRUE);
    spr_draw_at(w, 15, 0, 0);
    draw_orbit(w, 0, 1);
    spr_draw_at(w, 8, 0x80, 0x98);
    DrawBottomSpriteScaled(w, 0, 0x1000);
    DSUB(w)[2].x++;
    DrawSubTitle(w, 0, TRUE);
}

static void PlayerSelectMenu_State_Confirmed(void) {
    PselWork *w = PW;
    BOOL back = FALSE, next = FALSE;
    if (PS_VS(w)) {
        if (w->unk4 >= 0xe2) {
            if (PlayerSelectMenu_IsDifferentChoiceFromPartner(w)) {
                s32 mode = w->unkC, sel = w->unk14;
                if (PlayerSelectMenu_VsGuestDone(w))
                    return;
                Task_DestroyByTag(0x1000);
                PlayerSelectMenu_ExitToGame(mode, sel);
                return;
            }
            {
                u16 prio = (u16)(0x3000 + w->player);
                Task_DestroyRange(prio, prio);
            }
            g_mn_bright[w->eng + 1] = 0;
            g_mn_bright[w->eng] = g_mn_bright[w->eng + 1];
            back = TRUE;
        }
        if (w->unkC == 2 && PlayerSelectMenu_IsSameChoiceAsPartner(w))
            back = TRUE;
        if (PlayerSelectMenu_HasPartnerCancelled(w))
            next = TRUE;
    } else {
        s32 limit = w->unk14 == 2 ? 0x78 : 0xe2;
        if (limit - w->unk0 > 0x10 && w->unk0 >= 0x20 && (w->pad->trg & PAD_A))
            w->unk0 = limit - 0x10;
        if (limit - w->unk0 == 0x10) {
            PlayerSelectMenu_StartBrightnessFade(w, 0, 0x10, 0x10, 0);
            if (w->unkC == 0 && g_menu_hooks.bgm_stop)
                g_menu_hooks.bgm_stop(0x10);   /* SysSound_StopBGM(0x10) */
        }
        if (w->unk0 >= limit) {
            s32 mode = w->unkC, sel = w->unk14;
            Task_DestroyByTag(0x1000);
            PlayerSelectMenu_ExitToGame(mode, sel);
            return;
        }
    }
    PlayerSelectMenu_DrawConfirmed(w);
    if (w->unk0 == 8 && PS_VS(w)) {
        switch (w->unk14) {
        case 0:
            Sound_PlayVoice(6);   /* SysSound_PlayMenuVoice */
            break;
        case 1:
            Sound_PlayVoice(7);
            break;
        }
    }
    w->unk0++;
    if (PS_VS(w)) {
        if (PlayerSelectMenu_IsDifferentChoiceFromPartner(w)) {
            if (w->unk8 > 0x10) {
                if (w->unk4 == 0)
                    PlayerSelectMenu_StartBrightnessFade(w, 0, 0x10, 0x10, 0xd2);
                w->unk4++;
            }
            w->unk8++;
        } else {
            w->unk8 = 0;
            w->unk4 = 0;
        }
    }
    if (PS_VS(w) && w->unk4 == 0 && w->unk8 == 0 && (w->pad->trg & PAD_B))
        back = TRUE;
    if (back)
        PlayerSelectMenu_Action_Select();
    else if (next)
        PlayerSelectMenu_Action_PartnerLeft();
}

static void PlayerSelectMenu_State_FadeOutBack(void) {
    PselWork *w = PW;
    if (w->unk0 >= 0x28) {
        s32 mode = w->unkC;
        if (PlayerSelectMenu_VsGuestDone(w))
            return;
        Task_DestroyByTag(0x1000);
        PlayerSelectMenu_ExitBack(mode);
        return;
    }
    PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, 0);
    draw_common(w, TRUE);
    draw_orbit(w, 0, 1);
    spr_draw_at(w, 8, 0x80, 0x98);
    DrawBottomSpriteScaled(w, 0, 0x1000);
    DSUB(w)[2].x++;
    DrawSubTitle(w, 0, TRUE);
    w->unk0++;
}

/* PlayerSelectMenu_State_QuitPrompt: MsgBox 9 YES -> the host quits (SetLocalQuit), NO -> select */
static void PlayerSelectMenu_State_QuitPrompt(void) {
    PselWork *w = PW;
    PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, 0);
    draw_common(w, TRUE);
    draw_orbit(w, 0, 1);
    w->spr[8].x = 0x80;
    w->spr[8].y = 0x98;
    AnimatorSprite_ProcessAnimation(&w->spr[8], 0, 0);
    AnimatorSprite_Draw(&w->spr[8]);
    DrawBottomSpriteScaled(w, 0, 0x1000);
    DSUB(w)[2].x++;
    DrawSubTitle(w, 0, TRUE);
    w->unk0++;
    if (MsgBox_IsClosed()) {
        if (!MsgBox_GetResult()) {
            /* Wireless_EnableErrorCallback(0) */
            PlayerSelectMenu_SetLocalQuit(w);
            PlayerSelectMenu_Action_FadeOutBack();
        } else {
            w->unk338 = 1;
            PlayerSelectMenu_Action_Select();
        }
    }
}

/* PlayerSelectMenu_State_PartnerLeft (guest): 16 frames to black. The original then shows MsgBox 2
 * (the partner has left) and leaves for the title menu (_State_WaitPartnerLeftMsg); here the host
 * quit on the same screen and its exit takes both players to the title menu: the guest waits black. */
static void PlayerSelectMenu_State_PartnerLeft(void) {
    PselWork *w = PW;
    PlayerSelectMenu_DrawConfirmed(w);
    w->unk0++;
    g_mn_bright[w->eng + 1] = (s16)-w->unk0;
    g_mn_bright[w->eng] = g_mn_bright[w->eng + 1];
    if (w->unk0 >= 0x10) {
        w->unk0 = 0;
        gTaskCurrent->func = PlayerSelectMenu_State_VsGuestWait;
    }
}

/* ---------------------------------------------------------- create */
static const char *const s_psel_lang[6] = { "act/mnu_psel_jpn.bac", "act/mnu_psel_eng.bac", "act/mnu_psel_fra.bac",
                                            "act/mnu_psel_deu.bac", "act/mnu_psel_ita.bac", "act/mnu_psel_spa.bac" };
static const char *const s_psel_bg[10] = { "bg/mnu_psel_bis_son.bbg",    "bg/mnu_psel_bis_blz.bbg",
                                           "bg/mnu_psel_bis_extra.bbg",  "bg/mnu_psel_scname_son.bbg",
                                           "bg/mnu_psel_scname_blz.bbg", "bg/mnu_psel_scname_extra.bbg",
                                           "bg/mnu_psel_base_b.bbg",     "bg/mnu_psel_base_a.bbg",
                                           "bg/mnu_psel_circle_ext.bbg", "bg/mnu_psel_circle_text.bbg" };

/* PlayerSelectMenu_LoadAssets: data_ov000_02369094 / data_ov000_02369074[lang] (Battle Play: one set
 * for both players' instances) */
static PselAssets s_vs_as;
static int s_vs_refs;
static void PlayerSelectMenu_LoadAssets(PselAssets *a) {
    int i;
    for (i = 0; i < 10; i++)
        a->bg[i] = mn_bg_load(s_psel_bg[i]);
    a->res_psel = mn_res_load("act/mnu_psel.bac");
    a->res_lang = mn_res_load(s_psel_lang[mn_lang()]);
    a->res_titl = mn_res_load("act/mnu_titl.bac");
    a->ama = mn_ama_load("ama/ps_act.ama");
}
static void PlayerSelectMenu_FreeAssets(PselAssets *a) {
    int i;
    for (i = 0; i < 10; i++)
        mn_bg_free(a->bg[i]);
    mn_res_free(a->res_psel);
    mn_res_free(a->res_lang);
    mn_res_free(a->res_titl);
    free(a->ama);
    memset(a, 0, sizeof(*a));
}

static void PlayerSelectMenu_Destructor(void) {
    PselWork *w = PW;
    int l;
    if (w->as == &s_vs_as)
        s_ps_vs[w->player] = NULL;
    if (w->as == &s_vs_as && --s_vs_refs > 0) {
        for (l = 0; l < 4; l++) {   /* this player's screens only */
            Background_Hide(w->eng, l);
            Background_Hide(w->eng + 1, l);
        }
        return;
    }
    mn_display_reset();
    PlayerSelectMenu_FreeAssets(w->as);
}

static void init_spr(PselWork *w, int i, const MnRes *res, s32 anim, s32 prio, int engine) {
    AnimatorSprite_Init(&w->spr[i], res, (u16)anim, 0, w->eng + engine);
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

/* Battle Play: each player's two DS screens in one half of the frame, the top screen (character art,
 * name banner) over the touch screen (dial, OK plate, back button, message box) at 1.25x; each half
 * fades with that player's brightness. */
static void PlayerSelect_ComposeVs(void) {
    int p;
    mn_comp_reset(1);
    for (p = 0; p < 2; p++) {
        int e = p * MN_ENGINE_P2;
        float x = p * 320.0f;
        mn_comp_add(e + 1, 0, 0, 256, 192, x, 0, 1.25f, 0, 0);
        mn_comp_add(e, 0, 0, 256, 192, x, 240, 1.25f, 0, 0);
        g_mn_comp.br[p].x = x;
        g_mn_comp.br[p].y = 0;
        g_mn_comp.br[p].w = 320;
        g_mn_comp.br[p].h = 480;
        g_mn_comp.br[p].engine = (u8)e;
    }
    g_mn_comp.nbr = 2;
}

/* PlayerSelectMenu_Create(mode) for one player: player 1 (gPad, engines 0 / 1) or player 2 of
 * Battle Play (gPad2, engines 2 / 3) */
static MnTask *PlayerSelectMenu_Create(int mode, int player) {
    MnTask *t = Task_Create(PlayerSelectMenu_Main, PlayerSelectMenu_Destructor, 0, 0x1000, 0x1000, sizeof(PselWork));
    PselWork *w = (PselWork *)t->work;
    PselAssets *a;
    int i;
    w->unkC = mode;
    w->player = player;
    w->eng = player ? MN_ENGINE_P2 : 0;
    w->pad = player ? &gPad2 : &gPad;
    w->unk14 = 0;
    w->unk18 = 4;
    w->unk1C = 4;
    w->unk24 = 4;
    switch (mode) {
    case 0:
        w->unk10 = PlayerSelectMenu_IsFullyUnlocked() ? 3 : 2;
        break;
    case 1:
        w->unk14 = 0;
        w->unk10 = 2;
        break;
    case 2:
        w->unk14 = 1;
        w->unk10 = 2;
        break;
    default:
        w->unk10 = 2;
        break;
    }
    w->unk32C = 4;
    w->unk338 = 1;
    if (PS_VS(w)) {
        if (s_vs_refs++ == 0)
            PlayerSelectMenu_LoadAssets(&s_vs_as);
        w->as = &s_vs_as;
        s_ps_vs[player] = w;
    } else {
        w->as = &w->own;
        PlayerSelectMenu_LoadAssets(w->as);
    }
    a = w->as;
    /* PlayerSelectMenu_InitBackgrounds */
    Background_Show(w->eng, 2, a->bg[6]);
    Background_Show(w->eng, 3, a->bg[8]);
    Background_Show(w->eng + 1, 3, a->bg[7]);
    /* PlayerSelectMenu_InitDisplay: main BG1 0, BG0 (3D) 1, BG3 (the circle) 2, BG2 (the base) 3 */
    Background_SetPriority(w->eng, 0, 1);
    Background_SetPriority(w->eng, 1, 0);
    Background_SetPriority(w->eng, 2, 3);
    Background_SetPriority(w->eng, 3, 2);
    Background_SetPriority(w->eng + 1, 0, 1);
    Background_SetPriority(w->eng + 1, 2, 0);
    Background_SetPriority(w->eng + 1, 3, 3);
    Background_SetSize(w->eng + 1, 0, 512, 256);
    /* PlayerSelectMenu_InitSprites */
    init_spr(w, 0, a->res_psel, 1, 2, 1);
    init_spr(w, 1, a->res_psel, 3, 1, 0);
    init_spr(w, 2, a->res_psel, 3, 1, 0);
    w->spr[2].flags |= SPR_HFLIP;
    init_spr(w, 3, a->res_psel, 6, 1, 0);
    init_spr(w, 4, a->res_psel, 7, 1, 0);
    init_spr(w, 5, a->res_psel, 8, 2, 0);
    init_spr(w, 6, a->res_psel, 0xb, 2, 0);
    init_spr(w, 7, a->res_psel, 0xe, 2, 0);
    init_spr(w, 8, a->res_psel, 9, 1, 0);
    for (i = 9; i <= 12; i++)
        init_spr(w, i, a->res_psel, 0x11 + (i - 9), 3, 0);
    init_spr(w, 15, a->res_psel, 0x1d, 1, 0);
    init_spr(w, 16, a->res_psel, 0x1e, 1, 0);
    init_spr(w, 13, a->res_lang, 0, 0, 1);
    init_spr(w, 14, a->res_lang, 2, 1, 0);
    init_spr(w, 17, a->res_titl, 0, 1, 0);
    /* PlayerSelectMenu_InitMainSprite's oamPrio (unk46): the plate 8 / 0x10, the circle icons and the OK
     * button 0xc, the character's name 4 (in front of the plate), the confirm flash 3 */
    w->spr[3].oam = 8;
    w->spr[4].oam = 0x10;
    for (i = 5; i <= 8; i++)
        w->spr[i].oam = 0xc;
    w->spr[14].oam = 4;
    w->spr[15].oam = 3;
    for (i = 0; i < 19; i++)
        w->anim[i] = -1;
    KeyframeAnim_Init(&w->posB68, a->ama, 0);
    for (i = 0; i < 8; i++)
        KeyframeAnim_Init(&w->pos[i], a->ama, 0);
    if (PS_VS(w)) {
        /* the wireless record, then the handshake (fade-in, BGM 2 and the opening after it) */
        u8 *s = WirelessSession_GetSendBuffer(w->player);
        s[0] = (u8)w->unk14;
        s[2] = 0;
        s[1] = 0;
        s[3] = 0;
        g_mn_bright[w->eng] = g_mn_bright[w->eng + 1] = 0x10;   /* PlayerSelectMenu_InitDisplay(fade) */
        w->unk0 = 0;
        t->func = PlayerSelectMenu_State_StartSync;
        return t;
    }
    /* fade in, BGM, voice */
    if (BRIGHT_MAIN >= 0)
        BRIGHT_MAIN = BRIGHT_SUB = 0x10;
    else
        BRIGHT_MAIN = BRIGHT_SUB = -0x10;
    PlayerSelectMenu_StartBrightnessFade(w, BRIGHT_MAIN, 0, 0x10, 0);
    SysSound_PlayBGM(0);
    Sound_PlayVoice(0);
    PlayerSelectMenu_Action_Opening(t);
    return t;
}

void PlayerSelectMenu_StartMode(int mode) {
    if ((u32)(mode - 1) <= 1) {   /* SceneChange_ToPlayerSelectMenuMode1 / 2: Battle Play, both players */
        PlayerSelectMenu_StartVs();
        return;
    }
    /* PlayerSelectMenu_InitDisplay: main engine (bottom) = models + buttons, sub (top) = illustration */
    mn_display_reset();
    PlayerSelect_Compose();
    g_mn_scene = "PlayerSelect";
    PlayerSelectMenu_Create(mode, 0);
    gSaveDataBusy = 0;
}

/* Battle Play (SceneChange_ToPlayerSelectMenuMode1 on the host, ..Mode2 on the guest): both players'
 * selects at once, player 1 the host (mode 1), player 2 the guest (mode 2; Vs CPU: the computer) */
void PlayerSelectMenu_StartVs(void) {
    mn_display_reset();
    PlayerSelect_ComposeVs();
    g_mn_scene = "PlayerSelectVs";
    mn_vs_reset_records();
    PlayerSelectMenu_Create(1, 0);   /* PlayerSelectMenu_StartMode1 */
    PlayerSelectMenu_Create(2, 1);   /* PlayerSelectMenu_StartMode2 */
    if (mn_vs_cpu())
        PlayerSelectMenu_CreateCpuTag();
    gSaveDataBusy = 1;               /* gSystemFlags |= 0x10 */
}

/* host test access: player p's state (0 select, 1 confirmed, 3 quit prompt, 2 other; -1 none) and choice */
int PlayerSelectMenu_VsState(int p, int *choice) {
    const PselWork *w = (p == 0 || p == 1) ? s_ps_vs[p] : NULL;
    int i;
    if (!w)
        return -1;
    if (choice)
        *choice = w->unk14;
    for (i = 0; i < MN_MAX_TASKS; i++) {
        const MnTask *t = mn_task_at(i);
        if (!t || t->work != (const void *)w)
            continue;
        if (t->func == PlayerSelectMenu_State_Select)
            return 0;
        if (t->func == PlayerSelectMenu_State_Confirmed)
            return 1;
        if (t->func == PlayerSelectMenu_State_QuitPrompt)
            return 3;
        return 2;
    }
    return -1;
}

/*
 * TimeSelectMenu_* (src/game/Menus/TimeSelect/, 112 functions, overlay ov000): the Battle Play stage
 * select after the two players' character select. On the DS both units run it: the host is the
 * chooser (TimeSelectMenu_StartMode0, w->unk4 = 1: zone / act with LEFT / RIGHT, A, B back to the
 * character select), the guest the follower (StartMode1: it shows the chooser's stage); the stage
 * list is the union of both units' unlocked stages (TimeSelectMenu_GetUnlockedStageMask). When the
 * chooser decides, both press A ("ready", or 200 frames pass) and the race starts
 * (Game_StartVsStage: gGameState.unk10 = 1, zone = stage / 2, act = stage % 2).
 *
 * One Dreamcast: player 1's chooser screen is the screen of both. Player 2's side of the records
 * (the follower's TimeSelectMenu_State_Sync handshake and unlock mask, its A in
 * TimeSelectMenu_State_FollowerReady) is TimeSelectMenu_LocalGuest_Main on gPad2.
 *
 * DS display (TimeSelectMenu_InitDisplay: POWCNT swap, the main engine is the top screen):
 *   main (top): BG0 3D (the two characters' models, DrawModelA / B), BG1 mnu_conf_base02, BG2
 *     mnu_conf_base (scrolls 2 px/frame), sprites (mnu_conf_zsel + _<lang>): name plates, character
 *     names, the characters' sprite groups, the VS emblem, the players' names (font_ipl_8x8 OAM tiles)
 *   sub (bottom, touch): BG1 mnu_time_selectbase, BG2 mnu_conf_base, sprites: the act label
 *     (mnu_time_act_lc_<lang>), the header, the arrows and the zone emblem (mnu_time_select), the back
 *     button (mnu_titl), the message boxes (MsgBox 0xa / 0xd / 0xe on sub BG0)
 * Single screen: TimeSelectMenu_Compose (MENUS.md 15).
 *
 * DS service                          here
 * WirelessSession records             mn_vs.c (the two local players')
 * gTouchState rectangles              pad only (the original's pad path: A, B, LEFT, RIGHT)
 * TimeSelectMenu_LoadArchives (player_lz7.narc models)  the menus' 3D layer (mn_model.c)
 * download play transfer (Transfer_*, StageStart_Create), link icon (wireless_icon.bac)  not translated
 */
#include <stdlib.h>
#include "mn_internal.h"

typedef struct {
    u32 unk0;        /* frame counter of the state */
    s32 unk4;        /* 1: the chooser (host) */
    s32 unk8;        /* unlocked stages */
    s32 unkC;        /* stage 0..13: zone * 2 + act */
    s32 unk10;       /* previous stage */
    s32 unk14;       /* a message box is up */
    s32 unk18, unk1C;   /* the chooser's / the follower's "ready" (unk18 in Select: A during the scroll) */
    s32 unk20;       /* download play transfer done */
    u16 unk24;       /* the stage mask (both players') */
    s32 unk28;       /* the host's character (the left side) */
    s32 unk2C;       /* the guest's character */
    MnSpr spr30, spr78, sprC0;   /* header, left / right arrow */
    MnSpr spr[21];
    s32 anim6F0, anim6F4, anim6F8;
    s32 anim[21];
    KeyframeAnim pos[18];        /* +0x850 (zone_sel_vs.ama) */
    MnRes *res[6];               /* +0x1010: time_select, time_act_lc_<lang>, conf_zsel, conf_zsel_<lang>, titl */
    void *ama;                   /* +0x1028 */
    MnBgImg *bg[3];              /* +0x102c: conf_base, conf_base02, time_selectbase */
    MnBgImg *names;              /* the two players' name glyphs (TimeSelectMenu_LoadPlayerNames) */
    s32 model_frame[2];          /* Model_Animate: Sonic / Blaze */
} TsWork;

static TsWork *s_ts;
static void TimeSelectMenu_Main(void);
static void TimeSelectMenu_State_Sync(void);
static void TimeSelectMenu_State_Select(void);
static void TimeSelectMenu_State_WaitScroll(void);
static void TimeSelectMenu_State_ChooserConfirmed(void);
static void TimeSelectMenu_State_ChooserReady(void);
static void TimeSelectMenu_State_StartStage(void);
static void TimeSelectMenu_State_ExitToPlayerSelect(void);

/* the records' u16 fields (+4 handshake, +6 unlock mask) */
static u16 rec16(const u8 *p, int o) { return (u16)(p[o] | p[o + 1] << 8); }
static void rec16_set(u8 *p, int o, u16 v) {
    p[o] = (u8)v;
    p[o + 1] = (u8)(v >> 8);
}

/* ---------------------------------------------------------------- fade */
typedef struct {
    u32 t;
    s16 from, to;
    u32 dur, delay;
} TsFadeWork;

static void TimeSelectMenu_BrightnessFade_Main(void) {
    TsFadeWork *w = TASK_WORK(TsFadeWork);
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

static void TimeSelectMenu_StartBrightnessFade(s32 x, s32 y, s32 a, s32 b) {
    TsFadeWork *w;
    Task_DestroyRange(0x4000, 0x4000);
    if (x < -16)
        x = -16;
    if (x > 16)
        x = 16;
    if (y < -16)
        y = -16;
    if (y > 16)
        y = 16;
    w = (TsFadeWork *)Task_Create(TimeSelectMenu_BrightnessFade_Main, NULL, 0, 0x4000, 0x1000, sizeof(TsFadeWork))->work;
    w->t = 0;
    w->from = (s16)x;
    w->to = (s16)y;
    w->dur = (u32)a;
    w->delay = (u32)b;
}

static void TimeSelectMenu_ScrollBg_Main(void) {
    gDisplayMain[2].x += 2;   /* gDisplayMain.unk8 */
    gDisplaySub[2].x += 2;
}

/* ----------------------------------------------------- partner records */
static BOOL TimeSelectMenu_IsMode0Flag(void) { return gGameState.unk4 == 1; }
static BOOL TimeSelectMenu_IsPartnerReadyAsChooser(TsWork *w) {
    u8 *p = w->unk4 ? WirelessSession_GetPartnerRecvBuffer(0) : WirelessSession_GetSendBuffer(0);
    return p[3] != 0;
}

/* TimeSelectMenu_GetUnlockedStageMask: bit zone * 2 + act, from both stories' progress */
static u16 TimeSelectMenu_GetUnlockedStageMask(void) {
    u16 m = 1;
    u16 n = gSaveData.unkC;
    if (n >= 1)
        m |= 3;
    if (n >= 4)
        m |= 0xc;
    if (n >= 8)
        m |= 0x30;
    if (n >= 0xc)
        m |= 0xc0;
    if (n >= 0x10)
        m |= 0x300;
    if (n >= 0x14)
        m |= 0xc00;
    if (n >= 0x17)
        m |= 0x3000;
    n = gSaveData.unk64;   /* data_022c46c0.unk24 */
    if (n >= 2)
        m |= 0xc0;
    if (n >= 6)
        m |= 3;
    if (n >= 9)
        m |= 0x30;
    if (n >= 0xd)
        m |= 0xc;
    if (n >= 0x11)
        m |= 0xc00;
    if (n >= 0x15)
        m |= 0x300;
    if (n >= 0x19)
        m |= 0x3000;
    return m;
}

/* ------------------------------------------------------- sprite helpers */
static void set_anim(MnSpr *s, s32 *cur, s32 a) {
    if (*cur != a) {
        AnimatorSprite_SetAnimation(s, (u16)a);
        *cur = a;
    }
}
static void draw_at(MnSpr *s, s32 x, s32 y) {
    s->x = (s16)x;
    s->y = (s16)y;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}
static void draw_scaled(MnSpr *s, s32 x, s32 y, s32 scale) {
    s->x = (s16)x;
    s->y = (s16)y;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    if (scale == 0x1000)
        AnimatorSprite_Draw(s);
    else
        AnimatorSprite_DrawScaled(s, scale, scale);   /* AnimatorSprite_DrawRotScale(s, scale, scale, 0) */
}

static void TimeSelectMenu_SetSprite0Anim(TsWork *w, s32 a) {   /* the zone emblem */
    set_anim(&w->spr[0], &w->anim[0], a + 5);
    w->spr[0].flags &= ~(u32)SPR_LOOP;
    w->spr[0].flags &= ~(u32)SPR_STOPPED;
}
static void TimeSelectMenu_SetSprite1Anim(TsWork *w, s32 a, s32 b) {   /* the act label */
    set_anim(&w->spr[1], &w->anim[1], (a * 2 + b) * 2);
    w->spr[1].flags &= ~(u32)SPR_LOOP;
}
static void TimeSelectMenu_SetSprite1SelectAnim(TsWork *w, s32 a, s32 b) {
    set_anim(&w->spr[1], &w->anim[1], (a * 2 + b) * 2 + 1);
    w->spr[1].flags &= ~(u32)SPR_LOOP;
}
static void TimeSelectMenu_SetSprite2Anim0(TsWork *w) {   /* the back button */
    set_anim(&w->spr[2], &w->anim[2], 0);
    w->spr[2].flags &= ~(u32)SPR_LOOP;
}
static void TimeSelectMenu_SetSprite30Anim0(TsWork *w) { set_anim(&w->spr30, &w->anim6F0, 0); }
static void TimeSelectMenu_SetSprite30Anim1(TsWork *w) { set_anim(&w->spr30, &w->anim6F0, 1); }
static void TimeSelectMenu_SetSprite78Anim2(TsWork *w) {
    AnimatorSprite_SetAnimation(&w->spr78, 2);
    w->anim6F4 = 2;
    AnimatorSprite_ProcessAnimation(&w->spr78, 0, 0);
    w->spr78.flags |= SPR_LOOP;
    w->spr78.flags |= SPR_STOPPED;
}
static void TimeSelectMenu_SetSprite78Anim2Hidden(TsWork *w) {
    AnimatorSprite_SetAnimation(&w->spr78, 2);
    w->anim6F4 = 2;
    w->spr78.flags |= SPR_LOOP;
    w->spr78.flags &= ~(u32)SPR_STOPPED;
}
static void TimeSelectMenu_SetSprite78Anim3Hidden(TsWork *w) {
    AnimatorSprite_SetAnimation(&w->spr78, 3);
    w->anim6F4 = 3;
    w->spr78.flags |= SPR_LOOP;
    w->spr78.flags &= ~(u32)SPR_STOPPED;
}
static void TimeSelectMenu_SetSprite78Anim4(TsWork *w) {
    set_anim(&w->spr78, &w->anim6F4, 4);
    w->spr78.flags &= ~(u32)SPR_LOOP;
    w->spr78.flags &= ~(u32)SPR_STOPPED;
}
static void TimeSelectMenu_SetSpriteC0Anim2(TsWork *w) {
    AnimatorSprite_SetAnimation(&w->sprC0, 2);
    w->anim6F8 = 2;
    AnimatorSprite_ProcessAnimation(&w->sprC0, 0, 0);
    w->sprC0.flags |= SPR_LOOP | SPR_STOPPED | SPR_HFLIP;
}
static void TimeSelectMenu_SetSpriteC0Anim2Hidden(TsWork *w) {
    set_anim(&w->sprC0, &w->anim6F8, 2);
    w->sprC0.flags |= SPR_LOOP;
    w->sprC0.flags &= ~(u32)SPR_STOPPED;
    w->sprC0.flags |= SPR_HFLIP;
}
static void TimeSelectMenu_SetSpriteC0Anim3Hidden(TsWork *w) {
    set_anim(&w->sprC0, &w->anim6F8, 3);
    w->sprC0.flags |= SPR_LOOP;
    w->sprC0.flags &= ~(u32)SPR_STOPPED;
    w->sprC0.flags |= SPR_HFLIP;
}
static void TimeSelectMenu_SetSpriteC0Anim4(TsWork *w) {
    set_anim(&w->sprC0, &w->anim6F8, 4);
    w->sprC0.flags &= ~(u32)SPR_LOOP;
    w->sprC0.flags &= ~(u32)SPR_STOPPED;
    w->sprC0.flags |= SPR_HFLIP;
}
static void TimeSelectMenu_SetSprite6Anim0(TsWork *w) {
    set_anim(&w->spr[6], &w->anim[6], 0);
    w->spr[6].flags &= ~(u32)SPR_LOOP;
}
static void TimeSelectMenu_SetSprite7Anim1(TsWork *w) {
    set_anim(&w->spr[7], &w->anim[7], 1);
    w->spr[7].flags &= ~(u32)SPR_LOOP;
}
static void TimeSelectMenu_SetSprites8To11(TsWork *w) {   /* Sonic's group (anims 6..9) */
    s32 i;
    for (i = 0; i < 4; i++) {
        set_anim(&w->spr[8 + i], &w->anim[8 + i], 6 + i);
        w->spr[8 + i].flags &= ~(u32)SPR_LOOP;
    }
}
static void TimeSelectMenu_SetSprites12To15(TsWork *w) {   /* Blaze's group (anims 10..13) */
    s32 i;
    for (i = 0; i < 4; i++) {
        set_anim(&w->spr[12 + i], &w->anim[12 + i], 10 + i);
        w->spr[12 + i].flags &= ~(u32)SPR_LOOP;
    }
}
static void TimeSelectMenu_SetSprite16Anim4(TsWork *w) {
    set_anim(&w->spr[16], &w->anim[16], 4);
    w->spr[16].flags &= ~(u32)SPR_LOOP;
}
static void TimeSelectMenu_SetSprite17Anim3(TsWork *w) {
    set_anim(&w->spr[17], &w->anim[17], 3);
    w->spr[17].flags &= ~(u32)SPR_LOOP;
}
static void TimeSelectMenu_SetSprite19Anim0(TsWork *w) {
    set_anim(&w->spr[19], &w->anim[19], 0);
    w->spr[19].flags &= ~(u32)SPR_LOOP;
}

/* ------------------------------------------------------------ drawing */
/* TimeSelectMenu_DrawPlayerName: 7 glyph tiles of player `row` (0 the host, 1 the guest) at
 * (x0 + 0x14, y0 + 0xb), OAM priority 0, palette 0xc */
static void TimeSelectMenu_DrawPlayerName(TsWork *w, s32 x0, s32 y0, s32 row) {
    s32 x = x0 + 0x14, y = y0 + 0xb, i;
    if (!w->names)
        return;
    for (i = 0; i < 8; i++, x += 8)   /* 7 glyphs (an unnamed player's identifier: 8) */
        if (x >= -8 && x < 0x100 && y >= -8 && y < 0xc0)
            mn_emit_bg_part(0, 0, w->names, (float)x, (float)y, (float)(i * 8), (float)(row * 16), 8, 8);
}
static void TimeSelectMenu_DrawSpriteGroup(TsWork *w, s32 first, s32 x, s32 y, s32 scale) {
    s32 i;
    for (i = 0; i < 4; i++)
        draw_scaled(&w->spr[first + i], x, y, scale);
}
#define TimeSelectMenu_DrawSpriteGroupA(w, x, y, s) TimeSelectMenu_DrawSpriteGroup(w, 12, x, y, s)
#define TimeSelectMenu_DrawSpriteGroupB(w, x, y, s) TimeSelectMenu_DrawSpriteGroup(w, 8, x, y, s)

/* TimeSelectMenu_SetupOrthoCamera + DrawModelA (Blaze, slot 1) / DrawModelB (Sonic, slot 0): the
 * player_lz7 model with its joint animation 0 (Sonic) / 0x37 (Blaze) and visibility animation 1,
 * looping (slot flag 4), at the DS screen position (x, y), x mirrored for the left side */
static void TimeSelectMenu_DrawModel(TsWork *w, s32 chara, s32 x, s32 y, BOOL flip) {
    MnModelDesc d;
    memset(&d, 0, sizeof(d));
    d.mdl = chara;
    d.set = 0;
    d.jidx = chara ? 0x37 : 0;
    d.vidx = 1;
    d.frame = w->model_frame[chara];
    d.loop = 1;
    d.alpha = 31;
    d.t = 95.0f;   /* G3i_OrthoW_(0x5f000, -0x60000, -0x80000, 0x7f000, 0x1000, 0xc8000) */
    d.b = -96.0f;
    d.l = -128.0f;
    d.r = 127.0f;
    d.n = 1.0f;
    d.f = 200.0f;
    d.at[2] = -1.0f;   /* G3i_LookAt_((0, 0, 0), up (0, 1, 0), (0, 0, -1)) */
    d.pos[0] = (float)(x - 0x80);
    d.pos[1] = (float)-(y - 0x60);
    d.pos[2] = -100.0f;
    if (chara) {
        d.scale[0] = (flip ? -0xc800 : 0xc800) / 4096.0f;
        d.scale[1] = d.scale[2] = 0xc000 / 4096.0f;
    } else {
        d.scale[0] = (flip ? -0xd000 : 0xd000) / 4096.0f;
        d.scale[1] = d.scale[2] = 0xd000 / 4096.0f;
    }
    d.cull = 0;   /* NNS_G3dGlbPolygonAttr(1, 0, 3 = no culling, 0, 0x1f, 0) */
    d.diff = 0x7fff;
    d.amb = 0x6f7b;
    d.nlights = 1;
    d.lcol[0] = 0x7fff;
    d.lvec[0][0] = flip ? 0xfff : -0x1000;
    d.lvec[0][1] = -0x1000;
    d.lvec[0][2] = -0x2000;
    mn_3d_add(&d);
    w->model_frame[chara] += 0x1000;   /* Model_Animate */
}

/* TimeSelectMenu_DrawLayoutA: the top screen */
static void TimeSelectMenu_DrawLayoutA(TsWork *w) {
    draw_at(&w->spr[16], 8, 0x88);
    draw_at(&w->spr[16], 0x98, 0x88);
    if (w->unk28 == 0) {
        draw_at(&w->spr[6], 8, 0x98);
        draw_at(&w->spr[7], 0x98, 0x98);
        TimeSelectMenu_DrawSpriteGroupB(w, 0x38, 0x58, 0x1000);
        TimeSelectMenu_DrawSpriteGroupA(w, 0xc8, 0x58, 0x1000);
    } else {
        draw_at(&w->spr[7], 8, 0x98);
        draw_at(&w->spr[6], 0x98, 0x98);
        TimeSelectMenu_DrawSpriteGroupA(w, 0x38, 0x58, 0x1000);
        TimeSelectMenu_DrawSpriteGroupB(w, 0xc8, 0x58, 0x1000);
    }
    TimeSelectMenu_DrawPlayerName(w, 8, 0, 0);
    draw_at(&w->spr[17], 8, 0);
    TimeSelectMenu_DrawPlayerName(w, 0x98, 0, 1);
    draw_at(&w->spr[17], 0x98, 0);
    draw_at(&w->spr[18], 0x80, 0x58);
    draw_scaled(&w->spr[19], 0x80, 0x58, 0x1000);
    if (w->unk28 == 0) {
        TimeSelectMenu_DrawModel(w, 0, 0x38, 0x90, TRUE);
        TimeSelectMenu_DrawModel(w, 1, 0xc8, 0x90, FALSE);
    } else {
        TimeSelectMenu_DrawModel(w, 1, 0x38, 0x90, TRUE);
        TimeSelectMenu_DrawModel(w, 0, 0xc8, 0x90, FALSE);
    }
}

/* TimeSelectMenu_DrawLayoutB: the touch screen (scale: the zone emblem's; flag: the back button) */
static void TimeSelectMenu_DrawLayoutB(TsWork *w, s32 scale, BOOL flag) {
    draw_at(&w->spr[1], 0, 0);
    if (w->unk4)
        draw_at(&w->spr30, 0, 0);
    draw_at(&w->spr78, 0x60, 0x90);
    draw_at(&w->sprC0, 0xfa, 0x90);
    draw_scaled(&w->spr[0], 0x48, 0xa0, scale);
    if (w->unk4 && flag)
        draw_at(&w->spr[2], 8, 0x90);
}

/* MenuO_Update */
static void TimeSelectMenu_Update(TsWork *w, BOOL flag) {
    KeyframeAnim_Update(&w->pos[0]);
    TimeSelectMenu_DrawLayoutA(w);
    if (w->unkC / 2 != w->unk10 / 2)
        TimeSelectMenu_DrawLayoutB(w, w->pos[0].unk4, flag);
    else
        TimeSelectMenu_DrawLayoutB(w, 0x1000, flag);
    w->unk0++;
}

/* --------------------------------------------------------------- states */
static void TimeSelectMenu_State_Sync(void) {
    TsWork *w = TASK_WORK(TsWork);
    u8 *a = WirelessSession_GetSendBuffer(0), *b = WirelessSession_GetPartnerRecvBuffer(0);
    s32 i;
    w->unk0++;
    if (w->unk0 < 8)
        return;
    if (w->unk4 != 0) {
        if (rec16(a, 4) == 0) {
            if (rec16(b, 4) == 1) {
                rec16_set(a, 4, 1);
                rec16_set(a, 6, TimeSelectMenu_GetUnlockedStageMask());
            }
            return;
        }
        if (rec16(a, 4) == 1) {
            if (rec16(b, 4) == 2)
                rec16_set(a, 4, 2);
            return;
        }
    } else {
        if (rec16(a, 4) == 0) {
            rec16_set(a, 4, 1);
            rec16_set(a, 6, TimeSelectMenu_GetUnlockedStageMask());
            return;
        }
        if (rec16(a, 4) == 1) {
            if (rec16(b, 4) == 1)
                rec16_set(a, 4, 2);
            return;
        }
        if (rec16(b, 4) != 2)
            return;
    }
    w->unk0 = 0;
    w->unk24 = (u16)(rec16(a, 6) | rec16(b, 6));
    w->unk8 = 0;
    for (i = 0; i < 0xd; i++)
        if (w->unk24 & (1 << i))
            w->unk8++;
    if (w->unk4 != 0) {
        if (w->unk8 > 1) {
            TimeSelectMenu_SetSprite78Anim2(w);
            TimeSelectMenu_SetSpriteC0Anim2(w);
        } else {
            TimeSelectMenu_SetSprite78Anim4(w);
            TimeSelectMenu_SetSpriteC0Anim4(w);
        }
    } else {
        TimeSelectMenu_SetSprite78Anim4(w);
        TimeSelectMenu_SetSpriteC0Anim4(w);
    }
    TimeSelectMenu_StartBrightnessFade(0x10, 0, 0x10, 0);
    SysSound_PlayBGM(2);   /* SysSound_Init; SysSound_PlayBGM(2) */
    gTaskCurrent->func = TimeSelectMenu_Main;
}

/* the opening: the key-frame slides of zone_sel_vs.ama */
static void TimeSelectMenu_Main(void) {
    TsWork *w = TASK_WORK(TsWork);
    KeyframeAnim *p;
    BOOL done;
    s32 i;
    if (w->unk0 == 2) {
        MsgBox_Create(w->unk4 ? 0xa : 0xd, 3, 5, 1, 0, 0, 0xe, 0x200010);
        w->unk14 = 1;
    }
    done = TRUE;
    for (i = 0; i < 0x12; i++) {
        KeyframeAnim_Update(&w->pos[i]);
        if (!KeyframeAnim_IsFinished(&w->pos[i]))
            done = FALSE;
    }
    draw_at(&w->spr[1], w->pos[1].x, w->pos[1].y);
    if (w->unk4)
        draw_at(&w->spr30, w->pos[2].x, w->pos[2].y);
    draw_at(&w->spr78, w->pos[3].x, w->pos[3].y);
    draw_at(&w->sprC0, w->pos[4].x, w->pos[4].y);
    p = &w->pos[0];
    draw_scaled(&w->spr[0], p->x, p->y, p->unk4);
    if (w->unk4)
        draw_at(&w->spr[2], w->pos[5].x, w->pos[5].y);
    draw_at(&w->spr[16], w->pos[8].x, w->pos[8].y);
    draw_at(&w->spr[16], w->pos[9].x, w->pos[9].y);
    if (w->unk28 == 0) {
        draw_at(&w->spr[6], w->pos[10].x, w->pos[10].y);
        draw_at(&w->spr[7], w->pos[11].x, w->pos[11].y);
        TimeSelectMenu_DrawSpriteGroupB(w, w->pos[16].x, w->pos[16].y, w->pos[16].unk4);
        TimeSelectMenu_DrawSpriteGroupA(w, w->pos[17].x, w->pos[17].y, w->pos[17].unk4);
    } else {
        draw_at(&w->spr[7], w->pos[10].x, w->pos[10].y);
        draw_at(&w->spr[6], w->pos[11].x, w->pos[11].y);
        TimeSelectMenu_DrawSpriteGroupA(w, w->pos[16].x, w->pos[16].y, w->pos[16].unk4);
        TimeSelectMenu_DrawSpriteGroupB(w, w->pos[17].x, w->pos[17].y, w->pos[17].unk4);
    }
    TimeSelectMenu_DrawPlayerName(w, w->pos[12].x, w->pos[12].y, 0);
    draw_at(&w->spr[17], w->pos[12].x, w->pos[12].y);
    TimeSelectMenu_DrawPlayerName(w, w->pos[13].x, w->pos[13].y, 1);
    draw_at(&w->spr[17], w->pos[13].x, w->pos[13].y);
    draw_at(&w->spr[18], w->pos[14].x, w->pos[14].y);
    draw_scaled(&w->spr[19], w->pos[15].x, w->pos[15].y, w->pos[15].unk4);
    if (w->unk28 == 0) {
        TimeSelectMenu_DrawModel(w, 0, w->pos[6].x, w->pos[6].y, TRUE);
        TimeSelectMenu_DrawModel(w, 1, w->pos[7].x, w->pos[7].y, FALSE);
    } else {
        TimeSelectMenu_DrawModel(w, 1, w->pos[6].x, w->pos[6].y, TRUE);
        TimeSelectMenu_DrawModel(w, 0, w->pos[7].x, w->pos[7].y, FALSE);
    }
    w->unk0++;
    if (done) {
        rec16_set(WirelessSession_GetSendBuffer(0), 4, 0);
        w->unk0 = 0;
        if (w->unk4) {
            w->unk18 = 0;
            gTaskCurrent->func = TimeSelectMenu_State_Select;
            return;
        }
        /* the follower (TimeSelectMenu_State_FollowChooser): not on this screen */
        KeyframeAnim_SetAnim(&w->pos[0], 6);
        w->unkC = 0;
        w->unk10 = 0;
    }
}

static void TimeSelectMenu_State_Select(void) {
    TsWork *w = TASK_WORK(TsWork);
    s32 old, n, q;
    u16 mask;
    TimeSelectMenu_DrawLayoutA(w);
    TimeSelectMenu_DrawLayoutB(w, 0x1000, TRUE);
    w->unk0++;
    if (w->unk0 == 1 && w->unk8 > 1) {
        TimeSelectMenu_SetSprite78Anim2Hidden(w);
        TimeSelectMenu_SetSpriteC0Anim2Hidden(w);
    }
    if ((gPad.trg & PAD_A) || w->unk18) {   /* TimeSelectMenu_IsConfirmInput */
        u8 *s;
        w->unk0 = 0;
        SysSound_PlaySE(7);
        s = WirelessSession_GetSendBuffer(0);
        s[0] = (u8)w->unkC;
        s[1] = 1;
        MsgBox_RequestClose();
        w->unk14 = 0;
        TimeSelectMenu_SetSprite30Anim1(w);
        n = w->unkC;
        TimeSelectMenu_SetSprite1SelectAnim(w, n / 2, n % 2);
        gGameState.unk1C = (u8)(w->unkC / 2);
        gGameState.unk1D = (u8)(w->unkC % 2);
        gTaskCurrent->func = TimeSelectMenu_State_ChooserConfirmed;
        return;
    }
    if (gPad.trg & PAD_B) {   /* TimeSelectMenu_IsBackPressed */
        w->unk0 = 0;
        SysSound_PlaySE(8);
        WirelessSession_GetSendBuffer(0)[2] = 1;
        TimeSelectMenu_StartBrightnessFade(0, 0x10, 0x10, 0);
        gTaskCurrent->func = TimeSelectMenu_State_ExitToPlayerSelect;
        return;
    }
    if (gPad.cur & PAD_LEFT) {   /* TimeSelectMenu_IsLeftInput */
        w->unk0 = 0;
        old = w->unkC;
        n = old;
        mask = w->unk24;
        do {
            if (--n < 0)
                n = 0xd;
        } while (!(mask & (1 << n)));
        if (old != n) {
            w->unk10 = old;
            w->unkC = n;
            WirelessSession_GetSendBuffer(0)[0] = (u8)w->unkC;
            KeyframeAnim_SetAnim(&w->pos[0], 6);
            TimeSelectMenu_SetSprite78Anim3Hidden(w);
            q = n / 2;
            TimeSelectMenu_SetSprite1Anim(w, q, n % 2);
            if (w->unk10 / 2 != q)
                TimeSelectMenu_SetSprite0Anim(w, q);
            gTaskCurrent->func = TimeSelectMenu_State_WaitScroll;
        }
        return;
    }
    if (gPad.cur & PAD_RIGHT) {   /* TimeSelectMenu_IsRightInput */
        w->unk0 = 0;
        old = w->unkC;
        n = old;
        mask = w->unk24;
        do {
            if (++n > 0xd)
                n = 0;
        } while (!(mask & (1 << n)));
        if (old != n) {
            w->unk10 = old;
            w->unkC = n;
            WirelessSession_GetSendBuffer(0)[0] = (u8)w->unkC;
            KeyframeAnim_SetAnim(&w->pos[0], 6);
            TimeSelectMenu_SetSpriteC0Anim3Hidden(w);
            q = n / 2;
            TimeSelectMenu_SetSprite1Anim(w, q, n % 2);
            if (w->unk10 / 2 != q)
                TimeSelectMenu_SetSprite0Anim(w, q);
            gTaskCurrent->func = TimeSelectMenu_State_WaitScroll;
        }
    }
}

static void TimeSelectMenu_State_WaitScroll(void) {
    TsWork *w = TASK_WORK(TsWork);
    TimeSelectMenu_Update(w, TRUE);
    if (gPad.trg & PAD_A)
        w->unk18 = 1;
    if (KeyframeAnim_IsFinished(&w->pos[0])) {
        w->unk0 = 0;
        gTaskCurrent->func = TimeSelectMenu_State_Select;
    }
}

static void TimeSelectMenu_State_ChooserConfirmed(void) {
    TsWork *w = TASK_WORK(TsWork);
    TimeSelectMenu_DrawLayoutA(w);
    TimeSelectMenu_DrawLayoutB(w, 0x1000, TRUE);
    w->unk0++;
    if (MsgBox_IsClosed()) {
        s32 n;
        w->unk0 = 0;
        TimeSelectMenu_SetSprite30Anim0(w);
        n = w->unkC;
        TimeSelectMenu_SetSprite1Anim(w, n / 2, n % 2);
        TimeSelectMenu_SetSprite78Anim4(w);
        TimeSelectMenu_SetSpriteC0Anim4(w);
        MsgBox_Create(0xe, 3, 5, 1, 0, 0, 0xe, 0x200010);
        w->unk14 = 1;
        /* (download play: TimeSelectMenu_State_ChooserTransferSync) */
        w->unk18 = 0;
        w->unk1C = 0;
        gTaskCurrent->func = TimeSelectMenu_State_ChooserReady;
    }
}

static void TimeSelectMenu_State_ChooserReady(void) {
    TsWork *w = TASK_WORK(TsWork);
    TimeSelectMenu_DrawLayoutA(w);
    TimeSelectMenu_DrawLayoutB(w, 0x1000, FALSE);
    w->unk0++;
    if (gPad.trg & PAD_A) {
        w->unk18 = 1;
        WirelessSession_GetSendBuffer(0)[3] = 1;
    }
    if (TimeSelectMenu_IsPartnerReadyAsChooser(w))
        w->unk1C = 1;
    if ((w->unk18 && w->unk1C) || w->unk0 >= 0xc8) {
        w->unk0 = 0;
        TimeSelectMenu_StartBrightnessFade(0, 0x10, 0x10, 0);
        gTaskCurrent->func = TimeSelectMenu_State_StartStage;
    }
}

static void TimeSelectMenu_State_StartStage(void) {
    TsWork *w = TASK_WORK(TsWork);
    if (w->unk0 == 8)
        memset(WirelessSession_GetSendBuffer(0), 0, 0x40);
    if (w->unk0 >= 0x10) {
        s32 n = w->unkC;
        u8 a = (u8)(n / 2), b = (u8)(n % 2);
        w->unk0 = 0;
        MsgBox_Destroy();
        Task_DestroyByTag(0x1000);
        SysSound_Release();
        Game_StartVsStage(a, b);
        return;
    }
    TimeSelectMenu_DrawLayoutA(w);
    TimeSelectMenu_DrawLayoutB(w, 0x1000, FALSE);
    w->unk0++;
}

static void TimeSelectMenu_State_ExitToPlayerSelect(void) {
    TsWork *w = TASK_WORK(TsWork);
    if (w->unk0 >= 0x10) {
        w->unk0 = 0;
        MsgBox_Destroy();
        Task_DestroyByTag(0x1000);
        PlayerSelectMenu_StartVs();   /* SceneChange_ToPlayerSelectMenuMode1 (the guest: ..Mode2) */
        return;
    }
    TimeSelectMenu_DrawLayoutA(w);
    TimeSelectMenu_DrawLayoutB(w, 0x1000, TRUE);
    w->unk0++;
}

/* Player 2's TimeSelect (the guest's TimeSelectMenu_StartMode1, the follower) without its screen: its
 * record (the State_Sync handshake with the unlock mask, the "ready" of State_FollowerReady on A)
 * from gPad2, after player 1's task each frame */
typedef struct {
    u32 t;
} TsGuestWork;
static void TimeSelectMenu_LocalGuest_Main(void) {
    TsGuestWork *g = TASK_WORK(TsGuestWork);
    TsWork *w = s_ts;
    MnTask *host = NULL;
    u8 *a = WirelessSession_GetSendBuffer(1), *b = WirelessSession_GetPartnerRecvBuffer(1);
    s32 i;
    for (i = 0; i < MN_MAX_TASKS; i++) {
        const MnTask *t = mn_task_at(i);
        if (t && t->work == (void *)w)
            host = (MnTask *)t;
    }
    if (!w || !host)
        return;
    if (host->func == TimeSelectMenu_State_Sync) {
        /* TimeSelectMenu_State_Sync, w->unk4 == 0 */
        if (++g->t < 8)
            return;
        if (rec16(a, 4) == 0) {
            rec16_set(a, 4, 1);
            rec16_set(a, 6, TimeSelectMenu_GetUnlockedStageMask());
        } else if (rec16(a, 4) == 1) {
            if (rec16(b, 4) == 1)
                rec16_set(a, 4, 2);
        }
        return;
    }
    if (host->func == TimeSelectMenu_State_ChooserReady) {
        /* TimeSelectMenu_State_FollowerReady */
        if (gPad2.trg & PAD_A)
            a[3] = 1;
        return;
    }
    if (host->func == TimeSelectMenu_Main)
        rec16_set(a, 4, 0);
}

/* ------------------------------------------------------------- create */
static const char *const s_ts_act[6] = { "act/mnu_time_act_lc_jpn.bac", "act/mnu_time_act_lc_eng.bac",
                                         "act/mnu_time_act_lc_eng.bac", "act/mnu_time_act_lc_eng.bac",
                                         "act/mnu_time_act_lc_eng.bac", "act/mnu_time_act_lc_eng.bac" };
static const char *const s_ts_zsel[6] = { "act/mnu_conf_zsel_jpn.bac", "act/mnu_conf_zsel_eng.bac",
                                          "act/mnu_conf_zsel_fra.bac", "act/mnu_conf_zsel_deu.bac",
                                          "act/mnu_conf_zsel_ita.bac", "act/mnu_conf_zsel_spa.bac" };

static void TimeSelectMenu_Destructor(void) {
    TsWork *w = TASK_WORK(TsWork);
    s32 i;
    if (s_ts == w)
        s_ts = NULL;
    g_mn_3d.on = 0;
    mn_3d_free();   /* TimeSelectMenu_ReleaseArchives */
    mn_display_reset();
    for (i = 0; i < 6; i++)
        mn_res_free(w->res[i]);
    for (i = 0; i < 3; i++)
        mn_bg_free(w->bg[i]);
    mn_bg_free(w->names);
    free(w->ama);
}

/* TimeSelectMenu_InitMainSprite / InitSubSprite(spr, res, anim, priority, oamPrio) */
static void init_spr(MnSpr *s, const MnRes *r, s32 anim, s32 prio, s32 oam, int engine) {
    AnimatorSprite_Init(s, r, (u16)anim, 0, engine);
    s->flags &= ~0x60000u;
    s->flags |= (u32)prio << 17;
    s->oam = (u8)oam;
}

/* The single 640x480 frame: the top screen's backgrounds (the scrolling mnu_conf_base, the
 * mnu_conf_base02 band) fill the frame at 2.5x; its sprites and the 3D layer (the two players' plates,
 * names, characters and the VS emblem) at 2.0x over the upper 512 x 384; under them the touch screen's
 * stage selector strip (back button, zone emblem, arrows, act label: (0, 0x86)-(256, 0xb8)) at 1.75x;
 * the touch screen's message box (BG0, (0x10, 0x10)-(0xf0, 0x80)) at 1.25x between the characters. */
static void TimeSelectMenu_Compose(void) {
    mn_comp_reset(0);
    mn_comp_add(0, 0, 0, 256, 192, 0, 0, 2.5f, 1 << 0, 1);
    mn_comp_add(0, 0, 0, 256, 192, 64, 0, 2.0f, (1 << 1) | (1 << 2) | (1 << 3), 0);
    mn_comp_add(1, 0, 0x86, 256, 0xb8, 96, 384, 1.75f, 1 << 2, 0);
    mn_comp_add(1, 0x10, 0x10, 0xf0, 0x80, 180, 214, 1.25f, (1 << 1) | (1 << 2) | (1 << 3), 0);
}

static void TimeSelectMenu_Create(s32 a, s32 b) {
    MnTask *task = Task_Create(TimeSelectMenu_State_Sync, TimeSelectMenu_Destructor, 0, 0x1000, 0x1000,
                               sizeof(TsWork));
    TsWork *w = (TsWork *)task->work;
    u8 *p;
    s32 i, lang = mn_lang();
    s_ts = w;
    w->unk0 = 0;
    if (a == 0) {
        w->unk4 = 1;
        w->unk28 = b == 0 ? 0 : 1;
        w->unk2C = b == 0 ? 1 : 0;
    } else {
        w->unk4 = 0;
        w->unk2C = b == 0 ? 0 : 1;
        w->unk28 = b == 0 ? 1 : 0;
    }
    w->unkC = 0;
    /* TimeSelectMenu_InitDisplay */
    mn_display_reset();
    TimeSelectMenu_Compose();
    g_mn_scene = "TimeSelect";
    BRIGHT_MAIN = 0x10;
    BRIGHT_SUB = 0x10;
    /* TimeSelectMenu_LoadAssets: data_ov000_0236995c[lang] */
    w->res[0] = mn_res_load("act/mnu_time_select.bac");
    w->res[1] = mn_res_load(s_ts_act[lang]);
    w->res[2] = mn_res_load("act/mnu_conf_zsel.bac");
    w->res[3] = mn_res_load(s_ts_zsel[lang]);
    w->res[4] = mn_res_load("act/mnu_titl.bac");
    w->ama = mn_ama_load("ama/zone_sel_vs.ama");
    w->bg[0] = mn_bg_load("bg/mnu_conf_base.bbg");
    w->bg[1] = mn_bg_load("bg/mnu_conf_base02.bbg");
    w->bg[2] = mn_bg_load("bg/mnu_time_selectbase.bbg");
    /* TimeSelectMenu_InitSprites */
    init_spr(&w->spr30, w->res[0], 0, 1, 8, 1);
    init_spr(&w->spr78, w->res[0], 2, 1, 2, 1);
    init_spr(&w->sprC0, w->res[0], 2, 1, 2, 1);
    init_spr(&w->spr[0], w->res[0], 5, 1, 6, 1);
    init_spr(&w->spr[1], w->res[1], 0, 1, 4, 1);
    init_spr(&w->spr[2], w->res[4], 0, 1, 4, 1);
    init_spr(&w->spr[3], w->res[2], 5, 1, 4, 1);
    init_spr(&w->spr[4], w->res[3], 1, 1, 2, 1);
    init_spr(&w->spr[5], w->res[3], 2, 1, 2, 1);
    init_spr(&w->spr[6], w->res[2], 0, 0, 4, 0);
    init_spr(&w->spr[7], w->res[2], 1, 0, 4, 0);
    for (i = 0; i < 8; i++)
        init_spr(&w->spr[8 + i], w->res[2], 6 + i, 1, 4, 0);
    init_spr(&w->spr[16], w->res[2], 4, 1, 2, 0);
    init_spr(&w->spr[17], w->res[2], 3, 0, 4, 0);
    init_spr(&w->spr[18], w->res[2], 2, 0, 2, 0);
    init_spr(&w->spr[19], w->res[3], 0, 0, 0, 0);
    /* spr[20]: the link icon (wireless_icon.bac): no wireless link */
    w->anim6F0 = w->anim6F4 = w->anim6F8 = -1;
    for (i = 0; i < 21; i++)
        w->anim[i] = -1;
    /* TimeSelectMenu_InitBackgrounds */
    Background_Show(0, 1, w->bg[1]);
    Background_Show(0, 2, w->bg[0]);
    Background_Show(1, 1, w->bg[2]);
    Background_Show(1, 2, w->bg[0]);
    /* TimeSelectMenu_InitKeyframeAnims: objects 0..5 = sequences 0..5, 6..17 = 7..18 */
    for (i = 0; i < 18; i++)
        KeyframeAnim_Init(&w->pos[i], w->ama, (u32)(i < 6 ? i : i + 1));
    /* TimeSelectMenu_LoadArchives: the 3D layer on main BG0 */
    g_mn_3d.on = 1;
    g_mn_3d.engine = 0;
    TimeSelectMenu_SetSprite30Anim0(w);
    TimeSelectMenu_SetSprite0Anim(w, 0);
    TimeSelectMenu_SetSprite1Anim(w, 0, 0);
    TimeSelectMenu_SetSprite2Anim0(w);
    TimeSelectMenu_SetSprite6Anim0(w);
    TimeSelectMenu_SetSprite7Anim1(w);
    TimeSelectMenu_SetSprites8To11(w);
    TimeSelectMenu_SetSprites12To15(w);
    TimeSelectMenu_SetSprite16Anim4(w);
    TimeSelectMenu_SetSprite17Anim3(w);
    TimeSelectMenu_SetSprite19Anim0(w);
    {   /* TimeSelectMenu_LoadPlayerNames: row 0 the host (player 1), row 1 the guest; ink black
         * (PaletteTransfer 0x500039e: OBJ palette 0xc colour 15 = 0) */
        const u16 *nm[2];
        int len[2];
        u16 ink[2] = { 0, 0 };
        len[0] = mn_vs_player_name(0, &nm[0]);
        len[1] = mn_vs_player_name(1, &nm[1]);
        w->names = mn_name_strips(nm, len, 2, ink);
    }
    p = WirelessSession_GetSendBuffer(0);
    memset(p, 0, 8);
    p = WirelessSession_GetPartnerRecvBuffer(0);
    memset(p, 0, 8);
    Task_Create(TimeSelectMenu_ScrollBg_Main, NULL, 0, 0x3000, 0x1000, 0);
    Task_Create(TimeSelectMenu_LocalGuest_Main, NULL, 0, 0x1001, 0x1000, sizeof(TsGuestWork));
    gSaveDataBusy = 1;   /* gSystemFlags |= 0x10 */
}

/* SceneChange_ToTimeSelectMenuMode0 (host): TimeSelectMenu_Create(0, gGameState.unk4 == 1) */
void TimeSelectMenu_StartMode0(void) { TimeSelectMenu_Create(0, TimeSelectMenu_IsMode0Flag()); }

/* host test access */
int TimeSelectMenu_Selected(void) { return s_ts ? s_ts->unkC : -1; }
int TimeSelectMenu_StateId(void) {
    int i;
    for (i = 0; i < MN_MAX_TASKS; i++) {
        const MnTask *t = mn_task_at(i);
        if (!t || !s_ts || t->work != (void *)s_ts)
            continue;
        if (t->func == TimeSelectMenu_State_Sync)
            return 0;
        if (t->func == TimeSelectMenu_Main)
            return 1;
        if (t->func == TimeSelectMenu_State_Select || t->func == TimeSelectMenu_State_WaitScroll)
            return 2;
        if (t->func == TimeSelectMenu_State_ChooserConfirmed)
            return 3;
        if (t->func == TimeSelectMenu_State_ChooserReady)
            return 4;
        if (t->func == TimeSelectMenu_State_StartStage)
            return 5;
        return 6;
    }
    return -1;
}

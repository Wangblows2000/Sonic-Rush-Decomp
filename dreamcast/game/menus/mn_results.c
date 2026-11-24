/*
 * ResultsScreen_* (src/game/Menus/Results/, 77 functions, overlay ov000): the Battle Play results
 * after a race (SceneChange_ToVsResults): both players' characters with WIN / LOSE (or both DRAW),
 * then the host chooses REMATCH (the same stage again, SceneChange_ToVsStage), the character select
 * (SceneChange_ToPlayerSelectMenuMode1) or QUIT (MsgBox 9 YES / NO, then the title screen); the
 * guest's unit follows the host's choice (ResultsScreen_Main_WaitPartnerChoice). The result goes to
 * the VS record (ResultsScreen_SaveVsResult -> SaveData_RecordVsResult) and the save is committed.
 *
 * One Dreamcast: the host's screen (player 1, gPad) is the screen of both; player 2's unit only
 * follows the host (it chooses nothing on the DS either), its side of the records is the State_Sync
 * handshake (ResultsScreen_LocalGuest_Main). The winner comes from menus_vs_exit
 * (data_022c4590.unk40 -> ResultsScreen_GetResultKind: 0 player 1, 1 player 2, 2 draw).
 *
 * DS display (ResultsScreen_InitDisplay: POWCNT swap, the main engine is the top screen):
 *   main (top): BG0 3D (the two models: win / lose poses of menu.narc), BG2 mnu_conf_base02, BG3
 *     mnu_conf_base (scrolls 2 px/frame), sprites (mnu_conf_zsel, mnu_conf_result_<lang>): name
 *     plates and names, the characters' groups, the WIN / LOSE / DRAW stamps
 *   sub (bottom, touch): BG2 mnu_conf_result (the choice window: opened and closed by per-line HOFS,
 *     the upper 0x54 lines by -v and the rest by +v, HDMA), BG3 mnu_conf_base, sprites: the three
 *     choice buttons and the cursor (mnu_conf_result), MsgBox 9 on sub BG0
 * Single screen: ResultsScreen_Compose (MENUS.md 15).
 *
 * DS service                          here
 * HDMA BG2HOFS (EnableLineScroll*, SetWindowLineScroll)  the window as two row bands (mn_emit_bg_part)
 * WirelessSession records             mn_vs.c
 * gTouchState buttons                 pad only (the original's pad path: UP / DOWN repeat, A)
 * OS_GetMacAddress (VS record key)    zero; the opponent's name is player 2's (menus_set_player2_name)
 * link icon, RematchSync / StageStart (download play), link lost message (ShowLinkLostMessage)
 *                                     not translated
 */
#include <stdlib.h>
#include "mn_internal.h"

typedef struct {
    s32 set, jidx, vidx, loop;
    s32 frame;
} RsModel;

typedef struct {
    u32 unk0;        /* frame counter of the state */
    s32 unk4;        /* cursor blink counter */
    s32 unk8;        /* choice: 0 rematch, 1 character select, 2 quit */
    s32 unkC;        /* quit (fade to black) */
    s32 unk10;       /* 1: the host (ResultsScreen_Unk_ov000_0235cbd8) */
    s32 unk14;       /* the brightness the fade-in starts from */
    s32 unk18, unk1C;   /* the host's / the guest's character */
    s32 unk20;       /* the result: 0 the host won, 1 the guest won, 2 draw */
    MnSpr spr[18];   /* +0x24 */
    s32 anim[18];    /* +0x534 */
    MnRes *res[3];   /* +0xe14: mnu_conf_result, mnu_conf_result_<lang>, mnu_conf_zsel */
    MnBgImg *bg[3];  /* conf_base, conf_base02, conf_result */
    MnBgImg *names;
    s32 lines;       /* gSystemFlags & 0x400: sub BG2 drawn as the two row bands */
    s32 line_v;      /* the bands' shift */
    s32 bg2_on;      /* DISPCNT BG2 of the sub engine */
    RsModel mdl[2];  /* rA (Sonic, the 0x67c model) / rB (Blaze) */
    u8 unkC14[6];    /* the partner's MAC address (VS record key) */
} RsWork;

static RsWork *s_rs;
static void ResultsScreen_State_FadeIn(void);
static void ResultsScreen_State_ShowResult(void);
static void ResultsScreen_Main_CloseWindow(void);
static void ResultsScreen_State_Choose(void);
static void ResultsScreen_Main_DelayOpenWindowNoSprite16(void);
static void ResultsScreen_Main_OpenWindow(void);
static void ResultsScreen_State_QuitPrompt(void);
static void ResultsScreen_State_FadeOutExit(void);

/* ResultsScreen_EaseValue */
static s32 ResultsScreen_EaseValue(s32 from, s32 to, s32 n, s32 i, fx32 ease) {
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

static void ResultsScreen_ScrollBg_Main(void) {
    gDisplayMain[3].x += 2;   /* gDisplayMain.unkc */
    gDisplaySub[3].x += 2;
}

/* ------------------------------------------------------- window line scroll */
static void ResultsScreen_EnableLineScroll(RsWork *w) {   /* BG2 on, all lines 0 */
    w->bg2_on = 1;
    w->lines = 1;
    w->line_v = 0;
}
static void ResultsScreen_EnableLineScrollSplit(RsWork *w) {   /* BG2 off, lines -0x44 / +0x44 */
    w->bg2_on = 0;
    w->lines = 1;
    w->line_v = 0x44;
}
static void ResultsScreen_SetWindowLineScroll(RsWork *w, s32 v) {
    w->bg2_on = v <= 0x3c;
    w->line_v = v;
}
static void ResultsScreen_DisableLineScroll(RsWork *w) {   /* BG2 on at (0, 0) */
    w->lines = 0;
    w->bg2_on = 1;
    gDisplaySub[2].x = 0;
    gDisplaySub[2].y = 0;
}
static void ResultsScreen_DisableLineScrollHidden(RsWork *w) {   /* BG2 off, scrolled away */
    w->lines = 0;
    w->bg2_on = 0;
    gDisplaySub[2].x = 0;
    gDisplaySub[2].y = 0xc0;
}
/* sub BG2 this frame: the layer itself, or (line scroll) the two row bands, each shifted with the
 * 256-px wrap of the BG plane; the bands are emitted after the sprites (behind those of priority 2) */
static void ResultsScreen_EmitWindow(RsWork *w) {
    MnBgImg *img = w->bg[2];
    s32 band, k;
    if (!w->lines) {
        if (w->bg2_on)
            Background_Show(1, 2, img);
        else
            Background_Hide(1, 2);
        return;
    }
    Background_Hide(1, 2);
    if (!w->bg2_on || !img)
        return;
    for (band = 0; band < 2; band++) {
        s32 y0 = band ? 0x54 : 0, h = band ? 0xc0 - 0x54 : 0x54;
        s32 shift = band ? -w->line_v : w->line_v;   /* HOFS -v: the content moves right by v */
        if (h > img->h - y0)
            h = img->h - y0;
        if (h <= 0)
            continue;
        for (k = -1; k <= 1; k++)
            mn_emit_bg_part(1, g_mn_layer[1][2].prio, img, (float)(shift + k * 256), (float)y0, 0, (float)y0,
                            img->w < 256 ? img->w : 256, (float)h);
    }
}

/* ----------------------------------------------------- partner records */
static void ResultsScreen_SetLocalChoice(RsWork *w, s32 v) { (void)w; WirelessSession_GetSendBuffer(0)[0] = (u8)v; }
static void ResultsScreen_SetLocalDecided(RsWork *w) { (void)w; WirelessSession_GetSendBuffer(0)[1] = 1; }
static void ResultsScreen_ClearLocalDecided(RsWork *w) { (void)w; WirelessSession_GetSendBuffer(0)[1] = 0; }
static void ResultsScreen_SetLocalQuit(RsWork *w) { (void)w; WirelessSession_GetSendBuffer(0)[2] = 1; }

/* ------------------------------------------------------------ drawing */
static void set_anim(RsWork *w, s32 i, s32 a) {
    if (w->anim[i] != a) {
        w->anim[i] = a;
        AnimatorSprite_SetAnimation(&w->spr[i], (u16)a);
    }
}
static void ResultsScreen_DrawPlayerName(RsWork *w, s32 x, s32 y, s32 row) {
    s32 i;
    if (!w->names)
        return;
    for (i = 0; i < 8; i++, x += 8)   /* 7 glyphs (an unnamed player's identifier: 8) */
        if (x >= -8 && x < 0x100 && y >= -8 && y < 0xc0)
            mn_emit_bg_part(0, 0, w->names, (float)x, (float)y, (float)(i * 8), (float)(row * 16), 8, 8);
}
static void ResultsScreen_DrawHeader(RsWork *w) {
    MnSpr *spr = &w->spr[0];
    ResultsScreen_DrawPlayerName(w, 0x1c, 0xb, 0);
    ResultsScreen_DrawPlayerName(w, 0xac, 0xb, 1);
    set_anim(w, 0, 3);
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    spr->x = 8;
    spr->y = 0;
    AnimatorSprite_Draw(spr);
    spr->x = 0x98;
    spr->y = 0;
    AnimatorSprite_Draw(spr);
}
static void ResultsScreen_DrawSprite1Pair(RsWork *w) {
    MnSpr *spr = &w->spr[1];
    set_anim(w, 1, 4);
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    spr->x = 8;
    spr->y = 0x88;
    AnimatorSprite_Draw(spr);
    spr->x = 0x98;
    spr->y = 0x88;
    AnimatorSprite_Draw(spr);
}
static void ResultsScreen_DrawSprite2(RsWork *w) {   /* the characters' groups */
    s32 i, a;
    a = w->unk18 == 0 ? 6 : 10;
    for (i = 0; i < 4; i++, a++) {
        set_anim(w, 2 + i, a);
        w->spr[2 + i].x = 0x38;
        w->spr[2 + i].y = 0x58;
        AnimatorSprite_ProcessAnimation(&w->spr[2 + i], 0, 0);
        AnimatorSprite_Draw(&w->spr[2 + i]);
    }
    a = w->unk1C == 0 ? 6 : 10;
    for (i = 0; i < 4; i++, a++) {
        set_anim(w, 6 + i, a);
        w->spr[6 + i].x = 0xc8;
        w->spr[6 + i].y = 0x58;
        AnimatorSprite_ProcessAnimation(&w->spr[6 + i], 0, 0);
        AnimatorSprite_Draw(&w->spr[6 + i]);
    }
}
static void ResultsScreen_DrawSprite16(RsWork *w) {   /* the guest's "waiting" plate */
    set_anim(w, 16, 12);
    AnimatorSprite_ProcessAnimation(&w->spr[16], 0, 0);
    AnimatorSprite_Draw(&w->spr[16]);
}
static void ResultsScreen_DrawWinLoseSprites(RsWork *w, s32 sel, s32 scale) {
    s32 a, b;
    switch (sel) {
    case 0:
        a = 9;
        b = 10;
        break;
    case 1:
        a = 10;
        b = 9;
        break;
    default:
        a = 11;
        b = 11;
        break;
    }
    set_anim(w, 10, a);
    AnimatorSprite_ProcessAnimation(&w->spr[10], 0, 0);
    w->spr[10].x = 4;
    w->spr[10].y = 0xc0;
    AnimatorSprite_DrawScaled(&w->spr[10], 0x1000, scale);
    set_anim(w, 11, b);
    AnimatorSprite_ProcessAnimation(&w->spr[11], 0, 0);
    w->spr[11].x = 0x94;
    w->spr[11].y = 0xc0;
    AnimatorSprite_DrawScaled(&w->spr[11], 0x1000, scale);
}
static void ResultsScreen_DrawChoiceSprites(RsWork *w, BOOL flag) {
    s32 i, a;
    for (i = 0; i < 3; i++) {
        if (i == w->unk8) {
            MnSpr *spr = &w->spr[12];
            if (w->unk4 < 6) {
                a = 1;
                spr->flags |= SPR_LOOP;
            } else {
                a = 0;
                spr->flags &= ~(u32)SPR_LOOP;
            }
            set_anim(w, 12, a);
            spr->x = 0x30;
            spr->y = (s16)(0x28 + i * 0x20);
            AnimatorSprite_ProcessAnimation(spr, 0, 0);
            AnimatorSprite_Draw(spr);
        }
        a = i * 3;
        if (i == w->unk8) {
            if (flag) {
                a += 2;
                w->spr[13 + i].flags |= SPR_LOOP;
            } else {
                a += 1;
                w->spr[13 + i].flags &= ~(u32)SPR_LOOP;
            }
        } else {
            w->spr[13 + i].flags &= ~(u32)SPR_LOOP;
        }
        set_anim(w, 13 + i, a);
        AnimatorSprite_ProcessAnimation(&w->spr[13 + i], 0, 0);
        AnimatorSprite_Draw(&w->spr[13 + i]);
    }
}

/* ResultsScreen_SetModelAnimA (Sonic) / B (Blaze): sel 0 the idle loop of player_lz7 (joint 0 / 0x37,
 * visibility 1), 1 the win, 2 the lose pose of menu.narc (Sonic 2 / 3, Blaze 6 / 7), held */
static void ResultsScreen_SetModelAnim(RsWork *w, s32 chara, s32 sel) {
    RsModel *m = &w->mdl[chara];
    m->frame = 0;
    switch (sel) {
    case 1:
    case 2:
        m->set = 1;
        m->jidx = m->vidx = (chara ? 6 : 2) + (sel - 1);
        m->loop = 0;
        break;
    default:
        m->set = 0;
        m->jidx = chara ? 0x37 : 0;
        m->vidx = 1;
        m->loop = 1;
        break;
    }
}
#define ResultsScreen_SetModelAnimA(w, sel) ResultsScreen_SetModelAnim(w, 0, sel)
#define ResultsScreen_SetModelAnimB(w, sel) ResultsScreen_SetModelAnim(w, 1, sel)

/* ResultsScreen_SetupOrthoCamera + DrawModelA / B (as TimeSelectMenu's: Sonic 0xd000, Blaze 0xc800 /
 * 0xc000, the left one mirrored) */
static void ResultsScreen_DrawModel(RsWork *w, s32 chara, s32 x, s32 y, BOOL flip) {
    RsModel *m = &w->mdl[chara];
    MnModelDesc d;
    memset(&d, 0, sizeof(d));
    d.mdl = chara;
    d.set = m->set;
    d.jidx = m->jidx;
    d.vidx = m->vidx;
    d.frame = m->frame;
    d.loop = m->loop;
    d.alpha = 31;
    d.t = 95.0f;
    d.b = -96.0f;
    d.l = -128.0f;
    d.r = 127.0f;
    d.n = 1.0f;
    d.f = 200.0f;
    d.at[2] = -1.0f;
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
    d.cull = 0;
    d.diff = 0x7fff;
    d.amb = 0x6f7b;
    d.nlights = 1;
    d.lcol[0] = 0x7fff;
    d.lvec[0][0] = flip ? 0xfff : -0x1000;
    d.lvec[0][1] = -0x1000;
    d.lvec[0][2] = -0x2000;
    mn_3d_add(&d);
    m->frame += 0x1000;   /* Model_Animate */
}
static void ResultsScreen_DrawModels(RsWork *w) {
    ResultsScreen_DrawModel(w, w->unk18 == 0 ? 0 : 1, 0x38, 0x90, TRUE);
    ResultsScreen_DrawModel(w, w->unk1C == 0 ? 0 : 1, 0xc8, 0x90, FALSE);
}
static void ResultsScreen_DrawCommon(RsWork *w) {
    ResultsScreen_DrawHeader(w);
    ResultsScreen_DrawSprite2(w);
    ResultsScreen_DrawSprite1Pair(w);
    ResultsScreen_DrawModels(w);
}

/* ---------------------------------------------------------------- exits */
/* ResultsScreen_SaveVsResult: the record from the host's side (0 won, 1 lost, 2 draw) against player 2 (Vs CPU:
 * against "CPU", an opponent of its own: mn_vs_cpu_record) */
static void ResultsScreen_SaveVsResult(s32 mode, const u8 *mac) {
    const u16 *name;
    int len = mn_vs_cpu() ? mn_vs_cpu_record(&name, &mac) : mn_player2_name(&name);   /* Vs CPU: "CPU" */
    s32 v;
    switch (mode) {
    case 0:
        v = 0;
        break;
    case 1:
        v = 1;
        break;
    default:
        v = 2;
        break;
    }
    SaveData_RecordVsResult(v, name, (u8)(len > 10 ? 10 : len), mac);
}

static void ResultsScreen_Exit(s32 mode) {
    switch (mode) {
    case 0:   /* rematch: SceneChange_ToVsStage with the same stage and characters */
        SysSound_Release();
        mn_request_vs_stage();
        break;
    case 1:   /* SceneChange_ToPlayerSelectMenuMode1 */
        SysSound_Release();
        PlayerSelectMenu_StartVs();
        break;
    case 2:   /* WirelessSession_EndWhenAlone(SceneChange_ToTitleScreen) */
        SysSound_Release();
        TitleScreen_Start();
        break;
    }
}

/* --------------------------------------------------------------- states */
static void ResultsScreen_State_Sync(void) {
    RsWork *w = TASK_WORK(RsWork);
    u8 *a = WirelessSession_GetSendBuffer(0), *b = WirelessSession_GetPartnerRecvBuffer(0);
    u8 *c = a;   /* WirelessSession_GetOwnRecvBuffer */
    BOOL go = FALSE;
    if (a[3] == 0 && b[3] == 0)
        a[3] = 1;
    else if (a[3] == 1) {
        if (b[3] == 1)
            a[3] = 2;
    } else if (a[3] == 2) {
        if (b[3] == 2)
            a[3] = 3;
    } else if (c[3] >= 3) {
        go = TRUE;
    }
    (void)w;
    if (go) {
        Task_Create(ResultsScreen_ScrollBg_Main, NULL, 0, 0x2000, 0x1000, 0);
        SysSound_PlayBGM(3);   /* SysSound_Init; SysSound_PlayBGM(3) */
        gSaveDataBusy = 1;     /* gSystemFlags |= 0x10 */
        gTaskCurrent->func = ResultsScreen_State_FadeIn;
    }
}

static void ResultsScreen_State_FadeIn(void) {
    RsWork *w = TASK_WORK(RsWork);
    ResultsScreen_DrawCommon(w);
    if ((s32)w->unk0 >= 0) {
        s16 v = (s16)ResultsScreen_EaseValue(w->unk14, 0, 0x10, (s32)w->unk0, 0x1000);
        BRIGHT_MAIN = v;
        BRIGHT_SUB = v;
    }
    ResultsScreen_EmitWindow(w);
    w->unk0++;
    if (w->unk0 >= 0x20) {
        w->unk0 = 0;
        gTaskCurrent->func = ResultsScreen_State_ShowResult;
    }
}

static void ResultsScreen_State_ShowResult(void) {
    RsWork *w = TASK_WORK(RsWork);
    s32 t;
    ResultsScreen_DrawHeader(w);
    ResultsScreen_DrawSprite2(w);
    ResultsScreen_DrawSprite1Pair(w);
    if (w->unk0 == 0) {
        switch (w->unk20) {
        case 0:
            if (w->unk18 == 0) {
                ResultsScreen_SetModelAnimA(w, 1);
                ResultsScreen_SetModelAnimB(w, 2);
            } else {
                ResultsScreen_SetModelAnimA(w, 2);
                ResultsScreen_SetModelAnimB(w, 1);
            }
            break;
        case 1:
            if (w->unk18 == 0) {
                ResultsScreen_SetModelAnimA(w, 2);
                ResultsScreen_SetModelAnimB(w, 1);
            } else {
                ResultsScreen_SetModelAnimA(w, 1);
                ResultsScreen_SetModelAnimB(w, 2);
            }
            break;
        case 2:
            ResultsScreen_SetModelAnimA(w, 2);
            ResultsScreen_SetModelAnimB(w, 2);
            break;
        }
    }
    ResultsScreen_DrawModels(w);
    t = (s32)w->unk0 - 0x38;
    if (t >= 0)
        ResultsScreen_DrawWinLoseSprites(w, w->unk20, ResultsScreen_EaseValue(0, 0x1000, 8, t, 0x1000));
    ResultsScreen_EmitWindow(w);
    w->unk0++;
    if (w->unk0 >= 0x40) {
        w->unk0 = 0;
        WirelessSession_GetSendBuffer(0)[3] = 0;
        ResultsScreen_EnableLineScrollSplit(w);
        gTaskCurrent->func = ResultsScreen_Main_CloseWindow;
    }
}

/* the window closes in (the choices appear) */
static void ResultsScreen_Main_CloseWindow(void) {
    RsWork *w = TASK_WORK(RsWork);
    ResultsScreen_DrawCommon(w);
    ResultsScreen_DrawWinLoseSprites(w, w->unk20, 0x1000);
    ResultsScreen_SetWindowLineScroll(w, ResultsScreen_EaseValue(0x44, 0, 0xa, (s32)w->unk0, 0));
    if (w->unk10 == 0)
        ResultsScreen_DrawSprite16(w);
    ResultsScreen_EmitWindow(w);
    w->unk0++;
    if (w->unk0 >= 0xa) {
        w->unk0 = 0;
        w->unk4 = 0x64;
        w->unk8 = 0;
        ResultsScreen_DisableLineScroll(w);
        /* (the guest: ResultsScreen_Main_WaitPartnerChoice) */
        gTaskCurrent->func = ResultsScreen_State_Choose;
    }
}

static void ResultsScreen_State_Choose(void) {
    RsWork *w = TASK_WORK(RsWork);
    BOOL done = FALSE;
    ResultsScreen_DrawCommon(w);
    ResultsScreen_DrawWinLoseSprites(w, w->unk20, 0x1000);
    ResultsScreen_DrawChoiceSprites(w, FALSE);
    if (gPad.trg & PAD_A) {
        done = TRUE;
    } else if (gPad.rep & PAD_UP) {
        w->unk8--;
        if (w->unk8 < 0)
            w->unk8 = 2;
        w->unk4 = 0;
    } else if (gPad.rep & PAD_DOWN) {
        w->unk8++;
        if (w->unk8 >= 3)
            w->unk8 = 0;
        w->unk4 = 0;
    }
    ResultsScreen_SetLocalChoice(w, w->unk8);
    ResultsScreen_EmitWindow(w);
    w->unk0++;
    w->unk4++;
    if (done) {
        memcpy(w->unkC14, WirelessSession_GetPartnerRecvBuffer(0) + 6, 6);
        SysSound_PlaySE(7);
        w->unk0 = 0;
        ResultsScreen_SetLocalDecided(w);
        gTaskCurrent->func = ResultsScreen_Main_DelayOpenWindowNoSprite16;
    }
}

static void ResultsScreen_Main_DelayOpenWindowNoSprite16(void) {
    RsWork *w = TASK_WORK(RsWork);
    ResultsScreen_DrawCommon(w);
    ResultsScreen_DrawWinLoseSprites(w, w->unk20, 0x1000);
    ResultsScreen_DrawChoiceSprites(w, TRUE);
    ResultsScreen_EmitWindow(w);
    w->unk0++;
    if (w->unk0 >= 0x10) {
        w->unk0 = 0;
        ResultsScreen_EnableLineScroll(w);
        gTaskCurrent->func = ResultsScreen_Main_OpenWindow;
    }
}

/* the window opens out (the choices go) */
static void ResultsScreen_Main_OpenWindow(void) {
    RsWork *w = TASK_WORK(RsWork);
    ResultsScreen_DrawCommon(w);
    ResultsScreen_DrawWinLoseSprites(w, w->unk20, 0x1000);
    ResultsScreen_SetWindowLineScroll(w, ResultsScreen_EaseValue(0, 0x44, 0xa, (s32)w->unk0, 0x2000));
    if (w->unk10 == 0 && w->unk8 == 2)
        ResultsScreen_DrawSprite16(w);
    ResultsScreen_EmitWindow(w);
    w->unk0++;
    if (w->unk0 >= 0xa) {
        w->unk0 = 0;
        ResultsScreen_DisableLineScrollHidden(w);
        if (w->unk10 != 0) {
            if (w->unk8 == 2) {
                MsgBox_Create(9, 3, 1, 1, 0, 0, 0xe, 0x200010);
                gTaskCurrent->func = ResultsScreen_State_QuitPrompt;
                return;
            }
            gTaskCurrent->func = ResultsScreen_State_FadeOutExit;
            return;
        }
        gTaskCurrent->func = ResultsScreen_State_FadeOutExit;
    }
}

static void ResultsScreen_State_QuitPrompt(void) {
    RsWork *w = TASK_WORK(RsWork);
    ResultsScreen_DrawCommon(w);
    ResultsScreen_DrawWinLoseSprites(w, w->unk20, 0x1000);
    ResultsScreen_EmitWindow(w);
    w->unk0++;
    if (MsgBox_IsClosed()) {
        w->unk0 = 0;
        if (!MsgBox_GetResult()) {
            w->unkC = 1;
            ResultsScreen_SetLocalQuit(w);
            gTaskCurrent->func = ResultsScreen_State_FadeOutExit;
        } else {
            ResultsScreen_ClearLocalDecided(w);
            ResultsScreen_EnableLineScrollSplit(w);
            gTaskCurrent->func = ResultsScreen_Main_CloseWindow;
        }
    }
}

static void ResultsScreen_State_FadeOutExit(void) {
    RsWork *w = TASK_WORK(RsWork);
    s32 t, target, next;
    s16 b;
    ResultsScreen_DrawCommon(w);
    ResultsScreen_DrawWinLoseSprites(w, w->unk20, 0x1000);
    ResultsScreen_EmitWindow(w);
    t = (s32)w->unk0;
    target = w->unkC != 0 ? -0x10 : 0x10;
    if (t >= 0) {
        b = (s16)ResultsScreen_EaseValue(0, target, 0x10, t, 0x1000);
        BRIGHT_MAIN = b;
        BRIGHT_SUB = b;
    }
    w->unk0++;
    if (w->unk0 >= 0x10) {
        w->unk0 = 0;
        next = w->unk8;
        b = (s16)target;
        BRIGHT_MAIN = b;
        BRIGHT_SUB = b;
        ResultsScreen_SaveVsResult(w->unk20, w->unkC14);
        SaveData_CommitOrShowError(NULL);
        Task_DestroyAll();
        ResultsScreen_Exit(next);   /* the host (ResultsScreen_Unk_ov000_0235cbd8) */
    }
}

/* Player 2's ResultsScreen (the guest's) without its screen: its side of the State_Sync handshake */
static void ResultsScreen_LocalGuest_Main(void) {
    u8 *a = WirelessSession_GetSendBuffer(1), *b = WirelessSession_GetPartnerRecvBuffer(1);
    if (b[3] == 0)
        a[3] = 0;
    if (a[3] == 0) {
        if (b[3] == 1)
            a[3] = 1;
    } else if (a[3] == 1) {
        if (b[3] == 2)
            a[3] = 2;
    } else if (a[3] == 2) {
        if (b[3] == 3)
            Task_Destroy(gTaskCurrent);   /* the guest's State_FadeIn: nothing more in its record */
    }
}

/* ------------------------------------------------------------- create */
static const char *const s_rs_lang[6] = { "act/mnu_conf_result_jpn.bac", "act/mnu_conf_result_eng.bac",
                                          "act/mnu_conf_result_fra.bac", "act/mnu_conf_result_deu.bac",
                                          "act/mnu_conf_result_ita.bac", "act/mnu_conf_result_spa.bac" };

static void ResultsScreen_Destructor(void) {
    RsWork *w = TASK_WORK(RsWork);
    s32 i;
    if (s_rs == w)
        s_rs = NULL;
    g_mn_3d.on = 0;
    mn_3d_free();   /* ResultsScreen_ReleaseArchives */
    mn_display_reset();   /* ResultsScreen_ResetBgOffsets */
    for (i = 0; i < 3; i++)
        mn_res_free(w->res[i]);
    for (i = 0; i < 3; i++)
        mn_bg_free(w->bg[i]);
    mn_bg_free(w->names);
}

/* ResultsScreen_InitMainSprite / InitSubSprite(spr, res, anim, priority, palette, oamPrio) */
static void init_spr(MnSpr *s, const MnRes *r, s32 anim, s32 prio, s32 oam, int engine) {
    AnimatorSprite_Init(s, r, (u16)anim, 0, engine);
    s->flags &= ~0x60000u;
    s->flags |= (u32)prio << 17;
    s->oam = (u8)oam;
}

/* The single 640x480 frame: the top screen (players, characters, WIN / LOSE / DRAW) fills the frame
 * at 2.5x; the touch screen's choice window ((0x10, 0x10)-(0xf0, 0x98): mnu_conf_result on BG2 with
 * the buttons and the cursor, the quit message box on BG0; the scrolling BG3 left out) opens over its
 * centre at 1.5x while the host chooses. */
static void ResultsScreen_Compose(void) {
    mn_comp_reset(0);
    mn_comp_full(0);
    mn_comp_add(1, 0x10, 0x10, 0xf0, 0x98, 152, 138, 1.5f, 1 << 3, 0);
}

/* ResultsScreen_Create: CreateTask(the host's character, the guest's, the result kind), InitDisplay,
 * LoadSprites, InitSprites, InitBackgrounds, LoadArchives, SetModelAnimA / B(0), LoadPlayerNames */
void ResultsScreen_Create(void) {
    MnTask *task = Task_Create(ResultsScreen_State_Sync, ResultsScreen_Destructor, 0, 0x1000, 0x1000, sizeof(RsWork));
    RsWork *w = (RsWork *)task->work;
    s32 i;
    s_rs = w;
    w->unk18 = gGameState.unk4 != 0;   /* ResultsScreen_Unk_ov000_0235cc60 */
    w->unk1C = gGameState.unk8 != 0;   /* ResultsScreen_Unk_ov000_0235cc4c */
    if (w->unk1C == w->unk18) {   /* never one character for both players (menus_vs_prepare) */
        w->unk1C = w->unk18 ^ 1;
        gGameState.unk8 = w->unk1C;
    }
    w->unk20 = gGameState.vs_winner;   /* ResultsScreen_GetResultKind */
    w->unk10 = 1;
    w->unk14 = BRIGHT_MAIN;
    if (w->unk14 == 0)
        w->unk14 = -0x10;
    mn_vs_reset_records();   /* the local record (12 bytes) and the guest's (its StartStage clear) */
    BRIGHT_MAIN = -0x10;
    BRIGHT_SUB = -0x10;
    /* ResultsScreen_InitDisplay */
    mn_display_reset();
    ResultsScreen_Compose();
    g_mn_scene = "Results";
    /* ResultsScreen_LoadSprites: data_ov000_023692b4 / data_ov000_023692d4[lang] */
    w->res[0] = mn_res_load("act/mnu_conf_result.bac");
    w->res[1] = mn_res_load(s_rs_lang[mn_lang()]);
    w->res[2] = mn_res_load("act/mnu_conf_zsel.bac");
    w->bg[0] = mn_bg_load("bg/mnu_conf_base.bbg");
    w->bg[1] = mn_bg_load("bg/mnu_conf_base02.bbg");
    w->bg[2] = mn_bg_load("bg/mnu_conf_result.bbg");
    /* ResultsScreen_InitSprites */
    init_spr(&w->spr[0], w->res[2], 3, 0, 0, 0);
    init_spr(&w->spr[1], w->res[2], 4, 2, 0, 0);
    for (i = 0; i < 4; i++) {
        init_spr(&w->spr[2 + i], w->res[2], 6 + i, 2, 4, 0);
        init_spr(&w->spr[6 + i], w->res[2], 6 + i, 2, 4, 0);
    }
    init_spr(&w->spr[10], w->res[1], 0xb, 0, 0, 0);
    init_spr(&w->spr[11], w->res[1], 0xb, 0, 0, 0);
    init_spr(&w->spr[12], w->res[0], 0, 2, 0, 1);
    init_spr(&w->spr[13], w->res[1], 0, 2, 4, 1);
    init_spr(&w->spr[14], w->res[1], 3, 2, 4, 1);
    init_spr(&w->spr[15], w->res[1], 6, 2, 4, 1);
    init_spr(&w->spr[16], w->res[1], 0xc, 2, 4, 1);
    /* spr[17]: the link icon (wireless_icon.bac): no wireless link */
    for (i = 0; i < 18; i++)
        w->anim[i] = -1;
    /* ResultsScreen_InitBackgrounds: main / sub BG3 mnu_conf_base, main BG2 mnu_conf_base02, sub BG2
     * mnu_conf_result (DISPCNT: sub BG2 off) */
    Background_Show(0, 3, w->bg[0]);
    Background_Show(1, 3, w->bg[0]);
    Background_Show(0, 2, w->bg[1]);
    w->bg2_on = 0;
    /* ResultsScreen_LoadArchives: the 3D layer on main BG0 */
    g_mn_3d.on = 1;
    g_mn_3d.engine = 0;
    ResultsScreen_SetModelAnimA(w, 0);
    ResultsScreen_SetModelAnimB(w, 0);
    {   /* ResultsScreen_LoadPlayerNames: row 0 the host (player 1), row 1 the guest (player 2), black */
        const u16 *nm[2];
        int len[2];
        u16 ink[2] = { 0, 0 };
        len[0] = mn_vs_player_name(0, &nm[0]);
        len[1] = mn_vs_player_name(1, &nm[1]);
        w->names = mn_name_strips(nm, len, 2, ink);
    }
    Task_Create(ResultsScreen_LocalGuest_Main, NULL, 0, 0x1001, 0x1000, 0);
}

/* host test access: the choice (0 rematch, 1 character select, 2 quit), -1 when not choosing */
int ResultsScreen_Choice(void) {
    int i;
    for (i = 0; i < MN_MAX_TASKS; i++) {
        const MnTask *t = mn_task_at(i);
        if (t && s_rs && t->work == (void *)s_rs && t->func == ResultsScreen_State_Choose)
            return s_rs->unk8;
    }
    return -1;
}

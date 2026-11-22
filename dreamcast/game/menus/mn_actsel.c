/*
 * ZoneActSelect_* and ZoneActSelectMode_* (src/game/Menus/ZoneMap/): the
 * zone tabs + act list with best scores and ranks, opened from the zone map
 * on a cleared zone (and alone, from GAME START at story progress 1).
 *
 * Touch -> pad: zone tabs = LEFT/RIGHT, act rows = UP/DOWN + A, back button
 * = B (all present in the original pad path). The per-sprite palette numbers
 * (AnimatorSprite unk44: tab colours, highlighted digits) are not applied: the
 * converted BAC frames carry their own palette.
 */
#include <stdlib.h>
#include "mn_internal.h"

typedef struct {
    u32 unk0;          /* state timer */
    u32 unk4;          /* frames since the act cursor moved */
    u32 unk8;          /* frame counter (confirm flash) */
    s32 unkc;          /* number of zone tabs */
    u32 unk10;         /* flags: 1 fade both screens, 8/0x10 navi */
    s32 score[7][3];   /* +0x14 best scores (data_022c4690 / data_022c46e8) */
    s32 rank[7][3];    /* +0x68 */
    s32 offs[8];       /* +0xbc tab raise */
    s32 unkd8;         /* act confirmed */
    s32 unkdc;         /* back button pressed */
    MnBgImg *bg0, *bg1;
    u16 bg1_pal[256];  /* +0x160: the palette banks of zmap_act_b_02 (one per zone) */
    MnSpr spr[38];     /* +0x360 */
    s32 anim[38];
    MnRes *res2, *res3, *res4;   /* zmap_act, font_zmap_act_num, mnu_titl */
    MnTask *task;
} ActSelWork;

static ActSelWork *gZoneActSelectWork;
static s32 data_ov001_0231c3ec;   /* selected zone */
static s32 gZoneActSelectResult;  /* selected act */

BOOL ZoneActSelect_IsFinished(void) { return gZoneActSelectWork == NULL; }
s32 ZoneActSelect_GetResult(void) { return ZoneActSelect_IsFinished() ? gZoneActSelectResult : -1; }
s32 ZoneActSelect_GetSelectedZone(void) { return ZoneActSelect_IsFinished() ? data_ov001_0231c3ec : -1; }

static void ZoneActSelect_LoadRecords(ActSelWork *p) {
    s32 i, j;
    for (i = 0; i < 7; i++)
        for (j = 0; j < 3; j++)
            p->score[i][j] = (s32)(gGameState.unk0 == 0 ? gSaveData.unk10[i][j] : gSaveData.unk68[i][j]);
}

static void ZoneActSelect_CalcRecordRanks(ActSelWork *p) {
    const s32 *t1 = g_mn_data.as_rank1, *t2 = g_mn_data.as_rank2;
    s32 i, j;
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 2; j++) {
            s32 v = p->score[i][j];
            p->rank[i][j] = v >= t1[0] ? 0 : v >= t1[1] ? 1 : v >= t1[2] ? 2 : 3;
        }
        p->rank[i][2] = p->score[i][2] >= t2[0] ? 0 : p->score[i][2] >= t2[1] ? 1 : p->score[i][2] >= t2[2] ? 2 : 3;
    }
}

static void spr_anim(MnSpr *s, s32 *cur, s32 anim) {
    if (anim != *cur) {
        AnimatorSprite_SetAnimation(s, (u16)anim);
        *cur = anim;
    }
}

static void ZoneActSelect_DrawHeader(ActSelWork *p) {   /* ZoneActSelect_Unk_ov001_022e08e0 */
    MnSpr *s = &p->spr[7];
    spr_anim(s, &p->anim[7], 7);
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
    s = &p->spr[8];
    spr_anim(s, &p->anim[8], data_ov001_0231c3ec + 8);
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}

static void ZoneActSelect_DrawActCursor(ActSelWork *p) {
    MnSpr *s = &p->spr[12];
    s32 anim = 0x18;
    if (p->unk4 < 6 && p->unkd8 == 0)
        anim++;
    spr_anim(s, &p->anim[12], anim);
    s->x = 0x10;
    s->y = (s16)(gZoneActSelectResult * 32 + 0x38);
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}

static void ZoneActSelect_DrawActLabels(ActSelWork *p) {
    s32 i, anim;
    BOOL vis;
    for (i = 0; i < 3; i++) {
        MnSpr *s = &p->spr[i + 9];
        anim = i * 3 + 0xf;
        vis = FALSE;
        if (i == gZoneActSelectResult) {
            if (p->unkd8 != 0 && p->unk8 < 0x3c) {
                anim += 2;
                vis = TRUE;
            } else {
                anim++;
            }
        }
        spr_anim(s, &p->anim[i + 9], anim);
        if (vis)
            s->flags |= SPR_LOOP;
        else
            s->flags &= ~(u32)SPR_LOOP;
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_Draw(s);
    }
}

static void ZoneActSelect_DrawRecordRanks(ActSelWork *p) {
    s32 i, anim;
    BOOL vis;
    for (i = 0; i < 3; i++) {
        MnSpr *s = &p->spr[i + 13];
        anim = p->rank[data_ov001_0231c3ec][i] * 3 + 0x1a;
        vis = FALSE;
        if (i == gZoneActSelectResult) {
            if (p->unkd8 != 0 && p->unk8 < 0x3c) {
                anim += 2;
                vis = TRUE;
            } else {
                anim++;
            }
        }
        spr_anim(s, &p->anim[i + 13], anim);
        if (vis)
            s->flags |= SPR_LOOP;
        else
            s->flags &= ~(u32)SPR_LOOP;
        s->x = 0xa9;
        s->y = (s16)(i * 0x20 + 0x4f);
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_Draw(s);
    }
}

static void ZoneActSelect_DrawRecordScores(ActSelWork *p) {
    s32 v, i, j, k, n, digit[7];
    for (i = 0; i < 3; i++) {
        v = p->score[data_ov001_0231c3ec][i];
        k = 1;
        for (j = 0; j < 7; j++) {
            digit[j] = v % (k * 10) / k;
            k *= 10;
        }
        for (j = 6; j >= 0; j--) {
            if (digit[j] == 0)
                digit[j] = -1;
            if (digit[j] > 0)
                break;
        }
        if (digit[0] < 0)
            digit[0] = 0;
        for (j = 0; j < 7; j++) {
            s32 id = digit[j];
            BOOL flash = FALSE;
            MnSpr *s;
            if (id < 0)
                break;
            n = i * 7 + 0x10 + j;
            s = &p->spr[n];
            id *= 3;
            if (i == gZoneActSelectResult) {
                if (p->unkd8 != 0 && p->unk8 < 0x3c) {
                    id += 2;
                    flash = TRUE;
                } else {
                    id += 1;
                }
            }
            spr_anim(s, &p->anim[n], id);
            if (flash)
                s->flags |= SPR_LOOP;
            else
                s->flags &= ~(u32)SPR_LOOP;
            s->x = (s16)(j * -8 + 0xa0);
            s->y = (s16)(i * 0x20 + 0x4e);
            AnimatorSprite_ProcessAnimation(s, 0, 0);
            AnimatorSprite_Draw(s);
        }
    }
}

static void ZoneActSelect_UpdateTabOffsets(ActSelWork *p) {
    s32 i;
    for (i = 0; i < p->unkc; i++) {
        if (i == data_ov001_0231c3ec) {
            p->offs[i] -= 2;
            if (p->offs[i] < -8)
                p->offs[i] = -8;
        } else {
            p->offs[i] += 2;
            if (p->offs[i] > 0)
                p->offs[i] = 0;
        }
    }
}

static void ZoneActSelect_DrawZoneTabs(ActSelWork *p) {
    s32 i;
    for (i = 0; i < p->unkc; i++) {
        MnSpr *s = &p->spr[i];
        spr_anim(s, &p->anim[i], i);
        s->flags &= ~0x60000u;
        if (!(i == data_ov001_0231c3ec && p->offs[i] == -8))
            s->flags |= 0x20000;
        s->x = (s16)(i * 0x20 + 0x10);
        s->y = (s16)p->offs[i];
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_Draw(s);
    }
}

static void ZoneActSelect_DrawBackButton(ActSelWork *p) {
    MnSpr *s = &p->spr[37];
    spr_anim(s, &p->anim[37], p->unkdc != 0 ? 1 : 0);
    s->x = 0xd8;
    s->y = 0x98;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}

/* the zone's colour set: palette bank `i` of zmap_act_b_02 -> colours 0..15 */
static void ZoneActSelect_LoadZonePalette(ActSelWork *p, s32 i) {
    if (i >= 0 && i < 16)
        mn_bg_set_colors(p->bg1, 0, 16, &p->bg1_pal[i * 16]);
}

static void draw_all(ActSelWork *w, BOOL cursor) {
    mn_comp_reset(0);   /* the act select screen, with the navigator pieces while it is up */
    mn_comp_full(0);
    mn_navi_compose_ex(1);
    ZoneActSelect_DrawHeader(w);
    if (cursor)
        ZoneActSelect_DrawActCursor(w);
    ZoneActSelect_DrawRecordRanks(w);
    ZoneActSelect_DrawRecordScores(w);
    ZoneActSelect_DrawActLabels(w);
    ZoneActSelect_UpdateTabOffsets(w);
    ZoneActSelect_DrawZoneTabs(w);
    ZoneActSelect_DrawBackButton(w);
    ZoneActSelect_LoadZonePalette(w, data_ov001_0231c3ec);
}

static void ZoneActSelect_State_Select(void);
static void ZoneActSelect_State_FadeOut(void);

/* the navigator on the other screen (mn_navi.c): no-ops when it is not up */
static void ZoneActSelect_ShowIntroNaviMessage(void) {
    u32 f = gZoneActSelectWork->unk10;
    if (f & 2)
        return;
    if (!(f & 4)) {
        NaviMsg_ShowMessage(0, 0xb4);
        NaviMsg_SetFaceAnim(3);
        return;
    }
    NaviMsg_ShowMessage(3, 0xb4);
    NaviMsg_SetFaceAnim(2);
}

static void ZoneActSelect_ShowNaviMessage4(void) {
    if (!(gZoneActSelectWork->unk10 & 2)) {
        NaviMsg_ShowMessage(4, 0xb4);
        NaviMsg_SetFaceAnim(6);
    }
}

static void ZoneActSelect_State_FadeIn(void) {
    ActSelWork *w = TASK_WORK(ActSelWork);
    s32 v = (FX_Div(FX_Mul((fx32)(w->unk0 << 12), -0x10000), 0x1e000) + 0x10000 + 0x800) >> 12;
    if (v < -0x10)
        v = -0x10;
    if (v > 0x10)
        v = 0x10;
    BRIGHT_MAIN = (s16)v;
    if (w->unk10 & 1)
        BRIGHT_SUB = (s16)v;
    draw_all(w, FALSE);
    w->unk0++;
    if (w->unk0 >= 0x1e) {
        w->unk0 = 0;
        w->unk4 = 6;
        BRIGHT_MAIN = 0;
        if (w->unk10 & 1)
            BRIGHT_SUB = 0;
        if (gZoneActSelectWork->unk10 & 8)
            NaviM_StartEmeraldAppear();
        if (gZoneActSelectWork->unk10 & 0x10)
            NaviM_StartCounter();
        w->unk10 &= ~0x18u;
        gTaskCurrent->func = ZoneActSelect_State_Select;
    }
}

/* ZoneActSelect_State_Unk_ov001_022e0f08: 60-frame confirm flash */
static void ZoneActSelect_State_Confirmed(void) {
    ActSelWork *t0 = TASK_WORK(ActSelWork);
    ZoneActSelect_UpdateTabOffsets(t0);
    ZoneActSelect_DrawZoneTabs(t0);
    ZoneActSelect_DrawActCursor(t0);
    draw_all(t0, FALSE);
    t0->unk0++;
    t0->unk8++;
    t0->unk4++;
    if (t0->unk0 >= 0x3c) {
        t0->unk0 = 0;
        gTaskCurrent->func = ZoneActSelect_State_FadeOut;
    }
}

static void ZoneActSelect_State_Select(void) {
    ActSelWork *w = TASK_WORK(ActSelWork);
    BOOL ok = FALSE, cancel = FALSE;
    s32 sel = -1;
    if (gPad.trg & PAD_LEFT) {
        if (data_ov001_0231c3ec > 0)
            sel = data_ov001_0231c3ec - 1;
    } else if (gPad.trg & PAD_RIGHT) {
        if (data_ov001_0231c3ec < w->unkc - 1)
            sel = data_ov001_0231c3ec + 1;
    }
    if (sel >= 0) {
        if (data_ov001_0231c3ec != sel)
            SysSound_PlaySE(4);   /* SysSound_PlayCursorSE */
        data_ov001_0231c3ec = sel;
    }
    sel = -1;
    if (gPad.trg & PAD_UP) {
        if (gZoneActSelectResult > 0) {
            sel = gZoneActSelectResult - 1;
            SysSound_PlaySE(4);
        }
    } else if (gPad.trg & PAD_DOWN) {
        if (gZoneActSelectResult < 2) {
            sel = gZoneActSelectResult + 1;
            SysSound_PlaySE(4);
        }
    }
    if (sel >= 0 && gZoneActSelectResult != sel) {
        gZoneActSelectResult = sel;
        w->unk4 = 0;
    }
    if (gPad.trg & PAD_A)
        ok = TRUE;
    if (!ok && (gPad.trg & PAD_B))
        cancel = TRUE;
    draw_all(w, TRUE);
    w->unk0++;
    w->unk4++;
    if (cancel) {
        w->unk0 = 0;
        SysSound_PlaySE(0xc);
        gTaskCurrent->func = ZoneActSelect_State_FadeOut;
        return;
    }
    if (ok) {
        w->unk0 = 0;
        w->unkd8 = 1;
        SysSound_PlaySE(0x12);
        ZoneActSelect_ShowNaviMessage4();
        gTaskCurrent->func = ZoneActSelect_State_Confirmed;
    }
}

static void ZoneActSelect_State_FadeOut(void) {
    ActSelWork *w = TASK_WORK(ActSelWork);
    s32 target, v;
    fx32 a, b;
    if (data_ov001_0231c3ec < 0 || gZoneActSelectResult < 0)
        target = 0x10;
    else if (gZoneActSelectResult == 2)
        target = -0x10;
    else
        target = 0x10;
    a = 0x800;
    b = target << 12;
    b += b >= 0 ? 0x800 : -0x800;
    v = (a + FX_Div(FX_Mul((fx32)(w->unk0 << 12), b - a), 30 << 12)) >> 12;
    if (v < -0x10)
        v = -0x10;
    if (v > 0x10)
        v = 0x10;
    BRIGHT_MAIN = (s16)v;
    if (w->unkd8 != 0)
        BRIGHT_SUB = (s16)v;
    w->unk0++;
    if (w->unk0 < 0x1e)
        draw_all(w, FALSE);
    if (w->unk0 >= 0x1e) {
        w->unk0 = 0;
        if (w->unkd8 != 0) {
            BRIGHT_MAIN = (s16)target;
            BRIGHT_SUB = (s16)target;
        } else {
            BRIGHT_MAIN = (s16)target;
            data_ov001_0231c3ec = -1;
            gZoneActSelectResult = -1;
        }
        Task_Destroy(gTaskCurrent);
    }
}

static void ZoneActSelect_Destructor(void) {
    ActSelWork *w = TASK_WORK(ActSelWork);
    Background_Hide(0, 1);
    Background_Hide(0, 2);
    mn_bg_free(w->bg0);
    mn_bg_free(w->bg1);
    mn_res_free(w->res2);
    mn_res_free(w->res3);
    mn_res_free(w->res4);
    if (gZoneActSelectWork == w)
        gZoneActSelectWork = NULL;
}

static void ZoneActSelect_InitSprite(MnSpr *s, const MnRes *res, u32 idx, u32 mode) {
    AnimatorSprite_Init(s, res, (u16)idx, 0x40, 0);
    s->flags &= ~0x60000u;
    s->flags |= mode << 17;
}

void ZoneActSelect_Create(s32 a, s32 b, s32 c) {
    MnTask *t = Task_Create(ZoneActSelect_State_FadeIn, ZoneActSelect_Destructor, 0, 0x8000, 0, sizeof(ActSelWork));
    ActSelWork *w = (ActSelWork *)t->work;
    s32 i;
    gZoneActSelectWork = w;
    w->unk0 = 0;
    w->unkc = b + 1;
    w->task = t;
    w->unkd8 = 0;
    w->unkdc = 0;
    w->unk10 = (u32)c;
    data_ov001_0231c3ec = a;
    gZoneActSelectResult = 0;
    ZoneActSelect_LoadRecords(w);
    ZoneActSelect_CalcRecordRanks(w);
    /* ZoneActSelect_InitDisplay: BG1/BG2 of the main engine, the map BG off */
    Background_Hide(0, 3);
    Background_SetPriority(0, 1, 1);
    Background_SetPriority(0, 2, 2);
    Background_SetSize(0, 2, 0, 0);
    gDisplayMain[1].x = gDisplayMain[1].y = gDisplayMain[2].x = gDisplayMain[2].y = 0;
    mn_comp_reset(0);
    mn_comp_full(0);
    g_mn_scene = "ActSelect";
    /* gZoneActSelectFileList */
    w->bg0 = mn_bg_load("bg2/zmap/zmap_act_b_01.bbg");
    w->bg1 = mn_bg_load("bg2/zmap/zmap_act_b_02.bbg");
    w->res2 = mn_res_load("act2/zmap/zmap_act.bac");
    w->res3 = mn_res_load("act2/zmap/font_zmap_act_num.bac");
    w->res4 = mn_res_load("act/mnu_titl.bac");
    Background_Show(0, 1, w->bg0);
    Background_Show(0, 2, w->bg1);
    if (w->bg1)
        memcpy(w->bg1_pal, w->bg1->pal, sizeof(w->bg1_pal));
    for (i = 0; i < 3; i++)
        ZoneActSelect_InitSprite(&w->spr[i], w->res2, (u32)i, 1);
    for (i = 3; i < 6; i++)
        ZoneActSelect_InitSprite(&w->spr[i], w->res2, (u32)i, 1);
    ZoneActSelect_InitSprite(&w->spr[6], w->res2, 6, 1);
    ZoneActSelect_InitSprite(&w->spr[7], w->res2, 7, 0);
    ZoneActSelect_InitSprite(&w->spr[8], w->res2, 8, 0);
    ZoneActSelect_InitSprite(&w->spr[9], w->res2, 15, 0);
    ZoneActSelect_InitSprite(&w->spr[10], w->res2, 18, 0);
    ZoneActSelect_InitSprite(&w->spr[11], w->res2, 21, 0);
    ZoneActSelect_InitSprite(&w->spr[12], w->res2, 25, 0);
    for (i = 13; i <= 15; i++)
        ZoneActSelect_InitSprite(&w->spr[i], w->res2, 26, 0);
    ZoneActSelect_InitSprite(&w->spr[37], w->res4, 0, 0);
    for (i = 0x10; i <= 0x24; i++)
        ZoneActSelect_InitSprite(&w->spr[i], w->res3, 0, 0);
    for (i = 0; i < 38; i++)
        w->anim[i] = -1;
    w->anim[0] = 0;
    ZoneActSelect_ShowIntroNaviMessage();
}

/* ---------------------------------------------------- ZoneActSelectMode */
static void ZoneActSelectMode_Main(void) {
    s32 t1;
    if (ZoneActSelect_IsFinished()) {
        t1 = ZoneActSelect_GetResult();
        Task_Destroy(gTaskCurrent);
        mn_display_reset();   /* ZoneActSelectMode_Shutdown */
        if (t1 < 0) {
            TitleMenu_Start();   /* ZoneActSelectMode_ExitToTitleMenu */
            return;
        }
        gGameState.unk0 = 0;   /* ZoneActSelectMode_StartStage */
        Game_StartStoryStage(0, t1);
    }
}

void ZoneActSelectMode_Start(void) {
    gGameState.unk0 = 0;
    StoryProgress_RestoreFromSave();
    mn_display_reset();
    SysSound_PlayBGM(4);
    ZoneActSelect_Create(0, 0, 3);
    gSaveDataBusy = 0;
    Task_Create(ZoneActSelectMode_Main, NULL, 0, 0x5000, 0x2000, 4);
}

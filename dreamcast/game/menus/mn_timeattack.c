/*
 * TimeAttackMenu_* (src/game/Menus/TimeAttack/, 62 functions): the time-attack
 * stage select with the five best times of the selected stage. Modes (the
 * original's):
 *   0 acts   (PlayerSelect mode 3 -> SceneChange_ToTimeAttackMenuMode0): 14 acts + 4 zone-X acts
 *   1 bosses (PlayerSelect mode 4 -> SceneChange_ToTimeAttackMenuMode1): 7 bosses
 *   2 / 3    the same record lists, view only, from Options > PLAYER DATA >
 *            TIME ATTACK RECORD (SceneChange_ToTimeAttackMenuMode2/3)
 * A starts the stage (Game_StartTimeAttackStage -> MENU_REQ_STAGE, mode 2);
 * B goes back (ZoneMapPause_ExecuteChoice(mode)).
 *
 * DS display (both engines, TimeAttackMenu_InitDisplay / _InitBackgrounds):
 *   main (touch): BG1 mnu_time_rankbase (5 record bars, 512-px plane, per-line
 *     HOFS by HDMA: the bars slide in), BG2 mnu_time_selectbase (zone box), BG3
 *     mnu_time_base (zigzag, scrolls 2 px/frame), sprites: ranks, record times,
 *     character names, zone emblem / name, arrows, back button
 *   sub: BG2 mnu_time_image (character art; mnu_time_reco_image in modes 2/3),
 *     BG3 mnu_time_base, sprite: the "TIME ATTACK" / "TIME ATTACK RECORD" plate
 * Single screen: see TimeAttackMenu_Compose (MENUS.md 3).
 *
 * DS service                         here
 * HDMA BG1HOFS (TimeAttackMenu_StartBg1HOfsHdma / FillHdmaBuffer / StopHdma)
 *                                    BG1 drawn as row bands (mn_emit_bg_part), one per HDMA run
 * LoadBgPaletteRange (bank 0 / 1 of mnu_time_base for modes 0-1 / 2-3)
 *                                    mn_bg_set_colors on the two BG3 copies
 * gTouchState rectangles              pad only (the original's pad path: A, B, LEFT, RIGHT)
 * ObjVram_Alloc / palette numbers     not needed / not applied (converted atlases)
 */
#include <stdlib.h>
#include "mn_internal.h"

typedef struct {
    s32 x;           /* +0 shown x (previous frame's slide) */
    s32 unk4;        /* +4 slide value of this frame (TimeAttackMenu_Unk_ov001_022ea360) */
    s32 y;           /* +8 */
    s32 idx;         /* +0xc record set by: 0 Sonic, 1 Blaze (GetRecordTimes "cleared") */
    s32 digits[5];   /* +0x10 */
    s32 flag;        /* +0x24 valid record */
} TaItem;

typedef struct {
    u32 unk0;        /* frame counter of the state */
    u32 unk4;        /* mode */
    TaItem items[5];
    s32 unkd0;       /* selected stage */
    s32 unkd4;       /* next stage (scrolling) */
    s32 unkd8;       /* stage count */
    s32 unkdc;       /* confirm pressed while scrolling */
    s32 unke0;       /* 1 left, 0 right */
    s32 unke4;       /* back button touched */
    MnSpr spr[0x1f]; /* +0x228 */
    s32 anim[0x1f];  /* +0xae0 */
    KeyframeAnim anims[12]; /* +0xb5c (zone_sel.ama) */
    MnBgImg *bg[5];  /* +0x106c: base, rankbase, selectbase, reco_image, image */
    MnBgImg *bg3sub; /* the sub engine's copy of mnu_time_base (own palette bank) */
    MnRes *res[9];   /* +0x1080 (res[7] = the .ama bank) */
    void *ama;
    s32 hdma;        /* gSystemFlags & 0x400 with data_02087cdc -> BG1HOFS */
    s16 hofs[0xc0];  /* the HDMA buffer */
} TaWork;

static TaWork *s_ta;

/* SceneChange_ToTimeAttackMenuMode* */
void TimeAttackMenu_StartMode0(void);
void TimeAttackMenu_StartMode1(void);

/* ---------------------------------------------------------------- fade */
typedef struct {
    u32 t;
    s16 from, to;
    u32 dur, delay;
} TaFadeWork;

static void TimeAttackMenuFadeTask_Main(void) {
    TaFadeWork *w = TASK_WORK(TaFadeWork);
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

static void TimeAttackMenu_CreateFadeTask(s32 x, s32 y, s32 a, s32 b) {
    TaFadeWork *w;
    Task_DestroyRange(0x3000, 0x3000);
    if (x < -16)
        x = -16;
    if (x > 16)
        x = 16;
    if (y < -16)
        y = -16;
    if (y > 16)
        y = 16;
    w = (TaFadeWork *)Task_Create(TimeAttackMenuFadeTask_Main, NULL, 0, 0x3000, 0x1000, sizeof(TaFadeWork))->work;
    w->t = 0;
    w->from = (s16)x;
    w->to = (s16)y;
    w->dur = (u32)a;
    w->delay = (u32)b;
}

static void TimeAttackMenuBgScroll_Main(void) {
    gDisplayMain[3].x += 2;   /* gDisplayMain[6] */
    gDisplaySub[3].x += 2;
}
static void TimeAttackMenu_CreateBgScrollTask(void) {
    Task_Create(TimeAttackMenuBgScroll_Main, NULL, 0, 0x2000, 0x1000, 0);
}

/* ---------------------------------------------------------------- helpers */
/* TimeAttackMenu_IndexToGrid: stage index -> (zone column, act row) */
static void TimeAttackMenu_IndexToGrid(TaWork *w, s32 n, s32 *x, s32 *y) {
    switch (w->unk4) {
    case 0:
    case 2:
        if (n >= 14) {
            if (x != NULL)
                *x = 7;
            if (y != NULL)
                *y = n - 14;
        } else {
            if (x != NULL)
                *x = n / 2;
            if (y != NULL)
                *y = n % 2;
        }
        break;
    case 1:
    case 3:
        if (x != NULL)
            *x = n;
        if (y != NULL)
            *y = 2;
        break;
    }
}

/* data_ov001_023166c4 / data_ov001_023166bc: the zone order of Sonic / Blaze
 * (the same bytes as StoryProgress' data_02074bc4 / data_02074bd0) */
static s32 zone_at(s32 a) { return g_mn_data.zone_ord[gGameState.unk0 == 0 ? 0 : 1][a]; }

/* TimeAttackMenu_GetRecordTimes(zone column, act row, out[5]) */
static void TimeAttackMenu_GetRecordTimes(s32 a, s32 b, TaItem *out) {
    s32 q = 0, r = 0, i, t;
    if (a >= 7) {
        q = b / 2;
        r = b % 2;
    } else {
        a = zone_at(a);
    }
    for (i = 0; i < 5; i++) {
        if (a >= 7)
            out->idx = (gSaveData.unk1A4[q][r] & (1 << i)) ? 1 : 0;
        else
            out->idx = (gSaveData.unkBC[a][b] & (1 << i)) ? 1 : 0;
        if (a >= 7)
            t = gSaveData.unk1A8[q][r][i];
        else
            t = gSaveData.unkD2[a][b][i];
        if (t > 0x8c9f) {
            out->flag = 0;
            out->digits[4] = 10;
            out->digits[3] = 10;
            out->digits[2] = 10;
            out->digits[1] = 10;
            out->digits[0] = 10;
        } else {
            out->flag = 1;
            out->digits[4] = t / 3600;
            t = t % 3600;
            out->digits[3] = t / 600;
            t = t % 600;
            out->digits[2] = t / 60;
            t = t % 60;
            t = (t * 0x1b1) >> 8;
            if (t >= 100)
                t = 99;
            out->digits[1] = t / 10;
            t = t % 10;
            out->digits[0] = t;
        }
        out++;
    }
}

static void spr_set(TaWork *w, s32 i, s32 id) {
    if (w->anim[i] != id) {
        AnimatorSprite_SetAnimation(&w->spr[i], (u16)id);
        w->anim[i] = id;
    }
}

/* TimeAttackMenu_DrawRecordTime: row i (character name, record set by, 5 digits) */
static void TimeAttackMenu_DrawRecordTime(TaWork *p, s32 i) {
    TaItem *e = &p->items[i];
    MnSpr *spr;
#define DRAW(dx, dy)                                \
    do {                                            \
        spr->x = (s16)(e->x + (dx));                \
        spr->y = (s16)(e->y + (dy));                \
        AnimatorSprite_ProcessAnimation(spr, 0, 0); \
        AnimatorSprite_Draw(spr);                   \
    } while (0)
    spr = &p->spr[e->idx + 5];
    if (e->flag)
        DRAW(0x50, 1);
    spr = &p->spr[e->idx + 0x1a];
    if (e->flag)
        DRAW(0x78, 7);
    spr = e->flag ? &p->spr[e->digits[0] + 7] : &p->spr[17];
    DRAW(0xe8, 4);
    spr = e->flag ? &p->spr[e->digits[1] + 7] : &p->spr[17];
    DRAW(0xe0, 4);
    spr = e->flag ? &p->spr[e->digits[2] + 7] : &p->spr[17];
    DRAW(0xd0, 4);
    spr = e->flag ? &p->spr[e->digits[3] + 7] : &p->spr[17];
    DRAW(0xc8, 4);
    spr = e->flag ? &p->spr[e->digits[4] + 7] : &p->spr[17];
    DRAW(0xb8, 4);
    spr = &p->spr[18];
    DRAW(0xc0, 4);
    spr = &p->spr[19];
    DRAW(0xd8, 4);
#undef DRAW
}

/* TimeAttackMenu_Unk_ov001_022e9fa8: the rank number of row i */
static void TimeAttackMenu_Unk_ov001_022e9fa8(TaWork *p, s32 i) {
    MnSpr *s = &p->spr[i];
    s->x = (s16)p->items[i].x;
    s->y = 0;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    if (s->x < 0xc0)
        AnimatorSprite_Draw(s);
}
/* TimeAttackMenu_Unk_ov001_022ea360: shown x = last frame's value */
static void TimeAttackMenu_Unk_ov001_022ea360(TaWork *p, s32 i, s32 v) {
    p->items[i].x = p->items[i].unk4;
    p->items[i].unk4 = v;
}
/* the "TIME ATTACK" / "TIME ATTACK RECORD" plate (sub screen) */
static void TimeAttackMenu_Unk_ov001_022ea37c(TaWork *p) {
    s32 id = (p->unk4 == 2 || p->unk4 == 3) ? 1 : 0;
    spr_set(p, 24, id);
    p->spr[24].flags &= ~4u;
}
/* back button: pressed (1) / normal (0) */
static void TimeAttackMenu_Unk_ov001_022ea3e0(TaWork *p) {
    spr_set(p, 28, 1);
    p->spr[28].flags &= ~4u;
}
static void TimeAttackMenu_Unk_ov001_022ea41c(TaWork *p) {
    spr_set(p, 28, 0);
    p->spr[28].flags &= ~4u;
}
/* "ZONE n ACT m" plate, highlighted (+1) */
static void TimeAttackMenu_Unk_ov001_022ea458(TaWork *p, s32 a, s32 b) {
    u16 id = 0;
    if (a >= 7) {
        id = (u16)(b * 2 + 1);
        spr_set(p, 30, id);
        p->spr[30].flags &= ~4u;
        return;
    }
    switch (p->unk4) {
    case 0:
    case 2:
        id = (u16)(a * 4 + b * 2 + 1);
        break;
    case 1:
    case 3:
        id = (u16)(a * 2 + 1);
        break;
    }
    spr_set(p, 25, id);
    p->spr[25].flags &= ~4u;
}
static void TimeAttackMenu_Unk_ov001_022ea504(TaWork *p, s32 a, s32 b) {
    u16 id = 0;
    if (a >= 7) {
        id = (u16)(b * 2);
        spr_set(p, 30, id);
        p->spr[30].flags &= ~4u;
        return;
    }
    switch (p->unk4) {
    case 0:
    case 2:
        id = (u16)(a * 4 + b * 2);
        break;
    case 1:
    case 3:
        id = (u16)(a * 2);
        break;
    }
    spr_set(p, 25, id);
    p->spr[25].flags &= ~4u;
}
/* zone emblem (zone order + 5) or the zone-X clock emblem (act_x anim 8) */
static void TimeAttackMenu_Unk_ov001_022ea5a8(TaWork *p, s32 a) {
    if (a >= 7) {
        spr_set(p, 29, 8);
        p->spr[29].flags &= ~4u;
        return;
    }
    spr_set(p, 23, (s32)(u16)(zone_at(a) + 5));
    p->spr[23].flags &= ~4u;
}
/* right arrow (spr 22, flipped): 644 pressed, 694 normal, 6e4 normal + stopped */
static void TimeAttackMenu_Unk_ov001_022ea644(TaWork *p) {
    AnimatorSprite_SetAnimation(&p->spr[22], 3);
    p->anim[22] = 3;
    AnimatorSprite_ProcessAnimation(&p->spr[22], 0, 0);
    p->spr[22].flags &= ~4u;
    p->spr[22].flags |= SPR_HFLIP;
    p->spr[22].flags &= ~1u;
}
static void TimeAttackMenu_Unk_ov001_022ea694(TaWork *p) {
    AnimatorSprite_SetAnimation(&p->spr[22], 2);
    p->anim[22] = 2;
    AnimatorSprite_ProcessAnimation(&p->spr[22], 0, 0);
    p->spr[22].flags |= 4;
    p->spr[22].flags |= SPR_HFLIP;
    p->spr[22].flags &= ~1u;
}
static void TimeAttackMenu_Unk_ov001_022ea6e4(TaWork *p) {
    AnimatorSprite_SetAnimation(&p->spr[22], 2);
    p->anim[22] = 2;
    AnimatorSprite_ProcessAnimation(&p->spr[22], 0, 0);
    p->spr[22].flags |= 4;
    p->spr[22].flags |= SPR_HFLIP;
    p->spr[22].flags |= 1;
}
/* left arrow (spr 21): 734 pressed, 77c normal, 7c4 normal + stopped */
static void TimeAttackMenu_Unk_ov001_022ea734(TaWork *p) {
    AnimatorSprite_SetAnimation(&p->spr[21], 3);
    p->anim[21] = 3;
    AnimatorSprite_ProcessAnimation(&p->spr[21], 0, 0);
    p->spr[21].flags &= ~4u;
    p->spr[21].flags &= ~1u;
}
static void TimeAttackMenu_Unk_ov001_022ea77c(TaWork *p) {
    AnimatorSprite_SetAnimation(&p->spr[21], 2);
    p->anim[21] = 2;
    AnimatorSprite_ProcessAnimation(&p->spr[21], 0, 0);
    p->spr[21].flags |= 4;
    p->spr[21].flags &= ~1u;
}
static void TimeAttackMenu_Unk_ov001_022ea7c4(TaWork *p) {
    AnimatorSprite_SetAnimation(&p->spr[21], 2);
    p->anim[21] = 2;
    AnimatorSprite_ProcessAnimation(&p->spr[21], 0, 0);
    p->spr[21].flags |= 4;
    p->spr[21].flags |= 1;
}
/* the OK bar (spr 20): pressed (1) / normal (0) */
static void TimeAttackMenu_Unk_ov001_022ea80c(TaWork *p) {
    spr_set(p, 20, 1);
    p->spr[20].flags &= ~4u;
    p->spr[20].flags &= ~1u;
}
static void TimeAttackMenu_Unk_ov001_022ea850(TaWork *p) {
    spr_set(p, 20, 0);
    p->spr[20].flags &= ~4u;
    p->spr[20].flags &= ~1u;
}

/* HDMA BG1HOFS: lines 0..7 0, five runs of 24 lines at -items[j].unk4, then 0 */
static void TimeAttackMenu_StartBg1HOfsHdma(TaWork *p) { p->hdma = 1; }
static void TimeAttackMenu_FillHdmaBuffer(TaWork *p) {
    s32 i, j, end;
    if (!p->hdma)
        return;
    for (i = 0; i < 8; i++)
        p->hofs[i] = 0;
    for (j = 0; j < 5; j++) {
        s16 v = (s16)-p->items[j].unk4;
        for (end = i + 0x18; i < end; i++)
            p->hofs[i] = v;
    }
    for (; i < 0xc0; i++)
        p->hofs[i] = 0;
}
static void TimeAttackMenu_StopHdma(TaWork *p) {
    p->hdma = 0;
    gDisplayMain[1].x = 0;   /* gDisplayMain[2] / [3] */
    gDisplayMain[1].y = 0;
}

/* TimeAttackMenu_LoadBgPaletteRange: palette bank 0 (modes 0/1) or 1 (modes 2/3) of
 * mnu_time_base into BG palette 0 */
static void TimeAttackMenu_LoadBgPaletteRange(MnBgImg *b, s32 start, s32 end) {
    u16 pal[256];
    if (!b)
        return;
    memcpy(pal, b->pal, sizeof(pal));
    mn_bg_set_colors(b, 0, end - start + 1, &pal[start]);
}

/* the repeated drawing block of the State functions. `rows`: slide value per row
 * (NULL = 0, the rows are at rest); `sel`: the stage index that picks the zone-X sprites */
static void TimeAttackMenu_DrawFrame(TaWork *w, const s32 *rows, s32 sel, int emblem_scaled,
                                     void (*mid)(TaWork *)) {
    KeyframeAnim *a;
    s32 i;
    for (i = 0; i < 5; i++) {
        TimeAttackMenu_Unk_ov001_022ea360(w, i, rows ? rows[i] : 0);
        w->items[i].y = 8;
        w->items[i].y += i * 0x18;
        TimeAttackMenu_DrawRecordTime(w, i);
        TimeAttackMenu_Unk_ov001_022e9fa8(w, i);
    }
    if (rows)
        TimeAttackMenu_FillHdmaBuffer(w);
    a = &w->anims[6];
    if (sel >= 14) {
        w->spr[30].x = a->x;
        w->spr[30].y = a->y;
        AnimatorSprite_ProcessAnimation(&w->spr[30], 0, 0);
        AnimatorSprite_Draw(&w->spr[30]);
    } else {
        w->spr[25].x = a->x;
        w->spr[25].y = a->y;
        AnimatorSprite_ProcessAnimation(&w->spr[25], 0, 0);
        AnimatorSprite_Draw(&w->spr[25]);
    }
    if (w->unk4 <= 1) {
        a = &w->anims[7];
        w->spr[20].x = a->x;
        w->spr[20].y = a->y;
        AnimatorSprite_ProcessAnimation(&w->spr[20], 0, 0);
        AnimatorSprite_Draw(&w->spr[20]);
    }
    if (mid)
        mid(w);   /* TimeAttackMenu_Unk_ov001_022eab78: the plate switch at frame 8 */
    a = &w->anims[8];
    w->spr[21].x = a->x;
    w->spr[21].y = a->y;
    AnimatorSprite_ProcessAnimation(&w->spr[21], 0, 0);
    AnimatorSprite_Draw(&w->spr[21]);
    a = &w->anims[9];
    w->spr[22].x = a->x;
    w->spr[22].y = a->y;
    AnimatorSprite_ProcessAnimation(&w->spr[22], 0, 0);
    AnimatorSprite_Draw(&w->spr[22]);
    a = &w->anims[5];
    {
        MnSpr *s = sel >= 14 ? &w->spr[29] : &w->spr[23];
        s->x = a->x;
        s->y = a->y;
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        if (!emblem_scaled || (a->unk4 == 0x1000 && a->unk8 == 0x1000))
            AnimatorSprite_Draw(s);
        else
            AnimatorSprite_DrawScaled(s, a->unk4, a->unk8);
    }
    a = &w->anims[10];
    w->spr[28].x = a->x;
    w->spr[28].y = a->y;
    AnimatorSprite_ProcessAnimation(&w->spr[28], 0, 0);
    AnimatorSprite_Draw(&w->spr[28]);
    a = &w->anims[11];
    w->spr[24].x = a->x;
    w->spr[24].y = a->y;
    AnimatorSprite_ProcessAnimation(&w->spr[24], 0, 0);
    AnimatorSprite_Draw(&w->spr[24]);
}

/* BG1 rows: the HDMA runs as bands (emitted after the sprites: behind them) */
static void TimeAttackMenu_DrawBg1(TaWork *w) {
    MnBgImg *img = w->bg[1];
    s32 y = 0, n;
    if (!img)
        return;
    g_mn_layer[0][1].visible = (u8)!w->hdma;
    if (!w->hdma)
        return;
    while (y < 0xc0 && y < img->h) {
        s32 h = (s32)(-w->hofs[y]);   /* BG1HOFS = -x: the row is shown x px to the right (512-px plane) */
        for (n = 1; y + n < 0xc0 && w->hofs[y + n] == w->hofs[y]; n++) {
        }
        if (y + n > img->h)
            n = img->h - y;
        h &= 0x1ff;
        if (h < 256)
            mn_emit_bg_part(0, g_mn_layer[0][1].prio, img, (float)h, (float)y, 0, (float)y, (float)(256 - h),
                            (float)n);
        y += n;
    }
}

static BOOL TimeAttackMenu_IsBackPressed(TaWork *p) {
    if (gPad.trg & PAD_B) {
        p->unke4 = 0;
        return TRUE;
    }
    return FALSE;
}
static BOOL TimeAttackMenu_IsConfirmPressed(TaWork *p) {
    if (p->unk4 == 2 || p->unk4 == 3)
        return FALSE;
    return (gPad.trg & PAD_A) != 0;
}
static BOOL TimeAttackMenu_IsLeftPressed(void) { return (gPad.rep & PAD_LEFT) != 0; }
static BOOL TimeAttackMenu_IsRightPressed(void) { return (gPad.rep & PAD_RIGHT) != 0; }

/* ------------------------------------------------------------- states */
static void TimeAttackMenu_State_Enter(void);
static void TimeAttackMenu_State_Select(void);
static void TimeAttackMenu_State_Unk_ov001_022eae98(void);
static void TimeAttackMenu_Unk_ov001_022ea894(void);
static void TimeAttackMenu_Unk_ov001_022eab78(void);

static s32 slide_of(TaWork *w, s32 i) { return w->anims[i].x; }

/* TimeAttackMenu_Unk_ov001_022ebc3c: the rows are at rest, Select */
static void TimeAttackMenu_Unk_ov001_022ebc3c(MnTask *t) {
    TaWork *w;
    s32 x = 0, y = 0;
    if (t == NULL)
        t = gTaskCurrent;
    w = (TaWork *)t->work;
    w->unk0 = 0;
    TimeAttackMenu_IndexToGrid(w, w->unkd0, &x, &y);
    TimeAttackMenu_Unk_ov001_022ea850(w);
    TimeAttackMenu_Unk_ov001_022ea5a8(w, x);
    TimeAttackMenu_Unk_ov001_022ea504(w, x, y);
    TimeAttackMenu_Unk_ov001_022ea41c(w);
    TimeAttackMenu_StopHdma(w);
    t->func = TimeAttackMenu_State_Select;
}

/* TimeAttackMenu_Unk_ov001_022eba70: start scrolling to the next / previous stage */
static void TimeAttackMenu_Unk_ov001_022eba70(MnTask *t) {
    TaWork *w;
    s32 x = 0, y = 0, i;
    if (t == NULL)
        t = gTaskCurrent;
    w = (TaWork *)t->work;
    w->unk0 = 0;
    if (w->unke0 != 0) {
        w->unkd4 = w->unkd0 - 1;
        if (w->unkd4 < 0)
            w->unkd4 = w->unkd8 - 1;
    } else {
        w->unkd4 = w->unkd0 + 1;
        if (w->unkd4 >= w->unkd8)
            w->unkd4 = 0;
    }
    if (w->unkd0 == w->unkd4) {
        t->func = TimeAttackMenu_State_Select;
        return;
    }
    SysSound_PlaySE(4);
    KeyframeAnim_SetAnim(&w->anims[0], 0);
    KeyframeAnim_SetAnim(&w->anims[1], 1);
    KeyframeAnim_SetAnim(&w->anims[2], 2);
    KeyframeAnim_SetAnim(&w->anims[3], 3);
    KeyframeAnim_SetAnim(&w->anims[4], 4);
    switch (w->unk4) {
    case 0:
    case 2: {
        s32 next = w->unkd4, cur = w->unkd0;
        if (cur / 2 != next / 2 && (cur < 14 || next < 14))
            KeyframeAnim_SetAnim(&w->anims[5], 0xb);
        break;
    }
    case 1:
    case 3:
        if (w->unkd0 != w->unkd4)
            KeyframeAnim_SetAnim(&w->anims[5], 0xb);
        break;
    }
    if (w->unke0 != 0) {
        TimeAttackMenu_Unk_ov001_022ea734(w);
        TimeAttackMenu_Unk_ov001_022ea6e4(w);
    } else {
        TimeAttackMenu_Unk_ov001_022ea7c4(w);
        TimeAttackMenu_Unk_ov001_022ea644(w);
    }
    TimeAttackMenu_IndexToGrid(w, w->unkd4, &x, &y);
    TimeAttackMenu_Unk_ov001_022ea5a8(w, x);
    TimeAttackMenu_Unk_ov001_022ea504(w, x, y);
    TimeAttackMenu_GetRecordTimes(x, y, w->items);
    for (i = 0; i < 5; i++) {
        w->items[i].x = 0x100;
        w->items[i].unk4 = 0x100;
    }
    TimeAttackMenu_StartBg1HOfsHdma(w);
    TimeAttackMenu_FillHdmaBuffer(w);
    t->func = TimeAttackMenu_State_Unk_ov001_022eae98;
}

/* TimeAttackMenu_Unk_ov001_022ebc98: the opening (all 12 key-frame anims) */
static void TimeAttackMenu_Unk_ov001_022ebc98(MnTask *t) {
    TaWork *w = (TaWork *)t->work;
    s32 x = 0, y = 0, i;
    w->unk0 = 0;
    TimeAttackMenu_IndexToGrid(w, w->unkd0, &x, &y);
    for (i = 0; i <= 10; i++)
        KeyframeAnim_SetAnim(&w->anims[i], (u32)i);
    KeyframeAnim_SetAnim(&w->anims[11], 12);
    TimeAttackMenu_Unk_ov001_022ea850(w);
    TimeAttackMenu_Unk_ov001_022ea7c4(w);
    TimeAttackMenu_Unk_ov001_022ea6e4(w);
    TimeAttackMenu_Unk_ov001_022ea5a8(w, x);
    TimeAttackMenu_Unk_ov001_022ea504(w, x, y);
    TimeAttackMenu_Unk_ov001_022ea41c(w);
    TimeAttackMenu_Unk_ov001_022ea37c(w);
    TimeAttackMenu_GetRecordTimes(x, y, w->items);
    for (i = 0; i < 5; i++) {
        w->items[i].x = 0x100;
        w->items[i].unk4 = 0x100;
    }
    TimeAttackMenu_StartBg1HOfsHdma(w);
    TimeAttackMenu_FillHdmaBuffer(w);
    t->func = TimeAttackMenu_State_Enter;
}

static void TimeAttackMenu_Action_Confirm(MnTask *t);
static void TimeAttackMenu_Action_Back(MnTask *t);

static void TimeAttackMenu_State_Enter(void) {
    TaWork *w = TASK_WORK(TaWork);
    s32 rows[5], i;
    BOOL done;
    for (i = 0; i < 12; i++)
        KeyframeAnim_Update(&w->anims[i]);
    for (i = 0; i < 5; i++)
        rows[i] = slide_of(w, i);
    TimeAttackMenu_DrawFrame(w, rows, w->unkd0, 1, NULL);
    TimeAttackMenu_DrawBg1(w);
    w->unk0++;
    done = TRUE;
    for (i = 0; i < 12; i++)
        if (!KeyframeAnim_IsFinished(&w->anims[i]))
            done = FALSE;
    if (done) {
        w->unkdc = 0;
        TimeAttackMenu_Unk_ov001_022ebc3c(NULL);
    }
}

static void TimeAttackMenu_State_Select(void) {
    TaWork *w = TASK_WORK(TaWork);
    s32 i;
    for (i = 5; i < 12; i++)
        KeyframeAnim_Update(&w->anims[i]);
    TimeAttackMenu_DrawFrame(w, NULL, w->unkd0, 1, NULL);
    TimeAttackMenu_DrawBg1(w);
    if (w->unk0 == 0) {
        TimeAttackMenu_Unk_ov001_022ea77c(w);
        TimeAttackMenu_Unk_ov001_022ea694(w);
    }
    w->unk0++;
    if (TimeAttackMenu_IsConfirmPressed(w) || w->unkdc != 0) {
        TimeAttackMenu_Action_Confirm(NULL);
        return;
    }
    if (TimeAttackMenu_IsBackPressed(w)) {
        TimeAttackMenu_Action_Back(NULL);
        return;
    }
    if (TimeAttackMenu_IsLeftPressed()) {
        w->unke0 = 1;
        TimeAttackMenu_Unk_ov001_022eba70(NULL);
        return;
    }
    if (TimeAttackMenu_IsRightPressed()) {
        w->unke0 = 0;
        TimeAttackMenu_Unk_ov001_022eba70(NULL);
    }
}

/* scrolling to unkd4 (the rows slide out and in) */
static void TimeAttackMenu_State_Unk_ov001_022eae98(void) {
    TaWork *w = TASK_WORK(TaWork);
    s32 rows[5], i;
    BOOL done;
    if (w->unk0 == 0 || w->unk0 > 4)
        for (i = 0; i < 5; i++)
            KeyframeAnim_Update(&w->anims[i]);
    for (i = 5; i < 12; i++)
        KeyframeAnim_Update(&w->anims[i]);
    for (i = 0; i < 5; i++)
        rows[i] = slide_of(w, i);
    TimeAttackMenu_DrawFrame(w, rows, w->unkd4, 1, NULL);
    TimeAttackMenu_DrawBg1(w);
    w->unk0++;
    if (TimeAttackMenu_IsConfirmPressed(w)) {
        w->unkdc = 1;
        w->unkd0 = w->unkd4;
        TimeAttackMenu_Unk_ov001_022ebc3c(NULL);
        return;
    }
    done = TRUE;
    for (i = 0; i < 6; i++)
        if (!KeyframeAnim_IsFinished(&w->anims[i]))
            done = FALSE;
    if (done) {
        w->unkd0 = w->unkd4;
        TimeAttackMenu_Unk_ov001_022ebc3c(NULL);
        return;
    }
    if (TimeAttackMenu_IsLeftPressed()) {
        w->unke0 = 1;
        w->unkd0 = w->unkd4;
        TimeAttackMenu_Unk_ov001_022eba70(NULL);
        return;
    }
    if (TimeAttackMenu_IsRightPressed()) {
        w->unke0 = 0;
        w->unkd0 = w->unkd4;
        TimeAttackMenu_Unk_ov001_022eba70(NULL);
    }
}

static void TimeAttackMenu_Action_Confirm(MnTask *t) {
    TaWork *w;
    s32 x = 0, y = 0;
    if (t == NULL)
        t = gTaskCurrent;
    w = (TaWork *)t->work;
    w->unk0 = 0;
    TimeAttackMenu_IndexToGrid(w, w->unkd0, &x, &y);
    TimeAttackMenu_CreateFadeTask(0, 0x10, 8, 8);
    TimeAttackMenu_Unk_ov001_022ea80c(w);
    TimeAttackMenu_Unk_ov001_022ea458(w, x, y);
    TimeAttackMenu_Unk_ov001_022ea7c4(w);
    TimeAttackMenu_Unk_ov001_022ea6e4(w);
    SysSound_PlaySE(0);
    t->func = TimeAttackMenu_Unk_ov001_022eab78;
}

static void TimeAttackMenu_Action_Back(MnTask *t) {
    TaWork *w;
    s32 to;
    if (t == NULL)
        t = gTaskCurrent;
    w = (TaWork *)t->work;
    w->unk0 = 0;
    to = w->unk4 <= 1 ? 0x10 : -16;
    if (w->unke4 != 0) {
        TimeAttackMenu_CreateFadeTask(0, to, 8, 8);
        TimeAttackMenu_Unk_ov001_022ea3e0(w);
    } else {
        TimeAttackMenu_CreateFadeTask(0, to, 8, 0);
        TimeAttackMenu_Unk_ov001_022ea41c(w);
    }
    TimeAttackMenu_Unk_ov001_022ea7c4(w);
    TimeAttackMenu_Unk_ov001_022ea6e4(w);
    SysSound_PlaySE(1);
    t->func = TimeAttackMenu_Unk_ov001_022ea894;
}

/* ZoneMapPause_ExecuteChoice(mode): where B leads */
static void ZoneMapPause_ExecuteChoice(s32 mode) {
    switch (mode) {
    case 0:
        PlayerSelectMenu_StartMode(3);   /* SceneChange_ToPlayerSelectMenuMode3 */
        return;
    case 1:
        PlayerSelectMenu_StartMode(4);   /* SceneChange_ToPlayerSelectMenuMode4 */
        return;
    case 2:
        Options_StartFromZoneMapPauseChoice2();
        return;
    case 3:
        Options_StartFromZoneMapPauseChoice3();
        return;
    }
}

/* Game_StartTimeAttackStage(zone, act): SceneChange_ToTimeAttackStage */
void Game_StartTimeAttackStage(s32 p0, s32 p1) {
    gGameState.unk10 = 2;
    gGameState.unk18 &= ~0x20u;
    gGameState.unk18 &= ~0x10u;
    if (p0 >= 0)
        gGameState.unk1C = (u8)p0;
    if (p1 >= 0)
        gGameState.unk1D = (u8)p1;
    mn_request_stage();
}

static void TimeAttackMenu_StartStage(s32 mode, s32 id, s32 b) {
    switch (mode) {
    case 0:
    case 1:
        NNS_SndStopSoundAll();   /* SysSound_Release */
        if (id < 7)
            id = zone_at(id);
        Game_StartTimeAttackStage(id, b);
        return;
    default:
        ZoneMapPause_ExecuteChoice(mode);
        break;
    }
}

static void TimeAttackMenu_Unk_ov001_022ea894(void) {   /* after B */
    TaWork *w = TASK_WORK(TaWork);
    if (w->unk0 >= 16) {
        u32 id = w->unk4;
        Task_DestroyByTag(0x1000);
        ZoneMapPause_ExecuteChoice((s32)id);
        return;
    }
    {
        s32 i;
        for (i = 5; i < 12; i++)
            KeyframeAnim_Update(&w->anims[i]);
    }
    TimeAttackMenu_DrawFrame(w, NULL, w->unkd0, 1, NULL);
    TimeAttackMenu_DrawBg1(w);
    if (w->unk0 == 8)
        TimeAttackMenu_Unk_ov001_022ea41c(w);
    w->unk0++;
}

static s32 s_mid_x, s_mid_y;
static void confirm_mid(TaWork *w) {
    TimeAttackMenu_Unk_ov001_022ea850(w);
    TimeAttackMenu_Unk_ov001_022ea504(w, s_mid_x, s_mid_y);
}
static void TimeAttackMenu_Unk_ov001_022eab78(void) {   /* after A */
    TaWork *w = TASK_WORK(TaWork);
    s32 x = 0, y = 0, i;
    TimeAttackMenu_IndexToGrid(w, w->unkd0, &x, &y);
    if (w->unk0 >= 16) {
        u32 id = w->unk4;
        Task_DestroyByTag(0x1000);
        if (x == 7) {
            x += y / 2;
            y %= 2;
        }
        TimeAttackMenu_StartStage((s32)id, x, y);
        return;
    }
    for (i = 5; i < 12; i++)
        KeyframeAnim_Update(&w->anims[i]);
    s_mid_x = x;
    s_mid_y = y;
    TimeAttackMenu_DrawFrame(w, NULL, w->unkd0, 1, w->unk0 == 8 ? confirm_mid : NULL);
    TimeAttackMenu_DrawBg1(w);
    w->unk0++;
}

/* --------------------------------------------------------------- setup */
static void TimeAttackMenu_ReleaseAssets(TaWork *w) {
    s32 i;
    for (i = 0; i < 5; i++) {
        mn_bg_free(w->bg[i]);
        w->bg[i] = NULL;
    }
    mn_bg_free(w->bg3sub);
    w->bg3sub = NULL;
    for (i = 0; i < 9; i++) {
        mn_res_free(w->res[i]);
        w->res[i] = NULL;
    }
    free(w->ama);
    w->ama = NULL;
}
static void TimeAttackMenu_Destructor(void) {
    TaWork *w = TASK_WORK(TaWork);
    /* TimeAttackMenu_ReleaseSprites, _Dummy, _ReleaseAssets, _ResetDisplayThunk */
    TimeAttackMenu_ReleaseAssets(w);
    mn_display_reset();
    Pad_Reset(&gPad, NULL, NULL);
    if (s_ta == w)
        s_ta = NULL;
}

/* gTimeAttackMenuFileTable[lang] */
static const char *const k_ta_act[6] = { "act/mnu_time_act_jpn.bac", "act/mnu_time_act_eng.bac",
                                         "act/mnu_time_act_fra.bac", "act/mnu_time_act_deu.bac",
                                         "act/mnu_time_act_ita.bac", "act/mnu_time_act_spa.bac" };
static void TimeAttackMenu_LoadAssets(TaWork *p) {
    int lang = mn_lang(), jpn = lang == 0;
    p->bg[0] = mn_bg_load("bg/mnu_time_base.bbg");
    p->bg[1] = mn_bg_load("bg/mnu_time_rankbase.bbg");
    p->bg[2] = mn_bg_load("bg/mnu_time_selectbase.bbg");
    p->bg[3] = mn_bg_load("bg/mnu_time_reco_image.bbg");
    p->bg[4] = mn_bg_load("bg/mnu_time_image.bbg");
    p->bg3sub = mn_bg_load("bg/mnu_time_base.bbg");
    p->res[0] = mn_res_load("act/mnu_time_ranking.bac");
    p->res[1] = mn_res_load("act/mnu_time_select.bac");
    p->res[2] = mn_res_load(k_ta_act[lang]);
    /* entry 8 (act_lc) is skipped in modes 1/3, entry 9 (boss_lc) in modes 0/2 */
    if (p->unk4 == 0 || p->unk4 == 2)
        p->res[3] = mn_res_load(jpn ? "act/mnu_time_act_lc_jpn.bac" : "act/mnu_time_act_lc_eng.bac");
    else
        p->res[4] = mn_res_load(jpn ? "act/mnu_time_boss_lc_jpn.bac" : "act/mnu_time_boss_lc_eng.bac");
    p->res[5] = mn_res_load(jpn ? "act/mnu_time_oneself_jpn.bac" : "act/mnu_time_oneself_eng.bac");
    p->res[6] = mn_res_load("act/mnu_titl.bac");
    p->ama = mn_ama_load("ama/zone_sel.ama");
    p->res[8] = mn_res_load(jpn ? "act/mnu_time_act_x_jpn.bac" : "act/mnu_time_act_x_eng.bac");
}

/* TimeAttackMenu_InitBackgrounds: main BG3 base, BG2 selectbase, BG1 rankbase;
 * sub BG3 base, BG2 the character art */
static void TimeAttackMenu_InitBackgrounds(TaWork *p) {
    MnBgImg *art = p->unk4 <= 1 ? p->bg[4] : p->bg[3];
    Background_Show(1, 2, art);
    Background_Show(0, 3, p->bg[0]);
    Background_Show(1, 3, p->bg3sub);
    if (p->unk4 <= 1) {
        TimeAttackMenu_LoadBgPaletteRange(p->bg[0], 0, 0xf);
        TimeAttackMenu_LoadBgPaletteRange(p->bg3sub, 0, 0xf);
    } else {
        TimeAttackMenu_LoadBgPaletteRange(p->bg[0], 0x10, 0x1f);
        TimeAttackMenu_LoadBgPaletteRange(p->bg3sub, 0x10, 0x1f);
    }
    Background_Show(0, 1, p->bg[1]);
    Background_Show(0, 2, p->bg[2]);
    Background_SetSize(0, 1, 512, 256);   /* BG1CNT 0x4494: 512x256 plane */
    Background_SetSize(0, 2, 512, 256);
    gDisplayMain[1].x = 0x100;            /* gDisplayMain[2]: BG1 hidden until the HDMA */
    gDisplayMain[1].y = 0;
    gDisplayMain[2].x = 0;
    gDisplayMain[2].y = 0;
    gDisplayMain[3].x = 0;
    gDisplayMain[3].y = 0;
    gDisplaySub[3].x = 0;
    gDisplaySub[3].y = 0;
}

static void TimeAttackMenu_InitSprites(TaWork *p) {
    static const u16 k_ids[0x1f] = { 0, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
                                     16, 17, 18, 19, 0, 2, 2, 5, 0, 0, 0, 1, 0, 8, 0 };   /* data_ov001_023166cc */
    s32 i;
    MnRes *res = NULL;
    for (i = 0; i < 0x1f; i++) {
        if (i == 0)
            res = p->res[0];
        else if (i == 0x14)
            res = p->res[1];
        else if (i == 0x18)
            res = p->res[2];
        else if (i == 0x19)
            res = (p->unk4 == 0 || p->unk4 == 2) ? p->res[3] : p->res[4];
        else if (i == 0x1a)
            res = p->res[5];
        else if (i == 0x1c)
            res = p->res[6];
        else if (i == 0x1d)
            res = p->res[8];
        p->anim[i] = -1;
        /* TimeAttackMenu_InitSpriteSub (prio 0) for the plate, _InitSpriteMain (prio 1) */
        AnimatorSprite_Init(&p->spr[i], res, k_ids[i], (i == 0x18 ? 0u : 1u) << 17, i == 0x18 ? 1 : 0);
    }
    for (i = 0; i < 12; i++)
        KeyframeAnim_Init(&p->anims[i], p->ama, 0);
}

/* One frame: the sub screen (zigzag, the character art on the right, the
 * "TIME ATTACK" plate at the bottom left) is the backdrop at 2.5x; the main
 * screen's record table, zone box, arrows and back button (its zigzag left out,
 * so the backdrop shows between the bars) are a panel at 1.5x in the top left,
 * clear of the plate (MENUS.md 3). */
static void TimeAttackMenu_Compose(void) {
    mn_comp_reset(0);
    mn_comp_full(1);
    mn_comp_add(0, 0, 0, 256, 192, 8, 8, 1.5f, 1 << 3, 0);
}

void TimeAttackMenu_Create(s32 mode) {
    MnTask *t = Task_Create(TimeAttackMenu_State_Enter, TimeAttackMenu_Destructor, 0, 0x1000, 0x1000,
                            sizeof(TaWork));
    TaWork *w = (TaWork *)t->work;
    u16 buf[12];
    s32 i;
    s_ta = w;
    w->unk0 = 0;
    w->unk4 = (u32)mode;
    w->unkd0 = 0;
    switch (w->unk4) {
    case 0:
    case 2:
        w->unkd8 = 0xe;
        w->unkd8 += 4;
        break;
    case 1:
    case 3:
        w->unkd8 = 7;
        break;
    }
    switch (w->unk4) {
    case 0:
        w->unkd0 = gGameState.unk1D + StoryProgress_GetZoneIndex(gGameState.unk1C) * 2;
        break;
    case 1:
        w->unkd0 = StoryProgress_GetZoneIndex(gGameState.unk1C);
        break;
    }
    if (w->unkd0 >= w->unkd8)
        w->unkd0 = 0;
    mn_display_reset();   /* TimeAttackMenu_InitDisplay */
    TimeAttackMenu_Compose();
    g_mn_scene = "TimeAttackMenu";
    TimeAttackMenu_LoadAssets(w);
    TimeAttackMenu_InitBackgrounds(w);
    TimeAttackMenu_InitSprites(w);
    TimeAttackMenu_CreateBgScrollTask();
    if (BRIGHT_MAIN >= 0)
        BRIGHT_MAIN = 0x10;
    else
        BRIGHT_MAIN = -0x10;
    TimeAttackMenu_CreateFadeTask(BRIGHT_MAIN, 0, 8, 0);
    TimeAttackMenu_Unk_ov001_022ebc98(t);
    SysSound_PlayBGM(0);
    gSaveDataBusy = 0;
    for (i = 0; i < 12; i++)
        buf[i] = 4;
    Pad_Reset(&gPad, NULL, buf);
}

void TimeAttackMenu_StartMode0(void) { TimeAttackMenu_Create(0); }
void TimeAttackMenu_StartMode1(void) { TimeAttackMenu_Create(1); }
void TimeAttackMenu_StartMode2(void) {
    gGameState.unk0 = 0;
    gGameState.unk1C = 0;
    gGameState.unk1D = 0;
    TimeAttackMenu_Create(2);
}
void TimeAttackMenu_StartMode3(void) {
    gGameState.unk0 = 0;
    gGameState.unk1C = 0;
    gGameState.unk1D = 0;
    TimeAttackMenu_Create(3);
}

/* host test access */
int TimeAttackMenu_Selected(void) { return s_ta ? s_ta->unkd0 : -1; }

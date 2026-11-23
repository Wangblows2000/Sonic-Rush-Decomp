/*
 * HUD and score popup, translated from src/game/UI/Hud:
 *   Hud_Create, Hud_Main, Hud_DrawRings, Hud_DrawTime, Hud_DrawLives,
 *   Hud_DrawTensionGauge, Hud_UpdateTensionGauge, Hud_SetTensionGauge,
 *   Hud_TensionToGaugeValue, Hud_ToDecimalDigits, Hud_FramesToTimeDigits,
 *   Hud_Set* / Hud_Get*, ScorePopup_Create/_State_Main/_Draw/_IsActive/_Destructor.
 *
 * DS services replaced:
 *   - AnimatorSpriteDS (one sprite drawn on both engines, second copy at y-0xC0)
 *     -> one UiSpr drawn once; the HUD is one 256x192 layout split into anchor
 *     groups (rings/time top-left, gauge/lives bottom-left, score top-right).
 *   - Digit glyph VRAM swapping (unk5C = vram + digit * 0x80, white set vram[0],
 *     orange set vram[2]) -> the digit animation itself (0..9 white, 10..19 orange
 *     in ac_fix_moji).
 *   - PaletteTransfer of the gauge colours (palette 1 colours 1 and 2) -> texture
 *     variant UI_VAR_GAUGE0 + colour index (the converted bars are white where the
 *     DS palette has those two colours).
 *   - REG_DIV -> C division (same quotient/remainder for these non-negative values).
 * Field names keep the original offsets (unk28C = work+0x28C ...).
 */
#include "ui_internal.h"

typedef struct HudWork {
    u32 flags;   /* 0x000 1 time off, 2 rings off, 4 gauge off, 8 lives off, 0x10 no life digits,
                    0x20 tension max (Player_In), 0x80000000 full-gauge flash phase */
    s16 unk4, unk6; /* 0x004 */
    struct {     /* 0x01C rings */
        s16 x, y;
        UiSpr a;       /* 0x020 anim 0x21 ring icon */
        UiSpr b;       /* 0x09C anim 0x22 counter frame */
        UiSpr c[3];    /* 0x118 digits */
        s32 unk270;    /* 0x28C ring count */
        u32 unk274;    /* 0x290 digits */
        u32 prev;      /* 0x294 */
    } rings;
    struct {     /* 0x298 time */
        s16 x, y;
        UiSpr d4;      /* 0x29C anim 0x14 clock */
        UiSpr d80;     /* 0x318 minutes */
        UiSpr dfc[2];  /* 0x394 seconds */
        UiSpr d1f4[2]; /* 0x48C 1/100 s */
        UiSpr d2ec;    /* 0x584 anim 0x15 */
        UiSpr d368;    /* 0x600 anim 0x15 */
        u32 time;      /* 0x67C */
        u32 digits;    /* 0x680 */
        u32 prev;      /* 0x684 */
    } time;
    struct HudGauge { /* 0x688 tension gauge */
        s16 x, y;
        UiSpr d[5];    /* 0x68C 0x1d, 0x1e, 0x1b, 0x1c, 0x23.. */
        s16 unk270, unk272; /* 0x8F8 target / shown tension (0..300) */
        u16 unk274, unk276;
        s32 unk278[5];      /* scale[0..3], unk278[4] new-segment scale */
        u16 unk28C, unk28E, unk290;
        s16 unk292, unk294;
        s32 unk298;         /* mode */
    } gauge;
    struct {     /* 0x924 lives */
        s16 x, y;
        u16 unk928;    /* 0x928 Hud_SetLifeCount */
        UiSpr spr[3];  /* 0x92C icon (0x1f/0x20), 0x9A8 'x' (0x19), 0xA24 digit */
        u32 unk17C;    /* 0xAA0 digits */
        u32 unk180;    /* 0xAA4 */
    } lives;
    /* native */
    u32 score, score_digits;
    UiSpr score_spr[6];
    s16 tension_max;   /* gPlayers[0]->unk3B6 */
    u32 oam_prio;      /* Hud_SetOamPriority: 3 = behind the BG (title card) -> not drawn */
    int active;
} HudWork;

static HudWork s_hud;
#define gHudTask (s_hud.active ? &s_hud : (HudWork *)0)

static void InitSpr(UiSpr *s, u16 anim) {
    UiSpr_Init(s, UI_RES_MOJI, anim, UI_SPR_LOOP);
}

void Hud_Create(void) {
    HudWork *work = &s_hud;
    u32 i;
    memset(work, 0, sizeof(*work));
    work->rings.unk274 = (u32)-1; /* unk290 = -1 */
    work->time.digits = (u32)-1;  /* unk680 = -1 */
    work->lives.unk17C = (u32)-1; /* unkaa0 = -1 */
    InitSpr(&work->rings.a, 0x21);
    InitSpr(&work->rings.b, 0x22);
    for (i = 0; i < 3; i++)
        InitSpr(&work->rings.c[i], 0x22);
    InitSpr(&work->time.d4, 0x14);
    InitSpr(&work->time.d80, 0);
    for (i = 0; i < 2; i++) {
        InitSpr(&work->time.dfc[i], 0);
        InitSpr(&work->time.d1f4[i], 0);
    }
    InitSpr(&work->time.d2ec, 0x15);
    InitSpr(&work->time.d368, 0x15);
    for (i = 0; i < 4; i++)
        work->gauge.unk278[i] = 0x1000; /* sub->scale[i] = 0x1000 */
    InitSpr(&work->gauge.d[0], 0x1d);
    InitSpr(&work->gauge.d[1], 0x1e);
    InitSpr(&work->gauge.d[2], 0x1b);
    InitSpr(&work->gauge.d[3], 0x1c);
    InitSpr(&work->gauge.d[4], 0x23);
    /* gGameState.unk0 == 0 ? 0x1f : 0x20 (Sonic / Blaze life icon) */
    InitSpr(&work->lives.spr[0], G.character == 0 ? 0x1f : 0x20);
    InitSpr(&work->lives.spr[1], 0x19);
    InitSpr(&work->lives.spr[2], 0);
    for (i = 0; i < 6; i++)
        InitSpr(&work->score_spr[i], 0);
    work->active = 1;
    /* Hud_SetOamPriority0 / palette rows: not needed (one draw list) */
}

void Hud_Destroy(void) { s_hud.active = 0; }

/* ------------------------------------------------------------ setters */
void Hud_SetLifeCount(u16 v) {
    if (gHudTask != NULL)
        gHudTask->lives.unk928 = v;
}
void Hud_SetRingCount(s32 v) {
    if (gHudTask != NULL)
        gHudTask->rings.unk270 = v;
}
void Hud_SetTime(s32 v) {
    if (gHudTask != NULL)
        gHudTask->time.time = (u32)v;
}
void Hud_SetFlags(s32 v) {
    if (gHudTask != NULL)
        gHudTask->flags = (u32)v;
}
s32 Hud_GetFlags(void) {
    if (gHudTask != NULL)
        return (s32)gHudTask->flags;
    return 0;
}
void Hud_SetScore(u32 v) { s_hud.score = v; }
/* Hud_SetOamPriority (0/2/3): the OAM priority bits of every HUD sprite. With no
 * BG layer above the HUD on the DC, priority 3 (under the title card BG) hides it. */
void Hud_SetOamPriority(u32 prio) {
    if (gHudTask == NULL)
        return;
    gHudTask->oam_prio = prio;
}
void Hud_SetTensionMax(s16 v) { s_hud.tension_max = v; }

/* ------------------------------------------------- Hud_ToDecimalDigits */
void Hud_ToDecimalDigits(u32 *out, u32 v, u32 n) {
    s16 sh = 0;
    u32 q;
    s16 i;
    u32 o;
    s32 j;
    if (v >= n) {
        if (n == 0)
            v = 0;
        else
            v = n - 1;
    }
    *out = 0;
    if (n >= 100) {
        n = n / 10;
        sh = 0;
        for (;;) {
            if (v >= n) {
                q = v / n;
                *out |= q << sh;
                v -= q * n;
            }
            sh += 4;
            if (n <= 10)
                break;
            n = n / 10;
        }
    }
    *out |= v << sh;
    for (i = sh; i >= sh >> 1; i -= 4) {
        o = *out;
        j = sh - i;
        *out = o & ~((0xfu << i) | (0xfu << j));
        *out |= (((o & (0xfu << j)) >> j) << i) | (((o & (0xfu << i)) >> i) << j);
    }
}

/* ---------------------------------------------- Hud_FramesToTimeDigits */
void Hud_FramesToTimeDigits(u32 *out, u32 t) {
    s16 sh;
    s16 i;
    u32 d;
    s32 q;
    if (t > 0x8c9f)
        t = 0x8c9f;
    i = 0;
    *out = 0;
    sh = 0x1c;
    while (1) {
        d = g_ui_data.time_div[i]; /* data_ov000_02364d70 */
        if (d != 0 && t >= d) {
            q = (s32)(t / d);
            *out |= (u32)q << sh;
            t -= (u32)q * d;
        }
        sh -= 4;
        if (sh == 4) {
            t = (t * 0x1b1) >> 8;
            if (t >= 100)
                t = 99;
        }
        if (sh == 0)
            break;
        i++;
    }
    *out |= t << sh;
}

/* -------------------------------------------------- tension gauge logic */
s32 Hud_TensionToGaugeValue(s32 v) {
    if (v < 0)
        v = 0;
    else if (v > 50)
        v = 50;
    return ((v << 16) >> 4) / 40 + 0x1800;
}

void Hud_SetTensionGauge(s32 v, s32 mode) {
    u32 i;
    if (gHudTask == NULL)
        return;
    {
        struct HudGauge *s = &gHudTask->gauge;
        if ((u32)v > 300)
            v = 300;
        s->unk298 = mode;
        if (s->unk270 < s->unk272) {
            if (s->unk270 >= v) {
                if (s->unk298 == 1)
                    s->unk290 = 0x28;
                s->unk270 = (s16)v;
                s->unk278[4] = 0;
                return;
            }
            s->unk272 = s->unk270;
            s->unk270 = (s16)v;
            s->unk274 = s->unk276 = 0;
            s->unk278[4] = Hud_TensionToGaugeValue(s->unk270 - s->unk272);
            return;
        }
        if (s->unk270 > s->unk272) {
            if (s->unk270 <= v) {
                s->unk270 = (s16)v;
                v = Hud_TensionToGaugeValue(s->unk270 - s->unk272);
                if (s->unk278[4] < v)
                    s->unk278[4] = v;
                return;
            }
            if (s->unk298 == 1) {
                s->unk290 = 0x28;
                s->unk272 = s->unk270;
                s->unk270 = (s16)v;
                s->unk274 = s->unk276 = 0;
                s->unk278[4] = 0;
                for (i = 0; i < 4; i++)
                    s->unk278[i] = 0x1000;
                return;
            }
            s->unk270 = (s16)v;
            s->unk274 = s->unk276 = 0;
            return;
        }
        if (s->unk270 > v) {
            if (s->unk298 == 1)
                s->unk290 = 0x28;
            s->unk278[4] = 0;
            for (i = 0; i < 4; i++)
                s->unk278[i] = 0x1000;
        } else if (s->unk270 < v) {
            s->unk278[4] = Hud_TensionToGaugeValue(v - s->unk270);
        } else {
            s->unk278[4] = 0;
        }
        s->unk270 = (s16)v;
        s->unk274 = s->unk276 = 0;
    }
}

static void Hud_UpdateTensionGauge(HudWork *w) {
    struct HudGauge *s = &w->gauge;
    s32 sh;
    s32 i;
    s->unk292 = s->unk294 = 0;
    if (s->unk290 != 0) {
        s->unk290--;
        if (W.frame & 1) {
            sh = (40 - s->unk290) >> 4;
            s->unk292 = (s16)((4 - (UiRand() & 7)) >> sh);
            s->unk294 = (s16)((4 - (UiRand() & 7)) >> sh);
        }
    }
    if (s->unk270 < s->unk272) {
        if (s->unk274 != 0)
            s->unk274--;
        if (s->unk274 == 0) {
            s->unk272--;
            if (s->unk272 < s->unk270)
                s->unk272 = s->unk270;
            s->unk274 = 1;
        }
    } else {
        if (s->unk270 > s->unk272) {
            if (s->unk276 != 0)
                s->unk276--;
            if (s->unk276 == 0) {
                s->unk272 += 2;
                if (s->unk272 > s->unk270)
                    s->unk272 = s->unk270;
            }
            s->unk28E = s->unk28C;
            s->unk28C = (u16)(s->unk272 / 5);
            if (s->unk28E != s->unk28C) {
                for (i = 2; i >= 0; i--)
                    s->unk278[i + 1] = s->unk278[i];
                s->unk278[0] = s->unk278[4];
            }
        }
        for (i = 0; i < 4; i++) {
            if (s->unk278[i] > 0x1000) {
                s->unk278[i] -= 0x333;
                if (s->unk278[i] < 0x1000)
                    s->unk278[i] = 0x1000;
            }
        }
    }
    s->x = s->unk292; /* s->unk0 */
    s->y = s->unk294; /* s->unk2 */
}

/* ------------------------------------------------------------ drawing */
static void PutSpr(UiSpr *spr, s16 x, s16 y, int space) {
    spr->x = x;
    spr->y = y;
    ui_emit_spr(spr, space);
}

static void Hud_DrawRings(HudWork *w) {
    s16 x = w->unk4 + w->rings.x;
    s16 y = w->unk6 + w->rings.y + 2;
    int orange = w->rings.unk270 == 0 && (W.frame & 8); /* chr = unk14 (orange glyphs) */
    u32 i, sh = 0;
    s32 ofs = 0;
    for (i = 0; i < 3; i++) {
        u16 c = (u16)((w->rings.unk274 >> sh) & 0xf);
        UiSpr *spr = &w->rings.c[i];
        UiSpr_SetAnimation(spr, (u16)(c + (orange ? 10 : 0)));
        UiSpr_ProcessAnimation(spr);
        PutSpr(spr, (s16)(x + 0x30 - ofs), (s16)(y + 0x10), UI_SP_HUD_TL);
        sh += 4;
        ofs += 10;
    }
    PutSpr(&w->rings.a, x, y, UI_SP_HUD_TL);
    PutSpr(&w->rings.b, x, y, UI_SP_HUD_TL);
}

static void TimeDigit(UiSpr *d, u32 dig, int orange, s16 x, s16 y) {
    UiSpr_SetAnimation(d, (u16)(dig + (orange ? 10 : 0)));
    UiSpr_ProcessAnimation(d);
    PutSpr(d, x, y, UI_SP_HUD_TL);
}

static void Hud_DrawTime(HudWork *work) {
    s16 x = (s16)(work->unk4 + work->time.x + 0xa8);
    s16 y = (s16)(work->unk6 + work->time.y + 0x12);
    s16 off = 0;
    s32 px, pq, px2;
    u16 i;
    u32 dig;
    /* time >= 9:00 blinks orange (vram[2]) */
    int orange = work->time.time >= 0x7e90 && (W.frame & 8);
    px = x;
    for (i = 0; i < 2; i++) {
        dig = (work->time.digits >> (i * 4)) & 0xf;
        off -= 10;
        px -= 10;
        TimeDigit(&work->time.d1f4[i], dig, orange, (s16)px, y);
    }
    PutSpr(&work->time.d2ec, (s16)(x + (s16)(off - 8)), y, UI_SP_HUD_TL);
    off -= 6;
    pq = x + off;
    for (i = 0; i < 2; i++) {
        dig = (work->time.digits >> ((i + 2) * 4)) & 0xf;
        off -= 10;
        pq -= 10;
        TimeDigit(&work->time.dfc[i], dig, orange, (s16)pq, y);
    }
    PutSpr(&work->time.d368, (s16)(x + (s16)(off - 8)), y, UI_SP_HUD_TL);
    dig = (work->time.digits >> 16) & 0xf;
    px2 = x + (s16)(off - 6);
    TimeDigit(&work->time.d80, dig, orange, (s16)(px2 - 10), y);
    PutSpr(&work->time.d4, (s16)(x + (s16)((s16)(off - 16) - 14)), y, UI_SP_HUD_TL);
}

static void Hud_DrawLives(HudWork *w) {
    s16 x = (s16)(w->lives.x + w->unk4 + 4);
    s16 y = (s16)(w->lives.y + w->unk6 + 0xaa);
    u32 cur;
    PutSpr(&w->lives.spr[0], x, y, UI_SP_HUD_BL);
    if (w->flags & 0x10)
        return;
    PutSpr(&w->lives.spr[1], (s16)(x + 0x18), (s16)(y + 0x12), UI_SP_HUD_BL);
    cur = w->lives.unk17C & 0xf;
    if (cur != (w->lives.unk180 & 0xf)) {
        UiSpr_SetAnimation(&w->lives.spr[2], (u16)cur);
        UiSpr_ProcessAnimation(&w->lives.spr[2]);
    }
    PutSpr(&w->lives.spr[2], (s16)(x + 0x22), (s16)(y + 0x12), UI_SP_HUD_BL);
}

static void Hud_DrawTensionGauge(HudWork *work) {
    struct HudGauge *sub = &work->gauge;
    UiSpr *d, *d2;
    s32 pal, i, n, q;
    u32 flags;
    s16 x = (s16)(sub->x + work->unk4 + 4);
    s16 y = (s16)(sub->y + work->unk6 + 0xaa);
    int c1, c2;
    q = sub->unk272 / 100;
    n = (sub->unk272 % 100) / 5;
    if (sub->unk270 >= sub->unk272 && q >= 1 && n == 0) {
        q--;
        n = 20;
    }
    if (sub->unk272 < 300) {
        pal = q;
    } else {
        flags = work->flags;
        if (W.frame & 4)
            pal = 4;
        else
            pal = 5;
        if (flags & 0x20) {
            work->flags = flags | 0x80000000u;
        } else if (flags & 0x80000000u) {
            if (pal == 4)
                work->flags = flags & ~0x80000000u;
        } else {
            pal = 4;
        }
    }
    /* PaletteTransfer: palette 1 colour 1 = data_ov000_02364d60[pal], colour 2 = [pal + 1] */
    c1 = UI_VAR_GAUGE0 + pal;
    c2 = UI_VAR_GAUGE0 + pal + 1;
    d = &sub->d[1];
    d2 = &sub->d[3];
    for (i = 0; i < 4 && n - i > 0; i++) {
        d->x = x;
        d->y = (s16)(y - (n - i - 1) * 6);
        if (sub->unk270 < sub->unk272 && sub->unk298 != 0 && sub->unk298 == 1 && i == 0) {
            u16 r = (u16)((W.frame >> 1) * 0x196225 + 0x3c6ef35f);
            d->x += 4 - (r & 7);
            d->y += 4 - ((r * 0x196225 + 0x3c6ef35f) & 7);
        }
        ui_emit(d, UI_SP_HUD_BL, sub->unk278[i], 0x1000, 0, 0xffffffff, c2);
    }
    for (; (n - i) & 3; i++) {
        d->x = x;
        d->y = (s16)(y - (n - i - 1) * 6);
        ui_emit(d, UI_SP_HUD_BL, 0x1000, 0x1000, 0, 0xffffffff, c2);
    }
    for (; i < n; i += 4) {
        d2->x = x;
        d2->y = (s16)(y - (n - i - 4) * 6);
        ui_emit(d2, UI_SP_HUD_BL, 0x1000, 0x1000, 0, 0xffffffff, c2);
    }
    i = n;
    d = &sub->d[0];
    d2 = &sub->d[2];
    for (; i & 3; i++) {
        d->x = x;
        d->y = (s16)(y - i * 6);
        ui_emit(d, UI_SP_HUD_BL, 0x1000, 0x1000, 0, 0xffffffff, c1);
    }
    for (; i < 20; i += 4) {
        d2->x = x;
        d2->y = (s16)(y - i * 6);
        ui_emit(d2, UI_SP_HUD_BL, 0x1000, 0x1000, 0, 0xffffffff, c1);
    }
    d = &sub->d[4];
    if (q == 2 && n == 20) {
        s16 v = work->tension_max;
        if (d->anim != 0x26)
            UiSpr_SetAnimation(d, 0x26);
        if (v > 0) {
            d->speed = v >> 2;
            if (d->speed < 8)
                d->speed = 8;
            d->flags &= ~UI_SPR_STOPPED;
        }
    } else if (q == 2) {
        if (d->anim != 0x25) {
            UiSpr_SetAnimation(d, 0x25);
            d->speed = 0x10;
        }
        d->flags &= ~UI_SPR_STOPPED;
    } else if (q == 1) {
        if (d->anim != 0x24) {
            UiSpr_SetAnimation(d, 0x24);
            d->speed = 0x10;
        }
        d->flags &= ~UI_SPR_STOPPED;
    } else {
        if (d->anim != 0x23) {
            UiSpr_SetAnimation(d, 0x23);
            d->speed = 0x10;
        }
        d->flags &= ~UI_SPR_STOPPED;
    }
    d->x = x;
    d->y = (s16)(y - 0x78);
    UiSpr_ProcessAnimation(d);
    ui_emit_spr(d, UI_SP_HUD_BL);
}

/* native: score counter (the DS HUD shows no score; ScorePopup/results only).
 * Six white digits, right-aligned at the top-right, same baseline as the timer. */
static void Hud_DrawScore(HudWork *w) {
    int i, on = 0;
    Hud_ToDecimalDigits(&w->score_digits, w->score, 1000000);
    for (i = 5; i >= 0; i--) {
        u32 dg = (w->score_digits >> (i * 4)) & 0xf;
        if (!on && dg == 0 && i != 0)
            continue;
        on = 1;
        UiSpr_SetAnimation(&w->score_spr[i], (u16)dg);
        PutSpr(&w->score_spr[i], (s16)(256 - 6 - 10 * i), 0x12, UI_SP_HUD_TR);
    }
}

void Hud_Main(void) {
    HudWork *t1 = gHudTask;
    if (t1 == NULL)
        return;
    if (t1->oam_prio == 3)
        ui_cmd_mark_hidden_begin();
    t1->rings.prev = t1->rings.unk274;
    Hud_ToDecimalDigits(&t1->rings.unk274, (u32)t1->rings.unk270, 1000);
    if ((t1->flags & 2) == 0)
        Hud_DrawRings(t1);
    t1->time.prev = t1->time.digits;
    Hud_FramesToTimeDigits(&t1->time.digits, t1->time.time);
    if ((t1->flags & 1) == 0)
        Hud_DrawTime(t1);
    Hud_UpdateTensionGauge(t1);
    if ((t1->flags & 4) == 0)
        Hud_DrawTensionGauge(t1);
    t1->lives.unk180 = t1->lives.unk17C;
    Hud_ToDecimalDigits(&t1->lives.unk17C, t1->lives.unk928, 0xa);
    if ((t1->flags & 8) == 0)
        Hud_DrawLives(t1);
    if ((t1->flags & 0x40000000u) == 0) /* native flag: hide the score counter */
        Hud_DrawScore(t1);
    if (t1->oam_prio == 3)
        ui_cmd_mark_hidden_end();
}

/* ===================================================== ScorePopup */
typedef struct ScorePopupWork {
    s32 unk8, unkC;      /* StageObj w8 (scale), wC (settle counter) */
    s32 unk10, unk14;    /* position (24.8 world) */
    s16 unk44, unk46;    /* draw scale (0 = 1.0, else /0x100) */
    u32 unk54;           /* bit 2: destroy */
    const s32 *unk88x, *unk88y; /* parent position (followed) */
    s16 unk134[5];
    s16 unk13E;
    u32 unk140;          /* shown digits */
    u32 unk144;          /* shown value */
    u32 unk148;          /* target value */
    UiSpr spr0;
    int active;
} ScorePopupWork;
static ScorePopupWork s_popup;
static s32 s_popup_fx, s_popup_fy;  /* fixed anchor when no parent follows */
#define data_ov000_0236a980 (s_popup.active ? &s_popup : (ScorePopupWork *)0)

int ScorePopup_IsActive(void) { return data_ov000_0236a980 != NULL; }

/* ScorePopup_Create(arg, n): arg = the object the popup follows (unk88).
 * Native: the follow target is given by position pointers (ui_score_follow). */
static void ScorePopup_CreateEx(const s32 *px, const s32 *py, s32 n) {
    ScorePopupWork *w;
    s16 i;
    if (data_ov000_0236a980 == NULL) {
        if (n == 0)
            return;
        w = &s_popup;
        memset(w, 0, sizeof(*w));
        UiSpr_Init(&w->spr0, UI_RES_MOJI, 0, 0);
        w->active = 1;
        w->unk144 = (u32)n;
    } else {
        w = &s_popup;
        if (n == 0) {
            w->unk148 = 0;
            w->unk144 = 0;
            w->unk140 = 0;
            return;
        }
    }
    for (i = 0; i < 5; i++)
        w->unk134[i] = (s16)(i * 2 + 8);
    w->unk10 = *px;
    w->unk14 = *py;
    w->unk88x = px;
    w->unk88y = py;
    w->unkC = 0;
    w->unk8 = 0;
    w->unk44 = 0;
    w->unk46 = 0;
    w->unk13E = 0;
    w->unk148 += (u32)n;
    if (w->unk144 == 0)
        w->unk144 = w->unk148;
    Hud_ToDecimalDigits(&w->unk140, w->unk144, 100000);
    if (w->unk148 > 2000)
        w->unk8 = 0x180;
}

void ScorePopup_Create(s32 x, s32 y, s32 n) {
    s_popup_fx = x;
    s_popup_fy = y;
    ScorePopup_CreateEx(&s_popup_fx, &s_popup_fy, n);
}

/* native entry for the player hook: follow the player like arg = player work */
void ui_score_follow(const DcPlayer *p, s32 n) { ScorePopup_CreateEx(&p->x, &p->y, n); }

void ScorePopup_Destroy(void) {
    /* ScorePopup_Destructor also clears the parent's unkC8 / unk110 (player trick
     * state the DcPlayer does not model) */
    s_popup.active = 0;
}

static void ScorePopup_State_Main(ScorePopupWork *w) {
    int done = 1;
    s16 i;
    if (w->unk88x != NULL) {
        w->unk10 = *w->unk88x;
        w->unk14 = *w->unk88y;
    }
    if (w->unk148 != w->unk144) {
        if (((w->unk148 - w->unk144) >> 2) == 0)
            w->unk144 = w->unk148;
        w->unk144 = w->unk144 + ((w->unk148 - w->unk144) >> 2);
        Hud_ToDecimalDigits(&w->unk140, w->unk144, 100000);
    }
    for (i = 0; i < 5; i++) {
        if (w->unk134[i] != 0) {
            w->unk134[i]--;
            done = 0;
        }
    }
    if (done) {
        if (w->unk8 != 0) {
            w->unk8 -= 0x10;
            if (w->unk8 < 0x100)
                w->unk8 = 0;
        }
        if (w->unk148 == w->unk144)
            w->unkC++;
        if (w->unkC > 0x10)
            w->unk13E -= 0x180;
    }
    if (w->unk13E < -0x2800)
        w->unk54 |= 4;
}

static void ScorePopup_Draw(ScorePopupWork *w) {
    s16 i;
    int on = 0, orange = 0;
    u32 sh;
    s32 off, d;
    if (w->unk13E < -0x1000 && (W.frame & 2))
        return;
    for (i = 4, sh = 16, off = 0x2800; i >= 0; sh -= 4, off -= 0xa00, i--) {
        if (w->unk134[i] <= 8 && (on || (w->unk140 & (0xfu << sh)))) {
            s32 sx = 0x1000, sy = 0x1000;
            on = 1;
            orange = 0;
            if (w->unk148 >= 2000) {
                w->unk44 = (s16)w->unk8;
                w->unk46 = (s16)w->unk8;
                if (W.frame & 2)
                    orange = 1; /* Hud_GetDigitGlyphVramB */
            }
            d = (s32)((w->unk140 & (0xfu << sh)) >> sh);
            UiSpr_SetAnimation(&w->spr0, (u16)(d + (orange ? 10 : 0)));
            /* StageObj_Draw2DEx: world position, scale unk44/unk46 (/0x100) */
            w->spr0.x = (s16)((w->unk10 + (w->unk134[i] << 8) - off + 0x1800) >> 8);
            w->spr0.y = (s16)((w->unk13E + (w->unk14 - (w->unk134[i] << 9)) - 0x1400) >> 8);
            if (w->unk44 != 0)
                sx = (sx * w->unk44) >> 8;
            if (w->unk46 != 0)
                sy = (sy * w->unk46) >> 8;
            ui_emit(&w->spr0, UI_SP_WORLD, sx, sy, 0, 0xffffffff, 0);
        }
    }
}

/* task body: StageObj_Main -> state, then draw; destroyed when unk54 & 4 */
void ScorePopup_Main(void) {
    ScorePopupWork *w = data_ov000_0236a980;
    if (w == NULL)
        return;
    if (w->unk54 & 4) {
        ScorePopup_Destroy();
        return;
    }
    ScorePopup_State_Main(w);
    ScorePopup_Draw(w);
}

/* test access */
void ui_debug_gauge(s16 *target, s16 *shown, u32 *digits_rings, u32 *digits_time) {
    *target = s_hud.gauge.unk270;
    *shown = s_hud.gauge.unk272;
    *digits_rings = s_hud.rings.unk274;
    *digits_time = s_hud.time.digits;
}

/* Battle Play: one HUD per player (the work swapped around each player's frame and draw) */
size_t hud_ctx_size(void) { return sizeof(HudWork); }
void hud_ctx_save(void *dst) { memcpy(dst, &s_hud, sizeof s_hud); }
void hud_ctx_load(const void *src) { memcpy(&s_hud, src, sizeof s_hud); }
/* the Battle Play HUD of one player: Hud_SetRingCount / Hud_SetTime as ui_post_update does, no lives
 * (Battle Play has none) and no score, then the HUD quads alone */
void ui_vs_hud_draw(const DcPlayer *p, u32 frame) {
    if (gHudTask == NULL)
        Hud_Create();   /* StageTitleCard creates it at GO; Battle Play has no title card */
    if (gHudTask == NULL)
        return;
    ui_cmd_reset();
    Hud_SetRingCount(p ? p->rings : 0);
    Hud_SetTime((s32)frame);
    /* the title card (StageTitleCard) hides the HUD until GO (OAM priority 3, flags 0x10); Battle Play
     * has no title card: rings, time and gauge shown, no lives (8), no score */
    s_hud.oam_prio = 0;
    s_hud.flags = (s_hud.flags & ~0x17u) | 8 | 0x40000000u;
    Hud_Main();
    {   /* the stage's master brightness belongs to the single-player flow (not run in Battle Play) */
        s16 b = g_ui_bright_main;
        g_ui_bright_main = 0;
        ui_draw_tr();
        g_ui_bright_main = b;
    }
    ui_cmd_reset();
}


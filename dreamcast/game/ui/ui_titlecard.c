/*
 * Stage title card ("SONIC THE HEDGEHOG / LEAF STORM / ZONE 1 ACT 1 / On your mark /
 * Ready? / GO!"), translated from src/game/UI/TitleCard:
 *   StageTitleCard_Create, _Main, _InitSprites, _State_* (Init .. Finish),
 *   _HoldPlayerNone/X/Y, _DrawGlyphText, StageTitleCardStrip_*, StageTitleCardPart20_*,
 *   StageTitleCardPartE0_*, StageTitleCardPart180_*, StageTitleCardPart260_*,
 *   StageTitleCardPart324_*, StageTitleCardText3E8_*, StageTitleCardText404_*,
 *   StageTitleCardPart420_*, StageTitleCardBg_* (state here, drawing in ui_bg.c),
 *   StageTitleCardPlayer_* (state and camera/zoom values only).
 *
 * Every timer, lerp and flag is the original; positions are DS screen pixels in
 * 20.12 fixed point and are drawn in UI_SP_SCREEN.
 * The BG layers (stdm_zigzag + stdm_bg<zone>, tools/uibg.py) are composed into the
 * one frame by ui_bg.c.  Not drawn (UI.md): the player model close-up
 * (StageTitleCardPlayer_Draw: ui_titlecard_player_view exposes the values).
 * gDisplayMain master brightness -> the frame overlay, gDisplaySub -> the composed
 * zone art (g_ui_bright_main / g_ui_bright_sub, ui_draw.c).
 */
#include "ui_internal.h"

typedef struct TcWork TcWork;
typedef void (*TcState)(TcWork *w);

/* object with two sprites and the 0x4C.. fields (Strip, Part20, Part180, Part420) */
typedef struct TcObj {
    void (*fn)(TcWork *w, struct TcObj *o);
    UiSpr spr;
    s32 unk4C, unk50, unk54, unk58, unk5C, unk60;
    s16 unk64, unk66;
    s32 unk68;
    u16 delay;   /* 0x6C */
    s16 t;       /* 0x6E */
} TcObj;
/* PartE0: rotating zone circle + dots */
typedef struct TcObjE0 {
    void (*fn)(TcWork *w, struct TcObjE0 *o);
    UiSpr spr, spr2;
    s32 unk94, unk98;
    u16 unk9C;
    s16 unk9E;
} TcObjE0;
/* Part260 / Part324: two sprites scaled about a pivot */
typedef struct TcObjPair {
    void (*fn)(TcWork *w, struct TcObjPair *o);
    UiSpr spr[2];
    struct { s32 x, y; } pt[2];  /* 0x94 */
    s32 unkA4, unkA8;            /* centre */
    s32 unkAC, unkB0;            /* exit start */
    s32 unkB4, unkB8;            /* pivot */
    s16 unkBC, unkBE;            /* scale */
    s16 unkC0;                   /* t / delay */
} TcObjPair;
/* Text3E8 / Text404: glyph strings */
typedef struct TcText {
    void (*fn)(TcWork *w, struct TcText *o);
    const u8 *str;   /* unk4 */
    u16 count;       /* unk8 */
    s32 unkC, unk10, unk14; /* x, y, spacing */
    s16 t;           /* 0x18 */
} TcText;
/* Bg (scroll state only) */
typedef struct TcBg {
    void (*fn)(TcWork *w, struct TcBg *o);
    s32 unk4, unk8;
    s16 unkC;
} TcBg;
/* Player close-up / camera */
typedef struct TcPlayer {
    void (*fn)(TcWork *w, struct TcPlayer *o);
    s16 t;           /* 0xC */
} TcPlayer;

struct TcWork {
    TcPlayer fn0;            /* 0x000 */
    TcObj fn20;              /* 0x020 Part20 */
    TcBg fn78;               /* 0x078 */
    TcObj fn88;              /* 0x088 Strip */
    TcObjE0 fne0;            /* 0x0E0 */
    TcObj fn180, fn1f0;      /* 0x180 / 0x1F0 */
    TcObjPair fn260;         /* 0x260 ZONE n */
    TcObjPair fn324;         /* 0x324 ACT n */
    TcText fn3e8, fn404;     /* 0x3E8 / 0x404 */
    TcObj fn420;             /* 0x420 GO! */
    TcState fn474;
    void (*fn478)(TcWork *w, DcPlayer *p);
    s32 idx;                 /* 0x47C character */
    s32 x480, y484;          /* player position at create */
    u32 flags;               /* 0x488 */
    u32 flags2;              /* 0x48C */
    s32 v4b4[6];             /* camera position / target */
    UiSpr glyph[14];         /* 0x4DC */
    s16 unk4d8;
    u16 unk8CC;
    s16 unk8CE;
    s32 model_scale;         /* player obj unk24C (0x199 .. 0x1000) */
    s32 model_x, model_y;    /* player model screen position (20.12, DS screen) */
    s32 cam_h;               /* FX_Div(cos, sin) * 0x60 of the stage perspective angle
                                (data_ov000_0236a444); native: 0x3c000 (no change) */
    DcPlayer *obj;
    int active;
};

static TcWork s_tc;
#define data_022c4d98 (&s_tc)

int StageTitleCard_IsActive(void) { return s_tc.active; }
void StageTitleCard_Destroy(void) { s_tc.active = 0; }

static void tc_voice(s32 id) {
    if (g_ui_hooks.voice)
        g_ui_hooks.voice(g_ui_hooks.ctx, 0, id);
}

/* ---------------------------------------------------------------- Bg */
static void StageTitleCardBg_State_Scroll(TcWork *s, TcBg *o);
static void StageTitleCardBg_State_Done(TcWork *s, TcBg *o) { (void)o; s->flags |= 2; }
static void StageTitleCardBg_State_Exit(TcWork *a, TcBg *o) {
    (void)a;
    o->unkC += 0x28;
    if (o->unkC >= 0x1000) {
        o->unkC = 0x1000;
        o->fn = StageTitleCardBg_State_Done;
    }
    o->unk4 = (s32)(((int64_t)o->unkC * 0x80000 + 0x800) >> 12);
}
static void StageTitleCardBg_State_BeginExit(TcWork *a, TcBg *o) {
    o->unkC = 0;
    o->fn = StageTitleCardBg_State_Exit;
    o->fn(a, o);
}
static void StageTitleCardBg_State_Scroll(TcWork *s, TcBg *o) {
    o->unk8 += 0x1000;
    if (s->flags & 0x10000000)
        o->fn = StageTitleCardBg_State_BeginExit;
}
static void StageTitleCardBg_State_Init(TcWork *s, TcBg *o) {
    s->flags &= ~2u;
    s->flags2 |= 1;
    o->unk4 = 0;
    o->unk8 = 0;
    o->fn = StageTitleCardBg_State_Scroll;
}

/* ------------------------------------------------------------- Strip */
static void StageTitleCardStrip_State_Done(TcWork *s, TcObj *o) { (void)o; s->flags |= 8; }
static void StageTitleCardStrip_State_Exit(TcWork *a, TcObj *o) {
    (void)a;
    o->unk54 += 0x100;
    if (o->unk54 > 0x8000)
        o->unk54 = 0x8000;
    o->unk4C -= o->unk54;
    if (o->unk4C <= -0x100000)
        o->fn = StageTitleCardStrip_State_Done;
}
static void StageTitleCardStrip_State_Stop(TcWork *s, TcObj *o) {
    o->unk54 += 0x100;
    if (o->unk54 > 0x8000)
        o->unk54 = 0x8000;
    o->unk4C -= o->unk54;
    if (-o->unk54 <= o->unk4C) {
        s->flags &= ~0x10u;
        o->fn = StageTitleCardStrip_State_Exit;
    }
}
static void StageTitleCardStrip_State_Scroll(TcWork *s, TcObj *o) {
    o->unk4C -= o->unk54;
    if (s->flags & 0x10000000)
        o->fn = StageTitleCardStrip_State_Stop;
}
static void StageTitleCardStrip_State_Init(TcWork *s, TcObj *o) {
    s->flags &= ~8u;
    s->flags |= 0x10;
    s->flags2 |= 4;
    o->unk4C = 0;
    o->unk50 = 0xb4000;
    o->unk54 = 0x2000;
    o->fn = StageTitleCardStrip_State_Scroll;
}
static void StageTitleCardStrip_Draw(TcWork *w) {
    TcObj *s = &w->fn88;
    if ((w->flags & 0x10) && s->unk4C <= -0x100000)
        s->unk4C += 0x100000;
    s->spr.x = (s16)(s->unk4C >> 12);
    s->spr.y = (s16)(s->unk50 >> 12);
    ui_emit_spr(&s->spr, UI_SP_SCREEN);
    if ((w->flags & 0x10) == 0)
        return;
    s->spr.x += 0x100;
    ui_emit_spr(&s->spr, UI_SP_SCREEN);
}

/* ------------------------------------------------------------ Part20 */
static void StageTitleCardPart20_State_Idle(TcWork *s, TcObj *o) { (void)s; (void)o; }
static void StageTitleCardPart20_State_Init(TcWork *s, TcObj *o) {
    s->flags2 |= 2;
    o->unk4C = 0;
    o->unk50 = 0;
    o->fn = StageTitleCardPart20_State_Idle;
    o->fn(s, o);
}
static void StageTitleCardPart20_Draw(TcWork *s) {
    TcObj *p = &s->fn20;
    p->spr.x = (s16)(p->unk4C >> 12);
    p->spr.y = (s16)(p->unk50 >> 12);
    ui_emit_spr(&p->spr, UI_SP_SCREEN);
}

/* ------------------------------------------------------------ PartE0 */
static void StageTitleCardPartE0_State_Done(TcWork *s, TcObjE0 *o) { (void)o; s->flags |= 0x40; }
static void StageTitleCardPartE0_State_Enter(TcWork *w, TcObjE0 *o) {
    (void)w;
    o->unk9E += 0x28;
    if (o->unk9E >= 0x1000) {
        o->unk9E = 0x1000;
        o->fn = StageTitleCardPartE0_State_Done;
    }
    o->unk94 = 0x1a0000 + FX_Mul(-0xc8000, o->unk9E);
    o->unk98 = -0x5c000 + FX_Mul(0xc8000, o->unk9E);
    o->unk9C = (u16)(FX_Mul(-0x40000000, o->unk9E) >> 12);
}
static void StageTitleCardPartE0_State_Delay(TcWork *a, TcObjE0 *o) {
    (void)a;
    if (o->unk9E++ > 40) {
        o->unk9E = 0;
        o->fn = StageTitleCardPartE0_State_Enter;
    }
}
static void StageTitleCardPartE0_State_Init(TcWork *s, TcObjE0 *o) {
    s->flags &= ~0x40u;
    s->flags2 |= 8;
    o->unk94 = 0x1a0000;
    o->unk98 = -0x5c000;
    o->unk9E = 0;
    o->fn = StageTitleCardPartE0_State_Delay;
}
static void StageTitleCardPartE0_Draw(TcWork *s) {
    TcObjE0 *p = &s->fne0;
    p->spr.x = (s16)(p->unk94 >> 12);
    p->spr.y = (s16)(p->unk98 >> 12);
    /* AnimatorSprite_DrawRotScale(.., 0x1000, 0x1000, unk9C): the art is a disc,
     * the rotation is not reproduced */
    ui_emit_spr(&p->spr, UI_SP_SCREEN);
    p->spr2.x = 0;
    p->spr2.y = 0;
    UiSpr_ProcessAnimation(&p->spr2);
    ui_emit_spr(&p->spr2, UI_SP_SCREEN);
}

/* ----------------------------------------------------------- Part180 */
static void StageTitleCardPart180_State_Hold(TcWork *s, TcObj *o);
static void StageTitleCardPart180_State_Done(TcWork *s, TcObj *o) { (void)o; s->flags |= 0x100; }
static void StageTitleCardPart180_State_Exit(TcWork *w, TcObj *o) {
    (void)w;
    o->t += 0x28;
    if (o->t >= 0x1000) {
        o->t = 0x1000;
        o->fn = StageTitleCardPart180_State_Done;
    }
    o->unk4C = o->unk54 + FX_Mul(0x1c2000 - o->unk54, o->t);
    o->unk50 = o->unk58 + FX_Mul(o->unk68 + g_ui_data.tc_p180y[W.zone % 9][1] - o->unk58, o->t);
}
static void StageTitleCardPart180_State_BeginExit(TcWork *a, TcObj *o) {
    o->unk54 = o->unk4C;
    o->unk58 = o->unk50;
    o->t = 0;
    o->fn = StageTitleCardPart180_State_Exit;
    o->fn(a, o);
}
static void StageTitleCardPart180_State_Hold(TcWork *s, TcObj *o) {
    o->unk4C += 0x400;
    if (s->flags & 0x10000000)
        o->fn = StageTitleCardPart180_State_BeginExit;
}
static void StageTitleCardPart180_State_Enter(TcWork *w, TcObj *o) {
    u8 z = W.zone % 9;
    (void)w;
    if (o->delay == 0) {
        o->t += 0x51;
        if (o->t >= 0x1000) {
            o->t = 0x1000;
            o->fn = StageTitleCardPart180_State_Hold;
        }
    } else {
        o->delay--;
    }
    /* data_02076bb4[zone].a, data_02076bb8[zone].a */
    o->unk4C = UiLerpN(-0x50000, g_ui_data.tc_p180y[z][0], o->t, 3);
    o->unk50 = o->unk68 + UiLerpN(g_ui_data.tc_p180y[z][1], g_ui_data.tc_p180y[z][1], o->t, 3);
    o->unk64 = (s16)UiLerpN(0x4000, 0x1000, o->t, 1);
    o->unk66 = (s16)UiLerp2N(0x1000, 0x1000, o->t, 0);
}
static void StageTitleCardPart180_State_Init(TcWork *s, TcObj *o) {
    s->flags &= ~0x100u;
    s->flags2 |= 0x10;
    o->unk5C = g_ui_data.tc_p180x[W.zone % 9]; /* data_02076b90[zone] */
    o->unk60 = 0;
    o->t = 0;
    o->fn = StageTitleCardPart180_State_Enter;
    o->fn(s, o);
}
static void StageTitleCardPart180_Draw(TcWork *w) {
    u32 i;
    TcObj *s = &w->fn180;
    for (i = 0; i < 2; i++) {
        s->spr.x = (s16)((s->unk4C + s->unk5C - FX_Mul(s->unk5C, s->unk64)) >> 12);
        s->spr.y = (s16)((s->unk50 + s->unk60 - FX_Mul(s->unk60, s->unk66)) >> 12);
        ui_emit(&s->spr, UI_SP_SCREEN, s->unk64, s->unk66, 0, 0xffffffff, 0);
        s = &w->fn1f0;
    }
}

/* ---------------------------------------------------- Part260 / Part324 */
static void Pair_Draw(TcObjPair *s) {
    u32 i;
    for (i = 0; i < 2; i++) {
        s->spr[i].x = (s16)((s->unkB4 + (s->unkA4 + FX_Mul(s->pt[i].x, s->unkBC)) - FX_Mul(s->unkB4, s->unkBC)) >> 12);
        s->spr[i].y = (s16)((s->unkB8 + (s->unkA8 + FX_Mul(s->pt[i].y, s->unkBE)) - FX_Mul(s->unkB8, s->unkBE)) >> 12);
        ui_emit(&s->spr[i], UI_SP_SCREEN, s->unkBC, s->unkBE, 0, 0xffffffff, 0);
    }
}

static void StageTitleCardPart260_State_Hold(TcWork *s, TcObjPair *o);
static void StageTitleCardPart260_State_Done(TcWork *s, TcObjPair *o) { (void)o; s->flags |= 0x400; }
static void StageTitleCardPart260_State_Exit(TcWork *w, TcObjPair *o) {
    (void)w;
    o->unkC0 += 0x28;
    if (o->unkC0 >= 0x1000) {
        o->unkC0 = 0x1000;
        o->fn = StageTitleCardPart260_State_Done;
    }
    o->unkA4 = o->unkAC + FX_Mul(-0x80000 - o->unkAC, o->unkC0);
    o->unkA8 = o->unkB0 + FX_Mul(0x91000 - o->unkB0, o->unkC0);
}
static void StageTitleCardPart260_State_BeginExit(TcWork *a, TcObjPair *o) {
    o->unkAC = o->unkA4;
    o->unkB0 = o->unkA8;
    o->unkC0 = 0;
    o->fn = StageTitleCardPart260_State_Exit;
    o->fn(a, o);
}
static void StageTitleCardPart260_State_Hold(TcWork *s, TcObjPair *o) {
    o->unkA4 += -0x333;
    if (s->flags & 0x10000000)
        o->fn = StageTitleCardPart260_State_BeginExit;
}
static void StageTitleCardPart260_State_Enter(TcWork *w, TcObjPair *o) {
    (void)w;
    o->unkC0 += 0x51;
    if (o->unkC0 >= 0x1000) {
        o->unkC0 = 0x1000;
        o->fn = StageTitleCardPart260_State_Hold;
    }
    if (W.zone < 7)
        o->unkA4 = UiLerpN(0x12c000, 0x78000, o->unkC0, 3);
    else
        o->unkA4 = UiLerpN(0x12c000, 0x46000, o->unkC0, 3);
    o->unkA8 = UiLerpN(0x91000, 0x91000, o->unkC0, 3);
    o->unkBC = (s16)UiLerpN(0x2000, 0x1000, o->unkC0, 1);
    o->unkBE = (s16)UiLerp2N(0x1000, 0x1000, o->unkC0, 0);
}
static void StageTitleCardPart260_State_Delay(TcWork *a, TcObjPair *o) {
    (void)a;
    if (o->unkC0++ > 0) {
        o->unkC0 = 0;
        o->fn = StageTitleCardPart260_State_Enter;
    }
}
static void StageTitleCardPart260_State_Init(TcWork *w, TcObjPair *o) {
    w->flags &= ~0x400u;
    w->flags2 |= 0x20;
    if (W.zone < 7) {
        o->pt[0].x = -0x1c000;
        o->pt[0].y = 0;
        o->pt[1].x = 0xc000;
        o->pt[1].y = 0;
    } else {
        o->pt[0].x = 0x44000;
        o->pt[0].y = 0;
        o->pt[1].x = -0x8000;
        o->pt[1].y = 0;
    }
    o->unkB4 = o->pt[0].x * 2;
    o->unkB8 = o->pt[0].y;
    o->unkC0 = 0;
    o->fn = StageTitleCardPart260_State_Delay;
    o->fn(w, o);
}

static void StageTitleCardPart324_State_Hold(TcWork *s, TcObjPair *o);
static void StageTitleCardPart324_State_Done(TcWork *s, TcObjPair *o) { (void)o; s->flags |= 0x1000; }
static void StageTitleCardPart324_State_Exit(TcWork *w, TcObjPair *o) {
    (void)w;
    o->unkC0 += 0x28;
    if (o->unkC0 >= 0x1000) {
        o->unkC0 = 0x1000;
        o->fn = StageTitleCardPart324_State_Done;
    }
    o->unkA4 = o->unkAC + FX_Mul(-0x44000 - o->unkAC, o->unkC0);
    o->unkA8 = o->unkB0 + FX_Mul(0x91000 - o->unkB0, o->unkC0);
}
static void StageTitleCardPart324_State_BeginExit(TcWork *a, TcObjPair *o) {
    o->unkAC = o->unkA4;
    o->unkB0 = o->unkA8;
    o->unkC0 = 0;
    o->fn = StageTitleCardPart324_State_Exit;
    o->fn(a, o);
}
static void StageTitleCardPart324_State_Hold(TcWork *s, TcObjPair *o) {
    o->unkA4 += -0x333;
    if (s->flags & 0x10000000)
        o->fn = StageTitleCardPart324_State_BeginExit;
}
static void StageTitleCardPart324_State_Enter(TcWork *w, TcObjPair *o) {
    (void)w;
    o->unkC0 += 0x51;
    if (o->unkC0 >= 0x1000) {
        o->unkC0 = 0x1000;
        o->fn = StageTitleCardPart324_State_Hold;
    }
    o->unkA4 = UiLerpN(0x168000, 0xb4000, o->unkC0, 3);
    o->unkA8 = UiLerpN(0x91000, 0x91000, o->unkC0, 3);
    o->unkBC = (s16)UiLerpN(0x2000, 0x1000, o->unkC0, 1);
    o->unkBE = (s16)UiLerp2N(0x1000, 0x1000, o->unkC0, 0);
}
static void StageTitleCardPart324_State_Delay(TcWork *a, TcObjPair *o) {
    (void)a;
    if (o->unkC0++ > 15) {
        o->unkC0 = 0;
        o->fn = StageTitleCardPart324_State_Enter;
    }
}
static void StageTitleCardPart324_State_Init(TcWork *w, TcObjPair *o) {
    w->flags &= ~0x1000u;
    w->flags2 |= 0x40;
    if (W.zone < 7 && W.act < 2) {
        o->pt[0].x = -0x1c000;
        o->pt[0].y = 0;
        o->pt[1].x = 0;
        o->pt[1].y = 0;
    } else {
        o->pt[0].x = -0x1c000;
        o->pt[0].y = 0;
        o->pt[1].x = -0x8000;
        o->pt[1].y = 0;
    }
    o->unkB4 = o->pt[0].x * 2;
    o->unkB8 = o->pt[0].y;
    o->unkC0 = 0;
    o->fn = StageTitleCardPart324_State_Delay;
    o->fn(w, o);
}

/* ------------------------------------------------------------- texts */
static void StageTitleCardText3E8_State_Done(TcWork *s, TcText *o) { (void)o; s->flags |= 0x4000; }
static void StageTitleCardText3E8_State_Enter(TcWork *w, TcText *o) {
    (void)w;
    o->t += 0x51;
    if (o->t >= 0x1000) {
        o->t = 0x1000;
        o->fn = StageTitleCardText3E8_State_Done;
    }
    o->unkC = UiLerpN(0x118000, 0x50000, o->t, 3);
    o->unk10 = UiLerpN(0x28000, 0x28000, o->t, 3);
    o->unk14 = (s16)UiLerpN(0, 0x1000, o->t, 3);
}
static void StageTitleCardText3E8_State_Init(TcWork *s, TcText *o) {
    s->flags &= ~0x4000u;
    s->flags2 |= 0x80;
    o->str = g_ui_data.tc_text1; /* data_02076b68 */
    o->count = 12;
    o->t = 0;
    o->fn = StageTitleCardText3E8_State_Enter;
    o->fn(s, o);
}
static void StageTitleCardText404_State_Done(TcWork *s, TcText *o) { (void)o; s->flags |= 0x10000; }
static void StageTitleCardText404_State_Enter(TcWork *w, TcText *o) {
    (void)w;
    o->t += 0x51;
    if (o->t >= 0x1000) {
        o->t = 0x1000;
        o->fn = StageTitleCardText404_State_Done;
    }
    o->unkC = UiLerpN(0x118000, 0x64000, o->t, 3);
    o->unk10 = UiLerpN(0x28000, 0x28000, o->t, 3);
    o->unk14 = (s16)UiLerpN(0, 0x1000, o->t, 3);
}
static void StageTitleCardText404_State_Init(TcWork *s, TcText *o) {
    s->flags &= ~0x10000u;
    s->flags2 |= 0x100;
    o->str = g_ui_data.tc_text2; /* data_02076b54 */
    o->count = 6;
    o->t = 0;
    o->fn = StageTitleCardText404_State_Enter;
    o->fn(s, o);
}
static void StageTitleCard_DrawGlyphText(TcWork *w, TcText *t) {
    s32 x = 0;
    u32 i;
    if (W.zone >= 7)
        return;
    for (i = 0; i < t->count; i++) {
        u8 c = t->str[i];
        if (c < 14) {
            UiSpr *s = &w->glyph[c];
            s->x = (s16)((t->unkC + x) >> 12);
            s->y = (s16)(t->unk10 >> 12);
            ui_emit_spr(s, UI_SP_SCREEN);
        }
        x += t->unk14 * g_ui_data.tc_gw[t->str[i] & 15];
    }
}

/* ----------------------------------------------------------- Part420 */
static void StageTitleCardPart420_State_Done(TcWork *s, TcObj *o) { (void)o; s->flags |= 0x40000; }
static void StageTitleCardPart420_State_Init(TcWork *s, TcObj *o) {
    s->flags &= ~0x40000u;
    s->flags2 |= 0x200;
    o->unk4C = 0x80000;
    o->unk50 = 0x46000;
    o->fn = StageTitleCardPart420_State_Done;
}
static void StageTitleCardPart420_Draw(TcWork *s) {
    TcObj *p = &s->fn420;
    p->spr.x = (s16)(p->unk4C >> 12);
    p->spr.y = (s16)(p->unk50 >> 12);
    ui_emit_spr(&p->spr, UI_SP_SCREEN);
}

/* ------------------------------------------- player close-up (values only) */
static void StageTitleCardPlayer_State_Wait(TcWork *s, TcPlayer *o);
static void StageTitleCardPlayer_State_Finish(TcWork *s, TcPlayer *o) {
    (void)o;
    s->model_scale = 0x1000; /* obj->unk70 = saved draw function: normal model drawing */
    s->flags |= 0x400000;
}
static void StageTitleCardPlayer_State_Zoom(TcWork *w, TcPlayer *t) {
    s32 x, y, v, s, a;
    int n;
    t->t += 0x28;
    if (t->t >= 0x1000) {
        t->t = 0x1000;
        t->fn = StageTitleCardPlayer_State_Finish;
    }
    /* Camera_WorldToScreen(player): the model moves from the screen centre
     * (0x80000, -0x60000) to the player's screen position */
    if (w->obj) {
        s16 sx, sy;
        ui_world_to_screen(w->obj->x >> 8, w->obj->y >> 8, &sx, &sy);
        x = (w->obj->shake_x + sx) << 12;
        y = -(sy - w->obj->shake_y) << 12;
    } else {
        x = 0x80000;
        y = -0x60000;
    }
    n = 1;
    do {
        x = FX_Mul(x - 0x80000, t->t) + 0x80000;
    } while (n-- != 0);
    w->model_x = x;
    v = -0x60000;
    n = 1;
    do {
        v = v + FX_Mul(y - v, t->t);
    } while (n-- != 0);
    w->model_y = v;
    s = (s16)FX_Mul(t->t, 0x1333);
    if (s > 0x1000)
        s = 0x1000;
    a = FX_Mul(-0x5000, s) - 0x5b000;
    w->v4b4[0] = 0x80000;
    w->v4b4[1] = a;
    w->v4b4[2] = FX_Mul(w->cam_h - 0x3c000, s) + 0x3c000;
    w->v4b4[3] = 0x80000;
    w->v4b4[4] = a;
    w->v4b4[5] = 0;
    w->model_scale = 0x199 + ((t->t * (s32)0xe67) >> 12);
}
static void StageTitleCardPlayer_State_BeginZoom(TcWork *s, TcPlayer *o) {
    s->flags2 |= 0x400;
    o->t = 0;
    o->fn = StageTitleCardPlayer_State_Zoom;
    o->fn(s, o);
}
static void StageTitleCardPlayer_State_ToZoom(TcWork *a, TcPlayer *o) {
    o->fn = StageTitleCardPlayer_State_BeginZoom;
    o->fn(a, o);
}
static void StageTitleCardPlayer_State_CameraMove(TcWork *w, TcPlayer *o) {
    o->t += 0x28;
    if (o->t >= 0x1000) {
        o->t = 0x1000;
        o->fn = StageTitleCardPlayer_State_ToZoom;
    }
    if (W.zone != 8 || W.act != 2) {
        w->v4b4[0] = UiLerpN(0x9e000, 0x80000, o->t, 0);
        w->v4b4[1] = UiLerpN(-0x5b000, -0x5b000, o->t, 0);
        w->v4b4[2] = UiLerpN(0x1e000, 0x3c000, o->t, 0);
        w->v4b4[3] = UiLerpN(0x80000, 0x80000, o->t, 0);
        w->v4b4[4] = UiLerpN(-0x5b000, -0x5b000, o->t, 0);
        w->v4b4[5] = UiLerpN(0, 0, o->t, 0);
    } else {
        w->v4b4[0] = UiLerpN(0x80000, 0x80000, o->t, 0);
        w->v4b4[1] = UiLerpN(-0x5b000, -0x5b000, o->t, 0);
        w->v4b4[2] = UiLerpN(0x1e000, 0x3c000, o->t, 0);
        w->v4b4[3] = UiLerpN(0x80000, 0x80000, o->t, 0);
        w->v4b4[4] = UiLerpN(-0x5b000, -0x5b000, o->t, 0);
        w->v4b4[5] = UiLerpN(0, 0, o->t, 0);
    }
}
static void StageTitleCardPlayer_State_BeginCameraMove(TcWork *a, TcPlayer *s) {
    (void)a;
    s->t = 0;
    s->fn = StageTitleCardPlayer_State_CameraMove;
}
static void StageTitleCardPlayer_State_Wait(TcWork *s, TcPlayer *o) {
    if (s->flags & 0x100000)
        o->fn = StageTitleCardPlayer_State_BeginCameraMove;
}
static void StageTitleCardPlayer_State_SetCamera(TcWork *w, TcPlayer *o) {
    if (W.zone != 8 || W.act != 2) {
        w->v4b4[0] = 0x9e000;
        w->v4b4[1] = -0x5b000;
        w->v4b4[2] = 0x1e000;
        w->v4b4[3] = 0x80000;
        w->v4b4[4] = -0x5b000;
        w->v4b4[5] = 0;
    } else {
        w->v4b4[0] = 0x80000;
        w->v4b4[1] = -0x5b000;
        w->v4b4[2] = 0x1e000;
        w->v4b4[3] = 0x80000;
        w->v4b4[4] = -0x5b000;
        w->v4b4[5] = 0;
    }
    o->fn = StageTitleCardPlayer_State_Wait;
}
static void StageTitleCardPlayer_State_Init(TcWork *w, TcPlayer *o) {
    w->flags &= ~0x700000u;
    w->flags2 &= ~0x400u;
    if (W.zone != 8 || W.act != 2)
        w->model_scale = 0x199; /* sub->unk24C[i] */
    o->fn = StageTitleCardPlayer_State_SetCamera;
    o->fn(w, o);
}

/* ------------------------------------------------------- main states */
static void StageTitleCard_State_StartDelay(TcWork *s);
static void StageTitleCard_State_FadeIn(TcWork *w);
static void StageTitleCard_State_ShowParts(TcWork *s);
static void StageTitleCard_State_Hold(TcWork *s);
static void StageTitleCard_State_ShowText(TcWork *s);
static void StageTitleCard_State_WaitText(TcWork *s);
static void StageTitleCard_State_HoldText(TcWork *s);
static void StageTitleCard_State_ShowText2(TcWork *s);
static void StageTitleCard_State_WaitText2(TcWork *s);
static void StageTitleCard_State_Exit(TcWork *w);
static void StageTitleCard_State_StartStage(TcWork *w);
static void StageTitleCard_State_WaitStartStage(TcWork *s);
static void StageTitleCard_State_RestoreColors(TcWork *w);
static void StageTitleCard_State_FinalDelay(TcWork *s);

static void StageTitleCard_State_Init(TcWork *s) {
    s->unk8CC = 0;
    s->fn474 = StageTitleCard_State_StartDelay;
}
static void StageTitleCard_State_BeginFadeIn(TcWork *s) {
    s->unk8CE = 0;
    s->fn474 = StageTitleCard_State_FadeIn;
}
static void StageTitleCard_State_StartDelay(TcWork *s) {
    if (s->unk8CC++ > 5)
        s->fn474 = StageTitleCard_State_BeginFadeIn;
}
static void StageTitleCard_State_ShowDelay(TcWork *s) {
    if (s->unk8CC++ != 0)
        s->fn474 = StageTitleCard_State_ShowParts;
}
static void StageTitleCard_State_BeginShow(TcWork *s) {
    s->unk8CC = 0;
    s->fn474 = StageTitleCard_State_ShowDelay;
}
static void StageTitleCard_State_FadeIn(TcWork *w) {
    w->unk8CE += 0xa3;
    if (w->unk8CE >= 0x1000) {
        w->unk8CE = 0x1000;
        w->fn474 = StageTitleCard_State_BeginShow;
    }
    g_ui_bright_sub = (s16)((FX_Mul(-0x10000, w->unk8CE) + 0x10000) >> 12);
    g_ui_bright_main = g_ui_bright_sub;
}
static void StageTitleCard_State_BeginHold(TcWork *s) {
    s->unk8CC = 0;
    s->fn474 = StageTitleCard_State_Hold;
}
static void StageTitleCard_State_ShowParts(TcWork *s) {
    s->flags |= 0xa80;
    s->flags2 |= 0x70;
    s->fn474 = StageTitleCard_State_BeginHold;
}
static void StageTitleCard_State_Hold(TcWork *s) {
    if (s->unk8CC++ == 30)
        s->fn474 = StageTitleCard_State_ShowText;
}
static void StageTitleCard_State_ShowText(TcWork *s) {
    s->flags |= 0x102000;
    s->flags2 |= 0x80;
    s->fn474 = StageTitleCard_State_WaitText;
}
static void StageTitleCard_State_BeginHoldText(TcWork *s) {
    s->unk8CC = 0;
    s->fn474 = StageTitleCard_State_HoldText;
}
static void StageTitleCard_State_WaitText(TcWork *s) {
    if (s->flags & 0x4000)
        s->fn474 = StageTitleCard_State_BeginHoldText;
}
static void StageTitleCard_State_HoldText(TcWork *s) {
    if (s->unk8CC++ == 45)
        s->fn474 = StageTitleCard_State_ShowText2;
}
static void StageTitleCard_State_ShowText2(TcWork *s) {
    s->flags &= ~0x2000u;
    s->flags2 &= ~0x80u;
    s->flags |= 0x208000;
    s->flags2 |= 0x100;
    s->fn474 = StageTitleCard_State_WaitText2;
}
static void StageTitleCard_State_BeginExit(TcWork *s) {
    s->flags |= 0x10000000;
    s->unk8CE = 0;
    s->fn474 = StageTitleCard_State_Exit;
}
static void StageTitleCard_State_WaitText2(TcWork *s) {
    if (s->flags & 0x10000)
        s->fn474 = StageTitleCard_State_BeginExit;
}
static void StageTitleCard_State_ToStartStage(TcWork *s) { s->fn474 = StageTitleCard_State_StartStage; }
static void StageTitleCard_State_Exit(TcWork *w) {
    w->unk8CE += 0xa3;
    if (w->unk8CE >= 0x1000) {
        w->unk8CE = 0x1000;
        if (w->flags & 0x400000)
            w->fn474 = StageTitleCard_State_ToStartStage;
    }
    g_ui_bright_sub = (s16)(FX_Mul(0x10000, w->unk8CE) >> 12);
    if (W.zone == 8 && W.act == 2)
        return;
    if (UI_NORMAL_STAGE())
        return;
    g_ui_bright_main = (s16)(FX_Mul(0x10000, w->unk8CE) >> 12);
}
static void StageTitleCard_State_StartStage(TcWork *w) {
    w->flags &= 0xfff7555bu;
    w->flags2 &= ~0x1ffu;
    w->flags |= 0x20000;
    w->flags2 |= 0x200;
    w->flags |= 0x20000000;
    w->model_scale = 0x1000; /* obj->unk70 restored: normal player drawing */
    if (W.zone != 8 || W.act != 2) {
        if (w->obj) {
            if ((w->flags & 0x800000) == 0)
                w->obj->x = w->x480;
            else
                w->obj->y = w->y484;
            w->obj->pf &= ~0x600000u; /* NOINPUT | NOTENSION */
            w->obj->px = w->obj->x;
            w->obj->py = w->obj->y;
        }
        W.flags |= 1; /* gGameState.flags: the stage timer runs */
    }
    if (W.zone == 8 && W.act == 2) {
        g_ui_bright_main = 0;
        g_ui_bright_sub = 0x10;
    } else if (!UI_NORMAL_STAGE()) {
        g_ui_bright_main = 0x10;
        if (W.zone != 8 || W.act != 2) {
            NaviMsg_Create();      /* NaviMsg_Create(1, gGameState.unk0) */
            NaviMsg_SetVisible(1);
            NaviB_Create();
        }
    } else {
        g_ui_bright_main = 0;
        g_ui_bright_sub = 0x10;
        tc_voice(G.character == 0 ? 0x22 : 0x2b); /* "Go!" voice */
    }
    w->unk8CE = 0;
    w->fn474 = StageTitleCard_State_WaitStartStage;
}
static void StageTitleCard_State_BeginRestoreColors(TcWork *s) {
    if (W.zone != 8 || W.act != 2)
        Hud_SetOamPriority(2); /* Hud_SetOamPriority2 */
    s->unk8CE = 0;
    s->fn474 = StageTitleCard_State_RestoreColors;
}
static void StageTitleCard_State_WaitStartStage(TcWork *s) {
    if (s->flags & 0x40000)
        s->fn474 = StageTitleCard_State_BeginRestoreColors;
}
static void StageTitleCard_State_RestoreDisplay(TcWork *w) {
    if (W.zone != 8 || W.act != 2)
        Hud_SetOamPriority(0); /* Hud_SetOamPriority0 + Stage_InitBlend */
    w->unk8CC = 0;
    w->fn474 = StageTitleCard_State_FinalDelay;
}
static void StageTitleCard_State_RestoreColors(TcWork *w) {
    w->unk8CE += 0xa3;
    if (w->unk8CE >= 0x1000) {
        w->unk8CE = 0x1000;
        w->fn474 = StageTitleCard_State_RestoreDisplay;
    }
    if ((W.zone == 8 && W.act == 2) || UI_NORMAL_STAGE()) {
        g_ui_bright_sub = (s16)((FX_Mul(w->unk8CE, -0x10000) + 0x10000) >> 12);
        w->unk4d8 = (s16)((FX_Mul(w->unk8CE, -0x1f000) + 0x1f000) >> 12);
    } else {
        g_ui_bright_sub = (s16)((FX_Mul(w->unk8CE, -0x10000) + 0x10000) >> 12);
        g_ui_bright_main = g_ui_bright_sub;
    }
    /* NNS_G3dGlbLightColor / G3X_SetEdgeColorTable lerps: stage lighting (model module) */
}
static void StageTitleCard_State_Finish(TcWork *s) {
    g_ui_bright_sub = 0;
    s->flags |= 0x80000000u;
}
static void StageTitleCard_State_FinalDelay(TcWork *s) {
    if (s->unk8CC++ == 30)
        s->fn474 = StageTitleCard_State_Finish;
}

/* --------------------------------------------------------- player hold */
static void StageTitleCard_HoldPlayerNone(TcWork *w, DcPlayer *p) {
    (void)w;
    (void)p;
}
/* the player runs on the spot: x wraps inside a 0x10000 (256 px) window */
static void StageTitleCard_HoldPlayerX(TcWork *work, DcPlayer *player) {
    u32 dx, x;
    player->held |= 0x10; /* unk420: hold RIGHT */
    player->hs_timer = 0; /* unk3AC */
    if (player->top > player->gnd_vel)
        player->gnd_vel = player->top; /* unk358 -> unk34 */
    x = (u32)player->x;
    dx = x - (u32)work->x480;
    player->px = (s32)x;
    if (!(work->flags & 0x20000000))
        dx &= 0xffff;
    player->x = work->x480 + (s32)dx;
    /* gCamera snaps to the player (Camera module / main.c) */
}
static void StageTitleCard_HoldPlayerY(TcWork *w, DcPlayer *p) {
    s32 d;
    p->held = 0;
    p->hs_timer = 0;
    if (p->top > p->gnd_vel)
        p->gnd_vel = p->top;
    d = p->y - w->y484;
    p->py = p->y;
    if (!(w->flags & 0x20000000))
        d &= 0xffff;
    p->y = w->y484 + d;
}
void StageTitleCard_HoldPlayer(DcPlayer *p) {
    if (s_tc.active && s_tc.fn478)
        s_tc.fn478(&s_tc, p);
}

/* ------------------------------------------------------------- create */
static void StageTitleCard_InitSprites(TcWork *w) {
    u32 i, v;
    u16 a2;
    UiSpr *spr, *spr2;
    UiSpr_Init(&w->fn88.spr, UI_RES_STDM, g_ui_data.tc_char_b[w->idx & 1], 0);   /* 0x8C name strip */
    UiSpr_Init(&w->fn20.spr, UI_RES_STDM, g_ui_data.tc_char_a[w->idx & 1], 0);   /* 0x24 */
    UiSpr_Init(&w->fne0.spr, UI_RES_STDM, (u16)(W.zone + 0xf), 0);               /* 0xE4 */
    UiSpr_Init(&w->fne0.spr2, UI_RES_STDM, 0x29, UI_SPR_LOOP);                   /* 0x12C */
    UiSpr_Init(&w->fn180.spr, UI_RES_STDM_LANG, (u16)(W.zone * 2), 0);           /* 0x184 */
    UiSpr_Init(&w->fn1f0.spr, UI_RES_STDM_LANG, (u16)(W.zone * 2 + 1), 0);       /* 0x1F4 */
    v = StoryProgress_GetZoneIndex(W.zone);
    spr = v < 7 ? &w->fn260.spr[0] : &w->fn260.spr[1];
    UiSpr_Init(spr, UI_RES_STDM, (u16)(v < 7 ? 2 : 0x2a), 0);                    /* "ZONE" */
    spr2 = v < 7 ? &w->fn260.spr[1] : &w->fn260.spr[0];
    if (v == 7)
        a2 = 0xd;
    else if (v == 8)
        a2 = 0xe;
    else
        a2 = (u16)(v + 4);
    UiSpr_Init(spr2, UI_RES_STDM, a2, 0);                                        /* zone number */
    UiSpr_Init(&w->fn324.spr[0], UI_RES_STDM, UI_NORMAL_STAGE() ? 3 : 0xb, 0);   /* "ACT" / "VS" */
    UiSpr_Init(&w->fn324.spr[1], UI_RES_STDM, (u16)(UI_NORMAL_STAGE() ? W.act + 4 : 0xc), 0);
    for (i = 0; i < 14; i++)
        UiSpr_Init(&w->glyph[i], UI_RES_STDM, (u16)(i + 0x19), 0);               /* 0x4DC */
    UiSpr_Init(&w->fn420.spr, UI_RES_STDM, 0x18, 0);                             /* "GO!" */
}

void StageTitleCard_Create(DcPlayer *p) {
    TcWork *w = &s_tc;
    memset(w, 0, sizeof(*w));
    w->active = 1;
    w->fn474 = StageTitleCard_State_Init;
    w->fn478 = StageTitleCard_HoldPlayerNone;
    w->fn78.fn = StageTitleCardBg_State_Init;
    w->fn88.fn = StageTitleCardStrip_State_Init;
    w->fne0.fn = StageTitleCardPartE0_State_Init;
    w->fn180.fn = StageTitleCardPart180_State_Init;
    w->fn1f0.fn = StageTitleCardPart180_State_Init;
    w->fn260.fn = StageTitleCardPart260_State_Init;
    w->fn324.fn = StageTitleCardPart324_State_Init;
    w->fn3e8.fn = StageTitleCardText3E8_State_Init;
    w->fn404.fn = StageTitleCardText404_State_Init;
    w->fn420.fn = StageTitleCardPart420_State_Init;
    w->fn0.fn = StageTitleCardPlayer_State_Init;
    w->fn20.fn = StageTitleCardPart20_State_Init;
    w->obj = p;
    w->x480 = p ? p->x : 0;
    w->y484 = p ? p->y : 0;
    w->flags |= 0x80025;
    w->unk4d8 = 0x1f;
    w->fn180.unk68 = 0;        /* unk1E8: first zone-name word */
    w->fn1f0.unk68 = 0x10000;  /* unk258: second word 16 px lower */
    w->fn1f0.delay = 0xf;      /* unk25C: ... and 15 frames later */
    w->idx = G.character == 1;
    w->cam_h = 0x3c000;
    if (p) {
        p->pf |= 0x200000; /* no input */
        p->pf |= 0x400000; /* no tension */
    }
    if (UI_NORMAL_STAGE()) {
        u8 zone = W.zone;
        if (zone == 0 || zone == 1 || (zone == 3 && W.act == 0) || (zone == 4 && W.act == 0) ||
            (zone == 5 && W.act == 1) || zone == 6) {
            if ((W.flags & 2) == 0)
                w->fn478 = StageTitleCard_HoldPlayerX;
        } else if ((W.flags & 2) == 0 || g_obj_stage.check_idx == 0) {
            w->flags |= 0x800000;
            w->fn478 = StageTitleCard_HoldPlayerY;
        }
    }
    W.flags &= ~1u; /* the timer starts at StartStage */
    StageTitleCard_InitSprites(w);
    ui_tcbg_load();            /* res[2] / res[3]: StageTitleCard_InitBg(Boss) */
    g_ui_bright_main = g_ui_bright_sub = 0x10; /* FadeIn starts from white */
    Hud_SetOamPriority(3);     /* Hud_SetOamPriority3: behind the title card BG */
}

void StageTitleCard_Main(void) {
    TcWork *t0 = &s_tc;
    if (!t0->active)
        return;
    /* skip with A or B (not in VS) */
    if ((t0->flags & 0x20000000) == 0 && (t0->flags & 0x40000000) == 0 && W.mode != 1 &&
        (g_ui_trig & 3)) {
        t0->fn474 = StageTitleCard_State_ToStartStage;
        t0->flags |= 0x40000000;
    }
    t0->fn474(t0);
    if (t0->flags & 0x80000000u) {
        StageTitleCard_Destroy(); /* Task_Destroy */
        return;
    }
    if (t0->flags & 4)
        t0->fn88.fn(t0, &t0->fn88);
    if (t0->flags & 0x20)
        t0->fne0.fn(t0, &t0->fne0);
    if (t0->flags & 0x80) {
        t0->fn180.fn(t0, &t0->fn180);
        t0->fn1f0.fn(t0, &t0->fn1f0);
    }
    if (t0->flags & 0x200)
        t0->fn260.fn(t0, &t0->fn260);
    if (t0->flags & 0x800)
        t0->fn324.fn(t0, &t0->fn324);
    if (t0->flags & 0x2000)
        t0->fn3e8.fn(t0, &t0->fn3e8);
    if (t0->flags & 0x8000)
        t0->fn404.fn(t0, &t0->fn404);
    if (t0->flags & 0x20000)
        t0->fn420.fn(t0, &t0->fn420);
    if (t0->flags & 0x80000)
        t0->fn0.fn(t0, &t0->fn0);
    t0->fn20.fn(t0, &t0->fn20);
    if (t0->flags & 1)
        t0->fn78.fn(t0, &t0->fn78);
    if (t0->flags2 & 4)
        StageTitleCardStrip_Draw(t0);
    if (t0->flags2 & 8)
        StageTitleCardPartE0_Draw(t0);
    if (t0->flags2 & 0x10)
        StageTitleCardPart180_Draw(t0);
    if (t0->flags2 & 0x20)
        Pair_Draw(&t0->fn260);                 /* StageTitleCardPart260_Draw */
    if ((t0->flags2 & 0x40) && W.zone < 7)
        Pair_Draw(&t0->fn324);                 /* StageTitleCardPart324_Draw */
    if (t0->flags2 & 0x80)
        StageTitleCard_DrawGlyphText(t0, &t0->fn3e8);
    if (t0->flags2 & 0x100)
        StageTitleCard_DrawGlyphText(t0, &t0->fn404);
    if (t0->flags2 & 0x200)
        StageTitleCardPart420_Draw(t0);
    if (t0->flags2 & 2)
        StageTitleCardPart20_Draw(t0);
    if (t0->flags2 & 1) {      /* StageTitleCardBg_Draw: zigzag scroll, zone art (ui_bg.c) */
        int layer = g_ui_layer;
        g_ui_layer = UI_LAYER_TITLE_BG;
        ui_tcbg_draw(t0->fn78.unk4, t0->fn78.unk8);
        g_ui_layer = layer;
    }
}

int ui_titlecard_player_view(s32 *scale, s32 *x, s32 *y) {
    if (!s_tc.active || !(s_tc.flags & 0x80000))
        return 0;
    if (scale)
        *scale = s_tc.model_scale;
    if (x)
        *x = s_tc.model_x;
    if (y)
        *y = s_tc.model_y;
    return 1;
}

/*
 * Act results ("SONIC THE HEDGEHOG / ACT 1 CLEAR! / TIME BONUS .. TOTAL / rank"),
 * translated from src/game/Cutscenes/ClearDemo:
 *   ClearDemo_Create, _Main, _State_FadeIn/ShowBg/ShowName/ShowModel/WaitParts/
 *   Hold/FadeOut/Finish, _SkipParts, _SkipToRank, _Finish, _GoToNextScene,
 *   ClearDemo_InitScoreTally, _GetTimeBonus, _GetScoreBonus, _GetRank,
 *   ClearDemoScore_* (SlideIn .. Done, Draw, SkipRows, SkipToRank),
 *   ClearDemoName_*, ClearDemoActText_*, ClearDemoBars_*, ClearDemoBg_* (window and
 *   scroll; the BG layers cldm_far / cldm_bg are drawn by ui_bg.c) and the state
 *   timing of ClearDemoModel_* (WaitParts waits for it).
 *
 * Not translated (UI.md): the time-attack tally (ClearDemoTA_*, gGameState.unk10
 * == 2 uses the score tally here), the player model (ClearDemo_DrawModel), the
 * NaviMsg / NaviB sub-screen panel of boss results (health 0 of 8, no message:
 * nothing to show in the one frame), the Blaze palette
 * reload of the glyph sprites, SaveData_StoreStageResult / StoryProgress_Update.
 * The original names were kept even where they read swapped: GetTimeBonus reads
 * the stage timer (gGameState.unk20), GetScoreBonus the trick score
 * (data_022c4590.unk4).
 */
#include "ui_internal.h"

typedef struct CdWork CdWork;

typedef struct CdScore {     /* 0x274 */
    void (*fn)(CdWork *w, struct CdScore *o);
    s32 unk4;
    struct { s32 x, y; } pos[6]; /* 0x08 */
    s16 unk38[6];            /* 0x38 row slide (row 4 = unk40) */
    u16 unk44[6];            /* 0x44 row sound played */
    const u16 *unk50;        /* 0x50 row label sprites */
    s16 digits[6][6];        /* 0x54 */
    s16 unk9C;               /* rank sprite */
    s32 unkA0, unkA4, unkA8; /* rank scale, x, y */
    u8 unkBA, unkBB;
} CdScore;
typedef struct CdName {      /* 0x16C */
    void (*fn)(CdWork *w, struct CdName *o);
    const u8 *table;
    u16 count;
    s32 unkc, unk10;
    s16 alpha[32];
    u16 delay[32];
} CdName;
typedef struct CdActText {   /* 0x200 */
    void (*fn)(CdWork *w, struct CdActText *o);
    const u8 *table;
    u16 count;
    s32 unkc, unk10;
    s32 work[20];
    s16 unk64;               /* t */
    u16 unk66;
} CdActText;
typedef struct CdBars {      /* 0x330 */
    void (*fn)(CdWork *w, struct CdBars *o);
    u16 unk4;
    s16 t;
    s32 unk8, unkC;
} CdBars;
typedef struct CdBg {        /* 0x000 */
    void (*fn)(CdWork *w, struct CdBg *o);
    s32 unk4, unk8;          /* BG3 / BG2 y scroll (20.12) */
    s16 v, unkC;
    u8 win_x2;               /* gDisplayMain.unk10: window 0 right edge */
    u8 win_on;               /* gDisplayMain.unk1C: window 0 enabled */
} CdBg;
typedef struct CdModel {     /* 0x010: timing only */
    void (*fn)(CdWork *w, struct CdModel *o);
    s16 unk158, t;
} CdModel;

struct CdWork {
    CdBg obj0;
    CdModel obj10;
    CdName obj16C;
    CdActText obj200;
    CdScore obj274;
    CdBars obj330;
    void (*state)(CdWork *w);  /* 0x340 */
    u32 flags;                 /* 0x344 */
    s32 unk1630;               /* sound archive */
    u16 unk162C;
    s16 unk162E;
    UiSpr spr[0x33];           /* 0x39C dm_cldm anims 0..0x32 */
    int active;
};

static CdWork s_cd;

int ClearDemo_IsActive(void) { return s_cd.active; }
void ClearDemo_Destroy(void) {
    if (s_cd.active)
        ui_cdbg_free();
    s_cd.active = 0;
}

static void cd_sfx(s32 arc, s32 id) {
    if (g_ui_hooks.sfx)
        g_ui_hooks.sfx(g_ui_hooks.ctx, arc, id);
}
static void cd_voice(s32 arc, s32 id) {
    if (g_ui_hooks.voice)
        g_ui_hooks.voice(g_ui_hooks.ctx, arc, id);
}

/* ------------------------------------------------------------ bonus / rank */
u32 ClearDemo_GetTimeBonus(void) {
    const u32 *p;
    u32 score;
    u8 zone = W.zone, act = W.act;
    s32 i;
    u32 r;
    if (zone * 3 + act >= 27)
        return 0;
    p = g_ui_data.tb_thr[zone * 3 + act]; /* data_020791d0[zone][act] */
    score = W.frame;                       /* gGameState.unk20 */
    if (G.fail_flags & 4)
        return 0;
    for (i = 0, r = 0; i < 10; i++, p++) {
        if (*p >= score) {
            if (zone >= 9 || act >= 2)
                r = g_ui_data.tb_boss[i];
            else
                r = g_ui_data.tb_norm[i];
            break;
        }
    }
    return r;
}

u32 ClearDemo_GetScoreBonus(void) {
    u32 r, t;
    s32 i;
    const u32 *p;
    if (W.zone * 2 + W.act >= 41)
        return 0;
    t = G.score;                              /* data_022c4590.unk4 */
    p = g_ui_data.sb_thr[W.zone * 2 + W.act]; /* data_02079198[zone][act] */
    for (i = 0, r = 0; i < 10; i++, p++) {
        if (*p <= t) {
            r = g_ui_data.sb_bonus[i];
            break;
        }
    }
    return r;
}

s16 ClearDemo_GetRank(u32 v) {
    s16 i;
    const u32 *t;
    if (W.zone >= 9 || W.act >= 2) {
        for (i = 0, t = g_ui_data.rank_boss; i < 4; t++, i++)
            if (v >= *t)
                break;
    } else {
        for (i = 0, t = g_ui_data.rank_norm; i < 4; t++, i++)
            if (v >= *t)
                break;
    }
    return i;
}

static void ClearDemo_InitScoreTally(CdScore *s) {
    s32 i, j;
    u32 div;
    int found;
    s16 *p;
    u32 vals[6];
    if (!UI_NORMAL_STAGE()) {
        for (i = 0; i < 4; i++) {
            s->pos[i].x = -0x72000;
            s->pos[i].y = 0x33ffe + i * 0x10000;
            s->unk38[i] = 0;
        }
    } else {
        for (i = 0; i < 4; i++) {
            s->pos[i].x = -0x72000;
            s->pos[i].y = 0x34000 + i * 0x10000;
            s->unk38[i] = 0;
        }
    }
    s->pos[4].x = -0x72000;
    s->pos[4].y = 0x6c000;
    s->pos[5].x = 0x56000;
    s->pos[5].y = 0x7c000;
    memset(s->digits, 0, sizeof(s->digits));
    if (!UI_NORMAL_STAGE()) {
        vals[0] = (u32)-1;
        vals[1] = (u32)-1;
        vals[2] = ClearDemo_GetTimeBonus();
        vals[3] = (u32)g_obj_stage.ring_bonus;   /* data_022c4590.unk10 */
        vals[4] = 0;
        vals[5] = vals[2] + vals[3];
    } else {
        vals[0] = ClearDemo_GetTimeBonus();
        vals[1] = ClearDemo_GetScoreBonus();
        vals[2] = (u32)g_obj_stage.ring_bonus;   /* data_022c4590.unk10 */
        vals[3] = (u32)g_obj_stage.speed_bonus;  /* data_022c4590.unk8 */
        vals[4] = 0;
        vals[5] = vals[0] + vals[1] + vals[2] + vals[3];
    }
    for (i = 0; i < 6; i++) {
        if (vals[i] > 999999 && vals[i] != (u32)-1)
            vals[i] = 999999;
    }
    s->unk9C = (s16)(ClearDemo_GetRank(vals[5]) + 0x23);
    G.total = vals[5]; /* data_022c4590.unk0 */
    for (i = 0; i < 6; i++) {
        if (vals[i] != (u32)-1) {
            for (j = 0, found = 0, div = 100000, p = s->digits[i]; j < 6; j++, p++, div /= 10) {
                *p = (s16)(vals[i] / div);
                vals[i] = vals[i] % div;
                if (p[0] == 0 && j != 5 && !found)
                    *p = -1;
                else
                    found = 1;
            }
        }
    }
    s->unkA4 = 0x50000;
    s->unkA8 = 0x8a000;
    s->unkA0 = 0x2000;
    s->unk4 = 0;
    s->unk50 = UI_NORMAL_STAGE() ? g_ui_data.lbl_norm : g_ui_data.lbl_boss;
}

/* ------------------------------------------------------- score states */
static void ClearDemoScore_State_WaitRank(CdWork *w, CdScore *o);
static void ClearDemoScore_State_Done(CdWork *w, CdScore *o) { (void)w; (void)o; }
static void ClearDemoScore_State_HoldRank(CdWork *s, CdScore *o) {
    o->unk4++;
    if (o->unk4 >= 60) {
        s->flags |= 0x8000;
        o->fn = ClearDemoScore_State_Done;
        o->unk4 = 0;
    }
}
static void ClearDemoScore_State_ShakeRank(CdWork *w, CdScore *o) {
    s32 amp;
    o->unk4++;
    amp = (0x10 - o->unk4) * 0x500;
    if (amp != 0) {
        o->unkA4 = (s32)(UiRand() % amp + 0x50000) - amp / 2;
        o->unkA8 = (s32)(UiRand() % amp + 0x8a000) - amp / 2;
    } else {
        o->unkA4 = 0x50000;
        o->unkA8 = 0x8a000;
    }
    if (o->unk4 >= 0x10) {
        o->unkA4 = 0x50000;
        o->unkA8 = 0x8a000;
        o->fn = ClearDemoScore_State_HoldRank;
        o->unk4 = 0;
        w->flags |= 0x800000;
    }
}
static void ClearDemoScore_State_StampRank(CdWork *w, CdScore *o) {
    s32 t, v, n;
    (void)w;
    o->unk4++;
    t = (s16)(o->unk4 << 8);
    if (t > 0x1000)
        t = 0x1000;
    n = 5;
    v = 0x1000;
    do {
        v = FX_Mul(v - 0x2000, t) + 0x2000;
    } while (n--);
    o->unkA0 = v;
    if (o->unk4 >= 0x10) {
        o->unkA0 = 0x1000;
        o->fn = ClearDemoScore_State_ShakeRank;
        o->unk4 = 0;
    }
}
static void rank_voice(CdWork *w) {
    if (G.character == 0) {
        if (W.zone == 7)
            cd_voice(w->unk1630, 4);
        else if (!UI_NORMAL_STAGE())
            cd_voice(w->unk1630, 5);
        else
            cd_voice(w->unk1630, 4);
    } else {
        if (W.zone == 7)
            cd_voice(w->unk1630, 5);
        else if (!UI_NORMAL_STAGE())
            cd_voice(w->unk1630, 7);
        else
            cd_voice(w->unk1630, 6);
    }
}
static void ClearDemoScore_State_WaitRank(CdWork *w, CdScore *o) {
    o->unk4++;
    if (o->unk4 >= 0x3c) {
        w->flags |= 0x10000;
        o->fn = ClearDemoScore_State_StampRank;
        o->unk4 = 0;
        cd_sfx(w->unk1630, 3);
        rank_voice(w);
        o->unkBA = 1;
        o->unkBB = 1;
    }
}
static void ClearDemoScore_State_RevealTotal(CdWork *w, CdScore *o) {
    s32 n, k, i;
    o->unk4++;
    n = o->unk4 / 16 + 1;
    if (n > 6) {
        n = 6;
        o->fn = ClearDemoScore_State_WaitRank;
        o->unk4 = 0;
        w->flags |= 0x400000;
        /* NNS_SndPlayerStopSeq(roll sound) */
        cd_sfx(w->unk1630, 2);
    }
    k = 6 - n;
    for (i = 5; i >= 0 && n > 0; i--, n--)
        o->digits[4][i] = o->digits[5][i];
    for (i = 0; i < k; i++)
        o->digits[4][i] = (s16)(UiRand() % 10);
}
static void ClearDemoScore_State_RollTotal(CdWork *w, CdScore *s) {
    s32 i;
    (void)w;
    s->unk4++;
    if (s->unk4 & 1) {
        for (i = 0; i < 6; i++)
            s->digits[4][i] = (s16)(UiRand() % 10);
    }
    if (s->unk4 >= 0x2d) {
        s->fn = ClearDemoScore_State_RevealTotal;
        s->unk4 = 0;
    }
}
static void ClearDemoScore_State_ShowTotal(CdWork *w, CdScore *o) {
    o->unk4++;
    if (o->unk4 >= 0x3c) {
        o->fn = ClearDemoScore_State_RollTotal;
        o->unk4 = 0;
        w->flags |= 0x4000;
        cd_sfx(w->unk1630, 1);
    }
}
static void ClearDemoScore_State_SlideIn(CdWork *w, CdScore *o) {
    s32 n, i;
    o->unk4++;
    n = o->unk4 / 30 + 1;
    if (n > 5)
        n = 5;
    for (i = 0; i < n; i++) {
        o->unk38[i] += 0x100;
        if (o->unk38[i] > 0x1000) {
            o->unk38[i] = 0x1000;
            if (o->unk50[i] != 0xffff && o->unk44[i] == 0) {
                cd_sfx(w->unk1630, 0);
                o->unk44[i] = 1;
            }
        }
        o->pos[i].x = UiLerpN(0, 0xc8000, o->unk38[i], 5) - 0x72000;
    }
    if (n == 5 && o->unk38[n - 1] == 0x1000) {
        o->fn = ClearDemoScore_State_ShowTotal;
        o->unk4 = 0;
    }
}
static void ClearDemoScore_SkipRows(CdWork *o) {
    s32 i;
    CdScore *s = &o->obj274;
    s->unk4 = 0;
    o->flags |= 0xf000;
    for (i = 0; i <= 4; i++) {
        s->pos[i].x = 0x56000;
        s->unk38[i] = 0x1000;
    }
    for (i = 0; i < 6; i++)
        s->digits[4][i] = s->digits[5][i];
    s->fn = ClearDemoScore_State_WaitRank;
}
static void ClearDemoScore_SkipToRank(CdWork *w) {
    CdScore *o = &w->obj274;
    w->flags |= 0x10000;
    o->unkA4 = 0x50000;
    o->unkA8 = 0x8a000;
    o->unkA0 = 0x1000;
    o->unk4 = 0;
    o->fn = ClearDemoScore_State_HoldRank;
    if (o->unkBA == 0)
        cd_sfx(w->unk1630, 3);
    if (o->unkBB == 0)
        rank_voice(w);
}
static void ClearDemoScore_Draw(CdWork *w) {
    CdScore *o = &w->obj274;
    UiSpr *spr;
    u32 i, j;
    u16 idx;
    s16 d, bx, yy;
    s32 by;
    if (w->flags & 0x10000) {
        spr = &w->spr[o->unk9C % 0x33];
        spr->x = (s16)(o->unkA4 >> 12);
        spr->y = (s16)(o->unkA8 >> 12);
        ui_emit(spr, UI_SP_SCREEN, o->unkA0, o->unkA0, 0, 0xffffffff, 0);
    }
    for (i = 0; i < 4; i++) {
        if (o->unk38[i] != 0) {
            idx = o->unk50[i];
            if (idx != 0xffff) {
                spr = &w->spr[idx % 0x33];
                spr->x = (s16)(o->pos[i].x >> 12);
                spr->y = (s16)(o->pos[i].y >> 12);
                ui_emit_spr(spr, UI_SP_SCREEN);
                bx = (s16)(spr->x + 10);
                by = spr->y;
                for (j = 0; j < 6; j++) {
                    d = o->digits[i][j];
                    if (d != -1) {
                        spr = &w->spr[d + 0x13];
                        spr->x = (s16)(bx + ((j * 0xa000) >> 12));
                        spr->y = (s16)by;
                        ui_emit_spr(spr, UI_SP_SCREEN);
                    }
                }
            }
        }
    }
    if (o->unk38[4] != 0) { /* unk40 */
        spr = &w->spr[g_ui_data.lbl_norm[4] % 0x33]; /* data_02078b3c[4] */
        spr->x = (s16)((o->pos[4].x + 0x40000) >> 12);
        spr->y = (s16)(o->pos[4].y >> 12);
        ui_emit_spr(spr, UI_SP_SCREEN);
    }
    if (w->flags & 0x4000) {
        spr = &w->spr[33];
        spr->x = 0x56;
        spr->y = (s16)(o->pos[5].y >> 12);
        ui_emit_spr(spr, UI_SP_SCREEN);
        yy = (s16)(o->pos[5].y >> 12);
        for (j = 0; j < 6; j++) {
            d = o->digits[4][j];
            if (d != -1) {
                spr = &w->spr[d + 0x28];
                spr->x = (s16)(((j * 0xa000) >> 12) + 0x60);
                spr->y = yy;
                ui_emit_spr(spr, UI_SP_SCREEN);
            }
        }
    }
}

/* ------------------------------------------------------------- name */
static void ClearDemoName_State_Done(CdWork *s, CdName *o) { (void)o; s->flags |= 0x200; }
static void ClearDemoName_State_FadeIn(CdWork *o, CdName *s) {
    u32 i;
    for (i = 0; i < s->count; i++) {
        if (s->delay[i] == 0) {
            s->alpha[i] += 0x333;
            if (s->alpha[i] > 0x1000)
                s->alpha[i] = 0x1000;
        } else {
            s->delay[i]--;
        }
    }
    if (s->alpha[s->count - 1] == 0x1000) {
        s->fn = ClearDemoName_State_Done;
        o->flags |= 0x60400;
    }
}
static void ClearDemoName_State_Init(CdWork *o, CdName *s) {
    u32 i;
    o->flags &= ~0x200u;
    o->flags |= 1;
    if (G.character == 0) {
        s->unkc = 0x1b000;
        s->unk10 = 0x1e000;
        s->table = g_ui_data.cd_name_s; /* data_020732e8 */
        s->count = 0x12;
    } else {
        s->unkc = 0x32000;
        s->unk10 = 0x1e000;
        s->table = g_ui_data.cd_name_b; /* data_020732b8 */
        s->count = 0xd;
    }
    for (i = 0; i < 32; i++) {
        s->alpha[i] = 0;
        s->delay[i] = (u16)(i * 4);
    }
    s->fn = ClearDemoName_State_FadeIn;
}
static void ClearDemoName_Skip(CdWork *o) {
    CdName *s = &o->obj16C;
    s32 i;
    ClearDemoName_State_Init(o, s);
    o->flags |= 0x701;
    for (i = 0; i < s->count; i++)
        s->alpha[i] = 0x1000;
    s->fn = ClearDemoName_State_Done;
}
static void ClearDemoName_Draw(CdWork *w) {
    CdName *t = &w->obj16C;
    s32 xo = 0;
    u8 c;
    u32 i;
    for (i = 0; i < t->count; i++) {
        c = t->table[i];
        if (c != 0x13) {
            UiSpr *spr = &w->spr[c % 0x33];
            spr->x = (s16)((t->unkc + xo) >> 12);
            spr->y = (s16)(t->unk10 >> 12);
            if (t->alpha[i] != 0)
                ui_emit(spr, UI_SP_SCREEN, 0x1000, t->alpha[i], 0, 0xffffffff, 0);
        }
        xo += g_ui_data.cd_gw[t->table[i] % 20] << 12;
    }
}

/* --------------------------------------------------------- act text */
static int IsBossStage(void) { return W.zone >= 7 && W.zone < 9 && W.act < 2; }
static void ClearDemoActText_State_Done(CdWork *s, CdActText *o) { (void)o; s->flags |= 0x800; }
static void ClearDemoActText_State_Hold(CdWork *s, CdActText *o) {
    o->unk66++;
    if (o->unk66 > 30) {
        o->fn = ClearDemoActText_State_Done;
        s->flags |= 0x3000;
    }
}
static void ClearDemoActText_State_Spread(CdWork *o, CdActText *s) {
    u32 i;
    s32 acc = 0;
    int done = 1;
    (void)o;
    for (i = 0; i < s->count; i++) {
        s->work[i] += 0x2000;
        if (s->work[i] >= acc)
            s->work[i] = acc;
        else
            done = 0;
        acc += g_ui_data.cd_gw[s->table[i] % 20] << 12;
    }
    if (done) {
        s->unk66 = 0;
        s->fn = ClearDemoActText_State_Hold;
    }
}
static void ClearDemoActText_State_Delay(CdWork *a, CdActText *s) {
    (void)a;
    if (s->unk66++ > 15)
        s->fn = ClearDemoActText_State_Spread;
}
static void ClearDemoActText_State_BeginDelay(CdWork *a, CdActText *s) {
    (void)a;
    s->unk66 = 0;
    s->fn = ClearDemoActText_State_Delay;
}
static void ClearDemoActText_State_SlideIn(CdWork *w, CdActText *o) {
    (void)w;
    o->unk64 += 0x333;
    if (o->unk64 >= 0x1000) {
        o->unk64 = 0x1000;
        o->fn = ClearDemoActText_State_BeginDelay;
    }
    o->unk10 = UiLerpN(0xe0000, 0xb4000, o->unk64, 3);
}
static void ClearDemoActText_State_Init(CdWork *o, CdActText *s) {
    u32 i;
    o->flags &= ~0x800u;
    o->flags |= 4;
    if (IsBossStage())
        s->unkc = g_ui_data.cd_actx[3];
    else
        s->unkc = g_ui_data.cd_actx[W.act & 3];
    s->unk10 = 0xe0000;
    if (!UI_NORMAL_STAGE()) {
        s->table = g_ui_data.cd_act_ex;
        s->count = 0xe;
    } else if (IsBossStage()) {
        s->table = g_ui_data.cd_act_bs;
        s->count = 0xa;
    } else if (W.act == 0) {
        s->table = g_ui_data.cd_act_1;
        s->count = 0xb;
    } else {
        s->table = g_ui_data.cd_act_2;
        s->count = 0xb;
    }
    for (i = 0; i < 20; i++)
        s->work[i] = 0;
    s->unk64 = 0;
    s->fn = ClearDemoActText_State_SlideIn;
}
static void ClearDemoActText_Skip(CdWork *o) {
    CdActText *s = &o->obj200;
    s32 acc = 0, i;
    ClearDemoActText_State_Init(o, s);
    o->flags |= 0x3c04;
    s->unk10 = 0xb4000;
    for (i = 0; i < s->count; i++) {
        s->work[i] = acc;
        acc += g_ui_data.cd_gw[s->table[i] % 20] << 12;
    }
    s->fn = ClearDemoActText_State_Done;
}
static void ClearDemoActText_Draw(CdWork *o) {
    CdActText *s = &o->obj200;
    u32 i;
    for (i = 0; i < s->count; i++) {
        u8 t = s->table[i];
        if (t != 0x13) {
            UiSpr *spr = &o->spr[t % 0x33];
            spr->x = (s16)((s->unkc + s->work[i]) >> 12);
            spr->y = (s16)(s->unk10 >> 12);
            ui_emit_spr(spr, UI_SP_SCREEN);
        }
    }
}

/* ------------------------------------------------------------- bars */
static void ClearDemoBars_State_Done(CdWork *w, CdBars *o) { (void)w; (void)o; }
static void ClearDemoBars_State_SlideIn(CdWork *w, CdBars *o) {
    s32 v;
    o->unk4++;
    if (o->unk4 >= 0x10) {
        o->fn = ClearDemoBars_State_Done;
        o->unk4 = 0;
        o->unk8 = 0;
        o->unkC = 0xc0000;
        w->flags |= 0x80000;
        return;
    }
    o->t += 0x100;
    if (o->t > 0x1000)
        o->t = 0x1000;
    v = UiLerpN(0, 0x10000, o->t, 5);
    o->unk8 = -(0x10000 - v);
    o->unkC = 0xd0000 - v;
}
static void ClearDemoBars_Init(CdBars *s) {
    s->unk4 = 0;
    s->t = 0;
    s->unk8 = (s32)0xf0000000;
    s->unkC = 0x100c0000;
}
static void ClearDemoBars_Skip(CdWork *s) {
    CdBars *p = &s->obj330;
    p->unk8 = 0;
    p->unkC = 0xc0000;
    s->flags |= 0xe0000;
    p->fn = ClearDemoBars_State_Done;
}
static void ClearDemoBars_Draw(CdWork *o) {
    UiSpr *spr = &o->spr[39];
    spr->x = 0;
    spr->y = (s16)(o->obj330.unk8 >> 12);
    ui_emit_spr(spr, UI_SP_SCREEN);
    spr->x = 0;
    spr->y = (s16)(o->obj330.unkC >> 12);
    ui_emit(spr, UI_SP_SCREEN, 0x1000, 0x1000, UI_FLIP_V, 0xffffffff, 0);
}

/* -------------------------------------------------- bg / model (timing) */
static void ClearDemoBg_State_Done(CdWork *s, CdBg *o) { (void)o; s->flags |= 0x10; }
static void ClearDemoBg_State_Slide(CdWork *w, CdBg *o) {
    (void)w;
    o->unkC = (s16)(o->unkC + 40.96f);
    if (o->unkC >= 0x1000) {
        o->unkC = 0x1000;
        o->fn = ClearDemoBg_State_Done;
    }
    o->unk8 = UiLerpN(-0x23000, -0xa0000, o->unkC, 2);
}
static void ClearDemoBg_State_BeginSlide(CdWork *a, CdBg *o) {
    o->unk4 = 0xdf000;
    o->unk8 = -0x23000;
    o->win_on = 0;           /* gDisplayMain.unk1C = 0 */
    o->unkC = 0;
    o->fn = ClearDemoBg_State_Slide;
    o->fn(a, o);
}
static void ClearDemoBg_State_Open(CdWork *w, CdBg *s) {
    (void)w;
    s->v += 0x199;
    if (s->v >= 0x1000) {
        s->v = 0x1000;
        s->fn = ClearDemoBg_State_BeginSlide;
    }
    /* gDisplayMain.unk10: window 0 opens from x 1 to 255 */
    s->win_x2 = (u8)(((s32)(((int64_t)s->v * 0xfe000 + 0x800) >> 12) + 0x1000) >> 12);
}
static void ClearDemoBg_State_Init(CdWork *w, CdBg *o) {
    w->flags &= ~0x10u;
    o->unkC = 0;
    /* window 0: x 0..1, y 0x21..0x23; inside BG0/BG1/OBJ, outside BG2/BG3/OBJ */
    o->win_x2 = 1;
    o->win_on = 1;
    o->fn = ClearDemoBg_State_Open;
}
static void ClearDemoBg_Skip(CdWork *s) {
    s->flags |= 0x18;
    s->obj0.win_on = 0;      /* gDisplayMain.unk1C = 0 */
    s->obj0.unk4 = 0xdf000;
    s->obj0.unk8 = -0xa0000;
    s->obj0.fn = ClearDemoBg_State_Done;
}

static void ClearDemoModel_State_Done(CdWork *s, CdModel *o) { (void)o; s->flags |= 0x80; }
static void ClearDemoModel_State_Move(CdWork *w, CdModel *o) {
    s32 idx = G.character > 1 ? 0 : (s32)G.character;
    (void)w;
    if (!UI_NORMAL_STAGE())
        o->t += (s16)g_ui_data.cd_model[idx][0x28 / 4];
    else
        o->t += (s16)g_ui_data.cd_model[idx][0x20 / 4];
    if (o->t >= 0x1000) {
        o->t = 0x1000;
        o->fn = ClearDemoModel_State_Done;
    }
}
static void ClearDemoModel_State_Delay(CdWork *a, CdModel *s) {
    (void)a;
    s->unk158--;
    if (s->unk158 <= 0) {
        s->fn = ClearDemoModel_State_Move;
        s->t = 0;
    }
}
static void ClearDemoModel_State_Init(CdWork *o, CdModel *s) {
    s32 idx = G.character > 1 ? 0 : (s32)G.character;
    u32 d = (u32)g_ui_data.cd_model[idx][0x4c / 4];
    o->flags &= ~0x80u;
    o->flags |= 2;
    /* data_020734b8 / data_020734ba: s16 delay at +0x4C (normal) / +0x4E (boss) */
    s->unk158 = (s16)(UI_NORMAL_STAGE() ? (d & 0xffff) : (d >> 16));
    s->t = 0;
    s->fn = ClearDemoModel_State_Delay;
}
static void ClearDemoModel_Skip(CdWork *o) {
    o->flags |= 0xa2;
    o->obj10.fn = ClearDemoModel_State_Done;
}

/* -------------------------------------------------------- main states */
static void ClearDemo_State_ShowName(CdWork *s);
static void ClearDemo_State_ShowModel(CdWork *s);
static void ClearDemo_State_WaitParts(CdWork *s);
static void ClearDemo_State_Hold(CdWork *s);
static void ClearDemo_State_FadeOut(CdWork *o);

static void ClearDemo_State_Finish(CdWork *s) { s->flags |= 0x80000000u; }
static void ClearDemo_State_FadeOut(CdWork *o) {
    o->unk162E += 0xcc;
    if (o->unk162E >= 0x1000) {
        o->unk162E = 0x1000;
        o->state = ClearDemo_State_Finish;
    }
    g_ui_bright_main = (s16)((s32)(((int64_t)o->unk162E * 0x10000 + 0x800) >> 12) >> 12);
    g_ui_bright_sub = g_ui_bright_main;
}
static void ClearDemo_State_Hold(CdWork *s) {
    if (s->unk162C++ > 120) {
        s->state = ClearDemo_State_FadeOut;
        s->unk162C = 0;
        s->unk162E = 0;
    }
}
static void ClearDemo_State_WaitParts(CdWork *s) {
    if ((s->flags & 0x88a90) == 0x88a90)
        s->state = ClearDemo_State_Hold;
}
static void ClearDemo_State_ShowModel(CdWork *s) {
    if (s->unk162C++ > 8) {
        s->flags |= 0x20;
        s->state = ClearDemo_State_WaitParts;
        s->unk162C = 0;
    }
}
static void ClearDemo_State_ShowName(CdWork *s) {
    if (s->unk162C++ > 8) {
        s->flags |= 0x100;
        s->state = ClearDemo_State_ShowModel;
        s->unk162C = 0;
    }
}
static void ClearDemo_State_ShowBg(CdWork *s) {
    if (s->unk162C++ > 16) {
        s->flags |= 8;
        s->state = ClearDemo_State_ShowName;
        s->unk162C = 0;
    }
}
static void ClearDemo_State_FadeIn(CdWork *p0) {
    s16 t0 = g_ui_bright_main;
    if (t0 < 0) {
        g_ui_bright_main = (s16)(t0 + 1);
        g_ui_bright_sub = (s16)(g_ui_bright_sub + 1);
    } else if (t0 > 0) {
        g_ui_bright_main = (s16)(t0 - 1);
        g_ui_bright_sub = (s16)(g_ui_bright_sub - 1);
    }
    if (g_ui_bright_main == 0) {
        p0->state = ClearDemo_State_ShowBg;
        p0->unk162C = 0;
    }
}

static void ClearDemo_SkipParts(CdWork *p0) {
    p0->flags |= 0x500000;
    p0->state = ClearDemo_State_WaitParts;
    ClearDemoBg_Skip(p0);
    ClearDemoName_Skip(p0);
    ClearDemoActText_Skip(p0);
    ClearDemoModel_Skip(p0);
    ClearDemoScore_SkipRows(p0);
    ClearDemoBars_Skip(p0);
}
static void ClearDemo_SkipToRank(CdWork *s) {
    s->flags |= 0xa00000;
    s->state = ClearDemo_State_WaitParts;
    ClearDemoScore_SkipToRank(s);
}

/* ClearDemo_Finish -> ClearDemo_GoToNextScene (StageExit_Create(...)) */
static void ClearDemo_GoToNextScene(void) {
    if (W.mode == 2) {
        ui_request_exit(UI_EXIT_TRY_AGAIN);
        return;
    }
    /* zone 8: extra ending, zone 7: ending, otherwise next act / zone map */
    ui_request_exit(UI_EXIT_NEXT_ACT);
}

void ClearDemo_Create(void) {
    CdWork *w = &s_cd;
    u32 i;
    g_ui_bright_main = -16;
    g_ui_bright_sub = -16;
    BrightnessFade_Destroy();
    if (g_ui_hooks.music)
        g_ui_hooks.music(g_ui_hooks.ctx, -1); /* NNS_SndStopSoundAll */
    memset(w, 0, sizeof(*w));
    w->active = 1;
    w->state = ClearDemo_State_FadeIn;
    w->obj0.fn = ClearDemoBg_State_Init;
    /* res348 / res34c = act_com 0x1a cldm_bg, 0x1b cldm_far: ClearDemo_InitBg(Boss).
     * Native assumption: window 0 is taken as on (x 0..0) until ClearDemoBg_State_Init,
     * so cldm_far is not seen before the line opens */
    ui_cdbg_load();
    w->obj0.win_on = 1;
    w->obj16C.fn = ClearDemoName_State_Init;
    w->obj10.fn = ClearDemoModel_State_Init;
    w->obj200.fn = ClearDemoActText_State_Init;
    w->obj330.fn = ClearDemoBars_State_SlideIn;
    ClearDemoBars_Init(&w->obj330);
    /* gGameState.unk10 == 2: ClearDemo_InitTimeAttackTally (not translated) */
    ClearDemo_InitScoreTally(&w->obj274);
    w->obj274.fn = ClearDemoScore_State_SlideIn;
    for (i = 0; i < 0x33; i++)
        UiSpr_Init(&w->spr[i], UI_RES_CLDM, (u16)i, UI_SPR_LOOP);
    if (W.mode == 2)
        w->unk1630 = 5;
    else if (W.zone == 7 || !UI_NORMAL_STAGE())
        w->unk1630 = W.zone == 7 ? 7 : 6;
    else
        w->unk1630 = 6;
    /* Sound_PlaySeq(0, b): act clear jingle 0xf / boss 0x10 / zone 7 0x11 */
    if (g_ui_hooks.music)
        g_ui_hooks.music(g_ui_hooks.ctx, W.mode == 2 ? (UI_NORMAL_STAGE() ? 0xb : 0xd)
                                        : W.zone == 7 ? 0x11 : !UI_NORMAL_STAGE() ? 0x10 : 0xf);
}

void ClearDemo_Main(const InputState *in) {
    CdWork *w = &s_cd;
    u32 flags;
    u16 trig = ui_pad_bits(in->pressed);
    if (!w->active)
        return;
    w->state(w);
    flags = w->flags;
    if (!(flags & 0x80000000u)) {
        if ((trig & 1) && w->state != ClearDemo_State_FadeIn) {
            if (!(flags & 0x400000) && !(flags & 0x100000))
                ClearDemo_SkipParts(w);
            else if (!(flags & 0x800000) && !(flags & 0x200000))
                ClearDemo_SkipToRank(w);
        }
        if (w->flags & 8)
            w->obj0.fn(w, &w->obj0);
        if (w->flags & 0x100)
            w->obj16C.fn(w, &w->obj16C);
        if (w->flags & 0x400)
            w->obj200.fn(w, &w->obj200);
        if (w->flags & 0x20)
            w->obj10.fn(w, &w->obj10);
        if (w->flags & 0x1000)
            w->obj274.fn(w, &w->obj274);
        if (w->flags & 0x20000)
            w->obj330.fn(w, &w->obj330);
        /* the stage is gone (StageShutdown): black backdrop (DS backdrop colour),
         * then the BG layers (ClearDemoBg_Draw, ui_bg.c) */
        g_ui_layer = UI_LAYER_RESULTS_BG;
        ui_emit_flat(UI_SP_SCREEN, 0, 0, 256, 192, 0xff000000);
        ui_cdbg_draw(w->obj0.unk4, w->obj0.unk8, w->obj0.win_on, w->obj0.win_x2);
        g_ui_layer = UI_LAYER_RESULTS;
        if (w->flags & 0x2000)
            ClearDemoScore_Draw(w);
        if (w->flags & 1)
            ClearDemoName_Draw(w);
        if (w->flags & 4)
            ClearDemoActText_Draw(w);
        if (w->flags & 0x40000)
            ClearDemoBars_Draw(w);
    } else {
        ClearDemo_Destroy();
        if (g_ui_hooks.music)
            g_ui_hooks.music(g_ui_hooks.ctx, -1);
        ClearDemo_GoToNextScene();
    }
}

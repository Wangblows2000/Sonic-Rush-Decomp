/*
 * ZoneMap_* (src/game/Menus/ZoneMap/ + asm_fallback ZoneMap_ConfirmNode,
 * ZoneMap_State_MoveToNode): the story world map. Node graph, paths that are
 * revealed as the story advances, story steps (Sonic / Blaze), selecting a
 * zone (-> ZoneActSelect, mn_actsel.c) or the next story zone (-> stage),
 * free scrolling (X), the map pause (START, ZoneMapPause), progress saving.
 *
 * Not translated (see MENUS.md): the 3D character on the map (a map icon
 * sprite stands in), talk demos (TalkDemo: story dialogue scenes; hooks),
 * the message boxes (MsgBox 0x11 tutorial, 0x12 "start this zone?": accepted),
 * the HDMA fog effect over unexplored parts of the map.
 *
 * Touch -> pad: node taps = D-pad moves along the graph + A; scroll arrows /
 * corner button = X (scroll view) then D-pad / B; map taps on nodes in scroll
 * view = (navi hints only) not mapped.
 */
#include <math.h>
#include <stdlib.h>
#include "mn_internal.h"

#define ZM_NONE 0x12   /* no node */

typedef struct { s32 x, y, id, type; } ZmNode;          /* pts[17] (0x10) */
typedef struct {
    s32 count;
    s32 state[8];                                        /* 0 shown, 1 appearing, 2 hidden */
    struct { s32 x, y; } pos[8];
} ZmPath;                                               /* 0x64 */
typedef struct { s32 x, y; s16 w, h; s32 idx; } ZmEnt;  /* swaying decorations */

typedef struct {                       /* +4: map BG (ZoneMap_InitMapBg) */
    s32 x, y;                          /* scroll (unk0/unk4) */
    s32 w, h;                          /* unk8/unkc: map size in px */
    s32 idx;                           /* +0x54 ZoneMap_SetMapBgIndex */
    s32 unk58, unk5c, unk60;           /* unk5c: character shown (ZoneMap_Unk_ov001_022e5618) */
} ZmMapBg;

typedef struct {                       /* +0x674: node layer */
    ZmNode pts[17];
    ZmPath grp[22];
    ZmMapBg *cam;
    MnSpr spr[21];
    ZmEnt ent[7];
    MnSpr spr2[3];
    MnSpr spr3[8];
    s32 counter;
} ZmNodeLayer;

typedef struct {                       /* +0x1b08: scroll arrows + buttons */
    s32 timer, mode, blink;
    MnSpr spr[6];                      /* 0..3 arrows, 4 back button, 5 scroll button */
} ZmArrows;

typedef struct ZmNavi {               /* ZoneMap_InitNavi task work (UnkBgm_022e3180) */
    s32 id;                            /* message id shown (0x20 none) */
    s32 unk4;                          /* navigator face shown */
    s32 unk8, unkc;                    /* delayed call: count, limit (0 = off) */
    s32 unk10;                         /* delayed call: message id */
    s32 unk14;                         /* character (0 Sonic, 1 Blaze) */
} ZmNavi;

typedef struct ZoneMapWork {
    s32 unk0;                          /* 1: leave the map */
    ZmMapBg map;
    ZmNodeLayer nodes;
    s32 char_x, char_y;                /* +0x199c character position on screen (fx) */
    ZmArrows arrows;
    struct ZmNavi *unk1cc4;            /* ZoneMap_InitNavi: navigator message state (delayed-call task) */
    s32 unk1cc8, unk1ccc, unk1cd0;     /* result: character, zone index, act */
    MnBgImg *bg;                       /* files[0] zmap_map_bg.bbg */
    MnRes *icons, *arrows_res, *titl;  /* zmap_map_icon, zmap_scroll_arrow, mnu_titl */
    void *ama;                         /* zmap_map_icon.ama */
} ZoneMapWork;

typedef struct ZmState {               /* the 0xa8-byte state task work */
    s32 mode;                          /* 0 Sonic, 1 Blaze */
    s32 unk4, unk8;                    /* story step (Sonic / Blaze) */
    s32 unkc;                          /* map BG index (zones revealed) */
    u32 flags;
    s32 unk14, unk18, unk1c;           /* node, next node, path index */
    s32 unk20, unk24[8];               /* queued node moves (touch path / pass-through) */
    s32 unk44, unk48, unk4c;           /* path reveal: path, to, from */
    s32 unk50, unk54, unk58;           /* camera scroll (fx) and speed */
    s32 unk5c, unk60;                  /* camera target */
    s32 unk64, unk68;                  /* character position (fx, map space) */
    s32 unk6c, unk70;                  /* scroll (px) */
    u32 unk74;                         /* timer */
    s32 unk78, unk7c, unk80;
    s32 unk84, unk88, unk8c;
    s32 unk90, unk94, unk98;           /* free roam; pending Sonic / Blaze step */
    s32 unk9c, unka0;                  /* new emerald / counter (navi) */
    ZoneMapWork *map;                  /* +0xa4 */
} ZmState;

static ZoneMapWork *s_zm;
s32 g_zm_pause_result = 3;              /* data_ov001_0231c588 */
s32 g_zm_navi_msg;                      /* last ZoneMap_SetNaviMessage id (test aid) */
s32 g_zm_node = -1, g_zm_select_ticks;   /* test aid: node and frames spent in SelectNode */

/* ActSelect (mn_actsel.c) */
void ZoneActSelect_Create(s32 a, s32 b, s32 c);
BOOL ZoneActSelect_IsFinished(void);
s32 ZoneActSelect_GetSelectedZone(void);
s32 ZoneActSelect_GetResult(void);
/* map pause (below) */
static void ZoneMapPause_Create(BOOL flag);
/* ZoneMapPause_ShowNaviMessage */
static void ZoneMapPause_ShowNaviMessage(void) {
    NaviMsg_ShowMessage(5, 0xb4);
    NaviMsg_SetFaceAnim(7);
}
BOOL ZoneMapPause_IsClosed(void);

/* ----------------------------------------------------------- helpers */
static fx32 FX_Sqrt(fx32 v) {
    s64 x = (s64)v << 12, r;
    if (v <= 0)
        return 0;
    r = (s64)sqrt((double)x);
    while (r * r > x)
        r--;
    while ((r + 1) * (r + 1) <= x)
        r++;
    return (fx32)r;
}
/* FX_Atan: angle (0x10000 = full turn) of atan(x), x in 20.12 */
static s32 FX_Atan(fx32 x) { return (s32)(atan(x / 4096.0) * 32768.0 / 3.14159265358979); }

/* ZoneMap_SetNaviMessage: navigator message, face sequence and visibility for a
 * map event; most messages fall back to message 3 (face only) after 180 frames
 * through the delayed call */
static void ZoneMap_SetNaviMessage(ZmNavi *b, s32 id) {
    s32 mode, time, track = 0;
    BOOL fade, play;
    g_zm_navi_msg = id;
    if (!b || b->id == id)
        return;
    b->id = id;
    mode = 0;
    b->unk8 = 0;
    time = 0xb4;
    b->unkc = time;
    b->unk10 = 3;
    play = TRUE;
    fade = TRUE;
    switch (id) {
    case 0: fade = FALSE; b->unkc = 0; play = FALSE; break;
    case 1: fade = FALSE; b->unkc = 0x3c; break;
    case 2: fade = FALSE; b->unkc = 0; play = FALSE; break;
    case 3: fade = FALSE; b->unkc = 0; break;
    case 4: track = 7; mode = 4; break;
    case 5: track = 8; mode = 4; break;
    case 6: track = 9; mode = 4; break;
    case 7: track = 10; mode = 4; break;
    case 8: track = 11; mode = 4; break;
    case 9: track = 12; mode = 4; break;
    case 10: track = 13; mode = 4; break;
    case 11: track = 14; mode = 4; break;
    case 25: track = 0; mode = 3; break;
    case 26: track = 2; mode = 2; break;
    case 27: track = 3; mode = 2; break;
    case 28: track = 4; mode = 6; break;
    case 29: track = 5; mode = 7; break;
    case 30: track = 6; mode = 8; break;
    default:
        if (b->unk14 == 0) {
            switch (id) {
            case 12: track = 0x14; mode = 4; break;
            case 13: track = 0x15; mode = 4; break;
            case 14: track = 0x16; mode = 4; break;
            case 15: track = 0x17; mode = 4; break;
            case 23: track = 1; mode = 5; break;
            case 17: track = 0x18; mode = 4; break;
            case 18: track = 0x19; mode = 4; break;
            case 24: track = 0x1a; mode = 0; break;
            default: return;
            }
        } else {
            switch (id) {
            case 14: track = 0x15; mode = 4; break;
            case 15: track = 0x16; mode = 4; break;
            case 23: track = 1; mode = 5; break;
            case 16: track = 0x14; mode = 4; break;
            case 19: track = 0x17; mode = 4; break;
            case 20: track = 0x18; mode = 4; break;
            case 21: track = 0x19; mode = 4; break;
            case 22: track = 0x1a; mode = 4; break;
            case 24: track = 0x1b; mode = 0; break;
            default: return;
            }
        }
        break;
    }
    if (play) {
        if (fade)
            NaviMsg_ShowMessage(track, time);
        NaviMsg_SetFaceAnim(mode);
        if (b->unk4 == 0) {
            b->unk4 = 1;
            if (id == 1)
                NaviMsg_FadeVisible(TRUE);
            else
                NaviMsg_SetVisible(TRUE);
        }
    } else if (b->unk4 != 0) {
        b->unk4 = 0;
        if (id == 2)
            NaviMsg_FadeVisible(FALSE);
        else
            NaviMsg_SetVisible(FALSE);
    }
}

static void ZoneMap_DelayedCall_Main(void) {
    ZmNavi *t = TASK_WORK(ZmNavi);
    if (t->unkc != 0) {
        t->unk8++;
        if (t->unk8 >= t->unkc)
            ZoneMap_SetNaviMessage(t, t->unk10);
    }
}

static ZmNavi *ZoneMap_InitNavi(s32 mode, s32 keep) {
    ZmNavi *w;
    NaviBg_Load(0, mode == 1 ? 1 : 0);
    NaviM_Load();
    NaviMsg_Create();
    if (keep == 0)
        NaviM_ResetPending();
    w = (ZmNavi *)Task_Create(ZoneMap_DelayedCall_Main, NULL, 0, 0x3000, 0x1000, sizeof(ZmNavi))->work;
    memset(w, 0, sizeof(*w));
    w->id = 0x20;
    w->unk14 = mode;
    NaviMsg_SetVisible(FALSE);
    return w;
}

static BOOL ZoneMap_IsBlaze(void) { return gGameState.unk0 == 1; }

s32 ZoneMap_EaseInterp(s32 a, s32 b, s32 n, s32 m, s32 t) {
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

s32 ZoneMap_GetSonicStoryStep(void) {
    switch (gSaveData.unkC) {
    case 2: return 0;
    case 3: case 4: return 5;
    case 5: return 6;
    case 7: case 8: return 11;
    case 9: return 12;
    case 11: case 12: return 17;
    case 13: return 18;
    case 14: return 19;
    case 15: case 16: return 22;
    case 17: return 23;
    case 18: return 24;
    case 19: case 20: return 27;
    case 21: return 28;
    case 22: case 23: return 30;
    case 24: return 31;
    case 25: return 33;
    case 26: return 34;
    }
    return 0;
}

s32 ZoneMap_GetBlazeStoryStep(void) {
    switch (gSaveData.unk64) {
    case 0: return 0;
    case 1: case 2: return 3;
    case 3: return 4;
    case 5: case 6: return 11;
    case 7: return 12;
    case 8: case 9: return 13;
    case 10: return 14;
    case 11: return 15;
    case 12: case 13: return 17;
    case 14: return 18;
    case 15: return 19;
    case 16: case 17: return 21;
    case 18: return 22;
    case 19: return 23;
    case 20: case 21: return 25;
    case 22: return 26;
    case 23: return 27;
    case 24: case 25: return 29;
    case 26: return 30;
    case 27: return 32;
    case 28: return 33;
    }
    return 0;
}

static void ZoneMap_SetSonicProgressFromStep(u32 a) {
    s32 n = gSaveData.unkC, id;
    if (n == 1 || n == 4 || n == 8 || n == 12 || n == 16 || n == 20 || n == 23)
        return;
    switch (a) {
    case 0: id = 2; break;
    case 5: id = 3; break;
    case 6: id = 5; break;
    case 11: id = 7; break;
    case 12: id = 9; break;
    case 17: id = 11; break;
    case 18: id = 13; break;
    case 19: id = 14; break;
    case 22: id = 15; break;
    case 23: id = 17; break;
    case 24: id = 18; break;
    case 27: id = 19; break;
    case 28: id = 21; break;
    case 30: id = 22; break;
    case 31: id = 24; break;
    case 33: id = 25; break;
    case 34: id = 26; break;
    default: return;
    }
    SaveData_SetSonicProgress((u16)id);
}

static void ZoneMap_SetBlazeProgressFromStep(u32 a) {
    s32 n = gSaveData.unk64, id;
    if (n == 2 || n == 6 || n == 9 || n == 13 || n == 17 || n == 21 || n == 25)
        return;
    switch (a) {
    case 0: id = 0; break;
    case 3: id = 1; break;
    case 4: id = 3; break;
    case 11: id = 5; break;
    case 12: id = 7; break;
    case 13: id = 8; break;
    case 14: id = 10; break;
    case 15: id = 11; break;
    case 17: id = 12; break;
    case 18: id = 14; break;
    case 19: id = 15; break;
    case 21: id = 16; break;
    case 22: id = 18; break;
    case 23: id = 19; break;
    case 25: id = 20; break;
    case 26: id = 22; break;
    case 27: id = 23; break;
    case 29: id = 24; break;
    case 30: id = 26; break;
    case 32: id = 27; break;
    case 33: id = 28; break;
    default: return;
    }
    SaveData_SetBlazeProgress((u16)id);
}

/* the "limit" ladders of ZoneMap_Get*MaxSelectableZone / Is*ZoneSelectable */
s32 ZoneMap_GetSonicMaxSelectableZone(void) {
    s32 n = gSaveData.unkC;
    if (n >= 0x17) return 6;
    if (n >= 0x14) return 5;
    if (n >= 0x10) return 4;
    if (n >= 0xc) return 3;
    if (n >= 8) return 2;
    if (n >= 4) return 1;
    if (n >= 1) return 0;
    return -1;
}
s32 ZoneMap_GetBlazeMaxSelectableZone(void) {
    s32 n = gSaveData.unk64;
    if (n >= 0x19) return 6;
    if (n >= 0x15) return 5;
    if (n >= 0x11) return 4;
    if (n >= 0xd) return 3;
    if (n >= 9) return 2;
    if (n >= 6) return 1;
    if (n >= 2) return 0;
    return -1;
}
static BOOL ZoneMap_IsSonicZoneSelectable(s32 b) { return b <= ZoneMap_GetSonicMaxSelectableZone(); }
static BOOL ZoneMap_IsBlazeZoneSelectable(s32 b) { return b <= ZoneMap_GetBlazeMaxSelectableZone(); }
static BOOL ZoneMap_IsSonicNaviAvailable(s32 x) { return x >= 2; }
static BOOL ZoneMap_IsBlazeNaviAvailable(s32 x) {
    if (x < 9)
        return FALSE;
    if (x >= 0x1e && x < 0x21)
        return FALSE;
    return TRUE;
}

static void ZoneMap_SaveProgress(ZmState *p) {
    if (p->mode == 0)
        ZoneMap_SetSonicProgressFromStep((u32)p->unk4);
    else
        ZoneMap_SetBlazeProgressFromStep((u32)p->unk8);
    SaveData_StoreLives(gGameState.unk0, gGameState.lives, gGameState.unk28);
    SaveData_CommitOrShowError(NULL);
}

static void ZoneMap_SaveProgressOnPathArrival(ZmState *p) {
    s32 save, v, w;
    if (p->mode == 0) {
        v = p->unk94;
        if (v == 0x24)
            return;
        save = p->unk4;
        if (v == 0x15)
            v = 0x16;
        if (v == 0x1a)
            v = 0x1b;
        switch (v) {
        case 5: case 0xb: case 0x11: case 0x13: case 0x16: case 0x18: case 0x1b: case 0x1e: case 0x21:
            p->unk4 = v;
            ZoneMap_SaveProgress(p);
            break;
        }
        p->unk4 = save;
    } else {
        w = p->unk98;
        if (w == 0x23)
            return;
        save = p->unk8;
        switch (w) {
        case 3: case 11: case 13: case 15: case 17: case 19: case 21: case 23: case 25: case 27: case 29: case 32:
            p->unk8 = w;
            ZoneMap_SaveProgress(p);
            break;
        }
        p->unk8 = save;
    }
}

static void ZoneMap_Unk_ov001_022e21b8(ZmState *p) {
    s32 kind, save;
    if (p->mode != 0) {
        kind = p->unk98;
        if (kind != 0x23) {
            save = p->unk8;
            if (kind == 0x13 || kind == 0x1b) {
                p->unk8 = kind;
                ZoneMap_SaveProgress(p);
            }
            p->unk8 = save;
        }
    }
}

/* ZoneMap_Unk_ov001_022e26d0: map BG index (zones revealed) */
static s32 ZoneMap_GetMapBgIndex(void) {
    s32 t;
    if (gSaveData.unk8 & 0x1000)
        return 0;
    if (!ZoneMap_IsBlaze()) {
        t = ZoneMap_GetSonicStoryStep();
        if (t >= 0x22) return 8;
        if (t >= 0x1f) return 7;
        if (t >= 0x1c) return 6;
        if (t >= 0x17) return 5;
        if (t >= 0x12) return 4;
        if (t >= 0xc) return 3;
        if (t >= 6) return 2;
        return 1;
    }
    t = ZoneMap_GetBlazeStoryStep();
    if (t >= 0x21) return 8;
    if (t >= 0x1e) return 7;
    if (t >= 0x1a) return 6;
    if (t >= 0x16) return 5;
    if (t >= 0x12) return 4;
    if (t >= 0xe) return 3;
    if (t >= 0xc) return 2;
    if (t >= 4) return 1;
    return 0;
}

s32 ZoneMap_GetNodeForStoryStep(void) {
    s32 t;
    if (!ZoneMap_IsBlaze()) {
        t = ZoneMap_GetSonicStoryStep();
        if (t >= 0x22) return 1;
        if (t >= 0x21) return 0xe;
        if (t >= 0x1e) return 0xd;
        if (t >= 0x1b) return 0xc;
        if (t >= 0x16) return 8;
        if (t >= 0x11) return 6;
        if (t >= 0xb) return 3;
        if (t >= 5) return 2;
        return 1;
    }
    t = ZoneMap_GetBlazeStoryStep();
    if (t >= 0x21) return 1;
    if (t >= 0x20) return 0xe;
    if (t >= 0x1d) return 0xd;
    if (t >= 0x19) return 0xc;
    if (t >= 0x15) return 8;
    if (t >= 0x11) return 6;
    if (t >= 0xd) return 3;
    if (t >= 0xb) return 2;
    if (t >= 3) return 1;
    return 0;
}

s32 ZoneMap_GetNodeForCurrentZone(void) {
    static const s8 son[8] = { 1, 2, 3, 6, 8, 12, 13, 14 }, blz[8] = { 2, 6, 3, 1, 12, 8, 13, 14 };
    if (gGameState.unk1C < 8)
        return ZoneMap_IsBlaze() ? blz[gGameState.unk1C] : son[gGameState.unk1C];
    return ZoneMap_GetNodeForStoryStep();
}

s32 ZoneMap_GetPathIndex(s32 a, s32 b, s32 c) {
    switch (a) {
    case 0:
        switch (b) {
        case 1: if (c == 2) return 0; break;
        case 2: if (c == 1) return 0; if (c == 15) return 9; if (c == 3) return 1; break;
        case 3: if (c == 2) return 1; if (c == 6) return 2; if (c == 16) return 10; break;
        case 6: if (c == 3) return 2; if (c == 7) return 3; break;
        case 8: if (c == 7) return 4; if (c == 9) return 5; break;
        case 12: if (c == 9) return 6; if (c == 13) return 7; break;
        case 13: if (c == 12) return 7; if (c == 14) return 8; break;
        case 14: if (c == 13) return 8; break;
        case 15: if (c == 2) return 9; break;
        case 16: if (c == 3) return 10; break;
        case 7: if (c == 6) return 3; if (c == 8) return 4; break;
        case 9: if (c == 8) return 5; if (c == 12) return 6; break;
        }
        break;
    case 1:
        switch (b) {
        case 1: if (c == 0) return 11; if (c == 16) return 12; break;
        case 2: if (c == 16) return 13; if (c == 3) return 14; break;
        case 3: if (c == 2) return 14; if (c == 4) return 15; if (c == 5) return 15; break;
        case 6: if (c == 4) return 16; if (c == 5) return 16; break;
        case 8:
            if (c == 4) return 17;
            if (c == 5) return 17;
            if (c == 11) return 18;
            if (c == 10) return 18;
            break;
        case 12: if (c == 11) return 19; if (c == 10) return 19; break;
        case 13: if (c == 11) return 20; if (c == 10) return 20; if (c == 14) return 21; break;
        case 14: if (c == 13) return 21; break;
        case 16: if (c == 1) return 12; if (c == 2) return 13; break;
        case 0: if (c == 1) return 11; break;
        case 4: case 5: if (c == 3) return 15; if (c == 6) return 16; if (c == 8) return 17; break;
        case 10: case 11: if (c == 8) return 18; if (c == 12) return 19; if (c == 13) return 20; break;
        }
        break;
    }
    return 23;
}

s32 ZoneMap_GetDirectionAngle(s32 x0, s32 y0, s32 x1, s32 y1) {
    fx32 dx = (x1 - x0) << 12, dy = (y1 - y0) << 12;
    s32 a;
    if (dx == 0)
        return dy >= 0 ? 0 : 0x8000;
    if (dy == 0)
        return dx >= 0 ? 0x4000 : 0xc000;
    a = FX_Atan(FX_Div(dx < 0 ? -dx : dx, dy < 0 ? -dy : dy));
    if (dx >= 0) {
        if (dy < 0)
            return 0x8000 - a;
    } else {
        if (dy >= 0)
            return 0x10000 - a;
        a += 0x8000;
    }
    return a;
}

static void ZoneMap_CalcClampedScroll(const ZmMapBg *p, s32 x, s32 y, s32 *ox, s32 *oy) {
    s32 mx = p->w - 0x108, my = p->h - 0xc0;
    x -= 0x80;
    y -= 0x60;
    if (x < 8)
        x = 8;
    if (x > mx)
        x = mx;
    if (y < 0)
        y = 0;
    if (y > my)
        y = my;
    *ox = x;
    *oy = y;
}

/* ------------------------------------------------------------- paths */
static void ZoneMap_SplineSolve(s32 n, const s32 *x, const s32 *y, s32 *z) {
    s32 i, t, h[4], d[4];
    z[0] = 0;
    z[n - 1] = 0;
    for (i = 0; i < n - 1; i++) {
        h[i] = x[i + 1] - x[i];
        d[i + 1] = FX_Div(y[i + 1] - y[i], h[i]);
    }
    z[1] = d[2] - d[1] - FX_Mul(h[0], z[0]);
    d[1] = (x[2] - x[0]) * 2;
    for (i = 1; i < n - 2; i++) {
        t = FX_Div(h[i], d[i]);
        z[i + 1] = d[i + 2] - d[i + 1] - FX_Mul(z[i], t);
        d[i + 1] = (x[i + 2] - x[i]) * 2 - FX_Mul(h[i], t);
    }
    z[n - 2] -= FX_Mul(h[n - 2], z[n - 1]);
    for (i = n - 2; i > 0; i--)
        z[i] = FX_Div(z[i] - FX_Mul(h[i], z[i + 1]), d[i]);
}

/* ZoneMap_SplineEval(x, n, xs, ds (values), ys (second derivatives)) */
static s32 ZoneMap_SplineEval(s32 x, s32 n, const s32 *xs, const s32 *ds, const s32 *ys) {
    s32 lo = 0, hi = n - 1, mid, h, y0, y1, t, b, c;
    while (lo < hi) {
        mid = (lo + hi) / 2;
        if (xs[mid] < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo > 0)
        lo--;
    h = xs[lo + 1] - xs[lo];
    y1 = ys[lo + 1];
    y0 = ys[lo];
    t = x - xs[lo];
    b = y0 + (y0 << 1) + FX_Div(FX_Mul(y1 - y0, t), h);
    c = FX_Div(ds[lo + 1] - ds[lo], h);
    return ds[lo] + FX_Mul(FX_Mul(b, t) + (c - FX_Mul(y0 + y0 + y1, h)), t);
}

static void ZoneMap_BuildSplinePath(s32 n, s32 *out, fx32 x0, fx32 y0, fx32 x1, fx32 y1, fx32 x2, fx32 y2, fx32 x3,
                                    fx32 y3) {
    s32 i;
    fx32 dx, dy, s, t[4], cx[4], cy[4], xs[4] = { x0, x1, x2, x3 }, ys[4] = { y0, y1, y2, y3 };
    t[0] = 0;
    for (i = 1; i < 4; i++) {
        dx = xs[i] - xs[i - 1];
        dy = ys[i] - ys[i - 1];
        t[i] = t[i - 1] + FX_Sqrt(FX_Mul(dx, dx) + FX_Mul(dy, dy));
    }
    for (i = 1; i < 4; i++)
        t[i] = FX_Div(t[i], t[3]);
    ZoneMap_SplineSolve(4, t, xs, cx);
    ZoneMap_SplineSolve(4, t, ys, cy);
    for (i = 0; i < n; i++) {
        s = FX_Div((i + 1) << 12, (n + 1) << 12);
        out[0] = ZoneMap_SplineEval(s, 4, t, xs, cx) >> 12;
        out[1] = ZoneMap_SplineEval(s, 4, t, ys, cy) >> 12;
        out += 2;
    }
}

static void ZoneMap_BuildLinePath(s32 n, s32 *out, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 i;
    fx32 t;
    for (i = 0; i < n; i++) {
        t = FX_Div((i + 1) << 12, (n + 1) << 12);
        out[0] = (x0 + FX_Mul(x1 - x0, t)) >> 12;
        out[1] = (y0 + FX_Mul(y1 - y0, t)) >> 12;
        out += 2;
    }
}

static void ama_point(const void *ama, s32 seq, s32 *x, s32 *y) {
    KeyframeAnim a;
    KeyframeAnim_Init(&a, ama, 0);
    KeyframeAnim_SetAnim(&a, (u32)seq);
    KeyframeAnim_Update(&a);
    *x = a.x;
    *y = a.y;
}

static void ZoneMap_InitPath(ZmNodeLayer *p, const void *res, s32 i0, s32 a1, s32 a2, s32 i3, s32 idx, s32 n) {
    s32 x0, y0, x1, y1, x2 = 0, y2 = 0, x3, y3, i;
    x0 = p->pts[i0].x << 12;
    y0 = p->pts[i0].y << 12;
    ama_point(res, a1, &x1, &y1);
    x1 <<= 12;
    y1 <<= 12;
    if (a2 >= 0) {
        ama_point(res, a2, &x2, &y2);
        x2 <<= 12;
        y2 <<= 12;
    }
    x3 = p->pts[i3].x << 12;
    y3 = p->pts[i3].y << 12;
    if (a2 >= 0)
        ZoneMap_BuildSplinePath(n, &p->grp[idx].pos[0].x, x0, y0, x1, y1, x2, y2, x3, y3);
    else
        ZoneMap_BuildLinePath(n, &p->grp[idx].pos[0].x, x0, y0, x1, y1);
    p->grp[idx].count = n;
    for (i = 0; i < 8; i++)
        p->grp[idx].state[i] = 2;
}

static void ZoneMap_ClearPath(ZmNodeLayer *p, s32 idx) {
    s32 i;
    for (i = 0; i < 8; i++) {
        p->grp[idx].pos[i].x = 0;
        p->grp[idx].pos[i].y = 0;
        p->grp[idx].state[i] = 2;
        p->grp[idx].count = 0;
    }
}

static void set_node(ZmNode *out, const void *ama, s32 e, s32 anim, s32 n) {
    ama_point(ama, anim, &out[e].x, &out[e].y);
    out[e].id = n;
    out[e].type = 3;
}
static void clr_node(ZmNode *out, s32 e, s32 type) {
    out[e].x = 0;
    out[e].y = 0;
    out[e].id = 0x16;
    out[e].type = type;
}

static void ZoneMap_InitSonicNodes(ZmNode *out, const void *a) {
    set_node(out, a, 1, 0, 2);
    set_node(out, a, 2, 1, 3);
    set_node(out, a, 3, 2, 4);
    set_node(out, a, 6, 3, 5);
    set_node(out, a, 8, 4, 6);
    set_node(out, a, 12, 5, 7);
    set_node(out, a, 13, 6, 8);
    set_node(out, a, 14, 7, 9);
    set_node(out, a, 15, 8, 11);
    set_node(out, a, 16, 9, 12);
    set_node(out, a, 7, 11, 13);
    set_node(out, a, 9, 12, 14);
    clr_node(out, 0, 4);
    clr_node(out, 4, 4);
    clr_node(out, 5, 4);
    clr_node(out, 11, 4);
    clr_node(out, 10, 4);
}

static void ZoneMap_InitBlazeNodes(ZmNode *out, const void *a) {
    set_node(out, a, 1, 3, 5);
    set_node(out, a, 2, 0, 2);
    set_node(out, a, 3, 2, 4);
    set_node(out, a, 6, 1, 3);
    set_node(out, a, 8, 5, 7);
    set_node(out, a, 12, 4, 6);
    set_node(out, a, 13, 6, 8);
    set_node(out, a, 14, 7, 9);
    set_node(out, a, 16, 9, 12);
    set_node(out, a, 0, 10, 10);
    set_node(out, a, 4, 11, 15);
    set_node(out, a, 5, 11, 16);
    set_node(out, a, 11, 12, 17);
    set_node(out, a, 10, 12, 18);
    clr_node(out, 15, 4);
    clr_node(out, 7, 4);
    clr_node(out, 9, 3);
}

static void ZoneMap_InitSonicPaths(ZmNodeLayer *p, const void *a) {
    s32 i;
    ZoneMap_InitPath(p, a, 1, 13, 14, 2, 0, 6);
    ZoneMap_InitPath(p, a, 2, 15, 16, 3, 1, 5);
    ZoneMap_InitPath(p, a, 3, 17, 18, 6, 2, 4);
    ZoneMap_InitPath(p, a, 6, 19, 20, 7, 3, 4);
    ZoneMap_InitPath(p, a, 7, 21, 22, 8, 4, 4);
    ZoneMap_InitPath(p, a, 8, 23, 24, 9, 5, 5);
    ZoneMap_InitPath(p, a, 9, 25, 26, 12, 6, 5);
    ZoneMap_InitPath(p, a, 12, 27, 28, 13, 7, 5);
    ZoneMap_InitPath(p, a, 13, 29, 30, 14, 8, 5);
    ZoneMap_InitPath(p, a, 2, 31, 32, 15, 9, 5);
    ZoneMap_InitPath(p, a, 3, 33, 34, 16, 10, 5);
    for (i = 11; i <= 21; i++)
        ZoneMap_ClearPath(p, i);
}

static void ZoneMap_InitBlazePaths(ZmNodeLayer *p, const void *a) {
    s32 i;
    ZoneMap_InitPath(p, a, 0, 35, 36, 1, 11, 5);
    ZoneMap_InitPath(p, a, 1, 37, 38, 16, 12, 7);
    ZoneMap_InitPath(p, a, 16, 39, 40, 2, 13, 7);
    ZoneMap_InitPath(p, a, 2, 41, 42, 3, 14, 5);
    ZoneMap_InitPath(p, a, 3, 43, 44, 4, 15, 5);
    ZoneMap_InitPath(p, a, 4, 45, 46, 6, 16, 5);
    ZoneMap_InitPath(p, a, 5, 47, 48, 8, 17, 5);
    ZoneMap_InitPath(p, a, 8, 49, 50, 10, 18, 5);
    ZoneMap_InitPath(p, a, 10, 51, 52, 12, 19, 5);
    ZoneMap_InitPath(p, a, 11, 53, 54, 13, 20, 3);
    ZoneMap_InitPath(p, a, 13, 55, 56, 14, 21, 5);
    for (i = 0; i <= 10; i++)
        ZoneMap_ClearPath(p, i);
}

/* ZoneMap_Unk_ov001_022e432c: decorations */
static void ZoneMap_InitDecorations(ZmNodeLayer *p, const void *a) {
    static const s16 tbl[7][4] = { { 0x39, 0x38, 0x18, 0 }, { 0x3a, 0x38, 0x18, 0 }, { 0x3b, 0x38, 0x18, 0 },
                                   { 0x3c, 0x38, 0x18, 0 }, { 0x3d, 0x88, 0x40, 1 }, { 0x3e, 0x30, 0x10, 2 },
                                   { 0x3f, 0x30, 0x10, 2 } };
    s32 i;
    for (i = 0; i < 7; i++) {
        ama_point(a, tbl[i][0], &p->ent[i].x, &p->ent[i].y);
        p->ent[i].w = tbl[i][1];
        p->ent[i].h = tbl[i][2];
        p->ent[i].idx = tbl[i][3];
    }
}

static void ZoneMap_InitSprite(MnSpr *s, const MnRes *res, u32 idx, u32 prio) {
    AnimatorSprite_Init(s, res, (u16)idx, 0x40, 0);
    s->flags &= ~0x60000u;
    s->flags |= prio << 17;
}

static void ZoneMap_InitNodeLayer(ZmNodeLayer *p, ZmMapBg *a, const void *ama, const MnRes *c) {
    s32 i;
    memset(p, 0, sizeof(*p));
    p->cam = a;
    if (!ZoneMap_IsBlaze()) {
        ZoneMap_InitSonicNodes(p->pts, ama);
        ZoneMap_InitSonicPaths(p, ama);
    } else {
        ZoneMap_InitBlazeNodes(p->pts, ama);
        ZoneMap_InitBlazePaths(p, ama);
    }
    ZoneMap_InitDecorations(p, ama);
    for (i = 0; i <= 16; i++)
        ZoneMap_InitSprite(&p->spr[i], c, (u32)i, 3);
    ZoneMap_InitSprite(&p->spr[18], c, 17, 3);
    ZoneMap_InitSprite(&p->spr[17], c, 18, 3);
    ZoneMap_InitSprite(&p->spr[19], c, 19, 3);
    ZoneMap_InitSprite(&p->spr[20], c, 31, 3);
    for (i = 0; i < 8; i++)
        ZoneMap_InitSprite(&p->spr3[i], c, (u32)(i + 0x17), 3);
    for (i = 0; i < 3; i++)
        ZoneMap_InitSprite(&p->spr2[i], c, (u32)(20 + i), 2);   /* flags &= ~0x60000; |= 0x40000 */
}

/* ------------------------------------------------------------ drawing */
static void ZoneMap_DrawNodeIcons(ZmNodeLayer *p) {
    s32 i;
    for (i = 0; i < 8; i++) {
        const ZmNode *pt = &p->pts[g_mn_data.zm_icons[i] & 0x1f];
        MnSpr *s = &p->spr3[i];
        if (pt->type == 0) {
            s->x = (s16)(pt->x - p->cam->x);
            s->y = (s16)(pt->y - p->cam->y);
            s->x -= 0x1c;
            s->y += 7;
            if (s->x >= -0x38 && s->x < 0x100 && s->y >= -16 && s->y < 0xc0) {
                AnimatorSprite_ProcessAnimation(s, 0, 0);
                AnimatorSprite_Draw(s);
            }
        }
    }
}

static void ZoneMap_DrawNodeLayer(ZmNodeLayer *p) {
    MnSpr *s0 = &p->spr[0], *s1 = &p->spr[1], *s = s0;
    ZmNode *pt;
    s32 i, k, t, v, sgn = 0;
    AnimatorSprite_ProcessAnimation(s0, 0, 0);
    AnimatorSprite_ProcessAnimation(s1, 0, 0);
    ZoneMap_DrawNodeIcons(p);
    for (i = 0, pt = p->pts; i < 17; pt++, i++) {
        if (pt->type == 7 || pt->type == 4 || pt->type == 5)
            continue;
        if (pt->type == 0)
            s = &p->spr[pt->id < 21 ? pt->id : 0];
        else if (pt->type == 1 || pt->type == 3)
            s = &p->spr[19];
        else if (pt->type == 2)
            s = &p->spr[0];
        s->x = (s16)(pt->x - p->cam->x);
        s->y = (s16)(pt->y - p->cam->y);
        if (s->x >= -0x20 && s->x < 0x120 && s->y >= -0x20 && s->y < 0xe0) {
            AnimatorSprite_ProcessAnimation(s, 0, 0);
            AnimatorSprite_Draw(s);
            if (pt->type == 0 && (i == 4 || i == 5 || (u32)(i - 9) <= 2)) {
                s = &p->spr[19];
                s->x = (s16)(pt->x - p->cam->x);
                s->y = (s16)(pt->y - p->cam->y);
                AnimatorSprite_ProcessAnimation(s, 0, 0);
                AnimatorSprite_Draw(s);
            }
        }
    }
    for (i = 0; i < 22; i++) {
        ZmPath *g = &p->grp[i];
        for (k = 0; k < g->count; k++) {
            if (g->state[k] == 2)
                continue;
            s = g->state[k] == 0 ? s0 : s1;
            s->x = (s16)(g->pos[k].x - p->cam->x);
            s->y = (s16)(g->pos[k].y - p->cam->y);
            if (s->x >= -8 && s->x < 0x108 && s->y >= -8 && s->y < 200)
                AnimatorSprite_Draw(s);
        }
    }
    for (i = 0; i < 3; i++)
        AnimatorSprite_ProcessAnimation(&p->spr2[i], 0, 0);
    for (i = 0; i < 7; i++) {
        ZmEnt *e = &p->ent[i];
        s = &p->spr2[e->idx];
        s->x = (s16)(e->x - p->cam->x);
        s->y = (s16)(e->y - p->cam->y);
        t = i * 240 / 7;
        t = (t + p->counter) % 240;
        switch (t / 60) {
        case 0: t = t % 60; sgn = t; break;
        case 1: t = 59 - t % 60; sgn = t; break;
        case 2: t = t % 60; sgn = -t; break;
        case 3: t = 59 - t % 60; sgn = -t; break;
        default: t = 0; break;
        }
        v = ZoneMap_EaseInterp(0, 6, 60, t, 0x2000);
        if (sgn < 0)
            v = -v;
        s->x += (s16)v;
        if (s->x >= -e->w && s->x < 0x100 && s->y >= -e->h && s->y < 0xc0)
            AnimatorSprite_Draw(s);
    }
    p->counter++;
}

/* ZoneMap_UpdateMapBg: BG3 shows the map at the scroll position (the column
 * streaming and the per-zone fog are DS display work). */
static void ZoneMap_UpdateMapBg(ZmMapBg *p) {
    gDisplayMain[3].x = (u16)p->x;
    gDisplayMain[3].y = (u16)p->y;
}

static void ZoneMap_UpdateView(ZmState *p) {
    ZoneMapWork *sub = p->map;
    MnSpr *s;
    sub->char_x = p->unk64 - (p->unk6c << 12);
    sub->char_y = p->unk68 - (p->unk70 << 12);
    sub->map.x = p->unk6c;
    sub->map.y = p->unk70;
    ZoneMap_UpdateMapBg(&sub->map);
    sub->map.unk60 += sub->map.unk5c;   /* ZoneMap_Unk_ov001_022e560c */
    ZoneMap_DrawNodeLayer(&sub->nodes);
    /* ZoneMap_DrawCharModel: no 3D model here; the map icon the original
     * draws for Blaze's first steps stands for the character */
    if (sub->map.unk5c) {
        s = &sub->nodes.spr[20];
        s->x = (s16)(sub->char_x >> 12);
        s->y = (s16)(sub->char_y >> 12);
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_Draw(s);
    }
}

/* scroll arrows / buttons */
static void ZoneMap_InitScrollArrows(ZmArrows *p, const MnRes *a, const MnRes *b) {
    s32 i;
    p->timer = 0;
    p->mode = 0;
    p->blink = 0;
    for (i = 0; i < 4; i++) {
        ZoneMap_InitSprite(&p->spr[i], a, (u32)i, 0);
        p->spr[i].flags |= SPR_LOOP;
        AnimatorSprite_ProcessAnimation(&p->spr[i], 0, 0);
    }
    ZoneMap_InitSprite(&p->spr[4], b, 0, 0);
    p->spr[4].flags |= SPR_LOOP;
    ZoneMap_InitSprite(&p->spr[5], a, 4, 0);
    p->spr[5].flags |= SPR_LOOP;
}

static void ZoneMap_SetScrollArrowMode(ZmArrows *p, s32 mode) {
    s32 i;
    if (p->mode != mode) {
        p->mode = mode;
        for (i = 0; i < 4; i++) {
            AnimatorSprite_SetAnimation(&p->spr[i], (u16)i);
            AnimatorSprite_ProcessAnimation(&p->spr[i], 0, 0);
        }
    }
}
static void ZoneMap_SetScrollArrowBlink(ZmArrows *p, s32 v) {
    if (p->blink != v) {
        p->blink = v;
        p->timer = 0;
    }
}
static void ZoneMap_UpdateScrollArrows(ZmState *w) {
    ZmArrows *p = &w->map->arrows;
    s32 i;
    if (p->mode != 0)
        for (i = 0; i < 4; i++) {
            p->spr[i].flags |= SPR_LOOP;
            AnimatorSprite_ProcessAnimation(&p->spr[i], 0, 0);
        }
    if (p->blink != 0)
        p->timer++;
    if (p->blink == 0 || p->timer % 24 >= 6) {
        static const s16 pos[4][2] = { { 0, 0x50 }, { 0x70, 0 }, { 0xf0, 0x50 }, { 0x70, 0xb0 } };
        for (i = 0; i < 4; i++) {
            p->spr[i].x = pos[i][0];
            p->spr[i].y = pos[i][1];
            AnimatorSprite_Draw(&p->spr[i]);
        }
    }
}
static void ZoneMap_DrawButton(ZmState *w, int which) {
    MnSpr *s = &w->map->arrows.spr[which];
    s->x = 8;
    s->y = 0x98;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}
static void ZoneMap_RestartButtonAnim(ZmState *w, int which) {
    AnimatorSprite_SetAnimation(&w->map->arrows.spr[which], which == 4 ? 0 : 4);
    w->map->arrows.spr[5].flags &= ~(u32)SPR_LOOP;
}

/* ------------------------------------------------------- reveal / emeralds */
static void ZoneMap_SetPathDotsType(ZmState *p, s32 i, s32 v) {
    ZmPath *e = &p->map->nodes.grp[i];
    s32 j;
    for (j = 0; j < e->count; j++)
        e->state[j] = v;
}

static void ZoneMap_RevealSonicNodes(ZmState *p) {
    s32 t0 = p->unk4;
    ZmNode *n = p->map->nodes.pts;
    if (t0 >= 0)
        n[1].type = 0;
    if (t0 >= 5) {
        n[2].type = 0;
        ZoneMap_SetPathDotsType(p, 0, 0);
    }
    if (t0 >= 0xb) {
        n[15].type = 0;
        n[3].type = 0;
        ZoneMap_SetPathDotsType(p, 9, 0);
        ZoneMap_SetPathDotsType(p, 1, 0);
    }
    if (t0 >= 0x11) {
        n[16].type = 0;
        n[6].type = 0;
        ZoneMap_SetPathDotsType(p, 0xa, 0);
        ZoneMap_SetPathDotsType(p, 2, 0);
    }
    if (t0 >= 0x13) {
        n[7].type = 0;
        ZoneMap_SetPathDotsType(p, 3, 0);
    }
    if (t0 >= 0x14)
        n[7].type = 2;
    if (t0 >= 0x16) {
        n[8].type = 0;
        ZoneMap_SetPathDotsType(p, 4, 0);
    }
    if (t0 >= 0x18) {
        n[9].type = 0;
        ZoneMap_SetPathDotsType(p, 5, 0);
    }
    if (t0 >= 0x19)
        n[9].type = 2;
    if (t0 >= 0x1b) {
        n[12].type = 0;
        ZoneMap_SetPathDotsType(p, 6, 0);
    }
    if (t0 >= 0x1e) {
        n[13].type = 0;
        ZoneMap_SetPathDotsType(p, 7, 0);
    }
    if (t0 >= 0x21) {
        n[14].type = 0;
        ZoneMap_SetPathDotsType(p, 8, 0);
    }
}

static void ZoneMap_RevealBlazeNodes(ZmState *p) {
    s32 lv = p->unk8;
    ZmNode *n = p->map->nodes.pts;
    n[0].type = 0;
    if (lv >= 3) {
        n[1].type = 0;
        ZoneMap_SetPathDotsType(p, 0xb, 0);
    }
    if (lv >= 0xb) {
        n[2].type = 0;
        ZoneMap_SetPathDotsType(p, 0xd, 0);
    }
    if (lv >= 0xd) {
        n[3].type = 0;
        ZoneMap_SetPathDotsType(p, 0xe, 0);
    }
    if (lv >= 0x11) {
        n[6].type = 0;
        ZoneMap_SetPathDotsType(p, 0x10, 0);
    }
    if (lv >= 0x15) {
        n[8].type = 0;
        ZoneMap_SetPathDotsType(p, 0x11, 0);
    }
    if (lv >= 0x19) {
        n[12].type = 0;
        ZoneMap_SetPathDotsType(p, 0x13, 0);
    }
    if (lv >= 0x1d) {
        n[13].type = 0;
        ZoneMap_SetPathDotsType(p, 0x14, 0);
    }
    if (lv >= 0x20) {
        n[14].type = 0;
        ZoneMap_SetPathDotsType(p, 0x15, 0);
    }
    if (lv >= 6) {
        n[16].type = 0;
        ZoneMap_SetPathDotsType(p, 0xc, 0);
    }
    if (lv >= 0x14) {
        n[5].type = 1;
        n[4].type = 5;
    } else if (lv >= 0x13) {
        n[5].type = 0;
        n[4].type = 5;
    } else if (lv >= 0x10) {
        n[5].type = 1;
        n[4].type = 5;
    } else if (lv >= 0xf) {
        n[4].type = 0;
    }
    if (lv >= 0xf)
        ZoneMap_SetPathDotsType(p, 0xf, 0);
    if (lv >= 0x1c) {
        n[11].type = 1;
        n[10].type = 5;
    } else if (lv >= 0x1b) {
        n[11].type = 0;
        n[10].type = 5;
    } else if (lv >= 0x18) {
        n[11].type = 1;
        n[10].type = 5;
    } else if (lv >= 0x17) {
        n[10].type = 0;
    }
    if (lv >= 0x17)
        ZoneMap_SetPathDotsType(p, 0x12, 0);
}

static void ZoneMap_RevealNodesForStory(ZmState *p) {
    if (p->mode == 0)
        ZoneMap_RevealSonicNodes(p);
    else
        ZoneMap_RevealBlazeNodes(p);
}

static BOOL ZoneMap_CheckNewEmerald(ZmState *w) {
    s32 r = NaviM_CheckStepAdvanced();
    s32 idx = 0;
    BOOL found = FALSE, save = FALSE;
    u8 cur;
    u32 a, b;
    s32 i;
    if (w->mode == 0) {
        cur = gSaveData.unkF;
        b = gSaveData.unk67 & 0x7f;   /* data_022c46e0.unk7 */
        a = cur & 0x7fu;
        if (a > b) {
            for (i = 6; i >= 0; i--) {
                if ((a & (1u << i)) && !(b & (1u << i))) {
                    idx = i;
                    found = TRUE;
                    break;
                }
            }
        }
        if (found == TRUE) {
            gSaveData.unk67 = cur;
            save = TRUE;
        }
    } else {
        switch (w->unk8) {
        case 4: found = TRUE; idx = 0; break;
        case 0xc: found = TRUE; idx = 1; break;
        case 0xe: found = TRUE; idx = 2; break;
        case 0x12: found = TRUE; idx = 3; break;
        case 0x16: found = TRUE; idx = 4; break;
        case 0x1a: found = TRUE; idx = 5; break;
        case 0x1e: found = TRUE; idx = 6; break;
        }
    }
    if (r == 0 && !found)
        return FALSE;
    w->unk74 = 0;
    w->unk9c = found;
    w->unka0 = r;
    if (found)
        NaviM_SetNewEmerald(idx);
    if (save == TRUE)
        SaveData_CommitOrShowError(NULL);
    return TRUE;
}

/* ------------------------------------------------------------ states */
static void ZoneMap_State_WaitTimer(void);
static void ZoneMap_State_SelectNode(void);
static void ZoneMap_State_MoveToNode(void);
static void ZoneMap_State_ScrollToPathStart(void);
static void ZoneMap_State_MoveAlongPath(void);
static void ZoneMap_State_ScrollToPos(void);
static void ZoneMap_State_ScrollView(void);
static void ZoneMap_State_WaitNaviMessage(void);
static void ZoneMap_State_PlayTalkDemo(void);
static void ZoneMap_State_WaitMsgBox11(void);
static void ZoneMap_State_WaitMsgBox12(void);
static void ZoneMap_State_StartStage(void);
static void ZoneMap_State_OpenActSelect(void);
static void ZoneMap_State_WaitActSelect(void);
static void ZoneMap_Compose(void);
static void ZoneMap_State_WaitPauseClosed(void);
static void ZoneMap_State_FadeOutExit(void);
static void ZoneMap_RunSonicStoryStep(void);
static void ZoneMap_RunBlazeStoryStep(void);

#define ZS TASK_WORK(ZmState)

static void ZoneMap_SelectNextState(void) {
    ZmState *w = ZS;
    w->unk74 = 0;
    if (w->unk9c != 0 || w->unka0 != 0) {
        gTaskCurrent->func = ZoneMap_State_WaitNaviMessage;
        return;
    }
    if (w->unk90 != 0) {
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        return;
    }
    switch (w->mode) {
    case 0:
        if (w->unk94 != 0x24) {
            w->unk4 = w->unk94;
            w->unk94 = 0x24;
        }
        ZoneMap_RunSonicStoryStep();
        break;
    case 1:
        if (w->unk98 != 0x23) {
            w->unk8 = w->unk98;
            w->unk98 = 0x23;
        }
        ZoneMap_RunBlazeStoryStep();
        break;
    }
}

static void ZoneMap_Action_WaitTimer(s32 v) {
    ZmState *t = ZS;
    t->unk74 = 0;
    t->unk80 = v;
    gTaskCurrent->func = ZoneMap_State_WaitTimer;
}

static void ZoneMap_Action_ScrollToPathStart(s32 p0, s32 p1, s32 p2) {
    ZmState *t = ZS;
    t->unk44 = p0;
    t->unk48 = p2;
    t->unk4c = p1;
    t->unk50 = t->unk6c << 12;
    t->unk54 = t->unk70 << 12;
    t->unk58 = 0x3000;
    gTaskCurrent->func = ZoneMap_State_ScrollToPathStart;
}

static void ZoneMap_Action_ScrollToPos(s32 p0, s32 p1, s32 p2) {
    ZmState *t = ZS;
    t->unk50 = t->unk6c << 12;
    t->unk54 = t->unk70 << 12;
    t->unk58 = p2;
    t->unk5c = p0;
    t->unk60 = p1;
    t->unk74 = 0;
    gTaskCurrent->func = ZoneMap_State_ScrollToPos;
}

static void ZoneMap_Action_MoveToNode(s32 p0) {
    ZmState *t = ZS;
    t->unk74 = 0;
    t->unk18 = p0;
    t->unk1c = ZoneMap_GetPathIndex(t->mode, t->unk14, t->unk18);
    gTaskCurrent->func = ZoneMap_State_MoveToNode;
}

/* ZoneMap_Action_PlayTalkDemoSonic / Blaze: the story dialogue (TalkDemo) */
static void ZoneMap_Action_PlayTalkDemo(void) {
    ZmState *t = ZS;
    if (t->mode == 0)
        t->map->map.unk5c = 0;   /* ZoneMap_Unk_ov001_022e5618(.., 0) */
    ZoneMap_SetNaviMessage(t->map->unk1cc4,
                           (t->mode == 0 ? ZoneMap_IsSonicNaviAvailable(t->unk4) : ZoneMap_IsBlazeNaviAvailable(t->unk8))
                               ? 2
                               : 0);
    t->unk84 = 0;
    t->unk88 = 0;
    t->unk8c = 0;
    t->unk74 = 0;
    gTaskCurrent->func = ZoneMap_State_PlayTalkDemo;
}

static void ZoneMap_Action_ShowMsgBox11(void) {
    ZmState *w = ZS;
    w->unk74 = 0;
    w->map->map.unk5c = 1;
    gTaskCurrent->func = ZoneMap_State_WaitMsgBox11;
}
static void ZoneMap_Action_ShowMsgBox12(void) {
    ZmState *w = ZS;
    w->unk74 = 0;
    w->map->map.unk5c = 1;
    gTaskCurrent->func = ZoneMap_State_WaitMsgBox12;
}

/* ZoneMapFadeTask */
typedef struct { s32 timer, from, to, duration, delay, sub, main; } ZmFadeWork;
static void ZoneMapFadeTask_Main(void) {
    ZmFadeWork *w = TASK_WORK(ZmFadeWork);
    fx32 a, b;
    s32 v;
    if (w->timer < w->delay) {
        w->timer++;
        BRIGHT_SUB = (s16)w->from;
        BRIGHT_MAIN = BRIGHT_SUB;
        return;
    }
    a = w->from << 12;
    b = w->to << 12;
    a += a >= 0 ? 0x800 : -0x800;
    b += b >= 0 ? 0x800 : -0x800;
    v = (a + FX_Div(FX_Mul((w->timer - w->delay) << 12, b - a), w->duration << 12)) >> 12;
    if (v < -16)
        v = -16;
    if (v > 16)
        v = 16;
    if (w->sub != 0)
        BRIGHT_MAIN = (s16)v;
    if (w->main != 0)
        BRIGHT_SUB = (s16)v;
    w->timer++;
    if (w->timer - w->delay >= w->duration) {
        if (w->sub != 0)
            BRIGHT_MAIN = (s16)w->to;
        if (w->main != 0)
            BRIGHT_SUB = (s16)w->to;
        Task_Destroy(gTaskCurrent);
    }
}
void ZoneMap_CreateFadeTask(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
    ZmFadeWork *w;
    if (a < -16)
        a = -16;
    if (a > 16)
        a = 16;
    if (b < -16)
        b = -16;
    if (b > 16)
        b = 16;
    w = (ZmFadeWork *)Task_Create(ZoneMapFadeTask_Main, NULL, 0, 0x4000, 0x1000, sizeof(ZmFadeWork))->work;
    w->from = a;
    w->to = b;
    w->duration = c;
    w->delay = d;
    w->sub = e;
    w->main = f;
}

static void ZoneMap_Action_FadeOut(void) {
    ZS->unk74 = 0;
    ZoneMap_CreateFadeTask(0, 0x10, 0x10, 0, 1, 0);
    gTaskCurrent->func = ZoneMap_State_OpenActSelect;
}

static void ZoneMap_Action_FadeOutExit(void) {
    ZmState *t = ZS;
    t->unk74 = 0;
    gTaskCurrent->func = ZoneMap_State_FadeOutExit;
}

static void ZoneMap_Action_OpenPause(void) {
    ZoneMapPause_Create(FALSE);
    gTaskCurrent->func = ZoneMap_State_WaitPauseClosed;
}

/* story steps: the scripted sequence after a story stage */
static void ZoneMap_RunSonicStoryStep(void) {
    ZmState *w = ZS;
    switch (w->unk4) {
    case 0: w->unk94 = 1; ZoneMap_Action_PlayTalkDemo(); break;
    case 1: w->unk94 = 3; NaviMsg_StartIntro(); ZoneMap_Action_WaitTimer(0xf0); break;
    case 3: w->unk94 = 4; ZoneMap_Action_ShowMsgBox11(); break;
    case 4: w->unk94 = 5; ZoneMap_Action_ScrollToPathStart(0, 1, 2); break;
    case 5: w->unk90 = 1; gTaskCurrent->func = ZoneMap_State_SelectNode; break;
    case 6: w->unk94 = 7; ZoneMap_Action_PlayTalkDemo(); break;
    case 7: w->unk94 = 8; ZoneMap_Action_ScrollToPathStart(9, 2, 0xf); break;
    case 8: w->unk94 = 9; ZoneMap_Action_MoveToNode(0xf); break;
    case 9: w->unk94 = 10; ZoneMap_Action_PlayTalkDemo(); break;
    case 10: w->unk94 = 0xb; ZoneMap_Action_ScrollToPathStart(1, 2, 3); break;
    case 11: w->unk90 = 1; gTaskCurrent->func = ZoneMap_State_SelectNode; break;
    case 12: w->unk94 = 0xd; ZoneMap_Action_PlayTalkDemo(); break;
    case 13: w->unk94 = 0xe; ZoneMap_Action_ScrollToPathStart(10, 3, 0x10); break;
    case 14: w->unk94 = 0xf; ZoneMap_Action_MoveToNode(0x10); break;
    case 15: w->unk94 = 0x10; ZoneMap_Action_PlayTalkDemo(); break;
    case 16: w->unk94 = 0x11; ZoneMap_Action_ScrollToPathStart(2, 3, 6); break;
    case 17: w->unk90 = 1; gTaskCurrent->func = ZoneMap_State_SelectNode; break;
    case 18: w->unk94 = 0x13; ZoneMap_Action_ScrollToPathStart(3, 6, 7); break;
    case 19: w->unk90 = 1; gTaskCurrent->func = ZoneMap_State_SelectNode; break;
    case 20:
        w->map->nodes.pts[7].type = 2;   /* child->unk6f0 = 2 */
        w->unk94 = 0x15;
        ZoneMap_Action_ScrollToPathStart(4, 7, 8);
        break;
    case 21: w->unk94 = 0x16; ZoneMap_Action_MoveToNode(8); break;
    case 22:
        w->unk90 = 1;
        ZoneMap_SetNaviMessage(w->map->unk1cc4, 8);
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 23: w->unk94 = 0x18; ZoneMap_Action_ScrollToPathStart(5, 8, 9); break;
    case 24: w->unk90 = 1; gTaskCurrent->func = ZoneMap_State_SelectNode; break;
    case 25:
        w->map->nodes.pts[9].type = 2;   /* child->unk710 = 2 */
        w->unk94 = 0x1a;
        ZoneMap_Action_ScrollToPathStart(6, 9, 0xc);
        break;
    case 26: w->unk94 = 0x1b; ZoneMap_Action_MoveToNode(0xc); break;
    case 27:
        w->unk90 = 1;
        ZoneMap_SetNaviMessage(w->map->unk1cc4, 9);
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 28: w->unk94 = 0x1d; ZoneMap_Action_PlayTalkDemo(); break;
    case 29: w->unk94 = 0x1e; ZoneMap_Action_ScrollToPathStart(7, 0xc, 0xd); break;
    case 30: w->unk90 = 1; gTaskCurrent->func = ZoneMap_State_SelectNode; break;
    case 31: w->unk94 = 0x20; ZoneMap_Action_PlayTalkDemo(); break;
    case 32: w->unk94 = 0x21; ZoneMap_Action_ScrollToPathStart(8, 0xd, 0xe); break;
    case 33:
    case 34: w->unk90 = 1; gTaskCurrent->func = ZoneMap_State_SelectNode; break;
    }
}

/* ZoneMap_Unk_ov001_022e5de0 */
static void ZoneMap_OpenAliasNode(s32 p0, s32 p1) {
    ZmState *t0 = ZS;
    ZmNode *n = t0->map->nodes.pts;
    n[p0].type = 0;
    if (p1 != ZM_NONE)
        n[p1].type = 5;
    ZoneMap_Unk_ov001_022e21b8(t0);
    ZoneMap_Action_WaitTimer(0xa);
}

static void ZoneMap_RunBlazeStoryStep(void) {
    ZmState *w = ZS;
    switch (w->unk8) {
    case 0: w->unk98 = 1; ZoneMap_Action_WaitTimer(0x3c); break;
    case 1: w->unk98 = 2; ZoneMap_Action_PlayTalkDemo(); break;
    case 2: w->unk98 = 3; ZoneMap_Action_ScrollToPathStart(0xb, 0, 1); break;
    case 3: w->unk90 = 1; gTaskCurrent->func = ZoneMap_State_SelectNode; break;
    case 4: w->unk98 = 5; ZoneMap_Action_PlayTalkDemo(); break;
    case 5: w->unk98 = 6; ZoneMap_Action_ScrollToPathStart(0xc, 1, 0x10); break;
    case 6: w->unk98 = 7; ZoneMap_Action_MoveToNode(0x10); break;
    case 7: w->unk98 = 8; ZoneMap_Action_PlayTalkDemo(); break;
    case 8: w->unk98 = 10; NaviMsg_StartIntro(); ZoneMap_Action_WaitTimer(0xf0); break;
    case 10: w->unk98 = 0xb; ZoneMap_Action_ScrollToPathStart(0xd, 0x10, 2); break;
    case 11: w->unk90 = 1; gTaskCurrent->func = ZoneMap_State_SelectNode; break;
    case 12: w->unk98 = 0xd; ZoneMap_Action_ScrollToPathStart(0xe, 2, 3); break;
    case 13: w->unk90 = 1; gTaskCurrent->func = ZoneMap_State_SelectNode; break;
    case 14: w->unk98 = 0xf; ZoneMap_Action_ScrollToPathStart(0xf, 3, 4); break;
    case 15: w->unk90 = 1; gTaskCurrent->func = ZoneMap_State_SelectNode; break;
    case 16:
        w->unk98 = 0x11;
        w->map->nodes.pts[5].type = 5;   /* child->unk6c0 = 5 */
        w->map->nodes.pts[6].type = 1;   /* child->unk6d0 = 1 */
        w->unk14 = 5;
        ZoneMap_Action_ScrollToPathStart(0x10, 4, 6);
        break;
    case 17: w->unk90 = 1; gTaskCurrent->func = ZoneMap_State_SelectNode; break;
    case 18: w->unk98 = 0x13; ZoneMap_OpenAliasNode(5, 4); break;
    case 19: w->unk90 = 1; gTaskCurrent->func = ZoneMap_State_SelectNode; break;
    case 20:
        w->unk98 = 0x15;
        w->map->nodes.pts[6].type = 1;
        ZoneMap_Action_ScrollToPathStart(0x11, 5, 8);
        break;
    case 21: w->unk90 = 1; gTaskCurrent->func = ZoneMap_State_SelectNode; break;
    case 22: w->unk98 = 0x17; ZoneMap_Action_ScrollToPathStart(0x12, 8, 10); break;
    case 23: w->unk90 = 1; gTaskCurrent->func = ZoneMap_State_SelectNode; break;
    case 24:
        w->unk98 = 0x19;
        w->map->nodes.pts[11].type = 5;   /* child->unk720 = 5 */
        w->map->nodes.pts[12].type = 1;   /* child->unk730 = 1 */
        w->unk14 = 0xb;
        ZoneMap_Action_ScrollToPathStart(0x13, 10, 0xc);
        break;
    case 25: w->unk90 = 1; gTaskCurrent->func = ZoneMap_State_SelectNode; break;
    case 26: w->unk98 = 0x1b; ZoneMap_OpenAliasNode(0xb, 10); break;
    case 27: w->unk90 = 1; gTaskCurrent->func = ZoneMap_State_SelectNode; break;
    case 28:
        w->unk98 = 0x1d;
        w->map->nodes.pts[12].type = 1;
        ZoneMap_Action_ScrollToPathStart(0x14, 0xb, 0xd);
        break;
    case 29: w->unk90 = 1; gTaskCurrent->func = ZoneMap_State_SelectNode; break;
    case 30: w->unk98 = 0x1f; ZoneMap_Action_PlayTalkDemo(); break;
    case 31: w->unk98 = 0x20; ZoneMap_Action_ScrollToPathStart(0x15, 0xd, 0xe); break;
    case 32:
    case 33: w->unk90 = 1; gTaskCurrent->func = ZoneMap_State_SelectNode; break;
    }
}

static void ZoneMap_State_WaitTimer(void) {
    ZmState *t = ZS;
    ZoneMap_UpdateView(t);
    t->unk74++;
    if (t->unk74 >= (u32)t->unk80) {
        t->unk74 = 0;
        ZoneMap_SelectNextState();
    }
}

static void ZoneMap_State_WaitNaviMessage(void) {
    ZmState *w = ZS;
    ZoneMap_UpdateView(w);
    if (w->unk74 == 0) {
        if (w->unka0 != 0)
            NaviM_StartCounter();
        if (w->unk9c != 0)
            NaviM_StartEmeraldAppear();
    }
    w->unk74++;
    if (w->unk74 >= 30) {
        if (w->unka0 == 0 || !NaviM_IsCounting()) {
            w->unk9c = 0;
            w->unka0 = 0;
            w->unk74 = 0;
            ZoneMap_SelectNextState();
        }
    }
}

/* ZoneMap_State_PlayTalkDemo: with the talk_demo hooks (cutscene module) as the
 * original: SysSound_Release at 0xf, TalkDemo_CreateForZoneMap at 0x10 (the
 * talk id comes from the map step), wait for TalkDemo_IsActive() == 0, 15 frames,
 * then SysSound_Init + the map BGM. (The unk19ac BG fade and unk1998 are not
 * part of the port's map composition.) Without the hooks the scene is skipped
 * and only the 32-frame fade around it is kept. */
static void ZoneMap_State_PlayTalkDemo(void) {
    ZmState *w = ZS;
    if (g_menu_hooks.talk_demo_start && g_menu_hooks.talk_demo_active) {
        s32 d;
        if (w->unk74 == 0xf) {
            w->map->map.unk5c = 0;
            if (g_menu_hooks.stop_all)
                g_menu_hooks.stop_all();   /* SysSound_Release */
        }
        if (w->unk88 == 0 && w->unk74 >= 0x10)
            w->unk84 = 1;
        if (w->unk84 == 1 && w->unk88 == 0) {
            w->unk78 = 0;
            /* ZoneMap_Unk_ov001_022e1aac(unk4) / ZoneMap_SelectTalkDemo(unk8), TalkDemo_CreateForZoneMap */
            if (g_menu_hooks.talk_demo_start(w->mode, w->mode == 0 ? w->unk4 : w->unk8) != 0)
                w->unk88 = 2;   /* no data: as if it had ended at once */
            else
                w->unk88 = 1;
        }
        ZoneMap_UpdateView(w);
        w->unk74++;
        if (w->unk88 != 0 && (w->unk88 == 2 || !g_menu_hooks.talk_demo_active())) {
            if (w->unk78 == 0) {
                w->unk78 = w->unk74;
                w->unk8c = 1;
                w->map->map.unk5c = 1;
                if (w->mode == 0 ? ZoneMap_IsSonicNaviAvailable(w->unk4) : ZoneMap_IsBlazeNaviAvailable(w->unk8))
                    ZoneMap_SetNaviMessage(w->map->unk1cc4, 1);
            }
            d = w->unk74 - w->unk78;
            if (d >= 0xf) {
                w->unk74 = 0;
                w->unk78 = 0;
                SysSound_PlayBGM((w->mode == 0 ? (w->unk4 >= 0x1f && w->unk4 <= 0x21)
                                               : (w->unk8 >= 0x1e && w->unk8 <= 0x20))
                                     ? 5
                                     : 4);
                ZoneMap_SelectNextState();
            }
        }
        return;
    }
    if (w->unk74 == 0xf)
        w->map->map.unk5c = 0;
    ZoneMap_UpdateView(w);
    w->unk74++;
    if (w->unk74 >= 0x20) {
        w->unk74 = 0;
        w->unk78 = 0;
        w->map->map.unk5c = 1;
        SysSound_PlayBGM((w->mode == 0 ? (w->unk4 >= 0x1f && w->unk4 <= 0x21)
                                       : (w->unk8 >= 0x1e && w->unk8 <= 0x20))
                             ? 5
                             : 4);
        ZoneMap_SelectNextState();
    }
}

/* MsgBox 0x11 (map tutorial): closes at once */
static void ZoneMap_State_WaitMsgBox11(void) {
    ZmState *w = ZS;
    ZoneMap_UpdateView(w);
    w->unk74 = 0;
    w->map->map.unk5c = 1;
    ZoneMap_SelectNextState();
}

static void ZoneMap_Unk_ov001_022e606c(void);
static void ZoneMap_Unk_ov001_022e5f80(void);

/* MsgBox 0x12 ("go to this zone?"): the A press on the node is the answer
 * (MsgBox_GetResult() == 0) */
static void ZoneMap_State_WaitMsgBox12(void) {
    ZmState *w = ZS;
    ZoneMap_UpdateView(w);
    w->unk74 = 0;
    w->map->map.unk5c = 1;
    if (w->mode == 0)
        ZoneMap_Unk_ov001_022e606c();
    else
        ZoneMap_Unk_ov001_022e5f80();
}

static void ZoneMap_State_ScrollGeneric(s32 x, s32 y, BOOL to_path) {
    ZmState *w = ZS;
    fx32 dx, dy, speed, len;
    BOOL done;
    dx = (x << 12) - w->unk50;
    dy = (y << 12) - w->unk54;
    speed = w->unk58;
    len = FX_Sqrt(FX_Mul(dx, dx) + FX_Mul(dy, dy));
    if (len != 0) {
        dx = FX_Div(FX_Mul(dx, speed), len);
        dy = FX_Div(FX_Mul(dy, speed), len);
        w->unk50 += dx;
        w->unk54 += dy;
        w->unk6c = w->unk50 >> 12;
        w->unk70 = w->unk54 >> 12;
    } else {
        w->unk6c = x;
        w->unk70 = y;
        w->unk50 = w->unk6c << 12;
        w->unk54 = w->unk70 << 12;
    }
    done = TRUE;
    if (dx >= 0) {
        if (x <= w->unk6c)
            w->unk6c = x;
        else
            done = FALSE;
    } else {
        if (x >= w->unk6c)
            w->unk6c = x;
        else
            done = FALSE;
    }
    if (dy >= 0) {
        if (y <= w->unk70)
            w->unk70 = y;
        else
            done = FALSE;
    } else {
        if (y >= w->unk70)
            w->unk70 = y;
        else
            done = FALSE;
    }
    ZoneMap_UpdateView(w);
    w->unk74++;
    if (done) {
        if (to_path) {
            w->unk74 = 0;
            gTaskCurrent->func = ZoneMap_State_MoveAlongPath;
        } else {
            ZoneMap_SelectNextState();
        }
    }
}

static void ZoneMap_State_ScrollToPathStart(void) {
    ZmState *w = ZS;
    s32 x, y;
    ZoneMap_CalcClampedScroll(&w->map->map, w->map->nodes.pts[w->unk4c].x, w->map->nodes.pts[w->unk4c].y, &x, &y);
    ZoneMap_State_ScrollGeneric(x, y, TRUE);
}

static void ZoneMap_State_ScrollToPos(void) {
    ZmState *w = ZS;
    ZoneMap_State_ScrollGeneric(w->unk5c, w->unk60, FALSE);
}

static void ZoneMap_State_MoveAlongPath(void) {
    ZmState *w = ZS;
    ZoneMapWork *s = w->map;
    ZmPath *g = &s->nodes.grp[w->unk44];
    ZmNode *from = &s->nodes.pts[w->unk4c], *to = &s->nodes.pts[w->unk48];
    s32 i, n, t, base, x, y, x2, y2;
    for (i = 0; i < g->count; i++) {
        if (w->unk74 >= (u32)((i + 1) * 12)) {
            if (w->unk74 / 12 == (u32)(i + 1)) {
                if (g->state[i] != 1)
                    SysSound_PlaySE(0xe);
                g->state[i] = 1;
            } else {
                g->state[i] = 0;
            }
        } else {
            g->state[i] = 2;
        }
    }
    if (w->unk74 >= (u32)((g->count + 1) * 12)) {
        if (to->type != 0)
            SysSound_PlaySE(0xf);
        to->type = 0;
    } else {
        to->type = 3;
    }
    n = (g->count + 2) * 12;
    t = (s32)w->unk74;
    if (t > n)
        t = n;
    base = from->x;
    x = base + (to->x - base) * t / n;
    base = from->y;
    y = base + (to->y - base) * t / n;
    ZoneMap_CalcClampedScroll(&s->map, x, y, &x, &y);
    w->unk6c = x;
    w->unk70 = y;
    ZoneMap_UpdateView(w);
    w->unk74++;
    if (w->unk74 >= (u32)(n + 30)) {
        w->unk74 = 0;
        x2 = s->nodes.pts[w->unk14].x;
        y2 = s->nodes.pts[w->unk14].y;
        ZoneMap_CalcClampedScroll(&s->map, x2, y2, &x2, &y2);
        ZoneMap_Action_ScrollToPos(x2, y2, 0x3000);
        ZoneMap_SaveProgressOnPathArrival(w);
    }
}

/* ZoneMap_State_MoveToNode (asm_fallback, translated from the listing):
 * walk the path points (9 frames per segment), turn the character towards
 * the next segment, scroll with it, footstep SE every 30 (Sonic) / 20
 * (Blaze) frames. */
static void path_point(ZmPath *g, const ZmNode *p0, const ZmNode *pn, s32 i, s32 *x, s32 *y) {
    if (i == 0) {
        *x = p0->x;
        *y = p0->y;
    } else if (i == g->count + 1) {
        *x = pn->x;
        *y = pn->y;
    } else {
        *x = g->pos[i - 1].x;
        *y = g->pos[i - 1].y;
    }
}

static void ZoneMap_State_MoveToNode(void) {
    ZmState *w = ZS;
    ZoneMapWork *m = w->map;
    ZmPath *g = &m->nodes.grp[w->unk1c < 22 ? w->unk1c : 0];
    ZmNode *nodes = m->nodes.pts, *p0, *pn, *dst;
    BOOL rev = w->unk14 >= w->unk18;
    s32 seg, i0, i1, i2, ax, ay, bx, by, cx, cy, frac;
    if (w->unk1c == 0xd)
        rev = !rev;
    if (!rev) {
        p0 = &nodes[w->unk14];
        pn = &nodes[w->unk18];
        seg = (s32)(w->unk74 / 9);
        i0 = seg;
        i1 = i0 + 1;
        i2 = i1 + 1;
    } else {
        pn = &nodes[w->unk14];
        p0 = &nodes[w->unk18];
        seg = (s32)(w->unk74 / 9);
        i0 = g->count + 1 - seg;
        i1 = i0 - 1;
        i2 = i1 - 1;
    }
    dst = &nodes[w->unk18];
    path_point(g, p0, pn, i0, &ax, &ay);
    path_point(g, p0, pn, i1, &bx, &by);
    if (i2 == -1 || i2 == g->count + 2) {
        cx = 2 * bx - ax;
        cy = 2 * by - ay;
    } else {
        path_point(g, p0, pn, i2, &cx, &cy);
    }
    frac = (s32)(w->unk74 % 9);
    w->unk64 = (ax << 12) + ((bx - ax) * frac << 12) / 9;
    w->unk68 = (ay << 12) + ((by - ay) * frac << 12) / 9;
    /* ZoneMap_GetDirectionAngle(A,B) -> (B,C) blend: character model rotation (no model here) */
    ZoneMap_CalcClampedScroll(&m->map, w->unk64 >> 12, w->unk68 >> 12, &w->unk6c, &w->unk70);
    ZoneMap_UpdateView(w);
    if (w->mode == 0) {
        if (w->unk7c % 30 == 0)
            SysSound_PlaySE(0x10);
    } else if (w->unk7c % 20 == 0) {
        SysSound_PlaySE(0x11);
    }
    w->unk74++;
    w->unk7c++;
    if (w->unk74 >= (u32)((g->count + 1) * 9)) {
        w->unk74 = 0;
        w->unk14 = w->unk18;
        w->unk1c = 0x17;
        w->unk18 = ZM_NONE;
        w->unk64 = nodes[w->unk14].x << 12;
        w->unk68 = nodes[w->unk14].y << 12;
        ZoneMap_CalcClampedScroll(&m->map, dst->x, dst->y, &w->unk6c, &w->unk70);
        /* navi hint for the node (unk90 && no queued move): navigator panel, not here */
        ZoneMap_SelectNextState();
    }
}

/* ZoneMap_GetNextNodeFromPad (gPad.cont = held keys) */
static s32 ZoneMap_GetNextNodeFromPad(ZmState *p) {
    s32 dir, next, i, cur = p->unk14;
    ZmNode *nodes = p->map->nodes.pts;
    u32 t;
    if (cur == ZM_NONE)
        return ZM_NONE;
    if (gPad.cur & PAD_LEFT)
        dir = 0;
    else if (gPad.cur & PAD_UP)
        dir = 1;
    else if (gPad.cur & PAD_RIGHT)
        dir = 2;
    else if (gPad.cur & PAD_DOWN)
        dir = 3;
    else
        return ZM_NONE;
    next = g_mn_data.zm_adj[p->mode][cur][dir];
    if (next == ZM_NONE || next < 0 || next > 16)
        return ZM_NONE;
    if (nodes[next].type == 5)
        next++;
    t = (u32)nodes[next].type;
    if (t != 0 && t != 2 && t != 1)
        return ZM_NONE;
    if (t == 2) {
        for (i = 0; i < 4; i++) {
            s32 v = g_mn_data.zm_adj[p->mode][next][i];
            if (v != ZM_NONE && v != cur) {
                p->unk20 = 1;
                p->unk24[0] = v;
                break;
            }
        }
    }
    return next;
}

/* ZoneMap_GetNextNodeFromTouch: only its queue part is reachable without a
 * touch screen (pass-through nodes queue the next move) */
static s32 ZoneMap_GetNextNodeFromQueue(ZmState *p) {
    if (p->unk20 > 0) {
        p->unk20--;
        return p->unk24[p->unk20];
    }
    return ZM_NONE;
}

/* ZoneMap_Unk_ov001_022e606c (Sonic) / _022e5f80 (Blaze): what a node does */
static void ZoneMap_Unk_ov001_022e606c(void) {
    ZmState *w = ZS;
    switch (w->unk14) {
    case 1: case 2: case 3: case 6: case 8: case 12: case 13: case 14:
        w->unk74 = 0;
        gTaskCurrent->func = ZoneMap_State_StartStage;
        return;
    case 15:
        ZoneMap_SetNaviMessage(w->map->unk1cc4, 0xd);
        return;
    case 16:
        ZoneMap_SetNaviMessage(w->map->unk1cc4, 0xf);
        return;
    case 7:
        if (w->unk4 == 0x13) {
            w->unk74 = 0;
            w->unk90 = 0;
            w->unk94 = 0x14;
            ZoneMap_Action_PlayTalkDemo();
        }
        return;
    case 9:
        if (w->unk4 == 0x18) {
            w->unk74 = 0;
            w->unk90 = 0;
            w->unk94 = 0x19;
            ZoneMap_Action_PlayTalkDemo();
        }
        return;
    }
}

static void ZoneMap_Unk_ov001_022e5f80(void) {
    ZmState *w = ZS;
    switch (w->unk14) {
    case 1: case 2: case 3: case 6: case 8: case 12: case 13: case 14:
        w->unk74 = 0;
        gTaskCurrent->func = ZoneMap_State_StartStage;
        return;
    case 16:
        ZoneMap_SetNaviMessage(w->map->unk1cc4, 0xf);
        return;
    case 4:
        if (w->unk8 == 0xf) {
            w->unk74 = 0;
            w->unk90 = 0;
            w->unk98 = 0x10;
            ZoneMap_Action_PlayTalkDemo();
        }
        return;
    case 5:
        if (w->unk8 == 0x13) {
            w->unk74 = 0;
            w->unk90 = 0;
            w->unk98 = 0x14;
            ZoneMap_Action_PlayTalkDemo();
        }
        return;
    case 10:
        if (w->unk8 == 0x17) {
            w->unk74 = 0;
            w->unk90 = 0;
            w->unk98 = 0x18;
            ZoneMap_Action_PlayTalkDemo();
        }
        return;
    case 11:
        if (w->unk8 == 0x1b) {
            w->unk74 = 0;
            w->unk90 = 0;
            w->unk98 = 0x1c;
            ZoneMap_Action_PlayTalkDemo();
        }
        return;
    }
}

static s32 node_zone_index(s32 node) {
    switch (node) {
    case 1: return 0;
    case 2: return 1;
    case 3: return 2;
    case 6: return 3;
    case 8: return 4;
    case 12: return 5;
    case 13: return 6;
    case 14: return 7;
    }
    return -1;
}

/* ZoneMap_ConfirmNode (asm_fallback): zone node -> act select when the zone
 * is cleared (selectable), else MsgBox 0x12 then the story stage */
static void ZoneMap_ConfirmNode(void) {
    ZmState *w = ZS;
    s32 idx = node_zone_index(w->unk14);
    BOOL ok;
    if (idx < 0) {
        if (w->mode == 0)
            ZoneMap_Unk_ov001_022e606c();
        else if (w->mode == 1)
            ZoneMap_Unk_ov001_022e5f80();
        return;
    }
    ok = w->mode == 0 ? ZoneMap_IsSonicZoneSelectable(idx) : ZoneMap_IsBlazeZoneSelectable(idx);
    if (ok) {
        ZoneMap_Action_FadeOut();
        SysSound_PlaySE(0xd);
    } else {
        ZoneMap_Action_ShowMsgBox12();
        SysSound_PlaySE(0xb);
    }
}

static void ZoneMap_State_SelectNode(void) {
    ZmState *w = ZS;
    s32 n;
    g_zm_node = w->unk14;
    g_zm_select_ticks++;
    ZoneMap_UpdateView(w);
    if ((gPad.trg & PAD_X) && w->unk74 != 0) {   /* ZoneMap_IsCornerButtonOrXPressed */
        w->unk74 = 0;
        gTaskCurrent->func = ZoneMap_State_ScrollView;
        return;
    }
    n = ZoneMap_GetNextNodeFromQueue(w);
    if (n == ZM_NONE)
        n = ZoneMap_GetNextNodeFromPad(w);
    if (n != ZM_NONE && n != w->unk14) {
        w->unk74 = 0;
        w->unk18 = n;
        w->unk1c = ZoneMap_GetPathIndex(w->mode, w->unk14, w->unk18);
        gTaskCurrent->func = ZoneMap_State_MoveToNode;
        return;
    }
    if (w->unk74 == 0)
        ZoneMap_RestartButtonAnim(w, 5);
    ZoneMap_DrawButton(w, 5);
    if ((gPad.trg & PAD_A) || n == w->unk14) {
        ZoneMap_ConfirmNode();
        return;
    }
    if (gPad.trg & PAD_START) {
        ZoneMap_Action_OpenPause();
        return;
    }
    w->unk74++;
    w->unk7c = 0;
}

/* X: free scroll with the D-pad, B (or the corner button) returns */
static void ZoneMap_State_ScrollView(void) {
    ZmState *w = ZS;
    ZmMapBg *sub = &w->map->map;
    BOOL done = FALSE;
    s32 x, y;
    u32 dir = 0;
    ZoneMap_SetScrollArrowMode(&w->map->arrows, 1);
    ZoneMap_SetScrollArrowBlink(&w->map->arrows, 0);
    if (w->unk74 == 0)
        ZoneMap_RestartButtonAnim(w, 4);
    ZoneMap_DrawButton(w, 4);
    ZoneMap_UpdateScrollArrows(w);
    ZoneMap_UpdateView(w);
    if (gPad.cur & PAD_LEFT)
        dir |= 1;
    else if (gPad.cur & PAD_RIGHT)
        dir |= 4;
    if (gPad.cur & PAD_UP)
        dir |= 2;
    else if (gPad.cur & PAD_DOWN)
        dir |= 8;
    if (dir != 0) {
        s32 maxx = sub->w - 0x108, maxy = sub->h - 0xc0;
        if (dir & 1) {
            w->unk6c -= 2;
            if (w->unk6c < 8)
                w->unk6c = 8;
        } else if (dir & 4) {
            w->unk6c += 2;
            if (w->unk6c >= maxx)
                w->unk6c = maxx;
        }
        if (dir & 2) {
            w->unk70 -= 2;
            if (w->unk70 < 0)
                w->unk70 = 0;
        } else if (dir & 8) {
            w->unk70 += 2;
            if (w->unk70 >= maxy)
                w->unk70 = maxy;
        }
    } else if (gPad.trg & PAD_B) {   /* ZoneMap_IsCornerButtonOrBPressed */
        done = TRUE;
    }
    w->unk74++;
    if (done) {
        ZoneMap_SetScrollArrowMode(&w->map->arrows, 0);
        ZoneMap_SetScrollArrowBlink(&w->map->arrows, 0);
        ZoneMap_CalcClampedScroll(sub, w->map->nodes.pts[w->unk14].x, w->map->nodes.pts[w->unk14].y, &x, &y);
        ZoneMap_Action_ScrollToPos(x, y, 0x5000);
    }
}

/* ZoneMap_State_Unk_ov001_022e7114: start the story stage of the node */
static void ZoneMap_State_StartStage(void) {
    ZmState *w = ZS;
    s32 idx = node_zone_index(w->unk14);
    if (idx < 0)
        idx = 0;
    ZoneMap_UpdateView(w);
    w->unk74++;
    if (w->unk74 >= 0x96) {
        BRIGHT_MAIN = (s16)((w->unk74 - 0x96) >> 1);
        BRIGHT_SUB = (s16)((w->unk74 - 0x96) >> 1);
        if (idx == 7) {
            BRIGHT_MAIN = -BRIGHT_MAIN;
            BRIGHT_SUB = -BRIGHT_SUB;
        }
    } else {
        BRIGHT_MAIN = 0;
        BRIGHT_SUB = 0;
    }
    if (w->unk74 >= 0xb6) {
        w->unk74 = 0;
        w->map->unk1cc8 = w->mode;
        w->map->unk1ccc = idx;
        w->map->unk1cd0 = 0;
        w->map->unk0 = 1;
    }
}

/* ZoneMap_State_Unk_ov001_022e7428: after the fade, open the act select */
static void ZoneMap_State_OpenActSelect(void) {
    ZmState *w = ZS;
    s32 idx, n;
    ZoneMap_UpdateView(w);
    w->unk74++;
    if (w->unk74 >= 0x10) {
        w->unk74 = 0;
        idx = node_zone_index(w->unk14);
        if (idx < 0)
            idx = 0;
        n = w->mode == 0 ? ZoneMap_GetSonicMaxSelectableZone() : ZoneMap_GetBlazeMaxSelectableZone();
        if (w->mode == 0) {
            if (!ZoneMap_IsSonicNaviAvailable(w->unk4))
                w->flags |= 2;
        } else if (!ZoneMap_IsBlazeNaviAvailable(w->unk8)) {
            w->flags |= 2;
        }
        if (idx > n)
            idx = n;
        ZoneActSelect_Create(idx, n, (s32)w->flags);
        w->flags = 0;
        gTaskCurrent->func = ZoneMap_State_WaitActSelect;
    }
}

/* ZoneMap_State_Unk_ov001_022e736c */
static void ZoneMap_State_WaitActSelect(void) {
    ZmState *w = ZS;
    s32 zone, act;
    w->unk74++;
    if (ZoneActSelect_IsFinished()) {
        zone = ZoneActSelect_GetSelectedZone();
        act = ZoneActSelect_GetResult();
        if (zone < 0 || act < 0) {
            /* ZoneMap_RestoreGraphics */
            Background_Show(0, 3, w->map->bg);
            ZoneMap_Compose();
            g_mn_scene = "ZoneMap";
            ZoneMap_CreateFadeTask(0x10, 0, 8, 0, 1, 0);
            ZoneMap_Action_WaitTimer(8);
            return;
        }
        w->map->unk1cc8 = w->mode;
        w->map->unk1ccc = zone;
        w->map->unk1cd0 = act;
        w->map->unk0 = 1;
    }
}

static void ZoneMap_State_WaitPauseClosed(void) {
    ZmState *w = ZS;
    ZoneMap_UpdateView(w);
    w->unk74++;
    if (ZoneMapPause_IsClosed()) {
        if (g_zm_pause_result != 0) {   /* ZoneMap_Unk_ov001_022e955c */
            ZoneMap_SelectNextState();
            return;
        }
        ZoneMap_Action_FadeOutExit();
    }
}

/* back to the title screen (pause -> quit) */
static void ZoneMap_State_FadeOutExit(void) {
    ZmState *w = ZS;
    if (w->unk74 == 0xa4)
        ZoneMap_CreateFadeTask(0, 0x10, 0x10, 0, 1, 1);
    ZoneMap_UpdateView(w);
    w->unk74++;
    if (w->unk74 >= 0xb4) {
        w->map->unk1cc8 = 3;
        w->map->unk1ccc = -1;
        w->map->unk1cd0 = -1;
        w->map->unk0 = 1;
    }
}

/* ZoneMap_Unk_ov001_022e8090: the state task and the initial view */
static ZmState *ZoneMap_CreateState(ZoneMapWork *map, s32 alt, s32 mode) {
    ZmState *w = (ZmState *)Task_Create(mode != 0 ? ZoneMap_State_OpenActSelect : ZoneMap_State_WaitTimer, NULL, 0,
                                        0x2000, 0x1000, sizeof(ZmState))->work;
    w->mode = ZoneMap_IsBlaze();
    if (w->mode == 0) {
        w->unk4 = ZoneMap_GetSonicStoryStep();
        w->unk8 = 0;
    } else {
        w->unk4 = 0;
        w->unk8 = ZoneMap_GetBlazeStoryStep();
    }
    w->unkc = ZoneMap_GetMapBgIndex();
    if (mode != 0 || (alt != 0 && gGameState.unk1C == 7))
        w->unk14 = ZoneMap_GetNodeForCurrentZone();
    else
        w->unk14 = ZoneMap_GetNodeForStoryStep();
    w->unk18 = ZM_NONE;
    w->unk1c = 0x17;
    w->unk20 = 0;
    w->unk44 = 0x17;
    w->unk48 = ZM_NONE;
    w->unk74 = 0;
    w->unk7c = 0;
    w->unk80 = 8;
    w->unk90 = 0;
    w->unk94 = 0x24;
    w->unk98 = 0x23;
    w->map = map;
    w->flags = 0;
    if (mode != 0) {
        w->flags |= 1;
        w->flags |= 4;
    }
    ZoneMap_RevealNodesForStory(w);
    w->unk64 = map->nodes.pts[w->unk14].x << 12;
    w->unk68 = map->nodes.pts[w->unk14].y << 12;
    ZoneMap_CalcClampedScroll(&map->map, w->unk64 >> 12, w->unk68 >> 12, &w->unk6c, &w->unk70);
    map->map.idx = w->unkc;
    if (mode == 0)
        ZoneMap_CreateFadeTask(0x10, 0, 8, 0, 1, 1);
    else
        w->unk74 = 0x10;
    /* navi greeting message ids: navigator panel only */
    ZoneMap_CheckNewEmerald(w);
    if (mode != 0) {
        if (w->unk9c != 0)
            w->flags |= 8;
        if (w->unka0 != 0)
            w->flags |= 0x10;
        w->unk9c = 0;
        w->unka0 = 0;
    }
    if (w->mode == 0 && (w->unk4 == 0x16 || w->unk4 == 0x1b))
        w->unk90 = 1;
    return w;
}

/* --------------------------------------------------------- the map task */
static void ZoneMap_Destructor(void) {
    ZoneMapWork *w = TASK_WORK(ZoneMapWork);
    NaviM_Destroy();
    NaviMsg_Destroy();
    NaviM_Release();
    NaviMsg_CloseFiles();
    mn_display_reset();
    mn_bg_free(w->bg);
    mn_res_free(w->icons);
    mn_res_free(w->arrows_res);
    mn_res_free(w->titl);
    free(w->ama);
    if (s_zm == w)
        s_zm = NULL;
}

static void ZoneMap_Main(void) {
    ZoneMapWork *w = TASK_WORK(ZoneMapWork);
    s32 t1, t2, t3;
    if (strcmp(g_mn_scene, "ZoneMap") == 0)
        ZoneMap_Compose();   /* the navigator pieces come and go with the DS planes */
    if (w->unk0 == 1) {
        t1 = w->unk1cc8;
        t2 = w->unk1ccc;
        t3 = w->unk1cd0;
        (void)t1;
        Task_DestroyByTag(0x1000);
        if (t2 < 0) {
            TitleScreen_Start();   /* SceneChange_ToTitleScreen */
            return;
        }
        /* ZoneMap_StartStoryStage */
        if (t2 >= 7)
            t3 = 2;
        Game_StartStoryStage(t2, t3);
    }
}

/* The map (main engine, the touch screen) fills the frame at 2.5x. The other
 * screen holds the navigator: its pieces are placed over the map (mn_navi.c). */
static void ZoneMap_Compose(void) {
    mn_comp_reset(0);
    mn_comp_full(0);
    mn_navi_compose();
}

static void ZoneMap_InitDisplay(void) {
    mn_display_reset();
    Background_SetPriority(0, 3, 3);
    Background_SetPriority(0, 2, 0);
    Background_SetPriority(0, 1, 1);
    ZoneMap_Compose();
    g_mn_scene = "ZoneMap";
}

static void ZoneMap_Start(s32 a, s32 b) {
    ZoneMapWork *w;
    ZmState *r;
    BOOL flag;
    int i;
    StoryProgress_RestoreFromSave();
    w = (ZoneMapWork *)Task_Create(ZoneMap_Main, ZoneMap_Destructor, 0, 0x1000, 0x1000, sizeof(ZoneMapWork))->work;
    s_zm = w;
    w->unk0 = 0;
    w->unk1cc8 = 3;
    w->unk1ccc = -1;
    w->unk1cd0 = -1;
    /* ZoneMap_LoadAssets: gZoneMapFileTable[lang] */
    w->bg = mn_bg_load("bg2/zmap/zmap_map_bg.bbg");
    w->icons = mn_res_load("act2/zmap/zmap_map_icon.bac");
    w->arrows_res = mn_res_load("act2/zmap/zmap_scroll_arrow.bac");
    w->ama = mn_ama_load("ama/zmap_map_icon.ama");
    w->titl = mn_res_load("act/mnu_titl.bac");
    ZoneMap_InitDisplay();
    /* ZoneMap_InitWork */
    memset(&w->map, 0, sizeof(w->map));
    w->map.x = 8;
    w->map.w = w->bg ? w->bg->w : 648;
    w->map.h = w->bg ? w->bg->h : 240;
    w->map.unk58 = 1;
    w->map.unk5c = 1;
    Background_Show(0, 3, w->bg);
    Background_SetSize(0, 3, 1024, 256);
    ZoneMap_InitNodeLayer(&w->nodes, &w->map, w->ama, w->icons);
    ZoneMap_InitScrollArrows(&w->arrows, w->arrows_res, w->titl);
    w->unk1cc4 = ZoneMap_InitNavi(gGameState.unk0 == 1, a);
    r = ZoneMap_CreateState(w, a, b);
    NaviM_Create();
    for (i = 0; i < 12; i++) {
        gPad.delay[i] = 0x14;
        gPad.interval[i] = 1;
    }
    flag = FALSE;
    if (gGameState.unk0 == 0) {
        if (r->unk4 >= 0x1f)
            flag = TRUE;
    } else if (r->unk8 >= 0x1e) {
        flag = TRUE;
    }
    if (gSaveData.unk8 & 0x1000)
        flag = FALSE;
    SysSound_PlayBGM(flag ? 5 : 4);
    gSaveDataBusy = 0;
}

void ZoneMap_StartDefault(void) { ZoneMap_Start(0, 0); }
void ZoneMap_StartFromStage(void) { ZoneMap_Start(1, 1); }
void ZoneMap_StartFromStageWithFade(void) { ZoneMap_Start(1, 0); }

/* ============================================================ ZoneMapPause */
typedef struct {
    s32 mode;       /* 1..4 page A (open/select/confirm/close), 5..8 page B */
    s32 unk4;       /* timer */
    s32 unk8;       /* frames since the selection changed */
    s32 unkc;       /* slide position */
    s32 unk10;      /* selection */
    MnSpr spr[4];   /* 0 page title, 1/2 items, 3 cursor */
    s32 anim[4];
    MnBgImg *frame;
    MnRes *res[2];
    MnTask *task;
} ZmPauseWork;
static ZmPauseWork *gZoneMapPauseWork;

static void ZoneMapPause_State_Open(void);
static void ZoneMapPause_State_Select(void);
static void ZoneMapPause_State_ConfirmFlash(void);
static void ZoneMapPause_State_Close(void);

BOOL ZoneMapPause_IsClosed(void) { return gZoneMapPauseWork == NULL; }

static void ZoneMapPause_DrawTitle(ZmPauseWork *w) {   /* ZoneMapPause_Unk_ov001_022e8b5c */
    s32 anim, y;
    if (w->mode >= 1 && w->mode <= 4)
        anim = 0;
    else if (w->mode >= 5 && w->mode <= 8)
        anim = 1;
    else
        return;
    y = -w->unkc;
    if (y >= -16) {
        if (anim != w->anim[0]) {
            AnimatorSprite_SetAnimation(&w->spr[0], (u16)anim);
            w->anim[0] = anim;
        }
        w->spr[0].x = 0;
        w->spr[0].y = (s16)y;
        AnimatorSprite_ProcessAnimation(&w->spr[0], 0, 0);
        AnimatorSprite_Draw(&w->spr[0]);
    }
}

static void ZoneMapPause_DrawItem(ZmPauseWork *p, u32 sel) {
    s32 a, negx, y = 0, k, base = 0, idx = 1;
    BOOL flag = FALSE;
    k = (p->mode == 3 || p->mode == 7) ? 2 : 1;
    switch (sel) {
    case 0:
        base = 2;
        if (p->unk10 == 0) {
            flag = TRUE;
            base += k;
        }
        idx = 1;
        y = 0x18;
        break;
    case 1:
        base = 5;
        if (p->unk10 == 1) {
            flag = TRUE;
            base += k;
        }
        idx = 2;
        y = 0x2e;
        break;
    case 2:
        base = 8;
        if (p->unk10 == 0) {
            flag = TRUE;
            base += k;
        }
        idx = 1;
        y = 0x18;
        break;
    case 3:
        base = 11;
        if (p->unk10 == 1) {
            flag = TRUE;
            base += k;
        }
        idx = 2;
        y = 0x2e;
        break;
    }
    negx = -p->unkc;
    y -= p->unkc;
    if (flag) {
        a = p->unk8 < 8 ? 1 : 0;
        if (a != p->anim[3]) {
            AnimatorSprite_SetAnimation(&p->spr[3], (u16)a);
            p->anim[3] = a;
        }
        p->spr[3].x = 8;
        p->spr[3].y = (s16)y;
        AnimatorSprite_ProcessAnimation(&p->spr[3], 0, 0);
        AnimatorSprite_Draw(&p->spr[3]);
    }
    if (base != p->anim[idx]) {
        AnimatorSprite_SetAnimation(&p->spr[idx], (u16)base);
        p->anim[idx] = base;
    }
    p->spr[idx].x = 0;
    p->spr[idx].y = (s16)negx;
    if (k == 2)
        p->spr[idx].flags |= SPR_LOOP;
    else
        p->spr[idx].flags &= ~(u32)SPR_LOOP;
    AnimatorSprite_ProcessAnimation(&p->spr[idx], 0, 0);
    AnimatorSprite_Draw(&p->spr[idx]);
}

static void pause_draw_page(ZmPauseWork *w, int page) {
    ZoneMapPause_DrawTitle(w);
    ZoneMapPause_DrawItem(w, page ? 2 : 0);
    ZoneMapPause_DrawItem(w, page ? 3 : 1);
}

static void pause_set(ZmPauseWork *w, s32 mode, void (*fn)(void)) {
    w->mode = mode;
    w->unk4 = 0;
    gDisplayMain[2].x = 0;
    gDisplayMain[2].y = 0;
    w->unkc = 0;
    w->task->func = fn;
}

static void ZoneMapPause_Action_Open(ZmPauseWork *w, int page) {
    w->mode = page ? 5 : 1;
    w->unk4 = 0;
    w->unk10 = 0;
    gDisplayMain[2].x = 0;
    gDisplayMain[2].y = 0x48;
    w->unkc = 0x48;
    if (!page)
        SysSound_PlaySE(0xa);
    w->task->func = ZoneMapPause_State_Open;
}

static void ZoneMapPause_Finish(ZmPauseWork *w) {
    g_zm_pause_result = (w->mode == 8 && w->unk10 == 0) ? 0 : 1;
    Task_Destroy(w->task);
}

static void ZoneMapPause_State_Open(void) {
    ZmPauseWork *w = TASK_WORK(ZmPauseWork);
    s32 b = 0x48 - w->unk4 * 6;
    if (b < 0)
        b = 0;
    w->unkc = b;
    gDisplayMain[2].y = (u16)b;
    pause_draw_page(w, w->mode >= 5);
    w->unk4++;
    w->unk8++;
    if (b <= 0) {
        pause_set(w, w->mode == 1 ? 2 : 6, ZoneMapPause_State_Select);
        w->unk10 = 0;
    }
}

static void ZoneMapPause_State_Select(void) {
    ZmPauseWork *w = TASK_WORK(ZmPauseWork);
    BOOL ok = FALSE, cancel = FALSE;
    pause_draw_page(w, w->mode == 6);
    w->unk4++;
    w->unk8++;
    if (gPad.trg & PAD_UP) {
        if (w->unk10 != 0) {
            w->unk10 = 0;
            w->unk8 = 0;
            SysSound_PlaySE(0x13);
        }
    } else if (gPad.trg & PAD_DOWN) {
        if (w->unk10 != 1) {
            w->unk10 = 1;
            w->unk8 = 0;
            SysSound_PlaySE(0x13);
        }
    }
    if (gPad.trg & PAD_A) {
        ok = TRUE;
    } else if (gPad.trg & PAD_B) {
        w->unk10 = -1;
        cancel = TRUE;
    } else if (gPad.trg & PAD_START) {
        if (w->mode == 2) {
            w->unk10 = -1;
            cancel = TRUE;
        }
    }
    if (cancel) {
        pause_set(w, w->mode == 2 ? 4 : 8, ZoneMapPause_State_Close);
        SysSound_PlaySE(0xc);
        return;
    }
    if (ok) {
        pause_set(w, w->mode == 2 ? 3 : 7, ZoneMapPause_State_ConfirmFlash);
        SysSound_PlaySE(0xb);
    }
}

static void ZoneMapPause_State_ConfirmFlash(void) {
    ZmPauseWork *w = TASK_WORK(ZmPauseWork);
    pause_draw_page(w, w->mode == 7);
    w->unk4++;
    w->unk8++;
    if (w->unk4 >= 0x18)
        pause_set(w, w->mode == 3 ? 4 : 8, ZoneMapPause_State_Close);
}

static void ZoneMapPause_State_Close(void) {
    ZmPauseWork *w = TASK_WORK(ZmPauseWork);
    s32 v = w->unk4 * 6;
    if (v > 0x48)
        v = 0x48;
    w->unkc = v;
    gDisplayMain[2].y = (u16)v;
    pause_draw_page(w, w->mode == 8);
    w->unk4++;
    w->unk8++;
    if (v >= 0x48) {
        if (w->mode == 4 && w->unk10 == 0)
            ZoneMapPause_Action_Open(w, 1);
        else
            ZoneMapPause_Finish(w);
    }
}

static void ZoneMapPause_Destructor(void) {
    ZmPauseWork *w = TASK_WORK(ZmPauseWork);
    Background_Hide(0, 2);
    mn_bg_free(w->frame);
    mn_res_free(w->res[0]);
    mn_res_free(w->res[1]);
    if (gZoneMapPauseWork == w)
        gZoneMapPauseWork = NULL;
}

static const char *const s_pause_lang[6] = { "act2/zmap/zmap_pause_jpn.bac", "act2/zmap/zmap_pause_eng.bac",
                                             "act2/zmap/zmap_pause_fra.bac", "act2/zmap/zmap_pause_deu.bac",
                                             "act2/zmap/zmap_pause_ita.bac", "act2/zmap/zmap_pause_spa.bac" };

static void ZoneMapPause_Create(BOOL flag) {
    MnTask *t;
    ZmPauseWork *w;
    int i;
    if (gZoneMapPauseWork) {   /* ZoneMapPause_Destroy */
        Task_Destroy(gZoneMapPauseWork->task);
        gZoneMapPauseWork = NULL;
        g_zm_pause_result = 3;
    }
    t = Task_Create(ZoneMapPause_State_Open, ZoneMapPause_Destructor, 0, 0x8000, 0x1000, sizeof(ZmPauseWork));
    w = (ZmPauseWork *)t->work;
    gZoneMapPauseWork = w;
    w->task = t;
    /* gZoneMapPauseFileTable[lang] */
    w->frame = mn_bg_load("bg2/zmap/zmap_pause_frame.bbg");
    w->res[0] = mn_res_load("act2/zmap/zmap_pause.bac");
    w->res[1] = mn_res_load(s_pause_lang[mn_lang()]);
    Background_Show(0, 2, w->frame);
    AnimatorSprite_Init(&w->spr[0], w->res[1], 0, 0x60, 0);
    AnimatorSprite_Init(&w->spr[1], w->res[1], 2, 0x60, 0);
    AnimatorSprite_Init(&w->spr[2], w->res[1], 5, 0x60, 0);
    AnimatorSprite_Init(&w->spr[3], w->res[0], 0, 0x60, 0);
    for (i = 0; i < 4; i++) {
        w->spr[i].flags &= ~0x60000u;
        w->anim[i] = -1;
    }
    ZoneMapPause_Action_Open(w, 0);
    if (flag)
        ZoneMapPause_ShowNaviMessage();
}

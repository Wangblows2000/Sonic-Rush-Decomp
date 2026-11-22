/*
 * TalkDemo: the story dialogue scenes (src/game/Cutscenes/TalkDemo/).
 *
 * The DS runs five tasks: TalkDemo (main: script load, input, skip, close),
 * TalkDemoScene (the page interpreter: portrait on the affine BG2, the 22x10
 * text box on BG1 with the 16x7 message written into it, the motions, the
 * page sounds), TalkDemoWindow (the coloured band: window 0/1 + backdrop
 * colours changed by V-count alarms), TalkDemoDim (blend fade of the scene
 * behind) and TalkDemoRocks (rock page sprites). They are kept as state
 * machines with the original names, run in the DS task priority order
 * (main 0xeffd, then scene / window / dim / rocks 0xeffe, then the raster
 * and deferred-call tasks 0xf000..0xf00a).
 *
 * Display: the scene is the main engine (one DS screen); it is drawn over the
 * caller's scene at x2.5, i.e. the full 640x480 frame (single-screen rule:
 * the talk is a main-screen composition on the DS too, the sub screen keeps
 * the stage / map, which is the scene under the overlay here).
 * Hardware effects are rebuilt from their register values: the window band
 * rows and colours from the V-alarm limits, the dim from BLDY, BG1 from its
 * scroll offsets (512x256 wrap), BG2 from its affine parameters.
 */
#include <stdio.h>
#include <stdlib.h>
#include "cs_internal.h"

s16 gTalkDemoId;

/* ---- script (#BTK): header 0x1c bytes then pages of 13 s32 */
typedef struct {
    char magic[4];
    s32 size;
    s32 lang;        /* +0x08 -> scene unk70 (message file language) */
    s32 id;          /* +0x0c */
    s32 pages;       /* +0x10 -> scene unk78 (page limit) */
    s32 colour;      /* +0x14 -> window unk20 (band colour, BGR555) */
    s32 bgm;         /* +0x18 TalkDemo_PlayBGM index */
} TalkHeader;
typedef struct {
    s32 idx;         /* 0x00 */
    s32 mode;        /* 0x04: 2 rock page, 3 full image, 4 blink portrait, 5 keep message, 6..15 page sound */
    s32 port_a;      /* 0x08 portrait character (data_0207ca28[a]) */
    s32 port_b;      /* 0x0c portrait expression */
    s32 port_flip;   /* 0x10 2 = mirrored */
    s32 enter;       /* 0x14 enter motion (1 = keep portrait) */
    s32 exit;        /* 0x18 exit motion */
    s32 frame;       /* 0x1c text box image (data_02076c20) */
    s32 frame_flip;  /* 0x20 2 = box on the left, mirrored */
    s32 bg1_effect;  /* 0x24 0/2 hidden, 1 shown, 3 shake */
    s32 display;     /* 0x28 3 = automatic (no prompt) */
    s32 timer;       /* 0x2c display time before the prompt */
    s32 period;      /* 0x30 frames after the exit before the next page */
} TalkPage;

/* ---- BG2 motion (TalkDemo_SetupBg2EnterMotion / ExitMotion / UpdateBg2Motion) */
typedef BOOL (*MotionFn)(void *);
typedef struct {
    MotionFn fn;     /* 0x00 */
    s32 pad[2];
    s32 target[2];   /* 0x0c */
    s32 vel[2];      /* 0x14 */
    s32 acc[2];      /* 0x1c */
    s32 pos[2];      /* 0x24 x, y */
    s16 sx, sy;      /* 0x2c */
    u16 angle;       /* 0x30 */
    u32 flags;       /* 0x34 */
    u16 timer;       /* 0x36 */
} Bg2Obj;
/* ---- BG1 scroll (TalkDemo_StartBg1Effect / ShakeBg1 / StartBg1Exit) */
typedef struct {
    MotionFn state;
    s32 x, y;
    u8 mode;         /* page frame_flip: 2 = box at x 0 */
    u16 timer;
} Bg1Obj;

typedef struct TalkScene TalkScene;
typedef void (*SceneFn)(TalkScene *);
typedef struct {
    s32 id, frame;
    u32 flags;
} PageSnd;
struct TalkScene {
    SceneFn fn;
    u32 flags;       /* 1 blink portrait, 4 restore portrait, 0x10 keep message, 0x20 no messages, 0x100 page sound queued */
    s32 counter;     /* unk8: frames */
    s32 timer;       /* unkc */
    s32 phase;       /* unk10: 1 start .. 12 finished (page sound trigger) */
    Bg1Obj bg1;      /* unk14 */
    Bg2Obj bg2;      /* unk28 */
    const u8 *script;/* unk68 */
    const TalkPage *page; /* unk6c */
    s32 lang;        /* unk70 */
    s32 count;       /* unk74 page index */
    s32 limit;       /* unk78 */
    u16 t7e, period;
    CsSpr prompt;    /* unk84 */
    CsRes *prompt_res;
    const PageSnd *snd_def; /* unkd0 */
    int snd_handle;
    s32 confirm_se;
    u16 msg_base;    /* unkdc */
    s16 msg_cur;
};

typedef struct TalkWindow TalkWindow;
typedef void (*WindowFn)(TalkWindow *);
struct TalkWindow {
    WindowFn fn;
    u16 t;           /* unk8 */
    u16 colour;      /* unk20 */
    u8 top, bottom;  /* unk16 / unk17 (window y1 / y2) */
    int raster;      /* TalkDemoWindowRaster_Main / StartVAlarmCycle running */
    u16 pal[4];      /* unkec: backdrop colours of the 4 raster sections */
    u8 flag[4];      /* unkf4: BG2 visible in the section (centre mode) */
};
typedef struct TalkDim TalkDim;
struct TalkDim {
    void (*fn)(TalkDim *);
    s16 t;
};
typedef struct {
    int active;
    s32 life;                 /* unkc */
    CsSpr spr[2];
    CsRes *res;
    s32 a[6][2], b[6][2], c[6][2];
    u8 d[6];
} TalkRocks;

typedef struct TalkMain TalkMain;
typedef void (*MainFn)(TalkMain *);
struct TalkMain {
    MainFn fn;
    u32 flags;       /* 0x80000000 destroy */
    u32 fl[4];       /* unk10[0..3]: TalkDemo_GetFlags / SetFlags groups */
    s32 lang;        /* unk20 */
    u8 *script;      /* unk24 */
    u32 script_size;
    const s32 *entry;/* unk28: {group, seqarc, se, flags} */
};

/* display state the tasks write (gDisplayMain / window / blend registers) */
typedef struct {
    int win_on;               /* gDisplayMain+0x1c (WININ/OUT enable) */
    u8 w0x1, w0x2, w1x1, w1x2;/* WIN0H/WIN1H x1 (left) / x2 (right) */
    u8 wy1, wy2;              /* WIN0V/WIN1V */
    int out_bg2, in_bg2;      /* window bit 2: BG2 outside / inside */
    u16 backdrop;             /* palette colour 0 before the raster starts */
    s16 evy;                  /* BLDY (dim) */
    s32 bg1_x, bg1_y;         /* BG1 HOFS / VOFS */
    s32 m[4];                 /* BG2 affine */
    s32 bg2_x, bg2_y;         /* BG2 x1 / y1 */
} TalkDisp;

static TalkMain s_main;
static TalkScene s_scene;
static TalkWindow s_win;
static TalkDim s_dim;
static TalkRocks s_rocks;
static TalkDisp s_disp;
static int s_main_on, s_scene_on, s_win_on, s_dim_on, s_bg2win_on;
static void (*s_deferred[4])(void);
static int s_ndeferred;
/* BG maps rebuilt as textures */
static u8 s_box_idx[176 * 80];   /* BG1 22x10 frame + 16x7 message at (3,1) */
static int s_box_flip;
static u8 s_port_idx[256 * 56];  /* BG2: 10x10 portrait (80x80) or 32x7 full image (256x56) */
static int s_port_w, s_port_h, s_port_flip;
static u16 s_pal_bg1[256], s_pal_bg2[256];
static CsTex s_box_tex, s_port_tex;
static int s_box_dirty, s_port_dirty;
static int s_pages_run;

/* ----------------------------------------------------------- flags */
u32 TalkDemo_GetFlags(s32 i) { return s_main_on ? s_main.fl[i] : 0; }
void TalkDemo_SetFlags(s32 i, u32 v) { if (s_main_on) s_main.fl[i] |= v; }
void TalkDemo_ClearFlags(s32 i, u32 v) { if (s_main_on) s_main.fl[i] &= ~v; }
BOOL TalkDemo_IsActive(void) { return s_main_on; }

static const TalkHeader *TalkDemo_GetScriptHeader(const u8 *s) { return (const TalkHeader *)s; }
static const TalkPage *TalkDemo_GetScriptPage(const u8 *s, s32 i) { return (const TalkPage *)(s + 0x1c + 0x34 * i); }
static const s32 *TalkDemo_GetEntry(void) {
    const s32 *t = (const s32 *)cs_sec("td_snd", NULL);
    return t && gTalkDemoId >= 0 && gTalkDemoId < 0x3a ? t + 4 * gTalkDemoId : NULL;
}
/* entry present (the DS pointer non-NULL) */
static BOOL entry_ok(const s32 *e) { return e && ((u32)e[3] & 0x80000000u); }

static void TalkDemo_CreateDeferredCall(void (*f)(void)) {
    if (s_ndeferred < 4)
        s_deferred[s_ndeferred++] = f;
}

/* ----------------------------------------------------------- script / images */
static u8 *TalkDemo_LoadScript(s32 lang, u32 idx, u32 *size) {
    const char *names = (const char *)cs_sec("td_name", NULL);
    char path[200];
    if (idx == 0 || idx >= 0x3a || !names || !names[32 * idx])
        return NULL;
    snprintf(path, sizeof(path), "%s/talk/%s_%s.btk", g_cs_dir, names + 32 * idx, cs_lang_suffix[lang]);
    return (u8 *)cs_load_file(path, size);
}

static int load_talk_img(CsPic *p, s32 v) {
    char path[200];
    snprintf(path, sizeof(path), "%s/talk/img.csp", g_cs_dir);
    return cs_pack_read(p, path, v);
}
/* TalkDemo_LoadImageByPair: data_0207ca28[a][b] */
static int TalkDemo_LoadImageByPair(CsPic *p, s32 a, s32 b) {
    const s32 *t = (const s32 *)cs_sec("td_port", NULL);
    s32 v;
    if (a == 0 || b == 0 || !t || a >= 13 || b >= 24)   /* td_port is s32[13][24] (csdata.py) */
        return -1;
    v = t[24 * a + b];
    if (v < 0)
        return -1;
    return load_talk_img(p, v);
}
/* TalkDemo_LoadImageByIndex: data_02076c20[i] */
static int TalkDemo_LoadImageByIndex(CsPic *p, s32 i) {
    const s32 *t = (const s32 *)cs_sec("td_frame", NULL);
    if (i == 0 || !t || i >= 16 || t[i] < 0)
        return -1;
    return load_talk_img(p, t[i] & 0xffff);
}

static void put_pal(u16 *dst, const CsImg *im, int first, int count) {
    int i;
    for (i = first; i < first + count && i < 256; i++) {
        int k = i - im->pofs;
        dst[i] = (k >= 0 && k < im->npal) ? im->pal[k] : 0;
    }
}

static void TalkDemo_ClearMapRect10x10(void) {
    memset(s_port_idx, 0, sizeof(s_port_idx));
    s_port_dirty = 1;
}
/* TalkDemo_LoadImage10x10: chars + 224 colours (stage) or the ext palette (map) */
static void TalkDemo_LoadImage10x10(CsPic *p) {
    int y;
    if (!p) {
        TalkDemo_ClearMapRect10x10();
        return;
    }
    put_pal(s_pal_bg2, &p->img, 0, (TalkDemo_GetFlags(0) & 1) ? 256 : 0xe0);
    memset(s_port_idx, 0, sizeof(s_port_idx));
    s_port_w = 80;
    s_port_h = 80;
    for (y = 0; y < 80 && y < p->img.h; y++)
        memcpy(s_port_idx + y * 80, p->img.px + y * p->img.w, p->img.w < 80 ? p->img.w : 80);
    s_port_flip = 0;
    s_port_dirty = 1;
}
static void TalkDemo_FlipImage10x10(void) {
    if (!s_port_flip) {
        s_port_flip = 1;
        s_port_dirty = 1;
    }
}
/* TalkDemo_SetupBg2ExitMotion(13) flips back with the same call on the DS
 * (TalkDemo_SetMapRectHFlip(.., flag 1) only flips a rect that is not flipped) */

static u8 s_frame_idx[176 * 80];
static void compose_box(void);
static void TalkDemo_ClearImage22x10(void) {
    memset(s_frame_idx, 0, sizeof(s_frame_idx));
    s_box_flip = 0;
    compose_box();
}
static void TalkDemo_LoadImage22x10(CsPic *p) {
    int y;
    if (!p) {
        TalkDemo_ClearImage22x10();
        return;
    }
    /* LoadPal: 16 colours of the frame's palette at 0x1c0 (bank 14) */
    put_pal(s_pal_bg1, &p->img, 0, 256);
    memset(s_frame_idx, 0, sizeof(s_frame_idx));
    for (y = 0; y < 80 && y < p->img.h; y++)
        memcpy(s_frame_idx + y * 176, p->img.px + y * p->img.w, p->img.w < 176 ? p->img.w : 176);
    s_box_flip = 0;
    compose_box();
}
static void TalkDemo_FlipImage22x10(void) {
    if (!s_box_flip) {
        s_box_flip = 1;
        compose_box();
    }
}
/* message page (TalkDemo_LoadMessageBg): 16x7 tiles at (3,1) of BG1 */
static u8 s_msg_idx[128 * 56];
static int s_msg_on;
static void TalkDemo_LoadMessageBg(CsPic *p) {
    int y;
    memset(s_msg_idx, 0, sizeof(s_msg_idx));
    s_msg_on = p != NULL;
    if (p)
        for (y = 0; y < 56 && y < p->img.h; y++)
            memcpy(s_msg_idx + y * 128, p->img.px + y * p->img.w, p->img.w < 128 ? p->img.w : 128);
    compose_box();
}
static void compose_box(void) {
    int x, y;
    for (y = 0; y < 80; y++)
        for (x = 0; x < 176; x++)
            s_box_idx[y * 176 + x] = s_frame_idx[y * 176 + (s_box_flip ? 175 - x : x)];
    if (s_msg_on)
        for (y = 0; y < 56; y++)
            memcpy(s_box_idx + (8 + y) * 176 + 24, s_msg_idx + y * 128, 128);
    s_box_dirty = 1;
}

/* TalkDemo_ReadMessage: the next page of /bg2/talk/talk_m_<lang>.bb */
static int TalkDemo_ReadMessage(TalkScene *w, CsPic *p) {
    char path[200];
    s32 n;
    if (w->flags & 0x20)
        return -1;
    n = w->msg_base + w->msg_cur++;
    snprintf(path, sizeof(path), "%s/talk/msg_%s.csp", g_cs_dir, cs_lang_suffix[w->lang >= 0 && w->lang < 6 ? w->lang : 1]);
    cs_log("talk msg %d", n);
    if (cs_pack_read(p, path, n) != 0) {
        cs_log("talk msg %d missing", n);
        return -1;
    }
    return 0;
}

/* ----------------------------------------------------------- BG1 / BG2 */
static BOOL TalkDemo_SetBg1Scroll(Bg1Obj *s) {
    s_disp.bg1_x = -(s->x >> 8);
    s_disp.bg1_y = -(s->y >> 8);
    return TRUE;
}
static BOOL TalkDemo_ResetBg1Scroll(Bg1Obj *s) {
    Bg1Obj tmp = {0, 0, 0, 0, 0};
    if (!s)
        s = &tmp;
    s->x = -0xb000;
    s->y = -0x5000;
    TalkDemo_SetBg1Scroll(s);
    return TRUE;
}
static void TalkDemo_ApplyBg2Affine(Bg2Obj *o) {
    s32 s = cs_sin(o->angle), c = cs_cos(o->angle);
    /* MTX_Rot22 then MTX_ScaleApply22(sx << 4, sy << 4) */
    s_disp.m[0] = FX_Mul(c, o->sx << 4);
    s_disp.m[1] = FX_Mul(-s, o->sy << 4);
    s_disp.m[2] = FX_Mul(s, o->sx << 4);
    s_disp.m[3] = FX_Mul(c, o->sy << 4);
    s_disp.bg2_x = -(o->pos[0] >> 8);
    s_disp.bg2_y = -(o->pos[1] >> 8);
}
static BOOL TalkDemo_ResetBg2Affine(void *p) {
    Bg2Obj tmp;
    Bg2Obj *o = (Bg2Obj *)p;
    if (!o) {
        memset(&tmp, 0, sizeof(tmp));
        o = &tmp;
        tmp.sx = 0x100;
        tmp.sy = 0x100;
        tmp.angle = 0;
    }
    o->pos[0] = -0x5000;
    o->pos[1] = -0x5000;
    TalkDemo_ApplyBg2Affine(o);
    return TRUE;
}
static BOOL TalkDemo_UpdateBg2Motion(void *p) {
    Bg2Obj *o = (Bg2Obj *)p;
    u8 done = 0, i;
    BOOL ret = FALSE;
    for (i = 0; i < 2; i++) {
        if (o->flags & (1u << i)) {
            done |= 1 << i;
            continue;
        }
        o->vel[i] += o->acc[i];
        o->pos[i] += o->vel[i];
        if ((o->vel[i] > 0 && o->pos[i] >= o->target[i]) || (o->vel[i] < 0 && o->pos[i] <= o->target[i]) || o->vel[i] == 0) {
            o->pos[i] = o->target[i];
            done |= 1 << i;
        }
    }
    TalkDemo_ApplyBg2Affine(o);
    if (o->flags & 0x10) {
        o->timer--;
        if (o->timer == 0)
            ret = TRUE;
    } else if (done == 3) {
        ret = TRUE;
    }
    if (ret) {
        o->vel[0] = o->vel[1] = 0;
        o->acc[0] = o->acc[1] = 0;
        if (o->flags & 4)
            TalkDemo_ResetBg2Affine(o);
    }
    return ret;
}
static MotionFn motion_fn(u32 kind) {
    return kind == 1 ? TalkDemo_UpdateBg2Motion : kind == 2 ? TalkDemo_ResetBg2Affine : NULL;
}
static BOOL TalkDemo_SetupBg2EnterMotion(Bg2Obj *o, s32 id) {
    const u8 *t = (const u8 *)cs_sec("td_enter", NULL);
    const s16 *v;
    o->flags = 0;
    if (id == 1 || !t || id < 0 || id >= 16) {
        o->fn = NULL;
        return TRUE;
    }
    v = (const s16 *)(t + 20 * id + 4);
    o->sx = o->sy = 0x100;
    o->angle = 0;
    o->fn = motion_fn(*(const u32 *)(t + 20 * id));
    o->pos[0] = v[0] << 8;
    o->pos[1] = v[1] << 8;
    o->target[0] = v[2] << 8;
    o->target[1] = v[3] << 8;
    o->vel[0] = v[4] << 8;
    o->vel[1] = v[5] << 8;
    o->acc[0] = v[6];
    o->acc[1] = v[7];
    TalkDemo_ApplyBg2Affine(o);
    return TRUE;
}
static BOOL TalkDemo_SetupBg2ExitMotion(Bg2Obj *o, s32 id) {
    const u8 *t = (const u8 *)cs_sec("td_exit", NULL);
    const s16 *v;
    o->flags = 0;
    if (id == 1 || !t || id < 0 || id >= 14) {
        o->fn = NULL;
        return TRUE;
    }
    v = (const s16 *)(t + 20 * id + 8);
    o->fn = motion_fn(*(const u32 *)(t + 20 * id));
    o->target[0] = v[0] << 8;
    o->target[1] = v[1] << 8;
    o->vel[0] = v[2] << 8;
    o->vel[1] = v[3] << 8;
    o->acc[0] = v[4];
    o->acc[1] = v[5];
    o->flags |= 4;
    switch (id) {
    case 9:
    case 10:
        o->flags |= 1;
        break;
    case 11:
    case 12: {
        const s32 *e = TalkDemo_GetEntry();
        const u8 *ps = (const u8 *)cs_sec("td_psnd", NULL);
        o->flags |= 0x20;
        if (entry_ok(e) && ps)
            cs_se(CSH_SE, e[1], *(const s32 *)(ps + 0x18));
        break;
    }
    case 13:
        o->flags |= 0x40;
        TalkDemo_FlipImage10x10();
        break;
    }
    TalkDemo_ApplyBg2Affine(o);
    return TRUE;
}

static BOOL TalkDemo_State_ShakeBg1(void *p) {
    Bg1Obj *f = (Bg1Obj *)p;
    BOOL done = FALSE;
    s32 bx, by;
    if (f->mode == 2) {
        bx = 0;
        by = 0x3f00;
    } else {
        bx = 0x5000;
        by = 0x3f00;
    }
    f->timer--;
    if (f->timer != 0) {
        if (f->timer & 2) {
            f->x = bx + (0x300 - (cs_rand() & 0x600));
            f->y = by + (0x300 - (cs_rand() & 0x600));
        }
    } else {
        f->x = bx;
        f->y = by;
        done = TRUE;
    }
    TalkDemo_SetBg1Scroll(f);
    return done;
}
static BOOL TalkDemo_StartBg1Effect(Bg1Obj *f, u32 mode) {
    switch (mode) {
    case 0:
    case 2:
    default:
        TalkDemo_ResetBg1Scroll(f);
        f->state = NULL;
        break;
    case 1:
        if (f->mode == 2) {
            f->x = 0;
            f->y = 0x3f00;
        } else {
            f->x = 0x5000;
            f->y = 0x3f00;
        }
        f->state = NULL;
        break;
    case 3:
        f->timer = 0x1e;
        f->state = TalkDemo_State_ShakeBg1;
        TalkDemo_ResetBg1Scroll(f);
        break;
    }
    TalkDemo_SetBg1Scroll(f);
    return TRUE;
}
static BOOL TalkDemo_StartBg1Exit(Bg1Obj *s, u32 mode) {
    switch (mode) {
    case 0:
    case 1:
    case 3:
    default:
        s->x = -0xb000;
        s->y = -0x5000;
        s->state = NULL;
        break;
    case 2:
        s->state = NULL;
        break;
    }
    TalkDemo_SetBg1Scroll(s);
    return TRUE;
}

/* ----------------------------------------------------------- sound */
static void TalkDemoScene_InitSound(TalkScene *s) {
    const s32 *e = TalkDemo_GetEntry();
    s->snd_handle = CSH_TALK_PAGE;
    if (entry_ok(e))
        s->confirm_se = e[2];
}
static void TalkDemoScene_QueuePageSound(TalkScene *w) {
    const u8 *ps = (const u8 *)cs_sec("td_psnd", NULL);
    const s32 *pm = (const s32 *)cs_sec("td_pmap", NULL);
    s32 mode = w->page->mode;
    if (gTalkDemoId == 0x37) {
        w->snd_def = (const PageSnd *)cs_sec("td_p37", NULL);
        w->flags |= 0x100;
        return;
    }
    if (mode >= 6 && mode <= 0xf && ps && pm) {
        if (mode == 0xf) {
            /* PlayerDemo BGM restarted: Sound_PlaySeq(&pd->snd0, data_02075a34.unkC) */
            const s16 *bgm = (const s16 *)cs_sec("pd_bgm", NULL);
            PlayerDemo_StopBGMForTalk(bgm ? bgm[6] : 0);
        } else {
            w->snd_def = (const PageSnd *)(ps + 12 * pm[mode]);
            w->flags |= 0x100;
        }
    }
}
static void TalkDemoScene_StopPageSound(TalkScene *w) {
    if (w->snd_def && cs_snd_active(w->snd_handle) && (w->snd_def->flags & 2))
        cs_snd_stop(w->snd_handle, 0);
    w->snd_def = NULL;
    w->flags &= ~0x100u;
}
static void TalkDemoScene_UpdateSound(TalkScene *w) {
    const PageSnd *d;
    const s32 *e;
    if (!(w->flags & 0x100))
        return;
    d = w->snd_def;
    if (!d || d->frame > w->phase)
        return;
    e = TalkDemo_GetEntry();
    if (!entry_ok(e))
        return;
    cs_snd_stop(w->snd_handle, 0);
    if (d->flags & 1)
        cs_voice(e[1], d->id);
    else
        cs_se(w->snd_handle, e[1], d->id);
    w->flags &= ~0x100u;
}

/* ----------------------------------------------------------- rocks */
static void TalkDemoRocks_Init(TalkRocks *w) {
    const u8 *spr = (const u8 *)cs_sec("td_spr", NULL);
    const s16 *e = (const s16 *)cs_sec("td_rock", NULL);
    u8 i;
    if (!w->res && spr)
        w->res = cs_res_load((const char *)spr + 48 * 6);
    for (i = 0; i < 2; i++) {
        AnimatorSprite_Init(&w->spr[i], w->res, 0, SPR_LOOP);
        AnimatorSprite_ProcessAnimation(&w->spr[i]);
    }
    for (i = 0; i < 6 && e; i++) {
        const s16 *v = (const s16 *)((const u8 *)e + 14 * i);
        w->a[i][0] = (v[0] << 8) + (0x300 - (cs_rand() & 0x600));
        w->a[i][1] = (v[1] << 8) + (0x300 - (cs_rand() & 0x600));
        w->b[i][0] = (v[2] << 8) + (0x300 - (cs_rand() & 0x600));
        w->b[i][1] = (v[3] << 8) + (0x300 - (cs_rand() & 0x600));
        w->c[i][0] = v[4] + (7 - (cs_rand() & 0xe));
        w->c[i][1] = v[5] + (7 - (cs_rand() & 0xe));
        w->d[i] = ((const u8 *)v)[12];
    }
}
static void TalkDemoRocks_Create(s32 life) {
    s_rocks.active = 1;
    s_rocks.life = life;
    TalkDemoRocks_Init(&s_rocks);
}
static void TalkDemoRocks_Main(void) {
    u8 i;
    TalkRocks *w = &s_rocks;
    if (!w->active)
        return;
    if (--w->life == 0) {
        w->active = 0;
        return;
    }
    AnimatorSprite_ProcessAnimation(&w->spr[0]);
    AnimatorSprite_ProcessAnimation(&w->spr[1]);
    for (i = 0; i < 6; i++) {
        w->b[i][0] += w->c[i][0];
        w->b[i][1] += w->c[i][1];
        w->a[i][0] += w->b[i][0];
        w->a[i][1] += w->b[i][1];
    }
}

/* ----------------------------------------------------------- scene states */
static void TalkDemoScene_State_StartPage(TalkScene *s);
static void TalkDemoScene_State_LoadImage22x10(TalkScene *w);
static void TalkDemoScene_State_LoadMessage(TalkScene *s);
static void TalkDemoScene_State_StartEnterMotion(TalkScene *s);
static void TalkDemoScene_State_EnterMotion(TalkScene *s);
static void TalkDemoScene_State_StartBg1Effect(TalkScene *s);
static void TalkDemoScene_State_Bg1Effect(TalkScene *s);
static void TalkDemoScene_State_StartDisplay(TalkScene *s);
static void TalkDemoScene_State_Display(TalkScene *w);
static void TalkDemoScene_State_WaitInput(TalkScene *w);
static void TalkDemoScene_State_StartBg1Exit(TalkScene *s);
static void TalkDemoScene_State_Bg1Exit(TalkScene *s);
static void TalkDemoScene_State_StartExitMotion(TalkScene *s);
static void TalkDemoScene_State_ExitMotion(TalkScene *s);
static void TalkDemoScene_State_EndPage(TalkScene *s);
static void TalkDemoScene_State_NextPage(TalkScene *w);
static void TalkDemoScene_State_RockPage(TalkScene *s);
static void TalkDemoScene_State_RockPageWaitMotion(TalkScene *s);
static void TalkDemoScene_State_RockPageDelay(TalkScene *s);
static void TalkDemoScene_State_RockPageWaitDelay(TalkScene *s);
static void TalkDemoScene_State_RockPageSpawn(TalkScene *w);
static void TalkDemoScene_State_RockPageWait(TalkScene *s);
static void TalkDemoScene_State_RockPageEnd(TalkScene *s);
static void TalkDemoScene_State_RockPageNext(TalkScene *w);
static void TalkDemoScene_State_FullImageWait(TalkScene *s);

static void TalkDemo_LoadCommonSprites(TalkScene *w) {
    const u8 *spr = (const u8 *)cs_sec("td_spr", NULL);
    if (spr && !w->prompt_res)
        w->prompt_res = cs_res_load((const char *)spr + 48 * (w->lang >= 0 && w->lang < 6 ? w->lang : 1));
    AnimatorSprite_Init(&w->prompt, w->prompt_res, 0, SPR_LOOP);
    AnimatorSprite_ProcessAnimation(&w->prompt);
}
static BOOL TalkDemoScene_InitMessageIndex(TalkScene *s, u16 *out) {
    const s32 *t = (const s32 *)cs_sec("td_msg", NULL);
    s32 r = t ? t[0x3a * cs_lang() + gTalkDemoId] : -1;
    if (r < 0) {
        s->flags |= 0x20;
        return FALSE;
    }
    *out = (u16)r;
    return TRUE;
}
static void TalkDemoScene_State_WaitStart(TalkScene *s) {
    if (TalkDemo_GetFlags(3) & 1)
        s->fn = TalkDemoScene_State_StartPage;
}
static void TalkDemoScene_State_Init(TalkScene *s) {
    TalkDemo_ResetBg1Scroll(NULL);
    TalkDemo_ResetBg2Affine(NULL);
    TalkDemo_LoadCommonSprites(s);
    TalkDemoScene_InitMessageIndex(s, &s->msg_base);
    TalkDemoScene_InitSound(s);
    s->fn = TalkDemoScene_State_WaitStart;
}
static void TalkDemoScene_State_StartPage(TalkScene *s) {
    const TalkPage *p = TalkDemo_GetScriptPage(s->script, s->count & 0xff);
    s->page = p;
    s->t7e = 0;
    s->period = (u16)p->period;
    s_pages_run++;
    cs_log("talk %d page %d mode %d portrait %d/%d enter %d exit %d frame %d bg1 %d disp %d timer %d period %d", gTalkDemoId,
           s->count, p->mode, p->port_a, p->port_b, p->enter, p->exit, p->frame, p->bg1_effect, p->display, p->timer,
           p->period);
    if (p->mode == 2) {
        s->fn = TalkDemoScene_State_RockPage;
    } else {
        if (p->mode == 4)
            s->flags |= 1;
        if (p->mode == 5)
            s->flags |= 0x10;
        if (p->enter != 1)
            TalkDemo_ResetBg2Affine(NULL);
        s->timer = 0;
        s->fn = NULL;   /* -> LoadImage10x10 below */
    }
    TalkDemoScene_QueuePageSound(s);
    s->phase = 1;
    if (!s->fn) {
        /* TalkDemoScene_State_LoadImage10x10 (next frame on the DS) */
        extern void talk_scene_load10x10(TalkScene *);
        s->fn = talk_scene_load10x10;
    }
}
void talk_scene_load10x10(TalkScene *w) {
    const TalkPage *info = w->page;
    CsPic p;
    if (info->enter != 1) {
        int ok = TalkDemo_LoadImageByPair(&p, info->port_a, info->port_b) == 0;
        if (!ok && info->port_a != 0 && info->port_b != 0)
            cs_log("talk %d portrait %d/%d missing", gTalkDemoId, info->port_a, info->port_b);
        TalkDemo_LoadImage10x10(ok ? &p : NULL);
        if (ok)
            cs_pic_free(&p);
    }
    if (info->port_flip == 2)
        TalkDemo_CreateDeferredCall(TalkDemo_FlipImage10x10);
    w->fn = TalkDemoScene_State_LoadImage22x10;
}
static void TalkDemoScene_State_LoadImage22x10(TalkScene *w) {
    const TalkPage *e = TalkDemo_GetScriptPage(w->script, w->count);
    CsPic p;
    int ok = TalkDemo_LoadImageByIndex(&p, e->frame) == 0;
    TalkDemo_LoadImage22x10(ok ? &p : NULL);
    if (ok)
        cs_pic_free(&p);
    if (e->frame_flip == 2)
        TalkDemo_CreateDeferredCall(TalkDemo_FlipImage22x10);
    w->fn = TalkDemoScene_State_LoadMessage;
}
static void TalkDemoScene_State_LoadMessage(TalkScene *s) {
    if (!(s->flags & 0x10)) {
        CsPic p;
        if (TalkDemo_ReadMessage(s, &p) == 0) {
            TalkDemo_LoadMessageBg(&p);
            cs_pic_free(&p);
        }
    }
    s->fn = TalkDemoScene_State_StartEnterMotion;
}
static void TalkDemoScene_State_StartEnterMotion(TalkScene *s) {
    TalkDemo_SetupBg2EnterMotion(&s->bg2, s->page->enter);
    s->phase = 2;
    s->fn = TalkDemoScene_State_EnterMotion;
}
static void TalkDemoScene_State_EnterMotion(TalkScene *s) {
    if (!s->bg2.fn || s->bg2.fn(&s->bg2)) {
        s->phase = 3;
        s->fn = TalkDemoScene_State_StartBg1Effect;
    }
}
static void TalkDemoScene_State_StartBg1Effect(TalkScene *s) {
    const TalkPage *e = TalkDemo_GetScriptPage(s->script, s->count);
    s->bg1.mode = (u8)e->frame_flip;
    TalkDemo_StartBg1Effect(&s->bg1, (u32)e->bg1_effect);
    s->phase = 4;
    s->fn = TalkDemoScene_State_Bg1Effect;
}
static void TalkDemoScene_State_Bg1Effect(TalkScene *s) {
    if (!s->bg1.state || s->bg1.state(&s->bg1)) {
        s->phase = 5;
        s->fn = TalkDemoScene_State_StartDisplay;
    }
}
static void TalkDemoScene_State_StartDisplay(TalkScene *s) {
    s->timer = s->page->frame ? s->page->timer : 0;
    TalkDemo_ClearFlags(3, 0x40);
    TalkDemo_ClearFlags(3, 0x10);
    s->phase = 6;
    s->fn = TalkDemoScene_State_Display;
}
static void TalkDemoScene_State_Display(TalkScene *w) {
    w->timer--;
    if (w->timer <= 0) {
        if (w->page->display == 3) {
            w->fn = TalkDemoScene_State_StartBg1Exit;
            return;
        }
        w->phase = 7;
        w->fn = TalkDemoScene_State_WaitInput;
    }
    if (TalkDemo_GetFlags(3) & 0x40)
        w->fn = TalkDemoScene_State_StartBg1Exit;
}
static int s_prompt_drawn;
static void TalkDemoScene_DrawPrompt(TalkScene *w) {
    AnimatorSprite_ProcessAnimation(&w->prompt);
    w->prompt.x = w->page->frame_flip == 1 ? 0xd2 : 0x7e;
    w->prompt.y = 0x8a;
    s_prompt_drawn = 1;
}
static void TalkDemoScene_State_WaitInput(TalkScene *w) {
    TalkDemoScene_DrawPrompt(w);
    if (TalkDemo_GetFlags(3) & 0x50) {
        const s32 *e = TalkDemo_GetEntry();
        if (entry_ok(e))
            cs_se(CSH_SE, e[1], w->confirm_se);
        w->fn = TalkDemoScene_State_StartBg1Exit;
    }
}
static void TalkDemoScene_State_StartBg1Exit(TalkScene *s) {
    TalkDemo_StartBg1Exit(&s->bg1, (u32)s->page->display);
    s->timer = 0;
    s->fn = TalkDemoScene_State_Bg1Exit;
}
static void TalkDemoScene_State_Bg1Exit(TalkScene *s) {
    if (!s->bg1.state || s->bg1.state(&s->bg1))
        s->fn = TalkDemoScene_State_StartExitMotion;
}
static void TalkDemoScene_State_StartExitMotion(TalkScene *s) {
    TalkDemo_SetupBg2ExitMotion(&s->bg2, s->page->exit);
    switch ((u8)s->page->exit) {
    case 9:
    case 10:
        s->flags |= 1;
    }
    s->phase = 8;
    s->fn = TalkDemoScene_State_ExitMotion;
}
static void TalkDemoScene_State_ExitMotion(TalkScene *s) {
    if (!s->bg2.fn || s->bg2.fn(&s->bg2)) {
        s->phase = 9;
        s->fn = TalkDemoScene_State_EndPage;
    }
}
static void TalkDemoScene_State_EndPage(TalkScene *s) {
    if (s->flags & 1)
        s->flags |= 4;
    s->flags &= ~0x10u;
    s->phase = 10;
    s->t7e = 0;
    s->fn = TalkDemoScene_State_NextPage;
}
static void TalkDemoScene_State_NextPage(TalkScene *w) {
    w->t7e++;
    if (w->t7e < w->period)
        return;
    w->count++;
    if (w->count >= w->limit) {
        TalkDemo_SetFlags(3, 0x100);
        w->phase = 0xc;
        w->fn = NULL;
    } else {
        w->phase = 0xb;
        w->fn = TalkDemoScene_State_StartPage;
    }
    TalkDemoScene_StopPageSound(w);
}
static void TalkDemoScene_State_RockPageMotion(TalkScene *s) {
    TalkDemo_SetupBg2ExitMotion(&s->bg2, 5);   /* TalkDemo_SetupBg2RockMotion */
    s->fn = TalkDemoScene_State_RockPageWaitMotion;
}
static void TalkDemoScene_State_RockPage(TalkScene *s) { s->fn = TalkDemoScene_State_RockPageMotion; }
static void TalkDemoScene_State_RockPageWaitMotion(TalkScene *s) {
    if (!s->bg2.fn || s->bg2.fn(&s->bg2))
        s->fn = TalkDemoScene_State_RockPageDelay;
}
static void TalkDemoScene_State_RockPageDelay(TalkScene *s) {
    s->timer = 10;
    s->fn = TalkDemoScene_State_RockPageWaitDelay;
}
static void TalkDemoScene_State_RockPageWaitDelay(TalkScene *s) {
    s->timer--;
    if (s->timer <= 0)
        s->fn = TalkDemoScene_State_RockPageSpawn;
}
static void TalkDemoScene_State_RockPageSpawn(TalkScene *w) {
    const s32 *e = TalkDemo_GetEntry();
    const u8 *ps = (const u8 *)cs_sec("td_psnd", NULL);
    TalkDemoRocks_Create(w->period);
    if (entry_ok(e) && ps) {
        cs_snd_stop(w->snd_handle, 0);
        cs_se(w->snd_handle, e[1], *(const s32 *)(ps + 0x24));
    }
    w->timer = 1;
    w->fn = TalkDemoScene_State_RockPageWait;
}
static void TalkDemoScene_State_RockPageWait(TalkScene *s) {
    s->timer--;
    if (s->timer <= 0)
        s->fn = TalkDemoScene_State_RockPageEnd;
}
static void TalkDemoScene_State_RockPageEnd(TalkScene *s) {
    s->t7e = 0;
    s->fn = TalkDemoScene_State_RockPageNext;
}
static void TalkDemoScene_State_RockPageNext(TalkScene *w) {
    w->t7e++;
    if (w->t7e < w->period)
        return;
    w->count++;
    if (w->count >= w->limit) {
        TalkDemo_SetFlags(3, 0x100);
        w->fn = NULL;
        return;
    }
    w->fn = TalkDemoScene_State_StartPage;
}
/* full-image scene (page 0 mode 3): a 32x7 image on BG2 at y 68, shown for `period` frames */
static void TalkDemoScene_State_FullImage(TalkScene *w) {
    const TalkPage *info = TalkDemo_GetScriptPage(w->script, w->count);
    CsPic p;
    int y;
    w->page = info;
    s_pages_run++;
    cs_log("talk %d full image %d/%d period %d", gTalkDemoId, info->port_a, info->port_b, info->period);
    if (TalkDemo_LoadImageByPair(&p, info->port_a, info->port_b) == 0) {
        put_pal(s_pal_bg2, &p.img, 0, (TalkDemo_GetFlags(0) & 1) ? 256 : 0xf0);
        memset(s_port_idx, 0, sizeof(s_port_idx));
        s_port_w = 256;
        s_port_h = 56;
        for (y = 0; y < 56 && y < p.img.h; y++)
            memcpy(s_port_idx + y * 256, p.img.px + y * p.img.w, p.img.w < 256 ? p.img.w : 256);
        s_port_flip = 0;
        s_port_dirty = 1;
        cs_pic_free(&p);
    }
    {
        Bg2Obj prm;
        memset(&prm, 0, sizeof(prm));
        prm.sx = 0x100;
        prm.sy = 0x100;
        prm.angle = 0;
        prm.pos[0] = 0;
        prm.pos[1] = 0x4400;
        TalkDemo_ApplyBg2Affine(&prm);
    }
    w->t7e = 0;
    w->period = (u16)info->period;
    w->fn = TalkDemoScene_State_FullImageWait;
}
static void TalkDemoScene_State_FullImageWait(TalkScene *s) {
    s->t7e++;
    if (s->t7e >= s->period) {
        TalkDemo_SetFlags(3, 0x100);
        s->fn = NULL;
    }
}

static void TalkDemoScene_ReleaseSprites(TalkScene *w) {
    cs_res_free(w->prompt_res);
    w->prompt_res = NULL;
}
static void TalkDemoScene_Destructor(void) {
    TalkDemo_ResetBg1Scroll(NULL);
    TalkDemo_ResetBg2Affine(NULL);
    TalkDemoScene_ReleaseSprites(&s_scene);
    s_scene_on = 0;
}
static void TalkDemoScene_Create(void) {
    const TalkHeader *h;
    memset(&s_scene, 0, sizeof(s_scene));
    s_scene.script = s_main.script;
    if (!s_scene.script)
        return;
    h = TalkDemo_GetScriptHeader(s_scene.script);
    s_scene.lang = h->lang;
    s_scene.limit = h->pages;
    s_scene_on = 1;
    s_bg2win_on = 1;   /* TalkDemoScene_CreateBg2WindowTask */
    s_scene.fn = (TalkDemo_GetFlags(0) & 2) ? TalkDemoScene_State_FullImage : TalkDemoScene_State_Init;
}
static void TalkDemoScene_Main(void) {
    TalkScene *w = &s_scene;
    if (!TalkDemo_IsActive() || (TalkDemo_GetFlags(3) & 0x40000000)) {
        TalkDemoScene_Destructor();
        return;
    }
    w->counter++;
    if (TalkDemo_GetFlags(3) & 0x20000000) {
        TalkDemo_ResetBg2Affine(NULL);
        TalkDemo_ResetBg1Scroll(NULL);
        w->fn = NULL;
    }
    if (w->fn)
        w->fn(w);
    TalkDemoScene_UpdateSound(w);
    if (TalkDemo_GetFlags(3) & 0x80000000u)
        TalkDemoScene_Destructor();
}
/* TalkDemoSceneBg2Window_Main: the portrait blinks (frame parity) while flags & 1 */
static void TalkDemoSceneBg2Window_Main(void) {
    TalkScene *w = &s_scene;
    if (!s_scene_on) {
        s_bg2win_on = 0;
        return;
    }
    if (w->flags & 1) {
        if (w->flags & 4) {
            s_disp.in_bg2 = s_disp.out_bg2 = 1;
            w->flags &= ~7u;
        } else if (w->counter & 1) {
            s_disp.in_bg2 = s_disp.out_bg2 = 1;
        } else {
            s_disp.in_bg2 = s_disp.out_bg2 = 0;
        }
    }
}

/* ----------------------------------------------------------- window */
static void TalkDemoWindow_State_WipeLow(TalkWindow *t);
static void TalkDemoWindow_State_WipeCenter(TalkWindow *t);
static void TalkDemoWindow_State_OpenLow(TalkWindow *t);
static void TalkDemoWindow_State_OpenCenter(TalkWindow *t);
static void TalkDemoWindow_State_WaitCloseLow(TalkWindow *s);
static void TalkDemoWindow_State_WaitCloseCenter(TalkWindow *s);
static void TalkDemoWindow_State_CloseLow(TalkWindow *t);
static void TalkDemoWindow_State_CloseCenter(TalkWindow *t);

static void window_init(TalkWindow *t, u8 top, u8 bottom, int center) {
    t->top = top;
    t->bottom = bottom;
    s_disp.w0x1 = 0xff;
    s_disp.w0x2 = 0;
    s_disp.wy1 = t->top;
    s_disp.wy2 = t->bottom;
    s_disp.w1x1 = 0;
    s_disp.w1x2 = 0;
    /* WININ: bg1 (+bg2 low), effect; WINOUT: everything (bg2 only in low mode) */
    s_disp.in_bg2 = center ? 0 : 1;
    s_disp.out_bg2 = center ? 0 : 1;
    s_disp.win_on = 3;
    s_disp.backdrop = 0;
    t->t = 0;
}
static void TalkDemoWindow_State_InitLow(TalkWindow *t) {
    window_init(t, 0x86, 0x89, 0);
    t->fn = TalkDemoWindow_State_WipeLow;
}
static void TalkDemoWindow_State_InitCenter(TalkWindow *t) {
    window_init(t, 0x60, 0x63, 1);
    t->fn = TalkDemoWindow_State_WipeCenter;
}
static void TalkDemoWindow_StartRaster(TalkWindow *t) {
    t->raster = 1;
    t->pal[0] = 0;
    t->pal[1] = t->colour;
    t->pal[2] = 0;
    t->pal[3] = 0;
    t->flag[0] = 0;
    t->flag[1] = 1;
    t->flag[2] = 0;
    t->flag[3] = 0;
}
static void begin_open(TalkWindow *t, WindowFn next) {
    TalkDemoWindow_StartRaster(t);
    s_disp.w0x1 = 1;
    s_disp.w0x2 = 0;
    s_disp.w1x1 = 0;
    s_disp.w1x2 = 1;
    t->t = 0;
    t->fn = next;
}
static void TalkDemoWindow_State_BeginOpenLow(TalkWindow *t) { begin_open(t, TalkDemoWindow_State_OpenLow); }
static void TalkDemoWindow_State_BeginOpenCenter(TalkWindow *t) { begin_open(t, TalkDemoWindow_State_OpenCenter); }
static void TalkDemoWindow_State_BeginCloseLow(TalkWindow *t) {
    t->t = 0;
    t->fn = TalkDemoWindow_State_CloseLow;
}
static void TalkDemoWindow_State_BeginCloseCenter(TalkWindow *t) {
    t->t = 0;
    t->fn = TalkDemoWindow_State_CloseCenter;
}
static void TalkDemoWindow_State_WipeLow(TalkWindow *t) {
    t->t++;
    s_disp.w0x1 = (u8)((-(t->t << 8) / 0x14) + 0x100);
    s_disp.backdrop = 0;
    if (t->t >= 0x14)
        t->fn = TalkDemoWindow_State_BeginOpenLow;
}
static void TalkDemoWindow_State_WipeCenter(TalkWindow *t) {
    t->t++;
    s_disp.w0x1 = (u8)((-(t->t << 8) / 0xa) + 0x100);
    s_disp.backdrop = 0;
    if (t->t >= 0xa)
        t->fn = TalkDemoWindow_State_BeginOpenCenter;
}
static void TalkDemoWindow_State_OpenLow(TalkWindow *t) {
    t->t++;
    t->top = (u8)((-46 * (s16)t->t) / 0xf + 0x86);
    s_disp.wy1 = t->top;
    if (t->t >= 0xf) {
        t->fn = TalkDemoWindow_State_WaitCloseLow;   /* + TalkDemoWindow_StartVAlarmCycle */
        TalkDemo_SetFlags(1, 4);
    }
}
static void TalkDemoWindow_State_OpenCenter(TalkWindow *t) {
    t->t++;
    t->top = (u8)((-31 * (s16)t->t) / 0xa + 0x60);
    s_disp.wy1 = t->top;
    t->bottom = (u8)((0x1b * (s16)t->t) / 0xa + 0x63);
    s_disp.wy2 = t->bottom;
    if (t->t >= 0xa) {
        t->fn = TalkDemoWindow_State_WaitCloseCenter;
        TalkDemo_SetFlags(1, 4);
    }
}
static void wait_close(TalkWindow *s, WindowFn next) {
    if (TalkDemo_GetFlags(1) & 0x10) {
        TalkDemo_ClearFlags(1, 0x10);
        TalkDemo_SetFlags(1, 0x20);
        s->t = 0;
        s->fn = next;
    }
}
static void TalkDemoWindow_State_WaitCloseLow(TalkWindow *s) { wait_close(s, TalkDemoWindow_State_BeginCloseLow); }
static void TalkDemoWindow_State_WaitCloseCenter(TalkWindow *s) { wait_close(s, TalkDemoWindow_State_BeginCloseCenter); }
static void close_done(TalkWindow *t) {
    t->fn = NULL;
    s_disp.win_on = 0;
    TalkDemo_ClearFlags(1, 0x20);
    TalkDemo_SetFlags(1, 0x80000040);
}
static void TalkDemoWindow_State_CloseLow(TalkWindow *t) {
    t->t++;
    t->top = (u8)((0x31 * (s16)t->t) / 0x14 + 0x58);
    s_disp.wy1 = t->top;
    if (t->t >= 0x14)
        close_done(t);
}
static void TalkDemoWindow_State_CloseCenter(TalkWindow *t) {
    t->t++;
    t->top = (u8)((0x1f * (s16)t->t) / 0xa + 0x41);
    s_disp.wy1 = t->top;
    t->bottom = (u8)((-30 * (s16)t->t) / 0xa + 0x7e);
    s_disp.wy2 = t->bottom;
    if (t->t >= 0xa)
        close_done(t);
}
static void TalkDemoWindow_Create(void) {
    memset(&s_win, 0, sizeof(s_win));
    s_win.colour = (u16)TalkDemo_GetScriptHeader(s_main.script)->colour;
    s_win.fn = (TalkDemo_GetFlags(0) & 2) ? TalkDemoWindow_State_InitCenter : TalkDemoWindow_State_InitLow;
    s_win_on = 1;
}
static void TalkDemoWindow_Main(void) {
    if (!TalkDemo_IsActive()) {
        s_win_on = 0;
        return;
    }
    if (s_win.fn)
        s_win.fn(&s_win);
    if (TalkDemo_GetFlags(1) & 0x80000000u)
        s_win_on = 0;   /* TalkDemoWindow_Destructor: V-alarms cancelled */
}

/* ----------------------------------------------------------- dim */
static void TalkDemoDim_State_FadeIn(TalkDim *p);
static void TalkDemoDim_State_WaitClose(TalkDim *s);
static void TalkDemoDim_State_FadeOut(TalkDim *p);
static void TalkDemoDim_State_Init(TalkDim *t) {
    /* BLDCNT: brightness decrease of BG0+OBJ (stage) / BG3+OBJ (zone map) */
    t->t = 0;
    t->fn = TalkDemoDim_State_FadeIn;
}
static void TalkDemoDim_State_FadeIn(TalkDim *p) {
    p->t++;
    s_disp.evy = (s16)((p->t << 3) / 0x1e);
    if (p->t >= 0x1e) {
        p->fn = TalkDemoDim_State_WaitClose;
        TalkDemo_SetFlags(2, 4);
    }
}
static void TalkDemoDim_State_WaitClose(TalkDim *s) {
    if (TalkDemo_GetFlags(2) & 0x10) {
        TalkDemo_ClearFlags(2, 0x10);
        TalkDemo_SetFlags(2, 0x20);
        s->t = 0;
        s->fn = TalkDemoDim_State_FadeOut;
    }
}
static void TalkDemoDim_State_FadeOut(TalkDim *p) {
    p->t++;
    s_disp.evy = (s16)(8 - ((p->t << 3) / 0x1e));
    if (p->t >= 0x1e) {
        p->fn = NULL;
        TalkDemo_ClearFlags(2, 0x20);
        TalkDemo_SetFlags(2, 0x80000040);
    }
}
static void TalkDemoDim_Create(void) {
    memset(&s_dim, 0, sizeof(s_dim));
    s_dim.fn = TalkDemoDim_State_Init;
    s_dim_on = 1;
}
static void TalkDemoDim_Main(void) {
    if (!TalkDemo_IsActive()) {
        s_dim_on = 0;
        s_disp.evy = 0;
        return;
    }
    if (s_dim.fn)
        s_dim.fn(&s_dim);
    if (TalkDemo_GetFlags(2) & 0x80000000u) {
        s_dim_on = 0;   /* TalkDemoDim_Destructor: BLDCNT cleared */
        s_disp.evy = 0;
    }
}

/* ----------------------------------------------------------- main task */
static void TalkDemo_State_WaitWindowOpen(TalkMain *w);
static void TalkDemo_State_Start(TalkMain *w);
static void TalkDemo_State_Run(TalkMain *w);
static void TalkDemo_State_BeginClose(TalkMain *w);
static void TalkDemo_State_WaitClose(TalkMain *w);

static void TalkDemo_InitSkippable(void) {
    u32 f = g_cs_story.save_flags;
    switch (gTalkDemoId) {
    case 0xa: if (f & 0x20000) TalkDemo_SetFlags(0, 0x100); break;
    case 0xb: if (f & 0x40000) TalkDemo_SetFlags(0, 0x100); break;
    case 0xf: if (f & 0x80000) TalkDemo_SetFlags(0, 0x100); break;
    case 0x10: if (f & 0x100000) TalkDemo_SetFlags(0, 0x100); break;
    case 0x33: case 0x34: case 0x35: case 0x36: case 0x38: case 0x39:
        if (f & 0x1000) TalkDemo_SetFlags(0, 0x100);
        break;
    case 0xe:
        if (f & 0x1000) {
            u32 *pf = PlayerDemo_Flags();
            TalkDemo_SetFlags(0, 0x100);
            if (pf)
                *pf |= 0x100000;
        }
        break;
    }
}
static void TalkDemo_MarkSeen(void) {
    u32 *pf = PlayerDemo_Flags();
    switch (gTalkDemoId) {
    case 0xa: g_cs_result.set_save_flags |= 0x20000; break;
    case 0xb: g_cs_result.set_save_flags |= 0x40000; break;
    case 0xf: g_cs_result.set_save_flags |= 0x80000; break;
    case 0x10: g_cs_result.set_save_flags |= 0x100000; break;
    case 0x33: case 0x34: case 0x35: case 0x36: case 0x38: case 0x39:
        if ((TalkDemo_GetFlags(0) & 0x400) && pf)
            *pf |= 0x200000;
        break;
    case 0xe:
        if (pf) {
            *pf &= ~0x100000u;
            *pf &= ~0x800000u;
        }
        break;
    }
}
static void TalkDemo_LoadSound(TalkMain *w) {
    const s32 *e = TalkDemo_GetEntry();
    w->entry = e;
    if (!entry_ok(e)) {
        TalkDemo_SetFlags(0, 0x10);
        return;
    }
    if (!((u32)e[3] & 1))
        cs_snd_bank(e[1]);   /* Sound_LoadSoundArchive + NNS_SndArcLoadGroup(group) */
}
static void TalkDemo_PlayBGM(TalkMain *w) {
    s32 t1 = (s16)TalkDemo_GetScriptHeader(w->script)->bgm;
    const s16 *bgm = (const s16 *)cs_sec("td_bgm", NULL);
    if (!(TalkDemo_GetFlags(0) & 0x10) && t1 >= 0 && t1 < 0xa && t1 != 0 && t1 != 1 && t1 != 2 && bgm)
        cs_bgm_play(bgm[t1]);
}
static void TalkDemo_ReleaseSound(TalkMain *w) {
    if (!(TalkDemo_GetFlags(0) & 0x10) && entry_ok(w->entry) && !((u32)w->entry[3] & 1)) {
        cs_snd_stop_all();
        if (!((u32)w->entry[3] & 2))
            cs_snd_bank(-1);
    }
}
static void TalkDemo_Destructor(void) {
    TalkMain *w = &s_main;
    TalkDemo_MarkSeen();
    TalkDemo_ReleaseSound(w);
    free(w->script);
    w->script = NULL;
    s_main_on = 0;
    g_cs_freeze = 0;   /* System_Unfreeze */
    cs_log("talk %d end pages %d", gTalkDemoId, s_pages_run);
}
static void TalkDemo_State_Load(TalkMain *w) {
    w->script = TalkDemo_LoadScript(w->lang, (u32)gTalkDemoId, &w->script_size);
    if (!w->script || w->script_size < 0x1c || memcmp(w->script, "#BTK", 4) != 0) {
        cs_log("talk %d: script missing", gTalkDemoId);
        free(w->script);
        w->script = NULL;
        s_main_on = 0;
        g_cs_freeze = 0;
        return;
    }
    cs_log("talk %d start pages %d colour %04x bgm %d lang %d", gTalkDemoId, TalkDemo_GetScriptHeader(w->script)->pages,
           TalkDemo_GetScriptHeader(w->script)->colour, TalkDemo_GetScriptHeader(w->script)->bgm, w->lang);
    if (TalkDemo_GetScriptPage(w->script, 0)->mode == 3) {
        TalkDemo_SetFlags(0, 2);
        g_cs_bright = 0;   /* data_0208926c.unk8 = 0 */
    }
    TalkDemoScene_Create();
    memset(s_box_idx, 0, sizeof(s_box_idx));
    memset(s_frame_idx, 0, sizeof(s_frame_idx));
    memset(s_port_idx, 0, sizeof(s_port_idx));
    s_msg_on = 0;
    s_box_dirty = s_port_dirty = 1;
    TalkDemoWindow_Create();
    if (!(TalkDemo_GetFlags(0) & 2))
        TalkDemoDim_Create();
    TalkDemo_LoadSound(w);
    TalkDemo_PlayBGM(w);
    w->fn = TalkDemo_State_WaitWindowOpen;
}
static void TalkDemo_State_WaitWindowOpen(TalkMain *w) {
    if (TalkDemo_GetFlags(1) & 4)
        w->fn = TalkDemo_State_Start;
}
static void TalkDemo_State_Start(TalkMain *w) {
    TalkDemo_SetFlags(3, 1);
    w->fn = TalkDemo_State_Run;
}
static void TalkDemo_State_Run(TalkMain *w) {
    if (gPad.trg & PAD_A)
        TalkDemo_SetFlags(3, 0x10);
    if (gPad.held & PAD_R)
        TalkDemo_SetFlags(3, 0x40);
    if ((TalkDemo_GetFlags(0) & 0x200) && (TalkDemo_GetFlags(2) & 4)) {
        TalkDemo_SetFlags(0, 0x400);
        cs_log("talk %d skipped", gTalkDemoId);
        w->fn = TalkDemo_State_BeginClose;
    }
    if (TalkDemo_GetFlags(3) & 0x100)
        w->fn = TalkDemo_State_BeginClose;
}
static void TalkDemo_State_BeginClose(TalkMain *w) {
    TalkDemo_SetFlags(2, 0x10);
    TalkDemo_SetFlags(1, 0x10);
    if (!(TalkDemo_GetFlags(0) & 2))
        TalkDemo_SetFlags(3, 0x20000000);
    if (!(TalkDemo_GetFlags(0) & 0x10))
        cs_bgm_stop(0x1e);   /* TalkDemo_FadeOutBGM */
    w->fn = TalkDemo_State_WaitClose;
}
static void TalkDemo_State_WaitClose(TalkMain *w) {
    if (TalkDemo_GetFlags(1) & 0x40) {
        TalkDemo_SetFlags(3, 0x40000000);
        w->fn = NULL;
        w->flags |= 0x80000000u;
    }
}
static void TalkDemo_CheckSkip(void) {
    if ((TalkDemo_GetFlags(0) & 0x100) && (gPad.held & PAD_START))
        TalkDemo_SetFlags(0, 0x200);
}

static void talk_create(int map) {
    TalkDemo_Kill();
    memset(&s_main, 0, sizeof(s_main));
    memset(&s_disp, 0, sizeof(s_disp));
    memset(&s_rocks.a, 0, sizeof(s_rocks.a));
    s_rocks.active = 0;
    s_main_on = 1;
    s_pages_run = 0;
    s_main.lang = cs_lang();
    s_main.fn = TalkDemo_State_Load;
    g_cs_freeze = 1;   /* System_Freeze(1, 1, 1): the scene behind waits (cs_internal.h) */
    TalkDemo_InitSkippable();
    /* TalkDemo_InitDisplay(ZoneMap): BG1 22x10 box (prio 0), BG2 affine (prio 1) */
    TalkDemo_ResetBg1Scroll(NULL);
    TalkDemo_ResetBg2Affine(NULL);
    if (map)
        TalkDemo_SetFlags(0, 1);
}
void TalkDemo_Create(void) { talk_create(0); }
void TalkDemo_CreateForZoneMap(void) { talk_create(1); }

void TalkDemo_Kill(void) {
    if (s_main_on && s_main.script)
        free(s_main.script);
    s_main.script = NULL;
    if (s_main_on)
        g_cs_freeze = 0;
    s_main_on = s_scene_on = s_win_on = s_dim_on = s_bg2win_on = 0;
    s_rocks.active = 0;
    cs_res_free(s_scene.prompt_res);
    s_scene.prompt_res = NULL;
    cs_res_free(s_rocks.res);
    s_rocks.res = NULL;
    cs_tex_free(&s_box_tex);
    cs_tex_free(&s_port_tex);
    s_ndeferred = 0;
}

void TalkDemo_RunFrame(void) {
    int i, n;
    if (!s_main_on)
        return;
    s_prompt_drawn = 0;
    /* TalkDemo_Main (0xeffd) */
    TalkDemo_CheckSkip();
    if (s_main.fn)
        s_main.fn(&s_main);
    if (!s_main_on)
        return;
    /* 0xeffe: scene, window, dim, rocks (creation order) */
    if (s_scene_on)
        TalkDemoScene_Main();
    if (s_win_on)
        TalkDemoWindow_Main();
    if (s_dim_on)
        TalkDemoDim_Main();
    TalkDemoRocks_Main();
    /* 0xf000: deferred calls; 0xf00a: BG2 window */
    n = s_ndeferred;
    s_ndeferred = 0;
    for (i = 0; i < n; i++)
        s_deferred[i]();
    if (s_bg2win_on)
        TalkDemoSceneBg2Window_Main();
    if (s_main.flags & 0x80000000u)
        TalkDemo_Destructor();
}

/* ----------------------------------------------------------- draw */
static void upload(CsTex *t, int *dirty, const u8 *idx, int w, int h, const u16 *pal, int flip) {
    if (!t->argb || t->w != w || t->h != h) {
        cs_tex_free(t);
        cs_tex_init(t, w, h);
        *dirty = 1;
    }
    if (*dirty) {
        if (flip) {
            static u8 row[256];
            int y, x;
            for (y = 0; y < h; y++) {
                for (x = 0; x < w; x++)
                    row[x] = idx[y * w + (w - 1 - x)];
                cs_tex_put(t, 0, y, w, 1, row, w, pal);
            }
        } else {
            cs_tex_put(t, 0, 0, w, h, idx, w, pal);
        }
        *dirty = 0;
    }
}

/* rows of the band and their colours (TalkDemoWindowRaster_Main limits:
 * colour 0 from top-1, pal[1] from top+2, 0 from bottom-4); the columns are
 * the union of WIN0H and WIN1H (x1 > x2 wraps past 255 like the DS) */
static int win_span(int x1, int x2, int out[4]) {
    if (x1 <= x2) {
        out[0] = x1;
        out[1] = x2;
        return 1;
    }
    out[0] = x1;
    out[1] = 256;
    out[2] = 0;
    out[3] = x2;
    return 2;
}
static void band_rows(float x0, float x1) {
    int top = s_disp.wy1, bot = s_disp.wy2, lim1 = s_win.top + 2, lim2 = s_win.bottom - 4;
    u32 c = s_win.pal[1], col;
    if (x1 <= x0)
        return;
    if (!s_win.raster) {
        cs_quad_flat(x0, top, x1 - x0, bot - top, 0xff000000u | (cs_bgr_to_argb(s_disp.backdrop) & 0x7fff));
        return;
    }
    col = 0xff000000u | ((c & 31) * 255 / 31) << 16 | ((c >> 5 & 31) * 255 / 31) << 8 | ((c >> 10 & 31) * 255 / 31);
    if (lim1 >= lim2) {
        cs_quad_flat(x0, top, x1 - x0, bot - top, 0xff000000u);
        return;
    }
    cs_quad_flat(x0, top, x1 - x0, lim1 - top, 0xff000000u);
    cs_quad_flat(x0, lim1, x1 - x0, lim2 - lim1, col);
    cs_quad_flat(x0, lim2, x1 - x0, bot - lim2, 0xff000000u);
}
static void draw_band(void) {
    int sp[8], n, i;
    if (!s_disp.win_on || s_disp.wy2 <= s_disp.wy1)
        return;
    n = win_span(s_disp.w0x1, s_disp.w0x2, sp);
    n += win_span(s_disp.w1x1, s_disp.w1x2, sp + 2 * n);
    for (i = 0; i < n; i++)
        band_rows((float)sp[2 * i], (float)sp[2 * i + 1]);
}

void TalkDemo_Draw(void) {
    int full = TalkDemo_GetFlags(0) & 2;
    if (!s_main_on)
        return;
    cs_view_full();
    /* TalkDemoDim: BLDY brightness decrease of the scene behind (evy/16) */
    if (s_disp.evy > 0)
        cs_quad_flat(0, 0, 256, 192, (u32)(s_disp.evy * 255 / 16) << 24);
    draw_band();
    /* BG2: portrait (low) / full image clipped to the band's colour rows (centre) */
    upload(&s_port_tex, &s_port_dirty, s_port_idx, s_port_w ? s_port_w : 80, s_port_h ? s_port_h : 80,
           s_pal_bg2, s_port_flip);
    if (full) {
        if (s_disp.win_on && s_win.raster) {
            float y0 = s_win.top + 2, y1 = s_win.bottom - 4;
            if (y1 > y0) {
                g_cs_view.clip_y0 = y0 * 2.5f;
                g_cs_view.clip_y1 = y1 * 2.5f;
                cs_quad_tex(&s_port_tex, -s_disp.bg2_x, -s_disp.bg2_y, s_port_tex.w, s_port_tex.h, 0, 0, s_port_tex.w,
                            s_port_tex.h, 0xffffffff);
                cs_view_full();
            }
        }
    } else if (s_disp.out_bg2 || s_disp.in_bg2) {
        cs_quad_affine(&s_port_tex, s_disp.m, 0, 0, s_disp.bg2_x, s_disp.bg2_y, 0xffffffff);
    }
    /* BG1: box (512x256 map, wraps) */
    upload(&s_box_tex, &s_box_dirty, s_box_idx, 176, 80, s_pal_bg1, 0);
    {
        int bx = (-s_disp.bg1_x) & 511, by = (-s_disp.bg1_y) & 255;
        if (bx > 256)
            bx -= 512;
        if (by > 192)
            by -= 256;
        if (bx < 256 && bx + 176 > 0 && by < 192 && by + 80 > 0)
            cs_quad_tex(&s_box_tex, bx, by, 176, 80, 0, 0, 176, 80, 0xffffffff);
    }
    /* OBJ: hidden inside the window (WININ obj 0) */
    {
        int clip = s_disp.win_on && s_disp.wy2 > s_disp.wy1;
        int pass;
        for (pass = 0; pass < (clip ? 2 : 1); pass++) {
            if (clip) {
                g_cs_view.clip_y0 = pass == 0 ? 0 : s_disp.wy2 * 2.5f;
                g_cs_view.clip_y1 = pass == 0 ? s_disp.wy1 * 2.5f : 480;
            }
            if (s_prompt_drawn)
                AnimatorSprite_Draw(&s_scene.prompt);
            if (s_rocks.active) {
                int i;
                for (i = 0; i < 6; i++) {
                    CsSpr *sp = &s_rocks.spr[s_rocks.d[i] & 1];
                    sp->x = (s16)(s_rocks.a[i][0] >> 8);
                    sp->y = (s16)(s_rocks.a[i][1] >> 8);
                    AnimatorSprite_Draw(sp);
                }
            }
        }
        cs_view_full();
    }
}

int TalkDemo_PageCount(int id) {
    u32 size;
    u8 *b = TalkDemo_LoadScript(cs_lang(), (u32)id, &size);
    int n = b && size >= 0x1c ? (int)TalkDemo_GetScriptHeader(b)->pages : -1;
    free(b);
    return n;
}

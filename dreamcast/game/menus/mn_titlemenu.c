/*
 * TitleMenu_* (src/game/Menus/TitleMenu/): the paged main menu (GAME START,
 * VS, EXTRA page, OPTIONS), its button key-frame animations (mnu_button.ama),
 * the colour-wipe background slide (mnu_wipe.ama) and the exits.
 *
 * Touch -> pad: the original takes UP/DOWN/A/B and also touches on the item
 * buttons and the back button; the pad path is the same code, the touch path
 * is not needed. VS: the VS page and its wireless sub-page open as on the DS;
 * their modes (download play, host, join) start Battle Play on this Dreamcast
 * (two controllers): the wireless warning (MsgBox 0) and the VS lobby
 * (VsMenu, VsConnect) have no counterpart, the item goes on as the lobby's
 * host exit does (gTitleMenuExitTable[2] TitleMenu_StartVsHost ->
 * PlayerSelectMenu_StartVs), fading to white like the select that follows.
 * The VS page's two buttons are TWO PLAYERS and ONE PLAYER (Vs CPU), spelled
 * from the original English labels (TitleMenu_DrawVsButton).
 */
#include <stdlib.h>
#include "mn_internal.h"

enum { TM_ITEM_NONE = 0x14 };

typedef struct TitleMenuWork {
    s32 state;            /* +0 exit selection (9 = stay) */
    s32 unk4;             /* +4 frame counter */
    s32 unk8;             /* +8 player select page unlocked */
    s32 unk258;           /* BG slide finished */
    MnBgImg *bg_main3;    /* bgc: mnu_titl_bg01_a or bg03_a (main BG3) */
    MnBgImg *bg_vs;       /* bg4c: mnu_titl_bg02_a (main BG3 while the VS menu is attached) */
    MnBgImg *bg_sub3;     /* bg8c: mnu_titl_bg01_b (sub BG3) */
    MnBgImg *wipe[3];     /* objs[3]: mnu_titl_wipe on BG0..2 of both engines (own palettes) */
    MnSpr sprites[19];    /* +0x25c */
    s32 ids[19];          /* +0x7b4 */
    u16 pal[3];           /* +0x800 */
    MnRes *res808, *res80c;
    MnRes *res_vs;           /* Battle Play's buttons (items 0xd / 0xe): mnu_titl_eng.bac (res80c in English) */
    void *res824, *res828;   /* mnu_wipe.ama, mnu_button.ama */
} TitleMenuWork;

typedef void (*TitleMenuCb)(TitleMenuWork *p, s32 flag, s32 arg);

typedef struct {
    s32 id;               /* sprite index 9..18, page title 2..8, 0x14 none */
    s32 unk4;             /* flags: 1 fade white, 2 fade black, 4 message box first */
    KeyframeAnim anim;
    s32 unk34;            /* argument for cb */
    TitleMenuCb cb;
} TitleMenuItem;

typedef struct {
    s16 h[8];             /* h[1] first item y, h[3] item spacing */
    s32 unk10;            /* 1: opened "forward" */
    TitleMenuItem items[6];   /* 0..3 buttons, 4 back button, 5 page title */
    s32 unk17c, unk180, unk184[4];   /* BG slide colour sets: this page, back, per item */
} TitleMenuPage;

typedef struct {
    s32 unk0;
    s32 sel;
    u32 timer;
    s32 unkc;
    TitleMenuPage m;      /* +0x10 */
    TitleMenuWork *param; /* +0x1a4 */
} TitleMenuPageWork;

static TitleMenuWork *s_tm;   /* the TitleMenu task work (gTaskCurrent of TitleMenu_Main) */

/* ----------------------------------------------------------- fades */
typedef struct {
    s16 from, to;
    u32 t, dur, delay;
} TitleMenuFadeWork;

static void TitleMenuFadeTask_Main(void) {
    TitleMenuFadeWork *w = TASK_WORK(TitleMenuFadeWork);
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

static TitleMenuFadeWork *tm_fade(void) {
    return (TitleMenuFadeWork *)Task_Create(TitleMenuFadeTask_Main, NULL, 0, 0x4000, 0x1000,
                                            sizeof(TitleMenuFadeWork))->work;
}

static void TitleMenu_CreateFadeTask(s32 a, s32 b) {
    TitleMenuFadeWork *w;
    Task_DestroyRange(0x4000, 0x4000);
    if (a != 0) {
        if (BRIGHT_MAIN != 16 && BRIGHT_MAIN != -16)
            BRIGHT_MAIN = 16;
        BRIGHT_SUB = BRIGHT_MAIN;
        w = tm_fade();
        w->from = BRIGHT_MAIN;
        w->to = 0;
        w->t = 0;
        w->dur = (u32)a;
        w->delay = (u32)b;
    }
}

static void TitleMenu_StartFadeToBlack(s32 a, s32 b) {
    TitleMenuFadeWork *w;
    Task_DestroyRange(0x4000, 0x4000);
    if (a != 0) {
        w = tm_fade();
        w->from = 0;
        w->to = -16;
        w->t = 0;
        w->dur = (u32)a;
        w->delay = (u32)b;
        BRIGHT_SUB = 0;
        BRIGHT_MAIN = BRIGHT_SUB;
    }
}

static void TitleMenu_StartFadeToWhite(s32 a, s32 b) {
    TitleMenuFadeWork *w;
    Task_DestroyRange(0x4000, 0x4000);
    if (a != 0) {
        w = tm_fade();
        w->from = 0;
        w->to = 16;
        w->t = 0;
        w->dur = (u32)a;
        w->delay = (u32)b;
        BRIGHT_SUB = 0;
        BRIGHT_MAIN = BRIGHT_SUB;
    }
}

/* ------------------------------------------------------- BG slide */
typedef struct {
    u16 ids[6];           /* colour per anim */
    s32 vals[6];          /* priority per anim */
    KeyframeAnim anims[6];
    TitleMenuWork *parent;
} TitleMenuSlideWork;

typedef struct {
    u16 pal[3];
    struct { s16 x, y; } pos[3];
    u32 prio[3];
} TitleMenuSlideList;

static void TitleMenu_ApplyBgSlideLayers(const TitleMenuSlideList *a, TitleMenuWork *b) {
    s32 order[3], i, j, k, idx, x, y;
    for (i = 0; i < 3; i++) {
        order[i] = -1;
        for (j = 0; j < 3; j++) {
            for (k = 0; k < i; k++)
                if (j == order[k])
                    break;
            if (k < i)
                continue;
            if (order[i] < 0 || a->prio[order[i]] > a->prio[j])
                order[i] = j;
        }
    }
    for (i = 0; i < 3; i++) {
        idx = order[i];
        x = a->pos[idx].x;
        y = a->pos[idx].y;
        b->pal[idx] = a->pal[idx];
        mn_bg_set_colors(b->wipe[idx], 1, 1, &b->pal[idx]);
        if (x < 0)
            x += (-x / 512 + 1) * 512;
        if (y < 0)
            y += (-y / 512 + 1) * 512;
        x %= 512;
        y %= 512;
        x = 512 - x;
        y = 512 - y;
        gDisplayMain[idx].x = (u16)x;
        gDisplayMain[idx].y = (u16)y;
        gDisplaySub[idx].x = (u16)x;
        gDisplaySub[idx].y = (u16)y;
    }
}

static void TitleMenuBgSlideTask_Main(void) {
    TitleMenuSlideWork *w = TASK_WORK(TitleMenuSlideWork);
    s32 i, n = 0;
    BOOL done = TRUE, r;
    s16 x;
    TitleMenuSlideList l;
    memset(&l, 0, sizeof(l));
    for (i = 0; i < 6; i++) {
        KeyframeAnim_Update(&w->anims[i]);
        r = KeyframeAnim_IsFinished(&w->anims[i]);
        x = w->anims[i].x;
        if (x > -0x100 && x < 0x100 && n < 3) {
            l.pos[n].x = x;
            l.pos[n].y = 0;
            l.pal[n] = w->ids[i];
            l.prio[n] = (u32)w->vals[i];
            n++;
        }
        if (!r)
            done = FALSE;
    }
    for (; n < 3; n++) {
        l.pos[n].x = -0x100;
        l.pal[n] = 0;
        l.prio[n] = (u32)n;
    }
    TitleMenu_ApplyBgSlideLayers(&l, w->parent);
    if (done) {
        w->parent->unk258 = 1;
        Task_Destroy(gTaskCurrent);
    }
}

static TitleMenuSlideWork *tm_slide_create(TitleMenuWork *p) {
    TitleMenuSlideWork *w;
    Task_DestroyRange(0x3000, 0x3000);
    w = (TitleMenuSlideWork *)Task_Create(TitleMenuBgSlideTask_Main, NULL, 0, 0x3000, 0x1000,
                                          sizeof(TitleMenuSlideWork))->work;
    w->parent = p;
    w->vals[0] = 0;
    w->vals[1] = 10;
    w->vals[2] = 20;
    w->vals[3] = 30;
    w->vals[4] = 40;
    w->vals[5] = 50;
    return w;
}

/* data_ov001_0231616c[b * 4 + k] */
static u16 slide_col(s32 b, int k) { return g_mn_data.tm_slide[b][k]; }

static void TitleMenu_CreateBgSlideTaskAtMidpoint(TitleMenuWork *p, s32 b) {
    TitleMenuSlideWork *w = tm_slide_create(p);
    s32 i;
    for (i = 0; i < 6; i++) {
        KeyframeAnim_Init(&w->anims[i], p->res824, (u32)i);
        KeyframeAnim_SetFrame(&w->anims[i], (KeyframeAnim_SeqFrames(p->res824, (u32)i) << 12) / 2);
    }
    if (b >= 7)
        b = 0;
    w->ids[0] = slide_col(b, 1);
    w->ids[1] = slide_col(b, 0);
    w->ids[2] = slide_col(b, 3);
    w->ids[3] = slide_col(b, 2);
    w->ids[4] = slide_col(b, 1);
    w->ids[5] = slide_col(b, 0);
    p->unk258 = 0;
}

static void TitleMenu_CreateBgSlideTaskForward(TitleMenuWork *p, s32 a, s32 b) {
    TitleMenuSlideWork *w = tm_slide_create(p);
    s32 i;
    for (i = 0; i < 6; i++)
        KeyframeAnim_Init(&w->anims[i], p->res824, (u32)i);
    if (a >= 7)
        a = 0;
    if (b >= 7)
        b = 0;
    w->ids[0] = slide_col(a, 1);
    w->ids[1] = slide_col(a, 0);
    w->ids[2] = slide_col(b, 3);
    w->ids[3] = slide_col(b, 2);
    w->ids[4] = slide_col(b, 1);
    w->ids[5] = slide_col(b, 0);
    p->unk258 = 0;
}

static void TitleMenu_CreateBgSlideTaskReverse(TitleMenuWork *p, s32 a, s32 b) {
    TitleMenuSlideWork *w = tm_slide_create(p);
    s32 i;
    for (i = 0; i < 6; i++) {
        KeyframeAnim_Init(&w->anims[i], p->res824, (u32)i);
        KeyframeAnim_PlayReverse(&w->anims[i]);
    }
    if (a >= 7)
        a = 0;
    if (b >= 7)
        b = 0;
    w->ids[0] = slide_col(b, 1);
    w->ids[1] = slide_col(b, 0);
    w->ids[2] = slide_col(a, 3);
    w->ids[3] = slide_col(a, 2);
    w->ids[4] = slide_col(a, 1);
    w->ids[5] = slide_col(a, 0);
    p->unk258 = 0;
}

/* ------------------------------------------------------------ drawing */
static void TitleMenu_DrawBackButton(s32 x, s32 y, s32 mode, u32 t) {
    MnSpr *s;
    s32 anim;
    (void)t;
    switch (mode) {
    case 0:
        anim = 0;
        break;
    case 2:
        anim = 1;
        break;
    default:
        return;
    }
    s = &s_tm->sprites[0];
    if (s_tm->ids[0] != anim) {
        AnimatorSprite_SetAnimation(s, (u16)anim);
        s_tm->ids[0] = anim;
    }
    s->x = (s16)x;
    s->y = (s16)y;
    s->flags &= ~(u32)SPR_LOOP;
    s->flags &= ~(u32)SPR_STOPPED;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}

/* Battle Play's two buttons, the original VS page's items 0xd / 0xe (DS WIRELESS BATTLE / DS DOWNLOAD
 * BATTLE) relabelled TWO PLAYERS (two controllers) and ONE PLAYER (Vs CPU). The labels are spelled at
 * run time with letters cut from the English button labels of mnu_titl_eng.bac (the button font of the
 * disc has no digits, no V and no U: "2 PLAYERS" / "VS CPU" cannot be built). Each source label is split
 * into words at the columns equal to the button's background (a 2-pixel dither: the reference columns
 * 6 / 7) and each word into letters at the columns holding only background, halo or outline (the
 * colours of the word's first two columns); a letter is its fill columns and one outline column on each
 * side. A label must give the letter counts of its known text, else it is not used. The button (frame,
 * background) is the same state's image of item 0xd; the letters come from the same state's images
 * (normal, selected, the flashing frames of the confirm) of the other items, laid out with the font's
 * spacing (word space as in BATTLE PLAY) and moved by even columns only (the dither's phase), centred.
 * Every language shows the English words (other languages load mnu_titl_eng.bac for these two buttons);
 * if a letter is missing the original label is drawn. */
#define TM_LBL_MAXP 48
#define TM_LBL_CACHE 12
typedef struct {
    const MnRes *res;
    int lbl, anim, frame, n;
    MnImgPiece p[TM_LBL_MAXP];
} TmVsLabel;
static TmVsLabel s_tm_lbl[TM_LBL_CACHE];
static int s_tm_lbl_next;
static const char *const s_tm_vs_text[2] = { "TWO PLAYERS", "ONE PLAYER" };
static const struct { s8 item; const char *text; } s_tm_glyph_src[7] = {
    { 9, "GAMEPLAY" }, { 10, "BATTLE PLAY" }, { 11, "TIME ATTACK" }, { 12, "OPTIONS" },
    { 15, "ACT" },     { 16, "BOSS" },        { 13, "DS WIRELESS BATTLE" },
};
static int tm_col_is_bg(const MnRes *r, int img, int x, int h) {
    int y, ref = 6 + (x & 1);
    for (y = 0; y < h; y++)
        if (mn_image_texel(r, img, x, y) != mn_image_texel(r, img, ref, y))
            return 0;
    return 1;
}
/* the image of item `item`'s button in the state (offset 0..2 of its three animations) and frame */
static int tm_item_image(const MnRes *r, int item, int state, int frame) {
    const MnAnimDef *a;
    int an = 7 + (item - 9) * 3 + state;
    if (an >= r->n_anims)
        return -1;
    a = &r->anims[an];
    if (a->count == 0)
        return -1;
    return r->frames[a->first + (frame < a->count ? frame : a->count - 1)].image;
}
/* words (columns [ws, we)) of a label image between its frames; 0 when not a plain button */
static int tm_words(const MnRes *r, int img, int seg[][2], int max) {
    int w, h, x, n = 0;
    if (img < 0 || img >= r->n_images || mn_image_texel(r, img, 6, 0) < 0)
        return 0;
    w = r->images[img].w;
    h = r->images[img].h;
    if (w < 32)
        return 0;
    for (x = 8; x < w - 6;) {
        if (tm_col_is_bg(r, img, x, h)) {
            x++;
            continue;
        }
        if (n == max)
            return 0;
        seg[n][0] = x;
        while (x < w - 6 && !tm_col_is_bg(r, img, x, h))
            x++;
        seg[n++][1] = x;
    }
    return n;
}
/* a column of a word with no letter fill: background, or the outline / halo colours o[0..no) */
static int tm_col_is_gap(const MnRes *r, int img, int x, int h, const int *o, int no) {
    int y, k, ref = 6 + (x & 1);
    for (y = 0; y < h; y++) {
        int t = mn_image_texel(r, img, x, y);
        if (t == mn_image_texel(r, img, ref, y))
            continue;
        for (k = 0; k < no && o[k] != t; k++)
            ;
        if (k == no)
            return 0;
    }
    return 1;
}
typedef struct { s16 image, x0, x1; } TmGlyph;
/* the letters of the source labels in one button state: glyph[c - 'A'], and the word space */
static void tm_glyphs(const MnRes *r, int state, int frame, TmGlyph *glyph, int *space) {
    int k;
    memset(glyph, 0, sizeof(TmGlyph) * 26);
    *space = 6;
    for (k = 0; k < 7; k++) {
        const char *t = s_tm_glyph_src[k].text;
        int img = tm_item_image(r, s_tm_glyph_src[k].item, state, frame);
        int seg[8][2], nw = tm_words(r, img, seg, 8), o[8], no = 0, i, x, y, h, wi, nl;
        TmGlyph got[24];
        const char *c;
        if (nw <= 0)
            continue;
        h = r->images[img].h;
        for (x = seg[0][0]; x < seg[0][0] + 2; x++)   /* the outline and halo colours */
            for (y = 0; y < h; y++) {
                int tx = mn_image_texel(r, img, x, y), ref = mn_image_texel(r, img, 6 + (x & 1), y);
                if (tx == ref)
                    continue;
                for (i = 0; i < no && o[i] != tx; i++)
                    ;
                if (i == no && no < 8)
                    o[no++] = tx;
            }
        nl = 0;
        c = t;
        for (wi = 0; wi < nw; wi++) {
            int wl = 0;
            for (x = seg[wi][0]; x < seg[wi][1];) {
                int s0;
                if (tm_col_is_gap(r, img, x, h, o, no)) {
                    x++;
                    continue;
                }
                s0 = x;
                while (x < seg[wi][1] && !tm_col_is_gap(r, img, x, h, o, no))
                    x++;
                if (nl < 24) {
                    got[nl].image = (s16)img;
                    got[nl].x0 = (s16)(s0 - 1);
                    got[nl].x1 = (s16)(x + 1);
                }
                nl++;
                wl++;
            }
            while (*c == ' ')   /* the text's word must have as many letters */
                c++;
            for (i = 0; c[i] && c[i] != ' '; i++)
                ;
            if (i != wl)
                break;
            c += i;
        }
        while (*c == ' ')
            c++;
        if (wi != nw || *c || nl > 24)
            continue;
        if (nw >= 2 && k == 1)   /* BATTLE PLAY: the font's word space */
            *space = got[6].x0 - got[5].x1;
        for (i = 0, c = t; *c; c++) {
            if (*c == ' ')
                continue;
            if (*c >= 'A' && *c <= 'Z' && glyph[*c - 'A'].x1 == 0)
                glyph[*c - 'A'] = got[i];
            i++;
        }
    }
}
/* the pieces of label `lbl` for the button sprite's state: frame left, background, letters, frame right */
static int tm_vs_label_build(TmVsLabel *L, const MnRes *r, int lbl, int state, int frame) {
    TmGlyph g[26];
    int space, base = tm_item_image(r, 13, state, frame), seg[8][2], nw, w, h, cs = -1, cl = 0, side, a, b;
    int pass, x, e, tx = 0, tw = 0;
    const char *c;
    L->n = 0;
    nw = tm_words(r, base, seg, 8);
    if (nw <= 0)
        return 0;
    w = r->images[base].w;
    h = r->images[base].h;
    tm_glyphs(r, state, frame, g, &space);
    for (c = s_tm_vs_text[lbl]; *c; c++)
        if (*c != ' ' && (*c < 'A' || *c > 'Z' || g[*c - 'A'].x1 == 0))
            return 0;
    /* the longest background run (even start / length) left of the first or right of the last word */
    for (side = 0; side < 2; side++) {
        a = side ? seg[nw - 1][1] : 6;
        b = side ? w - 6 : seg[0][0];
        if ((a & 1) != 0)
            a++;
        if (((b - a) & ~1) > cl) {
            cs = a;
            cl = (b - a) & ~1;
        }
    }
    if (cl < 2)
        return 0;
#define TM_LPIECE(IMG, SX, W, DX)                                                              \
    do {                                                                                       \
        if (L->n < TM_LBL_MAXP) {                                                              \
            MnImgPiece *q_ = &L->p[L->n++];                                                    \
            q_->image = (s16)(IMG);                                                            \
            q_->sx = (s16)(SX);                                                                \
            q_->sy = 0;                                                                        \
            q_->w = (s16)(W);                                                                  \
            q_->h = (s16)h;                                                                    \
            q_->dx = (s16)(DX);                                                                \
            q_->dy = 0;                                                                        \
        }                                                                                      \
    } while (0)
/* background columns [A, B) (the dither's phase kept) */
#define TM_FILL(A, B)                                                                          \
    do {                                                                                       \
        int a_ = (A), b_ = (B);                                                                \
        if ((a_ & 1) != 0 && a_ < b_) {                                                        \
            TM_LPIECE(base, cs + 1, 1, a_);                                                    \
            a_++;                                                                              \
        }                                                                                      \
        while (a_ < b_) {                                                                      \
            int n_ = b_ - a_ < cl ? b_ - a_ : cl;                                              \
            TM_LPIECE(base, cs, n_, a_);                                                       \
            a_ += n_;                                                                          \
        }                                                                                      \
    } while (0)
    /* pass 0 measures the text from column 0, pass 1 lays it out centred */
    for (pass = 0; pass < 2; pass++) {
        x = pass ? tx : 0;
        e = 8;   /* the end of the last piece */
        if (pass)
            TM_LPIECE(base, 0, 8, 0);
        for (c = s_tm_vs_text[lbl]; *c; c++) {
            const TmGlyph *q;
            int nx;
            if (*c == ' ') {
                x += space;
                continue;
            }
            q = &g[*c - 'A'];
            nx = x;
            if (((nx - q->x0) & 1) != 0)   /* moved by an even number of columns */
                nx++;
            if (pass) {
                TM_FILL(e, nx);
                TM_LPIECE(q->image, q->x0, q->x1 - q->x0, nx);
            }
            x = e = nx + q->x1 - q->x0;
        }
        if (!pass) {
            tw = x;
            tx = (w - tw) / 2;
            if (tx < 8 || tw > w - 16)
                return L->n = 0;
        } else {
            if (x > w - 8)
                return L->n = 0;
            TM_FILL(x, w - 8);
            TM_LPIECE(base, w - 8, 8, w - 8);
        }
    }
#undef TM_FILL
#undef TM_LPIECE
    return L->n < TM_LBL_MAXP ? L->n : (L->n = 0);
}
static void TitleMenu_DrawVsButton(MnSpr *s, int lbl) {
    TmVsLabel *L = NULL;
    int k, state = (s->anim - 7) % 3;
    for (k = 0; k < TM_LBL_CACHE && !L; k++)
        if (s_tm_lbl[k].res == s->res && s_tm_lbl[k].lbl == lbl && s_tm_lbl[k].anim == s->anim &&
            s_tm_lbl[k].frame == s->frame)
            L = &s_tm_lbl[k];
    if (!L) {   /* each button's states and frames are cached */
        L = &s_tm_lbl[s_tm_lbl_next++ % TM_LBL_CACHE];
        L->res = s->res;
        L->lbl = lbl;
        L->anim = s->anim;
        L->frame = s->frame;
        if (s->res && s->anim >= 7 && strcmp(s->res->id, "act__mnu_titl_eng") == 0)
            tm_vs_label_build(L, s->res, lbl, state, s->frame);
        else
            L->n = 0;
    }
    if (L->n > 0)
        AnimatorSprite_DrawImagePieces(s, L->p, L->n);
    else
        AnimatorSprite_Draw(s);
}

static void TitleMenu_DrawItemButton(s32 idx, s16 x, s16 y, s32 mode, u32 t) {
    s32 base = 0, anim = 0;
    MnSpr *s;
    switch (mode) {
    case 0:
        base = 0;
        break;
    case 1:
    case 3:
        base = 1;
        break;
    case 2:
        base = 2;
        break;
    }
    if (idx >= 9 && idx <= 18)
        anim = base + 7 + (idx - 9) * 3;
    if (mode == 1 || mode == 2) {
        s = &s_tm->sprites[1];
        if (t < 7 && mode != 2) {
            if (s_tm->ids[1] != 4 || t == 0) {
                AnimatorSprite_SetAnimation(s, 4);
                s_tm->ids[1] = 4;
            }
        } else if (s_tm->ids[1] != 3) {
            AnimatorSprite_SetAnimation(s, 3);
            s_tm->ids[1] = 3;
        }
        s->x = x;
        s->y = y;
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        if (t >= 7 && mode != 2) {
            if ((t - 7) % 60 < 40)
                AnimatorSprite_Draw(s);
        } else {
            s->x = x;
            s->y = y;
            AnimatorSprite_ProcessAnimation(s, 0, 0);
            AnimatorSprite_Draw(s);
        }
    }
    if (idx < 0 || idx >= 19)
        return;
    s = &s_tm->sprites[idx];
    if (anim != s_tm->ids[idx]) {
        AnimatorSprite_SetAnimation(s, (u16)anim);
        s_tm->ids[idx] = anim;
    }
    s->x = x;
    s->y = y;
    s->flags |= SPR_LOOP;
    s->flags &= ~(u32)SPR_STOPPED;
    s->flags |= 0x40;
    s->flags |= 0x20;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    if (idx == 0xd || idx == 0xe)
        TitleMenu_DrawVsButton(s, idx - 0xd);
    else
        AnimatorSprite_Draw(s);
}

/* TitleMenu_Unk_ov001_022d7ac4: the page title on the top screen */
static void TitleMenu_DrawPageTitle(s32 idx, s16 x, s16 y) {
    MnSpr *s;
    s32 anim;
    if (idx < 2 || idx > 8)
        return;
    anim = idx - 2;
    s = &s_tm->sprites[idx];
    if (anim != s_tm->ids[idx]) {
        AnimatorSprite_SetAnimation(s, (u16)anim);
        s_tm->ids[idx] = anim;
    }
    s->x = x;
    s->y = y;
    s->flags &= ~(u32)SPR_LOOP;
    s->flags &= ~(u32)SPR_STOPPED;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}

static void tm_draw_items(TitleMenuPageWork *w, int animate, int sel_mode, u32 t, int back_mode, BOOL *done) {
    s16 y = w->m.h[1];
    s32 i;
    TitleMenuItem *it;
    s_tm = w->param;
    for (i = 0, it = w->m.items; i < 6; it++, i++) {
        if (it->id == TM_ITEM_NONE)
            continue;
        if (animate) {
            KeyframeAnim_Update(&it->anim);
            if (!KeyframeAnim_IsFinished(&it->anim))
                *done = FALSE;
        }
        if (i == 4) {
            TitleMenu_DrawBackButton(it->anim.x, it->anim.y, back_mode, t);
        } else if (i == 5) {
            TitleMenu_DrawPageTitle(it->id, it->anim.x, it->anim.y);
        } else {
            if (w->sel != i)
                TitleMenu_DrawItemButton(it->id, it->anim.x, y, 0, 0);
            else
                TitleMenu_DrawItemButton(it->id, it->anim.x, y, sel_mode, t);
            y += w->m.h[3];
        }
    }
}

/* ------------------------------------------------------------ page states */
static void TitleMenu_State_Select(void);
static void TitleMenu_State_Confirm(void);

/* TitleMenu_State_Unk_ov001_022d85d0: close animations, then the item callback */
static void TitleMenu_State_Close(void) {
    TitleMenuPageWork *w = TASK_WORK(TitleMenuPageWork);
    BOOL done = TRUE;
    TitleMenuItem *it;
    tm_draw_items(w, 1, 3, 0, 0, &done);
    if (done == TRUE) {
        it = &w->m.items[w->sel];
        if (it->cb != NULL)
            it->cb(w->param, w->m.unk10, it->unk34);
        Task_Destroy(gTaskCurrent);
    }
}

static void TitleMenu_Unk_ov001_022d7dbc(TitleMenuPage *p, TitleMenuWork *r);
static void TitleMenu_Unk_ov001_022d7e60(TitleMenuPage *p, TitleMenuWork *r);

/* TitleMenu_State_Unk_ov001_022d86b0 (8-frame flash of a touched back
 * button) is reached only from a touch; B goes straight to the close state, as
 * in the original. */

/* TitleMenu_State_Unk_ov001_022d8798: confirm flash (30 frames) */
static void TitleMenu_State_Confirm(void) {
    TitleMenuPageWork *w = TASK_WORK(TitleMenuPageWork);
    u32 t = w->timer;
    BOOL done = TRUE, snd;
    TitleMenuItem *si;
    tm_draw_items(w, 0, w->unkc != 0 ? 3 : 2, t, 0, &done);
    w->timer++;
    if (t >= 30) {
        si = &w->m.items[w->sel];
        /* (si->unk4 & 4): TitleMenu_ShowMsgBox0, the wireless warning before the VS lobby: not on
         * the Dreamcast; the VS item goes on with the white fade of the lobby's exit */
        snd = FALSE;
        w->unkc = 0;
        if (si->unk4 & (1 | 4)) {
            TitleMenu_StartFadeToWhite(8, 0x14);
            snd = TRUE;
        }
        if (si->unk4 & 2) {
            TitleMenu_StartFadeToBlack(8, 0x14);
            snd = TRUE;
        }
        TitleMenu_Unk_ov001_022d7e60(&w->m, w->param);
        TitleMenu_CreateBgSlideTaskForward(w->param, w->m.unk17c, w->m.unk184[w->sel]);
        if (snd)
            SysSound_PlaySE(2);   /* SysSound_PlaySEDelayed(2, 0x14, ..) */
        gTaskCurrent->func = TitleMenu_State_Close;
    }
}

/* TitleMenu_State_Unk_ov001_022d8938: selection. Pad: UP/DOWN (key repeat), A
 * confirm, B back. (Touch on a button = confirm it, on the back button = back:
 * both covered by the pad.) */
static void TitleMenu_State_Select(void) {
    TitleMenuPageWork *w = TASK_WORK(TitleMenuPageWork);
    BOOL done = TRUE;
    u16 key;
    tm_draw_items(w, 0, 1, w->timer, 0, &done);
    key = gPad.rep;
    if (key & PAD_UP) {
        do {
            w->sel--;
            if (w->sel < 0)
                w->sel = 3;
        } while (w->m.items[w->sel].id == TM_ITEM_NONE);
        w->timer = 0;
        SysSound_PlaySE(4);
    } else if (key & PAD_DOWN) {
        do {
            w->sel++;
            if (w->sel >= 4)
                w->sel = 0;
        } while (w->m.items[w->sel].id == TM_ITEM_NONE);
        w->timer = 0;
        SysSound_PlaySE(4);
    }
    key = gPad.trg;
    if (key & PAD_A) {
        w->timer = 0;
        gTaskCurrent->func = TitleMenu_State_Confirm;
        SysSound_PlaySE(0);
    } else if (key & PAD_B) {
        w->sel = 4;
        w->timer = 0;
        TitleMenu_Unk_ov001_022d7dbc(&w->m, w->param);
        TitleMenu_CreateBgSlideTaskReverse(w->param, w->m.unk17c, w->m.unk180);
        SysSound_PlaySE(1);
        gTaskCurrent->func = TitleMenu_State_Close;
        if (w->m.items[4].unk4 & 1)
            TitleMenu_StartFadeToWhite(8, 0x14);
        if (w->m.items[4].unk4 & 2)
            TitleMenu_StartFadeToBlack(8, 0x14);
    }
    w->timer++;
}

static void TitleMenu_State_WaitOpenAnims(void) {
    TitleMenuPageWork *w = TASK_WORK(TitleMenuPageWork);
    BOOL done = TRUE;
    tm_draw_items(w, 1, 3, 0, 0, &done);
    if (done == TRUE && w->param->unk258 != 0) {
        w->unk0 = 7;   /* unk8 = 7 */
        gTaskCurrent->func = TitleMenu_State_Select;
    }
}

/* ------------------------------------------------------------- pages */
static void TitleMenu_Unk_ov001_022d7dbc(TitleMenuPage *p, TitleMenuWork *r) {
    s32 n = 0, i, off;
    TitleMenuItem *e;
    for (i = 0; i < 4; i++)
        if (p->items[i].id < 0x13)
            n++;
    p->unk10 = 1;
    e = &p->items[5];
    KeyframeAnim_Init(&e->anim, r->res828, 0);
    KeyframeAnim_PlayReverse(&e->anim);
    e = &p->items[4];
    e->id = 0;
    KeyframeAnim_Init(&e->anim, r->res828, 5);
    KeyframeAnim_PlayReverse(&e->anim);
    off = (4 - n) / 2;
    for (i = 0, e = p->items; i < 4; i++, e++)
        if (e->id < 0x13) {
            KeyframeAnim_Init(&e->anim, r->res828, (u32)(i + 1 + off));
            KeyframeAnim_PlayReverse(&e->anim);
        }
}

static void TitleMenu_Unk_ov001_022d7e60(TitleMenuPage *p, TitleMenuWork *r) {
    s32 n = 0, i, off;
    TitleMenuItem *e;
    for (i = 0; i < 4; i++)
        if (p->items[i].id < 0x13)
            n++;
    p->unk10 = 0;
    e = &p->items[5];
    KeyframeAnim_Init(&e->anim, r->res828, 6);
    e = &p->items[4];
    e->id = 0;
    KeyframeAnim_Init(&e->anim, r->res828, 0xb);
    off = (4 - n) / 2;
    for (i = 0, e = p->items; i < 4; i++, e++)
        if (e->id < 0x13)
            KeyframeAnim_Init(&e->anim, r->res828, (u32)(i + off + 7));
}

/* TitleMenu_Unk_ov001_022d7ee8 (forward = 1) / _022d8060 (forward = 0) */
static void TitleMenu_InitPage(TitleMenuPage *p, BOOL fwd, s32 id, s32 a, s32 b, s32 c, s32 d, TitleMenuWork *cfg) {
    s32 n, i, off;
    s16 h;
    TitleMenuItem *e;
    p->h[0] = 0x40;
    p->h[1] = 0x20;
    p->h[2] = 0;
    p->h[3] = 0x20;
    p->h[4] = 0x10;
    p->h[5] = 0x4c;
    p->h[6] = 0x40;
    p->h[7] = 0x90;
    p->unk10 = fwd ? 1 : 0;
    memset(p->items, 0, sizeof(p->items));
    i = 0;
    if (a != TM_ITEM_NONE)
        p->items[i++].id = a;
    if (b != TM_ITEM_NONE)
        p->items[i++].id = b;
    if (c != TM_ITEM_NONE)
        p->items[i++].id = c;
    if (d != TM_ITEM_NONE)
        p->items[i++].id = d;
    n = i;
    for (; i < 4; i++)
        p->items[i].id = TM_ITEM_NONE;
    switch (n) {
    case 1:
        h = p->h[3];
        p->h[1] = (s16)(p->h[1] + (s16)(h / 2) + h);
        break;
    case 2:
        p->h[1] = (s16)(p->h[1] + p->h[3]);
        break;
    case 3:
        p->h[1] = (s16)(p->h[1] + (s16)(p->h[3] / 2));
        break;
    default:
        break;
    }
    off = (4 - n) / 2;
    if (fwd) {
        e = &p->items[5];
        e->id = id;
        KeyframeAnim_Init(&e->anim, cfg->res828, 6);
        KeyframeAnim_PlayReverse(&e->anim);
        e = &p->items[4];
        e->id = 0;
        KeyframeAnim_Init(&e->anim, cfg->res828, 0xb);
        KeyframeAnim_PlayReverse(&e->anim);
        for (i = 0, e = p->items; i < 4; e++, i++)
            if (e->id < 0x13) {
                KeyframeAnim_Init(&e->anim, cfg->res828, (u32)(i + off + 7));
                KeyframeAnim_PlayReverse(&e->anim);
            }
    } else {
        e = &p->items[5];
        e->id = id;
        KeyframeAnim_Init(&e->anim, cfg->res828, 0);
        for (i = 0, e = p->items; i < 4; e++, i++)
            if (e->id < 0x13)
                KeyframeAnim_Init(&e->anim, cfg->res828, (u32)(i + off + 1));
        e = &p->items[4];
        e->id = 0;
        KeyframeAnim_Init(&e->anim, cfg->res828, 5);
    }
}

static void TitleMenu_CreatePageTask(const TitleMenuPage *src, TitleMenuWork *param, s32 n) {
    TitleMenuPageWork *w = (TitleMenuPageWork *)Task_Create(TitleMenu_State_WaitOpenAnims, NULL, 0, 0x2000, 0x1000,
                                                            sizeof(TitleMenuPageWork))->work;
    s32 i;
    w->m = *src;
    w->unk0 = param->unk4;
    w->sel = -1;
    w->unkc = 0;
    w->param = param;
    for (i = 0; i < 4; i++) {
        if (w->sel < 0 && w->m.items[i].id != TM_ITEM_NONE) {
            if (n <= 0) {
                w->sel = i;
                return;
            }
            n--;
        }
    }
}

static void TitleMenu_SelectGameStart(TitleMenuWork *p, s32 f, s32 a) {
    (void)f;
    (void)a;
    p->state = 1;
}
static void TitleMenu_SelectOptions(TitleMenuWork *p, s32 f, s32 a) {
    (void)f;
    (void)a;
    p->state = 7;
}
static void TitleMenu_SelectPlayerSelectMode3(TitleMenuWork *p, s32 f, s32 a) {
    (void)f;
    (void)a;
    p->state = 5;
}
static void TitleMenu_SelectPlayerSelectMode4(TitleMenuWork *p, s32 f, s32 a) {
    (void)f;
    (void)a;
    p->state = 6;
}
static void TitleMenu_SelectTitleScreen(TitleMenuWork *p, s32 f, s32 a) {
    (void)f;
    (void)a;
    p->state = 0;
}

static void TitleMenu_OpenRootPage(TitleMenuWork *p, s32 flag, s32 b);
static void TitleMenu_OpenVsPage(TitleMenuWork *p, s32 flag, s32 b);

static void TitleMenu_OpenPlayerSelectPage(TitleMenuWork *p, s32 flag, s32 b) {
    TitleMenuPage menu;
    memset(&menu, 0, sizeof(menu));
    TitleMenu_InitPage(&menu, flag, 4, 0xf, 0x10, 0x14, 0x14, p);
    menu.items[0].cb = TitleMenu_SelectPlayerSelectMode3;
    menu.items[1].cb = TitleMenu_SelectPlayerSelectMode4;
    menu.items[4].cb = TitleMenu_OpenRootPage;
    menu.items[0].unk4 = 1;
    menu.items[1].unk4 = 1;
    menu.items[0].unk34 = -1;
    menu.items[1].unk34 = -1;
    menu.items[2].unk34 = -1;
    menu.items[3].unk34 = -1;
    menu.items[4].unk34 = 2;
    menu.unk17c = 2;
    menu.unk180 = 0;
    menu.unk184[0] = 8;
    menu.unk184[1] = 8;
    menu.unk184[2] = 8;
    menu.unk184[3] = 8;
    TitleMenu_CreatePageTask(&menu, p, b);
}

/* TitleMenu_SelectVsMode0 / 1 / 2: VsMenu_CreateMode0 / 1 / 2 (download play, host, join) and the
 * wait task TitleMenuVsMode*Wait_Main, which sets the exit of the lobby when it closes. Battle Play
 * on one Dreamcast has no lobby: every mode takes the host's exit (2, TitleMenu_StartVsHost). The
 * item's argument (unk34) is the mode: 0 TWO PLAYERS, 1 ONE PLAYER (Vs CPU, menus_vs_set_cpu). */
static void TitleMenu_SelectVsMode(TitleMenuWork *p, s32 f, s32 a) {
    (void)f;
    menus_vs_set_cpu(a == 1);
    p->state = 2;
}

/* TitleMenu_OpenVsSubPage (CREATE ROOM / JOIN): not used, Battle Play has no rooms */

/* TitleMenu_OpenVsPage: the DS page has DS WIRELESS BATTLE (-> the host / join sub-page,
 * TitleMenu_OpenVsSubPage) and DS DOWNLOAD BATTLE (slide colours 3 / 6). Battle Play keeps the two
 * buttons relabelled TWO PLAYERS and ONE PLAYER (Vs CPU; TitleMenu_DrawVsButton); both go straight
 * on as the host item of the sub-page does (flags 4, slide colours 4 / the download item's 6), the
 * mode in their argument; B / the back button as the original. */
static void TitleMenu_OpenVsPage(TitleMenuWork *p, s32 flag, s32 b) {
    TitleMenuPage menu;
    memset(&menu, 0, sizeof(menu));
    TitleMenu_InitPage(&menu, flag, 3, 0xd, 0xe, 0x14, 0x14, p);
    menu.items[0].cb = TitleMenu_SelectVsMode;
    menu.items[1].cb = TitleMenu_SelectVsMode;
    menu.items[4].cb = TitleMenu_OpenRootPage;
    menu.items[0].unk4 = 4;
    menu.items[1].unk4 = 4;
    menu.items[0].unk34 = 0;   /* TWO PLAYERS */
    menu.items[1].unk34 = 1;   /* ONE PLAYER: Vs CPU */
    menu.items[2].unk34 = -1;
    menu.items[3].unk34 = -1;
    menu.items[4].unk34 = 1;
    menu.unk17c = 1;
    menu.unk180 = 0;
    menu.unk184[0] = 4;
    menu.unk184[1] = 6;
    menu.unk184[2] = 8;
    menu.unk184[3] = 8;
    TitleMenu_CreatePageTask(&menu, p, b);
}

static void TitleMenu_OpenRootPage(TitleMenuWork *p, s32 flag, s32 b) {
    TitleMenuPage menu;
    memset(&menu, 0, sizeof(menu));
    if (p->unk8)
        TitleMenu_InitPage(&menu, flag, 2, 9, 10, 11, 12, p);
    else
        TitleMenu_InitPage(&menu, flag, 2, 9, 10, 12, 0x14, p);
    menu.items[0].cb = TitleMenu_SelectGameStart;
    menu.items[1].cb = TitleMenu_OpenVsPage;
    if (p->unk8) {
        menu.items[2].cb = TitleMenu_OpenPlayerSelectPage;
        menu.items[3].cb = TitleMenu_SelectOptions;
    } else {
        menu.items[2].cb = TitleMenu_SelectOptions;
        menu.items[3].cb = NULL;
    }
    menu.items[4].cb = TitleMenu_SelectTitleScreen;
    menu.items[0].unk4 = 1;
    menu.items[1].unk4 = 0;
    if (p->unk8) {
        menu.items[2].unk4 = 0;
        menu.items[3].unk4 = 2;
    } else {
        menu.items[2].unk4 = 2;
        menu.items[3].unk4 = 0;
    }
    menu.items[4].unk4 = 1;
    menu.items[0].unk34 = -1;
    menu.items[1].unk34 = -1;
    menu.items[2].unk34 = -1;
    menu.items[3].unk34 = -1;
    menu.items[4].unk34 = -1;
    menu.unk17c = 0;
    menu.unk180 = 8;
    menu.unk184[0] = 8;
    menu.unk184[1] = 1;
    if (p->unk8) {
        menu.unk184[2] = 2;
        menu.unk184[3] = 8;
    } else {
        menu.unk184[2] = 8;
        menu.unk184[3] = 8;
    }
    TitleMenu_CreatePageTask(&menu, p, b);
}

/* ------------------------------------------------------------- exits */
static void TitleMenu_ExitToTitleScreen(void) { TitleScreen_Start(); }

static void TitleMenu_ExitToGameStart(void) {
    s32 t0 = gSaveData.unkC;
    gGameState.unk0 = 0;
    StoryProgress_RestoreFromSave();
    if (t0 == 0) {
        gGameState.unk0 = 0;
        Game_StartStoryStage(0, 0);
        return;
    }
    if (t0 == 1) {
        ZoneActSelectMode_Start();   /* SceneChange_ToZoneActSelectMode */
        return;
    }
    gGameState.unk10 = 0;
    gGameState.unk18 &= ~0x20u;
    gGameState.unk18 &= ~0x10u;
    if (t0 == 2) {
        gGameState.unk0 = 0;
        ZoneMap_StartDefault();   /* SceneChange_ToZoneMap */
        return;
    }
    PlayerSelectMenu_StartMode(0);   /* SceneChange_ToPlayerSelectMenuMode0 */
}

/* TitleMenu_StartVsHost: gGameState.unk10 = 1 (VS), unk18 |= 0x20, &= ~0x10, VsConnect_CreateHostSync
 * (-> SceneChange_ToPlayerSelectMenuMode1); StartVsJoin is the same for the guest, GoToVsLobby
 * (download play) sets 0x10 too: all three start the two players' select here */
static void TitleMenu_StartVsHost(void) {
    gGameState.unk10 = MENU_MODE_VS;
    gGameState.unk18 |= 0x20u;
    gGameState.unk18 &= ~0x10u;
    PlayerSelectMenu_StartVs();
}

static void TitleMenu_ExitToPlayerSelectMode3(void) {
    gGameState.unk10 = 2;
    gGameState.unk18 &= ~0x20u;
    gGameState.unk18 &= ~0x10u;
    gGameState.unk1C = 0;
    gGameState.unk1D = 0;
    PlayerSelectMenu_StartMode(3);
}

static void TitleMenu_ExitToPlayerSelectMode4(void) {
    gGameState.unk10 = 2;
    gGameState.unk18 &= ~0x20u;
    gGameState.unk18 &= ~0x10u;
    gGameState.unk1C = 0;
    gGameState.unk1D = 0;
    PlayerSelectMenu_StartMode(4);
}

static void TitleMenu_ExitToOptions(void) { Options_StartFromTitleMenu(); }

/* gTitleMenuExitTable */
static void (*const gTitleMenuExitTable[8])(void) = {
    TitleMenu_ExitToTitleScreen, TitleMenu_ExitToGameStart, TitleMenu_StartVsHost, TitleMenu_StartVsHost,
    TitleMenu_StartVsHost, TitleMenu_ExitToPlayerSelectMode3, TitleMenu_ExitToPlayerSelectMode4,
    TitleMenu_ExitToOptions,
};

static void TitleMenu_Main(void) {
    TitleMenuWork *w = TASK_WORK(TitleMenuWork);
    s32 st;
    s_tm = w;
    w->unk4++;
    st = w->state;
    if (st != 9) {
        Task_DestroyByTag(0x1000);
        if (st >= 0 && st < 8)
            gTitleMenuExitTable[st]();
    }
}

/* ----------------------------------------------------------- create */
static const char *const s_tm_lang_bac[6] = { "act/mnu_titl_jpn.bac", "act/mnu_titl_eng.bac",
                                              "act/mnu_titl_fra.bac", "act/mnu_titl_deu.bac",
                                              "act/mnu_titl_ita.bac", "act/mnu_titl_spa.bac" };
static MnBgImg *s_bg810, *s_bg81c;

static void TitleMenu_LoadAssets(TitleMenuWork *p) {
    int i;
    /* gTitleMenuFileTable[lang] */
    p->res808 = mn_res_load("/act/mnu_titl.bac");
    p->res80c = mn_res_load(s_tm_lang_bac[mn_lang()]);
    /* Battle Play's buttons are spelled in English (TitleMenu_DrawVsButton) */
    p->res_vs = mn_lang() == 1 ? p->res80c : mn_res_load(s_tm_lang_bac[1]);
    s_bg810 = mn_bg_load("/bg/mnu_titl_bg01_a.bbg");
    p->bg_sub3 = mn_bg_load("/bg/mnu_titl_bg01_b.bbg");
    p->bg_vs = mn_bg_load("/bg/mnu_titl_bg02_a.bbg");
    s_bg81c = mn_bg_load("/bg/mnu_titl_bg03_a.bbg");
    for (i = 0; i < 3; i++)
        p->wipe[i] = mn_bg_load("/bg/mnu_titl_wipe.bbg");
    p->res824 = mn_ama_load("/ama/mnu_wipe.ama");
    p->res828 = mn_ama_load("/ama/mnu_button.ama");
}

/* The art screen (main engine: picture, page title, colour wipes) is the
 * frame at 2.5x; the buttons and the back button of the touch screen are
 * laid over its right side at 1.5x, without their screen's BGs. */
static void TitleMenu_Compose(void) {
    mn_comp_reset(0);
    mn_comp_full(0);
    mn_comp_add(1, 8, 0x18, 0xc8, 0xa8, 336, 110, 1.5f, 0xf, 0);
}

static void TitleMenu_InitDisplay(void) {
    int e, l;
    mn_display_reset();
    for (e = 0; e < 2; e++) {
        for (l = 0; l < 3; l++)
            Background_SetSize(e, l, 512, 256);   /* BGxCNT 0x4084 / 0x4088: 512x256 */
        for (l = 0; l < 4; l++)
            Background_SetPriority(e, l, l);
    }
    TitleMenu_Compose();
    g_mn_scene = "TitleMenu";
}

static void TitleMenu_InitSprites(TitleMenuWork *w) {
    MnBgImg *res;
    s32 i;
    if (gSaveData.unk4 == 0) {
        res = s_bg810;
    } else {
        gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
        res = ((u16)(gRandSeed >> 16) % 2 == 0) ? s_bg810 : s_bg81c;
    }
    w->bg_main3 = res;
    if (res != s_bg810)
        mn_bg_free(s_bg810);
    if (res != s_bg81c)
        mn_bg_free(s_bg81c);
    s_bg810 = s_bg81c = NULL;
    Background_Show(0, 3, w->bg_main3);
    Background_Show(1, 3, w->bg_sub3);
    for (i = 0; i < 3; i++) {   /* BackgroundDS_Init: both engines */
        Background_Show(0, i, w->wipe[i]);
        Background_Show(1, i, w->wipe[i]);
    }
    /* sprites: TitleMenu_InitSpriteSub (engine 1) / _InitSpriteMain (engine 0) */
    AnimatorSprite_Init(&w->sprites[0], w->res808, 0, 0, 1);
    w->ids[0] = 0;
    AnimatorSprite_Init(&w->sprites[1], w->res808, 3, 0, 1);
    w->ids[1] = 3;
    for (i = 2; i <= 8; i++) {
        AnimatorSprite_Init(&w->sprites[i], w->res80c, (u16)(i - 2), 0, 0);
        w->ids[i] = i - 2;
    }
    for (i = 9; i <= 18; i++) {
        AnimatorSprite_Init(&w->sprites[i], (i == 0xd || i == 0xe) && w->res_vs ? w->res_vs : w->res80c,
                            (u16)(7 + (i - 9) * 3), 0, 1);
        w->ids[i] = 7 + (i - 9) * 3;
    }
}

static void TitleMenu_Destructor(void) {
    TitleMenuWork *p = TASK_WORK(TitleMenuWork);
    int i;
    mn_display_reset();
    mn_res_free(p->res808);
    memset(s_tm_lbl, 0, sizeof(s_tm_lbl));   /* the spelled labels of these resources */
    if (p->res_vs != p->res80c)
        mn_res_free(p->res_vs);
    mn_res_free(p->res80c);
    mn_bg_free(p->bg_main3);
    mn_bg_free(p->bg_sub3);
    mn_bg_free(p->bg_vs);
    for (i = 0; i < 3; i++)
        mn_bg_free(p->wipe[i]);
    free(p->res824);
    free(p->res828);
    memset(p, 0, sizeof(*p));
    if (s_tm == p)
        s_tm = NULL;
}

static TitleMenuWork *TitleMenu_Create(void) {
    TitleMenuWork *w = (TitleMenuWork *)Task_Create(TitleMenu_Main, TitleMenu_Destructor, 0, 0x1000, 0x1000,
                                                    sizeof(TitleMenuWork))->work;
    s_tm = w;
    w->state = 9;
    w->unk4 = 0;
    w->unk8 = (gSaveData.unk8 & 0x100) ? TRUE : FALSE;   /* TitleMenu_IsPlayerSelectPageUnlocked */
    TitleMenu_InitDisplay();
    TitleMenu_LoadAssets(w);
    TitleMenu_InitSprites(w);
    Pad_Reset(&gPad, NULL, NULL);
    SysSound_PlayBGM(0);
    return w;
}

void TitleMenu_Start(void) {
    TitleMenuWork *p = TitleMenu_Create();
    TitleMenu_OpenRootPage(p, 0, 0);
    TitleMenu_CreateFadeTask(8, 0);
    TitleMenu_CreateBgSlideTaskAtMidpoint(p, 0);
    gSaveDataBusy = 0;
}

void TitleMenu_StartAtPlayerSelectItem0(void) {
    TitleMenuWork *p = TitleMenu_Create();
    if (p->unk8)
        TitleMenu_OpenPlayerSelectPage(p, 1, 0);
    else
        TitleMenu_OpenRootPage(p, 0, 0);
    TitleMenu_CreateFadeTask(8, 0);
    TitleMenu_CreateBgSlideTaskAtMidpoint(p, 0);
}

void TitleMenu_StartAtPlayerSelectItem1(void) {
    TitleMenuWork *p = TitleMenu_Create();
    if (p->unk8)
        TitleMenu_OpenPlayerSelectPage(p, 1, 1);
    else
        TitleMenu_OpenRootPage(p, 0, 0);
    TitleMenu_CreateFadeTask(8, 0);
    TitleMenu_CreateBgSlideTaskAtMidpoint(p, 0);
}

void TitleMenu_StartFromOptions(void) {
    TitleMenuWork *p = TitleMenu_Create();
    if (p->unk8)
        TitleMenu_OpenRootPage(p, 1, 3);
    else
        TitleMenu_OpenRootPage(p, 1, 2);
    TitleMenu_CreateFadeTask(8, 0);
    TitleMenu_CreateBgSlideTaskAtMidpoint(p, 0);
}

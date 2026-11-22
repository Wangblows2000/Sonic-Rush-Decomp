/*
 * Records_* (src/game/Menus/Records/, 46 functions): Options > PLAYER DATA >
 * BATTLE PLAY RECORD. The VS totals (WIN / LOSE / DRAW, gSaveData.unk1D0) under
 * the owner's name, and the last 10 VS opponents (gSaveData.unk1D4: name and
 * counts), 4 rows visible, scrolled with UP / DOWN (or the bar on the DS).
 * The port has no wireless VS (MENUS.md 11), so a save made on the port has no
 * opponents; a DS save image keeps them.
 *
 * DS display (Records_InitDisplay / _InitBackgrounds / _InitSprites):
 *   main: BG1 mnu_reco_base02 (frame), BG2 mnu_reco_base01 (table), BG3
 *     mnu_reco_base (zigzag, scrolls), sprites: digits (mnu_reco 0..9), arrows and
 *     scroll bar (10..22), WIN / LOSE / DRAW (mnu_reco_<lang> 1..3), back button
 *     (mnu_titl 0/1); names as 8x8 OBJ tiles of font_ipl_8x8 (Records_DrawName)
 *   sub: BG2 mnu_reco_image (character art), BG3 mnu_reco_base, sprite: the
 *     "BATTLE PLAY RECORD" plate (slides in)
 * Single screen: Records_Compose (MENUS.md 3).
 *
 * DS service                          here
 * font_ipl_8x8 tiles + OAM per glyph  font8.bin (menudata.py): the 11 name strips are
 *                                     built into one indexed image (ink: palette 0xc
 *                                     colour 15 = 0x0000 header, 0xd = 0x7fff rows)
 * owner nickname (OS owner info,      menus_set_owner_name (default: empty, as an unset
 *   data_02072cbc)                    nickname)
 * gTouchState (back, arrows, thumb)   B, UP, DOWN (the original's pad path); the thumb
 *                                     drag state (Records_State_Unk_ov001_022ed548) is
 *                                     kept but only touch enters it
 */
#include <stdlib.h>
#include "mn_internal.h"

typedef struct {
    u8 a, b, c, pad;
} RecCounts;

typedef struct {
    s32 unk0;           /* timer */
    s32 unk4, unk8;     /* arrow state: 0 up pressed, 1 down pressed, 4 thumb held, 6 none */
    s32 unkC, unk10;    /* touch start */
    s32 unk14;          /* scroll at the touch start */
    s32 unk18;          /* last drag direction */
    s32 unk1C, unk20;   /* page scroll from / to */
    s32 unk24;          /* back button touched */
    s32 unk28;          /* opponents */
    s32 unk2C;          /* scroll bar shown (more than 4) */
    s32 unk30;          /* scroll (px) */
    s32 unk34;          /* max scroll */
    RecCounts tot;      /* +0x38 */
    RecCounts recs[10]; /* +0x3c */
    MnSpr spr[21];      /* +0x64 */
    s32 ids[21];
    MnRes *res[3];      /* mnu_reco, mnu_reco_<lang>, mnu_titl */
    MnBgImg *bg[4];     /* base01, base02, image, base */
    MnBgImg *bg3sub;    /* the sub engine's copy of mnu_reco_base */
    MnBgImg *names;     /* the 11 name strips (56 x 88, indexed) */
} RecordsWork;

static RecordsWork *s_rec;
static u16 s_owner[16];
static int s_owner_len;

void menus_set_owner_name(const uint16_t *utf16, int len) {
    if (len < 0)
        len = 0;
    if (len > 16)
        len = 16;
    s_owner_len = len;
    if (len)
        memcpy(s_owner, utf16, (size_t)len * 2);
}

/* ---------------------------------------------------------------- fade */
typedef struct {
    u32 t;
    s16 from, to;
    u32 dur, delay;
} RecordsFadeWork;

static void RecordsFadeTask_Main(void) {
    RecordsFadeWork *w = TASK_WORK(RecordsFadeWork);
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
static void Records_CreateFadeTask(s32 x, s32 y, s32 a, s32 b) {
    RecordsFadeWork *w;
    Task_DestroyRange(0x3000, 0x3000);
    if (x < -16)
        x = -16;
    if (x > 16)
        x = 16;
    if (y < -16)
        y = -16;
    if (y > 16)
        y = 16;
    w = (RecordsFadeWork *)Task_Create(RecordsFadeTask_Main, NULL, 0, 0x3000, 0x1000, sizeof(RecordsFadeWork))->work;
    w->t = 0;
    w->from = (s16)x;
    w->to = (s16)y;
    w->dur = (u32)a;
    w->delay = (u32)b;
}
static void Records_ScrollBg_Main(void) {
    gDisplayMain[3].x += 2;   /* gDisplayMain[6] */
    gDisplaySub[3].x += 2;
}

/* ------------------------------------------------------------- data */
static void Records_LoadFromSave(RecordsWork *p) {
    s32 i, n = 0;
    p->tot.a = gSaveData.unk1D0[0];   /* data_022c4840 + 0x10 */
    p->tot.b = gSaveData.unk1D0[1];
    p->tot.c = gSaveData.unk1D0[2];
    for (i = 0; i < 10; i++) {
        p->recs[i].a = gSaveData.unk1D4[i].cnt[0];
        p->recs[i].b = gSaveData.unk1D4[i].cnt[1];
        p->recs[i].c = gSaveData.unk1D4[i].cnt[2];
        if ((p->recs[i].a != 0 || p->recs[i].b != 0 || p->recs[i].c != 0) && i == n)
            n++;
    }
    p->unk28 = n;
}

/* font8.bin: Font_GetGlyphIndex's range tables and the tiles */
typedef struct {
    u8 *blob;
    u16 nr, nt;
    const u8 *start, *base, *len, *tiles;
} RecFont;
static u16 rd16(const u8 *p) { return (u16)(p[0] | p[1] << 8); }
static s32 rd32s(const u8 *p) { return (s32)((u32)p[0] | (u32)p[1] << 8 | (u32)p[2] << 16 | (u32)p[3] << 24); }

static s32 Font_GetGlyphIndex(const RecFont *f, u32 code) {
    s32 lo = 0, hi = 0x38, d;
    if (!f->blob)
        return 0;
    do {
        s32 s = lo + hi;
        s32 mid = (s & 1) + (s >> 1);
        if (rd16(f->start + 2 * mid) <= code)
            lo = mid;
        else
            hi = mid - 1;
    } while (lo < hi);
    d = (s32)code - rd16(f->start + 2 * lo);
    if (d >= rd32s(f->len + 4 * lo))
        return 0;
    return d + rd32s(f->base + 4 * lo);
}

/* Records_BuildNameGlyphs: 6 characters, then a space or "..." (U+2026) */
static void Records_BuildNameGlyphs(const RecFont *f, u32 *out, const u16 *ids, s32 n) {
    s32 i = 0;
    while (i < n && i < 6) {
        out[i] = (u32)Font_GetGlyphIndex(f, *ids);
        ids++;
        i++;
    }
    if ((u32)n < 7) {
        for (; i < 7; i++)
            out[i] = (u32)Font_GetGlyphIndex(f, 0x20);
    } else {
        out[6] = (u32)Font_GetGlyphIndex(f, 0x2026);
    }
}

/* Records_LoadNameGlyphs: the owner (row 0) and the opponents (rows 1..n) as 7
 * glyph tiles each; ink = value 15 (palette 0xc colour 15 = black for row 0,
 * palette 0xd colour 15 = white for the others) */
static void Records_LoadNameGlyphs(RecordsWork *p) {
    char path[160];
    RecFont f;
    u32 size, tbl[11][7];
    s32 i, j, y, x;
    MnBgImg *img;
    memset(&f, 0, sizeof(f));
    snprintf(path, sizeof(path), "%s/font8.bin", g_mn_dir);
    if (mn_load_file(path, &f.blob, &size) == 0 && size >= 12 && memcmp(f.blob, "RFNT", 4) == 0) {
        f.nr = rd16(f.blob + 6);
        f.nt = rd16(f.blob + 8);
        f.start = f.blob + 12;
        f.base = f.start + 2 * f.nr;
        f.len = f.base + 4 * f.nr;
        f.tiles = f.len + 4 * f.nr;
        if ((u32)(f.tiles - f.blob) + 64u * f.nt > size || f.nr < 0x39) {
            free(f.blob);
            f.blob = NULL;
        }
    } else {
        free(f.blob);
        f.blob = NULL;
    }
    Records_BuildNameGlyphs(&f, tbl[0], s_owner, s_owner_len);
    for (i = 1; i < 11; i++)
        if (i <= p->unk28)
            Records_BuildNameGlyphs(&f, tbl[i], gSaveData.unk1D4[i - 1].name, gSaveData.unk1D4[i - 1].len);
    img = (MnBgImg *)calloc(1, sizeof(*img));
    img->w = 56;
    img->h = 88;
    img->tw = (u16)mn_tex_pow2(img->w);
    img->th = (u16)mn_tex_pow2(img->h);
    img->idx = (u8 *)calloc((size_t)img->w * img->h, 1);
    img->pal[1] = 0x0000;   /* PaletteTransfer 0x500039e: OBJ palette 0xc colour 15 */
    img->pal[2] = 0x7fff;   /* 0x50003be: palette 0xd colour 15 */
    img->dirty = 1;
    snprintf(img->name, sizeof(img->name), "record_names");
    if (f.blob) {
        for (i = 0; i < p->unk28 + 1 && i < 11; i++)
            for (j = 0; j < 7; j++) {
                const u8 *t = f.tiles + 64u * (tbl[i][j] < f.nt ? tbl[i][j] : 0);
                for (y = 0; y < 8; y++)
                    for (x = 0; x < 8; x++)
                        if (t[y * 8 + x] == 15)
                            img->idx[(i * 8 + y) * img->w + j * 8 + x] = (u8)(i == 0 ? 1 : 2);
            }
        free(f.blob);
    }
    p->names = img;
}

/* Records_DrawName(p, x, y, which): 7 tiles of row `which`, OAM priority 1 (header)
 * or 2 (rows) */
static void Records_DrawName(RecordsWork *p, s32 x, s32 y, s32 which) {
    s32 prio = which == 0 ? 1 : 2, i;
    for (i = 0; i < 7; i++, x += 8)
        if (x >= -8 && x < 0x100 && y >= -8 && y < 0xc0)
            mn_emit_bg_part(0, prio, p->names, (float)x, (float)y, (float)(i * 8), (float)(which * 8), 8, 8);
}
static void Records_DrawHeaderName(RecordsWork *p) { Records_DrawName(p, 0x18, 0x2b, 0); }
static void Records_DrawRecordNames(RecordsWork *p) {
    s32 i = 0, y = 0x3d, v;
    for (; i < p->unk28; i++, y += 0x10) {
        v = y - p->unk30;
        if (0x2d < v && v < 0x7d)
            Records_DrawName(p, 0x18, v, i + 1);
    }
}

static void spr_set(RecordsWork *p, s32 i, s32 id) {
    if (p->ids[i] != id) {
        p->ids[i] = id;
        AnimatorSprite_SetAnimation(&p->spr[i], (u16)id);
    }
}
static void Records_DrawTwoDigits(RecordsWork *p, s16 x, s16 y, s32 n, s32 prio) {
    u32 d[2];
    int k;
    if (n > 99)
        n = 99;
    d[0] = (u32)n / 10;
    d[1] = (u32)n % 10;
    for (k = 0; k < 2; k++, x += 9) {
        MnSpr *s = &p->spr[d[k]];
        spr_set(p, (s32)d[k], (s32)d[k]);
        s->x = x;
        s->y = y;
        s->flags &= ~0x60000u;
        s->flags |= (u32)prio << 17;
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_Draw(s);
    }
}
static void Records_DrawHeaderTime(RecordsWork *p) {
    Records_DrawTwoDigits(p, 0x68, 0x26, p->tot.a, 1);
    Records_DrawTwoDigits(p, 0x90, 0x26, p->tot.b, 1);
    Records_DrawTwoDigits(p, 0xb8, 0x26, p->tot.c, 1);
}
static void Records_DrawRecordTimes(RecordsWork *p) {
    s32 i, y = 0x38, v;
    for (i = 0; i < p->unk28; i++, y += 0x10) {
        v = y - p->unk30;
        if (0x28 < v && v < 0x78) {
            Records_DrawTwoDigits(p, 0x68, (s16)v, p->recs[i].a, 2);
            Records_DrawTwoDigits(p, 0x90, (s16)v, p->recs[i].b, 2);
            Records_DrawTwoDigits(p, 0xb8, (s16)v, p->recs[i].c, 2);
        }
    }
}
static void Records_DrawBackButton(RecordsWork *p, s32 flag) {
    MnSpr *s = &p->spr[20];
    spr_set(p, 20, flag == 0 ? 1 : 0);
    s->x = 0x10;
    s->y = 0x98;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}
/* the "BATTLE PLAY RECORD" plate (sub screen) */
static void Records_Unk_ov001_022ed490(RecordsWork *p, s16 x, s16 y) {
    MnSpr *s = &p->spr[19];
    spr_set(p, 19, 0);
    s->x = x;
    s->y = y;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}
/* WIN / LOSE / DRAW */
static void Records_Unk_ov001_022ed3e0(RecordsWork *p) {
    s32 i;
    for (i = 0; i < 3; i++) {
        spr_set(p, 16 + i, i + 1);
        AnimatorSprite_ProcessAnimation(&p->spr[16 + i], 0, 0);
        AnimatorSprite_Draw(&p->spr[16 + i]);
    }
}
/* the scroll bar, its arrows and thumb */
static void Records_Unk_ov001_022ed19c(RecordsWork *p) {
    MnSpr *s;
    s32 anim, a, e, f;
    if (p->unk2C == 0)
        return;
    s = &p->spr[12];
    spr_set(p, 12, 0x10);
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
    s = &p->spr[10];
    if (p->unk8 == 0)
        anim = 0xc;
    else if (p->unk30 > 0)
        anim = 0xb;
    else
        anim = 0xa;
    spr_set(p, 10, anim);
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
    s = &p->spr[11];
    if (p->unk8 == 1)
        anim = 0xf;
    else if (p->unk30 < p->unk34)
        anim = 0xe;
    else
        anim = 0xd;
    spr_set(p, 11, anim);
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
    a = p->unk30 / 2;
    e = a + (0x57 - p->unk34 / 2);
    f = e - 8;
    s = &p->spr[13];
    spr_set(p, 13, p->unk4 == 4 ? 0x12 : 0x11);
    s->x = 0xd8;
    s->y = (s16)(a + 0x25);
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
    s = &p->spr[14];
    spr_set(p, 14, p->unk4 == 4 ? 0x16 : 0x15);
    s->x = 0xd8;
    s->y = (s16)(f + 0x25);
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
    s = &p->spr[15];
    spr_set(p, 15, p->unk4 == 4 ? 0x14 : 0x13);
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    for (; a < f; a += 8) {
        s->x = 0xd8;
        s->y = (s16)(a + 0x25);
        AnimatorSprite_Draw(s);
    }
}

static s32 Records_EaseInterp(s32 from, s32 to, s32 n, s32 i, fx32 ease) {
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
static s32 Records_NextPageStart(RecordsWork *p, s32 n) {
    s32 r = n % 16;
    n += r == 0 ? 16 : 16 - r;
    if (n > p->unk34)
        n = p->unk34;
    return n;
}
static s32 Records_PrevPageStart(s32 v) {
    s32 m = v % 16;
    v -= m == 0 ? 16 : m;
    if (v < 0)
        v = 0;
    return v;
}
static BOOL Records_IsBPressed(void) { return (gPad.trg & PAD_B) != 0; }
static BOOL Records_IsUpArrowTouched(RecordsWork *p) {
    if (p->unk2C == 0 || p->unk30 == 0)
        return FALSE;
    return (gPad.trg & PAD_UP) != 0;
}
static BOOL Records_IsDownArrowTouched(RecordsWork *p) {
    if (p->unk2C == 0 || p->unk30 == p->unk34)
        return FALSE;
    return (gPad.trg & PAD_DOWN) != 0;
}

/* everything every state draws */
static void Records_DrawAll(RecordsWork *w, s16 px, s32 back) {
    Records_Unk_ov001_022ed490(w, px, 0x90);
    Records_Unk_ov001_022ed3e0(w);
    Records_DrawBackButton(w, back);
    Records_Unk_ov001_022ed19c(w);
    Records_DrawHeaderName(w);
    Records_DrawRecordNames(w);
    Records_DrawHeaderTime(w);
    Records_DrawRecordTimes(w);
}

/* ------------------------------------------------------------- states */
static void Records_State_Unk_ov001_022ed6c4(void);

static void Records_State_Enter(void) {
    RecordsWork *w = TASK_WORK(RecordsWork);
    s16 px = 0;
    if (w->unk0 >= 0)
        px = (s16)Records_EaseInterp(-160, 0, 0x10, w->unk0, 0x2000);
    Records_DrawAll(w, px, 0);
    w->unk0++;
    if ((u32)w->unk0 >= 0x10) {
        w->unk0 = 0;
        gTaskCurrent->func = Records_State_Unk_ov001_022ed6c4;
    }
}

static void Records_State_ExitToOptions(void) {
    RecordsWork *w = TASK_WORK(RecordsWork);
    Records_DrawAll(w, 0, w->unk24);
    w->unk0++;
    if (w->unk0 >= 0x10) {
        BRIGHT_MAIN = -16;
        BRIGHT_SUB = -16;
        Task_DestroyAll();
        Options_StartFromRecords();
    }
}

/* the thumb drag (touch only) */
static void Records_State_Unk_ov001_022ed548(void) {
    RecordsWork *w = TASK_WORK(RecordsWork);
    Records_DrawAll(w, 0, 0);
    w->unk0++;
    w->unk0 = 0;
    gTaskCurrent->func = Records_State_Unk_ov001_022ed6c4;   /* the touch was released */
}

/* page scroll (8 frames) */
static void Records_State_Unk_ov001_022ed614(void) {
    RecordsWork *w = TASK_WORK(RecordsWork);
    Records_DrawAll(w, 0, 0);
    if (w->unk0 >= 0)
        w->unk30 = Records_EaseInterp(w->unk1C, w->unk20, 8, w->unk0, 0x2000);
    if (Records_IsUpArrowTouched(w)) {
        w->unk4 = 0;
        w->unk8 = 0;
    } else if (Records_IsDownArrowTouched(w)) {
        w->unk4 = 1;
        w->unk8 = 1;
    } else {
        w->unk4 = 6;
        w->unk8 = 6;
    }
    w->unk0++;
    if ((u32)w->unk0 >= 8) {
        w->unk0 = 0;
        w->unk30 = w->unk20;
        gTaskCurrent->func = Records_State_Unk_ov001_022ed6c4;
    }
}

static void Records_State_Unk_ov001_022ed6c4(void) {
    RecordsWork *w = TASK_WORK(RecordsWork);
    BOOL f1 = FALSE, f2 = FALSE, f3 = FALSE;
    Records_DrawAll(w, 0, 0);
    if (Records_IsBPressed()) {
        w->unk24 = 0;
        f1 = TRUE;
    } else if (Records_IsUpArrowTouched(w)) {
        f3 = TRUE;
        w->unk1C = w->unk30;
        w->unk20 = Records_PrevPageStart(w->unk30);
        w->unk4 = 0;
        w->unk8 = 0;
    } else if (Records_IsDownArrowTouched(w)) {
        f3 = TRUE;
        w->unk1C = w->unk30;
        w->unk20 = Records_NextPageStart(w, w->unk30);
        w->unk4 = 1;
        w->unk8 = 1;
    } else {
        w->unk4 = 6;
        w->unk8 = 6;
    }
    w->unk0++;
    if (f1) {
        SysSound_PlaySE(1);
        w->unk0 = 0;
        w->unk4 = 6;
        w->unk8 = 6;
        Records_CreateFadeTask(0, -16, 16, 0);
        gTaskCurrent->func = Records_State_ExitToOptions;
    } else if (f2) {
        w->unk0 = 0;
        gTaskCurrent->func = Records_State_Unk_ov001_022ed548;
    } else if (f3) {
        w->unk0 = 0;
        gTaskCurrent->func = Records_State_Unk_ov001_022ed614;
    }
}

/* -------------------------------------------------------------- setup */
static void Records_Destructor(void) {
    RecordsWork *w = TASK_WORK(RecordsWork);
    s32 i;
    mn_display_reset();
    for (i = 0; i < 3; i++)
        mn_res_free(w->res[i]);
    for (i = 0; i < 4; i++)
        mn_bg_free(w->bg[i]);
    mn_bg_free(w->bg3sub);
    mn_bg_free(w->names);
    if (s_rec == w)
        s_rec = NULL;
}

static RecordsWork *Records_Create(void) {
    RecordsWork *w = (RecordsWork *)Task_Create(Records_State_Enter, Records_Destructor, 0, 0x1000, 0x1000,
                                                sizeof(RecordsWork))
                         ->work;
    w->unk0 = 0;
    w->unk2C = 0;
    w->unk28 = 0;
    w->unk30 = 0;
    w->unk34 = 0;
    w->unk4 = 6;
    w->unk8 = 6;
    w->unkC = 0;
    w->unk10 = 0;
    w->unk24 = 0;
    Records_LoadFromSave(w);
    if (w->unk28 > 4) {
        w->unk2C = 1;
        w->unk34 = (w->unk28 - 4) * 16;
    }
    return w;
}

/* gRecordsFileList / gRecordsLangFileList */
static const char *const k_reco_lang[6] = { "act/mnu_reco_jpn.bac", "act/mnu_reco_eng.bac", "act/mnu_reco_fra.bac",
                                            "act/mnu_reco_deu.bac", "act/mnu_reco_ita.bac", "act/mnu_reco_spa.bac" };
static void Records_LoadAssets(RecordsWork *w) {
    w->res[0] = mn_res_load("act/mnu_reco.bac");
    w->res[1] = mn_res_load(k_reco_lang[mn_lang()]);
    w->res[2] = mn_res_load("act/mnu_titl.bac");
    w->bg[0] = mn_bg_load("bg/mnu_reco_base01.bbg");
    w->bg[1] = mn_bg_load("bg/mnu_reco_base02.bbg");
    w->bg[2] = mn_bg_load("bg/mnu_reco_image.bbg");
    w->bg[3] = mn_bg_load("bg/mnu_reco_base.bbg");
    w->bg3sub = mn_bg_load("bg/mnu_reco_base.bbg");
}
static void Records_InitSprites(RecordsWork *p) {
    s32 i;
    /* Records_InitSpriteMain(spr, res, anim, prio, ..) / _InitSpriteSub */
    for (i = 0; i < 10; i++)
        AnimatorSprite_Init(&p->spr[i], p->res[0], (u16)i, 2u << 17, 0);
    AnimatorSprite_Init(&p->spr[10], p->res[0], 10, 1u << 17, 0);
    AnimatorSprite_Init(&p->spr[11], p->res[0], 13, 1u << 17, 0);
    AnimatorSprite_Init(&p->spr[12], p->res[0], 16, 1u << 17, 0);
    AnimatorSprite_Init(&p->spr[13], p->res[0], 17, 1u << 17, 0);
    AnimatorSprite_Init(&p->spr[15], p->res[0], 19, 1u << 17, 0);
    AnimatorSprite_Init(&p->spr[14], p->res[0], 21, 1u << 17, 0);
    AnimatorSprite_Init(&p->spr[16], p->res[1], 1, 1u << 17, 0);
    AnimatorSprite_Init(&p->spr[17], p->res[1], 2, 1u << 17, 0);
    AnimatorSprite_Init(&p->spr[18], p->res[1], 3, 1u << 17, 0);
    AnimatorSprite_Init(&p->spr[19], p->res[1], 0, 2u << 17, 1);
    AnimatorSprite_Init(&p->spr[20], p->res[2], 0, 3u << 17, 0);
    for (i = 0; i < 21; i++)
        p->ids[i] = -1;
}
static void Records_InitBackgrounds(RecordsWork *p) {
    Background_Show(1, 3, p->bg3sub);
    Background_Show(0, 3, p->bg[3]);
    Background_Show(1, 2, p->bg[2]);
    Background_Show(0, 2, p->bg[0]);
    Background_Show(0, 1, p->bg[1]);
}

/* One frame: the sub screen (zigzag, the Chao art on the right, the "BATTLE PLAY
 * RECORD" plate at the bottom left) is the backdrop at 2.5x; the main screen's
 * table, scroll bar and back button (its zigzag left out) are a panel at 1.5x in
 * the top left (MENUS.md 3). */
static void Records_Compose(void) {
    mn_comp_reset(0);
    mn_comp_full(1);
    mn_comp_add(0, 0, 0, 256, 192, 8, 8, 1.5f, 1 << 3, 0);
}

void Records_Start(void) {
    RecordsWork *w = Records_Create();
    s_rec = w;
    mn_display_reset();   /* Records_InitDisplay */
    Records_Compose();
    g_mn_scene = "Records";
    Records_LoadAssets(w);
    Records_InitSprites(w);
    Records_InitBackgrounds(w);
    Records_LoadNameGlyphs(w);
    Records_CreateFadeTask(BRIGHT_MAIN, 0, 0x10, 0);
    Task_Create(Records_ScrollBg_Main, NULL, 0, 0x2000, 0x1000, 0);
    SysSound_PlayBGM(0);
}

int Records_Scroll(void) { return s_rec ? s_rec->unk30 : -1; }

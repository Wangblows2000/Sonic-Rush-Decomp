/*
 * Ending, ExtraEnding, StaffRoll, PostStaffRoll (src/game/Cutscenes/Ending/).
 *
 * Ending: five illustrations per character on main BG1 with the caption
 * strip (BG0, y 144) changing at the timed frames of data_ov001_02315f44, the
 * extra layers of the third / fourth picture (BG2 slide, BG3 affine zoom-shake
 * or rotation-zoom), white fades, SEQ_FINAL_SONIC. Bottom screen: the logo
 * panel and the PRESS START TO SKIP button (after the game was cleared once).
 * ExtraEnding: PlayerDemo 0x2a with the extra-end SDAT sequence, then the
 * extra staff roll. StaffRoll: the credits column (three 256x9600 BG maps
 * scrolled one pixel a frame, rows streamed into the map as on the DS), the
 * logo sprites (DS sprites on both screens), the running characters, the
 * closing banner and the SEGA / licence card; the staff-roll STRM.
 * PostStaffRoll: TalkDemo 0x37 (Sonic, not all Chaos Emeralds), then the
 * unlock notices of the menus.
 *
 * Single screen: the main (top) screen is the frame (x2.5): the pictures with
 * their captions, the credits column (it enters from the bottom edge, where
 * the DS sub screen was). The bottom screen's information is composed into it:
 * the skip button in the top-right corner, and for the closing card the SEGA
 * logo and licence block (sub (0,16)-(256,160) at x1.5) between the final logo
 * and the banner line (CUTSCENES.md section 3).
 */
#include <stdio.h>
#include <stdlib.h>
#include "cs_internal.h"

/* ---------------------------------------------------------------- BG layers */
typedef struct {
    CsPic pic;
    CsTex tex;
    u16 pal[256];      /* faded palette (StaffRoll_FadePalette) */
    float fade;        /* 1 = original colours */
    int ok, dirty;
    const s32 *rowidx; /* staff maps (CSR1, in pic.blob): per row, offset of its pixels in rows[] (-1 = empty) */
    const u8 *rows;
    u32 rows_size;
} EdLayer;

static int layer_load(EdLayer *l, const char *name) {
    memset(l, 0, sizeof(*l));
    if (cs_pic_load(&l->pic, name) != 0)
        return -1;
    l->ok = 1;
    l->fade = 1.0f;
    l->dirty = 1;
    return 0;
}
static int layer_copy(EdLayer *l, const EdLayer *src) {
    u32 size = src->pic.blob ? 16u + 2u * src->pic.img.npal + (u32)src->pic.img.w * src->pic.img.h : 0;
    memset(l, 0, sizeof(*l));
    if (!size || !(l->pic.blob = (u8 *)malloc(size)))
        return -1;
    memcpy(l->pic.blob, src->pic.blob, size);
    if (cs_img_parse(&l->pic.img, l->pic.blob, size) != 0) {
        free(l->pic.blob);
        l->pic.blob = NULL;
        return -1;
    }
    l->ok = 1;
    l->fade = 1.0f;
    l->dirty = 1;
    return 0;
}
static void layer_free(EdLayer *l) {
    cs_tex_free(&l->tex);
    cs_pic_free(&l->pic);
    l->rowidx = NULL;
    l->rows = NULL;
    l->ok = 0;
}
/* the 256x9600 credit maps, row-compacted by csdata.py (bg/<name>.csr, CSR1:
 * the CSI1 header + palette, s32 rowoff[h], the non-empty rows; ~0.87 MB for
 * the three instead of 7.4 MB as index buffers) */
static int staff_load(EdLayer *l, const char *name) {
    char path[200];
    u32 size, npal, w, h, hdr;
    const u8 *b;
    memset(l, 0, sizeof(*l));
    snprintf(path, sizeof(path), "%s/bg/%s.csr", g_cs_dir, name);
    l->pic.blob = (u8 *)cs_load_file(path, &size);
    b = l->pic.blob;
    if (!b || size < 16 || memcmp(b, "CSR1", 4) != 0)
        goto fail;
    w = (u32)(b[4] | b[5] << 8);
    h = (u32)(b[6] | b[7] << 8);
    npal = (u32)(b[12] | b[13] << 8);
    hdr = (16 + 2 * npal + 3) & ~3u;
    if (hdr + 4 * h > size)
        goto fail;
    l->pic.img.w = (u16)w;
    l->pic.img.h = (u16)h;
    l->pic.img.type = (u16)(b[8] | b[9] << 8);
    l->pic.img.flags = (u16)(b[10] | b[11] << 8);
    l->pic.img.npal = (u16)npal;
    l->pic.img.pofs = (u16)(b[14] | b[15] << 8);
    l->pic.img.pal = (const u16 *)(b + 16);
    l->pic.img.px = NULL;
    l->rowidx = (const s32 *)(b + hdr);
    l->rows = b + hdr + 4 * h;
    l->rows_size = size - hdr - 4 * h;
    l->ok = 1;
    l->fade = 1.0f;
    l->dirty = 1;
    return 0;
fail:
    printf("cutscene: missing or bad %s\n", path);
    cs_log("staffroll map %s missing", name);
    layer_free(l);
    return -1;
}
static void layer_set_fade(EdLayer *l, float f) {
    if (f < 0)
        f = 0;
    if (f > 1)
        f = 1;
    if (f != l->fade) {
        l->fade = f;
        l->dirty = 1;
    }
}
/* palette as the DS slot holds it, scaled like StaffRoll_FadePalette */
static void layer_pal(EdLayer *l) {
    int i;
    for (i = 0; i < 256; i++) {
        int k = i - l->pic.img.pofs;
        u16 c = (k >= 0 && k < l->pic.img.npal) ? l->pic.img.pal[k] : 0;
        if (l->fade < 1.0f) {
            s32 r = (s32)((c & 31) * l->fade), g = (s32)((c >> 5 & 31) * l->fade), b = (s32)((c >> 10 & 31) * l->fade);
            c = (u16)((r & 31) | (g & 31) << 5 | (b & 31) << 10);
        }
        l->pal[i] = c;
    }
}
static CsTex *layer_tex(EdLayer *l) {
    if (!l->ok)
        return NULL;
    if (!l->tex.argb && cs_tex_init(&l->tex, l->pic.img.w, l->pic.img.h > 1024 ? 1024 : l->pic.img.h) != 0)
        return NULL;
    if (l->dirty) {
        layer_pal(l);
        cs_tex_put(&l->tex, 0, 0, l->pic.img.w, l->tex.h, l->pic.img.px, l->pic.img.w, l->pal);
        l->dirty = 0;
    }
    return &l->tex;
}
static void layer_draw(EdLayer *l, float x, float y) {
    CsTex *t = layer_tex(l);
    if (t)
        cs_quad_tex(t, x, y, t->w, t->h, 0, 0, t->w, t->h, 0xffffffff);
}

/* ================================================================ Ending */
typedef struct Ending Ending;
struct Ending {
    s32 chara;        /* unk0 */
    s32 frame;        /* unk4 */
    void (*sub)(Ending *); /* unk8 */
    s32 t;            /* unkC */
    EdLayer bg[19];
    CsRes *hdr[2];
    CsSpr spr[5];
    s32 skippable;    /* unk68C */
    s32 skip;         /* unk690 */
    s32 skip_t;       /* unk694 */
    s32 cap, cap_prev;/* unk698 / unk69C */
    s32 pic;          /* unk6A0 */
    float shy, shx, shd; /* unk6A4 / 6A8 / 6AC */
    u32 layers;       /* DISPCNT BG bits */
    s16 bg2_hofs;
    s32 m3[4], c3x, c3y, o3x, o3y;   /* BG3 affine */
    int bg3_pic;      /* bgs index on BG3 (17, then 16) */
    int bg0_hide_task, bg_show_task;
    int done_staffroll;
};
static Ending s_ed;
static int s_ed_on;

/* StaffRoll */
typedef struct StaffRoll StaffRoll;
typedef struct {
    int on;
    s32 idx, x, y;
} SrCredit;
struct StaffRoll {
    s32 mode;
    s32 frame;        /* unk4 */
    void (*fn)(StaffRoll *); /* func10 */
    s32 t;            /* unk14 */
    EdLayer bg[15];
    CsRes *res[3];    /* ac_sf, ac_sf_<lang>, ac_sf_logo */
    CsSpr banner[18];
    CsSpr skipbtn, skiptxt;   /* spr978 (ac_sf 0x24) / spr930 (ac_sf_<lang> 0) */
    CsSpr credit[4];
    u8 credit_fade[4];
    SrCredit cred[4];
    s32 skippable, skip, skip_t;   /* unkbb0 / bb4 / bb8 */
    s32 scroll, end;  /* unkbbc / unkbc4 */
    int scroll_on;    /* task8 */
    void (*pic_fn)(StaffRoll *);   /* unkbd8 */
    s32 pic_t;        /* unkbdc */
    s32 pic_end;      /* unkbe0 */
    CsSpr ps[3];      /* unkbe4 */
    s32 px[3], py[3], pvx[3], pvy[3], pax[3], pay[3];
    s32 chase;        /* unkd04 */
    s32 extra_n;      /* unkd08 */
    u32 main_layers, sub_layers;
    int main_bg1;     /* bgs index shown on main BG1 in extra mode */
    float sprite_fade;
    int sub_card;     /* sub BG1 logo card shown */
    float alpha_bg1;
    CsTex staff_tex[3];
    s32 staff_row[3];
};
static StaffRoll s_sr;
static int s_sr_on;

/* PostStaffRoll */
static int s_psr_on, s_psr_state, s_psr_t;

static int s_xe_on;
BOOL Ending_IsActive(void) { return s_ed_on || s_sr_on || s_psr_on || s_xe_on; }

static const u8 *edtab(void) { return (const u8 *)cs_sec("ed_tab", NULL); }

static BOOL Ending_FadeIn(Ending *w, s32 num, s32 den, BOOL main, BOOL sub) {
    float t = (float)num / (float)den, r;
    if (w->skip != 0)
        return FALSE;
    if (t > 1.0f)
        t = 1.0f;
    r = 1.0f - t * t;
    if (main) {
        g_cs_bright = (s16)(16.0f * r);
        if (g_cs_bright < 0)
            g_cs_bright = 0;
    }
    if (sub) {
        g_cs_bright_sub = (s16)(16.0f * r);
        if (g_cs_bright_sub < 0)
            g_cs_bright_sub = 0;
    }
    return 0.0f == r;
}
static BOOL Ending_FadeOut(Ending *w, s32 num, s32 den, BOOL main, BOOL sub) {
    float t = (float)num / (float)den, r;
    if (w->skip != 0)
        return FALSE;
    if (t > 1.0f)
        t = 1.0f;
    r = 1.0f - t;
    r = 1.0f - r * r;
    if (main) {
        g_cs_bright = (s16)(16.0f * r);
        if (g_cs_bright > 16)
            g_cs_bright = 16;
    }
    if (sub) {
        g_cs_bright_sub = (s16)(16.0f * r);
        if (g_cs_bright_sub > 16)
            g_cs_bright_sub = 16;
    }
    return 1.0f == r;
}
static void Ending_SetSubState(Ending *p, void (*v)(Ending *)) {
    p->sub = v;
    p->t = 0;
}
/* Ending_CreateBg0PictureTask + EndingBg0Picture_Main: the next caption on BG0 */
static void Ending_CreateBg0PictureTask(Ending *p) {
    p->cap_prev = p->cap;
    p->cap++;
    p->layers |= 1;
    cs_log("ending caption %d", p->cap);
}
static void Ending_DrawNextPicture(Ending *p) {
    p->pic++;
    cs_log("ending picture %d", p->pic);
}
static void Ending_CheckTimedBg0Picture(Ending *p) {
    const s32 *t = (const s32 *)cs_sec("ed_timed", NULL);
    int i;
    if (!t || p->pic < 0 || p->pic > 4)
        return;
    for (i = 0; i < 3; i++) {
        s32 v = t[(p->chara * 5 + p->pic) * 3 + i];
        if (v == 0)
            return;
        if (p->t == v) {
            Ending_CreateBg0PictureTask(p);
            return;
        }
    }
}
static void ed_set_affine_scale(Ending *p, s32 sx, s32 sy) {
    p->m3[0] = sx;
    p->m3[1] = 0;
    p->m3[2] = 0;
    p->m3[3] = sy;
}
static void Ending_State_GoToStaffRoll(Ending *p) {
    (void)p;
    s_ed_on = 0;
    cs_bgm_stop(0);
    cs_scene_change(CS_NEXT_STAFFROLL);
}
static void Ending_UpdatePictureRotZoom(Ending *w) {
    float t = (float)w->t / 335.0f;
    s32 scale = FX_Div(0x64000, 0x64000 - (s32)(405504.0f * (1.0f - t * t)));
    u16 angle = (u16)(0x7555 + (w->t << 9));
    s32 s = cs_sin(angle), c = cs_cos(angle);
    w->m3[0] = FX_Mul(c, scale);
    w->m3[1] = FX_Mul(-s, scale);
    w->m3[2] = FX_Mul(s, scale);
    w->m3[3] = FX_Mul(c, scale);
    w->c3x = w->bg[16].pic.img.w / 2;
    w->c3y = w->bg[16].pic.img.h / 2;
    w->o3x = w->o3y = 0;
}
static void Ending_UpdatePictureZoomShake(Ending *w) {
    s32 v = 0x2000 - w->t * 10;
    float f = v ? (float)FX_Div(0x1000, v) / 4096.0f : 1.0f, a, b;
    if (v <= 0x1000) {
        f = 1.0f;
        v = 0x1000;
    }
    a = w->shx * f;
    b = w->shy * f;
    ed_set_affine_scale(w, v, v);
    w->c3x = w->bg[16].pic.img.w / 2;
    w->c3y = w->bg[16].pic.img.h;
    w->o3x = (s32)b;
    w->o3y = (s32)a;
    if (w->t & 0xc) {
        w->shx += 3.0f;
        if (w->shx > 10.0f)
            w->shx = 10.0f;
    } else {
        w->shx -= 3.0f;
        if (w->shx < -10.0f)
            w->shx = -10.0f;
    }
    w->shy += w->shd;
    if ((w->t & 0xf) == 8)
        w->shd *= -1.0f;
}

/* ---- Sonic */
static void Ending_SubState_SonicShowPicture0(Ending *p);
static void Ending_SubState_SonicShowPicture1(Ending *p);
static void Ending_SubState_SonicShowPicture2(Ending *p);
static void Ending_SubState_SonicShowPicture3(Ending *p);
static void Ending_SubState_SonicShowPicture4(Ending *p);
static void Ending_SubState_SonicFinish(Ending *p) {
    g_cs_bright = 16;
    g_cs_bright_sub = 16;
    Ending_SetSubState(p, NULL);
    Ending_State_GoToStaffRoll(p);
}
static void Ending_SubState_SonicNextPicture1(Ending *p) {
    g_cs_bright = 16;
    Ending_CreateBg0PictureTask(p);
    Ending_DrawNextPicture(p);
    Ending_SetSubState(p, Ending_SubState_SonicShowPicture1);
}
static void Ending_SubState_SonicNextPicture2(Ending *p) {
    g_cs_bright = 16;
    Ending_CreateBg0PictureTask(p);
    Ending_DrawNextPicture(p);
    Ending_SetSubState(p, Ending_SubState_SonicShowPicture2);
    p->layers = 0xf;
    ed_set_affine_scale(p, 0x1000, 0x1000);
    p->c3x = p->c3y = 0;
    p->o3x = -0xb0;
    p->o3y = -0x20;
}
static void Ending_SubState_SonicNextPicture3(Ending *p) {
    g_cs_bright = 16;
    Ending_CreateBg0PictureTask(p);
    Ending_DrawNextPicture(p);
    Ending_SetSubState(p, Ending_SubState_SonicShowPicture3);
    p->bg3_pic = 16;
    p->layers = 0xb;
    ed_set_affine_scale(p, 0, 0);
    p->shd = 0.5f;
}
static void Ending_SubState_SonicNextPicture4(Ending *p) {
    g_cs_bright = 16;
    Ending_CreateBg0PictureTask(p);
    Ending_DrawNextPicture(p);
    Ending_SetSubState(p, Ending_SubState_SonicShowPicture4);
    p->layers = 3;
}
static void Ending_SubState_SonicShowPicture0(Ending *p) {
    if (p->t >= 5)
        Ending_FadeIn(p, p->t - 5, 0x3c, 1, 1);
    if (p->t < 0x16d)
        return;
    if (!Ending_FadeOut(p, p->t - 0x16d, 0x3c, 1, 0))
        return;
    Ending_SetSubState(p, Ending_SubState_SonicNextPicture1);
}
static void Ending_SubState_SonicShowPicture1(Ending *p) {
    if (p->t >= 5)
        Ending_FadeIn(p, p->t - 5, 0x3c, 1, 0);
    if (p->t < 0x1a9)
        return;
    if (!Ending_FadeOut(p, p->t - 0x1a9, 0x3c, 1, 0))
        return;
    Ending_SetSubState(p, Ending_SubState_SonicNextPicture2);
}
static void Ending_SubState_SonicShowPicture2(Ending *p) {
    if (p->t >= 5)
        Ending_FadeIn(p, p->t - 5, 0x3c, 1, 0);
    if (p->t >= 0x299 && Ending_FadeOut(p, p->t - 0x299, 0x1e, 1, 0)) {
        Ending_SetSubState(p, Ending_SubState_SonicNextPicture3);
        return;
    }
    if (p->t == 0x1a4)
        p->bg0_hide_task = 1;   /* EndingBg0Hide_Main (0xf200) */
    if (p->t < 0x1e0)
        return;
    {
        float t = (float)(p->t - 0x1e0) / 60.0f, r;
        if (t > 1.0f)
            t = 1.0f;
        r = 1.0f - t;
        p->bg2_hofs = (s16)(-70.0f + 70.0f * (1.0f - r * r * r));
    }
}
static void Ending_SubState_SonicShowPicture3(Ending *p) {
    if (p->t >= 5)
        Ending_FadeIn(p, p->t - 5, 0x1e, 1, 0);
    if (p->t >= 0x14f && Ending_FadeOut(p, p->t - 0x14f, 0x3c, 1, 0)) {
        Ending_SetSubState(p, Ending_SubState_SonicNextPicture4);
        return;
    }
    Ending_UpdatePictureZoomShake(p);
}
static void Ending_SubState_SonicShowPicture4(Ending *p) {
    if (p->t >= 5)
        Ending_FadeIn(p, p->t - 5, 0x3c, 1, 0);
    if (p->t >= 0x3c5)
        Ending_FadeOut(p, p->t - 0x3c5, 0xb4, 1, 1);
    if (p->t == 0x3c5)
        cs_bgm_stop(0xb4);
    if (p->t < 0x483)
        return;
    Ending_SetSubState(p, Ending_SubState_SonicFinish);
}
static void Ending_SubState_SonicFadeIn(Ending *p) {
    g_cs_bright = 0x10;
    Ending_SetSubState(p, Ending_SubState_SonicShowPicture0);
}
static void Ending_SubState_SonicStart(Ending *p) {
    g_cs_bright = 0x10;
    g_cs_bright_sub = 0x10;
    Ending_SetSubState(p, Ending_SubState_SonicFadeIn);
}

/* ---- Blaze */
static void Ending_SubState_BlazeShowPicture0(Ending *p);
static void Ending_SubState_BlazeShowPicture1(Ending *p);
static void Ending_SubState_BlazeShowPicture2(Ending *p);
static void Ending_SubState_BlazeShowPicture3(Ending *p);
static void Ending_SubState_BlazeShowPicture4(Ending *p);
static void Ending_SubState_BlazeFinish(Ending *p) {
    g_cs_bright = 16;
    g_cs_bright_sub = 16;
    Ending_SetSubState(p, NULL);
    Ending_State_GoToStaffRoll(p);
}
static void Ending_SubState_BlazeNextPicture1(Ending *p) {
    g_cs_bright = 16;
    Ending_CreateBg0PictureTask(p);
    Ending_DrawNextPicture(p);
    Ending_SetSubState(p, Ending_SubState_BlazeShowPicture1);
}
static void Ending_SubState_BlazeNextPicture2(Ending *p) {
    g_cs_bright = 16;
    Ending_CreateBg0PictureTask(p);
    Ending_DrawNextPicture(p);
    Ending_SetSubState(p, Ending_SubState_BlazeShowPicture2);
}
static void Ending_SubState_BlazeNextPicture3(Ending *p) {
    g_cs_bright = 16;
    Ending_CreateBg0PictureTask(p);
    Ending_DrawNextPicture(p);
    Ending_SetSubState(p, Ending_SubState_BlazeShowPicture3);
    p->bg3_pic = 16;
    p->layers = 0xb;
    ed_set_affine_scale(p, 0, 0);
}
static void Ending_SubState_BlazeNextPicture4(Ending *p) {
    g_cs_bright = 16;
    Ending_CreateBg0PictureTask(p);
    Ending_DrawNextPicture(p);
    Ending_SetSubState(p, Ending_SubState_BlazeShowPicture4);
    p->layers = 3;
}
static void Ending_SubState_BlazeShowPicture0(Ending *p) {
    if (p->t >= 5)
        Ending_FadeIn(p, p->t - 5, 0x3c, 1, 1);
    if (p->t < 0x16d)
        return;
    if (!Ending_FadeOut(p, p->t - 0x16d, 0x3c, 1, 0))
        return;
    Ending_SetSubState(p, Ending_SubState_BlazeNextPicture1);
}
static void Ending_SubState_BlazeShowPicture1(Ending *p) {
    if (p->t >= 5)
        Ending_FadeIn(p, p->t - 5, 0x3c, 1, 0);
    if (p->t < 0x16d)
        return;
    if (!Ending_FadeOut(p, p->t - 0x16d, 0x3c, 1, 0))
        return;
    Ending_SetSubState(p, Ending_SubState_BlazeNextPicture2);
}
static void Ending_SubState_BlazeShowPicture2(Ending *p) {
    if (p->t >= 5)
        Ending_FadeIn(p, p->t - 5, 0x3c, 1, 0);
    if (p->t >= 0x113 && Ending_FadeOut(p, p->t - 0x113, 0x1e, 1, 0)) {
        Ending_SetSubState(p, Ending_SubState_BlazeNextPicture3);
        return;
    }
    if (p->cap == p->cap_prev)
        return;
    p->bg2_hofs = 0;
    p->bg_show_task = 1;   /* EndingBgLayersShow_Main (0xf400): BG0-2 */
}
static void Ending_SubState_BlazeShowPicture3(Ending *p) {
    if (p->t >= 5)
        Ending_FadeIn(p, p->t - 5, 0x1e, 1, 0);
    if (p->t >= 0x113 && Ending_FadeOut(p, p->t - 0x113, 0x3c, 1, 0)) {
        Ending_SetSubState(p, Ending_SubState_BlazeNextPicture4);
        return;
    }
    Ending_UpdatePictureRotZoom(p);
}
static void Ending_SubState_BlazeShowPicture4(Ending *p) {
    if (p->t >= 5)
        Ending_FadeIn(p, p->t - 5, 0x3c, 1, 0);
    if (p->t >= 0x389)
        Ending_FadeOut(p, p->t - 0x389, 0xb4, 1, 1);
    if (p->t == 0x389)
        cs_bgm_stop(0xb4);
    if (p->t < 0x447)
        return;
    Ending_SetSubState(p, Ending_SubState_BlazeFinish);
}
static void Ending_SubState_BlazeFadeIn(Ending *p) {
    g_cs_bright = 0x10;
    Ending_SetSubState(p, Ending_SubState_BlazeShowPicture0);
}
static void Ending_SubState_BlazeStart(Ending *p) {
    g_cs_bright = 0x10;
    g_cs_bright_sub = 0x10;
    Ending_SetSubState(p, Ending_SubState_BlazeFadeIn);
}

static void Ending_LoadAssets(Ending *w) {
    const char *pics = (const char *)cs_sec("ed_pics", NULL);
    const char *spr = (const char *)cs_sec("ed_spr", NULL);
    int i;
    for (i = 0; i < 19 && pics; i++)
        layer_load(&w->bg[i], pics + 32 * ((w->chara * 6 + cs_lang()) * 19 + i));
    for (i = 0; i < 2 && spr; i++)
        w->hdr[i] = cs_res_load(spr + 48 * (cs_lang() * 2 + i));
}
static void Ending_InitBackgroundsAndSprites(Ending *w) {
    const u8 *t = edtab();
    int i;
    if (!t)
        return;
    for (i = 0; i < 5; i++) {
        u8 r = t[i];
        u16 anim = *(const u16 *)(t + 0x24 + 2 * i);
        AnimatorSprite_Init(&w->spr[i], w->hdr[r & 1], anim, SPR_LOOP);
        AnimatorSprite_ProcessAnimation(&w->spr[i]);
    }
}

void Ending_Create(void) {
    Ending *w = &s_ed;
    const u8 *t;
    Ending_Kill();
    memset(w, 0, sizeof(*w));
    w->chara = g_cs_story.character == 0 ? 0 : 1;
    if (w->chara == 0) {
        if (g_cs_story.sonic_progress >= 0x1a)
            w->skippable = 1;
        Ending_SetSubState(w, Ending_SubState_SonicStart);
    } else {
        if (g_cs_story.blaze_progress >= 0x1c)
            w->skippable = 1;
        Ending_SetSubState(w, Ending_SubState_BlazeStart);
    }
    Ending_LoadAssets(w);
    Ending_InitBackgroundsAndSprites(w);
    w->bg2_hofs = -70;   /* Ending_InitDisplayOffsets: BG0 vofs -144, BG2 hofs -70 */
    w->bg3_pic = 17;
    w->layers = 3;       /* BG0 + BG1 */
    s_ed_on = 1;
    t = edtab();
    cs_bgm_play(t ? *(const s32 *)(t + 8 + 4 * w->chara) : 21);   /* SEQ_FINAL_SONIC */
    cs_log("ending %d start skippable %d", w->chara, w->skippable);
}

static void Ending_Main(void) {
    Ending *w = &s_ed;
    w->frame++;
    w->t++;
    if (w->skippable == 1) {
        AnimatorSprite_ProcessAnimation(&w->spr[4]);
        AnimatorSprite_ProcessAnimation(&w->spr[2]);
    }
    if (w->frame > 0x3c && w->skippable == 1 && w->skip == 0 && (gPad.trg & PAD_START)) {
        w->skip = 1;
        w->skip_t = 0x3c;
        cs_bgm_stop(0x3c);
        AnimatorSprite_SetAnimation(&w->spr[4], 2);
        AnimatorSprite_SetAnimation(&w->spr[2], 3);
        cs_log("ending skip");
    }
    AnimatorSprite_ProcessAnimation(&w->spr[0]);
    AnimatorSprite_ProcessAnimation(&w->spr[1]);
    AnimatorSprite_ProcessAnimation(&w->spr[3]);
    Ending_CheckTimedBg0Picture(w);
    if (w->sub)
        w->sub(w);
    if (!s_ed_on)
        return;
    if (w->skip != 0) {
        w->skip_t--;
        if (w->skip_t % 3 == 0) {
            g_cs_bright++;
            g_cs_bright_sub++;
        }
        if (g_cs_bright > 16)
            g_cs_bright = 16;
        if (g_cs_bright_sub > 16)
            g_cs_bright_sub = 16;
        if (w->skip_t <= 0 && g_cs_bright == 16 && g_cs_bright_sub == 16) {
            Ending_SetSubState(w, NULL);
            Ending_State_GoToStaffRoll(w);
            return;
        }
    }
    /* tasks created this frame: EndingBg0Hide (0xf200) / EndingBgLayersShow (0xf400) */
    if (w->bg0_hide_task) {
        w->layers &= ~1u;
        w->bg0_hide_task = 0;
    }
    if (w->bg_show_task) {
        w->layers = 7;
        w->bg_show_task = 0;
    }
    w->cap_prev = w->cap;
}

static void ending_skip_button(CsSpr *btn, CsSpr *txt, int blink, float sx0, float sy0) {
    cs_view_set(sx0, sy0, 1.5f, 640 - 192 - 24, 18);
    if (btn)
        AnimatorSprite_Draw(btn);
    if (txt && blink)
        AnimatorSprite_Draw(txt);
    cs_view_full();
}

static void Ending_DrawMain(void) {
    Ending *w = &s_ed;
    g_cs_backdrop = 0xff000000;
    cs_view_full();
    /* main: BG1 picture (prio 3), BG2 (2), BG3 affine (1), BG0 caption (0) */
    if (w->layers & 2 && w->pic >= 0 && w->pic < 5)
        layer_draw(&w->bg[w->pic], 0, 0);
    if (w->layers & 4)
        layer_draw(&w->bg[15], -w->bg2_hofs, 0);
    if (w->layers & 8) {
        CsTex *t = layer_tex(&w->bg[w->bg3_pic]);
        if (t)
            cs_quad_affine(t, w->m3, w->c3x, w->c3y, w->o3x, w->o3y, 0xffffffff);
    }
    if ((w->layers & 1) && w->cap + 5 < 15)
        layer_draw(&w->bg[5 + w->cap], 0, 144);
    /* bottom screen: the skip button (ac poly_demo_under2 2/3 + the language text) */
    if (w->skippable == 1)
        ending_skip_button(&w->spr[2], &w->spr[4], w->skip != 0 || (w->frame & 0x30), 64, 118);
}

/* ================================================================ ExtraEnding */
typedef void (*XeState)(void);
static XeState s_xe_fn;   /* unk4 */
static s32 s_xe_frame, s_xe_t;   /* unk0 / unk8 */
static int s_xe_goto;     /* the task's main replaced by ExtraEnding_State_GoToStaffRoll */
static void ExtraEnding_State_None(void) {}
static void ExtraEnding_SetState(XeState f) {
    s_xe_fn = f ? f : ExtraEnding_State_None;
    s_xe_t = 0;
}
static void ExtraEnding_State_End(void);
static void ExtraEnding_State_PlayDemo(void) {
    if (PlayerDemo_IsActive() == 1)
        return;
    ExtraEnding_SetState(ExtraEnding_State_End);
}
static void ExtraEnding_State_StartDemo(void) {
    g_cs_bright = 0x10;
    g_cs_bright_sub = 0x10;
    gPlayerDemoId = 0x2a;
    PlayerDemo_Start();
    ExtraEnding_SetState(ExtraEnding_State_PlayDemo);
}
static void ExtraEnding_State_End(void) {
    g_cs_bright = -16;
    g_cs_bright_sub = -16;
    s_xe_goto = 1;
    ExtraEnding_SetState(ExtraEnding_State_None);
}
static void ExtraEnding_State_GoToStaffRoll(void) {
    /* Task_DestroyByTag(0x1173): ExtraEnding_Destructor -> ExtraEnding_ReleaseSound */
    s_xe_on = 0;
    cs_bgm_stop(0);
    cs_scene_change(CS_NEXT_EXTRA_STAFFROLL);
}
void ExtraEnding_Create(void) {
    Ending_Kill();
    s_xe_on = 1;
    s_xe_frame = 0;
    s_xe_goto = 0;
    ExtraEnding_SetState(ExtraEnding_State_StartDemo);
    /* ExtraEnding_InitDisplay; ExtraEnding_LoadSound: the extra_end SDAT,
     * Sound_PlaySeq(0, 0) = SEQ_EXTRA_END (streamed as extra_end.wav) */
    cs_bgm_play_file("extra_end", 0);
    cs_log("extra ending start");
}
static void ExtraEnding_Main(void) {
    if (s_xe_goto) {
        ExtraEnding_State_GoToStaffRoll();
        return;
    }
    s_xe_frame++;
    s_xe_t++;
    s_xe_fn();
}

/* ================================================================ StaffRoll */
static const u8 *srtab(void) { return (const u8 *)cs_sec("sr_tab", NULL); }
static void StaffRoll_SetState(StaffRoll *p, void (*f)(StaffRoll *)) {
    p->fn = f;
    p->t = 0;
}
static void StaffRoll_SetPictureState(StaffRoll *p, void (*f)(StaffRoll *)) {
    p->pic_fn = f;
    p->pic_t = 0;
}
static void StaffRoll_SetMainBrightness(s16 v) {
    if (s_sr.skip == 0)
        g_cs_bright = v;
}
static void StaffRoll_SetSubBrightness(s16 v) {
    if (s_sr.skip == 0)
        g_cs_bright_sub = v;
}
static void StaffRoll_CalcStopMotion(float *a, float *b, float t, float d) {
    if (a)
        *a = 2.0f * d / t;
    if (b)
        *b = -(2.0f * d) / (t * t);
}
static s32 fx(float x) { return (s32)(x > 0.0f ? x * 4096.0f + 0.5f : x * 4096.0f - 0.5f); }
static void ps_anim(StaffRoll *p, int i, u16 a) { AnimatorSprite_SetAnimation(&p->ps[i], a); }
#define PSF(p, i) ((p)->ps[i].flags)

static void StaffRoll_CreateCreditTask(StaffRoll *p, int idx) {
    SrCredit *c = &p->cred[idx];
    c->on = 1;
    c->idx = idx;
    c->x = 0x80;
    c->y = 0x1d0;
    cs_log("staffroll credit %d", idx);
}
static void StaffRoll_DestroyCreditTask(StaffRoll *p, int idx) { p->cred[idx].on = 0; }
static void StaffRollCredit_Main(StaffRoll *p) {
    int i;
    for (i = 0; i < 4; i++) {
        SrCredit *w = &p->cred[i];
        s16 l, t, r, b;
        s32 h = 0;
        if (!w->on)
            continue;
        if (AnimatorSprite_FrameBox(&p->credit[i], &l, &t, &r, &b))
            h = b - t;
        w->y--;
        if (i == 2) {
            if (w->y < 16)
                w->y = 16;
        } else if (i == 3) {
            w->x = 0;
            w->y = 0x110;
        }
        AnimatorSprite_ProcessAnimation(&p->credit[i]);
        p->credit[i].x = (s16)w->x;
        p->credit[i].y = (s16)w->y;
        if (w->y + h < 0)
            StaffRoll_DestroyCreditTask(p, i);
    }
}

/* picture states (the characters running across the main screen) */
static void StaffRoll_AdvancePictureAnim(StaffRoll *p) {
    if (PSF(p, 0) & SPR_ENDED) {
        switch (p->ps[0].anim) {
        case 0x1a: ps_anim(p, 0, 0x1b); PSF(p, 0) &= ~(u32)SPR_LOOP; break;
        case 0x1b: ps_anim(p, 0, 0x1c); PSF(p, 0) &= ~(u32)SPR_LOOP; break;
        case 0x1c: ps_anim(p, 0, 0x1d); PSF(p, 0) |= SPR_LOOP; break;
        }
    }
}
static void StaffRoll_PictureState_None(StaffRoll *p) { (void)p; }
#define LOOPS(p, a, b, c) do { if (a) PSF(p, 0) |= SPR_LOOP; if (b) PSF(p, 1) |= SPR_LOOP; if (c) PSF(p, 2) |= SPR_LOOP; } while (0)
#define UNFLIP(p, a, b, c) do { if (a) PSF(p, 0) &= ~(u32)SPR_HFLIP; if (b) PSF(p, 1) &= ~(u32)SPR_HFLIP; if (c) PSF(p, 2) &= ~(u32)SPR_HFLIP; } while (0)
/* Sonic */
static void StaffRoll_PictureState_Sonic2(StaffRoll *p);
static void StaffRoll_PictureState_Sonic4(StaffRoll *p);
static void StaffRoll_PictureState_Sonic6(StaffRoll *p);
static void StaffRoll_PictureState_Sonic8(StaffRoll *p);
static void StaffRoll_PictureState_Sonic10(StaffRoll *p);
static void StaffRoll_PictureState_Sonic12(StaffRoll *p);
static void StaffRoll_PictureState_Sonic14(StaffRoll *p);
static void StaffRoll_PictureState_Sonic16(StaffRoll *p);
static void StaffRoll_PictureState_Sonic1(StaffRoll *p) {
    LOOPS(p, 1, 1, 0);
    UNFLIP(p, 1, 1, 0);
    p->px[0] = 0x46000; p->py[0] = 0x78000; p->px[1] = 0xb4000; p->py[1] = 0x78000;
    p->pvx[0] = 0; p->pvx[1] = 0;
    ps_anim(p, 0, 0x12);
    ps_anim(p, 1, 0x18);
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic2);
}
static void StaffRoll_PictureState_Sonic3(StaffRoll *p);
static void StaffRoll_PictureState_Sonic2(StaffRoll *p) { if (p->pic_end == 1) StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic3); }
static void StaffRoll_PictureState_Sonic3(StaffRoll *p) {
    LOOPS(p, 1, 1, 0);
    UNFLIP(p, 1, 1, 0);
    p->py[0] = 0x78000; p->py[1] = 0x78000;
    p->pvx[0] = -4096; p->pvx[1] = -2048; p->pax[0] = -192; p->pax[1] = -128;
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic4);
}
static void StaffRoll_PictureState_Sonic5(StaffRoll *p);
static void StaffRoll_PictureState_Sonic4(StaffRoll *p) {
    if (p->pvx[0] <= -0x3000) p->pvx[0] = -0x3000;
    if (p->pvx[1] <= -0x3000) p->pvx[1] = -0x3000;
    if (p->px[0] < -0x28000) p->px[0] = -0x28000;
    if (p->px[1] < -0x28000) p->px[1] = -0x28000;
    if (p->pic_t > 0xb4) StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic5);
}
static void StaffRoll_PictureState_Sonic5(StaffRoll *p) {
    float a, b;
    PSF(p, 0) |= SPR_LOOP | SPR_HFLIP;
    PSF(p, 1) |= SPR_HFLIP;
    p->px[0] = -0x14000; p->py[0] = 0x78000; p->pax[0] = 0;
    p->px[1] = -0x28000; p->py[1] = 0x78000; p->pvx[1] = 0; p->pax[1] = 0;
    StaffRoll_CalcStopMotion(&a, &b, 60.0f, 150.0f);
    p->pvx[0] = fx(a);
    p->pax[0] = fx(b);
    ps_anim(p, 0, 0x13);
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic6);
}
static void StaffRoll_PictureState_Sonic7(StaffRoll *p);
static void StaffRoll_PictureState_Sonic6(StaffRoll *p) {
    if (0x82000 <= p->px[0]) {
        p->pvx[0] = 0;
        p->pax[0] = 0;
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic7);
    }
}
static void StaffRoll_PictureState_Sonic7(StaffRoll *p) {
    PSF(p, 0) &= ~(u32)SPR_LOOP;
    PSF(p, 0) |= SPR_HFLIP;
    PSF(p, 1) |= SPR_HFLIP;
    p->py[0] = 0x78000; p->pvx[0] = 0; p->pax[0] = 0;
    p->px[1] = -0x28000; p->py[1] = 0x78000; p->pvx[1] = 0; p->pax[1] = 0;
    ps_anim(p, 0, 0x14);
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic8);
}
static void StaffRoll_PictureState_Sonic9(StaffRoll *p);
static void StaffRoll_PictureState_Sonic8(StaffRoll *p) { if (p->pic_t > 0x3c) StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic9); }
static void StaffRoll_PictureState_Sonic9(StaffRoll *p) {
    PSF(p, 0) &= ~(u32)SPR_LOOP;
    PSF(p, 0) |= SPR_HFLIP;
    PSF(p, 1) |= SPR_HFLIP;
    p->py[0] = 0x78000; p->pvx[0] = 0; p->pax[0] = 0;
    p->px[1] = -0x28000; p->py[1] = 0x78000; p->pvx[1] = 0; p->pax[1] = 0;
    ps_anim(p, 0, 0x15);
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic10);
}
static void StaffRoll_PictureState_Sonic11(StaffRoll *p);
static void StaffRoll_PictureState_Sonic10(StaffRoll *p) { if (p->pic_t > 0x28) StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic11); }
static void StaffRoll_PictureState_Sonic11(StaffRoll *p) {
    PSF(p, 0) |= SPR_LOOP | SPR_HFLIP;
    PSF(p, 1) |= SPR_HFLIP;
    p->pvx[0] = 0; p->pax[0] = 0;
    p->px[1] = -0x28000; p->py[1] = 0x78000; p->pvx[1] = 0; p->pax[1] = 0;
    ps_anim(p, 0, 0x16);
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic12);
}
static void StaffRoll_PictureState_Sonic13(StaffRoll *p);
static void StaffRoll_PictureState_Sonic12(StaffRoll *p) { if (p->pic_t > 0x28) StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic13); }
static void StaffRoll_PictureState_Sonic13(StaffRoll *p) {
    PSF(p, 0) |= SPR_LOOP | SPR_HFLIP;
    PSF(p, 1) |= SPR_HFLIP;
    p->py[0] = 0x78000; p->pvx[0] = 0x9000; p->pax[0] = 0;
    p->px[1] = -0x28000; p->py[1] = 0x78000; p->pvx[1] = 0; p->pax[1] = 0;
    ps_anim(p, 0, 0x17);
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic14);
}
static void StaffRoll_PictureState_Sonic15(StaffRoll *p);
static void StaffRoll_PictureState_Sonic14(StaffRoll *p) { if (p->px[0] >= 0x11d000) StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic15); }
static void StaffRoll_PictureState_Sonic15(StaffRoll *p) {
    PSF(p, 0) |= SPR_LOOP | SPR_HFLIP;
    PSF(p, 1) |= SPR_HFLIP;
    p->py[0] = 0x78000; p->pvx[0] = 0; p->pax[0] = 0;
    p->px[1] = -0x1e000; p->py[1] = 0x78000; p->pvx[1] = 0x7000; p->pax[1] = 0;
    ps_anim(p, 1, 0x19);
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic16);
}
static void StaffRoll_PictureState_Sonic16(StaffRoll *p) {
    if (p->px[1] >= 0x131000) {
        p->pvx[1] = 0;
        p->pax[1] = 0;
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_None);
    }
}
static void StaffRoll_PictureState_SonicStart(StaffRoll *p) { StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic1); }
/* Blaze */
static void StaffRoll_PictureState_Blaze2(StaffRoll *p);
static void StaffRoll_PictureState_Blaze3(StaffRoll *p);
static void StaffRoll_PictureState_Blaze4(StaffRoll *p);
static void StaffRoll_PictureState_Blaze5(StaffRoll *p);
static void StaffRoll_PictureState_Blaze6(StaffRoll *p);
static void StaffRoll_PictureState_Blaze7(StaffRoll *p);
static void StaffRoll_PictureState_Blaze8(StaffRoll *p);
static void StaffRoll_PictureState_Blaze9(StaffRoll *p);
static void StaffRoll_PictureState_Blaze10(StaffRoll *p);
static void StaffRoll_PictureState_Blaze11(StaffRoll *p);
static void StaffRoll_PictureState_Blaze12(StaffRoll *p);
static void StaffRoll_PictureState_Blaze13(StaffRoll *p);
static void StaffRoll_PictureState_Blaze14(StaffRoll *p);
static void StaffRoll_PictureState_Blaze15(StaffRoll *p);
static void StaffRoll_PictureState_Blaze16(StaffRoll *p);
static void StaffRoll_PictureState_BlazeExit1(StaffRoll *p);
static void StaffRoll_PictureState_BlazeRepeat(StaffRoll *p);
static void StaffRoll_PictureState_Blaze1(StaffRoll *p) {
    LOOPS(p, 1, 0, 1);
    UNFLIP(p, 1, 0, 1);
    p->px[0] = 0xa0000; p->py[0] = 0x78000; p->pvx[0] = 0; p->pax[0] = 0;
    p->px[1] = -0x1e000; p->px[2] = p->px[0] + 0x3c000; p->py[2] = 0x46000;
    ps_anim(p, 0, 0x1a);
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze2);
}
static void StaffRoll_PictureState_Blaze2(StaffRoll *p) { if (p->pic_t > 0xb4) StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze3); }
static void StaffRoll_PictureState_Blaze3(StaffRoll *p) {
    float a, b;
    PSF(p, 0) &= ~(u32)SPR_LOOP;
    PSF(p, 1) |= SPR_LOOP;
    PSF(p, 2) |= SPR_LOOP;
    UNFLIP(p, 1, 1, 1);
    p->px[0] = 0xa0000; p->py[0] = 0x78000; p->pvx[0] = 0; p->pax[0] = 0;
    p->px[1] = -0x28000; p->py[1] = 0x78000;
    StaffRoll_CalcStopMotion(&a, &b, 220.0f, 140.0f);
    p->pvx[1] = fx(a);
    p->pax[1] = fx(b);
    p->pax[2] = -0x18;
    ps_anim(p, 1, 0x1f);
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze4);
}
static void StaffRoll_PictureState_Blaze4(StaffRoll *p) {
    StaffRoll_AdvancePictureAnim(p);
    if (PSF(p, 1) & SPR_ENDED)
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze5);
}
static void StaffRoll_PictureState_Blaze5(StaffRoll *p) {
    StaffRoll_AdvancePictureAnim(p);
    PSF(p, 1) &= ~(u32)SPR_LOOP;
    PSF(p, 2) |= SPR_LOOP;
    UNFLIP(p, 1, 1, 1);
    p->px[0] = 0xa0000; p->py[0] = 0x78000; p->pvx[0] = 0; p->pax[0] = 0;
    ps_anim(p, 1, 0x20);
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze6);
}
static void StaffRoll_PictureState_Blaze6(StaffRoll *p) {
    if (p->px[2] <= p->px[0] + 0x14000) {
        p->pvy[0] = 0;   /* unkcdc */
        p->pay[0] = 0;   /* unkcf4 */
    }
    StaffRoll_AdvancePictureAnim(p);
    if (PSF(p, 1) & SPR_ENDED)
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze7);
}
static void StaffRoll_PictureState_Blaze7(StaffRoll *p) {
    StaffRoll_AdvancePictureAnim(p);
    PSF(p, 1) |= SPR_LOOP;
    PSF(p, 2) |= SPR_LOOP;
    UNFLIP(p, 1, 1, 1);
    p->px[0] = 0xa0000; p->py[0] = 0x78000;
    p->chase = 1;
    ps_anim(p, 1, 0x21);
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze8);
}
static void StaffRoll_PictureState_Blaze8(StaffRoll *p) {
    StaffRoll_AdvancePictureAnim(p);
    if (p->px[1] >= 0x50000) {
        p->px[1] = 0x50000;
        p->pvx[1] = 0;
        p->pax[1] = 0;
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze9);
    }
}
static void StaffRoll_PictureState_Blaze9(StaffRoll *p) {
    StaffRoll_AdvancePictureAnim(p);
    if (p->pic_t > 0x78)
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze10);
}
static void StaffRoll_PictureState_Blaze10(StaffRoll *p) {
    float a, b;
    LOOPS(p, 1, 1, 1);
    UNFLIP(p, 1, 1, 1);
    p->px[0] = 0xa0000; p->py[0] = 0x78000; p->px[1] = 0x50000; p->py[1] = 0x78000;
    p->pvx[1] = 0; p->pax[1] = 0;
    StaffRoll_CalcStopMotion(&a, &b, 90.0f, -40.0f);
    p->pvx[0] = fx(a);
    p->pax[0] = fx(b);
    ps_anim(p, 0, 0x1c);
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze11);
}
static void StaffRoll_PictureState_Blaze11(StaffRoll *p) {
    StaffRoll_AdvancePictureAnim(p);
    if (p->pvx[0] + p->pax[0] > 0) {
        p->pvx[0] = 0;
        p->pax[0] = p->pax[0] / 4;
    }
    if (p->pic_t > 0x14) {
        PSF(p, 1) &= ~(u32)SPR_LOOP;
        if (PSF(p, 1) & SPR_ENDED)
            StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze12);
    }
}
static void StaffRoll_PictureState_Blaze12(StaffRoll *p) {
    StaffRoll_AdvancePictureAnim(p);
    LOOPS(p, 1, 1, 1);
    UNFLIP(p, 1, 1, 1);
    p->px[1] = 0x50000; p->py[1] = 0x78000; p->pvx[1] = 0; p->pax[1] = 0;
    ps_anim(p, 1, 0x22);
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze13);
}
static void StaffRoll_PictureState_Blaze13(StaffRoll *p) {
    StaffRoll_AdvancePictureAnim(p);
    if (p->px[1] > 0x50000) {
        p->px[1] = 0x50000;
        p->pvx[1] = 0;
        p->pax[1] = 0;
    }
    if (p->pvx[0] + p->pax[0] > 0) {
        p->pvx[0] = 0;
        p->pax[0] = p->pax[0] / 4;
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze14);
    }
}
static void StaffRoll_PictureState_Blaze14(StaffRoll *p) {
    StaffRoll_AdvancePictureAnim(p);
    if (p->pvx[0] > 0x400) {
        p->pvx[0] = 0x400;
        p->pax[0] = 0;
    }
    if (p->pic_t > 0xb4) {
        p->pvx[1] = -0x400;
        p->pax[1] = 0;
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze15);
    }
}
static void StaffRoll_PictureState_Blaze15(StaffRoll *p) {
    StaffRoll_AdvancePictureAnim(p);
    if (p->pvx[0] > 0x400) {
        p->pvx[0] = 0x400;
        p->pax[0] = 0;
    }
    if (p->px[0] - p->px[1] >= 0x96000) {
        p->pvx[0] = 0;
        p->pax[0] = 0;
        p->pvx[1] = 0;
        p->pax[1] = 0;
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze16);
    }
}
static void StaffRoll_PictureState_Blaze16(StaffRoll *p) {
    StaffRoll_AdvancePictureAnim(p);
    if (p->pic_t > 1) {
        if (p->pic_end == 1)
            StaffRoll_SetPictureState(p, StaffRoll_PictureState_BlazeExit1);
        else
            StaffRoll_SetPictureState(p, StaffRoll_PictureState_BlazeRepeat);
    }
}
static void StaffRoll_PictureState_BlazeRepeat(StaffRoll *p) {
    float a, b;
    p->py[0] = 0x78000;
    StaffRoll_CalcStopMotion(&a, &b, 90.0f, -70.0f);
    p->pvx[0] = fx(a);
    p->pax[0] = fx(b);
    StaffRoll_CalcStopMotion(&a, &b, 90.0f, 50.0f);
    p->pvx[1] = fx(a);
    p->pax[1] = fx(b);
    ps_anim(p, 0, 0x1c);
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze13);
}
static void StaffRoll_PictureState_BlazeExit2(StaffRoll *p);
static void StaffRoll_PictureState_BlazeExit3(StaffRoll *p);
static void StaffRoll_PictureState_BlazeExit1(StaffRoll *p) {
    LOOPS(p, 1, 1, 1);
    UNFLIP(p, 1, 1, 1);
    p->py[0] = 0x78000; p->pvx[0] = 0; p->pax[0] = 0;
    p->py[1] = 0x78000; p->pvx[1] = 0x1000; p->pax[1] = 0;
    ps_anim(p, 0, 0x1c);
    ps_anim(p, 1, 0x1f);
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_BlazeExit2);
}
static void StaffRoll_PictureState_BlazeExit2(StaffRoll *p) {
    if (0x6e000 < p->px[1]) {
        p->pvx[0] = -0x1000;
        p->pax[0] = 0;
        p->pvx[1] = 0;
        p->pax[1] = 0;
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_BlazeExit3);
    }
}
static void StaffRoll_PictureState_BlazeEnd(StaffRoll *p);
static void StaffRoll_PictureState_BlazeExit3(StaffRoll *p) {
    if (p->px[0] < 0x82000) {
        p->pvx[0] = 0;
        p->pax[0] = 0;
        ps_anim(p, 0, 0x1e);
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_BlazeEnd);
    }
}
static void StaffRoll_PictureState_BlazeEnd(StaffRoll *p) { (void)p; }
static void StaffRoll_PictureState_BlazeStart(StaffRoll *p) { StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze1); }

static void StaffRoll_UpdateChaseSpeed(StaffRoll *p) {
    s32 d;
    if (p->chase == 1) {
        d = p->px[0] + 0x14000 - p->px[2];
        if (d > 0)
            p->pvx[2] = 0x800;
        else if (d < -0x14000)
            p->pvx[2] = -0xc00;
        else
            p->pvx[2] = (s32)(p->pvx[2] * 0.9f);
        p->py[2] = 0x46000;
    }
}

/* ---- states */
static void StaffRoll_State_ScrollCredits(StaffRoll *p);
static void StaffRoll_State_StartFinalCredit(StaffRoll *p);
static void StaffRoll_State_WaitFinalCredit(StaffRoll *p);
static void StaffRoll_State_RevealBanner(StaffRoll *p);
static void StaffRoll_State_ShowEndPicture(StaffRoll *p);
static void StaffRoll_State_EndPictureHold(StaffRoll *p);
static void StaffRoll_State_FadeOut(StaffRoll *p);
static void StaffRoll_State_WaitEnd(StaffRoll *p);
static void StaffRoll_State_End(StaffRoll *p);
static void StaffRoll_State_ExtraShowPicture(StaffRoll *p);
static void StaffRoll_State_ExtraPictureFadeIn(StaffRoll *p);
static void StaffRoll_State_ExtraPictureHold(StaffRoll *p);
static void StaffRoll_State_ExtraPictureFadeOut(StaffRoll *p);
static void StaffRoll_State_ExtraNextPicture(StaffRoll *p);
static void StaffRoll_State_ExtraFadeOut(StaffRoll *p);
static void StaffRoll_State_ExtraShowFinalPicture(StaffRoll *p);
static void StaffRoll_State_ExtraFinalPictureFadeIn(StaffRoll *p);
static void StaffRoll_State_FadeInFromWhite(StaffRoll *p);
static void StaffRoll_State_FadeInFromBlack(StaffRoll *p);
static int s_banner_draw;


static void StaffRoll_State_BeginFadeIn(StaffRoll *p) {
    StaffRoll_SetState(p, p->mode == 2 ? StaffRoll_State_FadeInFromBlack : StaffRoll_State_FadeInFromWhite);
}
static void StaffRoll_State_Start(StaffRoll *p) {
    g_cs_bright = g_cs_bright_sub = p->mode == 2 ? -0x10 : 0x10;
    StaffRoll_SetState(p, StaffRoll_State_BeginFadeIn);
}
static void StaffRoll_State_FadeInFromBlack(StaffRoll *p) {
    if (g_cs_bright < 0 && (p->t & 1))
        StaffRoll_SetMainBrightness((s16)(g_cs_bright + 1));
    if (g_cs_bright_sub < 0 && (p->t & 1))
        StaffRoll_SetSubBrightness((s16)(g_cs_bright_sub + 1));
    if (g_cs_bright >= 0 && g_cs_bright_sub >= 0) {
        StaffRoll_SetMainBrightness(0);
        StaffRoll_SetSubBrightness(0);
        StaffRoll_SetState(p, StaffRoll_State_ScrollCredits);
    }
}
static void StaffRoll_State_FadeInFromWhite(StaffRoll *p) {
    if (g_cs_bright > 0 && (p->t & 1))
        StaffRoll_SetMainBrightness((s16)(g_cs_bright - 1));
    if (g_cs_bright_sub > 0 && (p->t & 1))
        StaffRoll_SetSubBrightness((s16)(g_cs_bright_sub - 1));
    if (g_cs_bright <= 0 && g_cs_bright_sub <= 0) {
        StaffRoll_SetMainBrightness(0);
        StaffRoll_SetSubBrightness(0);
        StaffRoll_SetState(p, StaffRoll_State_ScrollCredits);
    }
}
static void StaffRoll_ResetBgScroll(StaffRoll *p) { p->scroll_on = 0; }
static void StaffRoll_State_ScrollCredits(StaffRoll *p) {
    const s32 *ct = srtab() ? (const s32 *)(srtab() + 0x2c) : NULL;
    s32 prev = p->scroll++;
    int i;
    for (i = 0; i < 4 && ct; i++) {
        s32 t = ct[i] - 0x1d0;
        if (t > prev && t <= p->scroll && !p->cred[i].on)
            StaffRoll_CreateCreditTask(p, i);
    }
    if (p->scroll >= p->end) {
        StaffRoll_ResetBgScroll(p);
        StaffRoll_SetState(p, StaffRoll_State_StartFinalCredit);
    }
}
static void StaffRoll_State_StartFinalCredit(StaffRoll *p) {
    StaffRoll_CreateCreditTask(p, 2);
    StaffRoll_SetState(p, StaffRoll_State_WaitFinalCredit);
}
static void StaffRoll_State_WaitFinalCredit(StaffRoll *p) {
    if (p->t > 0x1d0)
        StaffRoll_SetState(p, StaffRoll_State_RevealBanner);
}
static void StaffRoll_State_RevealBanner(StaffRoll *p) {
    if (p->mode != 2 && p->skip == 0) {
        p->skippable = 0;
        p->skip = 0;
    }
    s_banner_draw = 1;
    if (p->t > 0xf0) {
        p->pic_end = 1;
        cs_log("staffroll banner done");
        if (p->mode == 2) {
            p->main_layers = 0x18;
            StaffRoll_SetState(p, StaffRoll_State_ExtraShowPicture);
            return;
        }
        p->main_layers = 0x18;
        p->sub_layers = 0x18;
        layer_set_fade(&p->bg[2], 0);   /* StaffRoll_ShowBgBlackPalette (SEGA logo) */
        p->sub_card = 1;
        StaffRoll_SetState(p, StaffRoll_State_ShowEndPicture);
    }
}
static void StaffRoll_State_ShowEndPicture(StaffRoll *p) {
    p->sub_layers = 0x1a;
    g_cs_bright = 0;
    g_cs_bright_sub = 0;
    s_banner_draw = 2;
    if (p->t <= 0x40)
        layer_set_fade(&p->bg[2], p->t / 64.0f);
    if (p->t > 0x5e) {
        StaffRoll_CreateCreditTask(p, 3);
        p->credit_fade[3] = 0;   /* StaffRoll_BlackOutPicturePalette */
        StaffRoll_SetState(p, StaffRoll_State_EndPictureHold);
    }
}
static void StaffRoll_State_EndPictureHold(StaffRoll *p) {
    s_banner_draw = 2;
    if (p->t <= 0x40)
        p->sprite_fade = p->t / 64.0f;   /* StaffRoll_FadeSpritePalette */
    if (p->t > 0x1c6)
        StaffRoll_SetState(p, StaffRoll_State_FadeOut);
}
static void StaffRoll_State_FadeOut(StaffRoll *p) {
    if (p->mode != 2)
        s_banner_draw = 2;
    if (g_cs_bright > -16 && (p->t & 1))
        StaffRoll_SetMainBrightness((s16)(g_cs_bright - 1));
    if (g_cs_bright_sub > -16 && (p->t & 1))
        StaffRoll_SetSubBrightness((s16)(g_cs_bright_sub - 1));
    if (g_cs_bright <= -16 && g_cs_bright_sub <= -16) {
        StaffRoll_SetMainBrightness(-16);
        StaffRoll_SetSubBrightness(-16);
        StaffRoll_SetState(p, p->mode == 2 ? StaffRoll_State_End : StaffRoll_State_WaitEnd);
    }
}
static void StaffRoll_State_WaitEnd(StaffRoll *p) {
    if (p->t >= 0x5a)
        StaffRoll_SetState(p, StaffRoll_State_End);
}
static void StaffRoll_State_Finish(StaffRoll *p) {
    s_sr_on = 0;
    switch (p->mode) {
    case 0: g_cs_result.set_sonic_progress = 0x1a; break;
    case 1: g_cs_result.set_blaze_progress = 0x1c; break;
    case 2: g_cs_result.game_cleared = 1; break;
    }
    cs_bgm_stop(0);
    cs_log("staffroll %d finish", p->mode);
    cs_scene_change(CS_NEXT_POST_STAFFROLL);
}
static void StaffRoll_State_End(StaffRoll *p) {
    int i;
    p->scroll_on = 0;
    for (i = 0; i < 4; i++)
        StaffRoll_DestroyCreditTask(p, i);   /* StaffRoll_State_ReleasePictures */
    StaffRoll_SetState(p, StaffRoll_State_Finish);
}
static void StaffRoll_State_ExtraShowPicture(StaffRoll *p) {
    s_banner_draw = 2;
    p->main_layers = 0x1a;
    p->main_bg1 = p->extra_n + 0xb;
    layer_set_fade(&p->bg[p->main_bg1], 0);
    cs_log("staffroll extra picture %d", p->extra_n);
    StaffRoll_SetState(p, StaffRoll_State_ExtraPictureFadeIn);
}
static void StaffRoll_State_ExtraPictureFadeIn(StaffRoll *p) {
    s_banner_draw = 2;
    if (p->t <= 0x90) {
        layer_set_fade(&p->bg[p->main_bg1], p->t / 144.0f);
        return;
    }
    StaffRoll_SetState(p, StaffRoll_State_ExtraPictureHold);
}
static void StaffRoll_State_ExtraPictureHold(StaffRoll *p) {
    s_banner_draw = 2;
    if (p->t > 0x12c) {
        if (p->extra_n + 1 >= 4) {
            StaffRoll_SetState(p, StaffRoll_State_ExtraFadeOut);
            return;
        }
        StaffRoll_SetState(p, StaffRoll_State_ExtraPictureFadeOut);
    }
}
static void StaffRoll_State_ExtraPictureFadeOut(StaffRoll *p) {
    s_banner_draw = 2;
    if (p->t <= 0x90) {
        layer_set_fade(&p->bg[p->main_bg1], 1.0f - p->t / 144.0f);
        return;
    }
    StaffRoll_SetState(p, StaffRoll_State_ExtraNextPicture);
}
static void StaffRoll_State_ExtraNextPicture(StaffRoll *p) {
    s_banner_draw = 2;
    p->extra_n++;
    if (p->extra_n >= 4) {
        StaffRoll_SetState(p, StaffRoll_State_ExtraFadeOut);
        return;
    }
    StaffRoll_SetState(p, StaffRoll_State_ExtraShowPicture);
}
static void StaffRoll_State_ExtraFadeOut(StaffRoll *p) {
    float f;
    p->skippable = 0;
    p->skip = 0;
    s_banner_draw = 2;
    if (p->t <= 0x90) {
        f = p->t / 144.0f;
        if (f > 1.0f)
            f = 1.0f;
        StaffRoll_SetMainBrightness((s16)(-16.0f * f));
        return;
    }
    StaffRoll_SetState(p, StaffRoll_State_ExtraShowFinalPicture);
}
static void StaffRoll_State_ExtraShowFinalPicture(StaffRoll *p) {
    int i;
    StaffRoll_SetMainBrightness(-0x10);
    p->main_layers = 0x8;
    p->sub_layers = 0x18;
    layer_set_fade(&p->bg[2], 0);
    StaffRoll_CreateCreditTask(p, 3);
    p->credit_fade[3] = 0;
    p->sprite_fade = 0;
    p->alpha_bg1 = 0;
    p->main_bg1 = 10;   /* ed_extra_end */
    layer_set_fade(&p->bg[10], 1);
    p->sub_card = 1;
    StaffRoll_DestroyCreditTask(p, 2);
    for (i = 0; i < 4; i++)
        if (i != 3)
            StaffRoll_DestroyCreditTask(p, i);
    StaffRoll_SetState(p, StaffRoll_State_ExtraFinalPictureFadeIn);
}
static void StaffRoll_State_ExtraFinalPictureFadeIn(StaffRoll *p) {
    float f;
    p->sub_layers = 0x1a;
    p->main_layers = 0xa;
    if (p->t <= 0x78) {
        f = p->t / 120.0f;
        if (f > 1.0f)
            f = 1.0f;
        f = f * f;
        layer_set_fade(&p->bg[2], f);
        p->sprite_fade = f;
    }
    if (g_cs_bright < 0 || p->alpha_bg1 < 1.0f) {
        if ((p->t & 7) == 4)
            StaffRoll_SetMainBrightness((s16)(g_cs_bright + 1));
        else if ((p->t & 7) == 0) {
            p->alpha_bg1 += 1.0f / 15.0f;
            if (p->alpha_bg1 > 1.0f)
                p->alpha_bg1 = 1.0f;
        }
    }
    if (p->t > 0x1c6)
        StaffRoll_SetState(p, StaffRoll_State_FadeOut);
}
static void StaffRoll_State_SkipFadeIn(StaffRoll *p) {
    if (p->t == 1) {
        StaffRoll_CreateCreditTask(p, 2);
        p->cred[2].y = 0x10;
        p->main_layers = 0x18;
        p->sub_layers = 0x8;
    }
    if (g_cs_bright < 0)
        g_cs_bright++;
    if (g_cs_bright_sub < 0)
        g_cs_bright_sub++;
    if (g_cs_bright >= 0 && g_cs_bright_sub >= 0) {
        g_cs_bright = g_cs_bright_sub = 0;
        StaffRoll_SetState(p, StaffRoll_State_RevealBanner);
        p->t = 0xf0;
    }
}
static void StaffRoll_State_SkipSonic(StaffRoll *p) {
    ps_anim(p, 0, 0x12);
    ps_anim(p, 1, 0x18);
    StaffRoll_SetState(p, StaffRoll_State_SkipFadeIn);
}
static void StaffRoll_State_SkipBlaze(StaffRoll *p) {
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze15);
    LOOPS(p, 1, 1, 1);
    UNFLIP(p, 1, 1, 1);
    p->px[0] = 0xbe000; p->py[0] = 0x78000; p->px[1] = 0x28000; p->py[1] = 0x78000;
    p->px[2] = p->px[0] + 0x14000; p->py[2] = 0x46000;
    ps_anim(p, 0, 0x1d);
    ps_anim(p, 1, 0x22);
    ps_anim(p, 2, 0x23);
    p->pvx[0] = p->pax[0] = p->pvx[1] = p->pax[1] = p->pvx[2] = p->pax[2] = 0;
    p->chase = 1;
    StaffRoll_SetState(p, StaffRoll_State_SkipFadeIn);
}
static void StaffRoll_State_SkipExtra(StaffRoll *p) {
    g_cs_bright = g_cs_bright_sub = 0;
    StaffRoll_SetState(p, StaffRoll_State_ExtraShowFinalPicture);
}
static void StaffRoll_State_Skip(StaffRoll *p) {
    int i;
    p->skippable = 0;
    p->skip = 0;
    StaffRoll_ResetBgScroll(p);
    for (i = 0; i < 4; i++)
        StaffRoll_DestroyCreditTask(p, i);
    cs_log("staffroll skip");
    switch (p->mode) {
    case 0: StaffRoll_SetState(p, StaffRoll_State_SkipSonic); break;
    case 1: StaffRoll_SetState(p, StaffRoll_State_SkipBlaze); break;
    case 2: StaffRoll_SetState(p, StaffRoll_State_SkipExtra); break;
    }
}

static void StaffRoll_LoadAssets(StaffRoll *p) {
    const char *bgs = (const char *)cs_sec("sr_bgs", NULL), *spr = (const char *)cs_sec("sr_spr", NULL);
    int i, j;
    /* gStaffRollBgFileList / gStaffRollExtraBgFileList, loaded verbatim (the EUR
     * table names ed_staff03_eng for every language) */
    for (i = 0; i < 15 && bgs; i++) {
        const char *nm = bgs + 32 * ((p->mode == 2 ? 15 : 0) + i);
        /* 4..6: the credits column; 7..9: the sub screen's copy of the same maps
         * (the DS credits ran across both screens; one column here, section 3) */
        if (!nm[0] || (i >= 7 && i <= 9))
            continue;
        if (i >= 4 && i <= 6) {
            staff_load(&p->bg[i], nm);
            continue;
        }
        /* the same picture twice in the list (extra: ed_base_b on main and sub):
         * one disc read, two copies */
        for (j = 0; j < i; j++)
            if (p->bg[j].ok && !(j >= 4 && j <= 9) && !strcmp(bgs + 32 * ((p->mode == 2 ? 15 : 0) + j), nm))
                break;
        if (j < i && layer_copy(&p->bg[i], &p->bg[j]) == 0)
            continue;
        layer_load(&p->bg[i], nm);
    }
    for (i = 0; i < 3 && spr; i++)
        p->res[i] = cs_res_load(spr + 48 * (cs_lang() * 3 + i));
}
static void StaffRoll_InitSprites(StaffRoll *w) {
    const u8 *t = srtab();
    int i;
    for (i = 0; i < 18; i++) {
        AnimatorSprite_Init(&w->banner[i], w->res[0], (u16)i, 0);
        AnimatorSprite_ProcessAnimation(&w->banner[i]);
    }
    AnimatorSprite_Init(&w->skiptxt, w->res[1], 0, SPR_LOOP);
    AnimatorSprite_ProcessAnimation(&w->skiptxt);
    AnimatorSprite_Init(&w->skipbtn, w->res[0], 0x24, SPR_LOOP);
    AnimatorSprite_ProcessAnimation(&w->skipbtn);
    if (w->mode != 2 && t) {
        const u16 *a = (const u16 *)(t + 0x10);
        for (i = 0; i < 3; i++) {
            AnimatorSprite_Init(&w->ps[i], w->res[0], w->mode == 0 ? a[i] : a[3 + i], SPR_LOOP);
            AnimatorSprite_ProcessAnimation(&w->ps[i]);
        }
        if (w->mode == 0)
            w->ps[2].flags |= SPR_HIDDEN;
    }
    for (i = 0; i < 4 && t; i++) {
        AnimatorSprite_Init(&w->credit[i], w->res[2], *(const u16 *)(t + 8 + 2 * i), SPR_LOOP);
        AnimatorSprite_ProcessAnimation(&w->credit[i]);
        w->credit_fade[i] = 255;
    }
}

void StaffRoll_Create(s32 mode) {
    StaffRoll *w = &s_sr;
    int i;
    Ending_Kill();
    memset(w, 0, sizeof(*w));
    w->mode = mode;
    switch (mode) {
    case 0:
        if (g_cs_story.sonic_progress >= 0x1a)
            w->skippable = 1;
        StaffRoll_SetPictureState(w, StaffRoll_PictureState_SonicStart);
        break;
    case 1:
        if (g_cs_story.blaze_progress >= 0x1c)
            w->skippable = 1;
        StaffRoll_SetPictureState(w, StaffRoll_PictureState_BlazeStart);
        break;
    case 2:
        if (g_cs_story.save_flags & 0x1000)
            w->skippable = 1;
        StaffRoll_SetPictureState(w, StaffRoll_PictureState_None);
        break;
    }
    StaffRoll_SetState(w, StaffRoll_State_Start);
    StaffRoll_LoadAssets(w);
    StaffRoll_InitSprites(w);
    w->end = (w->bg[4].ok ? w->bg[4].pic.img.h : 9600) - 0x1d0;
    w->scroll_on = 1;   /* StaffRoll_ScrollBackgrounds task (0xf100) */
    w->main_layers = 0x1f;
    w->sub_layers = 0x1f;
    w->sprite_fade = 1.0f;
    w->alpha_bg1 = 1.0f;
    w->main_bg1 = -1;
    for (i = 0; i < 3; i++)
        w->staff_row[i] = -1000;
    s_sr_on = 1;
    /* NNS_SndArcStrmStart(strm, mode == 2 ? 1 : 0): Final_Roll / Extra_Roll */
    cs_bgm_play_file(mode == 2 ? "Extra_Roll" : "Final_Roll", 0);
    cs_log("staffroll %d start end %d skippable %d", mode, w->end, w->skippable);
}

static void StaffRoll_Main(void) {
    StaffRoll *w = &s_sr;
    int i;
    w->frame++;
    w->t++;
    s_banner_draw = 0;
    if (w->skippable == 1) {
        AnimatorSprite_ProcessAnimation(&w->skiptxt);
        AnimatorSprite_ProcessAnimation(&w->skipbtn);
    }
    if (w->frame > 0x1e && w->skippable == 1 && w->skip == 0 && (gPad.trg & PAD_START)) {
        w->skip = 1;
        w->skip_t = 0x3c;
        AnimatorSprite_SetAnimation(&w->skiptxt, 1);
        AnimatorSprite_SetAnimation(&w->skipbtn, 0x25);
        cs_bgm_stop(0x12c);
    }
    if (w->fn)
        w->fn(w);
    if (!s_sr_on)
        return;
    w->pic_t++;
    for (i = 0; i < 3; i++) {
        w->px[i] += w->pvx[i];
        w->py[i] += w->pvy[i];
        w->pvx[i] += w->pax[i];
        w->pvy[i] += w->pay[i];
    }
    if (w->pic_fn)
        w->pic_fn(w);
    StaffRoll_UpdateChaseSpeed(w);
    if (w->mode != 2)
        for (i = 0; i < 3; i++) {
            w->ps[i].x = (s16)(w->px[i] >> 12);
            w->ps[i].y = (s16)(w->py[i] >> 12);
            AnimatorSprite_ProcessAnimation(&w->ps[i]);
        }
    if (w->skip != 0) {
        w->skip_t--;
        if (w->skip_t % 3 == 0) {
            g_cs_bright--;
            g_cs_bright_sub--;
        }
        if (g_cs_bright < -16)
            g_cs_bright = -16;
        if (g_cs_bright_sub < -16)
            g_cs_bright_sub = -16;
        if (w->skip_t <= 0 && g_cs_bright == -16 && g_cs_bright_sub == -16)
            StaffRoll_SetState(w, StaffRoll_State_Skip);
    }
    StaffRollCredit_Main(w);
}

/* the credits column: rows [scroll, scroll+192) of the three staff maps
 * streamed into 256x256 ring textures (StaffRoll_ScrollBackgrounds copies 25
 * map rows a frame; here only the new pixel rows are expanded) */
static void staff_draw(StaffRoll *p) {
    int k;
    for (k = 0; k < 3; k++) {
        EdLayer *l = &p->bg[4 + k];
        CsTex *t = &p->staff_tex[k];
        s32 y, top = p->scroll, s0, h0;
        if (!l->ok)
            continue;
        if (!t->argb) {
            cs_tex_init(t, 256, 256);
            layer_pal(l);
        }
        for (y = top; y < top + 192; y++) {
            if (y < p->staff_row[k] || y >= p->staff_row[k] + 192 || p->staff_row[k] < -256) {
                if (y >= 0 && y < l->pic.img.h && l->rowidx[y] >= 0 &&
                    (u32)l->rowidx[y] + l->pic.img.w <= l->rows_size)
                    cs_tex_put(t, 0, y & 255, 256, 1, l->rows + l->rowidx[y], l->pic.img.w, l->pal);
                else {
                    static const u8 zero[256];
                    cs_tex_put(t, 0, y & 255, 256, 1, zero, 256, l->pal);
                }
            }
        }
        p->staff_row[k] = top;   /* rows [top, top+192) present */
        s0 = top & 255;
        h0 = 256 - s0 < 192 ? 256 - s0 : 192;
        cs_quad_tex(t, 0, 0, 256, h0, 0, s0, 256, s0 + h0, 0xffffffff);
        if (h0 < 192)
            cs_quad_tex(t, 0, h0, 256, 192 - h0, 0, 0, 256, 192 - h0, 0xffffffff);
    }
}

static void StaffRoll_Draw(void) {
    StaffRoll *p = &s_sr;
    int i;
    g_cs_backdrop = 0xff000000;
    cs_view_full();
    /* main BG3 base, then BG1/BG2/BG0 credits column (or BG1 = the extra pictures) */
    if (p->main_layers & 8)
        layer_draw(&p->bg[0], 0, 0);
    if (p->main_bg1 >= 0 && p->main_bg1 < 15) {
        CsTex *t = layer_tex(&p->bg[p->main_bg1]);
        if (t && (p->main_layers & 2))
            cs_quad_tex(t, 0, 0, t->w, t->h, 0, 0, t->w, t->h, (u32)(p->alpha_bg1 * 255) << 24 | 0xffffff);
    } else if (p->main_layers & 7) {
        staff_draw(p);
    }
    /* main OBJ: credit logos (main y), running characters */
    for (i = 0; i < 3; i++)
        if (p->cred[i].on) {
            p->credit[i].bright = 255;
            AnimatorSprite_Draw(&p->credit[i]);
        }
    if (p->mode != 2)
        for (i = 0; i < 3; i++)
            AnimatorSprite_Draw(&p->ps[i]);
    /* the sub screen's closing card (SEGA logo, licence): sub (0,16)-(256,160) at
     * x1.25, under the final logo, over the characters */
    if (p->sub_card && (p->sub_layers & 2) && p->bg[2].ok) {
        CsTex *t = layer_tex(&p->bg[2]);
        cs_view_set(0, 16, 1.25f, 160, 80);
        g_cs_view.clip_y0 = 80;
        g_cs_view.clip_y1 = 80 + 144 * 1.25f;
        if (t)
            cs_quad_tex(t, 0, 0, t->w, t->h, 0, 0, t->w, t->h, 0xffffffff);
        if (p->cred[3].on) {
            CsSpr *s = &p->credit[3];
            s->x = (s16)p->cred[3].x;
            s->y = 0;   /* sub y = main y - 0x110 */
            s->bright = (u8)(p->sprite_fade * 255);
            AnimatorSprite_Draw(s);
        }
        cs_view_full();
    }
    if (s_banner_draw)
        for (i = 0; i < 18; i++)
            if (s_banner_draw == 2 || i * 3 + 5 <= p->t)
                AnimatorSprite_Draw(&p->banner[i]);
    /* skip button of the sub screen (ac_sf 0x24 at (120,161), text at (136,166)) */
    if (p->skippable == 1)
        ending_skip_button(&p->skipbtn, &p->skiptxt, p->skip != 0 || (p->frame & 0x30), 104, 150);
}

/* ================================================================ PostStaffRoll */
void PostStaffRoll_Start(void) {
    /* PostStaffRoll_Begin: Sonic without all seven Chaos Emeralds: TalkDemo 0x37 */
    s_psr_on = 1;
    s_psr_state = 0;
    s_psr_t = 0;
    g_cs_bright = 0;
    if (g_cs_story.character == 0 && (g_cs_story.sonic_emeralds & 0x7f) != 0x7f) {
        gTalkDemoId = 0x37;
        TalkDemo_Create();
        cs_log("poststaffroll talk 0x37");
        return;
    }
    s_psr_state = 2;
}
static void PostStaffRoll_Main(void) {
    switch (s_psr_state) {
    case 0:   /* PostStaffRoll_State_TalkDemo */
        if (!TalkDemo_IsActive()) {
            s_psr_t = 0;
            s_psr_state = 1;
        }
        break;
    case 1:   /* PostStaffRoll_State_Wait */
        if (++s_psr_t >= 0x5a)
            s_psr_state = 2;
        break;
    case 2:   /* PostStaffRoll_ShowUnlockNotices: the menus' unlock notices */
        s_psr_on = 0;
        cs_scene_change(CS_NEXT_UNLOCK_NOTICES);
        break;
    }
}

/* ================================================================ common */
void Ending_Kill(void) {
    int i;
    for (i = 0; i < 19; i++)
        layer_free(&s_ed.bg[i]);
    for (i = 0; i < 2; i++) {
        cs_res_free(s_ed.hdr[i]);
        s_ed.hdr[i] = NULL;
    }
    for (i = 0; i < 15; i++)
        layer_free(&s_sr.bg[i]);
    for (i = 0; i < 3; i++) {
        cs_res_free(s_sr.res[i]);
        s_sr.res[i] = NULL;
        cs_tex_free(&s_sr.staff_tex[i]);
    }
    s_ed_on = s_sr_on = s_psr_on = s_xe_on = 0;
}

void Ending_RunFrame(void) {
    if (s_xe_on)
        ExtraEnding_Main();
    if (s_ed_on)
        Ending_Main();
    if (s_sr_on)
        StaffRoll_Main();
    if (s_psr_on)
        PostStaffRoll_Main();
    if (!s_ed_on) {
        int i;
        for (i = 0; i < 19; i++)
            if (s_ed.bg[i].ok)
                layer_free(&s_ed.bg[i]);
    }
}

void Ending_Draw(void) {
    if (s_ed_on)
        Ending_DrawMain();
    if (s_sr_on)
        StaffRoll_Draw();
    if (s_psr_on)
        g_cs_backdrop = 0xff000000;
}

/* keep the extra-ending flag visible to cutscene_active */
int Ending_ExtraActive(void) { return s_xe_on; }

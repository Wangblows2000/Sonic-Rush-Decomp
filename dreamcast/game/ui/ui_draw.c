/*
 * UI rendering. KOS: atlas upload and the TR-list quads (draw_rect_screen in
 * engine/render.c). Host: stubs; ui_draw_tr only orders the quads so the test
 * can read them.
 *
 * Palette effects of the DS code become texture variants built on demand from
 * the atlas pixels kept in RAM (UiRes.pixels), in one 256x256 ARGB1555 cache
 * texture:
 *   UI_VAR_GAUGE0 + k : opaque white -> data_ov000_02364d60[k] (tension gauge
 *                       colours 1/2 of OBJ palette 1, Hud_DrawTensionGauge)
 *   UI_VAR_DIM        : every colour through the data_0207ba24 nibble LUT
 *                       (PauseMenu_HighlightSelection)
 * Master brightness (gDisplayMain/Sub) -> one full-screen black/white quad.
 */
#include <stdlib.h>
#include "ui_internal.h"

static const UiQuad *sorted[512];
static int n_sorted;

/* Painter's order. Within a layer the DS OAM rule applies: the sprite drawn
 * first (lowest OAM index) is in front, so textured quads go back to front in
 * reverse call order; flat quads stand for BG fills and go first. */
static int sort_quads(void) {
    const UiQuad *q;
    int n = ui_debug_quads(&q), layer, i;
    n_sorted = 0;
    for (layer = UI_LAYER_POPUP; layer <= UI_LAYER_FADE; layer++) {
        for (i = 0; i < n; i++)
            if (q[i].layer == layer && q[i].tex < 0 && n_sorted < 512)
                sorted[n_sorted++] = &q[i];
        for (i = n - 1; i >= 0; i--)
            if (q[i].layer == layer && q[i].tex >= 0 && n_sorted < 512)
                sorted[n_sorted++] = &q[i];
    }
    return n_sorted;
}

/* ARGB1555 <-> DS BGR555 */
static u16 bgr_to_argb(u16 c) {
    return (u16)(0x8000 | (c & 0x1f) << 10 | (c & 0x3e0) | (c >> 10 & 0x1f));
}
static u16 argb_to_bgr(u16 p) {
    return (u16)((p >> 10 & 0x1f) | (p & 0x3e0) | (p & 0x1f) << 10);
}
u16 ui_variant_pixel(u16 p, int variant) {
    if (!(p & 0x8000))
        return p;
    if (variant >= UI_VAR_GAUGE0 && variant < UI_VAR_GAUGE0 + 7) {
        if (p == 0xffff)
            return bgr_to_argb(g_ui_data.gauge_pal[variant - UI_VAR_GAUGE0]);
        return p;
    }
    if (variant == UI_VAR_DIM) {
        const u8 *lut = g_ui_data.pause_lut;
        u16 c = argb_to_bgr(p), o = 0;
        o |= lut[c & 0xf];
        o |= (u16)(lut[(c & 0xf0) >> 4] << 4);
        o |= (u16)(lut[(c & 0xf00) >> 8] << 8);
        o |= (u16)(lut[(c & 0xf000) >> 12] << 12);
        return bgr_to_argb(o & 0x7fff);
    }
    return p;
}

#ifdef _arch_dreamcast
#include <kos.h>
#include "../engine/render.h"

int ui_res_upload(UiRes *r) {
    int i, ok = 0;
    for (i = 0; i < r->n_pages && i < 4; i++) {
        Texture *t;
        u32 size = (u32)r->pages[i].w * r->pages[i].h * 2;
        if (!r->pixels[i])
            continue;
        t = (Texture *)calloc(1, sizeof(Texture));
        if (!t)
            continue;
        t->w = r->pages[i].w;
        t->h = r->pages[i].h;
        t->fmt = PVR_TXRFMT_ARGB1555;
        t->bytes = size;
        t->ptr = pvr_mem_malloc(size);
        if (!t->ptr) {
            free(t);
            continue;
        }
        pvr_txr_load(r->pixels[i], t->ptr, size);
        r->tex[i] = t;
        ok++;
    }
    return ok;
}

void ui_res_release(UiRes *r) {
    int i;
    for (i = 0; i < 4; i++)
        if (r->tex[i]) {
            tex_free((Texture *)r->tex[i]);
            free(r->tex[i]);
            r->tex[i] = NULL;
        }
}

/* ------------------------------------------------------ variant cache */
#define VC_SIZE 256
#define VC_MAX 64
static Texture s_vc_tex;
static struct { const UiRes *res; int image, variant; u16 x, y; } s_vc[VC_MAX];
static int s_vc_n, s_vc_sx, s_vc_sy, s_vc_rowh, s_vc_ok;

static int vc_find(const UiRes *r, int image, int variant, u16 *ox, u16 *oy) {
    const UiImage *im = &r->images[image];
    const u16 *src;
    int i, x, y, pw;
    for (i = 0; i < s_vc_n; i++)
        if (s_vc[i].res == r && s_vc[i].image == image && s_vc[i].variant == variant) {
            *ox = s_vc[i].x;
            *oy = s_vc[i].y;
            return 1;
        }
    if (!s_vc_ok) {
        s_vc_tex.w = s_vc_tex.h = VC_SIZE;
        s_vc_tex.fmt = PVR_TXRFMT_ARGB1555;
        s_vc_tex.bytes = VC_SIZE * VC_SIZE * 2;
        s_vc_tex.ptr = pvr_mem_malloc(s_vc_tex.bytes);
        if (!s_vc_tex.ptr)
            return 0;
        {
            u16 *z = (u16 *)s_vc_tex.ptr;
            for (i = 0; i < VC_SIZE * VC_SIZE; i++)
                z[i] = 0;
        }
        s_vc_ok = 1;
    }
    if (s_vc_n >= VC_MAX || !r->pixels[im->page] || im->w + 1 > VC_SIZE)
        return 0;
    if (s_vc_sx + im->w + 1 > VC_SIZE) {
        s_vc_sx = 0;
        s_vc_sy += s_vc_rowh + 1;
        s_vc_rowh = 0;
    }
    if (s_vc_sy + im->h > VC_SIZE)
        return 0;
    pw = r->pages[im->page].w;
    src = r->pixels[im->page];
    for (y = 0; y < im->h; y++) {
        volatile u16 *dst = (volatile u16 *)s_vc_tex.ptr + (s_vc_sy + y) * VC_SIZE + s_vc_sx;
        for (x = 0; x < im->w; x++)
            dst[x] = ui_variant_pixel(src[(im->y + y) * pw + im->x + x], variant);
    }
    s_vc[s_vc_n].res = r;
    s_vc[s_vc_n].image = image;
    s_vc[s_vc_n].variant = variant;
    s_vc[s_vc_n].x = (u16)s_vc_sx;
    s_vc[s_vc_n].y = (u16)s_vc_sy;
    *ox = (u16)s_vc_sx;
    *oy = (u16)s_vc_sy;
    s_vc_n++;
    s_vc_sx += im->w + 1;
    if (im->h > s_vc_rowh)
        s_vc_rowh = im->h;
    return 1;
}

void ui_draw_tr(void) {
    int i, n = sort_quads();
    float z = 100.0f;
    s16 b;
    for (i = 0; i < n; i++, z += 0.01f) {
        const UiQuad *q = sorted[i];
        const UiRes *r;
        const Texture *t;
        if (q->tex < 0) {
            draw_rect_screen(NULL, PVR_LIST_TR_POLY, q->x, q->y, q->w, q->h, 0, 0, 0, 0, z, q->argb);
            continue;
        }
        r = &g_ui_res[q->tex];
        if (q->variant != 0) {
            u16 vx, vy;
            const UiImage *im = &r->images[q->image];
            if (vc_find(r, q->image, q->variant, &vx, &vy)) {
                float du = (float)vx - im->x, dv = (float)vy - im->y;
                draw_rect_screen(&s_vc_tex, PVR_LIST_TR_POLY, q->x, q->y, q->w, q->h, q->u0 + du,
                                 q->v0 + dv, q->u1 + du, q->v1 + dv, z, q->argb);
                continue;
            }
        }
        t = (const Texture *)r->tex[r->images[q->image].page];
        if (!t)
            continue;
        draw_rect_screen(t, PVR_LIST_TR_POLY, q->x, q->y, q->w, q->h, q->u0, q->v0, q->u1, q->v1, z, q->argb);
    }
    /* master brightness: the stronger of the two DS screens */
    b = g_ui_bright_main;
    if ((g_ui_bright_sub < 0 ? -g_ui_bright_sub : g_ui_bright_sub) > (b < 0 ? -b : b))
        b = g_ui_bright_sub;
    if (b != 0) {
        u32 a = (u32)((b < 0 ? -b : b) * 255 / 16);
        if (a > 255)
            a = 255;
        draw_rect_screen(NULL, PVR_LIST_TR_POLY, 0, 0, 640, 480, 0, 0, 0, 0, 200.0f,
                         a << 24 | (b > 0 ? 0xffffff : 0));
    }
}
#else
/* host build: no PVR */
int ui_res_upload(UiRes *r) {
    (void)r;
    return 0;
}
void ui_res_release(UiRes *r) { (void)r; }
void ui_draw_tr(void) { sort_quads(); }
int ui_debug_sorted(const UiQuad ***out) {
    *out = sorted;
    return n_sorted;
}
#endif

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
 *   UI_VAR_MAP0 + n   : colour map of an OBJ palette-row swap (ui_palmap_set; NaviB rows 7/8)
 *
 * One composition: the frame is the DS main screen, so gDisplayMain's master
 * brightness is the full-screen black/white overlay. Quads composed in from the sub
 * screen (UiQuad.screen = 1: title card zone art, navigator, boss panel) take
 * gDisplaySub's brightness on their own vertex colour (black: colour scaled; white:
 * the quad fades out over what is behind it). There is no second display path.
 *
 * The navigator face (UI_TEX_FACE) is one streamed texture: the shown face frame is
 * expanded from navifaces_<c>.bin when it changes (the DS streams each frame's tiles
 * from the card the same way, NaviMsg_ReadFaceFrameChr).
 */
#include <stdlib.h>
#include "ui_internal.h"

static const UiQuad *sorted[512];
static int n_sorted;

/* gDisplaySub master brightness on a sub-screen quad's vertex colour */
static u32 sub_bright(u32 argb) {
    s32 b = g_ui_bright_sub, a = (s32)(argb >> 24), r = (s32)(argb >> 16 & 0xff), g = (s32)(argb >> 8 & 0xff),
        c = (s32)(argb & 0xff);
    if (b < 0) {
        if (b < -16)
            b = -16;
        r = r * (16 + b) / 16;
        g = g * (16 + b) / 16;
        c = c * (16 + b) / 16;
    } else if (b > 0) {
        if (b > 16)
            b = 16;
        a = a * (16 - b) / 16;
    }
    return (u32)a << 24 | (u32)r << 16 | (u32)g << 8 | (u32)c;
}

/* palette-row swaps as colour maps */
static struct { int n; u16 from[64], to[64]; int res, src, dst; } s_palmap[4];
int ui_debug_palmap(int n, int *res, int *src, int *dst) {
    if (n < 0 || n >= 4 || !s_palmap[n].n)
        return 0;
    *res = s_palmap[n].res;
    *src = s_palmap[n].src;
    *dst = s_palmap[n].dst;
    return 1;
}
void ui_palmap_set(int n, int res, int src_image, int dst_image) {
    const UiRes *r = &g_ui_res[res];
    const UiImage *a, *b;
    int x, y, k;
    if (n < 0 || n >= 4)
        return;
    s_palmap[n].n = 0;
    s_palmap[n].res = res;
    s_palmap[n].src = src_image;
    s_palmap[n].dst = dst_image;
    if (!r->blob || src_image >= r->n_images || dst_image >= r->n_images)
        return;
    a = &r->images[src_image];
    b = &r->images[dst_image];
    if (!r->pixels[a->page] || !r->pixels[b->page])
        return;
    for (y = 0; y < a->h && y < b->h; y++)
        for (x = 0; x < a->w && x < b->w; x++) {
            u16 f = r->pixels[a->page][(a->y + y) * r->pages[a->page].w + a->x + x];
            u16 t = r->pixels[b->page][(b->y + y) * r->pages[b->page].w + b->x + x];
            if (!(f & 0x8000))
                continue;
            for (k = 0; k < s_palmap[n].n; k++)
                if (s_palmap[n].from[k] == f)
                    break;
            if (k == s_palmap[n].n && k < 64) {
                s_palmap[n].from[k] = f;
                s_palmap[n].to[k] = t;
                s_palmap[n].n++;
            }
        }
}

/* Painter's order. Within a layer the DS OAM rule applies: the sprite drawn
 * first (lowest OAM index) is in front, so textured quads go back to front in
 * reverse call order; flat quads stand for BG fills and go first. */
static int sort_quads(void) {
    const UiQuad *q;
    int n = ui_debug_quads(&q), layer, i;
    n_sorted = 0;
    if (g_ui_bright_sub != 0)
        for (i = 0; i < n; i++)
            if (q[i].screen)
                ((UiQuad *)q)[i].argb = sub_bright(q[i].argb);
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
    if (variant >= UI_VAR_MAP0 && variant < UI_VAR_MAP0 + 4) {
        int k, n = variant - UI_VAR_MAP0;
        for (k = 0; k < s_palmap[n].n; k++)
            if (s_palmap[n].from[k] == p)
                return s_palmap[n].to[k];
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

/* ------------------------------------------------------ streamed face */
int ui_navi_face_expand(int f, u16 *dst, int stride, int *w, int *h);   /* ui_navi.c */
static Texture s_face_tex;
static int s_face_cur = -1;
static u16 s_face_buf[128 * 128];
static const Texture *face_tex(int f) {
    int w, h;
    if (f == s_face_cur && s_face_tex.ptr)
        return &s_face_tex;
    if (!s_face_tex.ptr) {
        s_face_tex.w = s_face_tex.h = 128;
        s_face_tex.fmt = PVR_TXRFMT_ARGB1555;
        s_face_tex.bytes = 128 * 128 * 2;
        s_face_tex.ptr = pvr_mem_malloc(s_face_tex.bytes);
        if (!s_face_tex.ptr)
            return NULL;
    }
    memset(s_face_buf, 0, sizeof(s_face_buf));
    if (!ui_navi_face_expand(f, s_face_buf, 128, &w, &h))
        return NULL;
    pvr_txr_load(s_face_buf, s_face_tex.ptr, s_face_tex.bytes);
    s_face_cur = f;
    return &s_face_tex;
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
        if (q->tex == UI_TEX_FACE) {
            t = face_tex(q->image);
            if (t)
                draw_rect_screen(t, PVR_LIST_TR_POLY, q->x, q->y, q->w, q->h, q->u0, q->v0, q->u1, q->v1, z, q->argb);
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
    /* master brightness of the main screen (the frame); sub-screen quads took
     * gDisplaySub's in sort_quads */
    b = g_ui_bright_main;
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

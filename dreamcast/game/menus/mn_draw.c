/*
 * Front-end drawing. Portable part: projection of a DS-frame quad through a
 * composition region (MENUS.md section 3) onto the single 640x480 frame,
 * clipped to the region's source rect. KOS part: textures (sprite atlas
 * pages padded to a power of two, BG images re-expanded when their palette
 * changes) and the TR-list quads via engine/render.c draw_rect_screen, then
 * one brightness overlay over the whole frame.
 */
#include <assert.h>
#include <stdlib.h>
#include "mn_internal.h"

int mn_project(const MnQuad *q, const MnRegion *r, MnOut *o) {
    float x0 = q->x, y0 = q->y, x1 = q->x + q->w, y1 = q->y + q->h;
    float du = (q->u1 - q->u0) / (q->w != 0 ? q->w : 1), dv = (q->v1 - q->v0) / (q->h != 0 ? q->h : 1);
    float u0 = q->u0, v0 = q->v0, u1 = q->u1, v1 = q->v1;
    if (q->w <= 0 || q->h <= 0)
        return 0;
    if (x0 < r->sx0) {
        u0 += (r->sx0 - x0) * du;
        x0 = r->sx0;
    }
    if (y0 < r->sy0) {
        v0 += (r->sy0 - y0) * dv;
        y0 = r->sy0;
    }
    if (x1 > r->sx1) {
        u1 -= (x1 - r->sx1) * du;
        x1 = r->sx1;
    }
    if (y1 > r->sy1) {
        v1 -= (y1 - r->sy1) * dv;
        y1 = r->sy1;
    }
    if (x1 <= x0 || y1 <= y0)
        return 0;
    o->x = r->dx + (x0 - r->sx0) * r->scale;
    o->y = r->dy + (y0 - r->sy0) * r->scale;
    o->w = (x1 - x0) * r->scale;
    o->h = (y1 - y0) * r->scale;
    o->u0 = u0;
    o->v0 = v0;
    o->u1 = u1;
    o->v1 = v1;
    return 1;
}

static int is_pow2_pvr(int v) { return v >= 8 && v <= 1024 && (v & (v - 1)) == 0; }

#ifdef _arch_dreamcast
#include <kos.h>
#include "../engine/render.h"

/* PVR textures must be power-of-two in both directions (8..1024) */
static Texture *tex_new(int w, int h) {
    Texture *t;
    if (!is_pow2_pvr(w) || !is_pow2_pvr(h))
        return NULL;
    t = (Texture *)calloc(1, sizeof(Texture));
    if (!t)
        return NULL;
    t->w = w;
    t->h = h;
    t->fmt = PVR_TXRFMT_ARGB1555;
    t->bytes = (uint32_t)w * h * 2;
    t->ptr = pvr_mem_malloc(t->bytes);
    if (!t->ptr) {
        free(t);
        return NULL;
    }
    return t;
}

void mn_res_release_tex(MnRes *r) {
    int i;
    for (i = 0; i < 4; i++)
        if (r->tex[i]) {
            tex_free((Texture *)r->tex[i]);
            free(r->tex[i]);
            r->tex[i] = NULL;
        }
}
void mn_bg_release_tex(MnBgImg *b) {
    if (b->tex) {
        tex_free((Texture *)b->tex);
        free(b->tex);
        b->tex = NULL;
    }
}

/* atlas page: rows copied into a padded power-of-two texture. u/v stay in
 * texels of the page: draw_rect_screen divides them by the padded size. */
static const Texture *res_tex(MnRes *r, int page) {
    if (page >= r->n_pages || page >= 4)
        return NULL;
    if (!r->tex[page] && r->pixels[page]) {
        int w = r->pages[page].w, h = r->pages[page].h, y;
        Texture *t = (w <= 1024 && h <= 1024) ? tex_new(mn_tex_pow2(w), mn_tex_pow2(h)) : NULL;
        if (t) {
            u16 *buf = (u16 *)memalign(32, t->bytes);
            if (buf) {
                memset(buf, 0, t->bytes);
                for (y = 0; y < h; y++)
                    memcpy(buf + (size_t)y * t->w, r->pixels[page] + (size_t)y * w, (size_t)w * 2);
                pvr_txr_load(buf, t->ptr, t->bytes);
                free(buf);
                r->tex[page] = t;
            } else {
                tex_free(t);
                free(t);
            }
        }
    }
    return (const Texture *)r->tex[page];
}

/* BG image: argb is already laid out with the power-of-two stride tw x th */
static const Texture *bg_tex(MnBgImg *b) {
    if (!b->tex)
        b->tex = tex_new(b->tw, b->th);
    if (b->tex && b->dirty) {
        mn_bg_expand(b);
        pvr_txr_load(b->argb, ((Texture *)b->tex)->ptr, ((Texture *)b->tex)->bytes);
        b->dirty = 0;
    }
    return (const Texture *)b->tex;
}

void menus_draw_tr(void) {
    const MnDraw *s;
    int n = mn_sorted(&s), i;
    float z = 10.0f;
    MnOut o;
    s16 b = g_mn_bright[g_mn_comp.base];
    if (g_mn_comp.fill)
        draw_rect_screen(NULL, PVR_LIST_TR_POLY, 0, 0, 640, 480, 0, 0, 0, 0, z, g_mn_comp.fill);
    for (i = 0; i < n; i++, z += 0.005f) {
        const MnQuad *q = s[i].q;
        const Texture *t = NULL;
        if (!mn_project(q, s[i].r, &o))
            continue;
        if (q->kind == MNQ_SPRITE)
            t = res_tex((MnRes *)q->res, q->page);
        else if (q->kind == MNQ_BG)
            t = bg_tex((MnBgImg *)q->bg);
        if (q->kind != MNQ_FLAT && !t)
            continue;
        draw_rect_screen(t, PVR_LIST_TR_POLY, o.x, o.y, o.w, o.h, o.u0, o.v0, o.u1, o.v1, z, q->argb);
    }
    /* master brightness: one pass over the whole frame */
    if (b != 0) {
        uint32_t a = (uint32_t)((b < 0 ? -b : b) * 255 / 16);
        if (a > 255)
            a = 255;
        draw_rect_screen(NULL, PVR_LIST_TR_POLY, 0, 0, 640, 480, 0, 0, 0, 0, 90.0f, a << 24 | (b > 0 ? 0xffffff : 0));
    }
}
#else
void mn_res_release_tex(MnRes *r) { (void)r; }
void mn_bg_release_tex(MnBgImg *b) { (void)b; }
/* host: the PVR texture rule (the size tex_new gets on KOS) is asserted for
 * every texture the frame uses */
void menus_draw_tr(void) {
    const MnDraw *s;
    int n = mn_sorted(&s), i, w = 0, h = 0, tw, th;
    for (i = 0; i < n; i++) {
        const MnQuad *q = s[i].q;
        if (q->kind == MNQ_SPRITE && q->res && q->page < q->res->n_pages) {
            w = q->res->pages[q->page].w;
            h = q->res->pages[q->page].h;
            tw = mn_tex_pow2(w);
            th = mn_tex_pow2(h);
        } else if (q->kind == MNQ_BG) {
            w = q->bg->w;
            h = q->bg->h;
            tw = q->bg->tw;
            th = q->bg->th;
        } else {
            continue;
        }
        assert(is_pow2_pvr(tw) && is_pow2_pvr(th));
        assert(w <= tw && h <= th);
    }
}
#endif

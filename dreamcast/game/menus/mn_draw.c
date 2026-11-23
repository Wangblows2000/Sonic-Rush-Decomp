/*
 * Front-end drawing. Portable part: projection of a DS-frame quad through a
 * composition region (MENUS.md section 3) onto the single 640x480 frame,
 * clipped to the region's source rect. KOS part: textures (sprite atlas
 * pages: PAL4 + a palette row per palette, or ARGB1555; BG images re-expanded
 * when their palette changes) and the TR-list quads via engine/render.c draw_rect_screen, then
 * one brightness overlay over the whole frame.
 */
#include <assert.h>
#include <stdlib.h>
#include "mn_internal.h"

int mn_project(const MnQuad *q, const MnRegion *r, MnOut *o) {
    float x0 = q->x, y0 = q->y, x1 = q->x + q->w, y1 = q->y + q->h;
    float du = (q->u1 - q->u0) / (q->w != 0 ? q->w : 1), dv = (q->v1 - q->v0) / (q->h != 0 ? q->h : 1);
    float u0 = q->u0, v0 = q->v0, u1 = q->u1, v1 = q->v1;
    if (q->w <= 0 || q->h <= 0 || q->kind == MNQ_AFFINE || q->kind == MNQ_3D)
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
    if (r->atlas) {
        spr_atlas_free((SprAtlas *)r->atlas);
        free(r->atlas);
        r->atlas = NULL;
    }
}
void mn_bg_release_tex(MnBgImg *b) {
    if (b->tex) {
        tex_free((Texture *)b->tex);
        free(b->tex);
        b->tex = NULL;
    }
}

/* atlas pages: PAL4 (palette rows taken on first draw) or ARGB1555, uploaded on
 * first use from the RAM copies (the converter makes power-of-two pages) */
static const Texture *res_tex(MnRes *r, int page, int image, int *bank) {
    SprAtlas *a;
    *bank = -1;
    if (page >= r->n_pages || page >= 4)
        return NULL;
    if (!r->atlas) {
        int i;
        for (i = 0; i < r->n_pages && i < 4; i++)
            if (!is_pow2_pvr(r->pages[i].w) || !is_pow2_pvr(r->pages[i].h))
                return NULL;
        r->atlas = calloc(1, sizeof(SprAtlas));
        if (!r->atlas)
            return NULL;
        spr_atlas_adopt((SprAtlas *)r->atlas, r->blob, r->pal.blob ? &r->pal : NULL, r->texels, r->pixels,
                        SPR_ATLAS_LAZY);
    }
    a = (SprAtlas *)r->atlas;
    if (page >= a->n_pages)
        return NULL;
    *bank = spr_atlas_bank(a, image, -1);
    return a->tex[page].ptr ? &a->tex[page] : NULL;
}

/* BG image: argb is already laid out with the power-of-two stride tw x th */
static const Texture *bg_tex(MnBgImg *b) {
    if (!b->tex)
    {
        if (b->tw < 8 || b->th < 8) {   /* not loaded (missing file): nothing to draw */
            static int logged;
            if (logged++ < 4)
                printf("menus: BG '%s' has no image (%dx%d)\n", b->name, b->tw, b->th);
            return NULL;
        }
        b->tex = tex_new(b->tw, b->th);
    }
    if (b->tex && b->dirty) {
        mn_bg_expand(b);
        pvr_txr_load(b->argb, ((Texture *)b->tex)->ptr, ((Texture *)b->tex)->bytes);
        b->dirty = 0;
    }
    return (const Texture *)b->tex;
}

/* an affine BG quad (mn_emit_bg_affine) through its region: the image rotated and scaled, cut to the
 * region's source rectangle (mn_affine_clip: the DS screen's edge), one strip */
static void draw_affine(const MnQuad *q, const MnRegion *r, float z) {
    const Texture *t = bg_tex((MnBgImg *)q->bg);
    float p[8][4];
    pvr_poly_cxt_t cxt;
    pvr_poly_hdr_t hdr;
    pvr_vertex_t v;
    int n, k, lo, hi;
    if (!t || (n = mn_affine_clip(q, r, p)) < 3)
        return;
    pvr_poly_cxt_txr(&cxt, PVR_LIST_TR_POLY, tex_txr_format(t, -1), t->w, t->h, t->ptr, PVR_FILTER_BILINEAR);
    cxt.gen.culling = PVR_CULLING_NONE;
    pvr_poly_compile(&hdr, &cxt);
    pvr_prim(&hdr, sizeof(hdr));
    v.argb = q->argb;
    v.oargb = 0;
    v.z = z;
    /* the convex polygon as a strip: 0, 1, n-1, 2, n-2, ... */
    lo = 1;
    hi = n - 1;
    for (k = 0; k < n; k++) {
        int c = k == 0 ? 0 : (k & 1) ? lo++ : hi--;
        v.flags = k == n - 1 ? PVR_CMD_VERTEX_EOL : PVR_CMD_VERTEX;
        v.x = r->dx + (p[c][0] - r->sx0) * r->scale;
        v.y = r->dy + (p[c][1] - r->sy0) * r->scale;
        v.u = p[c][2] / t->w;
        v.v = p[c][3] / t->h;
        pvr_prim(&v, sizeof(v));
    }
}

static void bright_rect(s16 b, float x, float y, float w, float h, float z) {
    uint32_t a;
    if (b == 0)
        return;
    a = (uint32_t)((b < 0 ? -b : b) * 255 / 16);
    if (a > 255)
        a = 255;
    draw_rect_screen(NULL, PVR_LIST_TR_POLY, x, y, w, h, 0, 0, 0, 0, z, a << 24 | (b > 0 ? 0xffffff : 0));
}
static void bright_overlay(s16 b, float z) { bright_rect(b, 0, 0, 640, 480, z); }

void menus_draw_tr(void) {
    const MnDraw *s;
    int n = mn_sorted(&s), i, main_done = 0;
    float z = 10.0f;
    MnOut o;
    s16 b = g_mn_bright[g_mn_comp.base];
    if (g_mn_comp.fill)
        draw_rect_screen(NULL, PVR_LIST_TR_POLY, 0, 0, 640, 480, 0, 0, 0, 0, z, g_mn_comp.fill);
    for (i = 0; i < n; i++, z += 0.005f) {
        const MnQuad *q = s[i].q;
        const Texture *t = NULL;
        int bank = -1;
        if (g_mn_comp.split_bright && !main_done && s[i].r->engine != 0) {
            /* the main screen's brightness over its regions only (the 3D layer included) */
            z += 0.005f;
            bright_overlay(g_mn_bright[0], z);
            z += 0.005f;
            main_done = 1;
        }
        if (q->kind == MNQ_3D) {   /* the 3D layer's depth band (mn_3d_band, menus_draw_list) */
            z += MN_3D_GAP;
            continue;
        }
        if (q->kind == MNQ_AFFINE) {
            draw_affine(q, s[i].r, z);
            continue;
        }
        if (!mn_project(q, s[i].r, &o))
            continue;
        if (q->kind == MNQ_SPRITE)
            t = res_tex((MnRes *)q->res, q->page, q->image, &bank);
        else if (q->kind == MNQ_BG)
            t = bg_tex((MnBgImg *)q->bg);
        if (q->kind != MNQ_FLAT && !t)
            continue;
        draw_rect_screen_pal(t, bank, PVR_LIST_TR_POLY, o.x, o.y, o.w, o.h, o.u0, o.v0, o.u1, o.v1, z, q->argb);
    }
    /* master brightness: one pass over the whole frame (split: the sub screen's over everything, the
     * main screen's after its regions above) */
    if (g_mn_comp.nbr > 0) {   /* Battle Play: each player's half with that player's brightness */
        for (i = 0; i < g_mn_comp.nbr && i < 2; i++)
            bright_rect(g_mn_bright[g_mn_comp.br[i].engine & 3], g_mn_comp.br[i].x, g_mn_comp.br[i].y,
                        g_mn_comp.br[i].w, g_mn_comp.br[i].h, 90.0f);
    } else if (g_mn_comp.split_bright) {
        if (!main_done)
            bright_overlay(g_mn_bright[0], 89.0f);
        bright_overlay(g_mn_bright[1], 90.0f);
    } else {
        bright_overlay(b, 90.0f);
    }
    mn_flush_freed();   /* the frame's quads are submitted */
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
    mn_flush_freed();
}
#endif

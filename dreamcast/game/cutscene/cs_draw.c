/*
 * Cutscene drawing. KOS: 3D instances in the OP / PT / TR lists
 * (dreamcast/game/model dcm_draw), the 2D quads of the frame in the TR list
 * (z 100..120, in their DS compositing order), then the master-brightness
 * overlay. Textures are uploaded when their rows change (power-of-two sizes,
 * non-twiddled ARGB1555). Host: the textures are checked against the PVR
 * size rule; the host test rasterises the quad list itself (test/cs_raster.h).
 */
#include <stdlib.h>
#include "cs_internal.h"

#ifdef _arch_dreamcast
#include <kos.h>
#include <malloc.h>
#include "../engine/render.h"

void cs_tex_release_pvr(CsTex *t) {
    if (t->pvr)
        pvr_mem_free((pvr_ptr_t)t->pvr);
    t->pvr = NULL;
}

static int tex_ready(CsTex *t) {
    if (!t || !t->argb)
        return 0;
    if (!t->pvr) {
        t->pvr = pvr_mem_malloc((size_t)t->tw * t->th * 2);
        if (!t->pvr)
            return 0;
        t->dirty = 1;
        t->dy0 = 0;
        t->dy1 = t->th;
    }
    if (t->dirty) {
        int y0 = t->dy0 < 0 ? 0 : t->dy0, y1 = t->dy1 > t->th ? t->th : t->dy1;
        if (y1 > y0)
            pvr_txr_load(t->argb + (size_t)y0 * t->tw, (uint8_t *)t->pvr + (size_t)y0 * t->tw * 2,
                         (size_t)(y1 - y0) * t->tw * 2);
        t->dirty = 0;
        t->dy0 = t->dy1 = 0;
    }
    return 1;
}

static void submit_quad(const CsQuad *q, pvr_list_t list, float z) {
    pvr_poly_cxt_t cxt;
    pvr_poly_hdr_t hdr;
    pvr_vertex_t v;
    static const int order[4] = {2, 0, 3, 1};   /* BL, TL, BR, TR strip */
    int k;
    float iw = 1, ih = 1;
    if (q->kind == CSQ_SPR && q->res && q->res->atlas) {
        /* sprite atlas page: PAL4 with the image's palette row, or ARGB1555 */
        SprAtlas *a = (SprAtlas *)q->res->atlas;
        int bank = spr_atlas_bank(a, q->image, -1);
        const Texture *t = q->page < a->n_pages ? &a->tex[q->page] : NULL;
        if (!t || !t->ptr)
            return;
        pvr_poly_cxt_txr(&cxt, list, tex_txr_format(t, bank), t->w, t->h, t->ptr, PVR_FILTER_NONE);
        iw = 1.0f / t->w;
        ih = 1.0f / t->h;
    } else if (q->kind != CSQ_FLAT && tex_ready(q->tex)) {
        pvr_poly_cxt_txr(&cxt, list, PVR_TXRFMT_ARGB1555 | PVR_TXRFMT_NONTWIDDLED, q->tex->tw, q->tex->th,
                         (pvr_ptr_t)q->tex->pvr, PVR_FILTER_NONE);
        iw = 1.0f / q->tex->tw;
        ih = 1.0f / q->tex->th;
    } else if (q->kind == CSQ_FLAT) {
        pvr_poly_cxt_col(&cxt, list);
    } else {
        return;
    }
    cxt.gen.culling = PVR_CULLING_NONE;
    pvr_poly_compile(&hdr, &cxt);
    pvr_prim(&hdr, sizeof(hdr));
    v.oargb = 0;
    v.argb = q->argb;
    v.z = z;
    for (k = 0; k < 4; k++) {
        int i = order[k];
        v.flags = k == 3 ? PVR_CMD_VERTEX_EOL : PVR_CMD_VERTEX;
        v.x = q->x[i];
        v.y = q->y[i];
        v.u = q->u[i] * iw;
        v.v = q->v[i] * ih;
        pvr_prim(&v, sizeof(v));
    }
}

static void flat(pvr_list_t list, float z, uint32_t argb) {
    CsQuad q;
    memset(&q, 0, sizeof(q));
    q.kind = CSQ_FLAT;
    q.x[0] = 0; q.y[0] = 0; q.x[1] = 640; q.y[1] = 0; q.x[2] = 0; q.y[2] = 480; q.x[3] = 640; q.y[3] = 480;
    q.argb = argb;
    submit_quad(&q, list, z);
}

static void draw_2d(void) {
    const CsQuad *q = cs_quads();
    int n = cs_quad_count(), i;
    float z = 100.0f;
    for (i = 0; i < n; i++, z += 0.01f)
        submit_quad(&q[i], PVR_LIST_TR_POLY, z);
    if (g_cs_bright != 0) {
        int b = g_cs_bright < 0 ? -g_cs_bright : g_cs_bright;
        uint32_t a = (uint32_t)(b >= 16 ? 255 : b * 255 / 16);
        flat(PVR_LIST_TR_POLY, 120.0f, a << 24 | (g_cs_bright > 0 ? 0xffffff : 0));
    }
}

void cutscene_draw_list(int list) {
    if (list == PVR_LIST_OP_POLY && g_cs_backdrop && cutscene_is_fullscreen())
        flat(PVR_LIST_OP_POLY, 0.00012f, g_cs_backdrop);   /* above the PVR background plane (0.0001) */
    cs3d_draw_list(list);
    if (list == PVR_LIST_TR_POLY)
        draw_2d();
}

void cutscene_draw(void) {
    pvr_list_begin(PVR_LIST_OP_POLY);
    cutscene_draw_list(PVR_LIST_OP_POLY);
    pvr_list_finish();
    pvr_list_begin(PVR_LIST_PT_POLY);
    cutscene_draw_list(PVR_LIST_PT_POLY);
    pvr_list_finish();
    pvr_list_begin(PVR_LIST_TR_POLY);
    cutscene_draw_list(PVR_LIST_TR_POLY);
    pvr_list_finish();
}
#else
#include <assert.h>
void cs_tex_release_pvr(CsTex *t) { t->pvr = NULL; }
static int pow2ok(int v) { return v >= 8 && v <= 1024 && (v & (v - 1)) == 0; }
void cutscene_draw_list(int list) {
    const CsQuad *q = cs_quads();
    int n = cs_quad_count(), i;
    if (list == 0)
        cs3d_draw_list(0);   /* the 3D submission path without a PVR (material animations, clipping) */
    for (i = 0; i < n; i++)
        if (q[i].kind != CSQ_FLAT && q[i].tex && q[i].tex->argb) {
            assert(pow2ok(q[i].tex->tw) && pow2ok(q[i].tex->th));
            assert(q[i].tex->w <= q[i].tex->tw && q[i].tex->h <= q[i].tex->th);
        }
}
void cutscene_draw(void) { cutscene_draw_list(0); }
#endif

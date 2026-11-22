/*
 * Object rendering on the Dreamcast (KOS only): every object sprite is one
 * textured quad cut from the converted BAC atlas (atlas0.argb1555, ARGB1555,
 * non-twiddled; tools/dreamcast/sprites/bac2dc.py).  The animation state was
 * advanced in objects_update (StageObj_Draw2DEx animation part); this file
 * only places the current frame, like AnimatorSprite_Draw:
 *   unflipped  image top-left = position - origin
 *   H-flipped  mirrored about the origin (x = position - (w - origin_x))
 * Layering: OAM priority (sprite flags bits 17-18) against the BG layers
 * (front layer z 4.0 = BG priority 0, back layer z 2.0 = priority 2), then
 * OAM order (lower order in front).
 */
#include <kos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../engine/render.h"
#include "obj_internal.h"

int obj_draw_upload(ObjRes *r, const char *dir) {
    char path[160];
    int i, ok = 0;
    for (i = 0; i < r->n_pages && i < 4; i++) {
        Texture *t;
        u8 *b;
        u32 size;
        snprintf(path, sizeof(path), "%s/atlas%d.argb1555", dir, i);
        if (obj_load_file(path, &b, &size) != 0)
            continue;
        if (size != (u32)r->pages[i].w * r->pages[i].h * 2) {
            free(b);
            continue;
        }
        t = (Texture *)calloc(1, sizeof(Texture));
        if (!t) {
            free(b);
            continue;
        }
        t->w = r->pages[i].w;
        t->h = r->pages[i].h;
        t->fmt = PVR_TXRFMT_ARGB1555;
        t->bytes = size;
        t->ptr = pvr_mem_malloc(size);
        if (!t->ptr) {
            free(t);
            free(b);
            continue;
        }
        pvr_txr_load(b, t->ptr, size);
        free(b);
        r->tex[i] = t;
        ok++;
    }
    return ok;
}

void obj_draw_release(ObjRes *r) {
    int i;
    for (i = 0; i < 4; i++)
        if (r->tex[i]) {
            tex_free((Texture *)r->tex[i]);
            free(r->tex[i]);
            r->tex[i] = NULL;
        }
}

static const float k_zbase[4] = { 4.5f, 3.5f, 2.5f, 1.5f };

/* a textured quad with free corners (rot-scale OAM), screen coordinates */
static void quad4(const Texture *t, pvr_list_t list, const float *xs, const float *ys, float u0, float v0, float u1,
                  float v1, float z) {
    pvr_poly_cxt_t cxt;
    pvr_poly_hdr_t hdr;
    pvr_vertex_t v;
    pvr_poly_cxt_txr(&cxt, list, t->fmt | PVR_TXRFMT_NONTWIDDLED, t->w, t->h, t->ptr, PVR_FILTER_NONE);
    pvr_poly_compile(&hdr, &cxt);
    pvr_prim(&hdr, sizeof(hdr));
    u0 /= t->w; u1 /= t->w; v0 /= t->h; v1 /= t->h;
    v.flags = PVR_CMD_VERTEX; v.argb = 0xffffffff; v.oargb = 0; v.z = z;
    v.x = xs[3]; v.y = ys[3]; v.u = u0; v.v = v1; pvr_prim(&v, sizeof(v)); /* bottom left  */
    v.x = xs[0]; v.y = ys[0]; v.u = u0; v.v = v0; pvr_prim(&v, sizeof(v)); /* top left     */
    v.x = xs[2]; v.y = ys[2]; v.u = u1; v.v = v1; pvr_prim(&v, sizeof(v)); /* bottom right */
    v.flags = PVR_CMD_VERTEX_EOL;
    v.x = xs[1]; v.y = ys[1]; v.u = u1; v.v = v0; pvr_prim(&v, sizeof(v)); /* top right    */
}

static void draw_spr_at(const ObjSprite *s, pvr_list_t list, s32 px, s32 py, int vflip_extra) {
    const DsprImage *img = spr_image(s);
    const Texture *t;
    int hf, vf, prio, order;
    float x, y, x0, y0, z, u0, v0, u1, v1, tmp;
    if (img == NULL || s->hidden)
        return;
    t = (const Texture *)s->res->tex[img->page];
    if (t == NULL)
        return;
    hf = (s->flags & SPR_HFLIP) != 0;
    vf = ((s->flags & SPR_VFLIP) != 0) ^ vflip_extra;
    x = (float)px;
    y = (float)py;
    if (s->screen) {
        x += g_cam_x;
        y += g_cam_y;
    }
    x0 = hf ? x - (img->w - img->origin_x) : x - img->origin_x;
    y0 = vf ? y - (img->h - img->origin_y) : y - img->origin_y;
    prio = (int)((s->flags >> 17) & 3);
    order = s->order > 0x3f ? 0x3f : s->order;
    z = k_zbase[prio] + (0x40 - order) * 0.01f;
    if (s->rotscale) {
        /* AnimatorSprite_DrawRotScale: rotate / scale about the sprite position */
        float a = (float)s->rot * (6.2831853f / 256.0f), c = fcos(a), sn = fsin(a);
        float sx = (float)s->sx / 4096.0f, sy = (float)s->sy / 4096.0f, xs[4], ys[4], cx[4], cy[4];
        int i;
        cx[0] = x0 - x; cy[0] = y0 - y;
        cx[1] = x0 + img->w - x; cy[1] = y0 - y;
        cx[2] = x0 + img->w - x; cy[2] = y0 + img->h - y;
        cx[3] = x0 - x; cy[3] = y0 + img->h - y;
        for (i = 0; i < 4; i++) {
            float rx = cx[i] * sx, ry = cy[i] * sy;
            xs[i] = (x + rx * c - ry * sn - g_cam_x) * VIEW_SCALE;
            ys[i] = (y + rx * sn + ry * c - g_cam_y) * VIEW_SCALE;
        }
        u0 = (float)img->x; v0 = (float)img->y; u1 = (float)(img->x + img->w); v1 = (float)(img->y + img->h);
        if (hf) { tmp = u0; u0 = u1; u1 = tmp; }
        if (vf) { tmp = v0; v0 = v1; v1 = tmp; }
        quad4(t, list, xs, ys, u0, v0, u1, v1, z);
        return;
    }
    if (s->ppos != NULL) { /* StageObj_DrawSpriteAlt: piece 0 offset (one-piece frames) */
        x0 += (float)(s->ppos[0][0] >> 8);
        y0 += (float)(s->ppos[0][1] >> 8);
    }
    draw_sprite_world(t, list, x0, y0, (float)img->w, (float)img->h, (float)img->x, (float)img->y,
                      (float)(img->x + img->w), (float)(img->y + img->h), hf | (vf << 1), z, 0xffffffff);
}
static void draw_spr(const ObjSprite *s, pvr_list_t list, s32 px, s32 py, int vflip_extra) {
    int i;
    if (s->multi != NULL && s->n_multi > 1) { /* Rope_Draw: the same frame at every rope point */
        for (i = 0; i < s->n_multi; i++)
            draw_spr_at(s, list, s->multi[i][0], s->multi[i][1], vflip_extra);
        return;
    }
    draw_spr_at(s, list, px, py, vflip_extra);
}

void obj_draw(int list) {
    Obj *o;
    int i, vf;
    s32 x, y;
    const ObjSprite *s;
    if (!g_obj.hdr)
        return;
    for (o = g_obj.head; o; o = o->next) {
        if (!o->used || (o->h.obj_flags & 4))
            continue;
        if (o->has_sprA)
            draw_spr(&o->sprA, (pvr_list_t)list, o->sprA.x, o->sprA.y, 0);
        if (o->has_spr)
            draw_spr(&o->spr, (pvr_list_t)list, o->spr.x, o->spr.y, 0);
    }
    s = ring_draw_sprite();
    for (i = 0; i < ring_draw_count(); i++)
        if (ring_draw_get(i, &x, &y, &vf))
            draw_spr(s, (pvr_list_t)list, x >> 8, y >> 8, vf);
    for (i = 0; i < ring_fx_max(); i++)
        if (ring_fx_get(i, &s))
            draw_spr(s, (pvr_list_t)list, s->x, s->y, 0);
}

void objects_draw(void) { obj_draw(PVR_LIST_PT_POLY); }

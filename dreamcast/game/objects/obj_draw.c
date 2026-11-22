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

static void draw_spr(const ObjSprite *s, pvr_list_t list, s32 px, s32 py, int vflip_extra) {
    const DsprImage *img = spr_image(s);
    const Texture *t;
    int hf, vf, prio, order;
    float x, y, x0, y0, z;
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
    draw_sprite_world(t, list, x0, y0, (float)img->w, (float)img->h, (float)img->x, (float)img->y,
                      (float)(img->x + img->w), (float)(img->y + img->h), hf | (vf << 1), z, 0xffffffff);
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

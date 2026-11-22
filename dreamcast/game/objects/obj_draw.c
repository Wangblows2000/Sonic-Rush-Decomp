/*
 * Object rendering on the Dreamcast (KOS only): every object sprite is one
 * textured quad cut from the converted BAC atlas (tools/dreamcast/sprites/bac2dc.py):
 * PAL4 twiddled pages, the image's palette as a PVR palette row (a PaletteAnim
 * selects another row), ARGB1555 pages for images with more than 15 colours.  The animation state was
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

/* the pages of a resource: PAL4 twiddled with one PVR palette row per palette
 * (atlas<N>.pt4 + palettes.bin), ARGB1555 for direct pages or when the palette
 * RAM is full (engine/render.c spr_atlas_load) */
int obj_draw_upload(ObjRes *r, const char *dir) {
    SprAtlas *a = (SprAtlas *)calloc(1, sizeof(SprAtlas));
    int ok;
    if (!a)
        return 0;
    ok = spr_atlas_load(a, dir, r->dspr, 0);
    r->atlas = a;
    return ok;
}

void obj_draw_release(ObjRes *r) {
    if (r->atlas) {
        spr_atlas_free((SprAtlas *)r->atlas);
        free(r->atlas);
        r->atlas = NULL;
    }
}

/* VRAM / palette rows held by the object sprites (budget probe) */
u32 objects_vram_used(void) {
    u32 n = 0;
    int s;
    for (s = 0; s < OBJ_BAC_COUNT; s++)
        if (g_obj.res[s].atlas)
            n += ((SprAtlas *)g_obj.res[s].atlas)->vram;
    return n;
}

static const float k_zbase[4] = { 4.5f, 3.5f, 2.5f, 1.5f };
/* semi-transparent OBJ: the stage's BLDALPHA blend (EVA ~ EVB) as vertex alpha */
#define OBJ_SEMITRANS_ARGB 0xa0ffffffu

/* a textured quad with free corners (rot-scale OAM), screen coordinates */
static void quad4(const Texture *t, int bank, pvr_list_t list, const float *xs, const float *ys, float u0, float v0,
                  float u1, float v1, float z, uint32_t argb) {
    pvr_poly_cxt_t cxt;
    pvr_poly_hdr_t hdr;
    pvr_vertex_t v;
    pvr_poly_cxt_txr(&cxt, list, tex_txr_format(t, bank), t->w, t->h, t->ptr, PVR_FILTER_NONE);
    pvr_poly_compile(&hdr, &cxt);
    pvr_prim(&hdr, sizeof(hdr));
    u0 /= t->w; u1 /= t->w; v0 /= t->h; v1 /= t->h;
    v.flags = PVR_CMD_VERTEX; v.argb = argb; v.oargb = 0; v.z = z;
    v.x = xs[3]; v.y = ys[3]; v.u = u0; v.v = v1; pvr_prim(&v, sizeof(v)); /* bottom left  */
    v.x = xs[0]; v.y = ys[0]; v.u = u0; v.v = v0; pvr_prim(&v, sizeof(v)); /* top left     */
    v.x = xs[2]; v.y = ys[2]; v.u = u1; v.v = v1; pvr_prim(&v, sizeof(v)); /* bottom right */
    v.flags = PVR_CMD_VERTEX_EOL;
    v.x = xs[1]; v.y = ys[1]; v.u = u1; v.v = v0; pvr_prim(&v, sizeof(v)); /* top right    */
}

/* Water Palace (ZoneFx_Zone2_*TintedPalettes): below the water line the OBJ palette rows
 * 2..15 are tinted (r - 10, g - 1); rows 0 / 1 (rings, HUD) and the 3D layer (models, 3D
 * BACs, the boost ribbon) are not. Here: the vertex colour of the sprite part below the
 * line (objects_set_water_tint, set by the zone effects every frame). */
static float s_tint_y = 1e9f;
static uint32_t s_tint_argb = 0xffffffffu;
static int s_tint_on;
void objects_set_water_tint(float y, uint32_t argb) {
    s_tint_y = y;
    s_tint_argb = argb;
}
static uint32_t tint_mul(uint32_t a, uint32_t t) {
    uint32_t r = ((a >> 16) & 0xff) * ((t >> 16) & 0xff) / 255, g = ((a >> 8) & 0xff) * ((t >> 8) & 0xff) / 255;
    uint32_t b = (a & 0xff) * (t & 0xff) / 255;
    return (a & 0xff000000u) | r << 16 | g << 8 | b;
}
/* draw_sprite_world_pal with the part below the water line tinted (vflip: the quad's top
 * row is texture row v1) */
static void spr_quad(const Texture *t, int bank, pvr_list_t list, float x0, float y0, float w, float h, float u0,
                     float v0, float u1, float v1, int flip, float z, uint32_t argb) {
    float cut;
    if (!s_tint_on || y0 + h <= s_tint_y) {
        draw_sprite_world_pal(t, bank, list, x0, y0, w, h, u0, v0, u1, v1, flip, z, argb);
        return;
    }
    if (y0 >= s_tint_y) {
        draw_sprite_world_pal(t, bank, list, x0, y0, w, h, u0, v0, u1, v1, flip, z, tint_mul(argb, s_tint_argb));
        return;
    }
    cut = s_tint_y - y0;
    if (flip & 2) {
        draw_sprite_world_pal(t, bank, list, x0, y0, w, cut, u0, v1 - cut, u1, v1, flip, z, argb);
        draw_sprite_world_pal(t, bank, list, x0, y0 + cut, w, h - cut, u0, v0, u1, v1 - cut, flip, z,
                              tint_mul(argb, s_tint_argb));
    } else {
        draw_sprite_world_pal(t, bank, list, x0, y0, w, cut, u0, v0, u1, v0 + cut, flip, z, argb);
        draw_sprite_world_pal(t, bank, list, x0, y0 + cut, w, h - cut, u0, v0 + cut, u1, v1, flip, z,
                              tint_mul(argb, s_tint_argb));
    }
}

static void draw_spr_at(const ObjSprite *s, pvr_list_t list, s32 px, s32 py, int vflip_extra) {
    const DsprImage *img = spr_image(s);
    const Texture *t;
    SprAtlas *a;
    int hf, vf, prio, order, bank, pal = -1, im;
    float x, y, x0, y0, z, u0, v0, u1, v1, tmp;
    /* AnimatorSprite flag 0x400 = OAM semi-transparent (attr0 mode 1): those sprites go to the
     * translucent list (obj_draw(PVR_LIST_TR_POLY)), all others to the punch-through list */
    int semi = (s->flags & 0x400) != 0;
    uint32_t argb = semi ? OBJ_SEMITRANS_ARGB : 0xffffffff;
    if (img == NULL || s->hidden || semi != (list == PVR_LIST_TR_POLY))
        return;
    a = (SprAtlas *)s->res->atlas;
    if (a == NULL || img->page >= a->n_pages)
        return;
    im = (int)(img - s->res->images);
    if (s->palanim && a->pal.blob && a->pal.image_row[im] == 0) {
        /* PaletteAnim: the row holds the palette of animation obj_palanim_anim() */
        int an = obj_palanim_anim(s->palanim);
        if (an >= 0 && an < a->pal.n_anims && a->pal.anim_pal[an] != SPRPAL_NONE)
            pal = a->pal.anim_pal[an];
    }
    bank = spr_atlas_bank(a, im, pal);
    t = &a->tex[img->page];
    if (t->ptr == NULL)
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
        if (s_tint_on && y > s_tint_y)   /* rot-scale: the sprite position decides */
            argb = tint_mul(argb, s_tint_argb);
        quad4(t, bank, list, xs, ys, u0, v0, u1, v1, z, argb);
        return;
    }
    if (s->ppos != NULL) { /* StageObj_DrawSpriteAlt: piece 0 offset (one-piece frames) */
        x0 += (float)(s->ppos[0][0] >> 8);
        y0 += (float)(s->ppos[0][1] >> 8);
    }
    spr_quad(t, bank, list, x0, y0, (float)img->w, (float)img->h, (float)img->x, (float)img->y,
             (float)(img->x + img->w), (float)(img->y + img->h), hf | (vf << 1), z, argb);
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

int g_obj_model_sprites = 1; /* objects_set_model_sprites */
static int is_model(const Obj *o) { return o->model != NULL || (o->kind == OBJK_GIMMICK && o->prm && o->prm->type == 0x7a); }
/* the main sprite: stand-ins for a model / 3D BAC are drawn while
 * g_obj_model_sprites; otherwise AnimatorSprite flag 0x80000 hides it */
static int obj_spr_drawn(const Obj *o) {
    if (is_model(o) || o->bac3d)
        return g_obj_model_sprites;
    return !(o->spr.flags & 0x80000);
}
void objects_set_model_sprites(int on) { g_obj_model_sprites = on; }

static void draw_trails(void);
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
        /* the ring sparkles (ring BAC: OBJ rows 0 / 1) and 3D stand-ins stay untinted */
        s_tint_on = s_tint_y < 1e8f && !o->bac3d && !is_model(o) && !(o->has_spr && o->spr.res == &g_obj.res[OBJ_BAC_RING]);
        if (o->has_sprA)
            draw_spr(&o->sprA, (pvr_list_t)list, o->sprA.x, o->sprA.y, 0);
        /* StageObj_Draw2DEx: with a model (@0x124/@0x128, "busy") the main sprite is
         * not drawn; the model view stands for it (objects_model_get) */
        if (o->has_spr && obj_spr_drawn(o))
            draw_spr(&o->spr, (pvr_list_t)list, o->spr.x, o->spr.y, 0);
        for (i = 0; i < o->n_xspr; i++) { /* extra sprite pairs (Mine, Cargo): every recorded point */
            const ObjSprite *xs = o->xspr[i];
            int k;
            if (xs == NULL || xs->res == NULL)
                continue;
            if (xs->multi != NULL) {
                for (k = 0; k < xs->n_multi; k++)
                    draw_spr_at(xs, (pvr_list_t)list, xs->multi[k][0], xs->multi[k][1], 0);
            } else {
                draw_spr_at(xs, (pvr_list_t)list, xs->x, xs->y, 0);
            }
        }
    }
    s_tint_on = 0;   /* rings, ring sparkles, the ribbon: not tinted */
    if (list == PVR_LIST_TR_POLY)
        draw_trails();
    s = ring_draw_sprite();
    for (i = 0; i < ring_draw_count(); i++)
        if (ring_draw_get(i, &x, &y, &vf))
            draw_spr(s, (pvr_list_t)list, x >> 8, y >> 8, vf);
    for (i = 0; i < ring_fx_max(); i++)
        if (ring_fx_get(i, &s))
            draw_spr(s, (pvr_list_t)list, s->x, s->y, 0);
}

/* EffTrail_Draw: flat-shaded quads at the 3D layer's depth (just behind the player model, ~0.006),
 * polygon alpha a / 31 (objects_trail_quads) */
static void draw_trails(void) {
    ObjTrailQuad q[24];
    pvr_poly_cxt_t cxt;
    pvr_poly_hdr_t hdr;
    pvr_vertex_t v;
    int n = objects_trail_quads(q, 24), i, k;
    static const int order[4] = { 0, 1, 2, 3 };
    if (n <= 0)
        return;
    pvr_poly_cxt_col(&cxt, PVR_LIST_TR_POLY);
    cxt.gen.culling = PVR_CULLING_NONE;
    pvr_poly_compile(&hdr, &cxt);
    pvr_prim(&hdr, sizeof(hdr));
    for (i = 0; i < n; i++) {
        u16 c = q[i].bgr555;
        uint32_t r = (c & 0x1f) * 255 / 31, g = ((c >> 5) & 0x1f) * 255 / 31, b = ((c >> 10) & 0x1f) * 255 / 31;
        v.argb = ((uint32_t)q[i].alpha * 255 / 31) << 24 | r << 16 | g << 8 | b;
        v.oargb = 0;
        v.z = 0.0055f;
        v.u = v.v = 0.0f;
        for (k = 0; k < 4; k++) {
            v.flags = k == 3 ? PVR_CMD_VERTEX_EOL : PVR_CMD_VERTEX;
            v.x = ((float)q[i].x[order[k]] / 256.0f - g_cam_x) * VIEW_SCALE;
            v.y = ((float)q[i].y[order[k]] / 256.0f - g_cam_y) * VIEW_SCALE;
            pvr_prim(&v, sizeof(v));
        }
    }
}

void objects_draw(void) { obj_draw(PVR_LIST_PT_POLY); }
/* the semi-transparent sprites (player effects): call inside the translucent list */
void objects_draw_tr(void) { obj_draw(PVR_LIST_TR_POLY); }

/*
 * Object rendering on the Dreamcast (KOS only): every object sprite is one
 * textured quad cut from the converted BAC atlas (tools/dreamcast/sprites/bac2dc.py):
 * PAL4 twiddled pages, the image's palette as a PVR palette row (a PaletteAnim
 * selects another row), ARGB1555 pages for images with more than 15 colours.  The animation state was
 * advanced in objects_update (StageObj_Draw2DEx animation part); this file
 * only places the current frame, like AnimatorSprite_Draw:
 *   unflipped  image top-left = position - origin
 *   H-flipped  mirrored about the origin (x = position - (w - origin_x))
 * Layering (Stage_InitDisplay priorities: BG2 layer A 0, BG0 3D 1, BG3 layer B 2, BG1 zone
 * BG 3; an OBJ is in front of a BG of the same priority): OAM priority (sprite flags bits
 * 17-18) picks a depth band between the layers main.c draws (1/w style, bigger = nearer):
 *   zone BG 0.0005 < OBJ prio 3 < layer B 0.001 < OBJ prio 2 < 3D layer (player, models,
 *   3D BACs, trails: ~0.0035..0.03) < OBJ prio 1 < layer A 10 < OBJ prio 0 < HUD (100+)
 * then OAM order inside the band (lower order in front).  Object models (objects_model_get)
 * are drawn here too (objects_draw_models), in the 3D layer.
 */
#include <kos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../engine/render.h"
#include "../engine/pak.h"
#include "../engine/loading.h"
#include "../model/model.h"
#include "../player/dcp_model.h"
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

/* depth band per OAM priority: base + (0x40 - order) * step (see the header) */
static const float k_zbase[4] = { 11.0f, 2.0f, 0.0011f, 0.00055f };
static const float k_zstep[4] = { 0.01f, 0.01f, 0.00003f, 0.000007f };
/* 3D BACs (Sprite3D, drawn in BG0 on the DS): about the player model's centre depth */
#define OBJ_Z_BAC3D 0.0058f
#define OBJ_ZSTEP_BAC3D 0.000003f
static int s_bac3d;   /* the object being drawn is a 3D BAC stand-in */
/* boss arenas (RLVL_FLAG_BOSS): the whole arena is the 3D layer (sky, floor models), the
 * sprites stay in front of it (bands 4.5 / 3.5 / 2.5 / 1.5 by priority) */
static const float k_zbase_boss[4] = { 4.5f, 3.5f, 2.5f, 1.5f };
static int s_boss;
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
    if (s_boss)
        z = k_zbase_boss[prio] + (0x40 - order) * 0.01f;
    else if (s_bac3d)
        z = OBJ_Z_BAC3D + (0x40 - order) * OBJ_ZSTEP_BAC3D;
    else
        z = k_zbase[prio] + (0x40 - order) * k_zstep[prio];
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
static int model_ready(const Obj *o);
/* the main sprite: the stand-in of a model is drawn while g_obj_model_sprites or when its
 * model is not loaded; a 3D BAC (no Sprite3D renderer) is always drawn as its 2D BAC, in
 * the 3D layer; otherwise AnimatorSprite flag 0x80000 hides it */
static int obj_spr_drawn(const Obj *o) {
    if (is_model(o))
        return g_obj_model_sprites || !model_ready(o);
    if (o->bac3d)
        return 1;
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
    s_boss = (g_obj.hdr->flags & RLVL_FLAG_BOSS) != 0;
    for (o = g_obj.head; o; o = o->next) {
        if (!o->used || (o->h.obj_flags & 4))
            continue;
        /* the ring sparkles (ring BAC: OBJ rows 0 / 1) and 3D stand-ins stay untinted */
        s_tint_on = s_tint_y < 1e8f && !o->bac3d && !is_model(o) && !(o->has_spr && o->spr.res == &g_obj.res[OBJ_BAC_RING]);
        s_bac3d = 0;
        if (o->has_sprA)
            draw_spr(&o->sprA, (pvr_list_t)list, o->sprA.x, o->sprA.y, 0);
        s_bac3d = o->bac3d != 0;
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
    s_bac3d = 0;
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

/* ================================================================ object models
 * StageObj_Draw3DEx + Model_Draw for the objects_model_get views (StageObj_LoadModelSimple
 * blocks: NNS_G3dDraw1Mat1Shp, one material + one shape each, no node matrices):
 *   trans = (screen x, -screen y, z) of the object (Camera_WorldToScreen), rot = the
 *   object's 3x3 matrix, base scale = its scale: v' = v * S * R + T, under the stage camera
 *   (BossCamera fixed perspective); disp_flags 0x200 = the off-axis frustum centred on the
 *   object (dcm_camera_stage_offaxis, as for the player).  Lights: the stage vectors
 *   (Draw3DEx without flag 0x800) and the stage light colours.
 * The models are read from /cd/models/<id>.dcm by objects_models_prepare (outside the PVR
 * lists: the texture upload) and kept until the object data changes (next stage). */
#define OM_MAX 8
#define OM_MAX_MATS 32
typedef struct {
    const char *id;       /* ObjModelView.model (static strings) */
    int state;            /* 0 empty, 1 loaded, -1 missing */
    void *data;
    dcm_model_t m;
    dcm_mtx_t *pal;       /* bind pose palette (identity joints) */
} ObjModelSlot;
static ObjModelSlot s_om[OM_MAX];
static const void *s_om_hdr;
static u32 s_om_tris, s_om_views;   /* statistics (objects_models_stats) */

static void om_free_all(void) {
    int i;
    for (i = 0; i < OM_MAX; i++) {
        if (s_om[i].state == 1) {
            dcm_model_free(&s_om[i].m);
            free(s_om[i].data);
            free(s_om[i].pal);
        }
        memset(&s_om[i], 0, sizeof(s_om[i]));
    }
}
void objects_models_free(void) {
    om_free_all();
    s_om_hdr = NULL;
}

static ObjModelSlot *om_find(const char *id) {
    int i;
    for (i = 0; i < OM_MAX; i++)
        if (s_om[i].state != 0 && (s_om[i].id == id || strcmp(s_om[i].id, id) == 0))
            return &s_om[i];
    return NULL;
}

static void *om_read(const char *path, uint32_t *size) {
    void *pb;
    file_t f;
    uint8_t *b;
    switch (pak_load(path, &pb, size)) {
    case PAK_FOUND: return pb;
    case PAK_MISSING: return NULL;
    default: break;
    }
    f = fs_open(path, O_RDONLY);
    if (f == FILEHND_INVALID)
        return NULL;
    *size = fs_total(f);
    b = memalign(32, *size ? *size : 4);
    if (b && loading_read(f, b, *size) != (ssize_t)*size) {
        free(b);
        b = NULL;
    }
    fs_close(f);
    return b;
}

static ObjModelSlot *om_load(const char *id) {
    static dcm_local_t loc[DCM_MAX_JOINTS];
    char path[128];
    uint32_t size = 0;
    ObjModelSlot *s = NULL;
    int i;
    for (i = 0; i < OM_MAX && s == NULL; i++)
        if (s_om[i].state == 0)
            s = &s_om[i];
    if (s == NULL)
        return NULL;
    s->id = id;
    s->state = -1;
    snprintf(path, sizeof(path), "/cd/models/%s.dcm", id);
    s->data = om_read(path, &size);
    if (s->data == NULL || dcm_model_load(&s->m, s->data, size, 1) != 0) {
        printf("objects: model %s missing\n", id);
        free(s->data);
        s->data = NULL;
        return s;
    }
    s->pal = (dcm_mtx_t *)malloc(sizeof(dcm_mtx_t) * (s->m.hdr->num_palette ? s->m.hdr->num_palette : 1));
    if (s->pal == NULL || s->m.hdr->num_materials > OM_MAX_MATS || s->m.hdr->num_joints > DCM_MAX_JOINTS) {
        dcm_model_free(&s->m);
        free(s->data);
        free(s->pal);
        s->data = NULL;
        s->pal = NULL;
        return s;
    }
    dcm_bind_locals(&s->m, loc);
    dcm_pose(&s->m, loc, s->pal);
    s->state = 1;
    printf("objects: model %s loaded (%u bytes, vram free %lu)\n", id, (unsigned)size, (unsigned long)render_vram_free());
    return s;
}

/* call once per frame outside the PVR lists, before objects_draw_models */
void objects_models_prepare(void) {
    ObjModelView v;
    int i, n;
    if (g_obj.hdr != s_om_hdr) { /* a new stage: the previous stage's models go */
        om_free_all();
        s_om_hdr = g_obj.hdr;
    }
    if (!g_obj.hdr)
        return;
    n = objects_model_count();
    for (i = 0; i < n; i++)
        if (objects_model_get(i, &v) && v.model && om_find(v.model) == NULL)
            om_load(v.model);
}

static int model_ready(const Obj *o) {
    const char *id = o->model;
    ObjModelSlot *s;
    if (id == NULL) /* RopeC */
        id = o->k.mdl.shape ? "mod__gmk_rope_c__gmk_rope_c02" : "mod__gmk_rope_c__gmk_rope_c01";
    s = om_find(id);
    return s != NULL && s->state == 1;
}

/* NNS_G3dDraw1Mat1Shp for the blocks in `shapes`: joint visibility + material alpha 0 pick
 * the converted draws (dcm_draw_t: material, shape, gating joint) of those shapes.
 * Returns 0 if a draw outside the set cannot be left out that way (drawn anyway). */
static int om_shape_filter(const dcm_model_t *m, u32 shapes, dcm_matstate_t *st, uint8_t *vis) {
    int d, nm = m->hdr->num_materials, nj = m->hdr->num_joints, ok = 1;
    uint8_t mat_on[OM_MAX_MATS];
    memset(mat_on, 0, sizeof(mat_on));
    memset(vis, 0, (size_t)nj);
    dcm_matstate_init(m, st);
    for (d = 0; d < m->hdr->num_draws; d++) {
        const dcm_draw_t *dr = &m->draws[d];
        if (dr->shape >= 32 || !(shapes >> dr->shape & 1))
            continue;
        mat_on[dr->material] = 1;
        if (dr->vis_node != 0xFFFF && dr->vis_node < nj)
            vis[dr->vis_node] = 1;
    }
    for (d = 0; d < nm; d++)
        if (!mat_on[d])
            st[d].alpha = 0;
    for (d = 0; d < m->hdr->num_draws; d++) {
        const dcm_draw_t *dr = &m->draws[d];
        int in = dr->shape < 32 && (shapes >> dr->shape & 1);
        int drawn = mat_on[dr->material] && st[dr->material].alpha != 0 && (dr->vis_node == 0xFFFF || dr->vis_node >= nj || vis[dr->vis_node]);
        if (!in && drawn)
            ok = 0;
    }
    return ok;
}

/* the model matrix and camera of one view (rep k), DS screen units; 0 = off screen */
static int om_place(const ObjModelView *v, const dcm_model_t *m, int k, dcm_mtx_t *mdl, dcm_camera_t *cam) {
    float sc[3], tx, ty, tz, r = 0.0f;
    int i, j;
    for (i = 0; i < 3; i++) {
        float a = m->hdr->bbox_min[i] < 0 ? -m->hdr->bbox_min[i] : m->hdr->bbox_min[i];
        float b = m->hdr->bbox_max[i] < 0 ? -m->hdr->bbox_max[i] : m->hdr->bbox_max[i];
        sc[i] = v->scale ? (float)v->scale[i] / 4096.0f : 1.0f;
        a = (a > b ? a : b) * (sc[i] < 0 ? -sc[i] : sc[i]);
        if (a > r)
            r = a;
    }
    r *= 1.7320508f; /* the bounding box's corner */
    tx = (float)v->x / 256.0f - g_cam_x;
    ty = (float)(v->y + k * v->rep_dy) / 256.0f - g_cam_y;
    tz = (float)v->z / 256.0f;
    if (tx + r < -32.0f || tx - r > 256.0f + 32.0f || ty + r < -32.0f || ty - r > 192.0f + 32.0f)
        return 0;
    ty = -ty;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            mdl->m[i][j] = sc[i] * (v->mtx ? (float)v->mtx[i * 3 + j] / 4096.0f : (i == j ? 1.0f : 0.0f));
    dcm_camera_stage(cam);
    if (v->persp)
        dcm_camera_stage_offaxis(cam, &tx, &ty);
    mdl->m[3][0] = tx;
    mdl->m[3][1] = ty;
    mdl->m[3][2] = tz;
    return 1;
}

/* every model view in this PVR list (call in OP, PT and TR, with the player model) */
void objects_draw_models(int list) {
    static dcm_matstate_t st[OM_MAX_MATS];
    static uint8_t vis[DCM_MAX_JOINTS];
    static int warned;
    const u16 *lc = dcp_model_stage_lights(g_dcp_world.zone, g_dcp_world.act);
    dcm_lights_t lights;
    dcm_draw_opts_t opts;
    ObjModelView v;
    int i, k, n;
    if (!g_obj.hdr || g_obj.hdr != s_om_hdr)
        return;
    dcm_lights_stage(&lights, 0);
    for (i = 0; i < lights.count && i < 3; i++) {
        lights.col[i][0] = (float)(lc[i] & 31) / 31.0f;
        lights.col[i][1] = (float)((lc[i] >> 5) & 31) / 31.0f;
        lights.col[i][2] = (float)((lc[i] >> 10) & 31) / 31.0f;
    }
    opts.list = list;
    opts.pt_list = PVR_LIST_PT_POLY;
    opts.alpha_scale = 255;
    if (list == PVR_LIST_OP_POLY)
        s_om_views = 0;
    n = objects_model_count();
    for (i = 0; i < n; i++) {
        ObjModelSlot *s;
        if (!objects_model_get(i, &v) || !v.visible || v.model == NULL)
            continue;
        s = om_find(v.model);
        if (s == NULL || s->state != 1)
            continue;
        if (!om_shape_filter(&s->m, v.shapes, st, vis) && !warned) {
            warned = 1;
            printf("objects: model %s: shapes %08lx not separable\n", v.model, (unsigned long)v.shapes);
        }
        for (k = 0; k < (v.reps > 0 ? v.reps : 1); k++) {
            dcm_mtx_t mdl;
            dcm_camera_t cam;
            if (!om_place(&v, &s->m, k, &mdl, &cam))
                continue;
            if (list == PVR_LIST_OP_POLY)
                s_om_views++;
            s_om_tris += (u32)dcm_draw_ex(&s->m, s->pal, &mdl, &cam, vis, &lights, &opts, st);
        }
    }
}

/* statistics: model views drawn in the last frame, PVR triangles since boot */
void objects_models_stats(u32 *views, u32 *tris) {
    if (views)
        *views = s_om_views;
    if (tris)
        *tris = s_om_tris;
}

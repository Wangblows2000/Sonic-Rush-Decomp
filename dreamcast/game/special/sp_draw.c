/*
 * sp_draw.c - draw-command capture and the single-screen composition.
 *
 * The draw task of the original (SpecialStage_State_IntroDraw / _PlayDraw and
 * the HUD) records commands here while special_run_frame() runs; special_draw()
 * replays them on the PVR.  Composition of the two DS screens into one
 * 640x480 view (docs/dreamcast/translation/SPECIAL.md, section 3):
 *   - the main engine (the DS touch screen: 3D half-pipe + its sprites and BGs)
 *     is the view itself: 256x192 -> 640x480 (x2.5), same camera and framing;
 *   - top-screen (engine 1) HUD elements are drawn x1.5 near the edges:
 *     ring count / quota top-left, the seven emerald slots top-right,
 *     the course progress bar under them;
 *   - the top screen's "SPECIAL STAGE" logo (intro) is drawn over the
 *     backdrop of the intro, the results (emerald circle, "SONIC GOT A CHAOS
 *     EMERALD!") become a full-view card at x2.5 when the clear sequence starts.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sp_internal.h"
#ifdef _arch_dreamcast
#include <kos.h>
#include <dc/pvr.h>
#include "../engine/render.h"
#endif

int g_sp_light_mode;
u8 *g_sp_host_fb;
extern int g_sp_fog;
extern float g_sp_hud_palf;
extern int g_sp_layer_main[4], g_sp_layer_sub[4];
extern int g_sp_upload_textures;

#define MAX_CMDS 768
enum { GRP_MAIN = 0, GRP_RINGS, GRP_EMERALDS, GRP_BAR, GRP_CARD };

typedef struct {
    int kind;
    int list_hint;
    /* model / sprite 3D */
    const SpModel *m;
    const SpMdlRes *res;
    int light;
    dcm_mtx_t world;
    int image;
    const SpSprRes *sres;
    u32 sflags;
    /* sprite 2D */
    int group, prio, seq;
    float x, y, sx, sy;
    /* misc */
    float f;
    int a, b;
} SpCmd;

static SpCmd s_cmd[MAX_CMDS];
static int s_ncmd, s_seq;

void sp_draw_reset(void) {
    s_ncmd = 0;
    s_seq = 0;
}
int sp_draw_count(void) { return s_ncmd; }

static SpCmd *newcmd(int kind) {
    SpCmd *c;
    if (s_ncmd >= MAX_CMDS)
        return NULL;
    c = &s_cmd[s_ncmd++];
    memset(c, 0, sizeof(*c));
    c->kind = kind;
    c->seq = s_seq++;
    return c;
}

/* ------------------------------------------------------------ recording */
static void fx_to_f(const MtxFx43 *a, dcm_mtx_t *o) {
    int r, c;
    for (r = 0; r < 4; r++)
        for (c = 0; c < 3; c++)
            o->m[r][c] = a->m[r * 3 + c] / 4096.0f;
}

static int has_op(const SpModel *m, int op) {
    int i;
    for (i = 0; i < 8 && m->cb[i]; i++)
        if (m->cb[i] == op)
            return 1;
    return 0;
}

void sp_draw_model(const SpModel *m, int light_mode) {
    SpCmd *c;
    dcm_mtx_t W;
    int r, k;
    if (m->flag & 1)
        return;
    c = newcmd(m->type == 2 ? SPD_SPRITE2D + 100 : SPD_MODEL);
    if (!c)
        return;
    c->m = m;
    c->res = m->res;
    c->light = light_mode;
    if (has_op(m, 2)) {
        /* FlushVP + RestoreMtx(slot unk8): the stored (world) matrix */
        fx_to_f(&g_sp_mtx_slot[m->unk8 & 31], &W);
    } else {
        MtxFx33 rot = m->rot;
        if (has_op(m, 0x13) || has_op(m, 0x14))
            Model_TransposeRot(&g_sp_view, &rot);   /* Model_MatrixOp_SetCameraRot */
        for (r = 0; r < 3; r++)
            for (k = 0; k < 3; k++)
                W.m[r][k] = rot.m[r * 3 + k] / 4096.0f * (m->scale[r] / 4096.0f);
        for (k = 0; k < 3; k++)
            W.m[3][k] = m->trans[k] / 4096.0f;
        if (has_op(m, 0x10)) {
            /* MultMtx43 (@0x60) then Translate2 (@0x54): v * T2 * M60 * base */
            dcm_mtx_t M, t;
            fx_to_f(&m->unk60, &M);
            dcm_mtx_mul(&t, &M, &W);
            if (has_op(m, 0xd)) {
                float tx = m->trans2[0] / 4096.0f, ty = m->trans2[1] / 4096.0f, tz = m->trans2[2] / 4096.0f;
                for (k = 0; k < 3; k++)
                    t.m[3][k] += tx * t.m[0][k] + ty * t.m[1][k] + tz * t.m[2][k];
            }
            W = t;
        }
    }
    c->world = W;
    if (m->type == 2) {
        c->image = sp_sprite_image(&m->spr);
        c->sres = m->spr.res;
        c->sflags = m->spr.flags;
    }
}

static int sprite_group(const SpSprite *s, int *prio) {
    SpHud *h = gSpecialStageHud;
    *prio = s->unk46;
    if (s->engine == 0)
        return GRP_MAIN;
    if (!h)
        return GRP_CARD;
    if (h->unkBD4 >= 0)
        return GRP_CARD;
    if (s >= &h->spr[0] && s <= &h->spr[7])
        return GRP_RINGS;
    if (s == &h->spr[18])
        return GRP_RINGS;
    if (s >= &h->spr[10] && s <= &h->spr[17])
        return GRP_EMERALDS;
    return GRP_BAR;
}

void sp_draw_sprite(const SpSprite *s, s32 scale_x, s32 scale_y, int rotscale) {
    SpCmd *c;
    if (s->flags & 0x80000)
        return;
    if (sp_sprite_image(s) < 0)
        return;
    c = newcmd(SPD_SPRITE2D);
    if (!c)
        return;
    c->sres = s->res;
    c->image = sp_sprite_image(s);
    c->sflags = s->flags;
    c->group = sprite_group(s, &c->prio);
    c->x = s->x;
    c->y = s->y;
    c->sx = rotscale ? scale_x / 4096.0f : 1.0f;
    c->sy = rotscale ? scale_y / 4096.0f : 1.0f;
    if (gSpecialStageHud && s == &gSpecialStageHud->spr[18])
        c->f = g_sp_hud_palf;   /* OBJ palette slot 8 lerp -> stage-colour emerald */
}

void sp_draw_fadequad(s32 alpha) {
    SpCmd *c = newcmd(SPD_FADEQUAD);
    if (c)
        c->a = alpha;
}

void sp_draw_ratingbar(float f, int color_idx, int blink) {
    SpCmd *c = newcmd(SPD_RATINGBAR);
    if (!c)
        return;
    c->f = f;
    c->a = color_idx;
    c->b = blink;
}

void sp_draw_bg(int res, int engine, int layer, s16 sx, s16 sy, int alpha16) {
    SpCmd *c = newcmd(SPD_BG);
    if (!c)
        return;
    c->a = res;
    c->b = engine;
    c->prio = layer;
    c->x = sx;
    c->y = sy;
    c->f = alpha16 / 16.0f;
}

/* ------------------------------------------------------------ layout */
typedef struct {
    float ox, oy, k;   /* DC = (ox, oy) + DS * k  (sprite scale k) */
} SpXf;

static SpXf group_xf(int grp, float *kx) {
    SpXf t;
    *kx = 0;
    switch (grp) {
    case GRP_RINGS:   /* DS (20, 98) -> DC (16, 8), x1.5 */
        t.ox = 16 - 20 * 1.5f;
        t.oy = 8 - 98 * 1.5f;
        t.k = 1.5f;
        break;
    case GRP_EMERALDS: /* slots x 32..224 step 32 -> step 36 (x 1.125), y 84 -> 30, icons x1.5 */
        t.ox = 640 - 16 - 18 - 224 * 1.125f;
        t.oy = 30 - 84 * 1.5f;
        t.k = 1.5f;
        *kx = 1.125f;
        break;
    case GRP_BAR:      /* DS (244, 160) -> DC (624, 52), x1.5: under the emeralds (the bottom edge is the
                        * trick timing bar's, SpecialStageHud_Unk_ov001_02308884) */
        t.ox = 624 - 244 * 1.5f;
        t.oy = 52 - 160 * 1.5f;
        t.k = 1.5f;
        break;
    default:           /* main screen and the results card: x2.5 */
        t.ox = 0;
        t.oy = 0;
        t.k = 2.5f;
        break;
    }
    return t;
}

/* screen rectangle of a 2D sprite command (DC pixels) */
static void sprite_rect(const SpCmd *c, float *x0, float *y0, float *x1, float *y1, int *flip) {
    const SpDsprImage *im = &c->sres->images[c->image];
    float kx, px, py, w = im->w, h = im->h, ox = im->ox, oy = im->oy;
    SpXf t = group_xf(c->group, &kx);
    px = t.ox + c->x * (kx ? kx : t.k);
    py = t.oy + c->y * t.k;
    *flip = 0;
    if (c->sflags & 0x80) {
        *flip |= 1;
        ox = w - ox;
    }
    if (c->sflags & 0x100) {
        *flip |= 2;
        oy = h - oy;
    }
    *x0 = px - ox * t.k * c->sx;
    *y0 = py - oy * t.k * c->sy;
    *x1 = *x0 + w * t.k * c->sx;
    *y1 = *y0 + h * t.k * c->sy;
}

/* intro logo crop of sp_stdm_base_a (the two stripe colours keyed out) */
#define LOGO_X0 13
#define LOGO_Y0 46
#define LOGO_W 233
#define LOGO_H 108

/* ------------------------------------------------------------ BG images (mnbg) */
typedef struct {
    int w, h, tw, th;
    u16 *pix;          /* ARGB1555 tw x th */
    void *tex;
} SpImg;
static SpImg s_bgimg[8];   /* 0..6 bg resources, 7 = intro logo */
static SpImg s_sprimg[4 + 2];
static int s_inited;

static int pow2(int v) {
    int p = 8;
    while (p < v)
        p <<= 1;
    return p;
}

static void bg_image(int idx) {
    SpImg *im = &s_bgimg[idx];
    const u8 *b;
    int x, y, src = idx == 7 ? 0 : idx;
    u16 pal[256], w, h;
    int key0 = -1, key1 = -1;
    if (im->pix || !data_ov001_0231f5c4 || !data_ov001_0231f5c4->bg[src])
        return;
    b = data_ov001_0231f5c4->bg[src];
    memcpy(&w, b + 4, 2);
    memcpy(&h, b + 6, 2);
    memcpy(pal, b + 16, 512);
    if (idx == 7) {
        /* the two most frequent colours of the logo screen are the stripes */
        static int cnt[256];
        int i;
        memset(cnt, 0, sizeof(cnt));
        for (i = 0; i < w * h; i++)
            cnt[b[528 + i]]++;
        for (i = 0; i < 256; i++)
            if (key0 < 0 || cnt[i] > cnt[key0])
                key0 = i;
        for (i = 0; i < 256; i++)
            if (i != key0 && (key1 < 0 || cnt[i] > cnt[key1]))
                key1 = i;
        im->w = LOGO_W;
        im->h = LOGO_H;
    } else {
        im->w = w;
        im->h = h;
    }
    im->tw = pow2(im->w);
    im->th = pow2(im->h);
    im->pix = (u16 *)calloc((size_t)im->tw * im->th, 2);
    for (y = 0; y < im->h; y++)
        for (x = 0; x < im->w; x++) {
            int sx = idx == 7 ? x + LOGO_X0 : x, sy = idx == 7 ? y + LOGO_Y0 : y;
            int p = b[528 + sy * w + sx];
            u16 c = pal[p];
            if (p == 0 || p == key0 || p == key1)
                continue;
            im->pix[y * im->tw + x] = (u16)(0x8000 | (c & 0x1f) << 10 | (c & 0x3e0) | (c >> 10 & 0x1f));
        }
}

/* ------------------------------------------------------------ backend */
#ifdef _arch_dreamcast
static pvr_ptr_t upload(const u16 *pix, int w, int h) {
    pvr_ptr_t p = pvr_mem_malloc((size_t)w * h * 2);
    if (p)
        pvr_txr_load(pix, p, (size_t)w * h * 2);
    return p;
}
/* texture-format word of the sprite quads being drawn (sprres_tex: PAL4 + palette
 * row of the image); 0 = ARGB1555 non-twiddled (BG images, sprites without palettes) */
static int s_spr_fmt;
static void quad(void *tex, int tw, int th, int list, float x0, float y0, float x1, float y1, float u0, float v0,
                 float u1, float v1, float z, u32 argb, u32 oargb) {
    pvr_poly_cxt_t cxt;
    pvr_poly_hdr_t hdr;
    pvr_vertex_t v;
    if (tex) {
        pvr_poly_cxt_txr(&cxt, (pvr_list_t)list, s_spr_fmt ? s_spr_fmt : PVR_TXRFMT_ARGB1555 | PVR_TXRFMT_NONTWIDDLED,
                         tw, th, (pvr_ptr_t)tex, PVR_FILTER_NONE);
        u0 /= tw;
        u1 /= tw;
        v0 /= th;
        v1 /= th;
    } else
        pvr_poly_cxt_col(&cxt, (pvr_list_t)list);
    if (oargb)
        cxt.gen.specular = PVR_SPECULAR_ENABLE;
    pvr_poly_compile(&hdr, &cxt);
    pvr_prim(&hdr, sizeof(hdr));
    v.argb = argb;
    v.oargb = oargb;
    v.z = z;
    v.flags = PVR_CMD_VERTEX;
    v.x = x0; v.y = y1; v.u = u0; v.v = v1; pvr_prim(&v, sizeof(v));
    v.x = x0; v.y = y0; v.u = u0; v.v = v0; pvr_prim(&v, sizeof(v));
    v.x = x1; v.y = y1; v.u = u1; v.v = v1; pvr_prim(&v, sizeof(v));
    v.flags = PVR_CMD_VERTEX_EOL;
    v.x = x1; v.y = y0; v.u = u1; v.v = v0; pvr_prim(&v, sizeof(v));
}
static void quad3d(void *tex, int tw, int th, int list, const float *xyz, float u0, float v0, float u1, float v1,
                   u32 argb) {
    pvr_poly_cxt_t cxt;
    pvr_poly_hdr_t hdr;
    pvr_vertex_t v;
    int i;
    static const int order[4] = {3, 0, 2, 1};   /* bl tl br tr */
    const float uv[4][2] = {{u0, v0}, {u1, v0}, {u1, v1}, {u0, v1}};
    pvr_poly_cxt_txr(&cxt, (pvr_list_t)list, s_spr_fmt ? s_spr_fmt : PVR_TXRFMT_ARGB1555 | PVR_TXRFMT_NONTWIDDLED,
                     tw, th, (pvr_ptr_t)tex, PVR_FILTER_NONE);
    cxt.gen.culling = PVR_CULLING_NONE;
    pvr_poly_compile(&hdr, &cxt);
    pvr_prim(&hdr, sizeof(hdr));
    for (i = 0; i < 4; i++) {
        int k = order[i];
        v.flags = i == 3 ? PVR_CMD_VERTEX_EOL : PVR_CMD_VERTEX;
        v.x = xyz[k * 3];
        v.y = xyz[k * 3 + 1];
        v.z = xyz[k * 3 + 2];
        v.u = uv[k][0] / tw;
        v.v = uv[k][1] / th;
        v.argb = argb;
        v.oargb = 0;
        pvr_prim(&v, sizeof(v));
    }
}
#define LIST_OP PVR_LIST_OP_POLY
#define LIST_PT PVR_LIST_PT_POLY
#define LIST_TR PVR_LIST_TR_POLY
#else
static void *upload(const u16 *pix, int w, int h) {
    (void)w;
    (void)h;
    return (void *)pix;
}
#define LIST_OP 0
#define LIST_TR 2
#define LIST_PT 4
/* host: nearest-neighbour quad into the 640x480 RGB test frame buffer */
static void quad(void *tex, int tw, int th, int list, float x0, float y0, float x1, float y1, float u0, float v0,
                 float u1, float v1, float z, u32 argb, u32 oargb) {
    int x, y, X0, Y0, X1, Y1;
    float a = ((argb >> 24) & 255) / 255.0f, wb = ((oargb >> 16) & 255) / 255.0f;
    (void)th;
    (void)list;
    (void)z;
    if (!g_sp_host_fb)
        return;
    X0 = (int)(x0 < x1 ? x0 : x1);
    X1 = (int)(x0 < x1 ? x1 : x0);
    Y0 = (int)(y0 < y1 ? y0 : y1);
    Y1 = (int)(y0 < y1 ? y1 : y0);
    for (y = Y0 < 0 ? 0 : Y0; y < Y1 && y < 480; y++)
        for (x = X0 < 0 ? 0 : X0; x < X1 && x < 640; x++) {
            float r, g, b;
            u8 *d = g_sp_host_fb + (y * 640 + x) * 3;
            if (tex) {
                float fu = u0 + (u1 - u0) * ((x + 0.5f - x0) / (x1 - x0));
                float fv = v0 + (v1 - v0) * ((y + 0.5f - y0) / (y1 - y0));
                int iu = (int)fu, iv = (int)fv;
                u16 p;
                if (iu < 0 || iv < 0 || iu >= tw)
                    continue;
                p = ((const u16 *)tex)[iv * tw + iu];
                if (!(p & 0x8000))
                    continue;
                r = ((p >> 10) & 31) / 31.0f;
                g = ((p >> 5) & 31) / 31.0f;
                b = (p & 31) / 31.0f;
            } else {
                r = ((argb >> 16) & 255) / 255.0f;
                g = ((argb >> 8) & 255) / 255.0f;
                b = (argb & 255) / 255.0f;
            }
            r += (1 - r) * wb;
            g += (1 - g) * wb;
            b += (1 - b) * wb;
            d[0] = (u8)(d[0] * (1 - a) + r * 255 * a);
            d[1] = (u8)(d[1] * (1 - a) + g * 255 * a);
            d[2] = (u8)(d[2] * (1 - a) + b * 255 * a);
        }
}
static void quad3d(void *tex, int tw, int th, int list, const float *xyz, float u0, float v0, float u1, float v1,
                   u32 argb) {
    float minx = xyz[0], maxx = xyz[0], miny = xyz[1], maxy = xyz[1];
    int i;
    for (i = 1; i < 4; i++) {
        if (xyz[i * 3] < minx) minx = xyz[i * 3];
        if (xyz[i * 3] > maxx) maxx = xyz[i * 3];
        if (xyz[i * 3 + 1] < miny) miny = xyz[i * 3 + 1];
        if (xyz[i * 3 + 1] > maxy) maxy = xyz[i * 3 + 1];
    }
    quad(tex, tw, th, list, minx, miny, maxx, maxy, u0, v0, u1, v1, 0, argb, 0);
}
#endif

#ifdef _arch_dreamcast
static void s_spr_fmt_clear(void) { s_spr_fmt = 0; }
#else
static void s_spr_fmt_clear(void) {}
#endif
/* the page texture for drawing image `image` (Dreamcast: sets s_spr_fmt) */
static void *sprres_tex(const SpSprRes *r, int image) {
    SpSprRes *w = (SpSprRes *)r;
#ifdef _arch_dreamcast
    s_spr_fmt = 0;
    if (w->atlas) {
        SprAtlas *a = (SprAtlas *)w->atlas;
        int bank = spr_atlas_bank(a, image, -1);
        if (!a->tex[0].ptr)
            return NULL;
        s_spr_fmt = tex_txr_format(&a->tex[0], bank);
        return a->tex[0].ptr;
    }
#else
    (void)image;
#endif
    if (!w->tex && w->pixels)
        w->tex = upload(w->pixels, w->page_w, w->page_h);
    return w->tex;
}

static void *bg_tex(int idx) {
    SpImg *im = &s_bgimg[idx];
    bg_image(idx);
    if (!im->tex && im->pix)
        im->tex = upload(im->pix, im->tw, im->th);
    return im->tex;
}

int sp_render_init(void) {
    memset(s_bgimg, 0, sizeof(s_bgimg));
    memset(s_sprimg, 0, sizeof(s_sprimg));
#ifdef _arch_dreamcast
    g_sp_upload_textures = 1;
#endif
    s_inited = 1;
    return 0;
}

static void free_sprres_tex(SpSprRes *r) {
#ifdef _arch_dreamcast
    if (r && r->tex)
        pvr_mem_free((pvr_ptr_t)r->tex);
#endif
    if (r)
        r->tex = NULL;
}

void sp_render_shutdown(void) {
    int i;
    for (i = 0; i < 8; i++) {
#ifdef _arch_dreamcast
        if (s_bgimg[i].tex)
            pvr_mem_free((pvr_ptr_t)s_bgimg[i].tex);
#endif
        free(s_bgimg[i].pix);
        memset(&s_bgimg[i], 0, sizeof(s_bgimg[i]));
    }
    if (data_ov001_0231f5c4)
        for (i = 0; i < 5; i++)
            free_sprres_tex(&data_ov001_0231f5c4->spr[i]);
    if (gSpecialStageGimmickModels)
        free_sprres_tex(gSpecialStageGimmickModels->unk0);
    s_inited = 0;
}

/* ------------------------------------------------------------ 3D */
static dcm_camera_t s_cam;
static dcm_lights_t s_lights[2];

static void make_lights(void) {
    static const s32 dir[3][3] = {{0, -0x1000, 0}, {0xec7, -0x600, 0}, {0, 0x620, 0xec7}};
    static const u16 col[2][3] = {{0x7fff, 0x4631, 0x2108}, {0x6182, 0x6182, 0x6182}};
    int m, i, k;
    for (m = 0; m < 2; m++) {
        s_lights[m].count = 3;
        for (i = 0; i < 3; i++) {
            float d[3] = {dir[i][0] / 4096.0f, dir[i][1] / 4096.0f, dir[i][2] / 4096.0f};
            for (k = 0; k < 3; k++)
                s_lights[m].dir[i][k] = d[0] * s_cam.view.m[0][k] + d[1] * s_cam.view.m[1][k] + d[2] * s_cam.view.m[2][k];
            s_lights[m].col[i][0] = (col[m][i] & 31) / 31.0f;
            s_lights[m].col[i][1] = ((col[m][i] >> 5) & 31) / 31.0f;
            s_lights[m].col[i][2] = ((col[m][i] >> 10) & 31) / 31.0f;
        }
    }
}

static void project(const dcm_mtx_t *W, float x, float y, float *out) {
    float wx = x * W->m[0][0] + y * W->m[1][0] + W->m[3][0];
    float wy = x * W->m[0][1] + y * W->m[1][1] + W->m[3][1];
    float wz = x * W->m[0][2] + y * W->m[1][2] + W->m[3][2];
    const dcm_mtx_t *V = &s_cam.view;
    float ex = wx * V->m[0][0] + wy * V->m[1][0] + wz * V->m[2][0] + V->m[3][0];
    float ey = wx * V->m[0][1] + wy * V->m[1][1] + wz * V->m[2][1] + V->m[3][1];
    float ez = wx * V->m[0][2] + wy * V->m[1][2] + wz * V->m[2][2] + V->m[3][2];
    float cw = -ez, iw = cw > 0.001f ? 1.0f / cw : 0.0f;
    out[0] = (ex * s_cam.proj[0][0] * iw + 1.0f) * 320.0f;
    out[1] = (1.0f - ey * s_cam.proj[1][1] * iw) * 240.0f;
    out[2] = cw > 0.001f ? iw : -1.0f;
}

/* the material colour / texture pattern / texture SRT animation objects of slots 1..3 (cross-fade
 * source first) at their integer frames: the material values NNSi_G3dFuncSbc_MAT_InternalDefault
 * builds (MODELS.md 4.1); NULL when the model has none */
u32 g_sp_matanim_draws;   /* draws with material animations (host tests) */
static const dcm_matstate_t *matstates(const SpModel *m, const dcm_model_t *md) {
    static dcm_matstate_t st[64];
    int slot, k, any = 0;
    if (!md->matx || md->hdr->num_materials > 64)
        return NULL;
    for (slot = 1; slot <= 3; slot++)
        for (k = 0; k < 2; k++) {
            const SpAnmObj *a = k == 0 ? m->anm2[slot] : m->anm[slot];
            if (!a || !a->res || !a->res->set.hdr || a->res->set.hdr->kind < DCA_KIND_MATCOL)
                continue;
            if (!any) {
                dcm_matstate_init(md, st);
                any = 1;
            }
            dcm_matanim_apply(&a->res->set, a->idx, (int)(a->frame >> 12), md, st);
        }
    if (any)
        g_sp_matanim_draws++;
    return any ? st : NULL;
}

static void draw_model_cmd(const SpCmd *c, int list) {
    static dcm_local_t loc[DCM_MAX_JOINTS];
    static dcm_mtx_t pal[DCM_MAX_PALETTE];
    static u8 vis[DCM_MAX_JOINTS];
    const SpModel *m = c->m;
    const dcm_model_t *md;
    dcm_draw_opts_t o;
    const u8 *vp = NULL;
    const dcm_matstate_t *ms;
    if (!c->res || !c->res->loaded)
        return;
    md = &c->res->mdl;
    if (m->anm[0] && m->anm[0]->res && m->anm[0]->res->kind == 0 && m->anm[0]->res->loaded)
        dca_sample(&m->anm[0]->res->set, m->anm[0]->idx, (float)(m->anm[0]->frame >> 12), md, loc);
    else
        dcm_bind_locals(md, loc);
    dcm_pose(md, loc, pal);
    if (m->anm[4] && m->anm[4]->res && m->anm[4]->res->kind == 1 && m->anm[4]->res->loaded) {
        dca_visibility(&m->anm[4]->res->set, m->anm[4]->idx, (float)(m->anm[4]->frame >> 12), md, vis);
        vp = vis;
    }
    ms = matstates(m, md);
    o.list = list;
    o.pt_list = LIST_PT;
    o.alpha_scale = 255;
#ifdef _arch_dreamcast
    dcm_draw_ex(md, pal, &c->world, &s_cam, vp, &s_lights[c->light ? 1 : 0], &o, ms);
#else
    if (list == LIST_OP && g_sp_host_fb) {
        static float xyz[3 * 8192];
        int n = md->hdr->num_vertices < 8192 ? (int)md->hdr->num_vertices : 8192, i;
        (void)vp;
        (void)ms;
        dcm_transform_vertices(md, pal, &c->world, &s_cam, xyz);
        for (i = 0; i < n; i++) {
            int x = (int)xyz[i * 3], y = (int)xyz[i * 3 + 1];
            if (xyz[i * 3 + 2] > 0 && x >= 0 && y >= 0 && x < 640 && y < 480) {
                u8 *d = g_sp_host_fb + (y * 640 + x) * 3;
                d[0] = 255;
                d[1] = (u8)(c->light ? 64 : 200);
                d[2] = 64;
            }
        }
    }
    (void)o;
#endif
}

static void draw_sprite3d_cmd(const SpCmd *c) {
    const SpDsprImage *im;
    float xyz[12], ox, oy;
    int flip = 0, k;
    void *tex;
    if (c->image < 0 || !c->sres || !c->sres->loaded)
        return;
    im = &c->sres->images[c->image];
    tex = sprres_tex(c->sres, c->image);
    if (!tex)
        return;
    ox = im->ox;
    oy = im->oy;
    if (c->sflags & 0x80) {
        flip |= 1;
        ox = im->w - ox;
    }
    /* model space: x right, y up; corners tl tr br bl */
    project(&c->world, -ox, oy, &xyz[0]);
    project(&c->world, im->w - ox, oy, &xyz[3]);
    project(&c->world, im->w - ox, oy - im->h, &xyz[6]);
    project(&c->world, -ox, oy - im->h, &xyz[9]);
    for (k = 0; k < 4; k++)
        if (xyz[k * 3 + 2] <= 0) {
            s_spr_fmt_clear();
            return;
        }
    if (flip & 1)
        quad3d(tex, c->sres->page_w, c->sres->page_h, LIST_PT, xyz, im->x + im->w, im->y, im->x, im->y + im->h,
               0xffffffff);
    else
        quad3d(tex, c->sres->page_w, c->sres->page_h, LIST_PT, xyz, im->x, im->y, im->x + im->w, im->y + im->h,
               0xffffffff);
    s_spr_fmt_clear();
}

/* ------------------------------------------------------------ 2D */
static float s_z;
static u32 white_offset(int bright) {
    u32 v;
    if (bright <= 0)
        return 0;
    if (bright > 16)
        bright = 16;
    v = (u32)(bright * 255 / 16);
    return 0xff000000u | v << 16 | v << 8 | v;
}

static void draw_sprite2d_cmd(const SpCmd *c, int bright) {
    const SpDsprImage *im;
    float x0, y0, x1, y1;
    int flip;
    void *tex;
    if (c->image < 0 || !c->sres || !c->sres->loaded)
        return;
    im = &c->sres->images[c->image];
    tex = sprres_tex(c->sres, c->image);
    sprite_rect(c, &x0, &y0, &x1, &y1, &flip);
    if (flip & 1) {
        float t = x0;
        x0 = x1;
        x1 = t;
    }
    if (flip & 2) {
        float t = y0;
        y0 = y1;
        y1 = t;
    }
    {
        /* the course progress bar (top screen) lies over the top row of the Trick Spring targets of
         * the single view: see-through while a sequence is open */
        SpObj *p = gSpecialStageWork ? gSpecialStageWork->unk18 : NULL;
        u32 argb = (c->group == GRP_BAR && p && p->unk128) ? 0x50ffffffu : 0xffffffffu;
        quad(tex, c->sres->page_w, c->sres->page_h, LIST_TR, x0, y0, x1, y1, im->x, im->y, im->x + im->w,
             im->y + im->h, s_z, argb, white_offset(bright));
    }
    s_z += 0.01f;
    if (c->f > 0.0f && gSpecialStageWork) {
        /* palette slot 8 lerp: the emerald in this stage's colour (anim 0x13 + stage) over it */
        const SpSprRes *r = c->sres;
        int a = 0x13 + SpecialStage_GetStageIndex();
        if (a < r->n_anims) {
            int img = r->frametab[r->anims[a].first_frame * 2];
            if (img != 0xffff) {
                const SpDsprImage *i2 = &r->images[img];
                tex = sprres_tex(r, img);
                quad(tex, r->page_w, r->page_h, LIST_TR, x0, y0, x1, y1, i2->x, i2->y, i2->x + i2->w, i2->y + i2->h,
                     s_z, ((u32)(c->f * 255.0f) << 24) | 0xffffff, white_offset(bright));
                s_z += 0.01f;
            }
        }
    }
    s_spr_fmt_clear();
}

/* BG layer of 256 x h at x2.5 with the BG scroll (wraps horizontally) */
static void draw_bg_layer(int idx, float scrollx, float scrolly, float alpha, int bright, int list) {
    SpImg *im;
    void *tex = bg_tex(idx);
    float ox, oy, y0;
    int k;
    im = &s_bgimg[idx];
    if (!tex)
        return;
    ox = -(float)(((int)scrollx % 256 + 256) % 256);
    oy = -scrolly;
    y0 = oy * 2.5f;
    for (k = 0; k < 2; k++) {
        float x0 = (ox + k * 256) * 2.5f;
        quad(tex, im->tw, im->th, list, x0, y0, x0 + im->w * 2.5f, y0 + im->h * 2.5f, 0, 0, (float)im->w,
             (float)im->h, s_z, ((u32)(alpha * 255.0f) << 24) | 0xffffff, white_offset(bright));
        if (ox == 0)
            break;
    }
    s_z += 0.01f;
}

static int cmp_sprite(const void *a, const void *b) {
    const SpCmd *x = *(const SpCmd *const *)a, *y = *(const SpCmd *const *)b;
    /* back to front: higher OAM priority value first, then later draws first (OAM: first = front) */
    if (x->prio != y->prio)
        return y->prio - x->prio;
    return y->seq - x->seq;
}

static void render_2d(void) {
    static const SpCmd *main_s[MAX_CMDS], *sub_s[MAX_CMDS];
    int nm = 0, ns = 0, i;
    SpHud *h = gSpecialStageHud;
    int card = h && h->unkBD4 >= 0;
    float card_a;
    for (i = 0; i < s_ncmd; i++) {
        if (s_cmd[i].kind != SPD_SPRITE2D)
            continue;
        if (s_cmd[i].group == GRP_MAIN)
            main_s[nm++] = &s_cmd[i];
        else
            sub_s[ns++] = &s_cmd[i];
    }
    qsort(main_s, (size_t)nm, sizeof(main_s[0]), cmp_sprite);
    qsort(sub_s, (size_t)ns, sizeof(sub_s[0]), cmp_sprite);
    /* main BG1 (intro caption / sp_fix_touch steering guide, blended) */
    /* (sp_fix_touch, res 4, is the stylus guide of the start sequence: not drawn with the
     * controller scheme, see draw_controller_overlay) */
    if ((gSpDisplayMain.bg_enable & 2) && g_sp_layer_main[1] >= 0 && g_sp_layer_main[1] != 4) {
        float a = 1.0f;
        if (gSpDisplayMain.blend_on && gSpecialStageWork && gSpecialStageWork->taskC != 0)
            a = gSpDisplayMain.eva / 16.0f;
        draw_bg_layer(g_sp_layer_main[1], gSpDisplayMain.bg[1].x, gSpDisplayMain.bg[1].y, a, 0, LIST_TR);
    }
    /* main-screen sprites (captions, Go!/START!, trick circles, ratings) */
    for (i = 0; i < nm; i++)
        draw_sprite2d_cmd(main_s[i], 0);
    /* top screen: results card / HUD groups (the intro logo is part of the backdrop, PT list) */
    card_a = 0;
    if (card) {
        card_a = (16 - gSpDisplaySub.bright) / 16.0f;
        if (h->unkBD4 > 0x32)
            card_a = 1.0f;
        if (card_a < 0)
            card_a = 0;
        /* cldm_base (sub BG2) and the scrolling caption (sub BG1) as the full view */
        if (g_sp_layer_sub[2] >= 0)
            draw_bg_layer(g_sp_layer_sub[2], gSpDisplaySub.bg[2].x, gSpDisplaySub.bg[2].y, card_a, 0, LIST_TR);
        if ((gSpDisplaySub.bg_enable & 2) && g_sp_layer_sub[1] >= 0)
            draw_bg_layer(g_sp_layer_sub[1], gSpDisplaySub.bg[1].x, gSpDisplaySub.bg[1].y, card_a, 0, LIST_TR);
    }
    for (i = 0; i < ns; i++)
        draw_sprite2d_cmd(sub_s[i], card ? 0 : gSpDisplaySub.bright);
}

/* ------------------------------------------------------------ controller overlays
 * (Dreamcast controller scheme, SPECIAL.md section 4; art: ac_spst_pad anims 70.., caption-bar
 * font lines, tools/padart.py)
 *  - the legend card at the intro prompt of the first special stage of the session;
 *  - the pause card (the PauseMenu of the original: PAUSE, the controls, CONTINUE, GO BACK
 *    when this emerald is already owned);
 *  - the steering hint of the start sequence, where the original shows the stylus over the
 *    sp_fix_touch arc (HUD frames 0x17c..0x258). */
enum { PADL_CONTROLS, PADL_MOVE, PADL_ATTACK, PADL_TRICK, PADL_BUTTONS, PADL_PAUSE, PADL_BEGIN, PADL_PAUSED,
       PADL_CONTINUE, PADL_BACK };

static int pad_line_image(const SpSprRes *r, int line) {
    int a = 70 + line;
    u16 im;
    if (!r || !r->loaded || a >= r->n_anims || r->anims[a].frame_count == 0)
        return -1;
    im = r->frametab[r->anims[a].first_frame * 2];
    return im == 0xffff || im >= r->n_images ? -1 : im;
}

static float pad_line_w(const SpSprRes *r, int line) {
    int im = pad_line_image(r, line);
    return im < 0 ? 0 : r->images[im].w;
}

static void pad_line(const SpSprRes *r, int line, float cx, float y, float k, float z) {
    int im = pad_line_image(r, line);
    const SpDsprImage *i;
    void *tex;
    float x0;
    if (im < 0)
        return;
    i = &r->images[im];
    tex = sprres_tex(r, im);
    x0 = cx - i->w * k / 2;
    quad(tex, r->page_w, r->page_h, LIST_TR, x0, y, x0 + i->w * k, y + i->h * k, i->x, i->y, i->x + i->w,
         i->y + i->h, z, 0xffffffff, 0);
    s_spr_fmt_clear();
}

/* black card with a white rim (the caption bar's colours) */
static void pad_panel(float x0, float y0, float x1, float y1, float z) {
    quad(NULL, 0, 0, LIST_TR, x0 - 3, y0 - 3, x1 + 3, y1 + 3, 0, 0, 0, 0, z, 0xffffffffu, 0);
    quad(NULL, 0, 0, LIST_TR, x0, y0, x1, y1, 0, 0, 0, 0, z + 0.5f, 0xff000000u, 0);
}

static void pad_card(const SpSprRes *r, const int *lines, int n, float z) {
    const float k = 2.0f, lh = 20 * k, gap = 4;
    float w = 0, h = n * lh + (n - 1) * gap, y;
    int i;
    for (i = 0; i < n; i++)
        if (pad_line_w(r, lines[i]) > w)
            w = pad_line_w(r, lines[i]);
    w = w * k + 48;
    y = (480 - h) / 2;
    pad_panel(320 - w / 2, y - 16, 320 + w / 2, y + h + 16, z);
    for (i = 0; i < n; i++) {
        pad_line(r, lines[i], 320, y, k, z + 1 + i * 0.01f);
        y += lh + gap;
    }
}

static void draw_controller_overlay(void) {
    SpWork *g = gSpecialStageWork;
    SpHud *h = gSpecialStageHud;
    const SpSprRes *r = data_ov001_0231f5c4 ? &data_ov001_0231f5c4->spr[4] : NULL;
    if (!g || !r || !r->loaded)
        return;
    if (special_paused()) {
        int lines[8], n = 0;
        lines[n++] = PADL_PAUSED;
        lines[n++] = PADL_MOVE;
        lines[n++] = PADL_ATTACK;
        lines[n++] = PADL_TRICK;
        lines[n++] = PADL_BUTTONS;
        lines[n++] = PADL_CONTINUE;
        if (g->unk8 & (1u << g->unk4))
            lines[n++] = PADL_BACK;
        pad_card(r, lines, n, 810.0f);
        return;
    }
    if (g->taskC == 0 && g->timer == 0x5c && sp_legend_pending()) {
        static const int lines[7] = {PADL_CONTROLS, PADL_MOVE, PADL_ATTACK, PADL_TRICK, PADL_BUTTONS, PADL_PAUSE,
                                     PADL_BEGIN};
        quad(NULL, 0, 0, LIST_TR, 0, 0, 640, 480, 0, 0, 0, 0, 805.0f, 0x80000000u, 0);
        pad_card(r, lines, 7, 810.0f);
        return;
    }
    if (g->taskC == 1 && h && h->unkBD4 < 0 && h->unkAE0 >= 0x17c && h->unkAE0 < 0x258) {
        float w = pad_line_w(r, PADL_MOVE) * 2.0f + 24, y = 0x6e * 2.5f - 20;
        pad_panel(320 - w / 2, y - 6, 320 + w / 2, y + 46, 790.0f);
        pad_line(r, PADL_MOVE, 320, y, 2.0f, 791.0f);
    }
}

/* ------------------------------------------------------------ frame */
void sp_render_frame(void) {
    int i;
    void *bbtex = NULL;
    if (!gSpecialStageWork)
        return;
    sp_camera_dcm(&s_cam);
    make_lights();
    s_z = 2.0f;
#ifndef _arch_dreamcast
    if (g_sp_host_fb)
        memset(g_sp_host_fb, 0, 640 * 480 * 3);
#endif
    /* ---- OP: backdrop (main BG2: sp_stdm_base_b in the intro) + opaque 3D */
#ifdef _arch_dreamcast
    pvr_list_begin(PVR_LIST_OP_POLY);
#endif
    if ((gSpDisplayMain.bg_enable & 4) && g_sp_layer_main[2] >= 0) {
        bbtex = bg_tex(g_sp_layer_main[2]);
        if (bbtex)
            quad(bbtex, s_bgimg[g_sp_layer_main[2]].tw, s_bgimg[g_sp_layer_main[2]].th, LIST_OP, 0, 0, 640, 480, 0, 0,
                 256, 192, 0.0001f, 0xffffffff, 0);
    }
    for (i = 0; i < s_ncmd; i++)
        if (s_cmd[i].kind == SPD_MODEL)
            draw_model_cmd(&s_cmd[i], LIST_OP);
#ifdef _arch_dreamcast
    pvr_list_finish();
    /* ---- PT: punch-through 3D (alpha-tested textures) and the 3D sprites (rings, enemies, effects) */
    pvr_list_begin(PVR_LIST_PT_POLY);
#endif
    if (gSpecialStageWork->taskC == 0 && g_sp_layer_sub[2] == 0) {
        /* the top screen of the intro: "SPECIAL STAGE" (sp_stdm_base_a, stripes keyed out) on the
         * backdrop, behind the 3D Sonic */
        void *tex = bg_tex(7);
        if (tex) {
            float w = LOGO_W * 1.25f, hh = LOGO_H * 1.25f, x0 = 640 - 12 - w, y0 = 124;
            quad(tex, s_bgimg[7].tw, s_bgimg[7].th, LIST_PT, x0, y0, x0 + w, y0 + hh, 0, 0, LOGO_W, LOGO_H, 0.0002f,
                 0xffffffff, white_offset(gSpDisplaySub.bright));
        }
    }
    for (i = 0; i < s_ncmd; i++) {
        if (s_cmd[i].kind == SPD_MODEL)
            draw_model_cmd(&s_cmd[i], LIST_PT);
        else if (s_cmd[i].kind == SPD_SPRITE2D + 100)
            draw_sprite3d_cmd(&s_cmd[i]);
    }
#ifdef _arch_dreamcast
    pvr_list_finish();
    /* ---- TR: translucent 3D, fog, the white fade quad, 2D layers and sprites, brightness */
    pvr_list_begin(PVR_LIST_TR_POLY);
#endif
    for (i = 0; i < s_ncmd; i++)
        if (s_cmd[i].kind == SPD_MODEL)
            draw_model_cmd(&s_cmd[i], LIST_TR);
    if (g_sp_fog > 0)   /* fog colour 0x7fff, density = table value / 127 over the whole 3D */
        quad(NULL, 0, 0, LIST_TR, 0, 0, 640, 480, 0, 0, 0, 0, 1.5f, ((u32)(g_sp_fog * 255 / 127) << 24) | 0xffffff, 0);
    for (i = 0; i < s_ncmd; i++) {
        if (s_cmd[i].kind == SPD_FADEQUAD)   /* 3D white quad (alpha n/31) in front of the camera */
            quad(NULL, 0, 0, LIST_TR, 0, 0, 640, 480, 0, 0, 0, 0, 1.6f, ((u32)(s_cmd[i].a * 255 / 31) << 24) | 0xffffff, 0);
        else if (s_cmd[i].kind == SPD_RATINGBAR) {
            /* SpecialStageHud_Unk_ov001_02308884: ortho quads x 32..224, y 176..184 (DS px) */
            u16 col = sp_rom_u16(0x02317090 + (u32)s_cmd[i].a * 2);
            u32 argb;
            float x1 = (512.0f + 786432.0f * s_cmd[i].f / 256.0f) / 16.0f;
            if (s_cmd[i].b)
                col = sp_rom_u16(0x02317090 + 4);
            argb = 0xff000000u | (u32)((col & 31) * 255 / 31) << 16 | (u32)(((col >> 5) & 31) * 255 / 31) << 8 |
                   (u32)(((col >> 10) & 31) * 255 / 31);
            quad(NULL, 0, 0, LIST_TR, 32 * 2.5f, 176 * 2.5f, 224 * 2.5f, 184 * 2.5f, 0, 0, 0, 0, 1.7f, 0xff000000u, 0);
            quad(NULL, 0, 0, LIST_TR, 32 * 2.5f, 176 * 2.5f, x1 * 2.5f, 184 * 2.5f, 0, 0, 0, 0, 1.71f, argb, 0);
        }
    }
    render_2d();
    if (special_paused())   /* the frozen frame, dimmed; the pause card (draw_controller_overlay) over it */
        quad(NULL, 0, 0, LIST_TR, 0, 0, 640, 480, 0, 0, 0, 0, 800.0f, 0x80000000u, 0);
    draw_controller_overlay();
    if (gSpDisplayMain.bright != 0) {
        int b = gSpDisplayMain.bright < 0 ? -gSpDisplayMain.bright : gSpDisplayMain.bright;
        u32 rgb = gSpDisplayMain.bright > 0 ? 0xffffff : 0;
        if (b > 16)
            b = 16;
        quad(NULL, 0, 0, LIST_TR, 0, 0, 640, 480, 0, 0, 0, 0, 900.0f, ((u32)(b * 255 / 16) << 24) | rgb, 0);
    }
#ifdef _arch_dreamcast
    pvr_list_finish();
#endif
}

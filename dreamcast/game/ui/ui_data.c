/*
 * UI resources and the draw-command list (portable: host and KOS).
 *
 * - uidata.bin ("RUID", tools/uidata.py): ROM tables at the addresses the
 *   decompiled code reads (see ui_internal.h UiData).
 * - Sprite resources: converted BAC frame tables (frames.bin "DSPR" v1) and their
 *   atlases; UiSpr is the part of AnimatorSprite the UI uses (frame timing:
 *   wait = duration << 4, speed 0x10 per frame; EndAnimation loops to the loop
 *   frame when the sprite loops, otherwise it stops on the last frame).
 * - Draw commands: AnimatorSprite_Draw / _DrawRotScale calls become screen quads
 *   (UiQuad) in one of the layout spaces; ui_draw.c submits them to the PVR.
 */
#include <stdio.h>
#include <stdlib.h>
#include "ui_internal.h"
#ifdef _arch_dreamcast
#include "../engine/pak.h"   /* /cd/data.pak: per-stage files without a directory walk each */
#endif

UiRes g_ui_res[UI_RES_COUNT];
UiData g_ui_data;
UiNaviData g_ui_navi;
int g_ui_screen;
char g_ui_dir[96] = "/cd/ui";
char g_ui_sprite_dir[96] = "/cd/sprites";

int ui_load_file(const char *path, u8 **out, u32 *size) {
    FILE *f;
    long n;
    u8 *b;
    *out = NULL;
    *size = 0;
#ifdef _arch_dreamcast
    {   /* /cd/data.pak (engine/pak.h) */
        void *pb;
        u32 ps;
        int k = pak_load(path, &pb, &ps);
        if (k == PAK_FOUND) {
            *out = (u8 *)pb;
            *size = ps;
            return 0;
        }
        if (k == PAK_MISSING)
            return -1;
    }
#endif
    f = fopen(path, "rb");
    if (!f)
        return -1;
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (n <= 0) {
        fclose(f);
        return -1;
    }
    b = (u8 *)malloc((size_t)n);
    if (!b || fread(b, 1, (size_t)n, f) != (size_t)n) {
        free(b);
        fclose(f);
        return -1;
    }
    fclose(f);
    *out = b;
    *size = (u32)n;
    return 0;
}

/* ------------------------------------------------------------- uidata.bin */
static u16 rd16(const u8 *p) { return (u16)(p[0] | p[1] << 8); }
static u32 rd32(const u8 *p) { return (u32)p[0] | (u32)p[1] << 8 | (u32)p[2] << 16 | (u32)p[3] << 24; }

static int sect(const u8 *buf, u32 size, const char *name, void *dst, u32 want) {
    u32 n = rd16(buf + 6), i;
    for (i = 0; i < n; i++) {
        const u8 *d = buf + 8 + 20 * i;
        if (strncmp((const char *)d, name, 12) == 0) {
            u32 off = rd32(d + 12), sz = rd32(d + 16);
            if (sz != want || off + sz > size)
                return -1;
            memcpy(dst, buf + off, sz); /* little-endian host and SH-4 */
            return 0;
        }
    }
    return -1;
}

int ui_data_load(const u8 *buf, u32 size) {
    UiData *d = &g_ui_data;
    int e = 0;
    memset(d, 0, sizeof(*d));
    if (size < 8 || memcmp(buf, "RUID", 4) != 0 || rd16(buf + 4) != 1)
        return -1;
#define S(n, f) e |= sect(buf, size, n, &d->f, sizeof(d->f))
    S("gauge_pal", gauge_pal);
    S("time_div", time_div);
    S("tb_thr", tb_thr);
    S("tb_norm", tb_norm);
    S("tb_boss", tb_boss);
    S("sb_thr", sb_thr);
    S("sb_bonus", sb_bonus);
    S("rank_boss", rank_boss);
    S("rank_norm", rank_norm);
    S("lbl_boss", lbl_boss);
    S("lbl_norm", lbl_norm);
    S("cd_name_s", cd_name_s);
    S("cd_name_b", cd_name_b);
    S("cd_gw", cd_gw);
    S("cd_actx", cd_actx);
    S("cd_act_ex", cd_act_ex);
    S("cd_act_bs", cd_act_bs);
    S("cd_act_1", cd_act_1);
    S("cd_act_2", cd_act_2);
    S("cd_model", cd_model);
    S("tc_char_a", tc_char_a);
    S("tc_char_b", tc_char_b);
    S("tc_text2", tc_text2);
    S("tc_text1", tc_text1);
    S("tc_lang", tc_lang);
    S("tc_gw", tc_gw);
    S("tc_p180x", tc_p180x);
    S("tc_p180y", tc_p180y);
    S("zone_ord_s", zone_ord[0]);
    S("zone_ord_b", zone_ord[1]);
    S("go_str", go_str);
    S("go_w", go_w);
    S("to_str", to_str);
    S("to_w", to_w);
    S("pause_lut", pause_lut);
    S("shake", shake);
#undef S
    /* time-attack tally tables: optional, added with ClearDemoTA (older uidata.bin
     * files lack them; the tally is then not drawn) */
    d->ta_ok = sect(buf, size, "ta_anim", d->ta_anim, sizeof(d->ta_anim)) == 0 &&
               sect(buf, size, "ta_dx", d->ta_dx, sizeof(d->ta_dx)) == 0 &&
               sect(buf, size, "ta_pal", d->ta_pal, sizeof(d->ta_pal)) == 0;
    d->ok = e == 0;
    return e;
}

/* ----------------------------------------------------------- navidata.bin */
int ui_navi_data_load(const u8 *buf, u32 size) {
    UiNaviData *d = &g_ui_navi;
    int e = 0;
    memset(d, 0, sizeof(*d));
    if (size < 8 || memcmp(buf, "RUID", 4) != 0 || rd16(buf + 4) != 1)
        return -1;
    e |= sect(buf, size, "seq_t", d->seq[0], sizeof(d->seq[0]));
    e |= sect(buf, size, "seq_c", d->seq[1], sizeof(d->seq[1]));
    e |= sect(buf, size, "geo_t", d->geo[0], sizeof(d->geo[0]));
    e |= sect(buf, size, "geo_c", d->geo[1], sizeof(d->geo[1]));
    e |= sect(buf, size, "navi_voice", d->voice, sizeof(d->voice));
    e |= sect(buf, size, "navib_icon", d->navib_icon, sizeof(d->navib_icon));
    e |= sect(buf, size, "navib_pos", d->navib_pos, sizeof(d->navib_pos));
    e |= sect(buf, size, "er_shake", d->er_shake, sizeof(d->er_shake));
    d->ok = e == 0;
    return e;
}

/* ---------------------------------------------------------- sprite resources */
int ui_res_load(int idx, const char *id) { return ui_res_load_dir(idx, g_ui_sprite_dir, id); }

int ui_res_load_dir(int idx, const char *dir, const char *id) {
    UiRes *r = &g_ui_res[idx];
    char path[200];
    u8 *b;
    u32 size, need;
    int i;
    ui_res_free(idx);
    snprintf(r->id, sizeof(r->id), "%s", id);
    snprintf(path, sizeof(path), "%s/%s/frames.bin", dir, id);
    if (ui_load_file(path, &b, &size) != 0)
        return -1;
    if (size < 16 || memcmp(b, "DSPR", 4) != 0 || rd16(b + 4) != 1) {
        free(b);
        return -1;
    }
    r->n_pages = rd16(b + 6);
    r->n_images = rd16(b + 8);
    r->n_anims = rd16(b + 10);
    r->n_frames = rd16(b + 12);
    need = 16 + 4u * r->n_pages + 16u * r->n_images + 12u * r->n_anims + 4u * r->n_frames;
    if (need > size || r->n_pages > 4) {
        free(b);
        return -1;
    }
    r->blob = b;
    r->pages = (const UiPage *)(b + 16);
    r->images = (const UiImage *)(b + 16 + 4 * r->n_pages);
    r->anims = (const UiAnim *)((const u8 *)r->images + 16 * r->n_images);
    r->frames = (const UiFrame *)((const u8 *)r->anims + 12 * r->n_anims);
    /* paletted atlas (palettes.bin + atlas<N>.pt4, SPRITES.md): PAL4 pages stay as
     * 4-bit texels; ARGB1555 pages (direct colour, or no palettes.bin) as pixels */
    snprintf(path, sizeof(path), "%s/%s/palettes.bin", dir, id);
    if (ui_load_file(path, &r->palblob, &size) == 0 &&
        sprpal_parse(&r->pal, r->palblob, size, r->n_pages, r->n_images, r->n_anims) != 0) {
        free(r->palblob);
        r->palblob = NULL;
        memset(&r->pal, 0, sizeof(r->pal));
    }
    for (i = 0; i < r->n_pages; i++) {
        u8 *px;
        u32 psz;
        if (sprpal_page_is_pal4(&r->pal, i)) {
            snprintf(path, sizeof(path), "%s/%s/atlas%d.pt4", dir, id, i);
            if (ui_load_file(path, &px, &psz) == 0) {
                if (psz == (u32)r->pages[i].w * r->pages[i].h / 2) {
                    r->texels[i] = px;
                    continue;
                }
                free(px);
            }
        }
        snprintf(path, sizeof(path), "%s/%s/atlas%d.argb1555", dir, id, i);
        if (ui_load_file(path, &px, &psz) == 0) {
            if (psz == (u32)r->pages[i].w * r->pages[i].h * 2)
                r->pixels[i] = (u16 *)px;
            else
                free(px);
        }
    }
    ui_res_upload(r);
    return 0;
}

u16 ui_res_pixel(const UiRes *r, int image, int x, int y) {
    int pg;
    if (image < 0 || image >= r->n_images)
        return 0;
    pg = r->images[image].page;
    if (pg >= r->n_pages || pg >= 4 || x < 0 || y < 0 || x >= r->pages[pg].w || y >= r->pages[pg].h)
        return 0;
    if (r->pixels[pg])
        return r->pixels[pg][y * r->pages[pg].w + x];
    if (r->texels[pg] && r->pal.image_pal[image] != SPRPAL_NONE)
        return r->pal.pal[16 * r->pal.image_pal[image] +
                          sprpal_texel(r->texels[pg], r->pages[pg].w, r->pages[pg].h, x, y)];
    return 0;
}

void ui_res_free(int idx) {
    UiRes *r = &g_ui_res[idx];
    int i;
    ui_res_release(r);
    for (i = 0; i < 4; i++) {
        free(r->pixels[i]);
        free(r->texels[i]);
        r->pixels[i] = NULL;
        r->texels[i] = NULL;
    }
    free(r->palblob);
    free(r->blob);
    memset(r, 0, sizeof(*r));
}

/* -------------------------------------------------------- AnimatorSprite subset */
static u16 frame_wait(const UiSpr *s) {
    const UiAnim *a = &s->res->anims[s->anim];
    return (u16)(s->res->frames[a->first + s->frame].duration << 4);
}

void UiSpr_SetAnimation(UiSpr *s, u16 anim) {
    s->anim = anim;
    s->frame = 0;
    s->flags &= ~(UI_SPR_ENDED | UI_SPR_STOPPED);
    s->wait = (s->res && s->res->blob && anim < s->res->n_anims) ? frame_wait(s) : 0;
}

void UiSpr_Init(UiSpr *s, int res, u16 anim, u16 flags) {
    memset(s, 0, sizeof(*s));
    s->res = &g_ui_res[res];
    s->speed = 0x10;
    s->flags = flags;
    UiSpr_SetAnimation(s, anim);
}

void UiSpr_ProcessAnimation(UiSpr *s) {
    const UiAnim *a;
    if (!s->res || !s->res->blob || s->anim >= s->res->n_anims)
        return;
    a = &s->res->anims[s->anim];
    if (a->count == 0 || (s->flags & UI_SPR_STOPPED))
        return;
    s->flags &= ~UI_SPR_ENDED;
    s->wait -= s->speed;
    while (s->wait <= 0) {
        if (s->frame + 1 >= a->count) {
            s->flags |= UI_SPR_ENDED;
            if (!(s->flags & UI_SPR_LOOP)) {
                s->flags |= UI_SPR_STOPPED;
                s->wait = 0;
                return;
            }
            s->frame = (u16)(a->loop >= 0 && a->loop < a->count ? a->loop : 0);
        } else {
            s->frame++;
        }
        if (frame_wait(s) == 0) {
            s->wait = 0;
            return;
        }
        s->wait += frame_wait(s);
    }
}

const UiImage *UiSpr_Image(const UiSpr *s, int *index) {
    const UiAnim *a;
    u16 im;
    if (!s->res || !s->res->blob || s->anim >= s->res->n_anims)
        return NULL;
    a = &s->res->anims[s->anim];
    if (a->count == 0)
        return NULL;
    im = s->res->frames[a->first + (s->frame < a->count ? s->frame : a->count - 1)].image;
    if (im == 0xffff || im >= s->res->n_images)
        return NULL;
    if (index)
        *index = im;
    return &s->res->images[im];
}

/* ----------------------------------------------------------------- commands */
#define UI_MAX_QUADS 512
static UiQuad s_quads[UI_MAX_QUADS];
static int s_nquads;
static int s_hud_k = 2, s_safe_x0 = 32, s_safe_y0 = 24, s_safe_x1 = 608, s_safe_y1 = 456;
static float s_cam_x, s_cam_y;
/* UI_SP_NAVI_B .. UI_SP_NAVI_FACE */
static struct { float dx, dy, sx, sy, k; } s_space[3];

void ui_space_set(int space, float dst_x, float dst_y, float src_x, float src_y, float k) {
    int i = space - UI_SP_NAVI_B;
    if (i < 0 || i >= 3)
        return;
    s_space[i].dx = dst_x;
    s_space[i].dy = dst_y;
    s_space[i].sx = src_x;
    s_space[i].sy = src_y;
    s_space[i].k = k;
}
void ui_hud_metrics(int *k, int *x0, int *y0, int *x1, int *y1) {
    *k = s_hud_k;
    *x0 = s_safe_x0;
    *y0 = s_safe_y0;
    *x1 = s_safe_x1;
    *y1 = s_safe_y1;
}

void ui_hud_layout(int scale, int x0, int y0, int x1, int y1) {
    s_hud_k = scale > 0 ? scale : 2;
    s_safe_x0 = x0;
    s_safe_y0 = y0;
    s_safe_x1 = x1;
    s_safe_y1 = y1;
}
void ui_set_camera(float x, float y) {
    s_cam_x = x;
    s_cam_y = y;
}
void ui_cmd_reset(void) { s_nquads = 0; }
static int s_hidden_mark = -1;
void ui_cmd_mark_hidden_begin(void) { s_hidden_mark = s_nquads; }
void ui_cmd_mark_hidden_end(void) {
    if (s_hidden_mark >= 0)
        s_nquads = s_hidden_mark;
    s_hidden_mark = -1;
}
void ui_world_to_screen(s32 wx, s32 wy, s16 *sx, s16 *sy) {
    *sx = (s16)(wx - (s32)s_cam_x);
    *sy = (s16)(wy - (s32)s_cam_y);
}
int ui_debug_quads(const UiQuad **out) {
    *out = s_quads;
    return s_nquads;
}

static void xform(int space, float x, float y, float *ox, float *oy, float *k);
void ui_space_map(int space, float x, float y, float *ox, float *oy, float *k) { xform(space, x, y, ox, oy, k); }
static void xform(int space, float x, float y, float *ox, float *oy, float *k) {
    switch (space) {
    case UI_SP_HUD_TL:
        *k = (float)s_hud_k;
        *ox = s_safe_x0 + x * *k;
        *oy = s_safe_y0 + y * *k;
        break;
    case UI_SP_HUD_BL:
        *k = (float)s_hud_k;
        *ox = s_safe_x0 + x * *k;
        *oy = s_safe_y1 - (192.0f - y) * *k;
        break;
    case UI_SP_HUD_TR:
        *k = (float)s_hud_k;
        *ox = s_safe_x1 - (256.0f - x) * *k;
        *oy = s_safe_y0 + y * *k;
        break;
    case UI_SP_HUD_BR:
        *k = (float)s_hud_k;
        *ox = s_safe_x1 - (256.0f - x) * *k;
        *oy = s_safe_y1 - (192.0f - y) * *k;
        break;
    case UI_SP_HUD_TC:
        /* DS x 128 = screen centre; DS y 0 = under the HUD's top row (24 DS px) */
        *k = (float)s_hud_k;
        *ox = 320.0f + (x - 128.0f) * *k;
        *oy = s_safe_y0 + (24.0f + y) * *k;
        break;
    case UI_SP_NAVI_B:
    case UI_SP_NAVI_MSG:
    case UI_SP_NAVI_FACE: {
        int i = space - UI_SP_NAVI_B;
        *k = s_space[i].k;
        *ox = s_space[i].dx + (x - s_space[i].sx) * *k;
        *oy = s_space[i].dy + (y - s_space[i].sy) * *k;
        break;
    }
    case UI_SP_WORLD:
        *k = 2.5f;
        *ox = (x - s_cam_x) * 2.5f;
        *oy = (y - s_cam_y) * 2.5f;
        break;
    default:
        *k = 2.5f;
        *ox = x * 2.5f;
        *oy = y * 2.5f;
        break;
    }
}

void ui_emit(const UiSpr *s, int space, s32 scale_x, s32 scale_y, int flip, u32 argb, int variant) {
    int idx;
    const UiImage *im = UiSpr_Image(s, &idx);
    UiQuad *q;
    float px, py, k, sx, sy, l, t;
    if (!im || s_nquads >= UI_MAX_QUADS || scale_x == 0 || scale_y == 0)
        return;
    xform(space, s->x, s->y, &px, &py, &k);
    sx = scale_x / 4096.0f * k;
    sy = scale_y / 4096.0f * k;
    /* AnimatorSprite_Draw: image top-left = position - origin; a flip mirrors
     * the frame about the origin */
    l = (flip & UI_FLIP_H) ? -(float)(im->w - im->ox) : -(float)im->ox;
    t = (flip & UI_FLIP_V) ? -(float)(im->h - im->oy) : -(float)im->oy;
    q = &s_quads[s_nquads++];
    q->x = px + l * sx;
    q->y = py + t * sy;
    q->w = im->w * sx;
    q->h = im->h * sy;
    q->u0 = im->x;
    q->u1 = (float)(im->x + im->w);
    q->v0 = im->y;
    q->v1 = (float)(im->y + im->h);
    if (flip & UI_FLIP_H) {
        q->u0 = (float)(im->x + im->w);
        q->u1 = im->x;
    }
    if (flip & UI_FLIP_V) {
        q->v0 = (float)(im->y + im->h);
        q->v1 = im->y;
    }
    q->argb = argb;
    q->tex = (int)(s->res - g_ui_res);
    q->image = idx;
    q->variant = variant;
    q->layer = g_ui_layer;
    q->screen = g_ui_screen;
}

void ui_emit_flat(int space, float x, float y, float w, float h, u32 argb) {
    UiQuad *q;
    float px, py, k;
    if (s_nquads >= UI_MAX_QUADS)
        return;
    xform(space, x, y, &px, &py, &k);
    q = &s_quads[s_nquads++];
    memset(q, 0, sizeof(*q));
    q->x = px;
    q->y = py;
    q->w = w * k;
    q->h = h * k;
    q->argb = argb;
    q->tex = -1;
    q->image = -1;
    q->layer = g_ui_layer;
    q->screen = g_ui_screen;
}

/* clipped image quad (BG pieces): destination (x, y, w, h) in space units shows
 * texels (u0, v0)-(u0 + uw, v0 + vh); clipping keeps the texel mapping */
static void emit_clipped(int tex, int image, int space, float x, float y, float w, float h, float u0, float v0,
                         float uw, float vh, float cx0, float cy0, float cx1, float cy1, u32 argb) {
    UiQuad *q;
    float l = x, t = y, r = x + w, b = y + h, px, py, k, px1, py1, su, sv;
    if (w <= 0 || h <= 0)
        return;
    su = uw / w;
    sv = vh / h;
    if (l < cx0)
        l = cx0;
    if (t < cy0)
        t = cy0;
    if (r > cx1)
        r = cx1;
    if (b > cy1)
        b = cy1;
    if (r <= l || b <= t || s_nquads >= UI_MAX_QUADS)
        return;
    xform(space, l, t, &px, &py, &k);
    xform(space, r, b, &px1, &py1, &k);
    q = &s_quads[s_nquads++];
    memset(q, 0, sizeof(*q));
    q->x = px;
    q->y = py;
    q->w = px1 - px;
    q->h = py1 - py;
    q->u0 = u0 + (l - x) * su;
    q->v0 = v0 + (t - y) * sv;
    q->u1 = u0 + (r - x) * su;
    q->v1 = v0 + (b - y) * sv;
    q->argb = argb;
    q->tex = tex;
    q->image = image;
    q->layer = g_ui_layer;
    q->screen = g_ui_screen;
}

void ui_emit_image(int res, int image, int space, float x, float y, float cx0, float cy0, float cx1, float cy1,
                   u32 argb) {
    const UiRes *r = &g_ui_res[res];
    const UiImage *im;
    if (!r->blob || image < 0 || image >= r->n_images)
        return;
    im = &r->images[image];
    emit_clipped(res, image, space, x, y, im->w, im->h, im->x, im->y, im->w, im->h, cx0, cy0, cx1, cy1, argb);
}

void ui_emit_image_part(int res, int image, int space, float x, float y, float w, float h, float su, float sv,
                        float suw, float svh, float cx0, float cy0, float cx1, float cy1, u32 argb) {
    const UiRes *r = &g_ui_res[res];
    const UiImage *im;
    if (!r->blob || image < 0 || image >= r->n_images)
        return;
    im = &r->images[image];
    emit_clipped(res, image, space, x, y, w, h, im->x + su, im->y + sv, suw, svh, cx0, cy0, cx1, cy1, argb);
}

void ui_emit_face(int space, float x, float y, int f, int w, int h, u32 argb) {
    emit_clipped(UI_TEX_FACE, f, space, x, y, (float)w, (float)h, 0, 0, (float)w, (float)h, -1e9f, -1e9f, 1e9f, 1e9f,
                 argb);
}

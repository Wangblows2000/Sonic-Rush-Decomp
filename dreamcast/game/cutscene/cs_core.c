/*
 * Cutscene runtime services (portable): csdata.bin sections, indexed images
 * (CSI1 / CSP1), textures expanded from DS palettes, the AnimatorSprite subset
 * on converted BAC frame tables (DSPR v1), the per-frame quad list and the
 * view that maps DS pixels to the single 640x480 frame, the log the host
 * tests read.
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "cs_internal.h"

char g_cs_dir[96] = "/cd/cutscene", g_cs_sprite_dir[96] = "/cd/sprites", g_cs_model_dir[96] = "/cd/models",
     g_cs_music_dir[96] = "/cd/music";
CsStory g_cs_story = {0, 0, 1, 0, 0, 0, 1, 0x7f};
CsResult g_cs_result;
CsPad gPad;
u32 gRandSeed = 0x12345678;
s16 g_cs_bright;
u32 g_cs_backdrop;
const char *const cs_lang_suffix[6] = {"jpn", "eng", "fra", "deu", "ita", "spa"};

CsQuad *cs_quad_new(void);
CsQuad *cs_add_rect(u8 kind, CsTex *t, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1,
                    u32 argb);
static u16 rd16(const u8 *p) { return (u16)(p[0] | p[1] << 8); }
static u32 rd32(const u8 *p) { return (u32)p[0] | (u32)p[1] << 8 | (u32)p[2] << 16 | (u32)p[3] << 24; }

#ifdef _arch_dreamcast
#include <arch/timer.h>
#endif
void *cs_load_file(const char *path, u32 *size) {
    FILE *f;
    long n;
    u8 *b;
#ifdef _arch_dreamcast
    uint64_t t0 = timer_ms_gettime64();
#endif
    f = fopen(path, "rb");
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    b = (u8 *)malloc(n > 0 ? (size_t)n : 1);
    if (b && n > 0 && fread(b, 1, (size_t)n, f) != (size_t)n) {
        free(b);
        b = NULL;
    }
    fclose(f);
    if (b && size)
        *size = (u32)n;
#ifdef _arch_dreamcast
    {
        /* load-time diagnostics (serial): files that took more than 100 ms */
        unsigned ms = (unsigned)(timer_ms_gettime64() - t0);
        if (ms > 100)
            printf("cs: load %s %ld B %u ms\n", path, n, ms);
    }
#endif
    return b;
}

/* ----------------------------------------------------------------- log */
#define LOG_MAX 4096
static char *s_log[LOG_MAX];
static int s_log_n;
void cs_log(const char *fmt, ...) {
    char buf[160];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
#ifdef CS_HOST
    if (s_log_n < LOG_MAX) {
        s_log[s_log_n] = (char *)malloc(strlen(buf) + 1);
        if (s_log[s_log_n])
            strcpy(s_log[s_log_n++], buf);
    }
#else
    printf("cs: %s\n", buf);
    (void)s_log;
#endif
}
int cutscene_log_count(void) { return s_log_n; }
const char *cutscene_log_line(int i) { return i >= 0 && i < s_log_n ? s_log[i] : ""; }
void cutscene_log_clear(void) {
    int i;
    for (i = 0; i < s_log_n; i++)
        free(s_log[i]);
    s_log_n = 0;
}

/* ----------------------------------------------------------------- data */
static u8 *s_data;
static u32 s_data_size;
int cs_data_load(void) {
    char path[160];
    if (s_data)
        return 0;
    snprintf(path, sizeof(path), "%s/csdata.bin", g_cs_dir);
    s_data = (u8 *)cs_load_file(path, &s_data_size);
    if (!s_data || s_data_size < 8 || memcmp(s_data, "RCSD", 4) != 0 || rd16(s_data + 4) != 1) {
        free(s_data);
        s_data = NULL;
        printf("cutscene: %s missing or invalid\n", path);
        return -1;
    }
    return 0;
}

const void *cs_sec(const char *name, u32 *size) {
    int i, n;
    if (!s_data)
        return NULL;
    n = rd16(s_data + 6);
    for (i = 0; i < n; i++) {
        const u8 *e = s_data + 8 + 20 * i;
        if (strncmp((const char *)e, name, 12) == 0) {
            u32 off = rd32(e + 12), sz = rd32(e + 16);
            if (off + sz > s_data_size)
                return NULL;
            if (size)
                *size = sz;
            return s_data + off;
        }
    }
    return NULL;
}

int cs_lang(void) { return g_cs_story.language >= 0 && g_cs_story.language <= 5 ? g_cs_story.language : 1; }

/* FX_SinIdx / FX_CosIdx: the ROM table (data_0206e470), idx = angle >> 4 */
s16 cs_sin(u16 a) {
    const u8 *t = (const u8 *)cs_sec("sincos", NULL);
    return t ? (s16)rd16(t + (a >> 4) * 4) : 0;
}
s16 cs_cos(u16 a) {
    const u8 *t = (const u8 *)cs_sec("sincos", NULL);
    return t ? (s16)rd16(t + (a >> 4) * 4 + 2) : 0x1000;
}

/* --------------------------------------------------------------- images */
int cs_img_parse(CsImg *img, const u8 *b, u32 size) {
    if (size < 16 || memcmp(b, "CSI1", 4) != 0)
        return -1;
    img->w = rd16(b + 4);
    img->h = rd16(b + 6);
    img->type = rd16(b + 8);
    img->flags = rd16(b + 10);
    img->npal = rd16(b + 12);
    img->pofs = rd16(b + 14);
    img->pal = (const u16 *)(b + 16);
    img->px = b + 16 + 2 * img->npal;
    if (16u + 2u * img->npal + (u32)img->w * img->h > size)
        return -1;
    return 0;
}

int cs_pack_load(CsPack *p, const char *path) {
    memset(p, 0, sizeof(*p));
    p->blob = (u8 *)cs_load_file(path, &p->size);
    if (!p->blob || p->size < 4) {
        free(p->blob);
        p->blob = NULL;
        printf("cutscene: missing %s\n", path);
        return -1;
    }
    p->count = rd32(p->blob);
    if (4 + 4 * p->count > p->size) {
        cs_pack_free(p);
        return -1;
    }
    return 0;
}

int cs_pack_get(const CsPack *p, int i, CsImg *out) {
    u32 off, end;
    if (!p->blob || i < 0 || (u32)i >= p->count)
        return -1;
    off = rd32(p->blob + 4 + 4 * i);
    if (!off)
        return -1;
    end = p->size;
    return cs_img_parse(out, p->blob + off, end - off);
}

void cs_pack_free(CsPack *p) {
    free(p->blob);
    memset(p, 0, sizeof(*p));
}

int cs_pic_load(CsPic *p, const char *name) {
    char path[200];
    u32 size;
    memset(p, 0, sizeof(*p));
    if (!name || !name[0])
        return -1;
    snprintf(path, sizeof(path), "%s/bg/%s.csi", g_cs_dir, name);
    p->blob = (u8 *)cs_load_file(path, &size);
    if (!p->blob || cs_img_parse(&p->img, p->blob, size) != 0) {
        printf("cutscene: missing %s\n", path);
        free(p->blob);
        p->blob = NULL;
        return -1;
    }
    return 0;
}
/* one entry of a CSP1 pack read from the file (TalkDemo_ReadMessage /
 * File_LoadArchiveEntry read one entry the same way) */
int cs_pack_read(CsPic *p, const char *path, int i) {
    FILE *f;
    u8 h[8];
    u32 n, off, next, size;
    long end;
    memset(p, 0, sizeof(*p));
    if (i < 0)
        return -1;
    f = fopen(path, "rb");
    if (!f) {
        printf("cutscene: missing %s\n", path);
        return -1;
    }
    if (fread(h, 1, 4, f) != 4) {
        fclose(f);
        return -1;
    }
    n = rd32(h);
    if ((u32)i >= n || fseek(f, 4 + 4 * i, SEEK_SET) != 0 || fread(h, 1, 4, f) != 4) {
        fclose(f);
        return -1;
    }
    off = rd32(h);
    next = 0;
    while (!next && (u32)++i < n)
        if (fread(h, 1, 4, f) == 4)
            next = rd32(h);
        else
            break;
    fseek(f, 0, SEEK_END);
    end = ftell(f);
    if (!next)
        next = (u32)end;
    if (!off || next <= off) {
        fclose(f);
        return -1;
    }
    size = next - off;
    p->blob = (u8 *)malloc(size);
    if (!p->blob || fseek(f, off, SEEK_SET) != 0 || fread(p->blob, 1, size, f) != size ||
        cs_img_parse(&p->img, p->blob, size) != 0) {
        fclose(f);
        free(p->blob);
        p->blob = NULL;
        return -1;
    }
    fclose(f);
    return 0;
}

void cs_pic_free(CsPic *p) {
    free(p->blob);
    memset(p, 0, sizeof(*p));
}

/* ------------------------------------------------------------- textures */
u16 cs_bgr_to_argb(u16 c) { return (u16)(0x8000 | (c & 0x1F) << 10 | (c & 0x3E0) | (c >> 10 & 0x1F)); }

static int pow2(int v) {
    int p = 8;
    while (p < v && p < 1024)
        p <<= 1;
    return p;
}

int cs_tex_init(CsTex *t, int w, int h) {
    memset(t, 0, sizeof(*t));
    t->w = w;
    t->h = h;
    t->tw = pow2(w);
    t->th = pow2(h);
    t->argb = (u16 *)calloc((size_t)t->tw * t->th, 2);
    t->dirty = 1;
    t->dy0 = 0;
    t->dy1 = t->th;
    return t->argb ? 0 : -1;
}

void cs_tex_put(CsTex *t, int x, int y, int w, int h, const u8 *idx, int stride, const u16 *pal) {
    int i, j;
    if (!t->argb)
        return;
    for (j = 0; j < h; j++) {
        int ty = y + j;
        u16 *d;
        const u8 *s = idx + (size_t)j * stride;
        if (ty < 0 || ty >= t->h)
            continue;
        d = t->argb + (size_t)ty * t->tw;
        for (i = 0; i < w; i++) {
            int tx = x + i;
            if (tx < 0 || tx >= t->w)
                continue;
            d[tx] = s[i] ? cs_bgr_to_argb(pal[s[i]]) : t->fill;
        }
    }
    if (!t->dirty) {
        t->dy0 = y;
        t->dy1 = y + h;
    } else {
        if (y < t->dy0)
            t->dy0 = y;
        if (y + h > t->dy1)
            t->dy1 = y + h;
    }
    if (t->dy0 < 0)
        t->dy0 = 0;
    if (t->dy1 > t->th)
        t->dy1 = t->th;
    t->dirty = 1;
}

void cs_tex_clear(CsTex *t) {
    int i;
    if (!t->argb)
        return;
    for (i = 0; i < t->tw * t->th; i++)
        t->argb[i] = t->fill;
    t->dirty = 1;
    t->dy0 = 0;
    t->dy1 = t->th;
}

void cs_tex_release_pvr(CsTex *t);   /* cs_draw.c */
void cs_tex_free(CsTex *t) {
    cs_tex_release_pvr(t);
    free(t->argb);
    memset(t, 0, sizeof(*t));
}

/* -------------------------------------------------------------- sprites */
CsRes *cs_res_load(const char *id) {
    char path[220];
    u8 *b;
    u32 size, need;
    int i;
    CsRes *r;
    snprintf(path, sizeof(path), "%s/%s/frames.bin", g_cs_sprite_dir, id);
    b = (u8 *)cs_load_file(path, &size);
    if (!b) {
        printf("cutscene: missing %s\n", path);
        return NULL;
    }
    if (size < 16 || memcmp(b, "DSPR", 4) != 0 || rd16(b + 4) != 1) {
        free(b);
        return NULL;
    }
    r = (CsRes *)calloc(1, sizeof(*r));
    r->n_pages = rd16(b + 6);
    r->n_images = rd16(b + 8);
    r->n_anims = rd16(b + 10);
    r->n_frames = rd16(b + 12);
    need = 16 + 4u * r->n_pages + 16u * r->n_images + 12u * r->n_anims + 4u * r->n_frames;
    if (need > size || r->n_pages > 4) {
        free(b);
        free(r);
        return NULL;
    }
    r->blob = b;
    r->pages = (const CsPage *)(b + 16);
    r->images = (const CsImage *)(b + 16 + 4 * r->n_pages);
    r->anims = (const CsAnimDef *)((const u8 *)r->images + 16 * r->n_images);
    r->frames = (const CsFrame *)((const u8 *)r->anims + 12 * r->n_anims);
    for (i = 0; i < r->n_pages; i++) {
        u8 *px;
        u32 psz;
        int y;
        snprintf(path, sizeof(path), "%s/%s/atlas%d.argb1555", g_cs_sprite_dir, id, i);
        px = (u8 *)cs_load_file(path, &psz);
        if (px && psz == (u32)r->pages[i].w * r->pages[i].h * 2 && cs_tex_init(&r->tex[i], r->pages[i].w, r->pages[i].h) == 0) {
            for (y = 0; y < r->pages[i].h; y++)
                memcpy(r->tex[i].argb + (size_t)y * r->tex[i].tw, px + (size_t)y * r->pages[i].w * 2,
                       (size_t)r->pages[i].w * 2);
        }
        free(px);
    }
    return r;
}

void cs_res_free(CsRes *r) {
    int i;
    if (!r)
        return;
    for (i = 0; i < 4; i++)
        cs_tex_free(&r->tex[i]);
    free(r->blob);
    free(r);
}

void AnimatorSprite_Init(CsSpr *s, const CsRes *r, u16 anim, u32 flags) {
    memset(s, 0, sizeof(*s));
    s->res = r;
    s->speed = 0x1000;
    s->flags = flags;
    s->alpha = 16;
    s->bright = 255;
    AnimatorSprite_SetAnimation(s, anim);
}

void AnimatorSprite_SetAnimation(CsSpr *s, u16 anim) {
    s->anim = anim;
    s->frame = 0;
    s->wait = 0;
    s->flags &= ~(u32)(SPR_STOPPED | SPR_ENDED);
}

/* AnimatorSprite_ProcessAnimation: the first call after SetAnimation shows the
 * first frame; each frame then waits `duration` game frames (wait = dur << 12,
 * speed 0x1000 per call). At the end: loop (flag 4) to the BAC loop frame, or
 * stop on the last frame with flag 2 (ended). */
void AnimatorSprite_ProcessAnimation(CsSpr *s) {
    const CsAnimDef *ad;
    if (!s->res || s->anim >= s->res->n_anims || (s->flags & SPR_STOPPED))
        return;
    s->flags &= ~(u32)SPR_ENDED;
    ad = &s->res->anims[s->anim];
    if (ad->count == 0)
        return;
    if (s->wait == 0 && s->frame == 0) {
        s->wait = (s32)s->res->frames[ad->first].duration << 12;
        if (s->wait == 0)
            s->wait = 1;
        return;
    }
    s->wait -= s->speed;
    if (s->wait > 0)
        return;
    if (s->frame + 1 >= ad->count) {
        s->flags |= SPR_ENDED;
        if (!(s->flags & SPR_LOOP)) {
            s->flags |= SPR_STOPPED;
            return;
        }
        s->frame = (u16)(ad->loop >= 0 && ad->loop < ad->count ? ad->loop : 0);
    } else {
        s->frame++;
    }
    s->wait += (s32)s->res->frames[ad->first + s->frame].duration << 12;
    if (s->wait <= 0)
        s->wait = 1;
}

static const CsImage *spr_image(const CsSpr *s) {
    const CsAnimDef *ad;
    u16 im;
    if (!s->res || s->anim >= s->res->n_anims)
        return NULL;
    ad = &s->res->anims[s->anim];
    if (s->frame >= ad->count)
        return NULL;
    im = s->res->frames[ad->first + s->frame].image;
    if (im == 0xFFFF || im >= s->res->n_images)
        return NULL;
    return &s->res->images[im];
}

int AnimatorSprite_FrameBox(const CsSpr *s, s16 *l, s16 *t, s16 *r, s16 *b) {
    const CsImage *im = spr_image(s);
    if (!im)
        return 0;
    *l = -im->ox;
    *t = -im->oy;
    *r = (s16)(im->w - im->ox);
    *b = (s16)(im->h - im->oy);
    return 1;
}

void AnimatorSprite_Draw(CsSpr *s) {
    const CsImage *im = spr_image(s);
    CsRes *r = (CsRes *)s->res;
    float x, y, u0, u1, v0, v1;
    u32 a;
    if (!im || (s->flags & SPR_HIDDEN) || im->page >= r->n_pages)
        return;
    x = s->x - im->ox;
    y = s->y - im->oy;
    u0 = im->x;
    u1 = im->x + im->w;
    v0 = im->y;
    v1 = im->y + im->h;
    if (s->flags & SPR_HFLIP) {
        float t = u0;
        x = s->x + im->ox - im->w;
        u0 = u1;
        u1 = t;
    }
    if (s->flags & SPR_VFLIP) {
        float t = v0;
        y = s->y + im->oy - im->h;
        v0 = v1;
        v1 = t;
    }
    a = s->alpha >= 16 ? 255 : (u32)s->alpha * 255 / 16;
    {
        CsQuad *q;
        float sx0 = (x - g_cs_view.sx) * g_cs_view.scale + g_cs_view.dx;
        float sy0 = (y - g_cs_view.sy) * g_cs_view.scale + g_cs_view.dy;
        q = cs_add_rect(CSQ_SPR, &r->tex[im->page], sx0, sy0, sx0 + im->w * g_cs_view.scale,
                        sy0 + im->h * g_cs_view.scale, u0, v0, u1, v1,
                        a << 24 | (u32)s->bright << 16 | (u32)s->bright << 8 | s->bright);
        if (q) {
            q->res = r;
            q->page = im->page;
        }
    }
}

/* ---------------------------------------------------------------- quads */
#define QUAD_MAX 512
static CsQuad s_quads[QUAD_MAX];
static int s_nquads;
CsView g_cs_view;

void cs_view_set(float sx, float sy, float scale, float dx, float dy) {
    g_cs_view.sx = sx;
    g_cs_view.sy = sy;
    g_cs_view.scale = scale;
    g_cs_view.dx = dx;
    g_cs_view.dy = dy;
    g_cs_view.clip_x0 = 0;
    g_cs_view.clip_y0 = 0;
    g_cs_view.clip_x1 = 640;
    g_cs_view.clip_y1 = 480;
}
void cs_view_full(void) { cs_view_set(0, 0, 2.5f, 0, 0); }
void cs_quads_reset(void) { s_nquads = 0; }
int cs_quad_count(void) { return s_nquads; }
const CsQuad *cs_quads(void) { return s_quads; }

CsQuad *cs_quad_new(void) {
    CsQuad *q;
    if (s_nquads >= QUAD_MAX)
        return NULL;
    q = &s_quads[s_nquads++];
    memset(q, 0, sizeof(*q));
    return q;
}

/* clipped axis-aligned rectangle (screen px) */
CsQuad *cs_add_rect(u8 kind, CsTex *t, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1,
                    u32 argb) {
    CsQuad *q;
    float du = (x1 > x0) ? (u1 - u0) / (x1 - x0) : 0, dv = (y1 > y0) ? (v1 - v0) / (y1 - y0) : 0;
    if (x0 < g_cs_view.clip_x0) { u0 += (g_cs_view.clip_x0 - x0) * du; x0 = g_cs_view.clip_x0; }
    if (y0 < g_cs_view.clip_y0) { v0 += (g_cs_view.clip_y0 - y0) * dv; y0 = g_cs_view.clip_y0; }
    if (x1 > g_cs_view.clip_x1) { u1 -= (x1 - g_cs_view.clip_x1) * du; x1 = g_cs_view.clip_x1; }
    if (y1 > g_cs_view.clip_y1) { v1 -= (y1 - g_cs_view.clip_y1) * dv; y1 = g_cs_view.clip_y1; }
    if (x1 <= x0 || y1 <= y0)
        return NULL;
    q = cs_quad_new();
    if (!q)
        return NULL;
    q->kind = kind;
    q->tex = t;
    q->argb = argb;
    q->x[0] = x0; q->y[0] = y0; q->u[0] = u0; q->v[0] = v0;
    q->x[1] = x1; q->y[1] = y0; q->u[1] = u1; q->v[1] = v0;
    q->x[2] = x0; q->y[2] = y1; q->u[2] = u0; q->v[2] = v1;
    q->x[3] = x1; q->y[3] = y1; q->u[3] = u1; q->v[3] = v1;
    return q;
}

void cs_quad_tex(CsTex *t, float x, float y, float w, float h, float u0, float v0, float u1, float v1, u32 argb) {
    float x0 = (x - g_cs_view.sx) * g_cs_view.scale + g_cs_view.dx, y0 = (y - g_cs_view.sy) * g_cs_view.scale + g_cs_view.dy;
    if (!t || !t->argb)
        return;
    cs_add_rect(CSQ_TEX, t, x0, y0, x0 + w * g_cs_view.scale, y0 + h * g_cs_view.scale, u0, v0, u1, v1, argb);
}

void cs_quad_flat(float x, float y, float w, float h, u32 argb) {
    float x0 = (x - g_cs_view.sx) * g_cs_view.scale + g_cs_view.dx, y0 = (y - g_cs_view.sy) * g_cs_view.scale + g_cs_view.dy;
    cs_add_rect(CSQ_FLAT, NULL, x0, y0, x0 + w * g_cs_view.scale, y0 + h * g_cs_view.scale, 0, 0, 0, 0, argb);
}

/* G2_SetBGxAffine(mtx, cx, cy, x1, y1): texel = M * (screen + (x1,y1) - c) + c.
 * The quad covers the texture rectangle; its corners go through M^-1. */
void cs_quad_affine(CsTex *t, const s32 m[4], s32 cx, s32 cy, s32 ofsx, s32 ofsy, u32 argb) {
    float a = m[0] / 4096.0f, b = m[1] / 4096.0f, c = m[2] / 4096.0f, d = m[3] / 4096.0f;
    float det = a * d - b * c, ia, ib, ic, id;
    static const float cu[4] = {0, 1, 0, 1}, cv[4] = {0, 0, 1, 1};
    CsQuad *q;
    int k;
    if (!t || !t->argb || det == 0.0f)
        return;
    ia = d / det;
    ib = -b / det;
    ic = -c / det;
    id = a / det;
    q = cs_quad_new();
    if (!q)
        return;
    q->kind = CSQ_TEX;
    q->tex = t;
    q->argb = argb;
    for (k = 0; k < 4; k++) {
        float tu = cu[k] * t->w, tv = cv[k] * t->h;
        float px = ia * (tu - cx) + ib * (tv - cy) + cx - ofsx;
        float py = ic * (tu - cx) + id * (tv - cy) + cy - ofsy;
        q->x[k] = (px - g_cs_view.sx) * g_cs_view.scale + g_cs_view.dx;
        q->y[k] = (py - g_cs_view.sy) * g_cs_view.scale + g_cs_view.dy;
        q->u[k] = tu;
        q->v[k] = tv;
    }
}

/* ------------------------------------------------- BrightnessFade (DS task 0x3e00) */
typedef struct {
    u16 flags;   /* 1 white, 2 fade in (bright down), 4 keep after the end, 0x20 done, 0x100 main, 0x200 sub */
    s16 bright;  /* 0..0x1000 */
    s16 step;
    s16 delay;
    int active;
} CsFade;
static CsFade s_fade;
s16 g_cs_bright_sub;

static void BrightnessFade_Apply(CsFade *w) {
    s16 b = w->bright > 0x1000 ? 0x10 : (s16)(w->bright >> 8);
    if (!(w->flags & 1))
        b = (s16)-b;
    if (w->flags & 0x300) {
        if (w->flags & 0x100)
            g_cs_bright = b;
        if (w->flags & 0x200)
            g_cs_bright_sub = b;
    } else {
        g_cs_bright = b;
        g_cs_bright_sub = b;
    }
}
void BrightnessFade_Create(u16 flags, u16 v) {
    int exists = s_fade.active;
    if (!exists)
        memset(&s_fade, 0, sizeof(s_fade));
    s_fade.active = 1;
    s_fade.step = (s16)v;
    s_fade.flags = flags;
    s_fade.delay = 0;
    if (!exists || (s_fade.flags & 0x10))
        s_fade.bright = (s_fade.flags & 2) ? 0x1100 : 0;
    if (!(s_fade.flags & 8))
        BrightnessFade_Apply(&s_fade);
}
BOOL BrightnessFade_IsDone(void) { return !s_fade.active || (s_fade.flags & 0x20); }
void BrightnessFade_Destroy(void) { s_fade.active = 0; }
void BrightnessFade_Main(void) {
    CsFade *w = &s_fade;
    if (!w->active)
        return;
    if (w->flags & 2) {
        if (w->bright <= 0) {
            w->flags |= 0x20;
            if (!(w->flags & 4)) {
                w->active = 0;
                return;
            }
        }
        if (w->delay != 0)
            w->delay--;
        else
            w->bright = (s16)(w->bright - w->step);
        if (w->bright < 0)
            w->bright = 0;
    } else {
        if (w->bright >= 0x1000) {
            w->flags |= 0x20;
            if (!(w->flags & 4)) {
                w->active = 0;
                return;
            }
        }
        w->bright = (s16)(w->bright + w->step);
        if (w->bright > 0x1000)
            w->bright = 0x1000;
    }
    BrightnessFade_Apply(w);
}

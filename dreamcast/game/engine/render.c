#include "render.h"
#include "loading.h"
#include <kos.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

float g_cam_x, g_cam_y;
static pvr_list_t s_list;

int tex_load(Texture *t, const char *path)
{
    file_t f = fs_open(path, O_RDONLY);
    uint8_t hdr[12];
    static const int fmts[3] = { PVR_TXRFMT_ARGB1555, PVR_TXRFMT_RGB565, PVR_TXRFMT_ARGB4444 };
    void *buf;
    if(f == FILEHND_INVALID) return -1;
    if(fs_read(f, hdr, 12) != 12 || memcmp(hdr, "DCTX", 4)) { fs_close(f); return -1; }
    t->w = hdr[4] | hdr[5] << 8;
    t->h = hdr[6] | hdr[7] << 8;
    t->fmt = fmts[(hdr[8] | hdr[9] << 8) % 3];
    t->bytes = t->w * t->h * 2;
    t->twiddled = 0;
    buf = memalign(32, t->bytes);
    if(!buf || loading_read(f, buf, t->bytes) != (ssize_t)t->bytes) { free(buf); fs_close(f); return -1; }
    fs_close(f);
    t->ptr = pvr_mem_malloc(t->bytes);
    if(!t->ptr) { free(buf); return -1; }
    pvr_txr_load(buf, t->ptr, t->bytes);
    free(buf);
    return 0;
}

void tex_free(Texture *t)
{
    if(t->ptr) pvr_mem_free(t->ptr);
    t->ptr = NULL;
}

static uint32_t s_frame;   /* palette RAM: frames for the idle-row rule */
void render_begin_frame(void) { pvr_wait_ready(); pvr_scene_begin(); s_frame++; }
void render_end_frame(void) { pvr_scene_finish(); }
void render_begin_list(pvr_list_t list) { s_list = list; pvr_list_begin(list); }
void render_end_list(void) { pvr_list_finish(); }

int tex_txr_format(const Texture *t, int bank)
{
    int f = t->fmt | (t->twiddled ? PVR_TXRFMT_TWIDDLED : PVR_TXRFMT_NONTWIDDLED);
    if(bank < 0) bank = 0;
    if(t->fmt == PVR_TXRFMT_PAL4BPP) f |= PVR_TXRFMT_4BPP_PAL(bank & 63);
    else if(t->fmt == PVR_TXRFMT_PAL8BPP) f |= PVR_TXRFMT_8BPP_PAL(bank & 3);
    return f;
}

static void quad(const Texture *t, int bank, pvr_list_t list, float x, float y, float w, float h,
                 float u0, float v0, float u1, float v1, float z, uint32_t argb)
{
    pvr_poly_cxt_t cxt;
    pvr_poly_hdr_t hdr;
    pvr_vertex_t v;
    if(t && t->ptr && ((t->w & (t->w - 1)) || (t->h & (t->h - 1)) || t->w < 8 || t->h < 8 || t->w > 1024 || t->h > 1024)) {
        /* the PVR takes 8..1024 power-of-two sizes only: skip the quad instead of aborting */
        static int logged;
        if(logged++ < 8) printf("render: texture %dx%d is not a PVR size, quad skipped\n", t->w, t->h);
        return;
    }
    if(t && t->ptr) {
        pvr_poly_cxt_txr(&cxt, list, tex_txr_format(t, bank), t->w, t->h, t->ptr, PVR_FILTER_NONE);
        u0 /= t->w; u1 /= t->w; v0 /= t->h; v1 /= t->h;
    } else
        pvr_poly_cxt_col(&cxt, list);
    pvr_poly_compile(&hdr, &cxt);
    pvr_prim(&hdr, sizeof(hdr));
    v.flags = PVR_CMD_VERTEX; v.argb = argb; v.oargb = 0; v.z = z;
    v.x = x;     v.y = y + h; v.u = u0; v.v = v1; pvr_prim(&v, sizeof(v));
    v.x = x;     v.y = y;     v.u = u0; v.v = v0; pvr_prim(&v, sizeof(v));
    v.x = x + w; v.y = y + h; v.u = u1; v.v = v1; pvr_prim(&v, sizeof(v));
    v.flags = PVR_CMD_VERTEX_EOL;
    v.x = x + w; v.y = y;     v.u = u1; v.v = v0; pvr_prim(&v, sizeof(v));
}

void draw_sprite_world(const Texture *t, pvr_list_t list, float wx, float wy, float w, float h,
                       float u0, float v0, float u1, float v1, int flip, float z, uint32_t argb)
{
    draw_sprite_world_pal(t, -1, list, wx, wy, w, h, u0, v0, u1, v1, flip, z, argb);
}

void draw_sprite_world_pal(const Texture *t, int bank, pvr_list_t list, float wx, float wy, float w, float h,
                           float u0, float v0, float u1, float v1, int flip, float z, uint32_t argb)
{
    float sx = (wx - g_cam_x) * VIEW_SCALE, sy = (wy - g_cam_y) * VIEW_SCALE;
    float sw = w * VIEW_SCALE, sh = h * VIEW_SCALE, tmp;
    if(sx > 640 || sy > 480 || sx + sw < 0 || sy + sh < 0) return;
    if(flip & 1) { tmp = u0; u0 = u1; u1 = tmp; }
    if(flip & 2) { tmp = v0; v0 = v1; v1 = tmp; }
    quad(t, bank, list, sx, sy, sw, sh, u0, v0, u1, v1, z, argb);
}

void draw_rect_screen(const Texture *t, pvr_list_t list, float x, float y, float w, float h,
                      float u0, float v0, float u1, float v1, float z, uint32_t argb)
{
    quad(t, -1, list, x, y, w, h, u0, v0, u1, v1, z, argb);
}

void draw_rect_screen_pal(const Texture *t, int bank, pvr_list_t list, float x, float y, float w, float h,
                          float u0, float v0, float u1, float v1, float z, uint32_t argb)
{
    quad(t, bank, list, x, y, w, h, u0, v0, u1, v1, z, argb);
}

uint32_t render_vram_free(void) { return pvr_mem_available(); }

/* ------------------------------------------------------------------ PVR palette RAM */
enum { ROW_FREE, ROW_PAL8, ROW_PAL4 };
static struct {
    uint8_t kind;
    uint16_t refs;      /* pal4_get references */
    uint16_t mask;      /* defined entries (PAL4) */
    uint16_t gen;       /* bumped when the row is given back (PalRef check) */
    uint32_t last;      /* frame of the last pal4_use */
    uint8_t used_lazy;  /* pal4_use has drawn with it */
    uint16_t col[16];
} s_rows[PAL_ROWS];
static int s_pal_init;

static void pal_setup(void)
{
    if(s_pal_init) return;
    pvr_set_pal_format(PVR_PAL_ARGB1555);
    s_pal_init = 1;
}

/* a PAL4 row nobody holds and nobody drew with for 3 frames (the PVR is done with it) */
static int row_idle(int r)
{
    return s_rows[r].kind == ROW_PAL4 && s_rows[r].refs == 0 &&
           (!s_rows[r].used_lazy || s_frame - s_rows[r].last >= 3);
}
static void row_release(int r)
{
    s_rows[r].kind = ROW_FREE;
    s_rows[r].refs = 0;
    s_rows[r].mask = 0;
    s_rows[r].used_lazy = 0;
    s_rows[r].gen++;
}

uint16_t pal8_rowmask(const uint8_t *texels, uint32_t n)
{
    uint16_t m = 0;
    uint32_t i;
    for(i = 0; i < n && m != 0xffff; i++) m |= (uint16_t)(1u << (texels[i] >> 4));
    return m;
}

/* pass 0: free rows and idle UI rows; pass 1 (no bank otherwise): any row only
 * pal4_use draws with (the UI takes another row on its next draw) */
static int pal8_row_ok(int r, int pass)
{
    if(s_rows[r].kind == ROW_FREE || row_idle(r)) return 1;
    return pass && s_rows[r].kind == ROW_PAL4 && s_rows[r].refs == 0;
}
int pal8_alloc(const uint16_t *pal, uint16_t rowmask)
{
    int b, r, i, pass;
    pal_setup();
    for(pass = 0; pass < 2; pass++)
    for(b = 0; b < 4; b++) {
        for(r = 0; r < 16; r++)
            if((rowmask >> r & 1) && !pal8_row_ok(b * 16 + r, pass)) break;
        if(r < 16) continue;
        for(r = 0; r < 16; r++) {
            if(!(rowmask >> r & 1)) continue;
            if(s_rows[b * 16 + r].kind != ROW_FREE) row_release(b * 16 + r);
            s_rows[b * 16 + r].kind = ROW_PAL8;
            for(i = 0; i < 16; i++) pvr_set_pal_entry(b * 256 + r * 16 + i, pal[r * 16 + i]);
        }
        return b;
    }
    return -1;
}

void pal8_free(int bank, uint16_t rowmask)
{
    int r;
    if(bank < 0 || bank > 3) return;
    for(r = 0; r < 16; r++)
        if((rowmask >> r & 1) && s_rows[bank * 16 + r].kind == ROW_PAL8) row_release(bank * 16 + r);
}

static int row_fits(int r, const uint16_t *col, uint16_t mask)
{
    int k;
    uint16_t both = mask & s_rows[r].mask;
    for(k = 1; k < 16; k++)
        if((both >> k & 1) && s_rows[r].col[k] != col[k]) return 0;
    return 1;
}
static void row_define(int r, const uint16_t *col, uint16_t mask)
{
    int k;
    uint16_t add = mask & ~s_rows[r].mask & 0xfffe;
    for(k = 1; k < 16; k++)
        if(add >> k & 1) {
            s_rows[r].col[k] = col[k];
            pvr_set_pal_entry(r * 16 + k, col[k]);
        }
    s_rows[r].mask |= add;
}
/* a row holding these colours (shared) or a new one: free rows from the top (the
 * low 256-entry banks stay for the level), then idle UI rows */
static int row_find(const uint16_t *col, uint16_t mask)
{
    int r, best = -1;
    pal_setup();
    mask &= 0xfffe;
    for(r = 0; r < PAL_ROWS; r++)
        if(s_rows[r].kind == ROW_PAL4 && row_fits(r, col, mask)) {
            /* prefer the row that already defines most of the entries */
            if(best < 0 || __builtin_popcount(mask & s_rows[r].mask) > __builtin_popcount(mask & s_rows[best].mask))
                best = r;
        }
    if(best >= 0) {
        row_define(best, col, mask);
        return best;
    }
    for(r = PAL_ROWS - 1; r >= 0; r--)
        if(s_rows[r].kind == ROW_FREE) break;
    if(r < 0) {
        for(r = PAL_ROWS - 1; r >= 0; r--)
            if(row_idle(r) && s_rows[r].used_lazy) break;
        if(r < 0) return -1;
        row_release(r);
    }
    s_rows[r].kind = ROW_PAL4;
    s_rows[r].mask = 0;
    s_rows[r].col[0] = 0;
    pvr_set_pal_entry(r * 16, 0);
    row_define(r, col, mask);
    return r;
}

int pal4_get(const uint16_t col[16], uint16_t mask)
{
    int r = row_find(col, mask);
    if(r >= 0) s_rows[r].refs++;
    return r;
}

void pal4_put(int row)
{
    if(row < 0 || row >= PAL_ROWS || s_rows[row].kind != ROW_PAL4 || s_rows[row].refs == 0) return;
    if(--s_rows[row].refs == 0 && !s_rows[row].used_lazy) row_release(row);
}

int pal4_use(PalRef *ref, const uint16_t col[16], uint16_t mask)
{
    int r = ref->row;
    if(r < 0 || r >= PAL_ROWS || s_rows[r].kind != ROW_PAL4 || s_rows[r].gen != ref->gen ||
       !row_fits(r, col, mask & 0xfffe) || (mask & 0xfffe & ~s_rows[r].mask)) {
        r = row_find(col, mask);
        if(r < 0) { ref->row = -1; return -1; }
        ref->row = (int16_t)r;
        ref->gen = s_rows[r].gen;
    }
    s_rows[r].used_lazy = 1;
    s_rows[r].last = s_frame;
    return r;
}

int pal_rows_used(int *pal8, int *pal4)
{
    int r, a = 0, b = 0;
    for(r = 0; r < PAL_ROWS; r++) {
        if(s_rows[r].kind == ROW_PAL8) a++;
        else if(s_rows[r].kind == ROW_PAL4 && !row_idle(r)) b++;
    }
    if(pal8) *pal8 = a;
    if(pal4) *pal4 = b;
    return a + b;
}
int pal_rows_free(void) { return PAL_ROWS - pal_rows_used(NULL, NULL); }

/* ------------------------------------------------------------------ sprite atlases */
static uint8_t *read_all(const char *path, uint32_t *size)
{
    file_t f = fs_open(path, O_RDONLY);
    uint8_t *b;
    if(f == FILEHND_INVALID) return NULL;
    *size = fs_total(f);
    b = memalign(32, *size ? *size : 4);
    if(b && loading_read(f, b, *size) != (ssize_t)*size) { free(b); b = NULL; }
    fs_close(f);
    return b;
}

static const uint8_t *dspr_image(const uint8_t *dspr, int i)
{
    int np = dspr[6] | dspr[7] << 8;
    return dspr + 16 + 4 * np + 16 * i;
}
static int dspr_count(const uint8_t *dspr, int field) { return dspr[field] | dspr[field + 1] << 8; }

static int page_upload(Texture *t, int fmt, int twiddled, int w, int h, const void *data, uint32_t bytes)
{
    memset(t, 0, sizeof(*t));
    t->ptr = pvr_mem_malloc(bytes);
    if(!t->ptr) return -1;
    t->w = w; t->h = h; t->fmt = fmt; t->twiddled = twiddled; t->bytes = bytes;
    pvr_txr_load((void *)data, t->ptr, bytes);
    return 0;
}

/* a PAL4 page as ARGB1555 (non-twiddled): each image with its own palette */
static int page_expand(SprAtlas *a, int page, const uint8_t *pt4)
{
    int w = a->tex[page].w, h = a->tex[page].h, n = dspr_count(a->dspr, 8), i, x, y;
    uint32_t bytes = (uint32_t)w * h * 2;
    uint16_t *buf = memalign(32, bytes);
    Texture old = a->tex[page];
    if(!buf) return -1;
    memset(buf, 0, bytes);
    for(i = 0; i < n; i++) {
        const uint8_t *im = dspr_image(a->dspr, i);
        int ix = im[2] | im[3] << 8, iy = im[4] | im[5] << 8, iw = im[6] | im[7] << 8, ih = im[8] | im[9] << 8;
        const uint16_t *pal;
        if(im[0] != page || a->pal.image_pal[i] == SPRPAL_NONE) continue;
        pal = a->pal.pal + 16 * a->pal.image_pal[i];
        for(y = iy; y < iy + ih && y < h; y++)
            for(x = ix; x < ix + iw && x < w; x++)
                buf[y * w + x] = pal[sprpal_texel(pt4, w, h, x, y)];
    }
    if(old.ptr) { pvr_mem_free(old.ptr); a->vram -= old.bytes; }
    if(page_upload(&a->tex[page], PVR_TXRFMT_ARGB1555, 0, w, h, buf, bytes) != 0) {
        memset(&a->tex[page], 0, sizeof(a->tex[page]));
        free(buf);
        return -1;
    }
    a->vram += bytes;
    a->pal4[page] = 0;
    if((a->flags & SPR_ATLAS_KEEP) && !(a->flags & SPR_ATLAS_BORROWED)) { free(a->argb[page]); a->argb[page] = buf; }
    else free(buf);
    printf("sprites: palette RAM full, page %d (%dx%d) expanded to ARGB1555\n", page, w, h);
    return 0;
}

/* upload the pages (texels[p] PAL4 data or argb[p] ARGB1555 data per page) and,
 * static, take the palette rows; expands the pages whose palettes find no row */
static int atlas_build(SprAtlas *a, int n_images)
{
    int p, i, ok = 0;
    uint8_t *fail = NULL;
    if(a->pal.n_pal) {
        a->row = malloc(sizeof(int16_t) * a->pal.n_pal);
        a->ref = calloc(a->pal.n_pal, sizeof(PalRef));
        fail = calloc(a->pal.n_pal, 1);
        if(!a->row || !a->ref || !fail) {
            free(a->row); free(a->ref); free(fail);
            a->row = NULL; a->ref = NULL; fail = NULL;
        } else
            for(i = 0; i < a->pal.n_pal; i++) { a->row[i] = -1; a->ref[i].row = -1; }
    }
    for(p = 0; p < a->n_pages; p++) {
        const uint8_t *pg = a->dspr + 16 + 4 * p;
        int w = pg[0] | pg[1] << 8, h = pg[2] | pg[3] << 8;
        if(a->texels[p] && a->row &&
           page_upload(&a->tex[p], PVR_TXRFMT_PAL4BPP, 1, w, h, a->texels[p], (uint32_t)w * h / 2) == 0) {
            a->vram += (uint32_t)w * h / 2;
            a->pal4[p] = 1;
            ok++;
        } else if(a->argb[p] && page_upload(&a->tex[p], PVR_TXRFMT_ARGB1555, 0, w, h, a->argb[p], (uint32_t)w * h * 2) == 0) {
            a->vram += (uint32_t)w * h * 2;
            ok++;
        }
    }
    if(!(a->flags & SPR_ATLAS_LAZY) && a->row) {
        /* static rows for every palette an image of a PAL4 page uses */
        for(i = 0; i < n_images; i++) {
            int pal = a->pal.image_pal[i], pg = dspr_image(a->dspr, i)[0];
            if(pal == SPRPAL_NONE || pg >= a->n_pages || !a->pal4[pg] || a->row[pal] >= 0 || fail[pal]) continue;
            a->row[pal] = (int16_t)pal4_get(a->pal.pal + 16 * pal, a->pal.pal_mask[pal]);
            if(a->row[pal] < 0) fail[pal] = 1;
        }
        for(p = 0; p < a->n_pages; p++) {
            int bad = 0;
            if(!a->pal4[p]) continue;
            for(i = 0; i < n_images; i++) {
                int pal = a->pal.image_pal[i];
                if(dspr_image(a->dspr, i)[0] == p && pal != SPRPAL_NONE && fail[pal]) bad = 1;
            }
            if(bad) page_expand(a, p, a->texels[p]);
        }
        /* rows only the expanded pages needed go back */
        for(i = 0; i < a->pal.n_pal; i++) {
            int k, need = 0;
            if(a->row[i] < 0) continue;
            for(k = 0; k < n_images && !need; k++) {
                int pg = dspr_image(a->dspr, k)[0];
                need = a->pal.image_pal[k] == i && pg < a->n_pages && a->pal4[pg];
            }
            if(!need) { pal4_put(a->row[i]); a->row[i] = -1; }
        }
    }
    free(fail);
    return ok;
}

int spr_atlas_load(SprAtlas *a, const char *dir, const uint8_t *dspr, int flags)
{
    char path[200];
    uint32_t sz;
    int p, n_images, ok;
    memset(a, 0, sizeof(*a));
    a->dspr = dspr;
    a->flags = flags | ((flags & SPR_ATLAS_LAZY) ? SPR_ATLAS_KEEP : 0);
    a->n_pages = dspr_count(dspr, 6);
    if(a->n_pages > SPR_ATLAS_PAGES) a->n_pages = SPR_ATLAS_PAGES;
    n_images = dspr_count(dspr, 8);
    snprintf(path, sizeof(path), "%s/palettes.bin", dir);
    a->palblob = read_all(path, &sz);
    if(a->palblob && sprpal_parse(&a->pal, a->palblob, sz, dspr_count(dspr, 6), n_images, dspr_count(dspr, 10)) != 0) {
        free(a->palblob);
        a->palblob = NULL;
        memset(&a->pal, 0, sizeof(a->pal));
    }
    for(p = 0; p < a->n_pages; p++) {
        const uint8_t *pg = dspr + 16 + 4 * p;
        int w = pg[0] | pg[1] << 8, h = pg[2] | pg[3] << 8;
        uint8_t *d;
        if(sprpal_page_is_pal4(&a->pal, p)) {
            snprintf(path, sizeof(path), "%s/atlas%d.pt4", dir, p);
            d = read_all(path, &sz);
            if(d && sz == (uint32_t)w * h / 2) { a->texels[p] = d; continue; }
            free(d);
        }
        snprintf(path, sizeof(path), "%s/atlas%d.argb1555", dir, p);
        d = read_all(path, &sz);
        if(d && sz == (uint32_t)w * h * 2) a->argb[p] = (uint16_t *)d;
        else free(d);
    }
    ok = atlas_build(a, n_images);
    if(!(a->flags & SPR_ATLAS_KEEP))
        for(p = 0; p < SPR_ATLAS_PAGES; p++) {
            free(a->texels[p]); a->texels[p] = NULL;
            free(a->argb[p]); a->argb[p] = NULL;
        }
    return ok;
}

int spr_atlas_adopt(SprAtlas *a, const uint8_t *dspr, const SprPal *pal, uint8_t *const *texels,
                    uint16_t *const *argb, int flags)
{
    int p;
    memset(a, 0, sizeof(*a));
    a->dspr = dspr;
    a->flags = flags | SPR_ATLAS_BORROWED | ((flags & SPR_ATLAS_LAZY) ? SPR_ATLAS_KEEP : 0);
    a->n_pages = dspr_count(dspr, 6);
    if(a->n_pages > SPR_ATLAS_PAGES) a->n_pages = SPR_ATLAS_PAGES;
    if(pal && pal->blob) a->pal = *pal;
    for(p = 0; p < a->n_pages; p++) {
        a->texels[p] = sprpal_page_is_pal4(&a->pal, p) ? texels[p] : NULL;
        a->argb[p] = a->texels[p] ? NULL : argb[p];
    }
    return atlas_build(a, dspr_count(dspr, 8));
}

void spr_atlas_free(SprAtlas *a)
{
    int p, i;
    for(p = 0; p < SPR_ATLAS_PAGES; p++) {
        if(a->tex[p].ptr) pvr_mem_free(a->tex[p].ptr);
        if(!(a->flags & SPR_ATLAS_BORROWED)) {
            free(a->texels[p]);
            free(a->argb[p]);
        } else if(a->own_argb[p])
            free(a->argb[p]);
    }
    if(a->row)
        for(i = 0; i < a->pal.n_pal; i++)
            if(a->row[i] >= 0) pal4_put(a->row[i]);
    free(a->row);
    free(a->ref);
    if(!(a->flags & SPR_ATLAS_BORROWED)) free(a->palblob);
    memset(a, 0, sizeof(*a));
}

int spr_atlas_pal_row(SprAtlas *a, int pal)
{
    if(!a->row || pal < 0 || pal >= a->pal.n_pal) return -1;
    if(a->flags & SPR_ATLAS_LAZY)
        return pal4_use(&a->ref[pal], a->pal.pal + 16 * pal, 0xfffe);
    if(a->row[pal] < 0) a->row[pal] = (int16_t)pal4_get(a->pal.pal + 16 * pal, 0xfffe);
    return a->row[pal];
}

int spr_atlas_bank(SprAtlas *a, int image, int pal)
{
    int page, own, r;
    if(image < 0 || image >= dspr_count(a->dspr, 8)) return -1;
    page = dspr_image(a->dspr, image)[0];
    if(page >= a->n_pages || !a->pal4[page]) return -1;
    own = a->pal.image_pal[image];
    if(pal >= 0 && pal != own) {
        r = spr_atlas_pal_row(a, pal);
        if(r >= 0) return r;
    }
    if(own == SPRPAL_NONE) return -1;
    if(a->flags & SPR_ATLAS_LAZY) {
        r = pal4_use(&a->ref[own], a->pal.pal + 16 * own, a->pal.pal_mask[own]);
        if(r < 0 && a->texels[page] && page_expand(a, page, a->texels[page]) == 0) return -1;
        return r;
    }
    return a->row[own];
}

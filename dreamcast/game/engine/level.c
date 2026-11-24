#include "level.h"
#include "loading.h"
#include "render.h"
#include <kos.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static uint8_t *read_file(const char *path, uint32_t *size)
{
    file_t f = fs_open(path, O_RDONLY);
    uint8_t *buf;
    if(f == FILEHND_INVALID) return NULL;
    *size = fs_total(f);
    buf = memalign(32, *size);
    if(buf && loading_read(f, buf, *size) != (ssize_t)*size) { free(buf); buf = NULL; }
    fs_close(f);
    return buf;
}

int level_load(Level *lv, const char *dir)
{
    char path[128];
    uint32_t sz;
    uint8_t *bt;
    const rbtx_header_t *th;
    int i;

    memset(lv, 0, sizeof(*lv));
    lv->pal_bank = lv->bg_pal_bank = -1;
    lv->alt_pal_bank = lv->alt_bg_pal_bank = -1;
    snprintf(path, sizeof(path), "%s/level.bin", dir);
    lv->data = read_file(path, &lv->size);
    if(!lv->data) return -1;
    lv->hdr = (const rlvl_header_t *)lv->data;
    if(lv->hdr->magic != RLVL_MAGIC) { level_free(lv); return -2; }
    lv->layer[0] = (const uint16_t *)(lv->data + lv->hdr->sec[RLVL_SEC_LAYER_A].offset);
    lv->layer[1] = (const uint16_t *)(lv->data + lv->hdr->sec[RLVL_SEC_LAYER_B].offset);

    snprintf(path, sizeof(path), "%s/blocktex.bin", dir);
    bt = read_file(path, &sz);
    if(!bt) { level_free(lv); return -3; }
    th = (const rbtx_header_t *)bt;
    lv->num_pages = th->num_pages;
    lv->pages = calloc(lv->num_pages, sizeof(pvr_ptr_t));
    for(i = 0; i < lv->num_pages; i++) {
        uint32_t psz = th->page_size * th->page_size;
        lv->pages[i] = pvr_mem_malloc(psz);
        if(!lv->pages[i]) { free(bt); level_free(lv); return -4; }
        pvr_txr_load(bt + th->page_offset + i * psz, lv->pages[i], psz);
        loading_tick();
        lv->vram_used += psz;
        lv->pal_rows |= pal8_rowmask(bt + th->page_offset + i * psz, psz);
    }
    free(bt);
    /* only the palette rows the block textures use are taken from the palette RAM */
    lv->pal_bank = pal8_alloc((const uint16_t *)(lv->data + lv->hdr->sec[RLVL_SEC_PALETTE].offset), lv->pal_rows);
    if(lv->pal_bank < 0) { level_free(lv); return -5; }

    if(lv->hdr->flags & RLVL_FLAG_HAS_BG) {
        uint8_t *bg;
        const rbgd_header_t *bh;
        snprintf(path, sizeof(path), "%s/bg.bin", dir);
        bg = read_file(path, &sz);
        if(bg) {
            bh = (const rbgd_header_t *)bg;
            lv->bg_w = bh->width_px; lv->bg_h = bh->height_px;
            lv->bg_tw = bh->tex_width; lv->bg_th = bh->tex_height;
            lv->bg_pal_rows = pal8_rowmask(bg + bh->texture_offset, lv->bg_tw * lv->bg_th);
            lv->bg_pal_bank = pal8_alloc((const uint16_t *)(bg + bh->palette_offset), lv->bg_pal_rows);
            lv->bg_tex = lv->bg_pal_bank >= 0 ? pvr_mem_malloc(lv->bg_tw * lv->bg_th) : NULL;
            if(lv->bg_tex) {
                pvr_txr_load(bg + bh->texture_offset, lv->bg_tex, lv->bg_tw * lv->bg_th);
                lv->vram_used += lv->bg_tw * lv->bg_th;
            }
            /* every palette stays in RAM for the zone effects (alternate palettes = the
             * palette animation sources, ZoneFx_InitPaletteAnim) */
            lv->bg_num_pals = bh->num_palettes ? bh->num_palettes : 1;
            lv->bg_pals = malloc(lv->bg_num_pals * 512);
            if(lv->bg_pals) memcpy(lv->bg_pals, bg + bh->palette_offset, lv->bg_num_pals * 512);
            else lv->bg_num_pals = 0;
            free(bg);
        }
    }
    return 0;
}

void level_free(Level *lv)
{
    int i;
    for(i = 0; lv->pages && i < lv->num_pages; i++) if(lv->pages[i]) pvr_mem_free(lv->pages[i]);
    if(lv->bg_tex) pvr_mem_free(lv->bg_tex);
    if(lv->pal_bank >= 0) pal8_free(lv->pal_bank, lv->pal_rows);
    if(lv->bg_pal_bank >= 0) pal8_free(lv->bg_pal_bank, lv->bg_pal_rows);
    if(lv->alt_pal_bank >= 0) pal8_free(lv->alt_pal_bank, lv->pal_rows);
    if(lv->alt_bg_pal_bank >= 0) pal8_free(lv->alt_bg_pal_bank, lv->bg_pal_rows);
    free(lv->bg_pals);
    free(lv->pages);
    free(lv->data);
    memset(lv, 0, sizeof(*lv));
    lv->pal_bank = lv->bg_pal_bank = -1;
    lv->alt_pal_bank = lv->alt_bg_pal_bank = -1;
}

int level_width_px(const Level *lv) { return lv->hdr->width_blocks * RLVL_BLOCK_PX; }
int level_height_px(const Level *lv) { return lv->hdr->height_blocks * RLVL_BLOCK_PX; }

static void pal_quad(pvr_ptr_t tex, int tw, int th, int bank, pvr_list_t list,
                     float x, float y, float w, float h, float u0, float v0, float u1, float v1, float z)
{
    pvr_poly_cxt_t cxt;
    pvr_poly_hdr_t hdr;
    pvr_vertex_t v;
    pvr_poly_cxt_txr(&cxt, list, PVR_TXRFMT_PAL8BPP | PVR_TXRFMT_8BPP_PAL(bank) | PVR_TXRFMT_TWIDDLED,
                     tw, th, tex, PVR_FILTER_NONE);
    pvr_poly_compile(&hdr, &cxt);
    render_hdr_clip(&hdr);
    pvr_prim(&hdr, sizeof(hdr));
    v.flags = PVR_CMD_VERTEX; v.argb = 0xffffffff; v.oargb = 0; v.z = z;
    v.x = x;     v.y = y + h; v.u = u0; v.v = v1; pvr_prim(&v, sizeof(v));
    v.x = x;     v.y = y;     v.u = u0; v.v = v0; pvr_prim(&v, sizeof(v));
    v.x = x + w; v.y = y + h; v.u = u1; v.v = v1; pvr_prim(&v, sizeof(v));
    v.flags = PVR_CMD_VERTEX_EOL;
    v.x = x + w; v.y = y;     v.u = u1; v.v = v0; pvr_prim(&v, sizeof(v));
}

void level_draw_layer(const Level *lv, int layer, pvr_list_t list, float z)
{
    const float bs = RLVL_BLOCK_PX * VIEW_SCALE;
    int bx0 = (int)(g_cam_x / RLVL_BLOCK_PX), by0 = (int)(g_cam_y / RLVL_BLOCK_PX);
    int bx1 = (int)((g_cam_x + VIEW_W) / RLVL_BLOCK_PX), by1 = (int)((g_cam_y + VIEW_H) / RLVL_BLOCK_PX);
    int bx, by, W = lv->hdr->width_blocks, H = lv->hdr->height_blocks;
    const float ps = 512.0f, inset = 0.25f;
    for(by = by0; by <= by1; by++) {
        if(by < 0 || by >= H) continue;
        for(bx = bx0; bx <= bx1; bx++) {
            int id, page, slot;
            float u0, v0, sx, sy;
            if(bx < 0 || bx >= W) continue;
            id = lv->layer[layer][by * W + bx];
            if(id == 0 || id >= lv->hdr->num_blocks) continue;
            page = id / 64; slot = id % 64;
            if(page >= lv->num_pages) continue;
            u0 = (slot % 8) * 64.0f; v0 = (slot / 8) * 64.0f;
            sx = VIEW_OX + (bx * RLVL_BLOCK_PX - g_cam_x) * VIEW_SCALE;
            sy = VIEW_OY + (by * RLVL_BLOCK_PX - g_cam_y) * VIEW_SCALE;
            pal_quad(lv->pages[page], 512, 512, lv->pal_bank, list, sx, sy, bs, bs,
                     (u0 + inset) / ps, (v0 + inset) / ps, (u0 + 64 - inset) / ps, (v0 + 64 - inset) / ps, z);
        }
    }
}

void level_draw_bg(const Level *lv, float fx, float fy, float z)
{
    float ox, oy, x, y;
    if(!lv->bg_tex) return;
    ox = -(g_cam_x * fx); oy = -(g_cam_y * fy);
    ox = ox - (int)(ox / lv->bg_w) * lv->bg_w; if(ox > 0) ox -= lv->bg_w;
    oy = oy - (int)(oy / lv->bg_h) * lv->bg_h; if(oy > 0) oy -= lv->bg_h;
    for(y = oy; y < VIEW_H; y += lv->bg_h)
        for(x = ox; x < VIEW_W; x += lv->bg_w)
            pal_quad(lv->bg_tex, lv->bg_tw, lv->bg_th, lv->bg_pal_bank, PVR_LIST_OP_POLY,
                     VIEW_OX + x * VIEW_SCALE, VIEW_OY + y * VIEW_SCALE, lv->bg_w * VIEW_SCALE, lv->bg_h * VIEW_SCALE,
                     0, 0, (float)lv->bg_w / lv->bg_tw, (float)lv->bg_h / lv->bg_th, z);
}

/* ------------------------------------------------------------------ zone effects */
static pvr_poly_hdr_t s_bg_hdr[2];

static void bg_hdr(const Level *lv, int bank, pvr_poly_hdr_t *hdr)
{
    pvr_poly_cxt_t cxt;
    pvr_poly_cxt_txr(&cxt, PVR_LIST_OP_POLY, PVR_TXRFMT_PAL8BPP | PVR_TXRFMT_8BPP_PAL(bank) | PVR_TXRFMT_TWIDDLED,
                     lv->bg_tw, lv->bg_th, lv->bg_tex, PVR_FILTER_NONE);
    cxt.txr.uv_clamp = PVR_UVCLAMP_NONE;   /* the BG wraps (DS 512x512 / 256x512 text BG) */
    pvr_poly_compile(hdr, &cxt);
    render_hdr_clip(hdr);
}

/* rows [r0, r1) of the window with BG offset (x, y): one quad, the texture repeats */
static void bg_strip(const Level *lv, int r0, int r1, int x, int y, float z, uint32_t argb)
{
    pvr_vertex_t v;
    float u0 = (float)(x & (lv->bg_w - 1)) / lv->bg_tw;
    float u1 = u0 + VIEW_W / lv->bg_tw;
    float v0 = (float)((r0 + y) & (lv->bg_h - 1)) / lv->bg_th;
    float v1 = v0 + (float)(r1 - r0) / lv->bg_th;
    float y0 = VIEW_OY + r0 * VIEW_SCALE, y1 = VIEW_OY + r1 * VIEW_SCALE;
    float x0 = VIEW_OX, x1 = VIEW_OX + g_view_w;
    v.flags = PVR_CMD_VERTEX; v.argb = argb; v.oargb = 0; v.z = z;
    v.x = x0; v.y = y1; v.u = u0; v.v = v1; pvr_prim(&v, sizeof(v));
    v.x = x0; v.y = y0; v.u = u0; v.v = v0; pvr_prim(&v, sizeof(v));
    v.x = x1; v.y = y1; v.u = u1; v.v = v1; pvr_prim(&v, sizeof(v));
    v.flags = PVR_CMD_VERTEX_EOL;
    v.x = x1; v.y = y0; v.u = u1; v.v = v0; pvr_prim(&v, sizeof(v));
}

void level_draw_bg_lines(const Level *lv, const LevelBgLine *lines, int alt_row, uint32_t alt_argb, float z)
{
    int r, r0, k, cur = -1;
    if(!lv->bg_tex || lv->bg_pal_bank < 0) return;
    if(lv->bg_w != lv->bg_tw || lv->bg_h != lv->bg_th) return;   /* the converter makes them equal */
    bg_hdr(lv, lv->bg_pal_bank, &s_bg_hdr[0]);
    if(lv->alt_bg_pal_bank >= 0) bg_hdr(lv, lv->alt_bg_pal_bank, &s_bg_hdr[1]);
    /* window row w shows DS row w - g_view_row_ofs; rows outside the DS table continue the nearest
     * edge row (same horizontal offset, the next BG lines) */
    {
    int nrow = (int)(VIEW_H + 0.5f), ofs = g_view_row_ofs;
#define BGX(w) (lines[(w) - ofs < 0 ? 0 : (w) - ofs > 191 ? 191 : (w) - ofs].x & 0x1ff)
#define BGY(w) ((lines[(w) - ofs < 0 ? 0 : (w) - ofs > 191 ? 191 : (w) - ofs].y + \
                 ((w) - ofs < 0 ? (w) - ofs : (w) - ofs > 191 ? (w) - ofs - 191 : 0) - ofs) & 0x1ff)
    if(nrow < 1) nrow = 1;
    for(r0 = 0; r0 < nrow; r0 = r) {
        int x = BGX(r0), y = BGY(r0);
        k = r0 - ofs >= alt_row;
        /* rows with the same offset; a strip ends where the texture would wrap or the bank changes */
        for(r = r0 + 1; r < nrow; r++) {
            if(BGX(r) != x || BGY(r) != y) break;
            if((r - ofs >= alt_row) != k) break;
            if(((r + y) & (lv->bg_h - 1)) == 0) break;
        }
        /* rows below alt_row: the second bank, or the normal one with the tint colour */
        if(cur != k) { pvr_prim(&s_bg_hdr[k && lv->alt_bg_pal_bank >= 0], sizeof(pvr_poly_hdr_t)); cur = k; }
        bg_strip(lv, r0, r, x, y, z, (k && lv->alt_bg_pal_bank < 0) ? alt_argb : 0xffffffff);
    }
#undef BGX
#undef BGY
    }
}

/* pal_quad with a vertex colour (MODULATE): the tint when no second bank was free */
static void pal_quad_c(pvr_ptr_t tex, int bank, pvr_list_t list, float x, float y, float w, float h,
                       float u0, float v0, float u1, float v1, float z, uint32_t argb)
{
    pvr_poly_cxt_t cxt;
    pvr_poly_hdr_t hdr;
    pvr_vertex_t v;
    pvr_poly_cxt_txr(&cxt, list, PVR_TXRFMT_PAL8BPP | PVR_TXRFMT_8BPP_PAL(bank) | PVR_TXRFMT_TWIDDLED,
                     512, 512, tex, PVR_FILTER_NONE);
    pvr_poly_compile(&hdr, &cxt);
    render_hdr_clip(&hdr);
    pvr_prim(&hdr, sizeof(hdr));
    v.flags = PVR_CMD_VERTEX; v.argb = argb; v.oargb = 0; v.z = z;
    v.x = x;     v.y = y + h; v.u = u0; v.v = v1; pvr_prim(&v, sizeof(v));
    v.x = x;     v.y = y;     v.u = u0; v.v = v0; pvr_prim(&v, sizeof(v));
    v.x = x + w; v.y = y + h; v.u = u1; v.v = v1; pvr_prim(&v, sizeof(v));
    v.flags = PVR_CMD_VERTEX_EOL;
    v.x = x + w; v.y = y;     v.u = u1; v.v = v0; pvr_prim(&v, sizeof(v));
}

void level_draw_layer_split(const Level *lv, int layer, pvr_list_t list, float z, float split_y, uint32_t alt_argb)
{
    int alt_bank = lv->alt_pal_bank >= 0 ? lv->alt_pal_bank : lv->pal_bank;
    uint32_t alt_c = lv->alt_pal_bank >= 0 ? 0xffffffff : alt_argb;
    const float bs = RLVL_BLOCK_PX * VIEW_SCALE;
    int bx0 = (int)(g_cam_x / RLVL_BLOCK_PX), by0 = (int)(g_cam_y / RLVL_BLOCK_PX);
    int bx1 = (int)((g_cam_x + VIEW_W) / RLVL_BLOCK_PX), by1 = (int)((g_cam_y + VIEW_H) / RLVL_BLOCK_PX);
    int bx, by, W = lv->hdr->width_blocks, H = lv->hdr->height_blocks;
    const float ps = 512.0f, inset = 0.25f;
    if(split_y >= g_cam_y + VIEW_H || (lv->alt_pal_bank < 0 && alt_argb == 0xffffffff)) { level_draw_layer(lv, layer, list, z); return; }
    for(by = by0; by <= by1; by++) {
        float wy0 = by * RLVL_BLOCK_PX, wy1 = wy0 + RLVL_BLOCK_PX;
        if(by < 0 || by >= H) continue;
        for(bx = bx0; bx <= bx1; bx++) {
            int id, page, slot;
            float u0, v0, sx, sy;
            if(bx < 0 || bx >= W) continue;
            id = lv->layer[layer][by * W + bx];
            if(id == 0 || id >= lv->hdr->num_blocks) continue;
            page = id / 64; slot = id % 64;
            if(page >= lv->num_pages) continue;
            u0 = (slot % 8) * 64.0f; v0 = (slot / 8) * 64.0f;
            sx = VIEW_OX + (bx * RLVL_BLOCK_PX - g_cam_x) * VIEW_SCALE;
            sy = VIEW_OY + (wy0 - g_cam_y) * VIEW_SCALE;
            if(wy1 <= split_y) {
                pal_quad(lv->pages[page], 512, 512, lv->pal_bank, list, sx, sy, bs, bs,
                         (u0 + inset) / ps, (v0 + inset) / ps, (u0 + 64 - inset) / ps, (v0 + 64 - inset) / ps, z);
            } else if(wy0 >= split_y) {
                pal_quad_c(lv->pages[page], alt_bank, list, sx, sy, bs, bs,
                           (u0 + inset) / ps, (v0 + inset) / ps, (u0 + 64 - inset) / ps, (v0 + 64 - inset) / ps, z, alt_c);
            } else {
                /* the block crosses the water line: two parts, the lower one tinted */
                float h0 = split_y - wy0;
                pal_quad(lv->pages[page], 512, 512, lv->pal_bank, list, sx, sy, bs, h0 * VIEW_SCALE,
                         (u0 + inset) / ps, (v0 + inset) / ps, (u0 + 64 - inset) / ps, (v0 + h0) / ps, z);
                pal_quad_c(lv->pages[page], alt_bank, list, sx, sy + h0 * VIEW_SCALE, bs, (64 - h0) * VIEW_SCALE,
                           (u0 + inset) / ps, (v0 + h0) / ps, (u0 + 64 - inset) / ps, (v0 + 64 - inset) / ps, z, alt_c);
            }
        }
    }
}

static void set_rows(int bank, uint16_t rows, const uint16_t *pal)
{
    int r, i;
    if(bank < 0 || !pal) return;
    for(r = 0; r < 16; r++)
        if(rows & (1u << r))
            for(i = 0; i < 16; i++) pvr_set_pal_entry(bank * 256 + r * 16 + i, pal[r * 16 + i]);
}
void level_set_palette(const Level *lv, int alt, const uint16_t *pal)
{
    set_rows(alt ? lv->alt_pal_bank : lv->pal_bank, lv->pal_rows, pal);
}
void level_set_bg_palette(const Level *lv, int alt, const uint16_t *pal)
{
    set_rows(alt ? lv->alt_bg_pal_bank : lv->bg_pal_bank, lv->bg_pal_rows, pal);
}
int level_alloc_alt_banks(Level *lv, const uint16_t *pal, const uint16_t *bg_pal)
{
    if(lv->alt_pal_bank < 0 && lv->pal_rows) lv->alt_pal_bank = pal8_alloc(pal, lv->pal_rows);
    if(lv->alt_bg_pal_bank < 0 && lv->bg_pal_rows && bg_pal) lv->alt_bg_pal_bank = pal8_alloc(bg_pal, lv->bg_pal_rows);
    return (lv->alt_pal_bank >= 0 && (lv->alt_bg_pal_bank >= 0 || !lv->bg_pal_rows)) ? 0 : -1;
}
const uint16_t *level_palette(const Level *lv)
{
    return (const uint16_t *)(lv->data + lv->hdr->sec[RLVL_SEC_PALETTE].offset);
}

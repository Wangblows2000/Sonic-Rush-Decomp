#include "level.h"
#include "render.h"
#include <kos.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LEVEL_PAL_BANK 0
#define BG_PAL_BANK 1

static uint8_t *read_file(const char *path, uint32_t *size)
{
    file_t f = fs_open(path, O_RDONLY);
    uint8_t *buf;
    if(f == FILEHND_INVALID) return NULL;
    *size = fs_total(f);
    buf = memalign(32, *size);
    if(buf && fs_read(f, buf, *size) != (ssize_t)*size) { free(buf); buf = NULL; }
    fs_close(f);
    return buf;
}

static void load_palette(const uint16_t *pal, int bank)
{
    int i;
    pvr_set_pal_format(PVR_PAL_ARGB1555);
    for(i = 0; i < 256; i++)
        pvr_set_pal_entry(bank * 256 + i, pal[i]);
}

int level_load(Level *lv, const char *dir)
{
    char path[128];
    uint32_t sz;
    uint8_t *bt;
    const rbtx_header_t *th;
    int i;

    memset(lv, 0, sizeof(*lv));
    snprintf(path, sizeof(path), "%s/level.bin", dir);
    lv->data = read_file(path, &lv->size);
    if(!lv->data) return -1;
    lv->hdr = (const rlvl_header_t *)lv->data;
    if(lv->hdr->magic != RLVL_MAGIC) return -2;
    lv->layer[0] = (const uint16_t *)(lv->data + lv->hdr->sec[RLVL_SEC_LAYER_A].offset);
    lv->layer[1] = (const uint16_t *)(lv->data + lv->hdr->sec[RLVL_SEC_LAYER_B].offset);
    load_palette((const uint16_t *)(lv->data + lv->hdr->sec[RLVL_SEC_PALETTE].offset), LEVEL_PAL_BANK);

    snprintf(path, sizeof(path), "%s/blocktex.bin", dir);
    bt = read_file(path, &sz);
    if(!bt) return -3;
    th = (const rbtx_header_t *)bt;
    lv->num_pages = th->num_pages;
    lv->pages = calloc(lv->num_pages, sizeof(pvr_ptr_t));
    for(i = 0; i < lv->num_pages; i++) {
        uint32_t psz = th->page_size * th->page_size;
        lv->pages[i] = pvr_mem_malloc(psz);
        if(!lv->pages[i]) { free(bt); return -4; }
        pvr_txr_load(bt + th->page_offset + i * psz, lv->pages[i], psz);
        lv->vram_used += psz;
    }
    free(bt);

    if(lv->hdr->flags & RLVL_FLAG_HAS_BG) {
        uint8_t *bg;
        const rbgd_header_t *bh;
        snprintf(path, sizeof(path), "%s/bg.bin", dir);
        bg = read_file(path, &sz);
        if(bg) {
            bh = (const rbgd_header_t *)bg;
            lv->bg_w = bh->width_px; lv->bg_h = bh->height_px;
            lv->bg_tw = bh->tex_width; lv->bg_th = bh->tex_height;
            load_palette((const uint16_t *)(bg + bh->palette_offset), BG_PAL_BANK);
            lv->bg_tex = pvr_mem_malloc(lv->bg_tw * lv->bg_th);
            if(lv->bg_tex) {
                pvr_txr_load(bg + bh->texture_offset, lv->bg_tex, lv->bg_tw * lv->bg_th);
                lv->vram_used += lv->bg_tw * lv->bg_th;
            }
            free(bg);
        }
    }
    return 0;
}

void level_free(Level *lv)
{
    int i;
    for(i = 0; i < lv->num_pages; i++) if(lv->pages[i]) pvr_mem_free(lv->pages[i]);
    if(lv->bg_tex) pvr_mem_free(lv->bg_tex);
    free(lv->pages);
    free(lv->data);
    memset(lv, 0, sizeof(*lv));
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
            sx = (bx * RLVL_BLOCK_PX - g_cam_x) * VIEW_SCALE;
            sy = (by * RLVL_BLOCK_PX - g_cam_y) * VIEW_SCALE;
            pal_quad(lv->pages[page], 512, 512, LEVEL_PAL_BANK, list, sx, sy, bs, bs,
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
            pal_quad(lv->bg_tex, lv->bg_tw, lv->bg_th, BG_PAL_BANK, PVR_LIST_OP_POLY,
                     x * VIEW_SCALE, y * VIEW_SCALE, lv->bg_w * VIEW_SCALE, lv->bg_h * VIEW_SCALE,
                     0, 0, (float)lv->bg_w / lv->bg_tw, (float)lv->bg_h / lv->bg_th, z);
}

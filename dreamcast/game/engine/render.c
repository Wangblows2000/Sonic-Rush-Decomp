#include "render.h"
#include <kos.h>
#include <string.h>
#include <stdlib.h>

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
    buf = memalign(32, t->bytes);
    if(!buf || fs_read(f, buf, t->bytes) != (ssize_t)t->bytes) { free(buf); fs_close(f); return -1; }
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

void render_begin_frame(void) { pvr_wait_ready(); pvr_scene_begin(); }
void render_end_frame(void) { pvr_scene_finish(); }
void render_begin_list(pvr_list_t list) { s_list = list; pvr_list_begin(list); }
void render_end_list(void) { pvr_list_finish(); }

static void quad(const Texture *t, pvr_list_t list, float x, float y, float w, float h,
                 float u0, float v0, float u1, float v1, float z, uint32_t argb)
{
    pvr_poly_cxt_t cxt;
    pvr_poly_hdr_t hdr;
    pvr_vertex_t v;
    if(t && t->ptr) {
        pvr_poly_cxt_txr(&cxt, list, t->fmt | PVR_TXRFMT_NONTWIDDLED, t->w, t->h, t->ptr, PVR_FILTER_NONE);
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
    float sx = (wx - g_cam_x) * VIEW_SCALE, sy = (wy - g_cam_y) * VIEW_SCALE;
    float sw = w * VIEW_SCALE, sh = h * VIEW_SCALE, tmp;
    if(sx > 640 || sy > 480 || sx + sw < 0 || sy + sh < 0) return;
    if(flip & 1) { tmp = u0; u0 = u1; u1 = tmp; }
    if(flip & 2) { tmp = v0; v0 = v1; v1 = tmp; }
    quad(t, list, sx, sy, sw, sh, u0, v0, u1, v1, z, argb);
}

void draw_rect_screen(const Texture *t, pvr_list_t list, float x, float y, float w, float h,
                      float u0, float v0, float u1, float v1, float z, uint32_t argb)
{
    quad(t, list, x, y, w, h, u0, v0, u1, v1, z, argb);
}

uint32_t render_vram_free(void) { return pvr_mem_available(); }

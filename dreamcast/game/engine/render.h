/* Native PowerVR2 renderer for Sonic Rush DC.
 * World space = original DS pixels. One DS screen is 256x192; the Dreamcast
 * view shows a 256x192 world window scaled by 2.5 to 640x480 (CAMERA.md). */
#ifndef SRDC_RENDER_H
#define SRDC_RENDER_H
#include <stdint.h>
#include <dc/pvr.h>

#define VIEW_W 256.0f
#define VIEW_H 192.0f
#define VIEW_SCALE 2.5f

typedef struct {
    pvr_ptr_t ptr;
    int w, h;          /* power-of-two texture size */
    int fmt;           /* PVR_TXRFMT_* */
    uint32_t bytes;
} Texture;

/* Raw texture file: "DCTX" u16 w, u16 h, u16 pvrfmt(0=ARGB1555,1=RGB565,2=ARGB4444), u16 pad, data (non-twiddled). */
int  tex_load(Texture *t, const char *path);
void tex_free(Texture *t);

void render_begin_frame(void);
void render_end_frame(void);
void render_begin_list(pvr_list_t list);
void render_end_list(void);

/* Camera: top-left of the 256x192 world window, in world pixels (float for smooth scrolling). */
extern float g_cam_x, g_cam_y;

/* Draw a textured rect in world space. (u0,v0,u1,v1) in texels. flip bit0 = H, bit1 = V. */
void draw_sprite_world(const Texture *t, pvr_list_t list, float wx, float wy, float w, float h,
                       float u0, float v0, float u1, float v1, int flip, float z, uint32_t argb);
/* Screen-space (640x480) rect for HUD/menus; tex may be NULL for flat colour. */
void draw_rect_screen(const Texture *t, pvr_list_t list, float x, float y, float w, float h,
                      float u0, float v0, float u1, float v1, float z, uint32_t argb);

uint32_t render_vram_free(void);
#endif

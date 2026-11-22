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
    int fmt;           /* PVR_TXRFMT_* pixel format (ARGB1555, ..., PAL4BPP, PAL8BPP) */
    uint32_t bytes;
    int twiddled;      /* 0: non-twiddled (every ARGB texture of the game); paletted: 1 */
} Texture;

/* Texture-format word of a texture for pvr_poly_cxt_txr: twiddle bit and, for the
 * paletted formats, the palette bank (PAL4: row 0..63 of 16 entries; PAL8: 0..3). */
int tex_txr_format(const Texture *t, int bank);

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

/* The same with a palette bank for a PAL4 / PAL8 texture (bank < 0: ignored). */
void draw_sprite_world_pal(const Texture *t, int bank, pvr_list_t list, float wx, float wy, float w, float h,
                           float u0, float v0, float u1, float v1, int flip, float z, uint32_t argb);
void draw_rect_screen_pal(const Texture *t, int bank, pvr_list_t list, float x, float y, float w, float h,
                          float u0, float v0, float u1, float v1, float z, uint32_t argb);

uint32_t render_vram_free(void);

/* ------------------------------------------------------------------ PVR palette RAM
 * 1024 ARGB1555 entries = 64 rows of 16, shared by every paletted texture: a PAL4
 * bank is one row, a PAL8 bank (level, BG) is 16 aligned rows of which only the
 * rows its texels use are taken. One allocator for the level and the sprites
 * (GRAPHICS.md "palette RAM"). Sprite banks are shared by content: a request whose
 * used entries (mask) agree with a bank's defined entries reuses that bank. */
#define PAL_ROWS 64
/* PAL8: the 256-entry bank (0..3) whose rows in rowmask (bit r = row r) are all
 * free, rows written from pal[256]; -1 if none. */
int  pal8_alloc(const uint16_t *pal, uint16_t rowmask);
void pal8_free(int bank, uint16_t rowmask);
uint16_t pal8_rowmask(const uint8_t *texels, uint32_t n);   /* rows a PAL8 texel array uses */
/* PAL4, counted reference (until pal4_put): row or -1 when the palette RAM is full. */
int  pal4_get(const uint16_t col[16], uint16_t mask);
void pal4_put(int row);
/* PAL4 for drawing only (UI): the row is kept while it is used; one not used for 3
 * frames may be taken back for another request. ref holds the row and its generation. */
typedef struct { int16_t row; uint16_t gen; } PalRef;
#define PALREF_INIT { -1, 0 }
int  pal4_use(PalRef *ref, const uint16_t col[16], uint16_t mask);
int  pal_rows_free(void);          /* budget probe: free rows (incl. idle UI rows) */
int  pal_rows_used(int *pal8, int *pal4);

/* ------------------------------------------------------------------ sprite atlases
 * The pages of one converted BAC (sprpal.h): PAL4 pages from atlas<N>.pt4 when
 * palettes.bin is there, else (or for direct pages) atlas<N>.argb1555.
 *   static (flags 0): every palette of the PAL4 pages gets a row at load; a page
 *     whose palettes find no free row is expanded to ARGB1555 (exact, 4x the VRAM);
 *   SPR_ATLAS_LAZY: rows are taken on first draw (spr_atlas_bank) and given back
 *     when idle; the PAL4 texels stay in RAM so a page can still be expanded.
 * dspr = the frames.bin blob (images give the page / rect of every palette). */
#define SPR_ATLAS_PAGES 4
#define SPR_ATLAS_LAZY 1
#define SPR_ATLAS_KEEP 2           /* keep the PAL4 texels (and ARGB1555 pages) in RAM */
#define SPR_ATLAS_BORROWED 4       /* spr_atlas_adopt: the caller owns the page data / palettes */
#include "sprpal.h"
typedef struct SprAtlas {
    int n_pages, flags;
    Texture tex[SPR_ATLAS_PAGES];
    uint8_t pal4[SPR_ATLAS_PAGES];        /* page drawn as PAL4 */
    uint8_t *texels[SPR_ATLAS_PAGES];     /* PAL4 data (KEEP / LAZY) */
    uint16_t *argb[SPR_ATLAS_PAGES];      /* ARGB1555 page data (KEEP, direct pages) */
    uint8_t own_argb[SPR_ATLAS_PAGES];
    uint8_t *palblob;
    SprPal pal;
    int16_t *row;                         /* static rows per palette, -1 none */
    PalRef *ref;                          /* lazy rows per palette */
    const uint8_t *dspr;
    uint32_t vram;                        /* bytes of PVR texture memory held */
} SprAtlas;
int  spr_atlas_load(SprAtlas *a, const char *dir, const uint8_t *dspr, int flags);
/* the same from data the caller loaded and keeps (texels[p] PAL4 / argb[p] ARGB1555
 * per page, pal parsed palettes.bin or NULL); it must outlive the atlas */
int  spr_atlas_adopt(SprAtlas *a, const uint8_t *dspr, const SprPal *pal, uint8_t *const *texels,
                     uint16_t *const *argb, int flags);
void spr_atlas_free(SprAtlas *a);
/* bank to draw image `image` with (pal: palette override, -1 = the image's own);
 * -1 = the page is ARGB1555. The texture is &a->tex[page]. */
int  spr_atlas_bank(SprAtlas *a, int image, int pal);
/* the row of palette `pal` with every entry defined (PaletteAnim steps, overrides) */
int  spr_atlas_pal_row(SprAtlas *a, int pal);
#endif

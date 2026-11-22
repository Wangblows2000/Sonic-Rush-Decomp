/* Native stage loader + renderer: reads the RLVL/RBTX/RBGD packages made by
 * tools/dreamcast/stages/stage2dc.py from the owner's ROM. */
#ifndef SRDC_LEVEL_H
#define SRDC_LEVEL_H
#include <stdint.h>
#include <dc/pvr.h>
#include "../../../tools/dreamcast/stages/level_format.h"

typedef struct Level {
    uint8_t *data;             /* whole level.bin in RAM (collision queries read it) */
    uint32_t size;
    const rlvl_header_t *hdr;
    const uint16_t *layer[2];  /* A (front), B (back) block ids */
    pvr_ptr_t *pages;          /* blocktex pages (512x512 PAL8 twiddled) */
    int num_pages;
    pvr_ptr_t bg_tex;
    int bg_w, bg_h, bg_tw, bg_th;
    uint32_t vram_used;
    int pal_bank, bg_pal_bank;  /* PAL8 banks from the shared palette RAM allocator (-1 none) */
    uint16_t pal_rows, bg_pal_rows; /* rows of those banks the texels use */
    /* zone effects (dreamcast/game/zonefx): every bg.bin palette (0 = the BG, 1.. = the
     * alternate z4pn / z5pn / z7pn palettes of the event archive), ARGB1555 */
    uint16_t *bg_pals;
    int bg_num_pals;
    /* a second bank of the same rows for the map tiles and the BG (Water Palace: the
     * tinted palettes below the water line; -1 none) */
    int alt_pal_bank, alt_bg_pal_bank;
} Level;

/* One screen row of the BG (DS BG1HOFS / BG1VOFS of that line, 9 bits used): pixel
 * (sx, row) of the 256x192 window shows BG image pixel ((sx + x) & (w - 1), (row + y) & (h - 1)). */
typedef struct { uint16_t x, y; } LevelBgLine;

int  level_load(Level *lv, const char *dir);  /* dir e.g. "/cd/stages/z11" */
void level_free(Level *lv);
int  level_width_px(const Level *lv);
int  level_height_px(const Level *lv);
/* Draw layer (0=A front, 1=B back) for the current camera into a list. */
void level_draw_layer(const Level *lv, int layer, pvr_list_t list, float z);
/* Draw the background with a parallax factor (fixed per zone, STAGES.md). */
void level_draw_bg(const Level *lv, float fx, float fy, float z);
/* The BG as the DS line scroll shows it: lines[0..191] per window row (rows with the same
 * offset are one strip); rows >= alt_row use the alternate bank, or, when none was free,
 * the normal bank modulated by the vertex colour alt_argb. */
void level_draw_bg_lines(const Level *lv, const LevelBgLine *lines, int alt_row, uint32_t alt_argb, float z);
/* Layer drawing with the alternate tile bank (or alt_argb, see above) below world row split_y */
void level_draw_layer_split(const Level *lv, int layer, pvr_list_t list, float z, float split_y, uint32_t alt_argb);
/* Palette RAM updates (zone palette animations, BLDY darkness, water tint): the 256
 * ARGB1555 entries of the map tiles / the BG; only the rows the texels use are written.
 * alt = the alternate bank. */
void level_set_palette(const Level *lv, int alt, const uint16_t *pal);
void level_set_bg_palette(const Level *lv, int alt, const uint16_t *pal);
/* Allocate the alternate banks (same rows as the normal ones); 0 = both allocated */
int  level_alloc_alt_banks(Level *lv, const uint16_t *pal, const uint16_t *bg_pal);
const uint16_t *level_palette(const Level *lv);   /* the map palette as loaded */
#endif

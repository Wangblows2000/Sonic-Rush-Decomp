/* Native stage loader + renderer: reads the RLVL/RBTX/RBGD packages made by
 * tools/dreamcast/stages/stage2dc.py from the owner's ROM. */
#ifndef SRDC_LEVEL_H
#define SRDC_LEVEL_H
#include <stdint.h>
#include <dc/pvr.h>
#include "../../../tools/dreamcast/stages/level_format.h"

typedef struct {
    uint8_t *data;             /* whole level.bin in RAM (collision queries read it) */
    uint32_t size;
    const rlvl_header_t *hdr;
    const uint16_t *layer[2];  /* A (front), B (back) block ids */
    pvr_ptr_t *pages;          /* blocktex pages (512x512 PAL8 twiddled) */
    int num_pages;
    pvr_ptr_t bg_tex;
    int bg_w, bg_h, bg_tw, bg_th;
    uint32_t vram_used;
} Level;

int  level_load(Level *lv, const char *dir);  /* dir e.g. "/cd/stages/z11" */
void level_free(Level *lv);
int  level_width_px(const Level *lv);
int  level_height_px(const Level *lv);
/* Draw layer (0=A front, 1=B back) for the current camera into a list. */
void level_draw_layer(const Level *lv, int layer, pvr_list_t list, float z);
/* Draw the background with a parallax factor (fixed per zone, STAGES.md). */
void level_draw_bg(const Level *lv, float fx, float fy, float z);
#endif

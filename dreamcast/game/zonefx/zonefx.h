/*
 * Zone backgrounds and zone effects (Dreamcast port): ZoneBg_* and ZoneFx_*
 * (src/game/Stage/ZoneBackground, src/game/Zone/<zone>/Background), translated.
 *
 *   ZoneBg  per-zone BG1 scroll: the parallax rules of zones 1, 4 (x / 14, x / 18 and
 *           the map-height ratio), the line scroll of zones 2, 3, 5, 7 (bands above /
 *           below the water / quicksand / limit line, the underwater wave, the heat
 *           haze), the Altitude Limit cloud drift; for the DC window (one screen).
 *   ZoneFx  Night Carnival: 24 palette animations (neon lights, z4pnNN palettes) and the
 *           BLDY darkness (flash / ramp, the dark areas and switches);
 *           Huge Crisis: palette rotation, 2 animations (z5pnNN), the enemy-room alarm;
 *           Dead Line: 2 rotations, 2 animations (z7pnNN);
 *           Water Palace: the tinted palettes below the water line (VAlarm split ->
 *           alternate PAL8 banks), the shimmer cycle, the surface sprites;
 *           Mirage Road boss: the light flash (zonefx_boss3_lights).
 *
 * Palettes go through engine/level.c (the PAL8 banks of the level and the BG); the
 * BG is drawn by level_draw_bg_lines from the line table built here.
 */
#ifndef SRDC_ZONEFX_H
#define SRDC_ZONEFX_H

#include <stdint.h>
#include "../player/dcp_types.h"

struct Level;
typedef struct ZfxLine { uint16_t x, y; } ZfxLine;   /* == LevelBgLine */

/* /cd/zonefx/zonefx.bin (tools/zonefxdata.py); also gives the camera its shake tables */
int  zonefx_load_data(const char *path);
int  zonefx_load_data_mem(const void *data, uint32_t size);
/* ZoneBg_Init + ZoneFx_Init: after level_load, objects_init and camera_create.
 * lv may be NULL (host tests: palettes only in RAM). Installs the camera zone hook. */
void zonefx_init(struct Level *lv, int zone, int act);
void zonefx_shutdown(void);
/* ZoneBg_Update + ZoneFx_Update + the zone tasks (line scroll build, palette tasks):
 * called by the camera inside Camera_Main (camera_set_zone_hook) */
void zonefx_update(void);
/* OBJ_FX_ZONE4_BLDY (g_obj_hooks.effect): a = 0 flash(b), 1 ramp up(b >> 8, b & 0xff),
 * 2 ramp down */
void zonefx_bldy(s32 a, s32 b);
/* drawing (KOS): the BG (OP list), the map layers with the underwater bank, the
 * water surface (TR list). zonefx_draw_bg replaces level_draw_bg(lv, 0.25, 0.125, z). */
void zonefx_draw_bg(float z);
void zonefx_draw_layer(int layer, int list, float z);
void zonefx_draw_tr(void);
/* the palettes written to palette RAM this frame (call before the draws; KOS) */
void zonefx_commit_palettes(void);

/* state for tests / other modules */
const ZfxLine *zonefx_lines(void);       /* the 192 window rows of the BG */
int  zonefx_bldy_level(void);            /* current EVY (0..16) */
int  zonefx_water_row(void);             /* first tinted row of the window (192 = none) */
const uint16_t *zonefx_palette(int which);/* 0 = map tiles, 1 = BG, 2 / 3 = tinted (as shown) */
u32  zonefx_palette_updates(void);       /* palette RAM rewrites so far */
/* ZoneFx_Zone3Boss_Update (Mirage Road boss arena: gBossStatus & 1): the three light
 * colours (BGR555) the 3D lights use this frame; 0 = no change */
int  zonefx_boss3_lights(int boss_active, uint16_t out[3]);

#endif

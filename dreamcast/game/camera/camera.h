/*
 * Native stage camera (Dreamcast port of Sonic Rush) - public API.
 *
 * Translation of src/game/Camera (Camera_*, the gCamera task), the camera part of
 * Player_Last (Player_UpdateCamera, Player_UpdateCameraLead), ScreenShake_* and the
 * camera hooks of ZoneFx_Zone6 (Altitude Limit wave / locked camera). Field comments
 * give the gCamera offsets (layer i at gCamera + i * 0x64, globals at gCamera[2]).
 *
 * Two cameras run every frame (docs/dreamcast/CAMERA.md):
 *   - the DS screen pair (gCamera[0] = the player's screen, gCamera[1] = the other
 *     screen, Camera_InitDualScreen / Camera_UpdateYDualScreen with the split line,
 *     the screen swap and the partner offset). It is the original gameplay state
 *     (swap flag, restart camera, the views of the zone effects);
 *   - the single DC window (256x192 world px shown at 2.5x): the same Camera_* code
 *     for gCamera[0] in its single-screen form (Camera_UpdateY, no split line, no
 *     swap), i.e. the player's DS screen without the split-line pinning. Horizontal
 *     = gCamera[0].x exactly. Discontinuities (lead offset change on a swap, a look
 *     pan finished by the pair) are eased; the player is kept inside the window.
 *
 * Shared state: the gCamera fields the objects read or write live in g_obj_camera
 * (objects.h): flags, flags34, limit, player, ofs/def_ofs, bound, lock, area_objs,
 * water. Positions are private (g_obj_camera[i].x/y are the object views, filled
 * by objects_set_view). g_obj_camera[2].flags bit 0 (dual screen) stays clear for
 * the objects (their single-screen branches are the ones tested); the camera keeps
 * its own dual bit.
 *
 * Frame order (DS task order):
 *     camera_update();                          Camera_Main (task 0x80) + ZoneBg/ZoneFx hook
 *     camera_get_view(&x, &y);                  the DC window, 24.8
 *     objects_set_view(0, x, y); objects_set_view(1, x, y - 0xC000);
 *     objects_frame_begin(); dcp_player_frame(...);
 *     camera_player_last(&player);              Player_Last: Player_UpdateCamera(+Lead)
 *     objects_update(...);
 *     camera_frame_end();                       ScreenShake_Main
 *   (every frame, also paused)
 *     camera_apply();                           g_cam_x / g_cam_y for drawing (+ shake)
 */
#ifndef SRDC_CAMERA_H
#define SRDC_CAMERA_H

#include "../player/dcp_player.h"

/* gCamera[i] fields that are not mirrored in g_obj_camera */
typedef struct CamLayer {
    s32 x, y;         /* @0x04 @0x08 view top-left (24.8) */
    s32 px, py;       /* @0x0C @0x10 previous frame (Camera_Main) */
    s16 vx, vy;       /* @0x14 @0x16 one-frame displacement, added after the follow */
    s16 sx, sy;       /* @0x18 @0x1A pixel offset of the draw origin (-ScreenShake) */
    u8 spd_x, spd_y;  /* @0x1C @0x1D max move per frame (ClampXAroundTarget) */
    u16 angle;        /* @0x22 */
    s32 bg_x, bg_y;   /* @0x2C @0x30 zone BG scroll (ZoneBg_Update -> BG1 HOFS/VOFS) */
} CamLayer;

/* gCamera[2] globals */
typedef struct CamGlobal {
    u32 prev_flags;   /* @0xCC */
    s32 d0;           /* @0xD0 screen gap (0x50 px * scale) */
    s32 d4, d8;       /* @0xD4 @0xD8 partner screen offset (x, y: +-0x110 px) */
    s32 dc;           /* @0xDC split line override (flags & 8) */
    u16 map_w, map_h; /* data_ov000_0236abd8 +0xE +0x10: map size in px */
    s32 map_bound[4]; /* @0x114..@0x120 left, top, right, bottom (8 px inset) */
    u8 dual;          /* gCamera[2].flags & 1 (Camera_InitDualScreen) */
    u16 mode;         /* data_ov000_0236aab0 (4: fixed camera, boss stages) */
} CamGlobal;

/* Camera_Main task work (Camera_Look_*) */
typedef struct CamLook {
    u32 flags;        /* +0x0 2 look up, 4 look down */
    s16 timer;        /* +0x6 */
    s32 speed;        /* +0x8 pan speed added to y */
    void (*fn)(struct CamLook *w); /* +0xC */
} CamLook;

/* Camera_Create: after objects_init (the mirror exists) and dcp_player_create.
 * level_data = the whole level.bin (split map, size); zone/act = gGameState.unk1C/1D. */
void camera_create(const void *level_data, DcPlayer *p, int zone, int act);
void camera_destroy(void);
/* Camera_Main (or Camera_Main_Fixed on boss stages) */
void camera_update(void);
/* Player_Last: Player_UpdateCamera + Player_UpdateCameraLead */
void camera_player_last(DcPlayer *p);
/* StageTitleCard_HoldPlayerX / Y: the camera snaps to the held player */
void camera_snap_to_player(DcPlayer *p);
/* the DC window (24.8, top-left of the 256x192 world window) */
void camera_get_view(s32 *x, s32 *y);
void camera_get_partner_view(s32 *x, s32 *y);   /* gCamera[1] (object view 1) */
/* ScreenShake_Main (task 0x3e00): once per world frame, after the objects */
void camera_frame_end(void);
/* g_cam_x / g_cam_y (render.h) = window + Camera_GetViewOrigin's shake offset */
void camera_apply(void);
/* ScreenShake_Start / _Stop (g_dcp_hooks.shake and g_obj_hooks.shake are installed) */
void camera_shake(s32 kind);
s32 camera_shake_x(void);   /* ScreenShake_GetOffsetX */
s32 camera_shake_y(void);
/* shake tables data_0207323c / _40 / _4c (zonefx.bin) */
void camera_set_shake_tables(const s8 *t3c, const s8 *t40, const s8 *t4c);

/* Called inside Camera_Main where ZoneBg_Update / ZoneFx_Update run (zonefx module) */
void camera_set_zone_hook(void (*fn)(void));

/* read access for the zone effects / tests */
const CamLayer *camera_layer(int i);     /* 0, 1 = DS screen pair, 2 = DC window */
CamLayer *camera_layer_rw(int i);
const CamGlobal *camera_global(void);
u32 camera_flags2(void);                 /* gCamera[2].flags as the DS sees it (dual bit) */
s32 Stage_GetSplitLineY(s32 x);
u8 Camera_GetSplitLineScreenY(s32 idx);
s32 camera_window_split_y(void);         /* water line row in the DC window (0..192) */

/* Camera_* entry points other modules call on the DS */
void Camera_LockX(s32 i);
void Camera_LockY(s32 i);
void Camera_UnlockX(s32 i);
void Camera_UnlockY(s32 i);
void Camera_ResetOnRespawn(void);
void Camera_SetLeadTarget(s32 idx, s32 a, s32 b);
void Camera_SetLeadTargetPrimary(s32 a, s32 b);
u8 Camera_GetPlayerScreenMask(const DcPlayer *p);
void Camera_WorldToScreen(int layer, s32 x, s32 y, s16 *ox, s16 *oy);

/* statistics (tests, frame log) */
typedef struct CamStats {
    u32 frames, swaps, eased, snaps, clamped_player;
    s32 max_dy;       /* largest |window y step| of an eased frame (24.8) */
} CamStats;
const CamStats *camera_stats(void);
int camera_look_state(void);

/* Battle Play: per-player camera contexts (camera.c) */
#include <stddef.h>
size_t camera_ctx_size(void);
void camera_ctx_save(void *dst);
void camera_ctx_load(const void *src);
void camera_set_rival(DcPlayer *p);

#endif

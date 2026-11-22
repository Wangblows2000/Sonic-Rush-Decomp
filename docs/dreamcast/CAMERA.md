# Dreamcast port — camera and presentation

Module: `dreamcast/game/camera/` (camera.c, camera.h, test/, patches/). Translation of
`src/game/Camera` (the gCamera task), the camera part of `Player_Last`
(`Player_UpdateCamera`, `Player_UpdateCameraLead`), `ScreenShake_*` and the camera hooks of
ZoneFx_Zone6 (Altitude Limit). The zone backgrounds and effects that run inside Camera_Main
are in `dreamcast/game/zonefx/` (GRAPHICS.md section 8).

## 1. Single-screen rule

The DS camera drives two stacked screens. gCamera[0] is the screen the player is on,
gCamera[1] the other one, 0x110 px (192 + an 80 px gap) above or below
(`Camera_InitDualScreen`: gap @0xD0 = 0x50 px, partner offset @0xD4/@0xD8 = 0 / ±0x110 px).
A per-stage split map (`Stage_GetSplitLineY`, level.bin section SPLIT) gives a line per
64 px column: above it the player's screen is the upper one and its camera may not go
below line − 0xE8; below it the screens swap (`Camera_SwapScreens`, the LCDs swap with
`Camera_ApplyLcdSwap`) and the camera may not go above line + 0x28. Near the line the
player's camera is pinned and the player crosses the gap between the two screens.

The Dreamcast shows one 640x480 view: a 256x192 world window (one DS screen) at 2.5x.
Two cameras run every frame:

- **The DS pair** (gCamera[0], [1]): the original dual-screen code, unchanged — split line,
  swaps, partner screen, `Camera_ClampYToBounds` with the dual-screen limits. It is the
  gameplay state (swap flag 2 for `Stage_SaveRestartCamera`, flag 0x400 for
  `Display_IsMainScreenOnBottom` in the lead offset).
- **The DC window**: the same `Camera_*` code for gCamera[0] in its single-screen form
  (`Camera_UpdateY` with `Camera_ClampLookPanY`, the single-screen bounds clamp; no split
  line, no swap), using gCamera[0]'s flags, offsets, lead target, bounds and locks.
  - x = gCamera[0].x exactly (the original horizontal behaviour; it is the same code in both
    forms).
  - y = gCamera[0] without the split-line pinning: where the DS pins the player's screen and
    lets the player run into the gap, the window keeps following.
  - Discontinuities are eased, never snapped: a jump of the single-screen camera of more than
    32 px in one frame (a gimmick lock / unlock, the 8 px lead change at a screen swap
    — `Player_UpdateCameraLead` uses 0x58 instead of 0x60 when the player's screen is the
    lower one —, a look pan ended by the pair) is closed by 1/6 per frame, at most 12 px.
  - The player stays inside the window: 24 px from the top and from the bottom (the limits of
    the original look pans, 0x18 and 0xC0 − 0xA8), then the bounds win (enemy rooms, areas,
    the map).
  - Respawns and the title card snap (`camera_snap_to_player`, `Camera_Create` again).
- **Objects**: `objects_set_view(0)` = the window, `objects_set_view(1)` = the window moved
  192 px up (unchanged main.c convention). g_obj_camera[i].x/y are those views; the DS pair
  positions are private to the camera.

## 2. What is translated

| original | here | notes |
|---|---|---|
| Camera_Create, InitBoundsAndBuffers, InitDualScreen, InitSingleScreen, InitFixedVram | `camera_create` | map bounds inset 8 px (@0x114), view 0x100 x 0x100 scale (@0x1E/@0x20: g_obj_camera[i].w/h; the object module had h = 0xC0), start screen from the split line at the start point, boss stages (`zone >= 9 \|\| act >= 2`) = fixed camera |
| Camera_Main, Camera_Main_Fixed | `camera_update` | prev, gap/partner, follow, in-view flags, ZoneBg/ZoneFx hook, displacement; fixed mode: the window follows the player so 2D draws keep an origin (the DS arena camera is 3D, boss module) |
| Camera_UpdateFollow, UpdateX, UpdateY, UpdateYDualScreen, FollowPlayerX/Y, ClampX/YToBounds, ClampX/YToMapBounds, ClampX/YAroundTarget, SetX/YFromPartnerScreen | yes | both forms |
| Camera_RunLookCallback, Camera_Look_Idle/Wait/PanUp/PanDown/ReturnFromUp/ReturnFromDown/Reset, ClampLookPanY, ClampLookPanYDual, HasReturnedToPlayerY(Dual) | yes | Up / Down held 60 frames in the look anims (0x21/0x22, 3/4), pan 4 px/frame, back 16 px/frame; the window pans 72 px (single-screen limits) |
| Camera_SwapScreens, CheckPlayerCrossScreen, TrySwitchScreen, ApplyLcdSwap | yes (pair) | |
| Camera_SetLeadTarget(Primary), Player_UpdateCameraLead | yes | lead state @0x3D2..@0x3D8 kept per player in the camera (the objects reuse those fields for the swing) |
| Player_UpdateCamera | `camera_player_last` | gimmick locks pf2 0x10/0x20 at gimmick − (@0x3EC + 0x80, @0x3EE + 0x60) (ObjPlayerCtx.g3EC/g3EE), ObjShift_Set approach, unlock easing, Camera_Lock/UnlockX/Y |
| Camera_CheckPosInView(Latched), AddAreaObject, RemoveAreaObject | object module (obj_shared.c) | their two-camera form |
| Camera_ClearPosInViewFlags, UpdateInViewLatch | yes | on both cameras (the objects' form) |
| Camera_GetSplitLineScreenY, Stage_GetSplitLineY | yes | + `camera_window_split_y` for the window |
| Camera_GetPlayerScreenMask, WorldToScreen, GetViewOrigin | yes | |
| Camera_ResetOnRespawn, Player_Respawn camera part | `g_dcp_hooks.restart` (chained) | see section 4 |
| ScreenShake_Create/Start/Stop/Main/GetOffsetX/Y | yes | tables data_0207323c/40/4c from zonefx.bin; installed as `g_dcp_hooks.shake` and `g_obj_hooks.shake`; the offset moves the layers, objects and player (Stage_Main: gCamera.unk18 = −shake, ObjDraw_SetGlobalOffset), not the BG |
| ZoneFx_Zone6_Update, SetLockedCameraX, ApplyCameraWave | yes | Altitude Limit ±4 px vertical bob |
| StageTitleCard_HoldPlayerX/Y (camera part) | `camera_snap_to_player` | |
| Camera_HandleSwapScreenInput, ApplyNetPartnerPosition, DisableSwapForPlayer, GetBySwapState, GetOtherBySwapState, GetPrimary/Secondary | not needed | VS / wireless / Select swap |
| Camera_DrawMapBg, LcdSwapDestructor, Destructor, Reset | replaced | the DC draws the map from level.bin (engine/level.c) |

## 3. Frame order and API

```c
camera_update();                      /* Camera_Main (task 0x80) + ZoneBg/ZoneFx */
camera_get_view(&x, &y);              /* the window, 24.8 */
objects_set_view(0, x, y); objects_set_view(1, x, y - 0xC000);
objects_frame_begin(); dcp_player_frame(...);
camera_player_last(&player);          /* Player_Last: Player_UpdateCamera(+Lead) */
objects_update(...); boss_update(...);
camera_frame_end();                   /* ScreenShake_Main (0x3e00) */
/* every frame, also paused */
if (title card) camera_snap_to_player(&player);
camera_apply();                       /* g_cam_x / g_cam_y = window + shake */
```

`camera_create(level_data, &player, zone, act)` after `objects_init`; `zonefx_init` after it;
`camera_destroy` / `zonefx_shutdown` before `level_free`. Patches for main.c, the game
Makefile and build_disc.sh: `dreamcast/game/camera/patches/`.

## 4. Deviations

- Respawn: the DS restarts the stage (`StageRestart_Create` → `Camera_Create`); the DC keeps
  the objects, so the camera keeps their state (bounds, locks, water, limit) and resets its
  own: positions and start screen (split line at the checkpoint), look state, lead, area
  offset overrides (flags 0xF000), locks 0x40/0x80; `objects_respawn_view()` is called.
- g_obj_camera[2].flags bit 0 (dual screen) stays clear for the object module; the camera
  keeps its own bit. The objects' in-view checks and area bounds use their single-screen form,
  which is also the window's form.
- In boss arenas the window follows the player (the DS arena camera is the boss module's 3D
  camera; the gCamera position is not used there).
- Sand ship drop-in Egg Pawns: they spawn when a view comes within 60 px of them. With view 1
  = window − 192 px they stay out of range (host test: 0 seen in 3000 frames); with view 1 =
  the DS partner screen (gCamera[1], 272 px up) they drop in (1 seen). The main.c convention
  is kept; `camera_layer(1)` gives the DS screen if view 1 should follow the DS instead.

## 5. Measurements

Host test (`make -C dreamcast/game/camera host-test`, real player + objects, 18 stages, 2400
frames of play + a sweep from every 40th ring, 157 checks, all pass): window x == gCamera[0].x
on every frame; no window step over 32 px except respawn snaps (largest eased step 17 px);
player always 24..168 px from the window top unless the camera bounds exclude him (a test
teleport out of a locked enemy room); window always inside bounds and map; DS pair offset
±0x110 px on every frame; look pans 72 px (80 / 64 px with the 0x58 lead of the lower screen,
±4 px bob in Altitude Limit); sand ship: locked 3000 frames, x on the lock target, y within
16 px (the player margin), no jump.

Flycast (display :97, debug-start discs of the test tree = this tree + the patches; frame log
line `cam ds <gCamera[0] x,y> / <gCamera[1] y> swap <flag 2> window <x,y> ...` every 120
frames): z11, z21, z22, z31, z32, z41, z51, z61, z71, z82 and z91 run at 59.3 fps, the boss arenas
z13 / z23 at 58.8-59.3 (fixed camera, boss draw cost unchanged). The DS pair swaps (z11: upper -> lower screen at the
split line, gCamera[1] 272 px above) without a window step; window = gCamera[0] while it
follows freely (z11, z41, z61, z71: identical y in every log line), the window keeps the
player where the pair is pinned. Screens: one 256x192 window, no screen split, BG parallax
per zone (GRAPHICS.md section 8.3). The sand ship could not be reached from a debug start:
the title card's forced run (StageTitleCard_HoldPlayerX holds Right) carries a player placed
near the ship over its front edge, and spots up the approach slope left the player stuck; the
ship lock is covered by the host test only.

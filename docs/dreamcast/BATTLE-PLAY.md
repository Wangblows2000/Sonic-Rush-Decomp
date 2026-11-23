# Battle Play (local two-player VS) — Dreamcast port

Branch `battle-play`. Two players on one Dreamcast: controller port A = player 1, port B = player 2.
No wireless, network or second console. Menus: docs/dreamcast/translation/MENUS.md section 15.

## How the original works and what changed

On the DS each console simulated only its own player. The rival was a puppet driven by wireless
state (`Player_ApplyNetState`). Rival events went over the air (`Stage_NetProcessRivalEvents`:
7 slowdown, 8 confusion, 9 tension drain, 10 warp to rival, 2 object taken, 3 goal).

Here both players run the full player code. Both have `pid` (`unk325`) = 0, the "local" value,
so every local-only path (effects, boost, time over) runs for both. The wireless events become
direct calls in `dreamcast/game/vs/vs.c`.

## Original → port

| original | port | status |
|---|---|---|
| `ItemBox_GiveItem` 8..11, `itembox_vs` random table | `objects/obj_gimmicks.c` (already ported) + `vs.c h_item` | reused / adapted (net send -> the rival) |
| `Player_ApplySlowdown` (`unk3C2 = 0x200`), `Player_ApplyConfusion` (`unk3C0 = 0x258`) | `vs.c h_item` | adapted |
| `Player_In` slowdown / confusion timers, D-pad remap | `player/dcp_player.c` (already ported) | reused |
| `Player_DrainTension`, `Player_AddTension` VS branch, `TensionSound_Create/_Main` | `dcp_player.c` + `g_dcp_hooks.vs_tension` -> `vs.c` | adapted |
| `Player_WarpToRival`, `Player_Action_Warp`, `Player_State_Warp`, `Player_Respawn` warp branch | `vs.c warp_to_rival`, `dcp_states.c dcp_act_warp / dcp_state_warp`, `dcp_respawn` | translated |
| `Player_OnPushRival` (coll2F8 push box) | `vs.c on_push_rival` (each player against the other's box) | translated |
| `Player_UpdateBoost` pf 0x20000 + `Player_ApplyNetState` knock-back | `dcp_states.c` (already ported) + `vs.c knock_back` | adapted |
| `Player_Action_ReachGoal`, `StageClear_HandleGoalReached` (unk40) | `vs.c reach_goal / h_goal` | translated |
| `Player_CheckTimeOver` / `Player_Last` VS branches (0x8c9f, draw) | `vs.c vs_after_players` | translated |
| `Player_Respawn` VS (no lives, no StageRestart, own checkpoint) | `dcp_respawn` (already ported) + per-player checkpoint in `vs.c` | reused / adapted |
| `Marker_OnHit` (one marker per console) | `obj_gimmicks.c marker_on_hit`: VS keeps it live until both players took it | adapted |
| `RingManager` VS (`nplayers = 2`, both collect) | `objects/obj_ring.c` (already ported) | reused |
| `EffVs*`, `FixVsPushIcon` | `objects/obj_vs.c` (`ac_eff_vs.bac`, `ac_fix_vs.bac`) | translated |
| `GoalRing_State_Idle` VS -> `ResultsScreen_Create` | `obj_gimmicks.c` (already ported) -> `vs.c h_stage_exit` | reused |
| `FixVsHud` (1st / 2nd by x) | the bar between the viewports | new (DC presentation) |
| `Stage_NetProcessRivalEvents`, `NetPacket_*`, `Player_SendNetState`, `VsConnect_*`, lobby | — | not needed |

## Dreamcast presentation (new code)

The split screen follows Sonic 2's VS mode (`src/main.c run_vs_stage`):
- Player 1's viewport is on top and player 2's below. Each is 640x224 and shows the world at
  7/6 scale. That covers 548x192 world px: the DS screen's full height, and a wider view.
- A 32-px bar sits between the viewports. It shows the players' colours and who leads. The
  bar is needed because the PVR user clip works in 32-px tiles.
- **Engine:** `render.c` has a view rectangle and scale (`render_set_view`) and the PVR user
  clip (`render_emit_clip`, `render_hdr_clip`). `level.c`, `obj_draw.c` and the model stage
  cameras (`model.c`) draw into the current view. The defaults (640x480 at 2.5, clip off) give
  the same numbers as before.
- **Contexts:** each player has its own camera (`camera_ctx_save/load`), zone BG line scroll
  (`zonefx_view_*`; time and palette effects run once per frame), HUD (`hud_ctx_*`) and
  checkpoint.
- **HUD:** the single-player HUD (rings, time, tension gauge; no lives, no score) is drawn in
  each viewport at 224/480 scale (`render_set_screen_xform`).
- **Fades:** death, warp and goal fades apply per viewport.
- **Start:** a 3-2-1-GO countdown, with players held until GO. There is no countdown art among
  the VS assets.
- **Pause:** Start pauses; B while paused ends the race and opens the results.
- **Objects:** 8 spawn views (the DS screen pair plus the wide window for each player).
  Objects, item boxes and rings are shared, as the DS synchronised them (event 2, the two-player
  RingManager).
- **Models:** player 2's model is a second `DcpModel` (`dcp_model_attach_second`).

## Test builds

- `/cd/debug/vs.txt` `zone act chara1 chara2 [x y] [clock]` starts a race directly.
- `/cd/debug/vs_p2.txt` `1` scripts player 2's pad, in the menus and the race; `2` scripts both
  pads. Use this for emulator runs with one keyboard. It also turns on the VS frame log (fps,
  update / wait / draw ms).
- Host tests: `make -C dreamcast/game/vs host-test` (rules on Leaf Storm with the real player and
  objects code) and `make -C dreamcast/game/menus host-test` (`test_vs`: the VS menu flow).

## Known gaps

- **Sound bank:** the stage loads player 1's character's sound bank. Player 2's character
  voices come from that bank (not checked by ear).
- **Countdown:** drawn with flat segments, not original art.
- **Menu text:** the Battle Play page keeps the original "DS WIRELESS / DOWNLOAD" button art.
  The name plates on the VS screens are blank (no player 2 name entry).
- **Shared effects:** objects' screen shakes and fades act on the camera loaded during the
  shared objects update (player 1's).
- **Not tested on hardware.** Flycast runs used one keyboard and a scripted pad for player 2.
  Two physical controllers in Flycast were not tested.

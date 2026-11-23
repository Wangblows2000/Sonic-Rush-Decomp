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
| `FixVsHud_Create` / `FixVsHud_Draw` (position "1" / "2" + "/2" at DS (200, 4), pop on change) | `ui/ui_hud.c FixVsHud_Draw` in each player's HUD | translated |
| `StageTitleCard` (start: "Ready? / GO!", players held until GO) | `ui_vs_titlecard_draw` over both viewports; the race starts at GO | reused |
| `StageLoader_SetCharacterSoundPath` (one SDAT per console) | `sound_load_bank2`: player 2's character SDAT resident beside player 1's; player 2's own sounds start from it | adapted |
| objects' `ScreenShake_Start` / `BrightnessFade_Create` (on every console that ran the object) | `vs.c h_obj_shake / h_obj_fade`: the cameras / viewports of the players near the object | adapted |
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
- **HUD:** the single-player HUD (rings, time, tension gauge; no lives, no score) and the original
  position badge, laid out in each viewport with the DS anchors (top-left / top-right / bottom-left)
  at 7/6 (`ui_hud_viewport`).
- **Fades:** death, warp and goal fades apply per viewport.
- **Start:** the original title card ("Ready? / GO!") over the whole screen; the race clock and
  both players start at GO.
- **Pause:** Start pauses; B while paused ends the race and opens the results.
- **Objects:** 8 spawn views (the DS screen pair plus the wide window for each player).
  Objects, item boxes and rings are shared, as the DS synchronised them (event 2, the two-player
  RingManager).
- **Models:** player 2's model is a second `DcpModel` (`dcp_model_attach_second`).

## Test builds

- `/cd/debug/vs.txt` `zone act chara1 chara2 [x y] [clock]` starts a race directly.
- `/cd/debug/vs_p2.txt` `1` scripts player 2's pad, in the menus and the race; `2` scripts both
  pads (the real pad's Start and B still reach player 1, so pause / quit can be tested). Use this
  for emulator runs with one keyboard. It also turns on the VS frame log (fps, update / wait /
  draw ms; the first sample of a race spans the results and the stage load).
- `/cd/debug/menutrace.txt` (any content) prints menu scene changes, as `SRDC_MENUTRACE=1`.
- Release discs (and the kits' discs) have no scripted pad: a race needs a second controller in
  port B. Flycast must map one to port B to play it.
- Host tests: `make -C dreamcast/game/vs host-test` (rules on Leaf Storm with the real player and
  objects code) and `make -C dreamcast/game/menus host-test` (`test_vs`: the VS menu flow).

## Sound

The DS loaded one zone SDAT per console: the local character's (zNs for Sonic, zNb for Blaze).
Here player 1's SDAT is the stage bank. Player 2's SDAT stays resident beside it (`sound_load_bank2`,
about 150 KB of sound RAM).
- **Player 2's sounds:** its own sounds (voice, boost, trick and global channels) start from that
  bank (`sound_select_bank2`, `NNS_SndArcSelectBank`).
- **Driver:** a playing sequence keeps its bank (`Player.bank`, channel `bank`), so player 2's
  waves come from its own upload.
- **Shared sounds:** stage objects, rings and music use the stage bank. There is one music stream.
- **Host check:** `test_sound` (bank2) confirms that the same voice id starts from z1b for player 2
  and from z1s for player 1.

## Known gaps

- **Wording in other languages:** the cropped "BATTLE" label and "QUIT BATTLE ?" are built for
  English, French, German and Italian (label) and English and French (message). The other
  languages keep the original art.
- **Shared effects:** an object far from both players (off both screens) has no viewport; its
  shake / fade is dropped.
- **Not tested on hardware.** Flycast runs used one keyboard and a scripted pad for player 2.
  Two physical controllers in Flycast were not tested.

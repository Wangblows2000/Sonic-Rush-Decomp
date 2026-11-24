# Battle Play (local two-player VS) — Dreamcast port

Branch `dreamcast-port` (developed on `battle-play`, merged; see README.md "Branches"). Two players on one Dreamcast: controller port A = player 1, port B = player 2.
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

- `/cd/debug/vs.txt` `zone act chara1 chara2 [x y] [clock] [cpu]` starts a race directly (`cpu` 1:
  player 2 is the CPU).
- `/cd/debug/cpu_skill.txt` `0` / `1` / `2`: the CPU's skill (easy, normal, hard).
- `SRDC_ROUTES=copy sh build_disc.sh`: put only the racing lines already made on the disc (no search).
- `/cd/debug/vs_p2.txt` `1` scripts player 2's pad, in the menus and the race; `2` scripts both
  pads (the real pad's Start and B still reach player 1, so pause / quit can be tested). Use this
  for emulator runs with one keyboard. It also turns on the VS frame log (fps, update / wait /
  draw ms; the first sample of a race spans the results and the stage load).
- `/cd/debug/menutrace.txt` (any content) prints menu scene changes, as `SRDC_MENUTRACE=1`.
- Release discs (and the kits' discs) have no scripted pad: a race needs a second controller in
  port B. Flycast must map one to port B to play it.
- Host tests: `make -C dreamcast/game/vs host-test` (rules on Leaf Storm with the real player and
  objects code; `test_cpu_unit`: the CPU's decisions on synthetic lines), `make -C dreamcast/game/vs
  cpu-test [CPU_ARGS="-p1 ... -upsets 3 -seed 2"]` (the CPU racing every stage with the real simulation:
  goal time, deaths, recoveries; `test/test_cpu.c` options: a CPU player 1 on its own lines, random upsets,
  skill, seed, a trace) and `make -C dreamcast/game/menus host-test` (`test_vs`: the VS menu flow).

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

## Vs CPU (native)

The DS game had no computer rival (Battle Play needed two consoles). The Dreamcast adds one:
BATTLE PLAY page → **ONE PLAYER** (the original button font has no digits, V or U, so the labels are
TWO PLAYERS / ONE PLAYER; menus: MENUS.md section 15). Player 1 picks a character; player 2, the CPU,
takes the other one and is marked **CPU** on the select, stage select and results plates and in the
race bar (the name plates' font, `menus_text_mask`). Stage select, race, results, TRY AGAIN and CHANGE
CHARACTER/ACT are the Battle Play ones; `MenuRequest.cpu` carries the mode.

**What the CPU is.** `vs/vs_cpu.c` only makes player 2's DS pad (the bits `ds_pad` makes from a
controller); player 2 then runs the same player, object, item, checkpoint and race code as a human.
Nothing about the CPU changes the world: no speed, invulnerability, rings or rule exceptions. It reads
its own player, the map collision around it and the rival's position and pad (what a player sees on the
split screen).

**How it drives.** Each stage has racing lines for each character (`/cd/vs/zNA_C.rte`, `vs/vs_route.h`):
the position and pad of every frame of a way through the stage, and the race-clock frame each line
starts at. Every frame the CPU finds where it is on its line (around where it expects to be, so standing
on a ride or a loop keeps the place) and plays the line's pad there; a press it already holds is let go
for a frame so the press happens. The lines carry the moves the search found worth it: boosts, jumps,
spin dashes, tricks off springs and ramps (R in the air, up / forward and the A / B combo; they fill the
tension gauge), boosts in the air, air steering and waits for timed gimmicks.

Off its line (knocked back, pushed, warped, slowed, a missed jump, a respawn) it takes the nearest point
of any line with the least time left to the goal, counting distance (about 4 px a frame) and how far the
line's own time is from the race clock (the objects move on the race clock, so a line made at about the
present time fits the world as it is now); lines are indexed by a 128 px grid. It steers there with a
player's moves (run, jump at walls and up to the line, air action). When its line rides a gimmick (ship,
crane, rope, rocket) that left without it, it waits at the boarding place for up to 10 s. No progress
for 3 s: it first takes another line from where it is (the route tool's branches start at exactly such
places); otherwise it backs up along the line for a run-up (twice, further back each time), then jumps,
backs off and spin-dashes, or waits. A line that failed at a place is not taken within 320 px of it for
30 s. Confusion (the rival's item turns the D-pad): after 40 frames, like a player, it presses the key
that now gives the way it wants. Next to the rival on the ground with tension to spare it boosts into it
(at most every 4 s). Without any line (a missing file) it follows the rival's path (the furthest place
the rival passed 2 s or more ago).

**Variation and skill.** The seed changes every race (reproducible: race count, zone, act). Skill
(normal by default; `/cd/debug/cpu_skill.txt` 0 easy, 1 normal, 2 hard in test builds) decides how many
of the line's boost stretches the CPU uses (easy drops about half, normal and hard none; never just
before one of the line's jumps; never when the rival is well ahead).

**Where the lines come from.** `vs/route/vs_route.c` (host tool, run at disc build time on the owner's
converted stages; nothing derived from the game is in the repository):
1. **Line 0:** a search with the real game simulation (`route/vs_sim.c`: player, objects, cameras,
   race rules, the Dead Line gravity split) in the style of Go-Explore. The stage is cut into 64 px cells,
   also split by the guide's stage, ground / air, the collision layer and whether a switch timer runs.
   The archive keeps a snapshot of the whole simulation (`route/snap.c`: every static of the simulation
   sources, bracketed in link order by `snap_begin.c` / `snap_end.c`, and the objects' mutable heap
   copies; at most 1.2 GB) and the fastest way into each cell. Each round restores a promising cell and
   plays pad macros (run, boost, jumps, spin dash, crouch, air action, tricks, boosts in the air, air
   steering, waits up to a minute; mostly waiting and hops while a gimmick carries the player), biased
   by a distance field to the goal through the stage's free space that costs 3x far above any floor.
   When that finds nothing the guide becomes the checkpoint markers in their order; when that finds
   nothing either, the search goes on in stages from the furthest place reached (a fresh archive there,
   up to 8 stages, 5 s further back when a stage gets no closer).
2. **Shortcuts:** from 40 random points of line 0, a search for a way that meets line 0 again sooner;
   kept when the whole line replays to the goal faster.
3. **Coverage:** up to 24 places the search reached early that no line passes near; from each, a short
   search to a point where it joins a line going the same way.
4. **Trials:** the CPU itself, in the simulation, from the start with an upset: slowdown, confusion,
   tension drain, knock-back, a missed jump, no boost, easy skill, a rival CPU on the same lines, a rival
   and an item, a death (respawn into a world that went on), or two of them. Progress is the line's frames
   to the goal; 7 s without progress (or three deaths) fails the trial. A branch is searched from the frame
   the CPU found itself stuck to a point of the lines past the trouble (fewer frames to the goal), and
   the same trial runs again with it (up to 4 repairs) until it reaches the goal. 90 trials, or 15
   passes in a row; up to 140 lines.
Limits are in search rounds, so a build gives the same lines on every machine (the time is only a
safety cap; the Windows build `vs_route.exe` and the Linux build replay each other's lines exactly). A
line is checked by replaying it from the start. A stage where no way to the goal is found gets the
furthest grounded part as a partial line (header flag `VSR_INCOMPLETE`).

**Making them (cache, resume, reports).** `make -C dreamcast/game/vs routes` (Linux, `-j` for parallel)
or the Windows kit (`vs_route.exe`, `winbuild.py`) runs every pair with `-check`. Each file's header has
a generation key: a CRC of the stage (`level.bin`), the object data and hit boxes, the player tables,
the format version and a hash of the simulation, follower and tool sources (`make -s vsr-src`; the
Windows tool is built with the same hash). A file with the same key is kept at once. A file made by
another build is made again, but its line 0 is kept when it replays to the goal frame for frame (the
line 0 search is the long part; the coverage, trials and branches are redone); a partial one is searched
on from its end. Files are written as `<pair>.rte.tmp` and renamed when complete, so a stopped run carries
on with the pairs not yet made. Per pair: `routes/<pair>.rte.txt` (result, key, every line's span) and
`routes/<pair>.log`. `vs_route -rekey` gives a file the present key after its line 0 replays frame for
frame (for tool changes that cannot reach the lines). The lines stay in `build/` (never in git).

**Shared world fix (both modes).** Water levels are a per-camera value, and the objects run with player
1's camera, so player 2's water (Water Palace underwater state, air bubbles, splashes; the Mirage Road
sand ship) followed player 1. The water triggers now also keep a level per player (`obj_water_of`,
`objects_player_water`), the player-specific readers use it, and player 2's camera gets its own level
(`vs_sync_water`). On the DS each console had its own.

## Known gaps

- **Wording in other languages:** the TWO PLAYERS / ONE PLAYER buttons are English in every language
  (built from the English labels); "QUIT BATTLE ?" is built for English and French, the other languages
  keep the original message art.
- **Shared effects:** an object far from both players (off both screens) has no viewport; its
  shake / fade is dropped.
- **Vs CPU lines (host results, `vs/test/test_cpu`, seed 4).** With complete lines (zones 1..3, 5, Night
  Carnival act 1 Sonic: 18 of 28 stage / character pairs) the CPU finishes undisturbed in the line's
  time (1:30 .. 3:13). Upsets (items, knock-backs) are handled on Leaf Storm (10 of 10 seeds with two
  upsets, z11 Sonic) and partly elsewhere; on most stages one upset can send it into pits again and
  again (it does not reach the goal in many of those runs). No complete line was found for Night
  Carnival act 1 (Blaze), Night Carnival act 2, Altitude Limit, Huge Crisis and Dead Line (both acts):
  there the CPU drives a partial line (from the start to the furthest place the search reached), then
  follows the rival's path (where the human has been 2 s or more before), so it trails the human there.
  The line search is long (50 min for the 28 pairs on two cores, most of it on those stages).
- **Vs CPU variation:** the race seed varies only the easy skill's boost choices and the rival
  interplay: the stages' objects run on the race clock, so a CPU that starts or moves at another time
  meets them differently (a reaction delay at GO broke the lines and was removed).
- **Not tested on hardware.** Flycast runs used one keyboard and a scripted pad for player 2.
  Two physical controllers in Flycast were not tested.

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
- `/cd/debug/vsseq.txt`: Vs CPU races one after another from the boot, one a line
  `zone act chara1 chara2 [p1cpu [from to]]` (0-based; `p1cpu` 1: player 1 is a CPU too, on its own
  character's lines). Each race runs to its end; the results scene is skipped and its numbers logged.
  The races run on from one random seed, as a player's races do (see "Random world" in Known gaps).
  Fields 6 and 7 are a race clock range: player 2 is printed every frame of it on the serial console in
  `test_cpu -dump`'s format (`dump <clock> pad .. -> x,y vx anim df pf rings`), so a race in Flycast
  can be compared with the host simulation frame for frame (`main.c`, `g_vs_tr0` / `g_vs_tr1`).
- `/cd/debug/menutrace.txt` (any content) prints menu scene changes, as `SRDC_MENUTRACE=1`.
- Release discs (and the kits' discs) have no scripted pad: a race needs a second controller in
  port B. Flycast must map one to port B to play it.
- Host tests: `make -C dreamcast/game/vs host-test` (rules on Leaf Storm with the real player and
  objects code; `test_cpu_unit`: the CPU's decisions on synthetic lines), `make -C dreamcast/game/vs
  cpu-test [CPU_ARGS="-p1 ... -upsets 3 -seed 2"]` (the CPU racing every stage with the real simulation:
  goal time, deaths, recoveries; `test/test_cpu.c` options: a CPU player 1 on its own lines, random upsets,
  skill, seed, a trace) and `make -C dreamcast/game/menus host-test` (`test_vs`: the VS menu flow).
  `test_cpu -rseed n` sets the game's random seed (`gRandSeed`) at the race start (the simulation
  otherwise starts at 0); `-dump a b` prints player 2 every frame from a to b: pad, position, ground
  speed, animation, display flags, `pf`, rings and the race clock. `-dist k -at f` disturbs the CPU at
  frame f (0 an item from the rival, 1 a knock-back, 2 a death, 3 an enemy's hit, 4 no keys for 10 s)
  and prints whether it recovered (the release recovery test in Known gaps runs it with `-p1` and
  `-rseed` for every pair).

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
30 s; a ride that goes nowhere counts as no progress after 10 s.

**Recovery: following the rival.** When the lines fail twice running at a place (no progress for 3 s,
twice, between which it got no further: an item, a knock-back, an enemy, a world not as the line knew
it), or when it dies again within 640 px of where it last died (after the respawn), the CPU is lost: for
10 s (30 s after the second death) it takes the rival's path instead, the furthest place the rival
passed 2 s or more ago within reach, and runs after the rival along it with its own moves (steering to
the rival's position 0.5 s further on: the rival is the other character, whose moves are not its own).
When the 10 s are over and it is on a line point (40 px) that has not failed there, it takes the line
again; failing again makes it lost again. The rival's path is the human player's way through the race
as it is now (the objects in their present phase), so it is a way that works; it costs one table of
positions (the rival's position every frame, recorded anyway) and no search. No teleport, speed, rings
or invulnerability: it is still only the pad. A death resets the progress count (the respawn point is
behind), so a respawn is not taken for being stuck.

Confusion (the rival's item turns the D-pad): after 40 frames, like a player, it presses the key
that now gives the way it wants. Next to the rival on the ground with tension to spare it boosts into it
(at most every 4 s). Without any line (a missing file) it follows the rival's path all the race.

**Variation and skill.** The seed changes every race (reproducible: race count, zone, act). Skill
(normal by default; `/cd/debug/cpu_skill.txt` 0 easy, 1 normal, 2 hard in test builds) decides how many
of the line's boost stretches the CPU uses (easy drops about half, normal and hard none; never just
before one of the line's jumps; never when the rival is well ahead). Apart from the CPU's seed, the
game's own random seed (`gRandSeed`) differs from race to race, as on the DS: see "Random world" in
Known gaps.

**One race after another.** Each race starts as the line simulation's do, whatever ran before it (the
story, a boss, another race): the race clock, the stage flags and the object manager's flags are set
afresh, player 2 is cleared, the item box popup pointer, the camera's respawn flag and a reused player
model's animation speeds are reset (`run_vs_stage`, `objects_init`, `camera_create`,
`dcp_model_attach`). Only the game's random seed runs on, as on the DS.

**Where the lines come from.** The 28 files ship with the source (`dreamcast/game/vs/routes`, 8.5 MB:
each frame's pad and position, no game data) and go onto the disc as they are (see "Shipped lines"
below); nobody has to make them. They were made by `vs/route/vs_route.c`, a host tool that the disc
build runs again only for converted stages whose key does not match (another ROM revision):
1. **Line 0:** a search with the real game simulation (`route/vs_sim.c`: player, objects, cameras,
   race rules, the Dead Line gravity split, and the two players' models, whose animation ends drive some
   player states such as the Altitude Limit parachute, as `dcp_model_step` does on the Dreamcast) in the
   style of Go-Explore. The stage is cut into 64 px cells, also split by the guide's stage, ground / air,
   the collision layer and the switch timers (none, about to run out, time to spare).
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
   and an item, a death (respawn into a world that went on), two of them, or a random world alone (no
   upset: the undisturbed race as the Dreamcast runs it; picked more often, with death, two upsets and
   rival + item). Every trial runs with its own game random seed at the race start (`world_seed()`), so
   item box rings, enemy debris and the other random events differ from trial to trial; line 0 itself is
   searched with the simulation's seed 0. Progress is the line's frames to the goal (the respawn frame
   does not count); 7 s without progress (or three deaths) fails the trial.
   A branch is searched from the frame the CPU found itself stuck to a point of the lines at least 3 s
   further on (fewer frames to the goal) where the player is in the same state (ground / air, ridden,
   underwater); the same trial then runs again with it (up to 4 repairs) until it reaches the goal.
   When the CPU died and came back far behind (a Battle Play death respawns at the last checkpoint
   passed, often the start) and in the failed trial never got back to within 600 frames to the goal of
   where it first died, the branch search starts instead from a snapshot about 4 s before that death,
   to go round the hit or fall that led to it. Deaths the trial itself gives (the death upset) do not
   count for this.
   90 trials, or 15 passes in a row (a failed trial that got no branch is not a pass); up to 140 lines. Shortcuts (2.) run only on a line 0 found in the
   same run, so a kept line 0 stays as it is.
Limits are in search rounds, so a build gives the same lines on every machine (the time is only a
safety cap; the Windows build `vs_route.exe` and the Linux build replay each other's lines exactly). A
line is checked by replaying it from the start. A stage where no way to the goal is found gets the
furthest grounded part as a partial line (header flag `VSR_INCOMPLETE`).

**Shipped lines.** The 28 files are in the source (`dreamcast/game/vs/routes`: pads and positions only)
and go to the build and the disc as they are; with the EUR ROM's stages their key matches and nothing is
searched (BUILD.md). The generator below runs only for converted stages that differ.

**Making them (cache, resume, reports).** `make -C dreamcast/game/vs routes` (Linux, `-j` for parallel:
one pair per job) or the Windows kit (`vs_route.exe`, `winbuild.py`, one job per core) runs every pair
with `-check`.
- **Keys.** A line is valid for the simulation it was made in. Each file's header holds the generation
  key: a CRC of the stage (`level.bin`), the object data and hit boxes, the player tables and models, the
  format version and a hash of the simulation's sources (`make -s vsr-src`, first value). It also records
  the tool build (second value: the search tool and the follower), which decides only how good the
  lines are. Linux and Windows give the same keys and byte-identical files.
- **Cache.** A complete file with the present key is kept whatever tool build made it; a partial one
  only when this tool build made it. `ROUTE_ARGS=-retool` makes the coverage, trials and branches of
  files from other tool builds again, with line 0 kept.
- **Reuse.** A file with another key is made again, reusing as much as holds: its line 0 is kept when it
  replays to the goal frame for frame, else its part up to where it stops replaying (1 s before) is the
  start of the new search. `vs_route -rekey` gives a file the present key once its line 0 replays frame
  for frame (simulation changes that cannot reach the lines, such as a faster way to compute the same
  thing). The other character's line of the stage is passed as `-guide`.
- **More trials.** `vs_route ... -from <file> -more` improves a complete file without starting again:
  its line 0 must replay to the goal; its other lines (branches) are kept as they are and the trials
  run again, adding branches where the CPU still fails. The file does not store the branches'
  per-point player states, so the merge tests accept any state on a kept branch. (`-check` keeps a
  file with the present key as it is, so `-more` is run with `-from`.)
- **Resume.** Files are written as `<pair>.rte.tmp` and renamed when complete, so a stopped run carries
  on with the pairs not yet made.
- **Reports.** Per pair `routes/<pair>.rte.txt` (result, keys, every line's span) and `routes/<pair>.log`;
  `python3 tools/dreamcast/vs_manifest.py [--replay]` writes `routes/manifest.json` / `manifest.md`: every
  pair complete / partial / missing, key current, line 0 replaying to the goal.
- The lines stay in `build/` (derived from the owner's ROM; never in git).

**Generation speed (measured).** The first full generations took 50 min (two cores, 18 complete pairs,
old search) and then many hours once the trials grew; one pair took 8 to 27 min, with other work on the
machine. Profiling (gprof, Leaf Storm act 1 Sonic) showed:
- 24 % of the time in snapshots;
- 31 % in posing the player models (drawn only);
- 94 % of all simulated frames in the branch and coverage searches, most of them the fixed 1500 rounds
  each branch search spent after its first goal.

Changes, each checked by an identical output file where the result must not change:
- snapshots: one pass over 64-byte blocks;
- no pose in the simulation;
- searches stop 400 rounds after their last improvement (2000 for line 0), and 20000 rounds without
  progress end a goal-less search (the staged search takes over);
- keys that keep valid lines across tool changes;
- reuse of the valid part of an old line 0.

Leaf Storm act 1 Sonic, 90 trials with line 0 kept: 309 s → 180 s CPU (the snapshot and pose changes
alone: 56.9 → 35.6 s on a fixed workload, byte-identical output). Host races with upsets of the old and
new files: 47 vs 44 and 30 vs 32 of 48 (within the noise). After the simulation fix that gave the players
their models (animation ends drive player states), 22 of 28 kept lines still replayed and were re-keyed
in seconds. Only 6 pairs were searched again: 4.5 to 12.5 min each, against hours for a full generation.

**What speedruns taught (research).** Sources:
- TASVideos' Sonic Rush any% submission notes (2025);
- speedrun.com's Sonic Rush general movement guide;
- Sonic Center's shortcuts guide.

Their frame-perfect zips (a pixel- and frame-exact landing, then alternating boost and jump) and
damage-boost skips are not used: they need tool assistance, or they give the CPU an advantage a player
would not take in a race. Used as search moves (macros):
- boost jumps up slopes (height for shortcuts);
- tricks off springs and ramps, including the diagonal trick (Blaze's best shortcut move);
- Blaze's hover (R held in the air);
- the air action after a jump;
- spin dash;
- waits for timed gimmicks.

The search keeps only what the real simulation confirms.

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
- **Vs CPU lines (host results, `vs/test/test_cpu`).**
  - All 28 stage / character pairs have a complete line 0, shipped. It replays to the goal frame for
    frame in the simulation (`vs_manifest.py --replay`), and the CPU finishes undisturbed in the line's
    time: 1:39 (Leaf Storm act 1 Sonic) to 5:04 (Mirage Road act 2 Sonic), unchanged by the recovery
    code (`make -C dreamcast/game/vs cpu-test`). This holds at the simulation's random seed 0; with other
    seeds see "Random world" below.
  - **Recovery (the release test).** 560 disturbed races, 20 per pair: 4 seeds each of 5 disturbances at
    a seeded time in the first 60 percent of the line, the rival a CPU on its own line (a racing player),
    every race in a random world (`test_cpu -p1 ... -dist k -at f -rseed n`). A race counts as recovered
    when the CPU reaches the goal, or gets at least 20 s of course further than where it was disturbed
    (measured along its own line 0 or the rival's, whichever is further; when the rival wins sooner, at
    least half the time left): **378 of 560 recovered (68 percent)**, 68 of them to the goal before the
    race ended. By disturbance:
    - an item from the rival (slowdown, confusion or tension drain): 80 of 112 (lines only: 79)
    - a knock-back: 91 of 112 (lines only: 93)
    - a death (a fall): 60 of 112 (lines only: 68)
    - an enemy's hit (rings scattered): 64 of 112 (lines only: 57)
    - losing ground (no keys for 10 s): 83 of 112 (lines only: 91)
  - With the lines alone (no rival following, the previous build): 388 of 560. Following the rival
    changes which races recover (105 lost, 95 gained of 560) but not how many; it is kept for the
    places where every line fails (it gives the CPU a way on that works in this race).
  - Per pair (recovered with rival following, with the lines alone, reached the goal):

  | pair | recovered | lines alone | goal |
  |---|---|---|---|
  | z11_0 | 11/20 | 13/20 | 3 |
  | z11_1 | 17/20 | 19/20 | 0 |
  | z12_0 | 12/20 | 14/20 | 6 |
  | z12_1 | 17/20 | 19/20 | 0 |
  | z21_0 | 16/20 | 16/20 | 7 |
  | z21_1 | 8/20 | 16/20 | 7 |
  | z22_0 | 4/20 | 4/20 | 3 |
  | z22_1 | 4/20 | 7/20 | 0 |
  | z31_0 | 14/20 | 11/20 | 0 |
  | z31_1 | 19/20 | 18/20 | 0 |
  | z32_0 | 11/20 | 9/20 | 1 |
  | z32_1 | 10/20 | 11/20 | 2 |
  | z41_0 | 12/20 | 10/20 | 2 |
  | z41_1 | 16/20 | 16/20 | 0 |
  | z42_0 | 8/20 | 9/20 | 1 |
  | z42_1 | 17/20 | 16/20 | 0 |
  | z51_0 | 17/20 | 13/20 | 0 |
  | z51_1 | 18/20 | 18/20 | 5 |
  | z52_0 | 9/20 | 8/20 | 0 |
  | z52_1 | 20/20 | 19/20 | 0 |
  | z61_0 | 13/20 | 11/20 | 9 |
  | z61_1 | 5/20 | 6/20 | 0 |
  | z62_0 | 19/20 | 18/20 | 0 |
  | z62_1 | 14/20 | 15/20 | 2 |
  | z71_0 | 20/20 | 20/20 | 2 |
  | z71_1 | 19/20 | 20/20 | 4 |
  | z72_0 | 16/20 | 19/20 | 2 |
  | z72_1 | 12/20 | 13/20 | 12 |

  - Weakest: Water Palace act 2 (z22), Huge Crisis act 2 Sonic (z52_0), Altitude Limit act 1 Blaze
    (z61_1). The cause, traced: the stages' objects (pistons, platforms, switches with their bridges,
    rides) move on the race clock, and a CPU that comes back late after an upset or a respawn meets
    them in another phase; lines found at another time then fail. A death in Battle Play respawns at the
    last checkpoint passed, often the start.
  - Tried and measured, no gain (kept out): waiting for the race clock's phase after a death; weighting
    lines by their time harder; a speed / way match on joining; more trials and branches on existing
    files (`-more`: Night Carnival act 1, 22 against 17 of 96 recovered for Sonic, 47 and 47 for Blaze); a
    jump or boost at enemies (it broke lines that bounce off enemies on purpose).
- **Random world (finding).** On the DS and on the Dreamcast the game's random seed `gRandSeed` is
  seeded once at boot (`Rand_SeedFromRTC`) and never reset per race. Item box rings, enemy debris (such
  as Night Carnival's Kuragen bombs) and the other random events therefore differ from race to race. The
  host simulation starts at seed 0, so a line found there can fail in a real race.
  - Found in Flycast (`vsseq.txt`, all 28 pairs in one sequence): Night Carnival act 1 Blaze and act 2
    Blaze did not finish, while act 1 Blaze finished when run alone. Dense traces (`vsseq.txt` fields 6
    and 7 against `test_cpu -dump`) showed the same positions until the rings differed (an item box);
    later an EnemyFlapper, whose flight is random, hit the CPU while it waited at a Shooter.
  - Measured on the host (`test_cpu -rseed`, 24 random seeds per pair, no upsets): 623 of 672 races
    finish. Every pair 24/24 except z41_0 23, z41_1 20, z42_0 3, z42_1 2 and z71_1 23. Night Carnival
    act 2's lines bounce off EnemyFlappers that are elsewhere in most other worlds.
  - Accepted: the lines stay the normal racing paths; when the world takes the CPU off them, the
    recovery above (the rival's path) takes over. The generator's trials run with their own random
    seeds (and a plain random world trial), so lines made again account for it.
- **Vs CPU variation:** the CPU's race seed varies only the easy skill's boost choices and the rival
  interplay; the game's random seed varies the world (above). The stages' objects run on the race clock,
  so a CPU that starts or moves at another time meets them differently (a reaction delay at GO broke the
  lines and was removed).
- **Not tested on hardware.** Flycast runs used one keyboard and a scripted pad for player 2.
  Two physical controllers in Flycast were not tested.

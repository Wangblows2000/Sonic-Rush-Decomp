# Native in-stage presentation and flow (Dreamcast port): translation notes

Status: HUD, score popup, stage title card, pause menu, act results (score
tally and rank), game over / time over, brightness fades, and the
score/time/lives rules are translated. The host test passes (61/61 checks)
with the real ROM tables and converted sprites. The module compiles with
`kos-cc -Werror`. A KOS link test (UI + player + objects + `engine/render.c` +
a stub `main`) links. It has **not** run on SH-4 hardware or in an emulator.
The screen layout was checked only with a host renderer of the quad list
(`tools/render_quads.py`), not on a PVR.

- Code: `dreamcast/game/ui/` (`ui.h` public API, `ui_internal.h`, `ui_*.c`,
  `Makefile`, `test/test_ui.c`, `tools/uidata.py`, `tools/render_quads.py`)
- Source of truth: `src/game/UI/Hud/` (Hud_*, ScorePopup_*),
  `src/game/UI/TitleCard/`, `src/game/UI/GameOver/`, `src/game/Menus/Pause/`,
  `src/game/Cutscenes/ClearDemo/` (the act results screen; `Menus/Results/
  ResultsScreen_*` is the **VS** results screen and is not translated),
  `src/game/Graphics/Screen/` (BrightnessFade_*, ScreenShake_*),
  `src/game/Stage/` (Stage_Main, Stage_ResetProgress, Stage_ExitAfterFail,
  StageExit_*, StageRestart_*, `asm_fallback/Stage_Create.c`),
  `src/game/Player/` (Player_AddLives, Player_AddTrickScore,
  Player_CheckTimeOver, Player_Last, Player_Create, Player_Respawn).
- Third-party code: none.

## 1. Files

| file | content |
|---|---|
| `ui.h` | public API, `UiGame` (game state fields no other module owns), `UiHooks` (sound) |
| `ui_internal.h` | `UiRes`/`UiSpr` (sprite resources, AnimatorSprite subset), `UiData` (ROM tables), draw command API, internal prototypes |
| `ui_data.c` | `uidata.bin` loader, `frames.bin` loader, UiSpr animation, draw commands -> screen quads (layout spaces) |
| `ui_state.c` | stage flow: Stage_Main part, stage entry/restart, lives, score, time over, Player_Last exits, Stage_ExitAfterFail, BrightnessFade, player/object hooks, `ui_update` / `ui_post_update` |
| `ui_hud.c` | Hud_*, ScorePopup_*, native score counter |
| `ui_titlecard.c` | StageTitleCard_* and all its parts |
| `ui_pause.c` | PauseMenu_* |
| `ui_results.c` | ClearDemo_*, ClearDemoScore_*, ClearDemoName_*, ClearDemoActText_*, ClearDemoBars_*, ClearDemoBg/Model state timing |
| `ui_gameover.c` | GameOverSeq_*, GameOver_*, ScreenShake_* (local copy) |
| `ui_draw.c` | KOS: atlas upload, palette-effect texture variants, TR-list quads, brightness overlay. Host: stubs |
| `tools/uidata.py` | BYOR converter: ROM tables -> `build/dreamcast/ui/uidata.bin` |
| `tools/render_quads.py` | dev tool: renders a quad dump of the host test to PNG |

## 2. Integration (main.c)

```c
#include "../ui/ui.h"

/* once */
ui_set_paths("/cd/ui", "/cd/sprites");
ui_init();                                  /* uidata.bin + 7 sprite sets */
g_ui_hooks.sfx = ...; g_ui_hooks.voice = ...; g_ui_hooks.music = ...; g_ui_hooks.music_pause = ...;
g_dcp_world.mode = UI_MODE_STORY;           /* gGameState.unk10 */
g_ui_game.save_lives[0] = g_ui_game.save_lives[1] = 3;   /* see section 6 */

/* stage start (spawn_player): after dcp_player_create and objects_init */
ui_bind_player(&g_player);
ui_stage_start(k_stages[idx][1] - '1', k_stages[idx][2] - '1', 0 /* Sonic */);
ui_set_camera(g_cam_x, g_cam_y);

/* per frame */
input_poll(&in);
ui_update(&in, &g_player);                  /* Stage_Main + pause/results/game-over tasks */
if (!ui_is_paused() && !ui_world_frozen()) {
    objects_set_view(...); objects_frame_begin();
    dcp_player_frame(&g_player, ds_pad(in.held), ds_pad(in.pressed), ds_pad(in.released));
    objects_update(&g_player);
    /* camera: snap (no damping) while ui_titlecard_active() - see below */
}
ui_post_update(&g_player);                  /* every frame, also while paused */
ui_set_camera(g_cam_x, g_cam_y);            /* ScorePopup is in world space */
...
render_begin_list(PVR_LIST_TR_POLY);
... other TR geometry ...
ui_draw_tr();                               /* last: HUD/menus at z 100..200 */
render_end_list();

if (ui_state() == UI_EXIT) {
    switch (ui_exit_kind()) {
    case UI_EXIT_NEXT_ACT:   /* load the next stage, then ui_stage_start(...) again */
    case UI_EXIT_QUIT:       /* pause "GO BACK": zone map in the original */
    case UI_EXIT_GAME_OVER:  /* SceneChange_ToSegaLogo in the original */
    case UI_EXIT_TRY_AGAIN:  /* time attack */
    case UI_EXIT_VS_RESULTS: ;
    }
}
```

Required changes in main.c (not done here, main.c is not in this area):

- Remove `if (in.pressed & IN_START) break;` and the B/Y stage switch: START
  opens the pause menu, B is the trick button.
- Call `ui_post_update` every frame, including paused and frozen frames.
- `ui_update` has to run before the world update and `ui_post_update` after it
  (DS task order: Stage_Main, player, objects, ScorePopup 0xa00, HUD 0x3800,
  BrightnessFade 0x3e00, StageTitleCard 0x3efe, PauseMenu 0x6100).
- During the title card, `StageTitleCard_HoldPlayerX` forces RIGHT and wraps
  the player x inside a 256 px window (`dx & 0xffff`, the player runs on the
  spot). The DS camera snaps to the player every frame there
  (`gCamera.x = player.x - offset`). main.c's damped camera would show the
  wrap as a jump: snap the camera while `ui_titlecard_active()`.
- Timer: Stage_Main owns `gGameState.unk20` (`g_dcp_world.frame`, +1 per frame
  while `flags & 1`). `dcp_player_frame` also does `W.frame++` (a stand-in
  from before this module existed). `ui_post_update` restores the Stage_Main
  value, so the timer counts once per frame. Objects updated after the player
  see timer + 1 during that frame. Better fix: remove the `W.frame++` in
  `dcp_player_frame` (player owner).
- `ui_stage_start` installs hooks: `g_dcp_hooks.score`,
  `score_active`, `add_life`, `tension_hud`, `fade`, `fade_done`, `restart` and
  `g_obj_hooks.fade`, `stage_exit`. Each chains to the hook that was there
  before. `objects_shutdown` restores the hooks that were there before
  `objects_init`. Call `ui_stage_start` (or `ui_install_hooks`) after
  `objects_init`.
- Death: `Player_Respawn` calls `g_dcp_hooks.restart`. The UI hook runs
  `ui_stage_restart` (StageRestart -> Stage_Create: timer reset, new HUD, new
  title card with `gGameState.unk18 |= 2`, so the player is not held) and then
  chains to main.c's restart hook. That hook must reset the level and objects
  (`objects_respawn_view`), as StageRestart reloads the stage on the DS.
- The player model close-up of the title card is not drawn
  (StageTitleCardPlayer_Draw). `ui_titlecard_player_view(&scale, &x, &y)`
  returns the original values: model scale 0x199..0x1000 and screen position
  (20.12, DS screen). A model renderer can use them.

## 3. Assets the disc needs

```
/cd/ui/uidata.bin                                   (4.5 KB, tools/uidata.py)
/cd/sprites/<id>/frames.bin + atlas0.argb1555       (from build/dreamcast/sprites/bac/<id>/)
```

| id | original resource | used by | atlas | VRAM |
|---|---|---|---|---|
| `act__ac_fix_moji` | act_com file 0 `ac_fix_moji.bac` | HUD (Hud_Create), ScorePopup, score counter | 128x128 | 32 KB |
| `act__dm_stdm` | act_com file 2 `dm_stdm.bac` | title card res[0] | 512x256 | 256 KB |
| `act__dm_stdm_eng` | act_com file 4 (`data_02076b74[lang]`; `act__dm_stdm_jpn` = file 3 for Japanese) | title card zone names res[1] | 128x256 | 64 KB |
| `act__dm_cldm` | act_com file 0x19 `dm_cldm.bac` | act results (ClearDemo_Create) | 512x256 | 256 KB |
| `act__ac_fix_pause` | `/act/ac_fix_pause.bac` | pause menu | 128x256 | 64 KB |
| `act__ac_fix_pause_frame` | `/act/ac_fix_pause_frame.bac` | pause menu frame | 128x64 | 16 KB |
| `act__ac_fix_gameover` | `/act/ac_fix_gameover.bac` | GAME OVER / TIME'S UP letters | 64x128 | 16 KB |

Plus one 256x256 ARGB1555 variant cache texture (128 KB), allocated on first
use. In total about 830 KB of PVR VRAM, all loaded at `ui_init`. The atlases
also stay in main RAM (about 0.7 MB) because the variant cache is built from them.
If VRAM gets tight, `dm_cldm` can be loaded only when the results start.

`build_disc.sh` is not in this area. The integrator copies these two items.
Build them with `make -C dreamcast/game/ui data` (uidata.bin). The sprites come
from the existing `tools/dreamcast/sprites/bac2dc.py` output.

## 4. How the original is mapped

### 4.1 State ownership

| original | here |
|---|---|
| gGameState.unk1C / unk1D (zone, act) | `g_dcp_world.zone/act` |
| gGameState.unk10 (mode: 0 story, 1 VS, 2 time attack) | `g_dcp_world.mode` |
| gGameState.unk18 (1 timer runs, 2 restarted, 0x20 VS, 0x40 special stage return) | `g_dcp_world.flags` |
| gGameState.unk20 (stage timer, frames) | `g_dcp_world.frame` |
| gGameState.unk24 | `g_ui_game.frame24` |
| gGameState.unk0 (character) | `g_ui_game.character` |
| data_022c4590.unk0 (tally total) | `g_ui_game.total` |
| data_022c4590.unk4 (score) | `g_ui_game.score` |
| data_022c4590.unk8 / unk10 (speed / ring bonus) | `g_obj_stage.speed_bonus / ring_bonus` (objects module, goal) |
| data_022c4590.unk14 (lives) | `g_dcp_world.lives` |
| data_022c4590.unk15 / unk18 (marker index / time) | `g_obj_stage.check_idx / check_time` |
| data_022c4590.unk24 (1/2 progress initialised, 4 time over) | `g_ui_game.fail_flags` |
| data_022c4590.unk40 | `g_obj_stage.goal_state` |
| gSaveData.unkE / unk66 (lives per character) | `g_ui_game.save_lives[2]` |
| gSaveData.unk8 bit 1 (no time limit) | `g_ui_game.save_flags` |
| gGameLanguage | `g_ui_game.language` (default 1, English) |
| gDisplayMain/Sub master brightness | `g_ui_bright_main/sub` -> one overlay quad (the stronger of the two) |

### 4.2 Rules (all from the code)

- Timer: +1 per frame while `flags & 1` (Stage_Main). The title card clears
  bit 0 at create and sets it in `StageTitleCard_State_StartStage`. The goal
  clears it (objects module). Display clamp 9'59"99 (`0x8c9f`).
- Time over (Player_CheckTimeOver): at `0x8c9f` frames in story mode (unless
  the "no time limit" save option is set) and in time attack: marker time = 0,
  `unk24 |= 4` (no time bonus), Player_Action_Die. From 0x87ef, a warning sound
  plays every second (`g_ui_hooks.sfx`: not wired, see section 6).
- Lives: Player_AddLives adds n, clamps at 99 (u8 arithmetic), no-op in VS and
  time attack. 100 rings and the 1-up box use it via `g_dcp_hooks.add_life`.
  Player_Respawn (player module) decrements, or sets `DCP_PF_GAMEOVER` at 0.
  Player_Last -> StageExit -> Stage_ExitAfterFail: story mode saves
  `max(lives, 2)` and starts TIME'S UP (time > 0x8c9f) or GAME OVER.
- Score: only Player_AddTrickScore adds score in the original. ScorePopup_Create
  is referenced only there. Enemies give no score. `ui_add_score` is a native
  entry for other callers.
- Results (ClearDemo_InitScoreTally): rows TIME BONUS
  (`ClearDemo_GetTimeBonus`: first `data_020791d0[zone][act]` threshold >=
  timer -> `data_02078f90` / boss `data_02079008`), TRICK BONUS
  (`ClearDemo_GetScoreBonus`: first `data_02079198[zone][act]` threshold <=
  score -> `data_02078ef0`), RING BONUS (rings x 10 at the goal), SPEED BONUS
  (goal/marker average). Boss acts show only time and ring rows. Each value
  is clamped at 999999. Rank: first threshold of `data_02078b58` (boss
  `data_02078b48`) <= total -> S, A, B, C (dm_cldm anims 35..38). The total
  goes to `data_022c4590.unk0`.

### 4.3 Layout (DC redesign)

- The DS HUD coordinates are kept (they already describe one 256x192
  screen: rings top-left, timer top-centre, tension gauge on the left from y 50 to
  170, lives under it). They are drawn at an integer 2x scale (sharp pixel art)
  in three anchor groups inside the 4:3 safe area (32,24)-(608,456):
  rings and time at top-left, gauge and lives at bottom-left
  (DS y 192 = safe bottom), score at top-right. `ui_hud_layout()` changes the scale and safe area.
- Score counter: **native addition**. The DS HUD has no score. Six white HUD
  digits, leading zeros hidden, on the timer baseline at the top right.
  `Hud_SetFlags(Hud_GetFlags() | 0x40000000)` hides it.
- Title card, pause menu, results, and game over use DS screen coordinates
  scaled by 2.5 to 640x480 (the same framing as the world view).
- Draw order: the DS OAM rule (the first sprite drawn is in front) applies
  within a layer. Layers: score popups < HUD < title card < results < pause <
  game over < brightness overlay.

### 4.4 DS services replaced

| DS | native |
|---|---|
| Task_Create / task list order | one static work struct per task. `ui_update` / `ui_post_update` run them in the DS task order |
| AnimatorSprite(DS)_Init / ProcessAnimation / SetAnimation / Draw / DrawRotScale | `UiSpr_*` on converted `frames.bin` + `ui_emit` (scale about the sprite origin; the HUD gauge pulse scales x only, as the original) |
| OBJ VRAM glyph swapping (HUD digits: `unk5C = vram + d * 0x80`) | digit animation `d` (white) / `d + 10` (orange) of ac_fix_moji |
| PaletteTransfer of the gauge colours, PauseMenu palette dimming | texture variants built from the atlas pixels (`ui_draw.c`) with the ROM colours / LUT |
| G2/G3 master brightness | overlay quad: black (< 0) / white (> 0), alpha = \|b\| / 16 |
| REG_DIV, FX_DivS32 | C `/` and `%` (operands are non-negative) |
| gPad (trg / held / rep) | `InputState` -> DS bit order (`ui_pad_bits`). Key repeat (`rep`) = presses |
| System_Freeze, NNS_SndPlayerPauseByPlayerNo | `ui_is_paused()` + `g_ui_hooks.music_pause` |
| Sound_PlaySeqArc(Ex) / PlayVoice / PlaySeq / NNS_SndStopSoundAll | `g_ui_hooks.sfx/voice/music` with the original ids |
| StageExit_Create / StageShutdown_Create / SceneChange_* | `ui_state() == UI_EXIT` + `ui_exit_kind()` |
| Camera_WorldToScreen | `ui_world_to_screen` (camera from `ui_set_camera`) |

## 5. Function mapping

Classification: **as-is** = same statements and constants (types/helpers
renamed only); **modified** = same logic, DS calls/registers/VRAM replaced;
**rewritten** = behaviour kept, code substantially different; **new** = native
only; **not translated** = see section 6.

| original | translated | status |
|---|---|---|
| Hud_Create | `Hud_Create` | modified (UiSpr per element, no VRAM/palette setup) |
| Hud_Destructor | `Hud_Destroy` | rewritten (no VRAM to free) |
| Hud_Main | `Hud_Main` | modified (lifted template -> structured; calls native score counter) |
| Hud_DrawRings / Hud_DrawTime / Hud_DrawLives | same names (static) | modified (one draw per sprite instead of two engines; orange glyph set = anim + 10) |
| Hud_DrawTensionGauge | same | modified (palette copies -> variants; REG_DIV -> C) |
| Hud_UpdateTensionGauge / Hud_SetTensionGauge / Hud_TensionToGaugeValue | same | as-is (Rand -> UiRand on gRandSeed) |
| Hud_ToDecimalDigits / Hud_FramesToTimeDigits | same | as-is (divisor table from uidata) |
| Hud_SetLifeCount / SetRingCount / SetTime / SetFlags / GetFlags | same | as-is |
| Hud_GetDigitGlyphVram / Hud_GetDigitGlyphVramB | digit anim (+10 for B) | rewritten |
| Hud_SetOamPriority / 0 / 2 / 3 | `Hud_SetOamPriority` | rewritten (priority 3 = under the title card BG -> not drawn) |
| - | `Hud_SetScore`, `Hud_DrawScore`, `Hud_SetTensionMax` | new |
| ScorePopup_Create | `ScorePopup_Create` / `ui_score_follow` | modified (parent object -> followed position pointers) |
| ScorePopup_State_Main / ScorePopup_IsActive | same | as-is |
| ScorePopup_Draw | same | modified (StageObj_Draw2DEx scale rule kept, world space quad) |
| ScorePopup_Destructor | `ScorePopup_Destroy` | modified (player unkC8/unk110 not modelled) |
| StageTitleCard_Create | same | modified (no VRAM/BG/3D setup; unk1E8/unk258/unk25C, HUD priority, player locks kept) |
| StageTitleCard_Main | same | modified (lifted template -> structured) |
| StageTitleCard_InitSprites | same | modified (same anim choices, no VRAM/palette alloc) |
| StageTitleCard_State_* (Init, StartDelay, BeginFadeIn, FadeIn, BeginShow, ShowDelay, ShowParts, BeginHold, Hold, ShowText, WaitText, BeginHoldText, HoldText, ShowText2, WaitText2, BeginExit, Exit, ToStartStage, WaitStartStage, BeginRestoreColors, RestoreDisplay, FinalDelay, Finish) | same | as-is (brightness registers -> g_ui_bright_*) |
| StageTitleCard_State_StartStage | same | modified (camera flag writes and BG/DISPCNT setup dropped; NaviMsg for bosses not created) |
| StageTitleCard_State_RestoreColors | same | modified (light/edge colour lerps omitted) |
| StageTitleCard_HoldPlayerNone / X / Y | same | modified (gCamera snap left to main.c) |
| StageTitleCard_DrawGlyphText | same | as-is |
| StageTitleCardStrip_*, Part20_*, PartE0_*, Part180_*, Part260_*, Part324_*, Text3E8_*, Text404_*, Part420_* (State functions) | same | as-is |
| the Part Draw functions | same | modified (AnimatorSprite_Draw -> ui_emit; PartE0 rotation dropped: the art is a disc) |
| StageTitleCardBg_State_* | same | as-is (state only) |
| StageTitleCardBg_Draw | - | not translated (BBG) |
| StageTitleCardPlayer_State_* | same | as-is, except Zoom: modified (Camera_WorldToScreen -> ui_world_to_screen; perspective angle term fixed at 0x3c000) |
| StageTitleCardPlayer_Draw | `ui_titlecard_player_view` | not translated (values exported) |
| StageTitleCard_InitBg / InitBgBoss / LerpColor / ReleaseSprites / Destructor / CreateExtraBoss / DrawPlayer2Hidden | - | not translated |
| PauseMenu_Create | same | modified (System_Freeze -> pause flag; Load2dBAC -> UiSpr; re-entry guard) |
| PauseMenu_Main / PauseMenu_Draw / PauseMenu_Main_Quit | same | modified (logic as-is) |
| PauseMenu_HighlightSelection | UI_VAR_DIM in PauseMenu_Draw | rewritten (same LUT on the pixels) |
| PauseMenu_QuitToMap / QuitToTryAgain | same / `ui_request_exit` | rewritten (save lives, exit kind) |
| PauseMenu_Destructor | `PauseMenu_Destroy` | modified |
| ClearDemo_Create | same | modified (no BG/3D/sound archive setup; TA tally not created) |
| ClearDemo_Main | same | modified (draws a black backdrop for the BG) |
| ClearDemo_State_FadeIn / ShowBg / ShowName / ShowModel / WaitParts / Hold / FadeOut / Finish | same | as-is |
| ClearDemo_SkipParts / ClearDemo_SkipToRank | same | as-is |
| ClearDemo_Finish / ClearDemo_GoToNextScene | `ClearDemo_GoToNextScene` | rewritten (scene changes -> exit kind) |
| ClearDemo_InitScoreTally / GetTimeBonus / GetScoreBonus / GetRank | same | as-is (tables from uidata) |
| ClearDemoScore_State_* / SkipRows / SkipToRank / Draw | same | as-is / Draw modified |
| ClearDemoName_* / ClearDemoActText_* / ClearDemoBars_* | same | as-is / Draw modified |
| ClearDemoBg_State_* , ClearDemoBg_Skip | same | modified (timing only, no BG registers) |
| ClearDemoModel_State_Init/Delay/Move/Done, ClearDemoModel_Skip | same | modified (timing only, no model) |
| ClearDemo_InitTimeAttackTally, ClearDemoTA_*, ClearDemo_DrawModel, ClearDemoBg_Draw, ClearDemo_InitBg(Boss), ClearDemo_InitSprites palette part, ClearDemo_Destructor | - | not translated |
| GameOverSeq_StartGameOver / StartTimeOver | `GameOverSeq_Start` (shared) | modified (DISPCNT -> black backdrop) |
| GameOverSeq_Main | same | modified (raw KEYINPUT -> held) |
| GameOverSeq_Destructor | `GameOverSeq_Destroy` | modified |
| GameOver_Create / GameOver_State_Animate | same | as-is (StageObj task -> letter slot) |
| ScreenShake_Start / Stop / Main | same (static, game over only) | as-is |
| BrightnessFade_Create / Main / Apply / IsDone / Destroy | same | as-is (registers -> g_ui_bright_*) |
| Stage_Main (HUD feed, timer, pause request) | `ui_update` | modified |
| Stage_ResetProgress | same (static) | as-is (Camera_Reset / StageEvent_Reset belong to other modules) |
| Stage_Create (timer reset, HUD flags 4/0x10) | `Stage_Create_UI` | modified (only the UI part) |
| Player_Create Init_0234d62c | `Player_Create_InitProgress` | modified |
| Player_AddLives | `ui_add_lives` | modified (lifted template -> C; 1-up jingle via hook not wired) |
| Player_AddTrickScore (score part) | hook `h_score` | modified |
| Player_CheckTimeOver | same (static) | modified (VS branch: ReachGoal left to the player module) |
| Player_Last (exit checks) | `Player_Last_Exits` | modified (special stage exit not handled) |
| Stage_ExitAfterFail | same | modified (SaveData -> save_lives; scene changes -> exit kinds) |
| StageExit_Create / StageExit_Main | deferred callback in `ui_update` | rewritten |
| StageRestart_Create (UI part) | `ui_stage_restart` | rewritten (deferred to ui_post_update) |
| Math_LerpFx32 / StoryProgress_GetZoneIndex | same | as-is |
| ObjShift_Set / ObjShift_SetFromRef | objects module (`obj_fx.c`) | reused |

## 6. Gaps and assumptions (honest list)

- **Not drawn**: the title card and results backgrounds (`stdm_bg*.bbg`,
  `cldm_*.bbg`: there is no BBG converter yet. The title card draws over the live
  stage, and the results draw over black). The player model close-up of the
  title card and the results model are not drawn either.
- **Not translated**: time-attack tally (ClearDemoTA_*: time attack uses the
  score tally), CheckpointTimePopup / MarkerTimeList (time attack), VS results
  (ResultsScreen_*), EnemyRoomHud, BossHelp, NaviM/NaviB/NaviMsg (boss
  bottom-screen helpers), special-stage pause and exit, extra boss (zone 8 act 2)
  special cases beyond the brightness ones.
- The Blaze palette reload of the results glyphs (ClearDemo_InitSprites) is
  not done: Blaze's results use the BAC's own palette.
- Lives on a new game: `gSaveData.unkE/unk66` come from the save file. The
  save-file default was not traced. `save_lives` defaults to 3 (assumption).
- `StoryProgress_IsStageNew()` is not modelled. `g_ui_game.stage_new = 0`
  (default) shows CONTINUE / GO BACK. The original shows only "PAUSE" on a first
  visit (story) and always shows the entries in time attack.
- The pause menu "GO BACK" leads to the zone map in the original. Here it is
  `UI_EXIT_QUIT`. The original has no "restart" entry, and there is no art for one.
- Sound: every call site passes its original arc/id to `g_ui_hooks`.
  Nothing is mapped to DC audio yet: the ids are SDAT sequence-archive
  numbers. The countdown beep (`Sound_PlaySeqArcEx(0, 0x36)`) and 1-up jingle
  (`Sound_PlayBGMOnPlayer3(0x18)`) are not wired, only noted.
- `StageTitleCardPlayer_State_Zoom` uses the stage perspective angle
  (`data_ov000_0236a444`) for one camera term. It is fixed at 0x3c000 (no change).
- DrawRotScale pivots: the DS scales each OAM piece about its own centre. Here
  scaling is about the sprite origin. The title card parts compute their own
  pivot as in the original. Small offsets are possible on the scaled parts.
- `ui_draw.c` writes the variant cache with 16-bit stores into PVR VRAM
  (64-bit area). This is common practice, but it has not been verified on hardware.

## 7. Verification (what was run)

- `make -C dreamcast/game/ui data` -> `uidata.bin` (35 sections, 4.5 KB).
  Spot checks against the decompiled tables: gauge colours `7fff 7e00 031f 001f`,
  time divisors `2160000 .. 60, 10`, rank thresholds `100000/80000/50000/0`,
  first time-bonus threshold table `3599 .. 15599, 0xffffffff`.
- `make -C dreamcast/game/ui host-test`: 61/61 checks. Measured on the real
  tables and sprites:
  - digits (ring/time/score conversion, clamp 9'59"99);
  - bonus tables and ranks (85000 time bonus under 1 minute, 10000 trick bonus
    at 20000 points, rank boundaries, boss ranks);
  - lives clamp and mode rules;
  - title card: the timer starts on frame 283 after stage start (7 delay +
    26 fade-in + text sequence, then the exit waits for the 206-frame player
    zoom). The task ends 62 frames later. A/B skips to StartStage on the next frame;
  - tension gauge: fills 2 per frame, drains 1 per frame;
  - HUD quads stay inside the safe area;
  - score popup: 63 frames for a 2-step trick;
  - pause: freezes the timer, START resumes, GO BACK exits after the 17-frame fade;
  - results: total 99570 (rank A) and 864 frames to exit; skipping with A
    twice exits 139 frames later;
  - game over: 529 frames to exit, saved lives clamped to 2;
  - time over: kills the player and sets the no-time-bonus flag.
- `make -C dreamcast/game/ui dc`: all 8 sources compile with `kos-cc -std=gnu99
  -O2 -Wall -Wextra -Werror`.
- KOS link test (`/tmp/ui/link`): UI + player + objects + `engine/render.c` + stub main -> `link.elf`
  links with no unresolved symbols.
- Visual check (host only): `UI_DUMP=dir test_ui ...` then
  `tools/render_quads.py` gives PNGs of the HUD, title card, pause, results and
  game over. They were inspected: layout, digit order, gauge colours, pause dimming, rank
  stamp, letter spacing.

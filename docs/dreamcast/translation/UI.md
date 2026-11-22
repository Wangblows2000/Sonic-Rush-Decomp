# Native in-stage presentation and flow (Dreamcast port): translation notes

Status: HUD, score popup, stage title card (with its BBG backgrounds), pause
menu, act results (score tally, rank and BBG backgrounds), game over / time over,
brightness fades, the score/time/lives rules, and the boss-stage second-screen
helpers (NaviB boss health panel, NaviMsg navigator messages and face,
BossHelp) and the enemy-room counter (EnemyRoomHud) are translated. The host test
passes (93/93 checks) with the real ROM tables and converted assets. All 11
sources compile with `kos-cc -O2 -fwrapv -Wall -Wextra -Werror`. It has **not**
run on SH-4 hardware or in Flycast: the screen layout was checked only with the
host renderer of the quad list (`tools/render_quads.py`), not on a PVR.

- Code: `dreamcast/game/ui/` (`ui.h` public API, `ui_internal.h`, `ui_*.c`,
  `Makefile`, `test/test_ui.c`, `tools/uidata.py`, `tools/uibg.py`,
  `tools/render_quads.py`)
- Source of truth: `src/game/UI/Hud/` (Hud_*, ScorePopup_*, BossHelp_*,
  EnemyRoomHud_*), `src/game/UI/Navi/` (NaviB_*, NaviMsg_*, NaviMsgFace_*, NaviBg_Load),
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
| `ui_draw.c` | KOS: atlas upload, palette-effect texture variants and colour maps, streamed navigator face, TR-list quads, per-screen brightness. Host: stubs |
| `ui_navi.c` | NaviB_* (boss health panel), NaviMsg_* / NaviMsgFace_* (navigator), BossHelp_*, the DC composition of the boss-stage overlays |
| `ui_enemyroom.c` | EnemyRoomHud_* and the `ui_enemy_room` feed (OBJ_FX_ENEMY_ROOM) |
| `ui_bg.c` | title card BG layers (StageTitleCardBg_Draw) and results BG layers (ClearDemoBg_Draw) |
| `tools/uidata.py` | BYOR converter: ROM tables -> `build/dreamcast/ui/uidata.bin` |
| `tools/uibg.py` | BYOR converter: title card / results BBGs, navigator panels, faces and tables -> `build/dreamcast/ui/res/`, `navidata.bin` (reuses `menus/tools/menubg.py`) |
| `tools/render_quads.py` | dev tool: renders a quad dump of the host test to PNG |

## 2. Integration (main.c)

```c
#include "../ui/ui.h"

/* once */
ui_set_paths("/cd/ui", "/cd/sprites");
ui_init();                                  /* uidata.bin, navidata.bin + 10 sprite sets */
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

Boss stages, enemy rooms and the pause menu (added with the navigator work):

```c
/* once (with the other hooks) */
g_boss_hooks.navi = ui_boss_navi;            /* NaviMsg_SetFaceAnim + NaviMsg_ShowMessage */
g_ui_hooks.voice = ...;                      /* navigator voices: (arc, id), arc 0 */
g_ui_hooks.stage_new = mn_stage_new;         /* int mn_stage_new(void *c) { return StoryProgress_IsStageNew(); }
                                                (menus gGameState zone/act/character must be the stage's) */
/* stage start, before ui_stage_start */
g_ui_game.language = <gGameLanguage>;        /* 0 jpn 1 eng 2 fra 3 deu 4 ita 5 spa (data_0207b9bc) */
g_ui_game.difficulty = <gGameState.unk14>;   /* same value as boss_set_difficulty(): NaviB gauge 6 / 8 */
g_ui_game.save_progress[0] = gSaveData.unkC; /* navigator gate (no Tails before progress 3) */
g_ui_game.save_progress[1] = gSaveData.unk64;
/* every frame of a boss stage, before ui_update */
ui_set_boss_health(boss_health());           /* gBossStatus.unk8 */
/* g_obj_hooks.effect, case OBJ_FX_ENEMY_ROOM (replaces the g_room_* statics) */
ui_enemy_room(a, b);                         /* a 0 start (b = enemies), 1 count (b = defeated), 2 cleared */
/* Player_OnHurt (boss stages): after dcp_on_hurt's 0x40 sound, before the death checks */
ui_navi_player_hurt();                       /* needs a player-module hook, see section 6 */
/* Boss6/7/F (when translated): BossHelp_Create(idx) / BossHelp_Destroy() */
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
- The player model close-up of the title card (StageTitleCardPlayer_Draw) is
  drawn by the player model module (player/dcp_model.c,
  `dcp_model_titlecard_view`). `ui_titlecard_player_draw(&speed, eye, at, &x, &y,
  &zoom)` returns 1 while the close-up replaces the player's own draw
  (StageTitleCardPlayer_State_Init .. _Finish / StartStage) and gives the
  per-slot animation speed (obj unk24C, 0x199 .. 0x1000: the slow-motion run),
  the LookAt eye / target (v4b4 / v4c0) and, in the zoom phase (flags2 0x400),
  the model translation for the off-axis frustum. The zoom ends at the stage
  camera distance (`cam_h` = FX_Div(cos, sin) * 0x60 of angle 0x1555 =
  0x1bb6 * 0x60; it was 0x3c000 before, which ended the zoom 2.7x too close).
  Layering: the DS shows 3D BG0 between the title card BG and the title card
  sprites. While the close-up shows, `ui_draw_tr` moves the title card sprites
  and every later layer up by `UI_TC_GAP` (2000) and the model is drawn in the
  opaque lists with its PVR depth scaled into (`UI_TC_MODEL_Z0`,
  `UI_TC_MODEL_Z1`) = (400, 1900); the brightness overlay is at 2200.
  `ui_titlecard_player_view` stays for compatibility.

## 3. Assets the disc needs

```
/cd/ui/uidata.bin                                   (4.5 KB, tools/uidata.py)
/cd/ui/navidata.bin                                 (2.7 KB, tools/uibg.py)
/cd/ui/res/<dir>/frames.bin + atlas*.argb1555       (tools/uibg.py; not the atlas*.png previews)
/cd/ui/res/navifaces_t.bin, navifaces_c.bin         (431 KB, 353 KB)
/cd/sprites/<id>/frames.bin + atlas0.argb1555       (from build/dreamcast/sprites/bac/<id>/)
```

`make -C dreamcast/game/ui data` builds both converters. `res/` holds 34
directories (about 4.4 MB without the PNG previews):

| dir | content | page | loaded |
|---|---|---|---|
| `tcbg_6` .. `tcbg_14` | stdm_bg1..7, f, e (256x192) + stdm_zigzag pieces (Sonic / Blaze colours) | 256x256 | title card of a normal act (StageTitleCard_Create) |
| `tcbg_16` .. `tcbg_24` | stdm_bs_bg* + stdm_bs_zigzag pieces | 256x256 | title card of a boss act |
| `cldm_0`, `cldm_1` | cldm_far (character colours) + cldm_bg tile | 256x256 | ClearDemo_Create, freed with the results |
| `navi_<t\|c>_<lang>` | BOSS box of `<c>_b_b`, `b_bal` balloon, messages 15..19, face backing | 256x256 | boss stages (ui_stage_start) |

The three new sprite ids `act__navi_b`, `act__ac_fix_bosshelp` and
`act__ac_eff_enemyroom_fix` come from `bac2dc.py` like the others (atlases 64 KB,
32 KB, 64 KB, loaded at `ui_init`). PVR VRAM added: about 128 KB per loaded `res/`
directory, the 128x128 face texture (32 KB), and the three sprite atlases.

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
| gDisplayMain/Sub master brightness | `g_ui_bright_main` -> frame overlay; `g_ui_bright_sub` -> vertex colour of composed sub-screen quads |
| gBossStatus.unk8 (boss hit points) | `ui_set_boss_health()` (from `boss_health()`) |
| gGameState.unk14 (difficulty) | `g_ui_game.difficulty` |
| gSaveData.unkC / unk64 (story progress, navigator gate) | `g_ui_game.save_progress[2]` (default 0xffff: navigator on) |
| EnemyRoom_GetActive(1)->unk212 - unk213 | `ui_enemy_room()` need / defeated (objects module, OBJ_FX_ENEMY_ROOM) |
| data_0207b894 / data_0207b8a4 (navi mode / character) | boss stages: mode 1, `g_ui_game.character` |

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
- **One frame, one composition.** The frame is the DS main screen. Everything the
  DS showed on the second (touch) screen is composed into it, never as a second
  DS panel and with no dividing line. All art is the original, scaled uniformly
  (2x for HUD-class overlays, 2.5x for full-frame cards):

```
 (32,24) safe area                                                     (608,24)
 +---------------------------------------------------------------------------+
 | [ring 057] [clock 0:01:01]                                        [score] |  HUD row
 | +--------------------------------------------+  +----------------------+ |
 | | balloon (b_bal) + message panel (eng15..19) |  | face over <c>_b_b     | |  navigator,
 | +--------------------------------------------\  | (104x80 DS, 2x)       | |  timed (boss)
 |                                                 +----------------------+ |
 |            [ENEMY 05]  enemy-room counter (HUD_TC, normal acts)   [BossHelp]
 |                                                 +----------------------+ |
 |     (player / play area kept clear)             | BOSS box: counter,   | |  boss panel
 | [lives]                                         | ZONE n, gauge (2x)   | |
 +---------------------------------------------------------------------------+
```

  - Boss panel (NaviB): the BOSS box of `<c>_b_b.bbg` (DS sub px (16,79)-(121,135),
    the frame the NaviB sprites sit in) with its sprites at their original
    sub-screen offsets, 2x, bottom-right corner of the safe area (398..608,
    344..456). Present from StageTitleCard_State_StartStage (boss acts) to the
    stage exit. The "SONIC THE HEDGEHOG" header and the blue backdrop of the sub
    screen are not drawn.
  - Navigator message (NaviMsg): a timed overlay under the HUD's top row, only
    while the DS message plane (sub BG0) is on (`NaviMsg_ShowMessage` time,
    0xf0 / 0x96 frames), with a 6-frame native fade. The balloon is at 2x
    (360x96 px, x 32..392, y 64..160), the message panel at its original offset
    inside it; the face (head and gestures: crop (12,0)-(116,80) of the 128x128
    frame, over the `<c>_b_b` background of that place) at 2x to its right
    (x 396..604, y 64..224). On the DS the face stays on the touch screen
    between messages; here it shows only with a message.
  - BossHelp: main-screen OBJ at its DS place (bottom right), HUD scale, moved up
    above the boss panel while the panel exists (500..582, 224..320).
  - EnemyRoomHud: main-screen StageObj; HUD art in `UI_SP_HUD_TC` (DS x 128 =
    screen centre, DS y 0 = 24 DS px under the safe top, 2x), so its end
    position (212,12) lands under the score instead of on it (414..564, 84..140).
  - Title card: the frame is the main screen with the sub screen's zone art
    composed in, in the sub screen's own layer order: stage < stdm_zigzag (main
    BG2, scrolled by StageTitleCardBg) < stdm_bg<zone> (sub BG3) < title card
    sprites, all 2.5x. Drawn while `flags2 & 1` (from StageTitleCardBg_State_Init
    to StageTitleCard_State_StartStage). Boss: the zigzag band only inside
    window 0 (DS x 0x38..0xc0).
  - Results: cldm_bg (BG2/BG3, one colour) < cldm_far (BG1, revealed by window 0,
    a 2-px line that opens over 10 frames, then the whole layer) < tally sprites.
  - Brightness: gDisplayMain's master brightness is the full-frame overlay. The
    composed sub-screen quads (zone art, navigator, boss panel; `UiQuad.screen`)
    take gDisplaySub's brightness on their vertex colour: black fades darken
    them, white fades fade them out over what is behind (a textured quad cannot
    be brightened to white here). The previous "stronger of the two screens"
    overlay is gone: at StageTitleCard_State_StartStage the DS turns only the sub
    screen white, and the frame no longer flashes white with it.
- Draw order: the DS OAM rule (the first sprite drawn is in front) applies
  within a layer (BG pieces are emitted last, so they are behind). Layers: score
  popups < HUD < navigator / boss panel / enemy room / BossHelp < title card BG <
  title card < results BG < results < pause < game over < brightness overlay.

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
| BBG backgrounds (Background_Init/InitEx/Draw, PaletteTransfer of 16 character colours) | `tools/uibg.py`: BBG -> ARGB1555 pieces with the palette the code builds (tile base from the char VRAM address), drawn as clipped quads; uniform BG fills as one stretched texel (checked by the converter) |
| BG affine scroll / wrap, window 0 (title card, results) | quad offsets and clip rectangles (`ui_bg.c`) |
| sub-screen OBJ palette rows 7/8 (NaviB_Create tmp sprites) | colour maps from the anims' own images (`ui_palmap_set`, UI_VAR_MAP0+n) |
| NaviMsg face streaming (FS_ReadFile of chr/map halves, VBlank BG1CNT swap) | the same 3-frame flag pipeline; the shown frame is expanded from `navifaces_<c>.bin` into one 128x128 texture |
| sub BG0/BG1 planes, BLDALPHA (NaviB lost icon) | overlay visibility, quad alpha |
| Sound_PlayVoice(handle, arc, id) (navigator) | `g_ui_hooks.voice(arc, id)` after the original delay |

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
| StageTitleCard_State_StartStage | same | modified (camera flag writes and BG/DISPCNT setup dropped; boss: NaviMsg_Create, NaviMsg_SetVisible(1), NaviB_Create) |
| StageTitleCard_State_RestoreColors | same | modified (light/edge colour lerps omitted) |
| StageTitleCard_HoldPlayerNone / X / Y | same | modified (gCamera snap left to main.c) |
| StageTitleCard_DrawGlyphText | same | as-is |
| StageTitleCardStrip_*, Part20_*, PartE0_*, Part180_*, Part260_*, Part324_*, Text3E8_*, Text404_*, Part420_* (State functions) | same | as-is |
| the Part Draw functions | same | modified (AnimatorSprite_Draw -> ui_emit; PartE0 rotation dropped: the art is a disc) |
| StageTitleCardBg_State_* | same | as-is (state only) |
| StageTitleCardBg_Draw | `ui_tcbg_draw` (ui_bg.c) | modified |
| StageTitleCardPlayer_State_* | same | as-is, except Zoom: modified (Camera_WorldToScreen -> ui_world_to_screen; perspective angle term fixed at 0x3c000) |
| StageTitleCardPlayer_Draw | `ui_titlecard_player_view` | not translated (values exported) |
| StageTitleCard_LerpColor / ReleaseSprites / Destructor / CreateExtraBoss / DrawPlayer2Hidden | - | not translated |
| PauseMenu_Create | same | modified (System_Freeze -> pause flag; Load2dBAC -> UiSpr; re-entry guard) |
| PauseMenu_Main / PauseMenu_Draw / PauseMenu_Main_Quit | same | modified (logic as-is) |
| PauseMenu_HighlightSelection | UI_VAR_DIM in PauseMenu_Draw | rewritten (same LUT on the pixels) |
| PauseMenu_QuitToMap / QuitToTryAgain | same / `ui_request_exit` | rewritten (save lives, exit kind) |
| PauseMenu_Destructor | `PauseMenu_Destroy` | modified |
| ClearDemo_Create | same | modified (no BG/3D/sound archive setup; TA tally not created) |
| ClearDemo_Main | same | modified (black backdrop under the BG layers) |
| ClearDemo_State_FadeIn / ShowBg / ShowName / ShowModel / WaitParts / Hold / FadeOut / Finish | same | as-is |
| ClearDemo_SkipParts / ClearDemo_SkipToRank | same | as-is |
| ClearDemo_Finish / ClearDemo_GoToNextScene | `ClearDemo_GoToNextScene` | rewritten (scene changes -> exit kind) |
| ClearDemo_InitScoreTally / GetTimeBonus / GetScoreBonus / GetRank | same | as-is (tables from uidata) |
| ClearDemoScore_State_* / SkipRows / SkipToRank / Draw | same | as-is / Draw modified |
| ClearDemoName_* / ClearDemoActText_* / ClearDemoBars_* | same | as-is / Draw modified |
| ClearDemoBg_State_* , ClearDemoBg_Skip | same | modified (window/scroll values kept for `ui_cdbg_draw`, no registers) |
| ClearDemoModel_State_Init/Delay/Move/Done, ClearDemoModel_Skip | same | modified (timing only, no model) |
| ClearDemo_InitTimeAttackTally, ClearDemoTA_* (time-attack tally: clear time, ranking stamp, new-record jingle) | ui_results.c | as-is (sprites act__dm_cldm_tatk) |
| CheckpointTimePopup_*, MarkerTimeList_* | ui_tahud.c | as-is; positions mapped to UI_SP_HUD_TR / UI_SP_HUD_BL |
| DemoPlay label (dm_demoplay_<lang> anims 2 / 0) | ui_tahud.c `ui_demo_label` | as-is (blink driven by demo/demo.c) |
| ClearDemo_DrawModel, ClearDemo_InitSprites palette part, ClearDemo_Destructor (NaviMsg / NaviB of boss results) | - | not translated |
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
| StageTitleCard_InitBg / InitBgBoss | `ui_tcbg_load` + `tools/uibg.py` | rewritten (BG setup -> converted pieces) |
| StageTitleCardBg_Draw | `ui_tcbg_draw` | modified (affine scroll -> quad offsets, window -> clip) |
| ClearDemo_InitBg / InitBgBoss | `ui_cdbg_load` + `tools/uibg.py` | rewritten |
| ClearDemoBg_Draw, ClearDemoBg_State_Init/Open (window part), ClearDemoBg_Skip | `ui_cdbg_draw`, same | modified (window registers -> win_on / win_x2) |
| NaviB_Create | same | modified (no VRAM/DISPCNT; palette rows -> colour maps; difficulty from `g_ui_game`) |
| NaviB_Main, NaviB_DrawIcons, NaviB_Main_Health, _HealthLost, _HealthSprite, _HealthSpriteChange | same (`navib_run`) | as-is / Draw modified (sub OAM -> UI_SP_NAVI_B, BLDALPHA -> quad alpha) |
| NaviB_Destroy | same | modified |
| NaviB_Load / NaviB_Release / NaviB_Destructor, NaviBg_Load | `ui_init` (sprites), `ui_navi_stage_start` (panels, faces) | rewritten |
| NaviMsg_Create (mode 1) | same | modified (BG/VRAM setup dropped; gates and initial panel/face as-is) |
| NaviMsg_Main, NaviMsg_VBlankMain | same | modified (tile copies -> flag pipeline, BG planes -> overlay flags) |
| NaviMsg_ShowMessage (mode 1), NaviMsg_LoadPanel, NaviMsg_State_HideTimer, NaviMsg_SetVisible, NaviMsg_IsVisible, NaviMsg_HidePanel, NaviMsg_SetFaceAnim, NaviMsg_StartFaceSeq, NaviMsg_UpdateFaceAnim | same | as-is (file reads -> image indices) |
| NaviMsgFace_StartIdle / Play / State_Idle / State_Blink / State_Play | same | as-is |
| NaviMsg_ReadFaceFrameMap / Chr, LoadFacePalette, ParsePanelHeader, CopyTileRect | flag pipeline + `ui_navi_face_expand` | rewritten |
| NaviMsg_ShowMessage mode 0, NaviMsg_StartIntro, State_Intro, FadeVisible, State_FadeIn/Out, InitSubDisplay, RestoreSubDisplay, SetRestoreSubDisplayFlag, CloseFiles | - | not translated (zone map navigator / DS display restore) |
| NaviM_* , NaviMPartA/B_* (zone-map lives and emerald panel) | - | not translated (menus area, not in a stage) |
| BossHelp_Create / Main / Destroy | same | modified (OBJ VRAM/palette alloc dropped) |
| BossHelp_Destructor | `BossHelp_Destroy` | rewritten |
| EnemyRoomHud_Create | static, called by `ui_enemy_room(0, n)` | modified (EnemyRoom_GetActive -> the reported counts; StageObj -> UiSpr) |
| EnemyRoomHud_Action_Appear/Count/Vanish/Finish, _State_Appear/Count/Vanish/Finish, _UpdateDigitScale | same | as-is (palette row load -> converted colours; label unk7 offset not applied) |
| EnemyRoomHud_Draw / DrawFinish | same | modified (StageObj_Draw2DEx -> ui_emit in UI_SP_HUD_TC) |
| EnemyRoomHud_Destructor | `EnemyRoomHud_Destroy` | rewritten |
| EffEnemyRoomFix_* | - | not translated (world-space effect, objects area) |
| PauseMenu_Create: StoryProgress_IsStageNew() | `g_ui_hooks.stage_new` | modified |
| ObjShift_Set / ObjShift_SetFromRef | objects module (`obj_fx.c`) | reused |

## 6. Gaps and assumptions (honest list)

- **Results model**: ClearDemoModel_* are translated and `ui_results_model()` gives the pose; main.c draws
  it (`cutscene/patches/main_results_model.patch`, DEFECTS.md V3). The title card palette lerps (StageTitleCard_LerpColor /
  RestoreColors light and edge colours) are not applied to the BG art.
- **Not translated**: time-attack tally (ClearDemoTA_*: time attack uses the
  score tally), CheckpointTimePopup / MarkerTimeList (time attack), VS results
  (ResultsScreen_*), NaviM / NaviMPart* and the mode-0 (zone map) navigator
  (menus area), EffEnemyRoomFix (world-space effect: objects area), special-stage
  pause and exit, extra boss (zone 8 act 2) special cases beyond the brightness
  ones, and the NaviMsg / NaviB of boss **results** (ClearDemo_Create creates them
  with health 0 and no message: nothing to show in the one frame).
- **Second-screen redesign (DC choices, not in the original)**: the navigator
  face is shown only together with a message (the DS keeps it on the touch
  screen); the face is cropped to (12,0)-(116,80) of its 128x128 frame; the
  "SONIC THE HEDGEHOG" header and the blue backdrop of `<c>_b_b` are not drawn;
  the overlays' positions are a layout choice (section 4.3), checked only in
  host renders.
- **Navigator wiring outside this module**: Player_OnHurt's line
  (`NaviMsg_SetFaceAnim(0xc); NaviMsg_ShowMessage(0x11, 0x96)` in boss stages)
  has no hook in the player module (`dcp_on_hurt`). `ui_navi_player_hurt()` is
  the entry: the player owner needs a hook after the `hit_flags & 0x40` sound.
  Boss4..7/F are not translated: only Boss1..3 call `g_boss_hooks.navi`, and no
  boss calls BossHelp yet (Boss6/7/F). No normal-stage object calls NaviMsg in
  the original (the navigator exists in boss stages only).
- **Brightness of composed sub-screen quads**: white fades fade them out (alpha)
  instead of whitening them; black fades are exact (vertex colour).
- **Approximations**: the NaviB lost icon's BLDALPHA (eva v, evb 31 - v over
  sub BG2) is a plain alpha v/31; the lost icon scales about the sprite origin,
  not the OBJ centre. EnemyRoomHud's s8 `unk7` (label OAM offset during
  appear/vanish) is not applied; scale 0 counts as unscaled (as ScorePopup_Draw).
  Results: window 0 is taken as on (empty) from ClearDemo_Create until
  ClearDemoBg_State_Init (the DS register state before it was not traced);
  inside the 2-px window line BG2/BG3 are not hidden.
- **Link names**: `StoryProgress_GetZoneIndex` (also in menus/mn_story.c) and
  `NaviMsg_SetFaceAnim` / `NaviMsg_ShowMessage` (also in boss/boss_core.c, where
  they forward to `g_boss_hooks.navi`) keep their original names in the UI
  sources but link as `ui_*` (macros in `ui_internal.h`), so the game links with
  menus and boss.
- **Host test build**: the test links the objects and player modules, which
  other workers were editing. `make host-test` passed 93/93 with the whole
  objects module; in the last run `objects/obj_deadline.c` (new, not yet
  compiling) was left out and the previously compiled object files were linked.
- **gGameLanguage order**: `data_0207b9bc` reads jpn, eng, fra, deu, ita, spa;
  the old comment in ui.h (deu at 2) was wrong and is fixed.
- The Blaze palette reload of the results glyphs (ClearDemo_InitSprites: +0xcc into row 0) is done as a
  bank swap (`ui_pal_swap`, DEFECTS.md V4).
- Lives on a new game: `gSaveData.unkE/unk66` come from the save file. The
  save-file default was not traced. `save_lives` defaults to 3 (assumption).
- `StoryProgress_IsStageNew()` lives in the menus module (`mn_story.c`).
  PauseMenu_Create calls `g_ui_hooks.stage_new` when set (main.c wires it, the
  menus' gGameState must hold the stage's zone/act/character); otherwise
  `g_ui_game.stage_new` (default 0: CONTINUE / GO BACK). The original shows only
  "PAUSE" on a first visit (story) and always shows the entries in time attack.
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
- `make -C dreamcast/game/ui data` also runs `tools/uibg.py`: 18 title card,
  2 results and 12 navigator resource directories, `navifaces_t.bin` (50
  frames), `navifaces_c.bin` (41 frames), `navidata.bin` (9 sections). The
  converter asserts what the runtime relies on: the stdm_zigzag fill columns are
  all equal (stretched), cldm_bg is one colour, every page is power-of-two.
  Voice ids read: Tails 211..216, Cream 217..222, arcs 2 / 3.
- `make -C dreamcast/game/ui host-test`: 93/93 checks (the 61 earlier ones
  unchanged, plus the boss / enemy room / background ones below). Measured on
  the real tables and sprites:
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
  - boss stage (z1 act 3): zone art 640x480 and the zigzag band inside window
    0x38..0xc0 on the title card; NaviB created at StartStage; the start
    message (face 9, panel 0xf) is on screen 238 frames for time 0xf0 (the
    2-frame load pipeline), its voice plays 1 frame after the call; a hit
    (health 8 -> 7) shows the lost icon for 10 frames (BLDALPHA reaches 0) and
    the counter flash for 2 frames; the hurt line plays its voice; the face
    streams 104x80; BossHelp sits above the panel; Sonic progress 2: panel
    only, no navigator; normal act: no panel;
  - enemy room (5 enemies): counter 05 shown with sfx 0xe7, 3 defeated -> 02,
    cleared -> sfx 0xe8, the finish sprites, gone after 262 frames;
  - title card BG: 242 frames, off after StartStage; results BG: backdrop,
    cldm_far and two cldm_bg planes;
  - pause asks `g_ui_hooks.stage_new`;
  - every HUD/navigator quad inside the safe area, none as large as a DS screen.
- `make -C dreamcast/game/ui dc`: all 11 sources compile with `kos-cc -std=gnu99
  -O2 -fwrapv -Wall -Wextra -Werror`. Their undefined symbols are only KOS/libc,
  `engine/render.c` (draw_rect_screen, tex_free), the objects module (ObjShift_*,
  g_obj_stage, g_obj_hooks) and the player module (g_dcp_world, g_dcp_hooks,
  dcp_act_die). No new KOS link test was run.
- KOS link test (`/tmp/ui/link`): UI + player + objects + `engine/render.c` + stub main -> `link.elf`
  links with no unresolved symbols.
- Visual check (host only): `UI_DUMP=dir test_ui ...` then
  `tools/render_quads.py` gives PNGs of the HUD, title card, pause, results and
  game over. They were inspected: layout, digit order, gauge colours, pause dimming, rank
  stamp, letter spacing.
- Composited 640x480 frames (host renderer over a flat colour standing in for the
  world, copied to `build/dreamcast/ui/preview/`): `boss_message` (balloon
  "Good luck Sonic!", face, boss panel 8/8), `boss_hit` ("That's it, Sonic!",
  gauge 7/8), `boss_help`, `boss_titlecard`, `titlecard_120/240`, `results`,
  `results_open`, `enemyroom` (ENEMY 05), `enemyroom_clear` (finish sprites).
  Inspected: text readable at 2x, no overlay on the bottom-centre play area,
  no second DS panel or dividing line, nothing outside the safe area. The
  renderer is not a PVR: nearest filtering, no TR sorting effects, no Flycast run.


## 8. Time attack and attract-demo additions

- **Time-attack HUD** (`ui_tahud.c`): `ui_checkpoint_popup(time)` shows the
  checkpoint time under the top-right HUD (DS (248,20), 0x12c frames, digits
  flash with frame bit 8, closed by the enemy-room HUD); `ui_marker_time_list`
  / `ui_marker_time_set(lap, time)` the lap list at the bottom left. The
  checkpoint hook `g_obj_hooks.checkpoint_popup` is installed by the UI; the
  marker list needs the objects module to call `ui_marker_time_list(1)` and
  `ui_marker_time_set` where the DS calls MarkerTimeList_Create /
  SetTimeDigits.
- **Time-attack results**: with `W.mode == 2` ClearDemo runs the TA tally
  (clear time, rank via SaveData_GetTimeRank, new-record jingle 0xc / 0xe) and
  ends with `UI_EXIT_TRY_AGAIN`. The native score is hidden in time attack.
- **Attract demos**: with `W.flags & 8` the stage has no title card and the
  timer starts at once (boss acts create NaviMsg / NaviB as the DS);
  `W.flags & 4` starts the stage white for the fade-in. `ui_demo_label(1/0/-1)`
  shows, hides or releases the DEMO PLAY / PRESS START label (driven by
  demo/demo.c).
- Tests: `make -C dreamcast/game/ui host-test` 114 checks (popup lifetime, lap
  rows, TA tally quads before / after the stamp, jingle frame, TRY_AGAIN exit,
  resources freed; demo stage: no title card, flags, white start, label
  release, boss demo NaviB). Dumps looked at: popup, lap list, TA results,
  demo label.

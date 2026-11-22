# Game progression defects (flow audit, 2026-11-22)

One line per defect: id, severity, reproduction, root cause, fix, verification.
Severity: **critical** (softlock / crash / progress lost), **high** (wrong scene, music or save),
**medium** (deviation a player notices), **low** (cosmetic or test-only).
"main.c patch" = `dreamcast/game/menus/patches/main.c.patch` (integrator applies it);
"disc patch" = `dreamcast/game/menus/patches/build_disc.sh.patch`.
Host = `make -C dreamcast/game/menus story-test` (whole story, model of main.c), `make -C dreamcast/game/ui host-test`,
`make -C dreamcast/game/menus host-test`. Flycast = test disc with the switches in section "Test switches".

| id | sev | reproduction | root cause | fix | verified |
|---|---|---|---|---|---|
| F1 | critical | New game: clear act 1, act 2 never ends: the goal freezes the game (Flycast: frozen at the goal of z12, timer stops) | `ui_install_hooks` (ui_state.c) re-took `g_obj_hooks` as its "previous" hooks at every stage start, after main.c had wrapped `stage_exit` with its special-stage hook: from the 2nd stage on the two handlers called each other forever | ui_state.c: the previous object hooks are taken once (first install) | ui host test `test_hook_chain` (fails with the old code: hook ran 9 times); Flycast: z11 -> z12 -> boss -> map -> z21 ... |
| F2 | high | Zone map silent after the first talk, and after every return from a stage | `SysSound_PlayBGM` (mn_story.c) kept the last index in a static that nothing reset; the DS frees `gSysSound` in `SysSound_Release` (stage start, map talk) so the same index plays again | mn_story.c: `SysSound_Release` / `mn_bgm_forget` reset the index; `menus_stage_exit` and `NNS_SndStopSoundAll` forget it; map talk uses `SysSound_Release` | story test: 0 of 1302 map-select frames without the map BGM (old code: 1302 of 1302) |
| F3 | high | After DemoScript 4 (zone 6 boss, new) the zone map is silent | menus started the map (and its BGM) before main.c ran the script, whose music replaced it | new `menus_restart_bgm()`; main.c patch calls it after script 4 | story test (map BGM check after script 4) |
| F4 | high | After a zone-map talk, menu sound effects use the wrong bank | `menu_talk_start` (main.c) started the talk without a return bank: the talk loads `sys_a04` and nothing reloads `sys` | main.c patch: `cutscene_set_return_bank("sys")` for map talks, endings and script 4 | code path (cs_snd_bank restores the return bank); Flycast: see run log |
| F5 | medium | After the staff roll the title screen appears; the unlock notices only show at the next START, and then go to the SEGA logo instead of the title menu; with both flags already set a notice still showed | `CS_NEXT_UNLOCK_NOTICES` was ignored by main.c; `PostStaffRoll_ShowUnlockNotices` had no per-notice conditions and no exit argument | mn_title.c follows PostStaffRoll_ShowPlayerSelect/SoundTestUnlockNotice + PostStaffRollFade_Main (arg 0 SEGA logo, 1 title menu); new `menus_post_staff_roll()`; main.c patch `run_ending()` calls it | story test: Sonic ending -> notices -> flags 0x300 -> SEGA logo; Blaze / extra endings -> fade -> SEGA logo |
| F6 | medium | Time attack: pausing and quitting (or failing) a run saved its time as a record | main.c passed every `UI_EXIT_TRY_AGAIN` as `MENU_EXIT_CLEARED` (SaveData_InsertTimeRecord) | ui_state.c `ui_exit_cleared()`; menus `MENU_EXIT_TRY_AGAIN` (TryAgain, no record); main.c patch uses them | ui host test (TA pause quit: not cleared; TA clear: cleared); story test (TryAgain without a record, record after a clear) |
| F7 | medium | Story game over kept the lives of the save (e.g. 12) instead of the DS rule | `Stage_ExitAfterFail` (SaveData_SetLives(.., lives, 1) = at least 2, emeralds from the save, commit) was not done by the port | mn_main.c: `MENU_EXIT_GAME_OVER` in story mode does it (not for the Extra boss) | story test: lives 12 -> game over -> 2, survives a reload |
| F8 | critical | Extra boss: START freezes the fight for good | `boss_extra` sets `paused` (PauseMenu_Create(0)) and waits for `boss_extra_resume`, which main.c never called | main.c patch: START or A resumes | code path; Flycast extra run (see below) |
| F9 | low | Extra boss pause has no "quit" entry | the pause menu is not drawn outside stages | open (UI pause outside a stage) | - |

## Test switches added (test discs only; nothing happens without the files)

| build_disc.sh variable | file | effect | owner |
|---|---|---|---|
| `SRDC_AUTOCLEAR=N` | `/cd/debug/autoclear.txt` | at stage frame N the act is cleared through the goal hook (`OBJ_EXIT_CLEAR_DEMO`); boss acts run `Stage_OnBossCleared`; the Extra boss counts as beaten after N fight frames | main.c patch |
| `SRDC_AUTODIE="N L"` | `/cd/debug/autodie.txt` | at stage frame N lives = L and the player dies (L 1: game over) | main.c patch |
| `SRDC_SPAWN="x y [t]"` | `/cd/debug/spawn.txt` | the first stage starts the player at (x, y) with tension t | main.c patch |
| `SRDC_AUTOMAP=1` / `s` / `b` / `x` | `/cd/debug/automap.txt` | the zone map walks to the next story node and presses A; with s/b/x the player select picks Sonic / Blaze / the extra story | menus (mn_zonemap.c, mn_psel.c) |
| `SRDC_SPQUOTA=1` | `/cd/debug/spquota.txt` | special stages keep the rings at the goal quota | special (sp_core.c) |
| `SRDC_MENUSAVE="flags s b [em]"` | `/cd/debug/menusave.txt` | 4th value: Sonic's emerald bits | menus (mn_main.c) |
| `SRDC_MENUTRACE=1` | `/cd/debug/menutrace.txt` | also prints every `menus_stage_exit` with the save state | menus (mn_main.c) |
- FIXED (player) pit death never fired in the game: g_dcp_world.death_y / map_right kept their
  initialisers; dcp_bind_map sets them from the map, Player_UpdateGimmickStageBounds translated.
- FIXED (player/objects) Player_OnHurt kept its own ring-scatter level, never reset; now the ring
  manager's.
- OPEN (integrator: engine/input + main.c) no DS R button: tricks, homing attack / air dash / hover,
  grind tricks unreachable with the pad. Patch dreamcast/game/player/patches/input_r_trigger.patch
  (right trigger -> DS R, left -> DS L). Menus / UI / demo key mapping (ui_state.c, demo.c) may want
  IN_R too.
- OPEN (demo owner) z12 demo 1 / z21 demo 3 desync: DEMO_LEAD_FRAMES 0, the DS consumes 23 frames of
  keys during StageStart; with 23 demo 1 reaches the goal ring on the host. Patch
  dreamcast/game/player/patches/demo_lead_23.patch.
- FIXED (objects) trick effects missing (EffTrickForwardBoost, EffTrick_CreateAtOffset,
  EffTrickSparkle, EffTrickCombo); new sprite slot OBJ_BAC_EFF_TRICK: objdata.bin regenerated, the
  stage packs / preload list need rebuilding (make -C dreamcast/game/objects data preload).
- NOTE trick sound volume (NNS_SndPlayerSetVolume 0x7f >> combo level) and the tension-max model
  palette shift (ModelPalette_ShiftColor) are not translated.

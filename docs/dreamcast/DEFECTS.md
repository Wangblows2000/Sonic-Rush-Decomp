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
| F10 | critical | Special ring: Sonic circles, boosts 0xc0 frames, flies up (Player_State_GmkSpecialEnter) and then hangs invisible forever; no special stage (HEAD b4873e22 and 4845e568, z11 ring at 12608,2368; serial trace: `gimmick 0x0 ... timer 50`, `despawned` +1) | the ring object is despawned while Sonic flies up, `obj_free_player_refs` clears `p->gimmick`, and GmkSpecialEnter returns early on a NULL gimmick. main.c gives the objects the DC window (`camera_get_view`) as view 0 and the DS's other screen as view 1; the DS spawns for its two screens gCamera[0]/[1]. In this state gCamera[0] is locked on the ring (pf2 0x30, @0x3EE), the DC window is 160 px higher, so the ring leaves both views | OPEN, owners camera / objects / main.c. Verified fix candidate (private build): view 0 = `camera_layer(0)` (gCamera[0]) -> the special stage starts, clears, the act reloads 280 px below the ring. Without dropping the DC window: OBJ_MAX_VIEWS 3 (gCamera[0], gCamera[1], DC window) | Flycast: bug reproduced 3 times; with view 0 = gCamera[0]: special stage 0 entered, result 1 emerald 0x01, act reload, emerald saved at the act clear |
| F11 | low (test) | A save state reloaded with a disc whose `/cd/debug` files differ still sees the old files | KOS keeps the ISO directory in RAM: the state carries it | change debug switches only with a fresh boot; a state only goes with the disc it was saved from | Flycast: `spquota.txt` still found after a reload on a disc without it |
| F12 | low (test) | A Flycast run stalls at a talk page; xdotool keys and screenshots reach another window | another agent's Flycast (HOME build/dreamcast/fp/home4, c72 disc) was started on display :96, which this flow test had created and used | flow tests moved to display :86; give each agent its own display number | Flycast: run resumed from the z53 state on :86 and finished |

## Flycast verification (HEAD 2e79caa5 / b4873e22 + the patches, own HOME and display :96)

Test discs with the switches below; input from xdotool (`/tmp/flow/driver.py`, rules per run), serial trace.

| run | what | result |
|---|---|---|
| Sonic, new save (07a1f85c, 2e79caa5, 4845e568; on 4845e568 from boot to z53, then resumed from the state saved at z53) | title, GAME START, z11..z73, script 0 before / script 1 after each boss, the nine map talks 0x15..0x1d, DemoScript 4, map BGM, zf3, ending, staff roll 0, PostStaffRoll talk 0x37 (emeralds missing), unlock notices, SEGA logo | pass (27 min with autoclear 200) |
| Blaze (preset Sonic 26) | player select (autopilot b), z41 first, order 3 0 2 1 5 4 6 7, talks 0x28..0x2f, DemoScript 4, zf3, ending, staff roll 1, notices already set -> fade -> SEGA logo; progress 28 saved | pass |
| VMU round trip + Extra | reboot on the Blaze run's VMU file: `boot save progress 26/28 flags 0x300`; + emerald preset 0x7f: player select shows the extra story, script 3, Extra boss (autoclear), script 2, ExtraAct, extra ending, staff roll 2, SEGA logo | pass |
| special stage | z11 special ring, boost while circling (key held), clear (quota switch) and fail (no input): result 1 emerald 0x01 / result 2 none, act reload 280 px below the ring, emerald saved at the act clear | pass only with the F10 fix candidate |
| time attack | title menu -> player-select page -> TIME ATTACK -> Sonic -> z11: clear -> record 1500 -> TryAgain -> retry -> pause GO BACK -> exit 5, record unchanged -> TryAgain -> quit -> title | pass |
| game over / continue | preset Sonic 5: map autopilot -> z31, `AUTODIE "300 0"` (pit death with 0 lives): GAME OVER -> exit 2, lives saved 2 -> SEGA logo -> title -> GAME START -> map -> z31 again | pass |
| pause quit to the map | preset Sonic 26: map -> A -> act select -> z11 (not new: CONTINUE / GO BACK shown; a new stage shows none, as the DS) -> GO BACK -> exit 1 -> act select over the map | pass |
| special ring on 4845e568 | as above | F10 still reproduced (gimmick cleared, despawned +1) |
| save states | F5 / F8 / F6 / F7 in `$HOME/.config/flycast/mappings/SDL_Keyboard.cfg`, `-config config:Dreamcast.AutoLoadState=yes -config config:Dreamcast.SavestateSlot=N` to start from a state; the driver saves a state at each zone-map arrival and boss-act start, and before the special ring. No rewind in this build. A state needs the same srdc.elf, and debug switches are fixed at the boot that saved it (F11) | used for the special-stage reruns |

Not verified in Flycast: the mid-fight boss talks (Boss7 0xa/0xb, BossF 0xf/0x10) and their save flags: autoclear skips the fights.

## Test switches added (test discs only; nothing happens without the files)

| build_disc.sh variable | file | effect | owner |
|---|---|---|---|
| `SRDC_AUTOCLEAR=N` | `/cd/debug/autoclear.txt` | at stage frame N the act is cleared through the goal hook (`OBJ_EXIT_CLEAR_DEMO`); boss acts run `Stage_OnBossCleared`; the Extra boss counts as beaten after N fight frames | main.c patch |
| `SRDC_AUTODIE="N L"` | `/cd/debug/autodie.txt` | at stage frame N (once per stage start) lives = L and the player is put below the map: pit death (L 0: game over; L 1: respawn with 0) | main.c patch |
| `SRDC_SPAWN="x y [t]"` | `/cd/debug/spawn.txt` | the first stage starts the player at (x, y) with tension t, and puts him there again when the title card ends (camera snapped); a player trace every 30 frames follows | main.c patch |
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
- FIXED (player/sound) the NOTE above: the trick sound volume (NNS_SndPlayerSetVolume 0x7f >> combo
  level on @384 / @390, `DcpHooks.sfx_volume`, sound_hooks.c) and the model palette shift of Player_In
  (tension-max glow, invincibility flash; `dcm_set_color_shift`, offset colour) are translated.
- FIXED (ui/demo) the R / L pad bits (0x100 / 0x200) are carried by ui_pad_bits and the demo key
  mapping both ways (the original UI never reads them).
- FIXED (player) Blaze's spin jump animation ran at 1x: Player_Action_Jump sets the 3D animation
  speed 0x4000 (x4) for Blaze (`DcpHooks.model_speed`).
- FIXED (player/objects) under water, Player_Action_Jump and the brake smoke skipped their
  Player_Rand draw (shared seed drift) and the EffAirBubble_Spawn; DCP_FX_AIR_BUBBLE now spawns the
  bubbles (also Player_Action_Launch and the gimmick launch).
- FIXED (player) 0x280 / 400 / 0x7f >> combo level: the ARM shift gives 0 from level 32 on, C
  wrapped the count (TRICK_SHR).
- NOTE (player, VS / pf2 0x400000 only) Player_Respawn's no-restart branch sets pf2 0x200000 and
  Player_In then calls StageEvent_SpawnInRect around the player (taller toward the split line);
  the port relies on the view spawner there (objects_spawn_rect has no caller).
- CHECKED (player/objects) Dead Line gravity flip with the hook wired (1364f08f): Player_UpdateGravityFlip
  (velocity, angle, remap), the z72 top pit and the Flycast play of z71 / z72 for both characters
  match the original; test_zones has a gravity-flip check on z71 / z72.

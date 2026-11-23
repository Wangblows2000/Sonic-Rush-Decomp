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
| F8 | critical | Extra boss: START freezes the fight for good | `boss_extra` sets `paused` (PauseMenu_Create(0)) and waits for `boss_extra_resume`, which main.c never called | main.c patch: START or A resumed (now the pause menu, F9) | code path; Flycast extra run (see below) |
| F9 | low | Extra boss: START froze the fight with nothing on screen (F8 made START / A resume), no way to leave the fight | PauseMenu_Create(0) of ExtraBoss_State_Battle was not drawn: the UI pause only existed inside stages | FIXED ui_pause.c `ui_extra_pause_open()` / `ui_extra_pause_frame()` (the stage pause over the Extra boss: zone 8 act 2, CONTINUE, GO BACK once the extra story was cleared, as the DS), menus `MENU_EXIT_EXTRA_QUIT` (PauseMenu_Main_Quit: player select), main.c patch: the Extra loop opens / runs / draws it and returns through the menus | ui host test `test_extra_pause`, story test (EXTRA_QUIT -> PlayerSelect); Flycast (HEAD 16999623 + patches, flags 0x1300): START -> PAUSE CONTINUE / GO BACK over the frozen fight, START -> fight resumes (timer 0:07 -> 0:09), START, DOWN, A -> `extra boss: pause GO BACK`, exit 6 -> PlayerSelect, save unchanged |
| F10 | critical | Special ring: Sonic circles, boosts 0xc0 frames, flies up (Player_State_GmkSpecialEnter) and then hangs invisible forever; no special stage (HEAD b4873e22 and 4845e568, z11 ring at 12608,2368; serial trace: `gimmick 0x0 ... timer 50`, `despawned` +1) | the ring object is despawned while Sonic flies up, `obj_free_player_refs` clears `p->gimmick`, and GmkSpecialEnter returns early on a NULL gimmick. main.c gives the objects the DC window (`camera_get_view`) as view 0 and the DS's other screen as view 1; the DS spawns for its two screens gCamera[0]/[1]. In this state gCamera[0] is locked on the ring (pf2 0x30, @0x3EE), the DC window is 160 px higher, so the ring leaves both views | FIXED in HEAD 8f064918 (objects spawn for both DS cameras and the DC window) | Flycast on HEAD 8f064918: z11 ring, boost while circling: clear -> result 1, emeralds 0x01, act reloaded at 12608,2406, exit emeralds 0x1; fail (no input) -> result 2, none, back to the act |
| F11 | low (test) | A save state reloaded with a disc whose `/cd/debug` files differ still sees the old files | KOS keeps the ISO directory in RAM: the state carries it | change debug switches only with a fresh boot; a state only goes with the disc it was saved from | Flycast: `spquota.txt` still found after a reload on a disc without it |
| F12 | low (test) | A Flycast run stalls at a talk page; xdotool keys and screenshots reach another window | another agent's Flycast (HOME build/dreamcast/fp/home4, c72 disc) was started on display :96, which this flow test had created and used | flow tests moved to display :86; give each agent its own display number | Flycast: run resumed from the z53 state on :86 and finished |
| F13 | medium | Blaze story: the zone-map and boss talks showed no Blaze portrait (empty left box), e.g. talks 0x28.., 0xb | `td_port` (the portrait table, csdata.py) was cut to 8 x 12 entries; Blaze's portrait rows reach index 12 and up to 23 frames | FIXED cutscene: csdata.py exports s32[13][24] (rows sized to the next row), cs_talk.c bounds 13 / 24 and logs a missing portrait | cutscene host test 2684/2684 (talks 8..12 fail with the old limits); Flycast: Blaze portraits in talk 0xb and the map talks |
| F14 | low (test) | Boss7 on the flow test build with `SRDC_BOSSHP`: the clash started, then the game died (`Thread stack underrun`, kernel thread) | the test patch included boss/boss_internal.h in main.c: its `#define BossHelp_Create boss_BossHelp_Create` turned main.c's help hook into a call of the boss module's BossHelp_Create, which calls the hook again: endless recursion | not in HEAD: the patch declares `boss_gBossStatus` itself instead; never include boss_internal.h outside boss/ | Flycast: Blaze clash shown with the help sprite, won, defeat, after-boss DemoScript (pdemo 20, script 0x36), ClearDemo |

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
| Blaze on HEAD 16999623 + the patches (display :86) | preset Sonic 26, flags 0x300; autopilot b, autoclear 200: z41 first, zones 3 0 2 1 5 4 6 7, map talks 0x28..0x2f (ids 40..47, once each), script 0 x8, script 1 x7, script 4, zf3, ending 6/0, staff roll 1 (Final_Roll), PostStaffRoll: both flags set -> no notice, fade, SEGA logo (195 frames), title, player select | pass (39 min); reboot without presets: `boot save progress 26/28 flags 0x300` |
| Extra on HEAD 16999623 + the patches | preset "768 26 28 127", autopilot x, autoclear 600: player select -> extra story, script 3 (5/3), Extra boss cleared, script 2, ExtraAct, extra ending 7/0, staff roll 2 (extra pictures 0..3), PostStaffRoll, SEGA logo, title, player select | pass (20 min); reboot: `flags 0x1300` (SaveData_SetGameCleared saved) |
| save states | F5 / F8 / F6 / F7 in `$HOME/.config/flycast/mappings/SDL_Keyboard.cfg`, `-config config:Dreamcast.AutoLoadState=yes -config config:Dreamcast.SavestateSlot=N` to start from a state; the driver saves a state at each zone-map arrival and boss-act start, and before the special ring. No rewind in this build. A state needs the same srdc.elf, and debug switches are fixed at the boot that saved it (F11) | used for the special-stage reruns |

Mid-fight boss talks (Flycast, real fights: `SRDC_BOSSHP=2` keeps the boss's hit points at 2, test discs from HEAD 8f064918 /
16999623 + the patches, `SRDC_START` at the boss act, Blaze via `chara.txt`):

| run | what | result |
|---|---|---|
| BossF Sonic / Blaze | sky-charge phase reached, TalkDemo 0xf / 0x10 | pass: talk over the frozen fight (stage frame and player position the same at start and end), save flags 0x80000 / 0x100000 |
| Boss7 Sonic / Blaze | rival beaten at hit point 1, TalkDemo 0xa / 0xb | pass: frozen fight (stage frame 241 -> 240 at the end), save flags 0x20000 / 0x40000, Blaze portraits after F13 |
| Boss7 clash (Blaze) | after the talk: flash, approach, clash with the help sprite, A / B mashed (alternating, 14 per s) | pass: clash won, defeat, after-boss pdemo 20 / script 0x36, ClearDemo; with slow mashing (5 per s) the clash is lost: knock-back, rival skids, `Boss7_State_None` with health 1 until time over (as the DS, BOSSES.md) |
| Extra boss pause (F9) | flags 0x1300 (extra cleared), player select -> extra story, script 3 (about 9 minutes), fight: START / CONTINUE / START, DOWN, A | pass (see F9) |

## Regression on 9b422c76 / a4e9e57c (2026-11-23, clean worktrees, `SRDC_PREBUILT`, display :86)

Frame pacing from `/cd/debug/frameprof.txt` (per-scene summaries), shots every stage at +1.5 / +6 / +9.5 s.

| run | result |
|---|---|
| Sonic story, new save, autopilot s, autoclear 600 (9b422c76; resumed from the VMU after two autopilot game overs, zf3 with autoclear 200) | pass: z11..zf3, scripts 0 / 1 / 4, map talks, ending 6/0, staff roll 0, unlock notices, SEGA logo. Every act 59.86..60.0 fps with no frame over 34 ms except one 50 ms frame (3 vblanks, 48 ms outside objects / player / draw) in act 1 of each zone (z11 f1053, z21 f8552, z31 f16464, z41 f22660, z51 f29412, z61 f1500, z71 f10008, zf3 f1542), none in acts 2 / bosses: fits the zone music file's first open (act 2 reuses the cached file; owner audio). No mid-act model load (`objects: model ... loaded` only before the first frame). No crash |
| palettes | title cards, zone palette cycles (z31 / z41 / z42 lights, z51, z71), boss GO flashes (z13 / z33 / z63 white-out, zf3) as before; z21 / z22 water tint banks reserved (`water tint banks ok (map 2, bg 3)`) |
| Water Palace (a4e9e57c, debug start z21 / z22, Sonic and Blaze, right held 75 s) | pass: water line band above, full tint below (tiles, sprites, player), bubbles, drown digits 5..0, drowning; `StageJingle 0x17` (z2b_23) at the drown, `Stage_PlayBGM z2s / z2b` again at the respawn (Sonic f1555 -> f2312, Blaze z21 f2380 -> f3137, z22 f2272 -> f3029) |
| special stage (a4e9e57c, z11 ring, boost while circling) | pass: special stage 0, 7601 frames at 59.93 fps, result 1, emeralds 01, act reloaded at 12608,2406, `exit ... emeralds 0x1` |
| Blaze lives (a4e9e57c, autopilot b, autoclear 600, A mashed) | reproduced: Blaze 2 -> 1 at z13, game over at z33. Not a game defect: the shots show Blaze hit with 0 rings by the boss, the title card replayed (StageRestart) and the HUD count lowered; `dcp_respawn` lowers the lives at the respawn, the boss clear saves them, a death with 0 lives is the game over (DS rule). Since the boss starts at GO it attacks within the 600-frame autoclear window; boss acts need autoclear <= 200 or a player that fights (F15) |
| seen, other owners | z41: an opaque yellow light cone over the player (`/tmp/flow/shots/reg1_040_late.png`, 9b422c76), z42's searchlights are translucent; z23 / z43 / z73 title card "GO!" over black (arena drawn after boss_start, noted before) |

- F15 | low (test) | autopilot runs lose lives in boss acts and end in a game over (Sonic: z13, z23, z73, zf3; Blaze: z13, z33) | autoclear 600 leaves a standing / A-mashing player with 0 rings in the fight; the boss starts at GO (a4e9e57c) | use `SRDC_AUTOCLEAR=200` for full-story runs, or stop the A mash in acts (driver `MASH` start / stop regexes) | Flycast: Sonic zf3 with autoclear 200 cleared, ending reached |
- F16 | none (matches DS) | z41/z42 Night Carnival lamp (DecGroupAF 0xd8, `ac_dec_light4`) shows an opaque yellow cone in single screenshots | the DS shows the cone only while the player touches the lamp and alternates it with the front glow every other frame (`gGameState.unk20 & 1`, 30 Hz); OBJ mode 0, no BLDALPHA in zone 4; the port matches (obj_carnival.c `W.frame & 1`) | kept as on the DS; a DC-only 50 % TR-list blend of both children while touched is possible but not applied | Flycast z41 spawn 6940,750: 16-shot burst alternates cone / glow |

## Test switches added (test discs only; nothing happens without the files)

| build_disc.sh variable | file | effect | owner |
|---|---|---|---|
| `SRDC_AUTOCLEAR=N` | `/cd/debug/autoclear.txt` | at stage frame N the act is cleared through the goal hook (`OBJ_EXIT_CLEAR_DEMO`); boss acts run `Stage_OnBossCleared`; the Extra boss counts as beaten after N fight frames | main.c patch |
| `SRDC_AUTODIE="N L"` | `/cd/debug/autodie.txt` | at stage frame N (once per stage start) lives = L and the player is put below the map: pit death (L 0: game over; L 1: respawn with 0) | main.c patch |
| `SRDC_SPAWN="x y [t]"` | `/cd/debug/spawn.txt` | the first stage starts the player at (x, y) with tension t, and puts him there again when the title card ends (camera snapped); a player trace every 30 frames follows | main.c patch |
| `SRDC_AUTOMAP=1` / `s` / `b` / `x` | `/cd/debug/automap.txt` | the zone map walks to the next story node and presses A; with s/b/x the player select picks Sonic / Blaze / the extra story | menus (mn_zonemap.c, mn_psel.c) |
| `SRDC_SPQUOTA=1` | `/cd/debug/spquota.txt` | special stages keep the rings at the goal quota | special (sp_core.c) |
| `SRDC_BOSSHP=N` | `/cd/debug/bosshp.txt` | the boss's hit points are held at N or below once the fight starts (the late phases and mid-fight talks of a real fight); every boss event is printed (`boss: <event> a b`), Boss7 prints its clash state every 60 frames | main.c patch |
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

## Visual-fidelity review in the game flow (2026-11-22, V lines)

Flycast test discs of HEAD 2e79caa5..8f064918 + the menus/main.c flow patch, own HOME and display :87, driven
by the flow driver (serial-log rules, screenshots every 1.5-4 s): Sonic from a new save (title, title menu,
z11/z12 title cards and results, script 0 zone 0, z13, script 1, boss results, zone map); Sonic at progress 24
(map talk 0x1d, script 0 zone 7, zf3 title card / navigator / results, ending, staff roll 0, PostStaffRoll talk
0x37, unlock notices, SEGA logo, title); Sonic cleared + Blaze 5 (unlock notices at START, player select, Blaze
zone map with Cream's panel, Blaze title card, pause, Blaze results). Each shot compared with the original's
code (placement, timers, palettes) for one 640x480 frame.
- V1 | high | boss intro (DemoScript 0, every zone): PlayerDemo 30 starts talk 1 at t 30 and fades to black at
  t 35; the port kept running PlayerDemo while the talk ran, so the scene faded out, its BGM stopped and the
  whole dialogue played on a black frame (master brightness over the talk), the player pressing A blind | TalkDemo_Create
  calls System_Freeze(1,1,1): Task_RunList then runs only priority-group 7 tasks (TalkDemo, DemoScript,
  PolyDemoUnder2, PlayerDemoRepeatSE, a BrightnessFade made while frozen) and DisableSwapBuffersTask keeps
  the last 3D frame; System_Unfreeze in TalkDemo_Destructor; the latch (gSystemFlags 0x80000000) follows flag
  8 at the end of the frame | FIXED cutscene/: `g_cs_freeze` / `g_cs_frozen` (cs_internal.h, cs_core.c), set
  in talk_create, cleared in TalkDemo_Destructor / _Kill / a missing script; cutscene_run_frame keeps the 3D
  list and skips PlayerDemo's group-0 parts (cs_player.c), TransformDemo and Ending/StaffRoll while frozen;
  BrightnessFade takes group 7 when made while frozen | host test 2684/2684 (the PlayerDemo timeline check
  now moves the commands after a StartTalkDemo by the frames the talk froze it); Flycast: talk 1's six pages
  over the frozen, dimmed boss scene, then the fade (FadeToBlack after "talk 1 end"). Also holds the
  ExtraEnding (PlayerDemo 0x2a with talks 0x38 / 0x39: the staff roll waits for talk 0x39, as on the DS).
- V2 | medium | act select: the NaviM panel (lives, story %, emeralds) covered zone tabs 6 / 7, and the
  navigator face (bottom right) covered the back button and the ranks of ACT-2 / VS BOSS | the zone-map
  layout (mn_navi_compose) was reused for the act select, whose frame has the tabs across the top and the
  back button at the bottom right | FIXED menus/mn_navi.c `mn_navi_compose_ex(1)` (mn_actsel.c): NaviM at
  x1.25 in the free strip under the rows (20..160 x 406..476), the face at x1.0 right of the rows (512..640 x
  196..324); zone map unchanged | Flycast shot of the old layout (tabs 6 / 7 hidden); positions measured on it;
  menus host tests 38/38, 59/59, story 125/125.
- V3 | medium | act and boss results: the right half of the frame was empty (the DS shows the character's
  model sliding in and posing there) | ClearDemo_DrawModel / ClearDemoModel_* were timing only | FIXED ui/:
  ClearDemoModel_State_Init / _Delay / _Move / _Skip translated (data_0207346c: scale 0x107fc, from
  (-160,-192) to (220,-159,-25) normal / (200,-159,-25) boss for Sonic, rotation lerp, speed 0 during the
  delay), `ui_results_model()` (ui.h) gives pose / animation (0x26 / 0x27, Blaze 0x48 / 0x49; visibility 2,
  Blaze 1 / 5; no loop) and the ClearDemo camera / lights; ui_draw_tr moves the tally above the model's depth
  band; main.c draws it: `dreamcast/game/cutscene/patches/main_results_model.patch` (draw_results_model,
  replaces the stage player's draw while the results show it) | UI host test 153/153 (start / end positions,
  frame, Blaze boss anims); Flycast: Blaze's model slides in and poses right of the tally, tally in front.
- V4 | medium | Blaze's act / boss results drew the header and footer bars, their corner pieces and the
  header / clear text in Sonic's blue palette | ClearDemo_InitSprites loads the palette at +0xcc (anim 0x32's)
  into OBJ row data_02073404[0] for Blaze; not done | FIXED ui/: `ui_palmap_set_pal` + `ui_pal_swap`: the
  row-0 images of dm_cldm (anims 0..0x12, 0x22, 0x27) drawn as the bank of anim 0x32's palette for Blaze |
  UI host test (26 Blaze sprites recoloured, the others not); Flycast: purple bars and corners.
- V5 | medium | unlock notices (after the staff roll, or START on the title once a story is complete): a
  dark empty screen for up to 4 s each, the player was not told what was unlocked | the MsgBox system is not
  translated | FIXED menus/mn_title.c: the notice draws MsgBox 0x14 / 0x15's message (caution_d_<lang>.bac
  anims 0 / 1, "TIME ATTACK / SOUND TEST IS NOW AVAILABLE.", at the sprite's own place) on black, brightness 0;
  the box frame BG is still not drawn. Disc: `dreamcast/game/cutscene/patches/build_disc_caution.patch` adds
  the six caution_d sprite sets; the resource is freed with the task (freeing it in the frame that still drew
  it let mn_draw build its texture in freed memory) | Flycast: both notices readable at START on the title.
- V6 | checked, no change | z11 / z12 / zf3 title cards (Sonic, Blaze): the black disc at the right is the
  zone emblem of dm_stdm anims 0xf.. whose ROM palettes are black (row 1: entries 1..13 0x0000); the zone-map
  talks (band, box, portrait over the map, map not dimmed); script 1 captions and PlayerDemo 3D; Sonic ending
  pictures and captions (top band fades with the picture, caption strip stays: BLDY on BG1 only); staff roll 0
  (credits, Sonic / Amy run, CONGRATULATIONS / THANK YOU card); PostStaffRoll talk 0x37 over black; the
  final-boss navigator message; TransformDemo 0 / 1 (warp: light streaks, a5i3 dome with its 8 grey levels).
- V7 | low (test builds) | a private test tree crashed at the Blaze z13 spawn (Unhandled exception, PC in the
  heap, from dcp_player_init_state's g_dcp_hooks.atkdef) on two discs; a clean rebuild of the same sources
  ran z11..z33 without it | the game Makefile (KOS Makefile.rules) has no header dependencies: objects copied
  from a tree where boss.h had gained a BossHooks field (fx3d) before them kept the old layout | not a source
  defect: after a header change in any module run `make clean` (or delete its .o) before build_disc.sh.
- V8 | checked (Flycast, Blaze) | Blaze: script 1 talks over the frozen 3D scene (V1), title card (zone 2 =
  Leaf Storm for Blaze, BLAZE THE CAT strip), pause ("PAUSE" only on a new stage, as the DS), Cream's NaviM
  panel and map talk, the warp TransformDemo 2.
- Second pass (HEAD fe748fda + the patches below, own HOME / display :87): Extra story run (player select,
  script 3 with TransformDemo 4, Extra boss, script 2, ExtraAct, extra ending, staff roll 2, unlock check,
  SEGA logo), a stage sweep z13..zf3 for every boss title card (Sonic), a front-end session by hand
  (title menu, options pages, sound test, time attack menu, a time-attack clear, Try Again, player select,
  zone map, act select), a Blaze story run with the map autopilot (MsgBox 0x12, the map model, the final
  zones, Blaze ending, staff roll 1).
- V9 | high | Extra boss: the arena was plain black, no space backdrop | ex_dc_bg (boss_extra_draw.c) drew
  the zex BG1 strips in the OP list at depth 0.00002; KOS puts the PVR background plane at depth 0.0001
  (pvr_state.zclip -> PVR_BGPLANE_Z) and OP polygons pass only above it, so the whole backdrop failed the
  depth test (the zex data and its palettes were fine: the host dump shows the nebula) | FIXED
  boss/boss_extra_draw.c: depth 0.00012 (behind every model, above the plane); the same latent fault in
  cutscene/cs_draw.c (the full-screen cutscene backdrop at 0.00001, black so far: 0.00012) | Flycast (Extra
  rerun from boot, debug/extra.txt): the fight over the pink / violet zex nebula with its wave, the VS
  cut-in and the meteors in front; before the fix 10 of 10 fight shots on black.
- V10 | medium | final-boss title card (and every boss act): a black frame at "GO!" | the boss (BossF: the
  arena camera, CAM.unk0 |= 1, only then is the arena drawn) started when the title card task ended, ~62
  frames after StageTitleCard_State_StartStage, where the DS sets gBossStatus.flags |= 1 | FIXED ui/
  `ui_titlecard_started()` (ui.h, ui_titlecard.c: StartStage reached, flags 0x20000000) and main.c patch
  `dreamcast/game/cutscene/patches/main_boss_start_at_go.patch` (boss_start at StartStage) | Flycast sweep
  z13, z23, z33, z43, z53, z63, z73, zf3: title card, the white of StageTitleCard_State_Exit, then the arena
  fades in under "Go!" (RestoreColors) with the boss; no black frame in any of them.
- V11 | medium | message boxes: the unlock notices had no frame (V5), the zone map's MsgBox 0x11 closed at
  once and 0x12 ("are you sure you want to play this zone?") was answered YES by the A press on the node:
  NO could not be chosen | MsgBox (UI/MessageBox) was not translated | FIXED menus/mn_msgbox.c: MsgBox_Create
  / IsClosed / GetResult / RequestClose / SetFlags / Destroy as the DS (files by message, frame kinds
  data_02074c3c, split line scroll data_02074bf0 / bdc over 8 frames, icon, OK / YES-NO / single buttons,
  cursor blink, Confirm 0x20 frames, results 0 / 1 / 2 / 4; no touch); the frame BG as two pieces in the
  sprite order of its BG's priority; a closed box's files are freed at the next frame start
  (`mn_msgbox_gc`, mn_quads_reset: the closing frame's quads still use them). The notices (mn_title.c) now
  use MsgBox 0x14 / 0x15; zone map 0x11 / 0x12 as the DS (NO stays on the map); the map autopilot
  (debug/automap.txt) answers LEFT, A. Disc: `dreamcast/game/cutscene/patches/build_disc_msgbox.patch`
  (caution, caution_<lang>, caution_b_<lang>) | menus host 39/39 (MsgBox 0x12 open, answered YES), flow
  75/75, story 125/125; Flycast (Blaze, autopilot): the box opens over the map, "ARE YOU SURE YOU WANT TO
  PLAY THIS ZONE?", YES / NO with the cursor, YES -> the stage.
- V12 | medium | zone map: no character, the map icon sprite stood in | ZoneMap_DrawCharModel not translated
  | FIXED menus/mn_model.c (the menus' 3D layer: DS BG0 at its priority in the compositing order, a PVR
  depth band in the menus' TR order, the model in the OP / PT / TR lists) + mn_zonemap.c
  (ZoneMap_LoadPlayerArchive / SetCharModelRotY / SetCharModelAnim / AnimateCharModel / DrawCharModel /
  SetupLighting: ortho camera, scale 4, RotY, joint / visibility anims [0,2,1] / [55,57,5], menu.dca's
  "decided" pose, loop flags, Blaze walk speed 2, global material colours 0x7fff / 0x5ef7, light mask 1,
  polygon alpha with the talk fade; the turn between path segments of ZoneMap_State_MoveToNode; the icon
  only for Blaze's first steps as ZoneMap_UpdateView). main.c patch
  `dreamcast/game/cutscene/patches/main_menus_3d_layer.patch` (menus_draw_list in the front end's lists);
  cutscene/tools/csdata.py adds menu.dca / .dcv to models.txt. Not done: the DS edge marking (outline) |
  flow host test (Sonic / Blaze: one instance, marker over BG3, vertices inside the depth band, 55x98 px);
  Flycast: Sonic stands at his node, runs along the path turned with it, the node sprites in front;
  Blaze turns and plays the decided pose before her stage.
- V13 | low | act select: the navigator's balloon (shown a few seconds) hid zone tabs 1..4 | the zone-map
  place (top left, x1.5) was reused | FIXED menus/mn_navi.c: in the act select the balloon goes to the
  bottom strip between the NaviM panel and the back button (x1.0, 168..344 x 394..480); no free area of the
  act-select frame takes it larger | flow host test (region checked, dump); Flycast shot of the old place.
- V14 | checked, small fix | ExtraAct letterbox: the bottom bar is magenta by the data: ex_cldm_bg_pal row 0
  is a blue ramp (top, tile colour 2 = 0x7de2), row 1 the matching purple ramp (bottom, 0x12 = 0x6078) - the
  SONIC / BLAZE halves | colours kept; FIXED menus/mn_extraact.c: a band crossing y 96 is split there
  (map rows 0..11 tile 0, 12..23 tile 1), it took one colour | Flycast: blue SONIC & BLAZE bar, magenta
  EXTRA ZONE CLEAR bar, tally, rank.
- V15 | medium | options: the question of every choice page (DIFFICULTY?, TIME LIMIT?, the erase
  questions: white glyphs, anims 0x15..0x1a) was drawn over the light stripes of mnu_opti_bg02_b (image rows
  53..65, the dark header is 24..46) and could not be read; on the sound-test page the title sat on the top
  edge of its black bar, the track name between the meter boxes, the speakers, arrows, track number and
  PLAY / STOP were missing; the list drew a panel the DS does not show there | the options pages were
  rewritten with guessed positions | FIXED menus/mn_options.c from OptionsMenu_DrawItems / DrawItemLabel1-2
  / DrawPage0-4 / DrawPageBackButton / 022ca248 / 022ca1cc / 022ca150 / 022c9f54 / 022c9d58 / 022c9b78 /
  DrawSoundTestTrackNumber and mnu_opti.ama (the page origin: the list (240, 0) -> seq 22 -> (0, 0); tbl =
  Options_Unk_ov001_022ce960): list items at x 240 (selected) / 248, y 0x18 + 0x18 i, values at x - 0x44,
  OPTIONS banner (0, 144), back button (mnu_titl 0) at (16, 80), the panels (sub BG0 / BG1, 512 x 256)
  scrolled by -origin (only their left 16 columns at x 240 on the list); pages: question (0x18, 0x18),
  choices (0x20 / 0x88, 0x50) lit by the current setting, the cursor frame of mnu_opti.bac on the hovered
  one (blink 7 frames after a move, flash after the choice), PLAYER DATA's (0x38, 0x40 / 0x58) and ACT /
  BOSS (0x38 / 0x80, 0x70), "A: OK / B: BACK" (0x34) in the footer at (0x18, 0x98) (the DS's touch back
  button is not drawn); sound test: title, speakers with the four-quarter glow (anims 7 / 8, 6), arrows,
  track name (0x80, 0x49), number (0x68, 0x4c), deco / PLAY / STOP, mnu_opti_sou_<lang> label; oamPrio
  (unk46) of the buttons / cursors kept (V17). The page slides (ama seqs 13..27) are not played. Disc:
  `dreamcast/game/cutscene/patches/build_disc_menus_sprites.patch` (mnu_opti, mnu_opti_sou(_<lang>)) |
  menus host 45/45, flow 75/75, story 125/125; Flycast: DIFFICULTY ? / PLAYER DATA in the dark header,
  NORMAL lit with the cursor, EASY with the cursor after RIGHT, the sound test with its title in the header
  and the ticker in the black bar.
- V16 | checked | title menu, time attack menu (records, zone selector), player select, a time-attack clear
  -> Try Again (menu, Cream art), QUIT -> title; the extra ending (talks over the frozen 3D scenes, Super
  Sonic and Burning Blaze, white fades), staff roll 2 (credits, CONGRATULATIONS pictures, SEGA / END),
  TransformDemo 4 in script 3. TransformDemo 3 is not a Super form: tr_id[3] = kind 1, player 1 (Blaze's
  warp of the kind of TransformDemo 2); Flycast (Blaze, zone 7 boss intro): streaks, light, the a5i3 dome as
  TransformDemo 2. Blaze ending (five pictures with captions, white fades) and staff roll 1 (credits column,
  Blaze / Cream running across it as on the DS main screen); game over (debug/autodie.txt "300 0": fade to
  black, the letters drop in, GAME OVER centred, then the SEGA logo). Records (PLAYER DATA -> BATTLE PLAY
  RECORD, Flycast): WIN / LOSE / DRAW header, the totals row (00 00 00), the Chao art, BATTLE PLAY RECORD
  banner, back button; the totals row has no name (the DS shows the owner's name from the DS settings, which
  the Dreamcast does not have); the opponents' names (font8) in the host dump. No change.
- NOTE (flow, not changed here) the Blaze autopilot run (progress 20, autoclear at 900 fight frames) left
  the boss act of zone 5 with Blaze's lives saved as 0 (`lives 2/0`) after a clear; the next zone's boss act
  then ended in a game over at its first death. With autoclear 300 the same run kept 2 / 2: the deaths come
  from the long autoclear fights, the save of 0 lives after a clear is for the flow owner to check.
- V17 | medium | TitleDemo (the intro between the SEGA logo and the title screen) was not translated: the
  SEGA logo went straight to the title | FIXED menus/mn_titledemo.c (TitleDemo_* : six scenes, 258 frames:
  "SONIC" fades in, Blaze's then Sonic's picture scroll up under their bands, "RUSH" spins in (affine,
  scale 0.5 -> 1, 2.5 turns), Sonic (mirrored, scale 14) and Blaze (13.125) slide in on the ortho camera
  with the menu_ex poses, "RUSH" shines, white; SE 1..4 of SSAR 0 and the music Sound_PlaySeq(0); START / A
  fade to white in 0x20 frames and go to the title, as TitleDemo_CheckSkipInput) on the menus' 3D layer
  (mn_model.c now takes any instance: MnModelDesc, LookAt, per-axis scale, cull, material colours, lights;
  set 2 = menu_ex.narc). Single screen: the top screen at x1.875 over the top 480 x 360, the logo part of
  the bottom screen at x1.15 under it, white around; the two screens fade apart (MnComp.split_bright: the
  main screen's brightness overlay after its regions). Also: AnimatorSprite oamPrio (unk46) in the menus
  (MnSpr.oam, sorted like Oam_AllocObj's lists: the RUSH sprite in front of the bands), affine BG quads
  (mn_emit_bg_affine), menudata.py converts title_demo_rush_spin_*.bbg, csdata.py adds menu_ex.dca / .dcv to
  models.txt. Not done: the edge marking, the clip of the magnified RUSH to the bottom screen |
  menus host test (SegaLogo -> TitleDemo at frame 216, -> TitleScreen 260 frames later, 2 models in scenes
  4 / 5, skip with A in scene 2 -> white title screen), story 125/125; Flycast: the six scenes, the models
  animating in scene 5, SONIC RUSH assembled, white, the title screen.
- V18 | low (latent) | menus: a task destroyed in the frame that drew it (TitleScreen -> TitleMenu,
  Options exit, the title demo's end: Task_DestroyAll after the draw, as the DS code does) freed the files
  this frame's quads still drew from (mn_draw built textures from freed memory; the host test crashed
  once the heap layout changed) | FIXED menus/mn_core.c: mn_res_free / mn_bg_free queue the files,
  mn_flush_freed() frees them after menus_draw_tr and at the next frame start | host tests.
- Open: the DS edge marking (model outlines: DISP3DCNT 0x20 with G3X_SetEdgeColorTable) of the zone-map
  character and the title-demo models is not drawn.
- FIXED (boss; main.c patch dreamcast/game/boss/patches/boss_restart_main.patch) a death in a boss act
  did not re-create the boss: the fight went on with the damage done, the boss mid-attack. On the DS
  Player_Respawn -> StageRestart_Create destroys every task and creates the stage again (full
  health, title card, then the start). The patch chains g_dcp_hooks.restart and runs boss_shutdown +
  boss_init in the main loop; boss_start comes after the title card as on the first start.
- FIXED (boss; patches boss_fx3d_main.patch, boss_fx3d_build_disc.patch) Boss7's 3D BAC effects
  (EffBoostA_3D / EffBoostA2_3D, EffFire01_3D, EffKemuriSpin3D) were only logged: now one-shots through
  g_boss_hooks.fx3d -> objects_fx3d_spawn (follow states not kept; EffTrail not drawn).
- CHECKED (boss) not deviations, seen in the Flycast boss pass: the Boss1 hammer's edge knockback
  (Player_Action_KnockbackIntangible clears the solid-edge flag) sends the player over the pit strips at
  x < 16 / x > 431 of z13 and the pit line kills, as the same code does on the DS; Boss6 box 6 has the
  instant-kill flag (atk |= 0x20, Boss6_InitHitboxes) and kills with rings.
- NOTE (menus / flow, debug starts only) after a boss clear a debug-start disc continues with z12 as
  Sonic (also after a Blaze fight): menus_stage_exit has no story state there.

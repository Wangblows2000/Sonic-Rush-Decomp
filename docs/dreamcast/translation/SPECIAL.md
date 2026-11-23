# Special stages: native translation (Dreamcast port)

Status:

- All 275 functions of `src/game/SpecialStage/` (overlay 1) are in the native module
  `dreamcast/game/special/`: 199 translated statement by statement, 74 translated with a DS service
  replaced (loading, VRAM, geometry-engine and register code), 2 rewritten from the asm fallbacks. The
  function table is in section 7.
- Controls: a native Dreamcast controller scheme (section 4), no stylus or pointer emulation. The
  Trick Spring targets show the Dreamcast button of each number; "PRESS START TO BEGIN" replaces the
  touch prompt.
- Data: `dreamcast/game/special/tools/spdata.py` regenerates every asset from the owner's ROM extract
  into `build/dreamcast/special/`; `tools/padart.py` derives the controller art from the converted ROM
  art. Nothing converted is committed.
- Host tests: `make -C dreamcast/game/special host-test` runs the original logic headless on all 7
  stages, driven through the controller scheme. Integrated in the game and played in Flycast (REGRESSION.md); not run on real hardware.
- Blaze has no special stages in the original: `GmkSpecial_Create` refuses to create the special ring
  when `gGameState.unk0 == 1`, and the stages load only Sonic (`narc/ss_player_lz7.narc`).
  `special_init(stage, 1)` returns -2.

## 1. Files

| file | content |
|---|---|
| `special.h` | Public API (section 5). |
| `sp_internal.h` | The merged structs. The field names keep the original offsets (`unk148` = rings @0x148, ...). Also the prototypes. |
| `sp_core.c` | `SpecialStage_*` top level (Start, Init, intro and play states, StartPlay, RequestExit, State_Exit, Shutdown), `SpecialStageData_*` (SST scripts), `SpecialStageTrick_*` (SSD sequences), pipe and arc maths, and the API. |
| `sp_map.c` | `SpecialStageMap_*`: the segment list, scroll, curve matrices and segment type table, and the map/sky models. |
| `sp_gimmick.c` | `SpecialStageObjList_*`, `SpecialStageGimmick_*` (incl. the two asm rewrites), `SpecialStageActObj_*`, `SpecialStage_UpdateObject(s)`, `SpecialStage_CheckGimmickCollisions`, `SpecialStage_TestCollision`, `SpecialStage_ActObj*`, `SpecialStage_Unk_ov001_0230e3c8/0230ea6c/0230f284`, `SpecialStageTrickReward_Update`. |
| `sp_player.c` | `SpecialStagePlayer_*`: all states, actions, collision callbacks, update and draw. Also the table of the native functions behind the ROM code pointers. |
| `sp_camera.c` | `SpecialStageCamera_*` and the `dcm_camera_t` / `NNS_G3dWorldPosToScrPos` equivalents. |
| `sp_hud.c` | `SpecialStageHud_*`, `SpecialStage_CreateIntroTasks`, `SpecialStageIntroBg_*`, HUD/intro asset loading. |
| `sp_model.c` | The Model runtime (Model_Init/InitSprite3D/SetResource/SetAnimation/Animate). It keeps the original flag protocol at @0x10C and the speed at @0x114. |
| `sp_sprite.c` | AnimatorSprite on the converted BAC frame tables (`frames.bin`). |
| `sp_math.c` | NitroSDK fixed point, bit-exact: FX_Div / FX_Sqrt / VEC_Mag / VEC_Distance / VEC_Normalize emulate the DS divider and square-root unit. MTX_*, FX_Atan2 (ROM table), G3i_LookAt_. |
| `sp_data.c` | The `spdata.bin` loader: ROM tables by DS address (`SP_ROM(0x02317904, ...)`) and the address -> function map. |
| `sp_input.c` | The controller scheme (section 4): steering target, attack, Trick Spring inputs, begin. It also builds `gPad`. |
| `sp_draw.c` | Draw-command capture and the single-screen composition (section 3). KOS PVR backend; host software rasteriser for tests. |
| `sp_audio.c` | Sound calls with the original archive/sequence numbers. Hooks or the default DC sound/music. |
| `tools/spdata.py` | The BYOR converter (section 6). |
| `tools/padart.py` | Controller art from the converted ROM art (section 6): the button targets, PRESS START TO BEGIN, the legend lines, and the redrawn caption bar. |
| `test/test_special.c` | Host tests. |
| `test/sptest_main.c`, `test/make_test_disc.sh` | Standalone KOS test program and its CDI (special stage only). |
| `Makefile` | `data`, `host-test`, `dc`, `test-elf`, `test-disc`, `clean`. |

## 2. How the original works (what was kept)

- **Course.** The SST script of each stage (`ss/ss_N.sst`: header "SST", command offset/count,
  quotas at +0x10/+0x12, the first-half length at +0x14/+0x18) adds segments (straight, left/right/up/down
  curves, mid-point, goal) and spawns gimmicks per segment. `SpecialStageData_Update` runs 4 segments ahead.
  `SpecialStageMap_Update` scrolls the segment matrices (fx32), drops a passed segment and adds a step.
- **Player.** The state table `gSpecialStagePlayerStateTable` (27 states x 0x34 bytes) is read from the
  ROM. Each state sets the velocity mode, the animation, the visibility animation, the collision circles,
  the speed, the SE and the state function. Sonic's position is an angle on a radius-0x1e000 pipe plus a
  height. Steering turns at 0x800 per frame toward the target angle. Holding against a wall jumps
  (state 6, EdgeJump).
- **Gimmicks.** The 24 types of `gSpecialStageGimmickTypeTable` (rings, 5-ring bonus, dash panels,
  trick ramps, switches that spawn ring arcs, five enemy types, two bomb types, checkpoint and goal
  triggers, the emerald). They keep their hit points, ring reward, collision circles, update/defeat
  functions and the exact pipe-matrix maths.
- **Rules.**
  - Ring quota 1 is checked at the mid-point trigger (type 0x15, `SpecialStageHud_UpdateCheckpoint`:
    `quota1 <= rings`, else BAD and the fail sequence). Quota 2 is checked at the goal trigger
    (type 0x14, `SpecialStageHud_UpdateGoal`).
  - A bomb hit costs 10 rings and drops up to 10 rings (`SpecialStagePlayer_State_Hurt`), then 180
    frames of invulnerability.
  - Tapping an enemy or bomb in reach starts the homing attack (state 0x12). On the Dreamcast this is
    **A** (section 4). An enemy that is destroyed gives its ring reward.
  - Trick ramps (Trick Springs) lead to the numbered target sequences of `ss/ss.ssd`:
    - 26 sequences of 3 to 8 targets (`pts[8]`: the format holds at most 8), time limit 0xb4/0x78/0x3c
      frames;
    - the remaining-time thirds rate Nice / Great / Fantastic and give 5 / 10 / 15 reward rings
      (0x023178cc);
    - sequences 3, 11, 12, 13, 17, 18, 20, 21, 23 and 25 chain into another one.
  - The goal: Sonic grabs the emerald with the right hand (r_hand node matrix), the clear card shows,
    then `SpecialStage_RequestExit(1)`. The fail path is `SpecialStage_RequestExit(0)` after 200 frames.
  - The original has no timer and no score in the special stage. The clock is the course itself.
- **Camera.** 12 camera modes, the speed-limited / lerp chase (float factors as the original), the intro
  orbit, perspective half-angle 0x1555, aspect 0x1555, near 3, far 1500.

## 3. Single-screen composition

On the DS the special stage uses both screens:

- The **touch screen** shows the 3D half-pipe and its sprites: captions, Go!/START!/GOOD!/BAD, the trick
  circles, the ratings and the stylus pen. Before StartPlay it also shows the intro backdrop, the caption
  bar and "touch the screen". At the start of play the `sp_fix_touch` steering guide is blended at 4/16
  under the moving stylus (HUD frames 0x168..0x262).
- The **top screen** shows the ring counter and quota, the seven emerald slots, the course progress
  bar, the "SPECIAL STAGE" logo of the intro and the results (emerald circle, "SONIC GOT A CHAOS
  EMERALD!").

The Dreamcast draws one 640x480 view (`sp_draw.c`):

- The touch screen **is** the view: 256x192 -> 640x480 (x2.5, 4:3). The camera, field of view and framing
  are the original's, so the half-pipe and Sonic have the original apparent size. The main-engine sprites
  and BGs are drawn at their DS positions x2.5.
- The top-screen HUD elements are drawn at x1.5, in their original art and colours:
  - ring icon, count, "/", quota (and the big emerald behind them after the checkpoint): top-left;
  - the seven emerald slots: top-right (slot spacing 36 px, the icons are not scaled differently);
  - the progress bar with Sonic's head and the mid-point marker: under the emeralds. The bottom edge
    stays free for the trick timing bar of the touch screen.

  Their animations are the original ones, incl. the slide-in of the slots and the shaking head when hurt.
- Intro: the backdrop `sp_stdm_base_b`, the scrolling caption bar and the prompt in the blue square are
  the touch screen. The top screen's "SPECIAL STAGE" logo (`sp_stdm_base_a`, stripe colours keyed out)
  is part of the backdrop at x1.25, behind the 3D Sonic.
- Results: when the clear sequence starts (`unkBD4 >= 0`), the top screen's `sp_cldm_base` + scrolling
  caption + emerald circle become the full view at x2.5. They fade in with the top screen's brightness.
- Brightness: the main-screen master brightness is a white overlay. The top-screen brightness whitens
  the top-screen elements (PVR offset colour).
- Controller art in the same view (section 4):
  - The Trick Spring targets show the Dreamcast buttons at the size and place of the numbers.
  - While a sequence is open, the progress bar (top-right) is drawn see-through (alpha 0x50). It
    lies over the top row of targets.
  - The stylus demonstration (pen sprite `spr2[9]` + `sp_fix_touch` arc) is not drawn. In its frames
    (0x17c..0x258) a black card with "[stick] OR [<] [>] : MOVE SONIC" sits where the pen was.
  - Intro: the prompt in the blue square reads PRESS START TO BEGIN. At the first special stage of
    the session the legend card is drawn over the dimmed intro.
  - Pause: the frozen frame dimmed, with the pause card over it (PAUSE, the controls, START OR A :
    CONTINUE, and B : GO BACK when offered).

  The cards are black with a white rim, in the caption bar's font, at x2. Only the English art is
  replaced, because the port runs in English (`special_set_language(1)`).
- PVR lists:
  - **OP**: the intro backdrop and the opaque 3D;
  - **PT**: punch-through 3D, the 3D sprites (rings, enemies, effects) and the intro logo;
  - **TR**: translucent 3D, the fog/fade overlays, the rating bar, the 2D layers and sprites, the pause
    dim and the brightness.

Checked in Flycast screenshots (section 8): one view, no screen split, no duplicated view, the HUD at
the edges.

## 4. Controls (native Dreamcast controller scheme)

The original reads only the touch screen and START. The port emulates no stylus and no pointer. The
four places where the original reads `gTouchState` take physical inputs instead (`sp_input.c` ->
`g_sp_pad`). Everything after them is the original code:

- Sonic's movement, turn rate and momentum;
- the edge jump, collision, the homing attack and enemy defeat;
- the Trick Spring timer, rating, rewards and chains.

| DS (touch screen) | Dreamcast controller | where |
|---|---|---|
| Touch where Sonic should run. `SpecialStage_TouchToPipeAngle` turns the touch point into a target pipe position `r = 0x1e000 * dx / len(d)`: the horizontal component of the direction from (128, 45). Touching level with or above the centre = a wall, which arms the edge jump. | **Analogue stick, horizontal axis**, proportional. The engine dead zone (`g_input_deadzone`, 32) means no input. From its edge to full tilt (abs(x) >= 120) the axis is that horizontal component (`sp_stick_to_angle`): `r = 0x1e000 * (abs(x) - 32) / 88`, then `SpecialStage_PipeXToAngle`. Full tilt = the wall target, so the edge jump comes as on the DS. The stick in its centre targets the bottom of the pipe. The target is set in the frame of the input: no added delay. | `SpecialStagePlayer_UpdateTouchSteer` |
| (same) | **D-pad LEFT / RIGHT** (digital alternative): the target moves along the pipe at 0x600 per frame (90 degrees in 11 frames) from Sonic's angle while held. On release the target stays, as when the stylus is lifted. **D-pad DOWN**: back to the bottom. | (same) |
| Touches within 75 px of (128, 45) do not steer (`SpecialStagePlayer_IsWithinTouchRadius`). | Replaced by the stick dead zone. | `SpecialStagePlayer_State_Run` |
| Tap an enemy / bomb in reach: the homing attack (state 0x12). | **A** attacks the nearest gimmick the tap could hit: not destroyed, with a touch circle, attackable, in reach (`SpecialStageObj_IsInReach`) and on screen. These are the gimmicks the target cursor marks (`SpecialStageGimmick_Unk_ov001_0230f0f4`). Nearest = largest z, then the closest across the pipe (`SpecialStageGimmick_FindAttackTarget`). With nothing attackable A does nothing. Hit points, hit effects, rewards and defeat are the original. | `SpecialStagePlayer_CheckTouchTarget` |
| Touch the numbered Trick Spring targets in order. Touching the current target advances, touching a later one is a MISS, touching a done target or empty space does nothing. When the time limit runs out it is a MISS (`SpecialStageTrick_CheckTouch`). | Target n = input n, see the table below. The targets show the button. The DS rule is applied to the button **presses** of the frame (`sp_qte_step`): the current target's input advances; the input of a later target is a MISS; inputs of done targets, or of numbers the sequence does not have, do nothing. Two target inputs in the same frame (mashing) are a MISS unless only the current one is among them. The timer, time limits, ratings and rewards are untouched. | `SpecialStageTrick_CheckTouch` |
| "Touch the touch screen to continue" (intro, the original takes any touch) | **START** or **A**: "PRESS START TO BEGIN". | `SpecialStage_State_IntroUpdate` |
| START: pause (PauseMenu) | **START**. The frame freezes and dims under the pause card. START or A continues. B goes back (`SpecialStage_RequestExit(0)`) only when this stage's emerald is already owned, which is when the original offers GO BACK. | `special_run_frame` |

Trick Spring inputs:

| number | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 |
|---|---|---|---|---|---|---|---|---|---|---|
| input | A | B | X | Y | L trigger | R trigger | D-pad Up | D-pad Right | D-pad Down | D-pad Left |
| badge | red A | blue B | yellow X | green Y | silver L | silver R | charcoal up arrow | right arrow | down arrow | left arrow |

- **Press events only.** A held button or direction does not repeat. It registers again only after a
  release.
- **Triggers.** An analogue trigger presses at >= 160 (`SP_TRIG_ON`). It is released only after 2
  consecutive frames at <= 64 (`SP_TRIG_OFF`). This hysteresis plus release debounce makes a half-pulled
  or wobbling trigger give one press at most.
  - A trigger held when the stage starts counts only after a release.
  - Without the engine patch (5.3), the trigger is the engine's digital IN_L / IN_R (threshold 128),
    through the same edge logic.
- **Sequence length.** The SSD format holds at most 8 targets (`pts[8]`). The data has 3 to 8 per
  sequence (sequence 14 has 8). So 9 (Down) and 10 (Left) never appear in the game. They are
  implemented and host-tested with the rule above, and their badges exist (`ac_spst_pad` anims
  65..68).
- **Context.** The target inputs exist only while a sequence is open (player state 0xa with the trick
  flag @0x128). While one is open there is no steering and A does not attack. Outside a sequence A
  attacks, the D-pad and stick steer, and B / X / Y / L / R do nothing. START is never a target input.
  The press that closes the pause is not passed to the game.
- **Legend.** A card with the controls is drawn at the intro prompt of the first special stage of the
  session. It is dismissed by the begin press. `special_show_legend(1)` shows it again next time. The
  pause card shows the same controls.
- **Captions.** The scrolling caption bar "USE THE STYLUS TO MOVE SONIC" reads "USE STICK OR [<] [>] TO
  MOVE SONIC". The prompt "TOUCH THE TOUCH SCREEN TO CONTINUE" reads "PRESS START TO BEGIN". Both use
  the original fonts:
  - The bar is the caption-bar font of the six languages' captions. K, X, the arrows and "/" are drawn
    in its 6x12, 2-px-stroke style.
  - The prompt is the white-plate font of `ac_spst_<lang>` anim 0, letters sliced from the English,
    French and German art. Its G is the caption font's G in plate colours.

## 5. API and integration

```c
#include "special/special.h"
special_set_path("/cd/special");          /* disc tree of tools/spdata.py */
special_set_language(1);                  /* gGameLanguage, 1 English (captions, sprites) */
special_set_emerald_flags(g_emeralds);    /* gGameState.unk28 */
/* optional: special_show_legend(1) shows the controller legend at the next intro (default: once per boot) */
int r = special_init(stage, 0);           /* 0 ok, -1 assets missing, -2 not Sonic / bad stage */
for (;;) {                                /* 60 Hz */
    input_poll(&in);
    r = special_run_frame(&in);           /* 0 running, 1 cleared (emerald won), 2 failed / quit */
    render_begin_frame(); special_draw(); render_end_frame();   /* special_draw opens OP, PT, TR itself */
    sound_update(); audio_update();
    if (r) break;
}
SpecialResult res; special_result(&res);  /* stage, cleared, emerald_flags, rings, quota_1/2, frames, score (0) */
special_shutdown();
```

`special_init` calls `sound_load_bank("ss")`, which replaces the stage bank. Set `sound_set_dir` to where
`ss.srb` is (the game uses `/cd/sound`: copy `build/dreamcast/special/sound/ss.srb` there). It plays the
sys music `/cd/music/s30.wav` / `s31.wav` with `/cd/music/sys.txt`; `build_disc.sh` already ships these. To
route sound elsewhere, set `special_hooks` (sfx/music/music_stop/sfx_stop_arc).

What the original passes back to the game flow: only the emerald. `SpecialStage_Shutdown` does
`gGameState.unk28 |= 1 << stage` when cleared (`res.emerald_flags`). The rings of the special stage are
not added to anything, and there is no score: `SpecialResult.score` is 0 and `rings` is informational.

### 5.1 Entering (original: GmkSpecial, the big special ring)

1. Map object 0x9e (`GmkSpecial_Create`) is the special ring, only for Sonic in story mode
   (`gGameState.unk10` not 1/2/4, `unk0 != 1`). Touching it starts `Player_Action_GmkSpecial`: Sonic
   circles the ring. If he **boosts** (`pf & 0x80`, the tension gauge) for 0xc0 frames while circling, he
   is launched (`Player_Action_GmkSpecialEnter`, SE 0x47/0x48/0x49). After 0x40 frames of
   `Player_State_GmkSpecialEnter` it sets `pf |= 0x40000000` (white fade from frame 0x10).
2. `Player_Last` then does `gGameState.unk18 |= 0x40; StageExit_Create(Player_ExitToSpecialStage, 3)` ->
   `SceneChange_ToSpecialStage` -> `SpecialStage_Start` -> `SpecialStage_Init(gGameState.unk1C)`. So the
   special stage is the **zone index** of the act (zone 1 -> `ss_1.sst`).
3. The destructors save the act: `Player_Destructor` (with `unk18 & 0x40`) stores the following in
   `data_022c4590`:
   - rings (@0x3AE) and ring total (@0x3B0);
   - the stage timer `gGameState.unk20`;
   - the shield (1 barrier `pf 0x10000000`, 2 magnet `0x20000000`);
   - the tension (@0x350);
   - x >> 8, and (y >> 8) + 0x118 (- 0x118 with gravity flipped), so the respawn is 280 px below the ring;
   - the water flag (`pf2 & 0x8000`).

   `Stage_SaveEventState` copies the map-object list and the ring list (collected rings and destroyed
   objects stay gone). `Stage_SaveRestartCamera` saves the camera.

**DC (HEAD 4845e568):** the objects module runs steps 1-2. It calls
`g_obj_hooks.stage_exit(ctx, OBJ_EXIT_SPECIAL_STAGE)` and sets `pf |= 0x40000000`
(`objects/obj_pgimmick.c`). `main.c` intercepts the kind (`g_special_pending`), and `run_special_stage`
then does the rest:

1. saves the step-3 values (`g_ss`, `objects_save_event_state`);
2. stops the act;
3. runs the loop above with `special_init(g_dcp_world.zone, 0)`.

### 5.2 Returning

`SpecialStage_State_Exit` -> `SpecialStage_Shutdown` -> `SceneChange_ToStageFromSpecialStage` ->
`StageEntry_StartFromSpecialStage` = `Camera_Reset(); StageEvent_Reset(); StageStart_Create(3)` with
`unk18 & 0x40` still set. Then:

- `StageStart_State_SetupResources` restores the event state (map objects, rings) and the restart camera.
- `Player_Create` restores the saved values above. It also sets `pf2 |= 0x200000` and
  `invuln (@0x3B2) = invuln_len (@0x376)` (blinking invulnerability), and `unk2C4 |= 5`.
- The act restarts with the title card (`StageStart_Create(3)`).
- `Stage_ResetProgress` clears `unk18` on the next normal stage start.

The result (cleared or not) changes only the emerald flags. `main.c` (`run_special_stage`) stores
`g_emeralds = res.emerald_flags` and reloads the act with the saved state (`g_ss.active`).

## 6. Data (BYOR) and disc paths

`make -C dreamcast/game/special data` (= `python3 dreamcast/game/special/tools/spdata.py`) reads
`extract/eur` and `build/dreamcast/audio` (for the sound bank waves; run the audio pipeline first). It
writes `build/dreamcast/special/` (5.5 MB), and `manifest.txt` lists every file. The disc needs the same
tree under `/cd/special/`:

| path | content |
|---|---|
| `spdata.bin` | "SPD1" blob table: overlay-1 ROM ranges 0x02316FE0-0x02318010 and 0x0231DF70-0x0231F040 (all tables, vectors, collision circles, file names), arm9 0x0206E470-0x02072572 (FX_SinCosTable_, atan table), the 275 function symbols (address -> name), the node names of son.nsbmd. |
| `ss/ss.ssd`, `ss/ss_1..7.sst` | copied unchanged |
| `models/*.dcm/.dca/.dcv` | `mod/ss_*.nsbmd` (map: 5 + mid + goal + 7 skies; dash panel, 2 switches, emerald light with its 4 pattern textures, 7 emeralds), `narc/ss_player_lz7.narc` (son, son_ball, son.dca 62 anims, son.dcv), `narc/ss_act_lz7.narc` (eff_clash, eff_shoot_bomb with its 9 pattern textures). Converted with `tools/dreamcast/models/nsbmd2dc.py`; the vertex/polygon counts match the NNS headers. |
| `models/*.dcc/.dcp/.dct` | Material animations (MODELS.md 4.1): `mod__ss.dct` (`ss.nsbta`: the sky scroll `ss_map_sky`, 241 frames), `mod__ss.dcp` (`ss.nsbtp`: `ss_eme_light`, 25 frames), `narc__ss_act_lz7__eff_clash.dcc`, `eff_shoot_bomb.dcc` / `.dcp`. `sp_draw.c` applies slots 1..3 at the draw (`dcm_draw_ex`). |
| `sprites/act__ac_spst*/frames.bin + atlas0.argb1555` | `act/ac_spst*.bac` (base, 6 languages, ann, efct, gmk) via `tools/dreamcast/sprites/bac2dc.py` |
| `sprites/act__ac_spst_pad/` | Controller art (`tools/padart.py`, derived from the converted ROM art), DSPR v1 + `palettes.bin`, one 512x256 ARGB1555 page (66 images; some of them have more than 15 colours, so no PAL4). Contents: <ul><li>anim 0: PRESS START TO BEGIN (92x66, the anchor of `ac_spst_<lang>` anim 0);</li><li>anims 0x2a..0x39: targets 1..8 at the anims of the numbers, plain + lit 4-frame. Each is the target disc with its digit removed (radial fill of the rings), recoloured, with the button glyph outlined like the digits;</li><li>anim 0x3e: the target burst;</li><li>anims 65..68: targets 9 and 10;</li><li>anims 70..79: legend / pause lines.</li></ul> |
| `bg/sp_*.mnbg` | the 12 `bg/sp_*.bbg` (menus `.mnbg` format, `menus/tools/menubg.py`); `sp_stdm_caption_eng.mnbg` redrawn by `padart.py` (the converted original is kept in `build/dreamcast/special/.padart/`, not on the disc) |
| `sound/ss.srb` | sound-effect bank (`tools/dreamcast/audio/sfxbank.py` format): sys SDAT SSAR 9, 14, 16 with banks 17/22/24. 748 KB of sound RAM (budget 1.83 MB), 119 waves. Load it with `sound_set_dir` + `special_init` (or copy it to `/cd/sound/`). |
| `/cd/music/s30.wav`, `s31.wav`, `sys.txt` | SEQ_SS_GAME / SEQ_SS_END (already on the game disc) |

VRAM: about 0.6 MB of model textures at init. The sprite atlases and BGs are uploaded on first use;
the controller set takes 256 KB. In Flycast, `pvr_mem_available()` was 4,514,248 bytes after init
(HEAD 4845e568 with this change). All of it is freed by `special_shutdown`: 5,635,816 bytes free
afterwards, the same after every stage.

The converter fix in this change: `sfxbank.Builder.build` takes a `select` argument since HEAD. The
old way (hiding the other SSARs in a patched SDAT) then fell back to the front-end subset: 9 waves,
33 KB, and the special-stage sound effects were missing. `spdata.py` now passes
`select={9: None, 14: None, 16: None}`: 119 waves, 747,712 bytes of sound RAM, as before.

## 7. Function table

Statuses:

- **translated**: same statements and constants. Types are merged and named; DS pointers are native.
- **translated, DS service replaced**: the game logic is kept; loading / VRAM / geometry-engine /
  register / task code is replaced as noted.
- **rewritten from asm**: from the listing in `src/asm_fallback/` (no matching C exists).
- **translated, controller scheme**: the one statement that reads the touch screen takes the controller
  input of section 4 (`g_sp_pad`); the rest of the function is the translation. Native helpers of the
  scheme (not original functions): `sp_stick_to_angle`, `sp_qte_step`, `sp_trigger_step`
  (`sp_input.c`), `SpecialStageGimmick_FindAttackTarget` (`sp_gimmick.c`) and the overlays in
  `sp_draw.c` (`draw_controller_overlay`).

Not translated: none of the 275. Outside `src/game/SpecialStage/` the special stage also uses the
PauseMenu (replaced by the native pause of section 4) and the generic Model/AnimatorSprite/Background
runtime (re-implemented in `sp_model.c`, `sp_sprite.c`, `sp_hud.c`).

| function | status | note |
|---|---|---|
| SpecialStageActArchive_GetCompressionType | translated, DS service replaced | LZ77 decompressed offline (spdata.py) |
| SpecialStageActArchive_GetUncompressedSize | translated, DS service replaced | LZ77 decompressed offline |
| SpecialStageActObj_CreateByType | translated |  |
| SpecialStageActObj_Destroy | translated |  |
| SpecialStageActObj_SpawnAt | translated |  |
| SpecialStageCamera_Apply | translated, DS service replaced | G3i_LookAt_ (bit-exact) into g_sp_view; projection to sp_camera_dcm |
| SpecialStageCamera_CreateList | translated |  |
| SpecialStageCamera_FreeList | translated |  |
| SpecialStageCamera_GetVectors | translated |  |
| SpecialStageCamera_Init | translated |  |
| SpecialStageCamera_LoadLookAt | translated, DS service replaced | G3i_LookAt_ result kept in g_sp_view -> dcm_camera_t |
| SpecialStageCamera_LoadProjection | translated, DS service replaced | perspective kept in g_sp_proj -> dcm_camera_t |
| SpecialStageCamera_Orbit | translated |  |
| SpecialStageCamera_PopOrtho | translated, DS service replaced | as above |
| SpecialStageCamera_PushOrtho | translated, DS service replaced | rating bar drawn as 2D quads (same rectangle) |
| SpecialStageCamera_Register | translated |  |
| SpecialStageCamera_SetMode | translated |  |
| SpecialStageCamera_SetPerspective | translated |  |
| SpecialStageCamera_SetPosition | translated |  |
| SpecialStageCamera_SetTarget | translated |  |
| SpecialStageCamera_SetupMode | translated |  |
| SpecialStageCamera_Shutdown | translated |  |
| SpecialStageCamera_Unregister | translated |  |
| SpecialStageCamera_Update | translated |  |
| SpecialStageCamera_UpdateOrbitAngles | translated |  |
| SpecialStageData_AddStep | translated |  |
| SpecialStageData_RunCommand | translated |  |
| SpecialStageData_Update | translated |  |
| SpecialStageGimmick_CalcPipeMatrix | translated |  |
| SpecialStageGimmick_CreateManager | translated |  |
| SpecialStageGimmick_Destroy | translated |  |
| SpecialStageGimmick_DestroyManager | translated |  |
| SpecialStageGimmick_Draw | translated, DS service replaced | AnimatorSprite_Draw / Model_Draw -> draw commands (logic kept) |
| SpecialStageGimmick_FindTouched | translated, DS service replaced | NNS_G3dWorldPosToScrPos -> float projection; not called by the controller scheme (FindAttackTarget, sp_gimmick.c) |
| SpecialStageGimmick_OnAttacked | translated |  |
| SpecialStageGimmick_OnDestroyGiveRings | translated |  |
| SpecialStageGimmick_Spawn | translated |  |
| SpecialStageGimmick_SpawnActObjAt | translated |  |
| SpecialStageGimmick_SpawnTrickReward | translated |  |
| SpecialStageGimmick_SwayRotation | translated |  |
| SpecialStageGimmick_SwayX | translated |  |
| SpecialStageGimmick_TestTouchCircle | translated |  |
| SpecialStageGimmick_Unk_ov001_02309994 | translated |  |
| SpecialStageGimmick_Unk_ov001_023099c4 | translated |  |
| SpecialStageGimmick_Unk_ov001_0230e634 | translated |  |
| SpecialStageGimmick_Unk_ov001_0230e6c0 | translated |  |
| SpecialStageGimmick_Unk_ov001_0230e710 | translated |  |
| SpecialStageGimmick_Unk_ov001_0230e78c | rewritten from asm | src/asm_fallback listing |
| SpecialStageGimmick_Unk_ov001_0230e8fc | translated |  |
| SpecialStageGimmick_Unk_ov001_0230e94c | rewritten from asm | src/asm_fallback listing |
| SpecialStageGimmick_Unk_ov001_0230ed48 | translated |  |
| SpecialStageGimmick_Unk_ov001_0230ee44 | translated |  |
| SpecialStageGimmick_Unk_ov001_0230ef64 | translated |  |
| SpecialStageGimmick_Unk_ov001_0230ef9c | translated |  |
| SpecialStageGimmick_Unk_ov001_0230f0f4 | translated |  |
| SpecialStageGimmick_Unk_ov001_02310144 | translated |  |
| SpecialStageGimmick_Unk_ov001_02312dc8 | translated |  |
| SpecialStageGimmick_Update | translated |  |
| SpecialStageHud_AnimateSprites | translated |  |
| SpecialStageHud_CalcCirclePos | translated |  |
| SpecialStageHud_Create | translated, DS service replaced | Background/AnimatorSprite set-up kept; VRAM / palette allocation dropped |
| SpecialStageHud_Destroy | translated, DS service replaced | free (no VRAM) |
| SpecialStageHud_Draw | translated, DS service replaced | AnimatorSprite_Draw -> draw commands (logic kept) |
| SpecialStageHud_DrawDigits | translated, DS service replaced | AnimatorSprite_Draw / Model_Draw -> draw commands (logic kept) |
| SpecialStageHud_DrawEmeralds | translated, DS service replaced | AnimatorSprite_Draw / Model_Draw -> draw commands (logic kept) |
| SpecialStageHud_DrawPopDigits | translated, DS service replaced | AnimatorSprite_Draw / Model_Draw -> draw commands (logic kept) |
| SpecialStageHud_DrawRingCounter | translated, DS service replaced | AnimatorSprite_Draw / Model_Draw -> draw commands (logic kept) |
| SpecialStageHud_DrawRingCounterScaled | translated, DS service replaced | AnimatorSprite_Draw / Model_Draw -> draw commands (logic kept) |
| SpecialStageHud_EndTrickRating | translated |  |
| SpecialStageHud_FlushPaletteNow | translated, DS service replaced | OBJ palette slot 8 lerp -> cross-fade to the stage-colour emerald image |
| SpecialStageHud_InitGoalSequence | translated |  |
| SpecialStageHud_SetDigitAnims | translated |  |
| SpecialStageHud_ShowTrickRating | translated |  |
| SpecialStageHud_SpawnSparkle | translated |  |
| SpecialStageHud_ToggleSubBg1Task | translated, DS service replaced | sub BG1 enable bit kept |
| SpecialStageHud_Unk_ov001_02308884 | translated, DS service replaced | AnimatorSprite_Draw / Model_Draw -> draw commands (logic kept) |
| SpecialStageHud_Unk_ov001_02308b74 | translated |  |
| SpecialStageHud_Unk_ov001_02308d40 | translated, DS service replaced | AnimatorSprite_Draw / Model_Draw -> draw commands (logic kept) |
| SpecialStageHud_Unk_ov001_02308e44 | translated, DS service replaced | AnimatorSprite_Draw / Model_Draw -> draw commands (logic kept) |
| SpecialStageHud_Unk_ov001_02308fc8 | translated, DS service replaced | AnimatorSprite_Draw / Model_Draw -> draw commands (logic kept) |
| SpecialStageHud_Unk_ov001_023091a0 | translated, DS service replaced | AnimatorSprite_Draw / Model_Draw -> draw commands (logic kept) |
| SpecialStageHud_Unk_ov001_02309228 | translated |  |
| SpecialStageHud_Unk_ov001_023095e0 | translated, DS service replaced | BG1 enable + BLDALPHA eva/evb kept as display state; drawn as alpha |
| SpecialStageHud_Unk_ov001_02312dd8 | translated |  |
| SpecialStageHud_Unk_ov001_02314214 | translated |  |
| SpecialStageHud_Unk_ov001_02314368 | translated |  |
| SpecialStageHud_Update | translated |  |
| SpecialStageHud_UpdateCheckpoint | translated |  |
| SpecialStageHud_UpdateClear | translated |  |
| SpecialStageHud_UpdateEmeraldCircle | translated |  |
| SpecialStageHud_UpdateFail | translated |  |
| SpecialStageHud_UpdateGoal | translated |  |
| SpecialStageHud_UpdateQuotaCounter | translated |  |
| SpecialStageHud_UpdateRingCounter | translated |  |
| SpecialStageHud_UpdateRingCounterThunk | translated |  |
| SpecialStageIntroBg_Destructor | translated, DS service replaced | BG1/BG2 disabled, layer cleared |
| SpecialStageIntroBg_Main | translated, DS service replaced | BG1 enable bit kept |
| SpecialStageMap_AddSegment | translated |  |
| SpecialStageMap_AllocSegment | translated |  |
| SpecialStageMap_Curve1Advance | translated |  |
| SpecialStageMap_Curve1DrawOffset | translated |  |
| SpecialStageMap_Curve1PivotOffset | translated |  |
| SpecialStageMap_Curve2Advance | translated |  |
| SpecialStageMap_Curve2DrawOffset | translated |  |
| SpecialStageMap_Curve2PivotOffset | translated |  |
| SpecialStageMap_Curve3Advance | translated |  |
| SpecialStageMap_Curve3DrawOffset | translated |  |
| SpecialStageMap_Curve3PivotOffset | translated |  |
| SpecialStageMap_Curve4Advance | translated |  |
| SpecialStageMap_Curve4DrawOffset | translated |  |
| SpecialStageMap_Curve4PivotOffset | translated |  |
| SpecialStageMap_Draw | translated, DS service replaced | matrix code kept; MTX_STORE slot 0x1e -> g_sp_mtx_slot |
| SpecialStageMap_DrawSegmentType0 | translated, DS service replaced | folded into the segment table (draw model index 0) |
| SpecialStageMap_DrawSegmentType1 | translated, DS service replaced | folded into the segment table (draw model index 1) |
| SpecialStageMap_DrawSegmentType2 | translated, DS service replaced | folded into the segment table (draw model index 2) |
| SpecialStageMap_DrawSegmentType3 | translated, DS service replaced | folded into the segment table (draw model index 3) |
| SpecialStageMap_DrawSegmentType4 | translated, DS service replaced | folded into the segment table (draw model index 4) |
| SpecialStageMap_DrawSegmentType5 | translated, DS service replaced | folded into the segment table (draw model index 5) |
| SpecialStageMap_DrawSegmentType6 | translated, DS service replaced | folded into the segment table (draw model index 6) |
| SpecialStageMap_FreeSegment | translated |  |
| SpecialStageMap_GetCurrentSegment | translated |  |
| SpecialStageMap_GetCurveLength | translated |  |
| SpecialStageMap_GetScrollDistance | translated |  |
| SpecialStageMap_GetScrollSpeed | translated |  |
| SpecialStageMap_GetStraightLength | translated |  |
| SpecialStageMap_IsSegmentListEmpty | translated |  |
| SpecialStageMap_ScrollCurve1 | translated |  |
| SpecialStageMap_ScrollCurve2 | translated |  |
| SpecialStageMap_ScrollCurve3 | translated |  |
| SpecialStageMap_ScrollCurve4 | translated |  |
| SpecialStageMap_ScrollStraight | translated |  |
| SpecialStageMap_SetScrollSpeed | translated |  |
| SpecialStageMap_StraightAdvance | translated |  |
| SpecialStageMap_StraightDrawOffset | translated |  |
| SpecialStageMap_StraightPivotOffset | translated |  |
| SpecialStageMap_Update | translated |  |
| SpecialStageObjList_Alloc | translated |  |
| SpecialStageObjList_Create | translated |  |
| SpecialStageObjList_Destroy | translated |  |
| SpecialStageObjList_Draw | translated, DS service replaced | Model_Draw -> draw commands (rotation/scale/position code kept) |
| SpecialStageObjList_Free | translated |  |
| SpecialStageObjList_IsCulled | translated, DS service replaced | NNS_G3dWorldPosToScrPos -> float projection of the same camera |
| SpecialStageObjList_Update | translated |  |
| SpecialStageObj_IsInReach | translated |  |
| SpecialStageObj_MarkDestroyed | translated |  |
| SpecialStagePlayerArchive_GetCompressionType | translated, DS service replaced | LZ77 decompressed offline |
| SpecialStagePlayerArchive_GetUncompressedSize | translated, DS service replaced | LZ77 decompressed offline |
| SpecialStagePlayer_Action_Celebrate | translated |  |
| SpecialStagePlayer_Action_Fail | translated |  |
| SpecialStagePlayer_Action_Start | translated |  |
| SpecialStagePlayer_Action_StartTrick | translated |  |
| SpecialStagePlayer_ChangeState | translated |  |
| SpecialStagePlayer_CheckTouchTarget | translated, controller scheme | the tap -> A + SpecialStageGimmick_FindAttackTarget (section 4) |
| SpecialStagePlayer_ClampTilt | translated |  |
| SpecialStagePlayer_Create | translated |  |
| SpecialStagePlayer_Destroy | translated |  |
| SpecialStagePlayer_Draw | translated, DS service replaced | Model_Draw -> draw command |
| SpecialStagePlayer_DrawModels | translated, DS service replaced | after-images -> draw commands with light colour 0x6182 |
| SpecialStagePlayer_EndDash | translated |  |
| SpecialStagePlayer_GetSpeedAnimRate | translated |  |
| SpecialStagePlayer_IsAngleWithin90 | translated |  |
| SpecialStagePlayer_IsGrounded | translated |  |
| SpecialStagePlayer_IsWithinTouchRadius | translated | not called: the stick dead zone replaces it (section 4) |
| SpecialStagePlayer_MoveToTarget | translated |  |
| SpecialStagePlayer_OnAttackHit | translated |  |
| SpecialStagePlayer_OnHurt | translated |  |
| SpecialStagePlayer_OnTouchGimmick | translated |  |
| SpecialStagePlayer_RHandMtxCallback | translated, DS service replaced | r_hand joint world matrix computed from the pose (sp_player.c rhand_capture) |
| SpecialStagePlayer_SetRHandCallback | translated, DS service replaced | enables the capture; b->unkA = 4 kept |
| SpecialStagePlayer_SlowDownMap | translated |  |
| SpecialStagePlayer_StartDash | translated |  |
| SpecialStagePlayer_State_AttackFall | translated |  |
| SpecialStagePlayer_State_AttackRebound | translated |  |
| SpecialStagePlayer_State_Brake | translated |  |
| SpecialStagePlayer_State_Celebrate | translated |  |
| SpecialStagePlayer_State_CelebrateSpin | translated |  |
| SpecialStagePlayer_State_EdgeJump | translated |  |
| SpecialStagePlayer_State_EdgeLand | translated |  |
| SpecialStagePlayer_State_Fail | translated |  |
| SpecialStagePlayer_State_Fail2 | translated |  |
| SpecialStagePlayer_State_Fail3 | translated |  |
| SpecialStagePlayer_State_GoalSlowDown | translated |  |
| SpecialStagePlayer_State_Hurt | translated |  |
| SpecialStagePlayer_State_HurtRecover | translated |  |
| SpecialStagePlayer_State_Land | translated |  |
| SpecialStagePlayer_State_Run | translated, controller scheme | touch-radius test -> stick dead zone |
| SpecialStagePlayer_State_Start0 | translated |  |
| SpecialStagePlayer_State_Start1 | translated |  |
| SpecialStagePlayer_State_Stopped | translated |  |
| SpecialStagePlayer_State_TargetAttack | translated |  |
| SpecialStagePlayer_State_Trick | translated |  |
| SpecialStagePlayer_State_TrickChain | translated |  |
| SpecialStagePlayer_State_TrickEnd | translated |  |
| SpecialStagePlayer_State_TrickInput | translated |  |
| SpecialStagePlayer_State_TrickJump | translated |  |
| SpecialStagePlayer_State_WaitLand | translated |  |
| SpecialStagePlayer_TurnTowards | translated |  |
| SpecialStagePlayer_Unk_ov001_02312250 | translated |  |
| SpecialStagePlayer_Unk_ov001_02314694 | translated |  |
| SpecialStagePlayer_Update | translated |  |
| SpecialStagePlayer_UpdateAngleFromX | translated |  |
| SpecialStagePlayer_UpdateTouchSteer | translated, controller scheme | target from the stick / D-pad instead of TouchToPipeAngle(touch point) |
| SpecialStagePlayer_UpdateTurn | translated |  |
| SpecialStageTrickReward_Update | translated |  |
| SpecialStageTrick_CheckTouch | translated, controller scheme | the same rule on button presses (sp_qte_step) |
| SpecialStageTrick_GetRating | translated |  |
| SpecialStageTrick_GetSequence | translated |  |
| SpecialStageTrick_Init | translated, DS service replaced | Touch_Enable: no touch screen |
| SpecialStageTrick_Reset | translated |  |
| SpecialStageTrick_SetSequences | translated |  |
| SpecialStageTrick_Shutdown | translated, DS service replaced | Touch_Disable: no touch screen |
| SpecialStage_ActObjBounceMove | translated |  |
| SpecialStage_ActObjDestroyWhenDone | translated |  |
| SpecialStage_ActObjFollowParent | translated |  |
| SpecialStage_ActObjReleaseSlots | translated |  |
| SpecialStage_ActObjSpawnGimmick | translated |  |
| SpecialStage_ArcLengthToAngle | translated |  |
| SpecialStage_CheckGimmickCollisions | translated |  |
| SpecialStage_CreateIntroTasks | translated, DS service replaced | tasks -> task ids; Background_Init/Draw -> layer state; sprites kept |
| SpecialStage_DisableTexture | translated, DS service replaced | folded into the fade quad / rating bar commands |
| SpecialStage_DrawFadeQuad | translated, DS service replaced | white quad with alpha n/31 (the DS quad covers the view) |
| SpecialStage_DrawObjects | translated, DS service replaced | AnimatorSprite_Draw / Model_Draw -> draw commands (logic kept) |
| SpecialStage_FreeHudBlock | translated, DS service replaced | free() |
| SpecialStage_FreeStageDataBlock | translated, DS service replaced | free() |
| SpecialStage_GetEmeraldFlags | translated |  |
| SpecialStage_GetGimmickManager | translated |  |
| SpecialStage_GetPlayer | translated |  |
| SpecialStage_GetRingQuota | translated |  |
| SpecialStage_GetStageIndex | translated |  |
| SpecialStage_Init | translated, DS service replaced | VRAM bank / register set-up -> display state + lights in sp_draw.c; call sequence kept |
| SpecialStage_InitDummy | translated |  |
| SpecialStage_IsPointInCircle | translated |  |
| SpecialStage_LoadActArchive | translated, DS service replaced | converted eff_clash/eff_shoot_bomb; slot pools and list callbacks kept |
| SpecialStage_LoadGimmickModels | translated, DS service replaced | converted .dcm + BAC sprites; Model_InitSprite3D anims and flags kept |
| SpecialStage_LoadHudAssets | translated, DS service replaced | files from build/dreamcast/special (mnbg, DSPR, ss.ssd); the sound-archive set-up is the ss.srb bank |
| SpecialStage_LoadHudFile | translated, DS service replaced | sp_file_load |
| SpecialStage_LoadMapModels | translated, DS service replaced | converted .dcm per model; Model set-up (ops 4/2, slot 0x1e, sky ops 0x13/3, scale 50, SRT loop) kept; ss.nsbta = converted `mod__ss.dct` |
| SpecialStage_LoadPlayerArchive | translated, DS service replaced | converted son.dcm/son_ball.dcm/.dca/.dcv instead of the LZ NARC; Model set-up kept |
| SpecialStage_LoadStageData | translated |  |
| SpecialStage_LoadStageDataFile | translated, DS service replaced | sp_file_load |
| SpecialStage_PipeAngleToX | translated |  |
| SpecialStage_PipeXToAngle | translated |  |
| SpecialStage_PlayUpdateDummy | translated |  |
| SpecialStage_ReleaseActArchive | translated, DS service replaced | free |
| SpecialStage_ReleaseGimmickModels | translated, DS service replaced | free |
| SpecialStage_ReleaseHudAssets | translated, DS service replaced | free + NNS_SndStopSoundAll -> sp_audio_stop_all |
| SpecialStage_ReleaseMapModels | translated, DS service replaced | free |
| SpecialStage_ReleasePlayerArchive | translated, DS service replaced | free |
| SpecialStage_ReleaseStageData | translated, DS service replaced | free |
| SpecialStage_RelocateCurrentSsdHeader | translated, DS service replaced | as above |
| SpecialStage_RelocateSsdHeader | translated, DS service replaced | signature check; offsets resolved on use (64-bit host pointers) |
| SpecialStage_RelocateSstHeader | translated, DS service replaced | offset resolved on use (sst_cmds) |
| SpecialStage_RequestExit | translated |  |
| SpecialStage_RotateMtxXAroundPivot | translated |  |
| SpecialStage_RotateMtxYAroundPivot | translated |  |
| SpecialStage_SetPolygonAlpha | translated, DS service replaced | folded into the fade quad command |
| SpecialStage_Shutdown | translated, DS service replaced | frees; DISPCNT/blend resets -> display state |
| SpecialStage_ShutdownDummy | translated |  |
| SpecialStage_Start | translated |  |
| SpecialStage_StartPlay | translated |  |
| SpecialStage_State_Exit | translated, DS service replaced | SceneChange -> special_run_frame return value (main.c) |
| SpecialStage_State_IntroDraw | translated, DS service replaced | AnimatorSprite_Draw / Model_Draw -> draw commands (logic kept) |
| SpecialStage_State_IntroUpdate | translated, controller scheme | the touch at frame 0x5c -> START / A |
| SpecialStage_State_PlayDraw | translated, DS service replaced | fog table -> g_sp_fog overlay density |
| SpecialStage_State_PlayUpdate | translated, DS service replaced | PauseMenu_Create(1) -> native pause (see section 4) |
| SpecialStage_TestCollision | translated |  |
| SpecialStage_TouchToPipeAngle | translated | not called; its horizontal-component rule is sp_stick_to_angle (section 4) |
| SpecialStage_Unk_ov001_02309894 | translated |  |
| SpecialStage_Unk_ov001_023099ec | translated |  |
| SpecialStage_Unk_ov001_0230e3c8 | translated |  |
| SpecialStage_Unk_ov001_0230ea6c | translated |  |
| SpecialStage_Unk_ov001_0230f284 | translated |  |
| SpecialStage_Unk_ov001_02312dd0 | translated |  |
| SpecialStage_UpdateObject | translated |  |
| SpecialStage_UpdateObjects | translated |  |

## 9. Gaps and deviations (honest list)

- **Pause menu**: the original PauseMenu sprites (`ac_fix_pause`) are not in the special-stage data. The
  pause card in the caption-bar font replaces them.
- **Controller art** is English only. The six language variants of the prompt and caption bar keep the
  original touch text. The port calls `special_set_language(1)`.
- **Trick Spring targets 9 and 10** (D-pad Down / Left) cannot occur: the SSD format holds 8 targets. They
  are implemented and tested at the rule level only.
- **Not tested in Flycast:** partial analogue tilts (keyboard axes are full or ramping) and partial
  trigger pulls. Host tests cover both. Without the engine patch (5.3), a stick flick during an open
  sequence also counts as a D-pad direction.
- **Composition**: during a Trick Spring sequence the progress bar is see-through, because it lies over
  the top row of targets.
- **Rendering differences**:
  - The DS edge marking (`G3X_SetEdgeColorTable`, outlines) and the DS fog are not rendered. The
    120-frame white fog at the start is a white overlay with the same density curve.
  - The player animation cross-fade (flags 0x40) changes the timing as in the original, but the pose is
    drawn from the new animation only.
  - The palette lerp of top-screen OBJ palette slot 8 is drawn as a cross-fade of the big emerald to the
    stage-colour emerald image (anim 0x13 + stage).
  - The top-screen frame art `sp_fix_base` is not drawn: its panels sit behind the top-screen HUD on the
    DS, and drawn over the 3D they would cover the view.
  - Main-BG blending uses alpha eva/16 instead of the DS eva/evb sum.
- **Sound**: the effects use the native SSAR sequencer with the ss bank, music via the sys streams.
  `NNS_SndPlayerStopSeqBySeqArcIdx(0x10, 0xf, 0x5a)` needs `special_hooks.sfx_stop_arc` (no default).
  Nothing was listened to.
- **Undefined-behaviour guards** where the original reads out of range: `SpecialStageTrick_GetSequence`
  with idx < 0 (the original condition never fires) uses sequence 0. `SpecialStageObjList_Update` does
  not free a node twice (the original could, if both of its rules matched).
- Load time in Flycast is ~25 s per stage (emulated CD speed).
- `SpecialStage_Unk_ov001_0230ea6c` / RHandMtxCallback: the right-hand matrix is computed from the
  sampled pose at the integer frame. The DS takes it from the geometry engine during the draw.
- Act re-entry after the special stage is done in `main.c` (the act reloads at the special
  ring's position; the emerald is saved when the act is cleared).

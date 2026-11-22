# Bosses: native translation (Dreamcast port)

Status (2026-11-22):

- The shared boss framework and four bosses are translated: Boss1 (Egg Hammer, Leaf Storm act 3, stage
  `z13`), Boss2 (Egg Turtle, Water Palace act 3, `z23`), Boss3 (Mirage Road act 3, `z33`) and Boss4
  (Night Carnival act 3, `z43`). `boss_init` dispatches on the stage id (2, 5, 8, 11).
- Boss1 host test: the whole fight runs on the real z13 level with the real player module and ObjRect
  code. Measured: 16/16 checks pass with the default seed; the fight completes for 5 seeds x 2
  difficulties.
- Boss2 host test: the whole fight runs on the real z23 level (same setup). Measured: 18/18 checks pass
  for seeds 0x12345678, 1, 2, 3, 0xdeadbeef and 77, each at 6 and 8 hit points (12 runs).
- Boss3 host test (z33): 18/18 checks pass for the same 6 seeds x 2 difficulties (12 runs). Every run
  visits idle, push, summon, kick, throw attack, hurt and defeat, and reaches the stage-clear request.
- Boss5 (Huge Crisis act 3, `z53`, stage 14) is the Egg Hammer's zone-5 variant (map object 0x3b ->
  Boss1_Create, `g_dcp_world.zone == 4`): battery turrets (Boss1_NodeCallback_Batteries) and the
  Boss5PartD3 missiles (`boss5.c`). The Boss1 test runs it when given the z53 level: 16/16 checks for
  10 of the 12 seed / difficulty runs; seed 1 (6 hp) never picks a big slam and seed 77 (6 hp) never
  picks a plain slam, so one coverage check fails there (the same kind of gap as Boss1 seed 1). All 12
  runs reach the stage clear with exactly 6 (8) hits.
- Boss4 host test (z43): 17/17 checks pass for the same 12 runs. Every run visits spin, clash, hurt,
  slam, tilt spin and defeat, and reaches the stage-clear request.
- `kos-cc -O2 -Wall -Wextra -Werror` builds all 8 sources.
- Not yet run on Dreamcast hardware or in an emulator. Rendering is checked only on the host, by
  projecting vertices through the boss camera.
- Single screen (user rule): every boss arena renders in one 640x480 view with `boss_camera()`. Nothing
  in the boss module draws to a second screen. The DS sub-screen information goes to the UI through
  `boss_health()` / `boss_health_max()` / `boss_phase()` and `g_boss_hooks.navi` (NaviMsg). Boss3 and
  Boss4 do not write POWCNT (only Boss1/Boss2 did, and those writes are dropped).

## 1. Files

All files are under `dreamcast/game/boss/`.

| file | role |
|------|------|
| `boss.h` | Public API (below). |
| `boss_internal.h` | Shared structs, kept at the original offsets: `BossModel` (Model), `BossCamera` (`data_ov000_0236a444`), `BossStatus` (`gBossStatus`), `BossEffect`. Original names are mapped to `boss_`-prefixed link symbols. Declares the object-system entry points the module uses. |
| `boss_core.c` | `bossdata.bin` loader, `Boss_*`, `BossArena_*`, ScreenShake, the StageObj_Main subset (`boss_obj_main`), stage set-up and the API. |
| `boss_camera.c` | BossCamera Create / Main / UpdateFixed / UpdateTrack0 / UpdateTrack1. Outputs a `dcm_camera_t` (LookAt + PerspectiveW from the same fx32 state). |
| `boss_model.c` | Model runtime: Model_Init, SetResource, SetAnimation, Animate (Model_AnimateMdl copied), the Model_Draw matrix ops as `boss_model_pose`, node matrices, and `dcm_draw` submission. Also the fx32 MTX helpers. |
| `boss_effect.c` | BossEffect_Create (from the ARM listing), State_Main, all 35 `BossEffect_Create<Name>` wrappers, EffPebble, and the StageObj_Draw3D step for effects. |
| `boss1.c` | Boss1, Boss1Neck, Boss1Hammer: all 196 functions of `src/game/Boss/Boss1/`, plus BossStage1 (model load/free). |
| `boss2.c` | Boss2, Boss2Water, Boss2WaterLight, BossStage2: all 86 functions of `src/game/Zone/Water_Palace/Boss/`, plus EffRipple3D / EffSplash3D (Water Palace effects the boss spawns). |
| `boss3.c` | Boss3, Boss3PartCC (body), Boss3PartCD (ball), BossStage3: all 153 functions of `src/game/Zone/Mirage_Road/Boss/`. |
| `boss5.c` | Boss5PartD3 (missile) and the BossStage5 resources: all 9 functions of `src/game/Zone/Huge_Crisis/Boss/` (BossStage5_OnModelLoaded is merged into `boss1_load`). Boss1_NodeCallback_Batteries is in `boss1.c`. |
| `boss4.c` | Boss4, Boss4PartCE..D2, BossStage4: all 105 functions of `src/game/Zone/Night_Carnival/Boss/` plus the 2 kept as ARM listings in `src/asm_fallback/` (Boss4_State_WaitForStart, Boss4_SubState_SlamStopSpin). |
| `tools/bossdata.py` | Converter. It writes `build/dreamcast/boss/bossdata.bin` and `assets.txt`. |
| `test/test_boss.c` .. `test/test_boss4.c`, `test/stubs_objects.c` | Host tests (Boss1 on z13, Boss2 on z23, Boss3 on z33, Boss4 on z43). The stubs are weak definitions for object-module functions that are declared but not yet written. |
| `Makefile` | `make data`, `make host-test` (all four), `make host-test1` .. `host-test4`, `make dc`. |

`bossdata.bin` ("BOSD" v1) contains the following. Nothing from the ROM is compiled into the sources.

- The ROM tables the code reads, keyed by DS address (10 chunks, 16844 bytes in all):
  - Boss1: `data_ov000_02362b58..02362dc4`;
  - Boss2: `data_ov000_02362dc4..0236306c` (rodata) and `data_ov000_023650fc..02365414` (data: palette and
    node name pointer tables, defeat positions, the hit-box callback table, `gBoss2ModelColorsAlt`);
  - `gEffSplash3DModelNames` / `gEffRipple3DModelNames` (`0x02367b38..02367b9c`);
  - zone lights `data_020748bc` (vectors) / `data_02074960` (colours) (`0x020748bc..02074a04`);
  - ScreenShake: `data_0207323c`, `data_02073240`, `data_0207324c`;
  - Boss3: `data_ov000_0236306c..02363504` (rodata) and `data_ov000_02365414..023655e8` (palette name
    pointer tables, names);
  - Boss4: `data_ov000_02363504..023636e8` (rodata, incl. the float spin-scale table `02363668`) and
    `data_ov000_023655e8..023657e0`.
  - Pointers inside these tables keep their DS value; `ROM_STR(ptr)` resolves them through the same
    chunks. The hit-box callback table holds ROM function addresses; `Boss2_HitCallback` maps them to the
    native functions (the tool checks the table bytes).
  - The C code reads them with `BOSS_DATA(0x02362d64, ...)`, so the original symbol stays visible in
    the source.
- The node indices that the code looks up by name: `bs1_neck:neck_ham` = 10, `bs1_hammer:hammer` = 0.
  These come from the NSBMD node dictionaries (`g3d.py`).
- The Boss2 node names: `bs2_body:body0` / `body_neck` / `prop`, `bs2_neck:neck_head`, `bs2_head:head_01`.
- Boss3: `bs3_body:body_ball`. Boss4: `bs4_rod:rod_arm` / `rod_body`, `bs4_arm:arm_gun`, `bs4_body:body_exit`.
- An effect table: for each boss stage, the converted package of each BossEffect name. There are 49
  entries, 5 of them for z13 and 10 for z23 (incl. `eff_ripple_s/m/b`, `eff_splash_s/b`). Duplicates are
  resolved through `models/manifest.json`.
- A timing-only animation table: the frame counts of animation files that are not converted
  (`bs2.nsbta`: 60, 30, 30, 240; `bs3.nsbta`: 60; `bs4.nsbma`: 60; `bs4.nsbtp`: 21;
  `eff_ele_stay.nsbma`: 40; `eff_ele_flash.nsbma`: 8; `eff_wave_disap.nsbma`: 10). The tool reads
  numFrame from any NNS animation file (SRT0 / MAT0 / PAT0 / JNT0 block). `Model_SetAnimation` turns them into animation objects without
  joint data, so the "ended" bit the Boss2 water states wait on keeps the original timing.

## 2. API and how to wire it into main.c

```c
#include "boss/boss.h"
/* after level + player + objects are set up, for boss stages (act 3: hdr->act == 2) */
boss_set_paths("/cd/models", "/cd/boss");
boss_set_difficulty(0);                     /* gGameState.unk14: 1 -> 8 hit points, else 6 */
g_boss_hooks.sfx = ...; .voice = ...; .music = ...; .music_stop = ...;   /* audio.h */
g_boss_hooks.navi = ...;                     /* NaviMsg (HUD), optional */
g_boss_hooks.cleared = ...;                  /* Stage_OnBossCleared: go to results */
g_boss_hooks.ring_flags = ...;               /* RingManager_SetFlags(|1), optional */
if (boss_init(hdr->stage, level_bytes, &player, 1) == 0) is_boss = 1;
...
/* when the title card ends (StageTitleCard_State_StartStage): */
boss_start();
/* every frame, after objects_update(&player): */
boss_update(&player);
/* render (inside each list, before/after the player model): */
boss_draw(PVR_LIST_OP_POLY); ... boss_draw(PVR_LIST_PT_POLY); ... boss_draw(PVR_LIST_TR_POLY);
/* queries */
boss_health(); boss_health_max(); boss_phase(); boss_defeated(); boss_cleared();
boss_camera(&cam);          /* the boss-stage 3D camera: use it for the player model too */
boss_shake_offset(&sx,&sy); /* ScreenShake (also shakes 2D sprites/HUD on the DS) */
boss_brightness();          /* gDisplayMain.unk48: 16 = white flash, fades to 0 (defeat) */
boss_exc_background();      /* 6/7 = concentrated_line_01/02.bbg shown behind the defeat cut-ins, -1 none */
boss_player_model(tr, rot); /* Boss2: player model transform on the arena circle (fx32), 0 = not overridden */
boss_lights(&lights);       /* Boss2: this frame's lights in view space (use for the player model too) */
boss_water_level();         /* Boss2: gCamera.unk62 water level (also written to g_obj_camera[0].water) */
boss_shutdown();            /* leaving the stage */
/* Boss2 extra hooks (optional): */
g_boss_hooks.sfx_handle = ...;  /* Sound_PlaySeqArc(handle 1..3, 0, id): looping boss sounds */
g_boss_hooks.sfx_stop = ...;    /* NNS_SndPlayerStopSeq(handle, 0) */
g_boss_hooks.water_fx = ...;    /* EffWater3D_Create (water spray sprite, not translated) */
/* Boss4 (optional): */
g_boss_hooks.player_box_flags = ...; /* Boss4_Action_Defeat: player's hit boxes, box 0 body / 1 attack,
                                        flags = (flags & ~clear) | set (objects module ObjPlayerCtx) */
/* tests / debug: boss3_ball(), boss3_arc_x(), boss3_balls_alive(), boss4_info() */
```

Integration notes:

1. **The boss arena is 3D.** `StageTitleCard_State_StartStage` sets DISPCNT `0x1100` for
   `zone >= 9 || act >= 2`, so only BG0 (3D) and OBJ are shown. Do not draw the level.bin layers in
   boss stages. Keep level.bin for player collision. The arena floor, walls, trees and body are
   `bs1_base`, drawn by `boss_draw`.
2. **The player must be drawn with `boss_camera()`.** World position = `(x/256, -y/256, z/256)` at
   scale 3.3. This replaces `dcm_camera_stage` / `stage_offaxis` in boss acts. This was not checked
   against `Player_Draw` in boss acts. Check the `StageObj_Draw3DEx` flags there.
3. **Rings in boss stages are 3D** (`RingManager_*Arena*`). The objects module does not draw them yet
   (OBJECTS.md).
4. **The boss controls these player fields:**
   - `map_up` / `map_down` (@0x430/@0x432): up/down become right/left while the camera looks along
     the arena;
   - position and animation at the defeat;
   - input lock (`pf |= 0x200000`) and hide (`disp |= 0x20`) at the explosion.
   - It also stops the stage timer (`g_dcp_world.flags &= ~1`) and pauses the object system
     (`g_dcp_world.objmgr |= 1`, StageObj_SetPause) during the defeat cut-ins.
5. **Map object 0x3f (`Boss1_Create`)** must not be created by the objects module.
   `boss_init` creates it from the level's object list. Object types 0xca and 0xcb are the neck and
   the hammer.
6. **Boss2 (z23).** `boss_init(5, ...)` creates Boss2 from map object 0x3e (the objects module must
   not create it). After the title card:
   - The arena is a circle (radius 0x1152f0, circumference 0x6cd988 fx32 = 1741 px). The player's
     x is the arc position. Boss2_Draw_PlayerOnArena wraps it into 0x40..0x70d px at the start of
     every `boss_update` and builds the player model transform: draw the player with
     `boss_player_model()` and `boss_camera()`, not with `(x, -y, z)`. The 2D level keeps the
     collision (flat floor at y = 512 over the whole 1856 px map).
   - The boss writes the water level (`g_obj_camera[0].water`, px; 410 normally, 613 while drained).
     The player's underwater logic and any water surface drawing must read it. z23 has no
     WaterLevelTrigger object, so nothing else writes it.
   - Lights: `boss_lights()` (Boss2_SetLights: the boss lights above the water, the zone lights
     below). `boss_draw` uses them for the boss models.
   - Model colours: Boss2 rewrites the material diffuse / ambient / emission of the 11 stage models
     (default and the drained-water set). This is done on the loaded `.dcm`, so `boss_draw` shows it.
7. **Order:** boss_update runs after objects_update. Its ObjRect boxes are checked at the next
   `objects_frame_begin`, as on the DS, where Stage_Main runs first.
8. **Boss5 (z53).** `boss_init(14, ...)` creates Boss1_Create from map object 0x3b with
   `g_dcp_world.zone == 4` (set it before `boss_init`): the zone-5 base (bs5_base, battery joints
   rotated by `boss_model_set_joint_rot`), the bs5.nsbca missile animations of the neck, and the
   Boss5PartD3 missiles (map object 0xd3 must not be created by the objects module).
   `g_boss_hooks.missile` is still called for each launch (information only).
9. **Boss3 (z33).** `boss_init(8, ...)` creates Boss3 from map object 0x3d (the objects module must
   not create 0x3d, 0xcc or 0xcd). Arena circle radius 0xf42f8, circumference 0x5fe435 fx32 = 1534 px;
   the player's x is wrapped into 0x40..0x63e px by Boss3_Draw_PlayerOnArena every `boss_update`, and
   the player model transform comes from `boss_player_model()`, as for Boss2.
   - The boss locks the player's input (`pf |= 0x200000`) after a reflected ball and drives `held`
     (0x10 / 0x20) itself until the ball hits or gets away; the player module already honours the lock.
   - `Boss3PartCD_OnHit` calls `Player_OnHurt` / `Player_Action_AttackBounce` through the objects module.
10. **Boss4 (z43).** `boss_init(11, ...)` creates Boss4 from map object 0x3c (the objects module must
   not create 0x3c, 0xce..0xd2). Arena radius 0x128fb8, circumference 0x749fed fx32 = 1865 px; player
   x wrapped into 0x40..0x789 px. Wire `g_boss_hooks.player_box_flags` to the objects module's player
   boxes (`obj_player_ctx(p)->body` / `->atk`) so the defeat disarms them.

Disc assets (from `build/dreamcast/boss/assets.txt`: 14 files for z13, 31 for z23, 22 for z33, 26 for z43,
20 for z53; 113 lines, 0 missing):

- `/cd/boss/bossdata.bin`.
- `/cd/models/` with:
  - `mod__bs1_{base,neck,hammer,sky}__*.dcm`;
  - `narc__z13_act_lz7__bs1.dca`;
  - `narc__z13_act_lz7__eff_{hit,impact_small,impact_big,blast1}` (`.dcm` + `.dca`);
  - `narc__z13_act_lz7__eff_pebble__eff_pebble01.dcm`.
- z23: `mod__bs2_{base,lotus,water,shower,shower_s,shower_e,sky,body,neck,head,prop}__*.dcm`,
  `narc__z23_act_lz7__bs2.dca`, `narc__z23_act_lz7__eff_{blast,splash_s,splash_b,ripple_s,ripple_m,ripple_b}`
  (`.dcm` + `.dca`), `narc__z23_act_lz7__eff_suction2__eff_suction2.dcm` (no joint animation),
  `narc__z13_act_lz7__eff_{hit,impact_small}` (identical in z23, deduplicated by the converter),
  `narc__extra_act__eff_catch` (`.dcm` + `.dca`, identical in z23).
- z33: `mod__bs3_{base,body,ball,thorn,sky}__*.dcm`, `narc__z33_act_lz7__bs3.dca`,
  `narc__z33_act_lz7__eff_{blast3,pop,land}` (`.dcm` + `.dca`), and the deduplicated
  `narc__z13_act_lz7__eff_hit`, `narc__extra_act__eff_{catch,clash,jump}`, `narc__ss_act_lz7__eff_shoot_bomb`.
- z43: `mod__bs4_{base,body,arm,ele,rod,enemy,sky}__*.dcm`, `narc__z43_act_lz7__bs4.dca`,
  `narc__z43_act_lz7__eff_ele_atk__eff_ele_atk.dcm`, `..eff_ele_stay__eff_ele_stay.dcm`,
  `..eff_ele_flash__eff_flash.dcm` + `.dca`, `..eff_ele_wave__eff_wave.dcm` + `.dca`,
  `..eff_wave_disap__eff_wave_disap.dcm` + `.dca`, `..eff_blast4` (`.dcm` + `.dca`), and the deduplicated
  `eff_hit`, `eff_clash`, `eff_impact_small`, `eff_shoot_bomb` packages.
- Build them with `python3 tools/dreamcast/models/nsbmd2dc.py batch`, then `make -C dreamcast/game/boss data`.

## 3. How the translation is built

**Reuse-first.** Every original function keeps its name and body. The decompilation uses a
different `UnkStruct` per file. These were merged into `Boss1` / `Boss1Neck` / `Boss1Hammer`, and
fields keep their offset names (`unk20C`, `unk2CE`, ...). Fields of the StageObj header use the
DcPlayer names (`x` = @0x10, `vel_x` = @0x2E, `obj_flags` = @0x54, `disp_flags` = @0x5C). Each part
is a static instance. A small `StageObj_Main` subset (`boss_obj_main`) runs the parts in task order:
Boss1 0x500, neck 0x501, hammer 0x502, effects 0xa00, ScreenShake 0x3e00, BossCamera 0x3f00.

**Models and animation.** The DS Model struct is kept at its offsets:

| field | offset |
|-------|--------|
| op list | @0xA |
| scale | @0x18 |
| rot | @0x24 |
| trans | @0x48 |
| anm | @0xE4 |
| flags | @0x10C |
| speed | @0x114 |

Neck animation state: `unk334` = `unk228.flags[0]`, `unk33C` = `unk228.speed`, `unk30C` =
`unk228.anm[0]`. The state machines read the "ended" bit 2, the paused bit 1, the speed, and
`(numFrame << 12) - frame` directly, so `Model_AnimateMdl` is copied exactly. Joint poses are sampled
at the integer frame (FX_Whole, as NNS does) from the converted `bs1.dca`. Anim 0..0xc are the
hammer's and 0xd..0x1f the neck's.

**The hammer position comes from the render path.** On the DS, the hammer's game position is set in
`Boss1Hammer_Draw`:

1. The neck model's node callback stores the `neck_ham` node matrix in GE slot 0x1e.
2. The hammer is drawn with FlushVP and then MTX_RESTORE 0x1e.
3. Its own `hammer` node callback stores that node's matrix.
4. `NNS_G3dGetCurrentMtx` reads it back, and `x = m[3][0] >> 4`, `y = -m[3][1] >> 4`.

This position feeds the hit boxes and the impact effects. `boss_model_pose` and
`boss_model_joint_world` compute the same matrices (joint world x model matrix, in DS pixels). The
pose step runs where the DS draws, inside the part's main, so the game logic does not depend on
rendering. Measured: the hammer lands at y = 489 px. The impact effect is at the floor, y = 512, and
the hammer's attack box is -0x78..+0x20 around it. Standing under a slam hurts the player.

**Camera.**

- BossCamera state is copied. The 0x3C-byte track struct (objects followed per axis, offset, target,
  current, lerp, speed, accel) is named `t[0]` (eye, @0x48) and `t[1]` (at, @0x84). Every
  `data_ov000_0236a444.unkNN` write in Boss1 is mapped to its field, with the original offset in a
  comment.
- `boss_camera()` builds a row-vector LookAt (eye/at + the shake offset `unk3C`, up `unk30`) and a
  perspective projection from `unk4` (half-fovy 0x1555), `unk10` (aspect), `unk8` (near), `unkC`
  (far: Boss1 sets 0x1a66000), on a 640x480 viewport.
- `unk14` (W scale `1/3.3`) only affects depth precision on the DS, so it is not used.

**ObjRect protocol.** The hammer has 4 boxes:

| box | offset | geometry | atk / def | role |
|-----|--------|----------|-----------|------|
| 0 | @0x20C | (-0x14,-0x30,0x14,-0x20) | 9 / 0xa | damage: a player attack box (0x13) hits it, `Boss1Hammer_OnHit_Damage` |
| 1 | @0x238 | | 0x13 / 0x13 | slam attack: hurts the player body (def 2) |
| 2 | @0x264 | | 0x33 / 0x13 | big-slam attack |
| 3 | @0x290 | (-0x1c,-0x20,0x1c,0x20) | 0 / 0xfffffff0 | solid push box, `Boss1Hammer_OnHit_PushPlayer` (push or knock back the player) |

- Box 3 has flags 0xc0 and mask 0x12.
- Boxes are armed with flag 4 by the attack states and disarmed by Done/Hurt/Idle/Defeat, as in the
  decompilation.
- The boxes go through the objects module's `obj_handle_collider` and are checked by its
  `rect_check_all`.
- On a hit, the player bounces through `obj_player_attack_bounce`.

**Object-system entry points used.** They are declared in `boss_internal.h`; `obj_internal.h` is not
included, and the objects module was not edited:

- `rect_set_box3d`, `rect_set_atkdef`, `rect_forget`;
- `obj_handle_collider`, `obj_view_out`, `obj_pause_check`, `obj_box_player`;
- `obj_player_attack_bounce`.

Player actions that the player module lacks are copied into `boss1.c`. They are candidates to move
into the player module:

- `Player_Action_KnockbackIntangible`;
- `Player_Action_Blank` / `Player_State_Blank`.

## 4. Deviations and gaps (honest list)

- **Palette swaps are not shown.** `Boss1_SetPalettes` (hurt blink) and `Boss1_SetFlashPalette`
  (impact flash) load `bs1_{ham,neck}_0x_1p[d].nbfp` into the models' texture palettes. The `.dcm`
  textures are pre-decoded ARGB, so only the selection state is kept. A converter option that emits
  the alternate palette textures (or a tint in `dcm_draw`) would be needed.
- **Material, pattern and SRT animations are not converted** (`.nsbma` / `.nsbtp` / `.nsbta`, see
  MODELS.md). Effects animate their joints only. Their lifetime is still exact: every converted
  effect's `.nsbca` has the same length as its other animations.
- **Billboard op 0x14** (`SetCameraRotBillboard`) is treated like 0x13 (`SetCameraRot`). Boss1 does
  not use either.
- **Zone-5 variant (Boss5, z53)** (`gGameState.unk1C == 4`: battery missiles, `bs5.nsbca` file 9):
  - Translated: the code paths, the missile object Boss5PartD3 (`boss5.c`, a pool of 8), and
    `Boss1_NodeCallback_Batteries` (substantially rewritten: the DS multiplies RotY into the GE matrix
    at the battery nodes; natively the rotation is applied to the joint's local transform at the pose,
    so it also turns the joint's children; not compared with a DS capture).
  - The explosion's material / SRT / pattern animations (eff_bomb_up) are timing only.
- **Joint matrices are float** (dcm runtime). The hammer position is rounded from them, where the DS
  uses fx32 GE math: about 0.01 px difference (MODELS.md section 6). Everything else is the
  original fixed-point math.
- **Not shown:**
  - `Boss_DrawExcBackground` (BBG cut-in background): exposed as state;
  - `gDisplayMain.unk48` master brightness: exposed as state;
  - NaviMsg (HUD) and BossHelp (`src/game/UI/Hud/BossHelp_*`, used by Boss6/7/F only, not by Boss1).
- **Order of `Model_AnimateMdl`:** a paused slot reads an uninitialised `d` on the DS. Here it reads
  0, so no "ended" test is made for a paused slot.
- **The host tests link the object and player modules with `-ffunction-sections` and
  `--gc-sections`.** Both modules are being extended by their owners; object functions that call
  player functions not written yet (e.g. `obj_mirage.c` -> `Player_Action_Cargo`) would otherwise break
  the link. Only the code the boss tests reach is kept.
- **The host test does not call `objects_init`.** The objects module's `objdata.bin` format is being
  changed by its owner: loader v2, current tool v1. So the test registers the player's two boxes
  itself, with the son.bac boxes of the two poses it uses: standing (-6,-16,6,16), ball body
  (-8,-8,8,8) and attack (-8,-9,8,9). It links the real `obj_rect.c`, `obj_player.c` (Player_OnHurt,
  AttackBounce) and the player module.
- **Boss2 specifics:**
  - Palette swaps (hurt blink, flash of the head while its damage box is armed) are state only, as for
    Boss1 (`boss2_palette()` exposes the selection).
  - `bs2.nsbta` (water / shower texture scrolling) is not converted: timing only, no texture motion.
  - `EffWater3D_Create` (a 3D BAC sprite) is not translated; its arena position goes to
    `g_boss_hooks.water_fx`.
  - Joint cross-fades (`flags & 0x40`, Model_SetAnimation / Model_AnimateMdl) are copied; the pose
    blends translation / rotation / scale by the two ratios and re-orthonormalises the rotation. NNS's
    exact blend function was not compared.
  - Node matrices read back with NNS_G3dGetCurrentMtx are rounded from the float joint matrices (as for
    Boss1's hammer).
  - The light vectors are taken as world space and rotated into view space with the boss camera (NNS
    sends them through the camera matrix). Not compared against a DS capture.
  - Display registers (POWCNT swap, BG2 cut-in, master brightness) become `boss_exc_background()` /
    `boss_brightness()` state.
- **Boss3 specifics:**
  - Palette swaps (hurt blink, ball palette) and the ball's material flag / emission change
    (`Boss3_LoadBallPalette`: hittable ball flashes white) are state only (`boss3_palette`,
    `boss3_ball_white`).
  - `bs3.nsbta` (arena SRT animation) is timing only.
  - Balls are a pool of 8 static objects (the DS allocates a task per ball); task order 0x502 is kept
    by creation order.
  - `Boss3_Action_Defeat` / `Boss3_Action_DefeatAll` / `Boss3_State_DefeatFinish` skip a NULL ball
    (the DS would dereference it; it is never NULL on that path in the test runs).
  - The BossCamera "track the model translation" branch (camera flags 2/4, set by Boss3/Boss4) is now
    translated: `boss_obj_set_model` registers each part's Model (StageObj.unk124).
- **Boss4 specifics:**
  - Node callbacks (Rods / ArmGun / BodyExit) run per joint after the pose; `Boss4_CaptureNodeMatrix`
    keeps the fx32 scale step. Matrices are rounded from the float joint matrices.
  - Material / pattern animations (`bs4.nsbma`, `bs4.nsbtp`, `eff_*.nsbma`) are timing only; the
    D0 shot and the D2 vanish end on them, so their frame counts are in bossdata.bin.
  - The float easings (SlamTiltUp, SlamRecover, D0 Launch, SetSpinSpeed, Spin) use C float, like the
    DS softfloat single precision; not compared bit for bit with the DS.
  - `Player_Action_AttackBounceVel` is copied into boss4.c (the player module lacks it).
  - D2 draws the wave model twice per frame; the second model matrix is kept in `wave2`.
- **Not run on Dreamcast hardware or in an emulator.** PVR list classes, culling and the camera
  framing are derived, not observed. The host debug view (`DUMPAT=<frame>`) splats projected
  vertices. It shows the neck and hammer in front of the camera with the player below them, but it
  is only a point check.

## 5. Tests

```sh
make -C dreamcast/game/boss data        # bossdata.bin: 10524 bytes, effects: 44 packages (5 on z13)
make -C dreamcast/game/boss host-test   # build + run (z13 level, player tables, models)
make -C dreamcast/game/boss dc          # kos-cc -Werror: boss1.o 29.0 KB text, total ~44 KB text
SEED=0xdeadbeef LEVEL=1 build/dreamcast/boss/test_boss ...   # other seeds / 8-hp difficulty
DUMPAT=1002 build/dreamcast/boss/test_boss ...               # host debug view (PPM)
```

The scripted player (default seed 0x12345678, 6 hit points) runs in three phases:

- **[miss]**: 900 frames standing under the boss. Slams hit the player (1 hurt), and the boss health
  stays 6.
- **[fight]**: after each slam impact (damage box armed), the player is placed 40 px beside the
  hammer and does a full jump, then moves over the push box onto the head. While the hammer rolls,
  the script hops.
- **[defeat]**: runs to the stage-clear request.

Measured log of that run:

```
f  118 attack_slam ... f 1002 hit 5 ... f 1551 hit 4 ... f 1898 attack_bigslam ... f 2216 hit 3
f 2764 hit 2 (pinch music) ... f 3073 attack_roll -1 (hammer released at x=31, knockback at the
right edge, regrab) ... f 4009 hit 1 ... f 4358 attack_roll -2 ... f 5427 hit 0 -> defeat
f 5427..5827 defeat cut-ins (exc_bg 6/7/6, white flashes) -> f 5827 cleared (ring bonus 50000)
stats: attacks 9 (slam 5, big 2, roll 2), hits 6, player hurts 2, effects 13, pebbles 118
16 checks, 0 failed
```

Phases visited: 1 idle, 2 slam, 3 big slam, 4 roll, 5 hurt, 6 defeat.

Seeds 1, 2, 3, 0xdeadbeef and 77, each at 6 and 8 hit points: every fight reaches the stage clear
with exactly 6 (or 8) hits. With seed 1 at 6 hp, the random attack pattern never picked a big slam,
so the "big slam chosen" check fails for that seed only.

### Boss2 (z23)

```sh
make -C dreamcast/game/boss host-test2
SEED=1 LEVEL=1 build/dreamcast/boss/test_boss2 build/dreamcast/stages/z23/level.bin \
    build/dreamcast/player build/dreamcast/models build/dreamcast/boss [-v|-vv]
DUMPAT=1580 DUMPFILE=/tmp/v.ppm build/dreamcast/boss/test_boss2 ...      # host debug view
```

Scripted player (the level has only PlayerStart 0x72 at (424,496) and the boss 0x3e at (984,512)):

- **[miss]** 900 frames standing at the start: the boss never takes damage.
- **[fight]** when a head damage box is armed (box 4 after the attack-2 strike, box 5 after the
  attack-3 catch), the player is put on the floor 40 px before it and does a full jump onto the head
  (real son.bac attack box vs ObjRect). An "assist" (teleport above the head) exists in the script
  but was never needed: the armed heads are at y 479..484. On even-numbered dashes the player runs
  away in the dash direction (the boss then jumps at him); on odd ones he hops over the boss.
- **[defeat]** runs to the stage-clear request.

Measured log (default seed, 6 hp):

```
f  284 dash (player hurt)   f  991 attack2   f 1599 hit 5   f 1905 attack2   f 2466 hit 4
f 2692 water_lower 410 -> f 2812 attack_catch (water 562..613) -> f 2971 hit 3 -> f 3062 water_raise
f 3342 dash -> f 3428 jump at the fleeing player   f 4140 attack2   f 4669 hit 2 (pinch music)
f 4975 attack2 -> f 5490 hit 1   f 5796 attack2 -> f 6298 hit 0 -> defeat (3 cut-ins, exc_bg 6/7/6)
f 6687 cleared (ring bonus 50000)
stats: attacks 8 (dash 2, jumps 1, strike 5, catch 1), hits 6, player hurts 3
water: lowers 1 raises 1, level 410..613; ripples 268, splashes 7
18 checks, 0 failed
```

Phases visited: 7 idle, 8 dash, 9 attack 2, 10 attack 3, 11 hurt, 12 defeat. Checks also cover the
player transform on the arena circle (radius 277.2 px), the lights, the camera and the projection of
all models. All 12 seed / difficulty runs above pass 18/18.

### Boss3 (z33)

```sh
make -C dreamcast/game/boss host-test3
SEED=1 LEVEL=1 build/dreamcast/boss/test_boss3 build/dreamcast/stages/z33/level.bin \
    build/dreamcast/player build/dreamcast/models build/dreamcast/boss [-v|-vv]
```

The boss is hurt only by its own ball: during a kick the rolling ball alternates harmful / hittable; a
jumping player (anim 0x30) that touches it while hittable reflects it, and the reflected ball hits the
body's damage box. Scripted player: [miss] 900 frames standing still; [fight] when the kicked ball is
hittable for 24+ more frames the player is put on the floor in front of it and jumps ("intercept"); in
a push he jumps on the hittable ball (the body staggers) or keeps away from the harmful one.

Measured (default seed, 6 hp): hits at f 1936 / 4768 / 9188 / 9769 (pinch) / 13451 / 14232, cleared at
f 14634. Attacks 41 (push 17, kick 19, throw 5), summons 6, reflects 21 (13 balls caught again by the
body: a reflected ball that meets the body's front is caught, only the back is the damage box),
staggers 12, balls 21 spawned / 15 thrown / 15 exploded (2 alive at once). The checks also cover the
body and the player on the arena circle (radius 244.2 px), the camera and the projection.

### Boss4 (z43)

```sh
make -C dreamcast/game/boss host-test4
SEED=1 LEVEL=1 build/dreamcast/boss/test_boss4 build/dreamcast/stages/z43/level.bin \
    build/dreamcast/player build/dreamcast/models build/dreamcast/boss [-v|-vv]
```

The seesaw's body rides about 200 px above the floor while it spins. Hitting the arm (not electrified)
makes it clash: the seesaw tilts the body down (box bottom y 452) and drops an enemy; bouncing on the
enemy (-0x900) reaches the body even when it is high. Scripted player: [miss] 900 frames; [fight] jumps
at the arm or at the lowered body (placed on the floor in front of it), and when the body comes round
towards a grounded enemy it jumps and is put just above the enemy ("assist", as in the Boss2 test),
then steers under the body; [watch] at 5 hp or less it leaves the body alone until a slam was seen
(slams are only chosen at 5 hp or less), so every attack kind is covered.

Measured (default seed, 6 hp): hits at f 1042 / 2438 / 2626 / 2834 (pinch) / 3096 / 3250 (3 from a
normal jump at the lowered body, 3 after an enemy bounce), cleared at f 3649. Attacks 10 (shot bursts 7,
slam 1, tilt spins 2), shots 21 (2 at once), shock waves 1, clashes 3, enemies 3, bounces 3, assists 3.

## 6. Next

- Not yet translated (scope measured 2026-11-22, lines after removing declarations):

  | boss | stage id | level | map object | source | functions | lines | notes |
  |---|---|---|---|---|---|---|---|
  | Boss6 | 17 | z63 | 0x3a (+ part 0xd4) | `Zone/Altitude_Limit/Boss` | 100 | ~2850 | tilting deck: Boss6_ApplyTiltToPlayer reads gPad and calls `player->unk338`, `Player_Action_KnockbackIntangible`; BossHelp_Create/Destroy (needs a BossHelp hook); 9 models, NNS_G3dMdlSetMdlAlphaAll; float angle steps in Boss6_Draw |
  | Boss7 | 20 | z73 | 0x39 (+ 0xd5, 0xd6, 0xd7) | `Zone/Dead_Line/Boss` | 167 + asm `Boss7PartD5_InputState_ClashMash` | ~3020 | clash button-mash input state (gPad), BossHelp, its own lights (NNS_G3dGlbLight* in Boss7PartD5_Draw) |
  | BossF | 23 | zf3 | 0x38 (+ 0xd8..0xde) | `Zone/Final_Zone/Boss` | 267 | ~4630 | BossHelp (hand ride warning), Zone8Act3 stage set-up / StageZoneScript_ZoneFBoss |
  | Extra Zone boss | 26 (zone 8 act 2) | none (no level.bin; `extra_act*.narc`, `dm_ex_*` models) | - | `Zone/Extra_Zone/Boss` (overlay ov001) | 353 + 2 asm | ~8500 | ExtraBoss / ExtraBossObj / ExtraBossPlayer(Manager): own player control (gPad), own lights, own display set-up (ExtraBoss_InitDisplay) |

  Boss variants for Blaze's story: there is no separate Blaze boss object. Boss1-Boss6 select only
  palettes by the player slot (`gGameState.unk0`, the `_1p` / `_2p` palette files; translated as
  palette state). Boss7 branches on `gGameState.unk0` in Boss7_Create (animation 4 or 3) and
  Boss7PartD5_Create; BossFPartD8_OnHitHand branches on the player character (`unk324`); ExtraBoss_Start
  sets `gGameState.unk0` itself. These branches belong to those bosses' translations.
- Water surface / underwater handling for the player in z23 (reads `g_obj_camera[0].water`).
- In the player module: `Player_Action_KnockbackIntangible` / `Player_Action_Blank` /
  `Player_Action_AttackBounceVel`, and the boss-stage player draw.
- In the models converter: palette variants for the hurt/flash palettes.

## 7. Mapping table

Status meanings:

- **reused**: the body is copied from the decompilation. Only the merged-struct field spelling
  changed, plus FX/MTX/Rand helper names.
- **modified**: same logic, with DS services (G3D, sound, NaviMsg, archives, registers) replaced.
- **substantially rewritten**: a native mechanism replaces the original (asm-only function, node
  callbacks, palettes, asset loading).
- **not translated**: see section 4.

Counts: Boss3: 110 reused, 38 modified, 5 substantially rewritten, 0 not translated, out of 153.
Boss4: 51 reused, 43 modified, 13 substantially rewritten, 0 not translated, out of 107 (105 C files
+ 2 ARM listings). Boss1 + framework: 199 reused, 39 modified, 7 substantially rewritten, 2 not translated, out
of 247 original functions. Boss2 (+ the 7 Water Palace 3D effect functions it uses): 55 reused, 32
modified, 6 substantially rewritten, 2 not translated, out of 95.

Framework changes for Boss2: `Model_SetAnimation` (cross-fade branch copied; timing-only animation
objects), `Model_AnimateMdl` (cross-fade part copied), `boss_model_pose` (MTX_RESTORE from the matrix
stack `g_boss_mtx_slot[unk8]`, `Model_MatrixOp_Rotate` after a restore, joint cross-fade), new
`boss_model_set_colors` (NNS_G3dMdlSetMdlDiffAll / AmbAll / EmiAll), `BossEffect_State_Main` exported,
`BossEffect.unk2B0`.

New native functions:

- `boss_obj_main`: StageObj_Main subset.
- `boss_model_pose` / `boss_model_joint_world` / `boss_model_draw`: Model_Draw and the node matrices.
- `boss_camera`: G3i_PerspectiveW_ / LookAt.
- `boss_data` / `boss_joint_index` / `boss_effect_file`: the `bossdata.bin` loader.
- The Model runtime: Model_Init (reused from `src/game/Graphics/Model/Model_Init.c`),
  Model_AnimateMdl (reused), Model_SetAnimation (modified: slot 0 only).
- ScreenShake_* (reused from `src/game/Graphics/Screen/`).

| original | native file | status | note |
|---|---|---|---|
| `BossArena_ArcToCirclePos` | boss_core.c | reused |  |
| `BossArena_CirclePosToArc` | boss_core.c | reused |  |
| `BossCamera_Create` | boss_camera.c | reused | task -> called from boss_init |
| `BossCamera_Destructor` | boss_camera.c | modified | Model_Release of the sky model |
| `BossCamera_Main` | boss_camera.c | modified | mode table -> switch; track-by-model branch uses the object position (unused by Boss1); sky model posed |
| `BossCamera_UpdateFixed` | boss_camera.c | modified | G3i_PerspectiveW_/LookAt -> boss_camera() |
| `BossCamera_UpdateTrack0` | boss_camera.c | modified | same |
| `BossCamera_UpdateTrack1` | boss_camera.c | modified | same |
| `BossEffect_CreateBaseDest` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateBeamFinB` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateBlast` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateBlast1` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateBlast3` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateBlast4` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateBlast5` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateBlastF` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateBombBall` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateBoost` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateCatch` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateClash` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateDmgHand` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateHit` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateHoming` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateImpactBig` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateImpactPr` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateImpactSl` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateImpactSmall` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateImpactSt` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateJump` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateLand` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateLaserFinB` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateLaunch` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateParts` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreatePick` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreatePop` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateProduce` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateShootBomb` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateStrikeBig` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateStrikeFin` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateStrikeHit` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateSuction` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateSuction2` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_CreateTackle` | boss_effect.c | reused | wrapper (name, mask, ops) |
| `BossEffect_Destructor` | boss_effect.c | modified | resource release at boss_shutdown |
| `BossEffect_State_Main` | boss_effect.c | reused | .nsbtp bind skipped (not converted) |
| `Boss_Atan2` | boss_core.c | reused | FX_Atan2 -> dcp_atan2 |
| `Boss_DrawExcBackground` | boss_core.c | substantially rewritten | BG2 load -> boss_exc_background() state |
| `Boss_GetStartHealth` | boss_core.c | reused |  |
| `Boss_LoadModelPalettes` | - | not translated | texture palettes are pre-decoded (see deviations) |
| `Boss1Hammer_Action_Defeat` | boss1.c | reused |  |
| `Boss1Hammer_Action_Hurt` | boss1.c | reused |  |
| `Boss1Hammer_Action_Idle_ov000_022c5f94` | boss1.c | reused |  |
| `Boss1Hammer_Action_Idle_ov000_022c5fa8` | boss1.c | reused |  |
| `Boss1Hammer_Action_Idle_ov000_022c6944` | boss1.c | reused |  |
| `Boss1Hammer_Action_Idle_ov000_022c6958` | boss1.c | reused |  |
| `Boss1Hammer_Action_Idle_ov000_022c696c` | boss1.c | reused |  |
| `Boss1Hammer_Action_StartRoll` | boss1.c | reused |  |
| `Boss1Hammer_ApplySpinRotation` | boss1.c | reused | MTX_* fx32 helpers in boss_model.c |
| `Boss1Hammer_Collide` | boss1.c | reused | StageObj_HandleCollider = obj_handle_collider |
| `Boss1Hammer_Create` | boss1.c | modified | same; ObjRect copies from MapObject_Init defaults |
| `Boss1Hammer_Destructor` | boss1.c | modified | also rect_forget of the 4 boxes |
| `Boss1Hammer_Draw` | boss1.c | modified | pose with the restored neck_ham matrix; hammer position from the "hammer" node matrix |
| `Boss1Hammer_NodeCallback_Hammer` | boss1.c (Boss1Hammer_Draw) | substantially rewritten | same |
| `Boss1Hammer_OnHit_Damage` | boss1.c | modified | owner type check -> obj_box_player; NaviMsg/Sound_PlaySeq via hooks |
| `Boss1Hammer_OnHit_PushPlayer` | boss1.c | modified | owner type check -> obj_box_player |
| `Boss1Hammer_SpawnDebris` | boss1.c | modified | Rand() order of the EffPebble_Create arguments made explicit (ARM order) |
| `Boss1Hammer_State_Defeated` | boss1.c | reused |  |
| `Boss1Hammer_State_EndRoll` | boss1.c | reused |  |
| `Boss1Hammer_State_Fall` | boss1.c | reused |  |
| `Boss1Hammer_State_FinishRoll` | boss1.c | reused |  |
| `Boss1Hammer_State_Hurt` | boss1.c | reused |  |
| `Boss1Hammer_State_Idle` | boss1.c | reused |  |
| `Boss1Hammer_State_Idle_ov000_022c5f90` | boss1.c | reused |  |
| `Boss1Hammer_State_Idle_ov000_022c5fa4` | boss1.c | reused |  |
| `Boss1Hammer_State_Idle_ov000_022c5fb8` | boss1.c | reused |  |
| `Boss1Hammer_State_Idle_ov000_022c6940` | boss1.c | reused |  |
| `Boss1Hammer_State_Idle_ov000_022c6954` | boss1.c | reused |  |
| `Boss1Hammer_State_Idle_ov000_022c6968` | boss1.c | reused |  |
| `Boss1Hammer_State_Land` | boss1.c | reused |  |
| `Boss1Hammer_State_LaunchRoll` | boss1.c | reused |  |
| `Boss1Hammer_State_Roll` | boss1.c | reused |  |
| `Boss1Hammer_State_SlideBackSpinDown` | boss1.c | reused |  |
| `Boss1Hammer_State_SlideToEdge` | boss1.c | reused |  |
| `Boss1Hammer_State_SpinUp` | boss1.c | reused |  |
| `Boss1Hammer_State_StartFall` | boss1.c | reused |  |
| `Boss1Hammer_State_StartSpin` | boss1.c | reused |  |
| `Boss1Hammer_State_TurnAtEdge` | boss1.c | reused |  |
| `Boss1Hammer_State_WaitRelease` | boss1.c | reused |  |
| `Boss1Hammer_State_WaitTimer_ov000_022c67ec` | boss1.c | reused |  |
| `Boss1Neck_Action_Defeat` | boss1.c | reused |  |
| `Boss1Neck_Action_FollowPlayer` | boss1.c | reused |  |
| `Boss1Neck_Action_Hurt` | boss1.c | reused |  |
| `Boss1Neck_Action_StartBigSlam` | boss1.c | reused |  |
| `Boss1Neck_Action_StartHammerRoll` | boss1.c | reused |  |
| `Boss1Neck_Action_StartMissileAttack` | boss1.c | reused |  |
| `Boss1Neck_Action_StartSlam` | boss1.c | reused |  |
| `Boss1Neck_Action_StartTripleSlam` | boss1.c | reused |  |
| `Boss1Neck_CalcTurretAngle` | boss1.c | modified | returns the FX_Atan2 result |
| `Boss1Neck_Create` | boss1.c | modified | same |
| `Boss1Neck_Destructor` | boss1.c | modified |  |
| `Boss1Neck_Draw` | boss1.c | modified | Model_Animate + pose; stores the neck_ham node matrix (slot 0x1e) |
| `Boss1Neck_GetTurretMuzzlePos` | boss1.c | reused |  |
| `Boss1Neck_IsPlayerInSlamRange` | boss1.c | reused |  |
| `Boss1Neck_MoveToX` | boss1.c | reused |  |
| `Boss1Neck_MoveTowardX` | boss1.c | reused |  |
| `Boss1Neck_NodeCallback_NeckHam` | boss1.c (Boss1Neck_Draw) | substantially rewritten | MTX_STORE -> boss_model_joint_world |
| `Boss1Neck_State_AimAtTarget` | boss1.c | reused |  |
| `Boss1Neck_State_BigSlamApproach` | boss1.c | modified | camera field names |
| `Boss1Neck_State_BigSlamDone` | boss1.c | reused |  |
| `Boss1Neck_State_BigSlamDown` | boss1.c | reused |  |
| `Boss1Neck_State_BigSlamHold` | boss1.c | modified | camera field names |
| `Boss1Neck_State_BigSlamImpact` | boss1.c | modified | camera field names |
| `Boss1Neck_State_BigSlamImpactShake` | boss1.c | reused |  |
| `Boss1Neck_State_BigSlamRaise` | boss1.c | reused |  |
| `Boss1Neck_State_BigSlamRecover` | boss1.c | reused |  |
| `Boss1Neck_State_BigSlamRecoverWait` | boss1.c | reused |  |
| `Boss1Neck_State_BigSlamStrike` | boss1.c | modified | camera field names |
| `Boss1Neck_State_BigSlamWindUp` | boss1.c | reused |  |
| `Boss1Neck_State_Defeated` | boss1.c | reused |  |
| `Boss1Neck_State_FireAtPlayer` | boss1.c | modified | MapObject_Spawn(0xd3) + Boss5PartD3_Action_Launch (boss5.c); also g_boss_hooks.missile |
| `Boss1Neck_State_FireAtTarget` | boss1.c | modified | same |
| `Boss1Neck_State_FollowPlayer` | boss1.c | reused |  |
| `Boss1Neck_State_HammerRollDone` | boss1.c | reused |  |
| `Boss1Neck_State_Hurt` | boss1.c | reused |  |
| `Boss1Neck_State_Idle` | boss1.c | reused |  |
| `Boss1Neck_State_MissileDone` | boss1.c | reused |  |
| `Boss1Neck_State_MissileEndAnim` | boss1.c | reused |  |
| `Boss1Neck_State_MissileIntroAnim` | boss1.c | modified | camera field names |
| `Boss1Neck_State_MissilePoseAnim` | boss1.c | reused |  |
| `Boss1Neck_State_MissileReadyAnim` | boss1.c | reused |  |
| `Boss1Neck_State_MissileResetPose` | boss1.c | reused |  |
| `Boss1Neck_State_MissileRetractAnim` | boss1.c | reused |  |
| `Boss1Neck_State_MissileSelectPattern` | boss1.c | reused |  |
| `Boss1Neck_State_MissileSetPose` | boss1.c | reused |  |
| `Boss1Neck_State_MissileWaitEndAnim` | boss1.c | reused |  |
| `Boss1Neck_State_MissileWaitIntro` | boss1.c | reused |  |
| `Boss1Neck_State_MissileWaitReady` | boss1.c | reused |  |
| `Boss1Neck_State_MissileWaitResetPose` | boss1.c | reused |  |
| `Boss1Neck_State_MissileWaitRetract` | boss1.c | reused |  |
| `Boss1Neck_State_MissileWaitStart` | boss1.c | reused |  |
| `Boss1Neck_State_PickRandomTarget` | boss1.c | reused |  |
| `Boss1Neck_State_RollAnim2` | boss1.c | reused |  |
| `Boss1Neck_State_RollAnim3` | boss1.c | reused |  |
| `Boss1Neck_State_RollMoveToEdge` | boss1.c | reused |  |
| `Boss1Neck_State_RollRegrabHammer` | boss1.c | reused |  |
| `Boss1Neck_State_RollReleaseAnim` | boss1.c | reused |  |
| `Boss1Neck_State_RollReleaseHammer` | boss1.c | reused |  |
| `Boss1Neck_State_RollReturnAnim1` | boss1.c | reused |  |
| `Boss1Neck_State_RollReturnAnim2` | boss1.c | reused |  |
| `Boss1Neck_State_RollReturnAnim3` | boss1.c | reused |  |
| `Boss1Neck_State_RollReturnWait1` | boss1.c | reused |  |
| `Boss1Neck_State_RollReturnWait2` | boss1.c | reused |  |
| `Boss1Neck_State_RollReturnWait3` | boss1.c | reused |  |
| `Boss1Neck_State_RollStart` | boss1.c | reused |  |
| `Boss1Neck_State_RollStartWait` | boss1.c | reused |  |
| `Boss1Neck_State_RollWaitAnim2` | boss1.c | reused |  |
| `Boss1Neck_State_RollWaitAnim3` | boss1.c | reused |  |
| `Boss1Neck_State_RollWaitHammer` | boss1.c | reused |  |
| `Boss1Neck_State_SlamApproach` | boss1.c | reused |  |
| `Boss1Neck_State_SlamDone` | boss1.c | reused |  |
| `Boss1Neck_State_SlamDown` | boss1.c | reused |  |
| `Boss1Neck_State_SlamHold` | boss1.c | reused |  |
| `Boss1Neck_State_SlamImpact` | boss1.c | reused |  |
| `Boss1Neck_State_SlamImpactShake` | boss1.c | reused |  |
| `Boss1Neck_State_SlamRaise` | boss1.c | reused |  |
| `Boss1Neck_State_SlamRecover` | boss1.c | reused |  |
| `Boss1Neck_State_SlamRecoverWait` | boss1.c | reused |  |
| `Boss1Neck_State_SlamStrike` | boss1.c | reused |  |
| `Boss1Neck_State_SlamWindUp` | boss1.c | reused |  |
| `Boss1Neck_State_StartTrackPlayer` | boss1.c | reused |  |
| `Boss1Neck_State_TrackPlayer` | boss1.c | reused |  |
| `Boss1Neck_State_TripleSlamAfterImpact` | boss1.c | reused |  |
| `Boss1Neck_State_TripleSlamApproach` | boss1.c | reused |  |
| `Boss1Neck_State_TripleSlamDone` | boss1.c | reused |  |
| `Boss1Neck_State_TripleSlamDown` | boss1.c | reused |  |
| `Boss1Neck_State_TripleSlamFinalImpact` | boss1.c | reused |  |
| `Boss1Neck_State_TripleSlamFinalShake` | boss1.c | reused |  |
| `Boss1Neck_State_TripleSlamHold` | boss1.c | reused |  |
| `Boss1Neck_State_TripleSlamImpactShake` | boss1.c | reused |  |
| `Boss1Neck_State_TripleSlamRaise` | boss1.c | reused |  |
| `Boss1Neck_State_TripleSlamRecover` | boss1.c | reused |  |
| `Boss1Neck_State_TripleSlamRecoverWait` | boss1.c | reused |  |
| `Boss1Neck_State_TripleSlamReraiseShake` | boss1.c | reused |  |
| `Boss1Neck_State_TripleSlamRewind` | boss1.c | reused |  |
| `Boss1Neck_State_TripleSlamStartReraiseShake` | boss1.c | reused |  |
| `Boss1Neck_State_TripleSlamStartRewind` | boss1.c | reused |  |
| `Boss1Neck_State_TripleSlamStrike` | boss1.c | reused |  |
| `Boss1Neck_State_TripleSlamWindUp` | boss1.c | reused |  |
| `Boss1Neck_State_WaitTimer_ov000_022c8b90` | boss1.c | reused |  |
| `Boss1Neck_TrackPlayerFast` | boss1.c | reused |  |
| `Boss1Neck_TrackPlayerMedium` | boss1.c | reused |  |
| `Boss1Neck_TrackPlayerSlow` | boss1.c | reused |  |
| `Boss1Neck_UpdateSlamHitboxes` | boss1.c | modified | returns the range test (the ARM code leaves it in r0; callers use it) |
| `Boss1_Action_Defeat` | boss1.c | modified | Player_Action_Blank local; ring/music/timer via hooks |
| `Boss1_Action_DefeatAll` | boss1.c | reused |  |
| `Boss1_Action_Hurt` | boss1.c | modified | sound via hooks |
| `Boss1_Action_HurtAll` | boss1.c | reused |  |
| `Boss1_Action_Idle` | boss1.c | reused |  |
| `Boss1_Action_IdleAll` | boss1.c | reused |  |
| `Boss1_Action_StartBigSlam` | boss1.c | reused |  |
| `Boss1_Action_StartHammerRoll` | boss1.c | reused |  |
| `Boss1_Action_StartMissileAttack` | boss1.c | reused |  |
| `Boss1_Action_StartSlam` | boss1.c | reused |  |
| `Boss1_Action_StartTripleSlam` | boss1.c | reused |  |
| `Boss1_Action_WaitBigSlam` | boss1.c | reused |  |
| `Boss1_Action_WaitHammerRoll` | boss1.c | reused |  |
| `Boss1_Action_WaitMissileAttack` | boss1.c | reused |  |
| `Boss1_Action_WaitSlam` | boss1.c | reused |  |
| `Boss1_Action_WaitTripleSlam` | boss1.c | reused |  |
| `Boss1_Create` | boss1.c | modified | Task_Create/archives/Model_* -> static part + boss_model; MapObject_Spawn(0xca/0xcb) -> Boss1Neck_Create/Boss1Hammer_Create |
| `Boss1_Destructor` | boss1.c | modified | resource release |
| `Boss1_Draw` | boss1.c | modified | Model_Draw -> boss_model_pose |
| `Boss1_IsBigSlamDone` | boss1.c | reused |  |
| `Boss1_IsHammerRollDone` | boss1.c | reused |  |
| `Boss1_IsMissileAttackDone` | boss1.c | reused |  |
| `Boss1_IsSlamDone` | boss1.c | reused |  |
| `Boss1_IsTripleSlamDone` | boss1.c | reused |  |
| `Boss1_NodeCallback_Batteries` | boss1.c | substantially rewritten | RotY at the "base_battery01/02" joints of bs5_base -> boss_model_set_joint_rot before the pose |
| `Boss1_ResetCamera` | boss1.c | modified | camera offsets -> named BossCamera fields; POWCNT write dropped |
| `Boss1_SelectNextAttack` | boss1.c | reused |  |
| `Boss1_SetCameraSide` | boss1.c | modified | camera field names |
| `Boss1_SetFlashPalette` | boss1.c | substantially rewritten | same |
| `Boss1_SetPalettes` | boss1.c | substantially rewritten | palette swap -> state only |
| `Boss1_SetPartAnimation` | boss1.c | reused | Model_SetAnimation is the native one |
| `Boss1_State_Defeat` | boss1.c | modified | shared defeat_camera(); gDisplayMain.unk48 -> boss_brightness() |
| `Boss1_State_DefeatCamera2` | boss1.c | modified | same |
| `Boss1_State_DefeatCamera3` | boss1.c | modified | same |
| `Boss1_State_DefeatExplode` | boss1.c | modified | same; DISPCNT -> exc_bg = -1; Blast5 (zone 5) logged |
| `Boss1_State_DefeatFade_ov000_022ca890` | boss1.c | reused |  |
| `Boss1_State_DefeatFade_ov000_022ca9cc` | boss1.c | reused |  |
| `Boss1_State_DefeatFade_ov000_022cab0c` | boss1.c | reused |  |
| `Boss1_State_DefeatFinish` | boss1.c | modified | StageShutdown_Create -> g_boss_hooks.cleared (once); data_022c4590.unk10 -> g_obj_stage.ring_bonus |
| `Boss1_State_Hurt` | boss1.c | reused |  |
| `Boss1_State_Idle` | boss1.c | reused |  |
| `Boss1_State_WaitBigSlam` | boss1.c | reused |  |
| `Boss1_State_WaitForStart` | boss1.c | modified | camera field names; sky Model |
| `Boss1_State_WaitHammerRoll` | boss1.c | reused |  |
| `Boss1_State_WaitMissileAttack` | boss1.c | reused |  |
| `Boss1_State_WaitSlam` | boss1.c | reused |  |
| `Boss1_State_WaitTripleSlam` | boss1.c | reused |  |
| `Boss1_Unk_ov000_022cc110` | boss1.c | reused | empty |
| `Boss1_Unk_ov000_022cc6d4` | boss1.c | modified | gGameState.unk1C -> g_dcp_world.zone |
| `Boss1_UpdateCameraShake` | boss1.c | reused |  |
| `EffPebble_Create` | boss_effect.c | modified | StageObj_LoadModelSimple -> eff_pebble01 package |
| `EffPebble_State_Lifetime` | boss_effect.c | reused |  |
| `BossStage1_FreeModels` | boss1.c boss1_unload | substantially rewritten |  |
| `BossStage1_OnModelLoaded` | boss1.c boss1_load | substantially rewritten | async heap copy -> load the converted .dcm/.dca |

### Boss2 (boss2.c)

| original | native file | status | note |
|---|---|---|---|
| `Boss2WaterLight_CreateApplyAlt` | boss2.c | modified | Task_Create -> gBoss2WaterLightTask function pointer run after Boss2 |
| `Boss2WaterLight_CreateRestore` | boss2.c | modified | same |
| `Boss2WaterLight_Destructor` | boss2.c | reused |  |
| `Boss2WaterLight_Main_ApplyAltColors` | boss2.c | modified | NNS_G3dMdlSetMdl*All -> boss_model_set_colors (material colours of the loaded .dcm) |
| `Boss2WaterLight_Main_RestoreColors` | boss2.c | modified | same |
| `Boss2Water_Action_StartLower` | boss2.c | modified | Sound_PlaySeqArc(handle) -> boss_sfx_handle |
| `Boss2Water_Action_StartRaise` | boss2.c | modified | same |
| `Boss2Water_Destructor` | boss2.c | reused | empty |
| `Boss2Water_Main` | boss2.c | modified | NNS_G3dGlbLight* -> Boss2_SetLights; gCamera.unk62 -> g_obj_camera[0].water; EffWater3D -> hook |
| `Boss2Water_State_Lower` | boss2.c | reused |  |
| `Boss2Water_State_LowerEndShake` | boss2.c | modified | NNS_SndPlayerStopSeq(handle) -> boss_sfx_stop; the unk40 = unk3C >> 12 slip kept |
| `Boss2Water_State_LowerShake` | boss2.c | reused |  |
| `Boss2Water_State_LowerWait` | boss2.c | reused |  |
| `Boss2Water_State_Raise` | boss2.c | reused |  |
| `Boss2Water_State_RaiseEnd` | boss2.c | modified | same |
| `Boss2Water_State_RaiseWaitAnim` | boss2.c | reused |  |
| `Boss2_Action_Attack2` | boss2.c | reused |  |
| `Boss2_Action_Attack3` | boss2.c | modified | NNS_G3dRenderObjSetCallBack -> s_head_cb |
| `Boss2_Action_DashAtPlayer` | boss2.c | reused |  |
| `Boss2_Action_Defeat` | boss2.c | modified | display registers (BG2CNT, DISPCNT, offsets) -> exc_bg; gDisplayMain.unk48 -> bright; StopSeqAll/RingManager -> hooks |
| `Boss2_Action_Idle` | boss2.c | reused |  |
| `Boss2_Create` | boss2.c | modified | Task_Create/archives/Model_* -> static part + boss_model; hit callbacks from the ROM table data_ov000_0236521c mapped by address; Sound_AllocHandle -> handles 1..3 |
| `Boss2_CreateWaterController` | boss2.c | modified | Task_Create(0x4ff) -> static Boss2Water run first in boss2_update |
| `Boss2_Destructor` | boss2.c | modified | resource release; rect_forget of the 20 boxes; water task stopped |
| `Boss2_DrawDefeatBackground` | boss2.c | modified | BBG into BG2 -> Boss_DrawExcBackground (boss_exc_background) |
| `Boss2_Draw_ModelsAndHitboxes` | boss2.c | modified | Model_Draw -> boss_model_pose; MTX_RESTORE + NNS_G3dGetCurrentMtx -> g_boss_mtx_slot -> MtxFx43 (rounded from float) |
| `Boss2_Draw_PlayerOnArena` | boss2.c | modified | player model transform -> g_boss_game (boss_player_model); lights -> Boss2_SetLights; runs at the start of boss_update |
| `Boss2_InitCamera` | boss2.c | modified | camera offsets -> named BossCamera fields |
| `Boss2_NodeCallback_Body` | boss2.c | substantially rewritten | called per joint (NODEDESC order = joint order) after the pose; same unk1320 step logic |
| `Boss2_NodeCallback_Head` | boss2.c | substantially rewritten | installed/reset state kept as s_head_cb |
| `Boss2_NodeCallback_NeckHead` | boss2.c | substantially rewritten | same |
| `Boss2_OnHit_DamageAttack2` | boss2.c | modified | owner check -> obj_box_player; NaviMsg / Sound_PlaySeq via hooks |
| `Boss2_OnHit_DamageAttack3` | boss2.c | modified | same |
| `Boss2_OnHit_PushPlayer` | boss2.c | modified | owner check -> obj_box_player |
| `Boss2_SelectNextAttack` | boss2.c | reused |  |
| `Boss2_SetPalettes` | boss2.c | modified | Boss_LoadModelPalettes -> selection state only (pre-decoded textures) |
| `Boss2_SetTargetAngle` | boss2.c | reused |  |
| `Boss2_State_Attack2Aim` | boss2.c | reused | u16 field views kept through unions; head turn in shared Boss2_AimAtPlayer |
| `Boss2_State_Attack2AimEnd` | boss2.c | reused | same; suction effect restores matrix slot 0x1a |
| `Boss2_State_Attack2Charge` | boss2.c | reused |  |
| `Boss2_State_Attack2End` | boss2.c | reused |  |
| `Boss2_State_Attack2EnterTurnMode` | boss2.c | reused |  |
| `Boss2_State_Attack2Hurt` | boss2.c | modified | ResetCallBack -> s_head_cb = 0 |
| `Boss2_State_Attack2Recover` | boss2.c | modified | same (shared Boss2_StartAim) |
| `Boss2_State_Attack2StartAim` | boss2.c | modified | same (shared Boss2_StartAim); StopSeq -> hook |
| `Boss2_State_Attack2Strike` | boss2.c | reused |  |
| `Boss2_State_Attack2Turn` | boss2.c | reused |  |
| `Boss2_State_Attack3Catch` | boss2.c | reused |  |
| `Boss2_State_Attack3Close` | boss2.c | reused |  |
| `Boss2_State_Attack3End` | boss2.c | modified | same |
| `Boss2_State_Attack3Hold` | boss2.c | reused |  |
| `Boss2_State_Attack3Hurt` | boss2.c | reused |  |
| `Boss2_State_Attack3Recover` | boss2.c | reused |  |
| `Boss2_State_Attack3Start` | boss2.c | reused |  |
| `Boss2_State_Attack3Suck` | boss2.c | reused |  |
| `Boss2_State_Dash` | boss2.c | reused |  |
| `Boss2_State_DashBrake` | boss2.c | reused |  |
| `Boss2_State_DashHop` | boss2.c | reused |  |
| `Boss2_State_DashStart` | boss2.c | reused |  |
| `Boss2_State_DashStop` | boss2.c | reused |  |
| `Boss2_State_DashTurn` | boss2.c | reused |  |
| `Boss2_State_Defeat` | boss2.c | modified | DISPCNT BG2 off -> exc_bg = -1; brightness -> g_boss_game.bright |
| `Boss2_State_DefeatFinish` | boss2.c | modified | StageShutdown_Create -> g_boss_hooks.cleared (once); data_022c4590.unk10 -> g_obj_stage.ring_bonus |
| `Boss2_State_IdleAfterRaise` | boss2.c | reused |  |
| `Boss2_State_IdleAfterRaiseCountdown` | boss2.c | reused |  |
| `Boss2_State_IdleAfterRaiseFinish` | boss2.c | reused |  |
| `Boss2_State_IdleAfterRaiseWaitLoop` | boss2.c | reused |  |
| `Boss2_State_IdleBeforeAttack3` | boss2.c | reused |  |
| `Boss2_State_IdleBeforeAttack3Countdown` | boss2.c | reused |  |
| `Boss2_State_IdleBeforeAttack3Finish` | boss2.c | reused |  |
| `Boss2_State_IdleBeforeAttack3WaitLoop` | boss2.c | reused |  |
| `Boss2_State_IdleCountdown` | boss2.c | reused |  |
| `Boss2_State_IdleFinish` | boss2.c | reused |  |
| `Boss2_State_JumpAirborne` | boss2.c | reused |  |
| `Boss2_State_JumpFall` | boss2.c | reused |  |
| `Boss2_State_JumpLandShake` | boss2.c | reused |  |
| `Boss2_State_JumpRise` | boss2.c | reused |  |
| `Boss2_State_JumpStartFall` | boss2.c | reused |  |
| `Boss2_State_WaitAnimThenSelectMode1` | boss2.c | reused |  |
| `Boss2_State_WaitAnimThenSelectMode2` | boss2.c | reused |  |
| `Boss2_State_WaitForStart` | boss2.c | modified | POWCNT write dropped; colours -> boss_model_set_colors; gPlayers->unk70 -> g_boss_game.draw_player |
| `Boss2_StoreNodeMatrix` | boss2.c | substantially rewritten | NNS dictionary lookup + MTX_STORE -> joint index from bossdata.bin + boss_model_joint_world into g_boss_mtx_slot |
| `Boss2_UpdateArenaSpeed` | boss2.c | reused |  |
| `Boss2_UpdateAttack2Hitboxes` | boss2.c | modified | FX_Sqrt native; palette swap state only |
| `Boss2_UpdateAttack3Hitboxes` | boss2.c | modified | palette swap state only |
| `Boss2_UpdateBodyHitboxes` | boss2.c | modified | palette swap state only |
| `BossStage2_FreeModels` | boss2.c | substantially rewritten | boss2_unload |
| `BossStage2_OnModelLoaded` | boss2.c | substantially rewritten | boss2_load: the 11 converted .dcm, bs2.dca, bs2.nsbta frame counts; NaviBg/NaviB load not here |
| `EffRipple3D_Spawn` | boss2.c | reused | names from gEffRipple3DModelNames (bossdata.bin) |
| `EffRipple3D_SpawnForPlayer` | boss2.c | reused |  |
| `EffRipple3D_State_Main` | boss2.c | reused |  |
| `EffSplash3D_Spawn` | boss2.c | reused | names from gEffSplash3DModelNames |
| `EffSplash3D_State_Main` | boss2.c | reused |  |
| `EffWater3D_Create` | boss2.c | not translated | 3D BAC sprite (ac_eff_water_3d.bac): arena position -> g_boss_hooks.water_fx |
| `EffWater3D_State_DestroyOnAnimEnd` | boss2.c | not translated | see EffWater3D_Create |

### Boss5 (boss5.c, boss1.c)

Counts: 4 reused, 3 modified, 2 substantially rewritten, out of 9 (plus Boss1_NodeCallback_Batteries, now translated).

| original | native file | status | note |
|---|---|---|---|
| `Boss5PartD3_Action_Launch` | boss5.c | reused |  |
| `Boss5PartD3_Create` | boss5.c | modified | Task_Create -> pool of 8; archive files 0x3d, 0x41..0x45 -> converted packages / timing-only animations |
| `Boss5PartD3_Destructor` | boss5.c | modified | resource release, rect_forget |
| `Boss5PartD3_Draw` | boss5.c | modified | Model_Draw -> boss_model_pose |
| `Boss5PartD3_State_Explode` | boss5.c | modified | material / SRT / pattern animations timing only |
| `Boss5PartD3_State_Exploding` | boss5.c | reused |  |
| `Boss5PartD3_State_Fly` | boss5.c | reused |  |
| `BossStage5_FreeModels` | boss5.c | substantially rewritten | boss1_unload + boss5_unload |
| `BossStage5_OnModelLoaded` | boss5.c | substantially rewritten | boss1_load (zone 5 file list) + boss5_load |

### Boss3 (boss3.c)

| original | native file | status | note |
|---|---|---|---|
| `Boss3PartCC_Action_Defeat` | boss3.c | modified | NNS_SndPlayerStopSeq(handle) -> boss_sfx_stop |
| `Boss3PartCC_Action_Hurt` | boss3.c | modified | same |
| `Boss3PartCC_Action_Idle` | boss3.c | reused |  |
| `Boss3PartCC_Action_StartKickBall` | boss3.c | reused |  |
| `Boss3PartCC_Action_StartPushBall` | boss3.c | reused |  |
| `Boss3PartCC_Action_StartSummonBall` | boss3.c | reused |  |
| `Boss3PartCC_Action_StartThrowAttack` | boss3.c | reused |  |
| `Boss3PartCC_CanCatchBall` | boss3.c | reused | camera track objects = t[0].ox/oz |
| `Boss3PartCC_Create` | boss3.c | modified | Task_Create -> static part; MapObject_Init subset; Sound_AllocHandle -> handle 1; node callback -> Boss3PartCC_Draw |
| `Boss3PartCC_Destructor` | boss3.c | modified | resource release, rect_forget |
| `Boss3PartCC_Draw` | boss3.c | modified | Model_Draw -> boss_model_pose; MTX_RESTORE 0x1e + NNS_G3dGetCurrentMtx -> g_boss_mtx_slot -> unk228 (rounded from float) |
| `Boss3PartCC_IsOtherOnNegativeSide` | boss3.c | reused |  |
| `Boss3PartCC_MakeBallInert` | boss3.c | reused |  |
| `Boss3PartCC_NodeCallback_BodyBall` | boss3.c | substantially rewritten | "body_ball" joint (bossdata.bin) -> boss_model_joint_world into slot 0x1e after the pose |
| `Boss3PartCC_OnHit` | boss3.c | modified | owner type check -> obj_box_player; NaviMsg / Sound_PlaySeq via hooks |
| `Boss3PartCC_State_CatchBall` | boss3.c | modified | sound handle via hooks |
| `Boss3PartCC_State_CatchBallDone` | boss3.c | reused |  |
| `Boss3PartCC_State_CatchBallWait` | boss3.c | reused |  |
| `Boss3PartCC_State_Defeated` | boss3.c | reused |  |
| `Boss3PartCC_State_EndThrows` | boss3.c | reused |  |
| `Boss3PartCC_State_FacePlayerSide` | boss3.c | reused |  |
| `Boss3PartCC_State_HurtDone` | boss3.c | reused |  |
| `Boss3PartCC_State_HurtKnockback` | boss3.c | modified | palette blink -> selection state |
| `Boss3PartCC_State_Idle` | boss3.c | reused |  |
| `Boss3PartCC_State_Idle_ov000_022d8bb8` | boss3.c | reused |  |
| `Boss3PartCC_State_JumpFall` | boss3.c | reused |  |
| `Boss3PartCC_State_JumpFallStart` | boss3.c | reused |  |
| `Boss3PartCC_State_JumpLandTurn` | boss3.c | reused |  |
| `Boss3PartCC_State_JumpOverBall` | boss3.c | reused |  |
| `Boss3PartCC_State_JumpRise` | boss3.c | reused |  |
| `Boss3PartCC_State_JumpTakeoff` | boss3.c | reused |  |
| `Boss3PartCC_State_JumpToEdge` | boss3.c | reused |  |
| `Boss3PartCC_State_KickBall` | boss3.c | modified | Sound_PlaySeqArc(handle) -> boss_sfx_handle |
| `Boss3PartCC_State_KickFollowThrough` | boss3.c | reused |  |
| `Boss3PartCC_State_KickWindUp` | boss3.c | reused |  |
| `Boss3PartCC_State_KickWindUpWait` | boss3.c | reused |  |
| `Boss3PartCC_State_LandAtEdge` | boss3.c | modified | sound handle via hooks |
| `Boss3PartCC_State_LandAtEdgeShake` | boss3.c | reused |  |
| `Boss3PartCC_State_MoveToCentre` | boss3.c | reused |  |
| `Boss3PartCC_State_PickEdgeTarget` | boss3.c | modified | same; FX_Atan2 = Boss_Atan2 |
| `Boss3PartCC_State_PushBall` | boss3.c | reused |  |
| `Boss3PartCC_State_PushDone` | boss3.c | reused |  |
| `Boss3PartCC_State_PushEnd` | boss3.c | reused |  |
| `Boss3PartCC_State_PushSelectDirection` | boss3.c | reused |  |
| `Boss3PartCC_State_PushTurn` | boss3.c | reused |  |
| `Boss3PartCC_State_PushTurnWait` | boss3.c | reused |  |
| `Boss3PartCC_State_ReturnDone` | boss3.c | reused |  |
| `Boss3PartCC_State_SelectWatchOrCatch` | boss3.c | reused |  |
| `Boss3PartCC_State_SpawnBall` | boss3.c | modified | MapObject_Spawn(0xcd) -> Boss3PartCD_Create (pool) |
| `Boss3PartCC_State_Stagger` | boss3.c | reused |  |
| `Boss3PartCC_State_StartHurt` | boss3.c | reused |  |
| `Boss3PartCC_State_StartIdle` | boss3.c | reused |  |
| `Boss3PartCC_State_StartJumpToEdge` | boss3.c | reused |  |
| `Boss3PartCC_State_StartMoveToCentre` | boss3.c | modified | sound handle via hooks |
| `Boss3PartCC_State_StartPushBall` | boss3.c | reused |  |
| `Boss3PartCC_State_StartStagger` | boss3.c | reused |  |
| `Boss3PartCC_State_StartThrowAim` | boss3.c | reused |  |
| `Boss3PartCC_State_StartThrows` | boss3.c | reused |  |
| `Boss3PartCC_State_StartTurn` | boss3.c | reused |  |
| `Boss3PartCC_State_StartWaitAfterKick` | boss3.c | reused |  |
| `Boss3PartCC_State_StartWaitCatchBall` | boss3.c | reused |  |
| `Boss3PartCC_State_SummonBall` | boss3.c | modified | same |
| `Boss3PartCC_State_SummonBallDone` | boss3.c | reused |  |
| `Boss3PartCC_State_SummonBallGrow` | boss3.c | reused | same |
| `Boss3PartCC_State_ThrowAim` | boss3.c | modified | gPlayers->unk17C/unk184 -> g_boss_game.player_trans |
| `Boss3PartCC_State_ThrowAttackJump` | boss3.c | reused |  |
| `Boss3PartCC_State_ThrowAttackTurn` | boss3.c | reused |  |
| `Boss3PartCC_State_ThrowBall` | boss3.c | modified | same |
| `Boss3PartCC_State_ThrowFollowThrough` | boss3.c | modified | same |
| `Boss3PartCC_State_ThrowWindUp` | boss3.c | reused |  |
| `Boss3PartCC_State_ThrowWindUpTurn` | boss3.c | modified | same |
| `Boss3PartCC_State_Turn` | boss3.c | reused | unkE4 -> anm[0] (numFrames, frame) |
| `Boss3PartCC_State_TurnToEdgeTarget` | boss3.c | reused |  |
| `Boss3PartCC_State_TurnToSide` | boss3.c | reused |  |
| `Boss3PartCC_State_WaitAfterKick` | boss3.c | reused |  |
| `Boss3PartCC_State_WaitBallSpawned` | boss3.c | reused |  |
| `Boss3PartCC_State_WaitCatchBall` | boss3.c | reused |  |
| `Boss3PartCC_State_WaitTimer_ov000_022d7754` | boss3.c | reused |  |
| `Boss3PartCC_State_WatchBall` | boss3.c | reused |  |
| `Boss3PartCC_ThrowBall` | boss3.c | reused |  |
| `Boss3PartCD_Action_Defeat` | boss3.c | reused |  |
| `Boss3PartCD_Action_Throw` | boss3.c | reused |  |
| `Boss3PartCD_Collide` | boss3.c | modified | StageObj_HandleCollider = obj_handle_collider |
| `Boss3PartCD_Create` | boss3.c | modified | Task_Create -> pool of 8 balls (several live during the throw attack) |
| `Boss3PartCD_Destructor` | boss3.c | modified | resource release, rect_forget; clears gBoss3->unk218 if it points at it |
| `Boss3PartCD_Draw` | boss3.c | modified | Model_Draw -> boss_model_pose |
| `Boss3PartCD_OnHit` | boss3.c | modified | owner type check -> obj_box_player; Player_OnHurt / AttackBounce -> objects module |
| `Boss3PartCD_State_Defeated` | boss3.c | reused |  |
| `Boss3PartCD_State_Explode` | boss3.c | reused |  |
| `Boss3PartCD_State_Exploding` | boss3.c | reused |  |
| `Boss3PartCD_State_Fly` | boss3.c | reused | VEC_Mag as NitroSDK (CP sqrt of 4|v|^2, rounded) |
| `Boss3PartCD_State_Harmful` | boss3.c | reused |  |
| `Boss3PartCD_State_Hittable` | boss3.c | reused |  |
| `Boss3PartCD_State_Idle` | boss3.c | reused |  |
| `Boss3PartCD_State_Inert` | boss3.c | reused | the ARM code passes the object in r0 |
| `Boss3PartCD_State_StartHarmful` | boss3.c | reused |  |
| `Boss3PartCD_State_StartHittable` | boss3.c | reused |  |
| `Boss3PartCD_State_StartInert` | boss3.c | reused |  |
| `Boss3PartCD_State_ThrowStart` | boss3.c | reused |  |
| `Boss3PartCD_UpdateModeCycle` | boss3.c | reused |  |
| `Boss3_Action_Defeat` | boss3.c | modified | RingManager / StopSeqAll / timer via hooks; NULL ball skipped |
| `Boss3_Action_DefeatAll` | boss3.c | modified | NULL ball skipped |
| `Boss3_Action_Hurt` | boss3.c | reused |  |
| `Boss3_Action_HurtAll` | boss3.c | reused |  |
| `Boss3_Action_Idle` | boss3.c | reused |  |
| `Boss3_Action_WaitKickBall` | boss3.c | reused |  |
| `Boss3_Action_WaitPushBall` | boss3.c | reused |  |
| `Boss3_Action_WaitSummonBall` | boss3.c | reused |  |
| `Boss3_Action_WaitThrowAttack` | boss3.c | reused |  |
| `Boss3_ArenaDistance` | boss3.c | reused |  |
| `Boss3_Create` | boss3.c | modified | Task_Create/archives -> static part, BossStage3 resources; MapObject_Spawn(0xcc) -> Boss3PartCC_Create; effects via boss_effects_load |
| `Boss3_Destructor` | boss3.c | modified | resource release |
| `Boss3_Draw_Model` | boss3.c | modified | Model_Draw -> boss_model_pose |
| `Boss3_Draw_PlayerOnArena` | boss3.c | modified | player model transform -> g_boss_game (boss_player_model); runs at the start of boss_update |
| `Boss3_IdleAll` | boss3.c | reused |  |
| `Boss3_IsHurtDone` | boss3.c | reused |  |
| `Boss3_IsKickBallDone` | boss3.c | reused |  |
| `Boss3_IsPushBallDone` | boss3.c | reused |  |
| `Boss3_IsSummonBallDone` | boss3.c | reused |  |
| `Boss3_IsThrowAttackDone` | boss3.c | reused |  |
| `Boss3_LoadBallPalette` | boss3.c | substantially rewritten | NNS_G3dGlbMaterialColorSpecEmi / NNSi_G3dModifyMatFlag / palette -> selection state |
| `Boss3_LoadPalettes` | boss3.c | substantially rewritten | palette -> selection state |
| `Boss3_MakeBallHarmful` | boss3.c | reused |  |
| `Boss3_MakeBallHittable` | boss3.c | reused |  |
| `Boss3_ResetCamera` | boss3.c | reused | camera offsets -> named BossCamera fields |
| `Boss3_SelectNextAttack` | boss3.c | reused |  |
| `Boss3_SetBallHarmful` | boss3.c | reused |  |
| `Boss3_SetBallHittable` | boss3.c | reused |  |
| `Boss3_SetBallInert` | boss3.c | reused |  |
| `Boss3_StartKickBall` | boss3.c | reused |  |
| `Boss3_StartPushBall` | boss3.c | reused |  |
| `Boss3_StartSummonBall` | boss3.c | reused |  |
| `Boss3_StartThrowAttack` | boss3.c | reused |  |
| `Boss3_State_DefeatCamera1` | boss3.c | modified | BG2 cut-in (file 6) -> Boss_DrawExcBackground; brightness -> g_boss_game.bright |
| `Boss3_State_DefeatCamera2` | boss3.c | modified | same (file 7) |
| `Boss3_State_DefeatCamera3` | boss3.c | modified | same (file 6) |
| `Boss3_State_DefeatExplode` | boss3.c | modified | DISPCNT -> exc_bg = -1 |
| `Boss3_State_DefeatFade_ov000_022d94fc` | boss3.c | modified | gDisplayMain.unk48 -> g_boss_game.bright |
| `Boss3_State_DefeatFade_ov000_022d96d8` | boss3.c | modified | same |
| `Boss3_State_DefeatFade_ov000_022d98b0` | boss3.c | modified | same |
| `Boss3_State_DefeatFinish` | boss3.c | modified | StageShutdown_Create -> g_boss_hooks.cleared (once); ring bonus -> g_obj_stage |
| `Boss3_State_Idle` | boss3.c | reused |  |
| `Boss3_State_StartDefeat` | boss3.c | reused |  |
| `Boss3_State_WaitForStart` | boss3.c | modified | gPlayers->unk70 -> g_boss_game.draw_player; sky Model on the camera |
| `Boss3_State_WaitHurt` | boss3.c | reused |  |
| `Boss3_State_WaitKickBall` | boss3.c | reused |  |
| `Boss3_State_WaitPushBall` | boss3.c | reused |  |
| `Boss3_State_WaitSummonBall` | boss3.c | reused |  |
| `Boss3_State_WaitThrowAttack` | boss3.c | reused |  |
| `Boss3_StepAngleToward` | boss3.c | reused |  |
| `Boss3_UpdateCameraShake` | boss3.c | reused |  |
| `BossStage3_FreeModels` | boss3.c | substantially rewritten | boss3_unload |
| `BossStage3_OnModelLoaded` | boss3.c | substantially rewritten | boss3_load: 5 .dcm, bs3.dca, bs3.nsbta frame counts; NaviBg/NaviB not here |

### Boss4 (boss4.c)

| original | native file | status | note |
|---|---|---|---|
| `Boss4PartCE_Collide` | boss4.c | modified | StageObj_HandleCollider = obj_handle_collider |
| `Boss4PartCE_Create` | boss4.c | modified | Task_Create -> static part / pool |
| `Boss4PartCE_Destructor` | boss4.c | modified | resource release, rect_forget |
| `Boss4PartCE_Draw` | boss4.c | modified | Model_Draw -> boss_model_pose; BodyExit callback |
| `Boss4PartCF_Collide` | boss4.c | modified | StageObj_HandleCollider = obj_handle_collider |
| `Boss4PartCF_Create` | boss4.c | modified | Task_Create -> static part / pool |
| `Boss4PartCF_Destructor` | boss4.c | modified | resource release, rect_forget |
| `Boss4PartCF_Draw` | boss4.c | modified | Model_Draw -> boss_model_pose; ArmGun callback |
| `Boss4PartD0_Collide` | boss4.c | modified | StageObj_HandleCollider = obj_handle_collider |
| `Boss4PartD0_Create` | boss4.c | modified | Task_Create -> static part / pool; Model_SetAnimation of nsbma = timing only |
| `Boss4PartD0_Destructor` | boss4.c | modified | resource release, rect_forget |
| `Boss4PartD0_Draw` | boss4.c | modified | Model_Draw -> boss_model_pose |
| `Boss4PartD0_StartBurst` | boss4.c | reused |  |
| `Boss4PartD0_StartLaunch` | boss4.c | reused |  |
| `Boss4PartD0_State_Update` | boss4.c | reused |  |
| `Boss4PartD0_SubState_DestroyOnAnimEnd` | boss4.c | reused |  |
| `Boss4PartD0_SubState_Launch` | boss4.c | reused |  |
| `Boss4PartD1_Collide` | boss4.c | modified | StageObj_HandleCollider = obj_handle_collider |
| `Boss4PartD1_Create` | boss4.c | modified | Task_Create -> static part / pool |
| `Boss4PartD1_Destructor` | boss4.c | modified | resource release, rect_forget |
| `Boss4PartD1_Draw` | boss4.c | modified | Model_Draw -> boss_model_pose |
| `Boss4PartD1_GetPlayerDirection` | boss4.c | reused |  |
| `Boss4PartD1_OnHit` | boss4.c | modified | owner type check -> obj_box_player; Player_Action_AttackBounceVel copied into boss4.c |
| `Boss4PartD1_StartBrake` | boss4.c | reused |  |
| `Boss4PartD1_StartChase` | boss4.c | reused |  |
| `Boss4PartD1_StartFall` | boss4.c | reused |  |
| `Boss4PartD1_StartTurnToPlayer` | boss4.c | reused |  |
| `Boss4PartD1_State_Update` | boss4.c | reused |  |
| `Boss4PartD1_SubState_Brake` | boss4.c | reused |  |
| `Boss4PartD1_SubState_Chase` | boss4.c | reused |  |
| `Boss4PartD1_SubState_Fall` | boss4.c | reused |  |
| `Boss4PartD1_SubState_TurnToPlayer` | boss4.c | reused |  |
| `Boss4PartD2_Collide` | boss4.c | modified | StageObj_HandleCollider = obj_handle_collider |
| `Boss4PartD2_Create` | boss4.c | modified | Task_Create -> static part / pool; Model_SetAnimation of nsbma = timing only |
| `Boss4PartD2_Destructor` | boss4.c | modified | resource release, rect_forget |
| `Boss4PartD2_Draw` | boss4.c | modified | Model_Draw -> boss_model_pose; second Model_Draw kept as wave2 matrix |
| `Boss4PartD2_StartSpread` | boss4.c | reused |  |
| `Boss4PartD2_State_Update` | boss4.c | reused |  |
| `Boss4PartD2_SubState_Spread` | boss4.c | reused |  |
| `Boss4_Action_Clash` | boss4.c | reused |  |
| `Boss4_Action_Defeat` | boss4.c | modified | RingManager / StopSeqAll / display via hooks/state; player boxes via g_boss_hooks.player_box_flags |
| `Boss4_Action_Hurt` | boss4.c | reused |  |
| `Boss4_Action_StartSlam` | boss4.c | reused |  |
| `Boss4_Action_StartSpin` | boss4.c | reused |  |
| `Boss4_Action_StartTiltSpin` | boss4.c | reused |  |
| `Boss4_ActivateAuxModel` | boss4.c | reused |  |
| `Boss4_ArenaPosToWorld` | boss4.c | reused |  |
| `Boss4_ArenaToStagePos` | boss4.c | reused |  |
| `Boss4_CaptureNodeMatrix` | boss4.c | substantially rewritten | boss_model_joint_world + the same fx32 MTX_Scale43(1/3.3) x MTX_Concat43 |
| `Boss4_Create` | boss4.c | modified | static part; archive files -> converted packages / timing-only animations; callbacks native |
| `Boss4_DeactivateAuxModel` | boss4.c | reused |  |
| `Boss4_Destructor` | boss4.c | modified | resource release |
| `Boss4_DrawAuxModel` | boss4.c | modified | same |
| `Boss4_DrawSpinTiltModel` | boss4.c | modified | same + Rods callback |
| `Boss4_Draw_Body` | boss4.c | modified | Model_Draw -> boss_model_pose |
| `Boss4_Draw_PlayerOnArena` | boss4.c | modified | player model transform -> g_boss_game |
| `Boss4_GetHealthSpinScale` | boss4.c | reused | returns the float table entry (the decompilation types it s32) |
| `Boss4_HandleColliderWrapped` | boss4.c | modified | StageObj_HandleCollider = obj_handle_collider |
| `Boss4_IsTiltPastLimits` | boss4.c | reused |  |
| `Boss4_LoadModelPalettes1` | boss4.c | substantially rewritten | palette -> selection state |
| `Boss4_LoadModelPalettes1Clash` | boss4.c | substantially rewritten | same |
| `Boss4_LoadModelPalettes2` | boss4.c | substantially rewritten | same |
| `Boss4_LoadModelPalettes2Warn` | boss4.c | substantially rewritten | same |
| `Boss4_LoadModelPalettes3` | boss4.c | substantially rewritten | same |
| `Boss4_LoadModelPalettes4` | boss4.c | substantially rewritten | same |
| `Boss4_LoadModelPalettes5` | boss4.c | substantially rewritten | same |
| `Boss4_NodeCallback_ArmGun` | boss4.c | substantially rewritten | joint "arm_gun" |
| `Boss4_NodeCallback_BodyExit` | boss4.c | substantially rewritten | joint "body_exit" |
| `Boss4_NodeCallback_Rods` | boss4.c | substantially rewritten | joints "rod_arm"/"rod_body" (bossdata.bin) after the pose |
| `Boss4_ObjArenaPosToWorld` | boss4.c | reused |  |
| `Boss4_OnHit_Damage` | boss4.c | modified | owner type check -> obj_box_player; NaviMsg / Sound_PlaySeq via hooks |
| `Boss4_RandomBitInMask` | boss4.c | reused |  |
| `Boss4_ResetCamera` | boss4.c | reused | camera offsets -> named BossCamera fields |
| `Boss4_SetCameraDefaultDistance` | boss4.c | reused | c->unk80 written as u16 (low half of unkC8) |
| `Boss4_SetCameraZoomOut` | boss4.c | reused | same |
| `Boss4_SetSpinSpeed` | boss4.c | reused | _ffix(_fmul(_fflt(p), scale)) as C float |
| `Boss4_SpawnPartD0` | boss4.c | modified | MapObject_Spawn(0xd0) -> pool |
| `Boss4_StartAttack` | boss4.c | reused |  |
| `Boss4_StartAuxModelCycle` | boss4.c | reused |  |
| `Boss4_State_DefeatCamera1` | boss4.c | modified | gPlayers->unk17C -> g_boss_game.player_trans |
| `Boss4_State_DefeatCamera2` | boss4.c | modified | BG2 cut-in -> Boss_DrawExcBackground; brightness state |
| `Boss4_State_DefeatCamera3` | boss4.c | modified | same |
| `Boss4_State_DefeatExplode` | boss4.c | modified | ResetCallBack -> s_cb_off; DISPCNT -> exc_bg = -1 |
| `Boss4_State_DefeatFade_ov000_022dd410` | boss4.c | modified | gDisplayMain.unk48 -> g_boss_game.bright |
| `Boss4_State_DefeatFade_ov000_022dd4d0` | boss4.c | modified | same |
| `Boss4_State_DefeatFade_ov000_022dd590` | boss4.c | modified | same |
| `Boss4_State_DefeatFinish` | boss4.c | modified | StageShutdown_Create -> g_boss_hooks.cleared (once) |
| `Boss4_State_Fight` | boss4.c | reused |  |
| `Boss4_State_WaitForStart` | boss4.c | modified | from the ARM listing (src/asm_fallback); POWCNT not touched; MapObject_Spawn -> part create |
| `Boss4_StepTiltToward` | boss4.c | reused |  |
| `Boss4_SubState_ClashFlash` | boss4.c | modified | MapObject_Spawn(0xd1) -> pool |
| `Boss4_SubState_HurtFlash` | boss4.c | reused |  |
| `Boss4_SubState_SlamDown` | boss4.c | modified | MapObject_Spawn(0xd2) -> pool |
| `Boss4_SubState_SlamHold` | boss4.c | reused |  |
| `Boss4_SubState_SlamRecover` | boss4.c | reused |  |
| `Boss4_SubState_SlamShake` | boss4.c | reused |  |
| `Boss4_SubState_SlamStopSpin` | boss4.c | modified | from the ARM listing (src/asm_fallback) |
| `Boss4_SubState_SlamTiltUp` | boss4.c | reused |  |
| `Boss4_SubState_SlamWaitAnim` | boss4.c | reused |  |
| `Boss4_SubState_Spin` | boss4.c | reused |  |
| `Boss4_SubState_SpinUp` | boss4.c | reused |  |
| `Boss4_SubState_TiltSpin` | boss4.c | reused |  |
| `Boss4_UpdateAttackTimer` | boss4.c | reused |  |
| `Boss4_UpdateAuxModelCycle` | boss4.c | reused |  |
| `Boss4_WrapArenaX` | boss4.c | reused |  |
| `BossStage4_FreeModels` | boss4.c | substantially rewritten | boss4_unload |
| `BossStage4_OnModelLoaded` | boss4.c | substantially rewritten | boss4_load: 7 .dcm, bs4.dca, eff_ele_* packages, nsbma/nsbtp frame counts |

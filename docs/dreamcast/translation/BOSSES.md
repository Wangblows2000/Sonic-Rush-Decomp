# Bosses: native translation (Dreamcast port)

Status (2026-11-22):

- The shared boss framework, Boss1 (Egg Hammer, Leaf Storm act 3, stage `z13`) and Boss2 (Egg Turtle,
  Water Palace act 3, stage `z23`) are translated.
- Boss1 host test: the whole fight runs on the real z13 level with the real player module and ObjRect
  code. Measured: 16/16 checks pass with the default seed; the fight completes for 5 seeds x 2
  difficulties.
- Boss2 host test: the whole fight runs on the real z23 level (same setup). Measured: 18/18 checks pass
  for seeds 0x12345678, 1, 2, 3, 0xdeadbeef and 77, each at 6 and 8 hit points (12 runs). Every run
  visits idle, dash (+ jump), attack-2 strike, attack-3 catch (water drained and raised), hurt and
  defeat, and reaches the stage-clear request.
- `kos-cc -O2 -Wall -Wextra -Werror` builds all 6 sources.
- Not yet run on Dreamcast hardware or in an emulator. Rendering is checked only on the host, by
  projecting vertices through the boss camera.
- Boss3 (Mirage Road, 153 files, about 8.8k lines) is not started; see "Next".

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
| `tools/bossdata.py` | Converter. It writes `build/dreamcast/boss/bossdata.bin` and `assets.txt`. |
| `test/test_boss.c`, `test/test_boss2.c`, `test/stubs_objects.c` | Host tests (Boss1 on z13, Boss2 on z23). The stubs are weak definitions for object-module functions that are declared but not yet written. |
| `Makefile` | `make data`, `make host-test` (both), `make host-test1`, `make host-test2`, `make dc`. |

`bossdata.bin` ("BOSD" v1) contains the following. Nothing from the ROM is compiled into the sources.

- The ROM tables the code reads, keyed by DS address:
  - Boss1: `data_ov000_02362b58..02362dc4`;
  - Boss2: `data_ov000_02362dc4..0236306c` (rodata) and `data_ov000_023650fc..02365414` (data: palette and
    node name pointer tables, defeat positions, the hit-box callback table, `gBoss2ModelColorsAlt`);
  - `gEffSplash3DModelNames` / `gEffRipple3DModelNames` (`0x02367b38..02367b9c`);
  - zone lights `data_020748bc` (vectors) / `data_02074960` (colours) (`0x020748bc..02074a04`);
  - ScreenShake: `data_0207323c`, `data_02073240`, `data_0207324c`.
  - Pointers inside these tables keep their DS value; `ROM_STR(ptr)` resolves them through the same
    chunks. The hit-box callback table holds ROM function addresses; `Boss2_HitCallback` maps them to the
    native functions (the tool checks the table bytes).
  - The C code reads them with `BOSS_DATA(0x02362d64, ...)`, so the original symbol stays visible in
    the source.
- The node indices that the code looks up by name: `bs1_neck:neck_ham` = 10, `bs1_hammer:hammer` = 0.
  These come from the NSBMD node dictionaries (`g3d.py`).
- The Boss2 node names: `bs2_body:body0` / `body_neck` / `prop`, `bs2_neck:neck_head`, `bs2_head:head_01`.
- An effect table: for each boss stage, the converted package of each BossEffect name. There are 49
  entries, 5 of them for z13 and 10 for z23 (incl. `eff_ripple_s/m/b`, `eff_splash_s/b`). Duplicates are
  resolved through `models/manifest.json`.
- A timing-only animation table: the frame counts of animation files that are not converted
  (`bs2.nsbta`: 60, 30, 30, 240). `Model_SetAnimation` turns them into animation objects without
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

Disc assets (from `build/dreamcast/boss/assets.txt`: 14 files for z13, 31 for z23):

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
- **Zone-5 variant** (`gGameState.unk1C == 4`: battery missiles, `bs5.nsbca` file 9):
  - The code paths are translated.
  - The missile object (Boss5PartD3, map object 0xd3) is not; it goes to `g_boss_hooks.missile`.
  - `Boss1_NodeCallback_Batteries` (turret rotation) is not translated.
  - eff_launch / eff_blast5 resolve only when the stage archive has them.
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

## 6. Next

- **Boss3** (Mirage Road z33, `src/game/Zone/Mirage_Road/Boss/`, 153 files, about 8.8k lines). Arena
  constants for zone mode 2 (0xf42f8 / 0x5fe435) are already set by `boss_init`.
- Water surface / underwater handling for the player in z23 (reads `g_obj_camera[0].water`).
- In the player module: `Player_Action_KnockbackIntangible` / `Player_Action_Blank`, and the
  boss-stage player draw.
- In the models converter: palette variants for the hurt/flash palettes.

## 7. Mapping table

Status meanings:

- **reused**: the body is copied from the decompilation. Only the merged-struct field spelling
  changed, plus FX/MTX/Rand helper names.
- **modified**: same logic, with DS services (G3D, sound, NaviMsg, archives, registers) replaced.
- **substantially rewritten**: a native mechanism replaces the original (asm-only function, node
  callbacks, palettes, asset loading).
- **not translated**: see section 4.

Counts: Boss1 + framework: 199 reused, 39 modified, 7 substantially rewritten, 2 not translated, out
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
| `Boss1Neck_State_FireAtPlayer` | boss1.c | modified | Boss5PartD3 missile -> g_boss_hooks.missile |
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
| `Boss1_NodeCallback_Batteries` | - | not translated | zone-5 battery rotation (node matrix multiply); bs1_base has no battery nodes |
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

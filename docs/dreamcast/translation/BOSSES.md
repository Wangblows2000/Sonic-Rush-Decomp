# Bosses: native translation (Dreamcast port)

Status (2026-11-22):

- The shared boss framework and Boss1 (Egg Hammer, Leaf Storm act 3, stage `z13`) are translated.
- Host test: the whole fight runs on the real z13 level with the real player module and ObjRect code.
  Measured result: 16/16 checks pass with the default seed. The full fight completes (6 or 8 hits,
  then defeat, then the stage-clear request) for all 5 seeds x 2 difficulties tried.
- `kos-cc -O2 -Wall -Wextra -Werror` builds all 5 sources.
- Not yet run on Dreamcast hardware or in an emulator. Rendering is checked only on the host, by
  projecting vertices through the boss camera.
- Boss2 (Water Palace, 86 files, about 8.4k lines of decompiled C) is not started. The framework is
  ready for it; see "Next".

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
| `tools/bossdata.py` | Converter. It writes `build/dreamcast/boss/bossdata.bin` and `assets.txt`. |
| `test/test_boss.c`, `test/stubs_objects.c` | Host test. The stubs are weak definitions for object-module functions that are declared but not yet written. |
| `Makefile` | `make data`, `make host-test`, `make dc`. |

`bossdata.bin` ("BOSD" v1) contains the following. Nothing from the ROM is compiled into the sources.

- The ROM tables the code reads, keyed by DS address:
  - Boss1: `data_ov000_02362b58..02362dc4`;
  - ScreenShake: `data_0207323c`, `data_02073240`, `data_0207324c`.
  - The C code reads them with `BOSS_DATA(0x02362d64, ...)`, so the original symbol stays visible in
    the source.
- The node indices that the code looks up by name: `bs1_neck:neck_ham` = 10, `bs1_hammer:hammer` = 0.
  These come from the NSBMD node dictionaries (`g3d.py`).
- An effect table: for each boss stage, the converted package of each BossEffect name. There are 44
  entries, 5 of them for z13. Duplicates are resolved through `models/manifest.json`.

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
boss_shutdown();            /* leaving the stage */
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
6. **Order:** boss_update runs after objects_update. Its ObjRect boxes are checked at the next
   `objects_frame_begin`, as on the DS, where Stage_Main runs first.

Disc assets (from `build/dreamcast/boss/assets.txt`, about 1.1 MB for z13):

- `/cd/boss/bossdata.bin`.
- `/cd/models/` with:
  - `mod__bs1_{base,neck,hammer,sky}__*.dcm`;
  - `narc__z13_act_lz7__bs1.dca`;
  - `narc__z13_act_lz7__eff_{hit,impact_small,impact_big,blast1}` (`.dcm` + `.dca`);
  - `narc__z13_act_lz7__eff_pebble__eff_pebble01.dcm`.
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
- **The host test does not call `objects_init`.** The objects module's `objdata.bin` format is being
  changed by its owner: loader v2, current tool v1. So the test registers the player's two boxes
  itself, with the son.bac boxes of the two poses it uses: standing (-6,-16,6,16), ball body
  (-8,-8,8,8) and attack (-8,-9,8,9). It links the real `obj_rect.c`, `obj_player.c` (Player_OnHurt,
  AttackBounce) and the player module.
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

## 6. Next

- **Boss2** (Water Palace, `src/game/Zone/Water_Palace/Boss/`, 86 files, about 8.4k lines). It uses
  the same framework: BossCamera, BossModel / Model_AnimateMdl, BossEffect (the effect table already
  has the z23 packages), `boss_obj_main`, ObjRect, and `bossdata.bin` chunks for its ov000 tables.
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

Counts: 199 reused, 39 modified, 7 substantially rewritten, 2 not translated, out of 247 original
functions.

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

# Bosses: native translation (Dreamcast port)

Status: every boss is translated and integrated — Boss1 (Egg Hammer, z13) and its zone-5 variant
Boss5 (z53), Boss2 (z23), Boss3 (z33), Boss4 (z43), Boss6 (z63), Boss7 (z73), the final boss (BossF,
zf3) and the Extra Zone boss. Each was cleared in Flycast as Sonic and as Blaze; a death restarts the
fight as the original's stage rebuild does. Host tests: `make -C dreamcast/game/boss host-test`.
Integrated in the game and played in Flycast (REGRESSION.md); not run on real hardware.

Single screen: every arena renders in one 640x480 view with `boss_camera()`. The DS sub-screen
information goes to the UI through `boss_health()` / `boss_health_max()` / `boss_phase()` and
`g_boss_hooks.navi` (NaviMsg); nothing draws to a second screen.

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
- Material animations (bossdata.bin sections 5 and 6, appended after the table above; older readers
  stop before them):
  - section 5 maps each timing-table name to its converted `.dcc` / `.dcp` / `.dct`;
  - section 6 lists, per stage effect, its slot 1..3 sets (`<eff>.nsbma` / `.nsbtp` / `.nsbta`).
  - At draw time, `boss_model_matstates` finds the set of a timing resource through
    `boss_matanim_for_frames` (loaded once). The boss files that keep the timing resources need no
    change.
  - Effects load their sets in `boss_effects_load`. `BossEffect_Create` binds slots 1 / 3, and
    `BossEffect_State_Main` binds the pattern (slot 2) when the 2-frame delay ends, as on the DS.
  - `assets.txt` lists the sets (`build_disc.sh` must copy `*.dcc` / `*.dcp` / `*.dct` too).

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
g_boss_hooks.water_fx = ...;    /* EffWater3D_Create: objects_fx3d_water (obj_draw.c, 3D BAC sprite) */
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
- **Material, pattern and SRT animations** (`.nsbma` / `.nsbtp` / `.nsbta`) are converted
  (MODELS.md 4.1) and drawn: `boss_model_draw`, `boss7_model_draw`, `bossf_draw` and `ex_draw_frame`
  call `dcm_draw_ex` with `boss_model_matstates`. Objects of one kind are applied in order (cross-fade
  source first), not blended.
- **Billboard op 0x14** (`SetCameraRotBillboard`) is treated like 0x13 (`SetCameraRot`). Boss1 does
  not use either.
- **Zone-5 variant (Boss5, z53)** (`gGameState.unk1C == 4`: battery missiles, `bs5.nsbca` file 9):
  - Translated: the code paths, the missile object Boss5PartD3 (`boss5.c`, a pool of 8), and
    `Boss1_NodeCallback_Batteries` (substantially rewritten: the DS multiplies RotY into the GE matrix
    at the battery nodes; natively the rotation is applied to the joint's local transform at the pose,
    so it also turns the joint's children; not compared with a DS capture).
  - The explosion's material / SRT / pattern animations (eff_bomb_up) play from the converted sets.
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
  - `bs2.nsbta` (water / shower texture scrolling) plays from `bs2.dct`.
  - `EffWater3D_Create` (a 3D BAC sprite) goes to `g_boss_hooks.water_fx` with its arena
    position; main.c sets it to `objects_fx3d_water` (obj_draw.c), which plays animation 2
    of ac_eff_water_3d there and ends with it (EffWater3D_State_DestroyOnAnimEnd).
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
  - `bs3.nsbta` (arena SRT animation) plays from `bs3.dct`.
  - Balls are a pool of 8 static objects (the DS allocates a task per ball); task order 0x502 is kept
    by creation order.
  - `Boss3_Action_Defeat` / `Boss3_Action_DefeatAll` / `Boss3_State_DefeatFinish` skip a NULL ball
    (the DS would dereference it; it is never NULL on that path in the test runs).
  - The BossCamera "track the model translation" branch (camera flags 2/4, set by Boss3/Boss4) is now
    translated: `boss_obj_set_model` registers each part's Model (StageObj.unk124).
- **Boss4 specifics:**
  - Node callbacks (Rods / ArmGun / BodyExit) run per joint after the pose; `Boss4_CaptureNodeMatrix`
    keeps the fx32 scale step. Matrices are rounded from the float joint matrices.
  - Material / pattern animations (`bs4.nsbma`, `bs4.nsbtp`, `eff_*.nsbma`) play from the converted
    sets. `bs4_base` carries the 17 pattern textures of `bs4.nsbtp`. The D0 shot and the D2 vanish
    end on them, so their frame counts are still in bossdata.bin.
  - The float easings (SlamTiltUp, SlamRecover, D0 Launch, SetSpinSpeed, Spin) use C float, like the
    DS softfloat single precision; not compared bit for bit with the DS.
  - `Player_Action_AttackBounceVel` is copied into boss4.c (the player module lacks it).
  - D2 draws the wave model twice per frame; the second model matrix is kept in `wave2`.
- PVR list classes, culling and the camera framing were derived on the host and then checked in
  Flycast. The host debug view (`DUMPAT=<frame>`) splats projected
  vertices. It shows the neck and hammer in front of the camera with the player below them, but it
  is only a point check.

- **A death in a boss act** (Player_Respawn -> StageRestart_Create) re-creates the stage on the DS: the
  boss starts again with full health after the title card. main.c does it with boss_shutdown +
  boss_init from the g_dcp_hooks.restart chain (patch `boss/patches/boss_restart_main.patch`).

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
| `BossEffect_State_Main` | boss_effect.c | reused | .nsbtp bound after the delay (converted .dcp) |
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
| `EffWater3D_Create` | boss2.c + obj_draw.c | modified | arena position -> g_boss_hooks.water_fx = objects_fx3d_water (3D BAC sprite, ac_eff_water_3d anim 2) |
| `EffWater3D_State_DestroyOnAnimEnd` | obj_draw.c | modified | the 3D effect pool ends the effect with its animation |

### Boss5 (boss5.c, boss1.c)

Counts: 4 reused, 3 modified, 2 substantially rewritten, out of 9 (plus Boss1_NodeCallback_Batteries, now translated).

| original | native file | status | note |
|---|---|---|---|
| `Boss5PartD3_Action_Launch` | boss5.c | reused |  |
| `Boss5PartD3_Create` | boss5.c | modified | Task_Create -> pool of 8; archive files 0x3d, 0x41..0x45 -> converted packages / timing-only animations |
| `Boss5PartD3_Destructor` | boss5.c | modified | resource release, rect_forget |
| `Boss5PartD3_Draw` | boss5.c | modified | Model_Draw -> boss_model_pose |
| `Boss5PartD3_State_Explode` | boss5.c | modified | material / SRT / pattern animations: timing resources, values from the converted sets at the draw |
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

## Boss6 (Altitude Limit act 3, z63, stage id 17)

Status: all 100 functions of `src/game/Zone/Altitude_Limit/Boss/` are translated in
`dreamcast/game/boss/boss6.c` (Boss6, Boss6PartD4, the Boss6 pieces, BossStage6). None of them is an ARM
listing. Counts: 58 reused, 34 modified, 8 substantially rewritten, 0 not translated, out of 100.
Host test: 19/19 checks pass for seeds 0x12345678, 1, 2, 3, 0xdeadbeef and 77 at 6 and 8 hit points
(12 runs). Every run reaches the stage-clear request with exactly 6 (8) hits.
`kos-cc -O2 -fwrapv -Wall -Wextra -Werror` builds `boss6.c`: 30.3 KB text, 41 KB bss.

### Boss6: the fight

- **The arena.** The deck is the 2D level (floor y = 512 from x 16 to 815 px). It is drawn by
  `bs6_base` at (416, -512, 0), with two rotating cloud rings (`bs6_rcloud01/02`) and a scrolling cloud
  layer (`bs6_tcloud01/02`). The cloud layer's alpha is set by `Boss6_ScrollFadeModel` through
  NNS_G3dMdlSetMdlAlphaAll. The boss (`bs6_body`) walks on the deck. Its propeller (`bs6_prop01`, or
  `bs6_prop02` when spinning fast) is drawn twice, at the stored "body_pro_r" and "body_pro_l" node
  matrices.
- **Attacks.** After a short walk the boss picks the next attack from `unkF46`. The tables are
  `data_ov000_02363918` / `02363998` / `023639d8` by health band; mode 2/4 picks bullets with
  probability 1/2.
  1. **Bullets (phase 0x1e).** Boss6PartD4 (map object 0xd4) fires every 15 frames for 450 frames. Each
     bullet's two 2D hit boxes sit beside and above the boss, built from the inverse of the body
     rotation. A bullet that touches the player spawns piece 3 (`eff_shoot_bomb`).
  2. **Pick (0x1f).** The boss stops near the player, raises and aims the pick, and swings it into the
     deck. The swing stuns a grounded player through `Boss6_State_PlayerStunned`, a player state, and
     the impact zone (box 2) hurts. While the boss pulls the pick out, box 0 at the "body_core" node is
     the damage box. A hit there (`Boss6_OnHit_DamageFlip`) turns the boss round: `unkD5E` toggles and
     the camera goes to the other side. The PickTurnAround after an unhit pull-out does the same.
  3. **Tilt (0x20).** The boss jumps to the middle and lands on one end, and the deck tilts. Each frame
     `Boss6_ApplyTiltToPlayer` slides the player towards the low end. A/B presses (`gPad.trg`) give him
     running speed against the slide, through the player's `act_move`. Past x 8 / 824 px,
     `Player_Action_KnockbackIntangible` flings him off the deck. Plates (piece 0) and wind debris
     (piece 1) fly past. The BossHelp prompt is shown from the landing to the end of the tilt.
  4. **Edge jumps (0x21).** The boss leaps to one edge, then hops 3 times at the player. After each
     landing, box 1 at "body_core" is the damage box (`Boss6_OnHit_Damage`). Box 6 (atk 0x33) kills a
     player under the landing.

### Boss6: how it is translated

- **Merged struct.** Fields keep their offsets: `unk210[9]` models, `unkD60[11]` hit boxes,
  `unkF4A..unkF6C` walk / lean / propeller, `unk1044` / `unk1098` the stored "pelvis" / "body_core"
  matrices (`unk1068` / `unk10BC` translations), `unk10EC` node-callback state, `unk1118..unk112C`
  state work.
- **Mixed field widths.** Some states access `unk1118` / `unk111C` / `unk1124` as s16 (ldrsh/strh).
  They keep s32 storage, and those states use (s16) casts. Each state family writes the field before
  reading it, so the high half never leaks between families.
- **Node callbacks.** `bs6_body`'s SBC runs NODEDESC for joints 0..25 in index order (checked in the
  NSBMD). The callback is called once at joint 0 before the pose. A G3_MultMtx33 of the mirror there
  becomes a joint-0 rotation (`boss_model_set_joint_rot`), so the whole body turns round. The calls
  for joints 1..25 then store the posed joint world matrices into `g_boss_mtx_slot[0x1b..0x1e]` until
  the callback clears itself. Boss6_Draw reads slots 0x1b / 0x1c as NNS_G3dGetCurrentMtx did, and the
  propellers restore 0x1e / 0x1d.
- **Hit boxes.** The 11 boxes are registered directly from Boss6_Draw (ObjRect_Register), with x/y
  from the node translations. As on the DS, the boss has no StageObj collide step.

### Boss6: deviations and gaps

- **Palette swaps are state only** (`boss6_info()`: body palette, core flash palette), as for Boss1-4:
  the hurt blink, the white core flash while a damage box is armed, and the prop palettes.
- **Material / pattern / SRT animations.** The pieces' (`eff_plate`, `eff_shoot`, `eff_shoot_bomb`)
  are timing only. eff_pick's `.nsbma` / `.nsbta` are not converted (BossEffect as for the other
  bosses).
- **Cloud alpha 0.** NNS_G3dMdlSetMdlAlphaAll(0) draws as wireframe on the DS. Natively the materials
  become fully transparent (translucent list) at alpha 0. Not compared with a capture.
- **The mirror is a joint-0 rotation.** It is applied to the root joint's local transform, so it
  reaches every node, including those whose matrix the SBC restores from the stack. On the DS the
  multiplication happens after node 0's NODEDESC, so nodes restored from a slot stored before the
  multiplication could differ. bs6_body's node 0 stores nothing, so the result should be the same.
  This is derived from the SBC, not compared with a DS capture.
- **Joint matrices are float.** Node translations (core / pelvis hit-box positions, effect positions)
  are rounded from them, as for Boss1/2/4.
- **MTX_Inverse33 / MTX_MultVec33** are written after the NitroSDK fx32 code, not copied from a listing.
  MTX_Concat33 is the framework's (rounded per product).
- **The stunned player's saved state.** The DS keeps it in the boss's s32 `unk111C`, which
  PickPullOut overwrites with 8. A stun still running at that moment would crash the DS. Natively it is
  a separate pointer, and that moment is logged ("stun_overwritten"). It never happened in the runs.
  A NULL gBoss6 in `Boss6_State_PlayerStunned` falls back to the ground-idle state, where the DS would
  dereference NULL.
- **The pieces (0xa00) and bullets (0x502) are pools** of 24 and 8. A full pool returns a dummy object,
  as BossEffect does. Neither filled in the runs (at most 3 bullets alive).
- **StageObj_ViewOutCheck.** Pieces and effects have no 0x10 flag. With `g_dcp_world.objmgr & 4`, a
  piece outside the 2D view would be removed, as on the DS. The DS 2D camera in boss stages was not
  checked, same as for the other bosses' effects.
- **Player-module behaviour seen in the tests (not changed here).** After the copied
  `Player_Action_KnockbackIntangible`, `dcp_state_hurt` waits while `obj_flags & 2`, and only
  Player_InitState clears that flag. The edge knockback throws the player off the deck and he dies
  (`pf & 0x400`) and respawns, as intended by the tilt. Boss1's knockback has the same open question.
- **Not shown:** the BossHelp sprite (UI module, through the hook) and the NaviBg / NaviB HUD loads.

### Boss6: test (`test/test_boss6.c`)

```sh
make -C dreamcast/game/boss data host-test6
SEED=1 LEVEL=1 build/dreamcast/boss/test_boss6 build/dreamcast/stages/z63/level.bin \
    build/dreamcast/player build/dreamcast/models build/dreamcast/boss [-v|-vv]
DUMPAT=3100 DUMPFILE=/tmp/v.ppm build/dreamcast/boss/test_boss6 ...      # host debug view
```

The test registers the player's boxes itself, as test_boss.c does. Non-looping player animations end
(`DCP_DF_ANIM_DONE`) after their 3D animation length (anim 0xf com_dmg_b: 56 frames, 0x3c: 60, else
40). Scripted player:
- **[miss]** 900 frames standing still: no boss damage.
- **[fight]**
  - In a pick pull-out, the player is put on the floor 30 px beside the core and jumps into box 0.
  - After a hop landing, once the core has sunk (box 1 at y >= 445), the player is put 40 px beside it,
    jumps, and comes down on box 1 from above. Box 5, which hurts, lies below it.
  - Once a stun was seen, the player jumps away during the pick swing.
  - During a tilt he mashes A every 4 frames, except in the first tilt, which is left alone.
- **[defeat]** runs to the stage-clear request.

Measured (default seed, 6 hp):

```
f  240 bullets   f  700 edge jumps   [miss] 1 hurt, 0 hits   f 1481 hit 5 (landing)
f 1732 pick -> f 2006 stun   f 2796 tilt (first, not mashed): edge knockback f 3198, death f 3281
f 4606 hit 4   f 5844 hit 3 (pick, turns round)   f 7939 hit 2 (pinch music)   f 9924 hit 1
f 12828 hit 0 -> defeat (cut-ins exc_bg 6/7/6) -> f 13217 cleared (ring bonus 50000)
attacks 20 (bullets 6, pick 6, tilt 4, edge jumps 4); hits 6 (pick 2, landing 4); turns 6
hops 6, bullets 180 (max 3 alive, 13 hit the player), stuns 1, tilts 4 (360 mash presses; player x
344..709 in the mashed tilts), pieces plate/wind/shoot/bomb 12/304/180/13, help create/destroy 4/4
19 checks, 0 failed
```

All 12 seed / difficulty runs pass 19/19. They visit all phases and attacks, reach the clear with
6 (8) hits, and finish between f 11089 and f 24250.

The checks cover:
- the first, unmashed tilt (edge knockback and fall), and mashing keeping the player on the deck;
- the BossHelp hook (one create / destroy per tilt);
- bullets hitting the player;
- pinch music, the defeat hooks and the ring bonus;
- the camera and the projection of all models.

### Boss6: mapping table (boss6.c)

| original | native file | status | note |
|---|---|---|---|
| `Boss6PartD4_Create` | boss6.c | modified | Task_Create -> pool of 8; "bs6" file 8 and bs6_bult from BossStage6 |
| `Boss6PartD4_Destructor` | boss6.c | modified | Model_Release; MapObject_Destructor -> rect_forget |
| `Boss6PartD4_Draw` | boss6.c | modified | templates.py pointer code -> field names; Model_Draw -> boss_model_pose |
| `Boss6PartD4_OnHit` | boss6.c | modified | owner type check -> obj_box_player |
| `Boss6PartD4_State_Fly` | boss6.c | reused |  |
| `Boss6_Action_Defeat` | boss6.c | modified | music / ring manager via hooks; BG2 / DISPCNT / brightness -> state |
| `Boss6_Action_StartEdgeJump` | boss6.c | modified | NNS_G3dRenderObjSetCallBack -> body callback pointer |
| `Boss6_Action_StartMove` | boss6.c | modified | same |
| `Boss6_Action_StartPartD4Attack` | boss6.c | modified | same |
| `Boss6_Action_StartPickAttack` | boss6.c | modified | same |
| `Boss6_Action_StartTilt` | boss6.c | reused |  |
| `Boss6_Action_StunPlayer` | boss6.c | modified | the saved player state goes to a native pointer field (the DS keeps it in the s32 unk111C) |
| `Boss6_ApplyTiltToPlayer` | boss6.c | modified | gPad.trg -> gPlayers->pad_raw[1]; unk338 = act_move; Player_Action_KnockbackIntangible copied into boss6.c |
| `Boss6_Create` | boss6.c | modified | static object; archive files -> converted packages / timing-only animations; alpha, palettes, callback native |
| `Boss6_CreatePiece` | boss6.c | modified | Task_Create -> pool of 24; state from the ROM table data_ov000_023658ac (mapped to the native functions) |
| `Boss6_Destructor` | boss6.c | modified | resources released by boss6_unload / boss_effects_shutdown |
| `Boss6_Destructor_Piece` | boss6.c | modified | StageObj_Destructor -> Model_Release |
| `Boss6_Draw` | boss6.c | modified | Model_Draw -> boss_model_pose; MTX_RESTORE + NNS_G3dGetCurrentMtx -> g_boss_mtx_slot; float angle steps in double |
| `Boss6_EndTiltCamera` | boss6.c | modified | BossHelp_Destroy -> g_boss_hooks.help |
| `Boss6_InitHitboxes` | boss6.c | modified | native owner_kind on the 11 boxes |
| `Boss6_NodeCallback_PelvisAndPro` | boss6.c | substantially rewritten | runs per joint after the pose; G3_MultMtx33 -> joint-0 rotation before the pose |
| `Boss6_NodeCallback_Store2Nodes_ov000_022e620c` | boss6.c | substantially rewritten | same |
| `Boss6_NodeCallback_Store2Nodes_ov000_022e62fc` | boss6.c | substantially rewritten | same |
| `Boss6_NodeCallback_Store4Nodes_ov000_022e5df0` | boss6.c | substantially rewritten | same |
| `Boss6_NodeCallback_Store4Nodes_ov000_022e60a4` | boss6.c | substantially rewritten | same |
| `Boss6_OnHit_Damage` | boss6.c | modified | owner type check -> obj_box_player; NaviMsg / Sound_PlaySeq via hooks |
| `Boss6_OnHit_DamageFlip` | boss6.c | modified | same |
| `Boss6_OnHit_PushPlayer_ov000_022e08b0` | boss6.c | modified | owner type check -> obj_box_player |
| `Boss6_OnHit_PushPlayer_ov000_022e0998` | boss6.c | modified | same |
| `Boss6_OnHit_PushPlayer_ov000_022e0af8` | boss6.c | modified | same |
| `Boss6_OnHit_PushPlayer_ov000_022e0ba8` | boss6.c | modified | same |
| `Boss6_RegisterHitboxes2` | boss6.c | reused | ObjRect_Register = rect_register; palette = selection state |
| `Boss6_RegisterHitboxes3x2` | boss6.c | reused | same |
| `Boss6_RegisterHitboxes4` | boss6.c | reused | same |
| `Boss6_ResetCamera` | boss6.c | reused | data_ov000_0236a48c offsets -> named BossCamera fields (u16 stores kept) |
| `Boss6_ScrollFadeModel` | boss6.c | modified | NNS_G3dMdlSetMdlAlphaAll -> material alpha of the .dcm |
| `Boss6_SelectNextAttack` | boss6.c | reused |  |
| `Boss6_SetCameraAfterFlip` | boss6.c | reused | camera offsets -> named fields |
| `Boss6_SetCameraAngle` | boss6.c | reused | same |
| `Boss6_SetCameraEdgeJump` | boss6.c | reused | same |
| `Boss6_SetCameraFollowPlayer` | boss6.c | reused | same |
| `Boss6_SetCameraJump` | boss6.c | reused | same |
| `Boss6_SetCameraJumpStart` | boss6.c | reused | same |
| `Boss6_SetCameraPickPullOut` | boss6.c | reused | same |
| `Boss6_SetCameraPickStart` | boss6.c | reused | same |
| `Boss6_SetCameraPickSwing` | boss6.c | reused | same |
| `Boss6_SetCameraSideShot` | boss6.c | reused | same |
| `Boss6_SetDefeatCameraShot` | boss6.c | modified | Background_Init / Background_Draw of the cut-in BBG -> Boss_DrawExcBackground (6 / 7) |
| `Boss6_SpawnDebrisTimed` | boss6.c | reused |  |
| `Boss6_StartTiltCamera` | boss6.c | modified | BossHelp_Create(0) -> g_boss_hooks.help |
| `Boss6_State_AfterLand` | boss6.c | reused |  |
| `Boss6_State_Defeat` | boss6.c | modified | DISPCNT BG2 off -> exc_bg -1; gDisplayMain.unk48 -> g_boss_game.bright |
| `Boss6_State_DefeatFinish` | boss6.c | modified | StageShutdown_Create -> g_boss_hooks.cleared (once) |
| `Boss6_State_EdgeJumpLand` | boss6.c | reused |  |
| `Boss6_State_EdgeJumpLeap` | boss6.c | reused |  |
| `Boss6_State_EdgeJumpRecover` | boss6.c | reused | the camera shake writes unk40 from unk3C, as in the original |
| `Boss6_State_EdgeJumpStop` | boss6.c | reused |  |
| `Boss6_State_EndJumps` | boss6.c | reused |  |
| `Boss6_State_Hurt` | boss6.c | reused |  |
| `Boss6_State_HurtFlip` | boss6.c | reused |  |
| `Boss6_State_HurtRecover` | boss6.c | reused |  |
| `Boss6_State_HurtWaitSelectAttack` | boss6.c | reused |  |
| `Boss6_State_JumpFall` | boss6.c | reused | unk111C / unk1124 as s16 |
| `Boss6_State_JumpRise` | boss6.c | reused | same |
| `Boss6_State_LandShake` | boss6.c | reused |  |
| `Boss6_State_Move` | boss6.c | reused |  |
| `Boss6_State_PartD4Attack` | boss6.c | modified | MapObject_Spawn(0xd4) -> pool; MTX_Inverse33 / MTX_MultVec33 written after NitroSDK (fx32) |
| `Boss6_State_PickAim` | boss6.c | reused | unk1118 as s16 |
| `Boss6_State_PickApproach` | boss6.c | reused |  |
| `Boss6_State_PickImpact` | boss6.c | reused |  |
| `Boss6_State_PickPullOut` | boss6.c | reused |  |
| `Boss6_State_PickRaise` | boss6.c | reused | unk1118 as s16 |
| `Boss6_State_PickStop` | boss6.c | reused | unk1118 stored as u16 |
| `Boss6_State_PickSwing` | boss6.c | reused | unk1118 as s16 |
| `Boss6_State_PickTurnAround` | boss6.c | reused |  |
| `Boss6_State_PieceDestroyOnAnimEnd` | boss6.c | reused |  |
| `Boss6_State_PieceSlide` | boss6.c | reused | unk8 / unkC = StageObj @0x08 / @0x0C |
| `Boss6_State_PieceSpinFly` | boss6.c | reused |  |
| `Boss6_State_PieceStartSlide` | boss6.c | reused |  |
| `Boss6_State_PieceWaitSpinFly` | boss6.c | reused |  |
| `Boss6_State_PlayerStunned` | boss6.c | modified | player model "ended" bit (unk240 & 2) -> DCP_DF_ANIM_DONE |
| `Boss6_State_StartJump` | boss6.c | reused |  |
| `Boss6_State_TiltEnd` | boss6.c | modified | callback |
| `Boss6_State_TiltJump` | boss6.c | modified | callback |
| `Boss6_State_TiltLand` | boss6.c | reused | unk420 = held, unk338 = act_move |
| `Boss6_State_TiltReturn` | boss6.c | reused |  |
| `Boss6_State_TiltStart` | boss6.c | reused |  |
| `Boss6_State_TiltStop` | boss6.c | reused |  |
| `Boss6_State_TiltWaitJump` | boss6.c | reused |  |
| `Boss6_State_Tilting` | boss6.c | reused |  |
| `Boss6_State_WaitForStart` | boss6.c | modified | POWCNT screen swap dropped (single screen); sky model from BossStage6 |
| `Boss6_State_WaitSelectAttack` | boss6.c | reused |  |
| `Boss6_State_WaitSelectAttack3` | boss6.c | reused |  |
| `Boss6_StoreNodeMatrix` | boss6.c | substantially rewritten | NNS_G3dGetResDictIdxByName -> bossdata.bin joint table; MTX_STORE -> joint world matrix in g_boss_mtx_slot |
| `Boss6_Unk_ov000_022e1f10` | boss6.c | reused |  |
| `Boss6_UpdateAttackHitboxes` | boss6.c | reused |  |
| `Boss6_UpdateFlashPalettes` | boss6.c | reused | templates.py pointer code -> field names; palette = selection state |
| `Boss6_UpdateMoveSpeed` | boss6.c | reused |  |
| `BossStage6_FreeModels` | boss6.c | substantially rewritten | boss6_unload |
| `BossStage6_OnModelLoaded` | boss6.c | substantially rewritten | boss6_load: 10 .dcm, bs6.dca, piece packages, nsbma / nsbtp frame counts; NaviBg / NaviB not here |

## Boss7 (Dead Line act 3, z73, stage id 20)

The rival fight: Blaze when the story character is Sonic, Sonic when it is Blaze, then the button-mash clash.
Files: `boss7.c` (all of it), `boss7.h` (Boss7 public part), `test/test_boss7.c`; small additions to `boss_core.c`
(dispatch, `boss_data_dir()`), `boss_internal.h`, `tools/bossdata.py`, `Makefile` (`host-test7`).

Status: all 167 C functions of `src/game/Zone/Dead_Line/Boss/` plus the ARM listing
`src/asm_fallback/Boss7PartD5_InputState_ClashMash.c` (rewritten from the listing) are translated, and the stage
camera director `StageZoneScript_Zone7Boss` (`src/game/Zone/Dead_Line/Stage`). 168 originals: 122 reused, 43
modified, 3 substantially rewritten, 0 not translated. Host test: 43/43 checks for 12 seed / difficulty runs; each run
plays both story characters, a won and a lost clash. `kos-cc -O2 -fwrapv -Wall -Wextra -Werror` builds boss7.c
(39.5 KB text).

### Boss7: the fight

| object | task | map object | role |
|---|---|---|---|
| Boss7 | 0x500 | 0x39 | arena `bs7_base` (Eggman's pod for Blaze's story / Nega's pod for Sonic's: `bs7.nsbva` anim 3 / 4), director of the defeat, talk, clash and win cut-ins |
| Boss7PartD5 | 0x501 | 0xd5 | the rival: a player-like object. Input states (`unk43C`) return DS keys into its own `Pad`, move states (`unk440`) are its copy of the player physics (gPlayerPhysicsTable), map collision on |
| Boss7PartD6 | 0x502 | 0xd6 | the special's column: Sonic's tornado (pulls the player through `Boss7_State_Fight`, eff_suction) or Blaze's fire pillar; 2D box x -12..12 / -8..8, y -256..0 |
| Boss7PartD7 | 0x502 | 0xd7 | clash aura (eff_aura_son / eff_aura_blz) following the player or the rival |
| Boss7FrameCounter | 0x503 | - | `gBoss7->unk220++` |

Health is 6 / 8 (`Boss_GetStartHealth`); the player beats the rival at 1 with a jump (anim 0x30) or roll (0x31) on its
body box (box 0, atk 0x11, `Boss7PartD5_OnHit`); while it jumps (phase 0x28) or charges a spin dash (0x29) the same
contact only bounces both (`Boss7PartD5_PushAway`). At 1 the timer stops, the player's input is locked, the rival gets
up and the defeat scene puts both on their marks, the talk runs (TalkDemo 0xa / 0xb), then the clash: both mash A/B,
`unk384 += player power - rival power`, `|unk384| >= 0x4000` ends it (`Boss7_UpdateMashPower`: +(0x80 - p)/4 per
press, x0.969 per frame). Won: health 0, pause, three cut-ins (exc_bg 6/7/6, master brightness 16 -> 0), the rival
jumps off the arena -> ring bonus and `Stage_OnBossCleared`. Lost: the player is knocked back, Boss7 goes to
`Boss7_State_None` and the fight stays stopped with health 1 while the stage timer runs again (as on the DS).

gBossStatus.unk4 values: 0x24 idle, 0x25 run, 0x26 back hop, 0x27 crouch, 0x28 jump, 0x29 spin dash charge, 0x2a
roll, 0x2b air attack, 0x2c special, 0x2d hurt / exit (`boss7.h` BOSS7_PHASE_*). `boss_defeated()` = the clash
was won.

### Boss7: clash input (Dreamcast controller)

`Boss7_State_ClashStruggle` reads `gPad.trg & 3` (DS A or B, newly pressed). Natively it reads the raw pressed keys
the player module stored this frame (`DcPlayer.pad_raw[1]`, filled by `dcp_player_frame`), so the existing mapping
applies: **Dreamcast A or B** (`engine/input.c`: CONT_A -> IN_JUMP -> DS A, CONT_B -> IN_TRICK -> DS B, `main.c`
`ds_pad`). Each new press counts, holding does not; the player's input lock (pf 0x200000) does not block it, as on
the DS. The prompt sprite is `BossHelp_Create(0)` (ac_fix_bosshelp.bac animation 0) through `g_boss_hooks.help`.

### Boss7: how it is translated

- **The rival's model** is the player model of the other character (son.nsbmd / blz.nsbmd, models 0 and 1 = ball /
  spin), animated with son.nsbca / son.nsbva by the Player_SetAnimation tables (`data_ov000_02368bdc` joint anim,
  `02368c44` model, `02368cac` visibility, `02368d14` son.bac anim; index + 0x1b for Blaze from 0x30), or bs7.nsbca /
  bs7.nsbva for the Blaze specials 0x48..0x4a. Its op list `{FlushVP, Identity, RotateTranslate2Scale}` puts it 16 px
  lower (`unk58 = -0x10000`, v * S * T2 * R * T); `boss7_pose` adds t2 * R after `boss_model_pose`.
- **Visibility animations** (slot 4) are drawn: `dca_visibility` per pose, `dcm_draw(.., vis, ..)` in
  `boss7_model_draw`. `Model_SetResource` re-initialises the NNS render object, so a vis animation set before a model
  switch no longer applies until the next `Model_SetAnimation(.., 4, ..)` (kept as `vis_on`).
- **Hit boxes from son.bac**: the rival's AnimatorSprite runs the son.bac timing natively (`spr_process` semantics)
  on `boss7bac.bin` ("B7BC": 37 animations, 222 frames, 46 command-7 events, written by `bossdata.py` from
  `player_lz7.narc:act/son.bac`). `Boss7PartD5_SpriteCallback` gets the 16 command bytes as on the DS. The sprite
  never loops (Init flags 0x18) and its end sets disp 8, as does the model's slot 0.
- **Its own lights**: `Boss7PartD5_Draw` draws an after-image (an older position) while `gBoss7->unk20C & 0x10`
  with `NNS_G3dGlbLightColor` 0x6182 (Sonic) / 0x1a98 (Blaze) on lights 0..2, then restores the zone colours
  `data_02074960[zone][act]`. Natively that pose is kept with its own palette copy and drawn with a second light set
  (`s_lights_after`, zone vectors, those colours); everything else uses `g_boss_lights` = the zone lights
  (`data_020748bc` / `data_02074960`, zone 6 act 2) in view space, as Boss2 builds them.
- **Character branches**: `Boss7_Create` (bs7.nsbva anim 4 for Sonic's story, 3 for Blaze's: which pod is shown),
  `Boss7PartD5_Create` (rival character `unk3D0`), `Boss7_Action_StartTalkDemo` (0xa / 0xb),
  `Boss7_State_ClashSpawnPartD7` (the player's aura). gGameState.unk0 is `g_boss_game.unk0` when main.c set it,
  else the player's character.
- **Clash darkening**: `NNSi_G3dModifyMatFlag(.., 0, 0x40 / 0x80)` + `NNS_G3dGlbMaterialColorDiffAmb` -> bs7_base and
  bs7_sky material diffuse / ambient take the global colour (white, fading to RGB 4/4/4); restored at the rival's exit.
- **ClashMash** (rewritten from the listing): on odd frames
  `v = base*6/16 + FX_Mul((base*10/16) * FX_Mul(FX_Mul(ratio, fall + 0x4cc), (sin(f<<6)+0x1000)/2),
  (sin(unk22E + f<<5)+0x1000)/2) >> 12`, A pressed when `v >= Rand() & 0xff`; base = `data_ov000_02363b18` {0x78,
  0xa0} by level, ratio = player's lead (0..0x1000), fall = (0x708 - frames) * 0xb33 / 0x708; divisions truncate
  toward zero like the listing's sign corrections.
- Pools: one rival, 6 D6, 4 D7 (two per clash). D6 / D7 run at 0x502 in creation order, a part created this frame
  runs this frame (Task_Create appends after the same priority). The rival runs `MapObject_In_Default` (hurt blink,
  `unk1F0`) and map collision (`dcp_obj_collision`) in its StageObj_Main step.
- ROM data in bossdata.bin: `boss7_rodata` 0x02363b18..0x02363da4 and `player_tables` 0x02368b7c..0x02368d7c
  (physics + the four animation tables), the zone light chunk; timing-only frame counts of eff_tornado01..03 /
  eff_flame01..03 (.nsbma / .nsbtp) and eff_aura_*01..03.nsbma.

### Boss7: deviations and gaps

- **A DS stall is kept**: a special chosen while the rival is in the air goes through `StartSpecialAirWait` ->
  `BeginAirAttackCharge`, which does not reset `unk456`; a negative walk counter left there makes
  `HoldAirAttackCharge` (`unk456-- == 0`, s16) run about 65530 frames: the rival stands idle holding R and can be hit
  (hits do not change the input state; the defeat does). Seen in 2 of the 24 seed / difficulty / story combinations
  of the test (LEVEL=1: seed 3 Blaze's story, seed 0xdeadbeef Sonic's story), before any special; the test detects it.
- **3D BAC sprite effects play once at their spawn point**: EffBoostA_3D / EffBoostA2_3D (boost jets,
  `eff_sb_boost_a_3d`, `eff_sb_boost_a2_3d` / `eff_bb_boost_a2_3d`), EffFire01_3D(_CreateFollowing) (Blaze's
  flames, `ac_eff_fire_01_3d`, converted as `act__ac_eff_b7_3d`) and EffKemuriSpin3D (spin dash smoke,
  `ac_eff_kemuri_spin02_3d`, animation 0 or 2) go to `g_boss_hooks.fx3d` (main.c: `objects_fx3d_spawn`, boss-arena
  world coordinates, the x offset mirrored by the facing). Not kept: EffBoostA_3D_State_FollowParent,
  EffFire01_3D_State_FollowOffset's spiral and EffKemuriSpin3D_State_Main's second animation. EffTrail (Sonic's
  dash trail) is not drawn. EffFire01_3D_CreateFollowing keeps its three Rand draws (the shared seed). All are
  logged (`fx3d`) and counted; the fields the Boss7 code writes go to a dummy object.
- Material / pattern animations of D6 / D7 (eff_tornado / eff_flame / eff_aura .nsbma / .nsbtp) are timing only (frame
  counts in bossdata.bin); the D6 lifetimes and the aura cycle keep the original timing, the texture motion is not
  shown. `NNS_G3dGetTex` binds are dropped.
- `gDisplayMain` BLDCNT / BLDY (3D layer white blend) is state (`boss7_blend`), not drawn by the module.
  `NNS_SndPlayerSetVolume(SE, 0x30)` and `NNS_SndPlayerMoveVolume(BGM, 0x20, 0xb4)` are dropped (the latter logged).
  `NNS_SndPlayerStopSeq(&gBGMSoundHandle, 0x3c)` fades on the DS; natively it is `music_stop`.
- TalkDemo is only a hook; without it the clash starts right after the defeat scene.
- The player model's animation speed (`gPlayers->unk124->unk114 = 0x4000` at the clash) is reported in
  `boss7_info.player_anim_speed` while the player's animation is 0x42; the player draw in main.c must apply it.
- `Boss7_State_PlayerSkidToStop` waits for the player's animation-ended flag (disp 8), which the objects module sets
  only with `objects_set_player_anim_done(1)`; the stage clear does not depend on it (the rival's exit does it).
- Joint matrices are float (dcm runtime), as for the other bosses.

### Boss7: test (`test/test_boss7.c`)

```sh
make -C dreamcast/game/boss data host-test7
SEED=1 LEVEL=1 STORY=0 build/dreamcast/boss/test_boss7 build/dreamcast/stages/z73/level.bin \
    build/dreamcast/player build/dreamcast/models build/dreamcast/boss [-v|-vv]
DUMPAT=1500 DUMPFILE=/tmp/v.ppm ...                  # host debug view (projected vertices)
```

The level has PlayerStart 0x72 at (48,488) and Boss7 0x39 (rival spawned at (263,498)). For each story character
(STORY=2, default: both) the test runs a full fight with mashing and a second one without:

- **[miss]** 900 frames standing still: the rival attacks, it takes no damage.
- **[fight]** when the rival stands / runs / crouches with its body box armed, the player is put on the floor 36 px
  beside it and jumps at it (A held, steering); after 4 failed tries in a row the jumping player is put just above it
  ("assist", as in the Boss2/Boss4 tests).
- **[watch]** at 2 hit points the rival is left alone until it has used its special (unless the AI stalled, above).
- **[clash]** A every other frame; **[lose]** no presses.

Measured (default seed, 6 hp, Sonic's story / rival Blaze): hits at f 1075 / 1965 / 2285 / 3413 / 4636, talk 0xa at
f 4918, clash f 5118..5448 (player 158 presses, rival 38, balance +16428), won, cleared at f 5719 (ring bonus 50000).
AI: 9 table attacks, 12 spin dashes, 4 air attacks, 2 specials (12 fire pillars, 5 at once), 16 evades, 15
repositions; 17 clash bounces. Blaze's story / rival Sonic: hits at f 1113 .. 4854, talk 0xb, clash won at f 5692,
tornado pull 1025 frames, after-image 212 frames. Lost clashes end at balance -16404 / -16422 with health 1 and
Boss7_State_None.

Checks (43): init with all assets, 6/8 hp, title-card wait, the character branch (rival model), [miss], AI coverage
(spin dash / air attack / special / jump), the special's D6 (pillars, or tornado + pull + after-image), son.bac boxes,
bs7_base pod visibility (1 of 3 nodes hidden, different node per story), clash darkening and its restore, the
after-image pose (rival Sonic), hits = hp - 1, phases 0x24..0x2d, defeat scene + talk id, clash (two auras, BossHelp
0, music 0x11), both mash, the 4x player clash animation, won (exc_bg 6/7, brightness 16, 3D blend 14), cleared once
with ring bonus, models projected, and per character the lost clash. All 12 runs (seeds 0x12345678, 1, 2, 3,
0xdeadbeef, 77 x LEVEL 0 / 1) pass; in 2 of them the stall above replaces the special coverage (reported).

### Boss7: mapping table (boss7.c)

| original | native file | status | note |
|---|---|---|---|
| `Boss7FrameCounter_Main` | boss7.c | reused |  |
| `Boss7PartD5_Action_JumpOverPlayer` | boss7.c | reused |  |
| `Boss7PartD5_Action_StartAI` | boss7.c | reused |  |
| `Boss7PartD5_CheckCrouch` | boss7.c | reused |  |
| `Boss7PartD5_CheckJump` | boss7.c | reused |  |
| `Boss7PartD5_CheckRun` | boss7.c | reused |  |
| `Boss7PartD5_ChooseAttack` | boss7.c | reused |  |
| `Boss7PartD5_ChooseEvade` | boss7.c | reused |  |
| `Boss7PartD5_Collide` | boss7.c | reused | StageObj_HandleCollider = obj_handle_collider |
| `Boss7PartD5_Create` | boss7.c | modified | StageObj_LoadModel / LoadMotion / Load2dBAC -> converted player packages and the son.bac table (boss7bac.bin); gGameState.unk0 -> story character |
| `Boss7PartD5_CreateClashEffect` | boss7.c | reused | billboard ops {SetCameraRot, FlushP} |
| `Boss7PartD5_CreateHitEffect` | boss7.c | reused |  |
| `Boss7PartD5_Destructor` | boss7.c | modified | Model_Release + rect_forget |
| `Boss7PartD5_Draw` | boss7.c | modified | Model_Draw -> pose (+ Translate2 op, slot-4 visibility); the after-image pose keeps its own palette and light set (NNS_G3dGlbLightColor) |
| `Boss7PartD5_HoverBob` | boss7.c | reused |  |
| `Boss7PartD5_InputState_AirSteer` | boss7.c | reused |  |
| `Boss7PartD5_InputState_BeginAirAttackCharge` | boss7.c | modified | void in the decompilation; returns the tail-called state's keys (ARM r0) |
| `Boss7PartD5_InputState_ChargeSpindash` | boss7.c | reused |  |
| `Boss7PartD5_InputState_CheckCrouched` | boss7.c | reused |  |
| `Boss7PartD5_InputState_ClashMash` | boss7.c | substantially rewritten | from the ARM listing src/asm_fallback/Boss7PartD5_InputState_ClashMash.c (decoded: odd frames, probability v/256 from the clash balance, a 1800-frame fall-off and two sines) |
| `Boss7PartD5_InputState_CrouchTowardTarget` | boss7.c | reused |  |
| `Boss7PartD5_InputState_Decide` | boss7.c | reused |  |
| `Boss7PartD5_InputState_Defeated` | boss7.c | reused |  |
| `Boss7PartD5_InputState_HoldAirAttackCharge` | boss7.c | reused |  |
| `Boss7PartD5_InputState_HoldJump` | boss7.c | reused |  |
| `Boss7PartD5_InputState_HoldJumpThenAirAttack` | boss7.c | reused |  |
| `Boss7PartD5_InputState_None` | boss7.c | reused |  |
| `Boss7PartD5_InputState_RollThenStop` | boss7.c | reused |  |
| `Boss7PartD5_InputState_SpecialAirWait` | boss7.c | reused |  |
| `Boss7PartD5_InputState_StartAirAttack` | boss7.c | reused |  |
| `Boss7PartD5_InputState_StartBackHop` | boss7.c | modified | void; returns the tail-called state's keys (ARM r0) |
| `Boss7PartD5_InputState_StartClashMash` | boss7.c | modified | void; returns the tail-called state's keys (ARM r0) |
| `Boss7PartD5_InputState_StartJump` | boss7.c | reused |  |
| `Boss7PartD5_InputState_StartJumpThenAirAttack` | boss7.c | reused |  |
| `Boss7PartD5_InputState_StartSpecial` | boss7.c | reused |  |
| `Boss7PartD5_InputState_StartSpecialAirWait` | boss7.c | reused |  |
| `Boss7PartD5_InputState_StartSpindash` | boss7.c | reused |  |
| `Boss7PartD5_InputState_TapBack` | boss7.c | reused |  |
| `Boss7PartD5_InputState_TapBackAgain` | boss7.c | reused |  |
| `Boss7PartD5_InputState_TriggerSpecial` | boss7.c | reused |  |
| `Boss7PartD5_InputState_TurnAround` | boss7.c | reused |  |
| `Boss7PartD5_InputState_WaitAfterTurn` | boss7.c | reused |  |
| `Boss7PartD5_InputState_Walk` | boss7.c | reused |  |
| `Boss7PartD5_IsPlayerJumpAttacking` | boss7.c | reused |  |
| `Boss7PartD5_IsPlayerRollingToward` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_AirAttackCharge` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_AirDash` | boss7.c | modified | BLDY -> boss7_blend |
| `Boss7PartD5_MoveState_BackHop` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_BlazeSpecialEnd` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_BlazeSpecialFall` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_BlazeSpecialHover` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_BlazeSpecialHoverDelay` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_BlazeSpecialRise` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_BlazeSpecialSpawnPillar` | boss7.c | modified | MapObject_Spawn(0xd6) -> pool |
| `Boss7PartD5_MoveState_BlazeSpecialStartHover` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_Crouch` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_DashSkid` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_DefeatExit` | boss7.c | modified | data_022c4590.unk10 -> g_obj_stage.ring_bonus; StageShutdown_Create(Stage_OnBossCleared) -> g_boss_hooks.cleared once |
| `Boss7PartD5_MoveState_DefeatScene` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_DefeatSceneDone` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_Hurt` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_Idle` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_Jump` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_Roll` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_Run` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_SonicSpecialCharge` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_SonicSpecialDashToPlayer` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_SonicSpecialFall` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_SonicSpecialOrbit` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_SonicSpecialRise` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_SonicSpecialSpinDown` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_SonicSpecialSpinUp` | boss7.c | modified | MapObject_Spawn(0xd6) -> pool |
| `Boss7PartD5_MoveState_SonicSpecialStartCharge` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_SonicSpecialStartFall` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_SonicSpecialStartOrbit` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_SonicSpecialWaitDashAngle` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_SpindashCharge` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_SpindashRelease` | boss7.c | modified | EffTrail not drawn, EffBoostA_3D one-shot (g_boss_hooks.fx3d) |
| `Boss7PartD5_MoveState_StartAirAttackCharge` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_StartAirDash` | boss7.c | modified | BLDY -> boss7_blend, EffBoostA2_3D one-shot (g_boss_hooks.fx3d) |
| `Boss7PartD5_MoveState_StartBackHop` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_StartBlazeSpecial` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_StartCrouch` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_StartDashSkid` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_StartDefeatExit` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_StartHurt` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_StartIdle` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_StartJump` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_StartRun` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_StartSonicSpecial` | boss7.c | reused |  |
| `Boss7PartD5_MoveState_StartSpindashCharge` | boss7.c | modified | EffKemuriSpin3D not translated |
| `Boss7PartD5_MoveState_WaitDefeatScene` | boss7.c | modified | NNS_SndPlayerMoveVolume(BGM, 0x20, 0xb4) logged |
| `Boss7PartD5_OnHit` | boss7.c | modified | owner checked to be the player (obj_box_player); Player_OnHurt / Player_Action_AttackBounce -> objects module; NaviMsg / RingManager -> hooks |
| `Boss7PartD5_PushAway` | boss7.c | reused |  |
| `Boss7PartD5_SetAnimSpeedFromVelocity` | boss7.c | reused |  |
| `Boss7PartD5_SpawnFireParticle` | boss7.c | modified | EffFire01_3D_Create one-shot (g_boss_hooks.fx3d) |
| `Boss7PartD5_SpriteCallback` | boss7.c | reused | command 7 of the native son.bac table |
| `Boss7PartD5_StartSpecialAttack` | boss7.c | reused |  |
| `Boss7PartD5_State_Idle` | boss7.c | reused |  |
| `Boss7PartD5_State_RunAI` | boss7.c | reused |  |
| `Boss7PartD5_TryReposition` | boss7.c | reused |  |
| `Boss7PartD5_UpdateCircleMotion` | boss7.c | reused |  |
| `Boss7PartD5_UpdateRunAnimation` | boss7.c | reused |  |
| `Boss7PartD6_Create` | boss7.c | modified | pool slot, archive files -> converted packages / timing-only animations |
| `Boss7PartD6_Destructor` | boss7.c | modified | Sound_FreeHandle -> sfx_stop; Model_Release + rect_forget |
| `Boss7PartD6_Draw` | boss7.c | modified | Model_Draw -> pose |
| `Boss7PartD6_State_Activate` | boss7.c | reused |  |
| `Boss7PartD6_State_Active` | boss7.c | reused |  |
| `Boss7PartD6_State_Appear` | boss7.c | modified | NNS_G3dGetTex for the pattern animation dropped |
| `Boss7PartD6_State_DestroyOnAnimEnd` | boss7.c | reused |  |
| `Boss7PartD6_State_Vanish` | boss7.c | modified | NNS_SndPlayerStopSeq(handle, 0x3c) -> sfx_stop |
| `Boss7PartD6_State_WaitAnimEnd_ov000_022ec34c` | boss7.c | reused |  |
| `Boss7PartD7_Action_Appear` | boss7.c | reused |  |
| `Boss7PartD7_Create` | boss7.c | modified | pool slot, archive files -> converted packages |
| `Boss7PartD7_Destructor` | boss7.c | modified | Model_Release + rect_forget |
| `Boss7PartD7_Draw` | boss7.c | modified | Model_Draw -> pose |
| `Boss7PartD7_State_Hidden` | boss7.c | reused |  |
| `Boss7PartD7_State_Hide` | boss7.c | reused |  |
| `Boss7PartD7_State_Loop` | boss7.c | reused |  |
| `Boss7PartD7_State_StartAppear` | boss7.c | reused |  |
| `Boss7PartD7_State_StartDisappear` | boss7.c | reused |  |
| `Boss7PartD7_State_StartLoop` | boss7.c | reused |  |
| `Boss7PartD7_State_WaitAnimEnd_ov000_022ebdc4` | boss7.c | reused |  |
| `Boss7PartD7_State_WaitAnimEnd_ov000_022ebe64` | boss7.c | reused |  |
| `Boss7PartD7_State_WaitAnimEnd_ov000_022ebf08` | boss7.c | reused |  |
| `Boss7_Action_StartTalkDemo` | boss7.c | modified | TalkDemo_Create -> the talk hook |
| `Boss7_Create` | boss7.c | modified | archive files -> packages loaded by boss7_load; exc: effects -> boss_effects_load; MapObject_Spawn(0xd5) -> the static rival; Sound_AllocHandle -> handle 1 |
| `Boss7_CreateClashSpark` | boss7.c | reused | x/y/z pass through to BossEffect_CreateStrikeHit |
| `Boss7_Destructor` | boss7.c | modified | resources released by boss7_unload; gDisplaySub blend register dropped |
| `Boss7_Draw` | boss7.c | modified | Model_Draw -> pose |
| `Boss7_InitCamera` | boss7.c | modified | POWCNT LCD swap dropped; u16 writes to the low halves |
| `Boss7_SetCollidersEnabled` | boss7.c | reused |  |
| `Boss7_SetPartD7ScaleFromPower` | boss7.c | reused | unk320 = the aura's animation speed |
| `Boss7_SetRivalAnimation` | boss7.c | modified | Model_SetResource(m, m->unk138, i) -> rival model i (render object reset detaches slot 4); son.nsbca/.nsbva or bs7.nsbca/.nsbva for 0x48..0x4a |
| `Boss7_SpawnPartD7` | boss7.c | modified | MapObject_Spawn(0xd7, 0, 0, chara) -> pool |
| `Boss7_StartPartD5AI` | boss7.c | reused |  |
| `Boss7_State_ClashApproach` | boss7.c | reused |  |
| `Boss7_State_ClashEnd` | boss7.c | modified | NNS_SndPlayerStopSeqAll -> music_stop |
| `Boss7_State_ClashFadeOutBGM` | boss7.c | modified | NNS_SndPlayerStopSeq(BGM, 0x3c) -> music_stop |
| `Boss7_State_ClashFlash` | boss7.c | modified | BLDY -> boss7_blend |
| `Boss7_State_ClashLost` | boss7.c | reused |  |
| `Boss7_State_ClashLostRivalSkid` | boss7.c | reused |  |
| `Boss7_State_ClashResolve` | boss7.c | reused |  |
| `Boss7_State_ClashSpawnPartD7` | boss7.c | modified | gGameState.unk0 -> story character |
| `Boss7_State_ClashStart` | boss7.c | modified | BLDCNT/BLDY -> boss7_blend; Sound_PlaySeq(BGM, 0x11) -> music hook; BossHelp_Create -> boss_core.c; player model speed 0x4000 -> boss7_info |
| `Boss7_State_ClashStartApproach` | boss7.c | reused |  |
| `Boss7_State_ClashStartFlash` | boss7.c | modified | BLDCNT/BLDY -> boss7_blend, material flags -> colours |
| `Boss7_State_ClashStruggle` | boss7.c | modified | gPad.trg -> player->pad_raw[1] (DC A / B); BLDY -> boss7_blend; NNS_SndPlayerSetVolume(SE, 0x30) dropped |
| `Boss7_State_ClashWaitDelay` | boss7.c | reused |  |
| `Boss7_State_ClashWon` | boss7.c | reused |  |
| `Boss7_State_EndFight` | boss7.c | reused |  |
| `Boss7_State_Fight` | boss7.c | reused |  |
| `Boss7_State_None` | boss7.c | reused |  |
| `Boss7_State_PlayerSkidToStop` | boss7.c | reused | gDisplayMain.unk48 -> g_boss_game.bright |
| `Boss7_State_StartEndWait` | boss7.c | reused |  |
| `Boss7_State_StartRivalExit` | boss7.c | modified | DISPCNT BG2 off -> exc_bg = -1, material flags -> colours |
| `Boss7_State_WaitForStart` | boss7.c | modified | NNS sky resource -> BossModelRes |
| `Boss7_State_WaitTimer_ov000_022ed138` | boss7.c | reused |  |
| `Boss7_State_WaitTimer_ov000_022edc70` | boss7.c | reused |  |
| `Boss7_State_WinCut1` | boss7.c | modified | gDisplayMain.unk48 -> g_boss_game.bright |
| `Boss7_State_WinCut2` | boss7.c | modified | gDisplayMain.unk48 -> g_boss_game.bright |
| `Boss7_State_WinCut3` | boss7.c | modified | gDisplayMain.unk48 -> g_boss_game.bright |
| `Boss7_State_WinCutWait1` | boss7.c | reused |  |
| `Boss7_State_WinCutWait2` | boss7.c | reused |  |
| `Boss7_State_WinCutWait3` | boss7.c | reused |  |
| `Boss7_UpdateMashPower` | boss7.c | reused |  |
| `BossStage7_FreeModels` | boss7.c | substantially rewritten | boss7_unload |
| `BossStage7_OnModelLoaded` | boss7.c | substantially rewritten | boss7_load: bs7_base / bs7_sky (data_02074164) + the archive packages; NaviBg_Load / NaviB_Load dropped |
| `StageZoneScript_Zone7Boss` (src/game/Zone/Dead_Line/Stage, not in the 167) | boss7.c | reused | run at the end of boss7_update (task 0x3eff); gPlayers->unk17C = x << 4 |

## BossF (Final zone, zf3, stage id 23)

The final boss: a giant robot (`bsf_body`) standing behind the flying base (`bsf_base`) the player fights on. Files:
`boss_final.c` (all of it), `boss_final.h` (public part), `test/test_bossf.c`. There are also small additions to
`boss_core.c` (kind 0xf dispatch, `boss_update_pre`), `boss.h` (`g_boss_hooks.talk`, `boss_update_pre`),
`boss_internal.h`, `tools/bossdata.py` and the `Makefile` (`host-testF`).

Status: all 267 functions of `src/game/Zone/Final_Zone/Boss/` are translated, plus the three stage
functions the fight needs: `StageZoneScript_ZoneFBoss` (Stage) and `ZoneFx_ZoneFBoss_Init` / `_Update` (Background).
That is 270 originals: 210 reused, 59 modified, 1 substantially rewritten, 0 not translated, 0 asm fallbacks. Host
test: 18/18 checks in 24 runs (6 seeds x LEVEL 0/1 x Sonic / Blaze), plus a 5-check knock-off run. `kos-cc -O2
-fwrapv -Wall -Wextra -Werror` builds boss_final.c (55.6 KB text, 96 KB bss).

### BossF: the fight

| object | task | map object | role |
|---|---|---|---|
| BossF | 0x500 | 0x38 | base model, sky (`bsf_sky`, then `bsf_sky_b`), the laser-fin effects, the passage behind the arena (ZoneFx) |
| BossFPartD8 | 0xf0 | 0xd8 | the body: the attack state machine; 7 boxes on `body_core` / `body_fist_l` / `body_fist_r`; the arm's collision map `bsf_st_r.df/.di` |
| BossFPartD9 | 0x501 | 0xd9 | bomb ball thrown from the core during the hand ride (LEVEL 1 only: table 02363edc is 0 at level 0) |
| BossFPartDA | 0x501 | 0xda | shock wave rolled by the arm (`bsf_shock`) |
| BossFPartDB | 0x501 | 0xdb | slam waves (`bsf_wave_sl`) |
| BossFPartDC / DD | 0x501 | 0xdc / 0xdd | laser from the core (`eff_laser_fin`) and its floor impacts (`eff_laser_fin_b`, the harmful part) |
| BossFPartDE | 0x501 | 0xde | beam ball (`bsf_ball` + two `eff_beam_fin` ends): drops, aims, sweeps a beam between its ends; variant 1: a chain of balls dropped at the player |

Health is 6 / 8. The fight loop:
- `ChooseAttack` picks by health between the hand slam (0x30), the double press (0x31), the laser volley (0x36) and the
  beam balls (0x37).
- A slam or press leaves a fist on the floor with its damage box armed. A jump on it (anim 0x30) is phase 0x39, and
  then comes the arm throw (0x32): the body moves to x 0x5e8 and the arm lies across the arena as a collision map.
- Hitting the fist again starts the hand ride (0x33). The player runs up the arm and jumps at the core box. While
  `BossHelp_Create(1)` (the warning) is shown and the hand swings (0x34), the player must crouch (anim 3/4) or be
  knocked off. The ride ends at 0x35.
- A core hit is 0x3a (hurt), followed by ReturnHome.
- At 1 hp comes the sky charge (0x38): the timer stops, a white-out (`bossf_blend` evy 16) swaps in `bsf_sky_b`, and
  the talk runs (TalkDemo 0xf Sonic / 0x10 Blaze, music 0x13). There are three charges at the base. After
  `SkyChargeImpact3` the timer runs again and the core hangs open in front of the base. Hitting it is 0x3b: the
  timer stops, the music stops, the ring flags are set, the defeat camera and explosions play, and the stage is
  cleared with ring bonus = rings x 1000.
- If the sky window is missed, the charge destroys the base and the player falls (the layer change in
  `SkyChargeDestroyBase`), as on the DS.

`boss_final.h` lists the phases as BOSSF_PHASE_*. `boss_defeated()` is phase 0x3b.

Character, Super forms, time and rings:
- `BossFPartD8_OnHitHand` branches on the character: during the throw, the bounce sets vel_x to -0x800 (Sonic) or
  -0xb00 (Blaze).
- The final zone has no Super Sonic / Burning Blaze code. The Super forms belong to the Extra Zone boss
  (ExtraBossPlayer).
- zf3 has no ring drain.
- The time limit is the generic `Player_CheckTimeOver` (10 minutes, ui module). BossF only stops and restarts the
  stage timer (`g_dcp_world.flags & 1`) as described above.

### BossF: how it is translated

- **Work words**: BossFPartD8's 0x434 / 0x438 / 0x43C / 0x440 are read as u16 by some states and as s32 by others.
  They are unions (`unk434w` = s32, `unk434` / `unk436` = the halves), and each function uses its own file's width.
- **Node matrices** (the one substantial rewrite): `BossFPartD8_NodeCallback_Body` stores the matrices of 5 nodes
  in GE slots 0x1e..0x1a during the draw, and the Draw reads them back. Natively, the joint world matrices come from
  `boss_model_joint_world` after the pose, into the same fields (unk340..unk400, 4x3 fx32).
- **The arm's collision map**: `ObjCollisionMap_Register` adds the arm to the objects module's colmap list
  (`obj_colmap.c`). The owner the player's probes see is a proxy `Obj` kept at the part's position and rotation, so
  the player's ride pointer is that proxy.
- **Help / talk**: `BossHelp_Create` / `_Destroy` use the shared boss_core functions (help hook kinds 0/1).
  `EffEnemyRoomFix_Create` is help kind 2. `TalkDemo_Create` is the new `g_boss_hooks.talk`.
- **Draws**: the base's visibility animation (`bsf.dcv`) is applied with `dca_visibility`. The DC laser and the DE
  beam ends (DS: wireframe / alpha via NNS_G3dGlbPolygonAttr) are drawn translucent (alpha 96 / 200).
- **Pools and order**: 8 D9, 8 DA, 12 DB, 4 DC, 24 DD, 12 DE. D8 runs at 0xf0 (before the player) when
  `boss_update_pre` is called. Then BossF, the parts and ZoneFx run in `boss_update`, and the zone script runs after
  ScreenShake.
- **Player control**: the fight uses only the existing player module: knockback, attack bounce, ground idle,
  on_hurt. `Player_Action_Blank` is copied into boss_final.c (candidate for the player module).

### BossF: deviations and gaps

- Palettes (`Boss_LoadModelPalettes`) are selection state only (`bossf_palette` / `bossf_palette_flash`). The hurt
  blink is not shown unless main.c tints the model.
- Material, SRT and pattern animations (`bsf.nsbta` / `.nsbma`, `eff_laser_fin_b`, `eff_beam_fin`) are timing only.
- The DC / DE wireframe look is approximated by alpha.
- `NNS_G3dGlbPolygonAttr` and the DS display swap are dropped. `Task_DestroyByTag(4)` is a no-op.
- The NaviBg / NaviB loads in `BossStageF_OnModelLoaded` are dropped (HUD).
- BLDCNT / BLDY is state (`bossf_blend`), not drawn by the module.
- A beam ball (`BossFPartDE_State_Aim`) waits until the player is on the ground between its two ends. A player
  pressed into the left corner (x < ~21 px) is never between them, so the attack waits there, as on the DS. The test
  walks the player back to the middle.

### BossF: test (`test/test_bossf.c`)

```sh
make -C dreamcast/game/boss data host-testF
SEED=1 LEVEL=1 STORY=1 build/dreamcast/boss/test_bossf build/dreamcast/stages/zf3/level.bin \
    build/dreamcast/player build/dreamcast/models build/dreamcast/boss [-v|-vv]
NOCROUCH=1 ...     # the player stays standing on the arm: knock-off run
EXPLORE=1|2 ...    # traces of the boxes / the ride
```

The test runs the real zf3 level, player module, ObjRect and ObjCollisionMap code (`colmap_probe_for_obj` and the
per-frame commit). Its scripts:
- **[miss]** 900 frames standing still. The boss attacks and takes no damage.
- **[fist]** When a fist lies on the floor with its damage box armed, the player is put 56 px beside it and jumps at
  it.
- **[ride]** Climb the arm (jump when stuck), brake and crouch from 40 frames before the warning ends and while the
  hand swings, then jump at the core.
- **[beam]** Walk back to the middle if pressed into a corner.
- **[sky]** Stand under the open core and jump.

Measured on the default seed (6 hp, Sonic): ride hits at f 2293 / 4015 / 6921 / 10649 / 15421, talk 0xf at f 16223,
sky hit and defeat at f 17104, cleared at f 17604 (ring bonus 50000). The run had 19 fist tries, 5 core tries and 37
player hurts. Spawned: DA 19, DB 18, DC 14, DD 239, DE 25. At LEVEL 1 D9 bombs also spawn (16 in a Blaze run).

Checks (18):
- init with all assets, and the title-card wait;
- [miss];
- the full fight to the stage clear (defeat 0x3b, cleared once), and hits = max health (body hits + 1 sky hit);
- phases 0x2f..0x3b;
- help: BossHelp(1) created and destroyed, and help kind 2;
- talk id by character, music 0x13 / music_stop / ring flags, ring bonus;
- the white-out (evy 16, mode 2) and `bsf_sky_b`;
- the timer stopped and restarted in the sky phase;
- the ride, and the player drawn on the hand with a valid transform;
- the OnHitHand bounce by character (-0x800 / -0xb00);
- the parts spawned (D9 at LEVEL 1);
- the camera, and `boss_draw` vertices.

The knock-off run checks the knock-off, the ride end (0x35) without damage, and the return to idle. All 24 runs
pass: seeds 0x12345678, 1, 2, 3, 0xdeadbeef, 77 x LEVEL 0 / 1 x STORY 0 / 1, finishing between f 16461 and f 23742.

### BossF: mapping table (boss_final.c)

| original | native file | status | note |
|---|---|---|---|
| `BossFPartD8_Action_Defeat` | boss_final.c | modified | Task_DestroyByTag(4) = the parts with tag 4 (none of the translated objects use it); NNS_SndPlayerStopSeqAll -> music_stop |
| `BossFPartD8_Action_DoublePressHurt` | boss_final.c | reused |  |
| `BossFPartD8_Action_HandSlamHurt` | boss_final.c | reused |  |
| `BossFPartD8_Action_Hurt` | boss_final.c | modified | gPlayers->unk70 -> draw_player hook |
| `BossFPartD8_Action_HurtInSky` | boss_final.c | reused |  |
| `BossFPartD8_Action_PartDAThrowHurt` | boss_final.c | reused |  |
| `BossFPartD8_Action_ReturnHome` | boss_final.c | reused |  |
| `BossFPartD8_Action_StartDoublePress` | boss_final.c | reused |  |
| `BossFPartD8_Action_StartHandRide` | boss_final.c | modified | EffEnemyRoomFix_Create -> help hook |
| `BossFPartD8_Action_StartHandSlam` | boss_final.c | reused |  |
| `BossFPartD8_Action_StartPartDAThrow` | boss_final.c | reused |  |
| `BossFPartD8_Action_StartPartDCVolley` | boss_final.c | reused |  |
| `BossFPartD8_Action_StartPartDEAttack` | boss_final.c | reused |  |
| `BossFPartD8_Action_StartSkyCharge` | boss_final.c | reused |  |
| `BossFPartD8_BlinkPalette` | boss_final.c | reused |  |
| `BossFPartD8_CheckKnockOffRider` | boss_final.c | reused |  |
| `BossFPartD8_ChooseAttack` | boss_final.c | reused |  |
| `BossFPartD8_Collide` | boss_final.c | modified | ObjCollisionMap_Register -> the objects module's colmap list |
| `BossFPartD8_Create` | boss_final.c | modified | archive files from BossStageF; the collision map owner is s->cm |
| `BossFPartD8_CreateHitEffect` | boss_final.c | reused |  |
| `BossFPartD8_Destructor` | boss_final.c | modified | MapObject_Destructor + releases the colmap registration |
| `BossFPartD8_DisableHitboxes` | boss_final.c | reused |  |
| `BossFPartD8_Draw` | boss_final.c | modified | node matrices from BossFPartD8_NodeCallback_Body |
| `BossFPartD8_Draw_RidingPlayer` | boss_final.c | modified | StageObj_Draw3DEx(x, y, z, rot) -> the player model transform for boss_player_model(); main.c applies the stage model matrix, RotY(0x4000) and scale, before it |
| `BossFPartD8_GetRidePosition` | boss_final.c | reused | the point of the hand at radius unk43C from the right shoulder |
| `BossFPartD8_InitHitboxes` | boss_final.c | reused |  |
| `BossFPartD8_InitPartDCPath` | boss_final.c | reused |  |
| `BossFPartD8_IsPlayerCrouching` | boss_final.c | reused |  |
| `BossFPartD8_IsPlayerRiding` | boss_final.c | modified | the ride pointer is the collision map owner's DcpObjRef |
| `BossFPartD8_LoadModelPalettesA` | boss_final.c | modified |  |
| `BossFPartD8_LoadModelPalettesB` | boss_final.c | modified |  |
| `BossFPartD8_MoveTowardX` | boss_final.c | reused |  |
| `BossFPartD8_NodeCallback_Body` | boss_final.c | substantially rewritten | the DS stores the matrices of body_core, body_sh_l, body_sh_r, body_fist_l, body_fist_r in GE slots 0x1e..0x1a during the draw and BossFPartD8_Draw reads them back with MTX_RESTORE + NNS_G3dGetCurrentMtx; here the joint world matrices are read after the pose |
| `BossFPartD8_OnHitBody` | boss_final.c | modified | owner type -> obj_box_player |
| `BossFPartD8_OnHitHand` | boss_final.c | modified | owner type -> obj_box_player; branches on the character: during the throw the bounce pushes Blaze (unk324 1) further back than Sonic |
| `BossFPartD8_RollThrowVariant` | boss_final.c | reused |  |
| `BossFPartD8_SetAnimation` | boss_final.c | reused |  |
| `BossFPartD8_SetCameraFollowPlayer` | boss_final.c | reused |  |
| `BossFPartD8_SpawnPartD9` | boss_final.c | reused |  |
| `BossFPartD8_State_DefeatCamera1` | boss_final.c | modified | BG2 cut-in -> Boss_DrawExcBackground; brightness state |
| `BossFPartD8_State_DefeatCamera2` | boss_final.c | reused |  |
| `BossFPartD8_State_DefeatCamera3` | boss_final.c | reused |  |
| `BossFPartD8_State_DefeatExplode` | boss_final.c | modified | DISPCNT BG2 off -> exc_bg -1 |
| `BossFPartD8_State_DefeatFade_ov000_022ef4ec` | boss_final.c | reused |  |
| `BossFPartD8_State_DefeatFade_ov000_022ef5a4` | boss_final.c | reused |  |
| `BossFPartD8_State_DefeatFade_ov000_022ef664` | boss_final.c | reused |  |
| `BossFPartD8_State_DefeatFinish` | boss_final.c | modified | StageShutdown_Create(Stage_OnBossCleared) -> cleared hook, once |
| `BossFPartD8_State_DoublePressEnd` | boss_final.c | reused |  |
| `BossFPartD8_State_DoublePressEndWait` | boss_final.c | reused |  |
| `BossFPartD8_State_DoublePressHurt` | boss_final.c | reused |  |
| `BossFPartD8_State_DoublePressHurtWait` | boss_final.c | reused |  |
| `BossFPartD8_State_DoublePressImpact` | boss_final.c | modified | counters / log |
| `BossFPartD8_State_DoublePressRaise` | boss_final.c | reused |  |
| `BossFPartD8_State_DoublePressSetTrack` | boss_final.c | reused |  |
| `BossFPartD8_State_DoublePressStart` | boss_final.c | reused |  |
| `BossFPartD8_State_DoublePressStartVulnerable` | boss_final.c | reused |  |
| `BossFPartD8_State_DoublePressStrike` | boss_final.c | reused |  |
| `BossFPartD8_State_DoublePressTrack` | boss_final.c | reused |  |
| `BossFPartD8_State_DoublePressVulnerable` | boss_final.c | reused | the blink period counts @0x43E, the blink phase toggles @0x43A, as in the original |
| `BossFPartD8_State_HandRideEnd` | boss_final.c | reused |  |
| `BossFPartD8_State_HandRideEndWait` | boss_final.c | reused |  |
| `BossFPartD8_State_HandRideHold` | boss_final.c | reused |  |
| `BossFPartD8_State_HandRideMove` | boss_final.c | reused |  |
| `BossFPartD8_State_HandRideRetract` | boss_final.c | reused |  |
| `BossFPartD8_State_HandRideRetractWait` | boss_final.c | reused |  |
| `BossFPartD8_State_HandRideReturn` | boss_final.c | reused |  |
| `BossFPartD8_State_HandRideStartHold` | boss_final.c | reused |  |
| `BossFPartD8_State_HandRideStartReturn` | boss_final.c | reused |  |
| `BossFPartD8_State_HandRideStartWait` | boss_final.c | reused |  |
| `BossFPartD8_State_HandRideStartWarn` | boss_final.c | modified | BossHelp_Create -> help hook |
| `BossFPartD8_State_HandRideWait` | boss_final.c | reused |  |
| `BossFPartD8_State_HandRideWarn` | boss_final.c | reused |  |
| `BossFPartD8_State_HandSlamHurt` | boss_final.c | reused |  |
| `BossFPartD8_State_HandSlamHurtWait` | boss_final.c | reused |  |
| `BossFPartD8_State_HandSlamImpact` | boss_final.c | modified | counters / log |
| `BossFPartD8_State_HandSlamLift` | boss_final.c | reused |  |
| `BossFPartD8_State_HandSlamLiftWait` | boss_final.c | reused |  |
| `BossFPartD8_State_HandSlamNext` | boss_final.c | reused |  |
| `BossFPartD8_State_HandSlamRaise` | boss_final.c | reused |  |
| `BossFPartD8_State_HandSlamRaiseWait` | boss_final.c | reused |  |
| `BossFPartD8_State_HandSlamStartRecover` | boss_final.c | reused |  |
| `BossFPartD8_State_HandSlamStrike` | boss_final.c | reused |  |
| `BossFPartD8_State_HandSlamTrack` | boss_final.c | reused |  |
| `BossFPartD8_State_HandSlamVulnerable` | boss_final.c | reused |  |
| `BossFPartD8_State_Hurt` | boss_final.c | reused |  |
| `BossFPartD8_State_HurtInSky` | boss_final.c | reused |  |
| `BossFPartD8_State_HurtRecover` | boss_final.c | reused |  |
| `BossFPartD8_State_HurtStartRecover` | boss_final.c | reused |  |
| `BossFPartD8_State_Idle` | boss_final.c | reused |  |
| `BossFPartD8_State_LoadSkyModel` | boss_final.c | modified | the sky and passage resources stay loaded until bossf_unload; the pointers are cleared as on the DS, so ZoneFx stops drawing the passage |
| `BossFPartD8_State_PartDAThrowDelay` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDAThrowEnd` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDAThrowEndWait` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDAThrowFollowThrough` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDAThrowFollowThroughWait` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDAThrowHurt` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDAThrowHurtWait` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDAThrowMoveRight` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDAThrowMoveToPos` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDAThrowRelease` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDAThrowSetDelay` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDAThrowStart` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDAThrowStartVulnerable` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDAThrowStartWait` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDAThrowVulnerable` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDAThrowWindup` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDCVolleyDelay` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDCVolleyEnd` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDCVolleyEndWait` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDCVolleyFire` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDCVolleyInterval` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDCVolleyRaise` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDCVolleySetInterval` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDCVolleyStart` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDCVolleyStartDelay` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDEAttackDelay` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDEAttackEnd` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDEAttackEndWait` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDEAttackSpawn` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDEAttackStart` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDEAttackStartDelay` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDEAttackWait` | boss_final.c | reused |  |
| `BossFPartD8_State_PartDEAttackWindup` | boss_final.c | reused |  |
| `BossFPartD8_State_ReturnHome` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyCharge1` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyCharge2` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyCharge3` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeAftermath` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeDestroyBase` | boss_final.c | modified | BLDCNT state |
| `BossFPartD8_State_SkyChargeFadeIn` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeFadeOut` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeImpact1` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeImpact2` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeImpact3` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeRetreat` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeRetreat1` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeRetreat2` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeShake1` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeShake2` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeShake3` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeStart1` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeStart2` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeStart3` | boss_final.c | modified | the effect task pointer kept natively, see s->tackle |
| `BossFPartD8_State_SkyChargeStartFadeIn` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeStartFadeOut` | boss_final.c | modified | BLDCNT/BLDY state; NNS_SndPlayerStopSeq(BGM, 60) -> music_stop |
| `BossFPartD8_State_SkyChargeStartRetreat` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeStartRetreat1` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeStartRetreat2` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeStartWait` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeStartWait1` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeStartWait2` | boss_final.c | reused |  |
| `BossFPartD8_State_SkyChargeStartWait3` | boss_final.c | reused |  |
| `BossFPartD8_State_StartHandRideMove` | boss_final.c | modified | gPlayers->unk70 -> draw_player hook |
| `BossFPartD8_State_StartWaitTalkDemo` | boss_final.c | reused |  |
| `BossFPartD8_State_WaitNextAttack` | boss_final.c | reused |  |
| `BossFPartD8_State_WaitTalkDemo` | boss_final.c | modified | Sound_PlaySeq -> music hook; TalkDemo_Create -> talk hook |
| `BossFPartD8_State_WaitTimer_ov000_022f0280` | boss_final.c | reused |  |
| `BossFPartD8_State_WaitTimer_ov000_022f061c` | boss_final.c | reused |  |
| `BossFPartD8_State_WaitTimer_ov000_022f09b4` | boss_final.c | reused |  |
| `BossFPartD8_State_WaitTimer_ov000_022f0c8c` | boss_final.c | reused |  |
| `BossFPartD8_State_WaitTimer_ov000_022f0f04` | boss_final.c | reused |  |
| `BossFPartD8_StepFlashFade` | boss_final.c | modified | BLDY state |
| `BossFPartD8_TrackPlayerX_Far` | boss_final.c | reused |  |
| `BossFPartD8_TrackPlayerX_Fast` | boss_final.c | reused |  |
| `BossFPartD8_TrackPlayerX_Near` | boss_final.c | reused |  |
| `BossFPartD8_TrackPlayerX_NearSlow` | boss_final.c | reused |  |
| `BossFPartD8_TrackPlayerX_Offset` | boss_final.c | reused |  |
| `BossFPartD8_TrackPlayerX_Slow` | boss_final.c | reused |  |
| `BossFPartD8_TriggerDefeat` | boss_final.c | reused |  |
| `BossFPartD8_TriggerDoublePress` | boss_final.c | reused |  |
| `BossFPartD8_TriggerDoublePressHurt` | boss_final.c | reused |  |
| `BossFPartD8_TriggerHandRide` | boss_final.c | reused |  |
| `BossFPartD8_TriggerHandSlam` | boss_final.c | reused |  |
| `BossFPartD8_TriggerHandSlamHurt` | boss_final.c | reused |  |
| `BossFPartD8_TriggerHurt` | boss_final.c | reused |  |
| `BossFPartD8_TriggerHurtInSky` | boss_final.c | reused |  |
| `BossFPartD8_TriggerPartDAThrow` | boss_final.c | reused |  |
| `BossFPartD8_TriggerPartDAThrowHurt` | boss_final.c | reused |  |
| `BossFPartD8_TriggerPartDCVolley` | boss_final.c | reused |  |
| `BossFPartD8_TriggerPartDEAttack` | boss_final.c | reused |  |
| `BossFPartD8_TriggerReturnHome` | boss_final.c | reused |  |
| `BossFPartD8_TriggerSkyCharge` | boss_final.c | reused |  |
| `BossFPartD8_Unk_ov000_022f89a8` | boss_final.c | reused | the right map edge follows the player back to 0x340 |
| `BossFPartD8_Unk_ov000_022f8f78` | boss_final.c | reused |  |
| `BossFPartD8_UpdateColliderFromNode` | boss_final.c | reused |  |
| `BossFPartD9_Create` | boss_final.c | modified | resources from BossStageF |
| `BossFPartD9_Destructor` | boss_final.c | modified | releases the native models |
| `BossFPartD9_Draw` | boss_final.c | reused |  |
| `BossFPartD9_OnHit` | boss_final.c | modified | owner type -> obj_box_player |
| `BossFPartD9_State_Delay` | boss_final.c | reused |  |
| `BossFPartD9_State_Fly` | boss_final.c | reused | an arc from the core to the arm, then it rolls |
| `BossFPartD9_State_Init` | boss_final.c | reused |  |
| `BossFPartD9_State_Roll` | boss_final.c | reused | rolls while in the air or on the arm, and while the arm lies there |
| `BossFPartD9_State_StartDelay` | boss_final.c | reused |  |
| `BossFPartD9_State_StartFly` | boss_final.c | reused |  |
| `BossFPartDA_Create` | boss_final.c | modified | resources from BossStageF |
| `BossFPartDA_Destructor` | boss_final.c | modified | releases the native models |
| `BossFPartDA_Draw` | boss_final.c | reused |  |
| `BossFPartDA_State_Move` | boss_final.c | reused |  |
| `BossFPartDB_Create` | boss_final.c | modified | resources from BossStageF |
| `BossFPartDB_Destructor` | boss_final.c | reused | identical to BossFPartDA_Destructor in the original: an alias |
| `BossFPartDB_Draw` | boss_final.c | reused | identical to BossFPartDA_Draw in the original: an alias |
| `BossFPartDB_State_Move` | boss_final.c | reused |  |
| `BossFPartDC_Create` | boss_final.c | modified | resources from the zone archive; NNS_G3dGlbPolygonAttr(alpha 0x10) -> the laser's draw alpha; BLDCNT / BLDY state |
| `BossFPartDC_Destructor` | boss_final.c | modified | releases the native models |
| `BossFPartDC_Draw` | boss_final.c | modified | the stretched laser faces the camera; NNSi_G3dModifyPolygonAttrMask(alpha 0) draws it as a wireframe on the DS while it fades (unk20C & 8): drawn at a low alpha here |
| `BossFPartDC_FollowBossNode` | boss_final.c | reused |  |
| `BossFPartDC_State_ChasePlayer` | boss_final.c | modified | BLDY state |
| `BossFPartDC_State_SweepPath` | boss_final.c | modified | BLDY state |
| `BossFPartDD_Create` | boss_final.c | modified | resources from the zone archive; .nsbma / .nsbtp timing only |
| `BossFPartDD_Destructor` | boss_final.c | modified | releases the native models |
| `BossFPartDD_Draw` | boss_final.c | reused |  |
| `BossFPartDD_State_HitThenVanish` | boss_final.c | reused |  |
| `BossFPartDE_Create` | boss_final.c | modified | resources from the zone archive; a native sound handle 1..3 |
| `BossFPartDE_Destructor` | boss_final.c | modified | NNS_SndHandleReleaseSeq: the handle is released, the sound is not stopped |
| `BossFPartDE_Draw` | boss_final.c | modified | beams drawn see-through while unk4A4 (the DS draws them as wireframes: NNSi_G3dModifyPolygonAttrMask alpha 0) |
| `BossFPartDE_NarrowEnds` | boss_final.c | reused |  |
| `BossFPartDE_OnHit` | boss_final.c | modified | owner -> obj_box_player |
| `BossFPartDE_ShiftEndsTowardX` | boss_final.c | reused |  |
| `BossFPartDE_SpawnBeamFinEffects` | boss_final.c | reused |  |
| `BossFPartDE_StartChainRise` | boss_final.c | reused |  |
| `BossFPartDE_State_Activate` | boss_final.c | modified | the looping beam sound on the object's own handle |
| `BossFPartDE_State_Aim` | boss_final.c | reused |  |
| `BossFPartDE_State_ChainActivate` | boss_final.c | modified | looping beam sound on the object's own handle |
| `BossFPartDE_State_ChainDrop` | boss_final.c | modified | NNS_G3dGlbPolygonAttr -> draw alpha |
| `BossFPartDE_State_ChainFall` | boss_final.c | reused |  |
| `BossFPartDE_State_ChainLand` | boss_final.c | reused |  |
| `BossFPartDE_State_ChainRise` | boss_final.c | reused |  |
| `BossFPartDE_State_ChainSpawnNext` | boss_final.c | reused |  |
| `BossFPartDE_State_ChainSpread` | boss_final.c | modified | NNS_SndPlayerStopSeq -> boss_sfx_stop |
| `BossFPartDE_State_ChainStartWait` | boss_final.c | reused |  |
| `BossFPartDE_State_Deactivate` | boss_final.c | modified | NNS_SndPlayerStopSeq(handle, 60) -> sfx_stop |
| `BossFPartDE_State_Drop` | boss_final.c | reused |  |
| `BossFPartDE_State_Fall` | boss_final.c | reused |  |
| `BossFPartDE_State_Land` | boss_final.c | reused |  |
| `BossFPartDE_State_Leave` | boss_final.c | reused |  |
| `BossFPartDE_State_Patrol` | boss_final.c | reused |  |
| `BossFPartDE_State_Pause` | boss_final.c | reused |  |
| `BossFPartDE_State_PickTarget` | boss_final.c | reused |  |
| `BossFPartDE_State_ResizeToTarget` | boss_final.c | reused |  |
| `BossFPartDE_State_Rise` | boss_final.c | reused |  |
| `BossFPartDE_State_Start` | boss_final.c | reused |  |
| `BossFPartDE_State_StartAim` | boss_final.c | modified | NNS_G3dGlbPolygonAttr -> draw alpha |
| `BossFPartDE_State_StartPause` | boss_final.c | reused |  |
| `BossFPartDE_State_StartRise` | boss_final.c | reused |  |
| `BossFPartDE_State_StartSweep` | boss_final.c | reused | the first test is overwritten by the second, as in the original |
| `BossFPartDE_State_StartTrack` | boss_final.c | reused |  |
| `BossFPartDE_State_Sweep` | boss_final.c | reused |  |
| `BossFPartDE_State_Track` | boss_final.c | reused |  |
| `BossFPartDE_State_WaitTimer_ov000_022f5840` | boss_final.c | reused |  |
| `BossFPartDE_TrackPlayerX` | boss_final.c | reused |  |
| `BossFPartDE_UpdatePatrolSpeed` | boss_final.c | reused |  |
| `BossF_Action_StartLaserFinEffects` | boss_final.c | reused |  |
| `BossF_Create` | boss_final.c | modified | resources from BossStageF / the zone archive (bossf_load); the exc:eff_* resources of the BossEffect wrappers are loaded by boss_effects_load |
| `BossF_Destructor` | boss_final.c | modified | resources are released by bossf_unload; BLDCNT state |
| `BossF_Draw` | boss_final.c | modified | the base's visibility animation (bsf.nsbva, slot 4) is applied at the draw |
| `BossF_InitCamera` | boss_final.c | modified | POWCNT display swap dropped, single screen |
| `BossF_LoadModelPalettesAll` | boss_final.c | modified | palette selection state |
| `BossF_LoadPalette` | boss_final.c | modified |  |
| `BossF_PartD8StartWaitNextAttack` | boss_final.c | reused |  |
| `BossF_SpawnLaserFinEffect` | boss_final.c | modified | the .nsbma of eff_laser_fin_b runs as a timing-only animation, so the effect lives as long as on the DS |
| `BossF_StartNextAttack` | boss_final.c | reused |  |
| `BossF_State_SpawnLaserFinEffects` | boss_final.c | reused |  |
| `BossF_State_WaitForStart` | boss_final.c | modified | sky model from BossStageF, gPlayers->unk70 = draw_player hook |
| `BossF_UpdateCameraShake` | boss_final.c | reused |  |
| `BossStageF_FreeModels` | boss_final.c | modified | bossf_unload: frees the converted packages |
| `BossStageF_OnModelLoaded` | boss_final.c | modified | bossf_load: converted packages (.dcm / .dca / .dcv), timing-only lengths, bsf_st_r.df/.di from bossdata.bin; NaviBg_Load / NaviB_Load dropped (HUD) |
| `StageZoneScript_ZoneFBoss` (src/game/Zone/Final_Zone/Stage, not in the 267) | boss_final.c | reused | the screen shake moves the boss camera while gBossF->unk20C & 0x10 |
| `ZoneFx_ZoneFBoss_Init` (src/game/Zone/Final_Zone/Background, not in the 267) | boss_final.c | reused |  |
| `ZoneFx_ZoneFBoss_Update` (src/game/Zone/Final_Zone/Background, not in the 267) | boss_final.c | reused | the passage behind the arena: satellites, peels, clouds after 0xb40 / 0x1680 frames of stage time, dust always; models drawn twice keep both matrices |

## Extra Zone boss (stage 26, DS zone 8 act 2, overlay ov001)

The Extra Zone fight against the Egg Salamander, with Super Sonic and Burning Blaze. It is a self-contained mode:
there is no level.bin, no `boss_init`, no `dcp_player` and no objects module. ExtraBoss_Start builds everything
itself: its own player control (8-way flight, dash, Blaze's fire shot), lights, display set-up, camera, BG, HUD
and object/task system. It also sets the player slot itself.

| | |
|---|---|
| source | `src/game/Zone/Extra_Zone/Boss` (353 functions) + `src/asm_fallback/ExtraBoss_TestColliderRotatedBoxes.c`, `ExtraBoss_Unk_ov001_022fdbc0.c` |
| native | `boss_extra.h` (API), `boss_extra_int.h`, `boss_extra.c` (core, rounds, camera, lights, resources, API), `boss_extra_obj.c` (tasks, objects, hit boxes, effects and attacks), `boss_extra_player.c` (ExtraBossPlayer), `boss_extra_weapon.c` (weapon boss), `boss_extra_hand.c` (hand boss), `boss_extra_stage.c` (rings, BG, HUD, sub screen), `boss_extra_draw.c` (native renderer: PVR + host rasteriser) |
| converter | `tools/bossexdata.py` -> `build/dreamcast/boss/extra/` (`make data-ex`) |
| test | `test/test_bossex.c` (`make host-testEX`) |
| counts | 355 = 353 C + 2 asm. 353 are translated under their original names, including both asm fallbacks, which are rewritten from the listings. 2 are replaced by the converter: `ExtraBoss_GetCompressionType` and `ExtraBoss_GetUncompressedSize`, the LZ header helpers of the narc load. About 8000 lines of C. |

### Extra: the fight

- **Five rounds** (`work->unk40`). The character is `round % 2`: 0 = Super Sonic, 1 = Burning Blaze.
  - Rounds 0 and 1 start with the scrolling field (meteors, big beams, rings from the scroll tables). Their
    boss arrives when the BG scroll ends: the weapon boss in round 0, the hand boss in round 1.
  - Rounds 2 and 4 are the weapon boss; round 3 is the hand boss. They start with the boss.
  - Boss hit points come from `023167bc[level][round]`: normal {2,2,2,2,3}, hard {3,3,2,2,4}.
- **Between rounds**: the defeated boss falls and the next character flies in on the character-switch camera
  (the DS dual-screen 3D).
- **Rings and lives**: the rings drain by one a second. When they run out, or time passes 9:59 (`0x8c9f` frames),
  the player is down: fade out, -1 life, back to the Sonic round of the pair (`round / 2 * 2`).
  - Losing more lives than the character has is game over. Both characters' lives are counted.
- **Weapon boss** (Sonic):
  - Attacks: beam units (they orbit the player, then fire), homing beams, missiles, the sword, the small beam and
    the big beam.
  - How to hit it: dash (A or B) into a beam unit or a homing beam. It flies off in the dash direction; when it
    hits the core, that is a hit.
- **Hand boss** (Blaze):
  - Attacks: claws, the catch hand (it grabs the player), suction (eff_spaceb) and the bombs.
  - How to hit it: Blaze's fire shot (A), which flies right, hits the core.
- Messages: the fight has no NaviMsg. `g_boss_hooks.navi` is never called.
- Health and phase are published through `boss_health()` / `boss_phase()` (`gBossStatus`), as with the other bosses:
  - `boss_health()` = `work->unk308` while a boss is alive, otherwise 0.
  - `gBossStatus.unkA` = the start hit points.
  - `boss_phase()` takes these values (the original writes no phase):

  | phase | value | meaning |
  |---|---|---|
  | `BOSSEX_PHASE_TITLE` | 0x40 | round intro (title card) |
  | `BOSSEX_PHASE_SCROLL` | 0x41 | scrolling field, no boss yet |
  | `BOSSEX_PHASE_WEAPON` | 0x42 | weapon boss alive |
  | `BOSSEX_PHASE_HAND` | 0x43 | hand boss alive |
  | `BOSSEX_PHASE_DEFEAT` | 0x44 | defeat sequence (camera tracks the boss) |
  | `BOSSEX_PHASE_SWITCH` | 0x45 | character switch |
  | `BOSSEX_PHASE_PLAYER_DOWN` | 0x46 | player down |
  | `BOSSEX_PHASE_CLEAR` | 0x47 | cleared |

### Extra: controller

The fight reads only buttons (`gPad`). It has **no touch-screen or stylus input**.

| Dreamcast | DS | Super Sonic | Burning Blaze |
|---|---|---|---|
| D-pad / analog stick (folded into `held` by `input_poll`) | D-pad | 8-way flight | 8-way flight |
| A | A | dash (in the held direction) | fire shot |
| B | B | dash | dash |
| X | B | dash | dash |
| Y | A | dash | fire shot |
| START | START | pause request (`g_boss_extra_hooks.pause`) | same |

The table is from `ex_ds_pad`. The per-character buttons come from `data_ov001_0231da50`: Sonic dashes on A|B and
has no special; Blaze dashes on B and fires on A.

### Extra: single 640x480 view (the DS uses two screens)

- **Battle**: the DS main screen is a 256x192 window of the field through the "screen fit" camera (slot 0).
  It is drawn at 2.5x, the whole view. Layers from back to front:
  1. BG1 `zex.bbg`, with its per-line wave (HDMA HOFS, double-buffered) and palette cycling (21 palettes);
     colour 0 is the backdrop;
  2. 3D;
  3. the player's trail;
  4. OBJ: rings, then the HUD.

  The original HUD (`ac_ex_fix_moji.bac`, original art and positions) shows the ring count top left, the time
  m:ss:cc top centre (it blinks after 9:00) and the character face with lives bottom left. Ring count and time stay
  readable over the action. Sprites keep the OAM order: the first drawn is in front.
- **VS splash**: the DS sub screen shows the "SONIC / BLAZE VS EGGMAN / EGGMAN NEGA" portraits for the whole round.
  The single view shows it as a cut-in panel:
  - the sub screen at 1.25x, only its rows 40..152;
  - placed in the upper part of the view (x 160..480, y 64..204);
  - shown while the original sequence runs, with its window band, HDMA slide, name scale and the "VS" zoom / shake;
  - then held 60 frames and faded out over 16.

  The panel is half-transparent black where the portrait has colour 0. Nothing of the sub screen stays on screen
  during play.
- **Character switch**: the DS shows one panorama on both screens: slot 1, top, looking up, and slot 2, bottom,
  looking down, rendered on alternate frames. The single view draws it once with a composed camera ("slot 3"):
  the eye and target averaged, and a half-fovy of 0x2400 (about 51 degrees) that covers both screens.
  The roof of the boss's house, edge-on at eye height, is the line where the two DS screens meet. The test renders
  the DS slot 1 / 2 views next to it for comparison.
- **Brightness** (`BrightnessLerp`, the master brightness of the main screen): a full-screen white or black quad,
  alpha |b|/16.
- **Title card**: there is no title-card art in this module (UI module); see integration. Without the hook, the
  stand-in shows the fade-in only.

Frames rendered by the host test: `/tmp/bossex/ex_*.ppm`:
- title;
- round 0: field, VS open / cut-in / fade;
- weapon boss, weapon boss later;
- defeat;
- switch (early, composed, DS slot 1 / 2);
- hand boss, hand boss with suction.

### Extra: integration (main.c)

```c
#include "boss/boss_extra.h"
/* story: PlayerSelectMenu_ExitToGame (mode 0, sub 2) -> Game_StartDemoScript3AtZone8Act2
 *   -> cutscene_start(CS_SCRIPT, 3, character); on CS_NEXT_EXTRA_BOSS: */
boss_extra_set_paths("/cd/models", "/cd/boss/extra");
boss_extra_set_lives(save_lives_sonic, save_lives_blaze);  /* StoryProgress_RestoreFromSave */
boss_extra_set_difficulty(save_difficulty);                /* gGameState.unk14 */
g_boss_hooks.sfx = ...; g_boss_hooks.voice = ...; g_boss_hooks.music = ...;   /* snd/ze, see below */
g_boss_extra_hooks.pause = ...;          /* PauseMenu_Create(0): open the UI pause menu */
g_boss_extra_hooks.title_card = NULL;    /* or the UI's StageTitleCard_CreateExtraBoss, see below */
if (boss_extra_init(character) != 0) { /* data missing: back to the menu */ }
for (;;) {
    input_poll(&in);
    int r = boss_extra_run_frame(&in);           /* BOSS_EXTRA_RUNNING / _CLEARED / _FAILED */
    render_begin_frame();
    render_begin_list(PVR_LIST_OP_POLY); boss_extra_draw(PVR_LIST_OP_POLY); render_end_list();
    render_begin_list(PVR_LIST_TR_POLY); boss_extra_draw(PVR_LIST_TR_POLY); /* + ui_draw_tr() */ render_end_list();
    render_begin_list(PVR_LIST_PT_POLY); boss_extra_draw(PVR_LIST_PT_POLY); render_end_list();
    render_end_frame();
    if (r != BOSS_EXTRA_RUNNING) break;
}
BossExtraResult res;
boss_extra_result(&res);
boss_extra_shutdown();
```

- **Before the fight**: `DemoScript` mode 3 (`gDemoScriptTable` 0x40 / 0x41) ends in `SceneChange_ToExtraBoss`, which
  is `CS_NEXT_EXTRA_BOSS` in the cutscene module.
  - `ExtraBoss_Start` always begins with Super Sonic in round 0 and sets `gGameState.unk0` itself. The `character`
    argument is only remembered.
- **Cleared** (`BOSS_EXTRA_CLEARED`, the DS `ExtraBoss_SetTimeAndRings` + `DemoScript_StartAfterBoss`):
  1. Save `res.lives_sonic` / `res.lives_blaze` (`SaveData_SetLivesBoth`; the DS saves them after a game over too).
  2. Set the current lives to `res.lives_current`.
  3. Start `cutscene_start(CS_SCRIPT, 2, character)` (zone idx 8: `DemoScript_Create(2)` = entry 0x44).
  4. On `CS_NEXT_EXTRA_ACT`, run ExtraAct, the results scene with the time bonus and rank (`ov001 ExtraAct*`, 37
     functions, `src/game/Zone/Extra_Zone/Results`, not part of this module). It needs `res.time` (frames, at most
     0x8c9f) and `res.rings` (0..999) = `gExtraBossResultTime` / `gExtraBossResultRings`.
  5. After ExtraAct comes the extra ending (`CS_EXTRA_ENDING`).
- **Failed** (`BOSS_EXTRA_FAILED`): save the lives as above, then `GameOverSeq_StartGameOver` (the UI's game over).
- **Pause**:
  - START calls `g_boss_extra_hooks.pause`, and `boss_extra_run_frame` stops advancing until `boss_extra_resume()`.
  - Keep drawing while paused.
  - "Quit" (`PauseMenu_Main_Quit` with zone 8 act 2) is `boss_extra_shutdown()` (it runs `ExtraBoss_Shutdown`), then
    `SceneChange_ToPlayerSelectMenuMode0`.
- **Title card**:
  - The UI translation already handles zone 8 act 2 in `ui_titlecard.c` (brightness), but on the DS it is
    `StageTitleCard_State_StartStage` that calls `ExtraBoss_StartBattle`, and the UI has no hook for that yet.
  - Until it has one, leave `title_card` NULL; the module's 150-frame stand-in then fades in and starts the battle.
  - With a UI hook: create the title card in `title_card(ctx, character)` (`ui_stage_start(8, 2, character)`) and
    call `boss_extra_start_battle()` from its StartStage state. The title card returns at every player-down restart.
- **Sound**:
  - `boss_sfx` / `boss_voice` / music go through `g_boss_hooks` with the DS ids:
    - SE from the `snd/ze` archive (`/snd/ze/sound_data.sdat`, which ExtraBoss_LoadAssets loads into the sound heap);
    - music 0x14 for rounds 0..2, 0x15 from round 3;
    - "VS" voices 0x22 / 0x2b.
  - main.c's audio has to load that bank for this mode.
- **Lights / edge colours**: the module keeps its own `NNS_G3dGlbLight*` set (`ex_set_zone_lights`, from the arm9
  `zone_lights` table) and passes them to `dcm_draw`. It does not touch `g_boss_lights` or the stage lights.

### Extra: disc assets

`make -C dreamcast/game/boss data-ex` (after the model and sprite converters) writes `build/dreamcast/boss/extra/`.
`assets.txt` there lists 72 files, 0 missing:

- under `/cd/models`, 62 packages:
  - `bsex_md` (p_body, p_house, n_body, n_r_hand, n_house);
  - `e_son`, `e_blz`, `son_easy`, `blz_easy`;
  - `bsex_ca.dca`, `bsex_va.dcv`, `ex_ply_ca.dca`, `ex_ply_va.dcv`;
  - the 27 effect kinds of `gExtraBossFxResTable` (eff_blz_bullet / shot, meteo (+ r, brk), beam_big00/01,
    beam_unit, beam_small, clash, sword, bomb_wide, missile_hmg, beam_hmg, claw01/02, jump, catch, spaceb, suction3,
    dmg_ex, son_fly / dash, blz_fly / dash) and their .dca / .dcv.
- under `/cd/boss/extra`:
  - `bossex.bin`: the ROM tables, model / animation / joint / effect lists;
  - `zex.bin`: the BG with its 21 palettes;
  - `vs_son.bin`, `vs_blz.bin`;
  - `sprites/ex_fix_moji/`, `sprites/itm_ring/`, `sprites/ex_vs/` (`frames.bin` + `atlas0.argb1555` each).
- `/snd/ze/sound_data.sdat` (main.c).

The `dm_ex_*` models named in the task are **not used by the fight**: no ExtraBoss* function loads them. They
belong to the Extra Zone cutscenes. `eff_dmg_end01/02` in the narc are also unused here.

### Extra: how it is translated

- **DS task system, emulated** (`ex_task_*`): tasks run each frame in (id, creation) order. A task created during
  the frame runs in that frame if it sorts later. Ids:
  - 0x1000 state; 0x2f00 / 0x2f01 boss and parts; 0x2f80 trail; 0x3000 player, sub-screen draw;
  - 0x3100 sub-screen sequence; 0x3200 effects; 0x3efe title card; 0x4000 brightness;
  - 0xe000 post-update; 0xe010 draw.

  Destroyed objects stay readable until the end of the frame, as on the DS (`Task_Destroy` frees after the frame).
  Hit boxes of a released object leave the collision lists. On the first frame of a round the DS reads freed
  objects there; natively they are purged.
- **Objects**: `ExtraBossObj_Create` takes objects from a pool of 0x2e; field names keep the 0x1E0 work offsets.
  Each frame, each object's hit boxes (0x38 layout) go into the A / B lists of groups 0..2. `CheckAllCollisions`
  runs groups 1, 2, 0 and keeps the original's quirk of decrementing a's hit points in the b branch.
  `ExtraBoss_TestColliderRotatedBoxes` (asm) is rewritten from the listing: the z test, the bounding circles, the
  corner rotation and the edge intersection.
- **Node matrices**: the bosses' node callbacks store GE matrix slots 0x1b..0x1e (core, p_body, weapon_l / _r;
  l_hand0a, body_hand, hand_catch). Natively, `boss_model_joint_world` after the pose fills `g_boss_mtx_slot`, and
  `ex_get_slot_mtx` reads them back. The catch hand and the grabbed player are attached through `MOP_RESTORE`.
- **Animations**: joint (.dca, slot 0) and visibility (.dcv, slot 4) animations are played. Material, SRT and
  pattern animations are timing only (`BossAnimRes.nframes`), so the effects end on time.
- **Rand order**: in the hand / weapon wait timers, the table index comes from the first `Rand()` and `& 0x3f` from
  the second. This was checked against the disassembly.
- **HUD**: `ExtraBoss_Unk_ov001_022fdbc0` (asm, the HUD sprite set-up: 0x1b sprites, animations `02316e94`,
  palettes 1 / 2) is rewritten from the listing.
- **Renderer** (`boss_extra_draw.c`):
  - `ex_draw_model` poses the model and runs its node callback where the DS draws, then queues it. The queue is
    submitted per PVR list. Off-axis objects (flag 1) use the `G3i_FrustumW_` window of `ExtraBossObj_DrawModel`.
  - The host build rasterises the same queue in software (textures, z-buffer, BG, sprites, trail, cut-in) for the
    test frames.

### Extra: deviations and gaps

- `StageTitleCard_CreateExtraBoss` belongs to the UI. The module has the hook and a fade-only stand-in; there is no
  title-card art here (see integration).
- `ExtraBoss_LoadModelPalettes` (the boss's core flash and hurt palettes, `ex_*_0d` / `_1w` nbfp) only records the
  state: the converted textures carry fixed palettes. So the hurt / core flash colour change is not shown. The beam
  units' colour changes (`boss_model_set_colors`) are shown.
- Material, SRT and pattern animations are timing only (UV scroll of eff_suction3 / eff_blz_bullet, material fades,
  the eff_bomb_wide / eff_catch / eff_spaceb texture patterns).
- Not reproduced: edge marking (`G3X_SetEdgeColorTable`), `PaletteTransfer_Clear`, and the hardware windows outside
  the cut-in. `AnimatorSprite_DrawRotScale` keeps the scale but not the rotation (the fight draws no rotated sprite).
- The VS splash is a timed cut-in, not a permanent second screen (single-screen rule). The character switch uses
  the composed slot-3 camera instead of the two DS cameras.
- Sound: the `snd/ze` bank load and the seq handle (`work + 0x33c`, `NNS_SndPlayerStopSeq`) are main.c's
  (`boss_sfx_stop(1)`).
- Lives come from `boss_extra_set_lives` (StoryProgress_RestoreFromSave) and go out in `BossExtraResult` (saving is
  main.c's).
- `ExtraBoss_Main` (the title-card player object's callback) is empty natively.
- Host renderer lighting is approximate (emission + ambient + diffuse, no per-vertex lights); the PVR path uses the
  real lights.
- Not run on Dreamcast hardware or an emulator yet. `make dc` compiles it with `-Werror`.

### Extra: test (`test/test_bossex.c`)

```sh
make -C dreamcast/game/boss data-ex host-testEX      # 35 checks, about 40 s (5 whole fights, 2 game-over runs)
```

The scripted pad drives the fight only through `InputState`; no object is moved by the test.
- **Dodging**: it predicts every active group-0 attacker 10 frames ahead and holds the best of the 9 pad directions.
- **Super Sonic**: dashes into a beam unit / homing beam when one lies in a dash direction that points at the
  boss's core.
- **Blaze**: lines up left of the hand boss's core and fires.
- **Rings**: picks them up when it is low.

Runs:
- **[clear]** (seed 0x1234, normal): the fight runs to `BOSS_EXTRA_CLEARED`. Checks:
  - all 5 rounds, both characters, 4+ switches, weapon boss x3+ / hand boss x2+;
  - hit points {2,2,2,2,3}, 11+ boss hits, `boss_health()` steps down;
  - deflects and shots, the meteor field;
  - result time / rings in range, music 0x14 first, the 150-frame title stand-in, sound hooks, `boss_phase` 0x47;
  - a rendered weapon-boss frame.
- **[seeds]**: seeds 1, 0xdeadbeef, 77777 (normal) and 0x1234 (hard: {3,..,4} hit points) are all cleared.
  The tuning sweep over 12 seed / level pairs clears 12 / 12.
- **[fail]**: an idle pad with 0 reserve lives ends in `BOSS_EXTRA_FAILED`. All work, object manager, rings, BG,
  sub screen, flags and camera slots are freed. With 1 life each: two downs, the round restarts.
- **[pause]**: START pauses and the time stands still; resume; run / draw after shutdown is harmless; missing data
  fails `boss_extra_init` cleanly.

The whole suite is also clean under `-fsanitize=address,undefined`, rendering included.

### Extra: mapping table

Grouped by native file. Every function keeps its original name; most are `static` unless another file needs it.
"(asm)" marks the two listings that were rewritten. The two replaced functions are listed last.

- `boss_extra.c` (51): `ExtraBoss_AdvancePhase`, `ExtraBoss_AllocCharacterSlots`, `ExtraBoss_ApplyCamera`, `ExtraBoss_ApplyScreenFitCamera`, `ExtraBoss_CameraFollowPlayer`, `ExtraBoss_CameraTrackObject`, `ExtraBoss_CopyCharacterDefaults`, `ExtraBoss_Draw_Battle`, `ExtraBoss_Draw_CharacterSwitch`, `ExtraBoss_FreeCharacterSlots`, `ExtraBoss_GetCurrentCharacterSlot`, `ExtraBoss_GetWork`, `ExtraBoss_InitCharacterSlot`, `ExtraBoss_InitDisplay`, `ExtraBoss_InitPadMap`, `ExtraBoss_LoadArchiveFile26`, `ExtraBoss_LoadArchiveFile27`, `ExtraBoss_LoadArchiveFiles21_23`, `ExtraBoss_LoadArchiveFiles24_25`, `ExtraBoss_LoadAssets`, `ExtraBoss_LoadBgFiles`, `ExtraBoss_LoadBossModel`, `ExtraBoss_LoadModelPalettes`, `ExtraBoss_LoadObjResources`, `ExtraBoss_Main`, `ExtraBoss_PostUpdate_Battle`, `ExtraBoss_PostUpdate_CharacterSwitch`, `ExtraBoss_ReleaseArchiveFile26`, `ExtraBoss_ReleaseArchiveFile27`, `ExtraBoss_ReleaseArchiveFiles21_23`, `ExtraBoss_ReleaseArchiveFiles24_25`, `ExtraBoss_ReleaseAssets`, `ExtraBoss_ReleaseBgFiles`, `ExtraBoss_ReleaseModelRes`, `ExtraBoss_ReleaseObjResources`, `ExtraBoss_RemapPadBits`, `ExtraBoss_ResetCamera`, `ExtraBoss_ResetNodeStep`, `ExtraBoss_ResetRound`, `ExtraBoss_SetCameraParams`, `ExtraBoss_SetTimeAndRings`, `ExtraBoss_Shutdown`, `ExtraBoss_ShutdownDummy`, `ExtraBoss_Start`, `ExtraBoss_StartBattle`, `ExtraBoss_StartCharacterSwitch`, `ExtraBoss_StartDummy`, `ExtraBoss_State_Battle`, `ExtraBoss_State_CharacterSwitch`, `ExtraBoss_Unk_ov001_02300c54`, `ExtraBoss_UpdateCamera`

- `boss_extra_obj.c` (88): `ExtraBossObj_Create`, `ExtraBossObj_Destroy`, `ExtraBossObj_DestroyFlagged`, `ExtraBossObj_DestroyKind1`, `ExtraBossObj_DestroyKind2`, `ExtraBossObj_DestroyKind2And3`, `ExtraBossObj_DestroyPlayer`, `ExtraBossObj_DrawAll`, `ExtraBossObj_DrawModel`, `ExtraBossObj_DrawSprite`, `ExtraBossObj_Draw_Default`, `ExtraBossObj_IsAlive`, `ExtraBossObj_IsOutOfArena`, `ExtraBossObj_Main`, `ExtraBossObj_PropagateParentDestroy`, `ExtraBossObj_RegisterColliders`, `ExtraBossObj_ReleaseFxModelRes`, `ExtraBossObj_ReleaseFxModelResThunk`, `ExtraBossObj_ReleasePlayerModel`, `ExtraBossObj_State_BeamUnitActive`, `ExtraBossObj_State_BeamUnitCharge`, `ExtraBossObj_State_BeamUnitDeflected`, `ExtraBossObj_State_BeamUnitDeploy`, `ExtraBossObj_State_BeamUnitFiring`, `ExtraBossObj_State_BigBeamDelay`, `ExtraBossObj_State_BigBeamEnd`, `ExtraBossObj_State_BigBeamFire`, `ExtraBossObj_State_BigBeamStart`, `ExtraBossObj_State_Blink`, `ExtraBossObj_State_Blink2`, `ExtraBossObj_State_DestroyAfterTimer`, `ExtraBossObj_State_DestroyWhenAnimsDone`, `ExtraBossObj_State_DestroyWhenOutOfArena`, `ExtraBossObj_State_HomingBeamAim`, `ExtraBossObj_State_HomingBeamApproach`, `ExtraBossObj_State_HomingBeamFire`, `ExtraBossObj_State_MissileHoming`, `ExtraBossObj_State_MissileLaunch`, `ExtraBossObj_State_None`, `ExtraBossObj_State_SmallBeam`, `ExtraBossObj_State_SpacebActive`, `ExtraBossObj_State_SpacebEnd`, `ExtraBossObj_State_SpacebStart`, `ExtraBossObj_State_SuctionFx`, `ExtraBossObj_State_SwordFx`, `ExtraBossObj_UpdateMotionAll`, `ExtraBossPlayer_CreateFxObj`, `ExtraBossPlayer_SetupFxModel`, `ExtraBossPlayer_SpawnBlzShotFx`, `ExtraBossPlayer_SpawnClashFx`, `ExtraBossPlayer_SpawnSpecialShot`, `ExtraBoss_AdvanceSwordFxStep`, `ExtraBoss_AllocFlagBlock`, `ExtraBoss_BeamUnit_OnContact`, `ExtraBoss_BeamUnit_OnHit`, `ExtraBoss_CancelSwordFx`, `ExtraBoss_CheckAllCollisions`, `ExtraBoss_CheckCollisionGroup`, `ExtraBoss_ClearColliderLists`, `ExtraBoss_CopyParentPosition`, `ExtraBoss_CountActiveBeamUnits`, `ExtraBoss_FreeFlagBlock`, `ExtraBoss_HomingBeam_OnHit`, `ExtraBoss_InitCollider`, `ExtraBoss_Meteor_OnHit`, `ExtraBoss_ResetFxFlags`, `ExtraBoss_SetObjModelAnims`, `ExtraBoss_SnapObjToCameraX`, `ExtraBoss_SpawnBeamUnit`, `ExtraBoss_SpawnBigBeam`, `ExtraBoss_SpawnBombFx`, `ExtraBoss_SpawnCatchFx`, `ExtraBoss_SpawnClawFx`, `ExtraBoss_SpawnDamageFx`, `ExtraBoss_SpawnHomingBeam`, `ExtraBoss_SpawnHomingMissile`, `ExtraBoss_SpawnJumpFx`, `ExtraBoss_SpawnMeteor`, `ExtraBoss_SpawnMeteorOfKind`, `ExtraBoss_SpawnMeteorR`, `ExtraBoss_SpawnSmallBeam`, `ExtraBoss_SpawnSpacebFx`, `ExtraBoss_SpawnSuctionFx`, `ExtraBoss_SpawnSwordFx`, `ExtraBoss_SwapObjModel`, `ExtraBoss_TestColliderBoxes`, `ExtraBoss_TestColliderOverlap`, `ExtraBoss_TestColliderRotatedBoxes (asm)`

- `boss_extra_player.c` (42): `ExtraBossObj_Draw_PlayerTrail`, `ExtraBossObj_State_PlayerDashFx`, `ExtraBossObj_State_PlayerDashFxEnd`, `ExtraBossObj_State_PlayerFlyFx`, `ExtraBossObj_State_PlayerFxHidden`, `ExtraBossObj_State_PlayerTrailFull`, `ExtraBossObj_State_PlayerTrailGrow`, `ExtraBossObj_State_PlayerTrailHidden`, `ExtraBossObj_State_PlayerTrailShrink`, `ExtraBossPlayerManager_Create`, `ExtraBossPlayerManager_Destroy`, `ExtraBossPlayer_Action_Dash`, `ExtraBossPlayer_Action_Defeat`, `ExtraBossPlayer_Action_Hurt`, `ExtraBossPlayer_Action_LockControl`, `ExtraBossPlayer_Action_UnlockControl`, `ExtraBossPlayer_ApplyMoveInput`, `ExtraBossPlayer_ApplyMoveInputHalf`, `ExtraBossPlayer_ChangeState`, `ExtraBossPlayer_CheckActionInput`, `ExtraBossPlayer_OnDashHit`, `ExtraBossPlayer_State_Dash`, `ExtraBossPlayer_State_Defeat`, `ExtraBossPlayer_State_Hurt`, `ExtraBossPlayer_State_Idle`, `ExtraBossPlayer_State_Move`, `ExtraBossPlayer_State_None`, `ExtraBossPlayer_State_SpecialCharge`, `ExtraBossPlayer_State_SpecialEnd`, `ExtraBossPlayer_State_SpecialFire`, `ExtraBossPlayer_State_SpecialStart`, `ExtraBossPlayer_State_Unk_ov001_02301ba0`, `ExtraBossPlayer_UpdateInvincibility`, `ExtraBoss_CreatePlayerTrail`, `ExtraBoss_GetPlayer`, `ExtraBoss_IsPlayerSpawned`, `ExtraBoss_LoadPlayerModel`, `ExtraBoss_PlayerOnHurt`, `ExtraBoss_ResetPlayerForBattle`, `ExtraBoss_SpawnPlayerFlyFx`, `ExtraBoss_Unk_ov001_02302658`, `ExtraBoss_UpdatePlayerTrailNode`

- `boss_extra_weapon.c` (52): `ExtraBossObj_Draw_WeaponBoss`, `ExtraBossObj_Draw_WeaponBossDebris`, `ExtraBossObj_State_WeaponBossBeamCharge`, `ExtraBossObj_State_WeaponBossBeamFire`, `ExtraBossObj_State_WeaponBossBeamPause`, `ExtraBossObj_State_WeaponBossBeamRecover`, `ExtraBossObj_State_WeaponBossBeamRefire`, `ExtraBossObj_State_WeaponBossBeamUnitAim`, `ExtraBossObj_State_WeaponBossBeamUnitEnd`, `ExtraBossObj_State_WeaponBossBeamUnitLaunch`, `ExtraBossObj_State_WeaponBossBeamUnitResetArm`, `ExtraBossObj_State_WeaponBossBeamUnitTurnArm`, `ExtraBossObj_State_WeaponBossBeamUnitWaitClear`, `ExtraBossObj_State_WeaponBossDefeat`, `ExtraBossObj_State_WeaponBossDefeatFadeOut`, `ExtraBossObj_State_WeaponBossDefeatFinish`, `ExtraBossObj_State_WeaponBossDefeatPartBlink`, `ExtraBossObj_State_WeaponBossDefeatWaitAnim`, `ExtraBossObj_State_WeaponBossEnter`, `ExtraBossObj_State_WeaponBossHurt`, `ExtraBossObj_State_WeaponBossHurtRecover`, `ExtraBossObj_State_WeaponBossMissileEnd`, `ExtraBossObj_State_WeaponBossMissileFire`, `ExtraBossObj_State_WeaponBossMissileReady`, `ExtraBossObj_State_WeaponBossMissileRecover`, `ExtraBossObj_State_WeaponBossMissileWindUp`, `ExtraBossObj_State_WeaponBossSwordEnd`, `ExtraBossObj_State_WeaponBossSwordFollowThrough`, `ExtraBossObj_State_WeaponBossSwordNextSwing`, `ExtraBossObj_State_WeaponBossSwordRecover`, `ExtraBossObj_State_WeaponBossSwordSlash`, `ExtraBossObj_State_WeaponBossSwordWindUp`, `ExtraBossObj_State_WeaponBossWait`, `ExtraBoss_CreateWeaponBoss`, `ExtraBoss_HandNodeCallback`, `ExtraBoss_StepXTowardCameraTarget`, `ExtraBoss_StepXTowardRightEdge`, `ExtraBoss_StopAndFaceCenter`, `ExtraBoss_StopAndFaceCenter2`, `ExtraBoss_StoreNodeMtxIfMatch`, `ExtraBoss_WeaponBoss_Action_BeamUnitAttack`, `ExtraBoss_WeaponBoss_Action_Defeat`, `ExtraBoss_WeaponBoss_Action_Enter`, `ExtraBoss_WeaponBoss_Action_HomingBeamAttack`, `ExtraBoss_WeaponBoss_Action_Hurt`, `ExtraBoss_WeaponBoss_Action_MissileAttack`, `ExtraBoss_WeaponBoss_Action_SwordAttack`, `ExtraBoss_WeaponBoss_Action_Wait`, `ExtraBoss_WeaponBoss_FinishDefeat`, `ExtraBoss_WeaponBoss_OnHit`, `ExtraBoss_WeaponBoss_SelectNextAttack`, `ExtraBoss_WeaponNodeCallback`

- `boss_extra_hand.c` (55): `ExtraBossObj_Draw_HandBoss`, `ExtraBossObj_Draw_HandCatch`, `ExtraBossObj_State_HandBossCatchAim`, `ExtraBossObj_State_HandBossCatchMove`, `ExtraBossObj_State_HandBossCatchReady`, `ExtraBossObj_State_HandBossCatchResetArm`, `ExtraBossObj_State_HandBossCatchRewind`, `ExtraBossObj_State_HandBossCatchTurnArm`, `ExtraBossObj_State_HandBossCatchWaitHand`, `ExtraBossObj_State_HandBossClawDelay`, `ExtraBossObj_State_HandBossClawEnd`, `ExtraBossObj_State_HandBossClawRecover`, `ExtraBossObj_State_HandBossClawReturn`, `ExtraBossObj_State_HandBossClawSwipe1`, `ExtraBossObj_State_HandBossClawSwipe2`, `ExtraBossObj_State_HandBossClawWindUp`, `ExtraBossObj_State_HandBossClawWindUp2`, `ExtraBossObj_State_HandBossDefeat`, `ExtraBossObj_State_HandBossDefeatFadeOut`, `ExtraBossObj_State_HandBossDefeatFinish`, `ExtraBossObj_State_HandBossDefeatPartBlink`, `ExtraBossObj_State_HandBossDefeatWaitAnim`, `ExtraBossObj_State_HandBossEnter`, `ExtraBossObj_State_HandBossGrabEnd`, `ExtraBossObj_State_HandBossGrabHold`, `ExtraBossObj_State_HandBossGrabTurnArm`, `ExtraBossObj_State_HandBossHurt`, `ExtraBossObj_State_HandBossHurtRecover`, `ExtraBossObj_State_HandBossSuctionAim`, `ExtraBossObj_State_HandBossSuctionEnter`, `ExtraBossObj_State_HandBossSuctionExit`, `ExtraBossObj_State_HandBossSuctionFire`, `ExtraBossObj_State_HandBossSuctionHold`, `ExtraBossObj_State_HandBossSuctionLeave`, `ExtraBossObj_State_HandBossSuctionReady`, `ExtraBossObj_State_HandBossSuctionReturn`, `ExtraBossObj_State_HandBossSuctionWaitEnd`, `ExtraBossObj_State_HandBossWait`, `ExtraBossObj_State_HandCatchFlyOut`, `ExtraBossObj_State_HandCatchPullPlayer`, `ExtraBossObj_State_HandCatchReturn`, `ExtraBoss_CreateHandBoss`, `ExtraBoss_HandBoss_Action_CatchAttack`, `ExtraBoss_HandBoss_Action_ClawAttack`, `ExtraBoss_HandBoss_Action_Defeat`, `ExtraBoss_HandBoss_Action_Enter`, `ExtraBoss_HandBoss_Action_Hurt`, `ExtraBoss_HandBoss_Action_SuctionAttack`, `ExtraBoss_HandBoss_Action_Wait`, `ExtraBoss_HandBoss_FinishDefeat`, `ExtraBoss_HandBoss_OnHit`, `ExtraBoss_HandBoss_SelectNextAttack`, `ExtraBoss_HandCatchNodeCallback`, `ExtraBoss_HandCatch_OnGrabPlayer`, `ExtraBoss_Unk_ov001_022eed0c`

- `boss_extra_stage.c` (65): `ExtraBoss_AllocBgWork`, `ExtraBoss_AllocSubScreenWork`, `ExtraBoss_ClearSpawnLists`, `ExtraBoss_CollectRings`, `ExtraBoss_DrawHud`, `ExtraBoss_DrawRings`, `ExtraBoss_EndRingSuction`, `ExtraBoss_FreeBgWork`, `ExtraBoss_FreeObjVramSlots`, `ExtraBoss_FreeSpawnWork`, `ExtraBoss_FreeSubScreenWork`, `ExtraBoss_InitRings`, `ExtraBoss_IsOutOfField`, `ExtraBoss_IsRingTouchingPlayer`, `ExtraBoss_PushScrollEntry`, `ExtraBoss_ResetSpawnWork`, `ExtraBoss_ResetSubDisplay`, `ExtraBoss_RingList_AddAttracted`, `ExtraBoss_RingList_AddCollected`, `ExtraBoss_RingList_AddLoose`, `ExtraBoss_RingList_AddPlaced`, `ExtraBoss_RingList_RemoveAttracted`, `ExtraBoss_RingList_RemoveCollected`, `ExtraBoss_RingList_RemoveLoose`, `ExtraBoss_RingList_RemovePlaced`, `ExtraBoss_ScatterPlayerRings`, `ExtraBoss_SetRingPickupEnabled`, `ExtraBoss_SetScrollSpawnEnabled`, `ExtraBoss_SetupSubDisplay`, `ExtraBoss_SpawnBoss`, `ExtraBoss_SpawnFromScrollTable`, `ExtraBoss_SpawnRingBurst`, `ExtraBoss_StackPop`, `ExtraBoss_StackPush`, `ExtraBoss_StartBgShake`, `ExtraBoss_StartRingSuction`, `ExtraBoss_State_ScrollDone`, `ExtraBoss_State_ScrollSpawn`, `ExtraBoss_State_Unk_ov001_0230587c`, `ExtraBoss_State_Unk_ov001_02305fbc`, `ExtraBoss_State_Unk_ov001_0230611c`, `ExtraBoss_StopSubScreenTasks`, `ExtraBoss_Unk_ov001_022ffcf8`, `ExtraBoss_Unk_ov001_02304fb4`, `ExtraBoss_Unk_ov001_023059b4`, `ExtraBoss_Unk_ov001_02305aec`, `ExtraBoss_Unk_ov001_02305bb8`, `ExtraBoss_Unk_ov001_02305c84`, `ExtraBoss_Unk_ov001_02305cd8`, `ExtraBoss_Unk_ov001_02305d40`, `ExtraBoss_Unk_ov001_02305ec8`, `ExtraBoss_Unk_ov001_02305ee8`, `ExtraBoss_Unk_ov001_02305f08`, `ExtraBoss_Unk_ov001_023061b0`, `ExtraBoss_Unk_ov001_02306244`, `ExtraBoss_Unk_ov001_023062f0`, `ExtraBoss_Unk_ov001_02306424`, `ExtraBoss_UpdateBgScrollPalette`, `ExtraBoss_UpdateBgWaveScroll`, `ExtraBoss_UpdateDualScreen3D`, `ExtraBoss_UpdateRings`, `ExtraBoss_UpdateRingsSuction`, `ExtraBoss_UpdateSpawnTimer`, `ExtraBoss_UpdateSpawnWork`, `ExtraBoss_Unk_ov001_022fdbc0 (asm)`

- replaced by the converter (2): `ExtraBoss_GetCompressionType`, `ExtraBoss_GetUncompressedSize`


# Native stage objects (Dreamcast port): translation notes

Status: every object and decoration placed in the act stages of all zones is created by translated
code: Leaf Storm (z11, z12), Water Palace (z21, z22), Mirage Road (z31, z32), Night Carnival (z41,
z42), Huge Crisis (z51, z52), Altitude Limit (z61, z62), Dead Line (z71, z72), z81/z82 and z91/z92
(coverage table in section 8). The boss objects 0x38..0x3f of the act-3 arenas belong to the boss
module. The special stage round trip keeps the act's event state (section 2.5). Host tests:
`make -C dreamcast/game/objects host-test zone-test`. Integrated in the game and played in Flycast (REGRESSION.md); not run on real hardware.

Reuse-first rule: new code is the decompiled C copied
from `src/` and adapted: DS offset structs become named `Obj` / `DcPlayer`
fields (offsets kept in comments), DS services (VRAM, OAM, palettes, sound
handles) are dropped or sent to hooks, and the original function names are
kept. Section 5b classifies every new function as reused-as-is, modified,
substantially rewritten or new.

- Code: `dreamcast/game/objects/` (`obj_*.c/h`, `objects.h`, `Makefile`,
  `test/test_objects.c`, `test/test_coverage.c`, `tools/objdata.py`)
- Source of truth: `src/game/Stage/StageEvent_*`,
  `src/game/Stage/ObjectSystem/` (`MapObject_*`, `ObjRect_*`,
  `ObjCollisionMap_*`, `StageObj_Main` and helpers),
  `src/game/Objects/` (RingManager, Spring, DashP/DashC/DashCT, ItemBox,
  Needle, GoalRing, Marker, MapObj40), `src/game/Enemies/EnemyPawnGroup_*`,
  `src/game/Effects/` (EffZakoBombBurst, EffZakoDebris, RingEffect2D),
  `src/game/Graphics/Sprite/` (AnimatorSprite, BAC command handlers).
- Third-party code: none.

## 1. Files

| file | content |
|---|---|
| `objects.h` | public API (below), `ObjHooks`, `ObjStage`, `ObjStats` |
| `obj_types.h` | `Obj` (StageObj header + work, with original offsets), `ObjBox` (ObjRect), `ObjColMap`, `ObjParam`, `ObjSprite`, `ObjRes`, DSPR/HBOX records |
| `obj_internal.h` | module-internal prototypes, `ObjGlobals`, `ObjData` |
| `obj_manager.c` | task list, StageEvent spawning, MapObject_Init/Spawn/Destructor, StageObj_Main, ViewOutCheck, create table, API |
| `obj_rect.c` | ObjRect hit box lists and the attack/defence protocol, StageObj_HandleCollider |
| `obj_colmap.c` | ObjCollisionMap (solid objects) and the `g_dcp_col.obj_probe` implementation |
| `obj_anim.c` | sprite resources (frames.bin, .hbx), AnimatorSprite emulation, StageObj_SetAnimation, Draw2DEx animation part |
| `obj_player.c` | player boxes, Player_Collide, son.bac box timing, player hooks, player actions the objects need |
| `obj_ring.c` | RingManager (placed and scattered rings) and RingEffect2D (collect sparkle) |
| `obj_gimmicks.c` | Spring, DashP, DashC/DashCT, ItemBox (+ icon and popup), Needle, GoalRing (+ parts), Marker, MapObj40, StageClear_HandleGoalReached score part |
| `obj_enemy.c` | EnemyPawnGroup (Egg Pawn; v3: Egg Lancer charge, Egg Bishop long attack + throw, Egg Hammer stomp, drop-in, enemy-room entrance), MapObject_OnHit_Default, destruction (burst, debris, knock-away) |
| `obj_draw.c` | KOS only: atlas upload and quad drawing with `render.h`; rot-scale quads, rope multi-draw, piece offsets |
| `obj_fx.c` | NitroSDK MTX_Identity33_/RotX/Y/Z33_/Concat33/MultVec33, FX_DivS32, ObjShift_Set, ObjShift_SetFromRef |
| `obj_effects2.c` | EffZakoBombPuff, EffFound (+ Effect_State_FollowParent), EffBrakeSmoke |
| `obj_peffect.c` | player effects for `g_dcp_hooks.effect` (DCP_FX_*): EffBoost (+ _State_FollowPlayer), EffBoostAura, EffBoostBurst (+ mirrored second draw), EffHighSpeedStart, EffBlazeBoostFlame / RunFlame / SpinFlame / TrickFlame, EffTrail (logic + `objects_trail_quads`), EffBoostJet_Spawn, EffSpinSmoke1/2 (_Spawn), EffBrakeSmoke_Spawn |
| `obj_leafstorm.c` | Trampoline, BendStick, CorkscrewTrigger, RockBreak (+Part), RopeC (+Land, +Spring), EggPawnM (+ Enemy pop-on-contact) |
| `obj_shared.c` | Rope, JumpPedestal, PlayerStart, WallBreak, IronBall (+Part), GmkSpecial, GmkLand, GmkLandAA, MapObj73, MapObj75, MapObj70, WaterLevelTrigger, camera mirror, ScreenShake, model views |
| `obj_flapper.c` | EnemyFlapperGroup (types 2 and 4: hover / patrol) |
| `obj_decor.c` | StageEvent_SpawnDecorationsInBlock, Decoration framework, DecGroup0F, DecVine, DecBird, DecGroup35 (+Piece, debris), DecAnimal |
| `obj_pgimmick.c` | player gimmick states (Rope, RopeC, BendStick, CorkscrewPath, GmkSpecial, PedestalLaunch, Grind), Player_FindHomingTarget, Player_CheckStartGrind, Player_GiveMagnet |
| `obj_water.c` | v3, Water Palace: WaterRunTrigger, MapObj7D, MapObj80, LaunchZone, LandUnrest, Wheel2D, Mine (+MineDigit), Bubble, BubbleSpawner, WaterJet, Kuragen (+KuragenBomb), DecGroup41 (+DecFishPart), DecGroup4D, DecShared; effects EffZakoBomb, EffAirBubble, EffDrownCount, EffGmkBubble, EffWaterGmk (+Splash), EffWaterRunSplash, EffWaterSplash |
| `obj_pwater.c` | v3, player states: WaterRun, ZoneLaunch, BreatheAirBubble, Wheel2D, MapObj80, MapObj40, AttachToObject / AttachedToObject; Player_ApplyAutoRunAcceleration, Player_UpdateUnderwater, the gimmick check of Player_In |
| `obj_mirage.c` | v3, Mirage Road: GmkRail, Cargo, Hand2d, GmkTable, MapObj8C/8D/GmkWall/MapObj8F + EnemyRoom_* / Gimmick_* helpers, SandSlider (+part), Rock, EggBishop, DecGroup45; effects EffSand, EffSandSlide, EffShadow, EffEntrance |
| `obj_pmirage.c` | v3, player states: Cargo, Hand2D, SandSlide, SandSlider, SandSlider90, AttachGimmickTable (+NoInput, +Released), DetachGimmick |
| `obj_zone.h` | v4: small inline helpers shared by the zone 4..7 files (priorities, trigger boxes, collision map setup, counters, model view setup, matrix store) |
| `obj_carnival.c` | v4, Night Carnival: CraneSlide, NeonRoute, Shooter (+Part1, Part2), SwingSky, Switch (+Part), SwitchTimer, gSwitchTimers, ObjSpd_AccelToward; decorations DecGroupAF (+Child back/front), DecSonicNeon (+Part) |
| `obj_pcarnival.c` | v4, player: GrabCraneSlide, GrabCranePart, SwingSky; FixInformation (SwingSky / Swing hint) |
| `obj_crisis.c` | v4, Huge Crisis: Crane, CranePartED (hook), CranePartEE (mast), GmkPipe, JetCatapult (+Part, EffJet), Piston, Steam, Valve; decoration DecGroup67 |
| `obj_pcrisis.c` | v4, player: Pipe (+Exit), JetCatapult (+Launch) |
| `obj_altitude.c` | v4, Altitude Limit: Glider, NeedleG, Parachute (+Released), Saucer (+Explode), MapObj13 (3D flappers, glider rings, clouds; shared drivers), DecCloudSpawner, DecCloudW, DecGroup95 |
| `obj_paltitude.c` | v4, player: Glider, ResumeGlider, ClampToGliderArea, GliderSequenceStart / Intro / Sequence / Update / End / Exit, OnHit_GliderSequence, Parachute (+Release), Saucer |
| `obj_deadline.c` | v4, Dead Line: Cannon, Duct, GmkAir, MapObj9C, Rocket (+Explode), Swing; decoration DecArrow |
| `obj_pdeadline.c` | v4, player: Swing (+SwingAroundPivot, Retract), Cannon, Rocket, Air, Player_Unk_ov000_02350adc (depth lane) |
| `obj_enemy2.c` | v4: EggSf (0xe6, Egg Pawn type 9 child), GunHunter (0xec, type 0x0d child) |
| `test/test_coverage.c` | creates every placed object / decoration of every stage, prints the coverage table |
| `test/test_zones.c` | host test on z21..z92 (14 act stages): run, per-gimmick checks, sweep, special stage round trip |
| `tools/objdata.py` | BYOR converter: `objdata.bin`, `hbx/<id>.hbx`, `sprites.txt` |

## 2. Integration

```c
#include "../objects/objects.h"
/* once per stage, after rlvl_bind(&map, lv.data), dcp_bind_map(&map), dcp_player_create(&player, ...) */
objects_set_paths("/cd/objects", "/cd/sprites");
objects_add_player(&player);
objects_init(&map, lv.data);                /* installs g_dcp_hooks.{anim,anim_speed,atkdef,spawn_ring} (chained) */
objects_set_view(0, cam_x << 8, cam_y << 8); /* DS gCamera[0]: top-left of the 256x192 window, 24.8 */
/* per frame */
objects_frame_begin();                      /* Stage_Main: ObjRect_CheckAllGroup, CommitFrame; StageEvent_Main */
dcp_player_frame(&player, held, pressed, released);
objects_update(&player);                    /* Player_Collide, objects (0x500), effects (0xa00), rings (0x3000) */
/* camera update, then objects_set_view(0, ...) for the next frame */
render_begin_list(PVR_LIST_PT_POLY); ...level layers...; objects_draw(); render_end_list();
```

- Set any of your own `g_dcp_hooks` (`anim`, `anim_speed`, `atkdef`,
  `spawn_ring`) **before** `objects_init`: the module saves them and calls
  them after its own handling. `objects_shutdown()` restores them.
- `objects_init` ORs `0x1c` into `g_dcp_world.objmgr` (view-out, map
  collision, hit boxes on, as Stage_Create does).
- After a respawn or any camera jump call `objects_respawn_view()`. The DS
  restarts the whole stage there (StageEvent_InitialSpawn). The normal update
  only spawns in the band that enters the view, so objects inside a view
  that jumped would not appear.
- The marker writes `g_dcp_world.check_x/check_y` (the player's respawn point).
- `g_obj_hooks`: `sfx(id)` (Sound_PlaySeqArcEx ids, as in the original),
  `goal(p)` (Player_Action_ReachGoal is not in the player module), `stage_exit(kind)`
  (StageShutdown_Create: results / clear demo), `fade(kind, speed)`,
  `item(p, k)` (invincibility, barrier, magnet, VS items), `checkpoint_popup`, `log`.
- `g_obj_stage`: speed bonus, ring bonus, checkpoint index/time/position
  (data_022c4590 fields).
- `objects_ring_count()`: ring parts alive in the ring manager (placed and
  scattered). The player's ring counter stays `DcPlayer.rings`.
- `objects_set_player_anim_done(1)`: sets `DCP_DF_ANIM_DONE` on the player when
  the son.bac timing ends an animation. It is off by default, because the 3D model
  renderer may own that flag.

### 2.5 Special stage round trip (event state)

The DS keeps the act alive across the special stage with gGameState.unk18
& 0x40: StageExit_Main runs Stage_DestroyTasks (every destructor writes its
spawn state back into the map-object list) and then Stage_SaveEventState
(copies of the map-object list and the ring list into data_022c4520) and
Stage_SaveRestartCamera (gCamera x, y and gCamera[2] flags & 2 into
data_022c4590.unk58/5A/5C). On the way back StageStart_State_SetupResources
runs Stage_RestoreEventState and Stage_RestoreRestartCamera before
StageStart_State_CreateStage spawns anything. data_022c4590 itself
(checkpoint index / time / position, score tally, information hint) is not
cleared for the return.

Translated as:

- `objects_save_event_state()`: destroys every object task (destructors
  run, live objects and live placed rings give their entry back:
  MapObject_Destructor, RingManager_Destructor `ring_destroy`), then copies
  `g_obj.prm` (the map-object list: destroyed objects keep x0 = 0xff) and
  `g_obj.ring_state` (collected rings keep 0xff), the camera (`g_obj_camera`
  mirror) and `g_obj_stage` into static storage that survives
  `objects_shutdown` / `objects_init`.
- `objects_restore_event_state()`: before `objects_init` it arms the
  restore, which `objects_init` applies at the SetupResources point (after
  `camera_init`, before `spawn_all_map_objects` and the ring manager). After
  `objects_init` it destroys the objects that init created, applies the
  copy and creates them again from it. Both orders give the same result.
  The copy is used once and ignored for another stage.
- `objects_restart_camera(&x, &y, &flags)`: the restored gCamera[0]
  position (24.8) and flag 2; returns 1 after a restore. The mirror is set
  as Stage_RestoreRestartCamera does: `g_obj_camera[0].x/y`,
  `g_obj_camera[2].flags |= flag 2`, `g_obj_camera[1]` = same x, y +- 0x110
  px. The camera module places its camera from it and then calls
  `objects_set_view` as usual.

Call order in main.c:

```c
/* g_obj_hooks.stage_exit(ctx, OBJ_EXIT_SPECIAL_STAGE) */
objects_save_event_state();     /* first: it needs the live objects (destructors) */
objects_shutdown();
level_free(...);
/* special stage */
level_load(...);
objects_restore_event_state();  /* here (applied inside objects_init) ... */
objects_init(&map, lv.data);
/* ... or here: objects_restore_event_state(); either works */
/* camera from objects_restart_camera(), objects_set_view(0..1, ...),
   player restore (checkpoint from g_obj_stage.check_x/check_y), then frames */
```

What the round trip keeps: destroyed enemies and broken objects stay gone,
collected rings stay collected, untouched objects and rings come back, the
checkpoint (index, time, position), score tally and the SwingSky hint flag.
What it does not keep (as on the DS): positions and states of live objects
(they respawn from their placement), scattered rings, switch timers. The
special ring itself has no "used" state in the original (entry needs the
full tension gauge again). Host test: section 7, `[special stage round
trip]`.

## 3. Data (bring your own ROM)

`make data` (= `python3 dreamcast/game/objects/tools/objdata.py`) reads
`extract/eur` and the sprite converter output `build/dreamcast/sprites/` and
writes `build/dreamcast/objects/`:

- `objdata.bin` "ROBJ" **v4** (235 568 bytes; v3: 165 456). Header: `char[4]
  "ROBJ"; u16 version 4, slots (145), stages (27), id_len (48)`. v4 adds 54
  slots and four tables after `dec_margin`: `valve_ofs[4][2]` (s16, steam
  sprite offsets), `valve_box[4][4]` (s16), `valve_puff[4][4]` (s16, puff
  spawn rectangles) and `valve_anim[4][3]` (u32: active / end animation,
  sprite flags), data_ov000_02364444.. (Valve, Huge Crisis). The module
  refuses other versions. Then these tables, each
  padded to 4 bytes, copied from the owner's ROM:
  `margin[0xf2]` (data_ov000_023686e0), `s16 ring_gravity` (data_ov000_023690e4),
  `itembox_rings[8]` (data_ov000_02366b44), `itembox_vs[8]` (data_ov000_02366b3c),
  `player_spr_anim[0x68]` (data_ov000_02368d14), `solid[48][8]`
  (data_ov000_023661c0, the all-solid collision tiles; Needle uses the first
  12), `animal[8][3]` (data_ov000_023661a8), `s8 shake[16]` (arm9
  data_02077944), `col_attr[4]` (data_ov000_023661a4), `s8 bend_wobble[28]`
  (data_ov000_023643d4/_d8/_dc), `s16 pedestal[4][6]` (data_ov000_02364400),
  `dec_margin[0xf2]` (data_ov000_023685ec). After them `char
  id[stages][slots][48]`: the sprite id each BAC slot resolves to on each
  stage (zone archive first, then the loose `/act/` file, as ObjData_Load).
  Last, per stage, the `/dat/gmk/*.df` collision maps (JumpPedestal, and
  `gmk_cargo.df` for Cargo since v3; JetCatapult/Switch later): `u32 offset[stages + 1]` then records
  `{char name[24]; u32 size; data}`, taken from the zone archive or the loose
  `dat/gmk/` directory.
- `hbx/<id>.hbx` "HBOX" v1 (162 files, 17 000 bytes; v3: 108 files, 11 236 bytes; earlier versions of
  this file gave the disk usage in 4 KB blocks): the BAC command 7 boxes per
  frame (format unchanged).
- `sprites.txt`: the sprite directories and the stages that can use them.

Sprite slots (enum ObjBac, order == `SLOTS` in objdata.py): the 14 of v1
plus TRAMPOLINE, BEND_STICK, ROCK_BREAK, ROPE, ROPE_C_LAND, ROPE_C_SPRING,
ROPE_C, JUMP_PEDESTAL, FLOOR_BREAK, LAND, WALL_BREAK, IRON_BALL, SPECIAL,
CARGO, FLAPPER, EGG_DIVER, DEC_FLOWER, DEC_VINE, DEC_BIRD_NEST, DEC_BIRD,
DEC_LEAF, EFF_FOUND, EFF_BRAKE and ANIMAL01..ANIMAL16; v3 adds (zone 2)
LAND_UNREST, WHEEL2D, MINE, BUBBLE, WATER_JET, KURAGEN_BOMB, KURAGEN_BLITZ,
EGG_LANCER, EFF_WATER, EFF_WATER_GMK, FIX_MOJI (`act_com/ac_fix_moji.bac`,
the drowning and mine digits), DEC_FOUNTAIN, WATER_FALL, GMK_WHEEL,
DEC_ALGAE, DEC_CORAL, DEC_FISH and (zone 3) EGG_BISHOP, EGG_HAMMER, HAND2D,
ROCK, SAND_SLIDER, EFF_SAND, EFF_SHADOW, GMK_RAIL, GMK_WALL, GMK_HAND,
DEC_SAND, DEC_DECORA, DEC_RELIEF, DEC_STATUE, DEC_TORCH, DEC_CANOPS,
DEC_BED, DEC_BOX, DEC_TABLE_WATER, EFF_ENTRANCE, GMK_RAIL6; v4 adds the
zone 4..7 slots (`SLOTS_V4` in objdata.py, enum ObjBac after GMK_RAIL6:
crane slide, neon route, shooter, swing sky, switch, fix information, dec
neon / light / swing sky parts, Egg Bunny, flapper bat, crane, pipe,
piston, valve, jet catapult, EFF_JET, EFF_STEAM, dec pipe / plane, Gun
Hunter, Rhino, Bomb Hawk, glider, needle G, parachute, saucer, cloud_3, ring
graider, flapper3d, dec cloud / cloud W / inner pillar / propeller / jet /
light / laser, flapper cannon, Falco, rocket, air, cannon, swing, duct, dec
arrow and the rest). Resources are no longer
all loaded at `objects_init`: the module preloads the slots of the object and
decoration types placed on the stage, the zone's three freed animals and the
always-used effects; anything else loads on first use (`obj_res_ensure`,
counted in `g_obj.lazy_loads`; 0 on z11/z12/z21/z22/z31/z32). z11 and z12
preload 38 slots (1.60 / 1.66 MB of atlas data, PVR VRAM), z21 35, z22 36,
z31 and z32 43 slots (ids printed by the host tests). Zone 2 always preloads
EFF_WATER and BUBBLE (splashes and air bubbles appear without a placed
object).

**Disc layout** (`build_disc.sh`):

```
/cd/objects/objdata.bin                          (235 568 bytes)
/cd/objects/hbx/<id>.hbx                         (162 files, 17 000 bytes)
/cd/sprites/<id>/frames.bin + atlas0.argb1555    (from build/dreamcast/sprites/bac/<id>/)
```

with `<id>` = every id in `build/dreamcast/objects/sprites.txt` (162 ids,
9 846 521 bytes of frames.bin + atlases; v3: 108 ids, 5 289 056 bytes). For Leaf Storm alone the 38 ids
printed by the host test (`objects_init ... preloaded sprites:`) are enough;
for Water Palace / Mirage Road the ids printed by `test_zones`. Ids are up to 46
characters long (`narc__z41_act_lz7__act__ac_gmk_wall_braek01`), so the ISO
needs Rock Ridge/Joliet names (build_disc.sh uses `-r -J`; Joliet allows 64).
The RopeC model needs `build/dreamcast/models/mod__gmk_rope_c__gmk_rope_c01.dcm`
and `_c02.dcm` if main.c draws the model views; v3 adds
`mod__gmk_wheel__gmk_wheel.dcm`, `mod__gmk_hand__gmk_hand.dcm` and
`mod__gmk_table__gmk_table.dcm`; v4 adds `mod__gmk_slide__gmk_slide.dcm`,
`mod__gmk_crane__gmk_crane.dcm` and `mod__gmk_glider__gmk_glider.dcm` (all
present in build/dreamcast/models/).

## 4. How the original works, and how it is mapped

### 4.1 Objects and tasks

Every DS object is a task with a work struct that starts with the common
StageObj header (UnkStruct_StageObj, 0x20C bytes). `Obj` (obj_types.h) is that
struct:

- `Obj.h` is a `DcPlayer`. The player module's DcPlayer is the translated
  StageObj header, with the same fields at the same offsets for x/y, velocities,
  flags 0x54/0x58/0x5C, hit-stop 0x62, map box 0x64 and ride/touch 0x80/0x84.
  This lets StageObj_Move, StageObj_Collision and StageObj_HandleRide be reused
  (`dcp_obj_move`, `dcp_obj_collision`, `dcp_handle_ride`), not translated twice.
  The Egg Pawn walks with these on the real map, and the debris falls with them.
- All other header fields have their own members, commented with their offset:
  `w8/wC` 0x08/0x0C, view-out 0x4A..0x52, state 0x68, In 0x6C, draw 0x70,
  sprite callback 0x74, collide 0x78, last 0x7C, parent 0x88, spr 0x94,
  coll 0x134/0x160/0x18C, collision map 0x1B8, param 0x1E0, 0x1E4..0x204.
- Task order = Task_Create order: a list sorted by id, FIFO within one id.
  The ids are map objects 0x500, effects 0xa00, ring manager and sparkles 0x3000.
  A task created while the list runs runs in the same frame if it sits after
  the current one, as on the DS.
- Objects come from a fixed pool of 384 (the DS uses the task heap; v2 decorations share it). If the pool is full,
  creation returns NULL, like `task == OS_GetArenaLo(0)`.

### 4.2 Spawning (StageEvent)

- The level lists (`rlvl_object_t`, `rlvl_ring_t`) are grouped by 256 px region.
  `objects_init` makes a mutable copy of every object entry (`ObjParam`, the
  .ev entry layout). The objects write into it, as on the DS: `x0 = 0xff`
  means spawned, `p6` holds the item kind, `pB` counts hits, and `pA` is the trick counter.
  Rings keep one byte per entry (`.rg` byte 0).
- `StageEvent_Main`: on the first frame InitialSpawn runs (flags 1: spawn everything in
  view + margin). After that, UpdateDualScreen runs (flags 5) only when the view moved.
  The spawn rect is view ± margin (objects 0x100, rings 0x10, decorations 0x100;
  decorations use their own margin table data_ov000_023685ec). It is clamped to the map, and each clamped side clears the
  per-type margin (flags 0x10..0x80). An object spawns if it lies inside
  `rect - margin[type]`, and with flag 4 only if it lies outside
  `view ± (0x100 - margin[type]) / 3`. The arithmetic is unsigned 32-bit as in the
  original, including its wrap near the map's left/top edge.
- Respawn rules: MapObject_Init stores the entry's x and writes 0xff. When the
  object despawns (StageObj_ViewOutCheck with margin `0x108 - margin[type]` →
  destroyed next frame), MapObject_Destructor restores x. When the object was
  destroyed for good (`@0x1F4 & 0x10`: enemy killed, item box broken), x stays 0xff.
  Rings: a placed ring writes 0xff and restores `x & 0xff` when it leaves the view
  (+16 px). A collected ring stays 0xff.
- Stage_Create's StageEvent_SpawnAllMapObjects (entries with flag 0x4000) runs
  in `objects_init`. Boss stages (`zone >= 9 || act >= 2`) use SpawnAll.

### 4.3 Per-frame object update (StageObj_Main)

Unchanged order: destroy if `0x54 & 4` → `& 8` becomes `& 4` → view-out →
parent check → HandleRide → In (MapObject_In_Default: hit flash, box flags) →
hit-stop counters → state → Move (unless `0x58 & 0x2000`) → map collision
(unless `0x58 & 0x100`) → draw step → Collide (hit boxes + solid map registration) → clear
push/shake → Last.

The draw step is the animation half of StageObj_Draw2DEx: flags from `0x5C`
(bit0 inverted for 2D sprite art, 4 loop, 1/2 flip, 0x10 freeze, 0x20 hidden,
0x80 screen space, 0x1000 no processing), speed × time scale, process the
sprite with the object's sprite callback, and set `0x5C |= 8` when the animation
ended. `objects_draw()` later emits the quad of the current frame.

### 4.4 Sprite animation (AnimatorSprite on frames.bin)

`spr_process` is AnimatorSprite_ProcessAnimation on the converted frame table:
`wait = duration << 4` per frame; `speed` 0x10 per game frame (Goal ring: 0x10..0xc0;
player: Player_SetAnimSpeedFromVelocity). When the last frame's wait runs out,
`flags |= 2` (ended). A non-looping sprite then stops (`flags |= 1`) on the last frame. A looping
sprite jumps to `loop_frame` (EndAnimation) in the same call. A frame's hit box
events (command 7, from .hbx) fire through the callback when the frame starts, before
EndFrame, as in the command stream. Checked on the converted data: the spring
animation (2,4,2,4,2) ends after 14 frames and holds frame 5; the ring
spin loops every 32 frames. The ring manager clears the stop bit every frame
(RingManager_Main), so its non-looping shared ring sprite restarts with one
extra frame on the last image, as on the DS.

### 4.5 Hit boxes and the player interaction protocol (ObjRect)

- A box (`ObjBox` = ObjRect, 0x2C) has a rect relative to its owner, mirrored by
  flag 1/2 (HandleCollider sets them from `0x5C` and the angle), `atk` and
  `def` words, a callback, and a mask. The mask's low nibble is the list the box
  registers in, and the high nibble is the lists it is checked against. Player boxes
  use 0x21 (list 0, checked against list 1). Map objects use 0x12.
- Boxes register every frame (`fn78`, only when not in hit-stop or pause). Stage_Main
  checks the lists from the previous frame (ObjRect_CheckAllGroup) and swaps them.
  So a hit is applied at the start of the next frame, before the player's
  state runs. That is why `objects_frame_begin()` must come before
  `dcp_player_frame()`.
- For every overlapping pair (a, b) both directions are tried. If
  `ObjRect_FlagCheck(a.atk, b.def)` holds (atk has a bit outside def's bits above
  bit 2, and atk level `& 7` >= def level), then the **defender's** callback
  `b.cb(b, a)` runs. Flag 8 (attacker callback) is never set by these objects.
  Flags 0x100/0x200/0x400 stop repeated hits. 0x800 boxes fire only when contact
  starts (0x40/0x80 tracking). A box without 0x20 leaves the lists after a hit.
  `ObjRect_FuncNoHit` undoes the flags of the call.
- Values (all from the decompilation):

| box | atk | def | callback |
|---|---|---|---|
| player body @0x2A0 (son.bac slot 0) | 0xa (0x1f invincible) | 2 (7 while invulnerable or invincible) | Player_OnHurt |
| player attack @0x2CC (son.bac slot 1) | 0x13 jumping, rolling, spin dash, homing, tricks, bounce; 0 otherwise (player module `atkdef` hook) | 0 | none |
| enemy body coll0 | 9 | 0xa | MapObject_OnHit_Default |
| enemy attack coll1 | 0x13 | 0x13 | none |
| spring / dash / item box / marker / goal / triggers | 0 | -0x10 | the object's OnHit |
| spikes coll0 | 0x53 | -1 | none (they hurt the player body) |
| Egg Pawn detect @0x20C | 0 | -0x10 | EnemyPawnGroup_OnDetect |

  So: the player's attack box destroys enemies (0x13 vs 0xa). The enemy attack
  box hurts the player body (0x13 vs 2) unless the player is invulnerable (def 7).
  Gimmicks react to the player body only (0xa vs -0x10). The attack box (0x13) does
  not trigger them. Within one check pass, the player's attack box is checked first
  (Player_Collide registers it first), so a jump into an enemy destroys it, and its
  disabled boxes (0x1000) then cannot hurt the player.
- The player's boxes come from son.bac (player_lz7.narc), the data-only BAC.
  The module plays its timing with the sprite index of Player_SetAnimation
  (data_ov000_02368d14, +0x1b per character for ids >= 0x30) and the speed of
  Player_SetAnimSpeedFromVelocity. It does this through the `anim` / `anim_speed` hooks. A
  new animation disables both boxes until its first frame sets them again.
  The player module does not model the box flags of Player_In / Player_OnHurt
  (`@0x2C4 |= 5` during invulnerability, `= 2` at the end, `@0x2C0 |= 0x15`
  while invincible). `obj_player_collide` derives them from `invuln` / `invinc`
  each frame, and `obj_player_on_hurt` applies `|= 5` right after the hurt.

### 4.6 Solid objects (ObjCollisionMap)

Objects with a collision map (here the spikes) register it when in view
(MapObject_Collide_Default). `objects_frame_begin` commits the list.
`g_dcp_col.obj_probe` = ObjCollisionMap_ProbeForObj. It uses the same distance
convention as the map probes, and sets `p->ride` / `p->touch` to the object's
`DcpObjRef` and the entry's rider/toucher. The tile data is 8 bytes per 8x8 tile
(`height << 4 | width` per column/row), flipped by the object's `0x5C` bits.
The spikes hurt a player standing on them (Needle_State_Idle: rider check +
ObjRect_FlagCheck(0x53, body def) + Player_OnHurt), and their tips hurt by hit box.

### 4.7 Rings

RingManager (64 parts): placed rings (flags 0x620: static, no gravity, no
timer) and scattered rings (timer 0x100 + rand & 0x1f, gravity 18 per frame,
bounce with 3/4 speed against map tiles on the ring's layer, blink below 0x20,
collectable when timer < 0xd8). Pickup is ObjRect_BoxCheck of the player body
box against `(x-6, y-16, 20, 16)`, then `dcp_add_rings(p, 1)` (sound 4 through
the player's hook) and a RingEffect2D sparkle (ring BAC animation 1, removed
when it ends). Player_OnHurt → `dcp_scatter_rings` → `g_dcp_hooks.spawn_ring`
→ RingManager_SpawnRing. The hook also sets the manager's scatter-active flag.

## 5. Function mapping (original -> translated -> status)

Status: `translated` (line-by-line), `reused` (the player module translation is used), `partial` (see notes), `assumed`, `replaced` (native data instead), `not needed` / `not used` (no caller in this port), `not here` (other module), `not translated`.

Counts (v1 tables below, updated where v2 changed a row): see the rows; the v2 additions are in section 5b.

### StageEvent (src/game/Stage)

| original | translated | status | notes |
|---|---|---|---|
| `StageEvent_CreateManager` | `objects_init (margins 0x100/0x10/0x100)` | translated |  |
| `StageEvent_Destructor` | `objects_shutdown` | translated |  |
| `StageEvent_GetMapObjectList` | `g_obj.lv_obj / lv_obj_region (rlvl region tables)` | replaced | level.bin holds the lists |
| `StageEvent_GetRingList` | `g_obj.lv_ring / lv_ring_region` | replaced |  |
| `StageEvent_InitialSpawn` | `stage_event_main (event_state 0)` | translated | single-screen camera mode (gCamera.unkC8 & 1) not used: DC view = dual-screen formula, 1 view |
| `StageEvent_Main` | `stage_event_main` | translated |  |
| `StageEvent_Reset` | `objects_init / objects_shutdown` | translated |  |
| `StageEvent_SpawnAll` | `spawn_all` | translated | boss stages |
| `StageEvent_SpawnAllMapObjects` | `spawn_all_map_objects` | translated |  |
| `StageEvent_SpawnDecorationsInBlock` | `spawn_decorations_in_block` (obj_decor.c) | translated | v2 |
| `StageEvent_SpawnInView` | `spawn_in_view` | translated |  |
| `StageEvent_SpawnInViewSingleScreen` | - | not used | DC uses the dual-screen variant with n views |
| `StageEvent_SpawnMapObjectsInBlock` | `spawn_objects_in_block` | translated |  |
| `StageEvent_SpawnRingsInBlock` | `spawn_rings_in_block` | translated |  |
| `StageEvent_UpdateBoss` | `stage_event_main (event_state 2)` | translated | no-op |
| `StageEvent_UpdateDualScreen` | `stage_event_main (event_state 1)` | translated |  |
| `StageEvent_UpdateSingleScreen` | - | not used |  |
| `StageClear_HandleGoalReached` | `stage_clear_goal` | partial | score part; time-attack record and VS branch not translated; Player_Action_ReachGoal -> g_obj_hooks.goal |

### ObjectSystem (src/game/Stage/ObjectSystem)

| original | translated | status | notes |
|---|---|---|---|
| `MapObject_Action_KnockAway` | `mapobject_knock_away` | translated | reached through the boost AttackHitbox (v2) |
| `MapObject_AllocSpawnEntry` | `spawn_entry_alloc` | translated |  |
| `MapObject_Collide_Default` | `obj_mapobject_collide_default` | translated |  |
| `MapObject_Destructor` | `obj_mapobject_dtor` | translated | sound handle release dropped |
| `MapObject_FreeSpawnEntry` | `spawn_entry_free` | translated |  |
| `MapObject_In_Default` | `mapobject_in_default` | translated | MapObject_NetReceiveHits dropped |
| `MapObject_Init` | `obj_mapobject_init` | translated | @0x130 ride link not kept |
| `MapObject_NetReceiveHits` | - | not translated | wireless play |
| `MapObject_NetSendEvent` | - | not translated | wireless play |
| `MapObject_OnHit_Default` | `mapobject_on_hit_default` | translated | EffVs/EffVitality effects dropped (RNG draws kept); AttackHitbox branch (knock-away, hit-stop 0x44 while boosting) restored in v2 |
| `MapObject_Spawn` | `obj_mapobject_spawn` | translated |  |
| `MapObject_SpawnDestroyBurst` | `mapobject_spawn_destroy_burst` | translated |  |
| `MapObject_SpawnFreedAnimal` | `mapobject_spawn_freed_animal` | translated | v2: Decoration_Spawn(DecAnimal) |
| `MapObject_SpriteCallback_Default` | `obj_sprite_cb_default` | translated |  |
| `MapObject_State_KnockedAway` | `mapobject_state_knocked_away` | translated |  |
| `ObjCollisionMap_CommitFrame` | `colmap_commit` | translated |  |
| `ObjCollisionMap_GetOffset` | `get_offset` | translated |  |
| `ObjCollisionMap_GetTileHeight` | `get_tile (vert)` | translated |  |
| `ObjCollisionMap_GetTileWidth` | `get_tile (horz)` | translated |  |
| `ObjCollisionMap_ProbeDistance` | `colmap_probe_distance` | translated |  |
| `ObjCollisionMap_ProbeEntry` | `probe_entry` | translated |  |
| `ObjCollisionMap_ProbeEntryLong` | `probe_entry_long` | translated |  |
| `ObjCollisionMap_ProbeForObj` | `colmap_probe_for_obj = g_dcp_col.obj_probe` | translated |  |
| `ObjCollisionMap_Register` | `colmap_register` | translated |  |
| `ObjRect_BoxCheck` | `rect_box_check` | translated |  |
| `ObjRect_CenterX` | inline in `Rope_OnHit` | translated | v2 |
| `ObjRect_CenterY` | inline in `Rope_OnHit` | translated | v2 |
| `ObjRect_CheckAllGroup` | `rect_check_all` | translated |  |
| `ObjRect_CheckFuncCall` | `check_func_call` | translated |  |
| `ObjRect_CheckGroup` | `check_group` | translated |  |
| `ObjRect_CheckInit` | `rect_check_init` | translated |  |
| `ObjRect_CheckOut` | `check_out` | translated |  |
| `ObjRect_FlagCheck` | `rect_flag_check` | translated |  |
| `ObjRect_FuncNoHit` | `rect_no_hit` | translated |  |
| `ObjRect_HitCenterX` | `rect_hit_center_x` | translated |  |
| `ObjRect_HitCenterY` | `rect_hit_center_y` | translated |  |
| `ObjRect_PointCheck` | - | not needed | no caller in the translated objects |
| `ObjRect_RegistGet` | `rect_regist_get` | translated | v2 (homing target search) |
| `ObjRect_Register` | `rect_register` | translated |  |
| `ObjRect_SetAtkDefFlags` | `rect_set_atkdef` | translated |  |
| `ObjRect_SetBox2D` | `rect_set_box2d` | translated |  |
| `ObjRect_SetBox3D` | `rect_set_box3d` | translated |  |
| `StageObj_Main` | `obj_main` | translated | draw step = animation only; the quad is emitted by objects_draw |
| `StageObj_ViewOutCheck` | `obj_view_out` | translated | n views; gObjManagerFlags & 0x200 combined box not used |
| `StageObj_PauseCheck` | `obj_pause_check` | translated |  |
| `StageObj_HandleCollider` | `obj_handle_collider` | translated |  |
| `StageObj_SetAnimation` | `obj_set_anim` | translated | model branch absent (2D objects) |
| `StageObj_Draw2D` | `obj_draw2d` | translated |  |
| `StageObj_Draw2DEx` | `obj_draw2d_ex + draw_spr (obj_draw.c)` | translated | rotation/scale (rot-scale OAM) not drawn; screen-shake global offset not applied |
| `StageObj_Load2dBAC` | `obj_load_bac` | translated | zone archive / loose file resolution done offline by tools/objdata.py |
| `StageObj_SetMapHitbox` | `(box_l..box_b assignment)` | translated |  |
| `StageObj_Destructor` | `obj_stageobj_dtor` | translated | nothing to free natively |
| `StageObj_SetOAMOrder` | `ObjSprite.order` | translated |  |
| `StageObj_SetOAMPriority` | `sprite flags bits 17-18` | translated |  |
| `StageObj_AllocSpritePalette` | - | not needed | atlases carry their colours |
| `StageObj_SetSharedSpriteVram` | - | not needed |  |
| `StageObj_IsTouched` | `StageObj_IsTouched` (obj_decor.c) | replaced | DS touch screen: always FALSE on the DC |
| `StageObj_Move` | `dcp_obj_move (player module)` | reused |  |
| `StageObj_Collision` | `dcp_obj_collision (player module)` | reused |  |
| `StageObj_HandleRide` | `dcp_handle_ride (player module)` | reused |  |

### Sprite animation (src/game/Graphics/Sprite)

| original | translated | status | notes |
|---|---|---|---|
| `AnimatorSprite_Init` | `spr_init` | translated |  |
| `AnimatorSprite_SetAnimation` | `spr_set_anim` | translated |  |
| `AnimatorSprite_ProcessAnimation` | `spr_process` | translated | runs on frames.bin + .hbx instead of the BAC command stream |
| `BAC_FrameGroupFunc_EndFrame` | `spr_process` | translated |  |
| `BAC_FrameGroupFunc_EndAnimation` | `spr_process (loop_frame)` | translated |  |
| `BAC_FrameGroupFunc_Callback_7` | `spr_process (ObjSprCb per .hbx event)` | translated |  |

### Rings (src/game/Objects, src/game/Effects)

| original | translated | status | notes |
|---|---|---|---|
| `RingManager_AttractRings` | `RingManager_AttractRings` (obj_ring.c) | translated | v2, called from objects_update while pf & 0x20000000 |
| `RingManager_BoxCheck` | `rect_box_check` | translated |  |
| `RingManager_BoxCheckArena1` | - | not translated | 3D boss arenas |
| `RingManager_BoxCheckArena2` | - | not translated | 3D boss arenas |
| `RingManager_BoxCheckArena3` | - | not translated | 3D boss arenas |
| `RingManager_CollideMap` | `ring_collide (map)` | translated |  |
| `RingManager_CollideMapAll` | - | not translated | zone 7 boss arena |
| `RingManager_CollideObjMap` | `ring_collide (solid objects)` | translated |  |
| `RingManager_Create` | `ring_init` | translated | 2D stage branch; boss-arena variants (3D rings) not translated |
| `RingManager_Destructor` | `objects_shutdown` | translated |  |
| `RingManager_DrawRing2D` | `ring drawn flag + obj_draw` | translated |  |
| `RingManager_DrawRing3D` | - | not translated | 3D boss arenas |
| `RingManager_DrawRingArena1` | - | not translated | 3D boss arenas |
| `RingManager_DrawRingArena2` | - | not translated | 3D boss arenas |
| `RingManager_DrawRingArena3` | - | not translated | 3D boss arenas |
| `RingManager_GetFlags` | - | not needed |  |
| `RingManager_Main` | `ring_main` | translated |  |
| `RingManager_PlaceRing` | `ring_place` | translated |  |
| `RingManager_ScatterRings` | `dcp_scatter_rings (player module) + hook_spawn_ring` | reused | mgr flag 2 set by the hook; spread level kept by the player module (see deviations) |
| `RingManager_SetFlags` | - | not needed |  |
| `RingManager_SpawnEffect2D` | `ring_fx_create` | translated |  |
| `RingManager_SpawnEffect3D` | - | not translated | 3D boss arenas |
| `RingManager_SpawnEffectArena1` | - | not translated | 3D boss arenas |
| `RingManager_SpawnEffectArena2` | - | not translated | 3D boss arenas |
| `RingManager_SpawnEffectArena3` | - | not translated | 3D boss arenas |
| `RingManager_SpawnRing` | `ring_spawn (g_dcp_hooks.spawn_ring)` | translated |  |
| `RingManager_SuckScatteredRings` | - | not translated | magnet barrier |
| `Ring_Create` | - | not translated | type 0xc9 ring object (boss stages) |
| `RingEffect2D_Create` | `ring_fx_create` | translated |  |
| `RingEffect2D_Main` | `ring_main (fx loop)` | translated |  |
| `RingEffect3D_Create` | - | not translated | 3D boss arenas |
| `RingEffect3D_Destructor` | - | not translated | 3D boss arenas |
| `RingEffect3D_Main` | - | not translated | 3D boss arenas |

### Gimmicks (src/game/Objects)

| original | translated | status | notes |
|---|---|---|---|
| `DashCT_Create` | `dashc_create_by_type + dashc_setup` | translated |  |
| `DashC_Action_Init` | `dashc_setup` | translated |  |
| `DashC_Create` | `dashc_create_by_type + dashc_setup` | translated |  |
| `DashC_CreateByType` | `dashc_create_by_type` | translated |  |
| `DashC_Destructor` | `obj_mapobject_dtor` | translated |  |
| `DashC_OnHit` | `dashc_on_hit` | translated |  |
| `DashC_State_Idle` | `dashc_state_idle` | translated |  |
| `DashP_Action_Init` | `dashp_create` | translated |  |
| `DashP_Create` | `dashp_create` | translated |  |
| `DashP_OnHit` | `dashp_on_hit` | translated |  |
| `DashP_State_Idle` | `dashp_state_idle` | translated |  |
| `GoalRingPart_Action_FollowRing` | `goal_follow` | translated |  |
| `GoalRingPart_Action_Sparkle` | `goal_sparkle` | translated |  |
| `GoalRingPart_State_FollowRing` | `goalpart_state_follow` | translated |  |
| `GoalRingPart_State_Sparkle` | `goalpart_state_sparkle` | translated |  |
| `GoalRing_Action_Idle` | `goal_action_idle` | translated |  |
| `GoalRing_Action_Spin` | `goal_action_spin` | translated |  |
| `GoalRing_Create` | `goalring_create` | translated |  |
| `GoalRing_CreatePart_ov000_0230a864` | `goal_sparkle` | translated |  |
| `GoalRing_CreatePart_ov000_0230ab48` | `goal_follow` | translated |  |
| `GoalRing_OnHit` | `goal_on_hit` | translated |  |
| `GoalRing_State_Idle` | `goal_state_idle` | translated |  |
| `GoalRing_State_Spin` | `goal_state_spin` | translated | NNS_SndPlayerSetTempoRatio dropped |
| `ItemBoxPart_Destructor_ov000_0230d5b8` | `itempart_dtor` | translated |  |
| `ItemBoxPart_State_FadeOut` | `itempart_state_fadeout` | translated |  |
| `ItemBoxPart_State_FollowBox` | `itempart_state_follow` | partial | VS item cycling of the icon not translated |
| `ItemBox_Action_Init` | `itembox_create` | translated |  |
| `ItemBox_Create` | `itembox_create` | translated |  |
| `ItemBox_CreatePart_ov000_0230d5d4` | `itembox_popup` | translated |  |
| `ItemBox_CreatePart_ov000_0230d930` | `itembox_icon` | translated |  |
| `ItemBox_GiveItem` | `itembox_give` | translated |  |
| `ItemBox_OnHit` | `itembox_on_hit` | translated |  |
| `ItemBox_State_CycleVsItem` | `itembox_state_cycle` | translated |  |
| `MapObj40_Create` | `mapobj40_create` | translated |  |
| `MapObj40_OnHit` | `mapobj40_on_hit` | partial | 0x7e, 0xa9, 0xb6, 0xb7, 0xc3-0xc5 need player/zone code not translated (logged); 0xc6 switch timer always 0 |
| `Marker_Action_Init` | `marker_create` | translated |  |
| `Marker_Action_InitPassCheck` | `marker_create (zones 7-8)` | partial | MarkerTimeList_Create (HUD) dropped |
| `Marker_Action_StageClear` | `marker_action_stage_clear` | translated |  |
| `Marker_Create` | `marker_create` | translated |  |
| `Marker_OnHit` | `marker_on_hit` | translated |  |
| `Marker_State_StageClearFade` | `marker_state_clear_fade` | translated |  |
| `Marker_State_Unk_ov000_02310730` | `marker_state_main` | translated |  |
| `Marker_State_WaitPlayerPass` | `marker_state_wait_pass` | partial | CheckpointTimePopup_SetTimeDigits (HUD) dropped |
| `Needle_Action_Idle` | `needle_action_idle` | translated |  |
| `Needle_Action_Slide` | `needle_action_slide` | translated |  |
| `Needle_Action_StartExtendDelay` | `needle_action_start_extend_delay` | translated |  |
| `Needle_Create` | `needle_create` | translated |  |
| `Needle_State_ExtendDelay` | `needle_state_extend_delay` | translated |  |
| `Needle_State_Idle` | `needle_state_idle` | translated |  |
| `Needle_State_Slide` | `needle_state_slide` | translated |  |
| `Spring_Action_Activate` | `spring_action_activate` | translated |  |
| `Spring_Action_Idle` | `spring_action_idle` | translated |  |
| `Spring_Create` | `spring_create` | translated |  |
| `Spring_OnHit` | `spring_on_hit` | translated |  |
| `Spring_State_Activated` | `spring_state_activated` | translated |  |
| `Spring_State_Idle` | `spring_state_idle` | translated |  |

### Enemies and effects

| original | translated | status | notes |
|---|---|---|---|
| `EnemyPawnGroup_Action_Attack` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_Action_AttackSpawnEggBishop` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_Action_AttackSpawnEggPawnM` | `pawn_action_attack_spawn_m` | translated |  |
| `EnemyPawnGroup_Action_AttackSpawnEggSf` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_Action_AttackSpawnGunHunter` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_Action_Bounced` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_Action_Brake` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_Action_Chase` | `pawn_action_chase` | translated |  |
| `EnemyPawnGroup_Action_Dash` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_Action_JumpAttack` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_Action_LongAttack` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_Action_LongAttackAlt` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_Action_StartCharge` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_Action_StartChase` | `pawn_action_start_chase` | translated |  |
| `EnemyPawnGroup_Action_StartDropIn` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_Action_StartEntrance` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_Action_StartJumpAttack` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_Action_StartStomp` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_Action_Turn` | `pawn_action_turn` | translated |  |
| `EnemyPawnGroup_Action_Walk` | `pawn_action_walk` | translated |  |
| `EnemyPawnGroup_Create` | `pawn_create` | partial | types 0x00/0x14/0x01 with p9 == 0 and no 0x40 entrance flag; other types return NULL |
| `EnemyPawnGroup_Destructor` | `obj_mapobject_dtor` | translated |  |
| `EnemyPawnGroup_OnDetect` | `pawn_on_detect` | partial | EffFound_Create now created (v2); actions of untranslated types absent |
| `EnemyPawnGroup_OnHit_Box2_Bounce` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_OnHit_Box2_PushPlayer` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_SpawnChild` | `pawn_spawn_child` | partial | p9 parent link part not translated |
| `EnemyPawnGroup_State_Attack` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_State_AttackSpawnEggBishop` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_State_AttackSpawnEggPawnM` | `pawn_state_attack_spawn_m` | translated | the child (type 0xdf EggPawnM_Create) is translated in v2 |
| `EnemyPawnGroup_State_AttackSpawnEggSf` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_State_AttackSpawnGunHunter` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_State_Bounced` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_State_Brake` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_State_Charge` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_State_Chase` | `pawn_state_chase` | translated | @0x23A parent offset absent |
| `EnemyPawnGroup_State_ChaseWindup` | `pawn_state_chase_windup` | translated |  |
| `EnemyPawnGroup_State_Dash` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_State_DropIn` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_State_JumpAttack` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_State_JumpAttackWindup` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_State_LongAttackAlt` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_State_LongAttackSpin` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_State_Stomp` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_State_Turn` | `pawn_state_turn` | translated |  |
| `EnemyPawnGroup_State_Walk` | `pawn_state_walk` | partial | LongAttack/Attack branches (types 7/0xa/0xd/0x15) and the moving-parent offset (@0x23A) not translated |
| `Enemy_DisableCollidersSetAnim` | `obj_disable_colliders_set_anim` | translated |  |
| `EffZakoBombBurst_Create` | `eff_zako_bomb_burst` | translated |  |
| `EffZakoBombPuff_Create` | `EffZakoBombPuff_Create` (obj_effects2.c) | translated | v2 |
| `EffZakoBomb_Create` | - | not needed | no caller in the translated objects |
| `EffZakoDebris_Action_Init` | `eff_zako_debris` | translated |  |
| `EffZakoDebris_Create` | `eff_zako_debris` | translated |  |
| `EffZakoDebris_State_Main` | `debris_state_main` | translated |  |
| `Effect_SetMoveUntilAnimEndState` | `eff_set_move_until_anim_end` | translated |  |
| `Effect_State_MoveUntilAnimEnd` | `eff_state_move_until_anim_end` | translated |  |

### Player functions owned or completed here (src/game/Player)

| original | translated | status | notes |
|---|---|---|---|
| `Player_Collide` | `obj_player_collide` | translated |  |
| `Player_SpriteCallback` | `player_sprite_cb` | translated |  |
| `Player_OnHurt` | `obj_player_on_hurt -> dcp_on_hurt` | reused | hurt box def |= 5 added here (player module does not model box flags) |
| `Player_SetAnimation` | `hook_anim (sprite part)` | translated | sprite index from data_ov000_02368d14; boxes disabled; model part = integrator |
| `Player_SetAnimSpeedFromVelocity` | `hook_anim_speed (sprite part)` | translated |  |
| `Player_Action_AttackBounce` | `obj_player_attack_bounce` | translated |  |
| `Player_Action_AttackRecoil` | `obj_player_attack_recoil` | translated |  |
| `Player_Action_DashRing` | `obj_player_dash_ring` | translated | Camera_UnlockX/Y = camera module |
| `Player_Action_GimmickLaunch` | `gimmick_launch` | translated | EffAirBubble_Spawn dropped |
| `Player_StartLaunchTrick` | `obj_player_start_launch_trick` | translated |  |
| `Player_SetSourceMapObject` | `obj_player_set_source` | translated |  |
| `Player_Action_SpringLaunchAlt` | `dcp_act_launch + sound 0x72` | assumed | called without velocity arguments: r1/r2 still hold the spring velocity |

### 5b. objdata v2 additions (reuse classification)

Classification: reused-as-is (copied, only types renamed), modified (copied; offset structs -> named fields, DS calls -> native/hook), substantially rewritten (same logic, restructured), new (native code without an original).

| area | original | translated | classification | notes |
|---|---|---|---|---|
| NitroSDK fx | `MTX_Identity33_, MTX_RotX33_, MTX_RotY33_, MTX_RotZ33_` | obj_fx.c | substantially rewritten | SDK asm (stores) written as C stores |
| NitroSDK fx | `MTX_Concat33, MTX_MultVec33` | obj_fx.c | reused-as-is | copied |
| NitroSDK fx | `FX_DivS32` | obj_fx.c | substantially rewritten | hardware divider -> C division (same truncation) |
| ObjectSystem | `ObjShift_Set, ObjShift_SetFromRef` | obj_fx.c | reused-as-is | copied |
| ObjectSystem | `ObjRect_RegistGet` | obj_rect.c rect_regist_get | modified |  |
| ObjectSystem | `ObjRect_CenterX, ObjRect_CenterY` | inline in Rope_OnHit | modified |  |
| ObjectSystem | `MapObject_SpawnFreedAnimal` | obj_enemy.c | modified | now spawns the DecAnimal |
| ObjectSystem | `MapObject_OnHit_Default (AttackHitbox branch)` | obj_enemy.c | modified | branch restored |
| ObjectSystem | `StageObj_Draw2DEx (rot-scale, piece table)` | obj_anim.c + obj_draw.c | substantially rewritten | deviations 14, 15 |
| ObjectSystem | `StageObj_IsTouched` | obj_decor.c | substantially rewritten | always FALSE (no touch screen) |
| Stage | `StageEvent_SpawnDecorationsInBlock` | obj_decor.c spawn_decorations_in_block | modified |  |
| Stage | `PlayerStart_Create` | obj_shared.c | modified | writes g_obj_stage / g_dcp_world check_x/y |
| Camera | `Camera_CheckPosInView, Camera_CheckPosInViewLatched, Camera_AddAreaObject, Camera_RemoveAreaObject` | obj_shared.c | modified | on g_obj_camera; dual-screen branch only |
| Decorations | `Decoration_AllocSlot, Decoration_FreeSlot, Decoration_Spawn, Decoration_InitFromMap, Decoration_In_Default, Decoration_SpriteCallback_Default, Decoration_Collide_Default, Decoration_Destructor` | obj_decor.c | modified |  |
| Decorations | `DecGroup0F_Create` | obj_decor.c | modified | palette rows dropped (deviation 16) |
| Decorations | `DecVine_Create, DecVine_OnHit` | obj_decor.c | modified |  |
| Decorations | `DecBird_Create, DecBird_OnHit, DecBird_Action_FlyAway, DecBird_State_FlyAway` | obj_decor.c | modified |  |
| Decorations | `DecGroup35_Create, DecGroup35_OnHit, DecGroup35_State_Main, DecGroup35_CreateFallingDebris, DecGroup35Piece_Create, DecGroup35Piece_State_Flutter` | obj_decor.c | modified | EffWaterGmk splash (0x51/0x52) -> effect hook |
| Decorations | `DecGroup35_SpawnPieceBurst` | obj_decor.c | substantially rewritten | four calls as a loop; RNG drawn right to left |
| Decorations | `DecAnimal_Create, _Destructor, _Action_Idle, _Action_Move, _Action_Leave, _State_Idle, _State_Move, _State_Leave, _ChooseNextAction, _CheckTouch, _UpdateMove, _UpdateLeaveTimer` | obj_decor.c | modified | sprintf("ac_dec_animal%02d") -> slot ANIMAL01 + n |
| Effects | `EffZakoBombPuff_Create, EffFound_Create, Effect_SetFollowParentState, Effect_State_FollowParent, EffBrakeSmoke_Create` | obj_effects2.c | modified | second-screen sprite dropped |
| Enemies | `EggPawnM_Create, Enemy_Action_StartPopOnContact, Enemy_State_PopOnContact` | obj_leafstorm.c | modified |  |
| Enemies | `EnemyFlapperGroup_Create, _Action_Hover, _State_Hover, _Action_Patrol, _State_Patrol, _Action_PatrolTurn, _State_PatrolTurn, _UpdateHover` | obj_flapper.c | modified | types 2 and 4; Create returns NULL for 0x0b/0x0c/0x10..0x12 |
| Enemies | `EnemyFlapperGroup_FireShot*, _StartSwoop, _SwoopOut/Return, _OnHit_Box2` | - | not translated | shooting / swooping types |
| Leaf Storm | `Trampoline_Create, _OnHit, _Action_Idle, _State_Idle, _Action_Bounce, _State_Bounce` | obj_leafstorm.c | modified | NULL test moved before the read of t->unk2A |
| Leaf Storm | `BendStick_Create, _OnHit, _Action_Init, _State_Wobble` | obj_leafstorm.c | modified |  |
| Leaf Storm | `CorkscrewTrigger_Create, _OnHit` | obj_leafstorm.c | modified |  |
| Leaf Storm | `RockBreak_Create, _OnHit, _Action_Idle, _State_Idle, _CreatePart_ov000_023136ec, RockBreakPart_Action_Fly, _State_Fly` | obj_leafstorm.c | modified |  |
| Leaf Storm | `RopeC_Create, _OnHit, _SpawnParts, _Action_Init, _State_Main` | obj_leafstorm.c | modified | model -> objects_model_get (deviation 17) |
| Leaf Storm | `RopeCLand_Create, _Action_Idle, _State_Idle, RopeCSpring_Create, _Action_Idle, _State_Idle, _Action_Active, _State_Active` | obj_leafstorm.c | modified |  |
| Objects | `Rope_Create, _OnHit, _Action_Init, _State_Attached` | obj_shared.c | modified |  |
| Objects | `Rope_Draw` | obj_shared.c | substantially rewritten | points recorded for the multi-draw (ObjSprite.multi) |
| Objects | `JumpPedestal_Create, _Destructor, _Action_Init, _State_WaitPlayer, _CheckLaunch` | obj_shared.c | modified | ObjData_Load(.df) -> obj_df |
| Objects | `WallBreak_Create, _OnHit, _Action_Idle, _State_Idle, _Action_Break, _State_Broken, _DrawBroken` | obj_shared.c | modified | DrawBroken via the piece table (deviation 14) |
| Objects | `IronBall_Create, _Destructor, _Action_Idle, _State_Idle, _Action_StartRoll, _OnHitStartRoll, _OnHit_Unk_ov000_0230ce24, _State_Roll, _CreatePart_ov000_0230cd00, IronBallPart_Action_Follow, _State_FollowParent` | obj_shared.c | modified | looping sound simplified |
| Objects | `GmkSpecial_Create, _Destructor, _Draw, _OnHit, _Action_Init, _State_Main` | obj_shared.c | modified | OnHit requires a player (the original's NULL test is inverted) |
| Objects | `GmkLand_Create, _Destructor, _Draw, _InitOscillation, _UpdateOscillation, _Action_InitOscillating, _State_Oscillate, _Action_InitCrumbling, _State_WaitCrumble, _Action_StartCrumble, _State_Crumble, _Action_InitSpawner, _State_SpawnPeriodic, _Action_Unk_ov000_0230f4fc, _State_Unk_ov000_0230f4c8` | obj_shared.c | modified | unk24 frame counter (deviation 19) |
| Objects | `GmkLandAA_Create, _Action_Init, _State_Decelerate` | obj_shared.c | modified |  |
| Objects | `WaterLevelTrigger_Create, _Destructor, _Action_InitBySide, _State_SetBySide, _State_ApplyOnce, _State_Idle` | obj_shared.c | modified | on g_obj_camera |
| Objects | `MapObj70_Create, _Destructor, _Action_Init, _Action_ApplyInitialCameraLimit, _State_SetCameraLimitBySide, _State_Idle` | obj_shared.c | modified | on g_obj_camera |
| Objects | `MapObj73_Create, _Destructor, _OnHit, _Action_Init, _State_Main` | obj_shared.c | modified |  |
| Objects | `MapObj73_Unk_ov000_0230379c, _02303930, _02303ac4, _02303c58` | obj_shared.c mo73_dir | substantially rewritten | four mirrored copies -> one function with (vertical, far side) |
| Objects | `MapObj75_Create, _Destructor, _Action_Init, _IsPlayerInArea, _ApplyCameraBounds, _PlayerState_ReleaseBounds, _PlayerState_WaitEnter, _PlayerState_WaitExit, _State_Main` | obj_shared.c | modified | on g_obj_camera |
| Objects | `RingManager_AttractRings` | obj_ring.c | modified |  |
| Objects | `RingManager_SuckScatteredRings` | - | not translated | Duct (zone 7) |
| Player | `AttackHitbox_Create, _SetBoostState, _State_Boost, _State_Follow` | obj_player.c | modified | via g_dcp_hooks.effect (DCP_FX_ATTACK_HITBOX_BOOST) |
| Player | `AttackHitbox_SetJetCatapultState, _State_JetCatapult` | - | not translated | JetCatapult (zone 5) |
| Player | `Player_Action_Rope, Player_State_Rope, Player_Action_RopeC, Player_State_RopeC` | obj_pgimmick.c | modified |  |
| Player | `Player_Action_BendStick, Player_State_BendStick, Player_Action_BendStickLaunch` | obj_pgimmick.c | modified |  |
| Player | `Player_Action_CorkscrewPath, Player_State_CorkscrewPath, Player_MoveAlongCorkscrew, Player_MoveAlongCorkscrewPath` | obj_pgimmick.c | modified |  |
| Player | `Player_Action_GmkSpecial, Player_State_GmkSpecial, Player_Action_GmkSpecialEnter, Player_State_GmkSpecialEnter` | obj_pgimmick.c | modified | EffSpecial/EffTrick -> effect hook; special-stage start -> stage_exit hook |
| Player | `Player_Action_PedestalLaunch, Player_Action_GimmickLaunch (exported)` | obj_pgimmick.c / obj_player.c | modified |  |
| Player | `Player_Action_Grind, Player_State_Grind, Player_Action_GrindTrick, Player_CheckStartGrind` | obj_pgimmick.c | modified | EffGrind/EffTrickSparkle -> effect hook |
| Player | `Player_FindHomingTarget, Player_GiveMagnet` | obj_pgimmick.c | modified | Magnet_Create -> item hook; ItemBox_GiveItem kind 7 now calls Player_GiveMagnet |
| native | `obj_res_ensure, preload_resources, type_slots, dec_preload, obj_df, load_objdata v2, camera_init, objects_model_count/get, objects_set_grind_check, obj_screen_shake, quad4 / draw_spr_at` | obj_manager.c, obj_decor.c, obj_shared.c, obj_draw.c | new |  |

### 5c. objdata v3 additions: Water Palace and Mirage Road (reuse classification)

Same classification as 5b. "Exercised" = reached by a host-test check
(section 7: play = by moving the player there, direct = the test put the
player into the trigger box with the state the OnHit needs); "created" =
only created (spawn, coverage) and run for some frames without a check of
its behaviour.

| area | original | translated | classification | test | notes |
|---|---|---|---|---|---|
| Effects | `EffZakoBomb_Create` | obj_water.c | modified | exercised (mine, Kuragen bomb) |  |
| Effects | `EffAirBubble_Create, _State_Rise, EffAirBubble_Spawn` | obj_water.c | modified | exercised (play) | bubbles while under water |
| Effects | `EffDrownCount_Create, _State_FollowPlayer, _Spawn` | obj_water.c | modified | exercised (play: digits 5..0) | FixMoji digits from FIX_MOJI |
| Effects | `EffGmkBubble_Create, _State_Main` | obj_water.c | modified | created (spawned when the wheel releases; not checked) |  |
| Effects | `EffWaterGmk_Create, EffWaterGmk_CreateSplash, EffWaterRunSplash_Create, EffWaterSplash_Create, EffWaterSplash_Spawn` | obj_water.c | modified | exercised (entry splash, run spray) |  |
| Effects | `EffRipple3D_SpawnForPlayer, ScreenFlash_Create, StageJingle_Create, Stage_SelectBGMLayer` | - | not translated | hook calls counted | effect hook kinds |
| Water Palace | `WaterRunTrigger_Create, _OnHit` | obj_water.c | modified | exercised (play from the 0x80 slide) |  |
| Water Palace | `MapObj7D_Create, _OnHit` (water current) | obj_water.c | modified | exercised (direct) | gravity 0x54 inside |
| Water Palace | `MapObj80_Create, _OnHit` (water slide) | obj_water.c | modified | exercised (play) |  |
| Water Palace | `LaunchZone_Create, _OnHit` | obj_water.c | modified | exercised (direct) | type check added (deviation 30) |
| Water Palace | `LandUnrest_Create, _Action_Init, _State_Tilt` | obj_water.c | modified | exercised (play) | pre-player task 0xf6 |
| Water Palace | `Wheel2D_Create, _OnHit, _State_Main` | obj_water.c | modified | exercised (direct) | model view mod__gmk_wheel__gmk_wheel; type check added |
| Water Palace | `Mine_Create, _OnHit, _OnHitExplode, _State_Main, _Action_Drop, _State_Drop, _Explode, _Draw, MineDigit_Create, _State_FollowParent` | obj_water.c | modified | Mine exercised (direct); MineDigit created | Mine_Draw multi-sprite via ObjSprite xspr |
| Water Palace | `Bubble_Create, _OnHit, _Action_Spawn, _State_Grow, _Action_Float, _State_Float, _ProbeCorners` | obj_water.c | modified | exercised (direct + spawner) | NULL guard in OnHit (deviation 29); 0xe9/0xea created only |
| Water Palace | `BubbleSpawner_Create, _OnHit_Box1, _State_Emit, _SpawnBubble` | obj_water.c | modified | exercised (play) |  |
| Water Palace | `WaterJet_Create, _OnHit, _Action/_State_Wait, _Prepare, _Spout` | obj_water.c | modified | exercised (direct) | 0x85..0x87 |
| Enemies | `Kuragen_Create, _OnDetect, _State_Float, _State_FloatTurn, _Action_Float, _State_BombDrop, _UpdatePulse, _SpawnBomb` | obj_water.c | modified | exercised (direct: bomb; pulse timing) | prio 0x501; NULL guard in SpawnBomb |
| Enemies | `KuragenBomb_Create, _Destructor, _OnHit_Box1, _Attach, _Action/_State_Hold, _FollowCarrier, _Action_Drop, _State_Fall, _Action_Explode` | obj_water.c | modified | exercised (direct) |  |
| Decorations | `DecShared_Create, _Destructor, _State_Main, _State_PingParent` (+ native `dec_shared_ping`) | obj_water.c | substantially rewritten | exercised (fish: shared frames) | shared VRAM -> ObjSprite.vram (deviation 25) |
| Decorations | `DecGroup41_Create, _OnHit, _ChooseNextAction, _Action/_State_Cruise, _Turn, _Dart, _Sink, _Flee, _State_FollowParentPriority, DecFishPart_Create, _Action/_State_Swim, _Flee` | obj_water.c | modified | fish flee exercised (play); fountain, waterfall, algae, coral created | 0x41..0x44, 0x4f, 0x50, 0x90, 0x9d..0xae, 0xf1 |
| Decorations | `DecGroup4D_Create, _State_Main` | obj_water.c | modified | created | 0x4d, 0x4e |
| Player | `Player_ApplyAutoRunAcceleration` | obj_pwater.c | modified | exercised (water run) | belongs to the player module (2.2 item 10) |
| Player | `Player_Action_WaterRun, _State_WaterRun` | obj_pwater.c | modified | exercised (play) |  |
| Player | `Player_Action_ZoneLaunch, _State_ZoneLaunch` | obj_pwater.c | modified | exercised (direct) |  |
| Player | `Player_Action_BreatheAirBubble, _State_BreatheAirBubble` | obj_pwater.c | modified | exercised (direct; ANIM_DONE set by the test) |  |
| Player | `Player_Action_Wheel2D, _State_Wheel2D, Player_Action_MapObj80, _State_MapObj80, Player_Action_MapObj40, _State_MapObj40` | obj_pwater.c | modified | exercised (wheel direct, slide play, slope 0x7e direct) |  |
| Player | `Player_Action_AttachToObject, Player_State_AttachedToObject` | obj_pwater.c | modified | exercised (rail) |  |
| Player | `Player_UpdateUnderwater` | obj_pwater.c | modified | exercised (play: entry, countdown, drowning, exit) | zone index 1 only; Ripple3D/BGM/jingle via hook |
| Player | `Player_In` gimmick-destroyed check | obj_pwater.c obj_player_check_gimmick | modified | exercised (z11 special ring, wheel) | runs in objects_frame_begin |
| Mirage Road | `GmkRail_Create, _OnHit, _State_Move, _StartFromPlayerSpeed` | obj_mirage.c | modified | exercised (direct) | GMK_RAIL6 when W.zone == 5 |
| Mirage Road | `Cargo_Create, _OnHit, _State_Stopped, _Action_Stop, _State_Move, _Action_StartMove, _Draw` | obj_mirage.c | modified | exercised (direct) | StartMove takes r1 (deviation 33); gmk_cargo.df; prio 0xf6 |
| Mirage Road | `Hand2d_Create, _Destructor, _State_Main` | obj_mirage.c | modified | exercised (play) | model view mod__gmk_hand__gmk_hand |
| Mirage Road | `GmkTable_Create, _Destructor, _State_Main, _State_Released, _Action_Release, _UpdateBob` | obj_mirage.c | modified | exercised (play: z31 3540 frames, z32 5000 frames) | model view mod__gmk_table__gmk_table; sound once; NULL guard on ring write |
| Mirage Road | `EnemyRoom_Register/_Unregister/_Get/_SetActive/_ClearActive, Gimmick_GetRoomIndex, Gimmick_CheckRoomResetOnDeath` | obj_mirage.c | modified | exercised (room cleared) |  |
| Mirage Road | `MapObj8C_Create, _Destructor, _State_WaitPlayerEnter, _Action_StartRoom, _State_RoomActive, _Action_RoomCleared, _State_FinishRoom, MapObj8C_CreateByType` | obj_mirage.c | modified | exercised (play; enemies removed by the test) | HUD via OBJ_FX_ENEMY_ROOM; prio 0x4f6 |
| Mirage Road | `MapObj8D_Create, _State_WaitSpawn, _State_WaitEnemyDefeated, _State_Done` | obj_mirage.c | modified | exercised (spawned with entrance) |  |
| Mirage Road | `GmkWall_Create, _State_WaitRoomStart, _State_MoveDown, _Action_Closed, _State_Closed, _State_MoveUp, _State_Opened` | obj_mirage.c | modified | close exercised ("gates moved"); reopen created |  |
| Mirage Road | `MapObj8F_Create, _State_SetCameraBound, _State_CameraLock, _State_CameraLocked, _Action_StartCameraRelease, _State_CameraRelease, _State_SetRoomBound, _State_RoomBoundDone` | obj_mirage.c | modified (StartCameraRelease substantially rewritten from asm) | room bounds read by the test; camera values not checked | deviation 34 |
| Camera | `Camera_GetTrackedPlayerOffset` | obj_mirage.c | modified | created | on g_obj_camera |
| Mirage Road | `SandSlider_Create, _OnHit, _CreatePart` | obj_mirage.c | modified | exercised (direct, 0x90 and 0x91) |  |
| Mirage Road | `Rock_Create, _State_Wait, _State_Fall` | obj_mirage.c | modified | exercised (sand ship: 17 / 28 rocks) | falls onto objects only (flag 0x1000) |
| Enemies | `EggBishop_Create, _State_Spawned` | obj_mirage.c | modified | spawn exercised (throw); flight created |  |
| Decorations | `DecGroup45_Create` | obj_mirage.c | modified | created | 0x45..0x4c, 0x53..0x66 |
| Effects | `EffSand_Create, EffSandSlide_Create, EffShadow_Create, _State_Main, EffEntrance_Create, _State_ScaleInOut` | obj_mirage.c | modified | EffSand, EffEntrance exercised; EffShadow created (sand ship) | EffEntrance start scale (deviation 32) |
| Player | `Player_Action_Cargo, _State_Cargo, Player_Action_Hand2D, _State_Hand2D` | obj_pmirage.c | modified | exercised |  |
| Player | `Player_Action_SandSlide, _State_SandSlide, Player_Action_SandSlider, _State_SandSlider, Player_Action_SandSlider90, _State_SandSlider90` | obj_pmirage.c | modified | exercised (direct) |  |
| Player | `Player_AttachGimmickTable, _NoInput, _Released, Player_DetachGimmick` | obj_pmirage.c | modified | exercised (sand ship) | FixVs_CreateAbovePlayer dropped (log only) |
| Enemies | `EnemyPawnGroup_State_Walk (full: ride-relative u23A, LongAttack/Attack branches)` | obj_enemy.c pawn_state_walk | modified | exercised (z31/z32 pawns, bishop, hammer) |  |
| Enemies | `EnemyPawnGroup_State/_Action_StartCharge` (Egg Lancer, type 3) | obj_enemy.c | modified | exercised (play) |  |
| Enemies | `EnemyPawnGroup_State/_Action_StartStomp` (Egg Hammer, type 8) | obj_enemy.c | modified | exercised (play) | shake via hook |
| Enemies | `EnemyPawnGroup_State_LongAttackSpin, _Action_LongAttack, _State/_Action_Attack, _State/_Action_AttackSpawnEggBishop` | obj_enemy.c | modified | throw exercised; spin created | LongAttackAlt (zone 5) not translated |
| Enemies | `EnemyPawnGroup_State/_Action_StartDropIn` (0x14/0x15 on the sand ship) | obj_enemy.c | modified | exercised on z31 (landed); not spawned on z32 | EffShadow |
| Enemies | `StageObj_State_EntranceDrop, EnemyPawnGroup_Action_StartEntrance` | obj_enemy.c | modified | exercised (enemy room) | EffEntrance |
| Enemies | `EnemyPawnGroup_SpawnChild (p9 part), EnemyPawnGroup_OnDetect (cases 3, 7, 0x15, 8), EnemyPawnGroup_Create (types 0, 1, 3, 7, 8, 0x14, 0x15)` | obj_enemy.c | modified | exercised | other types return NULL (zones 4..7) |
| Stage | `StageEvent_SpawnInRect` | obj_manager.c objects_spawn_rect | substantially rewritten (from asm) | exercised (enemy room) | flags 0xf0 |
| Gimmicks | `MapObj40` cases 0x7e (slope) and 0xa9 (sand area: EffSand every 8 frames) | obj_gimmicks.c | modified | exercised (direct) | 0xb6, 0xb7, 0xc3..0xc5 still missing |
| Bosses | `EggBishop3D, EggPawn3D, Rock_3` (3D boss variants), boss objects | - | not translated |  | removed from the slot list |
| native | `objects_set_underwater_check, objects_spawn_rect, dec_shared_reset, mirage_reset, obj_pawn_in_entrance, ObjSprite.vram / xspr, objects_model_count/get (o->model), objdata v3 slots` | obj_manager.c, obj_anim.c, obj_draw.c, obj_shared.c, tools/objdata.py | new |  |  |

### 5d. objdata v4 additions: zones 4..7 (reuse classification)

Sources: `src/game/Zone/Night_Carnival/`, `Huge_Crisis/`, `Altitude_Limit/`,
`Dead_Line/` (Objects, Decorations, Effects, Enemies), `src/game/Player/`
(the gimmick states), `src/game/Enemies/EnemyPawnGroup_*` and
`EnemyFlapperGroup_*` (new types). Classes: **modified** = the decompiled C
adapted (work struct -> named `Obj` fields with the offsets in comments, DS
services dropped or sent to hooks, names kept); **modified (lifted C)** =
the source is C lifted from the ROM instruction stream by templates.py
(verified by match.py), translated the same way; **native helper** = no
single original function (resets of module statics, small shared bits of
several originals). No function of these zones exists only as asm in the
repository, so none had to be rewritten from a listing. Counts: 144
modified, 43 modified (lifted C), 14 native helpers in the nine new files.
`obj_spd_accel_toward` is ObjSpd_AccelToward (renamed like the other
ObjSpd helpers); `pipe_arrive`, `mo13_driver`, `cannon_sprite`,
`swing_sprite`, `sky_sprite`, `crane_slide_blocks`, `switch_press` are
repeated blocks of the listed originals; `fade` wraps BrightnessFade_Create.

Additions in shared files (all **modified** unless noted):

| file | functions |
|---|---|
| `obj_enemy.c` | EnemyPawnGroup types 9, 0x0a, 0x0d, 0x0e, 0x0f: create cases (BACs, coll2 PushPlayer / Bounce, f1F4 0x20, vitality, Egg Bunny map hitbox), `_State/_Action_LongAttackAlt`, `_AttackSpawnEggSf`, `_AttackSpawnGunHunter`, `_Dash`, `_Brake`, `_Bounced`, `_JumpAttack` (+Windup, +Start), `_OnHit_Box2_Bounce`, `_OnHit_Box2_PushPlayer`, OnDetect cases |
| `obj_flapper.c` | EnemyFlapperGroup types 0x0b, 0x0c, 0x10, 0x11, 0x12: `_OnHit_Box2`, `_FireShot`, `_State_FireShot_ov000_022ffcac / _02300338 / _02300484`, `_Action/_State_SwoopOut`, `_SwoopReturn`, Math_LerpFx32; shots EggFlapperLaser (0xe7), FlapperCannon (0xf0), EggBombHawk (0xeb, + Explode, ExplodeOnContact); EffEntrance native |
| `obj_enemy2.c` | EggSf_Create (0xe6), GunHunter_Create + `_State_CheckGround` (0xec, lifted C) |
| `obj_player.c` | AttackHitbox_State_JetCatapult, AttackHitbox_SetJetCatapultState; `objects_player_init_state` (Player_InitState box part) |
| `obj_gimmicks.c` | MapObj40_OnHit cases 0xb6 / 0xb7 (glider sequence start / end), 0xc3 / 0xc4 (dark area in / out, ZoneFx hook), 0xc5, 0xc6 (switch-timer layer areas; 0xc6 falls through into 0x59 as in the original); MapObj40_Create case 0xb6 / 0xb7 box |
| `obj_decor.c`, `obj_water.c` | create table rows for DecGroupAF, DecGroup95, DecGroup67, DecArrow; DecGroup4D case 0xcf (crane slide decoration) |
| `obj_rect.c` | ObjRect_PointCheck (`rect_point_check`) |
| `obj_fx.c` | FX_Sqrt (NitroSDK, 64-bit integer square root, result as the hardware root unit gives it), ObjAngle_MoveToward16 |
| `obj_ring.c` | RingManager_SuckScatteredRings (Duct), RingManager_Destructor (`ring_destroy`), RingPart.sucker |
| `obj_manager.c` | create table rows for all v4 types, type -> slot preloads, StageObj_HandleRide @0x130 part (`obj_handle_ride_link`), Stage_SaveEventState / Stage_SaveRestartCamera / Stage_RestoreEventState / Stage_RestoreRestartCamera (section 2.5) |

Per-file tables of the new files (generated from the sources; "source" is
the path under `src/game/`):

#### `obj_carnival.c` (43 functions)

| original | source | class |
|---|---|---|
| carnival_reset | - | native helper (no single original function) |
| obj_switch_timers | - | native helper (no single original function) |
| obj_spd_accel_toward | - | native helper (no single original function) |
| crane_slide_blocks | - | native helper (no single original function) |
| CraneSlide_State_Unk_ov000_023064d8 | Zone/Night_Carnival/Objects/CraneSlide_State_Unk_ov000_023064d8.c | modified |
| CraneSlide_OnHit | Zone/Night_Carnival/Objects/CraneSlide_OnHit.c | modified (lifted C) |
| CraneSlide_Create | Zone/Night_Carnival/Objects/CraneSlide_Create.c | modified |
| NeonRoute_Action_StartMove | Zone/Night_Carnival/Objects/NeonRoute_Action_StartMove.c | modified (lifted C) |
| NeonRoute_Action_Stop | Zone/Night_Carnival/Objects/NeonRoute_Action_Stop.c | modified (lifted C) |
| NeonRoute_State_WaitSwitch | Zone/Night_Carnival/Objects/NeonRoute_State_WaitSwitch.c | modified (lifted C) |
| NeonRoute_State_Move | Zone/Night_Carnival/Objects/NeonRoute_State_Move.c | modified |
| NeonRoute_Draw | Zone/Night_Carnival/Objects/NeonRoute_Draw.c | modified |
| NeonRoute_Create | Zone/Night_Carnival/Objects/NeonRoute_Create.c | modified |
| ShooterPart1_State_Main | Zone/Night_Carnival/Objects/ShooterPart1_State_Main.c | modified |
| Shooter_CreatePart1 | Zone/Night_Carnival/Objects/Shooter_CreatePart1.c | modified |
| Shooter_CreatePart2 | Zone/Night_Carnival/Objects/Shooter_CreatePart2.c | modified |
| Shooter_State_Main | Zone/Night_Carnival/Objects/Shooter_State_Main.c | modified |
| Shooter_Create | Zone/Night_Carnival/Objects/Shooter_Create.c | modified |
| SwingSky_State_Main | Zone/Night_Carnival/Objects/SwingSky_State_Main.c | modified |
| SwingSky_OnHit | Zone/Night_Carnival/Objects/SwingSky_OnHit.c | modified |
| SwingSky_Draw | Zone/Night_Carnival/Objects/SwingSky_Draw.c | modified |
| sky_sprite | - | native helper (no single original function) |
| SwingSky_Create | Zone/Night_Carnival/Objects/SwingSky_Create.c | modified |
| SwitchTimer_Destructor | Zone/Night_Carnival/Objects/SwitchTimer_Destructor.c | modified |
| SwitchTimer_Create | Zone/Night_Carnival/Objects/SwitchTimer_Create.c | modified |
| SwitchTimer_Main | Zone/Night_Carnival/Objects/SwitchTimer_Main.c | modified (lifted C) |
| Switch_CreatePart | Zone/Night_Carnival/Objects/Switch_CreatePart.c | modified |
| switch_press | - | native helper (no single original function) |
| Switch_State_Horizontal | Zone/Night_Carnival/Objects/Switch_State_Horizontal.c | modified |
| Switch_State_Vertical | Zone/Night_Carnival/Objects/Switch_State_Vertical.c | modified |
| Switch_Create | Zone/Night_Carnival/Objects/Switch_Create.c | modified |
| DecGroupAF_State_SwitchVisible | Zone/Night_Carnival/Decorations/DecGroupAF_State_SwitchVisible.c | modified |
| DecGroupAF_OnHit | Zone/Night_Carnival/Decorations/DecGroupAF_OnHit.c | modified |
| DecGroupAF_State_ClearTouched | Zone/Night_Carnival/Decorations/DecGroupAF_State_ClearTouched.c | modified |
| DecGroupAFChild_State_FollowParent | Zone/Night_Carnival/Decorations/DecGroupAFChild_State_FollowParent.c | modified |
| DecGroupAFChild_Draw | Zone/Night_Carnival/Decorations/DecGroupAFChild_Draw.c | modified (lifted C) |
| DecGroupAF_CreateChildBack | Zone/Night_Carnival/Decorations/DecGroupAF_CreateChildBack.c | modified |
| DecGroupAF_CreateChildFront | Zone/Night_Carnival/Decorations/DecGroupAF_CreateChildFront.c | modified |
| DecSonicNeonPart_State_FollowParentMask | Zone/Night_Carnival/Decorations/DecSonicNeonPart_State_FollowParentMask.c | modified |
| DecSonicNeonPart_Create | Zone/Night_Carnival/Decorations/DecSonicNeonPart_Create.c | modified |
| DecSonicNeon_State_BlinkFull | Zone/Night_Carnival/Decorations/DecSonicNeon_State_BlinkFull.c | modified |
| DecSonicNeon_State_BlinkPair | Zone/Night_Carnival/Decorations/DecSonicNeon_State_BlinkPair.c | modified |
| DecGroupAF_Create | Zone/Night_Carnival/Decorations/DecGroupAF_Create.c | modified |

#### `obj_pcarnival.c` (8 functions)

| original | source | class |
|---|---|---|
| Player_Action_GrabCraneSlide | Player/Player_Action_GrabCraneSlide.c | modified |
| Player_Action_GrabCranePart | Player/Player_Action_GrabCranePart.c | modified |
| FixInformation_State_SwingSky | UI/Hud/FixInformation_State_SwingSky.c | modified (lifted C) |
| FixInformation_CreateSwingSky | UI/Hud/FixInformation_CreateSwingSky.c | modified |
| FixInformation_State_Swing | UI/Hud/FixInformation_State_Swing.c | modified |
| FixInformation_CreateSwing | UI/Hud/FixInformation_CreateSwing.c | modified |
| Player_Action_SwingSky | Player/Player_Action_SwingSky.c | modified |
| Player_State_SwingSky | Player/Player_State_SwingSky.c | modified |

#### `obj_crisis.c` (50 functions)

| original | source | class |
|---|---|---|
| Crane_State_Unk_ov000_02305ea0 | Zone/Huge_Crisis/Objects/Crane_State_Unk_ov000_02305ea0.c | modified |
| Crane_Create | Zone/Huge_Crisis/Objects/Crane_Create.c | modified |
| CranePartED_NotifyParentArrived | Zone/Huge_Crisis/Objects/CranePartED_NotifyParentArrived.c | modified (lifted C) |
| CranePartED_State_Unk_ov000_023056fc | Zone/Huge_Crisis/Objects/CranePartED_State_Unk_ov000_023056fc.c | modified |
| CranePartED_OnHit | Zone/Huge_Crisis/Objects/CranePartED_OnHit.c | modified (lifted C) |
| CranePartED_Draw | Zone/Huge_Crisis/Objects/CranePartED_Draw.c | modified |
| CranePartED_Create | Zone/Huge_Crisis/Objects/CranePartED_Create.c | modified |
| CranePartEE_Draw | Zone/Huge_Crisis/Objects/CranePartEE_Draw.c | modified |
| CranePartEE_Create | Zone/Huge_Crisis/Objects/CranePartEE_Create.c | modified |
| GmkPipe_State_Cooldown | Zone/Huge_Crisis/Objects/GmkPipe_State_Cooldown.c | modified |
| GmkPipe_OnHit | Zone/Huge_Crisis/Objects/GmkPipe_OnHit.c | modified |
| GmkPipe_OnHit_Box1 | Zone/Huge_Crisis/Objects/GmkPipe_OnHit_Box1.c | modified |
| GmkPipe_Create | Zone/Huge_Crisis/Objects/GmkPipe_Create.c | modified |
| JetCatapult_State_Idle | Zone/Huge_Crisis/Objects/JetCatapult_State_Idle.c | modified |
| EffJet_State_FollowParent | Zone/Huge_Crisis/Effects/EffJet_State_FollowParent.c | modified |
| EffJet_Create | Zone/Huge_Crisis/Effects/EffJet_Create.c | modified |
| JetCatapultPart_State_Countdown | Zone/Huge_Crisis/Objects/JetCatapultPart_State_Countdown.c | modified |
| JetCatapult_CreatePart_ov000_0230e354 | Zone/Huge_Crisis/Objects/JetCatapult_CreatePart_ov000_0230e354.c | modified |
| JetCatapult_Action_Launch | Zone/Huge_Crisis/Objects/JetCatapult_Action_Launch.c | modified |
| JetCatapult_State_Fly | Zone/Huge_Crisis/Objects/JetCatapult_State_Fly.c | modified |
| JetCatapult_State_Blastoff | Zone/Huge_Crisis/Objects/JetCatapult_State_Blastoff.c | modified (lifted C) |
| JetCatapult_Action_Blastoff | Zone/Huge_Crisis/Objects/JetCatapult_Action_Blastoff.c | modified (lifted C) |
| JetCatapult_State_Countdown | Zone/Huge_Crisis/Objects/JetCatapult_State_Countdown.c | modified (lifted C) |
| JetCatapult_Action_Ignite | Zone/Huge_Crisis/Objects/JetCatapult_Action_Ignite.c | modified |
| JetCatapult_OnHit | Zone/Huge_Crisis/Objects/JetCatapult_OnHit.c | modified (lifted C) |
| obj_jet_catapult_flying | - | native helper (no single original function) |
| JetCatapult_Create | Zone/Huge_Crisis/Objects/JetCatapult_Create.c | modified |
| crisis_reset | - | native helper (no single original function) |
| Piston_UpdateOscillation | Zone/Huge_Crisis/Objects/Piston_UpdateOscillation.c | modified |
| Piston_DrawShaft | Zone/Huge_Crisis/Objects/Piston_DrawShaft.c | modified |
| Piston_State_Move | Zone/Huge_Crisis/Objects/Piston_State_Move.c | modified (lifted C) |
| Piston_InitOscillationPhase | Zone/Huge_Crisis/Objects/Piston_InitOscillationPhase.c | modified |
| Piston_Create | Zone/Huge_Crisis/Objects/Piston_Create.c | modified |
| Steam_State_Rise | Zone/Huge_Crisis/Objects/Steam_State_Rise.c | modified |
| Steam_Create | Zone/Huge_Crisis/Objects/Steam_Create.c | modified |
| Valve_DrawSprites | Zone/Huge_Crisis/Objects/Valve_DrawSprites.c | modified |
| Valve_SpawnSteamPuff | Zone/Huge_Crisis/Objects/Valve_SpawnSteamPuff.c | modified (lifted C) |
| Valve_SetSpriteAnimActive | Zone/Huge_Crisis/Objects/Valve_SetSpriteAnimActive.c | modified |
| Valve_SetSpriteAnimEnd | Zone/Huge_Crisis/Objects/Valve_SetSpriteAnimEnd.c | modified |
| Valve_Action_Wait | Zone/Huge_Crisis/Objects/Valve_Action_Wait.c | modified (lifted C) |
| Valve_Action_Puff | Zone/Huge_Crisis/Objects/Valve_Action_Puff.c | modified |
| Valve_Action_Active | Zone/Huge_Crisis/Objects/Valve_Action_Active.c | modified (lifted C) |
| Valve_Action_End | Zone/Huge_Crisis/Objects/Valve_Action_End.c | modified |
| Valve_State_Wait | Zone/Huge_Crisis/Objects/Valve_State_Wait.c | modified |
| Valve_State_Puff | Zone/Huge_Crisis/Objects/Valve_State_Puff.c | modified |
| Valve_State_Active | Zone/Huge_Crisis/Objects/Valve_State_Active.c | modified |
| Valve_State_End | Zone/Huge_Crisis/Objects/Valve_State_End.c | modified |
| Valve_InitSteamSprites | Zone/Huge_Crisis/Objects/Valve_InitSteamSprites.c | modified |
| Valve_Create | Zone/Huge_Crisis/Objects/Valve_Create.c | modified |
| DecGroup67_Create | Zone/Huge_Crisis/Decorations/DecGroup67_Create.c | modified |

#### `obj_pcrisis.c` (8 functions)

| original | source | class |
|---|---|---|
| Player_Action_Pipe | Player/Player_Action_Pipe.c | modified (lifted C) |
| pipe_arrive | - | native helper (no single original function) |
| Player_State_Pipe | Player/Player_State_Pipe.c | modified |
| Player_Action_PipeExit | Player/Player_Action_PipeExit.c | modified |
| Player_Action_JetCatapult | Player/Player_Action_JetCatapult.c | modified (lifted C) |
| Player_Action_JetCatapultLaunch | Player/Player_Action_JetCatapultLaunch.c | modified |
| Player_State_JetCatapult | Player/Player_State_JetCatapult.c | modified (lifted C) |
| Player_State_JetCatapultLaunch | Player/Player_State_JetCatapultLaunch.c | modified |

#### `obj_altitude.c` (35 functions)

| original | source | class |
|---|---|---|
| Glider_State_Carry | Zone/Altitude_Limit/Objects/Glider_State_Carry.c | modified |
| Glider_OnHit | Zone/Altitude_Limit/Objects/Glider_OnHit.c | modified (lifted C) |
| Glider_Create | Zone/Altitude_Limit/Objects/Glider_Create.c | modified |
| NeedleG_State_Idle | Zone/Altitude_Limit/Objects/NeedleG_State_Idle.c | modified |
| NeedleG_Create | Zone/Altitude_Limit/Objects/NeedleG_Create.c | modified |
| ParachuteReleased_State_Main | Zone/Altitude_Limit/Objects/ParachuteReleased_State_Main.c | modified |
| ParachuteReleased_Create | Zone/Altitude_Limit/Objects/ParachuteReleased_Create.c | modified |
| Parachute_State_Idle | Zone/Altitude_Limit/Objects/Parachute_State_Idle.c | modified |
| Parachute_OnHit | Zone/Altitude_Limit/Objects/Parachute_OnHit.c | modified (lifted C) |
| Parachute_Create | Zone/Altitude_Limit/Objects/Parachute_Create.c | modified |
| Saucer_Action_Idle | Zone/Altitude_Limit/Objects/Saucer_Action_Idle.c | modified |
| Saucer_Explode | Zone/Altitude_Limit/Objects/Saucer_Explode.c | modified (lifted C) |
| Saucer_Action_Ridden | Zone/Altitude_Limit/Objects/Saucer_Action_Ridden.c | modified (lifted C) |
| Saucer_State_Idle | Zone/Altitude_Limit/Objects/Saucer_State_Idle.c | modified |
| Saucer_State_Ridden | Zone/Altitude_Limit/Objects/Saucer_State_Ridden.c | modified |
| Saucer_Create | Zone/Altitude_Limit/Objects/Saucer_Create.c | modified |
| MapObj13Part_State_DestroyOnAnimEnd | Zone/Altitude_Limit/Objects/MapObj13Part_State_DestroyOnAnimEnd.c | modified |
| MapObj13Part_Destructor_ov000_0230bdf4 | Zone/Altitude_Limit/Objects/MapObj13Part_Destructor_ov000_0230bdf4.c | modified |
| MapObj13Part_Destructor_ov000_0230bfdc | Zone/Altitude_Limit/Objects/MapObj13Part_Destructor_ov000_0230bfdc.c | modified |
| MapObj13Part_State_DestroyWhenIdle | Zone/Altitude_Limit/Objects/MapObj13Part_State_DestroyWhenIdle.c | modified |
| MapObj13Part_State_DestroyWhenIdle2 | Zone/Altitude_Limit/Objects/MapObj13Part_State_DestroyWhenIdle2.c | modified |
| MapObj13_CreatePart_ov000_0230bc50 | Zone/Altitude_Limit/Objects/MapObj13_CreatePart_ov000_0230bc50.c | modified |
| mo13_driver | - | native helper (no single original function) |
| MapObj13_CreatePart_ov000_0230be44 | Zone/Altitude_Limit/Objects/MapObj13_CreatePart_ov000_0230be44.c | modified |
| MapObj13_CreatePart_ov000_0230c02c | Zone/Altitude_Limit/Objects/MapObj13_CreatePart_ov000_0230c02c.c | modified |
| altitude_reset | - | native helper (no single original function) |
| MapObj13_State_Unk_ov000_0230c1c8 | Zone/Altitude_Limit/Objects/MapObj13_State_Unk_ov000_0230c1c8.c | modified (lifted C) |
| MapObj13_Action_Unk_ov000_0230c290 | Zone/Altitude_Limit/Objects/MapObj13_Action_Unk_ov000_0230c290.c | modified (lifted C) |
| MapObj13_State_Unk_ov000_0230c2d0 | Zone/Altitude_Limit/Objects/MapObj13_State_Unk_ov000_0230c2d0.c | modified |
| MapObj13_OnHit | Zone/Altitude_Limit/Objects/MapObj13_OnHit.c | modified (lifted C) |
| MapObj13_Create | Zone/Altitude_Limit/Objects/MapObj13_Create.c | modified |
| DecCloudW_State_Main | Zone/Altitude_Limit/Decorations/DecCloudW_State_Main.c | modified |
| DecCloudW_Create | Zone/Altitude_Limit/Decorations/DecCloudW_Create.c | modified |
| DecCloudSpawner_Create | Zone/Altitude_Limit/Decorations/DecCloudSpawner_Create.c | modified |
| DecGroup95_Create | Decorations/DecGroup95_Create.c | modified |

#### `obj_paltitude.c` (18 functions)

| original | source | class |
|---|---|---|
| fade | - | native helper (no single original function) |
| Player_Action_Glider | Player/Player_Action_Glider.c | modified (lifted C) |
| Player_State_Glider | Player/Player_State_Glider.c | modified |
| Player_Action_ResumeGlider | Player/Player_Action_ResumeGlider.c | modified (lifted C) |
| Player_ClampToGliderArea | Player/Player_ClampToGliderArea.c | modified |
| Player_OnHit_GliderSequence | Player/Player_OnHit_GliderSequence.c | modified (lifted C) |
| Player_UpdateGliderSequence | Player/Player_UpdateGliderSequence.c | modified |
| Player_State_GliderSequence | Player/Player_State_GliderSequence.c | modified |
| Player_State_GliderSequenceIntro | Player/Player_State_GliderSequenceIntro.c | modified |
| Player_Action_GliderSequenceStart | Player/Player_Action_GliderSequenceStart.c | modified |
| Player_State_GliderSequenceExit | Player/Player_State_GliderSequenceExit.c | modified |
| Player_Action_GliderSequenceEnd | Player/Player_Action_GliderSequenceEnd.c | modified (lifted C) |
| Player_Action_ParachuteRelease | Player/Player_Action_ParachuteRelease.c | modified |
| Player_State_Parachute | Player/Player_State_Parachute.c | modified |
| Player_Action_Parachute | Player/Player_Action_Parachute.c | modified |
| Player_State_ParachuteRelease | Player/Player_State_ParachuteRelease.c | modified |
| Player_Action_Saucer | Player/Player_Action_Saucer.c | modified |
| Player_State_Saucer | Player/Player_State_Saucer.c | modified (lifted C) |

#### `obj_deadline.c` (25 functions)

| original | source | class |
|---|---|---|
| Cannon_RotateBarrel | Zone/Dead_Line/Objects/Cannon_RotateBarrel.c | modified |
| Cannon_State_Aim | Zone/Dead_Line/Objects/Cannon_State_Aim.c | modified |
| Cannon_OnHit | Zone/Dead_Line/Objects/Cannon_OnHit.c | modified (lifted C) |
| Cannon_Draw | Zone/Dead_Line/Objects/Cannon_Draw.c | modified (lifted C) |
| cannon_sprite | - | native helper (no single original function) |
| Cannon_Create | Zone/Dead_Line/Objects/Cannon_Create.c | modified |
| Duct_State_CheckPlayer | Zone/Dead_Line/Objects/Duct_State_CheckPlayer.c | modified (lifted C) |
| Duct_Create | Zone/Dead_Line/Objects/Duct_Create.c | modified |
| GmkAir_OnHit | Zone/Dead_Line/Objects/GmkAir_OnHit.c | modified |
| GmkAir_Create | Zone/Dead_Line/Objects/GmkAir_Create.c | modified |
| MapObj9C_OnHit | Zone/Dead_Line/Objects/MapObj9C_OnHit.c | modified (lifted C) |
| MapObj9C_Create | Zone/Dead_Line/Objects/MapObj9C_Create.c | modified |
| Rocket_State_Idle | Zone/Dead_Line/Objects/Rocket_State_Idle.c | modified |
| Rocket_Explode | Zone/Dead_Line/Objects/Rocket_Explode.c | modified (lifted C) |
| Rocket_Action_Launch | Zone/Dead_Line/Objects/Rocket_Action_Launch.c | modified |
| Rocket_State_Fly | Zone/Dead_Line/Objects/Rocket_State_Fly.c | modified |
| Rocket_OnHit | Zone/Dead_Line/Objects/Rocket_OnHit.c | modified (lifted C) |
| Rocket_OnHit_Box1 | Zone/Dead_Line/Objects/Rocket_OnHit_Box1.c | modified (lifted C) |
| Rocket_Create | Zone/Dead_Line/Objects/Rocket_Create.c | modified |
| Swing_State_Main | Zone/Dead_Line/Objects/Swing_State_Main.c | modified |
| Swing_OnHit | Zone/Dead_Line/Objects/Swing_OnHit.c | modified (lifted C) |
| Swing_Draw | Zone/Dead_Line/Objects/Swing_Draw.c | modified (lifted C) |
| swing_sprite | - | native helper (no single original function) |
| Swing_Create | Zone/Dead_Line/Objects/Swing_Create.c | modified |
| DecArrow_Create | Zone/Dead_Line/Decorations/DecArrow_Create.c | modified |

#### `obj_pdeadline.c` (11 functions)

| original | source | class |
|---|---|---|
| Player_SwingAroundPivot | Player/Player_SwingAroundPivot.c | modified |
| Player_SwingAroundPivotRetract | Player/Player_SwingAroundPivotRetract.c | modified |
| Player_Action_Swing | Player/Player_Action_Swing.c | modified |
| Player_State_Swing | Player/Player_State_Swing.c | modified (lifted C) |
| Player_Action_Cannon | Player/Player_Action_Cannon.c | modified |
| Player_State_Cannon | Player/Player_State_Cannon.c | modified |
| Player_Action_Rocket | Player/Player_Action_Rocket.c | modified |
| Player_State_Rocket | Player/Player_State_Rocket.c | modified |
| Player_Action_Air | Player/Player_Action_Air.c | modified |
| Player_State_Air | Player/Player_State_Air.c | modified |
| Player_Unk_ov000_02350adc | Player/Player_Unk_ov000_02350adc.c | modified |

#### `obj_enemy2.c` (3 functions)

| original | source | class |
|---|---|---|
| EggSf_Create | Zone/Dead_Line/Enemies/EggSf_Create.c | modified |
| GunHunter_State_CheckGround | Zone/Huge_Crisis/Enemies/GunHunter_State_CheckGround.c | modified (lifted C) |
| GunHunter_Create | Zone/Huge_Crisis/Enemies/GunHunter_Create.c | modified |

## 6. Deviations (all of them)

1. **Rendering is separate from the update.** On the DS the sprite is
   processed and put into OAM in the draw step of StageObj_Main. Here the draw
   step only advances the animation and records position, flips and visibility.
   `objects_draw()` emits one quad per object afterwards. Not drawn:
   rotation/scale (rot-scale OAM, used for tilted objects with `0x5C & 0x100`),
   palette changes at runtime (StageObj_AllocSpritePalette rows, palette
   animation), and the screen-shake global offset (data_022c4eec). The second
   DS screen sprite (spr1) is not kept. Layering: OAM priority against the two
   BG layers (front z 4.0 = priority 0, back z 2.0 = priority 2), then OAM order.
2. **One view.** The DC shows one 256x192 window (CAMERA.md). The spawner uses
   the dual-screen variant (StageEvent_SpawnInView / UpdateDualScreen) with
   `n` views (`objects_set_view_count`, default 1), and ViewOutCheck tests the same
   views. The single-screen variant (gCamera.unkC8 & 1, view height 0x1d0) and
   the combined dual box (gObjManagerFlags & 0x200) are not used. On the DS the
   second screen's camera spawns objects as well. With one view, objects that the DS would
   spawn for the other screen spawn later.
3. **Call order split.** Stage_Main + StageEvent_Main → `objects_frame_begin`;
   Player_Collide and all tasks after the player → `objects_update`. The DS
   order is kept as long as main.c calls them around `dcp_player_frame`.
4. **Hit box events come from `.hbx`** (frames.json), not the BAC command stream.
   The order inside a frame (boxes, then EndFrame) is the same.
5. **Spawn of objects that were in view after a jump**: `objects_respawn_view()`
   stands in for the DS stage restart (StageEvent_InitialSpawn).
6. **The scatter spread level** (RingManager unk9D5) lives in the player
   module as a private static (`dcp_on_hurt` passes `&s_scatter_level`).
   The ring manager resets *its* copy (`objects_ring_scatter_level()`) when all
   scattered rings are gone, but cannot reset the player's. A one-line change
   in the player module (pass `objects_ring_scatter_level()`) would make it
   exact. This is left to the player/integration owner; I did not edit the player module.
7. **Hurt box flags** (`@0x2C0/0x2C4`) are rebuilt by the object module from
   `invuln`/`invinc` (see 4.5), because the player module does not keep them.
   The result is the same values in the same frames.
8. **Player_Action_SpringLaunchAlt** (zone 3 springs) is called by the original
   without arguments. The spring velocity is still in r1/r2, so it is modelled
   as `dcp_act_launch(p, vx, vy)` + sound 0x72. This is an assumption; it was not checked on hardware.
9. **Dropped side effects** (no module for them yet, or not needed): sound
   handles (positional sound, tempo, pan, looping: IronBall plays 0x50 once
   per ground contact), EffVs/EffVitality, Camera_UnlockX/Y (EffAirBubble is
   translated since v3),
   HUD popups (only a hook), VRAM/palette allocation, PaletteAnim_Create
   (zone 4 walls/platforms), wireless play (MapObject_Net*). Effects without a
   module go to `g_obj_hooks.effect`.
10. **Pool limits**: 384 objects (DS: heap). There is a guard on the solid-object list
    (0x20), which the DS would overrun.
11. **Goal ring** reports the stage exit once (`stage_exit` hook) instead of
    calling StageShutdown_Create every frame until the stage ends.
12. Everything listed as `not translated` in sections 5 and 5b and in the
    coverage table (section 8).

v2 deviations:

13. **Pre-player tasks** (id 0xf6) run at the end of `objects_frame_begin`;
    **Player_FindHomingTarget / Player_CheckStartGrind** run there too, i.e.
    before the whole of Player_In instead of at their place inside it
   . Magnet attraction runs in `objects_update` after
    Player_Collide, which is where Player_Last runs in the player task.
14. **Composed sprites vs OAM pieces**: frames.bin holds each frame as one
    composed image. StageObj_Draw2DEx with a piece table (WallBreak and
    crumbling GmkLand pieces) offsets each OAM piece; the DC draws the whole
    image moved by piece 0. The wall frames are one piece each (exact); the
    crumbling floor (several pieces) falls as one block (visual only: the
    collision follows the original piece timing).
15. **Rot-scale OAM** is now drawn (obj_draw.c, `quad4`): rotation `angle +
    grav_rot` when `disp_flags & 0x100`, scale from `@0x44/@0x46` (RopeCSpring).
    Rotation direction and scale centre are not checked on hardware.
16. **Palette rows**: StageObj_AllocSpritePalette(w, row, ...) selects a BAC
    palette row; the converted atlases are rendered with the BAC's own palette,
    so objects that pick another row (DecGroup0F vines 0x3b..0x40 use row 8,
    DecVine 0x33/0x34 row 7) may show other colours. Not checked against the DS.
17. **3D model of RopeC** is not drawn by the module: `objects_model_get` hands
    the transform to main.c.
18. **Touch screen**: StageObj_IsTouched (animals react to the stylus) is always
    FALSE.
19. **gGameState.unk24** (GmkLand oscillation phase) is taken from
    `g_dcp_world.frame` like unk20; if the two DS counters differ (unk24 may run
    during pause) the platform phase differs, not its path.
20. **Camera**: no camera module exists; the camera-area objects write the
    `g_obj_camera` mirror, which nothing reads yet. The DC runs the dual-screen
    branches (`g_obj_camera[2].flags & 1` = 0) with camera 1 mirroring camera 0.
21. **Argument evaluation order**: where the original passes several `Rand()`
    calls as arguments of one call, the module draws them right to left (MWCC
    order, checked in the matched object code of DecGroup35_SpawnPieceBurst).
22. **Unchecked flapper / pawn variants**: EnemyFlapperGroup_Create returns NULL
    for types 0x0b, 0x0c, 0x10, 0x11, 0x12 (shooting / swooping);
    EnemyPawnGroup_Create still only builds types 0x00/0x14/0x01 walking
    (p9 == 0, no entrance flag). Superseded in v3: types 0, 1, 3, 7, 8, 0x14,
    0x15 with entrance and drop-in (section 5c); the others still return NULL.

v3 deviations (Water Palace, Mirage Road):

23. **3D BACs** (superseded: see 39): EffShadow (sand ship shadow), the
    drop-in Egg Pawns and Egg Bishops (types 0 / 20, 7 / 21 with p9) and Rock
    use a 3D BAC at depth @0x18 on the DS and hide the 2D sprite
    (`0x80000`). They are now 3D sprites: EffShadow `Obj.bac3d` 1 (its BAC is
    the 3D one), the drop-ins and Rock `Obj.bac3d` 2 (obj_draw.c draws the
    frames of ac_ene_eggpawn3d / ac_ene_egg_bishop3d / ac_gmk_rock_3 for the
    2D sprite's animation state; the layouts match). Wheel2D, Hand2d and
    GmkTable are NSBMD models: the module hands them to main.c through
    `objects_model_get` (as RopeC).
24. **Sounds**: object sounds go through `g_obj_hooks.sfx` with the
    original ids; sound handles, positional (pan/volume by distance) and
    tempo control are dropped. The GmkTable engine loop (restarted on the DS
    when `NNS_SndPlayerCountPlayingSeqBySeqArcIdx(0, 0x5e) == 0`) starts once
    per ship. Player-state sounds keep their channel (`g_dcp_hooks.sfx` /
    `sfx_stop`, VOICE/BOOST) as in the player module.
25. **Shared VRAM animation** (DecShared): on the DS one task animates a VRAM
    slot that every decoration of the kind displays. Here DecShared animates
    an ObjSprite and each decoration points `ObjSprite.vram` at it; `spr_image`
    uses that frame when the resource matches. AnimatorSprite_AnimateManual is
    approximated by the normal advance (same frame order; the frame timing of
    the manual step is not checked).
26. **HUD parts**: the drowning digits (EffDrownCount) and the mine digits
    (MineDigit) are translated sprites (FIX_MOJI). The enemy-room counter
    (EnemyRoomHud / EffEnemyRoomFix) goes through `OBJ_FX_ENEMY_ROOM`;
    FixVs_CreateAbovePlayer (VS marker over the player on the ship) is dropped
    (log entry only).
27. **Camera branches**: MapObj8F and GmkTable carry the single-screen branch
    (`g_obj_camera[2].flags & 1`), translated but never taken (deviation 20).
    Drop-in pawns spawn only within 60 px of a view: with one view, the
    drop-ins of z32 do not appear in the host test (section 7). Two views
    are the DS behaviour.
28. **Player_UpdateUnderwater and the gimmick-destroyed check** run in
    `objects_frame_begin`, before the whole of Player_In instead of at their
    place inside it. It is switchable
    (`objects_set_underwater_check`).
29. **NULL guards** where the matched code dereferences a possibly NULL
    pointer: Bubble_OnHit (reads byte 0 of a NULL toucher), Kuragen_SpawnBomb
    (bomb allocation failure), the GmkTable ring write (no ring under the
    ship). On the DS these read low memory; only the failure path changes.
30. **Type checks**: WaterRunTrigger, MapObj80, LaunchZone and Wheel2D OnHit
    return unless the toucher is a player (`type == 1`). Only players reach
    those boxes (list 1) in the original, so behaviour is the same.
31. **Player parents**: effects that follow a player (air bubbles, drowning
    digits, shadow) keep a `DcPlayer *ppar`, because DcPlayer is not an Obj
    in this port. Same positions.
32. **EffEntrance start scale** is 1 instead of 0: native scale 0 means
    "unscaled", so 1 (the smallest visible size) stands in for the DS 0.
33. **Templated call with a missing argument**: Cargo_Action_StartMove is
    called with one argument in the decompiled C; the ROM disassembly
    (arm-none-eabi-objdump) shows r1 still holds the player's `move_x`. The
    translation passes it explicitly.
34. **From asm**: MapObj8F_Action_StartCameraRelease and
    StageEvent_SpawnInRect have no decompiled C (asm only); both were
    rewritten from the listing. Not checked against the DS beyond the test.
35. **Animation end comes from the model**: BreatheAirBubble, Wheel2D release,
    SandSlide end and Hand2D wait for `DCP_DF_ANIM_DONE`. The host tests set
    it by hand (as the model renderer would). Without a renderer setting it,
    these states do not end.
36. **Zones 4..7** (v3 note, superseded by v4): the shared v3 code also
    created objects there before v4; v4 translates the zones and runs them
   . The 3D boss variants (EggPawn3D, EggBishop3D, Rock_3) and
    the boss objects are not translated (boss module).
37. **Water rendering** (surface band, tint below the line) is left to the
    renderer. The module only provides the level and the flags.
38. **Holder-cached resources** (v4): ObjData_Load returns the already
    loaded resource for the same holder. The light4 children and the Bomb
    Hawk bomb (EggBombHawk uses the Bomb Hawk BAC) share one slot, as on
    the DS; shared VRAM frames (NeedleG 0xb1 / 0xb2, MapObj13 drivers) are
    per-object frames natively (deviation 23 style).
39. **3D BACs** (Model_InitSprite3D / Model_DrawSprite3D): `Obj.bac3d`
    objects are drawn by obj_draw.c as quads in the 3D layer (GRAPHICS.md
    section 8.3): 1 = the object's own BAC (MapObj13 flappers, glider rings,
    clouds, DecGroup4D wheel / crane slide, EffShadow), 2 = the 3D BAC the DS
    loads next to the 2D one, shown with the 2D sprite's animation state
    (drop-in pawns / bishops, Rock, and on the boss arenas Bubble3D,
    FixMoji3D and the player effects EffBoostJet3D, EffSpinSmoke1/2_3D,
    EffBrakeSmoke3D). StageObj_Draw3DEx's placement is kept: depth @0x18,
    off-axis for disp 0x200, the rotation of disp 0x100 (angle, gravity,
    @2 / @3, scale @0x44 / @0x46). On the circle boss arenas (gCamera.unkE0 /
    unkE4: zones 2..4) the objects keep the arc position and the draw maps it
    with BossArena_ArcToCirclePos (the DS *_Spawn3D / Bubble3D / FixMoji3D
    store the circle position). Deviation: Bubble3D probes the map at the arc
    x (the DS at the circle x). EffKemuriSpin3D is Boss7's (boss module).
    2D sprites that the DS hides behind a 3D model (flag 0x80000) are drawn
    while `objects_set_model_sprites(1)` (default), hidden with 0.
40. **Model view `visible`** now means `!(disp_flags & 0x20)`; v4 objects use
    disp 0x200 for the perspective flag, which is passed as `persp`.
41. **Ring suck (Duct)**: RingManager_SuckScatteredRings is translated
    against the native ring manager (RingPart.sucker instead of the DS
    pointer field); same radius (0x80), speed and vanish rule.
42. **Palette animation and ZoneFx**: PaletteAnim_Create and the zone 4
    BLDY blends go to the effect hook (`OBJ_FX_PALETTE_ANIM`,
    `OBJ_FX_ZONE4_BLDY`); the module does no palette or blend work.
43. **Sound**: tempo changes (NNS_SndPlayerSetTempoRatio), positional sound
    (Sound_SetPositional), seq stops and handle releases are dropped; the
    piston loop is started once for all pistons (the DS asks the sound
    system whether it is already playing).
44. **NULL guards** (deviation 29) also in the v4 OnHit callbacks, the
    glider exit (`Player_State_GliderSequenceExit` with the trigger already
    gone: the DS reads the freed task) and DecCloudSpawner (the DS does not
    test the allocation).
45. **Per-screen sprites**: Valve steam, crane slide spr1 and DecCloudW are
    drawn per DS screen relative to gCamera[i]; the DC keeps the main-screen
    sprite in world coordinates. DecCloudW compares with view 0.
46. **Ride link**: the StageObj_HandleRide @0x130 part (rider / toucher of
    the collision map) runs in `obj_main` for objects with a placement,
    before the player's `dcp_handle_ride`; v3 objects did not need it.
47. **Coverage run**: test_coverage sets `g_dcp_world.wind_x = 0x100` so
    MapObj13 (created only while the glider-sequence wind blows) is counted.
48. **gGameState.unk24** in Piston / NeonRoute timing is `W.frame`
    (deviation 19).
49. **Event state round trip** (section 2.5): `objects_save_event_state`
    runs the object destructors itself (Stage_DestroyTasks) before copying,
    so main.c can call it before `objects_shutdown` (which frees without
    destructors). The copy also keeps `g_obj_stage`, which the DC clears in
    `objects_init` (the DS does not clear data_022c4590 for the return).
50. **Player_InitState box part** is a separate function
    (`objects_player_init_state`), called after the object code's own
    `dcp_player_init_state` calls; the player module's calls use the hook
    main.c installs.
51. **Test harness assists** (not module behaviour): the side shooter test
    sets the push animation end flag, the needle test
    gives `invuln = 1` so Player_In resets the body defence, and
    `place_player` calls `objects_player_init_state`.

## 7. Build and test

```sh
cd dreamcast/game/objects
make data        # BYOR: build/dreamcast/objects/{objdata.bin,hbx/,sprites.txt}
make host-test   # gcc -std=c99 -Wall -Wextra -Werror -fwrapv; test_objects on z11, z12 and test_zones on ZONE_STAGES
make audit       # item-C audit: test_objects AUDIT_ONLY=1 on z11, z12 + ZONE_STAGES, CHARA=0 and 1
make zone-test   # test_zones only (ZONE_STAGES = z21 z22 z31 z32 z41 z42 z51 z52 z61 z62 z71 z72 z91 z92)
make coverage    # create every placed object / decoration of every stage, print the table of section 8
make dc          # kos-cc -O2 -fwrapv -Wall -Wextra -Werror, objects in build/dreamcast/objects/dc/
```

`make host-test` needs `build/dreamcast/player/{physics,trig}.bin` (player
module `make tables`), `build/dreamcast/stages/z11|z12|z21|z22|z31|z32/level.bin` and
`build/dreamcast/sprites/bac/`. `test_objects ... -v` also prints every ring
pickup and the quiet events (camera bounds, decorations).

Besides the section 4 checks, the test has:

- `[v2 targets]`: the player is put at the first reachable instance of each
  new kind and the expected interaction is checked: trampoline launch
  (-0xc00..-0x780), bend stick (anim 0x31 then launch anim 0x30), rope (anim
  0x24, release), jump pedestal, corkscrew path, rock break, wall break (pieces
  fly), rope C (anim 0x25, swing, A releases), special ring (boosted 0xc0
  frames -> enter -> `OBJ_EXIT_SPECIAL_STAGE`), riding a platform, crumbling
  floor, iron ball roll (z12), flapper kill, Egg Pawn M child, boost
  knock-away, homing target search + homing attack kill, magnet attraction,
  decoration counts. The rope and the special ring float off the floor, so the
  test puts the player into their grab boxes with the speed / state their
  OnHit checks require.
- `[sweep]`: from every 25th placed ring (reachable spots over the whole act)
  the player runs right for 240 frames with jumps and boosts; pickups, kills
  and gimmick triggers are reported.


**test_zones (v3)** on z21, z22, z31, z32. Same frame loop as test_objects
(objects_frame_begin, dcp_player_frame, objects_update, camera follow with
the gimmick lock), two spawn views (view 1 = view 0 moved up 192 px, as the
DS top screen). Parts:

- `[run]`: PlayerStart, hold right, jump every 50 frames, 1500 frames.
- `[zone 2 targets]` / `[zone 3 targets]`: one check per kind. The column
  after `ok` says how the interaction was reached: `play` = the player was
  put on the floor nearby (or on the object that leads there) and moved by
  input / physics into it; `direct` = the player was put into the trigger
  box with the speed and state its OnHit requires (the kind sits in water,
  on a wall or needs a run-up that the level gives only from far away).
  The air bubble check signals `DCP_DF_ANIM_DONE` by hand, as the model
  renderer would. The enemy-room check removes the enemies the room spawns
  (as if defeated) to see the room clear; the gates, spawn with the entrance
  effect and HUD calls are real.
- `[sweep]`: from every 25th ring, run right 240 frames with jumps and
  boosts; counts pickups and v3 interactions met by chance.

Exercised vs only created (v3):

- Exercised by a check (play): water entry, drowning countdown and drowning,
  water exit, bubble spawner, water run (from the 0x80 slide), water slide
  0x80, tilting raft, Egg Lancer charge, fish flee + shared frames, hand,
  Egg Bishop throw, Egg Hammer stomp, enemy room (enemies removed by the
  test), sand ship with rocks (z31 also a drop-in pawn landing).
- Exercised by a check (direct): air bubble, water current, launch zone,
  wheel, mine, Kuragen bomb and pulse, water jet, water slope 0x7e, rail,
  cargo, sand slider 0x90 / 0x91, sand area 0xa9.
- Met in the sweep by play as well: wheel2d (z21), water jet (z21), mine +
  Kuragen bomb + MapObj40 slide (z22), cargo, rail, hand, enemy room start
  (z31), enemy room start (z32).
- Only created, behaviour not checked: DecGroup41 fountain / waterfall /
  algae / coral (spawned and animated), DecGroup4D, DecGroup45, MineDigit,
  EffGmkBubble, EffShadow drawing, Egg Bishop flight, the small bubbles
  0xe9 / 0xea, GmkWall reopening, the camera values MapObj8F writes, the
  z32 drop-in pawns (not spawned in the test: see deviation 27), and every
  v3 path on zones 4..7.
- Every placed object and decoration entry of z21, z22, z31, z32 is created
  by `make coverage` (section 8).

KOS: `make dc` builds all 20 files with `-Werror` (text 129 959 bytes, bss
484 235 bytes, mostly the 384-object pool; v2: 16 files, 87 KB / 390 KB). A test `main` calling every API
function (incl. `objects_model_get`, `objects_set_grind_check`,
`g_obj_camera`) links with the player module and `engine/render.c` into an
ELF with `kos-cc`.

Not verified:

- No frame-by-frame comparison with the DS game (no emulator traces).
- The SH-4 build was never run, and `obj_draw.c` (including the new rot-scale
  quads) was never seen on screen.
- Interactions are checked by events and values derived from the decompiled
  code, not against recordings. Some targets place the player by hand
  (rope, special ring, flapper, homing) instead of reaching them by play.
- z11, z12 and the 14 act stages of ZONE_STAGES were run frame by frame;
  z81 / z82 and the act-3 arenas only had every placed entry created once
  by `make coverage`.
- The water surface, tint and underwater music are not drawn / played by
  this module; nothing of them was checked.

## 8. Coverage

Placed object / decoration types and entries that come up as translated
objects (`make coverage`; MapObj40 cases that need untranslated code count
as missing; PlayerStart and the load-time forms of MapObj70 /
WaterLevelTrigger count as translated):

| stage | object types | object entries | decoration types | decoration entries |
|---|---|---|---|---|
| z11 | 42/42 | 280/280 | 36/36 | 915/915 |
| z12 | 47/47 | 411/411 | 37/37 | 2151/2151 |
| z13 | 1/2 | 1/2 | 0/0 | 0/0 |
| z21 | 43/43 | 328/328 | 21/21 | 402/402 |
| z22 | 42/42 | 377/377 | 22/22 | 408/408 |
| z23 | 1/2 | 1/2 | 0/0 | 0/0 |
| z31 | 53/53 | 419/419 | 27/27 | 328/328 |
| z32 | 56/56 | 515/515 | 26/26 | 398/398 |
| z33 | 1/2 | 1/2 | 0/0 | 0/0 |
| z41 | 38/38 | 233/233 | 21/21 | 567/567 |
| z42 | 46/46 | 407/407 | 26/26 | 533/533 |
| z43 | 1/2 | 1/2 | 0/0 | 0/0 |
| z51 | 48/48 | 369/369 | 29/29 | 185/185 |
| z52 | 49/49 | 410/410 | 32/32 | 275/275 |
| z53 | 1/2 | 1/2 | 0/0 | 0/0 |
| z61 | 44/44 | 729/729 | 23/23 | 681/681 |
| z62 | 44/44 | 653/653 | 17/17 | 455/455 |
| z63 | 1/2 | 1/2 | 0/0 | 0/0 |
| z71 | 47/47 | 771/771 | 7/7 | 96/96 |
| z72 | 47/47 | 616/616 | 2/2 | 41/41 |
| z73 | 1/2 | 1/2 | 0/0 | 0/0 |
| z81 | 9/9 | 28/28 | 23/23 | 100/100 |
| z82 | 11/11 | 43/43 | 0/0 | 0/0 |
| z91 | 14/14 | 33/33 | 5/5 | 14/14 |
| z92 | 12/12 | 36/36 | 12/12 | 47/47 |
| zf3 | 1/2 | 1/2 | 0/0 | 0/0 |

not translated (entries over all stages):
  objects: 0x38:1 0x39:1 0x3a:1 0x3b:1 0x3c:1 0x3d:1 0x3e:1 0x3f:1
  decorations:

v3 -> v4 (object types, entries, decoration types, entries): z41 29/38 ->
38/38, 168 -> 233, 0/21 -> 21/21, 0 -> 567; z42 33/46 -> 46/46, 298 -> 407,
0/26 -> 26/26, 0 -> 533; z51 37/48 -> 48/48, 289 -> 369, 0/29 -> 29/29;
z52 34/49 -> 49/49, 320 -> 410, 0/32 -> 32/32; z61 32/44 -> 44/44, 472 ->
729, 0/23 -> 23/23; z62 30/44 -> 44/44, 421 -> 653, 0/17 -> 17/17; z71
38/47 -> 47/47, 643 -> 771, 0/7 -> 7/7; z72 38/47 -> 47/47, 473 -> 616, 0/2
-> 2/2; z91 10/14 -> 14/14, 23 -> 33, 0/5 -> 5/5; z92 9/12 -> 12/12, 29 ->
36, 0/12 -> 12/12. "Translated" here means the entry creates a translated
object; how far each kind was exercised is in sections 5d and 7.1.

Still missing: the boss objects 0x38..0x3f, one placed in each act-3 arena
(z13 .. z73, zf3), owned by the boss module.

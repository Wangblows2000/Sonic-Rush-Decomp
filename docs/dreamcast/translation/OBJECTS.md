# Native stage objects (Dreamcast port): translation notes

Status: the object system and the core shared objects are translated and
tested on the host on the real Leaf Storm act 1 level (44/44 checks). The
module compiles with `kos-cc` (sh-elf-gcc, `-Werror`), and a test ELF that
calls the whole API links against the player module and `engine/render.c`.
It has never run on SH-4 hardware or in an emulator, and the drawing code
(`obj_draw.c`) has never been seen on screen.

- Code: `dreamcast/game/objects/` (`obj_*.c/h`, `objects.h`, `Makefile`,
  `test/test_objects.c`, `tools/objdata.py`)
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
| `obj_enemy.c` | EnemyPawnGroup (Egg Pawn), MapObject_OnHit_Default, destruction (burst, debris, knock-away) |
| `obj_draw.c` | KOS only: atlas upload and quad drawing with `render.h` |
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

## 3. Data (bring your own ROM)

`make data` (= `python3 dreamcast/game/objects/tools/objdata.py`) reads
`extract/eur` and the sprite converter output `build/dreamcast/sprites/` and
writes `build/dreamcast/objects/`:

- `objdata.bin` "ROBJ" v1. Header: `char[4] "ROBJ"; u16 version 1, slots (14),
  stages (27), id_len (48)`. Then these tables, each padded to 4 bytes, copied
  from the owner's ROM:
  `margin[0xf2]` (data_ov000_023686e0, spawn/despawn margin per type),
  `s16 ring_gravity` (data_ov000_023690e4 = 18), `itembox_rings[8]`
  (data_ov000_02366b44), `itembox_vs[8]` (data_ov000_02366b3c),
  `player_spr_anim[0x68]` (data_ov000_02368d14, Player_SetAnimation sprite
  index), `needle_shape[12][8]` (data_ov000_023661c0), `animal[8][3]`
  (data_ov000_023661a8), `s8 shake[16]` (arm9 data_02077944). After them comes
  `char id[stages][slots][48]`: the sprite id each BAC slot resolves to on each stage.
  This follows ObjData_Load: the zone archive `z??_act_lz7.narc` first, then the loose
  `/act/` file; act_com and player_lz7 for the shared files. The resolution uses
  `build/dreamcast/sprites/manifest.json`.
- `hbx/<id>.hbx` "HBOX" v1. It holds the BAC command 7 collision boxes per frame. `frames.bin`
  has no events, so they come from `frames.json`. Header:
  `char[4] "HBOX"; u16 version 1, anims, frames, events; u32 0`, then anims
  `{u16 first_frame, count}`, frames `{u16 first_event, count}` (frames.bin
  order; the tool checks that the counts match frames.bin), and events
  `{u8 slot, pad; u16 flags; s16 l, t, r, b}`.
- `sprites.txt`: the sprite directories each stage uses.

**Disc layout needed** (integrator; build_disc.sh is not mine):

```
/cd/objects/objdata.bin
/cd/objects/hbx/<id>.hbx                         (15 files, 4 KB total)
/cd/sprites/<id>/frames.bin + atlas0.argb1555    (from build/dreamcast/sprites/bac/<id>/)
```

with `<id>` =
`act__ac_itm_ring, act__ac_gmk_spring, act__ac_gmk_dash_p, act__ac_gmk_dash_c,
act__ac_gmk_dash_ct, act__ac_itm_box, act__ac_gmk_needle, act__ac_gmk_goalring,
act__ac_gmk_marker, act__ac_ene_eggpawn, act__ac_ene_eggpawn_m,
act__ac_eff_zako_bomb, act__ac_eff_hahen_zako, act__ac_sonic` (frames.bin
only, this BAC has no atlas), and `narc__z41_act_lz7__act__ac_gmk_spring` (z41 and z91 springs).
The atlases of one stage take about 1.0 MB of PVR VRAM (all 14 slots are loaded at
`objects_init`; the largest is zako_bomb, 256 KB). The id names are up to 38
characters long, so the ISO needs Rock Ridge/Joliet names (build_disc.sh already uses `-r -J`).

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
- Objects come from a fixed pool of 128 (the DS uses the task heap). If the pool is full,
  creation returns NULL, like `task == OS_GetArenaLo(0)`.

### 4.2 Spawning (StageEvent)

- The level lists (`rlvl_object_t`, `rlvl_ring_t`) are grouped by 256 px region.
  `objects_init` makes a mutable copy of every object entry (`ObjParam`, the
  .ev entry layout). The objects write into it, as on the DS: `x0 = 0xff`
  means spawned, `p6` holds the item kind, `pB` counts hits, and `pA` is the trick counter.
  Rings keep one byte per entry (`.rg` byte 0).
- `StageEvent_Main`: on the first frame InitialSpawn runs (flags 1: spawn everything in
  view + margin). After that, UpdateDualScreen runs (flags 5) only when the view moved.
  The spawn rect is view ± margin (objects 0x100, rings 0x10; decorations 0x100
  are another module). It is clamped to the map, and each clamped side clears the
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

Counts: assumed 1, not here 1, not needed 10, not translated 52, not used 2, partial 10, replaced 2, reused 5, translated 156

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
| `StageEvent_SpawnDecorationsInBlock` | - | not here | decorations are another module (spawner 2 skipped) |
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
| `MapObject_Action_KnockAway` | `mapobject_knock_away` | translated | unreachable: only the boost AttackHitbox (not translated) calls it |
| `MapObject_AllocSpawnEntry` | `spawn_entry_alloc` | translated |  |
| `MapObject_Collide_Default` | `obj_mapobject_collide_default` | translated |  |
| `MapObject_Destructor` | `obj_mapobject_dtor` | translated | sound handle release dropped |
| `MapObject_FreeSpawnEntry` | `spawn_entry_free` | translated |  |
| `MapObject_In_Default` | `mapobject_in_default` | translated | MapObject_NetReceiveHits dropped |
| `MapObject_Init` | `obj_mapobject_init` | translated | @0x130 ride link not kept |
| `MapObject_NetReceiveHits` | - | not translated | wireless play |
| `MapObject_NetSendEvent` | - | not translated | wireless play |
| `MapObject_OnHit_Default` | `mapobject_on_hit_default` | translated | EffVs/EffVitality effects dropped (RNG draws kept); AttackHitbox branch absent |
| `MapObject_Spawn` | `obj_mapobject_spawn` | translated |  |
| `MapObject_SpawnDestroyBurst` | `mapobject_spawn_destroy_burst` | translated |  |
| `MapObject_SpawnFreedAnimal` | `mapobject_spawn_freed_animal` | partial | kind chosen (RNG kept), Decoration_Spawn not called (decoration module) |
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
| `ObjRect_CenterX` | - | not needed | no caller in the translated objects |
| `ObjRect_CenterY` | - | not needed | no caller in the translated objects |
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
| `ObjRect_RegistGet` | - | not needed | no caller in the translated objects |
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
| `StageObj_IsTouched` | - | not translated |  |
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
| `RingManager_AttractRings` | - | not translated | magnet barrier (Player_GiveMagnet not translated) |
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
| `EnemyPawnGroup_OnDetect` | `pawn_on_detect` | partial | EffFound_Create ("!") dropped; actions of untranslated types absent |
| `EnemyPawnGroup_OnHit_Box2_Bounce` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_OnHit_Box2_PushPlayer` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_SpawnChild` | `pawn_spawn_child` | partial | p9 parent link part not translated |
| `EnemyPawnGroup_State_Attack` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_State_AttackSpawnEggBishop` | - | not translated | other Egg Pawn types |
| `EnemyPawnGroup_State_AttackSpawnEggPawnM` | `pawn_state_attack_spawn_m` | translated | the child (type 0xdf EggPawnM_Create) is not translated, so no child appears |
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
| `EffZakoBombPuff_Create` | - | not needed |  |
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
   order is kept as long as the integrator calls them around `dcp_player_frame`.
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
   handles (positional sound, tempo), EffFound ("!"), EffVs/EffVitality, the
   freed animal (Decoration_Spawn; its random draws are kept so `gRandSeed` stays
   in step), EffAirBubble, Camera_UnlockX/Y, HUD popups (only a hook),
   VRAM/palette allocation, wireless play (MapObject_Net*).
10. **Pool limits**: 128 objects (DS: heap). There is a guard on the solid-object list
    (0x20), which the DS would overrun.
11. **Goal ring** reports the stage exit once (`stage_exit` hook) instead of
    calling StageShutdown_Create every frame until the stage ends.
12. Everything listed as `not translated` in section 5. The main items: other
    Egg Pawn types (lancer, bishop, hammer, ...) and their drop-in/entrance variants, the
    EggPawnM child (type 0xdf), Flapper and Kuragen enemies, the magnet barrier attraction,
    boss-arena rings, and all other gimmicks (MapObj40 cases that need
    untranslated player actions are logged).

## 7. Build and test

```sh
cd dreamcast/game/objects
make data        # BYOR: build/dreamcast/objects/{objdata.bin,hbx/,sprites.txt}
make host-test   # gcc -std=c99 -Wall -Wextra -Werror -fwrapv; real z11 level + player tables + sprites
make dc          # kos-cc -O2 -fwrapv -Wall -Wextra -Werror, objects in build/dreamcast/objects/dc/
```

`make host-test` needs `build/dreamcast/player/{physics,trig}.bin` (player
module `make tables`), `build/dreamcast/stages/z11/level.bin` and
`build/dreamcast/sprites/bac/`. `test_objects ... -v` also prints every ring pickup.

Results (2026-11-22, host; data generated into /tmp/objects/data, the same
tool as `make data`):

```
objects_init: 0; stage 0; all 14 sprite resources loaded
[unit] ObjRect_FlagCheck table (9 cases), AnimatorSprite timing:
  spring anim 1: frame calls 2 4 2 4 (last held), end flag at call 15, stopped on frame 5
  ring anim 0 (loop): first wrap at call 33, 2 wraps in 70 calls
  eggpawn / son.bac boxes loaded from .hbx
[run] PlayerStart, hold right, jump every 50 frames, 1500 frames
  f  223 spring 0x43  player (1566,810)
  f  364..577  13 ring pickups (1968,1008) (1944,1008) (2576,1240) ... (3392,1968)
  f  677 spring 0x43  player (5106,2354)
  max x 5704 (3525 before MapObj40: the layer switches now take the player out of the loop)
  rings placed 53 collected 13; objects spawned 31, despawned 29 (mostly layer-switch triggers)
[targets]
  spring 0x43 (1576,832), p6 = 2: launch (0, -0xa80) = -0x780 - 2*0x180, after 20 frames
  spring 0x46 (18304,1072), p6 = 1: launch (+0x900, 0) from the right side
  dash panel 0x5f (16664,1280): gnd 2304 (= top), dash timer 0x80 - 1
  dash ring 0x6b (6016,1416): launch (0x800, 0) (angle 0 * 0x800)
  item box 0x30 item 1 (10200,640): broken, +5 rings (+1 ring picked up on the way)
  spikes 0x4d (1776,1216): hurt on landing, 10 rings scattered, 9 collected back
  marker 0x6e index 2 (11656,1344): respawn point (11656, 1344 - 14)
  egg pawn (17392,1024): detected, chased, destroyed by a jump; player bounces (vy -0x300)
  egg pawn (6568,1472): standing player is detected, chased and hurt
  goal ring (20736,1616): reached once, spin, 8 sparkles, stage exit after 165 frames
[respawn] the first spring (despawned earlier) is spawned again when the view returns;
          collected rings stay collected
44 checks, 0 failed
```

KOS: `make dc` builds all 9 files with `-Werror` (text: about 37 KB; bss 98 KB,
mostly the 128-object pool). A test `main` that calls every API function links
with the player module and `engine/render.c` into an ELF (`kos-cc`).

Not verified:

- No frame-by-frame comparison with the DS game (no emulator traces).
- The SH-4 build was never run, and `obj_draw.c` was never seen on screen.
- Interactions are checked by events and values derived from the decompiled
  code, not against recordings.
- Only z11 was exercised; the other stages only had their sprite ids resolved.

# Native player module (Dreamcast port): translation notes

Status: the player module (movement, state machine, collision, tricks, boost, gimmick states, model
presentation) is translated and integrated. Host test: `make -C dreamcast/game/player host-test`.
Integrated in the game and played in Flycast (REGRESSION.md); not run on real hardware.

- Code: `dreamcast/game/player/` (`dcp_*.c/h`, `Makefile`, `test/`, `tools/`)
- Source of truth: `src/game/Player/`, `src/game/Stage/ObjectSystem/`
  (`StageObj_*`, `ObjSpd_*`, `ObjAngle_*`, `MapCollision_*`),
  `src/asm_fallback/StageObj_CollideMapVertical.c`,
  `src/asm_fallback/Player_SetAnimation.c`.
- Stage collision: `tools/dreamcast/stages/level_format.h`, which is owned by
  the stage module (see `docs/dreamcast/formats/STAGES.md`). The player
  includes this header and does not copy it.

## 1. Units and data

- Positions are `s32` 24.8 fixed point (`0x100` = 1 px). This is **not**
  NitroSDK fx32 (20.12): the player code always uses `x >> 8` for pixels.
- Speeds and velocities are `s16` in 1/256 px per frame.
- Angles are `u8` (`0x40` = 90 degrees). The y axis points down, so
  `x += gnd*cos`, `y += gnd*sin` (`StageObj_Move`).
- Trig values are 4.12 fixed point. The sin/cos table uses the
  `data_0206e470` layout (sin and cos interleaved, 4096 steps, index =
  `angle16 >> 4`). `FX_Atan2` uses a 129-entry table.
- Time scale `data_0207cf7c` = `0x100` (1.0). `ObjSpd_*` and `StageObj_Move`
  multiply by it.

ROM data is not compiled in. `tools/extract_physics.py` (bring your own ROM)
writes these files to `build/dreamcast/player/`:

| file | content | source |
|---|---|---|
| `physics.bin` | `gPlayerPhysicsTable`: 2 entries of 0x30 bytes (Sonic, Blaze) | copied from `extract/eur/arm9_overlays/ov000.bin` @ 0x02368b7c |
| `trig.bin` | 8192 s16 sin/cos, then 129 s16 atan | **computed** (`round(sin*4096)`, `round(atan(i/128)*65536/2pi)`), then checked against arm9.bin @ 0x0206e470 / 0x02072470: 8192/8192 and 129/129 identical |
| `tables.json` | decoded physics fields | for debugging |

The runtime receives the tables through `dcp_set_trig()` and
`dcp_set_physics()`. The engine or asset pipeline loads the files.

Physics table fields (struct `DcpPhysics`, offsets are table offsets, `@` is
the player work offset):

| +off | field | @player | use |
|---|---|---|---|
| 00 | acc | 356 | ground/air acceleration |
| 02 | top | 358 | top speed (halved when zone >= 9 or act >= 2) |
| 04 | dec | 35A | friction/brake |
| 06,08,0A | sd_base, sd_add, sd_max | 35C..360 | spin dash charge |
| 0C | roll_dec | 362 | roll friction |
| 0E | hs_top | 364 | top speed in high-speed mode |
| 10,12,14 | boost_acc/top/dec | 366..36A | boost |
| 16 | hs_min | 36C | high-speed mode ends below this \|move\| |
| 18 | slope_bonus | 36E | top += slope_bonus*sin(angle) downhill |
| 1A | p1a | 374 | gimmick use |
| 1C | invuln_len | 376 | invulnerability frames after a hit (180) |
| 1E | fall_grace | 3CC (u8) | frames on a steep wall before falling off |
| 20,22 | slope_acc, gnd_max | 03C, 03E | slope force, absolute speed cap |
| 24 | roll_slope | (03C while rolling) | |
| 26 | jump | 370 | jump impulse |
| 28,2A | gravity, fall_max | 040, 042 | |
| 2C | hs_time | - | frames at top speed before high-speed mode |
| 2E | ride_max | 048 | |

## 2. Per-frame update order (`StageObj_Main` for the player task)

`Player_Create` creates the task with `StageObj_Main`, and installs
`fn6c = Player_In`, `fn78 = Player_Collide` and `fn7c = Player_Last`. The
current state (`fn68`, `@0x68`) is a `Player_State_*` function.
`dcp_player_frame()` keeps the original order:

1. Return if destroyed (`obj_flags & 4`). `StageObj_HandleRide` adds the
   ride/touch object deltas to `push_x/y/z`.
2. `Player_In` runs if the game is not paused:
   `Player_UpdateGimmickStageBounds`, `Player_UpdateGravityFlip`,
   `Player_ReadInput` (remap), layer override, and the timers (dash 352,
   VS 3BE, combo 3B8, confusion 3C0, slowdown 3C2, respawn 3C8,
   invulnerability 3B2, tension-max 3B6, invincibility 3B4). Then the
   death-plane check, `Player_UpdateUnderwater`, `Player_FindHomingTarget`,
   `Player_HandleSurfaceLanding`, `Player_UpdateBoost`,
   `Player_CheckStartGrind`, `Player_DrainBoostTension`,
   `Player_Unk_ov000_023460ec`, `Player_CheckCrushed`,
   `Player_CheckTimeOver`.
3. Hit-stop `@0x62`: the low nibble counts freeze frames and the high nibble
   counts shake.
4. The state function runs if there is no freeze frame (`hitstop & 0xf == 0`).
5. `StageObj_Move` runs (gravity, slope force, velocity, wind, gravity
   rotation) unless `move_flags & 0x2000`.
6. If map collision is on (`gObjManagerFlags & 8`): clear ride/touch, then
   `StageObj_Collision`. With `move_flags & 0x200000` this splits the
   movement into 8 px steps (4 px in the air). Each step runs
   `StageObj_CollideMapImpl`: Horizontal (push out), Vertical, `y &= ~0xff`
   when grounded, then Horizontal again (flags and speed zeroing only).
7. Draw (fn70, not translated), `Player_Collide` (hit-box registration,
   object module). Clear push and shake, then `Player_Last` (tension debt,
   camera, dash-timer reset, gravity restore, layer restore, stage exits).

## 3. State machine

The action pointer slots are filled in by `Player_Create`:

- `@334` idle = `Action_GroundIdle`
- `@338` move = `Action_GroundMove`
- `@33C` crouch = `Action_Crouch`
- `@340` jump = `Action_Jump`
- `@344` air = `Action_HomingAttack` (Sonic) or `Action_Hover` (Blaze)
- `@348` = NULL

An `Action_*` function starts a state: it sets the animation, flags and
velocities, and assigns `@0x68`. A `State_*` function runs every frame and
calls actions to make transitions.

Transitions of the core states (hand-checked):

| state | per-frame work | transitions |
|---|---|---|
| GroundIdle | stand/look-up/balance animations; slope force if \|angle\| >= 0x20 | not grounded or steep and slow -> `Action_Fall`; A/B pressed -> jump slot; Down -> crouch slot; direction held or gnd != 0 -> move slot (unless pushing a wall or at the map edge) |
| GroundMove | skid animations (5..10) on reversal at \|gnd\| >= 0x200; `ApplyGroundInput`; wall push animations 0xb/0xc | fall off -> `Action_Fall`; gnd == 0 -> idle slot; A/B -> jump slot; Down -> crouch slot |
| Crouch | slope force if \|angle\| >= 0x30 | fall off -> Fall; \|gnd\| > 0x80 -> `Action_Roll`; still and A/B -> `Action_SpinDash`; Down released -> idle slot |
| SpinDash | A/B adds sd_add (capped at sd_max); charge decays by charge>>5 each frame | Down released -> gnd = ((charge>>8)<<7)+0x800, `Action_Roll`; Up -> idle slot |
| Roll | `ApplyRollFriction` (roll_dec: /2 when holding the travel direction, x2 when holding neither direction) | fall off -> Fall; A/B -> jump slot (dash timer capped at 0x14); \|gnd\| < 0x80 -> idle slot |
| InAir | `SettleAirRotation`, `ApplyAirInput`; variable jump: when A/B are not held and vy < -0x40, vy = -0x40 (until pf bit 0 is set: at the apex or after Fall/Launch) | grounded -> `Action_LandOnGround` + idle slot; trickable (pf&2): R+Up -> TrickUp, R -> TrickForward, A/B -> TrickCombo; otherwise R -> air slot (homing/hover) |
| HomingAttack | 0x20 frames: velocity = 0.5 * unit vector to the target | timeout -> Fall; grounded or wall hit at speed -> Land + idle |
| Hover (Blaze) | gravity 8, fall_max 0x220, air physics; `hover_timer` 0x78 | R released or timer at 0 -> Fall; grounded -> Land + idle |
| Hurt | `SettleAirRotation` | grounded -> Land + idle slot |
| Die | count 0x3c frames, fade | `Player_Respawn` + idle slot |

Full list of every `Player_State_*` / `Player_Action_*` function, extracted
from the decompilation by `dreamcast/game/player/tools/statemap.py src/game/Player`. Columns: the state it
sets, the actions it calls, the slot calls, and the animation ids. Indirect
slot calls written in the template style (`*(u32*)(p+0x334)`) are not always
detected; the table above is authoritative for the core states. The gimmick
states (Grind, Pipe, Rope, Glider, Cannon, ...) are listed but not
translated.

| function | sets state (`unk68`) | calls actions | slot calls (`unk334..344`) | animations set |
|---|---|---|---|---|
| `Player_Action_Air` | State_Air |  |  | 0x2b |
| `Player_Action_AirDash` | State_InAir |  |  | 0x18 |
| `Player_Action_AttachToObject` | State_AttachedToObject |  |  | 0x2c |
| `Player_Action_AttackBounce` | State_InAir |  |  | 2 |
| `Player_Action_AttackBounceVel` | State_InAir |  |  |  |
| `Player_Action_AttackRecoil` |  | Action_AttackBounce |  |  |
| `Player_Action_BendStick` | State_BendStick |  |  | 0x31 |
| `Player_Action_BendStickLaunch` | State_InAir |  |  | 0x30 |
| `Player_Action_Blank` | State_Blank |  |  |  |
| `Player_Action_BreatheAirBubble` | State_BreatheAirBubble |  |  | 0x23 |
| `Player_Action_Cannon` | State_Cannon |  |  |  |
| `Player_Action_Cargo` |  | Action_LandOnGround |  | 0x30 |
| `Player_Action_CorkscrewPath` | State_CorkscrewPath |  |  |  |
| `Player_Action_Crouch` | State_Crouch | Action_Roll |  | 3 |
| `Player_Action_DashPanel` |  |  |  |  |
| `Player_Action_DashRing` |  | Action_GimmickLaunch |  |  |
| `Player_Action_Die` | State_Die |  |  | 0x11 |
| `Player_Action_Fall` | State_InAir |  |  | 2 |
| `Player_Action_GimmickLaunch` | State_InAir | Action_LandOnGround |  | 0x13 0x18 |
| `Player_Action_Glider` | State_Glider |  |  | 0x2e |
| `Player_Action_GliderSequenceEnd` | State_GliderSequenceExit |  |  |  |
| `Player_Action_GliderSequenceStart` | State_Glider State_GliderSequenceIntro |  |  |  |
| `Player_Action_GmkSpecial` | State_GmkSpecial |  |  | 0x25 |
| `Player_Action_GmkSpecialEnter` | State_GmkSpecialEnter |  |  |  |
| `Player_Action_GrabCranePart` |  | Action_AttachToObject |  |  |
| `Player_Action_GrabCraneSlide` |  | Action_AttachToObject |  |  |
| `Player_Action_Grind` | State_Grind | Action_LandOnGround |  | 0x1A |
| `Player_Action_GrindTrick` |  |  |  | 0x1c 0x1d 0x1e |
| `Player_Action_GroundIdle` | State_Crouch State_GroundIdle |  | move | 4 0x3c |
| `Player_Action_GroundMove` | State_GroundMove |  |  | 0 |
| `Player_Action_Hand2D` | State_Hand2D | Action_LandOnGround |  | 0x2a |
| `Player_Action_HomingAttack` | State_HomingAttack | Action_AirDash |  | 0x30 |
| `Player_Action_Hover` | State_Hover |  |  | 0x34 |
| `Player_Action_JetCatapult` | State_JetCatapult |  |  | 3 |
| `Player_Action_JetCatapultLaunch` | State_JetCatapultLaunch |  |  | 0x18 |
| `Player_Action_Jump` | State_InAir |  |  | 0x30 |
| `Player_Action_Knockback` | State_Hurt |  |  | 0xf |
| `Player_Action_KnockbackIntangible` | State_Hurt |  |  | 0xf |
| `Player_Action_LandOnGround` |  |  |  |  |
| `Player_Action_Launch` | State_InAir | Action_LandOnGround |  | 0x13 0x16 |
| `Player_Action_MapObj40` | State_MapObj40 State_MapObj80 |  |  | 0x26 |
| `Player_Action_MapObj80` | State_MapObj80 | Action_LandOnGround |  | 0x27 |
| `Player_Action_Parachute` | State_Parachute State_ParachuteRelease |  |  | 0x31 |
| `Player_Action_ParachuteRelease` | State_ParachuteRelease |  |  | 0x13 |
| `Player_Action_PedestalLaunch` |  | Action_GimmickLaunch |  |  |
| `Player_Action_Pipe` | State_Pipe |  |  | 0x31 |
| `Player_Action_PipeExit` | State_Pipe |  | jump |  |
| `Player_Action_ReachGoal` |  |  |  |  |
| `Player_Action_ResumeGlider` | State_Glider |  |  |  |
| `Player_Action_Rocket` | State_Rocket |  |  | 0x25 |
| `Player_Action_Roll` | State_Roll |  |  | 0x31 |
| `Player_Action_Rope` | State_Rope |  |  | 0x24 |
| `Player_Action_RopeC` | State_RopeC |  |  | 0x25 |
| `Player_Action_SandSlide` | State_SandSlide |  |  | 0x2d |
| `Player_Action_SandSlider` | State_SandSlider |  |  | 0x31 |
| `Player_Action_SandSlider90` | State_SandSlider90 |  |  | 0x31 |
| `Player_Action_Saucer` | State_Parachute State_ParachuteRelease State_Saucer |  |  | 0x3C |
| `Player_Action_SpinDash` | State_SpinDash |  |  | 0x32 |
| `Player_Action_SpringLaunch` |  | Action_Launch |  |  |
| `Player_Action_SpringLaunchAlt` |  | Action_Launch |  |  |
| `Player_Action_Swing` | State_Swing |  |  | 0x2c |
| `Player_Action_SwingSky` | State_SwingSky |  |  | 0x25 |
| `Player_Action_TrickCombo` |  |  |  | 0x39 0x3a 0x3b |
| `Player_Action_TrickForward` | State_InAir |  |  | 0x37 |
| `Player_Action_TrickUp` | State_InAir |  |  | 0x35 |
| `Player_Action_Warp` | State_Warp |  |  | 0x11 |
| `Player_Action_WaterRun` | State_WaterRun |  |  |  |
| `Player_Action_Wheel2D` | State_Wheel2D |  |  | 0x28 |
| `Player_Action_ZoneLaunch` | State_ZoneLaunch |  |  | 0x29 |
| `Player_State_Air` |  | Action_LandOnGround | idle |  |
| `Player_State_AttachedToObject` |  | Action_LandOnGround | idle jump |  |
| `Player_State_BendStick` |  | Action_BendStickLaunch |  |  |
| `Player_State_Blank` |  |  |  |  |
| `Player_State_BreatheAirBubble` |  | Action_Fall Action_LandOnGround | idle |  |
| `Player_State_Cannon` |  | Action_GimmickLaunch |  | 0x30 |
| `Player_State_Cargo` |  |  | idle |  |
| `Player_State_CorkscrewPath` |  | Action_Fall Action_Grind Action_LandOnGround | idle jump |  |
| `Player_State_Crouch` |  | Action_Fall Action_Roll Action_SpinDash | idle |  |
| `Player_State_Die` |  |  | idle | 0x12 |
| `Player_State_Glider` |  | Action_LandOnGround | idle |  |
| `Player_State_GliderSequence` |  |  |  |  |
| `Player_State_GliderSequenceExit` |  | Action_ResumeGlider |  |  |
| `Player_State_GliderSequenceIntro` | State_GliderSequence |  |  | 0x2F |
| `Player_State_GmkSpecial` |  | Action_GmkSpecialEnter Action_LandOnGround | idle jump |  |
| `Player_State_GmkSpecialEnter` |  |  |  |  |
| `Player_State_Grind` |  | Action_Fall Action_GimmickLaunch Action_GrindTrick | idle jump | 0x1a 0x1f 0x20 |
| `Player_State_GroundIdle` |  | Action_Fall | crouch jump move | 0x21 0x3c 0x45 0x47 |
| `Player_State_GroundMove` |  | Action_Fall | crouch idle jump | 5 6 7 8 9 10 0xb 0xc |
| `Player_State_Hand2D` |  |  | idle |  |
| `Player_State_HomingAttack` |  | Action_Fall Action_LandOnGround | idle |  |
| `Player_State_Hover` |  | Action_Fall Action_LandOnGround | idle |  |
| `Player_State_Hurt` |  | Action_LandOnGround | idle |  |
| `Player_State_InAir` |  | Action_LandOnGround Action_TrickCombo Action_TrickForward Action_TrickUp | airSpecial idle | 2 0x14 0x15 0x17 0x19 0x36 |
| `Player_State_JetCatapult` |  | Action_JetCatapultLaunch |  |  |
| `Player_State_JetCatapultLaunch` |  | Action_LandOnGround Action_TrickCombo | idle | 0x19 |
| `Player_State_MapObj40` |  | Action_LandOnGround | idle |  |
| `Player_State_MapObj80` |  | Action_LandOnGround | idle |  |
| `Player_State_Parachute` |  | Action_ParachuteRelease |  |  |
| `Player_State_ParachuteRelease` |  | Action_LandOnGround Action_TrickCombo | idle | 0x14 0x15 |
| `Player_State_Pipe` |  |  |  |  |
| `Player_State_Rocket` |  | Action_LandOnGround | idle jump |  |
| `Player_State_Roll` |  | Action_Fall | idle jump |  |
| `Player_State_Rope` |  | Action_SpringLaunch | idle jump |  |
| `Player_State_RopeC` |  |  | jump |  |
| `Player_State_SandSlide` |  | Action_LandOnGround |  |  |
| `Player_State_SandSlider` |  | Action_LandOnGround | idle |  |
| `Player_State_SandSlider90` |  | Action_SandSlide |  |  |
| `Player_State_Saucer` |  | Action_LandOnGround | idle jump |  |
| `Player_State_SpinDash` |  | Action_Roll | idle | 0x32 0x33 |
| `Player_State_Swing` |  |  | idle jump |  |
| `Player_State_SwingSky` |  | Action_LandOnGround | idle jump |  |
| `Player_State_Warp` |  |  | idle |  |
| `Player_State_WaterRun` |  | Action_Fall Action_Grind Action_LandOnGround | idle jump |  |
| `Player_State_Wheel2D` |  |  | jump | 0x29 |
| `Player_State_ZoneLaunch` |  | Action_LandOnGround | idle |  |

## 4. Collision interface

The map side is `rlvl_map_t` from `tools/dreamcast/stages/level_format.h`.
Call `dcp_bind_map(const rlvl_map_t *)` after `rlvl_bind()`. The player calls
these functions:

- `rlvl_probe_distance_long(m, x, y, flags, dir, &angle, &attr)`: wrapped as
  `dcp_probe_long`. It is used by `StageObj_ProbeCollision`, and therefore by
  every floor, wall and ceiling probe, and by `Player_CheckLedgeBalance`.
- `rlvl_probe_distance(...)`: wrapped as `dcp_probe_tile`. It is used by
  `StageObj_CollideMapVertical` for the 1 px snap re-check and by
  `Player_HandleSurfaceLanding`.

Semantics, from the original code (confirmed by the stage module against the
data):

- `x`, `y` are pixels. `dir`: 0 = +x, 1 = -x (width masks), 2 = +y,
  3 = -y (height masks).
- The result `d` is the signed distance such that moving the probe point by
  `d` along `dir` puts it on the last free pixel before the surface. `d <= 0`
  means the point touches or is inside the surface, embedded by `-d`. The
  long probe looks up to 3 tiles ahead (an empty column gives `>= 16`) and up
  to 2 tiles back when the point is inside a full tile. Callers truncate to
  `s8`.
- `*angle` and `*attr` are written only when a non-empty tile is hit.
  Otherwise the caller's values stay (the vertical pass relies on this: it
  seeds them with the current angle).
- `flags` comes from `StageObj_GetMapProbeFlags`:
  - bit 0: layer B (`obj_flags & 1`; `Player_Create` sets it, so the player
    starts on layer B).
  - `0x80`: top-solid chars (attr bit 0) are empty. It is set unless
    `move_flags & 0x100000` ("platforms solid this frame"). The vertical pass
    clears it when the feet probe 5 px higher is not embedded, so you land on
    platforms from above and pass through them from below.
  - `0x40`: outside the map is solid (`move_flags & 0x80000`, set by
    InitState).
  - `0x20`: arena-rect distance (`rlvl_map_t.arena_*`).

Solid objects use an optional callback, `g_dcp_col.obj_probe` (the original
`ObjCollisionMap_ProbeForObj(obj, x, y, flags, dir, angle, attr)`):

- It uses the same distance convention, and returns 0x18 when nothing is in
  range.
- When the player stands on an object (distance <= 0, or <= 1 in the air),
  it sets `p->ride` to a `DcpObjRef` that holds the object's position,
  previous position, last displacement and flags. It sets `p->touch` on any
  contact.
- `StageObj_HandleRide` then carries the player with the platform.
- Without the callback, only the map is solid.

The player-side collision code (translated):

- `dcp_probe` (`StageObj_ProbeCollision`): the minimum of the map and object
  probes.
- `dcp_collide_horizontal` (`StageObj_CollideMapHorizontal`): two wall probes
  per side, at feet-4 and at head, rotated by the ground quadrant.
- `dcp_collide_vertical` (`StageObj_CollideMapVertical`): translated by hand
  from the ARM listing. It does the following:
  - selects the floor quadrant from angle and gravity rotation;
  - decides whether platforms are solid;
  - probes both feet (x +-9, y + 13);
  - scans solid objects between the feet;
  - runs a centre probe for the attribute only;
  - ignores steep rails;
  - snaps to the floor within `min(|move|>>8 + 3, 11)` px when on the ground;
  - lands in the air when embedded up to 14 px;
  - picks the angle from the nearer foot (ties go to the angle closer to the
    current one), with optional easing (`move_flags & 0x800000`);
  - removes the velocity into the surface (converted with `vy*sin` when
    `0x20000` is set);
  - probes the ceiling at head-2 / feet-2.
- `dcp_check_ceiling` (`StageObj_CheckCeiling`): adds 6 px to the wall
  probes under a low ceiling.
- `dcp_obj_collision` (`StageObj_Collision`): the substep loop.

The map hitbox is `box_l, box_t, box_r, box_b` = -7, -12, 7, 13 (InitState).

## 5. Function mapping

### ObjectSystem / math

| original | native | status |
|---|---|---|
| `StageObj_Move` | `dcp_obj_move` | translated |
| `StageObj_Collision` | `dcp_obj_collision` | translated |
| `StageObj_CollideMap` / `StageObj_CollideMapImpl` | `dcp_collide_map` | translated |
| `StageObj_CollideMapHorizontal` | `dcp_collide_horizontal` | translated (both switch tables folded into `set_wall_probe`, same values) |
| `StageObj_CollideMapVertical` (asm only) | `dcp_collide_vertical` | translated from the ARM listing, stack slots named in comments |
| `StageObj_CheckCeiling` | `dcp_check_ceiling` | translated |
| `StageObj_ProbeCollision` | `dcp_probe` | translated |
| `StageObj_GetMapProbeFlags` | `dcp_probe_flags` | translated |
| `StageObj_ApplyCollisionAttr` | `dcp_apply_attr` | translated |
| `StageObj_HandleRide` | `dcp_handle_ride` | partial: ride (`@80`) and touch (`@84`); the `@130` link (pushing between linked objects) is not translated |
| `StageObj_Main` (player path) | `dcp_player_frame` | partial: no draw, view-out check, or `fn78` |
| `MapCollision_ProbeDistance`, `_ProbeDistanceLong`, `_GetTileHeight`, `_GetTileWidth`, `_GetTileAt` | `rlvl_*` (stage module) | used through `dcp_probe_tile` / `dcp_probe_long` |
| `ObjCollisionMap_ProbeForObj` | `g_dcp_col.obj_probe` callback | interface only (object module) |
| `ObjSpd_UpSet` / `ObjSpd_DownSet` | `dcp_spd_up` / `dcp_spd_down` | translated (time scale kept) |
| `ObjSpd_AccelToward` | - | not needed by the player |
| `ObjAngle_MoveToward8` / `ObjAngle_IsUpsideDown` | `dcp_angle_toward8` / `dcp_angle_upside_down` | translated |
| `Math_RotateVec2S16` (+ `MTX_RotZ33_`, `MTX_MultVec33`) | `dcp_rotate_vec` | translated |
| `FX_Atan2` (+ `FX_Div`) | `dcp_atan2` | translated (FX_Div = rounded 64-bit divide, as the DS divider) |
| `RingManager_ScatterRings` | `dcp_scatter_rings` | partial: velocity pattern and count; the spawn goes to the `spawn_ring` hook |
| Camera_* | - | camera module (it reads `x`, `y`, `move_x/y`, `pf2`) |

### src/game/Player (206 files)

| original | native | status |
|---|---|---|
| `AttackHitbox_Create` | `objects/obj_flapper.c` | translated |
| `AttackHitbox_SetBoostState` | - | no separate function in the port |
| `AttackHitbox_SetJetCatapultState` | `objects/obj_player.c` | translated |
| `AttackHitbox_State_Boost` | `objects/obj_player.c` | translated |
| `AttackHitbox_State_Follow` | `objects/obj_player.c` | translated |
| `AttackHitbox_State_JetCatapult` | `objects/obj_player.c` | translated |
| `Player_Action_Air` | `objects/obj_deadline.c` | translated |
| `Player_Action_AirDash` | `dcp_act_airdash` | translated |
| `Player_Action_AttachToObject` | `objects/obj_pwater.c` | translated |
| `Player_Action_AttackBounce` | `objects/obj_player.c` | translated |
| `Player_Action_AttackBounceVel` | `boss/boss4.c` | translated |
| `Player_Action_AttackRecoil` | `objects/obj_player.c` | translated |
| `Player_Action_BendStick` | `objects/obj_pgimmick.c` | translated |
| `Player_Action_BendStickLaunch` | `objects/obj_pgimmick.c` | translated |
| `Player_Action_Blank` | `boss/boss7.c` | translated |
| `Player_Action_BreatheAirBubble` | `objects/obj_pwater.c` | translated |
| `Player_Action_Cannon` | `objects/obj_deadline.c` | translated |
| `Player_Action_Cargo` | `objects/obj_pmirage.c` | translated |
| `Player_Action_CorkscrewPath` | `objects/obj_pgimmick.c` | translated |
| `Player_Action_Crouch` | `dcp_act_crouch` | translated |
| `Player_Action_DashPanel` | `dcp_act_dash_panel` | translated |
| `Player_Action_DashRing` | `objects/obj_player.c` | translated |
| `Player_Action_Die` | `dcp_act_die` | translated |
| `Player_Action_Fall` | `dcp_act_fall` | translated |
| `Player_Action_GimmickLaunch` | `objects/obj_player.c` | translated |
| `Player_Action_Glider` | `objects/obj_altitude.c` | translated |
| `Player_Action_GliderSequenceEnd` | `objects/obj_paltitude.c` | translated |
| `Player_Action_GliderSequenceStart` | `objects/obj_paltitude.c` | translated |
| `Player_Action_GmkSpecial` | `objects/obj_shared.c` | translated |
| `Player_Action_GmkSpecialEnter` | `objects/obj_pgimmick.c` | translated |
| `Player_Action_GrabCranePart` | `objects/obj_crisis.c` | translated |
| `Player_Action_GrabCraneSlide` | `objects/obj_carnival.c` | translated |
| `Player_Action_Grind` | `objects/obj_pwater.c` | translated |
| `Player_Action_GrindTrick` | `objects/obj_pgimmick.c` | translated |
| `Player_Action_GroundIdle` | `dcp_act_ground_idle` | translated |
| `Player_Action_GroundMove` | `dcp_act_ground_move` | translated |
| `Player_Action_Hand2D` | `objects/obj_pmirage.c` | translated |
| `Player_Action_HomingAttack` | `dcp_act_homing` | translated |
| `Player_Action_Hover` | `dcp_act_hover` | translated |
| `Player_Action_JetCatapult` | `objects/obj_crisis.c` | translated |
| `Player_Action_JetCatapultLaunch` | `objects/obj_pcrisis.c` | translated |
| `Player_Action_Jump` | `dcp_act_jump` | translated |
| `Player_Action_Knockback` | `dcp_act_knockback` | translated |
| `Player_Action_KnockbackIntangible` | `boss/boss7.c` | translated |
| `Player_Action_LandOnGround` | `dcp_act_land` | translated |
| `Player_Action_Launch` | `dcp_act_launch` | translated |
| `Player_Action_MapObj40` | `objects/obj_pwater.c` | translated |
| `Player_Action_MapObj80` | `objects/obj_pwater.c` | translated |
| `Player_Action_Parachute` | `objects/obj_altitude.c` | translated |
| `Player_Action_ParachuteRelease` | `objects/obj_paltitude.c` | translated |
| `Player_Action_PedestalLaunch` | `objects/obj_shared.c` | translated |
| `Player_Action_Pipe` | `objects/obj_crisis.c` | translated |
| `Player_Action_PipeExit` | `objects/obj_crisis.c` | translated |
| `Player_Action_ReachGoal` | `ui/ui_state.c` | translated |
| `Player_Action_ResumeGlider` | `objects/obj_paltitude.c` | translated |
| `Player_Action_Rocket` | `objects/obj_deadline.c` | translated |
| `Player_Action_Roll` | `dcp_act_roll` | translated |
| `Player_Action_Rope` | `objects/obj_shared.c` | translated |
| `Player_Action_RopeC` | `objects/obj_pgimmick.c` | translated |
| `Player_Action_SandSlide` | `objects/obj_pmirage.c` | translated |
| `Player_Action_SandSlider` | `objects/obj_pmirage.c` | translated |
| `Player_Action_SandSlider90` | `objects/obj_pmirage.c` | translated |
| `Player_Action_Saucer` | `objects/obj_altitude.c` | translated |
| `Player_Action_SpinDash` | `dcp_act_spindash` | translated |
| `Player_Action_SpringLaunch` | `dcp_act_spring` | translated |
| `Player_Action_SpringLaunchAlt` | `objects/obj_gimmicks.c` | translated |
| `Player_Action_Swing` | `objects/obj_deadline.c` | translated |
| `Player_Action_SwingSky` | `objects/obj_carnival.c` | translated |
| `Player_Action_TrickCombo` | `dcp_act_trick_combo` | translated |
| `Player_Action_TrickForward` | `dcp_act_trick_forward` | translated |
| `Player_Action_TrickUp` | `dcp_act_trick_up` | translated |
| `Player_Action_Warp` | - | not ported (VS mode / replay) |
| `Player_Action_WaterRun` | `objects/obj_pwater.c` | translated |
| `Player_Action_Wheel2D` | `objects/obj_pwater.c` | translated |
| `Player_Action_ZoneLaunch` | `objects/obj_pwater.c` | translated |
| `Player_AddLives` | `ui/ui_state.c` | translated |
| `Player_AddRings` | `dcp_add_rings` | translated |
| `Player_AddTension` | `dcp_add_tension` | translated |
| `Player_AddTensionCombo` | `dcp_add_tension_combo` | translated |
| `Player_AddTrickScore` | `dcp_add_trick_score` | translated |
| `Player_ApplyAirInput` | `dcp_apply_air_input` | translated |
| `Player_ApplyAutoRunAcceleration` | `objects/obj_pwater.c` | translated |
| `Player_ApplyConfusion` | - | not ported (VS mode / replay) |
| `Player_ApplyGroundInput` | `dcp_apply_ground_input` | translated |
| `Player_ApplyNetState` | - | not ported (VS mode / replay) |
| `Player_ApplyRollFriction` | `dcp_apply_roll_friction` | translated |
| `Player_ApplySlopeForce` | `dcp_apply_slope_force` | translated |
| `Player_ApplySlowdown` | - | not ported (VS mode / replay) |
| `Player_ApplyVerticalInput` | `dcp_apply_vertical_input` | translated |
| `Player_AttachGimmickTable` | `objects/obj_pmirage.c` | translated |
| `Player_AttachGimmickTableNoInput` | `objects/obj_pmirage.c` | translated |
| `Player_AttachGimmickTableReleased` | `objects/obj_pmirage.c` | translated |
| `Player_CheckCrushed` | `dcp_check_crushed` | translated |
| `Player_CheckFallOffGround` | `dcp_check_fall_off` | translated |
| `Player_CheckLedgeBalance` | `dcp_check_ledge_balance` | translated |
| `Player_CheckStartGrind` | `objects/obj_manager.c` | translated |
| `Player_CheckTimeOver` | `ui/ui_state.c` | translated |
| `Player_ClampToAttachedGimmick` | - | not ported (VS mode / replay) |
| `Player_ClampToGliderArea` | `objects/obj_paltitude.c` | translated |
| `Player_Collide` | `objects/obj_manager.c` | translated |
| `Player_Create` | dcp_player_create: physics/state part; tasks, models, sprites, VRAM, icons, VS/special-stage restore not translated | partial |
| `Player_Destructor` | `src/main.c` | translated |
| `Player_DetachGimmick` | `objects/obj_pmirage.c` | translated |
| `Player_DrainBoostTension` | `dcp_drain_boost_tension` | translated |
| `Player_DrainTension` | `dcp_drain_tension` | translated |
| `Player_Draw` | `src/main.c` | translated |
| `Player_ExitToSpecialStage` | `ui/ui_state.c` | translated |
| `Player_FindHomingTarget` | `objects/obj_manager.c` | translated |
| `Player_GetArchiveCompressionType` | - | replaced (converted data, `player/tools`) |
| `Player_GetArchiveUncompressedSize` | - | replaced (converted data, `player/tools`) |
| `Player_GiveBarrier` | - | not ported (VS mode / replay) |
| `Player_GiveInvincibility` | `sound/bgm.c` | translated |
| `Player_GiveMagnet` | `objects/obj_gimmicks.c` | translated |
| `Player_HandleSurfaceLanding` | `dcp_handle_surface_landing` | translated |
| `Player_In` | dcp_player_in: timers, layer override, confusion, slowdown, invulnerability, death plane, landing, boost, crush; replay/net, event spawn, palette shifts, underwater, homing search, grind start and time-over left to other modules | partial |
| `Player_InitState` | `dcp_player_init_state` | translated |
| `Player_IsPlayer2` | - | not ported (VS mode / replay) |
| `Player_Last` | dcp_player_last: tension debt, dash-timer reset, 3D-flag bookkeeping, gravity restore, layer restore; camera, magnet, exits, net, arena wrap not translated | partial |
| `Player_LoadArchive` | `ui/ui_state.c` | translated |
| `Player_LoadAssets` | - | replaced (converted data, `player/tools`) |
| `Player_LoadPhysicsParams` | `dcp_player_load_physics` | translated |
| `Player_MoveAlongCorkscrew` | `objects/obj_pgimmick.c` | translated |
| `Player_MoveAlongCorkscrewPath` | `objects/obj_pgimmick.c` | translated |
| `Player_OnHit_GliderSequence` | `objects/obj_paltitude.c` | translated |
| `Player_OnHurt` | `dcp_on_hurt` | translated |
| `Player_OnPushRival` | - | not ported (VS mode / replay) |
| `Player_PlayReplayFrame` | - | not ported (VS mode / replay) |
| `Player_ReadInput` | dcp_player_in (inline) | translated |
| `Player_RecordReplayFrame` | - | not ported (VS mode / replay) |
| `Player_ReleaseAssets` | - | replaced (converted data, `player/tools`) |
| `Player_RemapButtons` | `dcp_remap_buttons` | translated |
| `Player_ResetGimmickState` | `dcp_reset_gimmick_state` | translated |
| `Player_Respawn` | dcp_respawn: position, lives, flags; camera/stage restart via hook; VS-mode respawn point (unk408) not translated | partial |
| `Player_SendNetState` | - | not ported (VS mode / replay) |
| `Player_SetAnimSpeedFromVelocity` | anim_speed (dcp_states.c) + hook | translated |
| `Player_SetRunAnimationForSpeed` | `dcp_set_run_anim_for_speed` | translated |
| `Player_SetSourceMapObject` | `objects/obj_player.c` | translated |
| `Player_SettleAirRotation` | `dcp_settle_air_rotation` | translated |
| `Player_SpriteCallback` | `objects/obj_player.c` | translated |
| `Player_StartBoost` | `dcp_start_boost` | translated |
| `Player_StartHighSpeed` | `dcp_start_highspeed` | translated |
| `Player_StartLaunchTrick` | `objects/obj_player.c` | translated |
| `Player_State_Air` | `objects/obj_deadline.c` | translated |
| `Player_State_AttachedToObject` | `objects/obj_pwater.c` | translated |
| `Player_State_BendStick` | `objects/obj_pgimmick.c` | translated |
| `Player_State_Blank` | `boss/boss7.c` | translated |
| `Player_State_BreatheAirBubble` | `objects/obj_pwater.c` | translated |
| `Player_State_Cannon` | `objects/obj_deadline.c` | translated |
| `Player_State_Cargo` | `objects/obj_pmirage.c` | translated |
| `Player_State_CorkscrewPath` | `objects/obj_pgimmick.c` | translated |
| `Player_State_Crouch` | `dcp_state_crouch` | translated |
| `Player_State_Die` | `dcp_state_die` | translated |
| `Player_State_Glider` | `objects/obj_paltitude.c` | translated |
| `Player_State_GliderSequence` | `objects/obj_paltitude.c` | translated |
| `Player_State_GliderSequenceExit` | `objects/obj_paltitude.c` | translated |
| `Player_State_GliderSequenceIntro` | `objects/obj_paltitude.c` | translated |
| `Player_State_GmkSpecial` | `objects/obj_pgimmick.c` | translated |
| `Player_State_GmkSpecialEnter` | `objects/obj_pgimmick.c` | translated |
| `Player_State_Grind` | `objects/obj_pgimmick.c` | translated |
| `Player_State_GroundIdle` | `dcp_state_ground_idle` | translated |
| `Player_State_GroundMove` | `dcp_state_ground_move` | translated |
| `Player_State_Hand2D` | `objects/obj_pmirage.c` | translated |
| `Player_State_HomingAttack` | `dcp_state_homing` | translated |
| `Player_State_Hover` | `dcp_state_hover` | translated |
| `Player_State_Hurt` | `dcp_state_hurt` | translated |
| `Player_State_InAir` | `dcp_state_in_air` | translated |
| `Player_State_JetCatapult` | `objects/obj_player.c` | translated |
| `Player_State_JetCatapultLaunch` | `objects/obj_pcrisis.c` | translated |
| `Player_State_MapObj40` | `objects/obj_pwater.c` | translated |
| `Player_State_MapObj80` | `objects/obj_pwater.c` | translated |
| `Player_State_Parachute` | `objects/obj_altitude.c` | translated |
| `Player_State_ParachuteRelease` | `objects/obj_altitude.c` | translated |
| `Player_State_Pipe` | `objects/obj_pcrisis.c` | translated |
| `Player_State_Rocket` | `objects/obj_peffect.c` | translated |
| `Player_State_Roll` | `dcp_state_roll` | translated |
| `Player_State_Rope` | `objects/obj_pgimmick.c` | translated |
| `Player_State_RopeC` | `objects/obj_pgimmick.c` | translated |
| `Player_State_SandSlide` | `objects/obj_pmirage.c` | translated |
| `Player_State_SandSlider` | `objects/obj_pmirage.c` | translated |
| `Player_State_SandSlider90` | `objects/obj_pmirage.c` | translated |
| `Player_State_Saucer` | `objects/obj_paltitude.c` | translated |
| `Player_State_SpinDash` | `dcp_state_spindash` | translated |
| `Player_State_Swing` | `objects/obj_deadline.c` | translated |
| `Player_State_SwingSky` | `objects/obj_pcarnival.c` | translated |
| `Player_State_Warp` | - | not ported (VS mode / replay) |
| `Player_State_WaterRun` | `objects/obj_pwater.c` | translated |
| `Player_State_Wheel2D` | `objects/obj_pwater.c` | translated |
| `Player_State_ZoneLaunch` | `objects/obj_pwater.c` | translated |
| `Player_StopBoost` | `dcp_stop_boost` | translated |
| `Player_StopHighSpeed` | `dcp_stop_highspeed` | translated |
| `Player_StopSkid` | `dcp_stop_skid` | translated |
| `Player_SwingAroundPivot` | `objects/obj_pdeadline.c` | translated |
| `Player_SwingAroundPivotRetract` | `objects/obj_pdeadline.c` | translated |
| `Player_Unk_ov000_023460ec` | `dcp_update_ride_max` | translated |
| `Player_Unk_ov000_02350adc` | `objects/obj_deadline.c` | translated |
| `Player_UpdateBoost` | dcp_update_boost: all physics; VS rival effect (EffVs_Create) and the Blaze flame effect go to the effect hook | partial |
| `Player_UpdateCamera` | `src/main.c` | translated |
| `Player_UpdateCameraLead` | `src/main.c` | translated |
| `Player_UpdateGimmickStageBounds` | `player/dcp_probe.c` | translated |
| `Player_UpdateGliderSequence` | `objects/obj_paltitude.c` | translated |
| `Player_UpdateGravityFlip` | `dcp_update_gravity_flip` | translated |
| `Player_UpdateHighSpeedTimer` | `dcp_update_highspeed_timer` | translated |
| `Player_UpdateRunAnimation` | `dcp_update_run_anim` | translated |
| `Player_UpdateUnderwater` | `objects/obj_manager.c` | translated |
| `Player_WarpToRival` | - | not ported (VS mode / replay) |
| `Player_WrapLoopingArena` | - | not ported (VS mode / replay) |

Counts (206 functions): 189 translated (in the player module or, for gimmick states, in the objects module), 8 partial or interface only, 4 replaced by converted data, 14 not ported (VS mode / replay), 1 with no separate function.

## 6. Deliberate deviations

All of these are listed here. There are no silent changes.

1. **Side effects become hooks** (`g_dcp_hooks`): sound (`Sound_PlaySeqArcEx`
   ids passed as is, with channels VOICE = @384, BOOST = @38C, TRICK = @390,
   GLOBAL = NULL handle), effects (`DCP_FX_*`), screen shake, HUD tension
   gauge, score popups, ring spawn, collision-box atk/def flags,
   gravity-flip query, fades, stage restart, extra lives. The physics order
   is unchanged: each hook sits where the original call was. Palette shifts
   (`ModelPalette_ShiftColor`) and volume changes are dropped.
2. **Animation**: `dcp_set_anim` stores the id in `anim` (@328, the raw id,
   before the Blaze offset of 0x1b*chara), and clears `ANIM_DONE` (8),
   `LOOP` (4) and, after the hook, the blend request bit 0x400. The model
   part of Player_SetAnimation and the animation clock live in
   `dcp_model.c` (section 8): `DCP_DF_ANIM_DONE` is raised there from the
   joint animation, as StageObj_Draw3DEx does. The host physics test never
   sets it; `test_model.c` checks the timing.
3. **Inputs**: `dcp_player_frame(held, pressed, released)` replaces `gPad`.
   The remap (`Player_RemapButtons`) and its place in `Player_In` (after the
   gravity flip) are unchanged.
4. **Double constants** in `Player_LoadPhysicsParams` (top x 0.15, 0.3, 0.5,
   0.7, 0.9) are computed by `dcp_mul_double`. This is an integer emulation
   of the IEEE double multiply (53-bit round to nearest even, then truncate),
   because KOS builds use `-m4-single-only`, where `double` is `float`. Note:
   `0.7` in double is below 0.7, so `90*0.7` = 62. Checked against real
   double for all 5 x 65536 s16 inputs.
5. **Trig tables** are generated, not read from the ROM; they are verified
   identical to the ROM.
6. `Player_Action_GroundIdle`: the crouch-pose branch needs
   `model (@124) == NULL`, which never happens for the 3D player model. It
   is compiled out (always the stand animation 0x3c), which is what the
   original does in practice.
7. `RingManager_ScatterRings`: the spread-level cap is 4. The original uses
   3 when save option bit 0 is set; that option is not modelled.
8. Not translated: replay/net (`Player_RecordReplayFrame`,
   `Player_PlayReplayFrame`, `Player_ApplyNetState`, `Player_SendNetState`),
   `StageEvent_SpawnInRect`, underwater, homing target search (the object
   module sets `p->homing`), grind start, time over, VS-mode features, and
   all gimmick states and actions (rails, pipes, ropes, poles, cannons,
   gliders, sand, saucers, wheels, goal). These return as the stage objects
   are ported.
9. `Player_State_GroundIdle`: the `StageObj_IsTouched` push animation (0x45)
   needs the hit-box system and is skipped. The rest of the 0x45 / 0x47
   logic is kept.
10. `DcpWorld` gathers the globals the player reads: `gGameState` zone, act,
    mode, flags and frame; `gObjManagerFlags`; the map x bounds; the death
    line; the time scale; wind; `gRandSeed`; lives; checkpoint. The engine
    must keep it up to date. `dcp_player_frame` increments `frame` for test
    convenience; the engine may own it instead.

## 7. Build and test

```sh
cd dreamcast/game/player
make tables     # BYOR: build/dreamcast/player/{physics,trig}.bin from extract/eur
make host-test  # gcc -std=c99 -Wall -Wextra -Werror -fwrapv; runs the test (uses z11 level.bin if present)
make dc         # kos-cc (sh-elf-gcc) -O2 -fwrapv -Werror, objects in build/dreamcast/player/dc/
```

`test/test_player.c` builds synthetic worlds as real `rlvl_map_t` from a
heightfield, so it exercises the same path as a real level. The expected
values are derived from the ROM constants (Sonic: acc 24, top 0x900, dec 64,
jump 0x4e0, gravity 0x2a) with closed-form rules taken from the
decompilation. The test has its own reference loop and does not call the
module's state machine for its expectations.

Real level (`build/dreamcast/stages/z11/level.bin`, Leaf Storm act 1, from
the stage module):

- The player starts at the PlayerStart object (type 114, at 128,768), idles
  30 frames, then holds right for 870 frames.
- Grounded in 815 of 900 frames. It follows the slopes (angles 08..24), with
  one 2-tile drop at x of about 1700.
- From x of about 3390 it runs the loop continuously: the angle goes
  00, 3a, 5a, 88, b6, e8, 00. In the original, path-switch objects change
  the collision layer to leave the loop. Those objects are not ported, so
  the player keeps circling. This is expected and shows that wall and
  ceiling running work.
- Max x 3521 px. Full log: run `make host-test`.

Not verified:

- No frame-by-frame comparison with the DS game (no emulator traces are
  available).
- The SH-4 build was compiled but never run (no SH simulator in this
  environment).
- Object collision (`obj_probe`) is untested, because no object module
  exists yet.

## 8. Model presentation (`dcp_model.c`)

`dcp_model.c` / `dcp_model.h` translate the model half of the player's draw:
Player_SetAnimation (model part), Model_SetResource / Model_SetAnimation /
Model_AnimateMdl, StageObj_Draw3DEx for the player, the model's own matrix ops
from Player_Create, Player_Draw (after-image) and StageTitleCardPlayer_Draw.

Root causes it replaces (all in the old `draw_player_model` of main.c):

| defect | old code | original |
|---|---|---|
| model above the ground (F) | model origin (feet) at the player centre | Player_Create: `w->unk18C = -0x10000` is the Translate2 vector of the model (`cb` = FlushVP, Identity, RotateTranslate2Scale): v * S(0x34cc) * T(0, -16, 0) * R + pos. The feet sit 16 px below the centre and the rotation pivots on the centre (slopes, loops) |
| left/right mirrored, spinning (A) | facing from `pf & DCP_PF_FACE_L` (@0x32C bit 0, the "no jump cut" flag of the air states) | facing is `disp_flags & 1` (@0x5C), RotY(0xC000 / 0x4000) |
| ball animations broken (B) | only table 0x02368bdc used; every animation played on the body model | 0x02368c44 selects the model resource (1 = son_ball / blz_spin for ids 0x30..0x34 and Blaze 0x4b/0x4c), 0x02368cac the visibility animation (0 = keep, none on the ball); ids >= 0x30 + 0x1b for Blaze |
| restarts, stuck one-shots (A, B) | restart only when the joint index changed; +1 frame per frame; ANIM_DONE from son.bac sprite timing | every Player_SetAnimation restarts at frame 0 with speed 1.0; fx32 frame += speed * per-slot speed * timescale (Player_SetAnimSpeedFromVelocity: speed = s << 8); loop / clamp at numFrames; the ended bit raises ANIM_DONE (8) every frame the joint animation is at its end, once per cycle when looping; hit-stop / pause freeze the frame and keep 0x5C; 8-frame cross-fade (blend speed 0x200) on request 0x400 |
| no blink | hidden flag ignored | 0x5C & 0x20 hides the model (invulnerability blink) |
| no visibility animation | `dca_visibility(vis, 0, 0)` (the "dummy" set, frame 0) | com_gen00.. per the table, advanced with the joint clock |

Use from main.c (player/patches/main.c.patch): `dcp_model_load(&g_dcp_model,
chara, "/cd/player", 1)`, `dcp_model_attach` before `objects_init` (chains the
anim / anim_speed hooks), `objects_set_player_anim_done(!model_ok)`,
`dcp_model_step` once per world frame after `dcp_player_frame`,
`dcp_model_matrix` / `_matrix_ext` / `_lights` / `_afterimage` /
`_titlecard_view` / `_draw` in `draw_player_model`. Data: `son_ball.dcm`,
`blz_spin.dcm` and `animtab.bin` (the four ov000 tables, written by
`tools/extract_physics.py`) on the disc (player/patches/build_disc.sh.patch);
without `animtab.bin` the module derives the resource from the animation's
joint count.

Host test: `make model-test` (30 checks: table selection for Sonic and Blaze,
ball resource, one-shot / loop timing, ANIM_DONE pulses, restart, hit-stop,
blend, feet 16 px below the centre, slope pivot, a z11 run for both
characters with no wrong facing and no per-frame restarts).

Deviations: sampling uses the integer NNS frame (no interpolation, as the DS);
the blend renormalises rows 0/1 and rebuilds row 2. Light colours: the stage
table data_02074960 [9 zones][3 acts][3] (`tools/extract_physics.py` writes
`lights.bin`; `dcp_model_stage_lights`), overridden by the after-image (0x6182
Sonic, 0x1a98 Blaze) and underwater (0x7ef5 / 0x4548 / 0x2040) colours as in
Player_Draw / Player_Last. Without lights.bin on the disc the common
0x7fff / 0x4631 / 0x2108 is used. The title card's light fade-in
(StageTitleCard_State_RestoreColors lerps from 0) is not applied.

## 9. Stage bounds, ring scatter level, DS R

- `dcp_bind_map` sets `g_dcp_world.death_y` (map height, Player_In pit line) and `map_left` /
  `map_right` (0 / width); `dcp_player_in` runs Player_UpdateGimmickStageBounds (gimmick-held:
  +-0x6c px around it, pf2 0x200). The vertical pair (pf2 0x400) has no consumer.
- Player_OnHurt scatters with `g_dcp_scatter_level` (the ring manager's unk9D5, set by the objects
  module), falling back to a private level without a ring manager.
- DCP_FX_TRICK_FORWARD / DCP_FX_TRICK_SPARKLE (appended to the effect enum) separate the trick
  effects that shared DCP_FX_TRICK.
- The pad needs DS R (0x100) for tricks and the air action: player/patches/input_r_trigger.patch.

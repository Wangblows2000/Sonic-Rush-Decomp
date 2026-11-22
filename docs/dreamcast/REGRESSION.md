# Player presentation and interactive objects: regression checklist

Measured 2026-11-22 with the scratch build of `dreamcast-port` plus the
integrator patches in `dreamcast/game/player/patches/` (main.c, build_disc.sh).
Flycast = `/home/claude/dcwork/flycast/build/flycast`, software GL, own Xvfb
display; the emulator ran at 25-60 frames per second, so long runs were cut
short. Nothing here was run on Dreamcast hardware.

Host tests (all pass):

| test | command | result |
|---|---|---|
| player physics | `make -C dreamcast/game/player host-test` | 128 / 128 |
| player model presentation | `make -C dreamcast/game/player model-test` | 34 / 34 |
| objects (z11, z12) + zones | `make -C dreamcast/game/objects host-test` | z11 112 / 112, z12 111 / 111, 14 zone stages all pass, then `make audit` |
| item-C audit + core gameplay | `make -C dreamcast/game/objects audit` (16 stages x Sonic, Blaze) | 32 runs, 0 failed (sections 5, 9) |
| UI | `make -C dreamcast/game/ui host-test` | 114 / 114 |

Legend: **pass** = checked and matches the decompiled behaviour;
**partial** = works, with the gap named; **not checked**.

## 1. Walking and running (defect A)

| check | how | result |
|---|---|---|
| model faces the travel direction, right and left | test_model z11 run (Sonic and Blaze): 0 frames facing the wrong way over 116 / 106 left-running frames; Flycast z11: walk right, turn, walk left, idle facing left (screens c0..c7) | pass |
| no spin in place / no per-frame restart | test_model: longest run of consecutive Player_SetAnimation frames 2, 0 same-id restarts; Flycast log: run anims 0x3d..0x42 advance, turn 0x00 (com_turn01) plays once and hands over | pass |
| run speed follows the velocity | Player_SetAnimSpeedFromVelocity -> speed s << 8; test_model: s = 0x6c gives speed 0x6c00 and the wrap frame computed from it | pass |
| slope rotation | z11 slopes in Flycast (screen b3): feet on the grass, body rotated about the centre | pass |

## 2. Ball animations (defect B)

| animation | id (Sonic / Blaze) | model / joint animation | result |
|---|---|---|---|
| jump | 0x30 / 0x4b | son_ball son_jump_01 / blz_spin blz_screw | pass (test_model, Flycast c8/c9, m1) |
| roll | 0x31 / 0x4c | son_ball son_screw | pass (Flycast log: f534..f606 res 1, frames advance with the speed hook) |
| spin dash charge / rev | 0x32, 0x33 | son_ball son_scr_up, son_scr_wt | pass (Flycast c11..c13, log f494..f526) |
| 0x34 | 0x34 | son_ball (joint 0x4a) | pass (table) |
| fall, bounce, spring, hurt, landing (body) | 2, 0x13/0x14, 0x0f, ... | body model + com_gen00 visibility | pass (table, Flycast logs) |
| transitions ball <-> body | Model_SetResource: visibility dropped on the ball, re-attached on the body, no cross-fade | pass (test_model) |
| one-shot end (ANIM_DONE) | skid 0x05: ANIM_DONE on frame numFrames, then every frame, pose held | pass (test_model) |
| loop cycle pulse | idle 0x3c: one pulse per cycle | pass (test_model) |

## 3. Ground contact (defect F)

| check | result |
|---|---|
| flat ground | pass: lowest vertex 16.4 px below the player centre (floor at +14); Flycast z11 screens show the shoes on the grass |
| slopes, loops | pass: the (0, -16, 0) translation is applied before the rotation (pivot = centre); test_model angle 0x20 check |
| boss arena (z13) | pass: Flycast n3..n7, Sonic on the arena line |

## 4. Loops (defect G)

| check | result |
|---|---|
| z11 first loop (x 5380..5700), standing start on layer B | pass: objects host test `[loop]` (18 frames on the ceiling, exits to x 6390); host harness from 0 / 432 / 768 / 1000 entry speed, Sonic and Blaze, with and without boost |
| z11 loop in Flycast | pass: debug start at x 4618: layer A at the entry switch, ceiling, layer B at the top, out at x 6348 (log f8..f208) |
| z12 attract demo 1 (start 17048, 3346) | **open** (section 8) |

## 5. Interactive objects (defect C): per-class audit

`make -C dreamcast/game/objects audit` runs `test_objects` with `AUDIT_ONLY=1` on z11, z12 and the 14
zone stages, for Sonic (`CHARA=0`) and Blaze (`CHARA=1`). Each class is touched through the real
collision path (boxes registered one frame, checked at the next `objects_frame_begin`), and the
result is compared with the formula of the decompiled `*_OnHit`. Counts are the sum over the 32 runs.

| class | original | checked | result |
|---|---|---|---|
| springs 0x43..0x4c (all 10 directions) | Spring_OnHit: v = -0x780 - p6 * 0x180, diagonals * 0xb5 >> 8, 5C & 1 / 2 flip; side springs y = spring + 2 px; Player_Action_Launch: anim 0x16 / 0x13, pf |= 3, airborne, dash timer 0x40 sideways, facing; under water 3/4; zone 4 SpringLaunchAlt (+ sound 0x72) | OnHit velocity (logged), velocity after the player task (ApplyAirInput / ApplyGroundInput clamp to +-top with no input, gravity once; 0x13 -> 0x14 when falling > 0x40), pose, flags, one launch per contact | 646 / 646 |
| dash panels 0x5f..0x62 | DashP_OnHit: grounded only, x = +-top by 5C & 1 (0x61/0x62: y by 5C & 2); Player_Action_DashPanel: dash timer 0x80 | no boost in the air; on the floor gnd = +-top, timer 0x80; ceiling and wall panels fire (walls: Player_Last clears the timer on the wall push-out in the same frame) | 238 / 238 |
| rail dash panels 0x5c | grinding only (anim 0x1a..0x20) | no boost in the air over it; standing on the rail starts the grind (Player_CheckStartGrind) and the panel fires, +-top | 60 / 60 (2 panels in z71 have no rail under them on layers A/B or rail layers 1..3: skipped) |
| needles 0x4d | Player_OnHurt: +-0x180, -0x300, rings scattered, invulnerable invuln_len frames (Sonic 180, Blaze 120), blink | hurt once, knockback, no second hit while invulnerable, blink, hurt box back to def 2 | 24 / 24 runs with needles |
| item boxes 0x30 | ItemBox_OnHit on contact (not during respawn invulnerability), ItemBox_GiveItem by p6 | broken once, p6 1 gives 5 rings, p6 0 gives one of data_ov000_02366b44 | 122 / 122 |
| markers 0x6e | Marker_OnHit: respawn (x, y - (box_b + 1)), index, tension +800 | passed once, respawn point and index | 56 / 56 |
| dash rings 0x63..0x66, 0x6a..0x6d | DashC_OnHit: (cos, sin)(dir << 13) * 0x800 >> 12; Player_Action_DashRing: at the ring, GimmickLaunch, gravity off, timer 0x1e; DashCT: Player_StartLaunchTrick (anim 0x38) | velocity exact after the player task, gravity flag off, timer, anim (0x18 / 0x13, -> 0x19 / 0x14 when falling) | 162 / 162 |
| trampolines 0x55 | Trampoline_OnHit: v = -(my + my/8 + my/16) clamped -0xc00..-0x780, SpringLaunch(0, v) | from four fall speeds: vel_y = v + gravity | 12 / 12 |
| goal ring 0x71 | GoalRing_OnHit once (obj_flags 2) | goal hook once while held in it | 28 / 28 runs |
| corkscrews 0x68 / 0x69 | CorkscrewTrigger_OnHit: gnd_vel >= run_thr[2] in its direction | nothing below the threshold, starts at it | 10 / 10 |
| ropes 0x58 | Rope_OnHit: grounded, abs(gnd_vel) >= run_thr[2], one rider | not slower, not in the air, caught at the threshold (anim 0x24) | 22 / 22 |
| breakable rocks 0x6f | RockBreak_OnHit: airborne only, AttackBounce -0x300 | standing on it: no break; jumping on the spot: break, vel_y -0x300 + gravity | 8 / 8 |
| breakable walls 0x77 | WallBreak_OnHit: mode 0 any, 1 > run_thr[2], 2 > run_thr[4] (max of gnd_vel, vel_x), hit-stop 4 | no break 0x40 below the limit, break 0x40 above | 84 / 84 |
| enemies (pawn / flapper groups) | jump attack: destroyed + AttackBounce; Egg Hammer two hits; Rhino back spring (EnemyPawnGroup_OnHit_Box2_Bounce) | jump onto the nearest live enemy of each group | 142 / 142 (drop-in pawns, p9 != 0, skipped) |
| homing targets, boost knock-away, egg pawn hurt | test_objects [targets] / [v2] (z11, z12) | pass |
| boost through hazards | Player_UpdateBoost `2C4 |= 5`, Player_StopBoost `= 2` | hurt box def & 5 on all boost frames, 2 after | pass |
| layer switches MapObj40 0x41 / 0x42 / 0x59 | z11 loop checks (section 4) | pass |
| zone gimmicks (water, mirage, carnival, crisis, altitude, deadline, ...) | `test_zones` per zone stage | 14 stages pass; no per-formula audit beyond those tests |

Repeat triggers: dash panels fire on every frame of overlap (DashP_State_Idle clears 0x100 each
frame, as on the DS; Flycast z11 log: 4 dash_panel events f121..f124 for one pass).

Flycast (z11, scratch disc with an event trace in the serial log):

| check | result |
|---|---|
| spring 0x43 (1616, 1216) | pass: event `spring 67`, OnHit (0, -3456) = -0x780 - 3 * 0x180, player vel_y -3456, anim 0x13 |
| item box, grind vine, corkscrew, floor dash panel (15700..16700), Sonic and Blaze | pass: events item_box (p6 1, +5 rings), grind, corkscrew (flags 2), dash_panel +-top (Sonic 2304, Blaze 2176) |
| no input after the dash panel at 16664 | the player climbs the wall at x 16754, falls back onto the panel and is boosted again until right is held (host: holding right passes at frame 276, gnd 2536). Matches the decompiled slope force; not a port defect as far as the code goes |

## 6. Level intros (defect D)

| stage | title card close-up | hold | handoff | result |
|---|---|---|---|---|
| z11 (Sonic) | slow-motion close-up, camera move, zoom to the stage size (screens t0..t11, u0..u3) | HoldPlayerX: x wraps 128..384, camera snapped after the wrap (no jump) | "Go!", control returns at StartStage, x reset to the start | pass |
| z11 (Blaze) | "On Your Mark" / "Ready?" close-up of the Blaze model, "BLAZE THE CAT" banner, "ZONE 2 ACT 1" (screens i0..i3) | runs in place behind the card | "Go!", Blaze on the stage (i4..i7) | pass (the zone number shown for Blaze is the UI module's) |
| z13 boss (Sonic) | close-up and zoom (n0, n1) | none | boss starts after the card | pass |
| z31 (Sonic) | close-up of the falling pose (o0..o2) | HoldPlayerY: y wraps 785..961 | drop into the stage (o3..o7) | pass |
| skip with A | | | no jump leaks into the stage (log f2..f98) | pass |

## 7. Boost visuals and sound (defect E)

| check | result |
|---|---|
| EffBoostBurst + EffBoostAura at boost start | pass: Flycast z11 k0..k4; host: burst 12 frames, aura on all 40 boost frames |
| aura follows, rotates with the movement, ends with the boost | pass: host (0 aura objects 4 frames after release) |
| semi-transparent sprites (flag 0x400) | pass: TR list, alpha 0xa0 |
| EffBoost dust in high-speed mode | pass: host 59 / 60 frames |
| EffHighSpeedStart | pass by code path (not created when boost starts high-speed mode, as on the DS) |
| Blaze boost flames (EffBlazeBoostFlame) | pass: host 11 creations over 40 boost frames; Flycast b0..b4 (Blaze, red flames behind the aura) |
| Blaze run flames (EffBlazeRunFlame, every 4 frames past run_thr[1]) | pass: host 17 creations over 90 running frames; Flycast b5..b7 (flames behind her after the boost) |
| Blaze spin flames (EffBlazeSpinFlame, jump anim 0x30 / rolling, every 8 frames, circling her) | pass: host, up to 5 flames following her during the jump |
| Blaze trick flames (EffBlazeTrickFlame, trick spins 0x35 / 0x36) | implemented, created on DCP_FX_BLAZE_FLAME a = 3; not captured |
| spin dash smoke, brake smoke, air-dash jet | implemented (EffSpinSmoke1/2, EffBrakeSmoke, EffBoostJet); not captured |
| high-speed after-image (Player_Draw) | implemented: previous-position copy with the 0x6182 / 0x1a98 light colour; visible in Flycast (Blaze, golden ghost, z3) |
| EffTrail (Sonic's boost ribbon) | pass: host: 10 segments while boosting, alpha 6 -> 0x12 -> 4 along it, colour 0x7ca5, gone 10 frames after the boost ends (EffTrail_State_Follow); none for Blaze. Flycast z11 b0/b4: blue translucent ribbon behind Sonic along his path |
| zone light colours (data_02074960) | pass: lights.bin [9 zones][3 acts][3]; dcp_model_lights uses them (Leaf Storm 0x7fff / 0x4631 / 0x2108; zone 4 boss 0x7fff / 0x75ed / 0x2c39), after-image and under-water colours override as in Player_Draw. Needs the build_disc.sh patch `player/patches/build_disc_lights.patch` (without it: the common 0x7fff / 0x4631 / 0x2108) |
| boost sound cues | host: voice 0xa (Sonic) / 0x17 (Blaze) and the boost channel stop in the frame pf 0x80 is set. Driver render of the z1s bank: 0x0a SE_Superboast 2 notes, 2.54 s (offline render 83033 samples, driver 83204: one 5.2 ms tick longer), audible 1.91 s, onset 10.4 ms, peak -15.7 dBFS, RMS -24.0 dBFS, SNR vs the offline render 9.2 dB; 0x17 SE_B_Superboast 2.51 s, audible 1.88 s, onset 10.4 ms, peak -17.3 dBFS, RMS -25.5 dBFS, SNR 12.7 dB (bank median 13.1). Not measured in Flycast: the serial trace slowed the emulator so much that the boost press fell inside the title card |

## 8. z12 attract demo 1 desync

**Cause found: the demo lead frames.** DemoPlay_Create starts DemoInput before StageStart loads
the stage, so the DS consumes the first records of key01.bin while no player exists (the frames the
card reads take); the port's `DEMO_LEAD_FRAMES` is 0. Host replay of every demo with the lead
swept (`loop` harness, `LEAD=n`):

| lead | z12 demo 1 (Sonic, 17048, 3346) | z21 demo 3 (Blaze, 306, 2034) |
|---|---|---|
| 0..21 | stalls at the rock (17312), breaks it at frame 121, cycles on the rail (max x 18247), 13 rings | max x 5878, 1 hurt |
| 22 | max x 23682, 73 rings, 2 hurts | max x 5878, 1 hurt |
| **23** | **reaches the goal ring (goals 1)**, 63 rings, 1 hurt, rock not touched | **max x 7246, 55 rings, no hurt** |
| 24 | max x 21138 | max x 7247, no hurt |
| 26..30 | worse | worse |

With 23 frames of lead the recorded inputs play the act through to the goal ring, so the port's
physics follow the DS recording. The data candidates were checked and are not the cause: the
son.bac hit boxes the port uses equal the ROM's (jump 0x30: slot 0 (-8,-8,8,8), slot 1
(-8,-9,8,9); fall 2: (-6,-16,6,4); stand / run (-6,-16,6,16)); the map box is constant
(-7,-12,7,13, Player_InitState, no change while jumping); the demo start is Player_Create at
data_022c4590.unk1C/unk20 << 8 (no fraction, zero velocity), which the port does. Fix:
`dreamcast/game/player/patches/demo_lead_23.patch` (demo.h `DEMO_LEAD_FRAMES 23`; demo host
tests 73 / 73 with it). Not run in Flycast (the attract demo needs the title-screen idle; the
emulator ran at about 5 frames per second under the shared load).

## 9. Core gameplay (tricks, homing, rings, lives, death)

Host checks in `test_objects` (z11, z12 and the 32 audit runs), each against the decompiled function:

| item | original | result |
|---|---|---|
| forward trick (R after a launch) | Player_Action_TrickForward: Sonic vel_x += +-2 * jump, vel_y 0, dash timer 0xc; Blaze vel_x = +-2 * boost_top, frozen 0xc frames; pose 0x37; Player_AddTrickScore(200) | pass, Sonic and Blaze |
| up trick (Up + R) | Player_Action_TrickUp: vel_y = -jump (Blaze -2 * jump, hit-stop 0x44), pose 0x35 | pass |
| trick combo | Player_State_InAir gate (no new pose within 0x18 frames), Player_Action_TrickCombo: B -> 0x39 (+400 >> lvl tension), B -> 0x3a (+400 >> lvl, pf 0x4000), A -> 0x3b (+0x280 >> lvl, two AddTrickScore); lvl = the source object's counter (ObjParam +0xA) | pass: 0x39 / 0x3a / 0x3b after 25..31 frames, tension +400 / +400 / +640 |
| trick score | Player_AddTrickScore: (0 / 100 / 200 / 300 by count) >> lvl + 200 >> lvl | pass (score hook pairs) |
| R in a plain jump | not a trick: the air action (Player_Action_HomingAttack / Hover) | pass |
| homing attack | Player_FindHomingTarget (nearest type-2 box, no range), Action_HomingAttack (d^2 < 0x8000, target not above), State_HomingAttack vel = (cos, sin) >> 1 | pass (vel 0, 2048 for a target below) |
| chained kills | no score per enemy in the decompiled code (score comes only from tricks, markers and the goal); MapObject_OnHit_Default: Player_AddTensionCombo(400) = +400, +600, +800 within 0x60 frames | pass |
| ring loss | RingManager_ScatterRings: min(rings, 64), the arc of the loop, level +1 (max 4) | pass (30 and 100 rings, levels 0 and 1) |
| scattered ring life | RingManager_SpawnRing 0x100 + (Rand & 0x1f); no pickup while timer >= 0xd8; blink (timer & 2) under 0x20 | pass |
| scatter level reset | RingManager_Main: back to 0 when every scattered ring expired uncollected (picking one up clears the flag and the level stays) | pass after the fix below |
| 1-up | Player_AddRings: a life at each 100 (story), cap 999; ItemBox_GiveItem kind 2 | pass (z12 1-up box) |
| pit death | Player_In: y >= map height - 0x1a (not in zone 6 act 0) | pass after the fix below |
| die / respawn | Player_Action_Die (0x11), Player_State_Die (0x3c frames + fade), Player_Respawn at the marker, rings 0, lives - 1; 0 lives: pf 0x800000 (game over for the UI) | pass |
| crush | Player_CheckCrushed: pf 0x800, no touch object, walls both sides | pass |
| time over | Player_CheckTimeOver | UI module (ui_state.c); not part of this audit |

Fixed port deviations:
- **Pit death never fired in the game**: `g_dcp_world.death_y` kept its initialiser 0x7fff and
  `map_right` 0x7fffffff (nothing set them outside the tests). `dcp_bind_map` now sets death_y = map
  height (StageLoader_OnMapArchiveLoaded) and map_left / map_right = 0 / width, and
  Player_UpdateGimmickStageBounds is translated (gimmick-held bounds +-0x6c px).
- **Ring scatter level**: Player_OnHurt used a private level that never reset; it now uses the ring
  manager's (RingManager unk9D5), reset by RingManager_Main.
- **No DS R button on the Dreamcast pad**: tricks, the homing attack / air dash / hover and the grind
  tricks read DS R (0x100), which `ds_pad` never produced. Patch
  `dreamcast/game/player/patches/input_r_trigger.patch` maps the right trigger to DS R (left to DS L).
- **Trick effects** were missing: EffTrickForwardBoost, EffTrick_CreateAtOffset, EffTrickSparkle,
  EffTrickCombo_Spawn / EffTrickComboPart (new slot OBJ_BAC_EFF_TRICK, ac_eff_trick.bac).

Flycast (scratch disc, inputs scripted by stage frame because the emulator ran at about 5 frames
per second; DS R injected through Y in the scratch build only, the keyboard trigger does not reach
the guest): spring 0x43 launch (vel_y -2646), R -> 0x37 (vel_x 2560), B -> 0x39 (tension 1600 ->
2000), B -> 0x3a (-> 2400), landing, A -> jump; a hurt with 0 rings -> die 0x11 / 0x12 -> respawn
at the start point. Matches the host run of the same inputs.

Zone gimmicks, formula level: water jet 0x85 (WaterJet_OnHit -0x780 - p6 * 0x180, 3/4 under water)
and launch zone 0x83 (LaunchZone_OnHit +-0xa00 * 0xb5 >> 8, Player_Action_ZoneLaunch (move + v) >> 1,
ObjSpd_DownSet 0x60) are now exact in `test_zones`. The other zone classes (water wheel, rafts, sand
slides, hands, cargo, neon routes, swings, switches, cranes, pistons, pipes, glider, parachute,
saucer, cannons, ducts, rockets) keep the activation checks of `test_zones`; their OnHit / Player_Action
functions were compared with the decompilation side by side (WaterJet, LaunchZone, Wheel2D,
SandSlider, Cannon, Rocket, Saucer_Action_Ridden, Parachute, GmkPipe) and match.

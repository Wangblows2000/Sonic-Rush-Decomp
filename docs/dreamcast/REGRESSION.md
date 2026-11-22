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
| player model presentation | `make -C dreamcast/game/player model-test` | 30 / 30 |
| objects (z11, z12) + zones | `make -C dreamcast/game/objects host-test` | z11 69 / 69, z12 67 / 67, 14 zone stages all pass |
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
| z12 attract demo 1 (start 17048, 3346) | **open**: replaying key01.bin on the host reproduces the drift. The area right of x 17860 is a wall -> ceiling run whose ceiling ends at x 18050; the player drops onto the A rail, whose left-facing rail dash panel (0x5c, flags 1) sends him up-left, and the recording then holds right, so he cycles. The recorded inputs (tricks with R at frames 272..306) show the DS player took another path earlier: at frame 144 (boost press) the port's player is still in the RockBreak attack bounce (lands at frame 166), so the boost start is lost and the replay desyncs. RockBreak, AttackBounce, DashP, Player_State_Grind were compared line by line with the decompilation and match. |

## 5. Interactive objects (defect C)

| class | check | result |
|---|---|---|
| boost through enemies / hazards | Player_UpdateBoost `2C4 |= 5`, Player_StopBoost `= 2`: objects host test (hurt box def & 5 on all 40 boost frames, 2 after release) | pass (was missing) |
| layer switches MapObj40 0x41 / 0x42 / 0x59 | z11 loop checks above | pass |
| springs, dash panels, rings, item boxes, checkpoints, goal, ropes, rails, pedestal, rock break, enemies | existing objects host tests (z11, z12, 14 zone stages) still pass; Flycast z11 run: spring at x 1640, rail grind 1887..2797, vine rope, rings collected | pass (no regression); no new per-class audit beyond the items listed |
| invulnerability blink | model hidden while 0x5C & 0x20 | pass (code path; not captured in Flycast) |

## 6. Level intros (defect D)

| stage | title card close-up | hold | handoff | result |
|---|---|---|---|---|
| z11 (Sonic) | slow-motion close-up, camera move, zoom to the stage size (screens t0..t11, u0..u3) | HoldPlayerX: x wraps 128..384, camera snapped after the wrap (no jump) | "Go!", control returns at StartStage, x reset to the start | pass |
| z11 (Blaze) | | | | not captured (the same code path) |
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
| Blaze boost flames | pass: Flycast m3/m4 (Blaze, boost, blz_spin ball) |
| spin dash smoke, brake smoke, air-dash jet | implemented (EffSpinSmoke1/2, EffBrakeSmoke, EffBoostJet); not captured |
| high-speed after-image (Player_Draw) | implemented: previous-position copy with the 0x6182 / 0x1a98 light colour; visible in Flycast b7 |
| EffTrail (Sonic's boost ribbon) | **not translated** |
| boost sound cues | unchanged hook path (voice 0xa / 0x17, boost channel stop); not measured |

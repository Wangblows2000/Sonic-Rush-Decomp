# Dreamcast port — known issues and blockers (2026-11-22)

## State (updated 2026-11-22, second pass)

Integrated into the running game since the first pass: HUD, act title card,
pause, act results, lives/game over/time over (ui/), native SSAR sound-effect
sequencer with per-stage banks (sound/), Boss1 Egg Hammer and the shared boss
framework (boss/, act 3 stages), all Leaf Storm act 1/2 objects, enemies,
gimmicks and decorations (objects/), Blaze selectable (hold Y at boot), story
progression z11 -> z12 -> z13 -> z21 ... on act clear.

First pass:

A playable vertical slice exists: any of the 14 act 1/2 stages loads natively;
Sonic (3D model from the owner's ROM) runs with the translated player physics;
rings, springs, dash panels, item boxes, spikes, checkpoints, the goal ring and
the Egg Pawn work through the translated object system; zone music streams
from disc as AICA ADPCM. Everything else in the game is not ported yet.

## Not implemented

- Bosses 2-7, F and Extra (Boss1 only); zone 2-7 gimmicks, enemies and
  decorations (objects are created but most behaviours are missing outside
  Leaf Storm, see translation/OBJECTS.md coverage table); title/main menus,
  zone map, VMU save, cutscenes, special stages, VS mode.
- Boss stages: 3D arena rings not drawn; defeat backgrounds/brightness not drawn.
- Title card / results backgrounds and close-up player model not drawn.

- HUD, title/menus, pause, results, save (VMU), lives/game over flow.
- Sound effects are not wired (the hooks exist; SFX are converted).
- Zone gimmicks other than the shared ones (rails, ropes, pipes, cannons,
  gliders, water, ...), most enemy types, all bosses (models convert, logic
  not translated), special stages, cutscenes (TalkDemo), Blaze selection
  (physics + model exist), Extra Zone, VS mode (no wireless equivalent).
- Background line-scroll/palette effects (ZoneFx) and per-zone parallax
  factors (a fixed 0.25/0.125 factor is used).
- Model lighting: no specular/toon/fog; near-plane clipping skips strips.
- Non-looping player animations are looped; DCP_DF_ANIM_DONE is never set by
  the model renderer, so transitions that wait for it (skid, spin-dash pose,
  death, trick poses) may stall.

## Verification limits

- Audio is now captured from Flycast (ALSA `file` device, AICA output, 44.1 kHz)
  and analysed, not listened to by ear: z11 run, 53 s emulated: silence while
  loading, music from ~20 s at -18..-31 dBFS RMS, sound-effect onsets during
  the run; two half-second windows hit full scale (peak 32762) around 48-50 s:
  clipping (several SFX at full level, or a fault) - not yet investigated.
- Offline renders (music) are 2.1-8.1 dB too quiet: sseqrender uses the squared
  dB table, the decompiled NNSi_SndPlayerMain uses the linear one (fix in
  tools/dreamcast/audio/sseqrender.c, re-run sdat2dc.py).

- Emulator only (Flycast built from source, HLE BIOS, software GL, Xvfb,
  keyboard mapped to the controller). No retail hardware test.
- Audio has never been listened to: playback is confirmed by stream throughput
  (32.7 KB/s = 32,728 Hz 4-bit stereo) and offline validation, not by ear.
- Frame rate figures are from the emulated clock (59.8 fps), not hardware.

## Data handling

All converted game data is generated locally from the owner's ROM
(bring-your-own-ROM) under build/ and is not committed or distributed.

## Boss1 arena (z13): sky not visible

In Flycast the Leaf Storm boss arena shows black behind the arena model, while
z33, z43 and z53 show their sky. Boss1_State_WaitForStart sets the camera sky
model (`mod__bs1_sky`, scale 0x1355b50, CAM.unk0 bit 0) the same way Boss3
does, the file is on the disc, and boss_draw draws CAM.unkE0 when the bit is
set. Not yet found: check the sky model's texture/material conversion
(bs1_sky is 67 KB vs bs3_sky 17 KB), far-plane clipping at that scale, and
whether the DS draws a 2D BG layer of the z13 level behind the 3D arena
(main.c skips level_draw_bg in boss stages).

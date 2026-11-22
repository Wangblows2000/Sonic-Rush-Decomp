# Dreamcast port — known issues and blockers (2026-11-22)

## State

A playable vertical slice exists: any of the 14 act 1/2 stages loads natively;
Sonic (3D model from the owner's ROM) runs with the translated player physics;
rings, springs, dash panels, item boxes, spikes, checkpoints, the goal ring and
the Egg Pawn work through the translated object system; zone music streams
from disc as AICA ADPCM. Everything else in the game is not ported yet.

## Not implemented

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

- Emulator only (Flycast built from source, HLE BIOS, software GL, Xvfb,
  keyboard mapped to the controller). No retail hardware test.
- Audio has never been listened to: playback is confirmed by stream throughput
  (32.7 KB/s = 32,728 Hz 4-bit stereo) and offline validation, not by ear.
- Frame rate figures are from the emulated clock (59.8 fps), not hardware.

## Data handling

All converted game data is generated locally from the owner's ROM
(bring-your-own-ROM) under build/ and is not committed or distributed.

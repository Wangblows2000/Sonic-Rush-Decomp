
## Player presentation / objects (gameplay-presentation agent, 2026-11-22)

- OPEN z12 attract demo 1 desyncs: the port breaks the rock 0x6f at (17344, 3312) at frame 121
  (AttackBounce, lands at 166); the DS recording boosts at frame 144, so the DS player broke it
  about 22 frames earlier. Every routine on the path matches the decompilation; suspects are data
  (son.bac per-frame boxes, jump map box) or sub-pixel demo start state. REGRESSION.md section 8.
- OPEN zone light colours need `dreamcast/game/player/patches/build_disc_lights.patch` (copies
  build/dreamcast/player/lights.bin to /cd/player/); without it the player model uses the common
  0x7fff / 0x4631 / 0x2108, so the Extra Zone / boss tints are missing.
- OPEN title card light fade-in (StageTitleCard_State_RestoreColors lerps the three light colours
  from 0 to data_02074960) is not applied to the player model.
- OPEN EffTrail_SetWrapShift (Player_WrapLoopingArena) is not translated: in looping boss arenas the
  boost ribbon of Sonic is not shifted with the wrap.
- OPEN boost sound cue timing not measured in Flycast (serial trace too slow); host: voice 0xa /
  0x17 + boost channel stop in the boost-start frame, driver render measured (REGRESSION.md 7).
- NOTE z71 rail dash panels at (33552, 1664) have no grind rail under them on layers A/B or rail
  layers 1..3 in a host placement test; not checked in game.

# Dreamcast port — known issues (2026-11-23)

All results are from the Flycast emulator. Nothing has been verified on real
Dreamcast hardware.

## State

The whole game runs natively from the translated decompiled code in one
640x480 4:3 view:

- SEGA logo, title intro, title screen, menus (options, sound test, records,
  time attack), player select, zone map with the character model and
  message boxes, VMU save.
- Sonic, Blaze and Extra stories: every act, every boss (1-7, final, Extra),
  special stages, cutscenes and talks, endings and staff rolls, unlock notices.
- Time attack, game over / time over, continue flow.
- 59.9-60 fps in every act, boss and special stage; VRAM and sound RAM stay
  flat over a 95-minute session.

Test method and latest results: REGRESSION.md. Building: BUILD.md.

## Differences from the DS (by design)

- One screen: the DS top and bottom screens are combined into one frame; HUD
  and second-screen information sit inside it.
- Controls: A jump, B trick, X boost, Y alternate, START pause, right / left
  trigger = DS R / L (tricks, air actions). Special stages: Trick Spring
  numbers are shown as Dreamcast buttons (1 A, 2 B, 3 X, 4 Y, 5 L, 6 R,
  7-10 D-pad); "PRESS START TO BEGIN" replaces the touch prompt.
- No DS edge marking (black outlines on menu models): the PowerVR has no
  equivalent post-process and every substitute adds geometry.
- Records: the totals row has no owner name (the DS takes it from the
  console's user settings).
- Battle Play (DS wireless VS) is two players on one Dreamcast (split screen, controller ports A and
  B) plus a native Vs CPU mode; see BATTLE-PLAY.md (its "Known gaps" list the CPU's limits).

## Known issues

- Cutscene scene loads take 140-177 ms on a black screen (the DS also shows
  black there; the load is not overlapped with the previous fade).
- Night Carnival lamps flicker their light cone every other frame while
  touched, exactly as the DS code does; on a TV this reads as 30 Hz flicker
  rather than the DS LCD's half-transparent look.
- Losing the Boss 7 button-mash clash leaves the boss idle at 1 hit point
  until time over (same as the DS code).
- Test-only discs (debug start files) continue to Sonic's next stage after
  any boss clear; normal play is not affected.

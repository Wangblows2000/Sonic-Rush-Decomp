# Dreamcast port — master task list (final phase)

Integrator-owned. One line per item: state, owner, acceptance. States: done / partial / missing / unverified.
Evidence for "done" items is in the commit messages, REGRESSION.md and the translation docs.

## Feature matrix (2026-11-23, HEAD after 7db54334; Flycast only, no hardware)

| area | state | notes |
|---|---|---|
| Boot, SEGA logo, title intro (TitleDemo), title, menus, options, sound test, records, VMU save | done | Flycast boot -> TitleDemo -> title -> menus -> zone map -> stage; options layout and slides from the original |
| Sonic story (zone map, acts, bosses, talks, scripts, ending, staff roll) | done | full autopilot run on 9b422c76, REGRESSION / DEFECTS regression section |
| Blaze story | done | full run on 16999623 + patches and fe748fda; z21/z22 and special stage on a4e9e57c |
| Extra Zone (script 3, Extra boss, ExtraAct, extra ending) | done | full run; Extra boss pause (F9) |
| Time attack, TryAgain, Records, game over / continue | done | Flycast runs (V16, flow) |
| Attract demos | done | DEMO_LEAD_FRAMES 23 (host replays pass) |
| VS mode | out of scope | needs DS wireless |
| Player movement / physics, tricks, boost effects | done | audited against the DS code; R/L triggers = DS R/L |
| Objects, enemies, gimmicks (all zones) | done | behaviour audit; F16 lamp flicker matches the DS |
| Camera, zone backgrounds, ZoneFx, water | done | Camera_* translated; water tint banks reserved (9b422c76) |
| Bosses 1-7, final, Extra | done | every boss cleared as Sonic and Blaze (REGRESSION 12); death restarts the fight; Boss7 3D effects |
| Special stages (7) | done | controller scheme, ring entry by boost, emerald save |
| Cutscenes, talks, endings, staff rolls | done | all three stories |
| HUD, pause, results, title card, message boxes | done | single-screen; boss starts at GO with no black frame |
| Audio (music, jingles, SFX, fades) | done | load fades, clear-asset prefetch (7db54334) |
| Loading, performance | done | 59.86-60 fps per act; no mid-act loads; no 3-vblank goal frame |
| Single-screen everywhere | done | all reviewed scenes (V16) |
| Kit (no game data) builds the CDI from the owner's ROM | done | clean-unpack test: make_disc.sh -> out/srdc.cdi, boots to z11 in Flycast |

## Open work

None assigned. Remaining items are listed in KNOWN-ISSUES.md (design differences and low-impact items).

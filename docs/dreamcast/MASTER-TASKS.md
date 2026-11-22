# Dreamcast port — master task list (final phase)

Integrator-owned. One line per item: state, owner, acceptance. States: done / partial / missing / unverified.
Evidence for "done" items is in the commit messages, REGRESSION.md and the translation docs.

## Feature matrix (2026-11-22)

| area | state | notes |
|---|---|---|
| Boot, SEGA logo, title, title menu, options, sound test, VMU save | done | Flycast: boot to stage; save writes SONICRUSH |
| Story mode flow (zone map, act 1 -> 2 -> boss, talks, scripts) | unverified | wired (menus + cutscenes + main.c); no full playthrough yet |
| Blaze story (zone order, talks, bosses) | unverified | same code paths, never played |
| Extra Zone (script 3, Extra boss, ExtraAct, extra ending) | unverified | host tests only |
| Time attack, TryAgain, Records | partial | reaches stage in Flycast; TryAgain host-only |
| Attract demos | partial | demos 1/2 play; demo 1 drifts after a rock bounce |
| VS mode | out of scope | needs two DS units (MENUS.md 11) |
| Player movement / physics | partial | grounded, facing, ball forms, loops fixed; per-object audit running |
| Interactive objects (all zones) | partial | 100% created; per-class behaviour audit running |
| Enemies | partial | translated; behaviour audit with objects |
| Camera | partial | simple damped follow in main.c; original Camera_* (66 fns) not translated |
| Zone backgrounds / parallax / zone FX | partial | level BG drawn with fixed parallax; ZoneFx_* and per-zone BG scrolls not translated |
| Water (Water Palace) | partial | tint + surface line; underwater BGM layer not mapped |
| Bosses 1-7, final, Extra | partial | all translated; Flycast: 1,3,4,5 load/fight; boss draw too slow (13 ms) |
| Boss music changes (pinch, talk) | missing | boss_music_hook not mapped to streams |
| Special stages (7) | partial | entry/return verified with test trigger; ring entry by boost not played |
| Cutscenes, endings, staff roll | unverified | host tests + cutscene test disc; not reached in a full playthrough |
| HUD, pause, results, game over, title card | done | single-screen; title-card close-up fixed |
| Boost effects | partial | aura/burst/flames; EffTrail ribbon missing |
| Tricks, scores, ranks | unverified | translated in player/UI; not audited |
| Audio: zone music, SFX, jingles, fades | partial | zone music per zone only; jingles/act-specific/boss music to audit |
| Loading: packs, ring, fades | done | z13 load ~2 s (was 18-21 s) |
| Performance | partial | acts ~59.5 fps; boss/model draw over budget |
| Single-screen everywhere | done for tested scenes | audit continues with each area |

## Open work (priority order) and owners

1. Model renderer speed (boss arenas, player) — renderer agent. Accept: z13/zf3 fights 60 fps sustained, frame log.
2. Object/enemy behaviour audit, demo-1 desync, EffTrail, Blaze flames, zone lights — specialist.
3. Camera (original Camera_* translation) + zone backgrounds/parallax/ZoneFx — camera agent.
4. Full progression playthrough (Sonic, Blaze, Extra, time attack, game over/continue, saves) with fixes — flow agent.
5. Audio audit: boss music, jingles, act/zone music mapping, underwater layer, invincibility/speed-up, results/game over — audio agent.
6. Final: kit + CDI rebuild from the owner's ROM, end-to-end Flycast run, docs.

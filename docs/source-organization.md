# Source organisation

How `src/` and `include/` are laid out, and where a new file goes.

The tree is organised by what the code does in Sonic Rush, not by how it was
decompiled. Every translation unit (TU) still holds one function, named exactly
as its symbol in `config/eur/arm9/**/symbols.txt`. Which module a function
belongs to (main, ITCM, DTCM, `ov000` stage overlay, `ov001` menu overlay) is
recorded in `delinks.txt`, not in the path. One directory can hold functions
from several modules.

## Top level

```
src/
  libs/              code Nintendo, Metrowerks and NitroSystem shipped
  game/              Sonic Rush itself
  asm/               hand-written SDK/MSL/runtime routines kept as CW asm
  asm_fallback/      game functions still written as asm (C does not match yet)
  secure/            secure-area BIOS stubs (GNU as)
include/
  nitro/             basic types, overlay ids
  NitroSDK/  NitroSystem/  MSL/   library struct headers
  game/<System>/     game struct headers, same names as src/game
```

`asm/`, `asm_fallback/` and `secure/` sit outside the subsystem tree on purpose.
`asm/` holds the original hand-written SDK assembly, which stays as it is.
`asm_fallback/` is the queue of game functions that still need matching C;
`tools/matching/swap_fallback.py` looks there. When a fallback is converted, its
C file goes into the game directory of its system (below).

## `src/libs/`

The ranges come from the main-module map in docs/naming-conventions.md §3.1.

| Directory | Contents | main range |
| --- | --- | --- |
| `NitroSDK/init` | crt0: `Entry`, autoload, `init_cp15` | 0x02000800–0x02000b24 |
| `NitroSDK/<lib>` | SDK 2.1 by library: `os`, `gx`, `fx`, `mi`, `snd`, `pxi`, `fs`, `spi` (PM/TP/MIC), `rtc`, `card`, `wm`, `mb`, `wbt`, `ctrdg`, `math` (incl. DGT), `cht` | 0x02000cb4–0x0201b194, plus SDK code in ITCM/DTCM |
| `NitroSDK/demolib` | SDK wireless demo libraries `wh.c`, WFS, MBP | 0x0203af64–0x0204069c |
| `NitroSystem/{snd,fnd,g3d}` | NNS sound, foundation (heaps, archives), G3D | 0x0201b194–0x0202a08c |
| `MSL` | Metrowerks C library (`mem*`, printf, …) | 0x0202a08c–0x0202d5d8 |
| `Runtime` | CW ARM runtime: soft-float, 64-bit helpers, static init | 0x0202d5d8–0x0202fd14 |

A static helper with no library prefix goes with the library of the code next
to it.

## `src/game/`

| Directory | What is in it |
| --- | --- |
| `GameSystem/` | main loop and game state: `Main`, `Game`, `System`, task scheduler, heaps, files and file requests, overlays, compression, maths and random numbers, pad/touch/mic input, story progress, scene changes |
| `Graphics/{Sprite,Model,Palette,Vram,Background,Screen,Font}` | engine graphics: BAC sprite animators and OAM, 3D models and keyframes, palettes, VRAM banks and transfers, background layers, brightness/flash/shake |
| `Sound/` | sound wrappers, system sounds, stage jingles, the tension-gauge sound |
| `SaveData/` | save data and the save-message screens |
| `Wireless/` | wireless manager, lobbies, sessions, Download Play, VS connection, net packets |
| `Stage/` | act flow: stage start/restart/exit, loading, stage events, stage scripts, stage clear, player start |
| `Stage/ObjectSystem/` | the stage-object framework every object is built on: `StageObj`, `MapObject`, object collision/rects/speed/VRAM, map collision |
| `Stage/ZoneBackground/` | the shared background and palette-effect drivers (`ZoneBg_*`, `ZoneFx_*`) that dispatch to per-zone code |
| `Player/` | Sonic and Blaze in stages (`Player_*`, attack hitboxes) |
| `Camera/` | the stage camera |
| `Objects/` | gimmicks and props used in more than one zone: rings, item boxes, springs, dash panels, goal ring, checkpoints, … |
| `Enemies/` | enemy code shared between zones (pawn and flapper groups, enemy base) |
| `Decorations/` | shared scenery objects |
| `Effects/` | shared effects: boost, trick, spin, brake smoke, enemy explosions, ring effects, … |
| `Boss/` | code all bosses use (`Boss`, arena, boss camera, boss effects) |
| `Boss/Boss1/` | the Egg Hammer boss: used by Leaf Storm and, upgraded, by Huge Crisis (stage z53 places the Boss1 object; Huge Crisis adds only `Boss5PartD3`) |
| `UI/{Hud,Navi,MessageBox,TitleCard,GameOver}` | in-stage HUD and popups, navigation messages, message boxes, act title cards, game over |
| `VsMode/` | effects and HUD that exist only in VS races |
| `Menus/<Menu>/` | front end: `SegaLogo`, `TitleScreen` (incl. the attract `TitleDemo`), `TitleMenu`, `Options` (incl. sound test), `Records`, `TimeAttack`, `VsMenu`, `TryAgain`, `ZoneMap` (incl. act select and map pause), `PlayerSelect`, `TimeSelect`, `Results`, `Pause` |
| `Cutscenes/<Kind>/` | `TalkDemo` (story dialogue), `PlayerDemo`, `ClearDemo` (act clear), `PolyDemo`, `TransformDemo`, `DemoScript` (demo sequencing and demo play), `Ending` (endings and staff roll) |
| `SpecialStage/` (+ `Player`, `Camera`, `Hud`) | the 3D special stages |
| `Zone/<Zone_Name>/` | code that belongs to one zone (next section) |

### Zones

Internal zone numbers and titles are established in
docs/research/0014-zones.md. Function names use 1-based numbers (`Zone2Act1_*`
is Water Palace act 1); `gGameState+0x1C` holds the 0-based index.

| N | Directory |
| --- | --- |
| 1 | `Zone/Leaf_Storm` |
| 2 | `Zone/Water_Palace` |
| 3 | `Zone/Mirage_Road` |
| 4 | `Zone/Night_Carnival` |
| 5 | `Zone/Huge_Crisis` |
| 6 | `Zone/Altitude_Limit` |
| 7 | `Zone/Dead_Line` |
| 8 (act 3), "F" | `Zone/Final_Zone` (the final boss stage, zf3) |
| 9 (act 3) | `Zone/Extra_Zone` (Extra Zone boss and its results scene, ov001) |

Inside a zone, only the subdirectories that have content exist:
`Stage` (act setup/release, zone stage scripts and boss-map loaders), `Boss`,
`Objects`, `Enemies`, `Decorations`, `Effects`, `Background` (per-zone
`ZoneBg_ZoneN_*` / `ZoneFx_ZoneN_*`), `UI`, `Results`.

Zone 8/9 acts 1–2 are not separate zones: they reuse Leaf Storm, Mirage Road,
Night Carnival and Huge Crisis assets and code (0014), so they have no
directory.

### Shared or zone-specific?

A directory under `Zone/` needs evidence that only that zone uses the code.
The classification of every stage-overlay prefix, with its evidence, is in
docs/research/0014-ov000-classes.csv. The evidence used:

- its sprite or model exists in only one zone's `zNN_act` archive (objects
  load from the current zone's archive);
- it is placed only in one zone's `zNN_eve` object/decoration lists;
- it checks the zone index itself;
- it is a part of that zone's boss.

Code used by two or more zones stays in the shared directory, even when its
name sounds like one zone (`IronBall`: Leaf Storm and Night Carnival; `Rope`:
Leaf Storm and Altitude Limit; `WaterLevelTrigger`: Water Palace and Mirage
Road; the `DecGroup35/4D/95` decorations). The CSV lists each one's zone set.

Not classified (no evidence either way, left in the shared directory):
`EggBombHawk` (its sprite file is not in the ROM and it is placed nowhere),
`MapObj74`, `MapObjC2` (placed in no zone; possibly unused).

## Headers

`include/` mirrors `src/`: `include/NitroSDK/`, `include/NitroSystem/`,
`include/MSL/`, `include/game/<System>/`. `include/game/Stage/stageobj.h` and
`stageobj_alt.h` are two independently recovered views of the same
stage-object structs; unifying them is struct-recovery work (PLAN.md §3).
`include/game/Camera/bgcam.h` is not included by any TU yet.

## Adding a file

1. Workers write matching C to `src/<agent>/<symbol>.c`
   (docs/agents/AGENT_BRIEF.md).
2. The lead integrates it (`integrate.py` or `swap_fallback.py`), then moves it
   with `git mv` into the directory above that matches its system, and edits
   the TU's path line in its `delinks.txt` the same way.
3. Run `python3 tools/configure.py eur` and `bash tools/utilities/verify_all.sh`.
   Headers an agent wrote to `include/<agent>/` move to `include/game/<System>/`
   (or the library directory) with their `#include` lines. No `src/<agent>/` or
   `include/<agent>/` directory is committed.

`tools/utilities/reorg_systems.py` is the script that performed this migration
(rules and per-file zone patterns); it is kept as the record of how each
directory was placed.

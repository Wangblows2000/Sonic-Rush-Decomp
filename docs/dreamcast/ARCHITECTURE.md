# Dreamcast port — architecture

The port runs the decompiled game logic natively on the Dreamcast through
KallistiOS (KOS 2.3.0, SH-4 GCC, `-O2 -fwrapv`). There is no DS emulator and no
NitroSDK compatibility layer: each game system was translated from the
decompiled C into a native module, keeping the original logic, data, timing
and order of operations, and replacing only the DS hardware services
(2D/3D engines, sound sequencer on the ARM7, file system, touch screen).
The DS decompilation (`src/`, `config/`, `tools/` apart from `tools/dreamcast/`)
is unchanged and still builds the matching ROM.

## Modules (`dreamcast/game/`)

| Directory | Contents |
| --- | --- |
| `src/main.c` | Boot, front-end loop, stage table (`k_stages`), stage load/spawn, per-frame order, hooks between modules, test switches |
| `engine/` | KOS platform: PVR renderer and shared palette rows (`render.c`, `sprpal.h`), music streaming and mixing (`audio.c`), controller input, level data, packed-file loader with prefetch (`pak.c`), loading ring and fades (`loading.c`), frame profiler, optional heap tracker |
| `player/` | Player physics, state machine, collision, model presentation (`dcp_*`) — translation/PLAYER.md |
| `objects/` | Stage objects, enemies, gimmicks, effects, ring manager, object models — translation/OBJECTS.md |
| `camera/` | The original `Camera_*` stage cameras in single-window form — CAMERA.md |
| `zonefx/` | Zone background scrolls and palette effects (water tint, palette cycles) — GRAPHICS.md §8 |
| `boss/` | Bosses 1–7, the final boss and the Extra boss — translation/BOSSES.md |
| `special/` | Special stages — translation/SPECIAL.md |
| `cutscene/` | Story scripts, talks, endings, staff rolls, transform demos — translation/CUTSCENES.md |
| `menus/` | Title intro, title, menus, options, records, time attack, player select, zone map, message boxes — translation/MENUS.md |
| `ui/` | HUD, title card, pause, results, game over — translation/UI.md |
| `demo/` | Attract demos (translation/MENUS.md §10) |
| `save/` | VMU save data |
| `sound/` | Sound-effect sequencer and banks, stage music selection (`bgm.c`) — AUDIO.md |
| `model/` | 3D model renderer for converted NitroSystem models — formats/MODELS.md |

`dreamcast/foundation/` is the first hardware test disc (KOS boot, PVR, AICA,
maple input) and is not part of the game.

## Rules the code follows

1. Game behaviour comes from the decompiled functions. Where the port differs
   (single screen, controller instead of touch, PVR instead of DS 2D/3D), the
   difference is in the presentation or service layer and is listed in the
   module's "deviations" section.
2. One continuous 640×480 4:3 view. The two DS screens are never shown
   stacked or side by side; second-screen information is placed in the frame
   (CAMERA.md §1).
3. No added polygons: models and effects use the original geometry.
4. Every asset is converted offline from the owner's ROM; nothing from the ROM
   is committed (BUILD.md).
5. The PVR lists are opened once per frame in the order opaque, punch-through,
   translucent. Textures are power-of-two from 8 to 1024; sprites use PAL4 /
   PAL8 atlases with one shared 64-row palette allocator (GRAPHICS.md).
6. Disc reads happen behind the loading ring: stage data, object models, the
   act-clear assets and the music headers are loaded or prefetched during the
   stage load, so no frame waits on the GD-ROM during play.

## Asset converters

| Converter | Source in the ROM | Output |
| --- | --- | --- |
| `tools/dreamcast/stages/stage2dc.py` | stage maps, collision, BG layers | per-stage map and collision data, tile textures |
| `tools/dreamcast/sprites/bac2dc.py` | BAC sprite animations | PAL4/PAL8 atlases + frame tables (formats/SPRITES.md) |
| `tools/dreamcast/models/nsbmd2dc.py` | NitroSystem models and their animations | converted models, joint and material animations (formats/MODELS.md) |
| `tools/dreamcast/audio/sdat2dc.py`, `sfxbank.py`, `musictab.py` | SDAT streams, sequences, wave archives | AICA ADPCM music, sound-effect banks, music table (formats/SOUND.md, AUDIO.md) |
| `dreamcast/game/*/tools/*.py` | ROM tables used by each module | per-module data files (objects, UI, bosses, menus, special stages, cutscenes, demos, physics) |
| `tools/dreamcast/mkpak.py` | converted files | `data.pak` and per-stage packs on the disc |

`dreamcast/game/build_disc.sh` collects the converted files into the disc
tree and builds the self-booting CDI with `cdi4dc`.

# Dreamcast port — build

Two ways to get a disc. Both convert every asset from your own Sonic Rush
(Europe) ROM, SHA-1 `52e5024efcf7617a820fa23789b37e5168c3c4fc`. The CDI that
comes out contains the game's data: do not redistribute it.

## 1. The kit (recommended, no toolchain needed)

`sh tools/dreamcast/kit/make_kit.sh` writes `build/dreamcast/kit/srdc_kit.zip`
(about 7 MB): the compiled program (`prebuilt/1ST_READ.BIN`, `IP.BIN`), the
converters, `dsd`, the `cdi4dc` and `wav2adpcm` sources, and the docs. It
contains no game data. On any Linux or WSL (Ubuntu) machine:

```sh
unzip srdc_kit.zip && cd srdc_kit
sh make_disc.sh /path/to/your/rom.nds     # -> out/srdc.cdi, about 3 minutes
```

`make_disc.sh` installs the packages it needs (python3, numpy, pillow,
genisoimage, gcc, cmake), builds `wav2adpcm` and `cdi4dc` (with the flags GCC 14
needs), extracts the ROM with `dsd`, runs every converter and builds the disc.
End-user instructions, including Windows: `tools/dreamcast/kit/README.txt`.

## 2. From this repository

### Toolchain

```sh
git clone --depth 1 https://github.com/KallistiOS/KallistiOS /opt/toolchains/dc/kos
cd /opt/toolchains/dc/kos/utils/kos-chain
cp Makefile.dreamcast.cfg Makefile.cfg      # set makejobs to a number
make                                        # sh-elf binutils/gcc/newlib
cp /opt/toolchains/dc/kos/doc/environ.sh.sample /opt/toolchains/dc/kos/environ.sh
. /opt/toolchains/dc/kos/environ.sh && make -C /opt/toolchains/dc/kos
make -C $KOS_BASE/utils/scramble && make -C $KOS_BASE/utils/makeip
gcc -O2 -o wav2adpcm $KOS_BASE/utils/wav2adpcm/wav2adpcm.c
```

Host tools: Python 3 with numpy and Pillow, `genisoimage`, `cdi4dc`
(github.com/Kazade/img4dc), `wav2adpcm`, and `dsd` from the decompilation
(`bash tools/setup_tools.sh`). Put `cdi4dc` and `wav2adpcm` on `PATH`.

### Extract and convert

```sh
cp /path/to/your/rom.nds extract/baserom_rush_eur.nds
python3 tools/project.py extract eur          # extract/eur/ (gitignored)
```

Then run the converters in the order `tools/dreamcast/kit/make_disc.sh` uses
(step 4/5 there): stages, sprites, models, audio (`sdat2dc.py`, `sfxbank.py`,
`musictab.py`), player physics, then the module data tools for objects, UI,
bosses, menus, special stages and cutscenes. All output goes to
`build/dreamcast/` (gitignored).

### Build the program and the disc

```sh
. /opt/toolchains/dc/kos/environ.sh
make -C dreamcast/game                 # -> dreamcast/game/srdc.elf
sh dreamcast/game/build_disc.sh        # -> build/dreamcast/game/srdc.cdi
```

The game Makefile has no header dependencies: after changing a header,
delete the objects (`find dreamcast/game -name "*.o" -not -path "*/test/*" -delete`)
and rebuild.

`build_disc.sh` variables:

| Variable | Effect |
| --- | --- |
| `SRDC_OUT=<dir>` | output directory (default `build/dreamcast/game`) |
| `SRDC_PREBUILT=<dir>` | take `1ST_READ.BIN` and `IP.BIN` from `<dir>` instead of building them (used by the kit and for testing other worktrees) |

### Host tests

Most modules have host tests that run the translated code on the PC against
the converted data: `make -C dreamcast/game/<module> host-test` for `player`,
`objects`, `camera`, `boss`, `special`, `cutscene`, `menus`, `ui`, `demo`,
`save` and `sound`; `make -C dreamcast/game/menus story-test` runs the
whole-story flow.

## 3. Test discs

These variables write files to `/cd/debug/` on the disc. A disc without them
behaves as the release game.

| Variable | Effect |
| --- | --- |
| `SRDC_START=<n>` | boot straight into stage `n` (index into `k_stages` in `main.c`) |
| `SRDC_STARTPOS="x y"` | that stage starts the player at (x, y) |
| `SRDC_SPAWN="x y [tension]"` | the first stage starts the player at (x, y), again after the title card; position trace every 30 frames |
| `SRDC_SPECIAL=<frame>` | enter the act's special stage at that act frame |
| `SRDC_DEBUGDIR=<dir>` | copy every file in `<dir>` to `/cd/debug/` (`chara.txt` "1" = Blaze, `extra.txt` = Extra Zone start) |
| `SRDC_AUTOCLEAR=<frame>` | clear every act (or boss) at that stage frame |
| `SRDC_AUTODIE="<frame> <lives>"` | kill the player at that frame with that many lives |
| `SRDC_AUTOMAP=1` / `s` / `b` / `x` | the zone map walks to the next story node and presses A; `s`/`b`/`x` also pick Sonic / Blaze / the Extra story on player select |
| `SRDC_MENUSAVE="flags s b [em]"` | start with that save state (progress flags, Sonic / Blaze progress, Sonic's emeralds) |
| `SRDC_MENUTRACE=1` | print every menu scene change and stage exit on the serial console |
| `SRDC_SPQUOTA=1` | special stages keep the rings at the goal quota |
| `SRDC_BOSSHP=<n>` | hold the boss's hit points at `n` or below (late phases, mid-fight talks); print boss events |
| `SRDC_DEMOLEAD=<n>` | override the attract-demo key lead (frames of keys consumed during the stage start) |
| `SRDC_SNDTRACE=1` | serial trace of sound effects and music |
| `SRDC_BGMTEST=<file>` | music event triggers for audio tests |

`/cd/debug/frameprof.txt` (via `SRDC_DEBUGDIR`) turns on the frame profiler:
per-second frame time, vblank counts and the worst frame on the serial
console. `make MEMTRACK=1` builds the heap tracker (`engine/memtrack.c`; name a
scene in `/cd/debug/memtrack.txt`).

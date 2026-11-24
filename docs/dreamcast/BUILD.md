# Dreamcast port — build

Two ways to get a disc. Both convert every asset from your own Sonic Rush
(Europe) ROM, SHA-1 `52e5024efcf7617a820fa23789b37e5168c3c4fc`. The CDI that
comes out contains the game's data: do not redistribute it.

## 1. Windows kit (Windows 10 and later, no installs)

`sh tools/dreamcast/winkit/make_winkit.sh` writes `build/dreamcast/winkit/SonicRushDC-Windows.zip`
(about 38 MB; the current one is committed as `dist/windows/SonicRushDC-Windows.zip`). It contains no
game data:

| Part | What it is |
| --- | --- |
| `SonicRushDC.exe` | Win32 GUI (`tools/dreamcast/winkit/gui/srdcgui.c`, MinGW-w64): choose or drop the ROM (a single `.nds` next to the program is picked automatically), one button, progress bar, friendly errors, "Show details" log, "Show my disc" |
| `app\winbuild.py` | the pipeline (`tools/dreamcast/winkit/winbuild.py`): ROM check (game code `ASCP`, SHA-1 warning), `dsd` extraction, every converter, `assemble_disc.py`, `isowriter.py`, `cdi4dc` → `Your Dreamcast disc\SonicRushDC.cdi`; reruns reuse the extraction and conversion of the same ROM |
| `app\python` | Python 3.13 embeddable build in a normal layout (no `._pth`, so scripts import their neighbours) with numpy and Pillow wheels |
| `app\bin` | `dsd.exe` (Rust, `x86_64-pc-windows-gnu`), `cdi4dc.exe`, `wav2adpcm.exe`, `sseqrender.exe`, `test_coverage.exe` (MinGW-w64, static) |
| `app\prebuilt` | `1ST_READ.BIN`, `IP.BIN` |

The GUI starts the pipeline with `CREATE_NO_WINDOW` in a job object: all helper processes share one hidden
console and are stopped together if the window is closed. Converters run with `PYTHONUTF8=1`;
`SRDC_SSEQRENDER` points `sdat2dc.py` at the prebuilt audio renderer; text files on the disc are written
with LF line ends.

Checks of the Windows build (no Windows machine here): `dsd.exe`, `cdi4dc.exe`, `wav2adpcm.exe`,
`sseqrender.exe` and `test_coverage.exe` give output identical to the Linux tools under Wine; the GUI's
pick / progress / done / error flow runs under Wine; `winbuild.py`, `assemble_disc.py` and `isowriter.py`
run end to end on Linux and the disc boots in Flycast. numpy does not load under the Wine build used, so
the converters themselves have not run in Windows Python yet.

## 2. The Linux / WSL kit (no toolchain needed)

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

## 3. From this repository

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

`build_disc.sh` builds the disc tree with `tools/dreamcast/disc/assemble_disc.py` (shared with the Windows
kit), then `genisoimage` and `cdi4dc`. `tools/dreamcast/disc/isowriter.py` is the Python ISO 9660 + Joliet
writer the Windows kit uses instead of `genisoimage`. Variables:

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

## 4. Test discs

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

Battle Play (two players, split screen) uses `SRDC_DEBUGDIR` files: `vs.txt`
starts a race directly, `vs_p2.txt` scripts player 2's pad (or both) for
one-keyboard emulator runs. See [BATTLE-PLAY.md](BATTLE-PLAY.md#test-builds).
Battle Play is in the same `1ST_READ.BIN`, and its menu and sound files are
converted from the ROM with the rest of the disc.

**Vs CPU racing lines.** `build_disc.sh` (through `assemble_disc.py`) makes the
CPU's racing lines for the 14 Battle Play stages and both characters with
`make -C dreamcast/game/vs -jN routes` (the Windows kit: `vs_route.exe`, one job
per core) and puts them in `/cd/vs`. This is a search with the real game
simulation (player, objects, cameras, race rules and the player models, whose
animation ends drive some player states) on your converted stages. Allow 10 to
25 minutes per stage and character on one core: about 3 to 4 hours for the 28
pairs on two cores.

- **Cache.** Every pair runs with `-check`. Each line file's header holds a
  generation key: a CRC of the stage, the object data and hit boxes, the player
  tables and models, the format version and a hash of the simulation, CPU and
  tool sources (`make -s -C dreamcast/game/vs vsr-src`). A file whose key
  matches is kept at once; any other file is made again. Its line 0 is reused
  when it still replays to the goal frame for frame, so the long search is
  skipped. Linux and Windows builds give the same keys and the same lines.
- **Resume.** A file is written as `<pair>.rte.tmp` and renamed when complete,
  so a stopped run carries on with the pairs not yet made.
- **Reports.** `build/dreamcast/vs/routes/<pair>.rte.txt` (result, key, every
  line's span) and `<pair>.log`; the disc step prints how many lines are
  complete and how many partial (no way to the goal found).
- **Models.** The simulation reads the player models from
  `build/dreamcast/player/model` (`make -C dreamcast/game/vs simmodel`; the
  `routes` target and the Windows kit copy them there).
- **Test discs.** `SRDC_ROUTES=copy` skips the search and puts only the lines
  already made on the disc. Without a stage's line the CPU follows the rival's
  path there.

The lines are derived from your ROM and stay in `build/` (never in git).

`/cd/debug/frameprof.txt` (via `SRDC_DEBUGDIR`) turns on the frame profiler:
per-second frame time, vblank counts and the worst frame on the serial
console. `make MEMTRACK=1` builds the heap tracker (`engine/memtrack.c`; name a
scene in `/cd/debug/memtrack.txt`).

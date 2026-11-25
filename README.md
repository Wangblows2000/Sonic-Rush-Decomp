# Sonic Rush — native Sega Dreamcast port (`dreamcast-port` branch)

<img width="800" height="800" alt="boxart" src="https://github.com/user-attachments/assets/649b1bfb-05d9-4ff0-b498-ed6ef0de8ff0" />

**Do be aware; this is all vibeslop. If you do not agree with the usage of AI, move along.**

This branch adds a native Dreamcast port of *Sonic Rush* (EUR) to the
[matching decompilation](https://github.com/Wangblows2000/Sonic-Rush-Decomp)
on `main`. The game logic is the decompiled C, translated to run directly on
KallistiOS: no DS emulator or compatibility layer. Both DS screens are combined
into one 640×480 4:3 view.

The repository contains **no game code from the ROM, no assets and no ROM
data**. Every asset is converted from your own legally dumped ROM.

## Build a disc

<img width="619" height="704" alt="RUSHDISC" src="https://github.com/user-attachments/assets/ac24a80c-ad36-483a-a77c-8ecf7d891029" />

**Windows 10 / 11:** download [`dist/windows/SonicRushDC-Windows.zip`](dist/windows/SonicRushDC-Windows.zip)
(the "Download raw file" button on GitHub), unzip it, double-click
`SonicRushDC.exe`, choose your ROM and press **Make my Dreamcast disc!** About five minutes later
`SonicRushDC.cdi` is in the folder *Your Dreamcast disc*. Nothing to install (it brings its own
Python and tools); instructions in the zip's `README.txt`. To build the zip:
`sh tools/dreamcast/winkit/make_winkit.sh`.

**Linux / WSL:** the kit: `tools/dreamcast/kit/make_kit.sh` packs the
Dreamcast program and the converters (no game data) into `srdc_kit.zip`; on
any Linux or WSL machine, `sh make_disc.sh <your rom.nds>` inside the
unpacked kit writes `out/srdc.cdi` (about 3 minutes). Step-by-step
instructions, including Windows/WSL: [tools/dreamcast/kit/README.txt](tools/dreamcast/kit/README.txt).

Building from this repository (toolchain, converters, test discs):
[docs/dreamcast/BUILD.md](docs/dreamcast/BUILD.md).

Supported ROM: Sonic Rush (Europe) `ASCP` rev 0, SHA-1
`52e5024efcf7617a820fa23789b37e5168c3c4fc`.

## State of the port

The whole game runs in the Flycast emulator: title intro and menus, zone map,
Sonic, Blaze and Extra stories with every act, boss, special stage, cutscene,
ending and staff roll, time attack and VMU saves, at 59.9–60 fps. Differences from the DS and known
issues: [docs/dreamcast/KNOWN-ISSUES.md](docs/dreamcast/KNOWN-ISSUES.md).
Port documentation index: [docs/dreamcast/README.md](docs/dreamcast/README.md).

Controls: analog/D-pad move, A jump, B trick, X boost, Y alternate, right/left
trigger = DS R/L, Start pause. Special stages show the Dreamcast button for
each Trick Spring step.

## The decompilation underneath

The DS decompilation on this branch is the same as on `main`: the EUR ROM
rebuilds byte for byte from C, 100 % of ARM9 code matched
([STATUS.md](STATUS.md), open work in [PLAN.md](PLAN.md), setup in
[INSTALL.md](INSTALL.md), full guide in [AGENTS.md](AGENTS.md)).

<!-- dashboard:start -->

_Verified progress, updated 2026-11-23 08:13:22Z ([full dashboard](docs/progress/dashboard.md))._

| Module | Matched code | Bytes | Functions | Remaining |
| ------ | ------------ | ----- | --------- | --------- |
| main | `████████████████████` 100.00% | 449,566 / 449,566 | 2,872 / 2,872 | 0 |
| itcm | `████████████████████` 100.00% | 2,064 / 2,064 | 14 / 14 | 0 |
| ov000 | `████████████████████` 100.00% | 642,724 / 642,724 | 2,799 / 2,799 | 0 |
| ov001 | `████████████████████` 100.00% | 328,118 / 328,118 | 1,723 / 1,723 | 0 |
| **ARM9 total** | `████████████████████` 100.00% | 1,422,472 / 1,422,472 | 7,408 / 7,408 | 0 |

- ROM SHA-1 (`build/eur/rush_eur.sha1.ok`): **OK** — last OK 2026-11-23 08:10:52Z
- GNU relink (`build/eur/reports/gnu_relink.json`): **OK (11/11 sections)** — 2026-11-23 08:11:29Z
- objdiff report: `build/eur/report.json` — 2026-11-23 08:11:31Z

<!-- dashboard:end -->

## Layout

```
dreamcast/game/    the port: engine (KOS platform, render, audio, input, loading),
                   player, objects, camera, zonefx, boss, special, cutscene, menus,
                   ui, demo, save, sound, model; build_disc.sh, Makefile
dreamcast/foundation/  first hardware test disc (KOS boot, PVR, AICA, maple)
tools/dreamcast/   asset converters (stages, sprites, models, audio), mkpak.py, kit/
docs/dreamcast/    port documentation
config/ src/ include/ tools/ docs/   the DS decompilation (see main)
extract/           your ROM + extracted files (gitignored)
build/             all build output, converted assets and discs (gitignored)
```

## Acknowledgements

Decompilation architecture informed by [zeldaret/ph](https://github.com/zeldaret/ph),
[pret/pokediamond](https://github.com/pret/pokediamond),
[tangosdev/sm64ds-decomp](https://github.com/tangosdev/sm64ds-decomp),
[XorTroll/mkds-re](https://github.com/XorTroll/mkds-re),
[mariopartyrd/mariopartyDS](https://github.com/mariopartyrd/mariopartyDS) and
[RushRE/SonicRushAdventure-Decomp](https://github.com/RushRE/SonicRushAdventure-Decomp).
Dreamcast side: [KallistiOS](https://github.com/KallistiOS/KallistiOS),
[img4dc/cdi4dc](https://github.com/Kazade/img4dc), [Flycast](https://github.com/flyinghead/flycast) for testing.
Hardware reference: [GBATEK](https://problemkaputt.de/gbatek.htm).

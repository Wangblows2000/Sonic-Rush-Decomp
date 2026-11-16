# Sonic Rush (Nintendo DS) decompilation

Work-in-progress **matching decompilation** of *Sonic Rush* (Sonic Team / Dimps,
2005) for the Nintendo DS. The goal is C/C++ source (plus assembly where
unavoidable) that compiles back to the original game code byte for byte.

The repository contains **no game code, assets or ROM data**. You need your own
legally dumped ROM; the tooling extracts everything it needs from it.

## Target

| Version | Game code | ROM ver. | SHA-1 |
| ------- | --------- | -------- | ----- |
| `eur` — Sonic Rush (Europe) (En,Ja,Fr,De,Es,It) | `ASCP` | 0 | `52e5024efcf7617a820fa23789b37e5168c3c4fc` |

USA (`ASCE`) and Japan (`ASCJ`) are not configured yet.

## Current state (see [STATUS.md](STATUS.md))

- ROM extract → rebuild is **byte-identical** to the base ROM.
- Code is split by [dsd](https://github.com/AetiasHax/ds-decomp) into relocatable
  ELF objects for ARM9 main, ITCM, DTCM and both overlays: **7,391 functions**,
  ~1.42 MB of ARM9 function code. An independent GNU-ld relink of those objects
  reproduces **every section byte for byte**.
- Matching decompilation is under way with CodeWarrior for NDS 1.2; the block
  below is regenerated from the objdiff report by `python tools/project.py dashboard`.

<!-- dashboard:start -->

_Verified progress, updated 2026-11-16 09:51:06Z ([full dashboard](docs/progress/dashboard.md))._

| Module | Matched code | Bytes | Functions | Remaining |
| ------ | ------------ | ----- | --------- | --------- |
| main | `████████████████████` 99.65% | 448,010 / 449,566 | 2,850 / 2,872 | 22 |
| itcm | `████████████████████` 100.00% | 2,064 / 2,064 | 14 / 14 | 0 |
| ov000 | `████████████████████` 98.10% | 630,520 / 642,724 | 2,770 / 2,799 | 29 |
| ov001 | `████████████████████` 100.00% | 328,118 / 328,118 | 1,723 / 1,723 | 0 |
| **ARM9 total** | `████████████████████` 99.03% | 1,408,712 / 1,422,472 | 7,357 / 7,408 | 51 |

Next milestone: **100%** matched code — 13,760 more bytes (currently 99.03%).

- ROM SHA-1 (`build/eur/rush_eur.sha1.ok`): **OK** — last OK 2026-11-16 09:49:33Z
- GNU relink (`build/eur/reports/gnu_relink.json`): **OK (11/11 sections)** — 2026-11-16 09:51:05Z
- objdiff report: `build/eur/report.json` — 2026-11-16 09:51:06Z

<!-- dashboard:end -->

## Layout

```
config/            versions.yaml, per-version dsd configs, compiler map, symbol evidence
  eur/arm9/        dsd symbols.txt / relocs.txt / delinks.txt per module
src/, libs/        decompiled game code and SDK/middleware (empty for now)
include/           shared headers
tools/             project CLI, configure.py, analysis/matching/extraction tools, Ghidra scripts
docs/research/     research log and technical notes (the knowledge base)
docs/architecture/ memory map and project architecture
docs/progress/     dated progress reports
extract/           your ROM + extracted files (gitignored)
build/             all build output and reports (gitignored)
```

## Quick start

```sh
python -m pip install -r tools/requirements.txt
bash tools/setup_tools.sh                 # builds dsd, objdiff-cli, wibo from source
cp /path/to/your/rom.nds extract/baserom_rush_eur.nds
python tools/project.py doctor
python tools/project.py extract eur
python tools/configure.py eur && ninja verify
python tools/project.py progress
```

Full instructions: [INSTALL.md](INSTALL.md). How to contribute:
[CONTRIBUTING.md](CONTRIBUTING.md). Roadmap: [PLAN.md](PLAN.md).

## Acknowledgements

Architecture informed by [zeldaret/ph](https://github.com/zeldaret/ph),
[pret/pokediamond](https://github.com/pret/pokediamond),
[XorTroll/mkds-re](https://github.com/XorTroll/mkds-re),
[mariopartyrd/mariopartyDS](https://github.com/mariopartyrd/mariopartyDS) and
[RushRE/SonicRushAdventure-Decomp](https://github.com/RushRE/SonicRushAdventure-Decomp).
Hardware reference: [GBATEK](https://problemkaputt.de/gbatek.htm).

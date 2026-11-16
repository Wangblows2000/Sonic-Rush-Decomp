# Sonic Rush (Nintendo DS) decompilation

**Do be aware; this is all vibeslop. If you do not agree with the usage of AI, move along.**

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
- [dsd](https://github.com/AetiasHax/ds-decomp) splits ARM9 main, ITCM, DTCM and
  both overlays into relocatable ELF objects: **7,408 functions**, ~1.42 MB of
  function code. An independent GNU-ld relink of those objects reproduces every
  section byte for byte.
- The full ROM builds from source with CodeWarrior for NDS 1.2/sp2 (linker
  1.2/sp3) and has the original SHA-1. **100 % of ARM9 code is matched** (all
  7,408 functions) and every function has an evidence-backed name; the block
  below is regenerated from the objdiff report by
  `python3 tools/project.py dashboard`.

<!-- dashboard:start -->

_Verified progress, updated 2026-11-16 15:31:28Z ([full dashboard](docs/progress/dashboard.md))._

| Module | Matched code | Bytes | Functions | Remaining |
| ------ | ------------ | ----- | --------- | --------- |
| main | `████████████████████` 100.00% | 449,566 / 449,566 | 2,872 / 2,872 | 0 |
| itcm | `████████████████████` 100.00% | 2,064 / 2,064 | 14 / 14 | 0 |
| ov000 | `████████████████████` 100.00% | 642,724 / 642,724 | 2,799 / 2,799 | 0 |
| ov001 | `████████████████████` 100.00% | 328,118 / 328,118 | 1,723 / 1,723 | 0 |
| **ARM9 total** | `████████████████████` 100.00% | 1,422,472 / 1,422,472 | 7,408 / 7,408 | 0 |

- ROM SHA-1 (`build/eur/rush_eur.sha1.ok`): **OK** — last OK 2026-11-16 15:28:00Z
- GNU relink (`build/eur/reports/gnu_relink.json`): **OK (11/11 sections)** — 2026-11-16 15:28:40Z
- objdiff report: `build/eur/report.json` — 2026-11-16 15:28:41Z

<!-- dashboard:end -->

## Layout

```
config/            versions.yaml, per-version dsd configs, compiler map, symbol evidence
  eur/arm9/        dsd symbols.txt / relocs.txt / delinks.txt per module
src/               matched translation units, one function per file (grouped by
                   the agent that wrote them for now; asm/ and asm_fallback/
                   hold functions written in asm instead of C)
include/           nitro/ (types, overlay ids) plus per-agent struct headers
nearmiss/          near-miss C drafts (db.jsonl)
state/             task board (tasks.json)
tools/             project CLI, configure.py, matching/analysis/extraction tools, Ghidra scripts
docs/research/     research log and technical notes (the knowledge base)
docs/architecture/ memory map and project architecture
docs/progress/     dashboard and dated progress reports
extract/           your ROM + extracted files (gitignored)
build/             all build output and reports (gitignored)
```

## Quick start

Easiest (Linux): copy your own Sonic Rush (Europe) ROM into the repository
directory and run

```sh
bash build.sh            # checks the ROM, sets up tools on first run, builds,
                         # verifies SHA-1; writes sonic_rush_eur_rebuilt.nds
bash build.sh --verify   # also runs the GNU relink check and objdiff report
```

Manual steps:

```sh
python -m pip install -r tools/requirements.txt
bash tools/setup_tools.sh                 # builds dsd, objdiff-cli, wibo from source
cp /path/to/your/rom.nds extract/baserom_rush_eur.nds
python tools/project.py doctor
python tools/project.py extract eur
python tools/configure.py eur && ninja verify
ninja && ninja report                     # matching build; needs the Metrowerks tools
python tools/project.py progress
```

On a fresh Linux machine, `bash tools/bootstrap_session.sh /path/to/rom.nds`
does all of the above in one step.

Full instructions: [INSTALL.md](INSTALL.md). How to contribute:
[CONTRIBUTING.md](CONTRIBUTING.md). Roadmap: [PLAN.md](PLAN.md).

## Acknowledgements

Architecture informed by [zeldaret/ph](https://github.com/zeldaret/ph),
[pret/pokediamond](https://github.com/pret/pokediamond),
[XorTroll/mkds-re](https://github.com/XorTroll/mkds-re),
[mariopartyrd/mariopartyDS](https://github.com/mariopartyrd/mariopartyDS) and
[RushRE/SonicRushAdventure-Decomp](https://github.com/RushRE/SonicRushAdventure-Decomp).
Hardware reference: [GBATEK](https://problemkaputt.de/gbatek.htm).

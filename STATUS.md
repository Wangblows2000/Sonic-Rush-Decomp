# Status — 2026-11-21

Measured state of the EUR (`ASCP`) build. Figures come from
`python3 tools/project.py progress`, which reads only `build/eur/report.json`
(objdiff) and `symbols.txt`. Run `bash tools/utilities/verify_all.sh` to
reproduce every line below.

## Measured output

| Check | Result | Command |
| ----- | ------ | ------- |
| Base ROM | EUR `ASCP` rev 0, SHA-1 `52e5024e…` | `project.py doctor` |
| Matching ROM build (mwccarm + mwldarm) | **SHA-1 identical** | `ninja` / `bash build.sh` |
| Delinked objects → GNU ld relink | all sections byte-identical | `ninja verify` |
| Functions matched (objdiff 100 %) | **7,408 / 7,408** | `ninja report && project.py progress` |
| Code bytes matched | **1,422,472 / 1,422,472 (100.00 %)** | `project.py progress` |
| Symbol/evidence consistency | 0 errors, 0 warnings | `tools/analysis/check_symbols.py` |
| Functions named | **7,408 / 7,408**; 284 are subsystem-level `<Subsystem>_Unk_<addr>` | `check_symbols.py`, `symbols.txt` |

## How the source is written

- 7,373 translation units in `config/eur/arm9/**/delinks.txt`, almost all one
  function per file, CodeWarrior for NDS 1.2/sp2 (linker 1.2/sp3) under wibo.
- C is the default. CodeWarrior `asm` is used for hand-written runtime routines
  (soft-float/64-bit helpers with secondary entry points, CP15/SWI/copy loops)
  and for the remaining `src/asm_fallback/` functions whose C does not yet
  match (each says why on line 1); `src/asm/__pformatter.c` (MSL printf: the
  switch-table clustering no CW 1.2 build reproduces).
- `src/secure/secure_area.s` (GNU as) covers the secure area 0x02000000–
  0x02000800: the 18 BIOS SWI stubs as Thumb functions at their exact
  (halfword) offsets, the filler pulled by `.incbin` from the user's own
  extracted binary at build time (docs/research/0012-secure-area.md).
- NitroSDK linker constants (`SDK_SYS_STACKSIZE`, `SDK_IRQ_STACKSIZE`,
  `SDK_SECTION_ARENA_DTCM_START`) are `link_time_const(NAME=VALUE)` entries in
  `relocs.txt`, defined in the LCF by `tools/utilities/lcf_consts.py`.

## Toolchain

| Component | Version / note |
| --------- | -------------- |
| Compiler | CW for NDS 1.2/sp2 (`config/eur/compilers.yaml`) |
| Linker | CW 1.2/sp3 |
| dsd | ds-decomp v0.12.1 + `tools/patches/*.patch` (Thumb unsigned jump tables; asm entry points / conditional `arm_branch` / `$d` for data in `.text` / named link-time constants) |
| objdiff | 3.8.3 |
| wibo | 1.2.0 |
| GNU binutils | arm-none-eabi 2.42 (relink check, `secure_area.s`) |
| Ghidra | 12.2 + JDK 25 (decompiler drafts, optional) |

## Dreamcast port (branch `dreamcast-port`, 2026-11-22)

Milestone 1 (porting report) and Milestone 2 (hardware foundation disc) done;
the game itself does not run on Dreamcast yet. Details: docs/dreamcast/.

## Recent changes (2026-11-21)

- asm to C: 15 more `asm_fallback` functions converted (20 left plus
  `__pformatter`, which stays asm).
- Naming: 184 `_Unk_` names upgraded (284 left).
- Source layout: `src/libs/{NitroSDK,NitroSystem,MSL,Runtime}`; game code by
  gameplay system and zone (`src/game/Zone/<Zone_Name>/`), headers likewise
  (docs/source-organization.md, zone evidence docs/research/0014-zones.md).
- First data TU: `OS_IRQTable` in DTCM (docs/research/0013).

## Earlier changes (2026-11-16)

- Secure-area stubs matched → 100.00 %.
- 54 asm functions converted to C (incl. FS_StartOverlay, GX_SetBankForSubBG,
  OS_GetInitArenaHi, OS_InitThread, the MapObj73 family, NNS G3D/sound code).
- Naming: every function named (rounds 1–6, ~9,300 renames incl. upgrades),
  audit corrections (FS_UnloadOverlayImage, FS_LoadOverlayImage,
  NNS_SndPlayerCountPlayingSeqBySeqArcIdx, _fgr/_fgeq), repeated library
  statics named `<Official>_<file stem>`.
- Tools: `check_symbols.py`, `verify_all.sh`, `swap_fallback.py`,
  `lcf_consts.py`, `build.sh`; `rename.py` safeguards (case-only collisions,
  repeated statics, evidence replacement, post-checks).

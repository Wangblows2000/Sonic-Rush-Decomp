# Status — 2026-11-23

Measured state of the EUR (`ASCP`) build at `main` 2c5e96e0. Every line below
was produced by `bash tools/utilities/verify_all.sh` on 2026-11-23 (exit 0,
`verify_all: OK`). Progress figures come from `python3 tools/project.py
progress`, which reads only `build/eur/report.json` (objdiff) and `symbols.txt`.

## Measured output

| Check | Result | Command |
| ----- | ------ | ------- |
| Base ROM | EUR `ASCP` rev 0, SHA-1 `52e5024e…` | `project.py doctor` |
| Matching ROM build (mwccarm + mwldarm) | **SHA-1 identical** | `ninja` / `bash build.sh` |
| Delinked objects → GNU ld relink | all sections byte-identical | `ninja verify` |
| Functions matched (objdiff 100 %) | **7,408 / 7,408** | `ninja report && project.py progress` |
| Code bytes matched | **1,422,472 / 1,422,472 (100.00 %)** | `project.py progress` |
| Symbol/evidence consistency | 0 errors, 0 warnings (12,401 symbols, 7,373 TUs, 7,698 evidence entries) | `tools/analysis/check_symbols.py` |
| Functions named | **7,408 / 7,408**; 284 are subsystem-level `<Subsystem>_Unk_<addr>` | `check_symbols.py`, `symbols.txt` |
| Functions still CodeWarrior `asm` | 20 in `src/asm_fallback/` + `src/asm/__pformatter.c` | `ls src/asm_fallback` |

Open work: [PLAN.md](PLAN.md).

## How the source is written

- 7,373 translation units in `config/eur/arm9/**/delinks.txt`, almost all one
  function per file, organised by library, gameplay system and zone
  (docs/source-organization.md). CodeWarrior for NDS 1.2/sp2 (linker 1.2/sp3)
  runs under wibo.
- C is the default. CodeWarrior `asm` is used for hand-written runtime routines
  (soft-float/64-bit helpers with secondary entry points, CP15/SWI/copy loops),
  for the `src/asm_fallback/` functions whose C does not yet match (each says
  why on line 1), and for `src/asm/__pformatter.c` (MSL printf: the
  switch-table clustering no CW 1.2 build reproduces).
- `src/secure/secure_area.s` (GNU as) covers the secure area 0x02000000–
  0x02000800: the 18 BIOS SWI stubs as Thumb functions at their exact
  (halfword) offsets, the filler pulled by `.incbin` from the user's own
  extracted binary at build time (docs/research/0012-secure-area.md).
- NitroSDK linker constants (`SDK_SYS_STACKSIZE`, `SDK_IRQ_STACKSIZE`,
  `SDK_SECTION_ARENA_DTCM_START`) are `link_time_const(NAME=VALUE)` entries in
  `relocs.txt`, defined in the LCF by `tools/utilities/lcf_consts.py`.
- Data: every TU declares only `extern` data except `OS_IRQTable` (DTCM), the
  first data TU (docs/research/0013-data-ownership.md); all other data comes
  from dsd's delinked gap objects.

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

## Other branches

- `dreamcast-port`: native Sega Dreamcast port built on this decompilation
  (docs/dreamcast/ on that branch).

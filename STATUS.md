# Status — 2026-11-16

Measured state of the EUR (`ASCP`) build. Figures come from
`python3 tools/project.py progress`, which reads only `build/eur/report.json`
(objdiff) and `symbols.txt`. The README dashboard block is regenerated from the
same report by `python3 tools/project.py dashboard`; when the two disagree, the
newer report wins.

## Measured output

| Check | Result | Command |
| ----- | ------ | ------- |
| Base ROM | EUR `ASCP` rev 0, SHA-1 `52e5024e…` verified | `project.py doctor` |
| ROM extract → rebuild | SHA-1 identical | `project.py verify eur` |
| Matching ROM build (mwccarm + mwldarm) | SHA-1 identical (`build/eur/rush_eur.sha1.ok`) | `ninja` |
| Delinked objects → GNU ld relink | 11/11 sections byte-identical | `ninja verify` |
| Functions | 7,408 (main 2,872 · itcm 14 · ov000 2,799 · ov001 1,723) | `project.py functions` |
| Matched functions (objdiff 100 %) | **7,389 / 7,408** | `ninja report && project.py progress` |
| Matched code bytes | **1,422,056 / 1,422,472 (99.97 %)** | `project.py progress` |
| Named functions (not progress) | 180; 164 evidence entries | `project.py symbols` |

Per module: itcm, ov000 and ov001 are at 100 %. All 416 remaining bytes are in
main:

- 18 BIOS SWI stubs in the secure area, 76 bytes in total (see PLAN.md for why
  they are not TUs).
- `func_02010018` (340 B), reported at 99.94 % by the last objdiff report. Its
  TU has since been replaced by `src/asm_fallback/func_02010018.c`. The C
  version matches the bytes, but CW folds the `OS_IrqHandler` (ITCM)
  relocation into a raw constant. The new TU counts only after the next
  checkpoint.

## How the source is written

- 7,371 translation units are registered in `config/eur/arm9/**/delinks.txt`,
  almost all one function per file, compiled with CodeWarrior for NDS 1.2/sp2.
- C is the default. Assembly (CodeWarrior `asm` functions) is used only for:
  - hand-written runtime routines (CP15/PSR access, SWI, `stm` copy loops,
    soft-float and 64-bit helpers), which sit in the agents' directories next
    to C;
  - `src/asm/` (6 files, 3,524 B): cases that C cannot express, such as
    linker constants in a pool with no relocations, two symbols at one address,
    or a switch-table layout no CW 1.2 build emits;
  - `src/asm_fallback/` (90 files, 44,082 B): functions whose C attempts did
    not match. Line 1 of each file says why.
- Files are still grouped by the agent that wrote them (`src/<agent>/`), plus
  `src/auto/` (1,008 TUs from the automatic template, clone and Ghidra tiers),
  `src/clones/`, `src/sweep/`, `src/pragma/` and `src/lead/`.

## Toolchain

| Tool | Version | Notes |
| ---- | ------- | ----- |
| mwccarm (compiler) | CW for NDS 1.2/sp2 | community bundle run via wibo; never committed (docs/research/0004) |
| mwldarm (linker) | CW for NDS 1.2/sp3 | reproduces the ROM SHA-1 |
| dsd (ds-decomp) | v0.12.1 + patches in `tools/patches/` | built from source (docs/research/0008) |
| objdiff-cli | v3.8.3 | built from source |
| wibo | 1.2.0 | built from source |
| arm-none-eabi binutils | 2.42 | GNU relink check |
| Ghidra | 12.2 + OpenJDK 25 | headless drafts in `build/eur/ghidra/` (docs/research/0007) |

## Process

A single writer integrates (`tools/utilities/checkpoint.sh`). Parallel
matching agents write `src/<agent>/` and report to `build/agents/<agent>.txt`.
Partitions and notes are in `state/tasks.json` (`python3 tools/tasks.py brief`).
The rules for counting a function are in CLAUDE.md §5.

Remaining work: [PLAN.md](PLAN.md).

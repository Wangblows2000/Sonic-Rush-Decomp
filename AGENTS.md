# AGENTS.md — Sonic Rush (EUR `ASCP`) matching decompilation

This is the entry point for any person or AI agent working in this repository.
Read it before you change anything. It describes the project's goal, the build
graph, the tools, the conventions and the safety rules, and it reflects the
code in `tools/` and `config/` as of 2026-11-16. Where another document
disagrees with this file or with the code, the code is authoritative. See
[Known stale documents](#17-known-stale-documents).

`CLAUDE.md` is the short resume protocol for Claude sessions. This file is the
long form. The two do not conflict.

---

## Contents

1. [Project goal and the definition of "matched"](#1-project-goal-and-the-definition-of-matched)
2. [Current state](#2-current-state)
3. [ROM and memory layout](#3-rom-and-memory-layout)
4. [Repository guide, directory by directory](#4-repository-guide-directory-by-directory)
5. [Dependencies](#5-dependencies)
6. [Fresh-machine setup](#6-fresh-machine-setup)
7. [Build pipeline, step by step](#7-build-pipeline-step-by-step)
8. [The dsd configuration files](#8-the-dsd-configuration-files)
9. [Source-file conventions (`src/`)](#9-source-file-conventions-src)
10. [How to match a function](#10-how-to-match-a-function)
11. [Codegen levers catalogue](#11-codegen-levers-catalogue)
12. [Integration (single writer)](#12-integration-single-writer)
13. [Renaming symbols and the evidence standard](#13-renaming-symbols-and-the-evidence-standard)
14. [Task board and multi-agent work](#14-task-board-and-multi-agent-work)
15. [Important systems in the code](#15-important-systems-in-the-code)
16. [Troubleshooting](#16-troubleshooting)
17. [Known stale documents](#17-known-stale-documents)
18. [Safety rules (summary)](#18-safety-rules-summary)
19. [Command reference](#19-command-reference)

---

## 1. Project goal and the definition of "matched"

The goal is C source (with CodeWarrior inline `asm` functions where C cannot
produce the bytes) that rebuilds the Sonic Rush EUR ROM byte for byte.

| Field | Value |
| ----- | ----- |
| Title | Sonic Rush (Europe) (En,Ja,Fr,De,Es,It) |
| Game code | `ASCP`, ROM version 0 |
| ROM SHA-1 | `52e5024efcf7617a820fa23789b37e5168c3c4fc` |
| ROM SHA-256 | `2606a10ead1656e201d8127ffae5614a970201fc37627688e5bccb48abf705c2` |
| Size | 67,108,864 bytes |

Only EUR is configured (`config/versions.yaml`). USA (`ASCE`) and Japan
(`ASCJ`) are not.

**Progress means verified matched code bytes. Nothing else counts.** A function
is matched only when all of the following hold:

1. `python3 tools/matching/match.py src/<dir>/<symbol>.c` exits 0 and prints a
   line that starts with `OK` for every function the file defines. The check
   masks relocated fields on both sides. Every other byte and the length must
   be equal.
2. The file is registered as its own translation unit (TU) in the owning
   module's `delinks.txt`.
3. The full ROM builds and its SHA-1 is
   `52e5024efcf7617a820fa23789b37e5168c3c4fc`. The marker
   `build/eur/rush_eur.sha1.ok` must be newer than `build/eur/rush_eur.nds`.
4. `ninja verify` passes. This is the independent GNU-ld relink check, and it
   writes `build/eur/reports/gnu_relink.json`.
5. objdiff reports the function at 100 % in `build/eur/report.json`.

`project.py progress`, `project.py dashboard` and `tasks.py brief` read only
`build/eur/report.json` and `symbols.txt`. They skip dsd gap units
(`auto_generated`) and count a function only at `fuzzy_match_percent >= 100`.
Agent reports, renamed symbols and near-misses never count.

---

## 2. Current state

Verified on 2026-11-16 (`python3 tools/project.py progress`; run
`python3 tools/tasks.py brief` for live numbers).

| Module | Matched bytes | Functions |
| ------ | ------------- | --------- |
| main | 449,490 / 449,566 (99.98 %) | 2,854 / 2,872 |
| itcm | 2,064 / 2,064 (100 %) | 14 / 14 |
| ov000 | 642,724 / 642,724 (100 %) | 2,799 / 2,799 |
| ov001 | 328,118 / 328,118 (100 %) | 1,723 / 1,723 |
| **ARM9 total** | **1,422,396 / 1,422,472 (99.99 %)** | **7,390 / 7,408** |

The 76 remaining bytes are 18 BIOS SWI wrapper stubs (4 to 6 bytes each:
`WaitByLoop`, `IsDebugger`, `CpuFastSet`, ..., `GetCRC16`) in the secure area
0x02000000–0x02000800, which the build keeps as original bytes (see §3 and
PLAN.md: 9 of them are halfword-aligned, which per-function TUs under the
LCF's `ALIGNALL(4)` cannot reproduce).

Of the matched functions, those in `src/asm/` and `src/asm_fallback/` are
CodeWarrior `asm` functions rather than C (see §9); converting the
`asm_fallback` set back to C is open work.

DTCM (0x027E0000) contains data only. It has no functions and no TUs.

ARM7 (0x02380000, 0x26F24 bytes) and the Download Play children
(`/mb/contest.srl`, `/mb/preview.srl`) are outside the dsd configuration. The
build rebuilds them verbatim from the extracted files.

---

## 3. ROM and memory layout

Sources: `config/eur/arm9/**/delinks.txt` (module section headers),
`docs/architecture/memory-map.md`, and `docs/research/0001-rom-identification.md`.

| Range | Module / section | Notes |
| ----- | ---------------- | ----- |
| 0x01FF8000–0x01FFF82C | ITCM `.text` | 14 functions, then a 0x701C-byte zero buffer from 0x01FF8744. `.bss` is empty at 0x01FFF840. |
| 0x02000000–0x02000800 | main, secure area | BIOS SWI wrapper stubs and data. **Kept as original bytes.** Do not create TUs here. |
| 0x02000800 | `Entry` | crt0. The ROM header gives the ARM9 entry point. Matched as `src/main_sdk2/Entry.c`. |
| 0x02000B00–0x02000B20 | `BuildInfo` | NitroSDK module parameters (SDK version 0x02017532), matched as an `asm` data block in `src/lead/`. |
| 0x02000000–0x0206E470 | main `.text` | ARM and Thumb code. |
| 0x0206E470–0x02077958 | main `.rodata` | |
| 0x02077958 | main `.ctor` | Empty. |
| 0x02077960–0x0207CFE0 | main `.data` | |
| 0x0207CFE0–0x022C5C80 | main `.bss` | 2.3 MB. |
| 0x022C5C80–0x02362B58 | ov000 `.text` | Mostly ARM. `.rodata` runs to 0x02364EF8, `.data` to 0x02369AC0, then 0x11E0 bytes of BSS. |
| 0x022C5C80–0x02315EB4 | ov001 `.text` | Thumb up to about 0x022ED000, then ARM. `.rodata` runs to 0x02317E8C, `.data` 0x02317EA0–0x0231F040, `.bss` to 0x0231F700. |
| 0x027E0000–0x027E2060 | DTCM `.data` | Data only. |
| 0x02380000–0x023A6F24 | ARM7 static | Not handled by dsd. |

**Overlays ov000 and ov001 share the base 0x022C5C80.** They are mutually
exclusive at run time. Addresses alone are ambiguous, so always name the
module. Overlay symbols carry the prefix (`func_ov000_…`, `func_ov001_…`,
`data_ov000_…`). Main code calls into both overlays directly.

**The secure area is not decompiled.** The ROM is a decrypted dump. The
secure-area CRC in the header (offset 0x6C) is computed over the
KEY1-encrypted area, so it cannot be derived without the console's ARM7 BIOS.
`config/versions.yaml` records it as `header_fixups.secure_area_crc: 0x190B`,
and `tools/extraction/fix_header.py` writes it back after `dsd rom build`. The
script then recomputes the header CRC at 0x15E. `match.py --promote` refuses
any main TU that starts below 0x02000800.

---

## 4. Repository guide, directory by directory

| Path | Committed? | Contents |
| ---- | ---------- | -------- |
| `AGENTS.md`, `CLAUDE.md` | yes | This guide; the short resume protocol. |
| `README.md` | yes | Overview. The block between `<!-- dashboard:start -->` and `<!-- dashboard:end -->` is rewritten by `project.py dashboard`. |
| `STATUS.md`, `PLAN.md`, `INSTALL.md`, `CONTRIBUTING.md` | yes | Older documents, partly stale (§17). |
| `.claude/settings.json` | yes | SessionStart hook: `cd "$CLAUDE_PROJECT_DIR" && timeout 20 python3 tools/tasks.py brief 2>&1 \| head -60`. |
| `config/versions.yaml` | yes | ROM identity, SHA-1/SHA-256, size, header fix-ups, list of zero-length files. |
| `config/eur/compilers.yaml` | yes | Compiler version and flags (default, per-glob rules, linker). |
| `config/eur/symbol_evidence.yaml` | yes | Evidence for every non-default symbol name. |
| `config/eur/arm9/config.yaml` | yes | dsd project config: paths of the ROM config, build paths, and per-module `delinks`/`symbols`/`relocations` files with dsd module hashes. Do not edit the hashes. |
| `config/eur/arm9/{symbols,relocs,delinks}.txt` | yes | Main module. |
| `config/eur/arm9/itcm/`, `dtcm/` | yes | ITCM and DTCM autoloads. |
| `config/eur/arm9/overlays/ov000/`, `ov001/` | yes | Overlays. |
| `include/nitro/types.h` | yes | `u8`…`s64`, `vu*`, `fx32` (20.12), `fx16` (4.12), `BOOL`, `TRUE`/`FALSE`, `NULL`. |
| `include/nitro/fs_overlay.h` | yes | `FS_OVERLAY_ID(ov000)`, which maps to the dsd LCF symbols `OVERLAY_0_ID` and `OVERLAY_1_ID`. |
| `include/<agent>/` | yes | Shared struct headers written by agents (`main_a2/task.h`, `main_sdk2/task.h`, `ov000_b/stageobj.h`, `ov000_d/player.h`, …). |
| `src/<agent>/` | yes (registered TUs only) | One function per `.c` file. Directory = name of the agent that produced it (§9). |
| `nearmiss/db.jsonl` | yes | Closest attempt per unmatched symbol (C source text plus diff stats), kept by `tools/matching/nearmiss.py`. |
| `state/tasks.json` | yes | Task board (§14). |
| `docs/agents/AGENT_BRIEF.md` | yes | Brief for matching workers. |
| `docs/architecture/` | yes | `overview.md` (principles, pipeline), `memory-map.md`. |
| `docs/research/0001…0011` | yes | Numbered research notes. `README.md` is the log. 0011 is the codegen-levers catalogue. |
| `docs/progress/` | yes | `dashboard.md` and `history.jsonl` (generated), dated reports, `REPORT-95.md`. |
| `tests/test_ndslib.py` | yes | Unit tests for `tools/ndslib` (no ROM required). Run them with `python -m unittest discover -s tests`. |
| `tools/` | yes | All project tooling (table below). |
| `tools/patches/` | yes | Two patches for dsd v0.12.1 (§5). |
| `tools/matching/probes/` | yes | Eight probe functions used by `compiler_sweep.py`. |
| `tools/bin/`, `tools/src/` | **no** | Locally built dsd, objdiff-cli and wibo, and their sources. |
| `tools/mwccarm/` | **no** | Proprietary Metrowerks bundle (`1.2/{base,sp2,sp2p3,sp3,sp4,b56}`, `2.0/*`, `dsi/*`). |
| `extract/` | **no** (except `README.md`) | `baserom_rush_eur.nds` (your ROM) and `extract/eur/` (dsd extraction). |
| `build/` | **no** | All build output, reports, agent reports, Ghidra exports. |
| `ghidra/` | **no** | Ghidra project. |
| `build.ninja`, `objdiff.json`, `.ninja_*` | **no** | Generated. |

### `tools/` in detail

| Tool | Purpose |
| ---- | ------- |
| `tools/project.py` | Project CLI: `doctor`, `analyze`, `extract`, `delink`, `verify`, `functions`, `symbols`, `coverage`, `progress`, `dashboard`, `ghidra`. Reports go to `build/<ver>/reports/`. |
| `tools/configure.py` | Writes `build.ninja` (§7). **Side effect:** it removes from every `delinks.txt` the TU entries whose source file no longer exists. |
| `tools/dashboard.py` | Called by `project.py dashboard`. Writes `docs/progress/dashboard.md`, appends to `docs/progress/history.jsonl`, and rewrites the README block. |
| `tools/tasks.py` | Task board CLI (§14). |
| `tools/ninja_syntax.py` | Vendored ninja writer. |
| `tools/bootstrap_session.sh` | Fresh-machine setup (§6). |
| `tools/setup_tools.sh` | Builds dsd (patched), objdiff-cli and wibo from pinned tags into `tools/bin/`. |
| `tools/ndslib/rom.py` | NDS header/OVT/FNT/FAT parser, `crc16_modbus`. |
| `tools/extraction/fix_path_order.py` | Rewrites `extract/<ver>/path_order.txt` from the base ROM's FAT. ds-rom orders zero-length files wrongly (docs/research/0005). |
| `tools/extraction/fix_header.py` | Restores the secure-area CRC and recomputes the header CRC. |
| `tools/utilities/sha1check.py` | `sha1check.py FILE EXPECTED`; exits 0 only on an exact match. |
| `tools/utilities/checkpoint.sh` | Lead integration cycle (§12). |
| `tools/utilities/checkpoint_loop.sh` | Runs `checkpoint.sh` every N seconds (default 900) and logs to `build/checkpoint_history.log`. |
| `tools/utilities/commit_verified.sh` | Runs verify, report and dashboard, then commits and pushes the **current** build without integrating anything. Needs a fresh `rush_eur.sha1.ok`. |
| `tools/matching/match.py` | The byte oracle for one source file (§10). `--promote` registers the TU. |
| `tools/matching/integrate.py` | Registers every unregistered `src/**/*.c` that passes `match.py`, and ejects TUs that `dsd delink` rejects (§12). |
| `tools/matching/compiler_sweep.py` | Compiles the probes with every installed compiler version and flag set. Writes `build/eur/reports/compiler_sweep.json`. |
| `tools/matching/gnu_relink.py` | Independent relink of the delinked objects with `arm-none-eabi-ld` (`ninja verify`, docs/research/0006). |
| `tools/matching/rom_diff.py` | Maps the byte differences between the built ROM and the base ROM to functions and owning TUs. |
| `tools/matching/eject_diffs.py` | Fast SHA-1 repair: blames TUs through `rom_diff.py`, moves them to `build/agents/rejected/`, and runs configure. |
| `tools/matching/bisect_rom.py` | Slow SHA-1 repair: binary search over the TUs that are not in git `HEAD`. Uses git and ninja. Lead only. |
| `tools/matching/fix_stale_names.py` | Rewrites default names (`func_/data_[ovNNN_]ADDR`) in `src/` that `symbols.txt` has since renamed. |
| `tools/matching/clone_dups.py` | Copies matched C to byte-identical twin functions, remapping relocation targets. Writes to `src/clones/`. |
| `tools/matching/recheck.py` | Regression test: re-runs `match.py` on every registered TU (`-j 4`, optional `--cc`). |
| `tools/matching/templates.py` | Zero-token tier. Lifts small branch-free ARM/Thumb functions to C, verifies them, and writes to `src/auto/`. Near-misses go to `build/eur/reports/templates_nearmiss.jsonl`. |
| `tools/matching/paramclone.py` | Reuses matched C for functions with the same instruction skeleton, remapping immediates and relocations. Writes to `src/auto/`. |
| `tools/matching/ghidra_tier.py` | Compiles mechanically fixed Ghidra drafts and keeps only the verified ones in `src/auto/`. |
| `tools/matching/similar.py` | Ranks the closest matched functions by opcode similarity. `--for <sym>` prints the best example source. Precomputed results are in `build/eur/reports/similar.jsonl`. |
| `tools/matching/nearmiss.py` | `save <file.c>`, `get <symbol>`, `list`. Keeps the best attempt per symbol in `nearmiss/db.jsonl`. |
| `tools/matching/accel_common.py` | Shared corpus (ROM bytes, modes, relocations, skeletons) for the four accelerators above. |
| `tools/analysis/rename.py` | Applies renames from a CSV everywhere (§13). |
| `tools/analysis/name_intrinsics.py` | Names the compiler runtime helpers from compiled TUs (§15.1). |
| `tools/analysis/sdk_ident.py` | Compiles reference library C out of tree and matches the result against Rush functions. Writes `build/eur/reports/sdk_ident*.csv`. |
| `tools/analysis/apply_names.py` | Applies `sdk_ident` names that pass the acceptance rule. |
| `tools/analysis/callgraph.py` | Builds the call graph from `relocs.txt`. Writes `build/eur/reports/callgraph.csv` and `candidates.csv`. |
| `tools/ghidra/` | `import.sh`, `export.sh`, `ApplyDsdSymbols.java`, `ExportAnalysis.java`, `ExportDecomp.java` (§10.1). |

---

## 5. Dependencies

| Dependency | Version | Notes |
| ---------- | ------- | ----- |
| Python | ≥ 3.11 (3.13 in use) | Modules: `pyyaml>=6`, `pyelftools>=0.31`, `capstone>=5` (`tools/requirements.txt`). |
| ninja | ≥ 1.10 (1.11.1 in use) | |
| dsd (ds-decomp) | **v0.12.1 plus two patches** | `tools/patches/ds-decomp-v0.12.1-thumb-unsigned-jumptable.patch` detects Sonic Rush's unsigned Thumb `switch` tables (`ldrh; add rX, pc; bx rX`). Without it ov001 analysis misses about 1,500 functions (docs/research/0008). `tools/patches/ds-decomp-v0.12.1-asm-entry-points.patch` adds `$d` mapping symbols for data inside `.text`, keeps the condition code of `R_ARM_PC24` branches, and allows functions defined in config to start with a conditional instruction. Hand-written asm with secondary entry points needs it. |
| objdiff-cli | v3.8.3 | `objdiff.json` requires `min_version` 2.3.2. |
| wibo | 1.2.0 | Runs the Windows Metrowerks executables on Linux. |
| Metrowerks CodeWarrior for NINTENDO DS | compiler **1.2/sp2**, linker **1.2/sp3** | Proprietary. Fetched by `bootstrap_session.sh`; never committed. |
| arm-none-eabi binutils | 2.42 | `arm-none-eabi-ld` for the relink check, `objdump`/`objcopy`. |
| Build-only: Rust/cargo ≥ 1.85, CMake, a C++ compiler, clang/libclang, lld | | Used by `setup_tools.sh` (lld and libclang are for wibo). |
| Ghidra | 12.2 (`ghidra_12.2_PUBLIC_20261016.zip`, SHA-256 `2c2cd776…a7be67`) | Optional. Needs **JDK 25** (`openjdk-25-jdk-headless`); 12.2 rejects JDK 21. |

**Compiler selection** (`config/eur/compilers.yaml`):

- `default.version: "1.2/sp2"` (status verified). Only sp2 reproduces the
  Thumb switch tables. sp3 and later emit `ldm {pc}` epilogues that the ROM
  does not have. 1.2/b56 and every 2.0 build are ruled out.
- Default flags:
  `-O4,p -proc arm946e -enum int -char signed -gccext,on -fp soft -interworking -sym on -gccinc -nolink -msgstyle gcc -w off -lang c99 -Cpp_exceptions off`
- `rules:` holds globs for `libs/NitroSDK/**` and `libs/NitroSystem/**`. No
  `libs/` directory exists, so every TU uses the default compiler.
- `linker: version "1.2/sp3"`, flags
  `-proc arm946e -nostdlib -interworking -nodead -map closure,unused -msgstyle gcc`.
  The `ninja` rule adds `-m Entry`. This linker reproduces the SHA-1.

**PATH.** Tools are looked up in the repository root, then in `tools/bin/`,
then on `PATH`. In this environment the built tools also live in
`/home/claude/bin`. Always run:

```sh
export PATH=/home/claude/bin:$PWD/tools/bin:$PATH
```

`integrate.py` calls a bare `dsd`, so the patched dsd must be first on `PATH`.

---

## 6. Fresh-machine setup

You need your own legally dumped ROM with the SHA-1 above. The tooling
refuses any other revision.

```sh
git clone <repo> sonic-rush-decomp && cd sonic-rush-decomp
bash tools/bootstrap_session.sh /path/to/baserom.nds
```

`tools/bootstrap_session.sh` does the following, in order:

1. If `ninja`, `cmake`, `arm-none-eabi-ld`, `clang` or `ld.lld` is missing, it
   runs `apt-get install ninja-build cmake binutils-arm-none-eabi clang lld libclang-dev`
   (with `sudo` when you are not root). Then it installs `tools/requirements.txt`.
2. If `tools/bin/{dsd,objdiff-cli,wibo}` are missing, it runs
   `tools/setup_tools.sh`. That script clones the pinned tags into
   `tools/src/`, applies `tools/patches/*.patch` to dsd, and builds.
3. If `tools/mwccarm/1.2/sp3/mwccarm.exe` is missing, it downloads
   `https://decomp.aetias.com/files/mwccarm.zip` (the bundle zeldaret/ph uses),
   checks SHA-256 `6f7b16098d2b1bc9b45b278f6e6e24327c4da9b3ebf7924be1afd4a721d83976`,
   and unzips it into `tools/`. The bundle is proprietary. The project owner
   directed its use (docs/research/0004). Never commit it.
4. It symlinks the ROM to `extract/baserom_rush_eur.nds`. Then it runs
   `project.py extract eur`, `configure.py eur`, `ninja`, `ninja verify`,
   `ninja report` and `project.py progress`.
5. Unless `WITH_GHIDRA=0`, it installs `openjdk-25-jdk-headless`, then
   downloads Ghidra 12.2, verifies its SHA-256, and unzips it into `$HOME/`.

Without Metrowerks, `configure.py` prints a note and the default ninja target
becomes `verify`. You can still extract, delink, run the relink check and
generate the objdiff config, but you cannot build the ROM or use `match.py`.

Optional: place `arm7_bios.bin` (your console's ARM7 BIOS) in the repository
root. Without it, the build restores the secure-area CRC from
`config/versions.yaml`.

`build/` is gitignored. After a fresh clone, the agent reports
(`build/agents/`), the Ghidra drafts and all reports are gone. The rules in
this file and in `CLAUDE.md` are enough to continue. Agent scratch
directories outside the repository (`/home/claude/w_*`) are referenced in task
notes, but they exist only on the machine that created them.

**Never commit** ROM images, BIOS dumps, anything under `extract/`, dsd asm
listings (`build/eur/asm`), Ghidra projects or exports, decompiler output, or
any Metrowerks file. `.gitignore` covers these paths. Check `git status`
before every commit.

---

## 7. Build pipeline, step by step

Generated by `tools/configure.py eur` into `build.ninja`. All paths are
relative to the repository root.

| # | ninja target / output | Command | Purpose |
| - | --------------------- | ------- | ------- |
| 1 | `extract` → `extract/eur/config.yaml` | `python3 tools/project.py extract eur`, which runs `dsd rom extract --rom extract/baserom_rush_eur.nds --output-path extract/eur`, then `tools/extraction/fix_path_order.py` | Checks the ROM SHA-1, extracts ARM9, the overlays, ARM7 and NitroFS, and fixes the file order. |
| 2 | `delink` → `build/eur/delinks.stamp` | `dsd delink --config-path config/eur/arm9/config.yaml` | Writes ELF objects with the original bytes and relocations to `build/eur/delinks/`: one per registered TU, plus gap objects `_dsd_gap@<module>_<n>` for everything else. Depends on every `config/eur/arm9/**/*.{txt,yaml}`. |
| 3 | `build/eur/arm9.lcf`, `build/eur/objects.txt` | `dsd lcf -c config/eur/arm9/config.yaml` | Linker command file (uses `ALIGNALL(4)`) and link order. |
| 4 | `build/eur/src/**.o` | `wibo tools/mwccarm/1.2/sp2/mwccarm.exe <flags> src/….c -o build/eur/src/….o` plus `-i include -i libs/include` | Compiles only TUs **registered in a `delinks.txt`**. Other files in `src/` do not affect the ROM. Header dependencies are scanned recursively. |
| 5 | `build/eur/arm9.o` | `wibo tools/mwccarm/1.2/sp3/mwldarm.exe -proc arm946e -nostdlib -interworking -nodead -map closure,unused -msgstyle gcc -m Entry @build/eur/objects.txt build/eur/arm9.lcf -o build/eur/arm9.o` | Links the compiled TUs and the delinked gap objects. |
| 6 | `build/eur/build/rom_config.yaml` | `dsd rom config --elf build/eur/arm9.o --config config/eur/arm9/config.yaml` | ROM layout from the linked ELF. |
| 7 | `build/eur/rush_eur.nds` | `dsd rom build --config build/eur/build/rom_config.yaml --rom build/eur/rush_eur.nds && python3 tools/extraction/fix_header.py eur build/eur/rush_eur.nds` | ROM image plus header fix-ups. |
| 8 | `rom` → `build/eur/rush_eur.sha1.ok` | `python3 tools/utilities/sha1check.py build/eur/rush_eur.nds 52e5024e… && touch …` | SHA-1 gate. **Default target** when the compiler is present. |
| 9 | `verify` → `build/eur/reports/gnu_relink.json` | `python3 tools/matching/gnu_relink.py --version eur --json …` | Relinks the delinked objects with GNU ld and compares all 11 sections. Default target when the compiler is absent. |
| 10 | `objdiff` → `objdiff.json` | `dsd objdiff --config-path config/eur/arm9/config.yaml --custom-make ninja` | objdiff project: target = delinked TU object, base = compiled TU object. |
| 11 | `report` → `build/eur/report.json` | `objdiff-cli report generate -o build/eur/report.json` | Per-function match percentages. |
| 12 | (not ninja) | `python3 tools/project.py progress` / `dashboard` | Verified numbers. |

Typical full run (lead only):

```sh
export PATH=/home/claude/bin:$PWD/tools/bin:$PATH
python3 tools/configure.py eur      # prints "wrote build.ninja (N source files; default target: rom)"
ninja                               # ROM + SHA-1
ninja verify                        # GNU relink check
ninja report                        # objdiff report
python3 tools/project.py progress
```

Flags for one TU are resolved in this order:

1. The default (or the matching `rules:` glob) from `compilers.yaml`.
2. A `// flags: …` line within the **first three lines** of the source file
   replaces those flags completely.
3. `-nolink` and `-Cpp_exceptions off` are appended if missing. The original
   ROM has no `.exceptix` tables.

`match.py` resolves flags the same way. The one difference is that it passes
only `-i include`, not `libs/include`.

`ninja verify` and gap objects make every commit byte-perfect. Code that is
not yet decompiled is linked from dsd's delinked objects (original bytes),
never from committed asm.

---

## 8. The dsd configuration files

All three file types are per module (`config/eur/arm9/`, `itcm/`, `dtcm/`,
`overlays/ov000/`, `overlays/ov001/`).

**`symbols.txt`**. One symbol per line:

```
WaitByLoop kind:function(thumb,size=0x4) addr:0x0200008c
BuildInfo kind:data(any) addr:0x02000b00
data_0207cfe0 kind:bss addr:0x0207cfe0
```

The kinds are `function(arm|thumb,size=…)`, `data(…)`, `bss` and `label`.
Default names are `func_XXXXXXXX`, `func_ovNNN_XXXXXXXX`, `data_…`, `bss_…`,
`__sinit_…` and `.L_…`, and they mean "unknown". Function sizes include the
literal pool. A source file must name its function exactly as it appears
here.

**`relocs.txt`**. One relocation per line:

```
from:0x02000808 kind:arm_call to:0x02000a14 module:main
from:0x02044ab0 kind:load to:0x02369ac0 module:overlay(0)
```

The kinds include `arm_call`, `thumb_call`, `arm_call_thumb`,
`thumb_call_arm`, `arm_branch` and `load`. An optional `add:` field gives an
addend. The module is `main`, `itcm`, `dtcm` or `overlay(N)`. Use this file
to name callees and globals correctly: `match.py` masks relocated bytes, so a
wrong name still byte-matches, and only the ROM link catches it.

**`delinks.txt`**. The module header lists the module's sections
(`.text start:… end:… kind:code align:32`, `.rodata`, `.ctor`, `.data`,
`.bss`). A TU entry follows each blank line:

```
src/main/func_0201bf1c.c:
    complete
    .text       start:0x0201bf1c end:0x0201bf34
```

A registered TU must start on a 4-byte boundary, because the LCF uses
`ALIGNALL(4)`. `match.py --promote` rounds the end up to 4, which absorbs
Thumb padding, and refuses overlaps with existing TUs. Only the single writer
edits `delinks.txt` (§12). Two scripts also prune entries whose file has
vanished: `configure.py` and `integrate.py`.

`config.yaml` contains a `hash:` per module. dsd maintains these values; do
not edit them by hand.

---

## 9. Source-file conventions (`src/`)

**One function per file.** The path is `src/<dir>/<symbol>.c`, and the
function name is exactly the `symbols.txt` name. There are two exceptions:

- A hand-written runtime routine that dsd split at mid-function entry points
  is one TU that defines every piece (§15.1). Each defined function must exist
  in `symbols.txt`; otherwise `match.py` reports `?  name: not in symbols.txt`
  and fails.
- A file can keep an old dsd name after `name_intrinsics.py` renamed its
  symbol. For example, `src/main_a2/func_0202e1cc.c` defines `_fadd` and
  `_fadd_ec`. The name inside the file is what counts.

**Directory = producing agent.** `main`, `ov000`, `ov001` hold the first
matches. `main_a*`, `main_b*`, `main_c*`, `main_d*`, `main_sdk*`, `ov000_*`,
`ov001_*`, `ov001_t*` (Thumb) are worker agents. The special directories are:

| Directory | Producer |
| --------- | -------- |
| `src/auto/` | `templates.py`, `paramclone.py`, `ghidra_tier.py` (header comment names the tier) |
| `src/clones/` | `clone_dups.py` |
| `src/asm/` | Functions C cannot produce. The header states why. |
| `src/asm_fallback/` | `asm` used after C attempts failed. The header names the best C draft. |
| `src/lead/` | The lead (e.g. the `BuildInfo` block) |
| `src/pragma/`, `src/sweep/` | Pragma and flag sweep experiments that matched |

`src/` holds about 7,450 `.c` files, and about 7,370 of them are registered.
The rest are duplicates of functions that already have a registered TU. They
are never built. `integrate.py` lists them as skipped.

**File header lines.** These are read from the first three lines:

- `// flags: <full flag string>`: per-file compiler flags. Only one file uses
  this (`-O1,p`). Prefer pragmas.
- `// asm: <reason>`: required on every `asm` file. It says why the file is
  `asm` (`C-impossible: …`, `fallback — C attempts did not match (best C draft: …)`,
  or `hand-written runtime library routine …`).
- `/* templates.py: … */` and similar provenance comments in `src/auto/`.

**Pragmas.**

- Thumb: put `#pragma thumb on` before the function and `#pragma thumb reset`
  after it. About 1,780 files do this.
- Optimiser pragmas go at file level, at the top, with no reset:
  `opt_propagation off`, `opt_common_subs off`, `opt_dead_assignments off`,
  `opt_lifetimes off`, `opt_unroll_loops off`, `opt_strength_reduction off`,
  `opt_loop_invariants off`, `optimize_for_size on`. mwccarm silently ignores
  unknown pragmas, so a typo looks like "no effect".

**Content rules.**

- `#include <nitro/types.h>`. Declare callees with their `symbols.txt` names.
  Declare globals as `extern T data_XXXXXXXX;`, using the name of the address
  that the literal-pool word holds.
- Never *define* global data in a TU. Use `extern` only.
- No string literals and no float constants that would create `.rodata` or
  `.data` in the TU, unless the target function's own literal pool holds
  them inline.
- Name local struct types `UnkStruct_<addr>` and declare only the fields the
  function needs. Shared layouts go in `include/<agent>/`.
- Write your own C. Do not copy code from reference projects (pret/pokediamond
  has no licence). A reference may suggest a shape; only Rush's bytes decide.
- `asm` functions use CodeWarrior inline-asm syntax:
  `asm void f(void) { … }`, with labels like `_34:`.

---

## 10. How to match a function

### 10.1 Inputs

- **Disassembly.** Main code is `extract/eur/arm9/arm9.bin` (base
  0x02000000). The overlays are `extract/eur/arm9_overlays/ov000.bin` and
  `ov001.bin` (base 0x022C5C80). ITCM is `extract/eur/arm9/itcm.bin` (base
  0x01FF8000). Disassemble with capstone (`CS_MODE_ARM` or `CS_MODE_THUMB`),
  or read `match.py`'s diff output. Thumb pools: read the raw words, because
  capstone's decoding of them misleads.
- **Ghidra drafts** (reading aid only).
  `build/eur/ghidra/<module>/decomp/<symbol>.c` for module `main` (which
  includes the ITCM and DTCM blocks), `ov000` and `ov001`. The same folders
  hold `functions.csv`, `xrefs.csv` (callers and callees) and `strings.csv`.
  Regenerate them with:

  ```sh
  export JAVA_HOME=/usr/lib/jvm/java-25-openjdk-amd64 GHIDRA_INSTALL_DIR=/home/claude/ghidra_12.2_PUBLIC
  bash tools/ghidra/import.sh eur      # full import: project in ghidra/eur/, exports in build/eur/ghidra/
  bash tools/ghidra/export.sh eur      # re-export only
  ```

  The import uses language `ARM:LE:32:v5t` and `BinaryLoader`, with one
  program per module. Each overlay program also gets main, ITCM and DTCM as
  extra blocks. `ApplyDsdSymbols.java` sets Thumb mode for every function
  before disassembly. Draft pool offsets can be wrong; verify them against
  the ROM. Never commit drafts, and never paste them as final source.
- **Siblings.** `python3 tools/matching/similar.py --for <symbol>` prints the
  top opcode-similar matched functions and the best one's source.
- **Previous attempts.** `python3 tools/matching/nearmiss.py get <symbol>`.
- **Call graph.** `build/eur/reports/callgraph.csv` (columns: module,
  address, name, mode, size, callers, callees).
- **Callers and callee signatures.** Hidden extra arguments explain many
  register choices.

### 10.2 Loop

```sh
python3 tools/matching/match.py src/<dir>/<symbol>.c                # default compiler and flags
python3 tools/matching/match.py src/<dir>/<symbol>.c --cc 1.2/sp3    # another compiler directory
python3 tools/matching/match.py src/<dir>/<symbol>.c --flags "-O4,s …"
```

For each defined function the output is one of:

- `OK   <name> (<module> 0x…, n/n bytes, 0 differ) [1.2/sp2]`
- `DIFF …`, followed by an aligned capstone diff in which relocated
  instructions show as `<reloc>`
- `?    <name>: not in symbols.txt`

Exit codes: 0 means every function matched, 1 means a mismatch, 2 means a
compile error (messages printed).

Masking: ARM `BL`/`B` imm24 fields are masked, and the bits that change when
the linker turns `BL` into `BLX` are tolerated. The Thumb BL pair has its two
imm11 fields masked. Every other relocation masks the whole word. A function
that has `OK` in `match.py` can still fail the SHA-1 if a relocation points
at the wrong symbol. Check callees and pool words against `relocs.txt`.

Order of work:

1. Fix the first **structural** divergence before anything else: control
   flow, a missing or extra instruction, load width or sign. Most apparent
   "register allocation" misses are structural.
2. Then use register and schedule levers (§11).
3. Stop after a reasonable number of failed attempts. Save the best attempt
   with `python3 tools/matching/nearmiss.py save src/<dir>/<symbol>.c --note "…"`,
   then **delete the file**. Non-matching files must not stay in `src/`.
4. If C is impossible (for example two pool words to one symbol, linker
   constants in the pool without relocations, or a switch-table density rule
   that no CW 1.2 build reproduces), write an `asm` function with a
   `// asm: C-impossible: …` header. It must still pass `match.py`.

Bytes are the priority. One 600-byte function is worth fifty tiny ones.

Worker agents report each verified file as `OK <symbol> <path>` in
`build/agents/<agent>.txt`, followed by `asm <symbol>` lines and counts.

---

## 11. Codegen levers catalogue

These are distilled from `docs/research/0011-mwccarm-levers.md`, the agent
reports in `build/agents/`, the task notes, and `docs/progress/REPORT-95.md`.
All were observed on CW 1.2/sp2 unless marked otherwise.

### 11.1 Pragmas (file level)

| Symptom | Pragma |
| ------- | ------ |
| Two registers swapped (e.g. `w`/`i`), wrong CSE | `#pragma opt_common_subs off` |
| Loop index and pointer swapped | `#pragma opt_lifetimes off` |
| ROM recomputes a constant you fold; global base kept in a callee-saved register | `#pragma opt_propagation off` |
| Index strength-reduced into a pointer | `#pragma opt_strength_reduction off` |
| Invariant hoisted out of a loop | `#pragma opt_loop_invariants off` |
| Dead store the ROM keeps | `#pragma opt_dead_assignments off` |
| Loop unrolled | `#pragma opt_unroll_loops off` |

Each pragma can also rotate the register colouring. Remove a crutch once the
shape matches, then re-check.

### 11.2 Declaration and statement order

- Callee-saved registers are assigned in **reverse declaration order**: the
  last declared local gets the lowest register.
- Declare locals without initialisers and assign them separately. The
  declaration order sets the registers; the statement order sets the
  schedule.
- Pairwise swaps of declaration positions find matches that random shuffles
  miss.
- Block-scoped declarations (`{ T *q = &P[i]; … }`) change lifetimes.
- Swap if/else arms, or invert the condition, to match the block layout.
- To get `popeq`/`bxeq` instead of predicated tail stores, put an explicit
  `return;` inside the if-block.
- CW evaluates call arguments right to left. This matters when an argument
  calls a function such as `Rand()`.

### 11.3 Hiding addresses from the optimiser: `(T*)(u32)&g`

- `UnkStruct_X *p = (UnkStruct_X *)(u32)&data_ov000_0236a48c;` keeps a
  constant struct address live in a register and stops alias analysis and
  copy propagation. A variant is `(T*)((u32)&g | 0)`.
- `extern T *volatile p;` forces a reload of the pointer.
- Declaring contiguous globals as **one struct** with fields, instead of
  separate externs, fixes base+offset addressing. Examples: `NNS_G3dGlb`
  fields at `data_02084d38`, and camera vectors.
- `(u8 *)arr + off` versus `arr + off` controls whether the compiler folds a
  relocation addend.

### 11.4 Extra callee arguments

If the ROM leaves r1 to r3 live into a call, the callee takes more arguments
than the obvious ones. Declare the prototype with them, for example
`func_ov001_022d983c(w, 1)` or the NNS callbacks
`cb(rs, opt, data, idx)` / `cb(jnt, p, rs->c, flag)`. This fixes register
choices that no reordering fixes.

### 11.5 Runtime helpers called explicitly

The compiler inserts calls to helpers such as `_s32_div_f`, `_u32_div_f`,
`_ll_udiv`, `_ull_mod`, `_fadd`, `_fmul` and `_fgr` for `/`, `%`, 64-bit
math and soft-float. If the ROM calls a *different* entry point, call that
function explicitly in C:

- Float compares go through `func_0202e5e4(f, 1.0f)` (>),
  `func_0202e564` (<=) and `func_0202e528` (< 0). Writing `f > 1.0f` would
  emit `_fgr` (0x0202e620), which is the wrong target.
- Divides go through `func_02002094` wherever the ROM calls it directly.
- `func_0202f17c` converts float to u32 (ffix), and `func_0202f1bc`
  converts u32 to float. This comes from ov001 agent notes.

### 11.6 Relocation addends are accepted

A pool word written as `data_X + N` that lands on a dsd-split symbol
(e.g. `data_02084d38 + 0xec` instead of `data_02084e24`) produces the same
bytes. objdiff counts it at 100 %. This is acceptable when the symbol-plus-
addend equals the ROM word. Check this against `relocs.txt`, not only with
`match.py`.

### 11.7 Expression forms

| Want | Write |
| ---- | ----- |
| `lsr` bitfield extract | extract through an unsigned type (signed gives `asr`) |
| `mov rX,#A; movne rX,#B` | `int r = A; if (x) r = B;` |
| `movne r0,#A; moveq r0,#B` | `return x ? A : B;` |
| batched `ld,ld,st,st` | whole-struct assignment `*d = *s` |
| Thumb `lsls #16; lsrs #16` | the value is `u16` (`s16` gives `asrs`) |
| sin/cos table scaling | `(s16)((v * T) >> 12)`, not `(v*T<<4)>>16` |
| bg family `h` shift | `(s16)((u32)(div >> 4) >> 8)` |
| `x / K` as `smull` magic | plain signed constant division (this is CW's lowering) |
| NNS anim weight-3 interpolation | `v + v + v + vs` (fx32 locals), not `*3` |
| Thumb bool return | `switch (x) { case 0: return 0; default: return 1; }` |
| loop counters | `int`, not `s32` |
| sign-extended byte | `((s32)(cell[0] << 24) >> 24)` instead of `(s8)cell[0]` |
| element pointer in a loop | an explicit pointer variable (`u32 *p = s->arr; p[i]`) or natural indexing (`p->grp[i]`), whichever fixes the spill-slot order |
| stack slot the ROM keeps | a `volatile` local |
| un-folded pointer arithmetic / bool temps | `static inline` helpers (`FX_Mul`, `NNS_G3dGlb*`, accessor `(u32)r + r->ofs`) |
| switch with dense jump table | list the low cases explicitly; source label order = block order; default first where the ROM has it |

### 11.8 Compiler and flags

If a whole TU shows an idiom nothing else produces, run
`tools/matching/compiler_sweep.py` or `match.py --cc …` before grinding.
`-O4,s` is available through `--flags`. A per-file `// flags:` line is a last
resort, and the file must still pass `match.py`.

---

## 12. Integration (single writer)

Only one process integrates. That process alone runs
`tools/matching/integrate.py`, edits `delinks.txt`, and runs
`tools/configure.py`, `ninja`, `dsd delink` and `git`. Worker agents only
write `src/<agent>/` (and optionally `include/<agent>/`) plus their report.

### `tools/matching/integrate.py [--no-build]`

1. Prunes TU entries whose source files have disappeared.
2. For every `src/**/*.c` that is not yet registered, runs
   `match.py --promote`. A full match appends:

   ```
   src/<dir>/<symbol>.c:
       complete
       .text       start:0x… end:0x…
   ```

   to the owning module's `delinks.txt`. Promotion is refused in the secure
   area, at an unaligned start, and on overlap with an existing TU.
3. Runs `dsd delink` up to 50 times. On `Failed to delink '<file>'` it
   removes that TU's entry and moves the file to `build/agents/rejected/`. If
   the failure is inside a gap object, it finds the TU that holds the
   referenced address (`Imported symbol … at 0x…`) and ejects that one.
4. Without `--no-build`, runs `configure.py eur`, `ninja`, `ninja verify`,
   `ninja report` and `project.py progress`, and stops at the first failure.

### `tools/utilities/checkpoint.sh` (the normal lead cycle)

It logs to `build/checkpoint.log` and prints the `total` line of
`project.py progress`:

1. `integrate.py --no-build`, then `clone_dups.py`, then
   `integrate.py --no-build` again, then `fix_stale_names.py`, then
   `configure.py eur`, then `ninja && name_intrinsics.py`.
2. If `ninja` fails: run `name_intrinsics.py` and `ninja` again. If it still
   fails, run up to three rounds of `eject_diffs.py` + `ninja`, then fall back
   to `bisect_rom.py`. If that also fails, the script prints
   `BUILD FAILED` and exits 1.
3. `ninja verify` (on failure it prints `RELINK CHECK FAILED` and exits 1),
   then `ninja report`, then `project.py progress`, then `project.py dashboard`.
4. `git add config include tools docs state nearmiss README.md CLAUDE.md .claude STATUS.md PLAN.md`
   plus every registered TU source. It commits `Checkpoint: <total line>` and
   pushes to `origin main`. If the push fails, it fetches, rebases and retries
   once.

The script never commits a build that failed the SHA-1. **`AGENTS.md` is not
in its `git add` list.** Commit changes to this file explicitly
(`git add AGENTS.md`).

Related scripts:

- `tools/utilities/commit_verified.sh` commits the current verified build
  without integrating. It requires `rush_eur.sha1.ok` to be newer than the
  ROM.
- `tools/utilities/checkpoint_loop.sh [seconds]` runs `checkpoint.sh` on a
  timer.
- `python3 tools/matching/recheck.py [-j N] [--cc VER]` re-verifies every
  registered TU.

After a checkpoint, `python3 tools/project.py dashboard` refreshes
`docs/progress/` and the README. Commit `README.md`, `state`, `CLAUDE.md` and
`.claude` if they changed.

---

## 13. Renaming symbols and the evidence standard

A name is documentation, not progress. Every non-default name needs an entry
in `config/eur/symbol_evidence.yaml`:

| Confidence | Meaning |
| ---------- | ------- |
| `confirmed` | Fixed by ROM metadata (e.g. `Entry` from header 0x024), or a byte-exact match against code whose name is known (e.g. a compiler-generated helper call, a matched SDK function). |
| `strong` | Several independent indicators, or a published signature (e.g. the dsd secure-area SWI stub analysis). |
| `tentative` | A plausible guess from one indicator. Revisit it. |

`project.py symbols` reports a name with no evidence entry as `tentative`.

### `tools/analysis/rename.py` (lead only: it calls `git mv`)

Input is one or more CSV files with the header `module,old,new,evidence`,
where module is `main`, `itcm`, `dtcm`, `ov000` or `ov001`.

```sh
python3 tools/analysis/rename.py names/batch.csv --dry   # validate only
python3 tools/analysis/rename.py names/batch.csv
```

The script rejects a row (and reports it) when:

- the old symbol is not in that module;
- the new name is not a valid C identifier;
- the new name collides with any symbol in any module;
- the row is proposed twice;
- the evidence is empty.

For each accepted row the script:

1. Renames the line in `symbols.txt`.
2. Rewrites whole-word uses in `src/**/*.{c,h}` and `include/**/*.h`.
3. Renames a TU file named after the old symbol and updates its `delinks.txt`
   path.
4. Carries an existing evidence entry over to the new name, or appends
   `confidence: tentative` with `"<evidence> (was <old>, <module>, <date>)"`.
   Raise the confidence by editing the YAML when the evidence supports it.

### Other naming tools

- `tools/analysis/sdk_ident.py` compiles reference library sources **out of
  tree**, matches them against Rush, and writes
  `build/eur/reports/sdk_ident*.csv`.
- `tools/analysis/apply_names.py <csv> --source "…" [--dry]` applies a name
  only if all of the following hold:
  - the masked byte pattern is unique among Rush functions;
  - exactly one reference name claims the address;
  - the function is at least 32 bytes, or another accepted candidate from the
    same reference file lies within 0x200 bytes.

  It renames only default `func_` names and never overwrites an existing name
  (docs/research/0010).
- `tools/analysis/name_intrinsics.py` (§15.1) writes `confirmed` entries.

Name style: NitroSDK and NitroSystem APIs use their public names
(`OS_IrqHandler`, `NNS_FndInitList`, `FS_StartOverlay`). Game functions are
named only when there is evidence.

---

## 14. Task board and multi-agent work

The board is `state/tasks.json` (committed). Tasks are partitions (module plus
address range; ov000 and ov001 share addresses, so the module matters) or
research items.

```sh
python3 tools/tasks.py brief                                  # resume summary (also the SessionStart hook)
python3 tools/tasks.py list [-p] [-v] [--status open|claimed|done] [--kind partition|research]
python3 tools/tasks.py claim <id> <agent> [--force]           # agent writes src/<agent>/, reports build/agents/<agent>.txt
python3 tools/tasks.py release <id> [--note TEXT]
python3 tools/tasks.py done <id> [--note TEXT]
python3 tools/tasks.py note <id> "TEXT"
python3 tools/tasks.py add "title" [--id ID] [--kind research] [--module ov000 --start 0x.. --end 0x..]
```

`brief` shows the following:

- verified bytes and functions, ROM SHA-1 and relink health;
- the next milestone;
- for each claimed task: the verified bytes in its scope, and how many of the
  agent's `OK` lines are not yet in the objdiff report ("pending
  integration", never counted);
- the open tasks.

The partitions are `main-sdk`, `main-mid`, `main-hi`, `ov000-lo`, `ov000-hi`,
`ov001-thumb`, `ov001-arm` and `itcm`. The research items are `r-arm7`,
`r-sra-lib`, `r-compiler-sp`, `r-crt0`, `r-data` and `r-ghidra` (done).
Task notes record levers and the locations of near-miss drafts. Read them
before you pick up a range.

Protocol:

1. Claim before you start.
2. Release the task if you are interrupted.
3. Record findings with `note` so the next session sees them.

Worker rules (from `CLAUDE.md` and `docs/agents/AGENT_BRIEF.md`):

- Write only `src/<agent>/` and optionally `include/<agent>/`.
- Never run git, ninja, `configure.py` or `dsd delink`.
- Never edit `config/`, `tools/` or `docs/`.
- Never delete or move files outside your own directory. If a function
  already has a file elsewhere in `src/`, skip it.
- Leave only files that pass `match.py`.

---

## 15. Important systems in the code

Only what the matched code, headers and reports show is listed here.
Everything else is unknown.

### 15.1 Compiler runtime helpers (main, around 0x0202D5D8–0x0202FCC8)

The MSL/CW runtime soft-float, 64-bit and division routines are named in
`config/eur/arm9/symbols.txt`:

- **Float and double:** `_dadd`, `_dfix`, `_dfixu`, `_dfltu`, `_dflt`,
  `_dmul`, `_ddiv`, `_fadd`, `_fgr`, `_deq`, `_dls`, `_dleq`, `_dgr`,
  `_dgeq`, `_fdiv`, `_ffix`, `_fflt`, `_fmul`, `_fsub`.
- **64-bit:** `_ll_udiv`, `_ull_mod`, `_ll_sdiv`, `_ll_mul`.
- **Division:** `_s32_div_f`, `_u32_div_f`.

They are hand-written asm with **mid-function entry points**. dsd splits each
one at its secondary entry, so the split pieces are named with an entry
suffix:

| Entry (size) | Secondary piece (size) |
| ------------ | ---------------------- |
| `_fadd` (0xC) | `_fadd_ec` (0x210) |
| `_fsub` (0xC) | `_fsub_ec` (0x26C) |
| `_dadd` (0x10) | `_dadd_e10` |
| `_ddiv` (0x10) | `_ddiv_e10` |
| `_ull_mod` (0x8) | `_ull_mod_e8` |
| `_ll_sdiv` (0x10) | `_ll_sdiv_e10`, `_ll_sdiv_e74` |
| `_u32_div_f` (0x8) | `_u32_div_f_e8` |

Each routine is one TU that defines every piece as an `asm` function. For
example, `_fadd` ends with `bmi _fsub_ec`, and `_fadd_ec` follows. The asm
entry-point dsd patch (§5) is what makes these TUs delink. `_fdiv` emits its
256-byte reciprocal table as data inside `.text`.

`tools/analysis/name_intrinsics.py` (run after `ninja` has compiled the TUs)
handles the names. When a compiled TU calls an undefined `_…` symbol, the
original ROM has a `BL` at the same offset, and `relocs.txt` gives its
target. The script renames that main-module ARM `func_` target to the
compiler's name, rewrites `src/`, and adds a `confirmed` evidence entry.
Thumb stubs in overlays are linker veneers, so it does not rename them. The
naming mismatch caveat in §11.5 still applies: the ROM sometimes calls an
internal compare entry instead of `_fgr`.

### 15.2 NitroSDK and NitroSystem

The SDK version word is 0x02017532 (`BuildInfo`; NitroSDK 2.1 era). The main
block 0x02000800–0x0201C000 (task `main-sdk`) holds the following
(`src/main_sdk*`, `include/main_sdk2/`):

- `Entry`/crt0, `OS_IrqHandler` (ITCM 0x01FF8000);
- OS threads and messages;
- cache primitives (`DC_StoreRange`, `DC_InvalidateRange`, `IC_InvalidateRange`);
- `FS_StartOverlay` and `FSi_GetOverlayBinarySize`;
- MI copy and fill;
- card, WM and multiboot structures (`ctrdg.h`, `wm*.h`, `mb*.h`);
- the sound player (`sndp.h`).

NitroSystem names that are present include `NNS_FndInitList`,
`NNS_FndGetNextListObject`, `NNS_FndGetPrevListObject`, `NNSi_SndFaderInit`,
`NNSi_SndFaderUpdate` and `NNSi_SndFaderIsFinished`.

**NNS 3D globals.** `NNS_G3dGlb` lives at `data_02084d38`. Camera functions
need its fields accessed as struct members (camPos/up/target copies, then
`flag &= ~0xe8`), and an inline LookAt. The G3D render-state struct used by
the NNS callbacks is `include/main_a2/g3drs.h` (`UnkRS`: callback table
`cb[0x20]`, `timing[0x20]`, current node and material, mat/jnt animation
pointers, resMat/resShp). NNS callbacks take extra arguments (§11.4).

### 15.3 Task system

Two linked-task structures are reconstructed:

- `include/main_a2/task.h`, `UnkTask`: a doubly linked list
  (`prev`, `next`), `func`, `dtor(UnkTask*)`, `work`, `flags`, `prio`, `id`.
  This is the game task. Stage objects are created through task constructors
  such as `func_020372d0(f, g, b, c, d, e)` and `func_02006abc(i)`, which
  return `UnkStruct_Task *` (`include/ov000_b/stageobj.h`).
- `include/main_sdk2/task.h`, `UnkTask_0201a2e8`: a singly linked `next`,
  `busy:1`/`priority:31` bitfields, `func`, `done`. It sits in a context with
  a 0x94-byte thread, a `volatile` list head and an idle task. The context
  pointer is `data_02080f40`. This is an SDK-side thread task queue.

### 15.4 Stage objects (ov000)

`include/ov000_b/stageobj.h` defines the common **0x20C-byte stage object
header** `UnkStruct_StageObj`. It contains:

- position and velocity words;
- two 0x48-byte sprite/animation blocks (`UnkStruct_Spr spr0, spr1` at
  0x94);
- two 0x2C-byte collision boxes (`UnkStruct_Coll coll0, coll1` at 0x134);
- a parameter pointer `UnkStruct_ObjParam *` at 0x1E0.

The same header declares the sprite and collision setup helpers in main
(`func_02069d38`, `func_0206990c`, `func_0206e3c0`, `func_0206e2b4`, …).
`include/ov000_b/blkobj.h` describes a 0x76C-byte nine-block object. Other
headers cover the camera, model, player, scene and sprite objects:
`include/ov000_d/{bgcam,mdl,mdl2,menuo,player,scene,sprobj}.h`. A task note
records `data_0236aad8` as a stage entry array with stride 0x64.

Whether ov000 is gameplay and ov001 front-end is a hypothesis
(memory-map.md). It is not established.

---

## 16. Troubleshooting

### ROM SHA-1 mismatch after `ninja`

1. Find the differing bytes:

   ```sh
   python3 tools/matching/rom_diff.py --limit 30
   ```

   It prints `header bytes differing: N` and one line per function with
   module, address, name, differing byte count and `TU=<src path>`. When the
   code size shifted, everything downstream differs, so look at the
   **first** hit.
2. Eject the blamed TUs:

   ```sh
   python3 tools/matching/eject_diffs.py
   ```

   It moves the sources to `build/agents/rejected/` and re-runs configure,
   which prunes their entries. Run `ninja` again.
3. If no TU can be blamed (exit 1), run `python3 tools/matching/bisect_rom.py`
   (lead only; it uses git `HEAD` and rebuilds repeatedly).
4. Typical causes:
   - `match.py` printed `OK`, but a relocation names the wrong symbol (masked
     in `match.py`; check `relocs.txt`);
   - a TU defines data or `.rodata` (§9);
   - an unaligned TU boundary.

### Undefined symbol at the mwldarm link

- The source uses a default name that has since been renamed: run
  `python3 tools/matching/fix_stale_names.py`.
- The compiler emitted a runtime helper (`_s32_div_f`, `_ull_mod`, `_ll_udiv`,
  `_fgr`, …) whose ROM address still has a `func_` name: run
  `python3 tools/analysis/name_intrinsics.py`, then `ninja`. The objects must
  be compiled first. `checkpoint.sh` already does both steps.
- `OVERLAY_0_ID` / `ARM9_CTOR_START` and similar symbols are dsd LCF
  symbols. Use `include/nitro/fs_overlay.h`.

### `dsd delink` rejects a TU (`Failed to delink '<file>'`)

`integrate.py` ejects the TU automatically to `build/agents/rejected/`.
Common causes:

- A neighbour branches to a local label inside the TU. dsd reports
  `Imported symbol … at 0x…` from a gap object, and `integrate.py` ejects
  the TU that covers that address.
- A hand-written routine has secondary entry points. Make sure the dsd in use
  includes the asm-entry-points patch, and define every split piece in one TU.
- A wrong symbol size in `symbols.txt` (e.g. a bogus 4-byte split such as the
  former `func_0202d4e8`). The lead must fix it in config.

### `match.py` problems

- Exit 2: compile error. The compiler messages are printed. The output
  filters lines containing `MWCIncludes` or `Usage Warning`.
- `?    name: not in symbols.txt`: the file defines an extra function (a
  `static inline` that was not inlined, or a typo in the name).
- `--promote` prints `secure area …`, `unaligned …` or `duplicate …` (exit 4
  or 3): see §3 and §8.
- Length differs: count the literal-pool words on both sides. Symbol sizes
  include the pool.

### objdiff below 100 % while the SHA-1 is OK

- A pool relocation uses a different symbol than dsd's (an addend case). It
  is usually accepted. If objdiff rejects it, rewrite the symbol or eject the
  TU.
- A stale `objdiff.json` or report after a TU moved. `objdiff.json` is
  regenerated when the `delink` stamp changes. Re-run
  `configure.py eur && ninja && ninja report`.

### Other

- `ninja verify` fails: a `delinks.txt` or `relocs.txt` edit broke the
  delinked objects. Read `build/eur/reports/gnu_relink.json` and revert the
  config edit.
- `configure.py` prints `note: …mwccarm.exe not found`: install the
  Metrowerks bundle (§6). Until then the default target is `verify`.
- `configure.py` prints `pruned missing TU …`: a registered source vanished,
  and its entry was removed. This is expected when duplicates are deleted.
- `project.py doctor` lists everything that is present or missing.

---

## 17. Known stale documents

The following documents predate the current state. Trust this file and the
code over them.

- `STATUS.md` and `PLAN.md` describe session 2 (8 matched functions, Ghidra
  missing, compiler service pack open).
- `INSTALL.md` says Ghidra 11.x with JDK 21, says the compiler is not
  established, and gives `1.2/sp3` as an example. The current values are
  Ghidra 12.2 with JDK 25, compiler 1.2/sp2 and linker 1.2/sp3.
- `CONTRIBUTING.md` describes a manual delinks workflow. Today workers write
  `src/<agent>/` and the lead integrates with `checkpoint.sh`. Its ground
  rules (no game data, matched means bytes, names need evidence, Ghidra is
  not source) still apply.
- In `docs/research/README.md`, 0004 and 0007 are still marked open. The
  notes themselves record the results.
- `config/eur/compilers.yaml`: the `default.evidence` text predates
  verification. The `status`/`version` fields are current.
- `tools/ghidra/*` header comments say "not yet executed". The scripts work
  (0007).
- `docs/architecture/overview.md` mentions `libs/` and `include/sonic/`.
  Neither exists.

---

## 18. Safety rules (summary)

1. Never commit ROM data, BIOS or firmware, `extract/`, asm listings, Ghidra
   projects or drafts, decompiler output, or Metrowerks files.
2. Never commit a build that fails the SHA-1 check. `checkpoint.sh` and
   `commit_verified.sh` enforce this.
3. Never relax the verification rules in §1. Never count agent-reported,
   renamed or partially matching functions.
4. There is one integrator. Workers never run git, ninja, `configure.py` or
   `dsd delink`, and never edit `config/`, `tools/` or `docs/`.
5. Do not create TUs in the secure area (0x02000000–0x02000800).
6. Do not define global data in TUs. Do not add literals that create
   `.rodata`/`.data`.
7. Write your own C. Reference projects inform shape, not text.
8. Every non-default name needs evidence in `symbol_evidence.yaml`.
9. Delete non-matching files. Keep near-misses in `nearmiss/db.jsonl` through
   `nearmiss.py save`.
10. Commit only when the user asks (`CLAUDE.md` §4 for the lead).

---

## 19. Command reference

```sh
# environment
export PATH=/home/claude/bin:$PWD/tools/bin:$PATH
python3 tools/project.py doctor
python3 tools/tasks.py brief

# build (lead)
python3 tools/configure.py eur && ninja && ninja verify && ninja report
python3 tools/project.py progress
python3 tools/project.py dashboard
bash tools/utilities/checkpoint.sh

# matching (anyone)
python3 tools/matching/match.py src/<agent>/<symbol>.c [--cc 1.2/sp2] [--flags "..."]
python3 tools/matching/similar.py --for <symbol>
python3 tools/matching/nearmiss.py get <symbol>
python3 tools/matching/nearmiss.py save src/<agent>/<symbol>.c --note "..." --agent <agent>
python3 tools/matching/compiler_sweep.py --versions 1.2/sp2,1.2/sp3 [probes...]

# accelerators (lead; write src/auto or src/clones, verified by match.py)
python3 tools/matching/templates.py [--max 0x80] [--module ov000] [-j 2]
python3 tools/matching/paramclone.py [--max 0x400] [-j 2]
python3 tools/matching/ghidra_tier.py [--max 0x400] [-j 2]
python3 tools/matching/clone_dups.py [--dir clones]

# repair / regression (lead)
python3 tools/matching/rom_diff.py --limit 30
python3 tools/matching/eject_diffs.py
python3 tools/matching/bisect_rom.py
python3 tools/matching/fix_stale_names.py
python3 tools/analysis/name_intrinsics.py
python3 tools/matching/recheck.py -j 4

# analysis and reports
python3 tools/project.py functions|symbols|coverage|analyze eur
python3 tools/analysis/callgraph.py
python3 tools/analysis/rename.py names.csv --dry

# Ghidra
export JAVA_HOME=/usr/lib/jvm/java-25-openjdk-amd64 GHIDRA_INSTALL_DIR=/home/claude/ghidra_12.2_PUBLIC
bash tools/ghidra/import.sh eur

# tests
python -m unittest discover -s tests
```

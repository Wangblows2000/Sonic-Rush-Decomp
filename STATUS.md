# Status — 2026-11-15 (session 2)

## Current milestone

Milestone 6 (decompilation) started. Compiler established; full ROM rebuilds
from source + delinked objects with an identical SHA-1.

## Measured output

| Check | Result | Command |
| ----- | ------ | ------- |
| Base ROM | EUR `ASCP` rev 0, SHA-1 `52e5024e…` verified | `project.py doctor` |
| ROM extract → rebuild | **SHA-1 identical** | `project.py verify eur` |
| Delinked objects → GNU ld relink | **11/11 sections byte-identical** | `ninja verify` |
| Functions identified | 7,391 (main 2,855 · itcm 14 · ov000 2,799 · ov001 1,723) | `project.py functions` |
| `.text` covered by functions | main 99.5 % · ov000 100 % · ov001 100 % · itcm 100 % of code (rest is a 0x701C zero buffer) | `project.py coverage` |
| Named functions | 138 (2 confirmed, 83 strong, 53 tentative) | `project.py symbols` |
| Call graph | 27,575 call edges; 2,344 leaf functions; 1,970 small first-match candidates | `tools/analysis/callgraph.py` |
| **Matching ROM build (mwldarm)** | **SHA-1 identical** with 8 C translation units | `ninja` |
| Compiler | CW for NDS 1.2 (base–sp4 indistinguishable so far); 2.0 and 1.2/b56 ruled out | `tools/matching/compiler_sweep.py` |
| Matched functions / bytes | **8 / 364 B of 1,422,472 B** (objdiff 100 %) | `ninja report && project.py progress` |

## Completed

- Research of 5 reference projects; architecture chosen (dsd pipeline).
- Toolchain built from source; `tools/setup_tools.sh`.
- ROM parser, analysis report, extraction fix-ups, header fix-up.
- dsd analysis patched for Sonic Rush's unsigned Thumb jump tables
  (+1,848 functions, all main→overlay calls resolved).
- GNU-ld relink verifier with four object normalisations.
- Project CLI, ninja configure, Ghidra scripts, docs.
- Call-graph ranking; 5 NitroSDK primitives identified (0010).
- ITCM gap explained (zero buffer).

## Blockers

1. Ghidra: not installed here (prebuilt binaries cannot be executed in this
   environment); scripts still untested.
2. Exact CW 1.2 service pack: needs discriminating probes.

## Toolchain

| Tool | Status |
| ---- | ------ |
| dsd v0.12.1 + patch | ok |
| objdiff-cli v3.8.3 | ok |
| wibo 1.2.0 | ok (runs mwccarm/mwldarm) |
| mwccarm 1.2/sp3 | ok — community bundle, used on the user's explicit instruction; not committed (0004) |
| arm-none-eabi binutils 2.42 | ok |
| Ghidra | missing |

## Research findings (latest)

- Overlays 0/1 share base 0x022C5C80; main calls into both directly.
- Thumb `switch` idiom without sign extension → older compiler than
  dsd's usual targets (evidence for CW 1.2 era).
- dsd ELF objects need normalisation for GNU ld (symtab order, Thumb bit,
  R_ARM_CALL, ABS32 in-place value).

## Next concrete tasks

1. Decompile the SDK functions identified in 0010 (own C) and place NitroSDK TUs.
2. Work through `candidates.csv` (small leaves) with `tools/matching/match.py --promote`.
3. Discriminate the 1.2 service pack; test overlay code with the same compiler.
4. ARM7 identification; Ghidra on a machine where it can run.

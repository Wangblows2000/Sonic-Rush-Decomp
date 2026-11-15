# Status — 2026-11-15

## Current milestone

Milestone 4 (RE environment) mostly done; Milestone 5 (matching) blocked on
the compiler.

## Measured output

| Check | Result | Command |
| ----- | ------ | ------- |
| Base ROM | EUR `ASCP` rev 0, SHA-1 `52e5024e…` verified | `project.py doctor` |
| ROM extract → rebuild | **SHA-1 identical** | `project.py verify eur` |
| Delinked objects → GNU ld relink | **11/11 sections byte-identical** | `ninja verify` |
| Functions identified | 7,391 (main 2,855 · itcm 14 · ov000 2,799 · ov001 1,723) | `project.py functions` |
| `.text` covered by functions | main 99.5 % · ov000 100 % · ov001 100 % · itcm 100 % of code (rest is a 0x701C zero buffer) | `project.py coverage` |
| Named functions | 32 (2 confirmed, 30 strong); 7,359 unknown | `project.py symbols` |
| Call graph | 27,575 call edges; 2,344 leaf functions; 1,970 small first-match candidates | `tools/analysis/callgraph.py` |
| Matched functions / bytes | **0 / 0 of 1,422,472 B** (nothing compiled) | `project.py progress` |

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

1. **Metrowerks compiler.** Running `mwccarm.exe` was blocked in the setup
   environment. Needed for compiler identification and any matching. The
   user must run it locally (Linux via wibo, or Windows).
2. **Ghidra** not available in the setup environment; scripts untested.
3. **Push permission.** Work is committed locally on `main`; not pushed.

## Toolchain

| Tool | Status |
| ---- | ------ |
| dsd v0.12.1 + patch | ok |
| objdiff-cli v3.8.3 | ok |
| wibo 1.2.0 | built; untested with mwccarm |
| mwccarm | missing (proprietary) |
| arm-none-eabi binutils 2.42 | ok |
| Ghidra | missing |

## Research findings (latest)

- Overlays 0/1 share base 0x022C5C80; main calls into both directly.
- Thumb `switch` idiom without sign extension → older compiler than
  dsd's usual targets (evidence for CW 1.2 era).
- dsd ELF objects need normalisation for GNU ld (symtab order, Thumb bit,
  R_ARM_CALL, ABS32 in-place value).

## Next concrete tasks

1. Automated SDK signature search from pokediamond/pokeplatinum asm (0010).
2. Place NitroSDK translation-unit boundaries in delinks.txt.
3. Main secure-area / crt0 gap annotation.
4. With compiler: run the 0004 version sweep; first matched function.
5. ARM7 identification.

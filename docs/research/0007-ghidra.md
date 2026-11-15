# 0007 — Ghidra workflow

**Status.** Scripts written; **not executed yet** (no Ghidra in the setup
environment, and downloaded prebuilt binaries could not be run there).

## Setup

- Ghidra 11.x (official releases: github.com/NationalSecurityAgency/ghidra),
  JDK 21. `export GHIDRA_INSTALL_DIR=...`.
- `python tools/project.py ghidra eur` runs `tools/ghidra/import.sh`.

## What the import does

- One program per module: `main` (0x02000000, plus ITCM 0x01FF8000 and DTCM
  0x027E0000 blocks), `ov000`, `ov001` (both 0x022C5C80, each with main/ITCM/
  DTCM as extra blocks so calls back into main resolve).
- Language `ARM:LE:32:v5t` (ARM946E-S is ARMv5TE; Ghidra's v5t covers the
  instructions used here), loader `BinaryLoader`.
- `ApplyDsdSymbols.java` (pre-script): reads dsd `symbols.txt`, sets `TMode`
  over Thumb functions, disassembles, creates functions/labels. Default names
  get `SourceType.ANALYSIS`, evidence-backed names `IMPORTED` — keeps
  "unknown vs named" visible.
- `ExportAnalysis.java` (post-script): `functions.csv`, `xrefs.csv`,
  `strings.csv` into `build/<ver>/ghidra/<module>/` for comparison with dsd.

## Rules

- dsd's symbols are the source of truth for boundaries; Ghidra disagreements
  are investigated, not copied.
- Decompiler output is never committed or treated as source.

## Next

- Run on a machine with Ghidra; fix API issues; add `compare_ghidra.py`
  (dsd vs Ghidra function starts/sizes).

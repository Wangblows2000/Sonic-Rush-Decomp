# 0007 — Ghidra workflow

**Status (2026-11-15, session 2).** Working: Ghidra 12.2
(`ghidra_12.2_PUBLIC_20261016.zip`, official NSA release, SHA-256
`2c2cd776db3c1f6a097ba418922378ac3649cadd5e844a6f5f088ae628a7be67`) with
OpenJDK 25 (`openjdk-25-jdk-headless`; 12.2 rejects JDK 21). Headless import of
main/ov000/ov001 applies dsd symbols; `ExportDecomp.java` writes per-function
decompiler drafts to `build/eur/ghidra/<module>/decomp/` for matching agents.
Fix applied: Thumb context is set for all functions before any disassembly
(single-pass caused 531 ov000 / 986 main "context conflicts"; ov001 went to 0).

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

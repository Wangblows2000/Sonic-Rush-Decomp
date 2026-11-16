# 0007 — Ghidra workflow

**Status.** Done (final state, 2026-11-16). Ghidra 12.2
(`ghidra_12.2_PUBLIC_20261016.zip`, official NSA release, SHA-256
`2c2cd776db3c1f6a097ba418922378ac3649cadd5e844a6f5f088ae628a7be67`) with
OpenJDK 25 (`openjdk-25-jdk-headless`; 12.2 rejects JDK 21). Headless import of
main, ov000 and ov001 applies the dsd symbols and Thumb modes.
`ExportDecomp.java` writes one decompiler draft per function to
`build/eur/ghidra/<module>/decomp/`. Matching agents used these drafts for
control flow, struct offsets and call arguments throughout the project.
**Confidence.** High (run end to end; drafts consumed by agents).

## Setup

```sh
export JAVA_HOME=/usr/lib/jvm/java-25-openjdk-amd64
export GHIDRA_INSTALL_DIR=/home/claude/ghidra_12.2_PUBLIC   # or your install
bash tools/ghidra/import.sh eur          # same as: python tools/project.py ghidra eur
bash tools/ghidra/export.sh eur          # re-export only, no re-import
```

## What the import does

- One program per module: `main` (0x02000000, plus ITCM 0x01FF8000 and DTCM
  0x027E0000 blocks), `ov000`, `ov001` (both 0x022C5C80, each with main/ITCM/
  DTCM as extra blocks so calls back into main resolve).
- Language `ARM:LE:32:v5t` (ARM946E-S is ARMv5TE; Ghidra's v5t covers the
  instructions used here), loader `BinaryLoader`.
- `ApplyDsdSymbols.java` (pre-script): reads dsd `symbols.txt`, sets `TMode`
  over **all** Thumb functions before any disassembly, then disassembles and
  creates functions and labels. A first single-pass version caused 531 ov000 /
  986 main "context conflicts"; setting the context first removed them (ov001
  went to 0). Default names get `SourceType.ANALYSIS`, evidence-backed names
  `IMPORTED`, which keeps "unknown vs named" visible.
- `ExportAnalysis.java` (post-script): `functions.csv`, `xrefs.csv` and
  `strings.csv` in `build/<ver>/ghidra/<module>/`.
- `ExportDecomp.java` (post-script): `decomp/<symbol>.c`, 30 s timeout per
  function.
- `tools/matching/ghidra_tier.py` compiled the drafts after mechanical fixes
  (Ghidra types, `FUN_` names, pool words) and kept only those that
  `match.py` verified; the results are in `src/auto/`.

## Rules

- dsd's symbols are the source of truth for boundaries; Ghidra disagreements
  are investigated, not copied.
- Decompiler output is never committed or treated as source. `build/` and
  `ghidra/` are gitignored.

## Not done

- A dsd-vs-Ghidra boundary comparison script (`compare_ghidra.py`) was planned
  but never written. With every function matched and the ROM SHA-1 identical,
  the dsd boundaries no longer need it.

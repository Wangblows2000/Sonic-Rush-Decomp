# Plan

## Milestone 1 — Research
- [x] Inspect pokediamond, ph, mkds-re, mariopartyDS (docs/research/0002)
- [x] Find Sonic Rush / Rush Adventure prior work; separate the two (0009)
- [x] Architecture proposal (docs/architecture/overview.md)
- [ ] Inspect pokeplatinum / pokeheartgold SDK version notes
- [ ] Study SRA engine headers for hypotheses (task system, objects)

## Milestone 2 — Environment
- [x] Python, Git, C/C++ compilers, ninja, cmake checked
- [x] ARM binutils installed
- [x] dsd, objdiff-cli, wibo built from pinned sources (tools/setup_tools.sh)
- [x] `project.py doctor`
- [ ] Metrowerks compiler runnable (**blocked here**, user machine)
- [ ] Ghidra installed and scripts executed

## Milestone 3 — ROM infrastructure
- [x] User-supplied ROM, SHA-1 gate (config/versions.yaml)
- [x] NDS header/OVT/FNT/FAT parser (tools/ndslib)
- [x] Extract ARM9, ARM7, overlays, NitroFS (dsd + fixups)
- [x] Byte-identical ROM rebuild (0005)
- [x] Machine-readable metadata (`project.py analyze`)
- [ ] ARM7: identify SDK component/version

## Milestone 4 — Reverse-engineering environment
- [x] dsd analysis config committed (7,391 functions)
- [x] Fix dsd Thumb jump-table analysis (0008)
- [x] Function export (CSV), coverage and symbol reports
- [x] Ghidra import/export scripts (untested)
- [ ] ITCM: analyse 0x701C-byte gap (14 functions found)
- [ ] main secure-area/crt0 gaps (19 small gaps)
- [ ] Cross-reference tooling (call graph from relocs.txt)
- [ ] SDK identification by signature (dsd sigs + SRA asm-derived patterns)

## Milestone 5 — Matching infrastructure
- [x] ninja build (configure.py): extract/delink/verify/objdiff targets work
- [x] Independent GNU relink check (0006)
- [x] Progress report counting matched bytes only
- [ ] Establish compiler version/flags (0004) — needs mwccarm
- [ ] Full ROM build via mwldarm + SHA-1 (first time with zero TUs)
- [ ] objdiff report wired into progress
- [ ] CI workflow (verify target)

## Milestone 6 — Decompilation
- [ ] First matched function (small leaf, SDK)
- [ ] NitroSDK OS/MI/FX basics
- [ ] Startup: Entry → main → game loop
- [ ] Shared engine utilities

## Milestone 7 — Continuous improvement
- [ ] Upstream dsd patches (jump tables, ELF symtab/Thumb/ABS32)
- [ ] Upstream ds-rom zero-length file ordering
- [ ] USA/JPN versions

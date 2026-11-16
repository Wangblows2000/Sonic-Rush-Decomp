# Plan — what remains

Measured state: [STATUS.md](STATUS.md). Done so far: research and architecture,
toolchain built from source, byte-identical ROM rebuild, dsd analysis (7,408
functions), GNU relink check, compiler identified (CW 1.2/sp2, linker 1.2/sp3),
full matching ROM build, 100 % of ARM9 code bytes matched and every function named.

## 1. ARM9 code bytes — done

- [x] 7,408 / 7,408 functions, 1,422,472 / 1,422,472 bytes (secure-area BIOS
  stubs matched via one GNU-as TU, docs/research/0012-secure-area.md).

## 2. Assembly back to C

- [x] 54 converted this session (`tools/matching/swap_fallback.py` swaps a
  verified C file in for its asm TU).
- [ ] 34 functions still in `src/asm_fallback/` plus `src/asm/__pformatter.c`.
  Exact blockers per function are the `FAIL` lines in `build/agents/c_*.txt`
  of the session that produced them (not in git); typical: callee-saved
  register rotation, one instruction scheduled differently, Thumb stack-slot
  order. Best lever found: port the analogous Sonic Rush Adventure / SDK
  source shape literally.
- [ ] `__pformatter`: CW 1.2 clusters the conversion switch differently (no
  case set reproduces the d..u table); try an if/goto compare tree as done
  for GX_SetBankForSubBG.

## 3. Naming

- [x] Every function named; evidence for each in
  `config/eur/symbol_evidence.yaml`; conventions and audit in
  docs/naming-conventions.md.
- [ ] Upgrade the 468 subsystem-level `<Subsystem>_Unk_<addr>` names to
  behaviour names (all `tentative`).
- [ ] Tie zone numbers (ZoneN_*) to the in-game zone titles.
- [ ] Name struct types (`UnkStruct_<addr>`) and move shared declarations into
  `include/` headers instead of per-file `extern` prototypes.

## 4. Source reorganisation

- [ ] Move TUs out of the per-agent directories (`src/<agent>/`, `src/auto/`,
  `src/clones/`, …) into a subsystem layout: `libs/NitroSDK`,
  `libs/NitroSystem`, `libs/MSL`, game code by module and subsystem.
- [ ] Merge adjacent one-function TUs back into original translation units
  where the evidence supports it (library objects link contiguously).
- [ ] Remove the 80 unregistered `.c` files under `src/` (duplicates and
  leftovers that are not in any `delinks.txt`) once they are confirmed
  redundant.

## 5. Data and rodata

- [ ] Decompile `.rodata`, `.data` and `.bss` into TUs: strings, tables and
  float constants. Today every TU only declares `extern` data, and all data
  comes from dsd's delinked gap objects. Needs a strategy for which data
  belongs to which TU (task `r-data`).
- [ ] DTCM (0x2060 bytes of data, no functions).

## 6. Beyond ARM9 EUR

- [ ] ARM7: identify the NitroSDK component and version (task `r-arm7`).
- [ ] Download Play children (`/mb/contest.srl`, `/mb/preview.srl`).
- [ ] USA (`ASCE`) and Japan (`ASCJ`) versions.
- [ ] CI workflow for `ninja verify`.
- [ ] Upstream the dsd patches (jump tables, asm entry points, ELF symtab /
  Thumb bit / ABS32 fixes from docs/research/0006) and the ds-rom zero-length
  file ordering fix (docs/research/0005).

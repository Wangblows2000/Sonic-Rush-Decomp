# Plan — what remains

Measured state: [STATUS.md](STATUS.md). Done so far: research and architecture,
toolchain built from source, byte-identical ROM rebuild, dsd analysis (7,408
functions), GNU relink check, compiler identified (CW 1.2/sp2, linker 1.2/sp3),
full matching ROM build, and 99.97 % of ARM9 code bytes matched.

## 1. Last ARM9 code bytes

- [ ] **Secure-area BIOS stubs (18 functions, 76 B).** `WaitByLoop`, `CpuSet`,
  `Div`, `GetCRC16` and the other stubs are Thumb `svc #n; bx lr` pairs (4 or
  6 bytes each) scattered through the 2 KB secure area at 0x02000000–0x02000800.
  They are not TUs for three reasons:
  1. Half of them (9 of 18) start at halfword addresses. The LCF uses
     `ALIGNALL(4)`, so a TU cannot start there.
  2. The bytes between them are not code. The area begins with the
     decrypted-secure-area marker `E7FFDEFF E7FFDEFF`, followed by filler that
     is most likely a prebuilt NitroSDK secure-area object.
  3. The ROM header's secure-area CRC covers these bytes (docs/research/0005).
  The region is therefore kept as one dsd gap object with the original bytes,
  and `match.py --promote` refuses addresses below 0x02000800. Closing the gap
  needs either one assembly TU for the whole 0x800 bytes (stubs plus filler as
  data) or an LCF exception that allows halfword-aligned objects.
- [ ] **`func_02010018` (340 B):** integrated as `src/asm_fallback/` after the
  last report. Confirm 100 % at the next checkpoint.

## 2. Assembly back to C

- [ ] Convert `src/asm_fallback/` (90 files, 44,082 B) back to C. Line 1 of
  each file says why C did not match. Some near-miss drafts are committed in
  `nearmiss/db.jsonl` (56 entries; `tools/matching/nearmiss.py get <symbol>`).
  The draft paths quoted on line 1 point to scratch directories outside the
  repository, which do not survive a fresh machine.
- [ ] Re-examine `src/asm/` (6 files, 3,524 B) marked "C-impossible". Some
  cases (switch grouping, switch split point) may still be reachable with
  different case ordering or a different SDK compiler for that TU.
- Hand-written runtime routines (CP15, SWI, soft-float, 64-bit helpers) stay
  as `asm`: the original was assembly too.

## 3. Naming refactor

In progress on branch `refactor/function-naming` (conventions, evidence
standard and status: docs/naming-conventions.md).

- [x] Rounds 1–3: 5,188 / 7,408 functions named (560 subsystem-level
  `_Unk_` names), every batch build-verified.
- [ ] Name the remaining 2,220 default-named functions and upgrade `_Unk_`
  names (see docs/naming-conventions.md §6 for the open list).
- [ ] Disambiguation rule for file-local library helpers whose official
  names repeat across files; re-check the possible misnames listed there.
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

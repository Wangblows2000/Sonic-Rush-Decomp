# 0010 — Identifying NitroSDK functions

**Question.** Which ARM9 functions are NitroSDK library code, and under what
names?
**Sources.** `tools/analysis/callgraph.py` ranking; pret/pokediamond NitroSDK
sources (`arm9/lib/NitroSDK/src/*.c`, hand-written `asm` functions);
capstone disassembly of the Rush binary.
**Confidence.** Strong for the five names below (instruction-identical bodies).

## Method

1. `callgraph.py` builds a call graph from relocs.txt (27,575 call edges) and
   ranks small leaf functions by caller count. The most-called tiny functions
   in a NitroSDK game are SDK primitives.
2. Each candidate is disassembled and compared instruction by instruction with
   the NitroSDK `asm` function bodies matched in pokediamond. Hand-written
   `asm` bodies do not depend on compiler version, so identical sequences are
   strong evidence even across SDK releases.

## Results

| Address | Size | Callers | Name | Reference |
| ------- | ---- | ------- | ---- | --------- |
| 0x0200763C | 20 | 133 | OS_DisableInterrupts | OS_system.c |
| 0x02007650 | 24 | 132 | OS_RestoreInterrupts | OS_system.c |
| 0x020082E8 | 24 | 386 | MIi_CpuClear16 | MI_memory.c |
| 0x0200831C | 20 | 310 | MIi_CpuClear32 | MI_memory.c |
| 0x020067C0 | 28 | 245 | DC_StoreRange | OS_cache.c |
| 0x02000E7C | 28 | 71 | MTX_RotY33_ (Thumb) | FX_mtx33.c |

Not named yet: 0x02006ABC (451 callers; loads word `[0x027FFDA0 + 4*r0]`,
i.e. a table in the shared main-RAM work area at the top of main memory —
plausibly an OS system-work accessor; tentative, needs a reference match).

These cluster at 0x020067C0–0x0200831C next to `FS_*` (0x0200C1EC–), consistent
with the SDK library block sitting early in main `.text` (OS, MI, FS ordering
as in other NitroSDK games).

## Automated identification (session 2)

`tools/analysis/sdk_ident.py` compiles a reference decomp's library C
**out of tree** with our verified compiler (CW 1.2/sp3 at the time; the final compiler is
1.2/sp2, see 0004) and compares every
resulting function, with relocated fields masked, against all Rush functions of
the same size. `tools/analysis/apply_names.py` applies a name only if the match
is unique and either ≥ 32 bytes or backed by a neighbouring match from the same
source file (≤ 0x200 B away). Under 32 bytes the evidence is `tentative`.

| Reference | Files compiled | Ref. functions | Unique hits | Named |
| --------- | -------------- | -------------- | ----------- | ----- |
| pret/pokediamond NitroSDK | 92/99 | 631 | 110 | 94 (+4 agreeing with manual names) |
| pret/pokediamond libnns (NitroSystem) | 4/4 | 29 | 8 | 8 |
| pret/pokediamond MSL_C | 2/2 | 3 | 1 | 0 (too small, isolated) |
| SRA NitroSDK / NitroSystem | 5/120, 1/50 | 27, 5 | 1, 3 | 0 (all agreed with existing names) |

The 4 hand-made names (0010 first pass) were independently re-derived.
SRA's library sources mostly fail to compile out of their build system (MSL
include ordering); fixing that is deferred, since its SDK is 3.x.

**Licensing.** pret/pokediamond has no licence file, so its code is all
rights reserved: it is used only to derive names. Our `src/` C is written
independently. zeldaret/ph is CC0.

## Next

Superseded: every SDK function in main is now matched C or asm (STATUS.md).
What remains is applying names, tracked as the naming refactor in PLAN.md.
Original list:

- Automate: parse pokediamond/pokeplatinum NitroSDK `asm` bodies and `.s`
  files, assemble with `arm-none-eabi-as`, mask relocated fields, and search
  Rush modules for byte matches; emit candidates with evidence.
- Use the SDK block boundaries to place `libs/NitroSDK` translation units in
  `delinks.txt`.

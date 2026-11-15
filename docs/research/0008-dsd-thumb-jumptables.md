# 0008 — Unsigned Thumb jump tables break dsd analysis

**Problem.** `dsd init` (v0.12.1) found only 185 functions in overlay 1 and
classed ~220 KB of it as `.rodata`; main contained 9 calls into overlay space
that resolved to no function (`module:none`).
**Sources.** dsd source (`lib/src/analysis/jump_table.rs`, `functions.rs`),
ARM ARM (Thumb `ADD Rd, PC` hi-register form), code-density profiling,
capstone disassembly.
**Confidence.** High. **Tests:** function counts, coverage, GNU relink (0006).

## Investigation

1. Coverage (`project.py coverage`): ov001 `.text` 24 % covered; gaps of
   0x12030 and more. ARM/Thumb prologue density showed Thumb code at
   0x022C5C80–~0x022ED000 and ARM code to ~0x02315xxx.
2. (Error made and corrected: a first manual check used 0x022C5BC0 as the
   overlay base due to a decimal→hex slip; the OVT value 36461696 is
   0x022C5C80, identical to dsd's.)
3. The 9 unresolved main→overlay calls land on function prologues in ov001
   (8) and ov000 (1) — functions dsd never discovered.
4. Debug log: linear function sweep in ov001 stopped at 0x022CC044 with
   "illegal instruction … used R7 when it has an undefined value". The
   function there contains:

   ```
   cmp   r0, #5
   bhi   default
   adds  r0, r0, r0
   add   r0, pc          @ r0 = 2*idx + (this + 4)
   ldrh  r0, [r0, #4]
   add   r0, pc          @ r0 = offset + (this + 4) = table + offset
   bx    r0
   .hword case0-table, case1-table, ...
   ```

   dsd's Thumb state machine requires `lsl #16; asr #16` between `ldrh` and the
   final add (signed offsets, CW 2.0 style). Without it the table is decoded as
   code, the function fails, and the sweep ends.

## Fix (tools/patches/ds-decomp-v0.12.1-thumb-unsigned-jumptable.patch)

New state `BxJumpExact`: from `SignExtendLsl`, accept `add jump, pc` followed
by `bx jump`. Table address computed exactly as
`address(add offset, pc) + 4 + imm`. A hi-register `ADD Rd, PC` reads PC
*without* word alignment. A first version aligned it and mis-placed tables
whose `add` sat at a halfword address (e.g. 0x02047AB6), breaking
`func_020479d8`. That was caught by dsd failing, fixed, and re-verified.

## Results

| Module | Functions before | after | .text coverage after |
| ------ | ---------------- | ----- | -------------------- |
| main | 2855 | 2855 | 99.5 % (gaps = secure area stubs/data) |
| ov000 | 2489 | 2799 | 100 % |
| ov001 | 185 | 1723 | 100 % |
| unresolved main→overlay calls | 9 | 0 | |

GNU relink of all sections still byte-identical with the new relocations.

## Follow-up

- Upstream the patch to AetiasHax/ds-decomp.
- ITCM: 14 functions cover 6.7 %; a 0x701C-byte gap remains (next task).

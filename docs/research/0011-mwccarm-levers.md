# 0011 — mwccarm codegen levers (distilled from SM64DS, checked against CW 1.2/sp2)

Status: done (living note) · Confidence: M (H for the rows marked *probed*)

## Source and licence

Distilled from tangosdev/sm64ds-decomp `notes/mwccarm-codegen.md` and
`notes/levers.jsonl` (MIT License, Copyright (c) 2026 Tango). That catalogue was
derived on an all-ARM title built with CodeWarrior for NITRO `2004/b56` /
`1.2/sp2p3`. Sonic Rush builds with **CW 1.2/sp2** (config/eur/compilers.yaml) and
about a third of the game is **Thumb**, so every lever below is tagged:

- *probed*: compiled here on 1.2/sp2 (2026-11-15, `-O4,p ... -lang c99`) and the
  stated effect observed;
- *SM64DS*: proven there, untested here (treat as a hypothesis to try, not a fact);
- *differs*: probed here and the behaviour is NOT what SM64DS reports.

## 0. How to work (all transfer)

- Fix the first **structural** divergence (control flow, missing/extra instruction)
  before touching register colouring. Most "regalloc" misses are structural.
- Reloc slots (bl targets, pool words with relocs) are wildcards in match.py: a wrong
  callee name still byte-matches. Name callees/globals from relocs.txt.
- Symbol sizes include the literal pool; count pool words on both sides before
  blaming pool growth.
- Before any lever, check source semantics: callers, sibling twins (use
  `tools/matching/similar.py --for <sym>`), callee signatures, dropped arguments.
- Keep your closest attempt: `tools/matching/nearmiss.py save <file>`; resume from
  `nearmiss.py get <sym>`.

## 1. Symptom -> lever table

| Diff shows | Try | Tag |
|---|---|---|
| Wrong load width/sign (`ldr` vs `ldrh/ldrsh/ldrb/ldrsb`) | change the local/field type or cast (u8/s8/u16/s16) | SM64DS, same on Thumb |
| `asr` where you emit `lsr` (bitfield extract) | extract through an **unsigned** type; signed gives `asr` | *probed* ARM: `(x>>4)&0xf` on `unsigned` -> `lsr; and`; `(x<<24)>>28` on `int` -> `lsl; asr` |
| `mvn r,#0` vs `mov r,#0xff` storing a byte | signed byte store of -1 emits `mvn`; unsigned 0xff emits `mov #0xff` | *probed* ARM; Thumb: `movs #0; mvns` vs `movs #0xff` |
| Predicated `mov rX,#A; movne rX,#B` | **default-then-override**: `int r = A; if (x) r = B;` | *probed* ARM |
| Predicated `movne r0,#A; moveq r0,#B` | ternary `return x ? A : B;` | *probed* ARM |
| `cmp; moveq r0,#1; movne r0,#0` | `return a == b;` (ARM). Thumb emits `cmp; bne; movs #1; bx lr; movs #0; bx lr` for the same source | *probed* |
| Separate early-out epilogues vs one shared exit | `if (a) return 0; if (b) return 0;` gives separate `movs r0,#0; bx lr` copies; `if (a && b) return x; return 0;` gives one shared exit | *probed* Thumb |
| Guard `cmp; movne; strne` (ARM) / `cmp; beq` (Thumb) | plain `if (p == 0) return; p->f = 0;` — no asm hatch needed | *probed* both |
| Two-word copy `ld,ld,st,st` vs `ld,st,ld,st` | whole-struct assignment `*d = *s` batches loads; field-wise copy interleaves | *probed* ARM and Thumb |
| Thumb `lsls #16; lsrs #16` after arithmetic | the value is `u16` (return type, param, or local); `s16` gives `asrs` | *probed* Thumb |
| Push set/register rename off by one register | declaration order of callee-saved locals (reverse order: last declared gets the lowest register), type of the named local, first-use order | SM64DS |
| ROM has `add rX, base, #off` then `[rX]`, you fold the offset | u64-mask launder `*(int *)(((int)p + OFF) & 0xFFFFFFFFFFFFFFFF)` | *differs*: Thumb materialises (`adds r0,#0x10; str r1,[r0]`) as in SM64DS, but ARM 1.2/sp2 folds into a pre-indexed writeback `str r1,[r0,#0x10]!` when `p` is dead afterwards |
| Halfword/byte access at offset >= 0x100 on ARM | plain C reproduces the materialised base; no launder | SM64DS (ARM only; Thumb thresholds are 0x20/0x40/0x80 by width) |
| `x / K` emitted as `smull` magic | that **is** the CW lowering of a signed constant division (`x/7` -> `ldr; lsr #31; smull; add; asr; add`) | *probed* ARM |
| Unsigned division ends `ldr ip,=_u32_div_f; bx ip` | a far tail call: plain `return a / b;` | *probed* ARM |
| Loop index strength-reduced into a pointer vs base+i each iteration | `#pragma opt_strength_reduction off` | SM64DS; *probed* inert on a trivial sum loop here (both forms identical) |
| Duplicated early-exit epilogues where you branch to a shared tail | `#pragma optimize_for_size on` | SM64DS; *probed* no effect on a 3-return Thumb probe |
| Constant folded away where the ROM recomputes it | `#pragma opt_propagation off` (blunt; prefer reshaping) | SM64DS |
| Stack slots the ROM keeps are optimised away | a `volatile` local (`volatile` before the type) | SM64DS |
| Same compare appears twice where you emit one | two independent `if` blocks re-testing the same expression | SM64DS |
| Two-leaf `add` operands swapped, every respelling reloads | name the leaf the ROM does not re-load (`s32 e = ...;` right before use) | SM64DS |
| `ldm`/`stm` burst | only equal-width block (struct) moves produce it; a 3-in/4-out burst is not compiler output | SM64DS |
| Whole TU has an idiom nothing else produces | other compiler build or prebuilt library; check `compiler_sweep.py` before grinding | SM64DS (our splits: sp2 vs sp3 Thumb switch tables, see 0004) |

## 2. Control flow (transfer, mostly SM64DS)

- Derive loops from branch targets: bottom test = `do {} while`; top test = `while`;
  find-first = `for (;;) { if (..) break; }`.
- Same-variable else-if chains that lose skip branches become a `switch`; case block
  order follows source label order; dense jump tables need the low cases present.
  (Rush Thumb switch tables: `ldrh; add r0,pc; bx r0` only on sp2 — see 0008/0004.)
- ARM predication: the compiler will not predicate an arm whose skipped region
  contains another conditional region; budget ~7 instructions.
- A materialised bool inside a condition chain (`cmp; moveq 1; movne 0; cmp; beq`)
  suggests a named int (`int t = (f == k); if (t)`) — in SM64DS it implied C++.

## 3. Pragmas and traps

- Optimiser pragmas are per function. Useful ones: `opt_strength_reduction off`,
  `opt_common_subs off`, `opt_loop_invariants off`, `opt_propagation off`,
  `opt_dead_assignments off`, `optimize_for_size on`. Each can rotate colouring;
  remove a pragma crutch once the shape matches and re-check.
- mwccarm **silently ignores unknown pragmas**: a typo reads as "inert".
- match.py appends `-Cpp_exceptions off`; never compare C++ with exceptions on.
- Hand-written SDK asm primitives are matched as `asm` functions (see
  src/main_sdk2/DC_StoreRange.c); guard functions with early returns are plain C.

## 4. Not applicable / not yet relevant here

C++-only levers (real virtual calls vs C function-pointer casts, PMF ABI, by-value
class params, declared-destructor frame slots, placement new) — Rush code is C so
far. Build-delta levers specific to `2004/b56` (member RMW materialisation, register
recycling, `tst` vs `ands`) — re-measure on 1.2/sp2 before relying on them.

## Probe sources

`/tmp`-style probes compiled with the project flags; reproduce by compiling a
one-function file with `python3 tools/matching/match.py` or the compiler directly
and disassembling with capstone.

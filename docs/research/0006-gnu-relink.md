# 0006 — Independent relink check with GNU ld

**Question.** Are dsd's relocations (relocs.txt) correct, i.e. do the delinked
objects reproduce the original bytes when linked at their addresses?
**Why.** The matching build links delinked objects with mwldarm. A wrong
relocation silently corrupts a function once its neighbours move. We want a
check that runs without proprietary tools.
**Tool.** `tools/matching/gnu_relink.py` (`ninja verify`).
**Confidence.** High — byte comparison of every non-BSS section.

## Method

1. Copy each `build/<ver>/delinks/*.o` and normalise it:
   - `objcopy` — rewrites `.symtab` in canonical order. dsd 0.12.1 writes 90
     GLOBAL symbols before `sh_info` (first non-local index); GNU ld treats
     them as local, so undefined ones (e.g. `ARM9_CTOR_START`) never resolve.
   - set bit 0 of Thumb `STT_FUNC` symbol values (Thumb-ness is otherwise only
     in `$t` mapping symbols; without bit 0 GNU ld assumes ARM and inserts
     interworking veneers);
   - retype ARM-branch `R_ARM_PC24` to `R_ARM_CALL` (bl/blx) or
     `R_ARM_JUMP24` (b, conditional), so `--use-blx` reproduces `blx`;
   - zero the in-place word of RELA `R_ARM_ABS32` relocations (dsd leaves the
     original value for link-time constants; GNU ld added it to the addend,
     doubling `ARM9_CTOR_START`).
2. Generate a linker script with one output section per module section at its
   original address (overlays overlap; `--no-check-sections`) and define dsd
   link-time constants (`ARM9_CTOR_START`, `__DTCM_LO`, `__ITCM_HI`,
   `__OVERLAY_COUNT`).
3. Link with `arm-none-eabi-ld --use-blx` and compare each section with the
   extracted module.

## Results (2026-11-15)

| Stage | Result |
| ----- | ------ |
| raw objects | link error: `ARM9_CTOR_START` undefined (symtab order) |
| + objcopy | 15,000+ bytes differ: veneers instead of `blx` (main .text 9,679 B, ov000 1,689 B, ov001 3,353 B) |
| + Thumb bit + reloc retyping | 4 bytes differ (`ARM9_CTOR_START` doubled) |
| + ABS32 in-place zeroing | **all 11 sections match** |
| after dsd jump-table patch (0008), +1,850 functions | **all 11 sections match** |

## Limits

- Proves relocations are self-consistent with the bytes, not that symbol
  *boundaries* are right (a missing function only means missing relocations,
  which still relinks identically while nothing moves).
- mwldarm decides real ordering/padding; this is a regression guard.

## Upstream candidates (dsd)

- symtab ordering (`sh_info`), Thumb bit on FUNC symbols, R_ARM_CALL use,
  zeroed in-place addend for link-time constants.

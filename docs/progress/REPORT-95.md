# 95% milestone report: Sonic Rush (EUR `ASCP`) matching decompilation

Date: 2026-11-16 (verified checkpoint `853dfba` on `main`).

> Historical snapshot at 95 %. Current figures: [dashboard](dashboard.md) and
> STATUS.md. The structural blockers listed below were later resolved by
> writing those functions as assembly in `src/asm/`.

## Verified result

| module | functions | matched | code bytes | matched bytes | % |
|---|---|---|---|---|---|
| itcm  |   14 |   14 |     2,064 |     2,064 | 100.0 |
| main  | 2,855 | 2,779 |   449,566 |   412,734 | 91.8 |
| ov000 | 2,799 | 2,764 |   642,724 |   628,140 | 97.7 |
| ov001 | 1,723 | 1,692 |   328,118 |   313,880 | 95.7 |
| **total** | **7,391** | **7,249** | **1,422,472** | **1,356,818** | **95.38** |

A function counts only when **all** of these hold (none relaxed at any point):

1. `tools/matching/match.py` prints `OK`: the C (or `asm`) compiled with
   CodeWarrior for NDS 1.2/sp2 equals the ROM bytes, with only relocation fields masked.
2. It is integrated as its own TU in `config/eur/arm9/**/delinks.txt`.
3. The full ROM rebuilds with SHA-1 `52e5024efcf7617a820fa23789b37e5168c3c4fc`.
4. The independent GNU-ld relink check passes: every section of main, ITCM, DTCM,
   ov000 and ov001 has 0 differing bytes (`build/eur/reports/gnu_relink.json`).
5. objdiff reports the function at 100%.

Function total dropped from 7,392 to 7,391 because one bogus 4-byte dsd split
(`func_0202d4e8`, the trailing `bx lr` of `func_0202d49c`) was merged back into
its parent.

## What is left (65,690 bytes, 143 functions)

- 8 functions (2.9 KB) already matched by agents and waiting for the next
  checkpoint to integrate them.
- Large, spill-heavy functions nobody has matched: `func_020648c4` (3,424 B),
  `func_0206aa38`, `func_02032480`, `func_0205e378`, `func_0201ed04`,
  `func_ov001_022fde54`, `func_0204bf88`.
- Known structural blockers:
  - `func_0202a3e8` (MSL `__pformatter`): the switch jump-table density rule
    differs from every CW 1.2 build.
  - `FS_StartOverlay`: needs two symbols at one address.
  - `func_02006990` and `func_02005e64`: linker constants with no relocations.
  - `func_022f8224` (ov000): the switch split point.
- Near-misses that differ by 2–20 bytes, mostly register or stack-slot
  colouring. Drafts are kept in the agents' scratch directories; the task-board
  notes list them.

## How it got here

- **Toolchain.** dsd, objdiff and wibo were all built from source. CW 1.2/sp2
  was identified: it is the only build that reproduces the Thumb switch tables.
  The `ninja` pipeline goes from the ROM through extract, delink, LCF and
  mwldarm to the ROM build, then runs the SHA-1 check, the relink check and the
  objdiff report.
- **Process.** A single writer integrates; up to 8 parallel matching agents work
  on address partitions recorded in `state/tasks.json`. `checkpoint.sh` runs
  integrate, stale-name fixing, configure, ninja and the SHA-1 check, then
  repairs any failure by mapping ROM diffs to TUs and ejecting them
  (`tools/matching/eject_diffs.py`, falling back to bisection). After that it
  runs the relink check, the report and the dashboard, then commits and pushes.
- **Ghidra 12.2** (official release, JDK 25). Headless import applies the dsd
  symbols and Thumb modes; per-function decompiler drafts go to
  `build/eur/ghidra/<module>/decomp/`. Agents used them for control flow and
  struct offsets.
- **Codegen levers that unlocked the last 20 %** (all in the agent reports under
  `build/agents/`):
  - **File-level pragmas, with no reset.** `opt_common_subs off` fixes register
    swaps such as w/i. `opt_lifetimes off` fixes loop index/pointer swaps.
    `opt_propagation off` keeps a global's base address in a callee-saved
    register. `opt_strength_reduction off`, `opt_loop_invariants off` and
    `opt_dead_assignments off` each fixed specific shapes.
  - **Ordering searches.** Declaration-order permutation (pairwise swaps, not
    just moves), statement-order search, and if/else arm order.
  - **Hiding addresses from the compiler.** `(T*)(u32)&global` or
    `(void*)&global` stops alias analysis and copy propagation.
  - **Missing callee arguments.** When the target leaves r1/r2 live into a call,
    the callee takes extra arguments.
  - **Runtime helpers called explicitly.** Float compares go through
    `func_0202e5e4`/`e564`/`e528`, and divides through `func_02002094`, wherever
    the ROM calls those directly.
  - **Expression forms.** `(s16)((a*T)>>12)` for sin/cos, `int` rather than
    `s32` loop counters, and inline SDK helpers such as `FX_Mul` and
    `NNS_G3dGlb*`.
  - **Relocation addends are accepted.** A reloc written as `data_X+N` that
    lands on a dsd-split symbol is counted at 100% by objdiff.

## Integrity

No ROM data, extracted assets, asm listings, Ghidra output or Metrowerks files are
committed. Every commit on `main` is a SHA-1-verified build. Work in progress is
mirrored separately to the branch `wip/mirror`.

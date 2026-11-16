# Agent brief — Sonic Rush matching decompilation (shared by all matching agents)

Style: terse caveman reports (answer first, no fluff, keep every technical fact).
Principle: forward implementation first — produce verified matching C, not bookkeeping.

## Setup (already done; do not reinstall)
- Repo: /home/claude/sonic-rush-decomp  (cwd for all commands)
- `export PATH=/home/claude/bin:$PATH` before every command (dsd, wibo, objdiff-cli).
- Compiler: CodeWarrior for NDS 1.2/sp2 (default in config/eur/compilers.yaml; sp3/sp4 are WRONG for non-leaf ARM code; only sp2 reproduces Thumb switch tables).
- ROM extracted under extract/eur (read-only). Function table: config/eur/arm9/**/symbols.txt (read-only).

## Your loop
1. Pick functions in YOUR range (given in your task). Prefer small/leaf first; `build/eur/reports/callgraph.csv`
   lists module,address,name,mode,size,callers,callees.
2. Disassemble the target, e.g.
   `python3 -c "import capstone;d=open('extract/eur/arm9/arm9.bin','rb').read();a=0x0201bf1c;n=24;md=capstone.Cs(capstone.CS_ARCH_ARM,capstone.CS_MODE_ARM);[print(hex(i.address),i.mnemonic,i.op_str) for i in md.disasm(d[a-0x02000000:a-0x02000000+n],a)]"`
   Overlays: extract/eur/arm9_overlays/ov000.bin / ov001.bin, base 0x022C5C80. Thumb: CS_MODE_THUMB.
   ITCM: extract/eur/arm9/itcm.bin base 0x01FF8000.
3. Write C to `src/<YOUR_DIR>/<symbol>.c` — ONE function per file, function named exactly as in symbols.txt
   (e.g. func_0201bf1c, func_ov000_022c5c80, or an SDK name like OS_LockMutex if already named).
   - `#include <nitro/types.h>` (u8/u16/u32/s8/s16/s32/fx32/BOOL, include/nitro/types.h).
   - Called functions: declare prototypes with their symbols.txt names. Globals: `extern <type> data_XXXXXXXX;`
     using the symbols.txt name of the address loaded from the literal pool (pool word = address).
   - Thumb functions: `#pragma thumb on` before, `#pragma thumb reset` after.
   - Struct layouts: name them UnkStruct_<addr>; only what the function needs.
4. Verify: `python3 tools/matching/match.py src/<YOUR_DIR>/<symbol>.c`
   Exit 0 + line starting `OK` = verified match (relocated fields masked, length and all other bytes equal).
   On DIFF it prints an aligned disassembly diff. Iterate (reorder statements, change types/signedness,
   temporaries, loop forms, `-O4,s` via `--flags "-O4,s -proc arm946e -enum int -char signed -gccext,on -fp soft -interworking -sym on -gccinc -nolink -msgstyle gcc -w off -lang c99"`).
   If it only matches with non-default flags, put the flags on the first line as `// flags: ...` and report it.
5. Give up on a function after ~6 failed attempts; delete its file (never leave non-matching files in src/).
6. Hand-written asm routines (mrs/msr/mcr/swi/clz/stm loops, soft-float/64-bit helpers) ARE in scope now:
   write them as CodeWarrior `asm` functions, e.g.
       asm void OS_Halt(void) {
           mov r0, #0
           mcr p15, 0, r0, c7, c0, 4
           bx lr
       }
   (Thumb: `#pragma thumb on` around it; labels inside are plain `@1:` style local labels or names.)
   They must still pass match.py.
7. PRIORITY IS BYTES: progress is measured in matched code bytes. After the trivial ones, go for
   medium/large functions (100–1000 bytes). One 600-byte function = 50 tiny ones.
8. Helpers available: include/nitro/fs_overlay.h (FS_OVERLAY_ID(ov000)), link-time symbol ARM9_CTOR_START,
   compiler intrinsics like _s32_div_f are emitted automatically by `/` and `%`.
   Never define (non-extern) global data in a TU; only `extern`. No string literals / float constants that
   would create .rodata/.data in the TU unless the target function's own pool holds them inline.

## Ghidra drafts (reading aid)
Ghidra 12.2 decompiler output for every function (with dsd names/Thumb modes applied) is at
`build/eur/ghidra/<module>/decomp/<symbol>.c` (module = main | ov000 | ov001), plus `functions.csv`,
`xrefs.csv` (callers/callees), `strings.csv` in `build/eur/ghidra/<module>/`. Use it as a starting draft
for control flow, struct offsets and call arguments; rewrite into idiomatic C that matches. Never paste it
verbatim as final source and never commit anything from build/eur/ghidra/.

## Hard rules
- Never delete or move a file that is outside src/<YOUR_DIR>/; if a function already has a file elsewhere under src/, skip it (do not delete your copy after it may have been integrated — leave duplicates for the lead).
- Do NOT edit anything under config/, tools/, docs/, include/ (except you MAY add new headers under
  include/<YOUR_DIR>/ if needed), and do NOT run git, ninja, or dsd delink. The lead integrates.
- Only files under src/<YOUR_DIR>/ that pass match.py may remain at the end.
- Do not copy code from reference projects (pret/pokediamond has no licence). Write your own C.
- Never claim a match without match.py printing OK.

## Report (also write it to build/agents/<YOUR_DIR>.txt)
One line per verified file: `OK <symbol> <src path>`; then `asm <symbol>` lines; then counts.

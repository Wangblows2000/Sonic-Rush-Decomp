# Contributing

## Ground rules

1. **Never commit game data.** No ROMs, BIOS dumps, extracted files, assembly
   listings, Ghidra databases or decompiler output. `.gitignore` covers the
   usual paths; check `git status` before committing.
2. **A function is "matched" only when its compiled bytes equal the original**,
   as reported by objdiff/`ninja`. Renaming, commenting or documenting a
   function is not progress and is never counted as such.
3. **Names need evidence.** Any non-default symbol name gets an entry in
   `config/<ver>/symbol_evidence.yaml` with a confidence level:
   `confirmed`, `strong` or `tentative`. Default dsd names (`func_0200abcd`)
   mean "unknown".
4. **Ghidra pseudocode is a reading aid, not source.** Do not paste it.
5. Reference projects (including Sonic Rush Adventure) are sources of
   engineering knowledge. Their code may suggest how a shared-engine function
   looks; it counts only after it matches Sonic Rush's bytes.

## Workflow (dsd)

1. Pick a function or small translation unit (`python tools/project.py functions`
   writes `build/eur/reports/functions.csv`). Prefer small leaf functions and
   known SDK code first (PLAN.md, milestone 6).
2. Add a translation unit to the module's `delinks.txt`:

   ```
   src/sdk/os/os_spinlock.c:
       .text start:0x02012340 end:0x020123f0
   ```

3. `ninja delink` regenerates objects; `ninja verify` must still pass (the gap
   objects plus the new TU object must relink to the original bytes).
4. Write `src/...c`, build with `ninja`, and compare in objdiff
   (`ninja objdiff` writes `objdiff.json`).
5. When it matches, mark the unit complete and record any naming evidence.

## Style

- C (C99) for game code unless evidence shows C++ (vtables, mangled names).
- Types from `include/nitro/types.h` (`u8`, `s32`, `fx32`, …).
- Names: `PascalCase_Function` for SDK-style APIs (match the NitroSDK names),
  `camelCase` locals; game names follow evidence, not invention.
- Python tools: standard library + `tools/requirements.txt`; deterministic
  output; no network access at build time.

## Commits

Small and descriptive (`ov001: match func_ov001_022c5c80`,
`tools: detect unsigned Thumb jump tables`). Run `ninja verify` before pushing.

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

## Workflow

Almost all functions are already matched (STATUS.md). Open work is listed in
PLAN.md: converting `src/asm_fallback/` back to C, naming, source
reorganisation and data. For a function:

1. Write C in `src/<your-dir>/<symbol>.c`, one function per file, named exactly
   as in `config/eur/arm9/**/symbols.txt`. `docs/agents/AGENT_BRIEF.md` has the
   conventions (headers, `extern` data, Thumb pragmas, `asm` functions).
2. Verify: `python3 tools/matching/match.py src/<your-dir>/<symbol>.c` must
   exit 0 and print an `OK` line. The compiler is CW for NDS 1.2/sp2 with the
   default flags in `config/eur/compilers.yaml`; a per-file `// flags: ...`
   first line overrides them.
3. Integration is done by one writer only (`tools/matching/integrate.py`, run
   by `tools/utilities/checkpoint.sh`). It adds the TU to the module's
   `delinks.txt`, rebuilds, checks the ROM SHA-1 and the GNU relink
   (`ninja verify`), and refreshes the objdiff report. A function counts only
   when all of these pass (CLAUDE.md §5).
4. Record naming evidence in `config/eur/symbol_evidence.yaml`; apply renames
   with `tools/analysis/rename.py`.

## Style

- C (C99) for game code unless evidence shows C++ (vtables, mangled names).
- Types from `include/nitro/types.h` (`u8`, `s32`, `fx32`, …).
- `asm` only where the original was assembly (runtime routines) or C cannot
  match; line 1 of such a file must say why (`// asm: ...`).
- Names: `PascalCase_Function` for SDK-style APIs (match the NitroSDK names),
  `camelCase` locals; game names follow evidence, not invention.
- Python tools: standard library + `tools/requirements.txt`; deterministic
  output; no network access at build time.

## Commits

Small and descriptive (`ov001: match func_ov001_022c5c80`,
`tools: detect unsigned Thumb jump tables`). Never commit a build that fails
the ROM SHA-1; `checkpoint.sh` enforces this. Run `ninja verify` before pushing.

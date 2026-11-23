# CLAUDE.md — resume protocol (read first, every session)

Sonic Rush (EUR `ASCP`) matching decompilation. Goal: C that rebuilds the ROM
byte for byte. Progress = verified matched code bytes, nothing else.
State: 100 % of ARM9 code matched and every function named (STATUS.md);
open work in PLAN.md. Full project guide: **AGENTS.md**.
The Dreamcast port lives on the `dreamcast-port` branch (docs/dreamcast/ there).

## 1. Resume

```sh
cd <repo>
export PATH=$PWD/tools/bin:$PATH
python3 tools/tasks.py brief
```

`brief` prints verified bytes/functions, ROM SHA-1 and relink health, claimed
tasks with verified bytes per scope (and agent `OK` lines not yet in the
objdiff report), then the open tasks.

Fresh machine / fresh clone (no `build/`, no compiler):
`bash tools/bootstrap_session.sh /path/to/baserom.nds` (deps, dsd/objdiff/wibo,
Metrowerks bundle, extract, full build, verify, report). `build/` is
gitignored, so agent reports (`build/agents/`) do not survive a fresh clone.

Ghidra: `export JAVA_HOME=/usr/lib/jvm/java-25-openjdk-amd64
GHIDRA_INSTALL_DIR=/path/to/ghidra_12.2_PUBLIC; bash tools/ghidra/import.sh eur`
→ drafts in `build/eur/ghidra/<module>/decomp/` (docs/research/0007-ghidra.md).

## 2. Task board — `state/tasks.json` (committed)

```sh
python3 tools/tasks.py list [-p] [-v] [--status open|claimed|done]   # -p: verified bytes per scope
python3 tools/tasks.py claim <id> <agent>      # agent writes src/<agent>/, reports build/agents/<agent>.txt
python3 tools/tasks.py release <id> [--note ..]
python3 tools/tasks.py done <id> [--note ..]
python3 tools/tasks.py note <id> "text"
python3 tools/tasks.py add "title" [--id ID] [--kind research] [--module ov000 --start 0x.. --end 0x..]
```

Claim before starting work; release on interruption; record findings with
`note` so the next session sees them.

## 3. Agents (workers)

Brief: `docs/agents/AGENT_BRIEF.md`. Essentials: one function per file in
`src/<agent>/<symbol>.c`, named exactly as in `config/eur/arm9/**/symbols.txt`;
verify with `python3 tools/matching/match.py src/<agent>/<symbol>.c` (exit 0 +
`OK` line); delete files that do not match; report `OK <symbol> <path>` lines to
`build/agents/<agent>.txt`. Compiler: CW for NDS 1.2/sp2. Agents never run
git, ninja, configure.py, dsd delink, and never edit `config/`, `tools/`, `docs/`.

## 4. Single-writer integration (lead only)

Only one process integrates: `tools/matching/integrate.py` /
`tools/matching/swap_fallback.py`, delinks.txt edits, `tools/configure.py`,
`ninja`, `git`.

```sh
bash tools/utilities/verify_all.sh            # check_symbols, build, SHA-1, relink, report, progress
python3 tools/project.py dashboard            # docs/progress/dashboard.md, history.jsonl, README block
# Renames: python3 tools/analysis/rename.py names.csv   (CSV module,old,new,evidence; see AGENTS.md §13)
```

`tools/utilities/checkpoint.sh` and `commit_verified.sh` still stage `.claude`,
which no longer exists on `main`; their `git add` line fails there, so stage
and commit by hand after `verify_all.sh` prints `verify_all: OK`.

## 5. Verification rules (never relax)

- A function counts only when **all** hold: `match.py` printed `OK`; it is
  integrated in delinks.txt; the ROM builds with SHA-1
  `52e5024efcf7617a820fa23789b37e5168c3c4fc` (`build/eur/rush_eur.sha1.ok`
  newer than `rush_eur.nds`); `ninja verify` (GNU relink,
  `build/eur/reports/gnu_relink.json`) passes; objdiff reports it at 100 %.
- Never count agent-reported, renamed, or partially matching functions.
  `project.py progress` / `dashboard` / `tasks.py brief` read only
  `build/eur/report.json` + `symbols.txt`.
- Never commit ROM data, extracted files, asm listings, or the Metrowerks bundle.
- Never commit a build that fails SHA-1.

## 6. Where things are

STATUS.md (measured state), PLAN.md (open work), AGENTS.md (full guide),
docs/research/ (numbered findings), docs/progress/dashboard.md (latest
verified numbers, generated), `python3 tools/project.py --help`.

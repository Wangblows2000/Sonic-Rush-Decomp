# CLAUDE.md — resume protocol (read first, every session)

Sonic Rush (EUR `ASCP`) matching decompilation. Goal: C that rebuilds the ROM
byte for byte. Progress = verified matched code bytes, nothing else.

## 1. Resume

```sh
cd /home/claude/sonic-rush-decomp            # or wherever the repo is
export PATH=/home/claude/bin:$PWD/tools/bin:$PATH
python3 tools/tasks.py brief                  # also runs automatically (SessionStart hook)
```

`brief` prints: verified bytes/functions, ROM SHA-1 + relink health, next 10 %
milestone, claimed tasks with verified bytes per scope and how many of the
agent's `OK` lines are not yet in the objdiff report, then the next open tasks.

Fresh machine / fresh clone (no `build/`, no compiler):
`bash tools/bootstrap_session.sh /path/to/baserom.nds` (deps, dsd/objdiff/wibo,
Metrowerks bundle, extract, full build, verify, report). `build/` is gitignored:
agent reports and `docs/agents/AGENT_BRIEF.md` do not survive a fresh clone;
the rules below are sufficient on their own.

Ghidra: `export JAVA_HOME=/usr/lib/jvm/java-25-openjdk-amd64
GHIDRA_INSTALL_DIR=/home/claude/ghidra_12.2_PUBLIC; bash tools/ghidra/import.sh eur`
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

Partitions are module + address ranges (overlays ov000/ov001 share base
0x022C5C80, so module matters). Claim before starting work; release on
interruption; record findings with `note` so the next session sees them.

## 3. Agents (matching workers)

Brief: `docs/agents/AGENT_BRIEF.md`. Essentials: one function per file in
`src/<agent>/<symbol>.c`, named exactly as in `config/eur/arm9/**/symbols.txt`;
verify with `python3 tools/matching/match.py src/<agent>/<symbol>.c` (exit 0 +
`OK` line); delete files that do not match; report `OK <symbol> <path>` lines to
`build/agents/<agent>.txt`. Compiler: CW for NDS 1.2/sp2 (default in
`config/eur/compilers.yaml`). Prioritise bytes (medium/large functions).
Agents never run git, ninja, configure.py, dsd delink, and never edit
`config/`, `tools/`, `docs/`.

## 4. Single-writer integration (lead only)

Only one process integrates: `tools/matching/integrate.py`, delinks.txt edits,
`tools/configure.py`, `ninja`, `git`. Checkpoint (verify → build → SHA-1 →
relink → objdiff report → commit + push):

```sh
bash tools/utilities/checkpoint.sh            # prints the `total` progress line
python3 tools/project.py dashboard            # docs/progress/dashboard.md, history.jsonl, README block
git add README.md state CLAUDE.md .claude && git commit -m "..."   # checkpoint.sh only adds config include tools docs + src TUs
```

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
- Never commit a build that fails SHA-1 (checkpoint.sh enforces this).

## 6. Where things are

STATUS.md (measured state), PLAN.md (milestones), docs/research/ (findings,
numbered), docs/progress/dashboard.md (latest verified numbers),
`python3 tools/project.py --help` (doctor/analyze/extract/verify/functions/
symbols/coverage/progress/dashboard).

#!/usr/bin/env bash
# Lead integration cycle: verify agent output, build, check, commit, push.
# Prints one summary line. Never commits a ROM that fails SHA-1.
set -uo pipefail
cd "$(dirname "$0")/../.."
export PATH="$PWD/tools/bin:/home/claude/bin:$PATH"
log=build/checkpoint.log
{
  python3 tools/matching/integrate.py --no-build
  python3 tools/matching/clone_dups.py
  python3 tools/matching/integrate.py --no-build
  python3 tools/configure.py eur
  ninja && python3 tools/analysis/name_intrinsics.py
} > "$log" 2>&1
if ! ninja >> "$log" 2>&1; then
  python3 tools/analysis/name_intrinsics.py >> "$log" 2>&1
  if ! ninja >> "$log" 2>&1; then
    # fast path: blame TUs from ROM byte diffs (up to 3 rounds), then bisect
    for _ in 1 2 3; do
      python3 tools/matching/eject_diffs.py >> "$log" 2>&1 || break
      ninja >> "$log" 2>&1 && break
    done
    if ! ninja >> "$log" 2>&1; then
      python3 tools/matching/bisect_rom.py >> "$log" 2>&1 || { echo "BUILD FAILED (see $log)"; exit 1; }
    fi
  fi
fi
ninja verify >> "$log" 2>&1 || { echo "RELINK CHECK FAILED (see $log)"; exit 1; }
ninja report >> "$log" 2>&1
summary=$(python3 tools/project.py progress | grep '^total')
python3 tools/project.py dashboard >> "$log" 2>&1 || true
regs=$(python3 - <<'EOF'
import re,glob
s=set()
for p in glob.glob('config/eur/arm9/**/delinks.txt',recursive=True):
    s|=set(re.findall(r'^(src/\S+\.c):$',open(p).read(),re.M))
print('\n'.join(sorted(s)))
EOF
)
git add config include tools docs state nearmiss README.md CLAUDE.md .claude STATUS.md PLAN.md >/dev/null 2>&1
echo "$regs" | xargs -r git add >/dev/null 2>&1
if ! git diff --cached --quiet; then
  git commit -q -m "Checkpoint: ${summary}

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>
Claude-Session: https://claude.ai/code/session_01C159brNewWSmfwXnEBfLVp"
  if ! git -c push.negotiate=true push -q origin main >> "$log" 2>&1; then
    # another session pushed first: rebase our checkpoint on top and retry once
    git fetch -q --depth=50 origin main >> "$log" 2>&1 && git rebase -q origin/main >> "$log" 2>&1 \
      && git -c push.negotiate=true push -q origin main >> "$log" 2>&1 || echo "push failed (see $log)"
  fi
fi
echo "$summary"

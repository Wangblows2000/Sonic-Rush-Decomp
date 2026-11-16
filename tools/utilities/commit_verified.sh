#!/usr/bin/env bash
# Verify (relink), report, dashboard, commit + push the CURRENT build without
# integrating new TUs. Requires build/eur/rush_eur.sha1.ok newer than rush_eur.nds.
set -u; cd "$(dirname "$0")/../.."; log=build/checkpoint.log
[ build/eur/rush_eur.sha1.ok -nt build/eur/rush_eur.nds ] || { echo "SHA-1 not verified"; exit 1; }
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
git add config include tools docs state nearmiss README.md CLAUDE.md AGENTS.md .claude STATUS.md PLAN.md CONTRIBUTING.md INSTALL.md >/dev/null 2>&1
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

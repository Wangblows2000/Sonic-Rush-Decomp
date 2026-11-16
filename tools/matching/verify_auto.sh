#!/bin/bash
# Re-verify every src/auto/*.c with match.py; delete files that do not pass (exit 0 + OK line).
cd "$(dirname "$0")/../.." || exit 1
ok=0; bad=0
for f in src/auto/*.c; do
  out=$(python3 tools/matching/match.py "$f" 2>&1); rc=$?
  if [ $rc -eq 0 ] && grep -q '^OK' <<<"$out" && ! grep -q '^DIFF\|^?' <<<"$out"; then ok=$((ok+1)); else bad=$((bad+1)); echo "removed $f"; rm -f "$f"; fi
done
echo "src/auto verified: $ok ok, $bad removed"

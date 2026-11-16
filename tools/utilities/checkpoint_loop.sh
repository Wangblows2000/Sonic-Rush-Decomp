#!/usr/bin/env bash
# Run checkpoint.sh every N seconds (default 900); append each progress line to
# build/checkpoint_history.log. Run exactly one instance (single writer).
#   bash tools/utilities/checkpoint_loop.sh [seconds]
cd "$(dirname "$0")/../.." || exit 1
interval="${1:-900}"
while true; do
  line=$(bash tools/utilities/checkpoint.sh 2>&1 | tail -1)
  echo "$(date -u +%FT%TZ) $line" >> build/checkpoint_history.log
  sleep "$interval"
done

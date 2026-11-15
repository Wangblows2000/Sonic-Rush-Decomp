#!/usr/bin/env bash
# Repeat checkpoint.sh every N seconds (default 900); append summaries to build/checkpoint_history.log.
cd "$(dirname "$0")/../.."
while true; do
  echo "$(date -u +%FT%TZ) $(tools/utilities/checkpoint.sh 2>&1 | tail -1)" >> build/checkpoint_history.log
  sleep "${1:-900}"
done

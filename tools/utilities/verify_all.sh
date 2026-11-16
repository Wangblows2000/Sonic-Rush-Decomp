#!/usr/bin/env bash
# Full, repeatable verification of the current tree (no integration, no commit):
#   consistency checks -> configure -> build -> ROM SHA-1 -> GNU relink -> objdiff
#   report -> progress. Exit status is non-zero if any step fails.
#   bash tools/utilities/verify_all.sh [eur]
set -euo pipefail
cd "$(dirname "$0")/../.."
ver="${1:-eur}"
python3 tools/analysis/check_symbols.py --version "$ver"
python3 tools/configure.py "$ver"
ninja
python3 tools/utilities/sha1check.py "build/$ver/rush_$ver.nds" \
  "$(python3 -c "import yaml;print(yaml.safe_load(open('config/versions.yaml'))['$ver']['sha1'])")"
ninja verify
ninja report
python3 tools/project.py progress | grep '^total'
echo "verify_all: OK"

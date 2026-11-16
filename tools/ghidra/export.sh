#!/usr/bin/env bash
# Re-run exports (analysis CSVs + decompiler drafts) on the existing Ghidra
# project without re-importing. Usage: bash tools/ghidra/export.sh [eur]
set -euo pipefail
ver="${1:-eur}"
root="$(cd "$(dirname "$0")/../.." && pwd)"
: "${GHIDRA_INSTALL_DIR:?set GHIDRA_INSTALL_DIR}"
for prog in arm9.bin:main ov000.bin:ov000 ov001.bin:ov001; do
  file="${prog%%:*}"; name="${prog##*:}"
  "$GHIDRA_INSTALL_DIR/support/analyzeHeadless" "$root/ghidra/$ver" "rush_$ver" \
    -process "$file" -noanalysis -readOnly -scriptPath "$root/tools/ghidra" \
    -postScript ExportAnalysis.java "$root/build/$ver/ghidra/$name" \
    -postScript ExportDecomp.java "$root/build/$ver/ghidra/$name/decomp" 30
done

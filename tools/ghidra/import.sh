#!/usr/bin/env bash
# Headless Ghidra import of every Sonic Rush ARM9 module.
#
#   GHIDRA_INSTALL_DIR=/opt/ghidra bash tools/ghidra/import.sh eur
#
# One Ghidra program per module. Overlays 0 and 1 share their load address
# (0x022C5C80), so each overlay program also gets main RAM (ARM9 static),
# ITCM and DTCM as extra blocks to keep cross-module references resolvable.
# Language ARM:LE:32:v5t matches the ARM946E-S (ARMv5TE; Ghidra has no
# separate v5te variant, v5t decodes the same instruction set used here).
#
# Output: ghidra/<ver>/ (project, gitignored) and build/<ver>/ghidra/<module>/
# (functions.csv, xrefs.csv, strings.csv, decomp/<symbol>.c drafts).
# Tested with Ghidra 12.2 + OpenJDK 25 (see docs/research/0007-ghidra.md).
set -euo pipefail
ver="${1:-eur}"
root="$(cd "$(dirname "$0")/../.." && pwd)"
: "${GHIDRA_INSTALL_DIR:?set GHIDRA_INSTALL_DIR}"
ah="$GHIDRA_INSTALL_DIR/support/analyzeHeadless"
ex="$root/extract/$ver"
cfg="$root/config/$ver/arm9"
proj="$root/ghidra/$ver"
mkdir -p "$proj"

main_bin="$ex/arm9/arm9.bin"
itcm="itcm=$ex/arm9/itcm.bin@0x01ff8000"
dtcm="dtcm=$ex/arm9/dtcm.bin@0x027e0000"

import_module() {  # name file base symbols extra-blocks...
  local name="$1" file="$2" base="$3" syms="$4"; shift 4
  "$ah" "$proj" "rush_$ver" -overwrite \
    -import "$file" -loader BinaryLoader -loader-baseAddr "$base" \
    -loader-blockName "$name" -processor ARM:LE:32:v5t -cspec default \
    -scriptPath "$root/tools/ghidra" \
    -preScript ApplyDsdSymbols.java "$syms" "$@" \
    -postScript ExportAnalysis.java "$root/build/$ver/ghidra/$name" \
    -postScript ExportDecomp.java "$root/build/$ver/ghidra/$name/decomp" 30
}

import_module main "$main_bin" 0x02000000 "$cfg/symbols.txt" "$itcm" "$dtcm"
for ov in "$ex"/arm9_overlays/ov*.bin; do
  id="$(basename "$ov" .bin)"
  import_module "$id" "$ov" 0x022c5c80 "$cfg/overlays/$id/symbols.txt" \
    "main=$main_bin@0x02000000" "$itcm" "$dtcm"
done
echo "done: project in $proj, exports in build/$ver/ghidra/"

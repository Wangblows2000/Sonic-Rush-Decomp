#!/usr/bin/env bash
# Rebuild the full working environment in a fresh Linux session.
#
#   bash tools/bootstrap_session.sh /path/to/baserom.nds
#
# 1. system packages + Python deps  2. dsd/objdiff/wibo from pinned sources
# 3. Metrowerks bundle (the one zeldaret/ph downloads; proprietary, never
#    committed, used on the project owner's instruction — docs/research/0004)
# 4. ROM extract + full matching build + checks.
set -euo pipefail
root="$(cd "$(dirname "$0")/.." && pwd)"
rom="${1:-}"
cd "$root"

need=()
for b in ninja cmake arm-none-eabi-ld clang ld.lld; do command -v "$b" >/dev/null || need+=("$b"); done
if [ ${#need[@]} -gt 0 ]; then
  sudo_cmd=""; [ "$(id -u)" -ne 0 ] && sudo_cmd="sudo"
  $sudo_cmd apt-get update -q && $sudo_cmd apt-get install -y -q ninja-build cmake binutils-arm-none-eabi clang lld libclang-dev
fi
python3 -m pip install -q -r tools/requirements.txt --break-system-packages 2>/dev/null || python3 -m pip install -q -r tools/requirements.txt

[ -x tools/bin/dsd ] && [ -x tools/bin/objdiff-cli ] && [ -x tools/bin/wibo ] || bash tools/setup_tools.sh
export PATH="$root/tools/bin:$PATH"

MW_SHA256=6f7b16098d2b1bc9b45b278f6e6e24327c4da9b3ebf7924be1afd4a721d83976
if [ ! -f tools/mwccarm/1.2/sp3/mwccarm.exe ]; then
  tmp="$(mktemp -d)"
  curl -sSL --max-time 600 -o "$tmp/mwccarm.zip" https://decomp.aetias.com/files/mwccarm.zip
  echo "$MW_SHA256  $tmp/mwccarm.zip" | sha256sum -c -
  unzip -q -o "$tmp/mwccarm.zip" -d tools/
  rm -rf "$tmp"
fi

if [ -n "$rom" ]; then ln -sf "$(realpath "$rom")" extract/baserom_rush_eur.nds; fi
python3 tools/project.py extract eur
python3 tools/configure.py eur
ninja
ninja verify
ninja report
python3 tools/project.py progress

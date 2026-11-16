#!/usr/bin/env bash
# One-command local ROM build for Linux.
#
#   1. Put your own Sonic Rush (Europe, ASCP) ROM in this directory (any *.nds name).
#   2. Run:  bash build.sh            (or: bash build.sh path/to/rom.nds)
#
# The script checks the ROM is the expected EUR revision (SHA-1), installs the
# toolchain on first run if it is missing (tools/bootstrap_session.sh: apt
# packages, dsd/objdiff/wibo from source, the CodeWarrior bundle), extracts the
# ROM, compiles every translation unit, links, rebuilds the ROM and verifies that
# the result is byte-identical to the original. Everything after the one-time
# toolchain setup runs locally. Output: ./sonic_rush_eur_rebuilt.nds
#
# Options:  --verify   also run the independent GNU relink check and objdiff report
set -euo pipefail
cd "$(dirname "$0")"
root="$(pwd)"
want_sha1=52e5024efcf7617a820fa23789b37e5168c3c4fc
verify=0
rom=""
for a in "$@"; do
  case "$a" in
    --verify) verify=1 ;;
    *) rom="$a" ;;
  esac
done

if [ -z "$rom" ]; then
  mapfile -t found < <(find "$root" -maxdepth 1 -iname '*.nds' ! -name 'sonic_rush_eur_rebuilt.nds' | sort)
  [ ${#found[@]} -gt 0 ] || { echo "error: no .nds ROM found in $root (copy your Sonic Rush EUR ROM here)"; exit 1; }
  rom="${found[0]}"
fi
[ -f "$rom" ] || { echo "error: $rom not found"; exit 1; }

echo "== checking $rom"
got=$(sha1sum "$rom" | cut -d' ' -f1)
if [ "$got" != "$want_sha1" ]; then
  echo "error: SHA-1 $got does not match Sonic Rush (Europe) ASCP rev 0 ($want_sha1)."
  echo "       Other revisions/regions are not supported (never substitute a different ROM)."
  exit 1
fi

mkdir -p extract
cp -f "$rom" extract/baserom_rush_eur.nds

export PATH="$root/tools/bin:$PATH"
export WITH_GHIDRA=0   # Ghidra is a research tool, not needed to build

[ -f "$HOME/.cargo/env" ] && . "$HOME/.cargo/env"
missing=()
for b in git curl unzip python3 cargo; do command -v "$b" >/dev/null || missing+=("$b"); done
if [ ${#missing[@]} -gt 0 ] && [ ! -x tools/bin/dsd ]; then
  echo "build.sh: missing prerequisites: ${missing[*]}" >&2
  echo "  Debian/Ubuntu: sudo apt install git curl unzip python3 python3-pip; Rust >= 1.88 from https://rustup.rs (distro rustc may be too old)" >&2
  exit 1
fi
if [ ! -x tools/bin/dsd ] || [ ! -x tools/bin/wibo ] || [ ! -x tools/bin/objdiff-cli ] || [ ! -f tools/mwccarm/1.2/sp2/mwccarm.exe ]; then
  echo "== first run: installing toolchain (tools/bootstrap_session.sh)"
  bash tools/bootstrap_session.sh "$root/extract/baserom_rush_eur.nds"
fi

echo "== building"
python3 tools/configure.py eur
ninja

python3 tools/utilities/sha1check.py build/eur/rush_eur.nds "$want_sha1"
cp -f build/eur/rush_eur.nds sonic_rush_eur_rebuilt.nds

if [ "$verify" = 1 ]; then
  echo "== independent verification"
  bash tools/utilities/verify_all.sh eur
fi
echo "== done: sonic_rush_eur_rebuilt.nds is byte-identical to the original ($want_sha1)"

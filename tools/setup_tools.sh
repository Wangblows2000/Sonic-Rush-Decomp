#!/usr/bin/env bash
# Build the open-source toolchain from pinned sources into tools/bin/.
#
#   bash tools/setup_tools.sh
#
# Requires: git, cargo (Rust >= 1.88, from rustup; Debian 13's rustc 1.85 is too old), cmake, ninja, a C++ compiler, clang/libclang
# (wibo), lld (wibo on binutils < 2.43). Tested on Ubuntu 24.04.
#
# dsd is patched with tools/patches/*.patch: unsigned Thumb jump tables used by
# Sonic Rush's compiler (docs/research/0008-dsd-thumb-jumptables.md) and asm
# entry-point fixes for hand-written assembly TUs (conditional first
# instruction, conditional ARM branches, $d for data inside .text).
# The Metrowerks compiler is NOT handled here: it is proprietary (INSTALL.md).
set -euo pipefail
root="$(cd "$(dirname "$0")/.." && pwd)"
src="$root/tools/src"; bin="$root/tools/bin"
mkdir -p "$src" "$bin"

[ -f "$HOME/.cargo/env" ] && . "$HOME/.cargo/env"   # prefer rustup's toolchain over the distro's
rv="$(rustc --version 2>/dev/null | awk '{print $2}')"
if [ -z "$rv" ] || [ "$(printf '%s\n' 1.88.0 "$rv" | sort -V | head -1)" != 1.88.0 ]; then
  echo "ERROR: Rust >= 1.88 required (found: ${rv:-none})." >&2
  echo "  Install rustup:  curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh" >&2
  echo "  then:            . \"\$HOME/.cargo/env\" && rustup default stable" >&2
  exit 1
fi

DSD_TAG=v0.12.1
OBJDIFF_TAG=v3.8.3
WIBO_TAG=1.2.0

clone() { [ -d "$src/$2" ] || git clone -q --depth 1 --branch "$3" "https://github.com/$1" "$src/$2"; }

clone AetiasHax/ds-decomp ds-decomp "$DSD_TAG"
( cd "$src/ds-decomp"
  for p in "$root"/tools/patches/ds-decomp-"$DSD_TAG"-*.patch; do
    if git apply --check "$p" 2>/dev/null; then git apply "$p"
    elif git apply --reverse --check "$p" 2>/dev/null; then echo "patch already applied: $(basename "$p")"
    else echo "ERROR: patch does not apply: $(basename "$p")" >&2; exit 1; fi
  done
  cargo build --locked --release -p ds-decomp-cli )
cp "$src/ds-decomp/target/release/dsd" "$bin/dsd"

clone encounter/objdiff objdiff "$OBJDIFF_TAG"
( cd "$src/objdiff" && cargo build --locked --release -p objdiff-cli )
cp "$src/objdiff/target/release/objdiff-cli" "$bin/objdiff-cli"

if [ "$(uname -s)" = Linux ]; then
  clone decompals/wibo wibo "$WIBO_TAG"
  libclang="$(ls /usr/lib/llvm-*/lib/libclang.so.1 2>/dev/null | sort -V | tail -1 || true)"
  ( cd "$src/wibo"
    cmake --preset release64 ${libclang:+-DLIBCLANG_LIBRARY=$libclang} -DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=lld
    cmake --build --preset release64 )
  cp "$src/wibo/build/release64/wibo" "$bin/wibo"
fi
"$bin/dsd" --version
"$bin/objdiff-cli" --version
echo "tools installed in $bin"

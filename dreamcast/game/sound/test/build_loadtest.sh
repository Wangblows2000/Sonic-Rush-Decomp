#!/bin/sh
# Flycast test disc for the loading bracket (test/dc_loadtest.c).
#   sh dreamcast/game/sound/test/build_loadtest.sh [OUTDIR]   -> OUTDIR/loadtest.cdi
# Needs build/dreamcast/audio/sfxbank/z1s.srb, music/z1s/SEQ_1sonic.wav and a
# 2 MB stage file as load payload. Converted data from YOUR ROM: do not redistribute.
set -e
HERE=$(cd "$(dirname "$0")/.." && pwd)
ROOT=$(cd "$HERE/../../.." && pwd)
OUT=${1:-$ROOT/build/dreamcast/audio/loadtest}
. /opt/toolchains/dc/kos/environ.sh
rm -rf "$OUT/disc" "$OUT/IP.BIN"; mkdir -p "$OUT/disc/sound" "$OUT/disc/music" "$OUT/disc/load"
cd "$HERE"
kos-cc -std=gnu99 -O2 -fwrapv -Wall -Wextra -Werror -o "$OUT/loadtest.elf" \
    test/dc_loadtest.c snd_bank.c snd_drv.c snd_nns.c sound.c snd_out_kos.c ../engine/audio.c -lm
sh-elf-objcopy -R .stack -O binary "$OUT/loadtest.elf" "$OUT/loadtest.bin"
"$KOS_BASE/utils/scramble/scramble" "$OUT/loadtest.bin" "$OUT/disc/1ST_READ.BIN"
cp "$ROOT/build/dreamcast/audio/sfxbank/z1s.srb" "$OUT/disc/sound/"
cp "$ROOT/build/dreamcast/audio/music/z1s/SEQ_1sonic.wav" "$OUT/disc/music/z1.wav"
cp "$ROOT/build/dreamcast/stages/z11/blocktex.bin" "$OUT/disc/load/blob.bin"
cd "$OUT"
"$KOS_BASE/utils/makeip/makeip" -g "SRDC LOADTEST" -c "PORT" "$OUT/IP.BIN" >/dev/null
genisoimage -quiet -C 0,11702 -V LOADTEST -G IP.BIN -r -J -l -o "$OUT/loadtest.iso" "$OUT/disc"
${CDI4DC:-/home/claude/dcwork/bin/cdi4dc} "$OUT/loadtest.iso" "$OUT/loadtest.cdi" >/dev/null
rm -rf "$OUT/disc" "$OUT/loadtest.iso" "$OUT/loadtest.bin" "$OUT/IP.BIN"
echo "built $OUT/loadtest.cdi"

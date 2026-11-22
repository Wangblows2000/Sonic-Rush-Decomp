#!/bin/sh
# Flycast test disc for the native sound runtime (test/dc_sndtest.c + z1s bank).
#   sh dreamcast/game/sound/test/build_cdi.sh   -> build/dreamcast/audio/sfxbank/sndtest.cdi
# Needs build/dreamcast/audio/sfxbank/z1s.srb (tools/dreamcast/audio/sfxbank.py).
# The disc holds converted game data from YOUR ROM: do not redistribute.
set -e
HERE=$(cd "$(dirname "$0")/.." && pwd)
ROOT=$(cd "$HERE/../../.." && pwd)
OUT=$ROOT/build/dreamcast/audio/sfxbank/cdi
. /opt/toolchains/dc/kos/environ.sh
rm -rf "$OUT"; mkdir -p "$OUT/disc/sound" "$OUT/disc/music"
cd "$HERE"
kos-cc -std=gnu99 -O2 -fwrapv -Wall -Wextra -Werror -o "$OUT/sndtest.elf" \
    test/dc_sndtest.c snd_bank.c snd_drv.c snd_nns.c sound.c snd_out_kos.c ../engine/audio.c -lm
sh-elf-objcopy -R .stack -O binary "$OUT/sndtest.elf" "$OUT/sndtest.bin"
"$KOS_BASE/utils/scramble/scramble" "$OUT/sndtest.bin" "$OUT/disc/1ST_READ.BIN"
cp "$ROOT/build/dreamcast/audio/sfxbank/z1s.srb" "$OUT/disc/sound/"
M=$ROOT/build/dreamcast/audio/music/z1s/SEQ_1sonic.wav
[ -f "$M" ] && cp "$M" "$OUT/disc/music/z1.wav"
cd "$OUT"
"$KOS_BASE/utils/makeip/makeip" -g "SRDC SNDTEST" -c "PORT" "$OUT/IP.BIN" >/dev/null
genisoimage -quiet -C 0,11702 -V SNDTEST -G IP.BIN -r -J -l -o "$OUT/sndtest.iso" "$OUT/disc"
${CDI4DC:-/home/claude/dcwork/bin/cdi4dc} "$OUT/sndtest.iso" "$ROOT/build/dreamcast/audio/sfxbank/sndtest.cdi" >/dev/null
echo "built $ROOT/build/dreamcast/audio/sfxbank/sndtest.cdi"

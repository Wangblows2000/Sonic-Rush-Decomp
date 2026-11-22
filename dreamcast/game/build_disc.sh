#!/bin/sh
# Build the Sonic Rush DC game disc from the owner's converted assets.
#   1. tools/dreamcast/stages/stage2dc.py --all        (needs extract/eur from YOUR ROM)
#   2. sh dreamcast/game/build_disc.sh                  -> build/dreamcast/game/srdc.cdi
# The disc contains converted game data and must not be redistributed.
set -e
HERE=$(cd "$(dirname "$0")" && pwd)
ROOT=$(cd "$HERE/../.." && pwd)
OUT=$ROOT/build/dreamcast/game
. /opt/toolchains/dc/kos/environ.sh
make -C "$HERE"
rm -rf "$OUT/disc"; mkdir -p "$OUT/disc/stages"
sh-elf-objcopy -R .stack -O binary "$HERE/srdc.elf" "$OUT/srdc.bin"
"$KOS_BASE/utils/scramble/scramble" "$OUT/srdc.bin" "$OUT/disc/1ST_READ.BIN"
for d in "$ROOT"/build/dreamcast/stages/z*/; do
    id=$(basename "$d")
    [ -f "$d/level.bin" ] || continue
    mkdir -p "$OUT/disc/stages/$id"
    cp "$d/level.bin" "$d/blocktex.bin" "$OUT/disc/stages/$id/"
    [ -f "$d/bg.bin" ] && cp "$d/bg.bin" "$OUT/disc/stages/$id/"
done
cd "$OUT"
rm -f IP.BIN
"$KOS_BASE/utils/makeip/makeip" -g "SONIC RUSH DC" -c "PORT" "$OUT/IP.BIN" >/dev/null
genisoimage -quiet -C 0,11702 -V SRDC -G IP.BIN -r -J -l -o "$OUT/srdc.iso" "$OUT/disc"
${CDI4DC:-cdi4dc} "$OUT/srdc.iso" "$OUT/srdc.cdi" >/dev/null
echo "built $OUT/srdc.cdi ($(du -h "$OUT/srdc.cdi" | cut -f1))"

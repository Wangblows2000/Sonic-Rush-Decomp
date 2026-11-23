#!/bin/sh
# Build the Sonic Rush DC game disc from the owner's converted assets.
#   1. tools/dreamcast/stages/stage2dc.py --all        (needs extract/eur from YOUR ROM)
#   2. sh dreamcast/game/build_disc.sh                  -> build/dreamcast/game/srdc.cdi
# The disc contains converted game data and must not be redistributed.
set -e
HERE=$(cd "$(dirname "$0")" && pwd)
ROOT=$(cd "$HERE/../.." && pwd)
OUT=${SRDC_OUT:-$ROOT/build/dreamcast/game}
if [ -z "$SRDC_PREBUILT" ]; then
    . /opt/toolchains/dc/kos/environ.sh
    make -C "$HERE"
fi
mkdir -p "$OUT"
if [ -n "$SRDC_PREBUILT" ]; then
    FIRST_READ=$SRDC_PREBUILT/1ST_READ.BIN
else
    sh-elf-objcopy -R .stack -O binary "$HERE/srdc.elf" "$OUT/srdc.bin"
    "$KOS_BASE/utils/scramble/scramble" "$OUT/srdc.bin" "$OUT/1ST_READ.BIN"
    FIRST_READ=$OUT/1ST_READ.BIN
fi
# disc tree, packs and test switches (SRDC_START, SRDC_AUTOCLEAR, ... -> /cd/debug, docs/dreamcast/BUILD.md):
# tools/dreamcast/disc/assemble_disc.py, shared with the Windows kit (tools/dreamcast/winkit)
python3 "$ROOT/tools/dreamcast/disc/assemble_disc.py" --root "$ROOT" --out "$OUT" --first-read "$FIRST_READ"
cd "$OUT"
rm -f IP.BIN
if [ -n "$SRDC_PREBUILT" ]; then cp "$SRDC_PREBUILT/IP.BIN" "$OUT/IP.BIN"; else
"$KOS_BASE/utils/makeip/makeip" -g "SONIC RUSH DC" -c "PORT" "$OUT/IP.BIN" >/dev/null; fi
genisoimage -quiet -C 0,11702 -V SRDC -G IP.BIN -r -J -l -o "$OUT/srdc.iso" "$OUT/disc"
${CDI4DC:-cdi4dc} "$OUT/srdc.iso" "$OUT/srdc.cdi" >/dev/null
echo "built $OUT/srdc.cdi ($(du -h "$OUT/srdc.cdi" | cut -f1))"

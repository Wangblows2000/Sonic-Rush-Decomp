#!/bin/sh
# Build the foundation CDI.
#   sh dreamcast/foundation/build_cdi.sh [--with-rom-assets]
# --with-rom-assets copies build/dreamcast/assets/{sprite.argb4444,sample.wav}
# (made from YOUR ROM by tools/dreamcast/convert_assets.py) onto the disc.
# Needs: KOS env (environ.sh), genisoimage, cdi4dc, wav2adpcm, KOS makeip/scramble.
set -e
HERE=$(cd "$(dirname "$0")" && pwd)
ROOT=$(cd "$HERE/../.." && pwd)
OUT=$ROOT/build/dreamcast/foundation
. /opt/toolchains/dc/kos/environ.sh
rm -rf "$OUT/disc"
mkdir -p "$OUT/disc"
make -C "$HERE"
cp "$HERE/srdc_foundation.elf" "$OUT/"
sh-elf-objcopy -R .stack -O binary "$OUT/srdc_foundation.elf" "$OUT/srdc_foundation.bin"
"$KOS_BASE/utils/scramble/scramble" "$OUT/srdc_foundation.bin" "$OUT/disc/1ST_READ.BIN"
python3 "$HERE/make_tone.py" "$OUT/tone_pcm16.wav"
${WAV2ADPCM:-wav2adpcm} -t "$OUT/tone_pcm16.wav" "$OUT/disc/tone.wav"
NAME=srdc_foundation
if [ "$1" = "--with-rom-assets" ]; then
    mkdir -p "$OUT/disc/assets"
    cp "$ROOT/build/dreamcast/assets/sprite.argb4444" "$ROOT/build/dreamcast/assets/sample.wav" "$OUT/disc/assets/"
    NAME=srdc_foundation_romassets
fi
cd "$OUT"
rm -f "$OUT/IP.BIN"
"$KOS_BASE/utils/makeip/makeip" -g "SONIC RUSH DC FOUNDATION" -c "PORT TEST" "$OUT/IP.BIN"
genisoimage -C 0,11702 -V SRDC -G IP.BIN -r -J -l -o "$OUT/$NAME.iso" "$OUT/disc"
${CDI4DC:-cdi4dc} "$OUT/$NAME.iso" "$OUT/$NAME.cdi" >/dev/null
echo "built $OUT/$NAME.cdi"

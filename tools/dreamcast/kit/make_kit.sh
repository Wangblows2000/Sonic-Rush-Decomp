#!/bin/sh
# Integrator: build the distributable kit (code and tools only, no game data).
#   sh tools/dreamcast/kit/make_kit.sh  -> build/dreamcast/kit/srdc_kit.zip
set -e
ROOT=$(cd "$(dirname "$0")/../../.." && pwd)
K=$ROOT/build/dreamcast/kit/srdc_kit
rm -rf "$K"; mkdir -p "$K/prebuilt" "$K/bin" "$K/thirdparty"
. /opt/toolchains/dc/kos/environ.sh
make -C "$ROOT/dreamcast/game" >/dev/null
sh-elf-objcopy -R .stack -O binary "$ROOT/dreamcast/game/srdc.elf" "$K/prebuilt/srdc.bin"
"$KOS_BASE/utils/scramble/scramble" "$K/prebuilt/srdc.bin" "$K/prebuilt/1ST_READ.BIN"
rm "$K/prebuilt/srdc.bin"
"$KOS_BASE/utils/makeip/makeip" -g "SONIC RUSH DC" -c "PORT" "$K/prebuilt/IP.BIN" >/dev/null
# tracked source only (git ls-files): converters, translated modules' data tools, docs
cd "$ROOT"
git ls-files tools/dreamcast tools/extraction/fix_path_order.py tools/ndslib dreamcast/game docs/dreamcast 'config/eur/arm9/*symbols.txt' \
  | tar -cf - -T - | tar -xf - -C "$K"
cp tools/dreamcast/kit/make_disc.sh tools/dreamcast/kit/README.txt "$K/"
cp tools/bin/dsd "$K/bin/"
mkdir -p "$K/thirdparty/wav2adpcm" && cp "$KOS_BASE/utils/wav2adpcm/wav2adpcm.c" "$K/thirdparty/wav2adpcm/"
(cd /home/claude/dcwork/img4dc && git ls-files | tar -cf - -T -) | (mkdir -p "$K/thirdparty/img4dc" && tar -xf - -C "$K/thirdparty/img4dc")
for l in LICENSE LICENSE-MIT LICENSE.md; do
    f=$(find "$ROOT/tools" /root/.cargo 2>/dev/null -path "*ds-decomp*/$l" | head -1); [ -n "$f" ] && cp "$f" "$K/thirdparty/ds-decomp-LICENSE" && break
done
# guard: refuse to ship anything that looks like converted or extracted game data
if find "$K" \( -name "*.dcm" -o -name "*.dca" -o -name "*.srb" -o -name "*.nds" -o -name "level.bin" -o -name "*.argb1555" -o -name "*.narc" -o -name "*.sdat" \) | grep -q .; then
    echo "error: game data found in kit"; exit 1
fi
cd "$ROOT/build/dreamcast/kit" && rm -f srdc_kit.zip && zip -qr srdc_kit.zip srdc_kit
echo "kit: $ROOT/build/dreamcast/kit/srdc_kit.zip ($(du -h srdc_kit.zip | cut -f1))"

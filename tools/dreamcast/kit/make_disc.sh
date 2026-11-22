#!/bin/sh
# Sonic Rush DC — make the game disc from YOUR OWN Sonic Rush (EUR) ROM.
# Runs on Linux or Windows WSL (Ubuntu). The kit contains no game data:
# everything on the disc is converted here, from your ROM.
#
#   sh make_disc.sh /mnt/c/Users/<you>/Downloads/rom.nds
#
# Output: out/srdc.cdi  (do not redistribute: it contains the game's data)
set -e
ROM=$1
[ -f "$ROM" ] || { echo "usage: sh make_disc.sh path/to/sonic_rush_eur.nds"; exit 1; }
KIT=$(cd "$(dirname "$0")" && pwd)
cd "$KIT"

echo "== 1/5 packages (python3, numpy, pillow, genisoimage, gcc, cmake)"
if ! command -v genisoimage >/dev/null || ! python3 -c "import numpy, PIL" 2>/dev/null || ! command -v cmake >/dev/null; then
    sudo apt-get update -q && sudo apt-get install -y -q python3 python3-numpy python3-pil genisoimage build-essential cmake
fi

echo "== 2/5 host tools (wav2adpcm, cdi4dc)"
mkdir -p bin
[ -x bin/wav2adpcm ] || gcc -O2 -o bin/wav2adpcm thirdparty/wav2adpcm/wav2adpcm.c
if [ ! -x bin/cdi4dc ]; then
    (mkdir -p thirdparty/img4dc/build && cd thirdparty/img4dc/build && cmake .. >/dev/null && make cdi4dc >/dev/null)
    cp thirdparty/img4dc/build/cdi4dc/cdi4dc bin/
fi
export PATH="$KIT/bin:$PATH"

echo "== 3/5 extract your ROM"
sha=$(sha1sum "$ROM" | cut -d' ' -f1)
if [ "$sha" != "52e5024efcf7617a820fa23789b37e5168c3c4fc" ]; then
    echo "warning: ROM SHA-1 $sha is not the supported EUR dump (52e5024e...); continuing anyway"
fi
if [ ! -d extract/eur/files ]; then
    mkdir -p extract
    bin/dsd rom extract --rom "$ROM" --output-path extract/eur
    python3 tools/extraction/fix_path_order.py "$ROM" extract/eur/path_order.txt || true
fi

echo "== 4/5 convert (stages, sprites, models, audio, tables) - takes a few minutes"
python3 tools/dreamcast/stages/stage2dc.py --all --no-preview
python3 tools/dreamcast/sprites/bac2dc.py --no-preview
python3 tools/dreamcast/models/nsbmd2dc.py batch
python3 tools/dreamcast/audio/sdat2dc.py --wav2adpcm "$KIT/bin/wav2adpcm" 2>/dev/null || python3 tools/dreamcast/audio/sdat2dc.py
python3 tools/dreamcast/audio/sfxbank.py
python3 -I dreamcast/game/player/tools/extract_physics.py
python3 dreamcast/game/objects/tools/objdata.py --out "$KIT/build/dreamcast/objects"
python3 dreamcast/game/ui/tools/uidata.py --out "$KIT/build/dreamcast/ui"
python3 dreamcast/game/boss/tools/bossdata.py --out "$KIT/build/dreamcast/boss"

echo "== 5/5 disc image"
SRDC_PREBUILT="$KIT/prebuilt" SRDC_OUT="$KIT/out" sh dreamcast/game/build_disc.sh
echo "done: $KIT/out/srdc.cdi"

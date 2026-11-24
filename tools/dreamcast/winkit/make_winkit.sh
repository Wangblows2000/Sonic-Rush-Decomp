#!/bin/sh
# Build the Windows kit (Windows 10 and later): a GUI that makes the disc from the owner's ROM.
#   sh tools/dreamcast/winkit/make_winkit.sh  -> build/dreamcast/winkit/SonicRushDC-Windows.zip
# Contents: SonicRushDC.exe (Win32 GUI), README.txt, app\ (bundled Python 3.13 + numpy + Pillow,
# dsd/cdi4dc/wav2adpcm/sseqrender/test_coverage/vs_route .exe, the converters, prebuilt 1ST_READ.BIN/IP.BIN).
# No game data: everything on the disc is converted on the player's PC from their own ROM.
# Needs: KallistiOS (environ.sh), MinGW-w64 (x86_64-w64-mingw32-gcc/windres), Rust with the
# x86_64-pc-windows-gnu target, pip, network access to python.org and PyPI.
set -e
ROOT=$(cd "$(dirname "$0")/../../.." && pwd)
HERE=$ROOT/tools/dreamcast/winkit
W=$ROOT/build/dreamcast/winkit
K=$W/SonicRushDC
A=$K/app
CACHE=$W/cache
PYVER=${PYVER:-3.13.9}
CC="x86_64-w64-mingw32-gcc -static -O2"
rm -rf "$K"; mkdir -p "$A/bin" "$A/prebuilt" "$A/LICENSES" "$CACHE"

echo "== Dreamcast program (1ST_READ.BIN, IP.BIN)"
. /opt/toolchains/dc/kos/environ.sh
make -C "$ROOT/dreamcast/game" >/dev/null
sh-elf-objcopy -R .stack -O binary "$ROOT/dreamcast/game/srdc.elf" "$W/srdc.bin"
"$KOS_BASE/utils/scramble/scramble" "$W/srdc.bin" "$A/prebuilt/1ST_READ.BIN"
rm "$W/srdc.bin"
"$KOS_BASE/utils/makeip/makeip" -g "SONIC RUSH DC" -c "PORT" "$A/prebuilt/IP.BIN" >/dev/null

echo "== converters and data tools (tracked sources only)"
cd "$ROOT"
git ls-files tools/dreamcast tools/extraction/fix_path_order.py tools/ndslib dreamcast/game docs/dreamcast \
    'config/eur/arm9/*symbols.txt' | grep -v '^tools/dreamcast/winkit/' | tar -cf - -T - | tar -xf - -C "$A"
cp "$HERE/winbuild.py" "$A/"

echo "== Python $PYVER (embeddable) + numpy + Pillow"
[ -f "$CACHE/python-$PYVER-embed-amd64.zip" ] || \
    curl -sSfL -o "$CACHE/python-$PYVER-embed-amd64.zip" "https://www.python.org/ftp/python/$PYVER/python-$PYVER-embed-amd64.zip"
P=$A/python
mkdir -p "$P/Lib/site-packages" "$P/DLLs"
(cd "$P" && unzip -q "$CACHE/python-$PYVER-embed-amd64.zip")
# a normal (non-isolated) layout: the ._pth file of the embeddable zip would stop scripts from importing
# the modules next to them, which the converters do
PV=$(echo "$PYVER" | cut -d. -f1-2 | tr -d .)
rm -f "$P"/python*._pth
(cd "$P" && unzip -q "python$PV.zip" -d Lib && rm "python$PV.zip" && mv ./*.pyd DLLs/)
pip download -q --only-binary=:all: --platform win_amd64 --python-version "$(echo "$PYVER" | cut -d. -f1-2)" \
    --implementation cp numpy pillow -d "$CACHE/wheels"
for w in "$CACHE"/wheels/numpy-*-cp$PV-*win_amd64.whl "$CACHE"/wheels/pillow-*-cp$PV-*win_amd64.whl; do
    unzip -q -o "$w" -d "$P/Lib/site-packages"
done
cp "$P/LICENSE.txt" "$A/LICENSES/python-LICENSE.txt"

echo "== Windows tools (MinGW-w64, Rust)"
# dsd (ROM extraction), from the repository's ds-decomp checkout
(cd "$ROOT/tools/src/ds-decomp" && cargo build -q --release --target x86_64-pc-windows-gnu --bin dsd \
    --config 'target.x86_64-pc-windows-gnu.linker="x86_64-w64-mingw32-gcc"')
cp "$ROOT/tools/src/ds-decomp/target/x86_64-pc-windows-gnu/release/dsd.exe" "$A/bin/"
cp "$ROOT/tools/src/ds-decomp/LICENSE" "$A/LICENSES/ds-decomp-LICENSE" 2>/dev/null || true
# cdi4dc (img4dc), wav2adpcm (KallistiOS), the audio renderer and the object coverage run (preload lists)
IMG=/home/claude/dcwork/img4dc
$CC -I"$IMG/cdi4dc/inc" -I"$IMG/edc/inc" -I"$IMG/common/inc" -Wno-implicit-function-declaration \
    -Wno-incompatible-pointer-types -Wno-int-conversion -o "$A/bin/cdi4dc.exe" \
    "$IMG"/cdi4dc/src/*.c "$IMG"/common/src/*.c "$IMG"/edc/src/*.c
cp "$IMG/license.txt" "$A/LICENSES/img4dc-license.txt"
$CC -o "$A/bin/wav2adpcm.exe" "$KOS_BASE/utils/wav2adpcm/wav2adpcm.c"
cp "$KOS_BASE/LICENSE" "$A/LICENSES/KallistiOS-LICENSE" 2>/dev/null || true
$CC -o "$A/bin/sseqrender.exe" "$ROOT/tools/dreamcast/audio/sseqrender.c" -lm
O=$ROOT/dreamcast/game/objects
OBJ_SRCS=$(cd "$O" && make -s -n -B ../../../build/dreamcast/objects/test_coverage | grep -o 'obj_[a-z0-9_]*\.c\|\.\./player/dcp_[a-z_]*\.c' | tr '\n' ' ')
[ -n "$OBJ_SRCS" ] || { echo "error: object module source list not found"; exit 1; }
(cd "$O" && $CC -std=c99 -fwrapv -o "$A/bin/test_coverage.exe" $OBJ_SRCS test/test_coverage.c -lm)
# Vs CPU racing line finder (dreamcast/game/vs/route): the same sources in the same link order (the
# snapshot regions are bracketed by route/snap_begin.c and route/snap_end.c)
V=$ROOT/dreamcast/game/vs
VS_SRCS=$(cd "$V" && make -s -n -B ../../../build/dreamcast/vs/vs_route | grep -o ' route/snap_begin\.c.*route/vs_route\.c')
[ -n "$VS_SRCS" ] || { echo "error: route finder source list not found"; exit 1; }
(cd "$V" && $CC -std=c99 -fwrapv -fno-common -o "$A/bin/vs_route.exe" $VS_SRCS -lm)

echo "== the disc maker (Win32 GUI)"
G=$HERE/gui
python3 "$G/make_icon.py" "$W/srdcgui.ico" >/dev/null
cp "$G/srdcgui.rc" "$G/srdcgui.manifest" "$W/"
(cd "$W" && x86_64-w64-mingw32-windres srdcgui.rc -O coff -o srdcgui_res.o)
x86_64-w64-mingw32-gcc -O2 -Wall -municode -mwindows -static -o "$K/SonicRushDC.exe" "$G/srdcgui.c" "$W/srdcgui_res.o" \
    -lcomctl32 -lcomdlg32 -lshell32 -lshlwapi
rm -f "$W/srdcgui_res.o" "$W/srdcgui.rc" "$W/srdcgui.manifest" "$W/srdcgui.ico"
cp "$HERE/README.txt" "$K/README.txt"
# Windows line endings for Notepad
sed -i 's/$/\r/' "$K/README.txt"

# guard: refuse to ship anything that looks like converted or extracted game data
if find "$K" \( -name "*.dcm" -o -name "*.dca" -o -name "*.srb" -o -name "*.nds" -o -name "level.bin" \
        -o -name "*.argb1555" -o -name "*.narc" -o -name "*.sdat" -o -name "*.cdi" -o -name "*.iso" \) | grep -q .; then
    echo "error: game data found in the Windows kit"; exit 1
fi
cd "$W" && rm -f SonicRushDC-Windows.zip && zip -qr -9 SonicRushDC-Windows.zip SonicRushDC
echo "kit: $W/SonicRushDC-Windows.zip ($(du -h SonicRushDC-Windows.zip | cut -f1))"

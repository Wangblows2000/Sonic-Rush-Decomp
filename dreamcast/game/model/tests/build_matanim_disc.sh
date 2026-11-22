#!/bin/sh
# Material-animation / near-plane test disc (emulator testing only; the disc holds data converted
# from YOUR ROM: never redistribute it).
#   sh dreamcast/game/model/tests/build_matanim_disc.sh   -> build/dreamcast/model_test/mtest.cdi
set -e
HERE=$(cd "$(dirname "$0")/.." && pwd)
ROOT=$(cd "$HERE/../../.." && pwd)
OUT=$ROOT/build/dreamcast/model_test
M=$ROOT/build/dreamcast/models
. /opt/toolchains/dc/kos/environ.sh
mkdir -p "$OUT"
kos-cc -std=gnu99 -O2 -fwrapv -Wall -Wextra -Werror -I"$HERE" -c "$HERE/model.c" -o "$OUT/model.o"
kos-cc -std=gnu99 -O2 -fwrapv -Wall -Wextra -Werror -I"$HERE" -c "$HERE/tests/matanim_dc.c" -o "$OUT/matanim_dc.o"
kos-cc -o "$OUT/mtest.elf" "$OUT/matanim_dc.o" "$OUT/model.o" -lm
rm -rf "$OUT/root"; mkdir -p "$OUT/root/models"
for f in narc__z73_act_lz7__eff_tornado__eff_tornado.dcm mod__bsf_sky__bsf_sky.dcm \
         narc__z73_act_lz7__eff_tornado01.dcc narc__z73_act_lz7__eff_tornado01.dcp \
         narc__z73_act_lz7__eff_tornado02.dcc narc__z73_act_lz7__eff_tornado02.dcp \
         narc__zf3_act_lz7__bsf.dct narc__zf3_act_lz7__bsf.dcc; do
    cp "$M/$f" "$OUT/root/models/"
done
sh-elf-objcopy -R .stack -O binary "$OUT/mtest.elf" "$OUT/mtest.bin"
"$KOS_BASE/utils/scramble/scramble" "$OUT/mtest.bin" "$OUT/root/1ST_READ.BIN"
cd "$OUT"; rm -f IP.BIN
"$KOS_BASE/utils/makeip/makeip" -g "SRDC MODEL TEST" -c "TEST" "$OUT/IP.BIN" >/dev/null
genisoimage -quiet -C 0,11702 -V MTEST -G IP.BIN -r -J -l -o "$OUT/mtest.iso" "$OUT/root"
${CDI4DC:-cdi4dc} "$OUT/mtest.iso" "$OUT/mtest.cdi" >/dev/null
rm -f "$OUT/mtest.iso" "$OUT/mtest.bin" "$OUT"/*.o
echo "built $OUT/mtest.cdi ($(du -h "$OUT/mtest.cdi" | cut -f1))"

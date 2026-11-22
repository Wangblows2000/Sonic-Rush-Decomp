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
rm -rf "$OUT/disc"; mkdir -p "$OUT/disc/stages" "$OUT/disc/player"
cp "$ROOT/build/dreamcast/player/physics.bin" "$ROOT/build/dreamcast/player/trig.bin" "$OUT/disc/player/"
M=$ROOT/build/dreamcast/models
cp "$M/mod__son__son.dcm" "$OUT/disc/player/son.dcm"
cp "$M/mod__blz__blz.dcm" "$OUT/disc/player/blz.dcm"
cp "$M/narc__player_lz7__son.dca" "$OUT/disc/player/son.dca"
cp "$M/narc__player_lz7__son.dcv" "$OUT/disc/player/son.dcv"
# Player_SetAnimation model-animation table (ov000 0x02368bdc, 0x68 bytes) from YOUR ROM
python3 -c "import sys;d=open(sys.argv[1],'rb').read();o=0x02368bdc-0x022C5C80;open(sys.argv[2],'wb').write(d[o:o+0x68])" \
    "$ROOT/extract/eur/arm9_overlays/ov000.bin" "$OUT/disc/player/animmap.bin"
mkdir -p "$OUT/disc/objects" "$OUT/disc/sprites"
cp -r "$ROOT/build/dreamcast/objects/objdata.bin" "$ROOT/build/dreamcast/objects/hbx" "$OUT/disc/objects/"
grep -v '^#' "$ROOT/build/dreamcast/objects/sprites.txt" | cut -f1 | while read -r sp; do
    [ -n "$sp" ] || continue
    S=$ROOT/build/dreamcast/sprites/bac/$sp
    mkdir -p "$OUT/disc/sprites/$sp"
    cp "$S/frames.bin" "$OUT/disc/sprites/$sp/"
    for a in "$S"/atlas*.argb1555; do [ -f "$a" ] && cp "$a" "$OUT/disc/sprites/$sp/"; done
done
# UI (HUD, title card, pause, results, game over)
mkdir -p "$OUT/disc/ui"; cp "$ROOT/build/dreamcast/ui/uidata.bin" "$OUT/disc/ui/"
for sp in act__ac_fix_moji act__dm_stdm act__dm_stdm_eng act__dm_cldm act__ac_fix_pause act__ac_fix_pause_frame act__ac_fix_gameover; do
    S=$ROOT/build/dreamcast/sprites/bac/$sp; mkdir -p "$OUT/disc/sprites/$sp"
    cp "$S/frames.bin" "$OUT/disc/sprites/$sp/"; for a in "$S"/atlas*.argb1555; do [ -f "$a" ] && cp "$a" "$OUT/disc/sprites/$sp/"; done
done
# sound effect banks (SSAR/SBNK/SWAR per stage SDAT, run by the native sequencer)
mkdir -p "$OUT/disc/sound"; cp "$ROOT"/build/dreamcast/audio/sfxbank/*.srb "$OUT/disc/sound/"
# bosses: data + models listed by the boss module
mkdir -p "$OUT/disc/boss" "$OUT/disc/models"; cp "$ROOT/build/dreamcast/boss/bossdata.bin" "$OUT/disc/boss/"
grep -v '^#' "$ROOT/build/dreamcast/boss/assets.txt" | grep -E '\.(dcm|dca|dcv)$' | while read -r f; do
    b=$(basename "$f"); [ -f "$ROOT/build/dreamcast/models/$b" ] && cp "$ROOT/build/dreamcast/models/$b" "$OUT/disc/models/"
done
for f in mod__gmk_rope_c__gmk_rope_c01.dcm mod__gmk_rope_c__gmk_rope_c02.dcm; do
    [ -f "$ROOT/build/dreamcast/models/$f" ] && cp "$ROOT/build/dreamcast/models/$f" "$OUT/disc/models/"
done
mkdir -p "$OUT/disc/music"
A=$ROOT/build/dreamcast/audio
for z in 1 2 3 4 5 6 7; do
    [ -f "$A/music/z${z}s/SEQ_${z}sonic.wav" ] && cp "$A/music/z${z}s/SEQ_${z}sonic.wav" "$OUT/disc/music/z${z}.wav"
done
python3 -c "
import json,sys
m=json.load(open(sys.argv[1]))
ents=m.get('entries') or m.get('music') or []
out=[]
for e in ents:
    if e.get('kind')=='music' and e.get('status')=='ok' and e['name'].startswith('SEQ_') and e['name'].endswith('sonic') and e['sdat'] in ['z%ds'%i for i in range(1,8)]:
        out.append('%s %d %d'%(e['sdat'][1], e.get('loop_start',0) if e.get('loop') else 0, e.get('loop_end',0) if e.get('loop') else 0))
open(sys.argv[2],'w').write('\\n'.join(sorted(out))+'\\n')
" "$A/manifest.json" "$OUT/disc/music/loops.txt"
if [ -n "$SRDC_PREBUILT" ]; then
    cp "$SRDC_PREBUILT/1ST_READ.BIN" "$OUT/disc/"
else
    sh-elf-objcopy -R .stack -O binary "$HERE/srdc.elf" "$OUT/srdc.bin"
    "$KOS_BASE/utils/scramble/scramble" "$OUT/srdc.bin" "$OUT/disc/1ST_READ.BIN"
fi
for d in "$ROOT"/build/dreamcast/stages/z*/; do
    id=$(basename "$d")
    [ -f "$d/level.bin" ] || continue
    mkdir -p "$OUT/disc/stages/$id"
    cp "$d/level.bin" "$d/blocktex.bin" "$OUT/disc/stages/$id/"
    [ -f "$d/bg.bin" ] && cp "$d/bg.bin" "$OUT/disc/stages/$id/"
done
# test builds: SRDC_START=<stage index> writes /cd/debug/start.txt (story index into k_stages)
if [ -n "$SRDC_START" ]; then mkdir -p "$OUT/disc/debug"; echo "$SRDC_START" > "$OUT/disc/debug/start.txt"; fi
cd "$OUT"
rm -f IP.BIN
if [ -n "$SRDC_PREBUILT" ]; then cp "$SRDC_PREBUILT/IP.BIN" "$OUT/IP.BIN"; else
"$KOS_BASE/utils/makeip/makeip" -g "SONIC RUSH DC" -c "PORT" "$OUT/IP.BIN" >/dev/null; fi
genisoimage -quiet -C 0,11702 -V SRDC -G IP.BIN -r -J -l -o "$OUT/srdc.iso" "$OUT/disc"
${CDI4DC:-cdi4dc} "$OUT/srdc.iso" "$OUT/srdc.cdi" >/dev/null
echo "built $OUT/srdc.cdi ($(du -h "$OUT/srdc.cdi" | cut -f1))"

#!/bin/sh
# Cutscene test disc (emulator testing only; NOT the game disc, which build_disc.sh makes).
#   make -C dreamcast/game/cutscene data link
#   sh dreamcast/game/cutscene/tools/build_test_disc.sh [list.txt]   -> build/dreamcast/cutscene/disc/cstest.cdi
# list.txt: lines "kind id character" (CsKind numbers), copied to /cd/cutscene/test.txt.
# The disc holds data converted from YOUR ROM: never redistribute it.
# The data layout below is the one the game disc needs (CUTSCENES.md section 6).
set -e
HERE=$(cd "$(dirname "$0")/.." && pwd)
ROOT=$(cd "$HERE/../../.." && pwd)
B=$ROOT/build/dreamcast
OUT=${CS_DISC_OUT:-$B/cutscene/disc}
. /opt/toolchains/dc/kos/environ.sh
D=$OUT/root
rm -rf "$D"; mkdir -p "$D/cutscene" "$D/sprites" "$D/models" "$D/sound" "$D/music"
# /cd/cutscene: tables, scripts, message / portrait packs, BG images, sequence aliases
cp "$B/cutscene/csdata.bin" "$B/cutscene/seqalias.txt" "$D/cutscene/"
cp -r "$B/cutscene/talk" "$B/cutscene/pldm" "$B/cutscene/bg" "$D/cutscene/"
[ -n "$1" ] && cp "$1" "$D/cutscene/test.txt"
# /cd/sprites: the cutscene sprite ids
grep -v '^#' "$B/cutscene/sprites.txt" | while read -r sp; do
    [ -n "$sp" ] || continue
    S=$B/sprites/bac/$sp; mkdir -p "$D/sprites/$sp"
    cp "$S/frames.bin" "$D/sprites/$sp/"
    for a in "$S"/atlas*.argb1555; do [ -f "$a" ] && cp "$a" "$D/sprites/$sp/"; done
done
# /cd/models: the cutscene models / animations
grep -v '^#' "$B/cutscene/models.txt" | while read -r f; do
    [ -n "$f" ] && cp "$B/models/$f" "$D/models/"
done
# /cd/sound: the sys SDAT per-SSAR banks
cp "$B"/cutscene/sound/sys_a*.srb "$D/sound/"
# /cd/music: sys sequences sNN.wav + sys.txt (as build_disc.sh), staff roll STRMs, extra ending
A=$B/audio
python3 -c "
import json,sys,shutil,re
m=json.load(open(sys.argv[1])); A=sys.argv[2]; D=sys.argv[3]; out=[]
for e in m.get('entries',[]):
    if e.get('kind')=='music' and e.get('sdat')=='sys' and e.get('status')=='ok':
        n=int(re.search(r'SEQ#(\d+)',e['source']).group(1))
        shutil.copy(A+'/'+e['output'], '%s/s%02d.wav'%(D,n))
        out.append('%d %d %d'%(n, e['loop_start'] if e.get('loop') else 0, e['loop_end'] if e.get('loop') else 0))
open(D+'/sys.txt','w').write('\n'.join(out)+'\n')
" "$A/manifest.json" "$A" "$D/music"
cp "$A/strm/sys/Final_Roll.wav" "$A/strm/sys/Extra_Roll.wav" "$D/music/"
cp "$A/music/extra_end/SEQ_EXTRA_END.wav" "$D/music/extra_end.wav"
# boot
sh-elf-objcopy -R .stack -O binary "$B/cutscene/cutscene_test.elf" "$OUT/cstest.bin"
"$KOS_BASE/utils/scramble/scramble" "$OUT/cstest.bin" "$D/1ST_READ.BIN"
cd "$OUT"; rm -f IP.BIN
"$KOS_BASE/utils/makeip/makeip" -g "SRDC CUTSCENE TEST" -c "TEST" "$OUT/IP.BIN" >/dev/null
genisoimage -quiet -C 0,11702 -V CSTEST -G IP.BIN -r -J -l -o "$OUT/cstest.iso" "$D"
${CDI4DC:-cdi4dc} "$OUT/cstest.iso" "$OUT/cstest.cdi" >/dev/null
echo "built $OUT/cstest.cdi ($(du -h "$OUT/cstest.cdi" | cut -f1))"

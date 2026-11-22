#!/bin/sh
# Standalone special-stage test disc (not the game disc): sptest.elf + the converted
# assets of build/dreamcast/special + the two special-stage music streams.
#   make -C dreamcast/game/special test-disc   [SPTEST_START="<stage 1..7> <emerald flags hex>"]
set -e
ROOT=$1
OUT=$2
. /opt/toolchains/dc/kos/environ.sh
D=$OUT/disc
rm -rf "$D/root"; mkdir -p "$D/root/special" "$D/root/music"
for f in $(cat "$OUT/manifest.txt"); do mkdir -p "$D/root/special/$(dirname "$f")"; cp "$OUT/$f" "$D/root/special/$f"; done
A=$ROOT/build/dreamcast/audio
python3 -c "
import json,sys,shutil,re
m=json.load(open(sys.argv[1])); A=sys.argv[2]; D=sys.argv[3]; out=[]
for e in m.get('entries',[]):
    if e.get('kind')=='music' and e.get('sdat')=='sys' and e.get('status')=='ok':
        n=int(re.search(r'SEQ#(\d+)',e['source']).group(1))
        if n in (30,31):
            shutil.copy(A+'/'+e['output'], '%s/s%02d.wav'%(D,n))
            out.append('%d %d %d'%(n, e['loop_start'] if e.get('loop') else 0, e['loop_end'] if e.get('loop') else 0))
open(D+'/sys.txt','w').write('\n'.join(out)+'\n')
" "$A/manifest.json" "$A" "$D/root/music"
[ -n "$SPTEST_START" ] && echo "$SPTEST_START" > "$D/root/special/start.txt"
sh-elf-objcopy -R .stack -O binary "$OUT/dc/sptest.elf" "$D/sptest.bin"
"$KOS_BASE/utils/scramble/scramble" "$D/sptest.bin" "$D/root/1ST_READ.BIN"
cd "$D"; rm -f IP.BIN
"$KOS_BASE/utils/makeip/makeip" -g "SRDC SPECIAL TEST" -c "PORT" "$D/IP.BIN" >/dev/null
genisoimage -quiet -C 0,11702 -V SPTEST -G IP.BIN -r -J -l -o "$D/sptest.iso" "$D/root"
${CDI4DC:-/home/claude/dcwork/bin/cdi4dc} "$D/sptest.iso" "$D/sptest.cdi" >/dev/null
echo "built $D/sptest.cdi ($(du -h "$D/sptest.cdi" | cut -f1))"

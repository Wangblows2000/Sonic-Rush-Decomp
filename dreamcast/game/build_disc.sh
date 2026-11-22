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
# ball models (Model_SetResource 1) and the four Player_SetAnimation tables (player/tools/extract_physics.py)
cp "$M/mod__son__son_ball.dcm" "$OUT/disc/player/son_ball.dcm"
cp "$M/mod__blz__blz_spin.dcm" "$OUT/disc/player/blz_spin.dcm"
cp "$ROOT/build/dreamcast/player/animtab.bin" "$OUT/disc/player/"
# Player_SetAnimation model-animation table (ov000 0x02368bdc, 0x68 bytes) from YOUR ROM
python3 -c "import sys;d=open(sys.argv[1],'rb').read();o=0x02368bdc-0x022C5C80;open(sys.argv[2],'wb').write(d[o:o+0x68])" \
    "$ROOT/extract/eur/arm9_overlays/ov000.bin" "$OUT/disc/player/animmap.bin"
mkdir -p "$OUT/disc/objects" "$OUT/disc/sprites"
cp -r "$ROOT/build/dreamcast/objects/objdata.bin" "$ROOT/build/dreamcast/objects/hbx" "$OUT/disc/objects/"
cp "$ROOT/build/dreamcast/objects/palanim.bin" "$OUT/disc/objects/"
grep -v '^#' "$ROOT/build/dreamcast/objects/sprites.txt" | cut -f1 | while read -r sp; do
    [ -n "$sp" ] || continue
    S=$ROOT/build/dreamcast/sprites/bac/$sp
    mkdir -p "$OUT/disc/sprites/$sp"
    cp "$S/frames.bin" "$OUT/disc/sprites/$sp/"
    for a in "$S"/atlas*.argb1555 "$S"/atlas*.pt4 "$S"/palettes.bin; do [ -f "$a" ] && cp "$a" "$OUT/disc/sprites/$sp/"; done
done
# UI (HUD, title card, pause, results, game over)
mkdir -p "$OUT/disc/ui/res"; cp "$ROOT/build/dreamcast/ui/uidata.bin" "$ROOT/build/dreamcast/ui/navidata.bin" "$OUT/disc/ui/"
# navigator, boss panel, title card / results backgrounds (UI.md: res/ without the png previews)
cp -r "$ROOT/build/dreamcast/ui/res/." "$OUT/disc/ui/res/"; find "$OUT/disc/ui/res" -name '*.png' -delete
for sp in act__ac_fix_moji act__dm_stdm act__dm_stdm_eng act__dm_cldm act__ac_fix_pause act__ac_fix_pause_frame act__ac_fix_gameover \
          act__navi_b act__ac_fix_bosshelp act__ac_eff_enemyroom_fix; do
    S=$ROOT/build/dreamcast/sprites/bac/$sp; mkdir -p "$OUT/disc/sprites/$sp"
    cp "$S/frames.bin" "$OUT/disc/sprites/$sp/"; for a in "$S"/atlas*.argb1555 "$S"/atlas*.pt4 "$S"/palettes.bin; do [ -f "$a" ] && cp "$a" "$OUT/disc/sprites/$sp/"; done
done
# sound effect banks (SSAR/SBNK/SWAR per stage SDAT, run by the native sequencer)
mkdir -p "$OUT/disc/sound"; cp "$ROOT"/build/dreamcast/audio/sfxbank/*.srb "$OUT/disc/sound/"
# bosses: data + models listed by the boss module
mkdir -p "$OUT/disc/boss" "$OUT/disc/models"; cp "$ROOT/build/dreamcast/boss/bossdata.bin" "$ROOT/build/dreamcast/boss/boss7bac.bin" "$OUT/disc/boss/"
grep -v '^#' "$ROOT/build/dreamcast/boss/assets.txt" | grep -E '\.(dcm|dca|dcv|dcc|dcp|dct)$' | while read -r f; do
    b=$(basename "$f"); [ -f "$ROOT/build/dreamcast/models/$b" ] && cp "$ROOT/build/dreamcast/models/$b" "$OUT/disc/models/"
done
# object models (objects_model_get: RopeC, Wheel2D, Hand2d, GmkTable, slide, crane, glider)
for f in mod__gmk_rope_c__gmk_rope_c01.dcm mod__gmk_rope_c__gmk_rope_c02.dcm mod__gmk_wheel__gmk_wheel.dcm \
         mod__gmk_hand__gmk_hand.dcm mod__gmk_table__gmk_table.dcm mod__gmk_slide__gmk_slide.dcm \
         mod__gmk_crane__gmk_crane.dcm mod__gmk_glider__gmk_glider.dcm; do
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
# front end: menu tables, backgrounds, animations, sound test list, menu sprites (MENUS.md section 4)
M=$ROOT/build/dreamcast/menus
mkdir -p "$OUT/disc/menus/bg" "$OUT/disc/menus/ama"
cp "$M/menudata.bin" "$M/sound_test.sld" "$OUT/disc/menus/"
# time attack / records / ExtraAct / zone-map navigator (MENUS.md 10-14)
cp "$M/font8.bin" "$M/extraact.bin" "$OUT/disc/menus/"; [ -d "$M/navi" ] && cp -r "$M/navi" "$OUT/disc/menus/"
for f in narc__extra_act__eff_dmg_end01__eff_dmg_end01.dcm narc__extra_act__eff_dmg_end02__eff_dmg_end02.dcm narc__extra_act__eff_dmg_end02.dca narc__extra_act__eff_dmg_end02.dcc; do [ -f "$ROOT/build/dreamcast/models/$f" ] && mkdir -p "$OUT/disc/models" && cp "$ROOT/build/dreamcast/models/$f" "$OUT/disc/models/"; done
# attract demos (demo/tools/demodata.py)
D=$ROOT/build/dreamcast/demo
if [ -f "$D/demotab.bin" ]; then mkdir -p "$OUT/disc/demo"; cp "$D/demotab.bin" "$OUT/disc/demo/"; [ -d "$D/key" ] && cp -r "$D/key" "$OUT/disc/demo/"; fi
cp "$M"/bg/*.mnbg "$OUT/disc/menus/bg/"; cp "$M"/ama/*.ama "$OUT/disc/menus/ama/"
for sp in act__title act__title_eng act__mnu_titl act__mnu_titl_eng act__mnu_psel act__mnu_psel_eng \
          act__mnu_opti_eng act__font_opti_sound act2__zmap__zmap_map_icon act2__zmap__zmap_scroll_arrow \
          act2__zmap__zmap_act act2__zmap__font_zmap_act_num act2__zmap__zmap_pause act2__zmap__zmap_pause_eng \
          act__ac_ex_cldm act__dm_cldm_tatk act__dm_demoplay_deu act__dm_demoplay_eng act__dm_demoplay_fra act__dm_demoplay_ita act__dm_demoplay_spa act__mnu_reco act__mnu_reco_deu act__mnu_reco_eng act__mnu_reco_fra act__mnu_reco_ita act__mnu_reco_jpn act__mnu_reco_spa act__mnu_time_act_deu act__mnu_time_act_eng act__mnu_time_act_fra act__mnu_time_act_ita act__mnu_time_act_jpn act__mnu_time_act_lc_eng act__mnu_time_act_lc_jpn act__mnu_time_act_spa act__mnu_time_act_x_eng act__mnu_time_act_x_jpn act__mnu_time_boss_jpn act__mnu_time_boss_lc_eng act__mnu_time_boss_lc_jpn act__mnu_time_oneself_eng act__mnu_time_oneself_jpn act__mnu_time_ranking act__mnu_time_select act__mnu_time_tryagain act__mnu_time_tryagain_deu act__mnu_time_tryagain_eng act__mnu_time_tryagain_fra act__mnu_time_tryagain_ita act__mnu_time_tryagain_jpn act__mnu_time_tryagain_spa act__navi_m; do
    S=$ROOT/build/dreamcast/sprites/bac/$sp; mkdir -p "$OUT/disc/sprites/$sp"
    cp "$S/frames.bin" "$OUT/disc/sprites/$sp/"; for a in "$S"/atlas*.argb1555 "$S"/atlas*.pt4 "$S"/palettes.bin; do [ -f "$a" ] && cp "$a" "$OUT/disc/sprites/$sp/"; done
done
# sys SDAT music (title, menus, zone map, results...) as sNN.wav by sequence id + loop table
python3 -c "
import json,sys,shutil,re
m=json.load(open(sys.argv[1])); A=sys.argv[2]; D=sys.argv[3]; out=[]
for e in m.get('entries',[]):
    if e.get('kind')=='music' and e.get('sdat')=='sys' and e.get('status')=='ok':
        n=int(re.search(r'SEQ#(\\d+)',e['source']).group(1))
        shutil.copy(A+'/'+e['output'], '%s/s%02d.wav'%(D,n))
        out.append('%d %d %d'%(n, e['loop_start'] if e.get('loop') else 0, e['loop_end'] if e.get('loop') else 0))
open(D+'/sys.txt','w').write('\\n'.join(out)+'\\n')
" "$A/manifest.json" "$A" "$OUT/disc/music"
# cutscenes (CUTSCENES.md section 2): scripts, talks, PlayerDemo scenes, ending BGs, sprites, models, sound, staff-roll music
C=$ROOT/build/dreamcast/cutscene
mkdir -p "$OUT/disc/cutscene"
cp "$C/csdata.bin" "$C/seqalias.txt" "$OUT/disc/cutscene/"; cp -r "$C/talk" "$C/pldm" "$C/bg" "$OUT/disc/cutscene/"
grep -v '^#' "$C/sprites.txt" | while read -r sp; do
    [ -n "$sp" ] || continue
    S=$ROOT/build/dreamcast/sprites/bac/$sp; mkdir -p "$OUT/disc/sprites/$sp"
    cp "$S/frames.bin" "$OUT/disc/sprites/$sp/"; for a in "$S"/atlas*.argb1555 "$S"/atlas*.pt4 "$S"/palettes.bin; do [ -f "$a" ] && cp "$a" "$OUT/disc/sprites/$sp/"; done
done
grep -v '^#' "$C/models.txt" | while read -r f; do [ -n "$f" ] && cp "$ROOT/build/dreamcast/models/$f" "$OUT/disc/models/"; done
cp "$C"/sound/sys_a*.srb "$OUT/disc/sound/"
cp "$A/strm/sys/Final_Roll.wav" "$A/strm/sys/Extra_Roll.wav" "$OUT/disc/music/"
cp "$A/music/extra_end/SEQ_EXTRA_END.wav" "$OUT/disc/music/extra_end.wav"
# Extra Zone boss (BOSSES.md "Extra Zone boss"): data, HUD/VS sprites, models
X=$ROOT/build/dreamcast/boss/extra
mkdir -p "$OUT/disc/boss/extra"
cp "$X/bossex.bin" "$X/zex.bin" "$X/vs_son.bin" "$X/vs_blz.bin" "$OUT/disc/boss/extra/"; cp -r "$X/sprites" "$OUT/disc/boss/extra/"
find "$OUT/disc/boss/extra" -name '*.png' -delete
grep -v '^#' "$X/assets.txt" | grep -E '\.(dcm|dca|dcv|dcc|dcp|dct)$' | while read -r f; do
    b=$(basename "$f"); [ -f "$ROOT/build/dreamcast/models/$b" ] && cp "$ROOT/build/dreamcast/models/$b" "$OUT/disc/models/"
done
# special stages (SPECIAL.md section 6): stage data, models, sprites, backgrounds, ss sound bank
SP=$ROOT/build/dreamcast/special
mkdir -p "$OUT/disc/special"
cp "$SP/spdata.bin" "$OUT/disc/special/"; cp -r "$SP/bg" "$SP/models" "$SP/sprites" "$SP/ss" "$OUT/disc/special/"
find "$OUT/disc/special" -name '*.png' -delete
cp "$SP/sound/ss.srb" "$OUT/disc/sound/"
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
if [ -n "$SRDC_SPECIAL" ]; then mkdir -p "$OUT/disc/debug"; echo "$SRDC_SPECIAL" > "$OUT/disc/debug/special.txt"; fi
if [ -n "$SRDC_MENUTRACE" ]; then mkdir -p "$OUT/disc/debug"; : > "$OUT/disc/debug/menutrace.txt"; fi
if [ -n "$SRDC_MENUSAVE" ]; then mkdir -p "$OUT/disc/debug"; echo "$SRDC_MENUSAVE" > "$OUT/disc/debug/menusave.txt"; fi
if [ -n "$SRDC_DEMOLEAD" ]; then mkdir -p "$OUT/disc/debug"; echo "$SRDC_DEMOLEAD" > "$OUT/disc/debug/demolead.txt"; fi
cd "$OUT"
rm -f IP.BIN
if [ -n "$SRDC_PREBUILT" ]; then cp "$SRDC_PREBUILT/IP.BIN" "$OUT/IP.BIN"; else
"$KOS_BASE/utils/makeip/makeip" -g "SONIC RUSH DC" -c "PORT" "$OUT/IP.BIN" >/dev/null; fi
genisoimage -quiet -C 0,11702 -V SRDC -G IP.BIN -r -J -l -o "$OUT/srdc.iso" "$OUT/disc"
${CDI4DC:-cdi4dc} "$OUT/srdc.iso" "$OUT/srdc.cdi" >/dev/null
echo "built $OUT/srdc.cdi ($(du -h "$OUT/srdc.cdi" | cut -f1))"

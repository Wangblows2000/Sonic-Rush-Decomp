#!/usr/bin/env python3
"""Assemble the Sonic Rush DC disc tree from the converted assets (build/dreamcast/).

Shared by dreamcast/game/build_disc.sh (Linux) and the Windows kit (tools/dreamcast/winkit).
The tree contains converted game data from the owner's ROM: never redistribute it.

  assemble_disc.py --root REPO --out OUT --first-read 1ST_READ.BIN
                   [--coverage-exe test_coverage(.exe)]   (default: make -C dreamcast/game/objects preload)

Writes OUT/disc/ (packed with tools/dreamcast/mkpak.py) and the test switches of build_disc.sh
(SRDC_* environment variables -> OUT/disc/debug/*.txt).
"""
import argparse
import os
import shutil
import subprocess
import sys
from pathlib import Path


def lines(p):
    """non-comment, non-empty lines of a list file (first tab field)"""
    out = []
    for l in Path(p).read_text(encoding="utf-8").splitlines():
        l = l.strip("\r")
        if not l or l.startswith("#"):
            continue
        out.append(l.split("\t")[0].strip())
    return [l for l in out if l]


class Disc:
    def __init__(self, root, out):
        self.root = Path(root)
        self.b = self.root / "build" / "dreamcast"
        self.d = Path(out) / "disc"

    def mk(self, rel):
        p = self.d / rel
        p.mkdir(parents=True, exist_ok=True)
        return p

    def cp(self, src, dst_dir, name=None):
        dst = self.mk(dst_dir) / (name or Path(src).name)
        shutil.copyfile(src, dst)

    def cp_opt(self, src, dst_dir):
        if Path(src).is_file():
            self.cp(src, dst_dir)

    def cptree(self, src, dst_rel):
        shutil.copytree(src, self.d / dst_rel, dirs_exist_ok=True)

    def sprite(self, sp):
        """frames.bin + atlas*.argb1555 / atlas*.pt4 / palettes.bin of one converted BAC"""
        s = self.b / "sprites" / "bac" / sp
        dst = "sprites/" + sp
        self.cp(s / "frames.bin", dst)
        for a in sorted(s.glob("atlas*.argb1555")) + sorted(s.glob("atlas*.pt4")) + [s / "palettes.bin"]:
            if a.is_file():
                self.cp(a, dst)

    def model(self, name):
        p = self.b / "models" / name
        if p.is_file():
            self.cp(p, "models")

    def drop_png(self, rel):
        for p in (self.d / rel).rglob("*.png"):
            p.unlink()


VS_STAGES = "z11 z12 z21 z22 z31 z32 z41 z42 z51 z52 z61 z62 z71 z72".split()


def make_routes(D, routes_exe=None, jobs=None):
    """Vs CPU racing lines (dreamcast/game/vs/route): one per Battle Play stage and character, searched
    with the real game simulation in the converted stages. Linux: make -C dreamcast/game/vs routes;
    Windows kit: vs_route.exe. Every pair runs with -check: a line whose generation key (the stage, the
    object data, the player tables and models, the simulation and tool sources, the format version) is
    unchanged is kept at once, the others are made again; a stopped run carries on with the pairs not yet
    made. Report per pair: build/dreamcast/vs/routes/<pair>.rte.txt (and <pair>.log)."""
    b = D.b
    rdir = b / "vs" / "routes"
    jobs = jobs or max(1, (os.cpu_count() or 2))
    if os.environ.get("SRDC_ROUTES") == "copy":   # test discs: only the lines already made
        pass
    elif not routes_exe:
        subprocess.run(["make", "-C", str(D.root / "dreamcast/game/vs"), "-j%d" % jobs, "routes"], check=True)
    else:
        import concurrent.futures
        rdir.mkdir(parents=True, exist_ok=True)
        # the players' models where the route simulation reads them (vs Makefile simmodel)
        pm = b / "player" / "model"
        pm.mkdir(parents=True, exist_ok=True)
        M = b / "models"
        for src, dst in (("mod__son__son.dcm", "son.dcm"), ("mod__son__son_ball.dcm", "son_ball.dcm"),
                         ("mod__blz__blz.dcm", "blz.dcm"), ("mod__blz__blz_spin.dcm", "blz_spin.dcm"),
                         ("narc__player_lz7__son.dca", "son.dca"), ("narc__player_lz7__son.dcv", "son.dcv")):
            shutil.copyfile(M / src, pm / dst)
        for f in ("animtab.bin", "lights.bin"):
            shutil.copyfile(b / "player" / f, pm / f)
        todo = [(s, c) for s in VS_STAGES for c in (0, 1)]
        exe = str(Path(routes_exe).resolve())

        def one(sc):
            s, c = sc
            out = rdir / ("%s_%d.rte" % (s, c))
            # relative paths from build/dreamcast: the object module keeps its directories in 64-byte buffers;
            # the tool writes <out>.tmp and renames it when complete
            r = subprocess.run([exe, "stages", "player", "objects", os.path.join("sprites", "bac"), s, str(c),
                                os.path.relpath(out, b), "-check", "-budget", "3600"], cwd=b, capture_output=True, text=True)
            (rdir / ("%s_%d.log" % (s, c))).write_text(r.stdout)
            return s, c, r.returncode, (r.stdout.strip().splitlines() or [""])[-1]

        with concurrent.futures.ThreadPoolExecutor(jobs) as ex:
            for s, c, rc, last in ex.map(one, todo):
                print("vs route %s %s: %s" % (s, "Blaze" if c else "Sonic",
                                               last if rc in (0, 3) else "failed (exit %d) %s" % (rc, last)), flush=True)
    n = partial = 0
    for p in sorted(rdir.glob("z*_*.rte")) if rdir.is_dir() else []:
        D.cp(p, "vs")
        n += 1
        if p.read_bytes()[9:10] == b"\x01":   # VsRouteHeader.flags VSR_INCOMPLETE: no way to the goal found
            partial += 1
    print("vs routes: %d of %d on the disc, %d complete, %d partial" % (n, 2 * len(VS_STAGES), n - partial, partial),
          flush=True)


def assemble(root, out, first_read, coverage_exe=None, routes_exe=None):
    D = Disc(root, out)
    b = D.b
    if D.d.exists():
        shutil.rmtree(D.d)
    D.mk("stages")
    D.mk("player")
    # player: physics, trig, models, animation tables
    for f in ("physics.bin", "trig.bin"):
        D.cp(b / "player" / f, "player")
    M = b / "models"
    D.cp(M / "mod__son__son.dcm", "player", "son.dcm")
    D.cp(M / "mod__blz__blz.dcm", "player", "blz.dcm")
    D.cp(M / "narc__player_lz7__son.dca", "player", "son.dca")
    D.cp(M / "narc__player_lz7__son.dcv", "player", "son.dcv")
    D.cp(M / "mod__son__son_ball.dcm", "player", "son_ball.dcm")
    D.cp(M / "mod__blz__blz_spin.dcm", "player", "blz_spin.dcm")
    for f in ("animtab.bin", "lights.bin"):
        D.cp(b / "player" / f, "player")
    # Player_SetAnimation model-animation table (ov000 0x02368bdc, 0x68 bytes) from the owner's ROM
    ov = (D.root / "extract" / "eur" / "arm9_overlays" / "ov000.bin").read_bytes()
    o = 0x02368BDC - 0x022C5C80
    (D.d / "player" / "animmap.bin").write_bytes(ov[o:o + 0x68])
    # objects
    D.mk("sprites")
    D.cp(b / "objects" / "objdata.bin", "objects")
    D.cptree(b / "objects" / "hbx", "objects/hbx")
    D.cp(b / "objects" / "palanim.bin", "objects")
    for sp in lines(b / "objects" / "sprites.txt"):
        D.sprite(sp)
    # zone effects
    zfx = b / "zonefx" / "zonefx.bin"
    if not zfx.is_file():
        subprocess.check_call([sys.executable, str(D.root / "dreamcast/game/zonefx/tools/zonefxdata.py")], cwd=D.root)
    D.cp(zfx, "zonefx")
    # UI
    D.mk("ui/res")
    for f in ("uidata.bin", "navidata.bin"):
        D.cp(b / "ui" / f, "ui")
    D.cptree(b / "ui" / "res", "ui/res")
    D.drop_png("ui/res")
    for sp in ("act__ac_fix_moji act__dm_stdm act__dm_stdm_eng act__dm_cldm act__ac_fix_pause act__ac_fix_pause_frame "
               "act__ac_fix_gameover act__navi_b act__ac_fix_bosshelp act__ac_eff_enemyroom_fix").split():
        D.sprite(sp)
    # sound effect banks
    for p in sorted((b / "audio" / "sfxbank").glob("*.srb")):
        D.cp(p, "sound")
    # bosses
    D.mk("models")
    for f in ("bossdata.bin", "boss7bac.bin"):
        D.cp(b / "boss" / f, "boss")
    for f in lines(b / "boss" / "assets.txt"):
        if f.rsplit(".", 1)[-1] in ("dcm", "dca", "dcv", "dcc", "dcp", "dct"):
            D.model(Path(f).name)
    for f in ("mod__gmk_rope_c__gmk_rope_c01.dcm mod__gmk_rope_c__gmk_rope_c02.dcm mod__gmk_wheel__gmk_wheel.dcm "
              "mod__gmk_hand__gmk_hand.dcm mod__gmk_table__gmk_table.dcm mod__gmk_slide__gmk_slide.dcm "
              "mod__gmk_crane__gmk_crane.dcm mod__gmk_glider__gmk_glider.dcm").split():
        D.model(f)
    # 3D BAC sprites
    for sp in ("act__ac_itm_ring3d act__ac_eff_water_3d act__ac_ene_eggpawn3d act__ac_ene_egg_bishop3d act__ac_gmk_rock_3 "
               "act__ac_gmk_bubble_3d act__ac_fix_moji3d act__ac_eff_kemuri_brake3d act__ac_eff_kemuri_spin01_3d "
               "act__ac_eff_kemuri_spin02_3d act__eff_b_jet_3d act__eff_sb_boost_a_3d act__eff_sb_boost_a2_3d "
               "act__eff_bb_boost_a2_3d act__ac_eff_b7_3d").split():
        D.sprite(sp)
    # music
    A = b / "audio"
    if not (A / "disc_music" / "streams.txt").is_file():
        raise SystemExit("run tools/dreamcast/audio/musictab.py first")
    D.cptree(A / "disc_music", "music")
    mt = D.d / "music" / "musictab.json"
    if mt.exists():
        mt.unlink()
    # front end
    Mn = b / "menus"
    D.mk("menus/bg")
    D.mk("menus/ama")
    for f in ("menudata.bin", "sound_test.sld", "font8.bin", "extraact.bin"):
        D.cp(Mn / f, "menus")
    if (Mn / "navi").is_dir():
        D.cptree(Mn / "navi", "menus/navi")
    for f in ("narc__extra_act__eff_dmg_end01__eff_dmg_end01.dcm narc__extra_act__eff_dmg_end02__eff_dmg_end02.dcm "
              "narc__extra_act__eff_dmg_end02.dca narc__extra_act__eff_dmg_end02.dcc").split():
        D.model(f)
    Dm = b / "demo"
    if (Dm / "demotab.bin").is_file():
        D.cp(Dm / "demotab.bin", "demo")
        if (Dm / "key").is_dir():
            D.cptree(Dm / "key", "demo/key")
    for p in sorted((Mn / "bg").glob("*.mnbg")):
        D.cp(p, "menus/bg")
    for p in sorted((Mn / "ama").glob("*.ama")):
        D.cp(p, "menus/ama")
    langs = "jpn eng fra deu ita spa".split()
    menu_sprites = ("act__title act__title_eng act__mnu_titl act__mnu_titl_eng act__mnu_psel act__mnu_psel_eng "
                    "act__mnu_opti_eng act__font_opti_sound act__mnu_opti act__mnu_opti_sou act__mnu_opti_sou_jpn "
                    "act__mnu_opti_sou_eng act__mnu_opti_sou_fra act__mnu_opti_sou_deu act__mnu_opti_sou_ita "
                    "act__mnu_opti_sou_spa act__title_demo act__title_demo_eng act2__zmap__zmap_map_icon "
                    "act2__zmap__zmap_scroll_arrow act2__zmap__zmap_act act2__zmap__font_zmap_act_num "
                    "act2__zmap__zmap_pause act2__zmap__zmap_pause_eng act__ac_ex_cldm act__dm_cldm_tatk "
                    "act__dm_demoplay_deu act__dm_demoplay_eng act__dm_demoplay_fra act__dm_demoplay_ita "
                    "act__dm_demoplay_spa act__mnu_reco act__mnu_reco_deu act__mnu_reco_eng act__mnu_reco_fra "
                    "act__mnu_reco_ita act__mnu_reco_jpn act__mnu_reco_spa act__mnu_time_act_deu act__mnu_time_act_eng "
                    "act__mnu_time_act_fra act__mnu_time_act_ita act__mnu_time_act_jpn act__mnu_time_act_lc_eng "
                    "act__mnu_time_act_lc_jpn act__mnu_time_act_spa act__mnu_time_act_x_eng act__mnu_time_act_x_jpn "
                    "act__mnu_time_boss_jpn act__mnu_time_boss_lc_eng act__mnu_time_boss_lc_jpn "
                    "act__mnu_time_oneself_eng act__mnu_time_oneself_jpn act__mnu_time_ranking act__mnu_time_select "
                    "act__mnu_time_tryagain act__mnu_time_tryagain_deu act__mnu_time_tryagain_eng "
                    "act__mnu_time_tryagain_fra act__mnu_time_tryagain_ita act__mnu_time_tryagain_jpn "
                    "act__mnu_time_tryagain_spa act__navi_m").split()
    menu_sprites += ["act2__caution__caution_d_" + l for l in langs]
    menu_sprites += ["act2__caution__caution"] + ["act2__caution__caution_" + l for l in langs]
    menu_sprites += ["act2__caution__caution_b_" + l for l in langs]
    # MsgBox files 2..7 (MsgBox_Create: s_mb_file): caution_a (Battle Play quit / zone select), _ad, _c, _e
    for k in ("a", "ad", "c", "e"):
        menu_sprites += ["act2__caution__caution_%s_%s" % (k, l) for l in langs]
    # Battle Play: stage select (TimeSelectMenu) and results (ResultsScreen)
    menu_sprites += ["act__mnu_conf_zsel", "act__mnu_conf_result"]
    menu_sprites += ["act__mnu_conf_zsel_" + l for l in langs] + ["act__mnu_conf_result_" + l for l in langs]
    for sp in menu_sprites:
        D.sprite(sp)
    # cutscenes
    C = b / "cutscene"
    for f in ("csdata.bin", "seqalias.txt"):
        D.cp(C / f, "cutscene")
    for sub in ("talk", "pldm", "bg"):
        D.cptree(C / sub, "cutscene/" + sub)
    for sp in lines(C / "sprites.txt"):
        D.sprite(sp)
    for f in lines(C / "models.txt"):
        D.cp(b / "models" / f, "models")
    for p in sorted((C / "sound").glob("sys_a*.srb")):
        D.cp(p, "sound")
    D.cp(A / "strm" / "sys" / "Final_Roll.wav", "music")
    D.cp(A / "strm" / "sys" / "Extra_Roll.wav", "music")
    D.cp(A / "music" / "extra_end" / "SEQ_EXTRA_END.wav", "music", "extra_end.wav")
    # Extra Zone boss
    X = b / "boss" / "extra"
    for f in ("bossex.bin", "zex.bin", "vs_son.bin", "vs_blz.bin"):
        D.cp(X / f, "boss/extra")
    D.cptree(X / "sprites", "boss/extra/sprites")
    D.drop_png("boss/extra")
    for f in lines(X / "assets.txt"):
        if f.rsplit(".", 1)[-1] in ("dcm", "dca", "dcv", "dcc", "dcp", "dct"):
            D.model(Path(f).name)
    # special stages
    S = b / "special"
    D.cp(S / "spdata.bin", "special")
    for sub in ("bg", "models", "sprites", "ss"):
        D.cptree(S / sub, "special/" + sub)
    D.drop_png("special")
    D.cp(S / "sound" / "ss.srb", "sound")
    # text files the game parses use LF: converters run on Windows write CRLF (text mode)
    for p in D.d.rglob("*.txt"):
        t = p.read_bytes()
        if b"\r\n" in t:
            p.write_bytes(t.replace(b"\r\n", b"\n"))
    # load-time packs: per-stage object packs (preload list from the object coverage run), then data.pak
    preload = b / "objects" / "preload.txt"
    if coverage_exe:
        stages = ("z11 z12 z13 z21 z22 z23 z31 z32 z33 z41 z42 z43 z51 z52 z53 z61 z62 z63 z71 z72 z73 "
                  "z81 z82 z91 z92 zf3").split()
        env = dict(os.environ, COV_RES="1")
        # relative paths from build/dreamcast: the object module keeps its directories in 64-byte buffers
        r = subprocess.run([str(Path(coverage_exe).resolve()), "player", "objects", os.path.join("sprites", "bac"),
                            "stages"] + stages, env=env, cwd=b, capture_output=True, text=True, check=True)
        preload.write_text("".join(l + "\n" for l in r.stdout.splitlines() if l.startswith("res ")), encoding="utf-8",
                           newline="\n")
    else:
        subprocess.run(["make", "-C", str(D.root / "dreamcast/game/objects"), "preload"], check=True,
                       stdout=subprocess.DEVNULL)
    mkpak = str(D.root / "tools/dreamcast/mkpak.py")
    subprocess.check_call([sys.executable, mkpak, "--stage-packs", str(D.d), str(preload), str(b / "stages"),
                           str(D.d / "objects")])
    subprocess.check_call([sys.executable, mkpak, str(D.d), str(D.d / "data.pak"), "sprites", "objects", "ui/res",
                           "special/sprites", "boss/extra/sprites", "models", "sound", "cutscene",
                           "--skip-argb-with-pt4", "--loose-ext=.csp", "--drop-packed"])
    shutil.copyfile(first_read, D.d / "1ST_READ.BIN")
    make_routes(D, routes_exe)
    for sd in sorted((b / "stages").glob("z*")):
        if not (sd / "level.bin").is_file():
            continue
        D.cp(sd / "level.bin", "stages/" + sd.name)
        D.cp(sd / "blocktex.bin", "stages/" + sd.name)
        D.cp_opt(sd / "bg.bin", "stages/" + sd.name)
    # test switches (build_disc.sh): SRDC_<NAME> -> /cd/debug/<file>
    dbg = D.d / "debug"
    def put(name, text):
        dbg.mkdir(parents=True, exist_ok=True)
        (dbg / name).write_text(text, encoding="utf-8", newline="\n")
    e = os.environ
    for var, fname in (("SRDC_START", "start.txt"), ("SRDC_STARTPOS", "startpos.txt"), ("SRDC_SPECIAL", "special.txt"),
                       ("SRDC_MENUSAVE", "menusave.txt"), ("SRDC_DEMOLEAD", "demolead.txt"),
                       ("SRDC_AUTOCLEAR", "autoclear.txt"), ("SRDC_AUTODIE", "autodie.txt"),
                       ("SRDC_AUTOMAP", "automap.txt"), ("SRDC_SPAWN", "spawn.txt"), ("SRDC_BOSSHP", "bosshp.txt")):
        if e.get(var):
            put(fname, e[var] + "\n")
    for var, fname in (("SRDC_MENUTRACE", "menutrace.txt"), ("SRDC_SNDTRACE", "sndtrace.txt"),
                       ("SRDC_SPQUOTA", "spquota.txt")):
        if e.get(var):
            put(fname, "")
    if e.get("SRDC_BGMTEST"):
        dbg.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(e["SRDC_BGMTEST"], dbg / "bgmtest.txt")
    if e.get("SRDC_DEBUGDIR"):
        dbg.mkdir(parents=True, exist_ok=True)
        for p in Path(e["SRDC_DEBUGDIR"]).iterdir():
            if p.is_file():
                shutil.copyfile(p, dbg / p.name)


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--root", required=True)
    ap.add_argument("--out", required=True)
    ap.add_argument("--first-read", required=True)
    ap.add_argument("--coverage-exe")
    ap.add_argument("--routes-exe")
    a = ap.parse_args()
    assemble(a.root, a.out, a.first_read, a.coverage_exe, a.routes_exe)


if __name__ == "__main__":
    main()

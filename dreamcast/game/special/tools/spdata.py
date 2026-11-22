#!/usr/bin/env python3
"""Special-stage asset converter (Sonic Rush Dreamcast port), bring-your-own-ROM.

Reads the owner's extracted ROM (extract/eur) and writes everything the native
special-stage module (dreamcast/game/special/) loads into build/dreamcast/special/
(gitignored).  Nothing from the ROM is committed; run it again to regenerate.

Outputs (paths relative to --out, the disc gets the same tree under /cd/special):
  spdata.bin            "SPD1" blob table:
                          rom tables of overlay 1 / arm9 keyed by DS address
                          ("ov1", "arm9" chunks: the code reads them with SP_ROM(addr)),
                          function symbol table (address -> name, resolves the
                          function pointers inside those tables), sin/cos table
                          (FX_SinCosTable_), atan table (FX_Atan2), sky texture-SRT
                          track (ss.nsbta), texture-pattern key tables (ss.nsbtp,
                          eff_shoot_bomb.nsbtp), ss_player node names.
  ss/ss.ssd ss/ss_N.sst the stage scripts and trick sequences (copied as is)
  models/*.dcm|dca|dcv  converted NSBMD/NSBCA/NSBVA (tools/dreamcast/models/nsbmd2dc.py)
                        incl. texture-pattern variants <model>__pN.dcm
  sprites/<id>/         BAC sprites (tools/dreamcast/sprites/bac2dc.py): frames.bin + atlas0.argb1555
                        (+ palettes.bin, atlas0.pt4: PVR PAL4 page, SPRITES.md section 4)
  bg/*.mnbg             BBG backgrounds (dreamcast/game/menus/tools/menubg.py format)
  sound/ss.srb          sound-effect bank: sys SDAT SSAR 9, 14, 16 (tools/dreamcast/audio/sfxbank.py)
  manifest.txt          every output file (what build_disc must copy to /cd/special)

Usage: python3 dreamcast/game/special/tools/spdata.py [--extract extract/eur]
           [--out build/dreamcast/special] [--audio build/dreamcast/audio] [--skip-sprites] [--skip-sound]
"""
import argparse
import copy
import re
import shutil
import struct
import subprocess
import sys
import types
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT / "tools/dreamcast/models"))
sys.path.insert(0, str(ROOT / "tools/dreamcast/sprites"))
sys.path.insert(0, str(ROOT / "tools/dreamcast/audio"))
sys.path.insert(0, str(ROOT / "dreamcast/game/menus/tools"))

OV_BASE = 0x022C5C80
ARM9_BASE = 0x02000000
# DS address ranges the special-stage code reads (tables, strings, constants)
OV1_RANGES = [(0x02316FE0, 0x02318010), (0x0231DF70, 0x0231F040)]
ARM9_RANGES = [(0x0206E470, 0x02072470 + 0x102)]   # FX_SinCosTable_ (data_0206e470..) + atan (data_02072470)


def load_symbols():
    syms = {}
    for f in ("config/eur/arm9/overlays/ov001/symbols.txt", "config/eur/arm9/symbols.txt"):
        for line in open(ROOT / f):
            m = re.match(r"(\S+) kind:(\S+) addr:0x([0-9a-f]+)", line)
            if m:
                syms[m.group(1)] = (int(m.group(3), 16), m.group(2))
    return syms


def blob_file(blobs, out):
    """'SPD1' u32 version, u32 count, u32 table ofs; table {char name[24]; u32 addr, ofs, size}."""
    hdr = 16
    table = hdr + 36 * len(blobs)
    data = bytearray()
    ents = []
    for name, addr, b in blobs:
        while (table + len(data)) % 4:
            data.append(0)
        ents.append((name, addr, table + len(data), len(b)))
        data.extend(b)
    o = bytearray(struct.pack("<4sIII", b"SPD1", 1, len(blobs), hdr))
    for name, addr, ofs, size in ents:
        o += struct.pack("<24sIII", name.encode()[:23], addr, ofs, size)
    o += data
    out.write_bytes(o)


def convert_models(extract, outdir, blobs, files):
    import g3d
    import nsbmd2dc as N
    root = Path(extract) / "files"
    md = outdir / "models"
    md.mkdir(parents=True, exist_ok=True)

    def emit_bmd(data, base, pattern=None):
        r = g3d.parse_bmd(data)
        for m in r["models"]:
            pkg, rep, _ = N.convert_model(m, r["tex"])
            fn = "%s__%s.dcm" % (base, m["name"])
            (md / fn).write_bytes(N.write_dcm(pkg))
            files.append("models/" + fn)
            print("  %-48s v%d tri%d quad%d %s" % (fn, rep["src"]["num_vertex"], rep["src"]["num_triangle"],
                                                   rep["src"]["num_quad"], "" if rep.get("counts_match", True) else "COUNT MISMATCH"))
            if pattern:
                # texture-pattern animation (NSBTP): one variant per pattern texture, material
                # texture/palette swapped (the DS swaps TEXIMAGE_PARAM at draw time)
                for k, (tex, pal) in enumerate(pattern["frames"]):
                    m2 = copy.deepcopy(m)
                    for mat in m2["materials"]:
                        if mat["name"] == pattern["material"]:
                            mat["texture"], mat["palette"] = tex, pal
                    pkg, rep, _ = N.convert_model(m2, r["tex"])
                    fn2 = "%s__%s__p%d.dcm" % (base, m["name"], k)
                    (md / fn2).write_bytes(N.write_dcm(pkg))
                    files.append("models/" + fn2)

    def emit_anim(data, fn):
        if data[:4] == b"BCA0":
            blob, rep = N.write_dca_joint(g3d.parse_bca(data))
        else:
            blob, rep = N.write_dca_vis(g3d.parse_bva(data))
        (md / fn).write_bytes(blob)
        files.append("models/" + fn)
        names = [a["name"] for a in rep] if isinstance(rep, list) and rep and isinstance(rep[0], dict) else None
        print("  %-48s %s" % (fn, (names or "")[:6] if names else ""))

    def pattern_of(btp_data, key_name):
        pats = g3d.parse_btp(btp_data)
        for a in pats:
            if a["name"] == key_name:
                mat = a["materials"][0]
                uniq = []
                for _, t, p in mat["keys"]:
                    if (t, p) not in uniq:
                        uniq.append((t, p))
                keys = [(f, uniq.index((t, p))) for f, t, p in mat["keys"]]
                tab = struct.pack("<HH", a["num_frame"], len(keys)) + b"".join(struct.pack("<HH", f, k) for f, k in keys)
                return {"material": mat["material"], "frames": uniq, "table": tab}
        raise SystemExit("pattern %s not found" % key_name)

    # mod/ (map, sky, gimmick models)
    ss_btp = (root / "mod/ss.nsbtp").read_bytes()
    eme_pat = pattern_of(ss_btp, "ss_eme_light")
    blobs.append(("pat_eme_light", 0, eme_pat["table"]))
    for p in sorted((root / "mod").glob("ss_*.nsbmd")):
        emit_bmd(p.read_bytes(), "mod__" + p.stem, eme_pat if p.stem == "ss_eme_light" else None)
    # sky texture SRT (ss.nsbta): per material and frame the S/T translation (fx32)
    srt = g3d.parse_bta((root / "mod/ss.nsbta").read_bytes())[0]
    b = bytearray(struct.pack("<HH", srt.num_frame, len(srt.mats)))
    for mname, ch in srt.mats:
        b += mname.encode()[:15].ljust(16, b"\0")
        for f in range(srt.num_frame):
            ts = srt.value(ch[3], f)
            tt = srt.value(ch[4], f)
            ts = ts[0] if isinstance(ts, tuple) else ts
            tt = tt[0] if isinstance(tt, tuple) else tt
            b += struct.pack("<ii", ts, tt)
    blobs.append(("sky_srt", 0, bytes(b)))
    # narc/ss_player_lz7.narc (son.nsbmd/nsbca/nsbva) and narc/ss_act_lz7.narc (effects)
    for narc in ("ss_player_lz7", "ss_act_lz7"):
        files_in = dict((n, N.maybe_decompress(d)) for n, d in N.open_narc(root / "narc" / (narc + ".narc")).files)
        pat = None
        if "eff_shoot_bomb.nsbtp" in files_in:
            pat = pattern_of(files_in["eff_shoot_bomb.nsbtp"], "eff_shoot_bomb")
            blobs.append(("pat_bomb", 0, pat["table"]))
        for name, data in files_in.items():
            stem = "narc__%s__%s" % (narc, name.rsplit(".", 1)[0])
            if data[:4] == b"BMD0":
                emit_bmd(data, stem, pat if name.startswith("eff_shoot_bomb") else None)
            elif data[:4] == b"BCA0":
                emit_anim(data, stem + ".dca")
            elif data[:4] == b"BVA0":
                emit_anim(data, stem + ".dcv")
        if narc == "ss_player_lz7":
            # node names of son.nsbmd (SpecialStagePlayer_RHandMtxCallback looks up "r_hand")
            r = g3d.parse_bmd(files_in["son.nsbmd"])
            nb = bytearray()
            for m in r["models"]:
                for n in m["nodes"]:
                    nb += n["name"].encode()[:15].ljust(16, b"\0")
                nb += b"\0" * 16
            blobs.append(("son_nodes", 0, bytes(nb)))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--extract", default=str(ROOT / "extract/eur"))
    ap.add_argument("--out", default=str(ROOT / "build/dreamcast/special"))
    ap.add_argument("--audio", default=str(ROOT / "build/dreamcast/audio"))
    ap.add_argument("--skip-sprites", action="store_true")
    ap.add_argument("--skip-sound", action="store_true")
    a = ap.parse_args()
    ext = Path(a.extract)
    out = Path(a.out)
    out.mkdir(parents=True, exist_ok=True)
    files = []
    blobs = []
    # ---- ROM tables
    ov1 = (ext / "arm9_overlays/ov001.bin").read_bytes()
    arm9 = (ext / "arm9/arm9.bin").read_bytes()
    for lo, hi in OV1_RANGES:
        blobs.append(("ov1", lo, ov1[lo - OV_BASE:hi - OV_BASE]))
    for lo, hi in ARM9_RANGES:
        blobs.append(("arm9", lo, arm9[lo - ARM9_BASE:hi - ARM9_BASE]))
    syms = load_symbols()
    fb = bytearray()
    for name, (addr, kind) in sorted(syms.items(), key=lambda kv: kv[1][0]):
        if kind.startswith("function") and OV_BASE <= addr < OV_BASE + len(ov1) and name.startswith("SpecialStage"):
            if any(lo <= addr < hi for lo, hi in [(0x02306000, 0x02317000)]):
                fb += struct.pack("<I", addr) + name.encode()[:59].ljust(60, b"\0")
    blobs.append(("funcs", 0, bytes(fb)))
    print("rom tables: %d bytes, %d function symbols" % (sum(len(b) for n, _, b in blobs if n in ("ov1", "arm9")), len(fb) // 64))
    # ---- stage scripts
    (out / "ss").mkdir(exist_ok=True)
    for p in sorted((ext / "files/ss").iterdir()):
        shutil.copyfile(p, out / "ss" / p.name)
        files.append("ss/" + p.name)
    # ---- models
    print("models:")
    convert_models(ext, out, blobs, files)
    blob_file(blobs, out / "spdata.bin")
    files.append("spdata.bin")
    # ---- backgrounds
    import menubg
    (out / "bg").mkdir(exist_ok=True)
    for p in sorted((ext / "files/bg").glob("sp_*.bbg")):
        n, b, W, H, op = menubg.convert(p, out / "bg")
        files.append("bg/%s.mnbg" % n)
    # ---- sprites
    if not a.skip_sprites:
        subprocess.check_call([sys.executable, str(ROOT / "tools/dreamcast/sprites/bac2dc.py"), "--extract", str(ext),
                               "--out", str(out / "sprites_full"), "--only", "act/ac_spst", "--no-preview"],
                              stdout=subprocess.DEVNULL)
        sd = out / "sprites"
        sd.mkdir(exist_ok=True)
        for d in sorted((out / "sprites_full/bac").iterdir()):
            if d.name.startswith("act__ac_spst"):
                (sd / d.name).mkdir(exist_ok=True)
                for f in ["frames.bin"] + [x.name for x in d.glob("atlas*.argb1555")] + \
                        [x.name for x in d.glob("atlas*.pt4")] + (["palettes.bin"] if (d / "palettes.bin").exists() else []):
                    shutil.copyfile(d / f, sd / d.name / f)
                    files.append("sprites/%s/%s" % (d.name, f))
        shutil.rmtree(out / "sprites_full")
    # ---- sound-effect bank: sys SDAT, sequence archives 9 / 14 / 16 only
    if not a.skip_sound:
        import sdatlib
        import sfxbank
        keep = {9, 14, 16}
        orig = sdatlib.SDAT

        class Sub(orig):
            def __init__(self, *aa, **kk):
                super().__init__(*aa, **kk)
                self.info["seqarc"] = [x if i in keep else None for i, x in enumerate(self.info["seqarc"])]
        sdatlib.SDAT = Sub
        tmp = out / "sound_tmp"
        ns = types.SimpleNamespace(extract=str(ext), audio=a.audio, out=str(tmp), only="sys",
                                   budget=sfxbank.DEFAULT_BUDGET)
        r = sfxbank.Builder(ns).build("sys")
        sdatlib.SDAT = orig
        (out / "sound").mkdir(exist_ok=True)
        shutil.copyfile(tmp / "sys.srb", out / "sound/ss.srb")
        shutil.rmtree(tmp)
        files.append("sound/ss.srb")
        print("sound bank: %d B sound RAM (budget %d), %d waves" % (r["sound_ram_bytes"], sfxbank.DEFAULT_BUDGET, r["waves"]))
    (out / "manifest.txt").write_text("\n".join(files) + "\n")
    print("%d files -> %s" % (len(files), out))


if __name__ == "__main__":
    main()

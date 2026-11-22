#!/usr/bin/env python3
"""musictab.py - stream table + disc music directory for the Sonic Rush DC port.

Bring-your-own-ROM: reads the renders made by sdat2dc.py (build/dreamcast/audio/
music/ + manifest.json), the SDAT INFO records and the stage BGM table of the
owner's arm9.bin. Nothing from the ROM goes into source.

Why: the original selects music by (loaded sound archive, sequence number):
  Stage_PlayBGM            data_02074850[zone][act][0 / 1] in the zone SDAT
                           (two layers, B = the character's track, A = the
                           alternate mix; Stage_SelectBGMLayer switches them:
                           Water Palace underwater, Dead Line gravity flip)
  boss acts                Sound_PlaySeq(gBGMSoundHandle, table[zone][2][0])
  Boss*_OnHit / pinch      Sound_PlaySeq(0, 0xf / 0x11 / 0x13), Extra 0x14 / 0x15
  StageJingle_Create       0x16 SEQ_muteki (invincibility), 0x17 SEQ_brethless
  Sound_PlayBGMOnPlayer3   0x18 SEQ_1up
  ClearDemo / GameOver / special stage / menus: sequences of the sys SDAT.
The port streams each render; this tool writes the lookup the runtime
(dreamcast/game/sound/bgm.c) uses: (sdat, seq) -> file, loop, layer, player.

Layer pairs: both layers of a stage track are the same SSEQ file with another
bank (z2s/z2b/z7s/z7b). They are re-rendered with ONE common loop (the
renderer found a slightly different loop start for each) and interleaved
into one .lyr file (32 KB blocks of each layer in turn, one sequential disc
read for both); engine/audio.c plays the layers as two AICA streams started
together, so the switch is sample-exact like the DS (both sequences run, one
at volume 0).

Output: <out> (default build/dreamcast/audio/disc_music) = the /cd/music
directory of the disc:
  streams.txt                  "sdat seq file loop_start loop_end layer player ram name"
                               (file "-" = the sequence is silent in that SDAT)
  st/<sdat>_<seq>.wav          canonical renders (duplicates resolved)
  st/<sdat>_<b>_<a>.lyr        layer pairs
  sNN.wav + sys.txt            legacy names of the sys sequences (menus, cutscenes,
                               special stage), now including the duplicates
                               (SEQ_ACT_CLEAR 15, SEQ_BOSS_CLEAR 16, SEQ_BOSS_TALK 22)

Usage: musictab.py [--extract extract/eur] [--audio build/dreamcast/audio] [--out DIR]
                   [--no-layers]
"""
import argparse
import json
import math
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import wave
from pathlib import Path

import numpy as np

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import sdatlib  # noqa: E402

BGM_TABLE = 0x02074850          # data_02074850 / data_02074852: u16 [9][3][2] (Stage_PlayBGM)
ARM9_BASE = 0x02000000
LYR_BLOCK = 32768               # bytes per layer per block (= frames, stereo ADPCM 1 byte/frame)
LYR_HDR = 2048                  # one sector: layer data is sector aligned
# gStageZoneFileListTable[zone][act].entries[3] (sound_stage_sdat in sound.c); the
# last letter is the character (StageLoader_SetCharacterSoundPath)
STAGE_SDAT = [["z1s", "z1s", "z1es"], ["z2s", "z2s", "z2es"], ["z3s", "z3s", "z3es"],
              ["z4s", "z4s", "z4es"], ["z5s", "z5s", "z5es"], ["z6s", "z6s", "z6es"],
              ["z7s", "z7s", "z7es"], ["z1s", "z3s", "zfes"], ["z4s", "z5s", None]]
RAM_SEQS = {22, 23, 24}         # stage SDAT jingles (muteki, brethless, 1up): held in RAM


def stage_sdat(zone, act, chara):
    n = STAGE_SDAT[zone][act]
    return None if n is None else n[:-1] + ("s" if chara == 0 else "b")


def read_bgm_table(extract):
    d = Path(extract, "arm9", "arm9.bin").read_bytes()
    t = struct.unpack_from("<54H", d, BGM_TABLE - ARM9_BASE)
    tab = [[(t[z * 6 + a * 2], t[z * 6 + a * 2 + 1]) for a in range(3)] for z in range(9)]
    assert all(v < 32 for v in t), "data_02074850 not found (unexpected arm9.bin)"
    return tab


def seqno(e):
    return int(re.search(r"SEQ#(\d+)", e["source"]).group(1))


def seqfile(e):
    return int(re.search(r" file(\d+)", e["source"]).group(1))


def wav_pcm(p):
    with wave.open(str(p)) as w:
        n, ch = w.getnframes(), w.getnchannels()
        return np.frombuffer(w.readframes(n), np.int16).reshape(-1, ch).copy(), w.getframerate()


def write_pcm(p, x, rate):
    with wave.open(str(p), "wb") as w:
        w.setnchannels(x.shape[1]); w.setsampwidth(2); w.setframerate(rate); w.writeframes(x.astype(np.int16).tobytes())


def adpcm_data(p):
    b = Path(p).read_bytes()
    o = 12
    while o + 8 <= len(b):
        cid, sz = b[o:o + 4], struct.unpack_from("<I", b, o + 4)[0]
        if cid == b"data":
            return b[o + 8:o + 8 + sz]
        o += 8 + sz + (sz & 1)
    raise ValueError("no data chunk in %s" % p)


class Tab:
    def __init__(self, a):
        self.a = a
        self.audio = Path(a.audio)
        self.out = Path(a.out)
        self.man = json.loads((self.audio / "manifest.json").read_text())
        self.music = {}
        for e in self.man["entries"]:
            if e["kind"] == "music":
                self.music[(e["sdat"], seqno(e))] = e
        self.sdat = {}
        self.rows = []
        self.report = {"layers": [], "files": 0, "bytes": 0}

    def sd(self, name):
        if name not in self.sdat:
            self.sdat[name] = sdatlib.SDAT(Path(self.a.extract, "files", "snd", name, "sound_data.sdat"))
        return self.sdat[name]

    def canonical(self, e):
        """output path (relative to audio/) of a music entry, or None if silent/failed"""
        if e["status"] == "ok":
            return e["output"]
        if e["status"] == "duplicate":
            return e["same_as"]
        return None

    def copy_wav(self, rel):
        # st/<sdat>_<seq>.wav named after the canonical entry (first rendering)
        src = self.audio / rel
        ce = next(x for x in self.man["entries"] if x.get("output") == rel)
        name = "st/%s_%02d.wav" % (ce["sdat"], seqno(ce))
        dst = self.out / name
        if not dst.exists():
            dst.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy(src, dst)
            self.report["files"] += 1
            self.report["bytes"] += dst.stat().st_size
        return name

    # ------------------------------------------------------------- layers
    def layer_pairs(self, tab):
        pairs = {}
        for z in range(9):
            for act in range(2):
                for ch in (0, 1):
                    sdn = stage_sdat(z, act, ch)
                    if not sdn:
                        continue
                    s0, s1 = tab[z][act]
                    b, a = (s0, s1) if ch == 0 else (s1, s0)
                    eb, ea = self.music.get((sdn, b)), self.music.get((sdn, a))
                    if not eb or not ea or not self.canonical(eb) or not self.canonical(ea):
                        continue
                    if seqfile(eb) != seqfile(ea):
                        continue
                    key = (sdn, min(b, a), max(b, a))
                    pairs[key] = (eb, ea)
        return sorted(pairs)

    def render_pcm(self, conv, sdn, s, tmp):
        sd = self.sd(sdn)
        q = sd.info["seq"][s]
        pcm = tmp / ("%s_%d.wav" % (sdn, s))
        st = conv.render(sd, q["file"], 0, q["bank"], q["vol"], q["ppr"], q["player"], 900, pcm)
        return pcm, st

    def build_layers(self, pairs):
        if not pairs:
            return {}
        import sdat2dc
        tmp = Path(tempfile.mkdtemp(prefix="musictab_"))
        ns = argparse.Namespace(out=str(self.audio), tmp=None, interp=False, jobs=1,
                                extract=self.a.extract, wav2adpcm="wav2adpcm", keep_pcm=None)
        conv = sdat2dc.Converter(ns)
        renderer = conv.renderer
        out = {}
        try:
            for sdn, s0, s1 in pairs:
                pcms, sts = [], []
                for s in (s0, s1):
                    p, st = self.render_pcm(conv, sdn, s, tmp)
                    pcms.append(p); sts.append(st)
                if not all(st["loop"] for st in sts):
                    print("[musictab] %s %d/%d: not looping, no layer file" % (sdn, s0, s1)); continue
                per = [st["loop_end"] - st["loop_start"] for st in sts]
                if per[0] != per[1]:
                    print("[musictab] %s %d/%d: loop lengths differ %s, no layer file" % (sdn, s0, s1, per)); continue
                ls = max(st["loop_start"] for st in sts)
                le = ls + per[0]
                enc = []
                for p, st in zip(pcms, sts):
                    x, rate = wav_pcm(p)
                    x = x[:st["loop_end"]]
                    if len(x) < le:   # periodic from its own loop start: x[t] = x[t - period]
                        ext = x[st["loop_start"]:st["loop_start"] + (le - len(x))]
                        x = np.concatenate([x, ext])
                    write_pcm(p, x, rate)
                    q = p.with_suffix(".adpcm.wav")
                    r = subprocess.run([str(renderer), "aicaenc", str(p), str(q), "--loop", "%d,%d" % (ls, le)],
                                       capture_output=True, text=True, check=True)
                    rep = json.loads(r.stdout)
                    ok = all(c.get("joined") and c.get("tail_samples_changed") == 0 for c in rep["ch"])
                    enc.append((adpcm_data(q), rate, ok, rep))
                if enc[0][1] != enc[1][1] or len(enc[0][0]) != le or len(enc[1][0]) != le:
                    print("[musictab] %s %d/%d: encoded sizes %d/%d != %d" % (sdn, s0, s1, len(enc[0][0]), len(enc[1][0]), le)); continue
                name = "st/%s_%02d_%02d.lyr" % (sdn, s0, s1)
                dst = self.out / name
                dst.parent.mkdir(parents=True, exist_ok=True)
                nblk = (le + LYR_BLOCK - 1) // LYR_BLOCK
                hdr = bytearray(LYR_HDR)
                struct.pack_into("<4s9I", hdr, 0, b"SRLY", 1, 2, enc[0][1], le, ls, le, LYR_BLOCK, LYR_HDR, nblk)
                with open(dst, "wb") as f:
                    f.write(hdr)
                    for k in range(nblk):
                        for d, *_ in enc:
                            blk = d[k * LYR_BLOCK:(k + 1) * LYR_BLOCK]
                            f.write(blk + b"\0" * (LYR_BLOCK - len(blk)))
                self.report["files"] += 1
                self.report["bytes"] += dst.stat().st_size
                self.report["layers"].append({"file": name, "sdat": sdn, "seqs": [s0, s1], "loop": [ls, le],
                                              "own_loops": [[st["loop_start"], st["loop_end"]] for st in sts],
                                              "seam_exact": [e[2] for e in enc], "bytes": dst.stat().st_size})
                out[(sdn, s0)] = (name, ls, le, 0)
                out[(sdn, s1)] = (name, ls, le, 1)
                print("[musictab] %s layers %d/%d -> %s loop %d-%d (own %s), seam exact %s" %
                      (sdn, s0, s1, name, ls, le, [[st["loop_start"], st["loop_end"]] for st in sts], [e[2] for e in enc]))
        finally:
            shutil.rmtree(tmp, ignore_errors=True)
            shutil.rmtree(conv.tmp, ignore_errors=True)
            conv.pool.shutdown()
        return out

    # ------------------------------------------------------------- table
    def run(self):
        if self.out.exists():
            shutil.rmtree(self.out)
        (self.out / "st").mkdir(parents=True)
        tab = read_bgm_table(self.a.extract)
        layers = {} if self.a.no_layers else self.build_layers(self.layer_pairs(tab))
        lines = ["# sdat seq file loop_start loop_end layer player ram name  (musictab.py)"]
        sysrows = []
        for (sdn, s), e in sorted(self.music.items()):
            q = self.sd(sdn).info["seq"][s]
            player = q["player"] if q else 0
            ram = 1 if sdn not in ("sys", "extra_end") and s in RAM_SEQS else 0
            rel = self.canonical(e)
            if (sdn, s) in layers:
                name, ls, le, layer = layers[(sdn, s)]
                lines.append("%s %d %s %d %d %d %d 0 %s" % (sdn, s, name, ls, le, layer, player, e["name"]))
                continue
            if rel is None:
                lines.append("%s %d - 0 0 0 %d 0 %s" % (sdn, s, player, e["name"]))
                continue
            name = self.copy_wav(rel)
            ls, le = (e["loop_start"], e["loop_end"]) if e.get("loop") else (0, 0)
            lines.append("%s %d %s %d %d 0 %d %d %s" % (sdn, s, name, ls, le, player, ram, e["name"]))
            if sdn == "sys":
                shutil.copy(self.out / name, self.out / ("s%02d.wav" % s))
                sysrows.append("%d %d %d" % (s, ls, le))
        # data_02074850 / data_02074852 (Stage_PlayBGM): "bgmtab zone act seq0 seq1"
        for z in range(9):
            for act in range(3):
                lines.append("bgmtab %d %d %d %d" % (z, act, tab[z][act][0], tab[z][act][1]))
        (self.out / "streams.txt").write_text("\n".join(lines) + "\n")
        (self.out / "sys.txt").write_text("\n".join(sysrows) + "\n")
        # stage BGM selection (for the report / AUDIO.md)
        sel = []
        for z in range(9):
            for act in range(3):
                for ch in (0, 1):
                    sdn = stage_sdat(z, act, ch)
                    if sdn is None:
                        continue
                    s0, s1 = tab[z][act]
                    b, a = (s0, s1) if (ch == 0 or act == 2) else (s1, s0)
                    nm = lambda s: (self.music.get((sdn, s)) or {}).get("name", "?") + \
                        ("" if self.canonical(self.music.get((sdn, s), {"status": "x"})) else "(silent)")
                    sel.append({"zone": z, "act": act, "chara": ch, "sdat": sdn, "B": [b, nm(b)],
                                "A": None if act == 2 else [a, nm(a)]})
        self.report["stage_bgm"] = sel
        self.report["entries"] = sum(1 for l in lines if not l.startswith(("#", "bgmtab")))
        (self.out / "musictab.json").write_text(json.dumps(self.report, indent=1))
        print("[musictab] %d entries, %d files, %.1f MB -> %s" % (self.report["entries"], self.report["files"],
                                                                 self.report["bytes"] / 1e6, self.out))
        return 0


def main():
    root = HERE.parents[2]
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--extract", default=str(root / "extract" / "eur"))
    ap.add_argument("--audio", default=str(root / "build" / "dreamcast" / "audio"))
    ap.add_argument("--out", default=None)
    ap.add_argument("--no-layers", action="store_true")
    a = ap.parse_args()
    if a.out is None:
        a.out = str(Path(a.audio) / "disc_music")
    return Tab(a).run()


if __name__ == "__main__":
    sys.exit(main())

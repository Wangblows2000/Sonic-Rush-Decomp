#!/usr/bin/env python3
"""sdat2dc.py - convert every Sonic Rush SDAT into Dreamcast (AICA ADPCM) audio.

Bring-your-own-ROM: reads extract/eur/files/snd/*/sound_data.sdat plus the
sound-driver tables in extract/eur/arm7/arm7.bin and arm9/arm9.bin, writes
build/dreamcast/audio/ (gitignored). Nothing from the ROM goes into source.

Outputs (all KOS wav2adpcm WAVs, Yamaha/AICA 4-bit ADPCM):
  music/<sdat>/<SEQ name>.wav        every SSEQ rendered offline (sseqrender.c)
                                     stereo, interleaved (fmt 0x20, for snd_stream),
                                     DS mixer rate 32728 Hz, intro + one loop
  sfx/<sdat>/<SSAR name>/<seq>.wav   every SSAR sequence rendered as a one-shot
                                     (or intro+loop if it loops); mono when L==R,
                                     else stereo non-interleaved (fmt 0x14, snd_sfx)
  waves/<sdat>/<SWAR name>/NNNN.wav  every raw SWAR wave at its native rate (mono)
  strm/<sdat>/<STRM name>.wav        every STRM (stereo interleaved, native rate)
  manifest.json                      source, SYMB name, rate, channels, loop start/end
                                     (sample frames), duration, sizes, render stats

Identical outputs are written once; later entries carry "same_as".

Usage:
  python3 tools/dreamcast/audio/sdat2dc.py [--extract extract/eur]
      [--out build/dreamcast/audio] [--wav2adpcm PATH] [--only z1s,sys]
      [--skip music,sfx,waves,strm] [--jobs N] [--keep-pcm DIR] [--interp]
"""
import argparse
import concurrent.futures as cf
import hashlib
import json
import os
import shutil
import subprocess
import sys
import tempfile
import time
import wave
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import sdatlib  # noqa: E402

MUSIC_MAX_SEC = 900
SFX_MAX_SEC = 30
SFX_HOLD_SEC = 4  # held (never-ending, non-looping) SFX are stopped here, see SOUND.md
# Port mix gain, the ONE shared gain of music and effects (docs/dreamcast/AUDIO.md
# "Mix level"): 2^(-MIX_GAIN_TL/16), in AICA TL steps (0.375 dB). Applied to every
# render here (after the DS master volume) and, through manifest.json -> sfxbank.py
# -> .srb header, to the runtime effects (dreamcast/game/sound/snd_drv.c), so
# music and effects keep the DS ratio with headroom: on the DS the summed mix is
# clamped at the same full scale, and the DS-level BGM renders alone already
# peak up to +4.4 dBFS (z1 +1.4 dBFS), a doubled SE_Spring reaches +4 dBFS.
MIX_GAIN_TL = 24  # -9.03 dB


def build_renderer(out_bin):
    src = HERE / "sseqrender.c"
    if not out_bin.exists() or out_bin.stat().st_mtime < src.stat().st_mtime:
        out_bin.parent.mkdir(parents=True, exist_ok=True)
        subprocess.run(["gcc", "-O2", "-o", str(out_bin), str(src), "-lm"], check=True)
    return out_bin


def wav_info(p):
    with wave.open(str(p)) as w:
        return w.getnchannels(), w.getframerate(), w.getnframes()


def pcm_sha(p):
    with wave.open(str(p)) as w:
        return hashlib.sha1(w.readframes(w.getnframes()) + bytes([w.getnchannels()]) +
                            w.getframerate().to_bytes(4, "little")).hexdigest()


def to_mono_if_same(p):
    """Rewrite a stereo PCM16 WAV as mono if both channels are (near) identical."""
    import numpy as np
    with wave.open(str(p)) as w:
        ch, rate, n = w.getnchannels(), w.getframerate(), w.getnframes()
        x = np.frombuffer(w.readframes(n), np.int16)
    if ch != 2:
        return 1
    x = x.reshape(-1, 2)
    if n and np.abs(x[:, 0].astype(int) - x[:, 1]).max() > 2:
        return 2
    m = ((x[:, 0].astype(int) + x[:, 1]) // 2).astype(np.int16)
    with wave.open(str(p), "wb") as w:
        w.setnchannels(1); w.setsampwidth(2); w.setframerate(rate); w.writeframes(m.tobytes())
    return 1


def pad_even(p):
    """wav2adpcm packs two samples per byte: pad an odd frame count with one silent frame."""
    ch, rate, n = wav_info(p)
    if n % 2 == 0:
        return n
    with wave.open(str(p)) as w:
        data = w.readframes(n)
    with wave.open(str(p), "wb") as w:
        w.setnchannels(ch); w.setsampwidth(2); w.setframerate(rate)
        w.writeframes(data + b"\0\0" * ch)
    return n


def encode(wav2adpcm, pcm, out, interleaved):
    out.parent.mkdir(parents=True, exist_ok=True)
    cmd = [wav2adpcm] + (["-i"] if interleaved else []) + ["-t", str(pcm), str(out)]
    subprocess.run(cmd, check=True, stdout=subprocess.DEVNULL)
    return out.stat().st_size


def safe(name):
    return "".join(c if c.isalnum() or c in "_-." else "_" for c in name)


class Converter:
    def __init__(self, a):
        self.a = a
        self.out = Path(a.out)
        self.tmp = Path(tempfile.mkdtemp(prefix="sdat2dc_", dir=a.tmp))
        self.renderer = build_renderer(self.out / "bin" / "sseqrender")
        tables, tinfo = sdatlib.extract_driver_tables(a.extract)
        # + ARM9 SNDi_DecibelTable (data_020725e0): NNS player volume, NNSi_SndPlayerMain
        dblin, tinfo["arm9_nns_decibel"] = sdatlib.nns_decibel_table(a.extract)
        self.tables = self.out / "tables.bin"
        self.tables.write_bytes(tables + dblin)
        self.manifest = {"tool": "tools/dreamcast/audio/sdat2dc.py", "renderer": "tools/dreamcast/audio/sseqrender.c",
                         "interpolation": "linear" if a.interp else "none (DS-like)",
                         "encoder": f"KOS wav2adpcm ({a.wav2adpcm}); music: sseqrender aicaenc (same bytes + exact loop-seam state)", "driver_tables": tinfo,
                         "music_rate": 32728, "mix_gain_tl": MIX_GAIN_TL,
                         "mix_gain_db": round(-MIX_GAIN_TL * 20 * 0.0188144, 2), "entries": []}
        self.seen = {}          # pcm hash -> output relpath
        self.counts = {}
        self.failures = []
        self.pool = cf.ThreadPoolExecutor(max_workers=a.jobs)

    def count(self, k, n=1):
        self.counts[k] = self.counts.get(k, 0) + n

    # ---------------- sequences ----------------
    def render(self, sd, seq_file, start, bank_idx, vol, ppr, player, max_sec, pcm, hold_sec=0):
        info = sd.info
        bank_file = info["bank"][bank_idx]["file"] if bank_idx < len(info["bank"]) and info["bank"][bank_idx] else -1
        swar = sd.bank_swar_files(bank_idx) if bank_file >= 0 else [-1] * 4
        mask = 0
        if player < len(info["player"]) and info["player"][player]:
            mask = info["player"][player]["mask"]
        cmd = [str(self.renderer), "render", "--sdat", str(sd.path), "--tables", str(self.tables),
               "--seq", str(seq_file), "--start", str(start), "--bank", str(bank_file),
               "--swar", ",".join(map(str, swar)), "--vol", str(vol), "--ppr", str(ppr),
               "--mask", hex(mask), "--max-sec", str(max_sec), "--hold-sec", str(hold_sec),
               "--gain-tl", str(MIX_GAIN_TL), "--out", str(pcm)] + (["--interp"] if self.a.interp else [])
        r = subprocess.run(cmd, capture_output=True, text=True)
        if r.returncode:
            raise RuntimeError(r.stderr.strip())
        st = json.loads(r.stdout)
        st.update(bank_file=bank_file, swar_files=swar, channel_mask=mask)
        return st

    def finish_seq(self, kind, sdname, name, rel, pcm, st, source, interleaved_stereo):
        e = {"kind": kind, "sdat": sdname, "name": name, "source": source}
        if st["samples"] == 0 or st["peak"] == 0:
            why = ("no note found an instrument in its bank (program/key absent)" if st["notes"] == 0 and st["inst_missing"]
                   else "no notes" if st["notes"] == 0 else "notes produced no signal")
            e.update(status="silent", reason=why, render=st)
            self.count(f"{kind}_silent")
            return e
        chans = 2
        if kind == "sfx":
            chans = to_mono_if_same(pcm)
        h = pcm_sha(pcm)
        loop = st["loop"]
        e.update(rate=st["rate"], channels=chans, samples=st["samples"],
                 duration=round(st["samples"] / st["rate"], 3),
                 loop=loop, loop_start=st["loop_start"] if loop else None,
                 loop_end=st["loop_end"] if loop else None,
                 render={k: st[k] for k in ("ended", "hit_max", "loop_start_tick", "loop_len_ticks", "tail_frames",
                                            "lcm_capped", "seam_wave_rms", "gain_tl", "ds_peak", "peak", "rms", "clipped", "max_voices",
                                            "steals", "notes", "held_stopped", "inst_missing", "wave_missing", "alloc_fail", "first_pass_differs", "converged", "bank_file", "swar_files", "channel_mask")})
        if kind == "sfx":
            e["fits_snd_sfx"] = st["samples"] <= 65534
        if h in self.seen:
            e.update(status="duplicate", same_as=self.seen[h])
            self.count(f"{kind}_duplicate")
            return e
        self.seen[h] = rel
        if kind == "music":
            # sseqrender aicaenc: same quantiser / bytes as wav2adpcm -i -t, plus a loop
            # seam whose decoder state at loop_end equals the state at loop_start
            # (the snd_stream long-stream decoder is never reset on the wrap)
            (self.out / rel).parent.mkdir(parents=True, exist_ok=True)
            cmd = [str(self.renderer), "aicaenc", str(pcm), str(self.out / rel)]
            if loop:
                cmd += ["--loop", f"{st['loop_start']},{st['loop_end']}"]
            r = subprocess.run(cmd, capture_output=True, text=True, check=True)
            enc = json.loads(r.stdout)
            e["adpcm_loop"] = enc["ch"]
            if loop:
                ok = all(c.get("joined") and c.get("tail_samples_changed") == 0 for c in enc["ch"])
                self.count("music_loop_seam_exact" if ok else "music_loop_seam_NOT_exact")
            size = (self.out / rel).stat().st_size
        else:
            pad_even(pcm)
            size = encode(self.a.wav2adpcm, pcm, self.out / rel, interleaved=(chans == 2 and interleaved_stereo))
        e.update(status="ok", output=rel, adpcm_bytes=size, pcm16_bytes=st["samples"] * chans * 2,
                 format="0x20 interleaved stereo" if chans == 2 and interleaved_stereo else
                 ("0x14 non-interleaved stereo" if chans == 2 else "0x14 mono"))
        if self.a.keep_pcm:
            kp = Path(self.a.keep_pcm) / rel
            kp.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy(pcm, kp)
        self.count(f"{kind}_ok")
        self.count(f"{kind}_bytes", size)
        return e

    def do_music(self, sd, sdname):
        futs = []
        for i, q in enumerate(sd.info["seq"]):
            if q is None:
                continue
            name = sd.name("seq", i, "SEQ")
            pcm = self.tmp / f"{sdname}_seq{i}.wav"
            src = f"{sdname}/sound_data.sdat SEQ#{i} file{q['file']} bank{q['bank']} vol{q['vol']} player{q['player']}"
            futs.append((i, name, pcm, src, self.pool.submit(
                self.render, sd, q["file"], 0, q["bank"], q["vol"], q["ppr"], q["player"], MUSIC_MAX_SEC, pcm)))
        for i, name, pcm, src, f in futs:
            try:
                st = f.result()
                e = self.finish_seq("music", sdname, name, f"music/{sdname}/{safe(name)}.wav", pcm, st, src, True)
            except Exception as ex:  # noqa: BLE001
                e = {"kind": "music", "sdat": sdname, "name": name, "source": src, "status": "failed", "error": str(ex)}
                self.failures.append(e); self.count("music_failed")
            self.manifest["entries"].append(e)
            pcm.unlink(missing_ok=True)

    def do_sfx(self, sd, sdname):
        for ai, a in enumerate(sd.info["seqarc"]):
            if a is None:
                continue
            aname = sd.name("seqarc", ai, "SSAR")
            sub = sd.ssar_names[ai] if ai < len(sd.ssar_names) else []
            futs = []
            for si, rec in enumerate(sd.ssar_records(a["file"])):
                if rec is None:
                    continue
                off, bank, vol, cpr, ppr, ply = rec
                name = sub[si] if si < len(sub) and sub[si] else f"SE_{si:03d}"
                pcm = self.tmp / f"{sdname}_ssar{ai}_{si}.wav"
                src = f"{sdname}/sound_data.sdat SSAR#{ai} file{a['file']} seq{si} off{off} bank{bank} vol{vol} player{ply}"
                futs.append((si, name, pcm, src, self.pool.submit(
                    self.render, sd, a["file"], off, bank, vol, ppr, ply, SFX_MAX_SEC, pcm, SFX_HOLD_SEC)))
            for si, name, pcm, src, f in futs:
                rel = f"sfx/{sdname}/{safe(aname)}/{si:03d}_{safe(name)}.wav"
                try:
                    st = f.result()
                    e = self.finish_seq("sfx", sdname, name, rel, pcm, st, src, False)
                    e["archive"] = aname
                except Exception as ex:  # noqa: BLE001
                    e = {"kind": "sfx", "sdat": sdname, "name": name, "archive": aname, "source": src,
                         "status": "failed", "error": str(ex)}
                    self.failures.append(e); self.count("sfx_failed")
                self.manifest["entries"].append(e)
                pcm.unlink(missing_ok=True)

    # ---------------- raw waves ----------------
    def do_waves(self, sd, sdname):
        for wi, w in enumerate(sd.info["wavearc"]):
            if w is None:
                continue
            wname = sd.name("wavearc", wi, "SWAR")
            d = self.tmp / f"{sdname}_swar{wi}"
            d.mkdir(exist_ok=True)
            r = subprocess.run([str(self.renderer), "swar", "--sdat", str(sd.path), "--file", str(w["file"]),
                                "--outdir", str(d)], capture_output=True, text=True)
            if r.returncode:
                e = {"kind": "wave", "sdat": sdname, "name": wname, "status": "failed", "error": r.stderr.strip()}
                self.failures.append(e); self.manifest["entries"].append(e); self.count("wave_failed")
                continue
            infos = json.loads(r.stdout)
            jobs = []
            for wv in infos:
                if wv is None:
                    self.count("wave_failed")
                    continue
                pcm = d / f"{wv['index']:04d}.wav"
                rel = f"waves/{sdname}/{safe(wname)}/{wv['index']:04d}.wav"
                e = {"kind": "wave", "sdat": sdname, "name": f"{wname}/{wv['index']:04d}", "archive": wname,
                     "source": f"{sdname}/sound_data.sdat SWAR#{wi} file{w['file']} wave{wv['index']}",
                     "ds_format": ["PCM8", "PCM16", "IMA-ADPCM"][wv["format"]], "rate": wv["rate"],
                     "rate_field": wv["rate_field"], "timer": wv["timer"], "channels": 1, "samples": wv["samples"],
                     "duration": round(wv["samples"] / wv["rate"], 3), "loop": wv["loop"],
                     "loop_start": wv["loop_start"] if wv["loop"] else None,
                     "loop_end": wv["samples"] if wv["loop"] else None,
                     "fits_snd_sfx": wv["samples"] <= 65534}
                h = pcm_sha(pcm)
                if h in self.seen:
                    e.update(status="duplicate", same_as=self.seen[h]); self.count("wave_duplicate")
                    self.manifest["entries"].append(e)
                    continue
                self.seen[h] = rel
                if self.a.keep_pcm:
                    kp = Path(self.a.keep_pcm) / rel
                    kp.parent.mkdir(parents=True, exist_ok=True)
                    shutil.copy(pcm, kp)
                pad_even(pcm)
                jobs.append((e, rel, self.pool.submit(encode, self.a.wav2adpcm, pcm, self.out / rel, False)))
            for e, rel, f in jobs:
                size = f.result()
                e.update(status="ok", output=rel, adpcm_bytes=size, pcm16_bytes=e["samples"] * 2, format="0x14 mono")
                self.count("wave_ok"); self.count("wave_bytes", size)
                self.manifest["entries"].append(e)
            shutil.rmtree(d, ignore_errors=True)

    # ---------------- streams ----------------
    def do_strm(self, sd, sdname):
        for i, s in enumerate(sd.info["strm"]):
            if s is None:
                continue
            name = sd.name("strm", i, "STRM")
            rel = f"strm/{sdname}/{safe(name)}.wav"
            src = f"{sdname}/sound_data.sdat STRM#{i} file{s['file']} vol{s['vol']}"
            pcm = self.tmp / f"{sdname}_strm{i}.wav"
            try:
                r = subprocess.run([str(self.renderer), "strm", "--sdat", str(sd.path), "--file", str(s["file"]),
                                    "--out", str(pcm)], capture_output=True, text=True)
                if r.returncode:
                    raise RuntimeError(r.stderr.strip())
                st = json.loads(r.stdout)
                n, ch, rate = st["samples"], st["channels"], st["rate"]
                h = pcm_sha(pcm)
                e = {"kind": "strm", "sdat": sdname, "name": name, "source": src, "rate": rate, "channels": ch,
                     "samples": n, "duration": round(n / rate, 3), "loop": st["loop"],
                     "loop_start": st["loop_start"] if st["loop"] else None, "loop_end": n if st["loop"] else None,
                     "ds_format": ["PCM8", "PCM16", "IMA-ADPCM"][st["type"]], "info_volume": s["vol"],
                     "note": "STRM volume (INFO) not applied to samples; apply at playback"}
                if h in self.seen:
                    e.update(status="duplicate", same_as=self.seen[h])
                else:
                    self.seen[h] = rel
                    if self.a.keep_pcm:
                        kp = Path(self.a.keep_pcm) / rel
                        kp.parent.mkdir(parents=True, exist_ok=True)
                        shutil.copy(pcm, kp)
                    pad_even(pcm)
                    size = encode(self.a.wav2adpcm, pcm, self.out / rel, interleaved=(ch == 2))
                    e.update(status="ok", output=rel, adpcm_bytes=size, pcm16_bytes=n * ch * 2,
                             format="0x20 interleaved stereo" if ch == 2 else "0x14 mono")
                    self.count("strm_ok"); self.count("strm_bytes", size)
            except Exception as ex:  # noqa: BLE001
                e = {"kind": "strm", "sdat": sdname, "name": name, "source": src, "status": "failed", "error": str(ex)}
                self.failures.append(e); self.count("strm_failed")
            pcm.unlink(missing_ok=True)
            self.manifest["entries"].append(e)

    def run(self):
        t0 = time.time()
        sdats = sorted(Path(self.a.extract, "files", "snd").glob("*/sound_data.sdat"))
        only = set(self.a.only.split(",")) if self.a.only else None
        skip = set(self.a.skip.split(",")) if self.a.skip else set()
        done = []
        for p in sdats:
            sdname = p.parent.name
            if only and sdname not in only:
                continue
            sd = sdatlib.SDAT(p)
            print(f"[sdat2dc] {sdname}: {sum(1 for x in sd.info['seq'] if x)} SEQ, "
                  f"{sum(1 for x in sd.info['seqarc'] if x)} SSAR, {sum(1 for x in sd.info['wavearc'] if x)} SWAR, "
                  f"{sum(1 for x in sd.info['strm'] if x)} STRM", flush=True)
            if "strm" not in skip:
                self.do_strm(sd, sdname)
            if "music" not in skip:
                self.do_music(sd, sdname)
            if "sfx" not in skip:
                self.do_sfx(sd, sdname)
            if "waves" not in skip:
                self.do_waves(sd, sdname)
            done.append(sdname)
        self.pool.shutdown()
        self.manifest["sdats"] = done
        self.manifest["counts"] = self.counts
        self.manifest["seconds"] = round(time.time() - t0, 1)
        (self.out / "manifest.json").write_text(json.dumps(self.manifest, indent=1))
        shutil.rmtree(self.tmp, ignore_errors=True)
        print(json.dumps(self.counts, indent=1))
        if self.failures:
            print(f"[sdat2dc] {len(self.failures)} failures (see manifest.json)")
        return 0


def main():
    root = HERE.parents[2]
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--extract", default=str(root / "extract" / "eur"))
    ap.add_argument("--out", default=str(root / "build" / "dreamcast" / "audio"))
    ap.add_argument("--wav2adpcm", default=shutil.which("wav2adpcm") or "/home/claude/dcwork/bin/wav2adpcm")
    ap.add_argument("--only", help="comma list of SDAT dir names (e.g. sys,z1s)")
    ap.add_argument("--skip", help="comma list of: music,sfx,waves,strm")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 2)
    ap.add_argument("--keep-pcm", help="also copy rendered/decoded PCM16 WAVs here (validation)")
    ap.add_argument("--tmp", default=None, help="scratch directory (default: system temp)")
    ap.add_argument("--interp", action="store_true",
                    help="linear sample interpolation (NOT DS-like; the DS uses none). Less aliasing, ~+2.7 dB ADPCM SNR")
    a = ap.parse_args()
    Path(a.out).mkdir(parents=True, exist_ok=True)
    return Converter(a).run()


if __name__ == "__main__":
    sys.exit(main())

#!/usr/bin/env python3
"""validate_audio.py - checks for the sdat2dc.py output (Sonic Rush DC port).

Checks (each reports counts; nothing here is required to run the converter):
  swav    every SWAR wave decoded by sseqrender (`swar`) vs an independent
          decoder (loveemu's nsswav from sdatxtract, built as a harness with
          --swav-ref) sample for sample, and its header fields vs ndspy.
  strm    every STRM decoded by sseqrender (`strm`) vs loveemu's nsstrm harness
          (--strm-ref), sample for sample.
  loops   music loop points found by the renderer vs an independent tick walk
          over ndspy's SSEQ event parse (first backward jump per track).
  seam    re-render each looping music track with --full and compare short-time
          spectra after loop start and after loop end (spectral rel. error),
          plus the sample step at the wrap of the final file.
  adpcm   decode the AICA ADPCM outputs (sseqrender aicadec) and compare with the
          kept PCM16 renders (--pcm, from sdat2dc --keep-pcm): SNR in dB.
  levels  peak / RMS / clipped samples of kept PCM renders.
  png     waveform + spectrogram PNGs for chosen tracks (--png-tracks).

Usage:
  python3 tools/dreamcast/audio/validate_audio.py --out build/dreamcast/audio
      --pcm /tmp/audio/pcm [--swav-ref /tmp/audio/val/swavref] [--strm-ref /tmp/audio/val/strmref]
      [--checks swav,loops,seam,adpcm,levels,png] [--png-dir DIR]
      [--png-tracks z1s:SEQ_1sonic,z1b:SEQ_1blaze]
"""
import argparse
import json
import subprocess
import sys
import tempfile
import wave
from pathlib import Path

import numpy as np

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import sdatlib  # noqa: E402


def read_wav(p):
    with wave.open(str(p)) as w:
        ch, rate, n = w.getnchannels(), w.getframerate(), w.getnframes()
        x = np.frombuffer(w.readframes(n), np.int16).reshape(-1, ch)
    return x, rate


def check_swav(a, man, tmp):
    import ndspy.soundWaveArchive as SWA
    res = dict(swar=0, waves=0, exact=0, mismatch=0, header_mismatch=0, ref_fail=0)
    seen = set()
    for p in sorted(Path(a.extract, "files", "snd").glob("*/sound_data.sdat")):
        sd = sdatlib.SDAT(p)
        for wi, w in enumerate(sd.info["wavearc"]):
            if w is None:
                continue
            raw = sd.file(w["file"])
            key = hash(raw)
            if key in seen:
                continue
            seen.add(key)
            res["swar"] += 1
            d = Path(tempfile.mkdtemp(dir=tmp)); dm = d / "m"; dr = d / "r"; dm.mkdir(); dr.mkdir()
            (d / "a.swar").write_bytes(raw)
            mine = json.loads(subprocess.run([str(a.renderer), "swar", "--sdat", str(p), "--file", str(w["file"]),
                                              "--outdir", str(dm)], capture_output=True, text=True, check=True).stdout)
            if a.swav_ref:
                subprocess.run([a.swav_ref, str(d / "a.swar"), str(dr)], capture_output=True, check=True)
            nd = SWA.SWAR(raw)
            for i, m in enumerate(mine):
                res["waves"] += 1
                ndw = nd.waves[i]
                # ndspy: loopOffset / totalLength are in 32-bit words
                fmt_ok = (m["format"] == int(ndw.waveType) and m["loop"] == bool(ndw.isLooped) and
                          m["rate_field"] == ndw.sampleRate and m["timer"] == ndw.time)
                spw = {0: 4, 1: 2, 2: 8}[m["format"]]
                hdr = 1 if m["format"] == 2 else 0
                exp_len = (ndw.totalLength - hdr) * spw if m["format"] == 2 else ndw.totalLength * spw
                if ndw.isLooped:
                    fmt_ok = fmt_ok and m["loop_start"] == (ndw.loopOffset - hdr) * spw
                fmt_ok = fmt_ok and m["samples"] == exp_len
                if not fmt_ok:
                    res["header_mismatch"] += 1
                if a.swav_ref:
                    rp = dr / f"{i:04d}.wav"
                    if not rp.exists():
                        res["ref_fail"] += 1
                        continue
                    x, _ = read_wav(dm / f"{i:04d}.wav")
                    y, _ = read_wav(rp)
                    if m["format"] == 0:  # reference writes PCM8 as unsigned 8-bit WAV
                        with wave.open(str(rp)) as ww:
                            y = (np.frombuffer(ww.readframes(ww.getnframes()), np.uint8).astype(np.int16) - 128) * 256
                            y = y.reshape(-1, 1)
                    if x.shape == y.shape and np.array_equal(x, y):
                        res["exact"] += 1
                    else:
                        res["mismatch"] += 1
    return res


def check_strm(a, tmp):
    res = dict(streams=0, exact=0, mismatch=0)
    for p in sorted(Path(a.extract, "files", "snd").glob("*/sound_data.sdat")):
        sd = sdatlib.SDAT(p)
        for s in sd.info["strm"]:
            if s is None:
                continue
            res["streams"] += 1
            raw = Path(tmp) / "s.strm"; raw.write_bytes(sd.file(s["file"]))
            subprocess.run([a.strm_ref, str(raw), str(Path(tmp) / "ref.wav")], check=True, capture_output=True)
            subprocess.run([str(a.renderer), "strm", "--sdat", str(p), "--file", str(s["file"]), "--out",
                            str(Path(tmp) / "mine.wav")], check=True, capture_output=True)
            x, _ = read_wav(Path(tmp) / "mine.wav"); y, _ = read_wav(Path(tmp) / "ref.wav")
            res["exact" if x.shape == y.shape and np.array_equal(x, y) else "mismatch"] += 1
    return res


def walk_ticks(sseq_events, first_event):
    """Independent tick walk of one track: returns (loop_start_tick, loop_end_tick) or None."""
    import ndspy.soundSequence as SS
    idx = {id(e): i for i, e in enumerate(sseq_events)}
    pc = idx[id(first_event)]
    tick, note_wait = 0, True
    first = {}
    stack = []
    steps = 0
    while pc < len(sseq_events) and steps < 200000:
        steps += 1
        e = sseq_events[pc]
        first.setdefault(pc, tick)
        pc += 1
        if isinstance(e, SS.NoteSequenceEvent):
            if note_wait:
                tick += e.duration
        elif isinstance(e, SS.RestSequenceEvent):
            tick += e.duration
        elif isinstance(e, SS.MonoPolySequenceEvent):
            note_wait = bool(e.value)
        elif isinstance(e, SS.JumpSequenceEvent):
            t = idx[id(e.destination)]
            if t in first and not stack:
                return first[t], tick
            pc = t
        elif isinstance(e, SS.CallSequenceEvent):
            stack.append(("call", pc)); pc = idx[id(e.destination)]
        elif isinstance(e, SS.ReturnSequenceEvent):
            if stack and stack[-1][0] == "call":
                pc = stack.pop()[1]
        elif isinstance(e, SS.BeginLoopSequenceEvent):
            stack.append(["loop", pc, e.loopCount])
        elif isinstance(e, SS.EndLoopSequenceEvent):
            if stack and stack[-1][0] == "loop":
                fr = stack[-1]
                if fr[2] == 0:
                    if len(stack) == 1 and fr[1] in first:
                        return first[fr[1]], tick
                    pc = fr[1]
                else:
                    fr[2] -= 1
                    if fr[2] == 0:
                        stack.pop()
                    else:
                        pc = fr[1]
        elif isinstance(e, (SS.EndTrackSequenceEvent,)):
            return None
        elif isinstance(e, SS.RawDataSequenceEvent):
            return None
    return None


def check_loops(a, man):
    import ndspy.soundArchive as SA
    import ndspy.soundSequence as SS
    res = dict(checked=0, agree=0, disagree=0, no_ref=0, details=[])
    cache = {}
    for e in man["entries"]:
        if e["kind"] != "music" or e["status"] != "ok" or not e["loop"]:
            continue
        sdn = e["sdat"]
        if sdn not in cache:
            cache[sdn] = SA.SDAT.fromFile(str(Path(a.extract, "files", "snd", sdn, "sound_data.sdat")))
        s = cache[sdn]
        q = dict(s.sequences)[e["name"]]
        q.parse()
        evs = q.events
        starts = [evs[0]] + [ev.firstEvent for ev in evs if isinstance(ev, SS.BeginTrackSequenceEvent)]
        loops = [walk_ticks(evs, f) for f in starts]
        loops = [l for l in loops if l and l[1] > l[0]]
        res["checked"] += 1
        if not loops:
            res["no_ref"] += 1
            continue
        S = max(l[0] for l in loops)
        L = max(l[1] - l[0] for l in loops)
        r = e["render"]
        # renderer loops on the 2nd pass when the 1st pass starts with different track state
        exp_S = S + L if r.get("first_pass_differs") else S
        ok = (exp_S == r["loop_start_tick"] and L == r["loop_len_ticks"])
        res["agree" if ok else "disagree"] += 1
        if not ok:
            res["details"].append(dict(sdat=sdn, name=e["name"], ref=(exp_S, L), renderer=(r["loop_start_tick"], r["loop_len_ticks"])))
    return res


def spec(x, n=2048):
    win = np.hanning(n)
    return np.array([np.abs(np.fft.rfft(x[i:i + n] * win)) for i in range(0, len(x) - n, n)])


def check_seam(a, man, tmp):
    res = dict(checked=0, spectral_rel_err=[], wrap_step=[], worst=[])
    tables = Path(a.out) / "tables.bin"
    for e in man["entries"]:
        if e["kind"] != "music" or e["status"] != "ok" or not e["loop"]:
            continue
        src = e["source"].split()
        sdn = e["sdat"]
        fid = int(src[2][4:]); vol = int(src[4][3:])
        r = e["render"]
        sd = sdatlib.SDAT(Path(a.extract, "files", "snd", sdn, "sound_data.sdat"))
        seqi = int(src[1].split("#")[1])
        q = sd.info["seq"][seqi]
        out = Path(tmp) / "full.wav"
        subprocess.run([str(a.renderer), "render", "--sdat", str(sd.path), "--tables", str(tables), "--seq", str(fid),
                        "--start", "0", "--bank", str(r["bank_file"]), "--swar", ",".join(map(str, r["swar_files"])),
                        "--vol", str(vol), "--ppr", str(q["ppr"]), "--mask", hex(r["channel_mask"]),
                        "--max-sec", "900", "--full", "--out", str(out)], capture_output=True, check=True)
        x, _ = read_wav(out)
        ls, le = e["loop_start"], e["loop_end"]
        n = min(len(x) - le, 2 * 32728)
        A = spec(x[ls:ls + n].mean(axis=1)); B = spec(x[le:le + n].mean(axis=1))
        den = np.sqrt((B ** 2).sum())
        err = float(np.sqrt(((A - B) ** 2).sum()) / den) if den > 0 else 0.0
        res["checked"] += 1
        res["spectral_rel_err"].append(err)
        if a.pcm and (Path(a.pcm) / e["output"]).exists():
            y, _ = read_wav(Path(a.pcm) / e["output"])
            step = float(np.abs(y[e["loop_end"] - 1].astype(int) - y[ls].astype(int)).max())
            typ = float(np.abs(np.diff(y[ls:ls + 32728].astype(int), axis=0)).mean()) or 1.0
            res["wrap_step"].append(step / typ)
        res["worst"].append((err, sdn, e["name"]))
    res["worst"] = sorted(res["worst"], reverse=True)[:5]
    for k in ("spectral_rel_err", "wrap_step"):
        v = res[k]
        res[k] = dict(n=len(v), median=float(np.median(v)) if v else None, max=float(np.max(v)) if v else None)
    return res


def check_adpcm(a, man, tmp, limit_kinds=("music", "sfx", "wave", "strm")):
    res = {}
    for e in man["entries"]:
        if e["status"] != "ok" or e["kind"] not in limit_kinds:
            continue
        pp = Path(a.pcm) / e["output"] if a.pcm else None
        if not pp or not pp.exists():
            continue
        dec = Path(tmp) / "dec.wav"
        subprocess.run([str(a.renderer), "aicadec", str(Path(a.out) / e["output"]), str(dec)], check=True)
        x, _ = read_wav(pp); y, _ = read_wav(dec)
        n = min(len(x), len(y))
        x = x[:n].astype(float); y = y[:n].astype(float)
        sig = (x ** 2).sum(); err = ((x - y) ** 2).sum()
        snr = 10 * np.log10(sig / err) if err > 0 and sig > 0 else 99.0
        r = res.setdefault(e["kind"], dict(n=0, snr=[], len_ok=0))
        r["n"] += 1; r["snr"].append(snr)
        r["len_ok"] += int(abs(len(read_wav(dec)[0]) - e["samples"]) <= 1)
    for k, r in res.items():
        v = np.array(r["snr"])
        r["snr"] = dict(median=float(np.median(v)), p5=float(np.percentile(v, 5)), min=float(v.min()))
    return res


def check_levels(a, man):
    res = {}
    for e in man["entries"]:
        if e["status"] != "ok" or not a.pcm:
            continue
        pp = Path(a.pcm) / e["output"]
        if not pp.exists():
            continue
        x, _ = read_wav(pp)
        xf = x.astype(float)
        r = res.setdefault(e["kind"], dict(n=0, peak_dbfs=[], rms_dbfs=[], files_with_fullscale=0))
        r["n"] += 1
        pk = np.abs(xf).max() if len(xf) else 0
        rms = np.sqrt((xf ** 2).mean()) if len(xf) else 0
        r["peak_dbfs"].append(20 * np.log10(max(pk, 1) / 32768))
        r["rms_dbfs"].append(20 * np.log10(max(rms, 1) / 32768))
        r["files_with_fullscale"] += int(pk >= 32767)
    for k, r in res.items():
        for f in ("peak_dbfs", "rms_dbfs"):
            v = np.array(r[f]); r[f] = dict(median=round(float(np.median(v)), 1), max=round(float(v.max()), 1),
                                            min=round(float(v.min()), 1))
    return res


def make_png(a, man):
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    out = Path(a.png_dir); out.mkdir(parents=True, exist_ok=True)
    made = []
    want = [t.split(":") for t in a.png_tracks.split(",")]
    for sdn, name in want:
        e = next((x for x in man["entries"] if x["sdat"] == sdn and x["name"] == name and x["kind"] == "music"), None)
        if not e or e["status"] not in ("ok", "duplicate"):
            continue
        rel = e.get("output") or e.get("same_as")
        x, rate = read_wav(Path(a.pcm) / rel)
        m = x.astype(float).mean(axis=1) / 32768
        t = np.arange(len(m)) / rate
        fig, ax = plt.subplots(3, 1, figsize=(14, 9), gridspec_kw=dict(height_ratios=[1, 2, 1]))
        ax[0].plot(t[::32], x[::32, 0] / 32768, lw=0.3, label="L")
        ax[0].plot(t[::32], x[::32, 1] / 32768, lw=0.3, alpha=0.6, label="R")
        for v, c in ((e["loop_start"], "g"), (e["loop_end"], "r")):
            if v is not None:
                ax[0].axvline(v / rate, color=c, lw=1)
        ax[0].set_ylim(-1, 1); ax[0].set_title(f"{sdn} {name}: waveform (green=loop start, red=loop end)")
        ax[0].legend(loc="upper right")
        ax[1].specgram(m, NFFT=1024, Fs=rate, noverlap=512, cmap="magma", vmin=-140)
        ax[1].set_ylabel("Hz"); ax[1].set_title("spectrogram (mono mix)")
        # RMS envelope in 50 ms windows
        w = rate // 20
        env = np.sqrt(np.array([np.mean(m[i:i + w] ** 2) for i in range(0, len(m) - w, w)]) + 1e-12)
        ax[2].plot(np.arange(len(env)) * 0.05, 20 * np.log10(env), lw=0.6)
        ax[2].set_ylabel("dBFS"); ax[2].set_xlabel("s"); ax[2].set_title("RMS (50 ms)")
        for a_ in ax:
            a_.set_xlim(0, t[-1])
        fig.tight_layout()
        p = out / f"{sdn}_{name}.png"
        fig.savefig(p, dpi=80); plt.close(fig)
        made.append(str(p))
    return made


def main():
    root = HERE.parents[2]
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--extract", default=str(root / "extract" / "eur"))
    ap.add_argument("--out", default=str(root / "build" / "dreamcast" / "audio"))
    ap.add_argument("--pcm", help="dir given to sdat2dc --keep-pcm")
    ap.add_argument("--swav-ref", help="independent SWAV decoder harness (see SOUND.md)")
    ap.add_argument("--strm-ref", help="independent STRM decoder harness (see SOUND.md)")
    ap.add_argument("--checks", default="swav,strm,loops,seam,adpcm,levels,png")
    ap.add_argument("--png-dir", default=str(root / "build" / "dreamcast" / "audio" / "validation"))
    ap.add_argument("--png-tracks", default="z1s:SEQ_1sonic,z1b:SEQ_1blaze,z1es:SEQ_boss,sys:SEQ_TITLE")
    a = ap.parse_args()
    a.renderer = Path(a.out) / "bin" / "sseqrender"
    man = json.loads((Path(a.out) / "manifest.json").read_text())
    tmp = tempfile.mkdtemp(prefix="valaudio_")
    report = {}
    for c in a.checks.split(","):
        if c == "swav": report[c] = check_swav(a, man, tmp)
        elif c == "strm":
            if not a.strm_ref:
                continue
            report[c] = check_strm(a, tmp)
        elif c == "loops": report[c] = check_loops(a, man)
        elif c == "seam": report[c] = check_seam(a, man, tmp)
        elif c == "adpcm": report[c] = check_adpcm(a, man, tmp)
        elif c == "levels": report[c] = check_levels(a, man)
        elif c == "png": report[c] = make_png(a, man)
        print(c, json.dumps(report[c], indent=1, default=str), flush=True)
    Path(a.png_dir).mkdir(parents=True, exist_ok=True)
    (Path(a.png_dir) / "report.json").write_text(json.dumps(report, indent=1, default=str))


if __name__ == "__main__":
    main()

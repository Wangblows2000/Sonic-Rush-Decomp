#!/usr/bin/env python3
"""sfxbank_check.py - compare the native runtime's driver renders with sseqrender.c.

dreamcast/game/sound/test/test_sound (env SND_DUMP_DIR=DIR) writes what the
real-time driver (dreamcast/game/sound/snd_drv.c) produces for every SSAR id
of one bank, mixed with the DS mixer model from the bank's AICA ADPCM waves.
This script renders the same ids with tools/dreamcast/audio/sseqrender.c
(PCM16, no ADPCM on the output, original DS-decoded waves) and reports the
SNR per id. Remaining difference = ADPCM quantisation of the waves + the
one-frame timing of the driver loop (both start at frame 1).

Usage: python3 tools/dreamcast/audio/sfxbank_check.py --dump /tmp/sound/dump --sdat z1s
       [--extract extract/eur] [--audio build/dreamcast/audio] [--sec 3]
"""
import argparse
import json
import re
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
        a = np.frombuffer(w.readframes(w.getnframes()), np.int16).astype(np.float64)
        return a.reshape(-1, w.getnchannels())


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--dump", required=True)
    ap.add_argument("--sdat", default="z1s")
    ap.add_argument("--extract", default="extract/eur")
    ap.add_argument("--audio", default="build/dreamcast/audio")
    ap.add_argument("--sec", type=float, default=3.0)
    a = ap.parse_args()
    audio = Path(a.audio)
    man = json.loads((audio / "manifest.json").read_text())
    sd = sdatlib.SDAT(Path(a.extract) / "files" / "snd" / a.sdat / "sound_data.sdat")
    recs = sd.ssar_records(sd.info["seqarc"][0]["file"])
    renderer = audio / "bin" / "sseqrender"
    tables = audio / "tables.bin"
    res = []
    with tempfile.TemporaryDirectory() as td:
        for e in man["entries"]:
            if e["kind"] != "sfx" or e["sdat"] != a.sdat or e["status"] == "silent":
                continue
            m = re.search(r"SSAR#0 file(\d+) seq(\d+)", e["source"])
            if not m:
                continue
            fid, si = int(m.group(1)), int(m.group(2))
            mine_p = Path(a.dump) / f"{a.sdat}_{si:03d}.wav"
            if not mine_p.exists():
                continue
            off, bank, vol, cpr, ppr, ply = recs[si]
            r = e["render"]
            out = Path(td) / f"{si}.wav"
            cmd = [str(renderer), "render", "--sdat", str(sd.path), "--tables", str(tables), "--seq", str(fid),
                   "--start", str(off), "--bank", str(r["bank_file"]), "--swar", ",".join(map(str, r["swar_files"])),
                   "--vol", str(vol), "--ppr", str(ppr), "--mask", hex(r["channel_mask"]),
                   "--max-sec", str(a.sec + 0.1), "--hold-sec", "0", "--full",
                   "--gain-tl", str(man.get("mix_gain_tl", 0)), "--out", str(out)]  # same mix gain as the bank
            subprocess.run(cmd, check=True, capture_output=True)
            ref, mine = read_wav(out), read_wav(mine_p)
            n = min(len(ref), len(mine), int(a.sec * 32728))
            if n == 0:
                continue
            d = ref[:n] - mine[:n]
            sr, se = float((ref[:n] ** 2).sum()), float((d ** 2).sum())
            snr = 10 * np.log10(sr / se) if se > 0 else 200.0
            res.append((snr, si, e["name"]))
    res.sort()
    snrs = np.array([x[0] for x in res])
    print(f"{a.sdat}: {len(res)} ids  SNR min {snrs.min():.1f}  p10 {np.percentile(snrs, 10):.1f}  "
          f"median {np.median(snrs):.1f}  max {snrs.max():.1f} dB")
    for s, si, name in res[:8]:
        print(f"  lowest: id {si:3d} {name:24s} {s:6.1f} dB")
    exact = sum(1 for s in snrs if s >= 100)
    print(f"  bit-exact (PSG/noise-only) ids: {exact}")
    return 0


if __name__ == "__main__":
    sys.exit(main())

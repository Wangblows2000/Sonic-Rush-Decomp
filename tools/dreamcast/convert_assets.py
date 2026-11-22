#!/usr/bin/env python3
"""Convert assets from the owner's extracted Sonic Rush ROM into Dreamcast formats.

Bring-your-own-ROM: input is extract/eur/ (made by tools/bootstrap_session.sh from
your own dump). Output goes to build/dreamcast/assets/ (gitignored) together with
manifest.json, which records source -> output, formats, sizes and settings.
No converted asset is committed.

Milestone 2/3 coverage:
  sprite.argb4444   banner icon (32x32, 4bpp paletted in the ROM) -> ARGB4444, non-twiddled
  sample.wav        first N seconds of SDAT STRM #105 (stereo IMA-ADPCM) -> mono PCM16
                    -> AICA Yamaha ADPCM WAV via KOS utils/wav2adpcm
  strm_<n>.wav      (--all-strm) every STRM in every SDAT, full length, stereo kept

Usage:
  python3 tools/dreamcast/convert_assets.py [--extract extract/eur] [--out build/dreamcast/assets]
         [--wav2adpcm PATH] [--sample-seconds 6] [--all-strm]
"""
import argparse
import hashlib
import json
import struct
import subprocess
import sys
import wave
from pathlib import Path

IMA_STEP = [7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 50,
            55, 60, 66, 73, 80, 88, 97, 107, 118, 130, 143, 157, 173, 190, 209, 230, 253, 279,
            307, 337, 371, 408, 449, 494, 544, 598, 658, 724, 796, 876, 963, 1060, 1166, 1282,
            1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871,
            5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899, 15289, 16818,
            18500, 20350, 22385, 24623, 27086, 29794, 32767]
IMA_INDEX = [-1, -1, -1, -1, 2, 4, 6, 8]


def ima_block(data):
    """Decode one NDS IMA-ADPCM block (4-byte header: s16 sample, s16 index)."""
    sample, index = struct.unpack_from("<hh", data, 0)
    index = max(0, min(88, index))
    out = []
    for byte in data[4:]:
        for nib in (byte & 0xF, byte >> 4):
            step = IMA_STEP[index]
            diff = step >> 3
            if nib & 1: diff += step >> 2
            if nib & 2: diff += step >> 1
            if nib & 4: diff += step
            sample = sample - diff if nib & 8 else sample + diff
            sample = max(-32768, min(32767, sample))
            out.append(sample)
            index = max(0, min(88, index + IMA_INDEX[nib & 7]))
    return out


def sdat_files(path):
    d = path.read_bytes()
    fat_off = struct.unpack_from("<I", d, 0x20)[0]
    n = struct.unpack_from("<I", d, fat_off + 8)[0]
    for i in range(n):
        off, sz = struct.unpack_from("<II", d, fat_off + 12 + i * 16)
        yield i, d[off:off + sz]


def decode_strm(s):
    """Return (rate, channels, loop_flag, loop_start, [per-channel samples])."""
    assert s[:4] == b"STRM"
    h = 0x10
    assert s[h:h + 4] == b"HEAD"
    enc, loop, ch = s[h + 8], s[h + 9], s[h + 10]
    rate, = struct.unpack_from("<H", s, h + 12)
    loop_start, nsamp, data_off, nblocks, blen, bsamp, lastlen, lastsamp = \
        struct.unpack_from("<8I", s, h + 16)
    if enc != 2:
        raise ValueError(f"STRM encoding {enc} not handled (only IMA-ADPCM=2)")
    chans = [[] for _ in range(ch)]
    pos = data_off
    for b in range(nblocks):
        L = lastlen if b == nblocks - 1 else blen
        for c in range(ch):
            chans[c].extend(ima_block(s[pos:pos + L]))
            pos += L
    for c in range(ch):
        chans[c] = chans[c][:nsamp]
    return rate, ch, loop, loop_start, chans


def write_wav(path, rate, chans):
    with wave.open(str(path), "wb") as w:
        w.setnchannels(len(chans))
        w.setsampwidth(2)
        w.setframerate(rate)
        frames = bytearray()
        for i in range(len(chans[0])):
            for c in chans:
                frames += struct.pack("<h", c[i])
        w.writeframes(bytes(frames))


def sha1(p):
    return hashlib.sha1(Path(p).read_bytes()).hexdigest()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--extract", default="extract/eur")
    ap.add_argument("--out", default="build/dreamcast/assets")
    ap.add_argument("--wav2adpcm", default="wav2adpcm")
    ap.add_argument("--sample-seconds", type=float, default=6.0)
    ap.add_argument("--all-strm", action="store_true")
    a = ap.parse_args()
    ext, out = Path(a.extract), Path(a.out)
    out.mkdir(parents=True, exist_ok=True)
    manifest = {"source_root": str(ext), "assets": []}

    # banner icon -> ARGB4444 32x32
    from PIL import Image
    src = ext / "banner/bitmap.png"
    img = Image.open(src).convert("RGBA")
    if img.size != (32, 32):
        sys.exit(f"unexpected banner size {img.size}")
    raw = bytearray()
    for y in range(32):
        for x in range(32):
            r, g, b, al = img.getpixel((x, y))
            raw += struct.pack("<H", ((al >> 4) << 12) | ((r >> 4) << 8) | ((g >> 4) << 4) | (b >> 4))
    (out / "sprite.argb4444").write_bytes(raw)
    manifest["assets"].append({"output": "sprite.argb4444", "source": str(src),
                               "source_sha1": sha1(src), "format": "ARGB4444 non-twiddled 32x32",
                               "bytes": len(raw)})

    # STRM -> PCM16 WAV -> AICA ADPCM WAV
    sdat = ext / "files/snd/sys/sound_data.sdat"
    strms = [(i, f) for i, f in sdat_files(sdat) if f[:4] == b"STRM"]
    i0, s0 = strms[0]
    rate, ch, loop, ls, chans = decode_strm(s0)
    n = int(rate * a.sample_seconds)
    mono = [(chans[0][k] + chans[-1][k]) // 2 for k in range(n)]
    pcm = out / "sample_pcm16.wav"
    write_wav(pcm, rate, [mono])
    adp = out / "sample.wav"
    subprocess.run([a.wav2adpcm, "-t", str(pcm), str(adp)], check=True)
    manifest["assets"].append({"output": "sample.wav", "source": f"{sdat}#file{i0}(STRM)",
                               "source_sha1": sha1(sdat), "format": "WAV fmt 0x14 AICA Yamaha ADPCM",
                               "rate": rate, "channels": 1, "source_channels": ch,
                               "seconds": round(n / rate, 3), "loop": bool(loop),
                               "settings": f"first {a.sample_seconds}s, L+R mixed, KOS wav2adpcm -t",
                               "bytes": adp.stat().st_size})
    pcm.unlink()

    if a.all_strm:
        for sd in sorted((ext / "files/snd").glob("*/sound_data.sdat")):
            for i, f in sdat_files(sd):
                if f[:4] != b"STRM":
                    continue
                rate, ch, loop, ls, chans = decode_strm(f)
                tag = f"strm_{sd.parent.name}_{i}"
                p = out / f"{tag}_pcm16.wav"
                write_wav(p, rate, chans)
                o = out / f"{tag}.wav"
                subprocess.run([a.wav2adpcm, "-t", str(p), str(o)], check=True)
                p.unlink()
                manifest["assets"].append({"output": o.name, "source": f"{sd}#file{i}(STRM)",
                                           "format": "WAV AICA ADPCM", "rate": rate,
                                           "channels": ch, "loop": bool(loop), "loop_start": ls,
                                           "seconds": round(len(chans[0]) / rate, 3),
                                           "bytes": o.stat().st_size})
    (out / "manifest.json").write_text(json.dumps(manifest, indent=1))
    print(f"{len(manifest['assets'])} assets -> {out}")


if __name__ == "__main__":
    main()

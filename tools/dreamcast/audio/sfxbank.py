#!/usr/bin/env python3
"""sfxbank.py - build per-SDAT sound-effect banks for the native Dreamcast runtime
(dreamcast/game/sound/). Sonic Rush DC port.

Why not one pre-rendered sample per sound id: the rendered SSAR effects
(build/dreamcast/audio/sfx/) of one zone SDAT are 5.7-10.1 MB of AICA ADPCM
(203 distinct files for z1s, 106 of them longer than the 65534-sample AICA
limit), against ~1.8 MB of sound RAM. So the bank keeps the effects the way the
DS keeps them: the SSAR sequences + the SBNK instruments + the SWAR waves
(already converted to AICA ADPCM by sdat2dc.py), and the runtime plays the
sequences (dreamcast/game/sound/snd_seq.c, a real-time port of
tools/dreamcast/audio/sseqrender.c). One zone bank is ~0.2 MB.

Inputs (all produced from the owner's ROM, nothing from the ROM goes into source):
  extract/eur/files/snd/<sdat>/sound_data.sdat   SSAR / SBNK / player INFO
  build/dreamcast/audio/manifest.json + waves/   SWAR waves as KOS wav2adpcm WAVs
  extract/eur/arm7/arm7.bin, arm9/arm9.bin       sound-driver tables (sdatlib)

Output: build/dreamcast/audio/sfxbank/<sdat>.srb (+ <sdat>.json: id -> name,
player, priority, volume; sizes) and budget.json (all banks).

.srb layout (little endian, all offsets from file start, see snd_bank.h):
  0x00 'SRSB'  u32 version(2)  char sdat[8]
  0x10 u32 n_players  off_players     players: {u8 max_seq, u8 0, u16 alloc_mask, u32 heap} x n
  0x18 u32 n_ssar     off_ssar        ssar:    {u32 n_seq, off_rec, off_data, data_size} x n
                                      rec (SSAR record, 12 B): u32 offset (0xffffffff empty),
                                        u16 bank, u8 volume, u8 channel_prio, u8 player_prio, u8 player
  0x20 u32 n_banks    off_banks       bank:    {u32 off, u32 size, s16 wave_base[4], u16 wave_count[4]} x n
                                               (raw SBNK file; size 0 when no SSAR record uses it)
  0x28 u32 n_waves    off_waves       wave:    {u32 data_off, u32 samples, u32 loop_start, u16 timer,
                                                u8 format (0 AICA ADPCM, 1 PCM16, 2 PCM8), u8 loop} x n
  0x30 u32 psg_wave   noise_wave      first of 8 generated PSG duty waves, generated noise wave
  0x38 u32 off_tables                 s16 dbsq[128] (ARM7), s16 db[128] (ARM9 NNS), u8 volume[724],
                                      u8 attack[19], u8 sine[33], u8 order[16], (pad to 4) u16 pitch[768]
  0x3c u32 off_wavedata  (0x40) u32 wavedata_size   wave blob (each wave 32-byte aligned) = what goes
                                      to sound RAM; always last in the file so the loader can stream it.
  0x44 u32 mix_gain_tl (v2)           port mix gain (AICA TL steps, gain 2^(-n/16)) = sdat2dc.py
                                      MIX_GAIN_TL from manifest.json: the same gain the music renders
                                      carry, applied by the runtime to every effect channel.

Usage:
  python3 tools/dreamcast/audio/sfxbank.py [--extract extract/eur]
      [--audio build/dreamcast/audio] [--out build/dreamcast/audio/sfxbank]
      [--only z1s,z1b] [--budget BYTES]
"""
import argparse
import json
import math
import re
import struct
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import sdatlib  # noqa: E402

VERSION = 2
# KOS: snd_mem pool = AICA_RAM_END 0x200000 - AICA_RAM_START 0x30000 = 1,900,544 B.
# engine/audio.c music stream: snd_stream_alloc(cb, SND_STREAM_BUFFER_MAX_ADPCM)
# x 2 channels (snd_stream_init) = 2 * 32704 = 65,408 B (+ snd_mem block rounding).
SND_MEM_POOL = 0x200000 - 0x30000
STREAM_BYTES = 2 * ((32 << 10) - 64)
DEFAULT_BUDGET = SND_MEM_POOL - STREAM_BYTES - 4096  # 4 KB slack for allocator headers / alignment


def u32(d, o):
    return struct.unpack_from("<I", d, o)[0]


def read_kos_adpcm_wav(p):
    """data bytes of a KOS wav2adpcm mono WAV (fmt 0x14)."""
    d = Path(p).read_bytes()
    assert d[:4] == b"RIFF" and d[8:12] == b"WAVE", p
    o, fmt, ch, data = 12, None, None, None
    while o + 8 <= len(d):
        cid, sz = d[o:o + 4], u32(d, o + 4)
        if cid == b"fmt ":
            fmt, ch = struct.unpack_from("<HH", d, o + 8)
        elif cid == b"data":
            data = d[o + 8:o + 8 + sz]
        o += 8 + sz + (sz & 1)
    assert fmt == 0x14 and ch == 1 and data is not None, f"{p}: fmt {fmt} ch {ch}"
    return data


def pitch_table():
    """NitroSDK SND_CalcTimer table: 65536*(2^(i/768)-1) (computed, same formula as sseqrender.c)."""
    return [int(round(65536.0 * (2.0 ** (i / 768.0) - 1.0))) for i in range(768)]


def nns_decibel_table(extract):
    """ARM9 SNDi_DecibelTable (data_020725e0), see sdatlib.nns_decibel_table."""
    return sdatlib.nns_decibel_table(extract)


def psg_waves():
    """8 duty-cycle waves (8 steps, PCM16): DS PSG step s is high when s >= 7 - duty; duty 7 = low."""
    out = []
    for duty in range(8):
        v = [(32767 if (duty != 7 and s >= 7 - duty) else -32767) for s in range(8)]
        out.append(struct.pack("<8h", *v))
    return out


def noise_wave():
    """DS noise channel: 15-bit LFSR, tap 0x6000, one period (32767 steps) as PCM8."""
    lfsr, out = 0x7FFF, bytearray()
    for _ in range(32767):
        carry = lfsr & 1
        lfsr >>= 1
        if carry:
            lfsr ^= 0x6000
            out.append(0x81)  # -127
        else:
            out.append(0x7F)
    # wave[k] = output after k LFSR steps (sseqrender.c: sample at step index k);
    # the sequence has period 32767, so wave[0] = output after step 32767
    return bytes(out[-1:] + out[:-1])


# sys SDAT (menus / zone map / staff roll): only the sequences the game plays
# from it, from src/game/Sound: SysSound_StartSE (SSAR 1 seqs 0-5, 0xb, 4;
# 0x11 seqs 0-2; 2/3 = ARC_ZONE_MAP_S/_B seqs 0-8, 0x10; 0xa seqs 0-2) and
# SysSound_StartVoice (SSAR 1 seqs 5-0xa; 0x11 seqs 3-4). Whole SSARs 1 and
# 0x11 are kept (12 / 5 records). The other 14 SSARs (and their banks / waves:
# the full sys SDAT has 4.2 MB of waves) are left out (n_seq 0). Groups the
# original loads for these: 1, 0x15, 2 / 3, 0xe (SysSound_LoadSEGroup /
# SysSound_LoadVoiceGroup).
SELECT = {
    "sys": {1: None, 0x11: None, 2: list(range(9)) + [0x10], 3: list(range(9)) + [0x10], 0xA: [0, 1, 2]},
}


class Builder:
    def __init__(self, a):
        self.a = a
        self.audio = Path(a.audio)
        self.manifest = json.loads((self.audio / "manifest.json").read_text())
        if "mix_gain_tl" not in self.manifest:
            raise SystemExit("manifest.json has no mix_gain_tl: re-run sdat2dc.py")
        self.mix_gain_tl = int(self.manifest["mix_gain_tl"])
        self.waves = {}
        for e in self.manifest["entries"]:
            if e["kind"] != "wave":
                continue
            m = re.search(r"file(\d+) wave(\d+)", e.get("source", ""))
            if m:
                self.waves[(e["sdat"], int(m.group(1)), int(m.group(2)))] = e
        self.sfx = {}
        for e in self.manifest["entries"]:
            if e["kind"] == "sfx":
                m = re.search(r"SSAR#(\d+) .*seq(\d+)", e["source"])
                self.sfx[(e["sdat"], int(m.group(1)), int(m.group(2)))] = e
        drv, self.tinfo = sdatlib.extract_driver_tables(a.extract)
        dblin, self.dbinfo = nns_decibel_table(a.extract)
        self.tables = drv[:256] + dblin + drv[256:]
        self.tables += b"\0" * (-len(self.tables) % 4)
        self.tables += struct.pack("<768H", *pitch_table())

    def wave_data(self, sdname, fid, idx):
        e = self.waves.get((sdname, fid, idx))
        if e is None or e["status"] not in ("ok", "duplicate"):
            return None, e
        rel = e.get("output") or e.get("same_as")
        return read_kos_adpcm_wav(self.audio / rel), e

    def build(self, sdname):
        sd = sdatlib.SDAT(Path(self.a.extract) / "files" / "snd" / sdname / "sound_data.sdat")
        report = {"sdat": sdname, "missing_waves": [], "ids": {}}
        # players
        players = b""
        for p in sd.info["player"]:
            p = p or {"max_seq": 0, "mask": 0, "heap": 0}
            players += struct.pack("<BBHI", p["max_seq"], 0, p["mask"], p["heap"])
        n_players = len(sd.info["player"])
        # ssar
        ssars = []
        used_banks = set()
        select = SELECT.get(sdname)
        for ai, a in enumerate(sd.info["seqarc"]):
            if a is None or (select is not None and ai not in select):
                ssars.append((0, b"", b""))
                continue
            f = sd.file(a["file"])
            n = u32(f, 0x1C)
            recs = bytearray(f[0x20:0x20 + 12 * n])
            data = f[u32(f, 0x18):]
            sub = sd.ssar_names[ai] if ai < len(sd.ssar_names) else []
            keep = select.get(ai) if select is not None else None
            for si, r in enumerate(sd.ssar_records(a["file"])):
                if r is not None and keep is not None and si not in keep:
                    struct.pack_into("<I", recs, 12 * si, 0xFFFFFFFF)   # not used by the game: empty record
                    r = None
                if r is None:
                    continue
                off, bank, vol, cpr, ppr, ply = r
                used_banks.add(bank)
                e = self.sfx.get((sdname, ai, si))
                report["ids"][f"{ai}:{si}"] = {
                    "name": sub[si] if si < len(sub) and sub[si] else None, "bank": bank, "volume": vol,
                    "channel_prio": cpr, "player_prio": ppr, "player": ply,
                    "player_max_seq": sd.info["player"][ply]["max_seq"] if ply < n_players and sd.info["player"][ply] else None,
                    "rendered": e["status"] if e else None,
                    "rendered_samples": e.get("samples") if e else None}
            ssars.append((n, bytes(recs), data))
        # banks + waves
        wave_tab, blob = [], bytearray()

        def add_wave(data, samples, loop_start, timer, fmt, loop):
            blob.extend(b"\0" * (-len(blob) % 32))
            wave_tab.append(struct.pack("<IIIHBB", len(blob), samples, loop_start, timer, fmt, loop))
            blob.extend(data)
            return len(wave_tab) - 1

        swar_base = {}
        banks = []
        bank_blobs = []
        for bi, b in enumerate(sd.info["bank"]):
            if b is None or bi not in used_banks:
                banks.append(None)
                continue
            slots = sd.bank_swar_files(bi)
            base, count = [], []
            for fid in slots:
                if fid < 0:
                    base.append(-1); count.append(0)
                    continue
                if fid not in swar_base:
                    sw = sd.file(fid)
                    nw = u32(sw, 0x38)
                    first = len(wave_tab)
                    for wi in range(nw):
                        data, e = self.wave_data(sdname, fid, wi)
                        if data is None:
                            report["missing_waves"].append([fid, wi, e["status"] if e else "absent"])
                            add_wave(b"", 0, 0, 1, 0, 0)
                            continue
                        loop = 1 if e["loop"] else 0
                        add_wave(data, e["samples"], e["loop_start"] or 0, e["timer"], 0, loop)
                    swar_base[fid] = (first, nw)
                base.append(swar_base[fid][0]); count.append(swar_base[fid][1])
            banks.append((base, count))
            bank_blobs.append((bi, sd.file(b["file"])))
        psg = [add_wave(w, 8, 0, 1, 1, 1) for w in psg_waves()][0]
        noise = add_wave(noise_wave(), 32767, 0, 1, 2, 1)
        blob.extend(b"\0" * (-len(blob) % 32))
        # layout
        hdr_size = 0x48  # 17 u32 + char[8]; v1 used 0x40, so wavedata_size overlapped players[0]
        out = bytearray(hdr_size)

        def put(b):
            out.extend(b"\0" * (-len(out) % 4))
            o = len(out)
            out.extend(b)
            return o

        off_players = put(players)
        ssar_tab_off = put(b"\0" * (16 * len(ssars)))
        for k, (n, recs, data) in enumerate(ssars):
            ro = put(recs)
            do = put(data)
            struct.pack_into("<IIII", out, ssar_tab_off + 16 * k, n, ro, do, len(data))
        bank_tab_off = put(b"\0" * (24 * len(banks)))
        blobs = dict(bank_blobs)
        for k, b in enumerate(banks):
            if b is None:
                struct.pack_into("<II4h4H", out, bank_tab_off + 24 * k, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0)
                continue
            bo = put(blobs[k])
            struct.pack_into("<II4h4H", out, bank_tab_off + 24 * k, bo, len(blobs[k]), *b[0], *b[1])
        off_waves = put(b"".join(wave_tab))
        off_tables = put(self.tables)
        out.extend(b"\0" * (-len(out) % 32))
        off_wavedata = len(out)
        out.extend(blob)
        name8 = sdname.encode()[:8].ljust(8, b"\0")
        struct.pack_into("<4sI8sIIIIIIIIIIIII", out, 0, b"SRSB", VERSION, name8,
                         n_players, off_players, len(ssars), ssar_tab_off, len(banks), bank_tab_off,
                         len(wave_tab), off_waves, psg, noise, off_tables, off_wavedata, len(blob))
        struct.pack_into("<I", out, 0x44, self.mix_gain_tl)
        outdir = Path(self.a.out)
        outdir.mkdir(parents=True, exist_ok=True)
        (outdir / f"{sdname}.srb").write_bytes(out)
        report.update(file_bytes=len(out), main_ram_bytes=off_wavedata, sound_ram_bytes=len(blob),
                      waves=len(wave_tab), banks=[i for i, b in enumerate(banks) if b],
                      ssar=[{"n_seq": n, "data_bytes": len(d)} for n, _, d in ssars],
                      rendered_sfx_bytes_if_prerendered=self.prerendered_bytes(sdname))
        (outdir / f"{sdname}.json").write_text(json.dumps(report, indent=1))
        # plain-text index of the offline renders (sdat2dc.py sfx/) for the host
        # test (dreamcast/game/sound/test): "<ssar> <seq> <status> <samples> <channels> <path|->"
        lines = []
        for (s, ai, si), e in sorted(self.sfx.items()):
            if s == sdname and (select is None or (ai in select and (select[ai] is None or si in select[ai]))):
                rel = e.get("output") or e.get("same_as") or "-"
                lines.append(f"{ai} {si} {e['status']} {e.get('samples', 0)} {e.get('channels', 0)} {rel}")
        (outdir / f"{sdname}.rendered.txt").write_text("\n".join(lines) + "\n")
        return report

    def prerendered_bytes(self, sdname):
        files = set()
        for (s, _, _), e in self.sfx.items():
            if s == sdname and (e.get("output") or e.get("same_as")):
                files.add(e.get("output") or e.get("same_as"))
        tot = 0
        for f in files:
            p = self.audio / f
            tot += p.stat().st_size - 44 if p.exists() else 0
        return {"files": len(files), "adpcm_bytes": tot}


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--extract", default="extract/eur")
    ap.add_argument("--audio", default="build/dreamcast/audio")
    ap.add_argument("--out", default="build/dreamcast/audio/sfxbank")
    ap.add_argument("--only", default="")
    ap.add_argument("--budget", type=int, default=DEFAULT_BUDGET)
    a = ap.parse_args()
    b = Builder(a)
    names = [x for x in a.only.split(",") if x] or sorted(
        n for n in b.manifest["sdats"] if n != "extra_end")
    rows, worst = [], 0
    for n in names:
        r = b.build(n)
        worst = max(worst, r["sound_ram_bytes"])
        ok = r["sound_ram_bytes"] <= a.budget
        rows.append({"sdat": n, "sound_ram": r["sound_ram_bytes"], "main_ram": r["main_ram_bytes"],
                     "file": r["file_bytes"], "waves": r["waves"], "missing_waves": len(r["missing_waves"]),
                     "ids": len(r["ids"]), "prerendered_adpcm": r["rendered_sfx_bytes_if_prerendered"]["adpcm_bytes"],
                     "fits": ok})
        print(f"{n:6s} sound RAM {r['sound_ram_bytes']:8d}  main RAM {r['main_ram_bytes']:6d}  waves {r['waves']:3d}"
              f"  ids {len(r['ids']):3d}  missing waves {len(r['missing_waves'])}"
              f"  (pre-rendered would be {r['rendered_sfx_bytes_if_prerendered']['adpcm_bytes']}){'' if ok else '  OVER BUDGET'}")
    budget = {"snd_mem_pool": SND_MEM_POOL, "music_stream": STREAM_BYTES, "budget": a.budget,
              "worst_bank": worst, "free_after_worst": a.budget - worst,
              "driver_tables": b.tinfo, "mix_gain_tl": b.mix_gain_tl, "nns_decibel_table": b.dbinfo, "banks": rows}
    Path(a.out, "budget.json").write_text(json.dumps(budget, indent=1))
    print(f"budget {a.budget} B, worst bank {worst} B, free {a.budget - worst} B")
    return 0 if all(r["fits"] for r in rows) else 1


if __name__ == "__main__":
    sys.exit(main())

"""SDAT parsing helpers and sound-driver table extraction (Sonic Rush DC port).

Written for this project from the SDAT format description in
docs/dreamcast/formats/SOUND.md. Reads only the owner's extracted ROM.
"""
import math
import struct
from pathlib import Path

SECTIONS = ["seq", "seqarc", "bank", "wavearc", "player", "group", "player2", "strm"]


def u16(d, o):
    return struct.unpack_from("<H", d, o)[0]


def u32(d, o):
    return struct.unpack_from("<I", d, o)[0]


class SDAT:
    def __init__(self, path):
        self.path = Path(path)
        d = self.data = self.path.read_bytes()
        assert d[:4] == b"SDAT", path
        symb_off, symb_size, info_off, info_size, fat_off, fat_size = struct.unpack_from("<6I", d, 0x10)
        # FAT
        n = u32(d, fat_off + 8)
        self.fat = [struct.unpack_from("<II", d, fat_off + 12 + 16 * i) for i in range(n)]
        # INFO
        recs = struct.unpack_from("<8I", d, info_off + 8)
        self.info = {}
        for k, name in enumerate(SECTIONS):
            o = info_off + recs[k]
            cnt = u32(d, o)
            ents = []
            for i in range(cnt):
                eo = u32(d, o + 4 + 4 * i)
                ents.append(self._info_entry(name, info_off + eo) if eo else None)
            self.info[name] = ents
        # SYMB (optional)
        self.names = {k: [] for k in SECTIONS}
        self.ssar_names = []
        if symb_off and symb_size:
            srecs = struct.unpack_from("<8I", d, symb_off + 8)
            for k, name in enumerate(SECTIONS):
                o = symb_off + srecs[k]
                cnt = u32(d, o)
                if name == "seqarc":
                    for i in range(cnt):
                        no, so = struct.unpack_from("<II", d, o + 4 + 8 * i)
                        self.names[name].append(self._str(symb_off + no) if no else None)
                        sub = []
                        if so:
                            sc = u32(d, symb_off + so)
                            for j in range(sc):
                                x = u32(d, symb_off + so + 4 + 4 * j)
                                sub.append(self._str(symb_off + x) if x else None)
                        self.ssar_names.append(sub)
                else:
                    for i in range(cnt):
                        x = u32(d, o + 4 + 4 * i)
                        self.names[name].append(self._str(symb_off + x) if x else None)

    def _str(self, o):
        e = self.data.index(b"\0", o)
        return self.data[o:e].decode("ascii", "replace")

    def _info_entry(self, kind, o):
        d = self.data
        if kind == "seq":
            fid, _, bank, vol, cpr, ppr, ply = struct.unpack_from("<HHHBBBB", d, o)
            return dict(file=fid, bank=bank, vol=vol, cpr=cpr, ppr=ppr, player=ply)
        if kind in ("seqarc", "wavearc"):
            return dict(file=u16(d, o))
        if kind == "bank":
            fid = u16(d, o)
            wa = list(struct.unpack_from("<4H", d, o + 4))
            return dict(file=fid, wavearc=[w for w in wa])
        if kind == "player":
            maxseq, _, mask, heap = struct.unpack_from("<BBHI", d, o)
            return dict(max_seq=maxseq, mask=mask, heap=heap)
        if kind == "strm":
            fid, _, vol, pri, ply = struct.unpack_from("<HHBBB", d, o)
            return dict(file=fid, vol=vol, pri=pri, player=ply)
        return dict(raw=o)

    def name(self, kind, i, prefix):
        lst = self.names.get(kind, [])
        if i < len(lst) and lst[i]:
            return lst[i]
        return f"{prefix}_{i:03d}"

    def file(self, fid):
        off, size = self.fat[fid]
        return self.data[off:off + size]

    def ssar_records(self, fid):
        """[(offset_in_data, bank, vol, cpr, ppr, player)] of an SSAR file (None for empty slots)."""
        f = self.file(fid)
        assert f[:4] == b"SSAR"
        cnt = u32(f, 0x1C)
        out = []
        for i in range(cnt):
            off, bank, vol, cpr, ppr, ply = struct.unpack_from("<IHBBBB", f, 0x20 + 12 * i)
            out.append(None if off == 0xFFFFFFFF else (off, bank, vol, cpr, ppr, ply))
        return out

    def bank_swar_files(self, bank_index):
        """File IDs of the (up to 4) wave archives linked to bank `bank_index` (-1 = none)."""
        b = self.info["bank"][bank_index]
        out = []
        for w in b["wavearc"]:
            if w == 0xFFFF or w >= len(self.info["wavearc"]) or self.info["wavearc"][w] is None:
                out.append(-1)
            else:
                out.append(self.info["wavearc"][w]["file"])
        return out


def extract_driver_tables(extract_dir):
    """Locate the NitroSDK sound-driver tables in the owner's arm7.bin / arm9.bin.

    Tables are found structurally (computed signatures), never copied into source:
      decibel-square table (128 x s16, ARM7) ~ round(400*log10(i/127)), [0]=-723
      LFO sine table (33 x u8, ARM7)          ~ round(127*sin(pi/2*i/32))
      channel order (16 x u8 permutation) + attack table (19 x u8 increasing from 0), ARM7
      channel volume table (724 x u8, ARM9, SND_CalcChannelVolume)
    Returns the packed bytes the C renderer reads with --tables.
    """
    ex = Path(extract_dir)
    a7 = (ex / "arm7" / "arm7.bin").read_bytes()
    a9 = (ex / "arm9" / "arm9.bin").read_bytes()
    sine = bytes(round(127 * math.sin(math.pi / 2 * i / 32)) for i in range(33))
    si = a7.find(sine)
    if si < 0:
        raise RuntimeError("LFO sine table not found in arm7.bin")
    approx = [max(-723, round(400 * math.log10(i / 127))) if i else -723 for i in range(128)]
    best = None
    for o in range(0, len(a7) - 256, 2):
        v = struct.unpack_from("<4h", a7, o)
        if v[0] != -723 or v[3] != approx[3]:
            continue
        t = struct.unpack_from("<128h", a7, o)
        err = sum(abs(t[i] - approx[i]) for i in range(128))
        if err < 8 and t[127] == 0:
            best = o
            break
    if best is None:
        raise RuntimeError("decibel-square table not found in arm7.bin")
    dbsq = a7[best:best + 256]
    att = None
    for o in range(16, len(a7) - 19):
        if a7[o] != 0:
            continue
        seg = a7[o:o + 19]
        if all(seg[i] < seg[i + 1] for i in range(18)) and sorted(a7[o - 16:o]) == list(range(16)):
            att = o
            break
    if att is None:
        raise RuntimeError("attack table / channel order not found in arm7.bin")
    attack = a7[att:att + 19]
    order = a7[att - 16:att]

    def vol_approx(db):
        div = 16 if db < -240 else 4 if db < -120 else 2 if db < -60 else 1
        return min(127, round(10 ** (db / 200) * 128 * div))
    va = bytes(vol_approx(i - 723) for i in range(724))
    vt = None
    for o in range(0, len(a9) - 724):
        if a9[o + 723] != 127 or a9[o] != va[0]:
            continue
        seg = a9[o:o + 724]
        if sum(1 for i in range(724) if seg[i] != va[i]) <= 8:
            vt = o
            break
    if vt is None:
        raise RuntimeError("channel volume table not found in arm9.bin")
    volume = a9[vt:vt + 724]
    info = dict(arm7_decibel_sq=hex(best), arm7_sine=hex(si), arm7_attack=hex(att),
                arm7_order=hex(att - 16), arm9_volume=hex(0x02000000 + vt))
    return dbsq + volume + attack + sine + order, info


def nns_decibel_table(extract_dir):
    """ARM9 SNDi_DecibelTable (128 x s16) = data_020725e0, the table
    NNSi_SndPlayerMain / NNSi_SndArcStrmMain use to turn the NNS volumes
    (0..127) into the driver player volume (SND_SetPlayerVolume, 0.1 dB).
    Located structurally: [0] = -723, [i] ~ round(200*log10(i/127)), [127] = 0.
    Returns (256 bytes, hex address)."""
    a9 = (Path(extract_dir) / "arm9" / "arm9.bin").read_bytes()
    approx = [-723] + [round(200 * math.log10(i / 127)) for i in range(1, 128)]
    for o in range(0, len(a9) - 256, 2):
        if struct.unpack_from("<h", a9, o)[0] != -723 or struct.unpack_from("<h", a9, o + 2)[0] != approx[1]:
            continue
        t = struct.unpack_from("<128h", a9, o)
        if t[127] == 0 and sum(abs(t[i] - approx[i]) for i in range(128)) < 16:
            return a9[o:o + 256], hex(0x02000000 + o)
    raise RuntimeError("NNS decibel table not found in arm9.bin")

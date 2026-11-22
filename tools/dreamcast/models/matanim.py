"""NitroSystem G3D material animations: BMA0 (material colour), BTP0 (texture
pattern), BTA0 (texture SRT) -> native .dcc / .dcp / .dct sets.

The values are evaluated per integer frame exactly as the DS evaluates them
(the animation objects use frame >> 12):
  BMA0  NNSi_G3dAnmCalcNsBma -> GetMatColAnmValue_ / GetMatColAnmuAlphaValue_
  BTA0  NNSi_G3dAnmCalcNsBta -> GetTexSRTAnm_ -> GetTexSRTAnmVectorVal_ / GetTexSRTAnmSinCosVal_
  BTP0  NNSi_G3dAnmCalcNsBtp -> NNSi_G3dGetTexPatAnmFV (texture / palette names)
(src/libs/NitroSystem/g3d/), so the runtime only looks samples up.  Materials
are bound by name, as NNSi_G3dAnmObjInitNsBma/Bta/Btp do.  Layout: the .dca
container of model_format.h, kinds 3 / 4 / 5.
"""
import struct

import g3d

DCA_KIND_MATCOL, DCA_KIND_TEXPAT, DCA_KIND_TEXSRT = 3, 4, 5
MODE_CONST, MODE_FRAMES = 2, 3


def _anims(d, blk):
    magic, blocks = g3d.file_blocks(d)
    bo = blocks[blk]
    for name, ad in g3d.read_dict(d, bo + 8):
        yield name, bo + g3d.u32(ad, 0)


# ----------------------------------------------------------------------------- BMA0

def matcol_value(d, o, info, f):
    """GetMatColAnmValue_ (GXRgb u16)."""
    if info & 0x20000000:
        return info & 0xFFFF
    base = o + (info & 0xFFFF)

    def at(i):
        return g3d.u16(d, base + 2 * i)

    def mix2(c0, c1):
        return ((((c0 & 0x7c1f) + (c1 & 0x7c1f)) >> 1) & 0x7c1f) | ((((c0 & 0x3e0) + (c1 & 0x3e0)) >> 1) & 0x3e0)

    if not info & 0xC0000000:
        return at(f)
    last = (info & 0x1FFF0000) >> 16
    if info & 0x40000000:
        if f & 1:
            if f > last:
                return at((last >> 1) + 1)
            return mix2(at(f >> 1), at((f >> 1) + 1))
        return at(f >> 1)
    if f & 3:
        if f > last:
            return at((last >> 2) + (f & 3))
        if f & 1:
            if f & 2:
                i1 = f >> 2
                i0 = i1 + 1
            else:
                i0 = f >> 2
                i1 = i0 + 1
            c0, c1 = at(i0), at(i1)
            return (((((c0 & 0x7c1f) * 3 + (c1 & 0x7c1f)) >> 2) & 0x7c1f) |
                    ((((c0 & 0x3e0) * 3 + (c1 & 0x3e0)) >> 2) & 0x3e0))
        return mix2(at(f >> 2), at((f >> 2) + 1))
    return at(f >> 2)


def matcol_alpha(d, o, info, f):
    """GetMatColAnmuAlphaValue_ (0..31)."""
    if info & 0x20000000:
        return info & 0xFFFF
    base = o + (info & 0xFFFF)

    def at(i):
        return d[base + i]

    if not info & 0xC0000000:
        return at(f)
    last = (info & 0x1FFF0000) >> 16
    if info & 0x40000000:
        if f & 1:
            if f > last:
                return at((last >> 1) + 1)
            return (at(f >> 1) + at((f >> 1) + 1)) >> 1
        return at(f >> 1)
    if f & 3:
        if f > last:
            return at((last >> 2) + (f & 3))
        if f & 1:
            if f & 2:
                i1 = f >> 2
                i0 = i1 + 1
            else:
                i0 = f >> 2
                i1 = i0 + 1
            return (at(i0) * 3 + at(i1)) >> 2
        return (at(f >> 2) + at((f >> 2) + 1)) >> 1
    return at(f >> 2)


def parse_bma(d):
    out = []
    for name, o in _anims(d, b"MAT0"):
        nf = g3d.u16(d, o + 4)
        mats = []
        for mname, md in g3d.read_dict(d, o + 8):
            infos = [g3d.u32(md, 4 * k) for k in range(5)]   # diffuse, ambient, specular, emission, alpha
            ch = []
            for k in range(5):
                fn = matcol_alpha if k == 4 else matcol_value
                ch.append([fn(d, o, infos[k], f) for f in range(nf)])
            mats.append({"material": mname, "channels": ch})
        out.append({"name": name, "num_frame": nf, "materials": mats})
    return out


# ----------------------------------------------------------------------------- BTA0

def _s32(v):
    return v - (1 << 32) if v & 0x80000000 else v


def srt_vector(d, o, info, data, f):
    """GetTexSRTAnmVectorVal_ (fx32)."""
    if info & 0x20000000:
        return _s32(data)
    base = o + data
    f16 = info & 0x10000000

    def at(i):
        return g3d.s16(d, base + 2 * i) if f16 else g3d.s32(d, base + 4 * i)

    if not info & 0xC0000000:
        return at(f)
    last = info & 0xFFFF
    if info & 0x40000000:
        if f & 1:
            if f > last:
                return at((last >> 1) + 1)
            return (at(f >> 1) + at((f >> 1) + 1)) >> 1
        return at(f >> 1)
    if f & 3:
        if f > last:
            return at((f & 3) + (last >> 2))
        if f & 1:
            if f & 2:              # a = data[w] weighs 3: the nearer sample
                g = f >> 2
                w = g + 1
            else:
                w = f >> 2
                g = w + 1
            return (at(w) * 3 + at(g)) >> 2
        return (at(f >> 2) + at((f >> 2) + 1)) >> 1
    return at(f >> 2)


def srt_sincos(d, o, info, data, f):
    """GetTexSRTAnmSinCosVal_ -> u32 (sin & 0xffff) | cos << 16."""
    if info & 0x20000000:
        return data
    base = o + data

    def sc(i):
        return g3d.s16(d, base + 4 * i), g3d.s16(d, base + 4 * i + 2)

    if not info & 0xC0000000:
        return g3d.u32(d, base + 4 * f)
    last = info & 0xFFFF
    if info & 0x40000000:
        if f & 1:
            if f > last:
                return g3d.u32(d, base + 4 * ((last >> 1) + 1))
            i = f >> 1
        else:
            return g3d.u32(d, base + 4 * (f >> 1))
    else:
        if f & 3:
            if f > last:
                return g3d.u32(d, base + 4 * ((last >> 2) + (f & 3)))
            if f & 1:
                if f & 2:
                    sub = f >> 2
                    i = sub + 1
                else:
                    i = f >> 2
                    sub = i + 1
                s, c = sc(i)
                s2, c2 = sc(sub)
                return (((s * 3 + s2) >> 2) & 0xFFFF) | ((((c * 3 + c2) >> 2) & 0xFFFF) << 16)
            i = f >> 2
        else:
            return g3d.u32(d, base + 4 * (f >> 2))
    s0, c0 = sc(i)
    s1, c1 = sc(i + 1)
    return (((s0 + s1) >> 1) & 0xFFFF) | ((((c0 + c1) >> 1) & 0xFFFF) << 16)


def parse_bta(d):
    """Per material, per frame: (scaleS, scaleT, sincos, transS, transT) as GetTexSRTAnm_ reads them
    (data[0..1] scaleS, [2..3] scaleT, [4..5] rotation, [6..7] transS, [8..9] transT)."""
    out = []
    for name, o in _anims(d, b"SRT0"):
        nf = g3d.u16(d, o + 4)
        mats = []
        for mname, md in g3d.read_dict(d, o + 8):
            w = [g3d.u32(md, 4 * k) for k in range(10)]
            ch = [[srt_vector(d, o, w[0], w[1], f) for f in range(nf)],
                  [srt_vector(d, o, w[2], w[3], f) for f in range(nf)],
                  [_s32(srt_sincos(d, o, w[4], w[5], f)) for f in range(nf)],
                  [srt_vector(d, o, w[6], w[7], f) for f in range(nf)],
                  [srt_vector(d, o, w[8], w[9], f) for f in range(nf)]]
            mats.append({"material": mname, "channels": ch})
        out.append({"name": name, "num_frame": nf, "materials": mats})
    return out


# ----------------------------------------------------------------------------- BTP0

def parse_btp(d):
    """Per material, per frame: (texture index, palette index or 0xFF) into the animation's name tables,
    looked up with NNSi_G3dGetTexPatAnmFV's search (start at scale * frame, walk back, then forward)."""
    out = []
    for name, o in _anims(d, b"PAT0"):
        nf = g3d.u16(d, o + 4)
        num_tex, num_pl = d[o + 6], d[o + 7]
        ot, op = g3d.u16(d, o + 8), g3d.u16(d, o + 10)
        texs = [d[o + ot + 16 * i:o + ot + 16 * i + 16].split(b"\0")[0].decode("latin-1") for i in range(num_tex)]
        pals = [d[o + op + 16 * i:o + op + 16 * i + 16].split(b"\0")[0].decode("latin-1") for i in range(num_pl)]
        mats = []
        for mname, md in g3d.read_dict(d, o + 12):
            count, scale, ofs = g3d.u16(md, 0), g3d.s16(md, 4), g3d.u16(md, 6)
            tbl = [(g3d.u16(d, o + ofs + 4 * i), d[o + ofs + 4 * i + 2], d[o + ofs + 4 * i + 3]) for i in range(count)]
            seq = []
            for key in range(nf):
                i = ((scale * key) >> 12) & 0xFFFFFFFF
                if i >= count:
                    i = count - 1
                while i != 0 and tbl[i][0] >= key:
                    i -= 1
                while i + 1 < count and tbl[i + 1][0] <= key:
                    i += 1
                seq.append((tbl[i][1], tbl[i][2]))
            mats.append({"material": mname, "frames": seq})
        out.append({"name": name, "num_frame": nf, "materials": mats, "textures": texs, "palettes": pals})
    return out


def btp_pairs(anims, material_names, static_pal):
    """(texture, palette) pairs a model needs for the pattern animations that bind to its materials.
    A key without a palette (0xFF) keeps the material's own palette."""
    pairs = []
    for a in anims:
        for m in a["materials"]:
            if m["material"] not in material_names:
                continue
            for ti, pi in set(m["frames"]):
                if ti >= len(a["textures"]):
                    continue
                pal = a["palettes"][pi] if pi != 0xFF and pi < len(a["palettes"]) else static_pal.get(m["material"])
                p = (a["textures"][ti], pal)
                if p not in pairs:
                    pairs.append(p)
    return pairs


# ----------------------------------------------------------------------------- writer

def _fixstr(s, n=16):
    return s.encode("latin-1")[:n - 1].ljust(n, b"\0")


def write_set(kind, anims):
    """.dca container (model_format.h): header 20 bytes, dca_anim_t[n], then per animation
    dcx_mat_t[num_materials] {char name[16]; dcx_chan_t ch[5] {u8 mode, pad[3]; s32 value; u32 ofs}}
    and its samples (ofs relative to dca_anim_t.ofs_data): kind 3 u16 (GXRgb / alpha), kind 5 s32,
    kind 4 u16 (texture index | palette index << 8, 0xFF = material palette) in ch[0].
    Kind 4: dca_anim_t.pad = absolute offset of {u16 num_tex, num_pltt; char tex[][16]; char pltt[][16]}."""
    n = len(anims)
    out = bytearray(20 + 32 * n)
    rep = []
    for ai, a in enumerate(anims):
        nf = max(1, a["num_frame"])
        mats = a["materials"]
        name_ofs = 0
        if kind == DCA_KIND_TEXPAT:
            while len(out) % 4:
                out.append(0)
            name_ofs = len(out)
            out += struct.pack("<HH", len(a["textures"]), len(a["palettes"]))
            for s in a["textures"] + a["palettes"]:
                out += _fixstr(s)
        while len(out) % 4:
            out.append(0)
        ofs_ch = len(out)
        out += bytes(76 * len(mats))
        while len(out) % 4:
            out.append(0)
        ofs_data = len(out)
        pool = bytearray()
        for mi, m in enumerate(mats):
            if kind == DCA_KIND_TEXPAT:
                chans = [[t | (p << 8) for t, p in m["frames"]]]
            else:
                chans = m["channels"]
            hdr = bytearray(_fixstr(m["material"]))
            for k in range(5):
                if k >= len(chans):
                    hdr += struct.pack("<B3xiI", 0, 0, 0)
                    continue
                s = list(chans[k])[:nf] or [0]
                if all(v == s[0] for v in s):
                    hdr += struct.pack("<B3xiI", MODE_CONST, s[0], 0)
                else:
                    while len(pool) % 4:
                        pool.append(0)
                    off = len(pool)
                    pool += struct.pack(("<%di" if kind == DCA_KIND_TEXSRT else "<%dH") % len(s), *s)
                    hdr += struct.pack("<B3xiI", MODE_FRAMES, 0, off)
            out[ofs_ch + 76 * mi:ofs_ch + 76 * (mi + 1)] = hdr
        out += pool
        struct.pack_into("<16sHHIII", out, 20 + 32 * ai, _fixstr(a["name"]), nf, len(mats), ofs_ch, ofs_data,
                         name_ofs)
        rep.append({"name": a["name"], "frames": nf, "materials": len(mats)})
    while len(out) % 4:
        out.append(0)
    struct.pack_into("<4sHHIHHI", out, 0, b"DCA1", 1, kind, len(out), n, 0, 20)
    return bytes(out), rep

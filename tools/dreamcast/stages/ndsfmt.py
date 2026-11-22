"""Minimal readers for the Nintendo DS container formats used by stage data.

NARC archives (BTAF/BTNF/GMIF) and the BIOS-style compression headers that
the stage loader dispatches on (0x10 LZ77, 0x30 RL; 0x00 = raw with a 4-byte
header).  Written from the format semantics used by the decompiled loaders
(StageLoader_UnpackMapArchive: type = header byte & 0xF0, size = u32 >> 8).
"""
import struct


def read_narc(data):
    """Return list of file payloads (bytes) of a NARC archive."""
    if data[:4] != b'NARC':
        raise ValueError('not a NARC')
    hdr_size = struct.unpack_from('<H', data, 0xC)[0]
    off = hdr_size
    # BTAF
    if data[off:off + 4] != b'BTAF':
        raise ValueError('missing BTAF')
    btaf_size, count = struct.unpack_from('<IH', data, off + 4)
    fat = [struct.unpack_from('<II', data, off + 12 + i * 8) for i in range(count)]
    off += btaf_size
    # BTNF
    if data[off:off + 4] != b'BTNF':
        raise ValueError('missing BTNF')
    btnf_size = struct.unpack_from('<I', data, off + 4)[0]
    btnf = data[off + 8:off + btnf_size]
    off += btnf_size
    if data[off:off + 4] != b'GMIF':
        raise ValueError('missing GMIF')
    base = off + 8
    files = [bytes(data[base + s:base + e]) for s, e in fat]
    names = _narc_names(btnf, count)
    return files, names


def _narc_names(btnf, count):
    """Flat file names from the root directory of a BTNF table (None if absent)."""
    try:
        if len(btnf) < 8:
            return [None] * count
        sub_off, first_id, ndirs = struct.unpack_from('<IHH', btnf, 0)
        names = []

        def walk(dir_index, prefix):
            o, fid, _ = struct.unpack_from('<IHH', btnf, dir_index * 8)
            while o < len(btnf):
                ln = btnf[o]
                o += 1
                if ln == 0:
                    break
                is_dir = ln & 0x80
                ln &= 0x7F
                nm = btnf[o:o + ln].decode('latin-1')
                o += ln
                if is_dir:
                    sub = struct.unpack_from('<H', btnf, o)[0] & 0xFFF
                    o += 2
                    walk(sub, prefix + nm + '/')
                else:
                    names.append((fid, prefix + nm))
                    fid += 1
        if sub_off == 4 * 0 or ndirs == 0:
            return [None] * count
        walk(0, '')
        out = [None] * count
        for fid, nm in names:
            if fid < count:
                out[fid] = nm
        return out
    except Exception:
        return [None] * count


def comp_type(res):
    return res[0] & 0xF0


def uncomp_size(res):
    return struct.unpack_from('<I', res, 0)[0] >> 8


def decompress(res):
    """Decompress a resource the way the stage loader does."""
    t = comp_type(res)
    size = uncomp_size(res)
    if t == 0x10:
        return lz77(res, size)
    if t == 0x30:
        return rl(res, size)
    if t == 0x20:
        raise NotImplementedError('Huffman resource')
    return bytes(res[4:4 + size])


def lz77(src, size):
    out = bytearray()
    i = 4
    while len(out) < size:
        flags = src[i]
        i += 1
        for b in range(8):
            if len(out) >= size:
                break
            if flags & (0x80 >> b):
                v = (src[i] << 8) | src[i + 1]
                i += 2
                ln = (v >> 12) + 3
                disp = (v & 0xFFF) + 1
                for _ in range(ln):
                    out.append(out[-disp])
            else:
                out.append(src[i])
                i += 1
    return bytes(out[:size])


def rl(src, size):
    out = bytearray()
    i = 4
    while len(out) < size:
        f = src[i]
        i += 1
        if f & 0x80:
            n = (f & 0x7F) + 3
            out += bytes([src[i]]) * n
            i += 1
        else:
            n = (f & 0x7F) + 1
            out += src[i:i + n]
            i += n
    return bytes(out[:size])


def lz77_any(data):
    """Decompress whole-file LZ77 (type 0x10, or 0x11 extended)."""
    t = data[0]
    size = struct.unpack_from('<I', data, 0)[0] >> 8
    if t == 0x10:
        return lz77(data, size)
    if t == 0x11:
        return lz11(data, size)
    raise ValueError('not LZ: 0x%02x' % t)


def lz11(src, size):
    out = bytearray()
    i = 4
    if size == 0:
        size = struct.unpack_from('<I', src, 4)[0]
        i = 8
    while len(out) < size:
        flags = src[i]
        i += 1
        for b in range(8):
            if len(out) >= size:
                break
            if flags & (0x80 >> b):
                b0 = src[i]
                ind = b0 >> 4
                if ind == 0:
                    ln = (((b0 & 0xF) << 4) | (src[i + 1] >> 4)) + 0x11
                    disp = (((src[i + 1] & 0xF) << 8) | src[i + 2]) + 1
                    i += 3
                elif ind == 1:
                    ln = (((b0 & 0xF) << 12) | (src[i + 1] << 4) | (src[i + 2] >> 4)) + 0x111
                    disp = (((src[i + 2] & 0xF) << 8) | src[i + 3]) + 1
                    i += 4
                else:
                    ln = ind + 1
                    disp = (((b0 & 0xF) << 8) | src[i + 1]) + 1
                    i += 2
                for _ in range(ln):
                    out.append(out[-disp])
            else:
                out.append(src[i])
                i += 1
    return bytes(out[:size])


def bgr555_to_rgb(c):
    r = (c & 0x1F) << 3
    g = ((c >> 5) & 0x1F) << 3
    b = ((c >> 10) & 0x1F) << 3
    return (r | r >> 5, g | g >> 5, b | b >> 5)

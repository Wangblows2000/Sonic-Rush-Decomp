#!/usr/bin/env python3
"""Nintendo DS archive + compression helpers (clean-room, written for the Dreamcast port).

Reusable by every converter under tools/dreamcast/:

    from nds_archive import decompress, maybe_decompress, Narc, open_narc

decompress(data)        -> bytes   DS BIOS-style stream with 4-byte header:
                                   0x00 raw ("stored", what Compress_GetData skips)
                                   0x10 LZ77 (LZ10), 0x11 LZ77 (LZ11),
                                   0x24/0x28 Huffman (4/8-bit), 0x30 RLE.
                                   Size = header >> 8; size 0 => 32-bit size follows (LZ11 style).
maybe_decompress(data)  -> bytes   decompress() if the header looks like a valid
                                   compressed stream, else data unchanged.
Narc(data)                         parses NARC (BTAF/BTNF/GMIF); .files is a list of
                                   (path, bytes); names come from BTNF when present,
                                   else "<index>.bin".
open_narc(path)         -> Narc    reads a file, LZ-decompresses it first when it starts
                                   with an LZ header (the *_lz7.narc files are LZ10 streams
                                   wrapping a NARC).

The game's own loader paths: VRAMTransfer_Flush / PaletteTransfer_Flush switch on
(header & 0xF0): 0x10 MI_UncompressLZ16, 0x20 MI_UncompressHuffman, 0x30 MI_UncompressRL16,
default: copy (header >> 8) bytes from header+4 (Compress_GetData).

CLI:  python3 nds_archive.py list  FILE.narc
      python3 nds_archive.py extract FILE.narc OUTDIR
      python3 nds_archive.py decompress IN OUT
"""
import struct
import sys
from pathlib import Path


class DecompressError(ValueError):
    pass


def _header(data):
    if len(data) < 4:
        raise DecompressError("stream shorter than 4 bytes")
    h = struct.unpack_from("<I", data, 0)[0]
    kind, size, pos = h & 0xFF, h >> 8, 4
    if size == 0:
        if len(data) < 8:
            raise DecompressError("missing extended size")
        size = struct.unpack_from("<I", data, 4)[0]
        pos = 8
    return kind, size, pos


def _lz10(data, size, pos):
    out = bytearray()
    n = len(data)
    while len(out) < size:
        if pos >= n:
            raise DecompressError("LZ10 truncated (flags)")
        flags = data[pos]
        pos += 1
        for bit in range(8):
            if len(out) >= size:
                break
            if flags & (0x80 >> bit):
                if pos + 1 >= n:
                    raise DecompressError("LZ10 truncated (ref)")
                b0, b1 = data[pos], data[pos + 1]
                pos += 2
                length = (b0 >> 4) + 3
                disp = (((b0 & 0xF) << 8) | b1) + 1
                if disp > len(out):
                    raise DecompressError("LZ10 back-reference before start")
                for _ in range(length):
                    out.append(out[-disp])
            else:
                if pos >= n:
                    raise DecompressError("LZ10 truncated (literal)")
                out.append(data[pos])
                pos += 1
    return bytes(out[:size])


def _lz11(data, size, pos):
    out = bytearray()
    n = len(data)
    while len(out) < size:
        if pos >= n:
            raise DecompressError("LZ11 truncated (flags)")
        flags = data[pos]
        pos += 1
        for bit in range(8):
            if len(out) >= size:
                break
            if flags & (0x80 >> bit):
                if pos + 1 >= n:
                    raise DecompressError("LZ11 truncated (ref)")
                b0 = data[pos]
                ind = b0 >> 4
                if ind == 0:
                    b1, b2 = data[pos + 1], data[pos + 2]
                    length = (((b0 & 0xF) << 4) | (b1 >> 4)) + 0x11
                    disp = (((b1 & 0xF) << 8) | b2) + 1
                    pos += 3
                elif ind == 1:
                    b1, b2, b3 = data[pos + 1], data[pos + 2], data[pos + 3]
                    length = (((b0 & 0xF) << 12) | (b1 << 4) | (b2 >> 4)) + 0x111
                    disp = (((b2 & 0xF) << 8) | b3) + 1
                    pos += 4
                else:
                    b1 = data[pos + 1]
                    length = ind + 1
                    disp = (((b0 & 0xF) << 8) | b1) + 1
                    pos += 2
                if disp > len(out):
                    raise DecompressError("LZ11 back-reference before start")
                for _ in range(length):
                    out.append(out[-disp])
            else:
                out.append(data[pos])
                pos += 1
    return bytes(out[:size])


def _rle(data, size, pos):
    out = bytearray()
    while len(out) < size:
        if pos >= len(data):
            raise DecompressError("RLE truncated")
        f = data[pos]
        pos += 1
        if f & 0x80:
            ln = (f & 0x7F) + 3
            out += bytes([data[pos]]) * ln
            pos += 1
        else:
            ln = (f & 0x7F) + 1
            out += data[pos:pos + ln]
            pos += ln
    return bytes(out[:size])


def _huffman8(data, size, pos):
    """8-bit Huffman (type 0x28). Tree size byte at pos, root node at pos+1, 32-bit MSB-first bitstream."""
    tree_size = (data[pos] + 1) * 2
    stream = pos + tree_size
    out = bytearray()
    acc = nacc = 0
    while len(out) < size:
        node = pos + 1
        while True:
            if nacc == 0:
                if stream + 4 > len(data):
                    raise DecompressError("Huffman truncated")
                acc = struct.unpack_from("<I", data, stream)[0]
                stream += 4
                nacc = 32
            bit = (acc >> 31) & 1
            acc = (acc << 1) & 0xFFFFFFFF
            nacc -= 1
            nd = data[node]
            child = (node & ~1) + (nd & 0x3F) * 2 + 2 + bit
            if nd & (0x40 if bit else 0x80):
                out.append(data[child])
                break
            node = child
    return bytes(out)


def decompress(data):
    """Decompress a DS compressed stream (see module docstring)."""
    data = bytes(data)
    kind, size, pos = _header(data)
    if kind == 0x00:
        if pos + size > len(data):
            raise DecompressError("stored stream truncated")
        return data[pos:pos + size]
    if kind == 0x10:
        return _lz10(data, size, pos)
    if kind == 0x11:
        return _lz11(data, size, pos)
    if kind == 0x30:
        return _rle(data, size, pos)
    if kind in (0x24, 0x28):
        if kind == 0x24:
            return _huffman4(data, size, pos)
        return _huffman8(data, size, pos)
    raise DecompressError("unknown compression type 0x%02x" % kind)


def _huffman4(data, size, pos):
    tree_size = (data[pos] + 1) * 2
    stream = pos + tree_size
    nibbles = []
    acc = nacc = 0
    while len(nibbles) < size * 2:
        node = pos + 1
        while True:
            if nacc == 0:
                if stream + 4 > len(data):
                    raise DecompressError("Huffman truncated")
                acc = struct.unpack_from("<I", data, stream)[0]
                stream += 4
                nacc = 32
            bit = (acc >> 31) & 1
            acc = (acc << 1) & 0xFFFFFFFF
            nacc -= 1
            nd = data[node]
            child = (node & ~1) + (nd & 0x3F) * 2 + 2 + bit
            if nd & (0x40 if bit else 0x80):
                nibbles.append(data[child] & 0xF)
                break
            node = child
    return bytes(nibbles[i] | (nibbles[i + 1] << 4) for i in range(0, size * 2, 2))


def looks_compressed(data):
    if len(data) < 5:
        return False
    kind, size, _ = _header(data)
    return kind in (0x10, 0x11) and 0 < size < 64 * 1024 * 1024


def maybe_decompress(data):
    """LZ-decompress when the bytes start with a plausible LZ10/LZ11 header, else return as is."""
    if data[:4] in (b"NARC", b"BAC\n"):
        return bytes(data)
    if looks_compressed(data):
        try:
            return decompress(data)
        except DecompressError:
            return bytes(data)
    return bytes(data)


class Narc:
    """NARC container: BTAF (file allocation), BTNF (names), GMIF (image)."""

    def __init__(self, data):
        if data[:4] != b"NARC":
            raise ValueError("not a NARC (magic %r)" % data[:4])
        hsize, nsec = struct.unpack_from("<HH", data, 12)
        pos = hsize
        sections = {}
        for _ in range(nsec):
            magic = data[pos:pos + 4]
            ssize = struct.unpack_from("<I", data, pos + 4)[0]
            sections[magic] = (pos, ssize)
            pos += ssize
        fat_pos, _ = sections[b"BTAF"]
        count = struct.unpack_from("<H", data, fat_pos + 8)[0]
        self.ranges = [struct.unpack_from("<II", data, fat_pos + 12 + 8 * i) for i in range(count)]
        img_pos, _ = sections[b"GMIF"]
        img = img_pos + 8
        names = self._names(data, sections.get(b"BTNF"), count)
        self.files = [(names[i], bytes(data[img + s:img + e])) for i, (s, e) in enumerate(self.ranges)]

    @staticmethod
    def _names(data, sec, count):
        names = ["%04d.bin" % i for i in range(count)]
        if sec is None:
            return names
        base = sec[0] + 8
        sub_off, first_id, ndirs = struct.unpack_from("<IHH", data, base)
        if sub_off <= 8 and ndirs <= 1 and data[base + sub_off] == 0:
            return names  # no names stored
        dirs = {0xF000: ""}

        def walk(dir_id, prefix):
            off, fid, _ = struct.unpack_from("<IHH", data, base + 8 * (dir_id & 0xFFF))
            p = base + off
            while True:
                ln = data[p]
                p += 1
                if ln == 0:
                    break
                is_dir = ln & 0x80
                name = data[p:p + (ln & 0x7F)].decode("latin-1")
                p += ln & 0x7F
                if is_dir:
                    sub = struct.unpack_from("<H", data, p)[0]
                    p += 2
                    walk(sub, prefix + name + "/")
                else:
                    if fid < count:
                        names[fid] = prefix + name
                    fid += 1
        try:
            walk(0xF000, "")
        except (struct.error, IndexError):
            pass
        return names


def open_narc(path):
    return Narc(maybe_decompress(Path(path).read_bytes()))


def _main(argv):
    if len(argv) >= 2 and argv[0] == "list":
        n = open_narc(argv[1])
        for i, (name, b) in enumerate(n.files):
            print("%4d %8d %-32s %r" % (i, len(b), name, b[:4]))
    elif len(argv) >= 3 and argv[0] == "extract":
        n = open_narc(argv[1])
        out = Path(argv[2])
        for name, b in n.files:
            p = out / name
            p.parent.mkdir(parents=True, exist_ok=True)
            p.write_bytes(b)
        print("extracted %d files to %s" % (len(n.files), out))
    elif len(argv) >= 3 and argv[0] == "decompress":
        Path(argv[2]).write_bytes(decompress(Path(argv[1]).read_bytes()))
    else:
        print(__doc__)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(_main(sys.argv[1:]))

#!/usr/bin/env python3
"""Menu .bbg -> Dreamcast texture converter (BYOR: reads the owner's extracted ROM).

BBG (STAGES.md section 5, Background_Init): "BBG\\0", u32 size, u32 chr ofs,
u32 screen ofs, u32 palette ofs, u32 flags (bits 0-3 type), u16 w, u16 h (tiles),
u16 +0x1C palette colour offset (Background_Init: dest += unk1c * 2),
u16 +0x1E palette slot (dest += unk1e * 512).
Every sub-resource has a DS compression header (nds_archive.decompress).

Types seen in the menu BBGs: 1 = 4 bpp text BG (16-colour banks, bits 12-15 of
the screen entry), 2 = 8 bpp text BG with an extended palette, 5 = 8 bpp
(affine-extended BG, same 16-bit screen entries). Pixel value 0 is transparent
on the DS for every type (the backdrop shows through).

Output "MNBG" v1 (little-endian), indexed so the runtime can apply the
palette transfers of the menu code (TitleScreen_PaletteCycle etc.):
  0  "MNBG"
  4  u16 width, u16 height     image size in px (w*8, h*8)
  8  u16 type (BBG flags & 0xF), u16 pofs (+0x1C), u16 pslot (+0x1E), u16 0
  16 u16 palette[256]          DS BGR555 as the BG palette slot holds it
                               (BBG palette placed at colour pofs)
  528 u8 pixels[height][width] palette index; 4 bpp tiles are flattened to
                               bank*16 + value; 0 = transparent (DS colour 0)
"""
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT / "tools/dreamcast/sprites"))
from nds_archive import decompress  # noqa: E402


def bgr_to_argb(c, opaque=True):
    return (0x8000 if opaque else 0) | (c & 0x1F) << 10 | (c & 0x3E0) | (c >> 10 & 0x1F)


def parse(data):
    if data[:4] != b"BBG\0":
        raise ValueError("not a BBG")
    _, ochr, oscr, opal, flags, w, h, pofs, pslot = struct.unpack_from("<IIIIIHHHH", data, 4)
    chars = decompress(data[ochr:]) if ochr else b""
    scr = decompress(data[oscr:]) if oscr else b""
    pal = decompress(data[opal:]) if opal else b""
    return dict(type=flags & 0xF, flags=flags, w=w, h=h, pofs=pofs, pslot=pslot,
                chars=chars, screen=scr, pal=pal)


def palette(b):
    """256 BGR555 colours as the BG palette slot sees them (palette at +pofs colours)."""
    cols = [0] * 256
    n = len(b["pal"]) // 2
    for i in range(n):
        if b["pofs"] + i < 256:
            cols[b["pofs"] + i] = struct.unpack_from("<H", b["pal"], 2 * i)[0]
    return cols


def render(b):
    """-> (w_px, h_px, palette index per pixel, 0 = transparent)."""
    W, H = b["w"] * 8, b["h"] * 8
    bpp8 = b["type"] != 1
    tsz = 64 if bpp8 else 32
    chars, scr = b["chars"], b["screen"]
    nchar = len(chars) // tsz
    out = [0] * (W * H)
    for ty in range(b["h"]):
        for tx in range(b["w"]):
            i = ty * b["w"] + tx
            if 2 * i + 2 > len(scr):
                continue
            e = struct.unpack_from("<H", scr, 2 * i)[0]
            c, hf, vf, bank = e & 0x3FF, e >> 10 & 1, e >> 11 & 1, e >> 12
            if c >= nchar:
                continue
            for y in range(8):
                sy = 7 - y if vf else y
                for x in range(8):
                    sx = 7 - x if hf else x
                    if bpp8:
                        v = chars[c * 64 + sy * 8 + sx]
                        if v == 0:
                            continue
                        col = v
                    else:
                        byte = chars[c * 32 + sy * 4 + sx // 2]
                        v = byte >> 4 if sx & 1 else byte & 0xF
                        if v == 0:
                            continue
                        col = (bank * 16 + v) & 0xFF
                    out[(ty * 8 + y) * W + tx * 8 + x] = col
    return W, H, out


def to_mnbg(b, W, H, px):
    pal = palette(b)
    return (struct.pack("<4sHHHHHH", b"MNBG", W, H, b["type"], b["pofs"], b["pslot"], 0) +
            struct.pack("<256H", *pal) + bytes(px))


def preview(path, W, H, px, pal):
    from PIL import Image
    im = Image.new("RGBA", (W, H))
    im.putdata([((pal[p] & 31) * 255 // 31, (pal[p] >> 5 & 31) * 255 // 31, (pal[p] >> 10 & 31) * 255 // 31,
                 255 if p else 0) for p in px])
    im.save(path)


def convert(src, out_dir, prev_dir=None):
    b = parse(Path(src).read_bytes())
    W, H, px = render(b)
    name = Path(src).stem
    blob = to_mnbg(b, W, H, px)
    (Path(out_dir) / (name + ".mnbg")).write_bytes(blob)
    if prev_dir:
        preview(Path(prev_dir) / (name + ".png"), W, H, px, palette(b))
    return name, b, W, H, sum(1 for p in px if p)


if __name__ == "__main__":
    out = Path(sys.argv[1])
    out.mkdir(parents=True, exist_ok=True)
    pv = out.parent / "preview"
    pv.mkdir(exist_ok=True)
    for s in sys.argv[2:]:
        n, b, W, H, op = convert(s, out, pv)
        print("%-28s type %d %4dx%-4d opaque %6d pal@%d slot %d" % (n, b["type"], W, H, op, b["pofs"], b["pslot"]))

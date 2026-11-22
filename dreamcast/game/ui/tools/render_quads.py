#!/usr/bin/env python3
"""Dev tool: render a UI quad dump (test_ui with UI_DUMP=dir) to a 640x480 PNG,
like ui_draw.c does on the PVR (nearest filtering, alpha blend, vertex colour,
variants, colour maps, the streamed navigator face, main-screen brightness).

usage: render_quads.py dump.txt out.png [--sprites build/dreamcast/sprites/bac]
       [--res build/dreamcast/ui/res] [--uidata build/dreamcast/ui/uidata.bin]
       [--bg r,g,b | --under image.png]

Texture ids: sprite resources are looked up in --sprites, then in --res (the
converted BG / navigator resources); "navifaces_<c>" is the face set in --res.
--under: a 640x480 picture used as the world under the UI (e.g. a level render).
"""
import argparse
import os
import struct
from PIL import Image


def uidata_sections(path):
    d = open(path, "rb").read()
    n = struct.unpack_from("<H", d, 6)[0]
    out = {}
    for i in range(n):
        name, off, sz = struct.unpack_from("<12sII", d, 8 + 20 * i)
        out[name.rstrip(b"\0").decode()] = d[off:off + sz]
    return out


def bgr_rgb(c):
    r, g, b = c & 31, (c >> 5) & 31, (c >> 10) & 31
    return (r * 255 // 31, g * 255 // 31, b * 255 // 31)


class Atlases:
    def __init__(self, sprites, res):
        self.dirs = [sprites, res]
        self.res = res
        self.cache = {}
        self.frames = {}

    def page(self, tex, page):
        k = (tex, page)
        if k not in self.cache:
            for d in self.dirs:
                p = os.path.join(d, tex, "atlas%d.argb1555" % page)
                if os.path.exists(p):
                    raw = open(p, "rb").read()
                    w, h = self.page_size(tex, page)
                    px = struct.unpack("<%dH" % (w * h), raw)
                    im = Image.new("RGBA", (w, h))
                    im.putdata([(((v >> 10) & 31) * 255 // 31, ((v >> 5) & 31) * 255 // 31, (v & 31) * 255 // 31,
                                 255 if v & 0x8000 else 0) for v in px])
                    self.cache[k] = im
                    break
        return self.cache[k]

    def table(self, tex):
        if tex not in self.frames:
            for d in self.dirs:
                p = os.path.join(d, tex, "frames.bin")
                if os.path.exists(p):
                    b = open(p, "rb").read()
                    npg, ni = struct.unpack_from("<HH", b, 6)
                    pages = [struct.unpack_from("<HH", b, 16 + 4 * i) for i in range(npg)]
                    o = 16 + 4 * npg
                    ims = [struct.unpack_from("<BBHHHHhhH", b, o + 16 * i) for i in range(ni)]
                    self.frames[tex] = (pages, ims)
                    break
        return self.frames[tex]

    def page_size(self, tex, page):
        return self.table(tex)[0][page]

    def image(self, tex, idx):
        pages, ims = self.table(tex)
        im = ims[idx]
        return im[0], self.page(tex, im[0])

    def face(self, tex, f):
        k = (tex, "face", f)
        if k not in self.cache:
            b = open(os.path.join(self.res, tex + ".bin"), "rb").read()
            n, w, h = struct.unpack_from("<HHH", b, 4)
            o = 12 + f * (512 + w * h)
            pal = struct.unpack_from("<256H", b, o)
            px = b[o + 512:o + 512 + w * h]
            im = Image.new("RGBA", (w, h))
            im.putdata([bgr_rgb(pal[p]) + (255,) if p else (0, 0, 0, 0) for p in px])
            self.cache[k] = im
        return self.cache[k]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("dump")
    ap.add_argument("out")
    ap.add_argument("--sprites", default="build/dreamcast/sprites/bac")
    ap.add_argument("--res", default="build/dreamcast/ui/res")
    ap.add_argument("--uidata", default="build/dreamcast/ui/uidata.bin")
    ap.add_argument("--bg", default="70,110,160")
    ap.add_argument("--under", default=None)
    a = ap.parse_args()
    sec = uidata_sections(a.uidata)
    gauge = struct.unpack("<8H", sec["gauge_pal"])
    lut = list(sec["pause_lut"])
    if a.under:
        img = Image.open(a.under).convert("RGBA").resize((640, 480))
    else:
        img = Image.new("RGBA", (640, 480), tuple(int(x) for x in a.bg.split(",")) + (255,))
    at = Atlases(a.sprites, a.res)
    lines = open(a.dump).read().splitlines()
    bm, bs = [int(x) for x in lines[0].split()[1:]]
    maps = {}
    for ln in lines[1:]:
        f = ln.split()
        if f[0] == "palmap":   # palette-row swap: colours of image src -> image dst
            var, tex, s_, d_ = int(f[1]), f[2], int(f[3]), int(f[4])
            pages, ims = at.table(tex)
            si, di = ims[s_], ims[d_]
            ps, pd = at.page(tex, si[0]), at.page(tex, di[0])
            m = {}
            for yy in range(min(si[5], di[5])):
                for xx in range(min(si[4], di[4])):
                    c = ps.getpixel((si[2] + xx, si[3] + yy))
                    if c[3]:
                        m.setdefault(c, pd.getpixel((di[2] + xx, di[3] + yy)))
            maps[var] = m
            continue
        tex, image = f[0], int(f[1])
        x, y, w, h, u0, v0, u1, v1 = [float(v) for v in f[2:10]]
        argb, var = int(f[10], 16), int(f[11])
        if w <= 0 or h <= 0:
            continue
        if tex == "-":
            col = ((argb >> 16) & 255, (argb >> 8) & 255, argb & 255, argb >> 24)
            ov = Image.new("RGBA", (max(1, round(w)), max(1, round(h))), col)
            img.alpha_composite(ov, (round(x), round(y)))
            continue
        if tex.startswith("navifaces_"):
            src = at.face(tex, image)
        else:
            _, src = at.image(tex, image)
        l, r_ = min(u0, u1), max(u0, u1)
        t, b = min(v0, v1), max(v0, v1)
        # a zero-width texel range is one stretched texel (uniform BG fill)
        if r_ - l < 1:
            r_ = l + 1
        if b - t < 1:
            b = t + 1
        c = src.crop((int(l), int(t), max(int(l) + 1, int(round(r_))), max(int(t) + 1, int(round(b)))))
        if var:
            px = c.load()
            for yy in range(c.size[1]):
                for xx in range(c.size[0]):
                    p = px[xx, yy]
                    if p[3] == 0:
                        continue
                    if 1 <= var <= 7 and p[:3] == (255, 255, 255):
                        px[xx, yy] = bgr_rgb(gauge[var - 1]) + (255,)
                    elif var == 8:
                        cb = (p[0] * 31 // 255) | (p[1] * 31 // 255) << 5 | (p[2] * 31 // 255) << 10
                        o = lut[cb & 15] | lut[(cb >> 4) & 15] << 4 | lut[(cb >> 8) & 15] << 8 | lut[(cb >> 12) & 15] << 12
                        px[xx, yy] = bgr_rgb(o & 0x7fff) + (255,)
                    elif var in maps and p in maps[var]:
                        px[xx, yy] = maps[var][p]
        if u0 > u1:
            c = c.transpose(Image.FLIP_LEFT_RIGHT)
        if v0 > v1:
            c = c.transpose(Image.FLIP_TOP_BOTTOM)
        c = c.resize((max(1, round(x + w) - round(x)), max(1, round(y + h) - round(y))), Image.NEAREST)
        if argb != 0xffffffff:   # vertex colour (modulate)
            ca, cr, cg, cbl = argb >> 24, (argb >> 16) & 255, (argb >> 8) & 255, argb & 255
            rr, gg, bb, aa = c.split()
            rr = rr.point(lambda v: v * cr // 255)
            gg = gg.point(lambda v: v * cg // 255)
            bb = bb.point(lambda v: v * cbl // 255)
            aa = aa.point(lambda v: v * ca // 255)
            c = Image.merge("RGBA", (rr, gg, bb, aa))
        canvas = Image.new("RGBA", img.size, (0, 0, 0, 0))
        canvas.paste(c, (round(x), round(y)))
        img.alpha_composite(canvas)
    bri = bm if abs(bm) >= abs(bs) else bs
    if bri:
        al = min(255, abs(bri) * 255 // 16)
        img.alpha_composite(Image.new("RGBA", img.size, (255, 255, 255, al) if bri > 0 else (0, 0, 0, al)))
    img.convert("RGB").save(a.out)


if __name__ == "__main__":
    main()

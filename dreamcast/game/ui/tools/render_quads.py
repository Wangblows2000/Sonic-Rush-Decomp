#!/usr/bin/env python3
"""Dev tool: render a UI quad dump (test_ui with UI_DUMP=dir) to a 640x480 PNG,
like ui_draw.c does on the PVR (nearest filtering, alpha blend, variants, brightness).
usage: render_quads.py dump.txt out.png [--sprites build/dreamcast/sprites/bac] [--uidata build/dreamcast/ui/uidata.bin] [--bg r,g,b]"""
import argparse, struct
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

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("dump"); ap.add_argument("out")
    ap.add_argument("--sprites", default="build/dreamcast/sprites/bac")
    ap.add_argument("--uidata", default="build/dreamcast/ui/uidata.bin")
    ap.add_argument("--bg", default="70,110,160")
    a = ap.parse_args()
    sec = uidata_sections(a.uidata)
    gauge = struct.unpack("<8H", sec["gauge_pal"])
    lut = list(sec["pause_lut"])
    img = Image.new("RGBA", (640, 480), tuple(int(x) for x in a.bg.split(",")) + (255,))
    atl = {}
    lines = open(a.dump).read().splitlines()
    bm, bs = [int(x) for x in lines[0].split()[1:]]
    for ln in lines[1:]:
        f = ln.split()
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
        if tex not in atl:
            atl[tex] = Image.open("%s/%s/atlas0.png" % (a.sprites, tex)).convert("RGBA")
        src = atl[tex]
        l, r_ = min(u0, u1), max(u0, u1)
        t, b = min(v0, v1), max(v0, v1)
        c = src.crop((int(l), int(t), int(r_), int(b)))
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
        if u0 > u1:
            c = c.transpose(Image.FLIP_LEFT_RIGHT)
        if v0 > v1:
            c = c.transpose(Image.FLIP_TOP_BOTTOM)
        c = c.resize((max(1, round(x + w) - round(x)), max(1, round(y + h) - round(y))), Image.NEAREST)
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

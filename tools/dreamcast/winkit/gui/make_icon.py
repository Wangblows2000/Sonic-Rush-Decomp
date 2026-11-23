#!/usr/bin/env python3
"""Draw the disc maker's icon (an original, generic optical disc) -> srdcgui.ico"""
import sys
from PIL import Image, ImageDraw

def disc(n):
    s = n * 4                     # supersample
    im = Image.new("RGBA", (s, s), (0, 0, 0, 0))
    d = ImageDraw.Draw(im)
    c = s / 2
    def circle(r, fill):
        d.ellipse((c - r, c - r, c + r, c + r), fill=fill)
    circle(s * 0.48, (40, 70, 150, 255))         # blue rim
    circle(s * 0.44, (215, 222, 232, 255))       # silver surface
    circle(s * 0.36, (190, 200, 215, 255))
    d.pieslice((c - s * 0.44, c - s * 0.44, c + s * 0.44, c + s * 0.44), 200, 250, fill=(245, 248, 252, 255))
    circle(s * 0.15, (40, 70, 150, 255))         # blue hub
    circle(s * 0.07, (0, 0, 0, 0))               # hole
    return im.resize((n, n), Image.LANCZOS)

out = sys.argv[1] if len(sys.argv) > 1 else "srdcgui.ico"
big = disc(256)
big.save(out, sizes=[(16, 16), (24, 24), (32, 32), (48, 48), (64, 64), (128, 128), (256, 256)])
print(out)

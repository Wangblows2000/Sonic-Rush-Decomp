#!/usr/bin/env python3
"""Controller art for the special stages (Dreamcast controller scheme, SPECIAL.md section 4).

Runs after spdata.py has converted the sprites and BGs (BYOR: everything here is derived
from the converted ROM art in build/dreamcast/special, nothing is drawn from outside art):

  sprites/act__ac_spst_pad/   a new DSPR sprite set (frames.bin + atlas0.argb1555, then
                              PAL4 via tools/dreamcast/sprites/atlas_pal4.py):
      anim 0            "PRESS START TO BEGIN" in the white-plate caption font of
                        ac_spst_<lang> anim 0 ("TOUCH THE TOUCH SCREEN TO CONTINUE"), same
                        image size and anchor, letters sliced from the English / French /
                        German art (G: the caption-bar font's G in plate colours)
      anims 42..57      the Trick Spring targets 1..8 (ac_spst anims 42..57: plain + lit
                        4-frame animations): the digit is removed (radial fill of the disc
                        rings) and the Dreamcast button glyph drawn in its place, outlined
                        like the digits; the disc is recoloured to the pad button colour
      anim 62           the target burst (copied from ac_spst anim 62)
      anims 65..68      targets 9 and 10 (plain, lit); the SSD data has at most 8 targets
      anims 70..        controller legend / pause lines in the caption-bar font
  bg/sp_stdm_caption_eng.mnbg   the scrolling caption "USE THE STYLUS TO MOVE SONIC" redrawn
                        as "USE STICK OR < > TO MOVE SONIC" with the same font and palette

Button glyphs are plain letters / trigger initials / arrows on round badges (generic shapes,
no console artwork). Colours: A red, B blue, X yellow, Y green, L/R silver, D-pad charcoal.

Usage: padart.py [--out build/dreamcast/special]
"""
import argparse
import colorsys
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT / "tools/dreamcast/sprites"))

# ---------------------------------------------------------------- DSPR / mnbg io


def argb_to_rgba(v):
    return (((v >> 10) & 31) * 255 // 31, ((v >> 5) & 31) * 255 // 31, (v & 31) * 255 // 31, 255 if v & 0x8000 else 0)


def rgba_to_argb(c):
    if len(c) == 4 and c[3] < 128:
        return 0
    r, g, b = c[:3]
    return 0x8000 | (r * 31 + 127) // 255 << 10 | (g * 31 + 127) // 255 << 5 | (b * 31 + 127) // 255


class Dspr:
    def __init__(self, d):
        f = (Path(d) / "frames.bin").read_bytes()
        _, npages, nimg, nanim, nfr, _ = struct.unpack_from("<HHHHHH", f, 4)
        self.pages = [struct.unpack_from("<HH", f, 16 + 4 * i) for i in range(npages)]
        o = 16 + 4 * npages
        self.imgs = [struct.unpack_from("<BBHHHHhhH", f, o + 16 * i) for i in range(nimg)]
        o += 16 * nimg
        self.anims = [struct.unpack_from("<HHhhBBH", f, o + 12 * i) for i in range(nanim)]
        o += 12 * nanim
        self.frames = [struct.unpack_from("<HH", f, o + 4 * i) for i in range(nfr)]
        W, H = self.pages[0]
        self.W, self.H = W, H
        self.px = struct.unpack("<%dH" % (W * H), (Path(d) / "atlas0.argb1555").read_bytes())

    def image(self, i):
        """-> (w, h, ox, oy, rows of RGBA tuples)"""
        _, _, x, y, w, h, ox, oy, _ = self.imgs[i]
        return w, h, ox, oy, [[argb_to_rgba(self.px[(y + yy) * self.W + x + xx]) for xx in range(w)] for yy in range(h)]

    def anim_frames(self, a):
        ff, fc = self.anims[a][0], self.anims[a][1]
        return [self.frames[ff + k] for k in range(fc)]


def load_mnbg(p):
    d = Path(p).read_bytes()
    W, H = struct.unpack_from("<HH", d, 4)
    return d, W, H


# ---------------------------------------------------------------- fonts
# caption-bar font (sp_stdm_caption_*): palette indices 47 black, 46 white, 45 / 43 / 42 shades
CAP_TEXT = {
    "eng": "USE THE STYLUS TO MOVE SONIC",
    "fra": "BOUGER SONIC AVEC STYLET",
    "deu": "SONIC M. TOUCHPEN BEWEGEN",
    "spa": "LÁPIZ TÁCTIL:MOVER SONIC",
    "ita": "STILO:MUOVERE SONIC",
}
CAP_BG = 47
CAP_WHITE = 46
# caption index -> plate colour (white plate, black glyphs): inverted shades
CAP_TO_PLATE = {46: (0, 0, 0), 45: (90, 90, 90), 43: (139, 139, 139), 42: (189, 189, 189)}

# glyphs the ROM fonts do not have, in the caption font's 12-row, 2-px stroke style
# ('#' = white, '+' = mid shade 43, '.' = black)
EXTRA_CAP = {
    "K": ["##..##", "##..##", "##.##.", "##.##.", "####..", "###...", "###...", "####..", "##.##.", "##.##.",
          "##..##", "##..##"],
    "X": ["##..##", "##..##", "##..##", ".####.", ".####.", "..##..", "..##..", ".####.", ".####.", "##..##",
          "##..##", "##..##"],
    "<": ["....##", "...###", "..####", ".#####", "######", "######", "######", "######", ".#####", "..####",
          "...###", "....##"],
    ">": ["##....", "###...", "####..", "#####.", "######", "######", "######", "######", "#####.", "####..",
          "###...", "##...."],
    "/": ["....##", "....##", "...##.", "...##.", "..##..", "..##..", "..##..", "..##..", ".##...", ".##...",
          "##....", "##...."],
}


def caption_font(bgdir):
    """letter -> list of 12 rows of palette indices (caption-bar font, rows 2..13 of the bar)"""
    font = {}
    for lang, text in CAP_TEXT.items():
        src = bgdir / ("sp_stdm_caption_%s.mnbg" % lang)
        if (bgdir.parent / ".padart" / src.name).exists():   # the English bar as converted, before redraw_caption
            src = bgdir.parent / ".padart" / src.name
        d, W, H = load_mnbg(src)
        px = d[528:528 + W * H]
        cols = [any(px[y * W + x] not in (0, CAP_BG) for y in range(2, 14)) for x in range(W)]
        runs, x = [], 0
        while x < W:
            if cols[x]:
                s = x
                while x < W and cols[x]:
                    x += 1
                runs.append((s, x))
            else:
                x += 1
        chars = [c for c in text if c != " "]
        if len(runs) != len(chars):
            raise SystemExit("caption font %s: %d glyphs, text has %d" % (lang, len(runs), len(chars)))
        for c, (a, b) in zip(chars, runs):
            if c not in font:
                font[c] = [[px[y * W + x] if px[y * W + x] else CAP_BG for x in range(a, b)] for y in range(2, 14)]
    for c, rows in EXTRA_CAP.items():
        font[c] = [[CAP_WHITE if ch == "#" else 43 if ch == "+" else CAP_BG for ch in r] for r in rows]
    return font


def kern(prev, nxt):
    if prev == " " or nxt == " ":
        return 0
    if prev in "TV" or nxt == "V":
        return 1
    return 2


def text_width(font, s):
    w = 0
    for i, c in enumerate(s):
        if c == " ":
            w += 4
        else:
            w += len(font[c][0])
        if i + 1 < len(s):
            w += kern(c, s[i + 1])
    return w


# white-plate font (ac_spst_<lang> anim 0): sliced per letter with the plate rows
PLATE_SRC = {
    "eng": ["TOUCHTHE", "TOUCHSCREEN", "TOCONTINUE"],
    "fra": ["TOUCHER", None, "TACTILEPOUR", "CONTINUER"],
    "deu": [None, "TOUCH", "SCREEN", "BERUHREN"],
}
DARK = {(0, 0, 0), (189, 189, 189), (90, 90, 90), (213, 213, 213), (139, 139, 139)}


def plate_font(sprdir):
    """letter -> 18 columns-rows image slice (rows band-2 .. band+15), plus caps and a gap column"""
    font, caps = {}, None
    for lang, lines in PLATE_SRC.items():
        s = Dspr(sprdir / ("act__ac_spst_" + lang))
        i = s.anim_frames(0)[0][0]
        w, h, _, _, rows = s.image(i)
        blk = [y for y in range(h) if any(rows[y][x][3] and rows[y][x][:3] == (0, 0, 0) for x in range(w))]
        bands, a, p = [], blk[0], blk[0]
        for r in blk[1:]:
            if r != p + 1:
                bands.append((a, p))
                a = r
            p = r
        bands.append((a, p))
        for (b0, b1), text in zip(bands, lines):
            cols = [any(rows[y][x][3] and rows[y][x][:3] in DARK for y in range(b0, b1 + 1)) for x in range(w)]
            runs, x = [], 0
            while x < w:
                if cols[x]:
                    st = x
                    while x < w and cols[x]:
                        x += 1
                    runs.append((st, x))
                else:
                    x += 1
            if text is None:
                continue
            if len(runs) != len(text):
                raise SystemExit("plate font %s %s: %d glyphs" % (lang, text, len(runs)))
            band = [rows[y] for y in range(b0 - 2, b0 + 16)]
            for c, (x0, x1) in zip(text, runs):
                if c not in font:   # "BERUHREN": its U is the U-umlaut, but eng comes first
                    font[c] = [r[x0:x1] for r in band]
            if lang == "eng" and text == "TOUCHSCREEN":
                # the word SCREEN: caps and the gap column
                sx0 = runs[5][0]
                ex1 = runs[10][1]
                l0 = sx0
                while l0 > 0 and band[0][l0 - 1][3] or l0 > 0 and band[8][l0 - 1][3]:
                    l0 -= 1
                r1 = ex1
                while r1 < w and (band[0][r1][3] or band[8][r1][3]):
                    r1 += 1
                gap = runs[5][1]
                caps = ([r[l0:sx0] for r in band], [r[ex1:r1] for r in band], [r[gap] for r in band])
    return font, caps


def plate_word(pfont, cfont, caps, word):
    left, right, gapcol = caps
    cols = []   # list of columns (18 RGBA each)
    for k in range(len(left[0])):
        cols.append([left[y][k] for y in range(18)])
    for i, c in enumerate(word):
        if c in pfont:   # the glyph rows on the gap column (plate rows of the source words differ)
            g = pfont[c]
            for k in range(len(g[0])):
                col = list(gapcol)
                for y in range(2, 14):
                    col[y] = g[y][k] if g[y][k][3] and g[y][k][:3] in DARK else gapcol[y]
                cols.append(col)
        else:   # caption-font glyph in plate colours on the gap column
            g = cfont[c]
            for k in range(len(g[0])):
                col = list(gapcol)
                for y in range(12):
                    v = g[y][k]
                    if v != CAP_BG:
                        col[2 + y] = CAP_TO_PLATE.get(v, (0, 0, 0)) + (255,)
                cols.append(col)
        if i + 1 < len(word):
            for _ in range(kern(c, word[i + 1])):
                cols.append(list(gapcol))
    for k in range(len(right[0])):
        cols.append([right[y][k] for y in range(18)])
    return [[cols[x][y] for x in range(len(cols))] for y in range(18)]


def blit(dst, src, x0, y0):
    for y, r in enumerate(src):
        for x, c in enumerate(r):
            if c[3] and 0 <= y0 + y < len(dst) and 0 <= x0 + x < len(dst[0]):
                dst[y0 + y][x0 + x] = c


def blank(w, h):
    return [[(0, 0, 0, 0)] * w for _ in range(h)]


def make_prompt(pfont, cfont, caps, w, h, lines):
    img = blank(w, h)
    words = [[plate_word(pfont, cfont, caps, wd) for wd in ln.split(" ")] for ln in lines]
    total = 18 * len(lines) + 6 * (len(lines) - 1)
    y = (h - total) // 2
    for ln in words:
        lw = sum(len(wd[0]) for wd in ln) + 4 * (len(ln) - 1)
        x = (w - lw) // 2
        for wd in ln:
            blit(img, wd, x, y)
            x += len(wd[0]) + 4
        y += 24
    return img


# ---------------------------------------------------------------- target badges
BUTTONS = ["A", "B", "X", "Y", "L", "R", "UP", "RIGHT", "DOWN", "LEFT"]
# hue (degrees) of the disc, or None = grey; value scale
BUTTON_COL = {"A": (0, 1.0), "B": (225, 1.0), "X": (50, 1.0), "Y": (125, 1.0), "L": (None, 0.80), "R": (None, 0.80),
              "UP": (None, 0.62), "RIGHT": (None, 0.62), "DOWN": (None, 0.62), "LEFT": (None, 0.62)}

G = {
    "A": ["..#######..", ".#########.", "###.....###", "###.....###", "###.....###", "###.....###", "###.....###",
          "###########", "###########", "###########", "###.....###", "###.....###", "###.....###", "###.....###",
          "###.....###", "###.....###"],
    "B": ["#########..", "##########.", "###....####", "###.....###", "###.....###", "###....####", "#########..",
          "##########.", "###....####", "###.....###", "###.....###", "###.....###", "###....####", "###########",
          "##########.", "#########.."],
    "X": ["###.....###", "###.....###", ".###...###.", ".###...###.", "..###.###..", "..###.###..", "...#####...",
          "....###....", "....###....", "...#####...", "..###.###..", "..###.###..", ".###...###.", ".###...###.",
          "###.....###", "###.....###"],
    "Y": ["###.....###", "###.....###", ".###...###.", ".###...###.", "..###.###..", "..###.###..", "...#####...",
          "....###....", "....###....", "....###....", "....###....", "....###....", "....###....", "....###....",
          "....###....", "....###...."],
    "L": ["###........"] * 13 + ["###########"] * 3,
    "R": ["#########..", "##########.", "###....####", "###.....###", "###.....###", "###....####", "##########.",
          "#########..", "###..###...", "###...###..", "###...###..", "###....###.", "###....###.", "###.....###",
          "###.....###", "###.....###"],
}


def arrow(direction):
    # solid triangle, base 15, height 14 (pointing up), then rotated
    rows = []
    for i in range(14):
        half = (i * 7 + 6) // 13
        rows.append("".join("#" if abs(x - 7) <= half else "." for x in range(15)))
    if direction == "UP":
        return rows
    if direction == "DOWN":
        return rows[::-1]
    t = ["".join(rows[y][x] for y in range(len(rows))) for x in range(15)]   # transpose: points left
    return t if direction == "LEFT" else [r[::-1] for r in t]


for _d in ("UP", "RIGHT", "DOWN", "LEFT"):
    G[_d] = arrow(_d)


def recolour(c, button):
    hue, val = BUTTON_COL[button]
    r, g, b, a = c
    if not a:
        return c
    h, s, v = colorsys.rgb_to_hsv(r / 255, g / 255, b / 255)
    if s < 0.12:   # white / greys stay
        return c
    if hue is None:
        s = s * 0.10
        v = v * val
    else:
        # blue (240) family -> button hue, the cyan/blue spread kept at half width
        dh = (h * 360 - 240)
        h = ((hue + dh * 0.5) % 360) / 360
        if button == "X":
            v = min(1.0, v * 1.05)
    r2, g2, b2 = colorsys.hsv_to_rgb(h, s, v)
    return (int(r2 * 255 + 0.5), int(g2 * 255 + 0.5), int(b2 * 255 + 0.5), 255)


def clean_disc(src, fill_rgb):
    """the target disc without its digit: every pixel takes the most common colour of the
    pixels at the same radius outside the digit (+2 px outline); src is the frame of target 1
    (the narrowest digit), the disc rings are the same for every number"""
    h, w = len(src), len(src[0])
    cx, cy = (w - 1) / 2, (h - 1) / 2
    fill = [[src[y][x][3] and src[y][x][:3] == fill_rgb for x in range(w)] for y in range(h)]
    mask = [[any(fill[y + dy][x + dx] for dy in range(-3, 4) for dx in range(-3, 4)
                 if 0 <= y + dy < h and 0 <= x + dx < w) for x in range(w)] for y in range(h)]
    bins = {}
    for y in range(h):
        for x in range(w):
            if not mask[y][x]:
                r = round(((x - cx) ** 2 + (y - cy) ** 2) ** 0.5 * 2)
                bins.setdefault(r, {}).setdefault(src[y][x], 0)
                bins[r][src[y][x]] += 1
    out = [list(r) for r in src]
    for y in range(h):
        for x in range(w):
            if mask[y][x]:
                r = round(((x - cx) ** 2 + (y - cy) ** 2) ** 0.5 * 2)
                for rr in (r, r - 1, r + 1, r - 2, r + 2, r - 3, r + 3):
                    if rr in bins:
                        out[y][x] = max(bins[rr].items(), key=lambda kv: kv[1])[0]
                        break
    return out


def make_badge(disc, button):
    """disc: 32x32 RGBA rows of a target without its digit"""
    h, w = len(disc), len(disc[0])
    cx, cy = (w - 1) / 2, (h - 1) / 2
    out = [list(r) for r in disc]
    out = [[recolour(c, button) for c in r] for r in out]
    # the glyph: fill colour of the frame's digit, 2-px dark outline as the digits have
    g = G[button]
    gh, gw = len(g), len(g[0])
    gx, gy = int(round(cx - (gw - 1) / 2)), int(round(cy - (gh - 1) / 2))
    on = [[False] * w for _ in range(h)]
    for yy in range(gh):
        for xx in range(gw):
            if g[yy][xx] == "#":
                on[gy + yy][gx + xx] = True
    dark = recolour((0, 41, 156, 255), button)
    if button in ("A", "B", "X", "Y"):
        dark = tuple(int(v * 0.75) for v in dark[:3]) + (255,)
    for y in range(h):
        for x in range(w):
            if on[y][x]:
                continue
            near = any(on[y + dy][x + dx] for dy in range(-2, 3) for dx in range(-2, 3)
                       if 0 <= y + dy < h and 0 <= x + dx < w and abs(dy) + abs(dx) <= 3)
            if near and out[y][x][3]:
                out[y][x] = dark
    for y in range(h):
        for x in range(w):
            if on[y][x]:
                out[y][x] = (255, 255, 255, 255)
    return out


# ---------------------------------------------------------------- legend lines (caption font)
WHITE = (255, 255, 255, 255)


def cap_rgba(idx, pal):
    return argb_to_rgba(pal[idx] | 0x8000) if idx != CAP_BG else (0, 0, 0, 255)


def small_badge(button, cfont, pal):
    n = 18
    img = blank(n, n)
    base = {"A": (220, 40, 40), "B": (40, 90, 230), "X": (240, 200, 30), "Y": (40, 170, 60), "L": (170, 170, 180),
            "R": (170, 170, 180), "UP": (80, 80, 90), "RIGHT": (80, 80, 90), "DOWN": (80, 80, 90), "LEFT": (80, 80, 90),
            "STICK": (120, 120, 130)}[button]
    rim = tuple(int(v * 0.45) for v in base)
    hi = tuple(min(255, int(v * 0.6 + 255 * 0.4)) for v in base)
    c = (n - 1) / 2
    for y in range(n):
        for x in range(n):
            d = ((x - c) ** 2 + (y - c) ** 2) ** 0.5
            if d <= 8.6:
                img[y][x] = (rim if d > 7.4 else hi if (y - c) < -3.5 and d > 4.5 else base) + (255,)
    if button == "STICK":
        for y in range(n):
            for x in range(n):
                d = ((x - c) ** 2 + (y - c - 0.5) ** 2) ** 0.5
                if d <= 4.6:
                    img[y][x] = ((60, 60, 66) if d > 3.4 else (200, 200, 210)) + (255,)
        return img
    if button in ("UP", "RIGHT", "DOWN", "LEFT"):
        a = arrow(button)
        # 15 -> 8 px: sample every 2nd pixel
        a = [r[::2] for r in a[::2]]
        ox, oy = (n - len(a[0])) // 2, (n - len(a)) // 2
        for y, r in enumerate(a):
            for x, ch in enumerate(r):
                if ch == "#":
                    img[oy + y][ox + x] = WHITE
        return img
    g = cfont[button]
    ox, oy = (n - len(g[0])) // 2, (n - 12) // 2
    for y in range(12):
        for x in range(len(g[0])):
            v = g[y][x]
            if v == CAP_WHITE or v == 45:
                img[oy + y][ox + x] = (rim if button == "X" else (255, 255, 255)) + (255,)
            elif v != CAP_BG:
                img[oy + y][ox + x] = tuple((a + b) // 2 for a, b in zip(base, (255, 255, 255))) + (255,)
    return img


def legend_line(items, cfont, pal):
    """items: strings (caption text) and ('B', name) badges -> RGBA rows, 20 px high"""
    H = 20
    parts = []
    for it in items:
        if isinstance(it, tuple):
            parts.append(small_badge(it[1], cfont, pal))
        else:
            w = text_width(cfont, it)
            img = blank(w, H)
            x = 0
            for i, ch in enumerate(it):
                if ch == " ":
                    x += 4
                else:
                    g = cfont[ch]
                    for y in range(12):
                        for xx in range(len(g[0])):
                            if g[y][xx] != CAP_BG:
                                img[4 + y][x + xx] = cap_rgba(g[y][xx], pal)
                    x += len(g[0])
                if i + 1 < len(it):
                    x += kern(ch, it[i + 1])
            parts.append(img)
    W = sum(len(p[0]) for p in parts) + 3 * (len(parts) - 1)
    out = blank(W, H)
    x = 0
    for p in parts:
        blit(out, p, x, (H - len(p)) // 2)
        x += len(p[0]) + 3
    return out


LEGEND = [   # anim 70 + index (sp_draw.c, SP_PAD_LINE_*)
    ["CONTROLS"],
    [("B", "STICK"), "OR", ("B", "LEFT"), ("B", "RIGHT"), ": MOVE SONIC"],
    [("B", "A"), ": ATTACK"],
    ["TRICK : PRESS THE BUTTONS SHOWN"],
    [("B", b) for b in BUTTONS],
    ["START : PAUSE"],
    ["PRESS START TO BEGIN"],
    ["PAUSE"],
    ["START OR", ("B", "A"), ": CONTINUE"],
    [("B", "B"), ": GO BACK"],
]

# ---------------------------------------------------------------- caption bar


def redraw_caption(path, cfont, text):
    orig = Path(path).parent.parent / ".padart" / Path(path).name   # kept (not on the disc) for a later run
    orig.parent.mkdir(exist_ok=True)
    if not orig.exists():
        orig.write_bytes(Path(path).read_bytes())
    d, W, H = load_mnbg(orig)
    d = bytearray(d)
    px = d[528:528 + W * H]
    # clear the glyph rows to the bar's black, then draw the text centred
    for y in range(H):
        for x in range(W):
            if px[y * W + x] not in (0,):
                px[y * W + x] = CAP_BG
    w = text_width(cfont, text)
    x = (W - w) // 2
    for i, ch in enumerate(text):
        if ch == " ":
            x += 4
        else:
            g = cfont[ch]
            for y in range(12):
                for xx in range(len(g[0])):
                    px[(2 + y) * W + x + xx] = g[y][xx]
            x += len(g[0])
        if i + 1 < len(text):
            x += kern(ch, text[i + 1])
    d[528:528 + W * H] = px
    Path(path).write_bytes(bytes(d))


# ---------------------------------------------------------------- atlas


def pack(images, W):
    """shelf packer: images = list of (w, h); -> positions, height used"""
    pos, x, y, sh = [], 1, 1, 0
    for w, h in images:
        if x + w + 1 > W:
            x, y, sh = 1, y + sh + 2, 0
        pos.append((x, y))
        x += w + 2
        sh = max(sh, h)
    return pos, y + sh + 1


def write_dspr(d, images, anims, frames, W, H):
    """images: list of (rows, ox, oy); anims: list of (first, count, loop, next); frames: (img, dur)"""
    d.mkdir(parents=True, exist_ok=True)
    pos, used = pack([(len(r[0][0]), len(r[0])) for r in images], W)
    if used > H:
        raise SystemExit("pad atlas: %d rows needed, page %d" % (used, H))
    px = [0] * (W * H)
    fb = struct.pack("<4sHHHHHH", b"DSPR", 1, 1, len(images), len(anims), len(frames), 0)
    fb += struct.pack("<HH", W, H)
    for (rows, ox, oy), (x, y) in zip(images, pos):
        h, w = len(rows), len(rows[0])
        for yy in range(h):
            for xx in range(w):
                px[(y + yy) * W + x + xx] = rgba_to_argb(rows[yy][xx])
        fb += struct.pack("<BBHHHHhhH", 0, 0, x, y, w, h, ox, oy, 0)
    for a in anims:
        fb += struct.pack("<HHhhBBH", a[0], a[1], a[2], a[3], 0, 0, 0)
    for f in frames:
        fb += struct.pack("<HH", *f)
    (d / "frames.bin").write_bytes(fb)
    (d / "atlas0.argb1555").write_bytes(struct.pack("<%dH" % len(px), *px))
    return used


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--out", default=str(ROOT / "build/dreamcast/special"))
    a = ap.parse_args(argv)
    out = Path(a.out)
    sprdir, bgdir = out / "sprites", out / "bg"
    cfont = caption_font(bgdir)
    pfont, caps = plate_font(sprdir)
    cap_pal = struct.unpack_from("<256H", load_mnbg(bgdir / "sp_stdm_caption_eng.mnbg")[0], 16)
    spst = Dspr(sprdir / "act__ac_spst")
    eng = Dspr(sprdir / "act__ac_spst_eng")

    images, frames = [], []
    anims = [(0, 0, 0, -1)] * 80

    def add_anim(a, imgs_durs, loop=0):
        first = len(frames)
        for img, dur in imgs_durs:
            if img is None:
                frames.append((0xFFFF, dur))
            else:
                images.append(img)
                frames.append((len(images) - 1, dur))
        anims[a] = (first, len(imgs_durs), loop, -1)

    # anim 0: the prompt (same size / anchor as ac_spst_eng anim 0)
    w, h, ox, oy, _ = eng.image(eng.anim_frames(0)[0][0])
    add_anim(0, [((make_prompt(pfont, cfont, caps, w, h, ["PRESS", "START", "TO BEGIN"]), ox, oy),
                  eng.anim_frames(0)[0][1])])
    # targets: anims 42 + 2k (plain) / 43 + 2k (lit) from the number art of target 1 (k < 8 use their own number)
    discs = {}
    for lit in (0, 1):
        for n, (img, dur) in enumerate(spst.anim_frames(42 + lit)):
            w, h, ox, oy, rows = spst.image(img)
            discs[lit, n] = (clean_disc(rows, (255, 255, 255) if lit else (189, 189, 255)), ox, oy, dur)
    for k, b in enumerate(BUTTONS):
        for lit in (0, 1):
            seq = []
            for n in range(4 if lit else 1):
                disc, ox, oy, dur = discs[lit, n]
                seq.append(((make_badge(disc, b), ox, oy), dur))
            add_anim((42 + 2 * k + lit) if k < 8 else (65 + 2 * (k - 8) + lit), seq)
    # anim 62: target burst
    seq = []
    for img, dur in spst.anim_frames(62):
        if img == 0xFFFF:
            seq.append((None, dur))
        else:
            w, h, ox, oy, rows = spst.image(img)
            seq.append(((rows, ox, oy), dur))
    add_anim(62, seq)
    # anims 70+: legend lines (anchor: top-left)
    for i, items in enumerate(LEGEND):
        rows = legend_line(items, cfont, cap_pal)
        add_anim(70 + i, [((rows, 0, 0), 1)])
    padd = sprdir / "act__ac_spst_pad"
    used = write_dspr(padd, images, anims, frames, 512, 256)
    import atlas_pal4
    r = atlas_pal4.convert(padd)
    print("pad sprites: %d images, %d anims, page 512x256 (%d rows used), %s" %
          (len(images), len(anims), used, "PAL4" if r and r[0][0] else "ARGB1555"))
    # caption bar
    redraw_caption(bgdir / "sp_stdm_caption_eng.mnbg", cfont, "USE STICK OR < > TO MOVE SONIC")
    files = ["sprites/act__ac_spst_pad/" + f.name for f in sorted(padd.iterdir())]
    return files


if __name__ == "__main__":
    for f in main():
        print(f)

#!/usr/bin/env python3
"""ARGB1555 sprite atlas -> PVR PAL4 atlas (palettes.bin + atlas<N>.pt4), same layout.

For atlases that were not made by bac2dc.py from a BAC (the UI backgrounds of
dreamcast/game/ui/tools/uibg.py, copies made by the special-stage / boss data tools):
reads frames.bin ("DSPR" v1) and atlas<N>.argb1555 of each directory and writes, next to
them, the files bac2dc.py writes (SPRITES.md section 4, engine/sprpal.h). A page becomes
PAL4 when every image on it has at most 15 colours; the images' colours are grouped
first-fit into shared 16-entry palettes (image_row = 0xFE, remapped). Every texel is
checked against the ARGB1555 pixel. Pages with a 16+ colour image stay ARGB1555.

Usage: atlas_pal4.py DIR [DIR ...]      (a DIR without frames.bin is searched recursively)
"""
import os
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from bac2dc import twiddle_index  # noqa: E402

PAL_DIRECT, PAL_REMAP = 0xFF, 0xFE


def convert(d):
    d = Path(d)
    fb = (d / "frames.bin").read_bytes()
    if fb[:4] != b"DSPR":
        return None
    _, npages, nimg, nanim, nfr, _ = struct.unpack_from("<HHHHHH", fb, 4)
    pages = [struct.unpack_from("<HH", fb, 16 + 4 * i) for i in range(npages)]
    imgs = [struct.unpack_from("<BBHHHHhhH", fb, 16 + 4 * npages + 16 * i) for i in range(nimg)]
    pals, pal_key = [], {}
    image_pal = [None] * nimg
    page_fmt = []
    for pn, (W, H) in enumerate(pages):
        raw = d / ("atlas%d.argb1555" % pn)
        if not raw.exists() or raw.stat().st_size != W * H * 2:
            page_fmt.append(0)
            continue
        px = struct.unpack("<%dH" % (W * H), raw.read_bytes())
        on = [i for i, im in enumerate(imgs) if im[0] == pn]
        cols = {}
        for i in on:
            _, _, x, y, w, h = imgs[i][:6]
            cols[i] = {px[(y + yy) * W + x + xx] for yy in range(h) for xx in range(w)
                       if px[(y + yy) * W + x + xx] & 0x8000}
        if any(len(c) > 15 for c in cols.values()):
            page_fmt.append(0)
            continue
        groups = []
        for i in on:
            for g in groups:
                if len(g[0] | cols[i]) <= 15:
                    g[0] |= cols[i]
                    g[1].append(i)
                    break
            else:
                groups.append([set(cols[i]), [i]])
        data = bytearray(W * H // 2)
        for cs, members in groups:
            order = sorted(cs)
            t = tuple([0] + order + [0] * (15 - len(order)))
            if t not in pal_key:
                pal_key[t] = len(pals)
                pals.append(t)
            pos = {c: k + 1 for k, c in enumerate(order)}
            for i in members:
                image_pal[i] = pal_key[t]
                _, _, x, y, w, h = imgs[i][:6]
                for yy in range(h):
                    for xx in range(w):
                        c = px[(y + yy) * W + x + xx]
                        if c & 0x8000:
                            m = twiddle_index(x + xx, y + yy, W, H)
                            data[m >> 1] |= pos[c] << ((m & 1) * 4)
        # exactness: texel + palette == ARGB1555 pixel inside every image rect
        for i in on:
            p = pals[image_pal[i]]
            _, _, x, y, w, h = imgs[i][:6]
            for yy in range(h):
                for xx in range(w):
                    m = twiddle_index(x + xx, y + yy, W, H)
                    want = px[(y + yy) * W + x + xx]
                    got = p[(data[m >> 1] >> ((m & 1) * 4)) & 15]
                    if got != (want if want & 0x8000 else 0):
                        raise SystemExit("%s page %d: PAL4 mismatch at %d,%d" % (d, pn, x + xx, y + yy))
        (d / ("atlas%d.pt4" % pn)).write_bytes(bytes(data))
        page_fmt.append(1)
    # mask: the entries a group palette defines (remapped palettes are dense from 1;
    # colour 0x0000 never occurs: every opaque ARGB1555 colour has bit 15)
    masks = [sum(1 << j for j in range(1, 16) if t[j]) for t in pals]

    def pad4(b):
        return b + bytes(-len(b) % 4)
    blob = struct.pack("<4sHHHHHH", b"DCPL", 1, npages, nimg, nanim, len(pals), 0)
    blob += pad4(bytes(page_fmt))
    blob += b"".join(struct.pack("<16H", *t) for t in pals)
    blob += struct.pack("<%dH" % len(pals), *masks)
    blob += struct.pack("<%dH" % nimg, *[0xFFFF if p is None else p for p in image_pal])
    blob += struct.pack("<%dH" % nanim, *([0xFFFF] * nanim))
    blob += bytes(PAL_DIRECT if p is None else PAL_REMAP for p in image_pal)
    (d / "palettes.bin").write_bytes(pad4(blob))
    return page_fmt, len(pals)


def main(argv):
    n = 0
    for a in argv:
        dirs = [Path(a)] if (Path(a) / "frames.bin").exists() else sorted(p.parent for p in Path(a).rglob("frames.bin"))
        for d in dirs:
            r = convert(d)
            if r:
                n += 1
                print("%-60s pages %s, %d palettes" % (d, "".join("P" if f else "D" for f in r[0]), r[1]))
    print("%d atlases" % n)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))

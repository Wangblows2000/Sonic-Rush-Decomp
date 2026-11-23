#!/usr/bin/env python3
"""BAC (Sonic Rush sprite animation) -> Dreamcast sprite data.

Bring-your-own-ROM: reads extract/eur/files (loose act/, act2/ .bac files and every .bac inside
the *.narc archives, LZ10-compressed or not) and writes to build/dreamcast/sprites/ (gitignored).

Per unique BAC (identical files found in several archives are converted once):
  bac/<id>/atlas<N>.png            RGBA atlas page(s), power-of-two, <= 1024x1024, 1 px gutter
  bac/<id>/atlas<N>.argb1555       same pixels, raw little-endian ARGB1555 (or .argb4444 with
                                   --format 4444), row-major; twiddled for the PVR with --twiddle
  bac/<id>/frames.json             animations, frames (atlas rect, origin, duration), loop info,
                                   hitbox events, palette/tile diagnostics
  bac/<id>/frames.bin              compact table, see docs/dreamcast/formats/SPRITES.md ("DSPR")
  preview/<id>/aNNN_fMMM.png       every composed frame (RGBA, untrimmed, origin = JSON origin)
  preview/<id>/sheet.png           all animations, one row each, dark background, origin cross
manifest.json                      every source (path, sha1, size) -> outputs (+ sizes)
report.json                        counts converted / data-only / failed + failure reasons

Usage:
  python3 tools/dreamcast/sprites/bac2dc.py [--extract extract/eur] [--out build/dreamcast/sprites]
          [--only SUBSTR ...] [--no-preview] [--format 1555|4444] [--twiddle] [-j N]
"""
import argparse
import hashlib
import json
import os
import shutil
import struct
import sys
import traceback
from multiprocessing import Pool
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from bac_format import Bac, BacError, RenderState, render, FMT_NAMES, bgr555_to_rgba  # noqa: E402
from nds_archive import open_narc, maybe_decompress  # noqa: E402

from PIL import Image, ImageDraw  # noqa: E402

MAX_TEX = 1024
GUTTER = 1


# ---------------------------------------------------------------- sources
def find_sources(files_root):
    """Yield (source_path_str, id, bytes). Loose files first, then NARC members."""
    root = Path(files_root)
    for p in sorted(root.rglob("*.bac")):
        rel = p.relative_to(root).as_posix()
        yield rel, rel[:-4].replace("/", "__"), p.read_bytes()
    for p in sorted(root.rglob("*.narc")):
        rel = p.relative_to(root).as_posix()
        try:
            narc = open_narc(p)
        except Exception as e:  # pragma: no cover - reported by caller
            yield rel, None, e
            continue
        for name, data in narc.files:
            if name.endswith(".bac") and data[:4] != b"BAC\n":
                data = maybe_decompress(data)  # tolerate LZ-compressed members
            if data[:4] == b"BAC\n" or name.endswith(".bac"):
                ident = (rel[:-5] + "/" + name[:-4]).replace("/", "__")
                yield rel + ":" + name, ident, data


# ---------------------------------------------------------------- packing
def pow2(n):
    p = 8
    while p < n:
        p <<= 1
    return p


# Converted BACs whose loaders take one page only (special/sp_sprite.c, boss/boss_extra_draw.c)
SINGLE_PAGE = ("ac_spst", "ac_ex_", "ac_itm_ring")
MAX_RATIO = 8          # page aspect ratio limit (PVR: any power-of-two pair 8..1024; twiddled blocks)
PAGE_COST = 1024       # texels an extra page must save to be worth its allocation and draw state


def _sizes():
    return [(8 << i, 8 << j) for i in range(8) for j in range(8)
            if (8 << i) <= MAX_TEX and (8 << j) <= MAX_TEX and (8 << i) <= (8 << j) * MAX_RATIO
            and (8 << j) <= (8 << i) * MAX_RATIO]


def _mr_place(free, w, h):
    """MaxRects, best short side fit. free: list of (x, y, w, h). Returns (new free list, (x, y))."""
    best = None
    for fx, fy, fw, fh in free:
        if w <= fw and h <= fh:
            k = (min(fw - w, fh - h), max(fw - w, fh - h), fy, fx)
            if best is None or k < best[0]:
                best = (k, fx, fy)
    if best is None:
        return None
    _, x, y = best
    nf = []
    for fx, fy, fw, fh in free:
        if x >= fx + fw or x + w <= fx or y >= fy + fh or y + h <= fy:
            nf.append((fx, fy, fw, fh))
            continue
        if x > fx:
            nf.append((fx, fy, x - fx, fh))
        if x + w < fx + fw:
            nf.append((x + w, fy, fx + fw - x - w, fh))
        if y > fy:
            nf.append((fx, fy, fw, y - fy))
        if y + h < fy + fh:
            nf.append((fx, y + h, fw, fy + fh - y - h))
    out = []
    for i, a in enumerate(nf):
        if any(j != i and a[0] >= b[0] and a[1] >= b[1] and a[0] + a[2] <= b[0] + b[2] and a[1] + a[3] <= b[1] + b[3]
               and (a != b or j < i) for j, b in enumerate(nf)):
            continue
        out.append(a)
    return out, (x, y)


ORDERS = (lambda t: (-t[2], -t[1], t[0]), lambda t: (-t[1], -t[2], t[0]), lambda t: (-t[1] * t[2], t[0]))


def _fill(items, W, H, order=ORDERS[0]):
    """Place what fits on a W x H page. Returns ({idx: (x, y)}, rest)."""
    free, pos, rest = [(0, 0, W, H)], {}, []
    for idx, w, h in sorted(items, key=order):
        r = _mr_place(free, w, h)
        if r is None:
            rest.append((idx, w, h))
        else:
            free, pos[idx] = r
    return pos, rest


def _single(items):
    """Smallest power-of-two page that holds every item, or None."""
    area = sum(w * h for _, w, h in items)
    mw, mh = max(w for _, w, _ in items), max(h for _, _, h in items)
    for W, H in sorted((s for s in _sizes() if s[0] * s[1] >= area and s[0] >= mw and s[1] >= mh),
                       key=lambda s: (s[0] * s[1], abs(s[0] - s[1]), -s[0])):
        for order in ORDERS:
            pos, rest = _fill(items, W, H, order)
            if not rest:
                return (W, H, pos)
    return None


def pack_pages(items, max_pages=4):
    """items: list of (idx, w, h) incl. gutter. Returns pages [(W, H, {idx: (x, y)})] with the least
    texture memory: one page, or a page filled first and the rest on more pages (MaxRects)."""
    if not items:
        return []
    for _, w, h in items:
        if w > MAX_TEX or h > MAX_TEX:
            raise BacError("frame %dx%d larger than %d" % (w, h, MAX_TEX))

    def cost(pages):
        return sum(W * H for W, H, _ in pages) + PAGE_COST * (len(pages) - 1)

    def solve(items, depth):
        one = _single(items)
        best = [one] if one else None
        if depth <= 1 or len(items) < 2:
            if best is None:   # more than one MAX_TEX page: fill full pages
                pos, rest = _fill(items, MAX_TEX, MAX_TEX)
                if not pos:
                    raise BacError("cannot pack")
                more = solve(rest, 1) if rest else []
                return [(MAX_TEX, MAX_TEX, pos)] + more
            return best
        limit = one[0] * one[1] if one else MAX_TEX * MAX_TEX * 4
        for W, H in sorted((s for s in _sizes() if s[0] * s[1] < limit or not one), key=lambda s: -s[0] * s[1])[:12]:
            pos, rest = _fill(items, W, H)
            if not pos or not rest:
                continue
            sub = solve(rest, depth - 1)
            cand = [(W, H, pos)] + sub
            if best is None or cost(cand) < cost(best):
                best = cand
        if best is None:
            return solve(items, 1)
        return best

    return solve(items, max_pages)


# ---------------------------------------------------------------- textures
def twiddle_index(x, y, w, h):
    """PVR twiddled (Morton, x in odd bits / y in even bits) address for a w x h pow2 texture;
    non-square textures are a row/column of square twiddled blocks."""
    s = min(w, h)
    blk = (x // s) if w > h else (y // s)
    x %= s
    y %= s
    idx = 0
    bit = 0
    while (1 << bit) < s:
        idx |= ((y >> bit) & 1) << (2 * bit)
        idx |= ((x >> bit) & 1) << (2 * bit + 1)
        bit += 1
    return blk * s * s + idx


def encode_texture(img, fmt, twiddle):
    w, h = img.size
    px = img.tobytes()
    out = [0] * (w * h)
    for y in range(h):
        for x in range(w):
            q = (y * w + x) * 4
            r, g, b, a = px[q], px[q + 1], px[q + 2], px[q + 3]
            if fmt == "1555":
                v = ((1 if a >= 128 else 0) << 15) | ((r >> 3) << 10) | ((g >> 3) << 5) | (b >> 3)
            else:
                v = ((a >> 4) << 12) | ((r >> 4) << 8) | ((g >> 4) << 4) | (b >> 4)
            out[twiddle_index(x, y, w, h) if twiddle else y * w + x] = v
    return struct.pack("<%dH" % len(out), *out)


# ---------------------------------------------------------------- conversion
def trim(r):
    """Crop a rendered frame to its opaque pixels. Returns (img, ox, oy, idx) or None if empty.
    idx = the cropped palette index plane (palette base + index, 0 = transparent)."""
    img = Image.frombytes("RGBA", (r["w"], r["h"]), r["rgba"])
    bbox = img.getchannel("A").getbbox()
    if bbox is None:
        return None
    x0, y0, x1, y1 = bbox
    plane = r["idx"]
    idx = b"".join(plane[y * r["w"] + x0:y * r["w"] + x1] for y in range(y0, y1))
    return img.crop(bbox), r["ox"] - x0, r["oy"] - y0, idx


# ---------------------------------------------------------------- palettes (PAL4 atlases)
PAL_DIRECT, PAL_REMAP = 0xFF, 0xFE   # image_row values besides the raw DS row 0..15


def c1555(rgba):
    r, g, b, a = rgba
    return ((1 if a >= 128 else 0) << 15) | ((r >> 3) << 10) | ((g >> 3) << 5) | (b >> 3)


def classify(im):
    """PVR form of one image: ('raw', row, pal16, idx4) when every opaque pixel reads one
    16-colour DS palette row with a non-zero index (the texture keeps the DS index and the
    bank is that row: palette swaps / PaletteAnim become bank selection); ('remap', colours,
    None, None) when it has at most 15 colours; ('direct',) otherwise."""
    vals = set(im["idx"]) - {0}
    rows = {v >> 4 for v in vals}
    if len(rows) == 1 and all(v & 15 for v in vals):
        row = rows.pop()
        pal = [0] + [c1555(im["pal"][row * 16 + i]) for i in range(1, 16)]
        return ("raw", row, tuple(pal), bytes(v & 15 for v in im["idx"]))
    px = im["img"].tobytes()
    cols = {c1555(px[q:q + 4]) for q in range(0, len(px), 4) if px[q + 3] >= 128}
    if len(cols) <= 15:
        return ("remap", cols, None, None)
    return ("direct",)


def assign_palettes(images):
    """Fill im['pal'] (index into the returned palette list or None = direct), im['row'] and
    im['idx4'] (4-bit indices, row-major, 0 = transparent). Remapped images are grouped
    first-fit into shared palettes of <= 15 colours (fewer PVR palette banks)."""
    pals, pal_key = [], {}

    def pal_id(t):
        if t not in pal_key:
            pal_key[t] = len(pals)
            pals.append(t)
        return pal_key[t]

    groups = []   # [set(colours), [image]]
    for im in images:
        c = classify(im)
        if c[0] == "raw":
            im.update(pal=pal_id(c[2]), row=c[1], idx4=c[3])
        elif c[0] == "remap":
            for g in groups:
                if len(g[0] | c[1]) <= 15:
                    g[0] |= c[1]
                    g[1].append(im)
                    break
            else:
                groups.append([set(c[1]), [im]])
        else:
            im.update(pal=None, row=PAL_DIRECT, idx4=None)
    for cols, members in groups:
        order = sorted(cols)
        t = tuple([0] + order + [0] * (15 - len(order)))
        pid = pal_id(t)
        pos = {c: i + 1 for i, c in enumerate(order)}
        for im in members:
            px = im["img"].tobytes()
            idx4 = bytes(pos[c1555(px[q:q + 4])] if px[q + 3] >= 128 else 0 for q in range(0, len(px), 4))
            im.update(pal=pid, row=PAL_REMAP, idx4=idx4)
    return pals, pal_id


def row0_palette(st):
    """OBJ palette row 0 of a sprite after a frame (ObjDraw_LoadPaletteRow copies one row of
    the animation's palette), ARGB1555 with entry 0 transparent; None without a palette."""
    if not st.has_palette:
        return None
    out = [0]
    for i in range(1, 16):
        c = st.pal.get(i)
        if c is None:
            g = (i * 17) & 0xFF
            out.append(c1555((g, g, g, 255)))
        else:
            out.append(c1555(bgr555_to_rgba(c)))
    return tuple(out)


def encode_pal4(page_w, page_h, placed):
    """PVR PAL4 twiddled texture (4 bpp, texel m of the twiddled order in byte m >> 1,
    low nibble first: KOS pvr_txr_load_ex PVR_TXRLOAD_4BPP order). placed: [(x, y, im)]."""
    out = bytearray(page_w * page_h // 2)
    for x0, y0, im in placed:
        w, h, idx4 = im["img"].width, im["img"].height, im["idx4"]
        for y in range(h):
            row = idx4[y * w:(y + 1) * w]
            for x in range(w):
                v = row[x]
                if v:
                    m = twiddle_index(x0 + x, y0 + y, page_w, page_h)
                    out[m >> 1] |= v << ((m & 1) * 4)
    return bytes(out)


def decode_pal4(data, page_w, page_h, x, y):
    m = twiddle_index(x, y, page_w, page_h)
    return (data[m >> 1] >> ((m & 1) * 4)) & 15


def pack_palettes(pages, images, anim_pals, pals):
    """'DCPL' v1 (palettes.bin, little-endian). See SPRITES.md section 4."""
    def pad4(b):
        return b + bytes(-len(b) % 4)
    hdr = struct.pack("<4sHHHHHH", b"DCPL", 1, len(pages), len(images), len(anim_pals), len(pals), 0)
    masks = [0] * len(pals)   # palette entries some image's texels use (bank sharing)
    for im in images:
        if im["pal"] is not None:
            for v in set(im["idx4"]) - {0}:
                masks[im["pal"]] |= 1 << v
    body = pad4(bytes(1 if p["pvr_format"] == "PAL4" else 0 for p in pages))
    body += b"".join(struct.pack("<16H", *p) for p in pals)
    body += struct.pack("<%dH" % len(pals), *masks)
    body += struct.pack("<%dH" % len(images), *[0xFFFF if im["pal"] is None else im["pal"] for im in images])
    body += struct.pack("<%dH" % len(anim_pals), *[0xFFFF if a is None else a for a in anim_pals])
    body += bytes(im["row"] for im in images)
    return pad4(hdr + body)


def convert_one(job):
    ident, data, out_root, opts = job
    out = Path(out_root)
    bdir = out / "bac" / ident
    pdir = out / "preview" / ident
    bdir.mkdir(parents=True, exist_ok=True)
    for old in list(bdir.glob("atlas*")) + [bdir / "palettes.bin"]:
        if old.exists():
            old.unlink()   # page count / formats can change between runs
    bac = Bac(data)
    stats = {"frames": 0, "empty_frames": 0, "no_tile_frames": 0, "no_palette_frames": 0}
    images = []          # unique trimmed images
    image_key = {}
    anims_json = []
    anim_row0 = []       # per animation: OBJ row 0 palette after its first frame
    previews = []        # (anim, frame, PIL img full, ox, oy)
    for a in bac.animations:
        st = RenderState(bac, a, opts["mapping_shift"])
        frames_json = []
        for fi, f in enumerate(a.frames):
            st.apply(f)
            if fi == 0:
                anim_row0.append(row0_palette(st))
            stats["frames"] += 1
            fj = {"duration": f.duration, "frame_no": f.frame_no, "frame_count": f.frame_count,
                  "image": -1}
            if f.events:
                fj["events"] = f.events
            asm = bac.assembly(f)
            if asm is not None:
                fj["bbox"] = [asm.left, asm.top, asm.right, asm.bottom]
                fj["pieces"] = asm.count
            if asm is None or asm.count == 0:
                stats["empty_frames"] += 1
            elif not st.has_tiles:
                stats["no_tile_frames"] += 1
                fj["note"] = "no tile data (assembly/hitbox-only animation)"
            else:
                if not st.has_palette:
                    stats["no_palette_frames"] += 1
                    fj["note"] = "no palette in BAC: rendered with fallback grey ramp"
                r = render(bac, a, st, f, stats)
                if r is not None:
                    full = Image.frombytes("RGBA", (r["w"], r["h"]), r["rgba"])
                    previews.append((a.index, fi, full, r["ox"], r["oy"]))
                    t = trim(r)
                    if t is None:
                        stats["empty_frames"] += 1
                    else:
                        img, ox, oy, idx = t
                        key = (img.size, img.tobytes(), ox, oy)
                        if key not in image_key:
                            image_key[key] = len(images)
                            images.append({"img": img, "ox": ox, "oy": oy, "idx": idx, "pal": r["pal"]})
                        fj["image"] = image_key[key]
            frames_json.append(fj)
        aj = {"index": a.index, "format": a.fmt, "format_name": FMT_NAMES.get(a.fmt, "?"),
              "loop_frame": a.loop_frame, "loop_anim": a.loop_anim, "next_anim": a.next_anim,
              "frames": frames_json}
        if a.vram is not None:
            aj["vram_units_1d"] = a.vram
        anims_json.append(aj)

    # palettes: PAL4 images (one 16-colour PVR bank per palette) and direct-colour ones
    pals, pal_id = assign_palettes(images)
    anim_pals = [None if p is None else pal_id(p) for p in anim_row0]
    # atlas: PAL4 images and direct images go on separate pages
    pages = []
    has_direct = any(im["pal"] is None for im in images)
    for cls in (True, False):
        sel = [(i, im["img"].width + 2 * GUTTER, im["img"].height + 2 * GUTTER)
               for i, im in enumerate(images) if (im["pal"] is not None) == cls]
        npg = 4 - len(pages) - (1 if cls and has_direct else 0)
        if any(k in ident for k in SINGLE_PAGE):
            npg = 1
        pages += [(W, H, pos, cls) for W, H, pos in (pack_pages(sel, npg) if sel else [])]
    if len(pages) > 4:
        raise BacError("%d atlas pages (loaders take 4)" % len(pages))
    outputs = []
    page_list = []
    for pn, (W, H, pos, pal4) in enumerate(pages):
        atlas = Image.new("RGBA", (W, H), (0, 0, 0, 0))
        placed = []
        for idx, (x, y) in pos.items():
            im = images[idx]
            atlas.paste(im["img"], (x + GUTTER, y + GUTTER))
            im.update(page=pn, x=x + GUTTER, y=y + GUTTER)
            placed.append((x + GUTTER, y + GUTTER, im))
        png = bdir / ("atlas%d.png" % pn)
        atlas.save(png, optimize=True)
        raw = bdir / ("atlas%d.argb%s" % (pn, opts["format"]))
        raw.write_bytes(encode_texture(atlas, opts["format"], opts["twiddle"]))
        outputs += [png, raw]
        pg = {"w": W, "h": H, "png": png.name, "raw": raw.name,
              "pixel_format": "ARGB" + opts["format"], "twiddled": opts["twiddle"],
              "pvr_format": "PAL4" if pal4 else "ARGB1555"}
        if pal4:
            data = encode_pal4(W, H, placed)
            # exactness: PAL4 texel + palette == the ARGB1555 atlas pixel, every texel
            px = atlas.tobytes()
            for x0, y0, im in placed:
                p = pals[im["pal"]]
                for y in range(im["img"].height):
                    for x in range(im["img"].width):
                        q = ((y0 + y) * W + x0 + x) * 4
                        want = c1555(px[q:q + 4]) if px[q + 3] >= 128 else 0
                        if p[decode_pal4(data, W, H, x0 + x, y0 + y)] != want:
                            raise BacError("PAL4 mismatch page %d at %d,%d" % (pn, x0 + x, y0 + y))
            pt4 = bdir / ("atlas%d.pt4" % pn)
            pt4.write_bytes(data)
            outputs.append(pt4)
            pg["pt4"] = pt4.name
        page_list.append(pg)

    img_json = [{"page": im["page"], "x": im["x"], "y": im["y"], "w": im["img"].width,
                 "h": im["img"].height, "origin_x": im["ox"], "origin_y": im["oy"],
                 "palette": im["pal"], "row": im["row"]} for im in images]
    doc = {"source_sha1": hashlib.sha1(data).hexdigest(), "pages": page_list, "images": img_json,
           "palettes": [list(p) for p in pals], "anim_palettes": anim_pals,
           "animations": anims_json, "info": bac.info, "stats": stats, "notes": bac.notes,
           "compression": {"%s_0x%02x" % k: v for k, v in sorted(bac.compression_kinds().items())},
           "obj_mapping_shift": opts["mapping_shift"],
           "timebase": "duration in 60 Hz DS frames (EndFrame wait << 4 at speed 0x10/frame)"}
    js = bdir / "frames.json"
    js.write_text(json.dumps(doc, indent=1))
    binf = bdir / "frames.bin"
    binf.write_bytes(pack_binary(page_list, img_json, anims_json))
    outputs += [js, binf]
    palf = bdir / "palettes.bin"
    palf.write_bytes(pack_palettes(page_list, images, anim_pals, pals))
    outputs.append(palf)

    if opts["preview"] and previews:
        pdir.mkdir(parents=True, exist_ok=True)
        for ai, fi, full, ox, oy in previews:
            full.save(pdir / ("a%03d_f%03d.png" % (ai, fi)))
        sheet = make_sheet(previews)
        sheet.save(pdir / "sheet.png")
    return {"id": ident, "outputs": [p.relative_to(out).as_posix() for p in outputs],
            "sizes": {p.relative_to(out).as_posix(): p.stat().st_size for p in outputs},
            "stats": stats, "animations": len(bac.animations), "images": len(images),
            "pages": [(p["w"], p["h"], p["pvr_format"]) for p in page_list], "palettes": len(pals)}


def pack_binary(pages, images, anims):
    """'DSPR' v1, little-endian (SH-4 is little-endian). See SPRITES.md."""
    nframes = sum(len(a["frames"]) for a in anims)
    hdr = struct.pack("<4sHHHHHH", b"DSPR", 1, len(pages), len(images), len(anims), nframes, 0)
    body = b"".join(struct.pack("<HH", p["w"], p["h"]) for p in pages)
    body += b"".join(struct.pack("<BBHHHHhhH", im["page"], 0, im["x"], im["y"], im["w"], im["h"],
                                 im["origin_x"], im["origin_y"], 0) for im in images)
    first = 0
    for a in anims:
        loop = a["loop_frame"] if a["loop_frame"] is not None else -1
        nxt = a["next_anim"] if a["next_anim"] is not None else (a["loop_anim"] if a["loop_anim"] is not None else -1)
        body += struct.pack("<HHhhBBH", first, len(a["frames"]), loop, nxt, a["format"], 0, 0)
        first += len(a["frames"])
    for a in anims:
        for f in a["frames"]:
            body += struct.pack("<HH", f["image"] & 0xFFFF, f["duration"])
    return hdr + body


def make_sheet(previews):
    rows = {}
    for ai, fi, full, ox, oy in previews:
        rows.setdefault(ai, []).append((full, ox, oy))
    pad = 4
    row_imgs = []
    for ai in sorted(rows):
        fr = rows[ai][:32]
        up = max(oy for _, _, oy in fr)
        down = max(im.height - oy for im, _, oy in fr)
        w = sum(im.width + pad for im, _, _ in fr) + pad
        row_imgs.append((ai, fr, up, down, w))
    W = min(4096, max(r[4] for r in row_imgs) + 40)
    H = min(8192, sum(r[2] + r[3] + pad for r in row_imgs) + pad)
    sheet = Image.new("RGBA", (W, H), (48, 48, 96, 255))
    dr = ImageDraw.Draw(sheet)
    y = pad
    for ai, fr, up, down, w in row_imgs:
        dr.text((1, y), "%d" % ai, fill=(255, 255, 0, 255))
        x = 30
        for im, ox, oy in fr:
            sheet.alpha_composite(im, (x, y + up - oy)) if x + im.width <= W and y + up - oy + im.height <= H else None
            cx, cy = x + ox, y + up
            dr.line([(cx - 2, cy), (cx + 2, cy)], fill=(255, 0, 0, 255))
            dr.line([(cx, cy - 2), (cx, cy + 2)], fill=(255, 0, 0, 255))
            x += im.width + pad
        y += up + down + pad
        if y > H:
            break
    return sheet


def _worker(job):
    try:
        return ("ok", convert_one(job))
    except (BacError, ValueError, IndexError, struct.error) as e:
        return ("fail", {"id": job[0], "error": "%s: %s" % (type(e).__name__, e),
                         "trace": traceback.format_exc(limit=3)})


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--extract", default="extract/eur")
    ap.add_argument("--out", default="build/dreamcast/sprites")
    ap.add_argument("--only", nargs="*", help="convert only sources whose path contains one of these")
    ap.add_argument("--no-preview", action="store_true")
    ap.add_argument("--format", choices=["1555", "4444"], default="1555")
    ap.add_argument("--twiddle", action="store_true")
    ap.add_argument("--mapping-shift", type=int, choices=[0, 1, 2, 3], default=1,
                    help="OBJ 1D mapping boundary assumed for explicit tile numbers "
                         "(1 = 64K, used by stages; only ac_gmk_needle depends on it)")
    ap.add_argument("-j", type=int, default=os.cpu_count() or 1)
    args = ap.parse_args(argv)
    out = Path(args.out)
    out.mkdir(parents=True, exist_ok=True)
    opts = {"format": args.format, "twiddle": args.twiddle, "preview": not args.no_preview,
            "mapping_shift": args.mapping_shift}

    sources = []
    archive_errors = []
    by_sha = {}
    for src, ident, data in find_sources(Path(args.extract) / "files"):
        if ident is None:
            archive_errors.append({"source": src, "error": str(data)})
            continue
        if args.only and not any(s in src for s in args.only):
            continue
        sha = hashlib.sha1(data).hexdigest()
        sources.append({"source": src, "sha1": sha, "bytes": len(data), "id": ident})
        by_sha.setdefault(sha, (ident, data))

    jobs = [(ident, data, str(out), opts) for sha, (ident, data) in by_sha.items()]
    results = {}
    with Pool(max(1, args.j)) as pool:
        for status, res in pool.imap_unordered(_worker, jobs, chunksize=1):
            results[res["id"]] = (status, res)

    manifest = []
    ok = data_only = fail = 0
    reasons = {}
    for s in sources:
        canon = by_sha[s["sha1"]][0]
        status, res = results[canon]
        entry = dict(s)
        entry["converted_as"] = canon
        entry["status"] = status
        if status == "ok":
            entry["outputs"] = res["outputs"]
            entry["output_sizes"] = res["sizes"]
        else:
            entry["error"] = res["error"]
        manifest.append(entry)
        # byte-identical duplicates (e.g. act/dm_demoplay_eng.bac == act/dm_demoplay.bac) are converted
        # once; give the duplicate id its own copy so build_disc.sh / loaders can name either id
        if status == "ok" and s["id"] != canon and (out / "bac" / canon).is_dir():
            shutil.copytree(out / "bac" / canon, out / "bac" / s["id"], dirs_exist_ok=True)
    for canon, (status, res) in results.items():
        if status == "ok":
            if res["images"] == 0:
                data_only += 1
            else:
                ok += 1
        else:
            fail += 1
            r = res["error"].split(" @")[0]
            reasons[r] = reasons.get(r, 0) + 1
    tot = {}
    for canon, (status, res) in results.items():
        if status == "ok":
            for k, v in res["stats"].items():
                tot[k] = tot.get(k, 0) + v
    report = {
        "sources_total": len(sources), "unique_bacs": len(by_sha),
        "converted_with_images": ok, "converted_data_only": data_only, "failed": fail,
        "failure_reasons": reasons, "archive_errors": archive_errors,
        "frame_stats": tot,
        "output_bytes": sum(sum(res["sizes"].values()) for st, res in results.values() if st == "ok"),
        "failures": [res for st, res in results.values() if st != "ok"],
        "data_only_ids": sorted(c for c, (st, res) in results.items() if st == "ok" and res["images"] == 0),
        "no_palette": sorted(c for c, (st, res) in results.items()
                             if st == "ok" and res["stats"].get("no_palette_frames")),
    }
    (out / "manifest.json").write_text(json.dumps(manifest, indent=1))
    (out / "report.json").write_text(json.dumps(report, indent=1))
    print("sources %d, unique %d: converted %d (+%d data-only, no graphics), failed %d"
          % (len(sources), len(by_sha), ok, data_only, fail))
    for r, n in sorted(reasons.items(), key=lambda t: -t[1]):
        print("  fail x%d: %s" % (n, r))
    print("frame stats:", tot)
    print("output bytes: %d" % report["output_bytes"])
    return 0 if fail == 0 else 1


if __name__ == "__main__":
    sys.exit(main())

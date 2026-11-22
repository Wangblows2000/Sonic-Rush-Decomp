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
import struct
import sys
import traceback
from multiprocessing import Pool
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from bac_format import Bac, BacError, RenderState, render, FMT_NAMES  # noqa: E402
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


def shelf_pack(sizes, W, H):
    """sizes: list of (idx, w, h) sorted by h desc. Returns {idx: (x, y)} or None."""
    pos = {}
    x = y = shelf_h = 0
    for idx, w, h in sizes:
        if w > W or h > H:
            return None
        if x + w > W:
            y += shelf_h
            x = shelf_h = 0
        if y + h > H:
            return None
        pos[idx] = (x, y)
        x += w
        shelf_h = max(shelf_h, h)
    return pos


def pack_pages(items):
    """items: list of (idx, w, h) incl. gutter. Returns list of pages: (W, H, {idx: (x, y)})."""
    pending = sorted(items, key=lambda t: (-t[2], -t[1], t[0]))
    pages = []
    while pending:
        area = sum(w * h for _, w, h in pending)
        cands = sorted(((w, h) for w in (8 << i for i in range(8)) for h in (8 << i for i in range(8))
                        if w <= MAX_TEX and h <= MAX_TEX and w * h >= area and h <= w * 2 and w <= h * 2),
                       key=lambda s: (s[0] * s[1], abs(s[0] - s[1])))
        placed = None
        for W, H in cands:
            pos = shelf_pack(pending, W, H)
            if pos is not None:
                placed = (W, H, pos)
                break
        if placed is None:
            # fill one full page greedily, continue with the rest
            pos = {}
            x = y = shelf_h = 0
            rest = []
            for idx, w, h in pending:
                if w > MAX_TEX or h > MAX_TEX:
                    raise BacError("frame %dx%d larger than %d" % (w, h, MAX_TEX))
                if x + w > MAX_TEX:
                    y += shelf_h
                    x = shelf_h = 0
                if y + h > MAX_TEX:
                    rest.append((idx, w, h))
                    continue
                pos[idx] = (x, y)
                x += w
                shelf_h = max(shelf_h, h)
            placed = (MAX_TEX, MAX_TEX, pos)
            pending = rest
        else:
            pending = []
        pages.append(placed)
    return pages


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
    """Crop a rendered frame to its opaque pixels. Returns (img, ox, oy) or None if empty."""
    img = Image.frombytes("RGBA", (r["w"], r["h"]), r["rgba"])
    bbox = img.getchannel("A").getbbox()
    if bbox is None:
        return None
    return img.crop(bbox), r["ox"] - bbox[0], r["oy"] - bbox[1]


def convert_one(job):
    ident, data, out_root, opts = job
    out = Path(out_root)
    bdir = out / "bac" / ident
    pdir = out / "preview" / ident
    bdir.mkdir(parents=True, exist_ok=True)
    bac = Bac(data)
    stats = {"frames": 0, "empty_frames": 0, "no_tile_frames": 0, "no_palette_frames": 0}
    images = []          # unique trimmed images
    image_key = {}
    anims_json = []
    previews = []        # (anim, frame, PIL img full, ox, oy)
    for a in bac.animations:
        st = RenderState(bac, a, opts["mapping_shift"])
        frames_json = []
        for fi, f in enumerate(a.frames):
            st.apply(f)
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
                        img, ox, oy = t
                        key = (img.size, img.tobytes(), ox, oy)
                        if key not in image_key:
                            image_key[key] = len(images)
                            images.append({"img": img, "ox": ox, "oy": oy})
                        fj["image"] = image_key[key]
            frames_json.append(fj)
        aj = {"index": a.index, "format": a.fmt, "format_name": FMT_NAMES.get(a.fmt, "?"),
              "loop_frame": a.loop_frame, "loop_anim": a.loop_anim, "next_anim": a.next_anim,
              "frames": frames_json}
        if a.vram is not None:
            aj["vram_units_1d"] = a.vram
        anims_json.append(aj)

    # atlas
    pages = pack_pages([(i, im["img"].width + 2 * GUTTER, im["img"].height + 2 * GUTTER)
                        for i, im in enumerate(images)]) if images else []
    outputs = []
    page_list = []
    for pn, (W, H, pos) in enumerate(pages):
        atlas = Image.new("RGBA", (W, H), (0, 0, 0, 0))
        for idx, (x, y) in pos.items():
            im = images[idx]
            atlas.paste(im["img"], (x + GUTTER, y + GUTTER))
            im.update(page=pn, x=x + GUTTER, y=y + GUTTER)
        png = bdir / ("atlas%d.png" % pn)
        atlas.save(png, optimize=True)
        raw = bdir / ("atlas%d.argb%s" % (pn, opts["format"]))
        raw.write_bytes(encode_texture(atlas, opts["format"], opts["twiddle"]))
        outputs += [png, raw]
        page_list.append({"w": W, "h": H, "png": png.name, "raw": raw.name,
                          "pixel_format": "ARGB" + opts["format"], "twiddled": opts["twiddle"]})

    img_json = [{"page": im["page"], "x": im["x"], "y": im["y"], "w": im["img"].width,
                 "h": im["img"].height, "origin_x": im["ox"], "origin_y": im["oy"]} for im in images]
    doc = {"source_sha1": hashlib.sha1(data).hexdigest(), "pages": page_list, "images": img_json,
           "animations": anims_json, "info": bac.info, "stats": stats, "notes": bac.notes,
           "compression": {"%s_0x%02x" % k: v for k, v in sorted(bac.compression_kinds().items())},
           "obj_mapping_shift": opts["mapping_shift"],
           "timebase": "duration in 60 Hz DS frames (EndFrame wait << 4 at speed 0x10/frame)"}
    js = bdir / "frames.json"
    js.write_text(json.dumps(doc, indent=1))
    binf = bdir / "frames.bin"
    binf.write_bytes(pack_binary(page_list, img_json, anims_json))
    outputs += [js, binf]

    if opts["preview"] and previews:
        pdir.mkdir(parents=True, exist_ok=True)
        for ai, fi, full, ox, oy in previews:
            full.save(pdir / ("a%03d_f%03d.png" % (ai, fi)))
        sheet = make_sheet(previews)
        sheet.save(pdir / "sheet.png")
    return {"id": ident, "outputs": [str(p.relative_to(out)) for p in outputs],
            "sizes": {str(p.relative_to(out)): p.stat().st_size for p in outputs},
            "stats": stats, "animations": len(bac.animations), "images": len(images),
            "pages": [(p["w"], p["h"]) for p in page_list]}


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

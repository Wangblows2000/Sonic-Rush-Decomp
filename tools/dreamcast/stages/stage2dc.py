#!/usr/bin/env python3
"""Sonic Rush stage converter for the native Dreamcast port.

Reads the owner's extracted ROM (extract/eur) and writes, per stage slot:

  build/dreamcast/stages/<id>/level.bin     RLVL package (level_format.h)
  build/dreamcast/stages/<id>/blocktex.bin  pre-rendered block texture pages
  build/dreamcast/stages/<id>/bg.bin        zone background (if the stage has one)
  build/dreamcast/stages/<id>/manifest.json
  build/dreamcast/stages/preview/<id>_*.png full-level previews
  build/dreamcast/stages/manifest.json      batch summary

Usage:
  stage2dc.py [--rom extract/eur] [--out build/dreamcast/stages] [--all | --stage z11 ...]
              [--no-preview] [--overlay z11:X,Y,W,H] [--testvec N]

Formats: docs/dreamcast/formats/STAGES.md.  No ROM data is embedded here.
"""
import argparse
import hashlib
import json
import os
import re
import struct
import sys
import time

import numpy as np
from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import ndsfmt  # noqa: E402
import dscoll  # noqa: E402

REPO = os.path.abspath(os.path.join(HERE, '..', '..', '..'))

ZONE_NAMES = ['Leaf Storm', 'Water Palace', 'Mirage Road', 'Night Carnival',
              'Huge Crisis', 'Altitude Limit', 'Dead Line', 'F (final)', 'Extra / arena']


def stage_slots():
    """27 slots: stage = zone*3 + act (zone, act 0-based)."""
    out = []
    for zone in range(9):
        for act in range(3):
            stage = zone * 3 + act
            if zone <= 6:
                sid = 'z%d%d' % (zone + 1, act + 1)
            elif zone == 7:
                sid = ['z81', 'z82', 'zf3'][act]
            else:
                sid = ['z91', 'z92', None][act]
            out.append((stage, zone, act, sid))
    return out


# Zone background scroll hints (from ZoneBg_Zone<N>_Update / _UpdateParallax).
BG_SCROLL = {
    0: {'x': 'camX/14', 'y': 'camY*48/(mapH-192)', 'linescroll': False,
        'code': 'ZoneBg_Zone1_Update'},
    1: {'x': 'camX/16 (>>4)', 'y': 'see code', 'linescroll': True,
        'code': 'ZoneBg_Zone2_UpdateParallax/BuildLineScroll'},
    2: {'x': 'camX/12', 'y': '((camY/12)+0x110)%0x118+0xE8', 'linescroll': True,
        'code': 'ZoneBg_Zone3_UpdateParallax/BuildLineScroll'},
    3: {'x': 'camX/18', 'y': 'camY*48/(mapH-192)', 'linescroll': False,
        'code': 'ZoneBg_Zone4_Update'},
    4: {'x': 'camX/14', 'y': '(((camY/14)+0x110)&0xFF)+0x100', 'linescroll': True,
        'code': 'ZoneBg_Zone5_UpdateParallax/BuildLineScroll'},
    5: {'x': 'x += (camX - prevCamX)/60 (auto-drift)', 'y': 'see code', 'linescroll': False,
        'code': 'ZoneBg_Zone6_Update'},
    6: {'x': 'camX/14', 'y': '(((camY/14)+0x110)&0xFF)+0x100', 'linescroll': True,
        'code': 'ZoneBg_Zone7_UpdateParallax/BuildLineScroll'},
}
# z81/z82/z91/z92 reuse zone 1/3/4/5 background code (gZoneBg* tables)
BG_ZONE_FOR_STAGE = {21: 0, 22: 2, 24: 3, 25: 4}


# --------------------------------------------------------------------------
# helpers

def argb1555(c, opaque=True):
    r = c & 0x1F
    g = (c >> 5) & 0x1F
    b = (c >> 10) & 0x1F
    return (0x8000 if opaque else 0) | (r << 10) | (g << 5) | b


def pal_rgb(pal_bytes):
    out = []
    for i in range(len(pal_bytes) // 2):
        out += ndsfmt.bgr555_to_rgb(struct.unpack_from('<H', pal_bytes, i * 2)[0])
    out += [0] * (768 - len(out))
    return out


_twid_cache = {}


def twiddle_index(w, h):
    """Linear (y*w+x) -> twiddled offset map for a PVR texture w x h (pow2)."""
    key = (w, h)
    if key in _twid_cache:
        return _twid_cache[key]
    mn = min(w, h)

    def spread(v):
        r = np.zeros_like(v)
        for bit in range(16):
            r |= ((v >> bit) & 1) << (2 * bit)
        return r
    ys, xs = np.mgrid[0:h, 0:w]
    xs = xs.astype(np.int64)
    ys = ys.astype(np.int64)
    m = mn - 1
    idx = spread(ys & m) | (spread(xs & m) << 1)
    idx += (xs // mn + ys // mn) * mn * mn
    _twid_cache[key] = idx
    return idx


def twiddle(img):
    """img: uint8 array (h,w) -> bytes in PVR twiddled order."""
    h, w = img.shape
    idx = twiddle_index(w, h)
    out = np.zeros(w * h, dtype=np.uint8)
    out[idx.ravel()] = img.ravel()
    return out.tobytes()


def untwiddle(data, w, h):
    idx = twiddle_index(w, h)
    flat = np.frombuffer(data, dtype=np.uint8)
    return flat[idx.ravel()].reshape(h, w)


def align(buf, a=32):
    pad = (-len(buf)) % a
    buf += b'\0' * pad


def sha1(b):
    return hashlib.sha1(b).hexdigest()


# --------------------------------------------------------------------------
# symbol tables (names only; table contents read from the ROM overlay)

def load_symbols():
    syms = {}
    for f in ('config/eur/arm9/symbols.txt', 'config/eur/arm9/overlays/ov000/symbols.txt'):
        p = os.path.join(REPO, f)
        if not os.path.exists(p):
            continue
        for line in open(p):
            m = re.match(r'(\S+) kind:function\S* addr:0x([0-9a-fA-F]+)', line)
            if m:
                syms[int(m.group(2), 16)] = m.group(1)
    return syms


def load_create_tables(rom):
    """gMapObjectCreateTable / gDecorationCreateTable -> {type: name}."""
    ov = open(os.path.join(rom, 'arm9_overlays', 'ov000.bin'), 'rb').read()
    base = 0x022C5C80
    syms = load_symbols()

    def tab(addr, n=0xF2):
        o = addr - base
        res = {}
        for i in range(n):
            a = struct.unpack_from('<I', ov, o + i * 4)[0]
            if a:
                res[i] = syms.get(a, syms.get(a & ~1, '0x%08x' % a))
        return res
    return tab(0x0236498C), tab(0x023645C4)


# --------------------------------------------------------------------------
# DS stage data

MAP_EXT = ['at', 'bk', 'ca', 'ch', 'df', 'di', 'pl', 'a', 'b']


def load_map(rom, sid):
    files, names = ndsfmt.read_narc(open(os.path.join(rom, 'files', 'narc', sid + '_map.narc'), 'rb').read())
    if len(files) != 9:
        raise ValueError('%s_map: expected 9 files, got %d' % (sid, len(files)))
    d = {}
    for i, ext in enumerate(MAP_EXT):
        d[ext] = ndsfmt.decompress(files[i])
    d['names'] = names
    return d


def load_eve(rom, sid):
    files, names = ndsfmt.read_narc(open(os.path.join(rom, 'files', 'narc', sid + '_eve.narc'), 'rb').read())
    d = {'names': names}
    # Stage_LoadEventResources: idx1 -> map objects, idx2 -> rings, idx0 -> decorations, idx3 -> bbg
    d['dc'] = ndsfmt.decompress(files[0])
    d['ev'] = ndsfmt.decompress(files[1])
    d['rg'] = ndsfmt.decompress(files[2])
    d['bbg'] = files[3] if len(files) > 3 else None
    d['bbg_name'] = names[3] if len(files) > 3 else None
    d['extra_bbg'] = [(names[i], files[i]) for i in range(4, len(files))]
    return d


def parse_regions(data, entry_size):
    """Event-file layout: u16 cols, u16 rows, u32 offset[cols*rows], then per
    region: u16 count + count entries.  Returns cols, rows, list of
    (region_index, entry_bytes)."""
    cols, rows = struct.unpack_from('<HH', data, 0)
    out = []
    for r in range(cols * rows):
        off = struct.unpack_from('<I', data, 4 + r * 4)[0]
        n = struct.unpack_from('<H', data, off)[0]
        for i in range(n):
            out.append((r, data[off + 2 + i * entry_size: off + 2 + (i + 1) * entry_size]))
    return cols, rows, out


def decode_chars8(ch):
    n = len(ch) // 64
    return np.frombuffer(ch[:n * 64], dtype=np.uint8).reshape(n, 8, 8)


def build_block_images(chars, bk):
    """bk words -> (nblocks,64,64) uint8 index images."""
    words = np.frombuffer(bk, dtype='<u2').reshape(-1, 8, 8)
    nb = words.shape[0]
    # pre-build the 4 flip variants of every char
    var = np.stack([chars, chars[:, :, ::-1], chars[:, ::-1, :], chars[:, ::-1, ::-1]])  # (4,n,8,8)
    cid = (words & 0x3FF).astype(np.int64)
    flip = ((words >> 10) & 3).astype(np.int64)  # bit0 h, bit1 v
    tiles = var[flip, cid]  # (nb,8,8,8,8) = block, ty, tx, py, px
    return tiles.transpose(0, 1, 3, 2, 4).reshape(nb, 64, 64)


def layer_words(mp):
    w, h = struct.unpack_from('<HH', mp, 0)
    return w, h, np.frombuffer(mp[4:4 + w * h * 2], dtype='<u2').reshape(h, w)


def render_layer(blockimg, lw):
    h, w = lw.shape
    img = blockimg[lw.astype(np.int64)]  # (h,w,64,64)
    return img.transpose(0, 2, 1, 3).reshape(h * 64, w * 64)


def decode_bbg(bbg):
    """BBG: 'BBG\\0', u32 size, u32 chr_off, u32 scr_off, u32 pal_off, u32 flags,
    u16 w_tiles, u16 h_tiles, u32 unk.  Sub-resources carry the usual 4-byte
    compression header.  Returns dict with index image (h*8,w*8) uint8."""
    if bbg[:4] != b'BBG\0':
        raise ValueError('not a BBG')
    size, chr_off, scr_off, pal_off, flags, wt, ht, unk = struct.unpack_from('<IIIIIHHI', bbg, 4)
    ch = ndsfmt.decompress(bbg[chr_off:])
    scr = ndsfmt.decompress(bbg[scr_off:])
    pal = ndsfmt.decompress(bbg[pal_off:])
    typ = flags & 0xF
    bpp = 4 if typ == 1 else 8
    entries = np.frombuffer(scr[:wt * ht * 2], dtype='<u2').reshape(ht, wt)
    if bpp == 4:
        raw = np.frombuffer(ch + b'\0' * (32 * 1024 * 2), dtype=np.uint8)
        n = (len(ch) + 31) // 32 + 1024
        raw = raw[:n * 32]
        lo = raw & 0xF
        hi = raw >> 4
        chars = np.stack([lo, hi], axis=1).reshape(n, 8, 8)
    else:
        raw = np.frombuffer(ch + b'\0' * (64 * 1024), dtype=np.uint8)
        n = (len(ch) + 63) // 64 + 1024
        chars = raw[:n * 64].reshape(n, 8, 8)
    var = np.stack([chars, chars[:, :, ::-1], chars[:, ::-1, :], chars[:, ::-1, ::-1]])
    cid = (entries & 0x3FF).astype(np.int64)
    flip = ((entries >> 10) & 3).astype(np.int64)
    tiles = var[flip, cid].astype(np.uint8)  # (ht,wt,8,8)
    if bpp == 4:
        palbank = (entries >> 12).astype(np.uint8)[:, :, None, None]
        tiles = np.where(tiles != 0, tiles + palbank * 16, 0).astype(np.uint8)
    img = tiles.transpose(0, 2, 1, 3).reshape(ht * 8, wt * 8)
    return {'type': typ, 'bpp': bpp, 'w_tiles': wt, 'h_tiles': ht, 'unk1c': unk,
            'img': img, 'pal': pal, 'flags': flags}


def bbg_palette(bbg):
    size, chr_off, scr_off, pal_off, flags = struct.unpack_from('<IIIII', bbg, 4)
    return ndsfmt.decompress(bbg[pal_off:])


# --------------------------------------------------------------------------
# package writers

def write_level(path, stage, zone, act, m, objs, rings, decor, regions, flags):
    bk = m['bk']
    nb = len(bk) // 128
    w, h, la = layer_words(m['a'])
    w2, h2, lb = layer_words(m['b'])
    assert (w, h) == (w2, h2)
    nchars = 1024
    # palette
    pl = m['pl']
    pal = [argb1555(struct.unpack_from('<H', pl, i * 2)[0], i != 0) for i in range(256)]
    secs = []
    secs.append(struct.pack('<256H', *pal))
    # char texture page 256x256: char c at ((c%32)*8, (c/32)*8)
    chars = decode_chars8(m['ch'])
    page = chars[:1024].reshape(32, 32, 8, 8).transpose(0, 2, 1, 3).reshape(256, 256)
    secs.append(twiddle(page))
    secs.append(bk)
    secs.append(la.astype('<u2').tobytes())
    secs.append(lb.astype('<u2').tobytes())
    df = np.frombuffer(m['df'], dtype=np.uint8).astype(np.int16)
    sb = np.where(df >= 128, df - 256, df)
    hn = sb >> 4                       # signed high nibble
    wn = (df & 0xF)
    wn = np.where(wn & 8, wn - 16, wn)  # signed low nibble
    hn = np.where(hn == -8, 8, hn)
    wn = np.where(wn == -8, 8, wn)
    secs.append(hn.astype(np.int8).tobytes())
    secs.append(wn.astype(np.int8).tobytes())
    secs.append(m['di'][:nchars])
    secs.append(m['at'][:nchars])
    ca = m['ca']
    cw, chh = struct.unpack_from('<HH', ca, 0)
    assert (cw, chh) == (w, h), 'split map size mismatch'
    secs.append(ca[4:])
    cols, rows = regions
    nreg = cols * rows

    def region_table(items):
        counts = [0] * nreg
        for it in items:
            counts[it[0]] += 1
        starts = [0]
        for c in counts:
            starts.append(starts[-1] + c)
        return struct.pack('<%dI' % (nreg + 1), *starts)
    secs.append(b''.join(struct.pack('<HHHHbbBB', *o[1:]) for o in objs))
    secs.append(region_table(objs))
    secs.append(b''.join(struct.pack('<HH', *r[1:]) for r in rings))
    secs.append(region_table(rings))
    secs.append(b''.join(struct.pack('<HHHH', d[1], d[2], d[3], 0) for d in decor))
    secs.append(region_table(decor))
    assert len(secs) == 16
    hdr_fmt = '<IHHBBBBHHHHHHIII' + 'II' * 16
    hsize = struct.calcsize(hdr_fmt)
    body = bytearray(b'\0' * hsize)
    align(body)
    table = []
    for s in secs:
        align(body)
        table.append((len(body), len(s)))
        body += s
    align(body)
    hdr = struct.pack(hdr_fmt, 0x4C564C52, 1, hsize, stage, zone, act, flags,
                      w, h, nb, nchars, cols, rows, len(objs), len(rings), len(decor),
                      *[v for t in table for v in t])
    body[:hsize] = hdr
    open(path, 'wb').write(body)
    return len(body), (w, h, nb), page


def write_blocktex(path, blockimg):
    nb = blockimg.shape[0]
    pages = (nb + 63) // 64
    out = bytearray(struct.pack('<IHHHHHHI3I', 0x58544252, 1, 1, 512, 8, pages, nb, 32, 0, 0, 0))
    assert len(out) == 32
    for p in range(pages):
        page = np.zeros((512, 512), dtype=np.uint8)
        for s in range(64):
            b = p * 64 + s
            if b >= nb:
                break
            page[(s // 8) * 64:(s // 8) * 64 + 64, (s % 8) * 64:(s % 8) * 64 + 64] = blockimg[b]
        out += twiddle(page)
    open(path, 'wb').write(out)
    return len(out), pages


def write_bg(path, bg, extra_pals):
    img = bg['img']
    h, w = img.shape
    th = 1 << (h - 1).bit_length()
    tw = 1 << (w - 1).bit_length()
    tex = np.zeros((th, tw), dtype=np.uint8)
    tex[:h, :w] = img
    pals = [bg['pal']] + extra_pals
    palbin = bytearray()
    for p in pals:
        for i in range(256):
            c = struct.unpack_from('<H', p, i * 2)[0] if i * 2 + 2 <= len(p) else 0
            opaque = (i == 0) or (i % 16 != 0) if bg['bpp'] == 4 else (i != 0)
            palbin += struct.pack('<H', argb1555(c, opaque))
    hdr_size = 36
    pal_off = 64
    tex_off = pal_off + len(palbin)
    tex_off += (-tex_off) % 32
    out = bytearray(struct.pack('<IHHHHHHHHII2I', 0x44474252, 1, bg['type'], w, h, tw, th,
                                len(pals), 0, pal_off, tex_off, 0, 0))
    assert len(out) == hdr_size
    out += b'\0' * (pal_off - len(out))
    out += palbin
    out += b'\0' * (tex_off - len(out))
    out += twiddle(tex)
    open(path, 'wb').write(out)
    return len(out)


# --------------------------------------------------------------------------
# previews / overlays

def save_indexed(path, idx, palette):
    im = Image.fromarray(idx, 'P')
    im.putpalette(palette)
    im.save(path, optimize=False, compress_level=6)


def fg_composite(m, blockimg):
    _, _, la = layer_words(m['a'])
    _, _, lb = layer_words(m['b'])
    A = render_layer(blockimg, la)
    B = render_layer(blockimg, lb)
    return A, B, np.where(A != 0, A, B)


def collision_masks(m):
    """Per-pixel solidity of both layers (vertical masks, after flips), as the
    DS GetTileHeight semantics define: h==8 full, h>0 rows h..7, h<0 rows 0..-h-1.
    Also returns a 'top-solid only' (attr bit0) mask."""
    df = np.frombuffer(m['df'], dtype=np.uint8).astype(np.int16)
    sb = np.where(df >= 128, df - 256, df)
    hn = (sb >> 4).reshape(1024, 8)
    hn = np.where(hn == -8, 8, hn)
    rows = np.arange(8)[None, :, None]
    hcol = hn[:, None, :]
    solid = (hcol == 8) | ((hcol > 0) & (rows >= hcol)) | ((hcol < 0) & (rows < -hcol))  # (1024,8,8) [y][x]
    solid = solid.astype(np.uint8)
    # char-level masks with flips applied *as the DS does*: hflip mirrors the
    # column lookup; vflip turns h>0 into h-8 (and vice versa) = row mirror.
    var = np.stack([solid, solid[:, :, ::-1], solid[:, ::-1, :], solid[:, ::-1, ::-1]])
    words = np.frombuffer(m['bk'], dtype='<u2').reshape(-1, 8, 8)
    cid = (words & 0x3FF).astype(np.int64)
    flip = ((words >> 10) & 3).astype(np.int64)
    nb = words.shape[0]
    blk = var[flip, cid].transpose(0, 1, 3, 2, 4).reshape(nb, 64, 64)
    at = np.frombuffer(m['at'][:1024], dtype=np.uint8)
    topm = (at & 1)[cid][:, :, :, None, None] * np.ones((1, 1, 1, 8, 8), dtype=np.uint8)
    topblk = topm.transpose(0, 1, 3, 2, 4).reshape(nb, 64, 64)
    _, _, la = layer_words(m['a'])
    _, _, lb = layer_words(m['b'])
    return (render_layer(blk, la), render_layer(blk, lb),
            render_layer(topblk, la), render_layer(topblk, lb))


def overlay_png(path, m, blockimg, rect, dsmap=None, sample_check=True):
    x0, y0, w, h = rect
    _, _, fg = fg_composite(m, blockimg)
    pal = pal_rgb(m['pl'])
    crop = fg[y0:y0 + h, x0:x0 + w]
    rgb = np.array(pal, dtype=np.uint8).reshape(256, 3)[crop].astype(np.float32)
    ma, mb, ta, tb = [a[y0:y0 + h, x0:x0 + w] for a in collision_masks(m)]
    # layer B (player default) red, layer A blue, top-solid-only chars yellow-tinted
    red = np.array([255, 0, 0], np.float32)
    blue = np.array([0, 80, 255], np.float32)
    yel = np.array([255, 230, 0], np.float32)
    sb_ = (mb == 1)[:, :, None]
    sa_ = (ma == 1)[:, :, None]
    tsb = ((mb == 1) & (tb == 1))[:, :, None]
    rgb = np.where(sb_, rgb * 0.45 + red * 0.55, rgb)
    rgb = np.where(sa_ & ~sb_, rgb * 0.45 + blue * 0.55, rgb)
    rgb = np.where(tsb, rgb * 0.4 + yel * 0.6, rgb)
    # outline the B surface (top edge of solid) in white for readability
    edge = (mb[1:, :] == 1) & (mb[:-1, :] == 0)
    rgb[1:, :][edge] = [255, 255, 255]
    img = Image.fromarray(rgb.clip(0, 255).astype(np.uint8), 'RGB')
    img = img.resize((w * 2, h * 2), Image.NEAREST)
    img.save(path)
    mism = 0
    checked = 0
    if dsmap is not None and sample_check:
        # cross-check the vectorised mask against the transcribed DS query
        rng = np.random.default_rng(1)
        for _ in range(4000):
            x = int(rng.integers(x0, x0 + w))
            y = int(rng.integers(y0, y0 + h))
            for layer, mk in ((0, ma), (1, mb)):
                checked += 1
                if dsmap.solid_pixel(x, y, layer) != bool(mk[y - y0, x - x0]):
                    mism += 1
    return checked, mism


# --------------------------------------------------------------------------

def convert_stage(rom, out, slot, tables, preview=True, testvec=0):
    stage, zone, act, sid = slot
    t0 = time.time()
    m = load_map(rom, sid)
    e = load_eve(rom, sid)
    boss = (act == 2)
    sdir = os.path.join(out, sid)
    os.makedirs(sdir, exist_ok=True)
    pdir = os.path.join(out, 'preview')
    os.makedirs(pdir, exist_ok=True)
    w, h, _ = layer_words(m['a'])

    # events
    cols, rows, ev = parse_regions(e['ev'], 12)
    cols2, rows2, rg = parse_regions(e['rg'], 2)
    cols3, rows3, dc = parse_regions(e['dc'], 4)
    assert (cols, rows) == (cols2, rows2) == (cols3, rows3), 'region grid mismatch'
    assert cols == ((w * 64 - 1) >> 8) + 1, 'region cols != map width'
    objs, rings, decor = [], [], []
    skipped = {'objects_ff': 0, 'rings_ff': 0, 'decor_ff': 0}
    for r, b in ev:
        x, y, typ, fl, p0, p1, p2, p3, rt = struct.unpack('<BBHHbbBBH', b)
        if x == 0xFF:
            skipped['objects_ff'] += 1
            continue
        objs.append((r, (r % cols) * 256 + x, (r // cols) * 256 + y, typ, fl, p0, p1, p2, p3))
    for r, b in rg:
        if b[0] == 0xFF:
            skipped['rings_ff'] += 1
            continue
        rings.append((r, (r % cols) * 256 + b[0], (r // cols) * 256 + b[1]))
    for r, b in dc:
        x, y, typ = struct.unpack('<BBH', b)
        if x == 0xFF:
            skipped['decor_ff'] += 1
            continue
        decor.append((r, (r % cols) * 256 + x, (r // cols) * 256 + y, typ))

    has_bg = e['bbg'] is not None
    flags = (1 if boss else 0) | (2 if has_bg else 0)
    lsize, (w, h, nb), charpage = write_level(os.path.join(sdir, 'level.bin'), stage, zone, act, m,
                                               objs, rings, decor, (cols, rows), flags)
    chars = decode_chars8(m['ch'])
    blockimg = build_block_images(chars, m['bk'])
    bsize, pages = write_blocktex(os.path.join(sdir, 'blocktex.bin'), blockimg)
    bginfo = None
    bg_img = None
    if has_bg:
        bg = decode_bbg(e['bbg'])
        bg_img = bg['img']
        extra = [bbg_palette(b) for _, b in e['extra_bbg']]
        gsize = write_bg(os.path.join(sdir, 'bg.bin'), bg, extra)
        bz = BG_ZONE_FOR_STAGE.get(stage, zone)
        bginfo = {'file': 'bg.bin', 'source': e['bbg_name'], 'bytes': gsize,
                  'image': [bg['img'].shape[1], bg['img'].shape[0]], 'src_bpp': bg['bpp'],
                  'bbg_type': bg['type'], 'alt_palettes': [n for n, _ in e['extra_bbg']],
                  'scroll': BG_SCROLL.get(bz)}
        if preview:
            save_indexed(os.path.join(pdir, sid + '_bg.png'), bg['img'], pal_rgb(bg['pal']))
    elif os.path.exists(os.path.join(sdir, 'bg.bin')):
        os.remove(os.path.join(sdir, 'bg.bin'))

    pv = []
    if preview:
        A, B, F = fg_composite(m, blockimg)
        pal = pal_rgb(m['pl'])
        for nm, img in (('fg', F), ('layer_a', A), ('layer_b', B)):
            p = os.path.join(pdir, '%s_%s.png' % (sid, nm))
            save_indexed(p, img, pal)
            pv.append(os.path.relpath(p, out))
        # quarter-size RGB thumbnail of the composite
        thumb = Image.fromarray(F, 'P')
        thumb.putpalette(pal)
        thumb = thumb.convert('RGB').resize((max(1, F.shape[1] // 4), max(1, F.shape[0] // 4)), Image.BOX)
        p = os.path.join(pdir, sid + '_fg_quarter.png')
        thumb.save(p)
        pv.append(os.path.relpath(p, out))
        cp = os.path.join(pdir, sid + '_chars.png')
        save_indexed(cp, charpage, pal)

    obj_types = {}
    for o in objs:
        obj_types[o[3]] = obj_types.get(o[3], 0) + 1
    dec_types = {}
    for d in decor:
        dec_types[d[3]] = dec_types.get(d[3], 0) + 1
    otab, dtab = tables
    man = {
        'id': sid, 'stage': stage, 'zone': zone, 'act': act, 'zone_name': ZONE_NAMES[zone],
        'boss_arena': boss,
        'size_blocks': [w, h], 'size_px': [w * 64, h * 64], 'num_blocks': nb,
        'region_grid': [cols, rows],
        'files': {
            'level.bin': {'bytes': lsize, 'sha1': sha1(open(os.path.join(sdir, 'level.bin'), 'rb').read())},
            'blocktex.bin': {'bytes': bsize, 'pages': pages},
        },
        'background': bginfo,
        'counts': {'objects': len(objs), 'rings': len(rings), 'decorations': len(decor), **skipped},
        'object_types': {str(k): {'count': v, 'create': otab.get(k)} for k, v in sorted(obj_types.items())},
        'decoration_types': {str(k): {'count': v, 'create': dtab.get(k)} for k, v in sorted(dec_types.items())},
        'unknown_object_types': sorted(k for k in obj_types if k not in otab),
        'unknown_decoration_types': sorted(k for k in dec_types if k not in dtab),
        'sources': {n: sha1(open(os.path.join(rom, 'files', 'narc', sid + n), 'rb').read())
                    for n in ('_map.narc', '_eve.narc')},
        'previews': pv,
    }
    man['verify'] = verify_package(sdir, m, blockimg, bg_img)
    if testvec:
        man['testvec'] = write_testvec(os.path.join(sdir, 'testvec.bin'), m, testvec)
    man['seconds'] = round(time.time() - t0, 2)
    json.dump(man, open(os.path.join(sdir, 'manifest.json'), 'w'), indent=1)
    return man, m, blockimg


def verify_package(sdir, m, blockimg, bg_img):
    """Re-read level.bin / blocktex.bin / bg.bin (as the native loader would),
    rebuild both foreground layers from the block texture pages and the char
    page, and compare with the render made from the DS files.  Returns a dict
    of mismatch counts (all 0 = package graphics are lossless)."""
    lv = open(os.path.join(sdir, 'level.bin'), 'rb').read()
    hdr = struct.unpack_from('<IHHBBBBHHHHHHIII' + 'II' * 16, lv, 0)
    w, h, nb = hdr[7], hdr[8], hdr[9]
    sec = [(hdr[16 + 2 * i], hdr[17 + 2 * i]) for i in range(16)]
    res = {}
    # blocks via blocktex pages
    bt = open(os.path.join(sdir, 'blocktex.bin'), 'rb').read()
    magic, ver, fmt, psz, bpr, npages, nblk, poff = struct.unpack_from('<IHHHHHHI', bt, 0)
    pages = [untwiddle(bt[poff + p * psz * psz: poff + (p + 1) * psz * psz], psz, psz) for p in range(npages)]
    blk2 = np.zeros((nblk, 64, 64), dtype=np.uint8)
    for b in range(nblk):
        s = b % 64
        blk2[b] = pages[b // 64][(s // 8) * 64:(s // 8) * 64 + 64, (s % 8) * 64:(s % 8) * 64 + 64]
    res['blocktex_vs_ds'] = int(np.count_nonzero(blk2 != blockimg))
    # blocks via char page + block words
    cp = untwiddle(lv[sec[1][0]:sec[1][0] + 65536], 256, 256)
    chars = cp.reshape(32, 8, 32, 8).transpose(0, 2, 1, 3).reshape(1024, 8, 8)
    bw = lv[sec[2][0]:sec[2][0] + sec[2][1]]
    blk3 = build_block_images(chars, bw)
    res['charpage_vs_ds'] = int(np.count_nonzero(blk3 != blockimg))
    for i, key in ((3, 'a'), (4, 'b')):
        lw = np.frombuffer(lv[sec[i][0]:sec[i][0] + sec[i][1]], dtype='<u2').reshape(h, w)
        _, _, ref = layer_words(m[key])
        res['layer_%s_vs_ds' % key] = int(np.count_nonzero(lw != ref))
    if bg_img is not None:
        g = open(os.path.join(sdir, 'bg.bin'), 'rb').read()
        _, _, _, gw, gh, tw, th, npal, _, poff, toff = struct.unpack_from('<IHHHHHHHHII', g, 0)
        tex = untwiddle(g[toff:toff + tw * th], tw, th)
        res['bg_vs_ds'] = int(np.count_nonzero(tex[:gh, :gw] != bg_img))
    return res


def write_testvec(path, m, n):
    """Random collision queries evaluated with the DS transcription (dscoll)
    on the raw DS files; test_level.c re-evaluates them on level.bin."""
    ds = dscoll.DSMap(m)
    rng = np.random.default_rng(12345)
    out = bytearray()
    W, H = ds.right, ds.bottom
    # bias half of the samples onto 8x8 tiles whose masks are partial (slopes,
    # thin floors, ceilings), where the flip/sign rules matter
    df = np.frombuffer(m['df'], dtype=np.uint8).reshape(1024, 8)
    partial = ~(np.all(df == 0, axis=1) | np.all(df == 0x88, axis=1))
    words = np.frombuffer(m['bk'], dtype='<u2').reshape(-1, 8, 8)
    bpart = partial[(words & 0x3FF).astype(np.int64)]  # (nb,8,8)
    cand = []
    for layer in ('a', 'b'):
        _, _, lw = layer_words(m[layer])
        t = bpart[lw.astype(np.int64)].transpose(0, 2, 1, 3).reshape(lw.shape[0] * 8, lw.shape[1] * 8)
        ys, xs = np.nonzero(t)
        cand.append(np.stack([xs * 8, ys * 8], axis=1))
    cand = np.concatenate(cand) if cand else np.zeros((0, 2), int)
    for i in range(n):
        if i & 1 and len(cand):
            cx, cy = cand[int(rng.integers(0, len(cand)))]
            x = int(cx + rng.integers(-12, 20))
            y = int(cy + rng.integers(-12, 20))
        else:
            x = int(rng.integers(-24, W + 24))
            y = int(rng.integers(-24, H + 24))
        fl = int(rng.choice([0, 1, 0x80, 0x81, 0x40, 0x41, 0xC0, 0xC1]))
        d = int(rng.integers(0, 4))
        ain = int(rng.integers(0, 256))
        tin = int(rng.integers(0, 256))
        hh, ah, th = ds.tile_height(y, x, fl)
        hw, aw, tw = ds.tile_width(x, y, fl)
        # GetTileHeight/Width leave outputs untouched when they return 0 / early
        ah = ain if ah is None else ah
        th = tin if th is None else th
        aw = ain if aw is None else aw
        tw = tin if tw is None else tw
        pl, apl, tpl = ds.probe_long(x, y, fl, d, ain, tin)
        out += struct.pack('<iiBBBB' + 'bBB' * 3, x, y, fl, d, ain, tin,
                           hh, ah, th, hw, aw, tw, pl, apl, tpl)
    open(path, 'wb').write(out)
    return n


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--rom', default=os.path.join(REPO, 'extract', 'eur'))
    ap.add_argument('--out', default=os.path.join(REPO, 'build', 'dreamcast', 'stages'))
    ap.add_argument('--stage', action='append', default=[], help='stage id (z11, zf3, ...) or slot number')
    ap.add_argument('--all', action='store_true')
    ap.add_argument('--no-preview', action='store_true')
    ap.add_argument('--overlay', action='append', default=[], help='ID:X,Y,W,H collision overlay PNG')
    ap.add_argument('--testvec', type=int, default=0, help='write N collision test vectors per stage')
    args = ap.parse_args()

    slots = stage_slots()
    if args.all or not args.stage:
        todo = slots
    else:
        want = set(args.stage)
        todo = [s for s in slots if s[3] in want or str(s[0]) in want]
    tables = load_create_tables(args.rom)
    os.makedirs(args.out, exist_ok=True)
    summary = {'stages': [], 'failed': [], 'no_data': []}
    cache = {}
    for slot in todo:
        stage, zone, act, sid = slot
        if sid is None or not os.path.exists(os.path.join(args.rom, 'files', 'narc', sid + '_map.narc')):
            summary['no_data'].append({'stage': stage, 'zone': zone, 'act': act,
                                       'reason': 'no zone map narc (Extra Zone: ExtraBoss loads extra_act_lz7.narc)'})
            print('slot %2d: no stage map data' % stage)
            continue
        try:
            man, m, bi = convert_stage(args.rom, args.out, slot, tables, not args.no_preview, args.testvec)
            cache[sid] = (m, bi)
            summary['stages'].append({k: man[k] for k in ('id', 'stage', 'zone', 'act', 'size_blocks',
                                                            'num_blocks', 'counts', 'files', 'verify',
                                                            'seconds')})
            if any(man['verify'].values()):
                summary['failed'].append({'id': sid, 'error': 'package verify mismatch %r' % man['verify']})
            print('slot %2d %-4s %4dx%-3d blocks=%3d obj=%4d rings=%4d decor=%4d level=%7d blocktex=%7d bg=%s (%.1fs)' % (
                stage, sid, man['size_blocks'][0], man['size_blocks'][1], man['num_blocks'],
                man['counts']['objects'], man['counts']['rings'], man['counts']['decorations'],
                man['files']['level.bin']['bytes'], man['files']['blocktex.bin']['bytes'],
                man['background']['bytes'] if man['background'] else '-', man['seconds']))
            print('        verify: %s' % man['verify'])
        except Exception as ex:  # report and continue
            import traceback
            traceback.print_exc()
            summary['failed'].append({'id': sid, 'error': repr(ex)})
    for ov in args.overlay:
        sid, rect = ov.split(':')
        rect = tuple(int(v) for v in rect.split(','))
        if sid not in cache:
            slot = [s for s in slots if s[3] == sid][0]
            man, m, bi = convert_stage(args.rom, args.out, slot, tables, False, 0)
            cache[sid] = (m, bi)
        m, bi = cache[sid]
        p = os.path.join(args.out, 'preview', '%s_collision_%d_%d.png' % (sid, rect[0], rect[1]))
        checked, mism = overlay_png(p, m, bi, rect, dscoll.DSMap(m))
        print('overlay %s -> %s (mask vs DS query: %d samples, %d mismatches)' % (sid, p, checked, mism))
        summary.setdefault('overlays', []).append({'id': sid, 'rect': rect, 'png': p,
                                                   'samples': checked, 'mismatches': mism})
    if args.all or not args.stage:
        json.dump(summary, open(os.path.join(args.out, 'manifest.json'), 'w'), indent=1)
    print('converted %d, failed %d, no data %d' % (len(summary['stages']), len(summary['failed']),
                                                   len(summary['no_data'])))
    return 1 if summary['failed'] else 0


if __name__ == '__main__':
    sys.exit(main())

#!/usr/bin/env python3
"""NitroSystem G3D resource reader (clean-room, written for the Dreamcast port).

Reads the binary resources produced by the NITRO-System G3D tool chain, as the
game loads them through StageObj_LoadModel / Model_SetResource / NNS_G3dDraw:

    BMD0 (.nsbmd)  MDL0 model set + optional TEX0 texture/palette block
    BTX0 (.nsbtx)  TEX0 only
    BCA0 (.nsbca)  JNT0 joint (skeletal) animations
    BVA0 (.nsbva)  VIS0 node visibility animations
    BTA0 (.nsbta)  SRT0 texture SRT animations
    BTP0 (.nsbtp)  PAT0 texture pattern animations
    BMA0 (.nsbma)  MAT0 material colour animations (header + names only)

Field layouts were taken from public format knowledge and checked against the
decompiled NNS code in src/libs/NitroSystem/g3d/ (NNSi_G3dFuncSbc_*.c,
getJntSRTAnmResult_.c, get{Trans,Rot,Scale}Data_.c, getRotDataByIdx_.c,
NNSi_G3dFuncSbc_NODEMIX.c, NNSi_G3dSendJointSRTBasic.c).

Matrix convention: DS row vectors. A 4x4 numpy matrix M maps v' = [x y z 1] @ M;
row 3 is the translation. The geometry engine's MULT commands do
current = M @ current, so a joint's world matrix is local @ parent_world and
local = S @ R @ T (NNSi_G3dSendJointSRTBasic: MULT_4x3(R|T) then SCALE).
"""
import struct

import numpy as np

FX = 4096.0


# ----------------------------------------------------------------------------- helpers

def u8(d, o):
    return d[o]


def u16(d, o):
    return struct.unpack_from("<H", d, o)[0]


def s16(d, o):
    return struct.unpack_from("<h", d, o)[0]


def u32(d, o):
    return struct.unpack_from("<I", d, o)[0]


def s32(d, o):
    return struct.unpack_from("<i", d, o)[0]


def sext(v, bits):
    m = 1 << (bits - 1)
    v &= (1 << bits) - 1
    return (v ^ m) - m


class FormatError(ValueError):
    pass


def read_dict(d, off):
    """NNSG3dResDict: header, patricia nodes, entry header {sizeUnit, ofsName}, data, names."""
    rev, num, size, _dummy, ofs_entry = struct.unpack_from("<BBHHH", d, off)
    if rev != 0:
        raise FormatError("dict revision %d at 0x%x" % (rev, off))
    eo = off + ofs_entry
    size_unit, ofs_name = struct.unpack_from("<HH", d, eo)
    items = []
    for i in range(num):
        data = d[eo + 4 + i * size_unit: eo + 4 + (i + 1) * size_unit]
        no = eo + ofs_name + 16 * i
        name = d[no:no + 16].split(b"\0")[0].decode("latin-1")
        items.append((name, data))
    return items


def file_blocks(d):
    """Common NITRO file header -> (magic, {block magic: offset})."""
    magic = d[:4]
    bom, ver, fsize, hsize, nblk = struct.unpack_from("<HHIHH", d, 4)
    if bom != 0xFEFF:
        raise FormatError("bad BOM")
    blocks = {}
    for i in range(nblk):
        bo = u32(d, hsize + 4 * i)
        blocks[d[bo:bo + 4]] = bo
    return magic, blocks


# ----------------------------------------------------------------------------- matrices

def pivot_matrix(pivot, neg, a, b):
    """Pivot-compressed rotation (NNSi_G3dFuncSbc_NODEDESC / getRotDataByIdx_).

    `pivot` (0..8) is the cell holding +-1; its row and column are 0 otherwise;
    the other four cells, row-major, receive A, B, +-B, +-A.  neg bit0 = pivot is
    -1, bit1 = negate C (=B), bit2 = negate D (=A)."""
    m = [0] * 9
    m[pivot] = -4096 if neg & 1 else 4096
    pr, pc = divmod(pivot, 3)
    cells = [i for i in range(9) if i // 3 != pr and i % 3 != pc]
    m[cells[0]] = a
    m[cells[1]] = b
    m[cells[2]] = -b if neg & 2 else b
    m[cells[3]] = -a if neg & 4 else a
    return m


def fx_cross(a, b):
    """FX cross product exactly as the NNS inline (>>12 on each 32-bit product)."""
    def tr(v):
        return int(v) >> 12
    return [tr(a[1] * b[2] - a[2] * b[1]), tr(a[2] * b[0] - a[0] * b[2]), tr(a[0] * b[1] - a[1] * b[0])]


def srt_matrix(trans=None, rot=None, scale=None):
    """Row-vector local matrix S @ R @ T from fx ints (trans 3, rot 9 row-major, scale 3)."""
    m = np.identity(4)
    if scale is not None:
        m = np.diag([scale[0] / FX, scale[1] / FX, scale[2] / FX, 1.0]) @ m
    if rot is not None:
        r = np.identity(4)
        r[:3, :3] = np.array(rot, dtype=float).reshape(3, 3) / FX
        m = m @ r
    if trans is not None:
        t = np.identity(4)
        t[3, :3] = np.array(trans, dtype=float) / FX
        m = m @ t
    return m


# ----------------------------------------------------------------------------- MDL0

SBC_NAMES = {0: "NOP", 1: "RET", 2: "NODE", 3: "MTX", 4: "MAT", 5: "SHP", 6: "NODEDESC", 7: "BB",
             8: "BBY", 9: "NODEMIX", 10: "CALLDL", 11: "POSSCALE", 12: "ENVMAP", 13: "PRJMAP"}


def parse_sbc(d, off, end):
    """Decode SBC bytecode into a list of (op, opt, args).  Stops at RET."""
    out = []
    p = off
    while p < end:
        b = d[p]
        op, opt = b & 0x1F, b & 0xE0
        if op == 0:      # NOP
            out.append(("NOP", opt, ()))
            p += 1
        elif op == 1:    # RET
            out.append(("RET", opt, ()))
            break
        elif op == 2:    # NODE nodeID visible
            out.append(("NODE", opt, (d[p + 1], d[p + 2])))
            p += 3
        elif op == 3:    # MTX stackIdx
            out.append(("MTX", opt, (d[p + 1],)))
            p += 2
        elif op == 4:    # MAT matID
            out.append(("MAT", opt, (d[p + 1],)))
            p += 2
        elif op == 5:    # SHP shpID
            out.append(("SHP", opt, (d[p + 1],)))
            p += 2
        elif op == 6:    # NODEDESC node parent flags [dst] [src]
            n = 4 + (1 if opt in (0x20, 0x40) else 2 if opt == 0x60 else 0)
            a = tuple(d[p + 1:p + n])
            node, parent, flags = a[0], a[1], a[2]
            dst = src = None
            if opt == 0x20:
                dst = a[3]
            elif opt == 0x40:
                src = a[3]
            elif opt == 0x60:
                dst, src = a[3], a[4]
            out.append(("NODEDESC", opt, (node, parent, flags, dst, src)))
            p += n
        elif op in (7, 8):  # BB / BBY node [dst] [src]
            n = 2 + (1 if opt in (0x20, 0x40) else 2 if opt == 0x60 else 0)
            a = tuple(d[p + 1:p + n])
            dst = src = None
            if opt == 0x20:
                dst = a[1]
            elif opt == 0x40:
                src = a[1]
            elif opt == 0x60:
                dst, src = a[1], a[2]
            out.append((SBC_NAMES[op], opt, (a[0], dst, src)))
            p += n
        elif op == 9:    # NODEMIX dst num {src node ratio}*num
            dst, num = d[p + 1], d[p + 2]
            items = [(d[p + 3 + 3 * i], d[p + 4 + 3 * i], d[p + 5 + 3 * i]) for i in range(num)]
            out.append(("NODEMIX", opt, (dst, items)))
            p += 3 + 3 * num
        elif op == 10:   # CALLDL relAddr size (unaligned u32s)
            out.append(("CALLDL", opt, (u32(d, p + 1), u32(d, p + 5))))
            p += 9
        elif op == 11:   # POSSCALE
            out.append(("POSSCALE", opt, ()))
            p += 1
        elif op in (12, 13):
            out.append((SBC_NAMES[op], opt, (d[p + 1], d[p + 2])))
            p += 3
        else:
            raise FormatError("unknown SBC op 0x%02x at 0x%x" % (b, p))
    return out


def parse_node(d, o):
    """NNSG3dResNodeData -> dict(flag, trans, rot, scale, inv_scale) with fx ints."""
    flag = u16(d, o)
    m00 = s16(d, o + 2)
    p = o + 4
    node = {"flag": flag, "trans": None, "rot": None, "scale": None, "inv_scale": None}
    if not flag & 1:
        node["trans"] = [s32(d, p), s32(d, p + 4), s32(d, p + 8)]
        p += 12
    if not flag & 2:
        if flag & 8:
            a, b = s16(d, p), s16(d, p + 2)
            node["rot"] = pivot_matrix((flag >> 4) & 0xF, (flag >> 8) & 7, a, b)
            p += 4
        else:
            node["rot"] = [m00] + [s16(d, p + 2 * i) for i in range(8)]
            p += 16
    if not flag & 4:
        node["scale"] = [s32(d, p), s32(d, p + 4), s32(d, p + 8)]
        node["inv_scale"] = [s32(d, p + 12), s32(d, p + 16), s32(d, p + 20)]
        p += 24
    return node


def parse_material(d, o):
    item_tag, size = u16(d, o), u16(d, o + 2)
    diff_amb, spec_emi, poly_attr, poly_mask, tex_param, tex_mask = struct.unpack_from("<6I", d, o + 4)
    pltt_base, flag, orig_w, orig_h = struct.unpack_from("<4H", d, o + 28)
    mag_w, mag_h = s32(d, o + 36), s32(d, o + 40)
    p = o + 44
    srt = {"scale": (4096, 4096), "rot": (0, 4096), "trans": (0, 0)}
    if not flag & 0x2:
        srt["scale"] = (s32(d, p), s32(d, p + 4))
        p += 8
    if not flag & 0x4:
        srt["rot"] = (s16(d, p), s16(d, p + 2))
        p += 4
    if not flag & 0x8:
        srt["trans"] = (s32(d, p), s32(d, p + 4))
        p += 8
    return {"tag": item_tag, "size": size, "diff_amb": diff_amb, "spec_emi": spec_emi,
            "poly_attr": poly_attr, "poly_attr_mask": poly_mask, "tex_param": tex_param,
            "tex_param_mask": tex_mask, "pltt_base": pltt_base, "flag": flag,
            "orig_w": orig_w, "orig_h": orig_h, "mag": (mag_w, mag_h), "tex_srt": srt,
            "texture": None, "palette": None}


def parse_model(d, mo, name):
    size, ofs_sbc, ofs_mat, ofs_shp, ofs_evp = struct.unpack_from("<5I", d, mo)
    io = mo + 20
    info = dict(zip(
        ("sbc_type", "scaling_rule", "texmtx_mode", "num_node", "num_mat", "num_shp", "first_unused_mtx", "dummy"),
        struct.unpack_from("<8B", d, io)))
    info["pos_scale"], info["inv_pos_scale"] = s32(d, io + 8), s32(d, io + 12)
    info["num_vertex"], info["num_polygon"], info["num_triangle"], info["num_quad"] = struct.unpack_from("<4H", d, io + 16)
    info["box"] = struct.unpack_from("<6h", d, io + 24)
    info["box_pos_scale"], info["box_inv_pos_scale"] = s32(d, io + 36), s32(d, io + 40)
    no = io + 44   # NNSG3dResNodeInfo: dict of u32 offsets relative to its start
    nodes = []
    for nname, nd in read_dict(d, no):
        n = parse_node(d, no + u32(nd, 0))
        n["name"] = nname
        nodes.append(n)
    sbc = parse_sbc(d, mo + ofs_sbc, mo + ofs_mat)
    # materials
    mat_o = mo + ofs_mat
    ofs_t2m, ofs_p2m = u16(d, mat_o), u16(d, mat_o + 2)
    mats = []
    for mname, md in read_dict(d, mat_o + 4):
        m = parse_material(d, mat_o + u32(md, 0))
        m["name"] = mname
        mats.append(m)
    for kind, ofs in (("texture", ofs_t2m), ("palette", ofs_p2m)):
        for tname, td in read_dict(d, mat_o + ofs):
            lo, num, _bound = u16(td, 0), td[2], td[3]
            for i in range(num):
                mats[d[mat_o + lo + i]][kind] = tname
    # shapes
    shp_o = mo + ofs_shp
    shapes = []
    for sname, sd in read_dict(d, shp_o):
        so = shp_o + u32(sd, 0)
        tag, ssize, flag, ofs_dl, size_dl = struct.unpack_from("<HHIII", d, so)
        shapes.append({"name": sname, "flag": flag, "dl": d[so + ofs_dl: so + ofs_dl + size_dl]})
    # envelope (inverse bind) matrices: one 4x3 + 3x3 per node, only present when NODEMIX is used
    evp = None
    if ofs_evp and ofs_evp < size:
        evp = []
        nev = min(info["num_node"], (size - ofs_evp) // 84)
        for i in range(nev):
            vals = struct.unpack_from("<21i", d, mo + ofs_evp + 84 * i)
            evp.append({"m": list(vals[:12]), "n": list(vals[12:])})
    return {"name": name, "info": info, "nodes": nodes, "sbc": sbc, "materials": mats,
            "shapes": shapes, "evp": evp}


# ----------------------------------------------------------------------------- TEX0

TEX_FORMATS = {0: "none", 1: "a3i5", 2: "pltt4", 3: "pltt16", 4: "pltt256", 5: "comp4x4", 6: "a5i3", 7: "direct"}
TEX_BPP = {1: 8, 2: 2, 3: 4, 4: 8, 5: 2, 6: 8, 7: 16}


def parse_tex0(d, to):
    tex_size = u16(d, to + 0x0C) << 3
    tex_dict = u16(d, to + 0x0E)
    tex_ofs = u32(d, to + 0x14)
    c_size = u16(d, to + 0x1C) << 3
    c_ofs = u32(d, to + 0x20)
    c_idx_ofs = u32(d, to + 0x24)
    pl_size = u32(d, to + 0x30) << 3
    pl_dict = u16(d, to + 0x34)
    pl_ofs = u32(d, to + 0x38)
    textures = {}
    for name, td in read_dict(d, to + tex_dict):
        param, extra = u32(td, 0), u32(td, 4)
        fmt = (param >> 26) & 7
        w, h = 8 << ((param >> 20) & 7), 8 << ((param >> 23) & 7)
        off = (param & 0xFFFF) << 3
        nbytes = w * h * TEX_BPP.get(fmt, 0) // 8
        t = {"name": name, "param": param, "format": fmt, "width": w, "height": h,
             "color0_transparent": bool(param & (1 << 29)), "orig_w": extra & 0x7FF,
             "orig_h": (extra >> 11) & 0x3FF}
        if fmt == 5:
            t["data"] = d[to + c_ofs + off: to + c_ofs + off + nbytes]
            io = to + c_idx_ofs + off // 2
            t["index"] = d[io: io + nbytes // 2]
        else:
            t["data"] = d[to + tex_ofs + off: to + tex_ofs + off + nbytes]
        textures[name] = t
    palettes = {}
    pal_all = d[to + pl_ofs: to + pl_ofs + pl_size]
    for name, pd in read_dict(d, to + pl_dict):
        off, flag = u16(pd, 0) << 3, u16(pd, 2)
        palettes[name] = {"name": name, "offset": off, "flag": flag}
    return {"textures": textures, "palettes": palettes, "palette_data": pal_all,
            "sizes": {"tex": tex_size, "tex4x4": c_size, "pltt": pl_size}}


def rgb555(c):
    r, g, b = c & 31, (c >> 5) & 31, (c >> 10) & 31
    return ((r << 3) | (r >> 2), (g << 3) | (g >> 2), (b << 3) | (b >> 2))


def decode_texture(t, pal_data, pal_off):
    """Decode one DS texture into an (h, w, 4) uint8 RGBA array (alpha 0..255)."""
    w, h, fmt, raw = t["width"], t["height"], t["format"], t["data"]
    out = np.zeros((h, w, 4), np.uint8)

    def pal(i):
        o = pal_off + 2 * i
        if o + 2 > len(pal_data):
            return (255, 0, 255)
        return rgb555(u16(pal_data, o))

    if fmt == 7:
        for i in range(w * h):
            c = u16(raw, 2 * i)
            out[i // w, i % w] = rgb555(c) + ((255 if c & 0x8000 else 0),)
    elif fmt in (2, 3, 4):
        bpp = TEX_BPP[fmt]
        per = 8 // bpp
        mask = (1 << bpp) - 1
        lut = [pal(i) for i in range(1 << bpp)]
        for i in range(w * h):
            idx = (raw[i // per] >> ((i % per) * bpp)) & mask
            a = 0 if (idx == 0 and t["color0_transparent"]) else 255
            out[i // w, i % w] = lut[idx] + (a,)
    elif fmt in (1, 6):
        ib = 5 if fmt == 1 else 3
        lut = [pal(i) for i in range(1 << ib)]
        for i in range(w * h):
            v = raw[i]
            idx, a = v & ((1 << ib) - 1), v >> ib
            a8 = ((a << 2) | (a >> 1)) * 255 // 31 if fmt == 1 else (a * 255 // 31)
            out[i // w, i % w] = lut[idx] + (a8,)
    elif fmt == 5:
        bw = w // 4
        for blk in range(len(raw) // 4):
            bits = u32(raw, 4 * blk)
            pi = u16(t["index"], 2 * blk)
            base = (pi & 0x3FFF) * 2      # in colours (4-byte units = 2 colours)
            mode = pi >> 14
            c0, c1 = pal(base), pal(base + 1)
            if mode == 0:
                cols = [c0 + (255,), c1 + (255,), pal(base + 2) + (255,), (0, 0, 0, 0)]
            elif mode == 1:
                mid = tuple((a + b) // 2 for a, b in zip(c0, c1))
                cols = [c0 + (255,), c1 + (255,), mid + (255,), (0, 0, 0, 0)]
            elif mode == 2:
                cols = [c0 + (255,), c1 + (255,), pal(base + 2) + (255,), pal(base + 3) + (255,)]
            else:
                c2 = tuple((5 * a + 3 * b) // 8 for a, b in zip(c0, c1))
                c3 = tuple((3 * a + 5 * b) // 8 for a, b in zip(c0, c1))
                cols = [c0 + (255,), c1 + (255,), c2 + (255,), c3 + (255,)]
            by, bx = divmod(blk, bw)
            for k in range(16):
                ty, tx = divmod(k, 4)
                y, x = by * 4 + ty, bx * 4 + tx
                if y < h:
                    out[y, x] = cols[(bits >> (2 * k)) & 3]
    else:
        raise FormatError("texture format %d" % fmt)
    return out


# ----------------------------------------------------------------------------- display lists

GE_PARAMS = {0x00: 0, 0x10: 1, 0x11: 0, 0x12: 1, 0x13: 1, 0x14: 1, 0x15: 0, 0x16: 16, 0x17: 12, 0x18: 16,
             0x19: 12, 0x1A: 9, 0x1B: 3, 0x1C: 3, 0x20: 1, 0x21: 1, 0x22: 1, 0x23: 2, 0x24: 1, 0x25: 1,
             0x26: 1, 0x27: 1, 0x28: 1, 0x29: 1, 0x2A: 1, 0x2B: 1, 0x30: 1, 0x31: 1, 0x32: 1, 0x33: 1,
             0x34: 32, 0x40: 1, 0x41: 0, 0x50: 1, 0x60: 1, 0x70: 3, 0x71: 2, 0x72: 1}
PRIM_NAMES = ("tris", "quads", "tristrip", "quadstrip")
VTX_CMDS = (0x23, 0x24, 0x25, 0x26, 0x27, 0x28)


def iter_ge_commands(dl):
    """Yield (cmd, [params]) from a packed GE display list (4 cmd bytes per word, then params)."""
    p, n = 0, len(dl)
    while p + 4 <= n:
        cmds = dl[p:p + 4]
        p += 4
        for c in cmds:
            if c not in GE_PARAMS:
                raise FormatError("unknown GE command 0x%02x" % c)
            k = GE_PARAMS[c]
            params = [u32(dl, p + 4 * i) for i in range(k)]
            p += 4 * k
            if c == 0 and k == 0:
                continue
            yield c, params


def decode_dl(dl, initial_mtx=-1):
    """Run a display list through a software model of the GE vertex state.

    Returns (prims, stats).  Each prim = {"type": 0..3, "verts": [...]}; each vertex
    is a dict with raw DS values: pos (x,y,z fx12 ints), mtx (matrix stack slot
    active when the vertex was emitted; `initial_mtx` before any MTX_RESTORE),
    uv (s,t raw 1.11.4 ints, last TEXCOORD), nrm (raw 10-bit triplet or None),
    col (RGB555 or None).  stats counts every command."""
    prims = []
    stats = {}
    cur = None
    mtx = initial_mtx
    pos = [0, 0, 0]
    uv = (0, 0)
    nrm = None
    col = None
    for c, prm in iter_ge_commands(dl):
        stats[c] = stats.get(c, 0) + 1
        if c == 0x14:
            mtx = prm[0] & 0x1F
        elif c == 0x40:
            cur = {"type": prm[0] & 3, "verts": []}
            prims.append(cur)
        elif c == 0x41:
            cur = None
        elif c == 0x20:
            col = prm[0] & 0x7FFF
        elif c == 0x21:
            v = prm[0]
            nrm = (sext(v, 10), sext(v >> 10, 10), sext(v >> 20, 10))
        elif c == 0x22:
            uv = (sext(prm[0], 16), sext(prm[0] >> 16, 16))
        elif c in VTX_CMDS:
            if c == 0x23:
                pos = [sext(prm[0], 16), sext(prm[0] >> 16, 16), sext(prm[1], 16)]
            elif c == 0x24:
                v = prm[0]
                pos = [sext(v, 10) << 6, sext(v >> 10, 10) << 6, sext(v >> 20, 10) << 6]
            elif c == 0x25:
                pos = [sext(prm[0], 16), sext(prm[0] >> 16, 16), pos[2]]
            elif c == 0x26:
                pos = [sext(prm[0], 16), pos[1], sext(prm[0] >> 16, 16)]
            elif c == 0x27:
                pos = [pos[0], sext(prm[0], 16), sext(prm[0] >> 16, 16)]
            elif c == 0x28:
                v = prm[0]
                pos = [(pos[0] + sext(v, 10)), (pos[1] + sext(v >> 10, 10)), (pos[2] + sext(v >> 20, 10))]
                pos = [sext(x, 16) for x in pos]
            if cur is None:
                raise FormatError("vertex outside BEGIN_VTXS")
            cur["verts"].append({"pos": tuple(pos), "mtx": mtx, "uv": uv, "nrm": nrm, "col": col})
    return prims, stats


def prim_polygons(ptype, n):
    """Polygons the GE forms from n vertices (incomplete tails are dropped by hardware)."""
    if ptype == 0:
        return n // 3, 0
    if ptype == 1:
        return 0, n // 4
    if ptype == 2:
        return max(0, n - 2), 0
    return 0, max(0, (n - 2) // 2)


def prim_triangles(ptype, idx):
    """Triangulate one primitive (indices into its vertex list) preserving DS winding."""
    n = len(idx)
    tris = []
    if ptype == 0:
        for i in range(0, n - 2, 3):
            tris.append((idx[i], idx[i + 1], idx[i + 2]))
    elif ptype == 1:
        for i in range(0, n - 3, 4):
            a, b, c, d = idx[i:i + 4]
            tris += [(a, b, c), (a, c, d)]
    elif ptype == 2:
        for i in range(n - 2):
            if i & 1:
                tris.append((idx[i + 1], idx[i], idx[i + 2]))
            else:
                tris.append((idx[i], idx[i + 1], idx[i + 2]))
    else:
        for i in range(0, n - 3, 2):
            a, b, c, d = idx[i], idx[i + 1], idx[i + 2], idx[i + 3]
            # quad strip vertex order v0 v1 v3 v2 forms the quad
            tris += [(a, b, d), (a, d, c)]
    return tris


# ----------------------------------------------------------------------------- BMD0 / BTX0

def parse_bmd(d):
    magic, blocks = file_blocks(d)
    if magic not in (b"BMD0", b"BTX0"):
        raise FormatError("not BMD0/BTX0: %r" % magic)
    res = {"magic": magic.decode(), "models": [], "tex": None}
    if b"MDL0" in blocks:
        bo = blocks[b"MDL0"]
        for name, md in read_dict(d, bo + 8):
            res["models"].append(parse_model(d, bo + u32(md, 0), name))
    if b"TEX0" in blocks:
        res["tex"] = parse_tex0(d, blocks[b"TEX0"])
    return res


# ----------------------------------------------------------------------------- BCA0 joint animation

class JointAnim:
    """One NNSG3dResJntAnm.  frame(n) returns per-node (trans|None, rot|None, scale|None) fx ints
    using the non-interpolating NNS path (getTransData_/getRotData_/getScaleData_)."""

    def __init__(self, d, o, name):
        self.d, self.o, self.name = d, o, name
        hdr = d[o:o + 4]
        if hdr[0:1] != b"J":
            raise FormatError("not a joint anim: %r" % hdr)
        self.num_frame, self.num_node = u16(d, o + 4), u16(d, o + 6)
        self.flag = u32(d, o + 8)
        self.ofs_rot3, self.ofs_rot5 = u32(d, o + 12), u32(d, o + 16)
        self.ofs_tag = [u16(d, o + 20 + 2 * i) for i in range(self.num_node)]

    def _track(self, info, ofs, frame, scale=False):
        d, base = self.d, self.o + ofs
        step = info & 0xC0000000
        last = (info & 0x1FFF0000) >> 16
        f16 = info & 0x20000000
        idx = frame

        def get(i):
            if scale:
                if f16:
                    return s16(d, base + 4 * i), s16(d, base + 4 * i + 2)
                return s32(d, base + 8 * i), s32(d, base + 8 * i + 4)
            return s16(d, base + 2 * i) if f16 else s32(d, base + 4 * i)

        def avg(a, b, w3=False):
            if scale:
                return tuple(((x * 3 + y) >> 2) if w3 else ((x + y) >> 1) if f16 else ((x + y) >> 1)
                             for x, y in zip(a, b))
            if w3:
                return (a * 3 + b) >> 2
            return ((a + b) >> 1) if f16 else ((a >> 1) + (b >> 1))

        if step == 0x40000000:
            if idx & 1:
                if idx > last:
                    return get((last >> 1) + 1)
                i0 = idx >> 1
                return avg(get(i0), get(i0 + 1))
            return get(idx >> 1)
        if step == 0x80000000:
            if idx & 3:
                if idx > last:
                    return get((idx & 3) + (last >> 2))
                if idx & 1:
                    if idx & 2:
                        i1 = idx >> 2
                        i0 = i1 + 1
                    else:
                        i0 = idx >> 2
                        i1 = i0 + 1
                    return avg(get(i0), get(i1), True)
                i0 = idx >> 2
                return avg(get(i0), get(i0 + 1))
            return get(idx >> 2)
        return get(idx)

    def rot_by_idx(self, v):
        d = self.d
        if v & 0x8000:
            o = self.o + self.ofs_rot3 + (v & 0x7FFF) * 6
            info, a, b = s16(d, o) & 0xFFFF, s16(d, o + 2), s16(d, o + 4)
            # getRotDataByIdx_: bit4 pivot minus, bit5 negate C, bit6 negate D
            return pivot_matrix(info & 0xF, (info >> 4) & 7, a, b), False
        o = self.o + self.ofs_rot5 + (v & 0x7FFF) * 10
        r = [s16(d, o + 2 * i) for i in range(5)]
        m = [0] * 9
        vv = r[4] & 7
        m[4] = r[4] >> 3
        vv = (vv << 3) | (r[0] & 7)
        m[0] = r[0] >> 3
        vv = (vv << 3) | (r[1] & 7)
        m[1] = r[1] >> 3
        vv = (vv << 3) | (r[2] & 7)
        m[2] = r[2] >> 3
        vv = (vv << 3) | (r[3] & 7)
        m[3] = r[3] >> 3
        m[5] = sext(vv, 13)
        return m, True

    def _rot(self, info, ofs, frame):
        d = self.d
        base = self.o + ofs
        step = info & 0xC0000000
        last = (info & 0x1FFF0000) >> 16

        def get(i):
            m, need = self.rot_by_idx(u16(d, base + 2 * i))
            if need:
                m[6:9] = fx_cross(m[0:3], m[3:6])
            return m, need

        def blend(i0, i1, w0):
            m, r0 = self.rot_by_idx(u16(d, base + 2 * i0))
            t, r1 = self.rot_by_idx(u16(d, base + 2 * i1))
            m = [m[k] * w0 + t[k] for k in range(9)]
            m[0:3] = fx_normalize(m[0:3])
            m[3:6] = fx_normalize(m[3:6])
            if not (r0 or r1):
                m[6:9] = fx_normalize(m[6:9])
            else:
                m[6:9] = fx_cross(m[0:3], m[3:6])
            return m

        idx = frame
        if step == 0x40000000:
            if idx & 1:
                if idx > last:
                    return get((last >> 1) + 1)[0]
                return blend(idx >> 1, (idx >> 1) + 1, 1)
            return get(idx >> 1)[0]
        if step == 0x80000000:
            if idx & 3:
                if idx > last:
                    return get((idx & 3) + (last >> 2))[0]
                if idx & 1:
                    if idx & 2:
                        i1 = idx >> 2
                        i0 = i1 + 1
                    else:
                        i0 = idx >> 2
                        i1 = i0 + 1
                    return blend(i0, i1, 3)
                return blend(idx >> 2, (idx >> 2) + 1, 1)
            return get(idx >> 2)[0]
        return get(idx)[0]

    def node_frame(self, node, frame):
        """-> (flags, trans, rot, scale). flags: 'identity' -> use identity, None entries -> model bind."""
        d = self.d
        o = self.o + self.ofs_tag[node]
        tag = u32(d, o)
        p = o + 4
        if tag & 1:
            return {"identity": True}
        r = {"identity": False, "trans": "bind", "rot": "bind", "scale": "bind"}
        if not tag & 6:
            t = []
            for bit in (8, 0x10, 0x20):
                if tag & bit:
                    t.append(s32(d, p))
                    p += 4
                else:
                    t.append(self._track(u32(d, p), u32(d, p + 4), frame))
                    p += 8
            r["trans"] = t
        elif tag & 2:
            r["trans"] = None
        if not tag & 0xC0:
            if not tag & 0x100:
                r["rot"] = self._rot(u32(d, p), u32(d, p + 4), frame)
                p += 8
            else:
                m, need = self.rot_by_idx(u32(d, p) & 0xFFFF)
                if need:
                    m[6:9] = fx_cross(m[0:3], m[3:6])
                r["rot"] = m
                p += 4
        elif tag & 0x40:
            r["rot"] = None
        if not tag & 0x600:
            s = []
            for k, bit in enumerate((0x800, 0x1000, 0x2000)):
                q = p + 8 * k
                if tag & bit:
                    s.append(s32(d, q))
                else:
                    s.append(self._track(u32(d, q), u32(d, q + 4), frame, scale=True)[0])
            r["scale"] = s
        elif tag & 0x200:
            r["scale"] = None
        return r

    def node_tag(self, node):
        return u32(self.d, self.o + self.ofs_tag[node])


def fx_normalize(v):
    """VEC_Normalize equivalent in float precision (result as fx ints)."""
    n = (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]) ** 0.5
    if n == 0:
        return [0, 0, 0]
    return [int(round(x * 4096.0 / n)) for x in v]


def parse_bca(d):
    magic, blocks = file_blocks(d)
    if magic != b"BCA0":
        raise FormatError("not BCA0")
    bo = blocks[b"JNT0"]
    return [JointAnim(d, bo + u32(ad, 0), name) for name, ad in read_dict(d, bo + 8)]


# ----------------------------------------------------------------------------- BVA0 visibility animation

def parse_bva(d):
    magic, blocks = file_blocks(d)
    if magic != b"BVA0":
        raise FormatError("not BVA0")
    bo = blocks[b"VIS0"]
    anims = []
    for name, ad in read_dict(d, bo + 8):
        o = bo + u32(ad, 0)
        num_frame, num_node, size = u16(d, o + 4), u16(d, o + 6), u16(d, o + 8)
        nbits = num_frame * num_node
        words = [u32(d, o + 12 + 4 * i) for i in range((nbits + 31) // 32)]
        vis = [[bool(words[(f * num_node + n) >> 5] >> ((f * num_node + n) & 31) & 1)
                for n in range(num_node)] for f in range(num_frame)]
        anims.append({"name": name, "num_frame": num_frame, "num_node": num_node, "vis": vis})
    return anims


# ----------------------------------------------------------------------------- BTP0 / BTA0 / BMA0

def parse_btp(d):
    """Texture pattern animation: per material a list of (frame, texture name, palette name)."""
    magic, blocks = file_blocks(d)
    if magic != b"BTP0":
        raise FormatError("not BTP0")
    bo = blocks[b"PAT0"]
    out = []
    for name, ad in read_dict(d, bo + 8):
        o = bo + u32(ad, 0)
        num_frame = u16(d, o + 4)
        num_tex, num_pltt = d[o + 6], d[o + 7]
        ofs_tex_name, ofs_pltt_name = u16(d, o + 8), u16(d, o + 10)
        tex_names = [d[o + ofs_tex_name + 16 * i: o + ofs_tex_name + 16 * i + 16].split(b"\0")[0].decode("latin-1")
                     for i in range(num_tex)]
        pl_names = [d[o + ofs_pltt_name + 16 * i: o + ofs_pltt_name + 16 * i + 16].split(b"\0")[0].decode("latin-1")
                    for i in range(num_pltt)]
        mats = []
        for mname, md in read_dict(d, o + 12):
            num_fv, flag, ofs_fv = u16(md, 0), u16(md, 2), u16(md, 6)
            keys = []
            for i in range(num_fv):
                q = o + ofs_fv + 4 * i
                keys.append((u16(d, q), tex_names[d[q + 2]] if d[q + 2] < num_tex else None,
                             pl_names[d[q + 3]] if d[q + 3] < num_pltt else None))
            mats.append({"material": mname, "keys": keys})
        out.append({"name": name, "num_frame": num_frame, "materials": mats,
                    "textures": tex_names, "palettes": pl_names})
    return out


class TexSrtAnim:
    """NNSG3dResTexSRTAnm: per material scaleS scaleT rot transS transT tracks (NNSi_G3dAnmCalcNsBta)."""

    def __init__(self, d, o, name):
        self.d, self.o, self.name = d, o, name
        self.num_frame, self.flag = u16(d, o + 4), d[o + 6]
        self.mats = []
        for mname, md in read_dict(d, o + 8):
            chans = []
            for k in range(5):
                chans.append((u32(md, 8 * k), u32(md, 8 * k + 4)))
            self.mats.append((mname, chans))

    def value(self, chan, frame):
        info, data = chan
        d = self.d
        if info & 0x20000000:          # constant
            return data
        step = (info >> 30) & 3
        f16 = info & 0x10000000
        rot = False
        i = frame >> step if step else frame
        base = self.o + data
        return (s16(d, base + 2 * i) if f16 else s32(d, base + 4 * i)), rot


def parse_bta(d):
    magic, blocks = file_blocks(d)
    if magic != b"BTA0":
        raise FormatError("not BTA0")
    bo = blocks[b"SRT0"]
    return [TexSrtAnim(d, bo + u32(ad, 0), name) for name, ad in read_dict(d, bo + 8)]


def parse_any(d):
    m = d[:4]
    if m in (b"BMD0", b"BTX0"):
        return parse_bmd(d)
    if m == b"BCA0":
        return parse_bca(d)
    if m == b"BVA0":
        return parse_bva(d)
    if m == b"BTP0":
        return parse_btp(d)
    if m == b"BTA0":
        return parse_bta(d)
    raise FormatError("unsupported %r" % m)

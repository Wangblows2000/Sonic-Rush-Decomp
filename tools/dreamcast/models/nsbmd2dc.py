#!/usr/bin/env python3
"""NSBMD/NSBCA/NSBVA -> native Dreamcast mesh/animation packages (.dcm / .dca).

Bring-your-own-ROM converter: reads the owner's extracted ROM (extract/eur/files)
and writes to build/dreamcast/models/.  Binary layouts are documented in
dreamcast/game/model/model_format.h and docs/dreamcast/formats/MODELS.md.

What it does per model (one .dcm per MDL0 entry):
  * runs the model's SBC program symbolically (NODEDESC/MTX/NODEMIX/BB/BBY/
    POSSCALE/NODE/MAT/SHP) to recover the joint hierarchy and, for every shape,
    the matrix-stack slot contents at draw time;
  * decodes every shape display list exactly (VTX_16/10/XY/XZ/YZ/DIFF, NORMAL,
    TEXCOORD, COLOR, BEGIN_VTXS tris/quads/strips, MTX_RESTORE) and emits the
    same vertices in PVR strip order (tri list -> 3-vertex strips, quad -> 4-vertex
    strip v0 v1 v3 v2, tri strip / quad strip -> one strip, same order): no vertex
    is added or dropped;
  * resolves every vertex to a matrix-palette entry (joint world / NODEMIX blend
    / billboard), bakes POSSCALE into positions, converts uv to normalised
    coordinates (with the static Si3d texture SRT);
  * decodes the TEX0 textures used (all DS formats incl. 4x4 compressed, A3I5,
    A5I3) to 16bpp ARGB1555 or ARGB4444 PVR textures (non-twiddled, power of two);
  * checks vertex/triangle/quad counts against the counts the NNS converter
    stored in the model header (numVertex/numPolygon/numTriangle/numQuad).

Animations: each NSBCA becomes a .dca with per-frame joint TRS resampled with
the exact NNS integer-frame rules (getTransData_/getRotData_/getScaleData_),
constant channels stored once, rotation as 1.14 quaternions (3x3 when a channel
contains a mirrored matrix).  NSBVA visibility -> .dca kind 2.

Usage:
  nsbmd2dc.py convert SRC [-o OUTDIR] [--png]     SRC = file or ARCHIVE.narc:member
  nsbmd2dc.py batch [--root extract/eur/files] [-o build/dreamcast/models] [--png]
"""
import argparse
import json
import math
import os
import struct
import sys
import time
from pathlib import Path

import numpy as np

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(HERE.parent / "sprites"))

import g3d  # noqa: E402
import matanim  # noqa: E402
from nds_archive import maybe_decompress, open_narc  # noqa: E402

DCM_MAGIC = b"DCM1"
DCA_MAGIC = b"DCA1"
VERSION = 2          # .dcm
DCA_VERSION = 1      # .dca / .dcv / .dcc / .dcp / .dct

PAL_JOINT, PAL_MIX, PAL_BB, PAL_BBY, PAL_IDENTITY = 0, 1, 2, 3, 4
TEX_ARGB1555, TEX_RGB565, TEX_ARGB4444 = 0, 1, 2

# material flags (dcm_material_t.flags)
MF_REPEAT_S, MF_REPEAT_T, MF_FLIP_S, MF_FLIP_T = 1, 2, 4, 8
MF_TRANSLUCENT, MF_VTXCOLOR, MF_ENVMAP, MF_WIREFRAME = 0x10, 0x20, 0x40, 0x80
MF_CULL_BACK, MF_CULL_FRONT, MF_TEXMTX, MF_DECAL = 0x100, 0x200, 0x400, 0x800

HEADER_FMT = "<4sHHI16sBBH6H2I11I4H3I6f2I"
HEADER_SIZE = struct.calcsize(HEADER_FMT)


class ConvertError(Exception):
    pass


def rgb555_argb(c, a=255):
    r, g, b = g3d.rgb555(c)
    return (a << 24) | (r << 16) | (g << 8) | b


# ----------------------------------------------------------------------------- SBC symbolic run

def run_sbc(model):
    """Symbolic SBC execution -> joints parents, draws (with slot snapshots), warnings."""
    nn = model["info"]["num_node"]
    parent = [None] * nn
    ssc = [0] * nn
    order = []
    stack = {}
    cur = ("ROOT",)
    vis_node, vis_def = 0xFFFF, 1
    mat = 0
    draws = []
    warnings = []
    for op, opt, a in model["sbc"]:
        if op == "NODEDESC":
            node, par, flags, dst, src = a
            if src is not None:
                if src not in stack:
                    raise ConvertError("NODEDESC loads empty slot %d" % src)
                cur = stack[src]
            if cur == ("ROOT",):
                p = -1
            elif cur[0] == "J":
                p = cur[1]
            else:
                raise ConvertError("NODEDESC %d on non-joint matrix %r" % (node, cur[0]))
            if parent[node] is None:
                parent[node] = p
                ssc[node] = flags
                order.append(node)
            elif parent[node] != p:
                warnings.append("node %d re-described with parent %d (was %d)" % (node, p, parent[node]))
            if p not in (-1, par) and not (p == -1 and par == node):
                warnings.append("node %d: SBC parent arg %d, matrix context %d" % (node, par, p))
            cur = ("J", node)
            if dst is not None:
                stack[dst] = cur
        elif op == "MTX":
            if a[0] not in stack:
                raise ConvertError("MTX loads empty slot %d" % a[0])
            cur = stack[a[0]]
        elif op == "NODEMIX":
            dst, items = a
            mix = []
            for src, node, ratio in items:
                s = stack.get(src)
                if s is None or s[0] != "J":
                    raise ConvertError("NODEMIX source slot %d is %r" % (src, s))
                mix.append((s[1], node, ratio / 256.0))
            cur = ("MIX", tuple(mix))
            stack[dst] = cur
        elif op in ("BB", "BBY"):
            node, dst, src = a
            if src is not None:
                cur = stack[src]
            cur = (op, cur)
            if dst is not None:
                stack[dst] = cur
        elif op == "POSSCALE":
            if opt == 0x20:
                cur = cur[1] if cur[0] == "PS" else ("PSI", cur)
            else:
                cur = ("PS", cur)
        elif op == "NODE":
            vis_node, vis_def = a[0], a[1] & 1
        elif op == "MAT":
            mat = a[0]
        elif op == "SHP":
            draws.append({"vis_node": vis_node, "vis_default": vis_def, "material": mat, "shape": a[0],
                          "cur": cur, "stack": dict(stack)})
        elif op in ("ENVMAP", "PRJMAP"):
            pass                       # material texgen setup; handled via material texgen mode
        elif op == "CALLDL":
            warnings.append("CALLDL not supported")
        elif op in ("NOP", "RET"):
            pass
    for i in range(nn):
        if parent[i] is None:
            parent[i] = -1
            order.append(i)
    return {"parent": parent, "ssc": ssc, "order": order, "draws": draws, "warnings": warnings}


# ----------------------------------------------------------------------------- pose evaluation (python reference)

def bind_local(node):
    return {"trans": node["trans"], "rot": node["rot"], "scale": node["scale"],
            "inv_scale": node["inv_scale"]}


def eval_world(model, sbc_info, locals_):
    """World matrices per joint using the Si3d scaling rule (NNSi_G3dSendJointSRTSi3d) or basic.

    locals_[j] = dict(trans|None, rot|None, scale|None) fx ints (None = zero/identity/one)."""
    rule = model["info"]["scaling_rule"]
    nn = model["info"]["num_node"]
    world = [np.identity(4) for _ in range(nn)]
    acc = [np.ones(3) for _ in range(nn)]
    one = [True] * nn
    for j in sbc_info["order"]:
        p = sbc_info["parent"][j]
        pw = world[p] if p >= 0 else np.identity(4)
        loc = locals_[j]
        t = np.array(loc["trans"], float) / g3d.FX if loc["trans"] is not None else np.zeros(3)
        r = np.array(loc["rot"], float).reshape(3, 3) / g3d.FX if loc["rot"] is not None else np.identity(3)
        s = np.array(loc["scale"], float) / g3d.FX if loc["scale"] is not None else np.ones(3)
        p_one = one[p] if p >= 0 else True
        p_acc = acc[p] if p >= 0 else np.ones(3)
        if rule == 2 and not p_one:
            # S_own @ S_pacc @ R @ T(t*pacc) @ S_pinv @ parent
            m = np.identity(4)
            m[:3, :3] = r
            m[3, :3] = t * p_acc
            m = np.diag(list(s * p_acc) + [1.0]) @ m @ np.diag(list(1.0 / p_acc) + [1.0])
        else:
            m = np.identity(4)
            m[:3, :3] = r
            m[3, :3] = t
            m = np.diag(list(s) + [1.0]) @ m
        world[j] = m @ pw
        if rule == 2:
            if loc["scale"] is None:
                one[j] = p_one
                acc[j] = p_acc.copy()
            else:
                one[j] = False
                acc[j] = s * p_acc if not p_one else s.copy()
    return world


def palette_matrix(entry, world, evp, cam_rot=None):
    kind = entry[0]
    if kind == PAL_IDENTITY:
        return np.identity(4)
    if kind == PAL_JOINT:
        return world[entry[1]]
    if kind == PAL_MIX:
        m = np.zeros((4, 4))
        for j, node, w in entry[1]:
            e = np.identity(4)
            e[:3, :3] = np.array(evp[node]["m"][:9], float).reshape(3, 3) / g3d.FX
            e[3, :3] = np.array(evp[node]["m"][9:12], float) / g3d.FX
            m += w * (e @ world[j])
        m[3, 3] = 1.0
        return m
    if kind in (PAL_BB, PAL_BBY):
        base = palette_matrix(entry[1], world, evp)
        m = np.identity(4)
        sc = np.linalg.norm(base[:3, :3], axis=1)
        if kind == PAL_BB:
            m[:3, :3] = np.diag(sc)        # facing the camera: preview camera looks down -Z
        else:
            m[:3, :3] = np.diag(sc)
            m[1, :3] = base[1, :3]
        m[3, :3] = base[3, :3]
        return m
    raise ValueError(kind)


# ----------------------------------------------------------------------------- textures

def to_pvr16(rgba):
    a = rgba[..., 3]
    if np.all((a == 0) | (a == 255)):
        fmt = TEX_ARGB1555
        v = ((a >= 128).astype(np.uint16) << 15) | ((rgba[..., 0].astype(np.uint16) >> 3) << 10) | \
            ((rgba[..., 1].astype(np.uint16) >> 3) << 5) | (rgba[..., 2].astype(np.uint16) >> 3)
    else:
        fmt = TEX_ARGB4444
        v = ((a.astype(np.uint16) >> 4) << 12) | ((rgba[..., 0].astype(np.uint16) >> 4) << 8) | \
            ((rgba[..., 1].astype(np.uint16) >> 4) << 4) | (rgba[..., 2].astype(np.uint16) >> 4)
    flags = (1 if np.any(a < 128) else 0) | (2 if np.any((a > 0) & (a < 255)) else 0)
    return fmt, v.astype("<u2").tobytes(), flags


# ----------------------------------------------------------------------------- model conversion

def material_uv_affine(mat, tex):
    """Static texture matrix for texgen mode 1 (NNSi_G3dSendTexSRTSi3d), in normalised units."""
    if not (mat["flag"] & 1) or (mat["tex_param"] >> 30) != 1 or tex is None:
        return 1.0, 1.0, 0.0, 0.0
    ss, st = mat["tex_srt"]["scale"]
    ts, tt = mat["tex_srt"]["trans"]
    ow = mat["orig_w"] or tex["width"]
    oh = mat["orig_h"] or tex["height"]
    a = ss / 4096.0
    d = st / 4096.0
    e = -(ss / 4096.0) * (ts / 4096.0) * ow / tex["width"]
    f = -(st / 4096.0) * (tt / 4096.0) * oh / tex["height"]
    mw, mh = mat["mag"]
    if mw != 4096:
        a *= mw / 4096.0
        e *= mw / 4096.0
    if mh != 4096:
        d *= mh / 4096.0
        f *= mh / 4096.0
    return a, d, e, f


def convert_model(model, texblk, want_png=False, extra_pairs=()):
    info = model["info"]
    sb = run_sbc(model)
    rep = {"name": model["name"], "src": {k: info[k] for k in ("num_vertex", "num_polygon", "num_triangle",
                                                                "num_quad", "num_node", "num_mat", "num_shp")},
           "scaling_rule": info["scaling_rule"], "pos_scale": info["pos_scale"] / 4096.0,
           "warnings": list(sb["warnings"])}
    pos_scale = info["pos_scale"] / 4096.0
    # ---------------- palette
    palette = []
    pal_index = {}

    def pal_of(sym):
        ps = 1.0
        while sym[0] in ("PS", "PSI"):
            ps *= pos_scale if sym[0] == "PS" else (info["inv_pos_scale"] / 4096.0)
            sym = sym[1]
        if sym[0] == "ROOT":
            ent = (PAL_IDENTITY, 0)
        elif sym[0] == "J":
            ent = (PAL_JOINT, sym[1])
        elif sym[0] == "MIX":
            ent = (PAL_MIX, sym[1])
        elif sym[0] in ("BB", "BBY"):
            inner, ips = pal_of(sym[1])
            if ips != 1.0:
                raise ConvertError("POSSCALE inside billboard")
            ent = (PAL_BB if sym[0] == "BB" else PAL_BBY, palette[inner])
        else:
            raise ConvertError("matrix %r" % (sym,))
        key = repr(ent)
        if key not in pal_index:
            pal_index[key] = len(palette)
            palette.append(ent)
        return pal_index[key], ps

    # ---------------- textures used
    textures = []
    tex_index = {}
    tex_pngs = []

    def tex_of(mat):
        if texblk is None or mat["texture"] is None:
            return None, None
        t = texblk["textures"].get(mat["texture"])
        if t is None:
            rep["warnings"].append("material %s: texture %s missing" % (mat["name"], mat["texture"]))
            return None, None
        key = (mat["texture"], mat["palette"])
        if key not in tex_index:
            pal_off = 0
            if t["format"] != 7:
                pl = texblk["palettes"].get(mat["palette"])
                if pl is None:
                    rep["warnings"].append("texture %s: palette %s missing" % (mat["texture"], mat["palette"]))
                else:
                    pal_off = pl["offset"]
            rgba = g3d.decode_texture(t, texblk["palette_data"], pal_off)
            fmt, data, tflags = to_pvr16(rgba)
            tex_index[key] = len(textures)
            textures.append({"name": mat["texture"], "palette": mat["palette"], "w": t["width"], "h": t["height"],
                             "format": fmt, "flags": tflags, "src_format": g3d.TEX_FORMATS[t["format"]], "data": data,
                             "orig_w": t["orig_w"], "orig_h": t["orig_h"]})
            if want_png:
                tex_pngs.append((mat["texture"] + "__" + str(mat["palette"]), rgba))
        return tex_index[key], t

    # ---------------- materials
    mats_out = []
    mat_tex = []
    for mat in model["materials"]:
        ti, t = tex_of(mat)
        mat_tex.append(t)
        pa = mat["poly_attr"]
        tp = mat["tex_param"] | (t["param"] if t else 0)
        flags = 0
        if tp & (1 << 16):
            flags |= MF_REPEAT_S
        if tp & (1 << 17):
            flags |= MF_REPEAT_T
        if tp & (1 << 18):
            flags |= MF_FLIP_S
        if tp & (1 << 19):
            flags |= MF_FLIP_T
        alpha = (pa >> 16) & 31
        if alpha == 0:
            flags |= MF_WIREFRAME
        elif alpha < 31:
            flags |= MF_TRANSLUCENT
        if t is not None and t["format"] in (1, 6):
            flags |= MF_TRANSLUCENT
        if mat["flag"] & 0x100:
            flags |= MF_VTXCOLOR
        if (mat["tex_param"] >> 30) == 2:
            flags |= MF_ENVMAP
        if not pa & 0x40:
            flags |= MF_CULL_BACK          # back face not rendered
        if not pa & 0x80:
            flags |= MF_CULL_FRONT
        if ((pa >> 4) & 3) == 1:
            flags |= MF_DECAL
        a, d, e, f = material_uv_affine(mat, t)
        if (a, d, e, f) != (1.0, 1.0, 0.0, 0.0):
            flags |= MF_TEXMTX
        da, se = mat["diff_amb"], mat["spec_emi"]
        srt_flags = 0
        if mat["flag"] & 1:       # NNSi_G3dFuncSbc_MAT_InternalDefault: res->flag from the SRT presence bits
            srt_flags = 8 | (1 if mat["flag"] & 2 else 0) | (2 if mat["flag"] & 4 else 0) | (4 if mat["flag"] & 8 else 0)
        matx = {"srt_flags": srt_flags, "texgen": mat["tex_param"] >> 30,
                "orig_w": mat["orig_w"], "orig_h": mat["orig_h"],
                "base_w": t["width"] if t else 0, "base_h": t["height"] if t else 0,
                "scale": mat["tex_srt"]["scale"], "trans": mat["tex_srt"]["trans"], "mag": mat["mag"]}
        mats_out.append({"matx": matx,
            "name": mat["name"], "texture": 0xFFFF if ti is None else ti, "flags": flags, "alpha": alpha,
            "light_mask": pa & 0xF, "poly_mode": (pa >> 4) & 3, "poly_id": (pa >> 24) & 63,
            "diffuse": rgb555_argb(da & 0x7FFF), "ambient": rgb555_argb((da >> 16) & 0x7FFF),
            "specular": rgb555_argb(se & 0x7FFF), "emission": rgb555_argb((se >> 16) & 0x7FFF),
            "uv": (a, d, e, f), "set_vtx_color": bool(da & 0x8000)})
    # ---------------- textures of the texture pattern animations bound to these materials
    for tname, pname in extra_pairs:
        tex_of({"name": "<pattern>", "texture": tname, "palette": pname})
    # ---------------- geometry
    vertices = []      # tuples
    strips = []        # (first_vertex, count, prim)
    draws = []
    cnt = {"vertex": 0, "triangle": 0, "quad": 0, "pvr_triangles": 0}
    shape_seen = {}
    for dr in sb["draws"]:
        shp = model["shapes"][dr["shape"]]
        prims, _stats = g3d.decode_dl(shp["dl"], initial_mtx=-1)
        mat = model["materials"][dr["material"]] if dr["material"] < len(model["materials"]) else None
        t = mat_tex[dr["material"]] if dr["material"] < len(mat_tex) else None
        tw, th = (t["width"], t["height"]) if t else (1, 1)
        init_pal, init_ps = pal_of(dr["cur"])
        slot_pal = {}
        for slot, sym in dr["stack"].items():
            slot_pal[slot] = pal_of(sym)
        first_strip = len(strips)
        for pr in prims:
            vs = pr["verts"]
            n = len(vs)
            tri, quad = g3d.prim_polygons(pr["type"], n)
            if dr["shape"] not in shape_seen:
                cnt["vertex"] += n
                cnt["triangle"] += tri
                cnt["quad"] += quad
            order = list(range(n))
            groups = []
            if pr["type"] == 0:
                groups = [order[i:i + 3] for i in range(0, n - n % 3, 3)]
            elif pr["type"] == 1:
                groups = [[i, i + 1, i + 3, i + 2] for i in range(0, n - n % 4, 4)]
            else:
                groups = [order]
            used = sum(len(gp) for gp in groups)
            if used != n:
                rep["warnings"].append("shape %s: %d trailing vertices outside complete polygons" % (shp["name"], n - used))
            for gp in groups:
                first = len(vertices)
                for i in gp:
                    v = vs[i]
                    if v["mtx"] < 0:
                        pi, ps = init_pal, init_ps
                    else:
                        if v["mtx"] not in slot_pal:
                            raise ConvertError("shape %s restores empty slot %d" % (shp["name"], v["mtx"]))
                        pi, ps = slot_pal[v["mtx"]]
                    x, y, z = (c / 4096.0 * ps for c in v["pos"])
                    uu, vv = v["uv"][0] / 16.0 / tw, v["uv"][1] / 16.0 / th
                    nrm = v["nrm"]
                    vflags = 0
                    if nrm is not None:
                        vflags |= 2
                        nx, ny, nz = (c * 32 for c in nrm)
                    else:
                        nx = ny = nz = 0
                    if v["col"] is not None:
                        vflags |= 1
                        col = rgb555_argb(v["col"])
                    else:
                        col = 0xFFFFFFFF
                    vertices.append((x, y, z, uu, vv, nx, ny, nz, pi, vflags, col))
                strips.append((first, len(gp), pr["type"]))
                cnt["pvr_triangles"] += len(gp) - 2
        shape_seen[dr["shape"]] = True
        draws.append({"vis_node": dr["vis_node"], "vis_default": dr["vis_default"], "material": dr["material"],
                      "shape": dr["shape"], "first_strip": first_strip, "num_strips": len(strips) - first_strip})
    # shapes never drawn by the SBC still count in the source header: count them for the check
    for si, shp in enumerate(model["shapes"]):
        if si not in shape_seen:
            prims, _ = g3d.decode_dl(shp["dl"])
            for pr in prims:
                tri, quad = g3d.prim_polygons(pr["type"], len(pr["verts"]))
                cnt["vertex"] += len(pr["verts"])
                cnt["triangle"] += tri
                cnt["quad"] += quad
            rep["warnings"].append("shape %s not referenced by SBC" % shp["name"])
    cnt["polygon"] = cnt["triangle"] + cnt["quad"]
    rep["decoded"] = dict(cnt)
    rep["emitted_vertices"] = len(vertices)
    rep["counts_match"] = (cnt["vertex"] == info["num_vertex"] and cnt["triangle"] == info["num_triangle"]
                           and cnt["quad"] == info["num_quad"] and cnt["polygon"] == info["num_polygon"])
    rep["palette"] = len(palette)
    rep["textures"] = [{"name": t["name"], "w": t["w"], "h": t["h"], "src": t["src_format"],
                        "pvr": ("ARGB1555", "RGB565", "ARGB4444")[t["format"]]} for t in textures]
    if len(palette) > 255:
        raise ConvertError("palette > 255 entries")
    # ---------------- joints
    joints = []
    for j, node in enumerate(model["nodes"]):
        flags = (1 if node["trans"] is not None else 0) | (2 if node["rot"] is not None else 0) | \
                (4 if node["scale"] is not None else 0)
        t = [c / 4096.0 for c in node["trans"]] if node["trans"] else [0.0] * 3
        r = [c / 4096.0 for c in node["rot"]] if node["rot"] else [1.0, 0, 0, 0, 1.0, 0, 0, 0, 1.0]
        s = [c / 4096.0 for c in node["scale"]] if node["scale"] else [1.0] * 3
        joints.append({"name": node["name"], "parent": sb["parent"][j], "ssc": sb["ssc"][j], "flags": flags,
                       "t": t, "r": r, "s": s})
    rep["joints"] = len(joints)
    pkg = {"name": model["name"], "info": info, "joints": joints, "order": sb["order"], "palette": palette,
           "materials": mats_out, "textures": textures, "draws": draws, "strips": strips, "vertices": vertices,
           "evp": model["evp"], "counts": cnt}
    return pkg, rep, tex_pngs


def write_dcm(pkg):
    """Serialise a converted model (layout: dreamcast/game/model/model_format.h)."""
    out = bytearray(HEADER_SIZE)

    def align():
        while len(out) % 4:
            out.append(0)

    joints = pkg["joints"]
    order = pkg["order"]
    # joints in evaluation order? keep DS node numbering (animations index by node) and store order table.
    ofs_joints = len(out)
    for j in joints:
        out += struct.pack("<hBB3f9f3f", j["parent"], j["ssc"], j["flags"], *j["t"], *j["r"], *j["s"])
    ofs_order = len(out)
    out += struct.pack("<%dH" % len(order), *order)
    align()
    mix = []
    ofs_palette = len(out)
    pal_bytes = bytearray()
    pal_list = pkg["palette"]
    pal_keys = [repr(p) for p in pal_list]
    for ent in pal_list:
        kind = ent[0]
        if kind == PAL_IDENTITY:
            pal_bytes += struct.pack("<BBHI", kind, 0, 0, 0)
        elif kind == PAL_JOINT:
            pal_bytes += struct.pack("<BBHI", kind, 0, ent[1], 0)
        elif kind == PAL_MIX:
            pal_bytes += struct.pack("<BBHI", kind, len(ent[1]), 0, len(mix))
            mix += list(ent[1])
        else:
            inner = pal_keys.index(repr(ent[1]))
            pal_bytes += struct.pack("<BBHI", kind, 0, inner, 0)
    out += pal_bytes
    ofs_mix = len(out)
    for j, node, w in mix:
        out += struct.pack("<HHf", j, node, w)
    ofs_evp = 0
    if mix and pkg["evp"]:
        ofs_evp = len(out)
        for e in pkg["evp"]:
            out += struct.pack("<12f", *[c / 4096.0 for c in e["m"]])
    ofs_mat = len(out)
    for m in pkg["materials"]:
        a, d, e, f = m["uv"]
        out += struct.pack("<HHBBBBIIII4f", m["texture"], m["flags"], m["alpha"], m["light_mask"], m["poly_mode"],
                           m["poly_id"], m["diffuse"], m["ambient"], m["specular"], m["emission"], a, d, e, f)
    ofs_tex = len(out)
    tex_hdr_pos = []
    for t in pkg["textures"]:
        tex_hdr_pos.append(len(out))
        out += struct.pack("<HHHHII", t["w"], t["h"], t["format"], t["flags"], 0, len(t["data"]))
    ofs_draw = len(out)
    for d_ in pkg["draws"]:
        out += struct.pack("<HBBHHII", d_["vis_node"], d_["vis_default"], d_["material"], d_["shape"], 0,
                           d_["first_strip"], d_["num_strips"])
    ofs_strip = len(out)
    for first, count, prim in pkg["strips"]:
        out += struct.pack("<IHH", first, count, prim)
    align()
    ofs_vtx = len(out)
    vb = np.zeros(len(pkg["vertices"]), dtype=[("p", "<f4", 3), ("uv", "<f4", 2), ("n", "<i2", 3),
                                                ("pal", "u1"), ("fl", "u1"), ("col", "<u4")])
    if pkg["vertices"]:
        arr = pkg["vertices"]
        vb["p"] = [v[0:3] for v in arr]
        vb["uv"] = [v[3:5] for v in arr]
        vb["n"] = [v[5:8] for v in arr]
        vb["pal"] = [v[8] for v in arr]
        vb["fl"] = [v[9] for v in arr]
        vb["col"] = [v[10] for v in arr]
    assert vb.itemsize == 32
    out += vb.tobytes()
    # texture data, 32-byte aligned for DMA/sq copies
    while len(out) % 32:
        out.append(0)
    ofs_texdata = len(out)
    for hp, t in zip(tex_hdr_pos, pkg["textures"]):
        struct.pack_into("<I", out, hp + 8, len(out))
        out += t["data"]
        while len(out) % 32:
            out.append(0)
    align()
    ofs_matx = len(out)
    for m in pkg["materials"]:
        x = m["matx"]
        out += struct.pack("<16sBBHHHHH6i", m["name"].encode("latin-1")[:15].ljust(16, b"\0"), x["srt_flags"],
                           x["texgen"], 0, x["orig_w"], x["orig_h"], x["base_w"], x["base_h"], x["scale"][0],
                           x["scale"][1], x["trans"][0], x["trans"][1], x["mag"][0], x["mag"][1])
    ofs_texx = len(out)
    for t in pkg["textures"]:
        out += struct.pack("<16s16sHHHH", t["name"].encode("latin-1")[:15].ljust(16, b"\0"),
                           (t["palette"] or "").encode("latin-1")[:15].ljust(16, b"\0"), t["w"], t["h"],
                           t["orig_w"], t["orig_h"])
    if pkg["vertices"]:
        pts = vb["p"]
        bmin, bmax = pts.min(axis=0).tolist(), pts.max(axis=0).tolist()
    else:
        bmin = bmax = [0.0, 0.0, 0.0]
    info = pkg["info"]
    c = pkg["counts"]
    name = pkg["name"].encode("latin-1")[:15].ljust(16, b"\0")
    struct.pack_into(HEADER_FMT, out, 0, DCM_MAGIC, VERSION, HEADER_SIZE, len(out), name,
                     info["scaling_rule"], info["texmtx_mode"], 0,
                     len(joints), len(pal_list), len(mix), len(pkg["materials"]), len(pkg["textures"]),
                     len(pkg["draws"]), len(pkg["strips"]), len(pkg["vertices"]),
                     ofs_joints, ofs_order, ofs_palette, ofs_mix, ofs_evp, ofs_mat, ofs_tex, ofs_draw, ofs_strip,
                     ofs_vtx, ofs_texdata,
                     info["num_vertex"], info["num_polygon"], info["num_triangle"], info["num_quad"],
                     c["vertex"], c["polygon"], c["pvr_triangles"], *bmin, *bmax, ofs_matx, ofs_texx)
    return bytes(out)


# ----------------------------------------------------------------------------- animation conversion

def mat_to_quat(r):
    """Row-vector rotation matrix R (v' = v @ R) -> quaternion (w, x, y, z) of the column matrix R^T."""
    m = np.array(r, float).reshape(3, 3).T
    tr = m[0, 0] + m[1, 1] + m[2, 2]
    if tr > 0:
        s = math.sqrt(tr + 1.0) * 2
        w, x, y, z = 0.25 * s, (m[2, 1] - m[1, 2]) / s, (m[0, 2] - m[2, 0]) / s, (m[1, 0] - m[0, 1]) / s
    elif m[0, 0] > m[1, 1] and m[0, 0] > m[2, 2]:
        s = math.sqrt(1.0 + m[0, 0] - m[1, 1] - m[2, 2]) * 2
        w, x, y, z = (m[2, 1] - m[1, 2]) / s, 0.25 * s, (m[0, 1] + m[1, 0]) / s, (m[0, 2] + m[2, 0]) / s
    elif m[1, 1] > m[2, 2]:
        s = math.sqrt(1.0 + m[1, 1] - m[0, 0] - m[2, 2]) * 2
        w, x, y, z = (m[0, 2] - m[2, 0]) / s, (m[0, 1] + m[1, 0]) / s, 0.25 * s, (m[1, 2] + m[2, 1]) / s
    else:
        s = math.sqrt(1.0 + m[2, 2] - m[0, 0] - m[1, 1]) * 2
        w, x, y, z = (m[1, 0] - m[0, 1]) / s, (m[0, 2] + m[2, 0]) / s, (m[1, 2] + m[2, 1]) / s, 0.25 * s
    q = np.array([w, x, y, z])
    return q / np.linalg.norm(q)


def quat_to_rowmat(q):
    w, x, y, z = q
    m = np.array([[1 - 2 * (y * y + z * z), 2 * (x * y - z * w), 2 * (x * z + y * w)],
                  [2 * (x * y + z * w), 1 - 2 * (x * x + z * z), 2 * (y * z - x * w)],
                  [2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x * x + y * y)]])
    return m.T


def sample_anim(anim):
    """-> per joint {'t': [...], 'r': [...], 's': [...]} where each is 'bind', None (identity) or list per frame."""
    nf, nj = anim.num_frame, anim.num_node
    res = []
    for j in range(nj):
        ch = {"t": [], "r": [], "s": []}
        for f in range(nf):
            r = anim.node_frame(j, f)
            if r["identity"]:
                ch["t"].append(None)
                ch["r"].append(None)
                ch["s"].append(None)
                continue
            for k in ("t", "r", "s"):
                ch[k].append(r[{"t": "trans", "r": "rot", "s": "scale"}[k]])
        res.append(ch)
    return res


def anim_locals(model_nodes, sampled, frame):
    """Per joint local SRT dict (fx ints) for frame from a sampled anim (bind fallback)."""
    out = []
    for j, node in enumerate(model_nodes):
        loc = {"trans": node["trans"], "rot": node["rot"], "scale": node["scale"]}
        if j < len(sampled):
            ch = sampled[j]
            for k, key in (("t", "trans"), ("r", "rot"), ("s", "scale")):
                v = ch[k][frame]
                if isinstance(v, str):
                    continue
                loc[key] = v
        out.append(loc)
    return out


def write_dca_joint(anims):
    """Joint animation set -> .dca bytes (kind 1). Returns (bytes, report)."""
    hdr = bytearray(struct.pack("<4sHHIHHI", DCA_MAGIC, DCA_VERSION, 1, 0, len(anims), 0, 0))
    table_pos = len(hdr)
    body = bytearray(32 * len(anims))
    data = bytearray()
    rep = []
    max_j = 0
    chan_blobs = []
    for ai, anim in enumerate(anims):
        s = sample_anim(anim)
        nf, nj = anim.num_frame, anim.num_node
        max_j = max(max_j, nj)
        chans = bytearray()
        stats = {"const": 0, "frames": 0, "matrix": 0}
        for j in range(nj):
            ch = s[j]
            modes, ofs, flags = [], [], 0
            for k in ("t", "r", "s"):
                seq = ch[k]
                if all(isinstance(v, str) for v in seq):
                    modes.append(0)
                    ofs.append(0)
                    continue
                if all(v is None for v in seq):
                    modes.append(1)
                    ofs.append(0)
                    continue
                if any(isinstance(v, str) for v in seq):
                    raise ConvertError("mixed bind/explicit channel")
                ident = {"t": [0, 0, 0], "r": [4096, 0, 0, 0, 4096, 0, 0, 0, 4096], "s": [4096] * 3}[k]
                vals = [ident if v is None else list(v) for v in seq]
                const = all(v == vals[0] for v in vals)
                samples = vals[:1] if const else vals
                modes.append(2 if const else 3)
                stats["const" if const else "frames"] += 1
                while len(data) % 4:
                    data.append(0)
                ofs.append(len(data))
                if k == "r":
                    mats = [np.array(v, float).reshape(3, 3) / 4096.0 for v in samples]
                    # quaternions only for proper rotations; mirrored matrices and the skewed
                    # matrices NNS produces when it blends two distant keys (getRotData_ interp2)
                    # are kept as 3x3 so the DS pose is reproduced exactly
                    skew = max(np.abs(mm @ mm.T - np.identity(3)).max() for mm in mats)
                    if min(np.linalg.det(mm) for mm in mats) < 0 or skew > 0.005:
                        flags |= 1
                        stats["matrix"] += 1
                        for v in samples:
                            data += struct.pack("<9h", *[max(-32768, min(32767, int(round(c * 4)))) for c in v])
                    else:
                        prev = None
                        for v in samples:
                            q = mat_to_quat(np.array(v, float) / 4096.0)
                            if prev is not None and np.dot(prev, q) < 0:
                                q = -q
                            prev = q
                            data += struct.pack("<4h", *[int(round(c * 16384)) for c in q])
                else:
                    for v in samples:
                        data += struct.pack("<3f", *[c / 4096.0 for c in v])
            chans += struct.pack("<BBBBIII", modes[0], modes[1], modes[2], flags, *ofs)
        chan_blobs.append(chans)
        name = anim.name.encode("latin-1")[:15].ljust(16, b"\0")
        struct.pack_into("<16sHHII4x", body, 32 * ai, name, nf, nj, 0, 0)
        rep.append({"name": anim.name, "frames": nf, "joints": nj, **stats})
    out = hdr + body
    chan_ofs = []
    for cb in chan_blobs:
        chan_ofs.append(len(out))
        out += cb
    while len(out) % 4:
        out.append(0)
    data_base = len(out)
    out += data
    for ai, cb in enumerate(chan_blobs):
        struct.pack_into("<II", out, table_pos + 32 * ai + 20, chan_ofs[ai], data_base)
    struct.pack_into("<IHHI", out, 8, len(out), len(anims), max_j, table_pos)
    return bytes(out), rep


def write_dca_vis(vanims):
    hdr = bytearray(struct.pack("<4sHHIHHI", DCA_MAGIC, DCA_VERSION, 2, 0, len(vanims), 0, 0))
    table_pos = len(hdr)
    out = hdr + bytearray(32 * len(vanims))
    max_j = 0
    rep = []
    for ai, va in enumerate(vanims):
        nf, nn = va["num_frame"], va["num_node"]
        max_j = max(max_j, nn)
        bits = bytearray(((nf * nn + 31) // 32) * 4)
        for f in range(nf):
            for n in range(nn):
                if va["vis"][f][n]:
                    b = f * nn + n
                    bits[b >> 3] |= 1 << (b & 7)
        pos = len(out)
        out += bits
        name = va["name"].encode("latin-1")[:15].ljust(16, b"\0")
        struct.pack_into("<16sHHII4x", out, table_pos + 32 * ai, name, nf, nn, 0, pos)
        rep.append({"name": va["name"], "frames": nf, "nodes": nn})
    struct.pack_into("<IHHI", out, 8, len(out), len(vanims), max_j, table_pos)
    return bytes(out), rep


# ----------------------------------------------------------------------------- sources / batch

def load_source(spec):
    """'path' or 'path.narc:member' -> (label, bytes)."""
    if ".narc:" in spec:
        path, member = spec.split(".narc:", 1)
        files = dict(open_narc(path + ".narc").files)
        return spec, maybe_decompress(files[member])
    return spec, maybe_decompress(Path(spec).read_bytes())


def iter_rom_resources(root):
    """Yield (label, out_stem, bytes) for every G3D resource in the ROM filesystem (files + NARC members)."""
    root = Path(root)
    for p in sorted(root.rglob("*")):
        if not p.is_file():
            continue
        rel = p.relative_to(root).as_posix()
        raw = p.read_bytes()
        if p.suffix == ".narc":
            try:
                narc = open_narc(p)
            except Exception:
                continue
            for name, b in narc.files:
                b = maybe_decompress(b)
                if b[:4] in (b"BMD0", b"BTX0", b"BCA0", b"BVA0", b"BTP0", b"BTA0", b"BMA0"):
                    stem = (rel[:-5] + "/" + name).replace("/", "__")
                    yield rel + ":" + name, stem, b
        else:
            b = maybe_decompress(raw)
            if b[:4] in (b"BMD0", b"BTX0", b"BCA0", b"BVA0", b"BTP0", b"BTA0", b"BMA0"):
                yield rel, rel.replace("/", "__"), b


def save_png(path, rgba):
    from PIL import Image
    Image.fromarray(rgba, "RGBA").save(path)


def pattern_pairs(model, texblk, btps):
    """(texture, palette) pairs of the pattern animations (parsed BTP0s) that bind to this model's
    materials (by material name, NNSi_G3dAnmObjInitNsBtp) and whose texture and palette are in the
    model's TEX0 (the game binds a pattern animation to the model whose textures it names; the
    boss models in mod/ get their patterns from the zone archives)."""
    names = {m["name"] for m in model["materials"]}
    static_pal = {m["name"]: m["palette"] for m in model["materials"]}
    pairs = []
    for anims in btps:
        for p in matanim.btp_pairs(anims, names, static_pal):
            t = texblk["textures"].get(p[0])
            if t is None or (t["format"] != 7 and p[1] not in texblk["palettes"]):
                continue
            if p not in pairs:
                pairs.append(p)
    return pairs


def convert_resource(label, stem, data, outdir, want_png=False, btps=()):
    """Convert one resource; returns manifest entry.  btps: parsed BTP0s next to a model (pattern textures)."""
    outdir = Path(outdir)
    magic = data[:4]
    ent = {"source": label, "kind": magic.decode("latin-1"), "in_bytes": len(data), "outputs": []}
    base = stem.rsplit(".", 1)[0]
    if magic in (b"BMD0", b"BTX0"):
        r = g3d.parse_bmd(data)
        ent["models"] = []
        for m in r["models"]:
            pairs = pattern_pairs(m, r["tex"], btps) if r["tex"] else []
            pkg, rep, pngs = convert_model(m, r["tex"], want_png, pairs)
            if pairs:
                rep["pattern_textures"] = len(pairs)
            blob = write_dcm(pkg)
            fn = "%s__%s.dcm" % (base, m["name"])
            (outdir / fn).write_bytes(blob)
            rep["file"] = fn
            rep["out_bytes"] = len(blob)
            ent["models"].append(rep)
            ent["outputs"].append(fn)
            if want_png:
                pd = outdir / "png" / base
                pd.mkdir(parents=True, exist_ok=True)
                for nm, rgba in pngs:
                    save_png(pd / (nm.replace("/", "_") + ".png"), rgba)
        if r["tex"]:
            ent["tex0"] = {"textures": len(r["tex"]["textures"]), "palettes": len(r["tex"]["palettes"]),
                           "formats": sorted({g3d.TEX_FORMATS[t["format"]] for t in r["tex"]["textures"].values()})}
    elif magic == b"BCA0":
        anims = g3d.parse_bca(data)
        blob, rep = write_dca_joint(anims)
        fn = base + ".dca"
        (outdir / fn).write_bytes(blob)
        ent["anims"] = rep
        ent["outputs"].append(fn)
        ent["out_bytes"] = len(blob)
    elif magic == b"BVA0":
        va = g3d.parse_bva(data)
        blob, rep = write_dca_vis(va)
        fn = base + ".dcv"
        (outdir / fn).write_bytes(blob)
        ent["vis"] = rep
        ent["outputs"].append(fn)
        ent["out_bytes"] = len(blob)
    elif magic in (b"BTP0", b"BTA0", b"BMA0"):
        kind, ext, parse, key = {b"BTP0": (matanim.DCA_KIND_TEXPAT, ".dcp", matanim.parse_btp, "pattern"),
                                 b"BTA0": (matanim.DCA_KIND_TEXSRT, ".dct", matanim.parse_bta, "srt"),
                                 b"BMA0": (matanim.DCA_KIND_MATCOL, ".dcc", matanim.parse_bma, "matcol")}[magic]
        blob, rep = matanim.write_set(kind, parse(data))
        fn = base + ext
        (outdir / fn).write_bytes(blob)
        ent[key] = rep
        ent["outputs"].append(fn)
        ent["out_bytes"] = len(blob)
    return ent


def cmd_batch(args):
    outdir = Path(args.out)
    outdir.mkdir(parents=True, exist_ok=True)
    manifest = {"generator": "tools/dreamcast/models/nsbmd2dc.py", "root": str(args.root), "entries": [],
                "failures": []}
    t0 = time.time()
    seen = {}
    resources = list(iter_rom_resources(args.root))
    # texture pattern animations: a model gets the textures of the BTP0s that bind to its materials
    # (by name, as the game does) and name textures of its TEX0 (pattern_pairs)
    btps = []
    for label, stem, data in resources:
        if data[:4] == b"BTP0":
            try:
                btps.append(matanim.parse_btp(data))
            except Exception as e:  # report, keep going
                manifest["failures"].append({"source": label, "error": "BTP0 pre-scan: %s" % e})
    for label, stem, data in resources:
        key = (data[:4], hash(data))
        if key in seen:                       # identical resource already converted (e.g. lz7 + raw narc)
            manifest["entries"].append({"source": label, "duplicate_of": seen[key]})
            continue
        seen[key] = label
        try:
            ent = convert_resource(label, stem, data, outdir, args.png, btps)
            manifest["entries"].append(ent)
        except Exception as e:  # report, keep going
            manifest["failures"].append({"source": label, "error": "%s: %s" % (type(e).__name__, e)})
    manifest["seconds"] = round(time.time() - t0, 1)
    summarize(manifest)
    (outdir / "manifest.json").write_text(json.dumps(manifest, indent=1))
    print(json.dumps(manifest["summary"], indent=1))
    return 0 if not manifest["failures"] else 1


def container_of(label):
    """'narc/x.narc:member' -> 'narc/x.narc'; 'mod/a.nsbmd' -> 'mod'."""
    return label.split(":", 1)[0] if ".narc:" in label else label.rsplit("/", 1)[0]


def summarize(man):
    s = {"resources": 0, "duplicates": 0, "failures": len(man["failures"]), "by_kind": {}, "models": 0,
         "models_count_match": 0, "models_count_mismatch": [], "vertices": 0, "polygons": 0, "pvr_triangles": 0,
         "textures": 0, "in_bytes": 0, "out_bytes": 0, "joint_anims": 0, "vis_anims": 0, "material_anims": 0,
         "warnings": 0}
    for e in man["entries"]:
        if "duplicate_of" in e:
            s["duplicates"] += 1
            continue
        s["resources"] += 1
        s["by_kind"][e["kind"]] = s["by_kind"].get(e["kind"], 0) + 1
        s["in_bytes"] += e["in_bytes"]
        s["out_bytes"] += e.get("out_bytes", 0)
        for m in e.get("models", []):
            s["models"] += 1
            s["out_bytes"] += m["out_bytes"]
            s["vertices"] += m["decoded"]["vertex"]
            s["polygons"] += m["decoded"]["polygon"]
            s["pvr_triangles"] += m["decoded"]["pvr_triangles"]
            s["textures"] += len(m["textures"])
            s["warnings"] += len(m["warnings"])
            if m["counts_match"]:
                s["models_count_match"] += 1
            else:
                s["models_count_mismatch"].append(e["source"] + ":" + m["name"])
        s["joint_anims"] += len(e.get("anims", []))
        s["vis_anims"] += len(e.get("vis", []))
        s["material_anims"] += sum(len(e.get(k, [])) for k in ("matcol", "pattern", "srt"))
    man["summary"] = s


def cmd_convert(args):
    outdir = Path(args.out)
    outdir.mkdir(parents=True, exist_ok=True)
    label, data = load_source(args.src)
    stem = Path(label.replace(".narc:", "__")).name
    ent = convert_resource(label, stem, data, outdir, args.png)
    print(json.dumps(ent, indent=1))
    return 0


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    c = sub.add_parser("convert")
    c.add_argument("src")
    c.add_argument("-o", "--out", default="build/dreamcast/models")
    c.add_argument("--png", action="store_true")
    b = sub.add_parser("batch")
    b.add_argument("--root", default="extract/eur/files")
    b.add_argument("-o", "--out", default="build/dreamcast/models")
    b.add_argument("--png", action="store_true")
    args = ap.parse_args(argv)
    return cmd_convert(args) if args.cmd == "convert" else cmd_batch(args)


if __name__ == "__main__":
    sys.exit(main())

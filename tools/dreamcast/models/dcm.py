#!/usr/bin/env python3
"""Reader + reference evaluator for the native .dcm / .dca / .dcv packages.

Mirrors dreamcast/game/model/model.c (same joint/Si3d/palette/interpolation
rules) so host-side previews validate the packages the Dreamcast loads, not the
DS source.  Layouts: dreamcast/game/model/model_format.h.
"""
import struct
from pathlib import Path

import numpy as np

HDR = "<4sHHI16sBBH6H2I11I4H3I6f"
HDR_NAMES = ("magic version header_size file_size name scaling_rule texmtx_mode flags num_joints num_palette "
             "num_mix num_materials num_textures num_draws num_strips num_vertices ofs_joints ofs_order "
             "ofs_palette ofs_mix ofs_evp ofs_materials ofs_textures ofs_draws ofs_strips ofs_vertices "
             "ofs_texdata src_num_vertex src_num_polygon src_num_triangle src_num_quad out_vertices "
             "out_polygons out_pvr_triangles").split()
VTX_DT = np.dtype([("p", "<f4", 3), ("uv", "<f4", 2), ("n", "<i2", 3), ("pal", "u1"), ("fl", "u1"), ("col", "<u4")])


class Model:
    def __init__(self, data):
        self.data = data
        vals = struct.unpack_from(HDR, data, 0)
        self.h = dict(zip(HDR_NAMES, vals[:len(HDR_NAMES)]))
        self.bbox = vals[len(HDR_NAMES):]
        h = self.h
        # version 2: material / texture extras (names, texture SRT inputs) for the material animations
        h["ofs_matx"], h["ofs_texx"] = struct.unpack_from("<2I", data, struct.calcsize(HDR)) \
            if h["version"] >= 2 else (0, 0)
        if h["magic"] != b"DCM1":
            raise ValueError("not a DCM1 package")
        self.name = h["name"].split(b"\0")[0].decode()
        self.joints = []
        for i in range(h["num_joints"]):
            v = struct.unpack_from("<hBB3f9f3f", data, h["ofs_joints"] + 64 * i)
            self.joints.append({"parent": v[0], "ssc": v[1], "flags": v[2], "t": np.array(v[3:6]),
                                "r": np.array(v[6:15]).reshape(3, 3), "s": np.array(v[15:18])})
        self.order = list(struct.unpack_from("<%dH" % h["num_joints"], data, h["ofs_order"]))
        self.palette = [struct.unpack_from("<BBHI", data, h["ofs_palette"] + 8 * i) for i in range(h["num_palette"])]
        self.mix = [struct.unpack_from("<HHf", data, h["ofs_mix"] + 8 * i) for i in range(h["num_mix"])]
        self.evp = None
        if h["ofs_evp"]:
            self.evp = [np.array(struct.unpack_from("<12f", data, h["ofs_evp"] + 48 * i))
                        for i in range(h["num_joints"])]
        self.materials = []
        for i in range(h["num_materials"]):
            v = struct.unpack_from("<HHBBBBIIII4f", data, h["ofs_materials"] + 40 * i)
            self.materials.append(dict(zip("texture flags alpha light_mask poly_mode poly_id diffuse ambient "
                                           "specular emission ua ud ue uf".split(), v)))
        self.textures = []
        for i in range(h["num_textures"]):
            w, hh, fmt, fl, ofs, size = struct.unpack_from("<HHHHII", data, h["ofs_textures"] + 16 * i)
            raw = np.frombuffer(data, "<u2", w * hh, ofs).reshape(hh, w)
            self.textures.append({"w": w, "h": hh, "format": fmt, "rgba": unpack16(raw, fmt)})
        self.draws = [dict(zip("vis_node vis_default material shape pad first_strip num_strips".split(),
                               struct.unpack_from("<HBBHHII", data, h["ofs_draws"] + 16 * i)))
                      for i in range(h["num_draws"])]
        self.strips = [struct.unpack_from("<IHH", data, h["ofs_strips"] + 8 * i) for i in range(h["num_strips"])]
        self.vtx = np.frombuffer(data, VTX_DT, h["num_vertices"], h["ofs_vertices"])
        self.material_names, self.texture_names = [], []
        if h["ofs_matx"]:
            for i in range(h["num_materials"]):
                v = struct.unpack_from("<16sBBHHHHH6i", data, h["ofs_matx"] + 52 * i)
                self.material_names.append(v[0].split(b"\0")[0].decode("latin-1"))
                self.materials[i].update(zip("srt_flags texgen pad orig_w orig_h base_w base_h scale_s scale_t "
                                             "trans_s trans_t mag_w mag_h".split(), v[1:]))
            for i in range(h["num_textures"]):
                v = struct.unpack_from("<16s16sHHHH", data, h["ofs_texx"] + 40 * i)
                self.texture_names.append((v[0].split(b"\0")[0].decode("latin-1"),
                                           v[1].split(b"\0")[0].decode("latin-1")))
                self.textures[i].update(zip("ds_w ds_h orig_w orig_h".split(), v[2:]))


def unpack16(raw, fmt):
    raw = raw.astype(np.uint32)
    out = np.zeros(raw.shape + (4,), np.uint8)
    if fmt == 0:
        out[..., 0] = ((raw >> 10) & 31) * 255 // 31
        out[..., 1] = ((raw >> 5) & 31) * 255 // 31
        out[..., 2] = (raw & 31) * 255 // 31
        out[..., 3] = np.where(raw & 0x8000, 255, 0)
    elif fmt == 2:
        out[..., 3] = ((raw >> 12) & 15) * 17
        out[..., 0] = ((raw >> 8) & 15) * 17
        out[..., 1] = ((raw >> 4) & 15) * 17
        out[..., 2] = (raw & 15) * 17
    else:
        out[..., 0] = ((raw >> 11) & 31) * 255 // 31
        out[..., 1] = ((raw >> 5) & 63) * 255 // 63
        out[..., 2] = (raw & 31) * 255 // 31
        out[..., 3] = 255
    return out


class AnimSet:
    """.dca kind 1 (joint) or kind 2 (visibility)."""

    def __init__(self, data):
        self.data = data
        magic, ver, kind, size, n, maxj, table = struct.unpack_from("<4sHHIHHI", data, 0)
        if magic != b"DCA1":
            raise ValueError("not DCA1")
        self.kind = kind
        self.anims = []
        for i in range(n):
            name, nf, nj, ofs_ch, ofs_data = struct.unpack_from("<16sHHII", data, table + 32 * i)
            self.anims.append({"name": name.split(b"\0")[0].decode(), "frames": nf, "joints": nj,
                               "ofs_ch": ofs_ch, "ofs_data": ofs_data})

    def find(self, name):
        for i, a in enumerate(self.anims):
            if a["name"] == name:
                return i
        raise KeyError(name)

    def _sample(self, a, j, f):
        d = self.data
        tm, rm, sm, fl, to, ro, so = struct.unpack_from("<BBBBIII", d, a["ofs_ch"] + 16 * j)
        base = a["ofs_data"]
        res = {}
        for key, mode, ofs, size in (("t", tm, to, 12), ("s", sm, so, 12)):
            if mode == 0:
                res[key] = "bind"
            elif mode == 1:
                res[key] = None
            else:
                k = 0 if mode == 2 else f
                res[key] = np.array(struct.unpack_from("<3f", d, base + ofs + size * k))
        if rm == 0:
            res["r"] = "bind"
        elif rm == 1:
            res["r"] = None
        else:
            k = 0 if rm == 2 else f
            if fl & 1:
                res["r"] = np.array(struct.unpack_from("<9h", d, base + ro + 18 * k), float).reshape(3, 3) / 16384.0
            else:
                res["r"] = np.array(struct.unpack_from("<4h", d, base + ro + 8 * k), float) / 16384.0
        return res

    def joint_locals(self, idx, frame, model):
        """Interpolated locals at float frame (clamped), like dcm_anim_eval()."""
        a = self.anims[idx]
        nf = a["frames"]
        frame = max(0.0, min(frame, nf - 1))
        f0 = int(frame)
        f1 = min(f0 + 1, nf - 1)
        w = frame - f0
        out = []
        for j, jt in enumerate(model.joints):
            if j >= a["joints"]:
                out.append((jt["t"], jt["r"], jt["s"]))
                continue
            s0, s1 = self._sample(a, j, f0), self._sample(a, j, f1)
            t = lerp_or(s0["t"], s1["t"], w, jt["t"], np.zeros(3))
            s = lerp_or(s0["s"], s1["s"], w, jt["s"], np.ones(3))
            r0, r1 = s0["r"], s1["r"]
            if isinstance(r0, str):
                r = jt["r"]
            elif r0 is None:
                r = np.identity(3)
            elif r0.shape == (4,):
                q = r0 * (1 - w) + r1 * w
                r = quat_rowmat(q / np.linalg.norm(q))
            else:
                r = r0 * (1 - w) + r1 * w
                if w > 0:
                    r[0] /= np.linalg.norm(r[0])
                    r[1] /= np.linalg.norm(r[1])
                    r[2] /= np.linalg.norm(r[2])
            out.append((t, r, s))
        return out

    def visibility(self, idx, frame):
        a = self.anims[idx]
        f = max(0, min(int(frame), a["frames"] - 1))
        nn = a["joints"]
        vis = []
        for n in range(nn):
            b = f * nn + n
            vis.append(bool(self.data[a["ofs_data"] + (b >> 3)] >> (b & 7) & 1))
        return vis


def lerp_or(a, b, w, bind, ident):
    if isinstance(a, str):
        return bind
    if a is None:
        return ident
    return a * (1 - w) + b * w


def quat_rowmat(q):
    w, x, y, z = q
    m = np.array([[1 - 2 * (y * y + z * z), 2 * (x * y - z * w), 2 * (x * z + y * w)],
                  [2 * (x * y + z * w), 1 - 2 * (x * x + z * z), 2 * (y * z - x * w)],
                  [2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x * x + y * y)]])
    return m.T


def bind_locals(model):
    return [(j["t"], j["r"], j["s"]) for j in model.joints]


def world_matrices(model, locals_):
    """Si3d (scaling_rule 2) / basic joint world matrices, row-vector 4x4."""
    nj = len(model.joints)
    world = [np.identity(4) for _ in range(nj)]
    acc = [np.ones(3) for _ in range(nj)]
    one = [True] * nj
    si3d = model.h["scaling_rule"] == 2
    for j in model.order:
        p = model.joints[j]["parent"]
        t, r, s = locals_[j]
        pw = world[p] if p >= 0 else np.identity(4)
        p_one = one[p] if p >= 0 else True
        p_acc = acc[p] if p >= 0 else np.ones(3)
        m = np.identity(4)
        m[:3, :3] = r
        if si3d and not p_one:
            m[3, :3] = t * p_acc
            m = np.diag(list(s * p_acc) + [1.0]) @ m @ np.diag(list(1.0 / p_acc) + [1.0])
        else:
            m[3, :3] = t
            m = np.diag(list(s) + [1.0]) @ m
        world[j] = m @ pw
        if si3d:
            unit = np.allclose(s, 1.0)
            one[j] = p_one and unit
            acc[j] = s * p_acc
    return world


def palette_matrices(model, world, view=np.identity(4)):
    mats = []
    for kind, num, joint, first in model.palette:
        if kind == 4:
            m = np.identity(4)
        elif kind == 0:
            m = world[joint]
        elif kind == 1:
            m = np.zeros((4, 4))
            for j, node, w in model.mix[first:first + num]:
                e = np.identity(4)
                e[:3, :3] = model.evp[node][:9].reshape(3, 3)
                e[3, :3] = model.evp[node][9:12]
                m += w * (e @ world[j])
            m[3, 3] = 1.0
        else:
            base = mats[joint]
            # billboard: keep translation + axis lengths, face the viewer (view-space identity rotation)
            m = np.identity(4)
            sc = np.linalg.norm(base[:3, :3], axis=1)
            m[:3, :3] = np.diag(sc)
            if kind == 3:
                m[1, :3] = base[1, :3]
            m[3, :3] = base[3, :3]
        mats.append(m)
    return mats


def load(path):
    return Model(Path(path).read_bytes())

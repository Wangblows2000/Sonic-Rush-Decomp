#!/usr/bin/env python3
"""Host-side software preview of converted .dcm/.dca packages (numpy rasteriser -> PNG).

Uses the same stage camera the game uses for player models (BossCamera_Create +
BossCamera_UpdateFixed: perspective, half-fovy 0x1555 (30 deg), aspect 4/3,
near 1, far 2048, eye (128,-96,96*cot(30deg)) looking at (128,-96,0), 1 unit =
1 DS pixel on the z=0 plane) and the player draw transform from
StageObj_Draw3DEx (rotY 0x4000 facing right, model scale 0x34cc from
Player_Create).  `--zoom` magnifies the model for inspection.

  preview.py MODEL.dcm [--anim SET.dca NAME FRAME ...] [--vis SET.dcv NAME] -o out.png
"""
import argparse
import math
import sys
from pathlib import Path

import numpy as np
from PIL import Image

sys.path.insert(0, str(Path(__file__).resolve().parent))
import dcm  # noqa: E402

LIGHTS = [np.array([-0xb50, -0xb50, 0]) / 4096.0, np.array([0, -0x617, -0xecd]) / 4096.0,
          np.array([-0xec7, 0x620, 0]) / 4096.0]
LIGHT_COL = [np.array([1.0, 1.0, 1.0]), np.array([0.55, 0.55, 0.6]), np.array([0.45, 0.45, 0.5])]


def argb(c):
    return np.array([(c >> 16) & 255, (c >> 8) & 255, c & 255], float) / 255.0


def ds_perspective(half_fovy_idx=0x1555, aspect=0x1555 / 4096.0, n=1.0, f=2048.0):
    a = half_fovy_idx / 65536.0 * 2 * math.pi
    cot = math.cos(a) / math.sin(a)
    m = np.zeros((4, 4))
    m[0, 0] = cot / aspect
    m[1, 1] = cot
    m[2, 2] = (f + n) / (n - f)
    m[2, 3] = -1.0
    m[3, 2] = 2 * f * n / (n - f)
    return m


def stage_view(half_fovy_idx=0x1555):
    a = half_fovy_idx / 65536.0 * 2 * math.pi
    eye = np.array([128.0, -96.0, 96.0 * math.cos(a) / math.sin(a)])
    v = np.identity(4)
    v[3, :3] = -eye
    return v


def rot_y(idx):
    a = idx / 65536.0 * 2 * math.pi
    s, c = math.sin(a), math.cos(a)
    m = np.identity(4)
    m[0, 0], m[0, 2], m[2, 0], m[2, 2] = c, -s, s, c
    return m


def render(model, locals_, vis, width=256, height=192, scale=0x34cc / 4096.0, facing=0x4000,
           pos=(128.0, -96.0, 0.0), extra_rot=None, bg=(40, 48, 72)):
    world = dcm.world_matrices(model, locals_)
    pal = dcm.palette_matrices(model, world)
    mdl = np.diag([scale, scale, scale, 1.0]) @ rot_y(facing)
    if extra_rot is not None:
        mdl = mdl @ extra_rot
    t = np.identity(4)
    t[3, :3] = pos
    mdl = mdl @ t
    view = stage_view()
    proj = ds_perspective()
    color = np.zeros((height, width, 3), float)
    color[:] = np.array(bg) / 255.0
    depth = np.full((height, width), np.inf)
    vtx = model.vtx
    P = vtx["p"].astype(float)
    hom = np.concatenate([P, np.ones((len(P), 1))], 1)
    mv = np.zeros((len(P), 4))
    nrm = np.zeros((len(P), 3))
    for pi, pm in enumerate(pal):
        sel = vtx["pal"] == pi
        if not sel.any():
            continue
        full = pm @ mdl @ view
        mv[sel] = hom[sel] @ full
        r = full[:3, :3]
        nv = vtx["n"][sel].astype(float) / 16384.0 @ r
        nrm[sel] = nv / np.maximum(np.linalg.norm(nv, axis=1, keepdims=True), 1e-9)
    clip = mv @ proj
    w = clip[:, 3]
    sx = (clip[:, 0] / w + 1) * width / 2
    sy = (1 - clip[:, 1] / w) * height / 2
    sz = clip[:, 2] / w
    tris_drawn = 0
    order = [d for d in model.draws if not (model.materials[d["material"]]["flags"] & 0x10)] + \
            [d for d in model.draws if model.materials[d["material"]]["flags"] & 0x10]
    for d in order:
        if d["vis_node"] != 0xFFFF:
            visible = vis[d["vis_node"]] if vis is not None and d["vis_node"] < len(vis) else d["vis_default"]
            if not visible:
                continue
        mat = model.materials[d["material"]]
        tex = model.textures[mat["texture"]] if mat["texture"] != 0xFFFF else None
        diff, amb, emi = argb(mat["diffuse"]), argb(mat["ambient"]), argb(mat["emission"])
        alpha = mat["alpha"] / 31.0
        for si in range(d["first_strip"], d["first_strip"] + d["num_strips"]):
            first, count, prim = model.strips[si]
            idx = list(range(first, first + count))
            for k in range(count - 2):
                a, b, c = (idx[k], idx[k + 1], idx[k + 2]) if k % 2 == 0 else (idx[k + 1], idx[k], idx[k + 2])
                tris_drawn += raster_tri(color, depth, (a, b, c), sx, sy, sz, w, vtx, nrm, mat, tex, diff, amb,
                                         emi, alpha)
    img = (np.clip(color, 0, 1) * 255).astype(np.uint8)
    return img, tris_drawn


def shade(nrm_v, mat, diff, amb, emi, vflags, vcol):
    if mat["light_mask"] == 0 or not (vflags & 2):
        if vflags & 1:
            return argb(vcol)
        return diff
    c = emi.copy()
    for i in range(3):
        if mat["light_mask"] & (1 << i):
            lvl = max(0.0, -float(np.dot(LIGHTS[i] / np.linalg.norm(LIGHTS[i]), nrm_v)))
            c += diff * LIGHT_COL[i] * lvl + amb * LIGHT_COL[i] * 0.5
    return np.clip(c, 0, 1)


def raster_tri(color, depth, tri, sx, sy, sz, w, vtx, nrm, mat, tex, diff, amb, emi, alpha):
    a, b, c = tri
    if min(w[a], w[b], w[c]) <= 0:
        return 0
    xs = np.array([sx[a], sx[b], sx[c]])
    ys = np.array([sy[a], sy[b], sy[c]])
    area = (xs[1] - xs[0]) * (ys[2] - ys[0]) - (xs[2] - xs[0]) * (ys[1] - ys[0])
    if abs(area) < 1e-9:
        return 0
    flags = mat["flags"]
    # DS front face = counter-clockwise on screen (y down) -> area < 0 here (checked visually:
    # culling area > 0 reproduces the uncullled image of a closed mesh)
    if area > 0 and (flags & 0x100):
        return 0
    if area < 0 and (flags & 0x200):
        return 0
    H, W = depth.shape
    x0, x1 = max(int(np.floor(xs.min())), 0), min(int(np.ceil(xs.max())), W - 1)
    y0, y1 = max(int(np.floor(ys.min())), 0), min(int(np.ceil(ys.max())), H - 1)
    if x0 > x1 or y0 > y1:
        return 0
    gx, gy = np.meshgrid(np.arange(x0, x1 + 1) + 0.5, np.arange(y0, y1 + 1) + 0.5)
    l0 = ((xs[1] - gx) * (ys[2] - gy) - (xs[2] - gx) * (ys[1] - gy)) / area
    l1 = ((xs[2] - gx) * (ys[0] - gy) - (xs[0] - gx) * (ys[2] - gy)) / area
    l2 = 1 - l0 - l1
    inside = (l0 >= -1e-6) & (l1 >= -1e-6) & (l2 >= -1e-6)
    if not inside.any():
        return 1
    z = l0 * sz[a] + l1 * sz[b] + l2 * sz[c]
    iw = np.array([1 / w[a], 1 / w[b], 1 / w[c]])
    pw = l0 * iw[0] + l1 * iw[1] + l2 * iw[2]
    cols = [shade(nrm[i], mat, diff, amb, emi, vtx["fl"][i], vtx["col"][i]) for i in (a, b, c)]
    rgb = sum(((l0, l1, l2)[k] * iw[k] / pw)[..., None] * cols[k] for k in range(3))
    texa = np.ones(z.shape)
    if tex is not None:
        uv = np.array([vtx["uv"][i] for i in (a, b, c)], float)
        uv = uv * [mat["ua"], mat["ud"]] + [mat["ue"], mat["uf"]]
        if flags & 0x40:     # env map: from view normal
            uv = np.array([[0.5 + 0.5 * nrm[i][0], 0.5 - 0.5 * nrm[i][1]] for i in (a, b, c)])
        u = sum((l0, l1, l2)[k] * iw[k] * uv[k, 0] for k in range(3)) / pw
        v = sum((l0, l1, l2)[k] * iw[k] * uv[k, 1] for k in range(3)) / pw
        tx = wrap(u * tex["w"], tex["w"], flags & 1, flags & 4)
        ty = wrap(v * tex["h"], tex["h"], flags & 2, flags & 8)
        texel = tex["rgba"][ty, tx].astype(float) / 255.0
        rgb = rgb * texel[..., :3]
        texa = texel[..., 3]
    m = inside & (z < depth[y0:y1 + 1, x0:x1 + 1]) & (texa > 0.5 if not (flags & 0x10) else texa > 0.02)
    region = color[y0:y1 + 1, x0:x1 + 1]
    if flags & 0x10:
        aa = (alpha * texa)[..., None]
        region[m] = (region * (1 - aa) + rgb * aa)[m]
    else:
        region[m] = rgb[m]
        depth[y0:y1 + 1, x0:x1 + 1][m] = z[m]
    return 1


def wrap(c, size, repeat, flip):
    i = np.floor(c).astype(int)
    if not repeat:
        return np.clip(i, 0, size - 1)
    if flip:
        p = np.mod(i, 2 * size)
        return np.where(p >= size, 2 * size - 1 - p, p)
    return np.mod(i, size)


def main(argv=None):
    ap = argparse.ArgumentParser()
    ap.add_argument("model")
    ap.add_argument("--anim", nargs=3, metavar=("DCA", "NAME", "FRAME"))
    ap.add_argument("--vis", nargs=2, metavar=("DCV", "NAME"))
    ap.add_argument("--zoom", type=float, default=1.0)
    ap.add_argument("--facing", type=lambda s: int(s, 0), default=0x4000)
    ap.add_argument("-o", "--out", required=True)
    a = ap.parse_args(argv)
    model = dcm.load(a.model)
    locals_ = dcm.bind_locals(model)
    frame = 0.0
    if a.anim:
        aset = dcm.AnimSet(Path(a.anim[0]).read_bytes())
        frame = float(a.anim[2])
        locals_ = aset.joint_locals(aset.find(a.anim[1]), frame, model)
    vis = None
    if a.vis:
        vset = dcm.AnimSet(Path(a.vis[0]).read_bytes())
        vis = vset.visibility(vset.find(a.vis[1]), frame)
    W, H = int(256 * a.zoom), int(192 * a.zoom)
    img, n = render(model, locals_, vis, W, H, scale=0x34cc / 4096.0 * a.zoom, facing=a.facing,
                    pos=(128.0, -96.0 - 16.0 * a.zoom, 0.0))
    Image.fromarray(img).save(a.out)
    print("%s: %d triangles rasterised" % (a.out, n))


if __name__ == "__main__":
    sys.exit(main())

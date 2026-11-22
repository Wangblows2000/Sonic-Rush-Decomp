# NitroSystem G3D models (DS) and the Dreamcast model pipeline

Status: converter, native runtime and host preview are working. Every G3D resource in the EUR ROM
converts: 366 models, 0 failures. For all 366 models, the vertex, triangle and quad counts decoded
from the display lists equal the counts in the DS model header. The runtime builds with `kos-cc`.
It has not yet run on Dreamcast hardware or in an emulator. See section 8.

Tools and code (all written for this port, no third-party code):

| file | role |
|------|------|
| `tools/dreamcast/models/g3d.py` | Parser for BMD0/BTX0 (MDL0 + TEX0), BCA0 (JNT0), BVA0 (VIS0), BTP0 (PAT0), BTA0 (SRT0, header only). It also has the texture decoder (all 7 DS formats) and the geometry-engine display-list decoder. |
| `tools/dreamcast/models/nsbmd2dc.py` | Converter and batch driver. It writes `.dcm` (model), `.dca` (joint animation set), `.dcv` (visibility set) and `manifest.json`. |
| `tools/dreamcast/models/dcm.py` | Python reader and reference evaluator for the packages. It mirrors `model.c`. |
| `tools/dreamcast/models/preview.py` | numpy software rasteriser. It renders a `.dcm` (+ `.dca`/`.dcv`) to PNG with the stage camera. |
| `dreamcast/game/model/model_format.h` | Binary layout of `.dcm` / `.dca` / `.dcv`. |
| `dreamcast/game/model/model.h`, `model.c` | Native runtime: load, sample animation, pose, cameras, SH-4 transform + lighting, PVR submission (`pvr_prim`). |
| `dreamcast/game/model/tests/model_hosttest.c` | Host harness. It prints the screen-space vertices from `model.c` so they can be compared with `dcm.py`. |
| `tools/dreamcast/models/matanim.py` | BMA0 / BTP0 / BTA0 → `.dcc` / `.dcp` / `.dct` (exact NNS per-frame evaluation, section 4.1). |
| `dreamcast/game/model/tests/model_matanim_test.c` | Host test: material animations, v2 extras, near-plane clipping (`gcc -std=c99 -I.. model_matanim_test.c ../model.c -lm`; argument: `build/dreamcast/models`). |
| `dreamcast/game/model/tests/matanim_dc.c`, `build_matanim_disc.sh` | KOS test disc (`build/dreamcast/model_test/mtest.cdi`): tornado + sky material animations, near-plane clipping. |

`g3d.py` imports `tools/dreamcast/sprites/nds_archive.py` for NARC and LZ77.

## 1. Where the models are and how the game uses them

- `files/mod/*.nsbmd` (147 files, 146 non-empty; `dummy.nsbmd` is 0 bytes) holds the bosses, the
  special stage, title, demo and menu models, `blz.nsbmd` and `son.nsbmd`-family variants.
  `mod/ss.nsbta` and `mod/ss.nsbtp` are the special-stage texture animations.
- These NARCs hold G3D resources:
  - `narc/player_lz7.narc`: `son.nsbmd` (models `son` + `son_ball`), `son.nsbca` (81 joint
    animations shared by Sonic and Blaze, prefixes `com_`/`son_`/`blz_`), `son.nsbva` (6 visibility
    animations: face and hand variants), `blz.nsbmd`.
  - `ss_player_lz7.narc`: the special-stage set.
  - `z??_act_lz7.narc`, `extra_act*.narc`, `act_com_lz7.narc`: boss parts, effects, gimmicks.
  - `pldm??_??_lz7.narc`: demo models with `mdl.nsbca/.nsbma/.nsbta/.nsbtp/.nsbva`.
  - `menu*.narc`: menu models.
- Counts in the ROM: 358 BMD0 (291 unique), 178 BCA0, 165 BMA0, 98 BTP0, 90 BTA0, 75 BVA0.
  779 unique resources in total; 185 are byte-identical duplicates (for example `extra_act.narc`
  and `extra_act_lz7.narc`).
- `Player_Create` calls `StageObj_LoadModel` (`Model_Init`, `ObjData_Load`, `Model_SetResource`, then
  `NNS_G3dRenderObjInit`) and sets the model scale `0x34cc` (3.3) and draw flag `0x200`.
  `StageObj_Draw3DEx` places the model in DS screen pixels (x = screen x, y = -screen y). It sets
  rotY `0x4000` (facing right) or `0xC000` (facing left). Then `Model_Draw` multiplies
  `v * S * [R | T]` (via `NNS_G3dGlbSetBase*`) and calls `NNS_G3dDraw`.

### Stage camera (found in the decompiled code)

No stage function calls `G3i_OrthoW_`, and the default `NNS_G3dGlbInit` projection is identity.
The stage projection comes from the boss camera, which `Stage_Create` creates in every act
(`src/asm_fallback/Stage_Create.c`). The call is `BossCamera_Create(act==2 ? 2 : 0)`, followed by
`data_022c4ef4 = &data_ov000_0236a448`.

- `BossCamera_Create`: half-fovy `0x1555` (30 deg), near `1.0`, far `0x800000` (2048.0),
  aspect `0x1555` (4/3), W scale 1.0.
- `BossCamera_UpdateFixed` (mode 0): `G3i_PerspectiveW_` with these values. It also calls
  `LookAt(eye = (128, -96, cot(30deg)*96), at = (128, -96, 0), up = (0,1,0))`.
  One world unit is one DS pixel on the z = 0 plane.
- `StageObj_Draw3DEx` with flag `0x200` (the player) uses an off-axis frustum:
  - `h = tan*near`, `w = h*aspect`, `d = near/eye_z`.
  - `a = d*(ty+96)`, `b = d*(128-tx)`.
  - The frustum is `G3i_FrustumW_(h-a, -(h+a), b-w, w+b, near, far)`, and the model is moved to
    `(128, -96)`.
  - The effect: the model is drawn as if it were at the screen centre (no perspective skew), but it
    appears at its real screen position.

`model.c` reproduces this camera: `dcm_camera_stage` and `dcm_camera_stage_offaxis`.
`dcm_lights_stage` uses the default light vectors from `Stage_Create` / `StageTitleCardPlayer_Draw`.

## 2. DS format notes (what the parser implements)

Common header: magic, BOM `FEFF`, version, file size, header size, block count, block offsets.
Every list is an `NNSG3dResDict`:

- header `{u8 rev=0, u8 num, u16 size, u16 pad, u16 ofsEntry}`;
- patricia nodes;
- at `ofsEntry`: `{u16 sizeUnit, u16 ofsName}`, then `num*sizeUnit` data, then 16-byte names at
  `entry+ofsName`.

**MDL0 model** (`NNSG3dResMdl`):

- `size, ofsSbc, ofsMat, ofsShp, ofsEvpMtx`, then `ResMdlInfo` (44 bytes):
  - `sbcType, scalingRule, texMtxMode, numNode, numMat, numShp, firstUnusedMtxStackID`;
  - `posScale, invPosScale`;
  - **`numVertex, numPolygon, numTriangle, numQuad`** (the converter checks its decode against these);
  - box (`x, y, z, w, h, d` as fx16), `boxPosScale`.
- Then the node dictionary. Each node is `NNSG3dResNodeData`:
  - `u16 flag`, `s16 _00`;
  - trans `3*fx32` unless flag 1;
  - rotation unless flag 2: either pivot-compressed (`flag&8`; pivot index `(flag>>4)&15`; sign bits
    `0x100` pivot -1, `0x200` negate C, `0x400` negate D; `A`, `B` fx16), or 8 fx16 (plus `_00`);
  - scale + inverse scale `6*fx32` unless flag 4.
- The pivot table (cells for A, B, C, D in row-major order after removing the pivot row and column)
  equals the 36 bytes at `0x02072b20` / `0x02072b44` in arm9.bin. This was checked.

**SBC bytecode** (low 5 bits = op, high 3 bits = option). `NNSi_G3dFuncSbc_*.c`:

| op | name | args |
|----|------|------|
| 1 | RET | (none) |
| 2 | NODE | node, visible |
| 3 | MTX | slot (restore) |
| 4 | MAT | mat |
| 5 | SHP | shp |
| 6 | NODEDESC | node, parent, flags, `[dst]` (opt `0x20`), `[src]` (opt `0x40`); opt `0x60` = dst, src |
| 7 / 8 | BB / BBY | node `[dst] [src]` |
| 9 | NODEMIX | dst, n, n x (src slot, evp node, ratio/256) |
| 10 | CALLDL | |
| 11 | POSSCALE | opt `0x20` = inverse |
| 12 / 13 | ENVMAP / PRJMAP | mat, flag |

NODEDESC semantics:

1. Restore `src` if given.
2. Multiply by the joint SRT (`NNSi_G3dSendJointSRT*`).
3. Store to `dst` if given.

The matrix stack slots are reused while the program runs. For example, in `son`, slot 1 holds the
pelvis and later the left shoulder. A slot therefore has to be resolved at the time each SHP runs.

**Materials**: `NNSG3dResMat` has `ofsDictTexToMatList`, `ofsDictPlttToMatList` and the material
dictionary. Each `NNSG3dResMatData` has:

- `diffAmb, specEmi, polyAttr(+mask), texImageParam(+mask), plttBase, flag, origW/H, magW/H`;
- optional texture SRT (scale unless flag 2, rot unless 4, trans unless 8).

The texture and palette names are bound through the two "to-material" dictionaries (index list +
count).

**Shapes**: `{tag, size, flag (1 normal, 2 colour, 4 texcoord, 8 restore-mtx), ofsDL, sizeDL}`.
A display list packs 4 command bytes into one word, followed by their parameters. Decoded commands:

- vertices: `VTX_16`, `VTX_10` (value<<6), `VTX_XY/XZ/YZ`, `VTX_DIFF` (signed 10-bit fx12 delta);
- `NORMAL` (3 x s10, /512), `TEXCOORD` (s16 1.11.4), `COLOR` (RGB555);
- `BEGIN_VTXS` (0 tris, 1 quads, 2 tri strip, 3 quad strip), `END_VTXS`;
- `MTX_RESTORE` (the skinning slot for the following vertices).

All other GE commands are parsed with their parameter counts and ignored.

**TEX0** (`NNSG3dResTex`): texInfo, tex4x4Info (data + palette-index data), plttInfo, and texture and
palette dictionaries. A texture entry is `{TEXIMAGE_PARAM, origW/H}`: offset<<3, S/T size, format,
colour-0 transparent. A palette entry is `{offset>>3, flag}`.

Formats decoded:

| # | format | notes |
|---|--------|-------|
| 1 | A3I5 | alpha `(a<<2)|(a>>1)` |
| 2 | 4-colour | |
| 3 | 16-colour | |
| 4 | 256-colour | |
| 5 | 4x4 compressed | 2-bit texels; per block `u16` = palette offset (4-byte units) + mode 0..3 (transparent / half / 4 colours / 5:3 3:5 blends) |
| 6 | A5I3 | |
| 7 | direct | A1BGR5 |

Colour-0 transparency applies to formats 2-4.

**BCA0 / JNT0** (`NNSG3dResJntAnm`):

- `'J' 0 'AC'`, `numFrame`, `numNode`, `flag`, `ofsRot3`, `ofsRot5`, then `u16 ofsTag[numNode]`.
- Per-node tag bits:
  - 1 = whole node identity;
  - translation: 2 identity, 4 bind, 8/`0x10`/`0x20` constant x/y/z;
  - rotation: `0x40` identity, `0x80` bind, `0x100` constant;
  - scale: `0x200` identity, `0x400` bind, `0x800`/`0x1000`/`0x2000` constant.
- A non-constant track is `{info, offset}`:
  - `info` bits 30-31: step 1/2/4;
  - bit 29: fx16 array;
  - bits 16-28: last interpolated frame.
- Scale samples are `(scale, invScale)` pairs.
- Rotation samples are u16 indices:
  - bit 15 set: pivot entry in rot3 (`info`, A, B);
  - otherwise a rot5 entry: 5 x s16; the 13-bit `m[5]` is spread over the low 3 bits; row 2 = cross
    product of rows 0 and 1.
- Integer-frame sampling follows `getTransData_` / `getRotData_` / `getScaleData_`. Odd frames
  between step keys blend two keys (1:1 or 3:1) and normalise the rows.

**BVA0 / VIS0**: `numFrame`, `numNode`, then a bit array `frame*numNode + node`.
**BTP0 / PAT0**: per material, a list of `(frame, texture index, palette index)` plus name tables
(parsed). **BTA0 / SRT0**: header and per-material channel table (parsed). **BMA0**: not parsed.

## 3. Conversion (`nsbmd2dc.py`)

```sh
python3 tools/dreamcast/models/nsbmd2dc.py batch [--png]          # whole ROM -> build/dreamcast/models/
python3 tools/dreamcast/models/nsbmd2dc.py convert extract/eur/files/narc/player_lz7.narc:son.nsbmd -o OUT [--png]
python3 tools/dreamcast/models/preview.py build/dreamcast/models/mod__son__son.dcm \
    --anim build/dreamcast/models/narc__player_lz7__son.dca com_walk6 10 \
    --vis build/dreamcast/models/narc__player_lz7__son.dcv com_gen00 [--zoom 3] -o run.png
```

`batch` reads `extract/eur/files` and walks loose files and NARC members (LZ-decompressed). It
converts each unique resource once and writes:

- `<path>__<model>.dcm` per MDL0 entry;
- `<path>.dca` per BCA0;
- `<path>.dcv` per BVA0;
- `png/<path>/<texture>__<palette>.png` (decoded textures, with `--png`);
- `manifest.json`, which has per-model source and decoded counts, `counts_match`, warnings, texture
  list, joint and palette counts, sizes, animation statistics, duplicates and failures.

Per model:

1. **SBC is run symbolically.** The current matrix and every stack slot hold a symbol: `ROOT`,
   `J(node)`, `MIX(...)`, `BB(x)`, `BBY(x)`, `PS(x)` (POSSCALE). This yields:
   - each joint's parent (checked against the NODEDESC parent argument);
   - the joint evaluation order;
   - for every SHP: material, visibility node + default, the current matrix and a snapshot of the
     stack.
2. **Matrix palette.** Each distinct symbol used by a vertex becomes a palette entry: joint world,
   NODEMIX blend, billboard, or identity. POSSCALE is baked into the vertex positions; no ROM model
   combines POSSCALE with MTX_RESTORE.
3. **Geometry without added or dropped vertices.** Every DS primitive becomes PVR strips in the same
   vertex order:
   - triangle list: one 3-vertex strip per triangle;
   - quad list: 4-vertex strips in order v0 v1 v3 v2;
   - triangle strip and quad strip: one strip each, same order (a DS quad strip already has
     triangle-strip order).

   The winding is kept. Vertex fields:
   - position as float (fx12 / 4096 x posScale);
   - uv normalised by the DS texture size;
   - normal as 1.14;
   - palette index: the vertex's `MTX_RESTORE` slot at SHP time, or the SHP current matrix before
     any restore;
   - `COLOR` as ARGB.
4. **Materials.**
   - repeat/flip S/T: from the material and texture `TEXIMAGE_PARAM`;
   - alpha: <31 gives translucent, 0 gives wireframe;
   - culling from polyAttr bits 6/7, light mask, poly mode, polygon ID;
   - diffuse, ambient, specular, emission as ARGB;
   - texgen mode 2: envmap flag;
   - texgen mode 1: static Si3d texture SRT (`NNSi_G3dSendTexSRTSi3d`) as a uv affine
     `u' = a*u + e`, `v' = d*v + f`.
5. **Textures.** Each used `(texture, palette)` pair is decoded to RGBA, then:
   - ARGB1555 when alpha is only 0/255, else ARGB4444;
   - linear (non-twiddled), DS power-of-two size (8..1024);
   - flags: bit 0 = has transparent texels (punch-through), bit 1 = partial alpha.
6. **Joints.** DS node numbering is kept (an animation indexes by node). Each joint stores parent,
   bind T/R/S as floats, and the NODEDESC flags.

Animations (`.dca`):

- Every frame `0..numFrame-1` is resampled with the exact NNS integer-frame rules.
- Each channel (T, R, S per joint) is one of: bind, identity, constant (1 sample) or per-frame.
- Translation and scale are float3.
- Rotation is a 1.14 quaternion, kept in the same hemisphere as the previous frame for nlerp.
  A channel is stored as 1.14 3x3 matrices instead when either:
  - a matrix is mirrored, or
  - NNS's key blend produced a skewed matrix (more than 0.005 off orthonormal). NNS blends two keys
    up to 123 deg apart in `son_goal` and does not orthogonalise the result; a quaternion cannot
    represent that.

Visibility (`.dcv`) keeps one bit per (frame, node).

## 4. Package layout

See `dreamcast/game/model/model_format.h` for the authoritative structures. All data is
little-endian, all offsets count from the file start, structures are 4-byte aligned and texture
pixels are 32-byte aligned.

`.dcm` = `dcm_header_t` (148 B, version 2; version 1 packages with the 140-byte header still load),
then these arrays:

| array | entry size |
|-------|-----------|
| joints `dcm_joint_t` | 64 B |
| order `u16` | 2 B |
| palette `dcm_palette_t` | 8 B |
| mix `dcm_mix_t` | 8 B |
| evp 4x3 floats (only when NODEMIX is used) | 48 B |
| materials `dcm_material_t` | 40 B |
| textures `dcm_texture_t` | 16 B |
| draws `dcm_draw_t` | 16 B |
| strips `dcm_strip_t` | 8 B |
| vertices `dcm_vertex_t` | 32 B |
| (v2) material extras `dcm_matx_t`: name, texture SRT inputs (flags, scale, trans, origW/H, magW/H), base texture size | 52 B |
| (v2) texture extras `dcm_texx_t`: TEX0 texture and palette name, DS size, origW/H | 40 B |

The texture pixel data follows the arrays (the v2 extras come after it). The header also records:

- `src_num_vertex/polygon/triangle/quad`, copied from the DS header;
- `out_vertices/out_polygons/out_pvr_triangles`, decoded by the converter;
- the bind-pose bounding box.

`.dca` / `.dcv` = `dca_header_t` (kind 1 joint / 2 visibility) + `dca_anim_t[]` (32 B; name,
frames, joints, channel table, data pool), then `dca_channel_t[]` (16 B: modes, flags, offsets) and
the sample pool.

### 4.1 Material animations: `.dcc` / `.dcp` / `.dct`

`tools/dreamcast/models/matanim.py` converts the three NNS material animation kinds into the same
container (`dca_header_t` kind 3 / 4 / 5). Each animation is a list of materials bound by **name**,
as `NNSi_G3dAnmObjInitNsBma/Btp/Bta` bind them. Each material is a `dcx_mat_t` (76 B: name + 5
`dcx_chan_t` {mode, value, offset}). Every channel is either constant or one sample per integer frame.

The converter evaluates the samples with exact ports of the NNS calc functions (the DS objects use
`frame >> 12`). The 1/2- and 1/4-rate interpolation and the "past the last interpolated frame" rules
are kept, so the runtime only indexes.

| kind | source | calc functions | channels |
|---|---|---|---|
| 3 `.dcc` | BMA0 | `NNSi_G3dAnmCalcNsBma`, `GetMatColAnmValue_`, `GetMatColAnmuAlphaValue_` | diffuse, ambient, specular, emission (GXRgb), polygon alpha |
| 4 `.dcp` | BTP0 | `NNSi_G3dAnmCalcNsBtp`, `NNSi_G3dGetTexPatAnmFV` | texture index \| palette index << 8 into the animation's name tables (`dca_anim_t.pad`) |
| 5 `.dct` | BTA0 | `NNSi_G3dAnmCalcNsBta`, `GetTexSRTAnm_`, `GetTexSRTAnmVectorVal_` / `SinCosVal_` | scaleS, scaleT, sin\|cos, transS, transT (fx32) |

Texture pattern frames name textures and palettes of the model's TEX0. When the batch converts a
model, it also decodes every (texture, palette) pair of every BTP0 in the ROM that:

- binds to one of the model's materials, and
- names a texture and palette that the model's TEX0 has.

`dcm_texx_t` records the names. Examples: `eff_tornado` gets 19 pattern textures, `bs4_base` 17
(from `z43_act_lz7.narc:bs4.nsbtp`). A key without a palette keeps the material's own palette
(SetPlttParamaters_ is skipped).

## 5. Runtime API (`dreamcast/game/model/model.h`)

```c
dcm_model_load(&m, bytes, size, upload_textures);     // pointers into the package; pvr_mem_malloc + pvr_txr_load
dca_set_load(&anims, bytes, size); dca_set_find(&anims, "com_walk6");
dca_sample(&anims, idx, frame, &m, locals);           // fractional frame: lerp T/S, nlerp quat / lerp+normalise matrix
dca_visibility(&vis, idx, frame, &m, visbits);
dcm_pose(&m, locals, palette);                        // Si3d/basic joint world -> palette (joint / mix / billboard)
dcm_camera_stage(&cam); dcm_camera_stage_offaxis(&cam, &tx, &ty);   // or dcm_camera_ortho
dcm_stage_model_matrix(&mdl, 0x34cc/4096.f, facing, rotz, tx, ty, tz);
dcm_lights_stage(&lights, facing_left);
dcm_draw(&m, palette, &mdl, &cam, visbits, &lights, &opts);  // opts.list = OP / TR / PT
dcm_matstate_init(&m, st);                            // material animations (v2 packages): static values ...
dcm_matanim_apply(&dcc_or_dcp_or_dct, idx, frame, &m, st);   // ... overridden per animation, integer frame
dcm_draw_ex(&m, palette, &mdl, &cam, visbits, &lights, &opts, st);
dcm_transform_vertices(...);                          // host/test hook
dcm_host_emit = hook;                                 // host tests: every vertex dcm_draw(_ex) would submit
```

Joint maths:

- `local = S*R*T`; `world = local*parent`.
- Si3d rule, once a parent has a non-unit accumulated scale A: `S_own*S_A*R*T(t*A)*S_A^-1*parent`.
  This is what `NNSi_G3dSendJointSRTSi3d` does. All 366 ROM models use scaling rule 2 (Si3d) and
  texture-matrix mode 1 (Si3d).

`dcm_draw`:

1. Builds one combined 4x4 (palette x model x view x projection) per palette entry.
2. Transforms each vertex on the SH-4.
3. Computes DS-style lighting per vertex: emission + sum(diffuse x light x max(0, -L.n) +
   ambient x light). Unlit vertices use COLOR or the material diffuse.
4. Sends one `pvr_poly_hdr_t` per draw, then `pvr_vertex_t` strips (EOL on each strip's last vertex).
   - Near plane: a strip with a vertex at clip `w < near_w` is split into its triangles (odd ones
     rewound). Each triangle is clipped against `w = near_w` (Sutherland–Hodgman; position, uv and
     colour interpolated) and sent as one 3- or 4-vertex strip. Triangles fully behind are dropped.
   - Material states (`dcm_draw_ex`), the `NNSG3dMatAnmResult` the MAT command builds:
     - polygon alpha 0 skips the material's shapes, as NNS does (also for static materials);
     - the texture comes from the pattern (vertex uv rescaled by base size / frame size);
     - colours feed the lighting / unlit colour;
     - the uv affine is `NNSi_G3dSendTexSRTSi3d` (scale, translation, origW/H, magW/H). Si3d texture
       matrices ignore the rotation, as on the DS; every ROM model is Si3d.
     - Class: alpha < 31 → TR, else the class of the bound texture.
   - Textures: non-twiddled; nearest filter (like the DS); clamp/flip from the material.
   - Culling: DS back faces are clockwise on screen, so `PVR_CULLING_CW`.
   - Depth: `1/w`.
   - Lists: translucent materials and ARGB4444 textures go to TR; ARGB1555 textures with transparent
     texels go to `opts.pt_list`; everything else goes to OP.

Build: `source /opt/toolchains/dc/kos/environ.sh && kos-cc -c dreamcast/game/model/model.c`. This
compiles without warnings with `-Wall -Wextra`: 8.4 KB text, 51 KB bss (static transform scratch for
256 palette entries).

## 6. Validation

All results are from runs in this session.

- **Material animations and near plane (2026-11-22).**
  - The batch now converts all 779 unique resources, with 0 failures and 0 warnings. That includes
    286 material-animation sets (127 `.dcc`, 80 `.dcp`, 79 `.dct`) and 876 textures (+132 pattern
    textures).
  - `tests/model_matanim_test.c` passes 5672/5672 checks on the converted packages:
    - every `.dcm` loads as version 2;
    - the uv affine that `dcm_matstate_uv` derives from the stored SRT inputs equals the converter's
      static `uv_a/d/e/f` for all 1451 materials;
    - `dcm_draw_ex(static states)` submits exactly what `dcm_draw` does;
    - every set loads and its sample offsets are in range;
    - `eff_tornado01`: t_grow alpha 0 until frame 17, t_mini_loop alpha 12 at frame 3, pattern
      t_grow01 → 02 at frame 19 → 04 at 23;
    - `bsf.nsbta`: transT -2 at frame 1, and the uv moves in t;
    - a synthetic triangle crossing the near plane gives 4 clipped vertices, none behind the camera.
  - Flycast, `tests/build_matanim_disc.sh` (`mtest.cdi`): two `eff_tornado` with their
    `.dcc`/`.dcp`, and `bsf_sky` as a sphere around the eye with `bsf.dct`/`.dcc`, observed in
    screenshots:
    - the left tornado fades in and switches texture frames;
    - the sky surrounds the camera without holes (its triangles cross the near plane and are
      clipped) and scrolls;
    - the moon's diffuse runs to 0 (black disc) as `bsf.nsbma` says.
    - The serial log matches the expected frames.

- **Counts (before the material animations).** The batch converted 779 unique resources: 291 BMD0, 134 BCA0, 68 BVA0, 80 BTP0,
  79 BTA0 and 127 BMA0. There were 0 failures and 0 warnings.
  - 366 models. For all 366, the decoded vertex, triangle, quad and polygon counts equal the
    `numVertex/numTriangle/numQuad/numPolygon` in their DS headers, and the emitted vertex count
    equals `numVertex` (no shape is drawn twice).
  - Totals: 107,759 vertices, 40,905 DS polygons, which the PVR draws as 63,075 triangles (a quad is
    2 triangles); 744 textures (575 ARGB1555, 169 ARGB4444) from pltt16 513, a3i5 105, a5i3 70,
    pltt256 38, pltt4 17 and direct 1. No texture in the ROM uses 4x4 compressed format 5.
  - Palette entries: 1,287 joint, 195 billboard (BB), 15 Y-billboard (BBY). No ROM model uses
    NODEMIX.
  - Sizes: 11.4 MB in. Out: `.dcm` 16.1 MB, `.dca` 16.9 MB, `.dcv` 0.25 MB.
- **son.nsbmd `son`.**
  - The DS header says 3246 vertices, 1445 polygons (1016 triangles, 429 quads). The decode gives
    3246 / 1016 / 429, which the PVR draws as 1874 triangles.
  - 36 joints, 19 palette entries, 24 draws, 5 textures (pltt16 to ARGB1555, 64-128 px).
  - `.dcm` 252,000 B. `son.nsbca`: 81 animations, 4841 frames, `.dca` 1,079,296 B. `.dcv` 3,596 B.
- **Animation accuracy.**
  - Package against the NNS sampler, all 136,876 (anim, frame, joint) samples of `son.nsbca`:
    translation and scale are exact; maximum rotation element error 0.0117 (1.14 quaternion
    quantisation).
  - Joint world matrices from the DS data (`nsbmd2dc.eval_world` on fx values) against the package
    evaluator, 951 poses: maximum difference 0.010 model units, which is 0.03 DS pixels at the
    player scale.
- **C runtime against the Python reference.** `model_hosttest.c` (host gcc, same `model.c`) for
  `com_walk6` f12.5, `com_fw` f30, `com_jump_s_02` f7.25 and `son_goal` f262 (a matrix-mode
  channel): screen positions of all 3246 vertices agree within 9e-5 px and depth within 5e-7.
  At the stage camera the player covers about 72x76 px on 640x480, which is about 29x30 DS pixels.
- **Visual check.** The preview PNGs were inspected:
  - `build/dreamcast/models/preview/son_idle_run_jump.png` (idle `com_fw`, run cycle `com_walk6`,
    jumps), `son_run_ds_scale.png`;
  - `misc_models.png`: Blaze `blz_dash1`/`blz_walk1` using the shared skeleton, `dm_son`, a
    translucent emerald, `son_ball` billboards;
  - `boss_arenas.png`: `bs1_base` with posScale 64, `bs4_base`, `bs2_body`.

  Sonic and Blaze are textured correctly, with the right face and hand variants from the visibility
  animation, and the poses change across frames. The DS front-face orientation was set from a
  closed mesh: with that orientation, back-face culling gives the same image as no culling.
- **Textures.** Decoded textures (`png/`) were inspected for `son`. The 4x4 (all 4 modes), A3I5, A5I3
  and 4-colour decoders were checked against hand-built blocks with known results.

## 7. Licences

No third-party code was used. The format knowledge comes from public descriptions of the NITRO G3D
binary formats and from the decompiled NNS code in this repository.

## 8. Gaps

- Not run on Dreamcast hardware. In Flycast, the material animations and the near-plane clipping
  were observed with `mtest.cdi` (section 6). The PVR culling direction, uv flip/clamp mapping and
  list classes are derived from the DS rules; they were not checked against the DS.
- Lighting covers only diffuse, ambient and emission. Not implemented: specular and the shininess
  table, toon/highlight mode, shadow polygons, fog, edge marking, polygon-ID effects.
  `dcm_lights_stage` uses default colours (white key, grey fill); the per-zone light colours that
  `Stage_Create` loads with `NNS_G3dGlbLightColor` are not wired in yet.
- Material animations: several anim objects of one kind on one material run in render-object
  order with no blending (`NNSi_G3dAnmBlendMat`). The boss cross-fade applies the fading-out object
  first; the real NNS list order was not checked.
- The special-stage module (`dreamcast/game/special`) still keeps `ss.nsbta` / `ss.nsbtp` as timing
  only. The converted `mod__ss.dct` / `.dcp` exist; `sp_map.c` / `sp_draw.c` would apply them the way
  `boss_model_matstates` does.
- ENVMAP is approximated with a sphere map from the view-space normal. PRJMAP is not implemented.
  The billboard (BB/BBY) is rebuilt in view space (axis lengths + translation kept), which is the
  intent of `NNSi_G3dFuncSbc_BB`; BBY is an approximation.
- NODEMIX (envelope skinning) and 4x4-compressed textures are implemented but not used by any ROM
  resource, so they are tested only with synthetic data (4x4) or not at all (NODEMIX). The Maya
  SSC rule and the non-Si3d texture-matrix modes are not implemented; no ROM model uses them.
- Not exported: the material "set vertex colour" bit, MAT/SHP option bits and CALLDL. Unlit
  vertices without COLOR always take the (animated) diffuse; every animated ROM material checked
  (e.g. `bsf_pas_moon`, diffuse 0xffff) has that bit set.
- Animation packages are per-frame and about 1.5x the DS size (16.9 MB for all sets; 1.08 MB for
  the player set). Keyframe reduction is possible if RAM gets tight.

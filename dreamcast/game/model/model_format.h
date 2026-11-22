/*
 * model_format.h - native Dreamcast 3D model / animation package layout.
 *
 * Produced by tools/dreamcast/models/nsbmd2dc.py from the owner's ROM
 * (NitroSystem G3D .nsbmd / .nsbca / .nsbva).  Read by model.c.
 * All data little-endian (SH-4 runs little-endian on the Dreamcast), all
 * offsets are byte offsets from the start of the file, structures are 4-byte
 * aligned, texture data 32-byte aligned.  Matrix convention is the DS one:
 * row vectors, v' = v * M, row 3 = translation; a joint's world matrix is
 * local * parent_world, local = S * R * T (Si3d scaling rule adds the
 * parent's accumulated scale compensation, see model.c).
 *
 * Format and tool: docs/dreamcast/formats/MODELS.md.
 */
#ifndef DC_MODEL_FORMAT_H
#define DC_MODEL_FORMAT_H

#include <stdint.h>

#define DCM_MAGIC 0x314D4344u /* "DCM1" */
#define DCA_MAGIC 0x31414344u /* "DCA1" */
#define DCM_VERSION 1

/* ------------------------------------------------------------------ .dcm (one model) */

typedef struct {
    uint32_t magic;          /* DCM_MAGIC */
    uint16_t version;        /* DCM_VERSION */
    uint16_t header_size;    /* sizeof(dcm_header_t) = 140 */
    uint32_t file_size;
    char name[16];           /* MDL0 model name, NUL padded */
    uint8_t scaling_rule;    /* 0 basic, 1 Maya SSC, 2 Si3d (all Sonic Rush models) */
    uint8_t texmtx_mode;     /* 0 Maya, 1 Si3d, 2 3dsMax, 3 XSI */
    uint16_t flags;
    uint16_t num_joints;     /* == DS node count; joint i == DS node i (animations index by it) */
    uint16_t num_palette;    /* matrix palette entries referenced by vertices */
    uint16_t num_mix;        /* NODEMIX weights (dcm_mix_t) */
    uint16_t num_materials;
    uint16_t num_textures;
    uint16_t num_draws;
    uint32_t num_strips;
    uint32_t num_vertices;
    uint32_t ofs_joints;     /* dcm_joint_t[num_joints] */
    uint32_t ofs_order;      /* uint16_t[num_joints]: joint evaluation order (parents first) */
    uint32_t ofs_palette;    /* dcm_palette_t[num_palette] */
    uint32_t ofs_mix;        /* dcm_mix_t[num_mix] */
    uint32_t ofs_evp;        /* float[num_joints][12] inverse bind (4x3) or 0 if no NODEMIX */
    uint32_t ofs_materials;  /* dcm_material_t[num_materials] */
    uint32_t ofs_textures;   /* dcm_texture_t[num_textures] */
    uint32_t ofs_draws;      /* dcm_draw_t[num_draws], in SBC draw order */
    uint32_t ofs_strips;     /* dcm_strip_t[num_strips] */
    uint32_t ofs_vertices;   /* dcm_vertex_t[num_vertices] */
    uint32_t ofs_texdata;    /* start of texture pixel data */
    /* counts stored by the NNS converter in the DS model header ... */
    uint16_t src_num_vertex, src_num_polygon, src_num_triangle, src_num_quad;
    /* ... and what the converter decoded from the display lists */
    uint32_t out_vertices;   /* == src_num_vertex when no shape is drawn twice */
    uint32_t out_polygons;   /* DS polygons (tri + quad) == src_num_polygon */
    uint32_t out_pvr_triangles; /* triangles the PVR rasterises (quad = 2) */
    float bbox_min[3], bbox_max[3]; /* bind-pose palette-space vertex bounds */
} dcm_header_t;

#define DCM_JF_HAS_TRANS 1
#define DCM_JF_HAS_ROT 2
#define DCM_JF_HAS_SCALE 4

typedef struct {
    int16_t parent;          /* -1 = root */
    uint8_t ssc;             /* NODEDESC flags (Maya segment-scale-compensate bits) */
    uint8_t flags;           /* DCM_JF_* (bind pose) */
    float t[3];              /* bind translation */
    float r[9];              /* bind rotation, row-major rows (DS layout) */
    float s[3];              /* bind scale */
} dcm_joint_t;               /* 64 bytes */

enum {
    DCM_PAL_JOINT = 0,       /* world matrix of joint `joint` */
    DCM_PAL_MIX = 1,         /* sum(weight * evp[node] * world[joint]) over mix[first..first+num) */
    DCM_PAL_BB = 2,          /* billboard of palette entry `joint` (camera facing) */
    DCM_PAL_BBY = 3,         /* Y-axis billboard of palette entry `joint` */
    DCM_PAL_IDENTITY = 4     /* model root (identity) */
};

typedef struct {
    uint8_t kind;            /* DCM_PAL_* */
    uint8_t num;             /* mix count */
    uint16_t joint;          /* joint index, or palette index for billboards */
    uint32_t first;          /* first dcm_mix_t */
} dcm_palette_t;             /* 8 bytes */

typedef struct {
    uint16_t joint;
    uint16_t evp_node;       /* inverse-bind matrix index */
    float weight;
} dcm_mix_t;                 /* 8 bytes */

/* material flags */
#define DCM_MF_REPEAT_S 0x0001
#define DCM_MF_REPEAT_T 0x0002
#define DCM_MF_FLIP_S 0x0004
#define DCM_MF_FLIP_T 0x0008
#define DCM_MF_TRANSLUCENT 0x0010 /* alpha < 31 or A3I5/A5I3 texture */
#define DCM_MF_VTXCOLOR 0x0020    /* DS material flag VTXCOLOR (informational) */
#define DCM_MF_ENVMAP 0x0040      /* texgen = normal (sphere map) */
#define DCM_MF_WIREFRAME 0x0080   /* DS alpha 0 = wireframe */
#define DCM_MF_CULL_BACK 0x0100   /* back faces not drawn */
#define DCM_MF_CULL_FRONT 0x0200  /* front faces not drawn */
#define DCM_MF_TEXMTX 0x0400      /* uv affine is not identity */
#define DCM_MF_DECAL 0x0800

typedef struct {
    uint16_t texture;        /* index into textures, 0xFFFF = untextured */
    uint16_t flags;          /* DCM_MF_* */
    uint8_t alpha;           /* 0..31 (DS polygon alpha) */
    uint8_t light_mask;      /* DS lights enabled (bits 0..3) */
    uint8_t poly_mode;       /* 0 modulate, 1 decal, 2 toon/highlight, 3 shadow */
    uint8_t poly_id;
    uint32_t diffuse, ambient, specular, emission; /* 0xAARRGGBB */
    float uv_a, uv_d, uv_e, uv_f;  /* u' = a*u + e, v' = d*v + f (normalised uv) */
} dcm_material_t;            /* 40 bytes */

enum { DCM_TEX_ARGB1555 = 0, DCM_TEX_RGB565 = 1, DCM_TEX_ARGB4444 = 2 };

typedef struct {
    uint16_t width, height;  /* powers of two, 8..1024 */
    uint16_t format;         /* DCM_TEX_* ; pixels are linear (non-twiddled) rows */
    uint16_t flags;
    uint32_t ofs;            /* pixel data offset (32-byte aligned) */
    uint32_t size;           /* width*height*2 */
} dcm_texture_t;             /* 16 bytes */

typedef struct {
    uint16_t vis_node;       /* node whose visibility gates this draw, 0xFFFF = always */
    uint8_t vis_default;     /* visibility when no visibility animation is bound */
    uint8_t material;
    uint16_t shape;          /* DS shape index (informational) */
    uint16_t pad;
    uint32_t first_strip;
    uint32_t num_strips;
} dcm_draw_t;                /* 16 bytes */

enum { DCM_PRIM_TRIS = 0, DCM_PRIM_QUADS = 1, DCM_PRIM_TRISTRIP = 2, DCM_PRIM_QUADSTRIP = 3 };

typedef struct {
    uint32_t first_vertex;
    uint16_t count;          /* vertices in this PVR strip (3 = DS triangle, 4 = DS quad) */
    uint16_t ds_prim;        /* DCM_PRIM_*: the DS primitive the strip came from */
} dcm_strip_t;               /* 8 bytes */

#define DCM_VF_COLOR 1       /* `color` came from a DS COLOR command */
#define DCM_VF_NORMAL 2      /* `n` came from a DS NORMAL command */

typedef struct {
    float x, y, z;           /* palette-entry space (POSSCALE already applied) */
    float u, v;              /* normalised texture coordinates (before material uv affine) */
    int16_t nx, ny, nz;      /* normal, 1.14 fixed point */
    uint8_t palette;         /* matrix palette entry */
    uint8_t flags;           /* DCM_VF_* */
    uint32_t color;          /* 0xAARRGGBB vertex colour */
} dcm_vertex_t;              /* 32 bytes */

/* ------------------------------------------------------------------ .dca / .dcv (animation sets) */

enum { DCA_KIND_JOINT = 1, DCA_KIND_VIS = 2 };

typedef struct {
    uint32_t magic;          /* DCA_MAGIC */
    uint16_t version;
    uint16_t kind;           /* DCA_KIND_* */
    uint32_t file_size;
    uint16_t num_anims;
    uint16_t max_joints;
    uint32_t ofs_anims;      /* dca_anim_t[num_anims] */
} dca_header_t;              /* 20 bytes */

typedef struct {
    char name[16];
    uint16_t num_frames;
    uint16_t num_joints;     /* joint count (kind 1) or node count (kind 2) */
    uint32_t ofs_channels;   /* kind 1: dca_channel_t[num_joints] */
    uint32_t ofs_data;       /* kind 1: sample pool base; kind 2: visibility bits */
    uint32_t pad;
} dca_anim_t;                /* 32 bytes */

enum {
    DCA_MODE_BIND = 0,       /* use the model's bind value */
    DCA_MODE_IDENTITY = 1,   /* zero translation / identity rotation / unit scale */
    DCA_MODE_CONST = 2,      /* one sample */
    DCA_MODE_FRAMES = 3      /* num_frames samples */
};
#define DCA_CF_ROT_MATRIX 1  /* rotation samples are int16[9] (1.14) instead of quaternions */

typedef struct {
    uint8_t t_mode, r_mode, s_mode; /* DCA_MODE_* */
    uint8_t flags;                  /* DCA_CF_* */
    uint32_t t_ofs;                 /* float[3] samples, relative to dca_anim_t.ofs_data */
    uint32_t r_ofs;                 /* int16[4] quaternion (w,x,y,z) 1.14 or int16[9] matrix 1.14 */
    uint32_t s_ofs;                 /* float[3] samples */
} dca_channel_t;             /* 16 bytes */

/* Visibility (kind 2): bit (frame * num_joints + node) of the byte array at ofs_data. */

/* A quaternion q = (w,x,y,z) represents the column-vector rotation Rc; the DS row
 * matrix used by the joint maths is R = transpose(Rc). */

#endif

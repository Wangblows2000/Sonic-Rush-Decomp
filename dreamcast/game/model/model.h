/*
 * model.h - native runtime for converted NitroSystem G3D models (.dcm) and
 * animation sets (.dca joint / .dcv visibility).  See model_format.h and
 * docs/dreamcast/formats/MODELS.md.
 *
 * Typical use (stage player, mirrors Player_Create + StageObj_Draw3DEx):
 *
 *   dcm_model_t son; dca_set_t anims, vis;
 *   dcm_model_load(&son, son_dcm_bytes, size, 1);
 *   dca_set_load(&anims, son_dca_bytes, size);
 *   dca_set_load(&vis, son_dcv_bytes, size);
 *   int run = dca_set_find(&anims, "com_walk6");
 *   ...each frame:
 *   dcm_local_t loc[DCM_MAX_JOINTS];  dcm_mtx_t pal[DCM_MAX_PALETTE];
 *   dca_sample(&anims, run, frame, &son, loc);          // interpolated TRS
 *   dcm_pose(&son, loc, pal);                            // joints -> palette matrices
 *   uint8_t visbits[DCM_MAX_JOINTS]; dca_visibility(&vis, 1, frame, &son, visbits);
 *   dcm_camera_t cam; dcm_camera_stage(&cam);            // BossCamera fixed camera
 *   float tx = sx, ty = -sy;                             // DS screen pixels of the player
 *   dcm_camera_stage_offaxis(&cam, &tx, &ty);            // StageObj_Draw3DEx flag 0x200
 *   dcm_mtx_t mdl; dcm_stage_model_matrix(&mdl, 0x34cc/4096.f, 0x4000, 0, tx, ty, 0);
 *   dcm_draw(&son, pal, &mdl, &cam, visbits, NULL, PVR_LIST_OP_POLY);  // + PT / TR lists
 */
#ifndef DC_MODEL_H
#define DC_MODEL_H

#include <stddef.h>
#include <stdint.h>

#include "model_format.h"

#define DCM_MAX_JOINTS 128
#define DCM_MAX_PALETTE 256

/* Row-vector affine matrix: v' = [x y z 1] * m (row 3 = translation). */
typedef struct {
    float m[4][3];
} dcm_mtx_t;

typedef struct {
    float t[3];
    float r[9]; /* row-major DS rotation rows */
    float s[3];
} dcm_local_t;

typedef struct {
    const uint8_t *base;          /* package bytes (kept by the caller) */
    const dcm_header_t *hdr;
    const dcm_joint_t *joints;
    const uint16_t *order;
    const dcm_palette_t *palette;
    const dcm_mix_t *mix;
    const float *evp;             /* [num_joints][12] or NULL */
    const dcm_material_t *materials;
    const dcm_texture_t *textures;
    const dcm_draw_t *draws;
    const dcm_strip_t *strips;
    const dcm_vertex_t *vertices;
    const dcm_matx_t *matx;       /* version 2 packages: material names + texture SRT inputs, else NULL */
    const dcm_texx_t *texx;       /* version 2: texture / palette names, else NULL */
    void *tex_vram[64];           /* pvr_ptr_t per texture (Dreamcast) */
    uint8_t tex_class[64];        /* DCM_CLASS_* derived from texture contents */
} dcm_model_t;

typedef struct {
    const uint8_t *base;
    const dca_header_t *hdr;
    const dca_anim_t *anims;
} dca_set_t;

/* Camera: row-vector view (world->eye) and projection, plus the viewport. */
typedef struct {
    dcm_mtx_t view;
    float proj[4][4];
    float vp_x, vp_y, vp_w, vp_h; /* screen rectangle (640x480 for the full DS screen x2.5) */
    float near_w;                 /* vertices with clip w below this are rejected */
} dcm_camera_t;

typedef struct {
    int count;                    /* 0..4 */
    float dir[4][3];              /* view-space direction (DS LIGHT_VECTOR, not normalised) */
    float col[4][3];              /* 0..1 */
} dcm_lights_t;

enum { DCM_CLASS_OP = 0, DCM_CLASS_PT = 1, DCM_CLASS_TR = 2 };

typedef struct {
    int list;                     /* pvr_list_t being submitted */
    int pt_list;                  /* list that receives punch-through draws (PT or TR) */
    uint8_t alpha_scale;          /* 0..255 multiplies material alpha (fades) */
} dcm_draw_opts_t;

/* ---- loading */
int dcm_model_load(dcm_model_t *m, const void *data, size_t size, int upload_textures);
void dcm_model_free(dcm_model_t *m);
int dca_set_load(dca_set_t *s, const void *data, size_t size);
int dca_set_find(const dca_set_t *s, const char *name);   /* -1 if missing */

/* ---- animation / pose */
void dcm_bind_locals(const dcm_model_t *m, dcm_local_t *out);
/* Interpolated joint TRS at `frame` (0..num_frames-1, fractional allowed; clamped).
 * Joints the animation does not cover keep the bind pose. */
void dca_sample(const dca_set_t *s, int anim, float frame, const dcm_model_t *m, dcm_local_t *out);
/* Per-joint visibility (1 = visible) from a visibility set; anim < 0 -> SBC defaults (all 1). */
void dca_visibility(const dca_set_t *s, int anim, float frame, const dcm_model_t *m, uint8_t *vis);
/* Joint world matrices (Si3d / basic scaling rule) then palette matrices. */
void dcm_pose(const dcm_model_t *m, const dcm_local_t *locals, dcm_mtx_t *palette_out);
void dcm_world(const dcm_model_t *m, const dcm_local_t *locals, dcm_mtx_t *world_out);

/* ---- cameras */
/* Stage 3D camera (BossCamera_Create + BossCamera_UpdateFixed): perspective half-fovy 0x1555,
 * aspect 0x1555, near 1, far 2048, eye (128,-96,96*cot) -> (128,-96,0); world units = DS pixels
 * (x right, y up = -screen y).  Viewport 640x480 (DS 256x192 x2.5). */
void dcm_camera_stage(dcm_camera_t *c);
/* StageObj_Draw3DEx flag 0x200 (players): off-axis frustum so the model is lit/projected as if
 * centred while appearing at (*tx,*ty); rewrites *tx,*ty to (128,-96) like the game. */
void dcm_camera_stage_offaxis(dcm_camera_t *c, float *tx, float *ty);
void dcm_camera_ortho(dcm_camera_t *c, float t, float b, float l, float r, float n, float f);
void dcm_camera_set_viewport(dcm_camera_t *c, float x, float y, float w, float h);

/* Model matrix as StageObj_Draw3DEx/Model_Draw build it: S(scale) * RotY(facing) * RotZ(-rotz<<8)
 * then translation.  facing: 0x4000 = right, 0xC000 = left (DS 16-bit angle). */
void dcm_stage_model_matrix(dcm_mtx_t *out, float scale, uint16_t facing, uint8_t rotz, float tx, float ty,
                            float tz);
void dcm_mtx_identity(dcm_mtx_t *m);
void dcm_mtx_mul(dcm_mtx_t *out, const dcm_mtx_t *a, const dcm_mtx_t *b); /* out = a * b */

/* Default stage lights (Stage_Create / StageTitleCardPlayer_Draw vectors, facing right). */
void dcm_lights_stage(dcm_lights_t *l, int facing_left);

/* ---- material animations (.dcc colour, .dcp texture pattern, .dct texture SRT; dca_set_load loads them)
 * The per-draw material values NNSi_G3dFuncSbc_MAT_InternalDefault builds (NNSG3dMatAnmResult):
 * dcm_matstate_init fills them from the static materials, dcm_matanim_apply runs one animation at an
 * integer frame (the NNS objects use frame >> 12) over them, dcm_draw_ex draws with them. */
typedef struct {
    uint32_t diffuse, ambient, specular, emission; /* 0xAARRGGBB */
    uint16_t texture;             /* dcm texture index, 0xFFFF = untextured */
    uint8_t alpha;                /* 0..31; 0 = the material's shapes are not drawn (NNS rs->flag 2) */
    uint8_t srt_flags;            /* DCM_SRT_* */
    uint8_t texgen;               /* TEXIMAGE_PARAM >> 30 (a texture SRT animation sets 1) */
    uint8_t animated;             /* bit 0 colour, 1 pattern, 2 SRT */
    uint16_t orig_w, orig_h;
    int32_t scale_s, scale_t, trans_s, trans_t, mag_w, mag_h; /* fx32 */
} dcm_matstate_t;
void dcm_matstate_init(const dcm_model_t *m, dcm_matstate_t *st);   /* st[num_materials] */
/* Animation `anim` of a material-animation set at `frame` (clamped to 0..num_frames-1).  Returns the
 * number of materials it bound (0 for a joint / visibility set or a model without names). */
int dcm_matanim_apply(const dca_set_t *s, int anim, int frame, const dcm_model_t *m, dcm_matstate_t *st);
/* Texture affine of a material state (NNSi_G3dSendTexSRTSi3d, uv normalised to the bound texture):
 * u' = a * u + e, v' = d * v + f; returns 0 when no texture matrix applies. */
int dcm_matstate_uv(const dcm_model_t *m, int material, const dcm_matstate_t *st, float *a, float *d, float *e,
                    float *f);

/* ---- drawing */
/* Transform (SH-4) + submit the draws whose class matches opts->list; returns PVR triangles sent.
 * vis may be NULL (SBC defaults), lights NULL (stage defaults). */
int dcm_draw(const dcm_model_t *m, const dcm_mtx_t *palette, const dcm_mtx_t *model_mtx, const dcm_camera_t *cam,
             const uint8_t *vis, const dcm_lights_t *lights, const dcm_draw_opts_t *opts);
/* Same with animated material states (st[num_materials] from dcm_matstate_init / dcm_matanim_apply;
 * NULL = the static materials).  Strips crossing the camera's near plane (clip w < near_w) are
 * clipped there (one PVR strip per clipped triangle), not dropped. */
int dcm_draw_ex(const dcm_model_t *m, const dcm_mtx_t *palette, const dcm_mtx_t *model_mtx, const dcm_camera_t *cam,
                const uint8_t *vis, const dcm_lights_t *lights, const dcm_draw_opts_t *opts,
                const dcm_matstate_t *st);
/* Host tests: every vertex dcm_draw / dcm_draw_ex would submit (screen x, y, z, u, v, argb, end-of-strip),
 * with the material index; NULL = off.  Without _arch_dreamcast nothing else is drawn. */
typedef void (*dcm_emit_hook_t)(int material, const float *xyzuv, uint32_t argb, int eol);
extern dcm_emit_hook_t dcm_host_emit;

/* Host/test hook: transformed screen position of every vertex (x, y, 1/w), no PVR. */
int dcm_transform_vertices(const dcm_model_t *m, const dcm_mtx_t *palette, const dcm_mtx_t *model_mtx,
                           const dcm_camera_t *cam, float *out_xyz);

#endif

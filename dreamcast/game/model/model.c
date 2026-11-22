/*
 * model.c - native runtime for converted NitroSystem G3D models.
 *
 * Joint maths follows the NNS code the DS game runs (src/libs/NitroSystem/g3d):
 *   - local = S * R * T (NNSi_G3dSendJointSRTBasic), world = local * parent;
 *   - Si3d scaling rule (NNSi_G3dGetJointScaleSi3d / NNSi_G3dSendJointSRTSi3d): once a
 *     parent carries a non-unit accumulated scale A, the child is
 *     S_own * S_A * R * T(t*A) * S_A^-1 * parent and its accumulated scale is s*A;
 *   - NODEMIX palette entries: sum(w * evp[node] * world[joint]).
 * Animation sampling interpolates between the per-frame samples the converter
 * resampled with the NNS integer-frame rules (nlerp for quaternions, lerp + row
 * normalisation for matrices like getRotDataEx_).
 *
 * Vertices are transformed on the SH-4 (one 4x4 per palette entry, combined
 * palette * model * view * projection) and submitted with pvr_prim as PVR strips;
 * strips that cross the near plane are clipped there triangle by triangle.
 * Material animations (.dcc / .dcp / .dct: NNSi_G3dAnmCalcNsBma / Btp / Bta,
 * values precomputed per integer frame by the converter) override the static
 * material values per draw (dcm_matstate_t, dcm_draw_ex).
 * Build: kos-cc -c model.c.  Without _arch_dreamcast the PVR parts compile out
 * (host tests use dcm_transform_vertices).
 */
#include "model.h"

#include <math.h>
#include <string.h>

#ifdef _arch_dreamcast
#include <dc/pvr.h>
#endif

/* ------------------------------------------------------------------ matrices */

void dcm_mtx_identity(dcm_mtx_t *m) {
    memset(m, 0, sizeof(*m));
    m->m[0][0] = m->m[1][1] = m->m[2][2] = 1.0f;
}

void dcm_mtx_mul(dcm_mtx_t *out, const dcm_mtx_t *a, const dcm_mtx_t *b) {
    dcm_mtx_t r;
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            float v = a->m[i][0] * b->m[0][j] + a->m[i][1] * b->m[1][j] + a->m[i][2] * b->m[2][j];
            if (i == 3)
                v += b->m[3][j];
            r.m[i][j] = v;
        }
    }
    *out = r;
}

static void mtx_from_local(dcm_mtx_t *m, const float *r, const float *t, const float *s) {
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            m->m[i][j] = s[i] * r[i * 3 + j];
    m->m[3][0] = t[0];
    m->m[3][1] = t[1];
    m->m[3][2] = t[2];
}

/* ------------------------------------------------------------------ loading */

#define AT(T, o) ((const T *)(m->base + (o)))

int dcm_model_load(dcm_model_t *m, const void *data, size_t size, int upload_textures) {
    const dcm_header_t *h = (const dcm_header_t *)data;
    uint32_t i;
    memset(m, 0, sizeof(*m));
    if (size < DCM_HEADER_SIZE_V1 || h->magic != DCM_MAGIC || h->file_size > size ||
        !((h->version == 1 && h->header_size == DCM_HEADER_SIZE_V1) ||
          (h->version == DCM_VERSION && h->header_size == sizeof(dcm_header_t))))
        return -1;
    if (h->num_joints > DCM_MAX_JOINTS || h->num_palette > DCM_MAX_PALETTE || h->num_textures > 64)
        return -2;
    m->base = (const uint8_t *)data;
    m->hdr = h;
    m->joints = AT(dcm_joint_t, h->ofs_joints);
    m->order = AT(uint16_t, h->ofs_order);
    m->palette = AT(dcm_palette_t, h->ofs_palette);
    m->mix = AT(dcm_mix_t, h->ofs_mix);
    m->evp = h->ofs_evp ? AT(float, h->ofs_evp) : NULL;
    m->materials = AT(dcm_material_t, h->ofs_materials);
    m->textures = AT(dcm_texture_t, h->ofs_textures);
    m->draws = AT(dcm_draw_t, h->ofs_draws);
    m->strips = AT(dcm_strip_t, h->ofs_strips);
    m->vertices = AT(dcm_vertex_t, h->ofs_vertices);
    if (h->version >= 2 && h->ofs_matx && h->ofs_texx) {
        m->matx = AT(dcm_matx_t, h->ofs_matx);
        m->texx = AT(dcm_texx_t, h->ofs_texx);
    }
    for (i = 0; i < h->num_textures; i++) {
        const dcm_texture_t *t = &m->textures[i];
        m->tex_class[i] = t->format == DCM_TEX_ARGB4444 ? DCM_CLASS_TR : (t->flags & 1) ? DCM_CLASS_PT : DCM_CLASS_OP;
#ifdef _arch_dreamcast
        if (upload_textures) {
            pvr_ptr_t p = pvr_mem_malloc(t->size);
            if (!p)
                return -3;
            pvr_txr_load(m->base + t->ofs, p, t->size);
            m->tex_vram[i] = (void *)p;
        }
#else
        (void)upload_textures;
#endif
    }
    return 0;
}

void dcm_model_free(dcm_model_t *m) {
#ifdef _arch_dreamcast
    uint32_t i;
    for (i = 0; m->hdr && i < m->hdr->num_textures; i++)
        if (m->tex_vram[i])
            pvr_mem_free((pvr_ptr_t)m->tex_vram[i]);
#endif
    memset(m, 0, sizeof(*m));
}

int dca_set_load(dca_set_t *s, const void *data, size_t size) {
    const dca_header_t *h = (const dca_header_t *)data;
    memset(s, 0, sizeof(*s));
    if (size < sizeof(dca_header_t) || h->magic != DCA_MAGIC || h->version != DCA_VERSION || h->file_size > size)
        return -1;
    s->base = (const uint8_t *)data;
    s->hdr = h;
    s->anims = (const dca_anim_t *)(s->base + h->ofs_anims);
    return 0;
}

int dca_set_find(const dca_set_t *s, const char *name) {
    int i;
    for (i = 0; i < s->hdr->num_anims; i++)
        if (strncmp(s->anims[i].name, name, 16) == 0)
            return i;
    return -1;
}

/* ------------------------------------------------------------------ animation */

void dcm_bind_locals(const dcm_model_t *m, dcm_local_t *out) {
    int j;
    for (j = 0; j < m->hdr->num_joints; j++) {
        memcpy(out[j].t, m->joints[j].t, sizeof(out[j].t));
        memcpy(out[j].r, m->joints[j].r, sizeof(out[j].r));
        memcpy(out[j].s, m->joints[j].s, sizeof(out[j].s));
    }
}

static void quat_to_rows(const float *q, float *r) {
    /* q represents the column rotation Rc; DS row matrix R = transpose(Rc) */
    float w = q[0], x = q[1], y = q[2], z = q[3];
    r[0] = 1 - 2 * (y * y + z * z);
    r[3] = 2 * (x * y - z * w);
    r[6] = 2 * (x * z + y * w);
    r[1] = 2 * (x * y + z * w);
    r[4] = 1 - 2 * (x * x + z * z);
    r[7] = 2 * (y * z - x * w);
    r[2] = 2 * (x * z - y * w);
    r[5] = 2 * (y * z + x * w);
    r[8] = 1 - 2 * (x * x + y * y);
}

static void norm3(float *v) {
    float l = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (l > 0.0f) {
        l = 1.0f / l;
        v[0] *= l;
        v[1] *= l;
        v[2] *= l;
    }
}

static void lerp3(const uint8_t *pool, uint32_t ofs, int mode, int f0, int f1, float w, float *out) {
    const float *a, *b;
    if (mode == DCA_MODE_CONST) {
        f0 = f1 = 0;
    }
    a = (const float *)(pool + ofs) + 3 * f0;
    b = (const float *)(pool + ofs) + 3 * f1;
    out[0] = a[0] + (b[0] - a[0]) * w;
    out[1] = a[1] + (b[1] - a[1]) * w;
    out[2] = a[2] + (b[2] - a[2]) * w;
}

void dca_sample(const dca_set_t *s, int anim, float frame, const dcm_model_t *m, dcm_local_t *out) {
    const dca_anim_t *a;
    const dca_channel_t *ch;
    const uint8_t *pool;
    int nf, f0, f1, j, nj;
    float w;
    dcm_bind_locals(m, out);
    if (!s || anim < 0 || anim >= s->hdr->num_anims)
        return;
    a = &s->anims[anim];
    ch = (const dca_channel_t *)(s->base + a->ofs_channels);
    pool = s->base + a->ofs_data;
    nf = a->num_frames;
    if (frame < 0.0f)
        frame = 0.0f;
    if (frame > (float)(nf - 1))
        frame = (float)(nf - 1);
    f0 = (int)frame;
    f1 = f0 + 1 < nf ? f0 + 1 : f0;
    w = frame - (float)f0;
    nj = a->num_joints < m->hdr->num_joints ? a->num_joints : m->hdr->num_joints;
    for (j = 0; j < nj; j++) {
        const dca_channel_t *c = &ch[j];
        dcm_local_t *o = &out[j];
        if (c->t_mode == DCA_MODE_IDENTITY)
            o->t[0] = o->t[1] = o->t[2] = 0.0f;
        else if (c->t_mode >= DCA_MODE_CONST)
            lerp3(pool, c->t_ofs, c->t_mode, f0, f1, w, o->t);
        if (c->s_mode == DCA_MODE_IDENTITY)
            o->s[0] = o->s[1] = o->s[2] = 1.0f;
        else if (c->s_mode >= DCA_MODE_CONST)
            lerp3(pool, c->s_ofs, c->s_mode, f0, f1, w, o->s);
        if (c->r_mode == DCA_MODE_IDENTITY) {
            static const float id[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
            memcpy(o->r, id, sizeof(id));
        } else if (c->r_mode >= DCA_MODE_CONST) {
            int g0 = c->r_mode == DCA_MODE_CONST ? 0 : f0;
            int g1 = c->r_mode == DCA_MODE_CONST ? 0 : f1;
            int k;
            if (c->flags & DCA_CF_ROT_MATRIX) {
                const int16_t *ra = (const int16_t *)(pool + c->r_ofs) + 9 * g0;
                const int16_t *rb = (const int16_t *)(pool + c->r_ofs) + 9 * g1;
                for (k = 0; k < 9; k++)
                    o->r[k] = ((float)ra[k] + (float)(rb[k] - ra[k]) * w) * (1.0f / 16384.0f);
                if (w > 0.0f) { /* exact samples are kept as NNS produced them */
                    norm3(&o->r[0]);
                    norm3(&o->r[3]);
                    norm3(&o->r[6]);
                }
            } else {
                const int16_t *qa = (const int16_t *)(pool + c->r_ofs) + 4 * g0;
                const int16_t *qb = (const int16_t *)(pool + c->r_ofs) + 4 * g1;
                float q[4], l;
                for (k = 0; k < 4; k++)
                    q[k] = (float)qa[k] + (float)(qb[k] - qa[k]) * w; /* converter keeps neighbours in one hemisphere */
                l = sqrtf(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
                if (l > 0.0f) {
                    l = 1.0f / l;
                    for (k = 0; k < 4; k++)
                        q[k] *= l;
                }
                quat_to_rows(q, o->r);
            }
        }
    }
}

void dca_visibility(const dca_set_t *s, int anim, float frame, const dcm_model_t *m, uint8_t *vis) {
    int n, nn, f;
    const dca_anim_t *a;
    memset(vis, 1, m->hdr->num_joints);
    if (!s || anim < 0 || anim >= s->hdr->num_anims || s->hdr->kind != DCA_KIND_VIS)
        return;
    a = &s->anims[anim];
    nn = a->num_joints < m->hdr->num_joints ? a->num_joints : m->hdr->num_joints;
    f = (int)frame;
    if (f < 0)
        f = 0;
    if (f >= a->num_frames)
        f = a->num_frames - 1;
    for (n = 0; n < nn; n++) {
        uint32_t b = (uint32_t)f * a->num_joints + (uint32_t)n;
        vis[n] = (s->base[a->ofs_data + (b >> 3)] >> (b & 7)) & 1;
    }
}

/* ------------------------------------------------------------------ pose */

void dcm_world(const dcm_model_t *m, const dcm_local_t *locals, dcm_mtx_t *world) {
    float acc[DCM_MAX_JOINTS][3];
    uint8_t one[DCM_MAX_JOINTS];
    int i, si3d = m->hdr->scaling_rule == 2;
    for (i = 0; i < m->hdr->num_joints; i++) {
        int j = m->order[i];
        int p = m->joints[j].parent;
        const dcm_local_t *l = &locals[j];
        dcm_mtx_t loc;
        int p_one = p >= 0 ? one[p] : 1;
        static const float unit[3] = {1.0f, 1.0f, 1.0f};
        const float *pa = p >= 0 ? acc[p] : unit;
        if (si3d && !p_one) {
            /* S_own * S_A * R * T(t*A) * S_A^-1 */
            float t[3], s[3];
            int r, c;
            t[0] = l->t[0] * pa[0];
            t[1] = l->t[1] * pa[1];
            t[2] = l->t[2] * pa[2];
            s[0] = l->s[0] * pa[0];
            s[1] = l->s[1] * pa[1];
            s[2] = l->s[2] * pa[2];
            mtx_from_local(&loc, l->r, t, s);
            for (r = 0; r < 4; r++)
                for (c = 0; c < 3; c++)
                    loc.m[r][c] /= pa[c];
        } else {
            mtx_from_local(&loc, l->r, l->t, l->s);
        }
        if (p >= 0)
            dcm_mtx_mul(&world[j], &loc, &world[p]);
        else
            world[j] = loc;
        if (si3d) {
            int unit_s = l->s[0] == 1.0f && l->s[1] == 1.0f && l->s[2] == 1.0f;
            one[j] = p_one && unit_s;
            acc[j][0] = l->s[0] * pa[0];
            acc[j][1] = l->s[1] * pa[1];
            acc[j][2] = l->s[2] * pa[2];
        } else {
            one[j] = 1;
        }
    }
}

void dcm_pose(const dcm_model_t *m, const dcm_local_t *locals, dcm_mtx_t *pal) {
    dcm_mtx_t world[DCM_MAX_JOINTS];
    int i;
    dcm_world(m, locals, world);
    for (i = 0; i < m->hdr->num_palette; i++) {
        const dcm_palette_t *p = &m->palette[i];
        switch (p->kind) {
        case DCM_PAL_JOINT:
            pal[i] = world[p->joint];
            break;
        case DCM_PAL_MIX: {
            int k, r, c;
            memset(&pal[i], 0, sizeof(pal[i]));
            for (k = 0; k < p->num; k++) {
                const dcm_mix_t *x = &m->mix[p->first + k];
                dcm_mtx_t e, t;
                const float *ev = m->evp + 12 * x->evp_node;
                for (r = 0; r < 4; r++)
                    for (c = 0; c < 3; c++)
                        e.m[r][c] = ev[r * 3 + c];
                dcm_mtx_mul(&t, &e, &world[x->joint]);
                for (r = 0; r < 4; r++)
                    for (c = 0; c < 3; c++)
                        pal[i].m[r][c] += x->weight * t.m[r][c];
            }
            break;
        }
        case DCM_PAL_BB:
        case DCM_PAL_BBY:
            pal[i] = pal[p->joint]; /* billboard applied after the view transform in dcm_draw */
            break;
        default:
            dcm_mtx_identity(&pal[i]);
            break;
        }
    }
}

/* ------------------------------------------------------------------ cameras */

#define DS_ANGLE(a) ((float)(a) * (6.28318530718f / 65536.0f))

static void cam_defaults(dcm_camera_t *c) {
    memset(c, 0, sizeof(*c));
    dcm_mtx_identity(&c->view);
    c->vp_x = 0.0f;
    c->vp_y = 0.0f;
    c->vp_w = 640.0f;
    c->vp_h = 480.0f;
    c->near_w = 0.5f;
}

static void proj_frustum(dcm_camera_t *c, float t, float b, float l, float r, float n, float f) {
    memset(c->proj, 0, sizeof(c->proj));
    c->proj[0][0] = 2 * n / (r - l);
    c->proj[1][1] = 2 * n / (t - b);
    c->proj[2][0] = (r + l) / (r - l);
    c->proj[2][1] = (t + b) / (t - b);
    c->proj[2][2] = (f + n) / (n - f);
    c->proj[2][3] = -1.0f;
    c->proj[3][2] = 2 * f * n / (n - f);
}

/* BossCamera_Create values (installed by Stage_Create as data_022c4ef4 for every act). */
#define STAGE_FOVY 0x1555
#define STAGE_ASPECT (0x1555 / 4096.0f)
#define STAGE_NEAR 1.0f
#define STAGE_FAR 2048.0f

void dcm_camera_stage(dcm_camera_t *c) {
    float a = DS_ANGLE(STAGE_FOVY), h, w, dist;
    cam_defaults(c);
    h = tanf(a) * STAGE_NEAR;
    w = h * STAGE_ASPECT;
    proj_frustum(c, h, -h, -w, w, STAGE_NEAR, STAGE_FAR);
    dist = 96.0f * cosf(a) / sinf(a); /* BossCamera_UpdateFixed: z = cot * 0x60 */
    c->view.m[3][0] = -128.0f;
    c->view.m[3][1] = 96.0f;
    c->view.m[3][2] = -dist;
}

void dcm_camera_stage_offaxis(dcm_camera_t *c, float *tx, float *ty) {
    float a = DS_ANGLE(STAGE_FOVY);
    float h = tanf(a) * STAGE_NEAR;
    float w = h * STAGE_ASPECT;
    float dist = 96.0f * cosf(a) / sinf(a);
    float d = STAGE_NEAR / dist;
    float ao = d * (*ty + 96.0f);
    float bo = d * (128.0f - *tx);
    proj_frustum(c, h - ao, -(h + ao), bo - w, w + bo, STAGE_NEAR, STAGE_FAR);
    *tx = 128.0f;
    *ty = -96.0f;
}

void dcm_camera_ortho(dcm_camera_t *c, float t, float b, float l, float r, float n, float f) {
    cam_defaults(c);
    c->proj[0][0] = 2.0f / (r - l);
    c->proj[1][1] = 2.0f / (t - b);
    c->proj[2][2] = -2.0f / (f - n);
    c->proj[3][0] = -(r + l) / (r - l);
    c->proj[3][1] = -(t + b) / (t - b);
    c->proj[3][2] = -(f + n) / (f - n);
    c->proj[3][3] = 1.0f;
    c->near_w = 0.0f;
}

void dcm_camera_set_viewport(dcm_camera_t *c, float x, float y, float w, float h) {
    c->vp_x = x;
    c->vp_y = y;
    c->vp_w = w;
    c->vp_h = h;
}

void dcm_stage_model_matrix(dcm_mtx_t *out, float scale, uint16_t facing, uint8_t rotz, float tx, float ty,
                            float tz) {
    /* StageObj_Draw3DEx: rot = RotY(facing) * RotZ(-(rotz << 8)); Model_Draw: v * S * [rot | trans] */
    float sy = sinf(DS_ANGLE(facing)), cy = cosf(DS_ANGLE(facing));
    float az = DS_ANGLE((uint16_t)(-(rotz << 8)));
    float sz = sinf(az), cz = cosf(az);
    dcm_mtx_t ry, rz;
    dcm_mtx_identity(&ry);
    ry.m[0][0] = cy;
    ry.m[0][2] = -sy;
    ry.m[2][0] = sy;
    ry.m[2][2] = cy;
    dcm_mtx_identity(&rz);
    rz.m[0][0] = cz;
    rz.m[0][1] = sz;
    rz.m[1][0] = -sz;
    rz.m[1][1] = cz;
    dcm_mtx_mul(out, &ry, &rz);
    {
        int r, c;
        for (r = 0; r < 3; r++)
            for (c = 0; c < 3; c++)
                out->m[r][c] *= scale;
    }
    out->m[3][0] = tx;
    out->m[3][1] = ty;
    out->m[3][2] = tz;
}

void dcm_lights_stage(dcm_lights_t *l, int facing_left) {
    /* Stage_Create default vectors / StageTitleCardPlayer_Draw (x mirrored when facing left). */
    static const float v[3][3] = {{-0xb50, -0xb50, 0}, {0, -0x617, -0xecd}, {-0xec7, 0x620, 0}};
    int i;
    memset(l, 0, sizeof(*l));
    l->count = 3;
    for (i = 0; i < 3; i++) {
        l->dir[i][0] = (facing_left ? -v[i][0] : v[i][0]) / 4096.0f;
        l->dir[i][1] = v[i][1] / 4096.0f;
        l->dir[i][2] = v[i][2] / 4096.0f;
        l->col[i][0] = l->col[i][1] = l->col[i][2] = i == 0 ? 1.0f : 0.5f;
    }
}

/* ------------------------------------------------------------------ transform + draw */

typedef struct {
    float c[4][4];   /* palette * model * view * proj */
    float n[3][3];   /* normal matrix (palette * model * view, rows) */
} xform_t;

static void build_xforms(const dcm_model_t *m, const dcm_mtx_t *pal, const dcm_mtx_t *model_mtx,
                         const dcm_camera_t *cam, xform_t *xf) {
    int i, r, c, k;
    for (i = 0; i < m->hdr->num_palette; i++) {
        dcm_mtx_t mv, t;
        const dcm_palette_t *p = &m->palette[i];
        dcm_mtx_mul(&t, &pal[i], model_mtx);
        dcm_mtx_mul(&mv, &t, &cam->view);
        if (p->kind == DCM_PAL_BB || p->kind == DCM_PAL_BBY) {
            /* NNSi_G3dFuncSbc_BB: keep translation and axis lengths, face the camera */
            float len[3];
            for (r = 0; r < 3; r++)
                len[r] = sqrtf(mv.m[r][0] * mv.m[r][0] + mv.m[r][1] * mv.m[r][1] + mv.m[r][2] * mv.m[r][2]);
            for (r = 0; r < 3; r++) {
                if (p->kind == DCM_PAL_BBY && r == 1)
                    continue;
                for (c = 0; c < 3; c++)
                    mv.m[r][c] = r == c ? len[r] : 0.0f;
            }
        }
        for (r = 0; r < 4; r++)
            for (c = 0; c < 4; c++) {
                float v = 0.0f;
                for (k = 0; k < 3; k++)
                    v += mv.m[r][k] * cam->proj[k][c];
                if (r == 3)
                    v += cam->proj[3][c];
                xf[i].c[r][c] = v;
            }
        for (r = 0; r < 3; r++)
            for (c = 0; c < 3; c++)
                xf[i].n[r][c] = mv.m[r][c];
    }
}

/* clip-space position (x, y, z, w) of a vertex */
static inline void xform_clip(const xform_t *x, const dcm_vertex_t *v, float *c) {
    c[0] = v->x * x->c[0][0] + v->y * x->c[1][0] + v->z * x->c[2][0] + x->c[3][0];
    c[1] = v->x * x->c[0][1] + v->y * x->c[1][1] + v->z * x->c[2][1] + x->c[3][1];
    c[2] = v->x * x->c[0][2] + v->y * x->c[1][2] + v->z * x->c[2][2] + x->c[3][2];
    c[3] = v->x * x->c[0][3] + v->y * x->c[1][3] + v->z * x->c[2][3] + x->c[3][3];
}

/* clip space -> screen x, y and PVR depth */
static inline void project(const float *c, const dcm_camera_t *cam, float *out) {
    float iw = c[3] != 0.0f ? 1.0f / c[3] : 0.0f;
    out[0] = cam->vp_x + (c[0] * iw + 1.0f) * 0.5f * cam->vp_w;
    out[1] = cam->vp_y + (1.0f - c[1] * iw) * 0.5f * cam->vp_h;
    /* PVR depth: larger = nearer.  Perspective: 1/w; orthographic (w == 1): map ndc z. */
    out[2] = cam->near_w > 0.0f ? iw : 1.0f / (c[2] + 2.0f);
}

static inline void xform_vertex(const xform_t *x, const dcm_vertex_t *v, const dcm_camera_t *cam, float *out) {
    float c[4];
    xform_clip(x, v, c);
    project(c, cam, out);
    out[3] = c[3];
}

int dcm_transform_vertices(const dcm_model_t *m, const dcm_mtx_t *pal, const dcm_mtx_t *model_mtx,
                           const dcm_camera_t *cam, float *out_xyz) {
    static xform_t xf[DCM_MAX_PALETTE];
    uint32_t i;
    float o[4];
    build_xforms(m, pal, model_mtx, cam, xf);
    for (i = 0; i < m->hdr->num_vertices; i++) {
        const dcm_vertex_t *v = &m->vertices[i];
        xform_vertex(&xf[v->palette], v, cam, o);
        out_xyz[3 * i + 0] = o[0];
        out_xyz[3 * i + 1] = o[1];
        out_xyz[3 * i + 2] = o[2];
    }
    return (int)m->hdr->num_vertices;
}

static float chan(uint32_t c, int sh) { return (float)((c >> sh) & 255) * (1.0f / 255.0f); }

/* ------------------------------------------------------------------ material animations */

static uint32_t gxrgb_argb(uint32_t c) {
    uint32_t r = c & 31, g = (c >> 5) & 31, b = (c >> 10) & 31;
    r = (r << 3) | (r >> 2);
    g = (g << 3) | (g >> 2);
    b = (b << 3) | (b >> 2);
    return 0xFF000000u | (r << 16) | (g << 8) | b;
}

static void mat_static(const dcm_model_t *m, int i, dcm_matstate_t *st) {
    const dcm_material_t *mat = &m->materials[i];
    memset(st, 0, sizeof(*st));
    st->diffuse = mat->diffuse;
    st->ambient = mat->ambient;
    st->specular = mat->specular;
    st->emission = mat->emission;
    st->texture = mat->texture;
    st->alpha = mat->alpha;
    st->mag_w = st->mag_h = 0x1000;
    st->scale_s = st->scale_t = 0x1000;
    if (m->matx) {
        const dcm_matx_t *x = &m->matx[i];
        st->srt_flags = x->srt_flags;
        st->texgen = x->texgen;
        st->orig_w = x->orig_w;
        st->orig_h = x->orig_h;
        st->scale_s = x->scale_s;
        st->scale_t = x->scale_t;
        st->trans_s = x->trans_s;
        st->trans_t = x->trans_t;
        st->mag_w = x->mag_w;
        st->mag_h = x->mag_h;
    } else {
        st->texgen = (mat->flags & DCM_MF_ENVMAP) ? 2 : 1;
    }
}

void dcm_matstate_init(const dcm_model_t *m, dcm_matstate_t *st) {
    int i;
    for (i = 0; i < m->hdr->num_materials; i++)
        mat_static(m, i, &st[i]);
}

static int find_material(const dcm_model_t *m, const char *name) {
    int i;
    for (i = 0; i < m->hdr->num_materials; i++)
        if (strncmp(m->matx[i].name, name, 16) == 0)
            return i;
    return -1;
}

static int32_t chan_sample(const dcx_chan_t *c, const uint8_t *pool, int kind, int f) {
    if (c->mode == DCA_MODE_FRAMES)
        return kind == DCA_KIND_TEXSRT ? ((const int32_t *)(pool + c->ofs))[f] : ((const uint16_t *)(pool + c->ofs))[f];
    return c->value;
}

int dcm_matanim_apply(const dca_set_t *s, int anim, int frame, const dcm_model_t *m, dcm_matstate_t *st) {
    const dca_anim_t *a;
    const dcx_mat_t *mats;
    const uint8_t *pool;
    int kind, k, n = 0;
    if (!s || !s->hdr || !m->matx || anim < 0 || anim >= s->hdr->num_anims)
        return 0;
    kind = s->hdr->kind;
    if (kind < DCA_KIND_MATCOL || kind > DCA_KIND_TEXSRT)
        return 0;
    a = &s->anims[anim];
    if (frame >= a->num_frames)
        frame = a->num_frames - 1;
    if (frame < 0)
        frame = 0;
    mats = (const dcx_mat_t *)(s->base + a->ofs_channels);
    pool = s->base + a->ofs_data;
    for (k = 0; k < a->num_joints; k++) {
        const dcx_mat_t *x = &mats[k];
        int mi = find_material(m, x->name);
        dcm_matstate_t *o;
        if (mi < 0)
            continue;
        o = &st[mi];
        n++;
        if (kind == DCA_KIND_MATCOL) {
            /* NNSi_G3dAnmCalcNsBma: diffuse / ambient, specular / emission, polygon alpha */
            o->diffuse = gxrgb_argb((uint32_t)chan_sample(&x->ch[0], pool, kind, frame));
            o->ambient = gxrgb_argb((uint32_t)chan_sample(&x->ch[1], pool, kind, frame));
            o->specular = gxrgb_argb((uint32_t)chan_sample(&x->ch[2], pool, kind, frame));
            o->emission = gxrgb_argb((uint32_t)chan_sample(&x->ch[3], pool, kind, frame));
            o->alpha = (uint8_t)(chan_sample(&x->ch[4], pool, kind, frame) & 31);
            o->animated |= 1;
        } else if (kind == DCA_KIND_TEXSRT) {
            /* GetTexSRTAnm_ + NNSi_G3dAnmCalcNsBta (texgen = texcoord, flag 8) */
            int32_t ss = chan_sample(&x->ch[0], pool, kind, frame), st_ = chan_sample(&x->ch[1], pool, kind, frame);
            uint32_t rot = (uint32_t)chan_sample(&x->ch[2], pool, kind, frame);
            int32_t ts = chan_sample(&x->ch[3], pool, kind, frame), tt = chan_sample(&x->ch[4], pool, kind, frame);
            uint8_t fl = o->srt_flags;
            if (ts == 0 && tt == 0)
                fl |= DCM_SRT_TRANS_ZERO;
            else {
                o->trans_s = ts;
                o->trans_t = tt;
                fl &= ~DCM_SRT_TRANS_ZERO;
            }
            if (rot == 0x10000000u)
                fl |= DCM_SRT_ROT_ZERO;
            else
                fl &= ~DCM_SRT_ROT_ZERO; /* Si3d texture matrices ignore the rotation (NNSi_G3dSendTexSRTSi3d) */
            if (ss == 0x1000 && st_ == 0x1000)
                fl |= DCM_SRT_SCALE_ONE;
            else {
                o->scale_s = ss;
                o->scale_t = st_;
                fl &= ~DCM_SRT_SCALE_ONE;
            }
            o->srt_flags = fl | DCM_SRT_ON;
            o->texgen = 1;
            o->animated |= 4;
        } else {
            /* NNSi_G3dAnmCalcNsBtp: SetTexParamaters_ / SetPlttParamaters_ by name */
            uint32_t v = (uint32_t)chan_sample(&x->ch[0], pool, kind, frame);
            const uint8_t *nb = s->base + a->pad;
            uint16_t ntex = ((const uint16_t *)nb)[0], npl = ((const uint16_t *)nb)[1];
            const char *tex, *pl;
            int ti = (int)(v & 0xFF), pi = (int)(v >> 8), j;
            if (!a->pad || ti >= ntex)
                continue;
            tex = (const char *)nb + 4 + 16 * ti;
            if (pi != 0xFF && pi < npl)
                pl = (const char *)nb + 4 + 16 * (ntex + pi);
            else
                pl = m->materials[mi].texture != 0xFFFF ? m->texx[m->materials[mi].texture].pltt : "";
            for (j = 0; j < m->hdr->num_textures; j++)
                if (!strncmp(m->texx[j].tex, tex, 16) && !strncmp(m->texx[j].pltt, pl, 16))
                    break;
            if (j < m->hdr->num_textures) {
                o->texture = (uint16_t)j;
                o->orig_w = m->texx[j].orig_w;
                o->orig_h = m->texx[j].orig_h;
                o->animated |= 2;
            }
        }
    }
    return n;
}

int dcm_matstate_uv(const dcm_model_t *m, int mi, const dcm_matstate_t *st, float *a, float *d, float *e,
                    float *f) {
    const dcm_material_t *mat = &m->materials[mi];
    const dcm_matx_t *x;
    float rs = 1.0f, rt = 1.0f, ss, sv, es = 0.0f, et = 0.0f, cw, ch, ow, oh;
    if (!st || !m->matx) {
        *a = mat->uv_a;
        *d = mat->uv_d;
        *e = mat->uv_e;
        *f = mat->uv_f;
        return (mat->flags & DCM_MF_TEXMTX) != 0;
    }
    x = &m->matx[mi];
    cw = x->base_w;
    ch = x->base_h;
    if (st->texture != 0xFFFF && st->texture < m->hdr->num_textures && x->base_w && x->base_h) {
        /* vertex uv is texcoord / base size: a pattern frame of another size samples other texels */
        cw = m->texx[st->texture].ds_w;
        ch = m->texx[st->texture].ds_h;
        rs = (float)x->base_w / cw;
        rt = (float)x->base_h / ch;
    }
    if (!(st->srt_flags & DCM_SRT_ON) || st->texgen != 1 || cw <= 0.0f || ch <= 0.0f) {
        *a = rs;
        *d = rt;
        *e = *f = 0.0f;
        return rs != 1.0f || rt != 1.0f;
    }
    /* NNSi_G3dSendTexSRTSi3d in units of the bound texture */
    ow = st->orig_w ? (float)st->orig_w : cw;
    oh = st->orig_h ? (float)st->orig_h : ch;
    ss = (st->srt_flags & DCM_SRT_SCALE_ONE) ? 1.0f : st->scale_s / 4096.0f;
    sv = (st->srt_flags & DCM_SRT_SCALE_ONE) ? 1.0f : st->scale_t / 4096.0f;
    if (!(st->srt_flags & DCM_SRT_TRANS_ZERO)) {
        es = -ss * (st->trans_s / 4096.0f) * ow / cw;
        et = -sv * (st->trans_t / 4096.0f) * oh / ch;
    }
    if (st->mag_w != 0x1000) {
        ss *= st->mag_w / 4096.0f;
        es *= st->mag_w / 4096.0f;
    }
    if (st->mag_h != 0x1000) {
        sv *= st->mag_h / 4096.0f;
        et *= st->mag_h / 4096.0f;
    }
    *a = ss * rs;
    *d = sv * rt;
    *e = es;
    *f = et;
    return 1;
}

/* ------------------------------------------------------------------ draw */

static uint32_t vertex_color(const dcm_material_t *mat, const dcm_matstate_t *ms, const dcm_vertex_t *v,
                             const xform_t *x, const dcm_lights_t *lights, uint32_t alpha) {
    float r, g, b;
    if (mat->light_mask && (v->flags & DCM_VF_NORMAL)) {
        float n[3];
        int i;
        float nx = v->nx * (1.0f / 16384.0f), ny = v->ny * (1.0f / 16384.0f), nz = v->nz * (1.0f / 16384.0f);
        n[0] = nx * x->n[0][0] + ny * x->n[1][0] + nz * x->n[2][0];
        n[1] = nx * x->n[0][1] + ny * x->n[1][1] + nz * x->n[2][1];
        n[2] = nx * x->n[0][2] + ny * x->n[1][2] + nz * x->n[2][2];
        norm3(n);
        r = chan(ms->emission, 16);
        g = chan(ms->emission, 8);
        b = chan(ms->emission, 0);
        for (i = 0; i < lights->count; i++) {
            float l[3], d;
            if (!(mat->light_mask & (1 << i)))
                continue;
            l[0] = lights->dir[i][0];
            l[1] = lights->dir[i][1];
            l[2] = lights->dir[i][2];
            norm3(l);
            d = -(l[0] * n[0] + l[1] * n[1] + l[2] * n[2]);
            if (d < 0.0f)
                d = 0.0f;
            r += lights->col[i][0] * (chan(ms->diffuse, 16) * d + chan(ms->ambient, 16));
            g += lights->col[i][1] * (chan(ms->diffuse, 8) * d + chan(ms->ambient, 8));
            b += lights->col[i][2] * (chan(ms->diffuse, 0) * d + chan(ms->ambient, 0));
        }
    } else if (v->flags & DCM_VF_COLOR) {
        r = chan(v->color, 16);
        g = chan(v->color, 8);
        b = chan(v->color, 0);
    } else {
        r = chan(ms->diffuse, 16);
        g = chan(ms->diffuse, 8);
        b = chan(ms->diffuse, 0);
    }
    if (r > 1.0f)
        r = 1.0f;
    if (g > 1.0f)
        g = 1.0f;
    if (b > 1.0f)
        b = 1.0f;
    return (alpha << 24) | ((uint32_t)(r * 255.0f) << 16) | ((uint32_t)(g * 255.0f) << 8) | (uint32_t)(b * 255.0f);
}

static int state_class(const dcm_model_t *m, const dcm_material_t *mat, const dcm_matstate_t *ms) {
    if (ms->alpha < 31 || ((mat->flags & DCM_MF_TRANSLUCENT) && ms->texture == mat->texture))
        return DCM_CLASS_TR;
    if (ms->texture != 0xFFFF && ms->texture < m->hdr->num_textures)
        return m->tex_class[ms->texture];
    return DCM_CLASS_OP;
}

dcm_emit_hook_t dcm_host_emit;

/* one vertex of the strip being built: clip position, uv, colour */
typedef struct {
    float c[4];
    float u, v;
    uint32_t col;
} cvert_t;

static void emit(int material, const cvert_t *cv, const dcm_camera_t *cam, int eol) {
    float o[5];
    project(cv->c, cam, o);
    o[3] = cv->u;
    o[4] = cv->v;
#ifdef _arch_dreamcast
    {
        pvr_vertex_t pv;
        pv.flags = eol ? PVR_CMD_VERTEX_EOL : PVR_CMD_VERTEX;
        pv.x = o[0];
        pv.y = o[1];
        pv.z = o[2];
        pv.u = o[3];
        pv.v = o[4];
        pv.argb = cv->col;
        pv.oargb = 0;
        pvr_prim(&pv, sizeof(pv));
    }
#endif
    if (dcm_host_emit)
        dcm_host_emit(material, o, cv->col, eol);
}

static uint32_t lerp_col(uint32_t a, uint32_t b, float t) {
    uint32_t r = 0;
    int sh;
    for (sh = 0; sh < 32; sh += 8) {
        float ca = (float)((a >> sh) & 255), cb = (float)((b >> sh) & 255);
        int v = (int)(ca + (cb - ca) * t + 0.5f);
        if (v < 0)
            v = 0;
        if (v > 255)
            v = 255;
        r |= (uint32_t)v << sh;
    }
    return r;
}

/* Triangle a, b, c clipped against the near plane w >= nw (Sutherland-Hodgman, 0, 3 or 4 vertices),
 * submitted as one PVR strip p0 p1 p(n-1) p2 (same winding as the triangle). */
static int clip_tri(int material, const cvert_t *a, const cvert_t *b, const cvert_t *c, const dcm_camera_t *cam) {
    const cvert_t *in[3];
    cvert_t out[4];
    int n = 0, i;
    float nw = cam->near_w;
    in[0] = a;
    in[1] = b;
    in[2] = c;
    for (i = 0; i < 3; i++) {
        const cvert_t *p = in[i], *q = in[(i + 1) % 3];
        int pin = p->c[3] >= nw, qin = q->c[3] >= nw;
        if (pin)
            out[n++] = *p;
        if (pin != qin && n < 4) {
            float t = (nw - p->c[3]) / (q->c[3] - p->c[3]);
            cvert_t *r = &out[n++];
            int k;
            for (k = 0; k < 4; k++)
                r->c[k] = p->c[k] + (q->c[k] - p->c[k]) * t;
            r->c[3] = nw;
            r->u = p->u + (q->u - p->u) * t;
            r->v = p->v + (q->v - p->v) * t;
            r->col = lerp_col(p->col, q->col, t);
        }
    }
    if (n < 3)
        return 0;
    emit(material, &out[0], cam, 0);
    emit(material, &out[1], cam, 0);
    if (n == 4) {
        emit(material, &out[3], cam, 0);
        emit(material, &out[2], cam, 1);
        return 2;
    }
    emit(material, &out[2], cam, 1);
    return 1;
}

#define DCM_STRIP_MAX 256

int dcm_draw(const dcm_model_t *m, const dcm_mtx_t *pal, const dcm_mtx_t *model_mtx, const dcm_camera_t *cam,
             const uint8_t *vis, const dcm_lights_t *lights, const dcm_draw_opts_t *opts) {
    return dcm_draw_ex(m, pal, model_mtx, cam, vis, lights, opts, NULL);
}

int dcm_draw_ex(const dcm_model_t *m, const dcm_mtx_t *pal, const dcm_mtx_t *model_mtx, const dcm_camera_t *cam,
                const uint8_t *vis, const dcm_lights_t *lights, const dcm_draw_opts_t *opts,
                const dcm_matstate_t *states) {
    static xform_t xf[DCM_MAX_PALETTE];
    static cvert_t sv[DCM_STRIP_MAX];
    dcm_lights_t deflights;
    int sent = 0;
    uint32_t d;
    int want;
    if (!lights) {
        dcm_lights_stage(&deflights, 0);
        lights = &deflights;
    }
    build_xforms(m, pal, model_mtx, cam, xf);
    for (d = 0; d < m->hdr->num_draws; d++) {
        const dcm_draw_t *dr = &m->draws[d];
        const dcm_material_t *mat = &m->materials[dr->material];
        dcm_matstate_t tmp;
        const dcm_matstate_t *ms;
        int cls, envmap;
        uint32_t s, alpha;
        float ua, ud, ue, uf;
        if (dr->vis_node != 0xFFFF) {
            int visible = vis ? vis[dr->vis_node] : dr->vis_default;
            if (!visible)
                continue;
        }
        if (states) {
            ms = &states[dr->material];
        } else {
            mat_static(m, dr->material, &tmp);
            ms = &tmp;
        }
        /* NNSi_G3dFuncSbc_MAT_InternalDefault: polygon alpha 0 -> the material's shapes are skipped */
        if (ms->alpha == 0)
            continue;
        cls = state_class(m, mat, ms);
        envmap = states ? (ms->texgen == 2) : (mat->flags & DCM_MF_ENVMAP) != 0;
        dcm_matstate_uv(m, dr->material, states ? ms : NULL, &ua, &ud, &ue, &uf);
#ifdef _arch_dreamcast
        want = cls == DCM_CLASS_OP ? PVR_LIST_OP_POLY : cls == DCM_CLASS_TR ? PVR_LIST_TR_POLY : opts->pt_list;
        if (want != opts->list)
            continue;
        {
            pvr_poly_cxt_t cxt;
            pvr_poly_hdr_t hdr;
            if (ms->texture != 0xFFFF && ms->texture < m->hdr->num_textures && m->tex_vram[ms->texture]) {
                const dcm_texture_t *t = &m->textures[ms->texture];
                static const int fmts[3] = {PVR_TXRFMT_ARGB1555, PVR_TXRFMT_RGB565, PVR_TXRFMT_ARGB4444};
                int fl = mat->flags, flip = 0, clamp = 0;
                pvr_poly_cxt_txr(&cxt, (pvr_list_t)opts->list, fmts[t->format] | PVR_TXRFMT_NONTWIDDLED, t->width,
                                 t->height, (pvr_ptr_t)m->tex_vram[ms->texture], PVR_FILTER_NONE);
                if (!(fl & DCM_MF_REPEAT_S))
                    clamp |= 2;
                else if (fl & DCM_MF_FLIP_S)
                    flip |= 2;
                if (!(fl & DCM_MF_REPEAT_T))
                    clamp |= 1;
                else if (fl & DCM_MF_FLIP_T)
                    flip |= 1;
                cxt.txr.uv_flip = (pvr_uv_flip_t)flip;   /* bit1 = U, bit0 = V, as PVR_UVFLIP_* */
                cxt.txr.uv_clamp = (pvr_uv_clamp_t)clamp;
            } else {
                pvr_poly_cxt_col(&cxt, (pvr_list_t)opts->list);
            }
            /* DS back faces are clockwise on screen (y down) */
            if ((mat->flags & (DCM_MF_CULL_BACK | DCM_MF_CULL_FRONT)) == DCM_MF_CULL_BACK)
                cxt.gen.culling = PVR_CULLING_CW;
            else if ((mat->flags & (DCM_MF_CULL_BACK | DCM_MF_CULL_FRONT)) == DCM_MF_CULL_FRONT)
                cxt.gen.culling = PVR_CULLING_CCW;
            else
                cxt.gen.culling = PVR_CULLING_NONE;
            pvr_poly_compile(&hdr, &cxt);
            pvr_prim(&hdr, sizeof(hdr));
        }
#else
        (void)cls;
        (void)want;
#endif
        alpha = (uint32_t)ms->alpha * 255u / 31u;
        if (opts)
            alpha = alpha * opts->alpha_scale / 255u;
        for (s = dr->first_strip; s < dr->first_strip + dr->num_strips; s++) {
            const dcm_strip_t *st = &m->strips[s];
            const dcm_vertex_t *v = &m->vertices[st->first_vertex];
            int k, n = st->count < DCM_STRIP_MAX ? st->count : DCM_STRIP_MAX, all_in = 1;
            for (k = 0; k < n; k++) {
                const xform_t *x = &xf[v[k].palette];
                cvert_t *o = &sv[k];
                xform_clip(x, &v[k], o->c);
                if (o->c[3] < cam->near_w)
                    all_in = 0;
                o->col = vertex_color(mat, ms, &v[k], x, lights, alpha);
                if (envmap) {
                    float nx = v[k].nx * (1.0f / 16384.0f), ny = v[k].ny * (1.0f / 16384.0f),
                          nz = v[k].nz * (1.0f / 16384.0f);
                    float ex = nx * x->n[0][0] + ny * x->n[1][0] + nz * x->n[2][0];
                    float ey = nx * x->n[0][1] + ny * x->n[1][1] + nz * x->n[2][1];
                    o->u = 0.5f + 0.5f * ex;
                    o->v = 0.5f - 0.5f * ey;
                } else {
                    o->u = ua * v[k].u + ue;
                    o->v = ud * v[k].v + uf;
                }
            }
            if (all_in) {
                for (k = 0; k < n; k++)
                    emit(dr->material, &sv[k], cam, k == n - 1);
                sent += n - 2;
                continue;
            }
            /* the strip crosses the near plane: its triangles one by one, clipped (odd triangles of a
             * strip are wound the other way: swap their first two vertices) */
            for (k = 0; k + 2 < n; k++) {
                if (k & 1)
                    sent += clip_tri(dr->material, &sv[k + 1], &sv[k], &sv[k + 2], cam);
                else
                    sent += clip_tri(dr->material, &sv[k], &sv[k + 1], &sv[k + 2], cam);
            }
        }
    }
    return sent;
}

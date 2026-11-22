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
#include <stdlib.h>
#include <string.h>

#ifdef _arch_dreamcast
#include <dc/pvr.h>
#endif
#if defined(_arch_dreamcast) && defined(DCM_PROFILE)
/* measurement builds only (-DDCM_PROFILE): time spent in dcm_draw_ex, printed every 2 s */
#include <stdio.h>
#include <arch/timer.h>
static uint64_t g_prof_us, g_prof_t0, g_prof_calls;
static uint32_t g_prof_xf, g_prof_lit, g_prof_emit, g_prof_clip, g_prof_hdr;
static uint64_t g_prof_ph[4];
#define PROF(x) (x)
static struct { const void *m; uint32_t nv, calls, xf; uint64_t us; } g_prof_m[24];
static void prof_add(uint64_t t0, const dcm_model_t *m, uint32_t xf0) {
    uint64_t now = timer_us_gettime64();
    int i;
    for (i = 0; i < 24; i++)
        if (g_prof_m[i].m == m || !g_prof_m[i].m) {
            g_prof_m[i].m = m;
            g_prof_m[i].nv = m->hdr->num_vertices;
            g_prof_m[i].calls++;
            g_prof_m[i].us += now - t0;
            g_prof_m[i].xf += g_prof_xf - xf0;
            break;
        }
    g_prof_us += now - t0;
    g_prof_calls++;
    if (!g_prof_t0)
        g_prof_t0 = now;
    else if (now - g_prof_t0 >= 2000000) {
        double f = 16667.0 / (double)(now - g_prof_t0);
        printf("dcm prof: %.2f ms per 16.67 ms (%lu draw calls/s); per 16.67 ms: xform %.0f lit %.0f emit %.0f "
               "clip %.0f hdr %.0f\n",
               (double)g_prof_us * f / 1000.0, (unsigned long)(g_prof_calls * 1000000 / (now - g_prof_t0)),
               g_prof_xf * f, g_prof_lit * f, g_prof_emit * f, g_prof_clip * f, g_prof_hdr * f);
        g_prof_xf = g_prof_lit = g_prof_emit = g_prof_clip = g_prof_hdr = 0;
        printf("  phases per 16.67 ms: positions %.3f lighting %.3f emit %.3f setup %.3f ms\n", g_prof_ph[0] * f / 1000.0,
               g_prof_ph[1] * f / 1000.0, g_prof_ph[2] * f / 1000.0, g_prof_ph[3] * f / 1000.0);
        memset(g_prof_ph, 0, sizeof(g_prof_ph));
        for (i = 0; i < 24 && g_prof_m[i].m; i++)
            printf("  model %u verts: %.3f ms per 16.67 ms, %.1f calls, %.0f xf\n", (unsigned)g_prof_m[i].nv,
                   g_prof_m[i].us * f / 1000.0, g_prof_m[i].calls * f, g_prof_m[i].xf * f);
        memset(g_prof_m, 0, sizeof(g_prof_m));
        g_prof_us = 0;
        g_prof_calls = 0;
        g_prof_t0 = now;
    }
}
#else
#define PROF(x) ((void)0)
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

/* draw caches (dcm_draw_ex): one slot per distinct (palette, position) / (palette, normal), valid while its
 * stamp equals the current call (position, normal) or draw (lit colour); grown at load time only */
typedef struct {
    uint32_t stamp;
    float c[4]; /* clip space */
    float s[3]; /* screen x, y, PVR depth (when w >= near_w) */
} pslot_t;     /* 32 bytes */
struct xform_s;
#define XFORM_SIZE 100 /* sizeof(xform_t), checked below */

/* Lighting of a (palette, normal) slot, kept in the model across draws and frames: d / e are valid while the
 * palette entry's normal matrix and the lights are bitwise the ones they were computed from (epoch), col while
 * also the material colours match (colour key).  Same inputs, same results. */
typedef struct {
    uint32_t ep;  /* palette epoch of d and of the slot's envmap normal (0: never) */
    uint32_t cid; /* colour key of col (0: none) */
    uint32_t col; /* lit rgb */
    float d[4];   /* max(0, -L.n) per light, unit view-space normal */
    uint32_t pad;
} nslot_t;        /* 32 bytes */

#define LC_KEYS 32
typedef struct {
    int nl;                  /* lights the slots were computed with (raw values) */
    float ldir[4][3], lcol[4][3];
    uint32_t gen, cgen, nkeys;
    struct {
        uint32_t dif, amb, em, mask, id;
    } key[LC_KEYS];
    float (*nmat)[9];        /* [num_palette] normal matrix of the current epoch */
    uint32_t *pep;           /* [num_palette] epoch */
    nslot_t *ns;             /* [num_unrm] */
    float (*env)[2];         /* [num_unrm] view-space normal x, y before normalising (envmap) */
    /* transforms kept while palette, model matrix and camera repeat bitwise (the list passes of a frame,
     * a still camera): xf of generation xgen, position slots valid when their stamp is xgen */
    struct xform_s *xf;      /* [num_palette] */
    pslot_t *ps;             /* [num_upos] */
    dcm_mtx_t *kpal;         /* [num_palette] */
    dcm_mtx_t kmodel, kview;
    float kproj[4][4], kvp[4], knear; /* projection, viewport and near_w also shape the screen positions */
    uint32_t xgen;
    int kvalid, kfront;
    /* per draw: passes already done for the current generations (positions: xgen; lighting: lgen, colour key,
     * wgen = slot-colour writes so far) */
    uint32_t lgen, wgen;
    struct dstate_s {
        uint32_t pgen, lgen, cid, wgen;
        int allin; /* every strip in front of the near plane at pgen */
    } *ds;          /* [num_draws] */
} lcache_t;

static uint32_t vhash(const uint32_t *w, int n, uint32_t h) {
    int i;
    for (i = 0; i < n; i++) {
        h ^= w[i];
        h *= 0x01000193u;
        h ^= h >> 15;
    }
    return h;
}

/* vref[2 * i] / vref[2 * i + 1]: slot of vertex i's position / normal (bitwise equal inputs share a slot,
 * so a shared slot gives exactly the per-vertex result) */
static int build_vref(dcm_model_t *m) {
    uint32_t n = m->hdr->num_vertices, size = 16, i, pass;
    uint32_t *tab;
    if (n == 0)
        return 0;
    if (n > 65535)
        return -1;
    while (size < 2 * n)
        size <<= 1;
    m->vref = (uint16_t *)malloc(4 * (size_t)n + 16 * (size_t)m->hdr->num_palette);
    tab = (uint32_t *)malloc(4 * (size_t)(size > 6u * m->hdr->num_palette ? size : 6u * m->hdr->num_palette));
    if (!m->vref || !tab) {
        free(tab);
        return -1;
    }
    m->pbound = (float *)(m->vref + 2 * n);
    {
        float *b = m->pbound, *lo = (float *)tab, *hi = lo + 3 * m->hdr->num_palette;
        uint32_t p;
        for (p = 0; p < m->hdr->num_palette; p++) {
            b[4 * p + 3] = -1.0f;
            lo[3 * p] = lo[3 * p + 1] = lo[3 * p + 2] = 1e30f;
            hi[3 * p] = hi[3 * p + 1] = hi[3 * p + 2] = -1e30f;
        }
        for (i = 0; i < n; i++) {
            const dcm_vertex_t *v = &m->vertices[i];
            const float *xyz = &v->x;
            int c;
            if (v->palette >= m->hdr->num_palette)
                continue;
            for (c = 0; c < 3; c++) {
                if (xyz[c] < lo[3 * v->palette + c])
                    lo[3 * v->palette + c] = xyz[c];
                if (xyz[c] > hi[3 * v->palette + c])
                    hi[3 * v->palette + c] = xyz[c];
            }
            b[4 * v->palette + 3] = 0.0f;
        }
        for (p = 0; p < m->hdr->num_palette; p++)
            if (b[4 * p + 3] == 0.0f) {
                int c;
                for (c = 0; c < 3; c++)
                    b[4 * p + c] = 0.5f * (lo[3 * p + c] + hi[3 * p + c]);
            }
        for (i = 0; i < n; i++) {
            const dcm_vertex_t *v = &m->vertices[i];
            float *c, dx, dy, dz, r;
            if (v->palette >= m->hdr->num_palette)
                continue;
            c = &b[4 * v->palette];
            dx = v->x - c[0];
            dy = v->y - c[1];
            dz = v->z - c[2];
            r = sqrtf(dx * dx + dy * dy + dz * dz) * 1.0001f + 1e-6f;
            if (r > c[3])
                c[3] = r;
        }
    }
    for (pass = 0; pass < 2; pass++) {
        uint16_t count = 0;
        memset(tab, 0, 4 * (size_t)size);
        for (i = 0; i < n; i++) {
            const dcm_vertex_t *v = &m->vertices[i];
            uint32_t key[4], h;
            if (pass == 0) {
                memcpy(key, &v->x, 12);
            } else {
                key[0] = (uint16_t)v->nx;
                key[1] = (uint16_t)v->ny;
                key[2] = (uint16_t)v->nz;
            }
            key[3] = v->palette;
            h = vhash(key, 4, 0x811C9DC5u) & (size - 1);
            for (;; h = (h + 1) & (size - 1)) {
                const dcm_vertex_t *o;
                if (!tab[h]) {
                    tab[h] = i + 1;
                    m->vref[2 * i + pass] = count++;
                    break;
                }
                o = &m->vertices[tab[h] - 1];
                if (o->palette == v->palette &&
                    (pass == 0 ? !memcmp(&o->x, &v->x, 12) : o->nx == v->nx && o->ny == v->ny && o->nz == v->nz)) {
                    m->vref[2 * i + pass] = m->vref[2 * (tab[h] - 1) + pass];
                    break;
                }
            }
        }
        if (pass == 0)
            m->num_upos = count;
        else
            m->num_unrm = count;
    }
    free(tab);
    {
        uint32_t np = m->hdr->num_palette;
        size_t xfsz = np * (size_t)XFORM_SIZE;
        lcache_t *lc = (lcache_t *)calloc(1, sizeof(lcache_t) + (size_t)m->num_upos * sizeof(pslot_t) + xfsz +
                                                 np * (36 + 4 + sizeof(dcm_mtx_t)) +
                                                 (size_t)m->num_unrm * (sizeof(nslot_t) + 8) +
                                                 (size_t)m->hdr->num_draws * sizeof(struct dstate_s));
        if (!lc)
            return -1;
        lc->nl = -1;
        lc->ps = (pslot_t *)(lc + 1); /* 32-byte entries first: keeps them 4-byte aligned */
        lc->xf = (struct xform_s *)(lc->ps + m->num_upos);
        lc->ns = (nslot_t *)((uint8_t *)lc->xf + xfsz);
        lc->env = (float(*)[2])(lc->ns + m->num_unrm);
        lc->kpal = (dcm_mtx_t *)(lc->env + m->num_unrm);
        lc->nmat = (float(*)[9])(lc->kpal + np);
        lc->pep = (uint32_t *)(lc->nmat + np);
        lc->ds = (struct dstate_s *)(lc->pep + np);
        m->lcache = lc;
    }
    return 0;
}

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
    if (build_vref(m) != 0) {
        dcm_model_free(m);
        return -4;
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
    free(m->vref);
    free(m->lcache);
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

/* Stored transposed (t[out][in]) so a vertex reads each output's coefficients in address order. */
typedef struct xform_s {
    float t[4][4];   /* palette * model * view * proj: t[c][r] = M[r][c] */
    float n[3][3];   /* normal matrix (palette * model * view): n[c][r] = M[r][c] */
} xform_t;

typedef char xform_size_check[sizeof(xform_t) == XFORM_SIZE ? 1 : -1];

/* Hot loops read floats in address order: SH-4 fmov.s has no displacement form, so post-increment loads save
 * the address arithmetic per value.  The arithmetic stays in C (same expressions, same results). */
#ifdef _arch_dreamcast
#define LDF(p)                                                                                                   \
    __extension__({                                                                                              \
        float f_;                                                                                                \
        __asm__("fmov.s @%1+,%0" : "=f"(f_), "+r"(p) : "m"(*(p)));                                              \
        f_;                                                                                                      \
    })
#else
#define LDF(p) (*(p)++)
#endif

/* rows of a * b (dcm_mtx_mul's expressions) with b given column-wise: bt[j] = b[0][j] b[1][j] b[2][j] b[3][j] */
static inline void mtx_mul_t(dcm_mtx_t *out, const dcm_mtx_t *a, const float (*bt)[4]) {
    int i, j;
    for (i = 0; i < 4; i++) {
        const float *ar = a->m[i];
        float a0 = ar[0], a1 = ar[1], a2 = ar[2];
        for (j = 0; j < 3; j++) {
            const float *q = bt[j];
            float b0 = LDF(q), b1 = LDF(q), b2 = LDF(q), v;
            v = a0 * b0 + a1 * b1 + a2 * b2;
            if (i == 3)
                v += *q;
            out->m[i][j] = v;
        }
    }
}

static void build_xforms(const dcm_model_t *m, const dcm_mtx_t *pal, const dcm_mtx_t *model_mtx,
                         const dcm_camera_t *cam, xform_t *xf) {
    int i, r, c, k;
    float mt[3][4], vt[3][4], pt[4][4];
    for (c = 0; c < 3; c++)
        for (r = 0; r < 4; r++) {
            mt[c][r] = model_mtx->m[r][c];
            vt[c][r] = cam->view.m[r][c];
        }
    for (c = 0; c < 4; c++)
        for (r = 0; r < 4; r++)
            pt[c][r] = cam->proj[r][c];
    for (i = 0; i < m->hdr->num_palette; i++) {
        dcm_mtx_t mv, t;
        const dcm_palette_t *p = &m->palette[i];
        mtx_mul_t(&t, &pal[i], (const float(*)[4])mt);
        mtx_mul_t(&mv, &t, (const float(*)[4])vt);
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
        for (r = 0; r < 4; r++) {
            float m0 = mv.m[r][0], m1 = mv.m[r][1], m2 = mv.m[r][2];
            for (c = 0; c < 4; c++) {
                const float *q = pt[c];
                float v = 0.0f;
                v += m0 * LDF(q);
                v += m1 * LDF(q);
                v += m2 * LDF(q);
                if (r == 3)
                    v += *q;
                xf[i].t[c][r] = v;
            }
        }
        for (r = 0; r < 3; r++)
            for (c = 0; c < 3; c++)
                xf[i].n[c][r] = mv.m[r][c];
    }
    (void)k;
}

/* clip-space position (x, y, z, w) of a vertex */
static inline void xform_clip(const xform_t *x, const dcm_vertex_t *v, float *c) {
    const float *q = &x->t[0][0], *p = &v->x;
    float vx = LDF(p), vy = LDF(p), vz = LDF(p), a, b, d, e;
    a = LDF(q), b = LDF(q), d = LDF(q), e = LDF(q);
    c[0] = vx * a + vy * b + vz * d + e;
    a = LDF(q), b = LDF(q), d = LDF(q), e = LDF(q);
    c[1] = vx * a + vy * b + vz * d + e;
    a = LDF(q), b = LDF(q), d = LDF(q), e = LDF(q);
    c[2] = vx * a + vy * b + vz * d + e;
    a = LDF(q), b = LDF(q), d = LDF(q), e = LDF(q);
    c[3] = vx * a + vy * b + vz * d + e;
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

/* Per-draw shading constants: the material colours as floats and the lights the material uses (the values
 * the per-vertex code used to recompute for every vertex; same expressions, same results). */
typedef struct {
    int lit;                      /* material has a light mask */
    int n;                        /* lights used, in light order */
    uint32_t alpha;               /* alpha << 24 */
    uint32_t flat;                /* unlit vertex without colour: diffuse */
    float em[3], dif[3], amb[3];
    int li[4];                    /* light index of each light used */
    float col[4][3];
} shade_t;

static inline uint32_t pack_rgb(float r, float g, float b) {
    if (r > 1.0f)
        r = 1.0f;
    if (g > 1.0f)
        g = 1.0f;
    if (b > 1.0f)
        b = 1.0f;
    /* r, g, b >= 0: the signed conversion gives the unsigned result without its range checks */
    return ((uint32_t)(int32_t)(r * 255.0f) << 16) | ((uint32_t)(int32_t)(g * 255.0f) << 8) |
           (uint32_t)(int32_t)(b * 255.0f);
}

static void shade_setup(shade_t *S, const dcm_material_t *mat, const dcm_matstate_t *ms, const dcm_lights_t *lights,
                        uint32_t alpha) {
    int i;
    S->lit = mat->light_mask != 0;
    S->n = 0;
    S->alpha = alpha << 24;
    S->em[0] = chan(ms->emission, 16);
    S->em[1] = chan(ms->emission, 8);
    S->em[2] = chan(ms->emission, 0);
    S->dif[0] = chan(ms->diffuse, 16);
    S->dif[1] = chan(ms->diffuse, 8);
    S->dif[2] = chan(ms->diffuse, 0);
    S->amb[0] = chan(ms->ambient, 16);
    S->amb[1] = chan(ms->ambient, 8);
    S->amb[2] = chan(ms->ambient, 0);
    S->flat = pack_rgb(S->dif[0], S->dif[1], S->dif[2]);
    for (i = 0; i < lights->count && i < 4; i++) {
        if (!(mat->light_mask & (1 << i)))
            continue;
        S->li[S->n] = i;
        memcpy(S->col[S->n], lights->col[i], sizeof(S->col[0]));
        S->n++;
    }
}

/* view-space normal of a slot: x, y before normalising (envmap), then per light max(0, -L.n) */
static void __attribute__((unused)) nslot_fill(nslot_t *N, float *e, const dcm_vertex_t *v, const xform_t *x, const float (*ldir)[3],
                       int nl) {
    float n[3];
    int i;
    float nx = v->nx * (1.0f / 16384.0f), ny = v->ny * (1.0f / 16384.0f), nz = v->nz * (1.0f / 16384.0f);
    const float *q = &x->n[0][0], *l = &ldir[0][0];
    float a, b, c, len;
    a = LDF(q), b = LDF(q), c = LDF(q);
    n[0] = nx * a + ny * b + nz * c;
    a = LDF(q), b = LDF(q), c = LDF(q);
    n[1] = nx * a + ny * b + nz * c;
    a = LDF(q), b = LDF(q), c = LDF(q);
    n[2] = nx * a + ny * b + nz * c;
    e[0] = n[0];
    e[1] = n[1];
    /* norm3 */
    len = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
    if (len > 0.0f) {
        len = 1.0f / len;
        n[0] *= len;
        n[1] *= len;
        n[2] *= len;
    }
    for (i = 0; i < nl; i++) {
        float d;
        a = LDF(l), b = LDF(l), c = LDF(l);
        d = -(a * n[0] + b * n[1] + c * n[2]);
        if (d < 0.0f)
            d = 0.0f;
        N->d[i] = d;
    }
}

/* DS lighting: emission + sum(light colour * (diffuse * max(0, -L.n) + ambient)) */
static inline uint32_t lit_rgb(const shade_t *__restrict S, const nslot_t *__restrict N) {
    const float *q = S->em, *c = &S->col[0][0];
    float r = LDF(q), g = LDF(q), b = LDF(q);
    float dr = LDF(q), dg = LDF(q), db = LDF(q), ar = LDF(q), ag = LDF(q), ab = LDF(q);
    int i;
    for (i = 0; i < S->n; i++) {
        float d = N->d[S->li[i]], c0 = LDF(c), c1 = LDF(c), c2 = LDF(c);
        r += c0 * (dr * d + ar);
        g += c1 * (dg * d + ag);
        b += c2 * (db * d + ab);
    }
    return pack_rgb(r, g, b);
}

static inline int class_of(const dcm_model_t *m, const dcm_material_t *mat, int alpha, uint16_t texture) {
    if (alpha < 31 || ((mat->flags & DCM_MF_TRANSLUCENT) && texture == mat->texture))
        return DCM_CLASS_TR;
    if (texture != 0xFFFF && texture < m->hdr->num_textures)
        return m->tex_class[texture];
    return DCM_CLASS_OP;
}

static int state_class(const dcm_model_t *m, const dcm_material_t *mat, const dcm_matstate_t *ms) {
    return class_of(m, mat, ms->alpha, ms->texture);
}

dcm_emit_hook_t dcm_host_emit;

/* one vertex of the strip being built: clip position, uv, colour */
typedef struct {
    float c[4];
    float u, v;
    uint32_t col;
} cvert_t;

/* screen-space vertex out: PVR direct render (store queue, no copy), host hook for tests */
#ifdef _arch_dreamcast
extern uint32_t *dcm_dc_capture;
extern uint32_t dcm_dc_capture_n;
#endif
static inline void submit(int material, float x, float y, float z, float u, float v, uint32_t col, int eol) {
#ifdef _arch_dreamcast
    if (dcm_dc_capture && !dcm_host_emit) {
        pvr_vertex_t *pv = (pvr_vertex_t *)(dcm_dc_capture + 8 * dcm_dc_capture_n++);
        pv->flags = eol ? PVR_CMD_VERTEX_EOL : PVR_CMD_VERTEX;
        pv->x = x;
        pv->y = y;
        pv->z = z;
        pv->u = u;
        pv->v = v;
        pv->argb = col;
        pv->oargb = 0;
    } else {
        pvr_vertex_t *pv = (pvr_vertex_t *)pvr_dr_target();
        pv->flags = eol ? PVR_CMD_VERTEX_EOL : PVR_CMD_VERTEX;
        pv->x = x;
        pv->y = y;
        pv->z = z;
        pv->u = u;
        pv->v = v;
        pv->argb = col;
        pv->oargb = 0;
        pvr_dr_commit(pv);
    }
#endif
    PROF(g_prof_emit++);
    if (dcm_host_emit) {
        float o[5];
        o[0] = x;
        o[1] = y;
        o[2] = z;
        o[3] = u;
        o[4] = v;
        dcm_host_emit(material, o, col, eol);
    }
}

static void emit(int material, const cvert_t *cv, const dcm_camera_t *cam, int eol) {
    float o[3];
    project(cv->c, cam, o);
    submit(material, o[0], o[1], o[2], cv->u, cv->v, cv->col, eol);
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
    PROF(g_prof_clip++);
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

#ifdef _arch_dreamcast
/* Compiled PVR polygon headers by value (list, texture, wrap, culling): pvr_poly_cxt_* + pvr_poly_compile
 * run once per distinct state instead of once per draw. The header depends only on the key. */
typedef struct {
    uint32_t list, tex, fmt, wh, mode; /* mode: flip | clamp << 2 | culling << 4 | textured << 8 | used << 9 */
    pvr_poly_hdr_t hdr;
} hdr_slot_t;
#define HDR_CACHE 64
static hdr_slot_t g_hdr_cache[HDR_CACHE] __attribute__((aligned(32)));

static const pvr_poly_hdr_t *material_header(const dcm_model_t *m, const dcm_material_t *mat,
                                             const dcm_matstate_t *ms, int list) {
    static const int fmts[3] = {PVR_TXRFMT_ARGB1555, PVR_TXRFMT_RGB565, PVR_TXRFMT_ARGB4444};
    uint32_t tex = 0, fmt = 0, wh = 0, mode, h;
    int flip = 0, clamp = 0, cull, textured = 0;
    hdr_slot_t *e;
    pvr_poly_cxt_t cxt;
    if (ms->texture != 0xFFFF && ms->texture < m->hdr->num_textures && m->tex_vram[ms->texture]) {
        const dcm_texture_t *t = &m->textures[ms->texture];
        int fl = mat->flags;
        textured = 1;
        tex = (uint32_t)(uintptr_t)m->tex_vram[ms->texture];
        fmt = (uint32_t)(fmts[t->format] | PVR_TXRFMT_NONTWIDDLED);
        wh = (uint32_t)t->width | (uint32_t)t->height << 16;
        if (!(fl & DCM_MF_REPEAT_S))
            clamp |= 2;
        else if (fl & DCM_MF_FLIP_S)
            flip |= 2;
        if (!(fl & DCM_MF_REPEAT_T))
            clamp |= 1;
        else if (fl & DCM_MF_FLIP_T)
            flip |= 1;
    }
    /* DS back faces are clockwise on screen (y down) */
    if ((mat->flags & (DCM_MF_CULL_BACK | DCM_MF_CULL_FRONT)) == DCM_MF_CULL_BACK)
        cull = PVR_CULLING_CW;
    else if ((mat->flags & (DCM_MF_CULL_BACK | DCM_MF_CULL_FRONT)) == DCM_MF_CULL_FRONT)
        cull = PVR_CULLING_CCW;
    else
        cull = PVR_CULLING_NONE;
    mode = (uint32_t)flip | (uint32_t)clamp << 2 | (uint32_t)cull << 4 | (uint32_t)textured << 8 | 1u << 9;
    h = (tex >> 5) ^ (tex >> 11) ^ fmt ^ wh ^ (wh >> 13) ^ mode * 0x9E37u ^ (uint32_t)list * 0x45D9F3Bu;
    h ^= h >> 16;
    e = &g_hdr_cache[h & (HDR_CACHE - 1)];
    if (e->mode == mode && e->list == (uint32_t)list && e->tex == tex && e->fmt == fmt && e->wh == wh)
        return &e->hdr;
    if (textured) {
        const dcm_texture_t *t = &m->textures[ms->texture];
        pvr_poly_cxt_txr(&cxt, (pvr_list_t)list, (int)fmt, t->width, t->height, (pvr_ptr_t)m->tex_vram[ms->texture],
                         PVR_FILTER_NONE);
        cxt.txr.uv_flip = (pvr_uv_flip_t)flip; /* bit1 = U, bit0 = V, as PVR_UVFLIP_* */
        cxt.txr.uv_clamp = (pvr_uv_clamp_t)clamp;
    } else {
        pvr_poly_cxt_col(&cxt, (pvr_list_t)list);
    }
    cxt.gen.culling = cull;
    pvr_poly_compile(&e->hdr, &cxt);
    PROF(g_prof_hdr++);
    e->list = (uint32_t)list;
    e->tex = tex;
    e->fmt = fmt;
    e->wh = wh;
    e->mode = mode;
    return &e->hdr;
}
#endif

#define DCM_STRIP_MAX 256

/* per dcm_draw_ex call / draw state of the strip loop */
typedef struct {
    const dcm_model_t *m;
    lcache_t *lc;
    const xform_t *xf;
    const dcm_camera_t *cam;
    pslot_t *ps;
    nslot_t *ns;
    float (*env)[2];
    const shade_t *S;
    const float (*ldir)[3];
    int nl;
    const uint32_t *pep;
    uint32_t cs, cid, sa;
    float near_w, ua, ud, ue, uf;
} dctx_t;

/* Whole model in front of the near plane: each palette entry's bounding sphere, with a margin far above the
 * rounding of the per-vertex w, has w >= near_w.  Then no strip needs the per-vertex test. */
static int model_in_front(const dcm_model_t *m, const xform_t *xf, float near_w) {
    int p;
    for (p = 0; p < m->hdr->num_palette; p++) {
        const float *b = &m->pbound[4 * p];
        const float *c = xf[p].t[3]; /* w coefficients */
        float w, len, mag;
        if (b[3] < 0.0f)
            continue;
        w = b[0] * c[0] + b[1] * c[1] + b[2] * c[2] + c[3];
        len = sqrtf(c[0] * c[0] + c[1] * c[1] + c[2] * c[2]);
        mag = (fabsf(b[0]) + b[3]) * fabsf(c[0]) + (fabsf(b[1]) + b[3]) * fabsf(c[1]) +
              (fabsf(b[2]) + b[3]) * fabsf(c[2]) + fabsf(c[3]);
        if (!(w - b[3] * len >= near_w + 1e-4f * mag + 1e-6f))
            return 0;
    }
    return 1;
}


#ifdef _arch_dreamcast
/* ------------------------------------------------------------------ SH-4 routines of the slot fills
 * The C above (xform_clip + project, nslot_fill, lit_rgb) instruction for instruction as GCC compiles it for
 * the SH-4 (same fmul / fmac / fadd / fdiv / fsqrt sequence, so the same results), without the address
 * arithmetic GCC adds around every float load and store (fmov.s has no displacement form).
 * Single precision (FPSCR.PR = 0, SZ = 0); fr0..fr11, r0..r7 scratch, fr12 saved where used. */
/* dcm_asm_pslot / dcm_asm_lslot: pslot_fill / lit_fill / nslot_fill_x below; a: */
typedef struct {
    const void *xf;       /* 0: xform_t[num_palette] */
    const float *vp;      /* 4: camera vp_x vp_y vp_w vp_h near_w */
    uint32_t cs;          /* 8: position generation */
    const uint32_t *pep;  /* 12: palette epochs */
    const float *ldir;    /* 16: unit light directions */
    int nl;               /* 20 */
    const float *em;      /* 24: shade_t em dif amb (li at +36, col at +52) */
    int nused;            /* 28: lights the material uses */
    uint32_t cid;         /* 32: colour key */
    float (*env)[2];      /* 36 */
    nslot_t *ns;          /* 40 */
} actx_t;
void dcm_asm_pslot(const actx_t *a, pslot_t *P, const dcm_vertex_t *v);
void dcm_asm_lslot(const actx_t *a, nslot_t *N, const dcm_vertex_t *v, int lit);
__asm__(
    "    .text\n"
    "    .align 2\n"
    /* dcm_asm_pslot(a, P, v): P->c = clip position of v, P->s = screen position when w >= near_w,
     * P->stamp = a->cs */
    "    .global _dcm_asm_pslot\n"
    "_dcm_asm_pslot:\n"
    "    mov.l   r5,@-r15\n"
    "    mov     r6,r1\n"
    "    add     #26,r1\n"
    "    mov.b   @r1,r0\n"
    "    extu.b  r0,r0\n"
    "    mov     #100,r1\n"
    "    mulu.w  r1,r0\n"
    "    mov.l   @(8,r4),r1\n"
    "    mov.l   r1,@-r15\n"
    "    sts     macl,r0\n"
    "    mov.l   @r4,r1\n"
    "    mov.l   @(4,r4),r7\n"
    "    add     r0,r1\n"
    "    mov     r6,r5\n"
    "    mov     r1,r4\n"
    "    mov.l   @(4,r15),r6\n"
    "    add     #4,r6\n"
    "    fmov.s  @r5+,fr8\n"
    "    fmov.s  @r5+,fr9\n"
    "    fmov.s  @r5,fr10\n"
    "    fmov.s  @r4+,fr1\n"
    "    fmov.s  @r4+,fr4\n"
    "    fmul    fr9,fr4\n"
    "    fmov.s  @r4+,fr2\n"
    "    fmov    fr8,fr0\n"
    "    fmac    fr0,fr1,fr4\n"
    "    fmov.s  @r4+,fr3\n"
    "    fmov    fr10,fr0\n"
    "    fmac    fr0,fr2,fr4\n"
    "    fadd    fr3,fr4\n"
    "    fmov.s  @r4+,fr1\n"
    "    fmov.s  @r4+,fr5\n"
    "    fmul    fr9,fr5\n"
    "    fmov.s  @r4+,fr2\n"
    "    fmov    fr8,fr0\n"
    "    fmac    fr0,fr1,fr5\n"
    "    fmov.s  @r4+,fr3\n"
    "    fmov    fr10,fr0\n"
    "    fmac    fr0,fr2,fr5\n"
    "    fadd    fr3,fr5\n"
    "    fmov.s  @r4+,fr1\n"
    "    fmov.s  @r4+,fr6\n"
    "    fmul    fr9,fr6\n"
    "    fmov.s  @r4+,fr2\n"
    "    fmov    fr8,fr0\n"
    "    fmac    fr0,fr1,fr6\n"
    "    fmov.s  @r4+,fr3\n"
    "    fmov    fr10,fr0\n"
    "    fmac    fr0,fr2,fr6\n"
    "    fadd    fr3,fr6\n"
    "    fmov.s  @r4+,fr1\n"
    "    fmov.s  @r4+,fr7\n"
    "    fmul    fr9,fr7\n"
    "    fmov.s  @r4+,fr2\n"
    "    fmov    fr8,fr0\n"
    "    fmac    fr0,fr1,fr7\n"
    "    fmov.s  @r4,fr3\n"
    "    fmov    fr10,fr0\n"
    "    fmac    fr0,fr2,fr7\n"
    "    fadd    fr3,fr7\n"
    "    mov     r6,r1\n"
    "    add     #16,r1\n"
    "    fmov.s  fr7,@-r1\n"
    "    fmov.s  fr6,@-r1\n"
    "    fmov.s  fr5,@-r1\n"
    "    fmov.s  fr4,@-r1\n"
    "    mov     r7,r1\n"
    "    add     #16,r1\n"
    "    fmov.s  @r1,fr1\n"
    "    fcmp/gt fr1,fr7\n"
    "    bt      1f\n"
    "    fcmp/eq fr1,fr7\n"
    "    bf      5f\n"
    "1:\n"
    "    fldi0   fr3\n"
    "    fcmp/eq fr3,fr7\n"
    "    bt      2f\n"
    "    fldi1   fr3\n"
    "    fdiv    fr7,fr3\n"
    "2:\n"
    "    mova    9f,r0\n"
    "    fmov.s  @r0+,fr9\n"
    "    fmov.s  @r0,fr10\n"
    "    fldi1   fr8\n"
    "    fmov    fr4,fr0\n"
    "    fmac    fr0,fr3,fr8\n"
    "    fmov    fr8,fr0\n"
    "    fmul    fr9,fr0\n"
    "    mov     r7,r1\n"
    "    fmov.s  @r1+,fr1\n"
    "    fmov.s  @r1+,fr2\n"
    "    fmov.s  @r1+,fr8\n"
    "    fmac    fr0,fr8,fr1\n"
    "    fmov.s  @r1+,fr11\n"
    "    fneg    fr5\n"
    "    fldi1   fr8\n"
    "    fmov    fr3,fr0\n"
    "    fmac    fr0,fr5,fr8\n"
    "    fmov    fr8,fr0\n"
    "    fmul    fr9,fr0\n"
    "    fmac    fr0,fr11,fr2\n"
    "    fmov.s  @r1,fr8\n"
    "    fldi0   fr0\n"
    "    fcmp/gt fr0,fr8\n"
    "    bt      3f\n"
    "    fadd    fr10,fr6\n"
    "    fldi1   fr3\n"
    "    fdiv    fr6,fr3\n"
    "3:\n"
    "    mov     r6,r1\n"
    "    add     #28,r1\n"
    "    fmov.s  fr3,@-r1\n"
    "    fmov.s  fr2,@-r1\n"
    "    fmov.s  fr1,@-r1\n"
    "    bra     5f\n"
    "    nop\n"
    "    .align 2\n"
    "9:  .long   0x3f000000\n"
    "    .long   0x40000000\n"
    "5:\n"
    "    mov.l   @r15+,r1\n"
    "    mov.l   @r15+,r5\n"
    "    rts\n"
    "    mov.l   r1,@r5\n"
    /* dcm_asm_lslot(a, N, v, lit): when N->ep is not the palette entry's epoch: view-space normal (envmap x, y),
     * per light max(0, -L.n), N->ep, N->cid = 0; then when lit: N->col = lit colour, N->cid = a->cid */
    "    .align 2\n"
    "    .global _dcm_asm_lslot\n"
    "_dcm_asm_lslot:\n"
    "    fmov.s  fr12,@-r15\n"
    "    mov.l   r8,@-r15\n"
    "    mov.l   r9,@-r15\n"
    "    mov.l   r10,@-r15\n"
    "    mov     r4,r8\n"
    "    mov     r5,r9\n"
    "    mov     r7,r10\n"
    "    mov     r6,r1\n"
    "    add     #26,r1\n"
    "    mov.b   @r1,r0\n"
    "    extu.b  r0,r0\n"
    "    mov.l   @(12,r8),r1\n"
    "    mov     r0,r2\n"
    "    shll2   r2\n"
    "    add     r2,r1\n"
    "    mov.l   @r1,r3\n"
    "    mov.l   @r9,r2\n"
    "    cmp/eq  r2,r3\n"
    "    bt      6f\n"
    "    mov.l   r3,@r9\n"
    "    mov     #0,r2\n"
    "    mov.l   r2,@(4,r9)\n"
    "    mov     #100,r1\n"
    "    mulu.w  r1,r0\n"
    "    mov.l   @r8,r4\n"
    "    sts     macl,r0\n"
    "    add     r0,r4\n"
    "    add     #64,r4\n"
    "    mov     r6,r5\n"
    "    add     #20,r5\n"
    "    mov.w   @r5+,r1\n"
    "    lds     r1,fpul\n"
    "    float   fpul,fr5\n"
    "    mov.w   @r5+,r1\n"
    "    lds     r1,fpul\n"
    "    float   fpul,fr2\n"
    "    mov.w   @r5,r1\n"
    "    lds     r1,fpul\n"
    "    float   fpul,fr1\n"
    "    mova    8f,r0\n"
    "    fmov.s  @r0,fr3\n"
    "    fmul    fr3,fr2\n"
    "    fmul    fr3,fr5\n"
    "    fmul    fr3,fr1\n"
    "    fmov.s  @r4+,fr6\n"
    "    fmov.s  @r4+,fr3\n"
    "    fmul    fr2,fr3\n"
    "    fmov.s  @r4+,fr7\n"
    "    fmov    fr5,fr0\n"
    "    fmac    fr0,fr6,fr3\n"
    "    fmov    fr1,fr0\n"
    "    fmac    fr0,fr7,fr3\n"
    "    fmov.s  @r4+,fr6\n"
    "    fmov.s  @r4+,fr4\n"
    "    fmul    fr2,fr4\n"
    "    fmov.s  @r4+,fr7\n"
    "    fmov    fr5,fr0\n"
    "    fmac    fr0,fr6,fr4\n"
    "    fmov    fr1,fr0\n"
    "    fmac    fr0,fr7,fr4\n"
    "    fmov.s  @r4+,fr6\n"
    "    fmov.s  @r4+,fr8\n"
    "    fmul    fr8,fr2\n"
    "    fmov.s  @r4,fr7\n"
    "    fmov    fr5,fr0\n"
    "    fmac    fr0,fr6,fr2\n"
    "    fmov    fr1,fr0\n"
    "    fmac    fr0,fr7,fr2\n"
    "    mov.l   @(40,r8),r1\n"
    "    mov     r9,r2\n"
    "    sub     r1,r2\n"
    "    shlr2   r2\n"
    "    mov.l   @(36,r8),r1\n"
    "    add     r2,r1\n"
    "    add     #8,r1\n"
    "    fmov.s  fr4,@-r1\n"
    "    fmov.s  fr3,@-r1\n"
    "    fmov    fr4,fr1\n"
    "    fmul    fr4,fr1\n"
    "    fmov    fr3,fr0\n"
    "    fmac    fr0,fr3,fr1\n"
    "    fmov    fr2,fr0\n"
    "    fmac    fr0,fr2,fr1\n"
    "    fsqrt   fr1\n"
    "    fldi0   fr5\n"
    "    fcmp/gt fr5,fr1\n"
    "    bf      1f\n"
    "    fldi1   fr5\n"
    "    fdiv    fr1,fr5\n"
    "    fmul    fr5,fr3\n"
    "    fmul    fr5,fr4\n"
    "    fmul    fr5,fr2\n"
    "1:\n"
    "    mov     r9,r2\n"
    "    add     #12,r2\n"
    "    mov.l   @(16,r8),r6\n"
    "    mov.l   @(20,r8),r7\n"
    "    cmp/pl  r7\n"
    "    bf      6f\n"
    "    fldi0   fr6\n"
    "2:\n"
    "    fmov.s  @r6+,fr0\n"
    "    fmov.s  @r6+,fr1\n"
    "    fmul    fr4,fr1\n"
    "    fmov.s  @r6+,fr5\n"
    "    fmac    fr0,fr3,fr1\n"
    "    fmov    fr5,fr0\n"
    "    fmac    fr0,fr2,fr1\n"
    "    fcmp/gt fr6,fr1\n"
    "    fneg    fr1\n"
    "    bf      4f\n"
    "    fldi0   fr1\n"
    "4:\n"
    "    fmov.s  fr1,@r2\n"
    "    dt      r7\n"
    "    bf/s    2b\n"
    "    add     #4,r2\n"
    "6:\n"
    "    tst     r10,r10\n"
    "    bt      7f\n"
    "    mov.l   @(24,r8),r4\n"
    "    mov     r9,r5\n"
    "    add     #12,r5\n"
    "    mov     r4,r6\n"
    "    add     #36,r6\n"
    "    mov.l   @(28,r8),r7\n"
    "    fmov.s  @r4+,fr1\n"
    "    fmov.s  @r4+,fr2\n"
    "    fmov.s  @r4+,fr3\n"
    "    fmov.s  @r4+,fr4\n"
    "    fmov.s  @r4+,fr5\n"
    "    fmov.s  @r4+,fr6\n"
    "    fmov.s  @r4+,fr7\n"
    "    fmov.s  @r4+,fr8\n"
    "    fmov.s  @r4,fr9\n"
    "    mov     r6,r1\n"
    "    add     #16,r1\n"
    "    cmp/pl  r7\n"
    "    bf      2f\n"
    "1:\n"
    "    mov.l   @r6+,r0\n"
    "    shll2   r0\n"
    "    fmov.s  @(r0,r5),fr10\n"
    "    fmov    fr7,fr12\n"
    "    fmov    fr4,fr0\n"
    "    fmac    fr0,fr10,fr12\n"
    "    fmov.s  @r1+,fr0\n"
    "    fmac    fr0,fr12,fr1\n"
    "    fmov    fr8,fr12\n"
    "    fmov    fr5,fr0\n"
    "    fmac    fr0,fr10,fr12\n"
    "    fmov.s  @r1+,fr0\n"
    "    fmac    fr0,fr12,fr2\n"
    "    fmov    fr9,fr12\n"
    "    fmov    fr6,fr0\n"
    "    fmac    fr0,fr10,fr12\n"
    "    fmov.s  @r1+,fr0\n"
    "    dt      r7\n"
    "    bf/s    1b\n"
    "    fmac    fr0,fr12,fr3\n"
    "2:\n"
    "    fldi1   fr10\n"
    "    fcmp/gt fr10,fr1\n"
    "    bf      3f\n"
    "    fmov    fr10,fr1\n"
    "3:\n"
    "    fcmp/gt fr10,fr2\n"
    "    bf      4f\n"
    "    fmov    fr10,fr2\n"
    "4:\n"
    "    fcmp/gt fr10,fr3\n"
    "    bf      5f\n"
    "    fmov    fr10,fr3\n"
    "5:\n"
    "    mova    9f,r0\n"
    "    fmov.s  @r0,fr11\n"
    "    fmul    fr11,fr1\n"
    "    ftrc    fr1,fpul\n"
    "    sts     fpul,r2\n"
    "    shll16  r2\n"
    "    fmul    fr11,fr2\n"
    "    ftrc    fr2,fpul\n"
    "    sts     fpul,r3\n"
    "    shll8   r3\n"
    "    or      r3,r2\n"
    "    fmul    fr11,fr3\n"
    "    ftrc    fr3,fpul\n"
    "    sts     fpul,r3\n"
    "    or      r3,r2\n"
    "    mov.l   r2,@(8,r9)\n"
    "    mov.l   @(32,r8),r1\n"
    "    mov.l   r1,@(4,r9)\n"
    "7:\n"
    "    mov.l   @r15+,r10\n"
    "    mov.l   @r15+,r9\n"
    "    mov.l   @r15+,r8\n"
    "    rts\n"
    "    fmov.s  @r15+,fr12\n"
    "    .align 2\n"
    "8:  .long   0x38800000\n"
    "9:  .long   0x437f0000\n"
);
#endif

/* slot misses, out of line: the per-vertex loops keep few live values */
#ifndef _arch_dreamcast
static void pslot_fill(const dctx_t *X, pslot_t *P, const dcm_vertex_t *v) {
    xform_clip(&X->xf[v->palette], v, P->c);
    if (P->c[3] >= X->near_w)
        project(P->c, X->cam, P->s);
    P->stamp = X->cs;
}

static void nslot_fill_x(const dctx_t *X, nslot_t *N, const dcm_vertex_t *v) {
    nslot_fill(N, X->env[N - X->ns], v, &X->xf[v->palette], X->ldir, X->nl);
    N->ep = X->pep[v->palette];
    N->cid = 0;
}

static void lit_fill(const dctx_t *X, nslot_t *N, const dcm_vertex_t *v) {
    if (N->ep != X->pep[v->palette])
        nslot_fill_x(X, N, v);
    N->col = lit_rgb(X->S, N);
    N->cid = X->cid;
}
#endif

static uint32_t __attribute__((noinline)) vcol_rgb(const dcm_vertex_t *v) {
    return pack_rgb(chan(v->color, 16), chan(v->color, 8), chan(v->color, 0));
}

/* colour of a vertex that is not lit through a normal slot */
static inline uint32_t unlit_rgb(uint32_t flat, const dcm_vertex_t *v) {
    return (v->flags & DCM_VF_COLOR) ? vcol_rgb(v) : flat;
}

#ifdef _arch_dreamcast
/* Test hook (DC): when non-NULL, the direct-render vertices go to this buffer (32 bytes each, as the
 * PVR receives them) instead of the store queues; dcm_dc_capture_n counts them. */
uint32_t *dcm_dc_capture;
uint32_t dcm_dc_capture_n;

/* All-in strips of one draw straight to the TA (PVR direct render through the store queues), from the
 * slots: screen position, lit colour, uv. */
typedef struct {
    const dcm_strip_t *st0, *st1;
    const dcm_vertex_t *verts;
    const uint16_t *vref;
    const pslot_t *ps;
    const nslot_t *ns;
    const float (*env)[2];
    uint32_t sa, flat;
    float ua, ue, ud, uf, near_w;
    int front;
    /* lazy (whole model in front): slots filled here instead of in the passes */
    const actx_t *A;
    pslot_t *wps;
    nslot_t *wns;
    int wrote;
} emit_t;

#define STF(p, f) __asm__ __volatile__("fmov.s %1,@-%0" : "+r"(p) : "f"(f))

static inline __attribute__((always_inline)) int emit_draw(emit_t *E, const int LIT, const int ENV,
                                                          const int CAP, const int LAZY) {
    const dcm_strip_t *st;
    const pslot_t *ps = E->ps;
    const nslot_t *ns = E->ns;
    uint32_t sa = E->sa, flat = E->flat;
    uint32_t dr_addr = pvr_dr_addr, *cap = CAP ? dcm_dc_capture + 8 * dcm_dc_capture_n : NULL;
    float ua = E->ua, ue = E->ue, ud = E->ud, uf = E->uf, near_w = E->near_w;
    int sent = 0;
    for (st = E->st0; st < E->st1; st++) {
        const dcm_vertex_t *v = E->verts + st->first_vertex;
        const uint16_t *vr = E->vref + 2 * st->first_vertex;
        int k, n = st->count < DCM_STRIP_MAX ? st->count : DCM_STRIP_MAX;
        if (!E->front) {
            for (k = 0; k < n; k++)
                if (ps[vr[2 * k]].c[3] < near_w)
                    break;
            if (k < n)
                break; /* crosses the near plane: the caller clips it, then goes on from the next strip */
        }
        for (k = n; k > 0; k--, v++, vr += 2) {
            const float *sp = ps[vr[0]].s;
            uint32_t col, *pv;
            float tu, tv, sx, sy, sz, *fp;
            if (LAZY) {
                pslot_t *P = &E->wps[vr[0]];
                if (P->stamp != E->A->cs) {
                    dcm_asm_pslot(E->A, P, v);
                    PROF(g_prof_xf++);
                }
            }
            if (LIT && (v->flags & DCM_VF_NORMAL)) {
                if (LAZY) {
                    nslot_t *N = &E->wns[vr[1]];
                    uint32_t cid = E->A->cid;
                    if (N->cid != cid || !cid || N->ep != E->A->pep[v->palette]) {
                        dcm_asm_lslot(E->A, N, v, 1);
                        E->wrote = 1;
                        PROF(g_prof_lit++);
                    }
                }
                col = sa | ns[vr[1]].col;
            } else {
                col = sa | unlit_rgb(flat, v);
            }
            if (ENV) {
                const float *e = E->env[vr[1]];
                tu = 0.5f + 0.5f * e[0];
                tv = 0.5f - 0.5f * e[1];
            } else {
                const float *uv = &v->u;
                float u0 = LDF(uv), v0 = LDF(uv);
                tu = ua * u0 + ue;
                tv = ud * v0 + uf;
            }
            sx = LDF(sp);
            sy = LDF(sp);
            sz = LDF(sp);
            if (CAP) {
                pv = cap;
                cap += 8;
            } else {
                pv = (uint32_t *)(dr_addr ^= 32);
            }
            fp = (float *)(pv + 6);
            pv[0] = k == 1 ? PVR_CMD_VERTEX_EOL : PVR_CMD_VERTEX;
            pv[6] = col;
            pv[7] = 0;
            STF(fp, tv);
            STF(fp, tu);
            STF(fp, sz);
            STF(fp, sy);
            STF(fp, sx);
            if (!CAP)
                __asm__ __volatile__("pref @%0" : : "r"(pv) : "memory");
        }
        sent += n - 2;
        PROF(g_prof_emit += n);
    }
    E->st0 = st;
    if (CAP)
        dcm_dc_capture_n = (uint32_t)(cap - dcm_dc_capture) / 8;
    else
        pvr_dr_addr = dr_addr;
    return sent;
}

#define EMIT_FN(name, L, V, C, Z)                                                                                \
    static int __attribute__((noinline)) name(emit_t *E) { return emit_draw(E, L, V, C, Z); }
EMIT_FN(emit_u, 0, 0, 0, 0)
EMIT_FN(emit_l, 1, 0, 0, 0)
EMIT_FN(emit_ue, 0, 1, 0, 0)
EMIT_FN(emit_le, 1, 1, 0, 0)
EMIT_FN(emit_u_c, 0, 0, 1, 0)
EMIT_FN(emit_l_c, 1, 0, 1, 0)
EMIT_FN(emit_ue_c, 0, 1, 1, 0)
EMIT_FN(emit_le_c, 1, 1, 1, 0)
EMIT_FN(emit_zu, 0, 0, 0, 1)
EMIT_FN(emit_zl, 1, 0, 0, 1)
EMIT_FN(emit_zu_c, 0, 0, 1, 1)
EMIT_FN(emit_zl_c, 1, 0, 1, 1)
#endif

/* one strip, generic path: all in front -> submitted, else clipped triangle by triangle */
static inline __attribute__((always_inline)) int strip_out(const dctx_t *X, const dcm_strip_t *st, int material,
                                                          const int LIT, const int ENV, const int FRONT) {
    static cvert_t sv[DCM_STRIP_MAX];
    const dcm_vertex_t *v = &X->m->vertices[st->first_vertex];
    const uint16_t *vr = X->m->vref + 2 * st->first_vertex;
    const pslot_t *ps = X->ps;
    const nslot_t *ns = X->ns;
    int k, n = st->count < DCM_STRIP_MAX ? st->count : DCM_STRIP_MAX, all_in = 1, sent = 0;
    if (!FRONT) {
        float near_w = X->near_w;
        for (k = 0; k < n; k++)
            if (ps[vr[2 * k]].c[3] < near_w)
                all_in = 0;
    }
    if (all_in) {
        for (k = 0; k < n; k++, v++, vr += 2) {
            const float *sp = ps[vr[0]].s;
            uint32_t col;
            float tu, tv, sx, sy, sz;
            if (LIT && (v->flags & DCM_VF_NORMAL))
                col = X->sa | ns[vr[1]].col;
            else
                col = X->sa | unlit_rgb(X->S->flat, v);
            if (ENV) {
                const float *e = X->env[vr[1]];
                tu = 0.5f + 0.5f * e[0];
                tv = 0.5f - 0.5f * e[1];
            } else {
                tu = X->ua * v->u + X->ue;
                tv = X->ud * v->v + X->uf;
            }
            sx = LDF(sp);
            sy = LDF(sp);
            sz = LDF(sp);
            submit(material, sx, sy, sz, tu, tv, col, k == n - 1);
        }
        return n - 2;
    }
    for (k = 0; k < n; k++) {
        const dcm_vertex_t *vk = &v[k];
        cvert_t *o = &sv[k];
        memcpy(o->c, ps[vr[2 * k]].c, sizeof(o->c));
        if (LIT && (vk->flags & DCM_VF_NORMAL))
            o->col = X->sa | ns[vr[2 * k + 1]].col;
        else
            o->col = X->sa | unlit_rgb(X->S->flat, vk);
        if (ENV) {
            const float *e = X->env[vr[2 * k + 1]];
            o->u = 0.5f + 0.5f * e[0];
            o->v = 0.5f - 0.5f * e[1];
        } else {
            o->u = X->ua * vk->u + X->ue;
            o->v = X->ud * vk->v + X->uf;
        }
    }
    /* the strip crosses the near plane: its triangles one by one, clipped (odd triangles of a
     * strip are wound the other way: swap their first two vertices) */
    for (k = 0; k + 2 < n; k++) {
        if (k & 1)
            sent += clip_tri(material, &sv[k + 1], &sv[k], &sv[k + 2], X->cam);
        else
            sent += clip_tri(material, &sv[k], &sv[k + 1], &sv[k + 2], X->cam);
    }
    return sent;
}

/* The strips of one draw, in passes over the vertices: positions (slot misses), normals / lit colours (slot
 * misses), then submission.  FRONT: whole model in front of the near plane (no per-strip test). */
static inline __attribute__((always_inline)) int draw_strips(const dctx_t *X, const dcm_draw_t *dr, const int LIT,
                                                            const int ENV, const int FRONT) {
    const dcm_model_t *m = X->m;
    const dcm_strip_t *st0 = &m->strips[dr->first_strip], *st1 = st0 + dr->num_strips, *st;
    pslot_t *ps = X->ps;
    nslot_t *ns = X->ns;
    uint32_t cs = X->cs;
    int sent = 0, material = dr->material;
    lcache_t *lc = X->lc;
    struct dstate_s *D = &lc->ds[dr - m->draws];
#ifdef _arch_dreamcast
    actx_t A;
    A.xf = X->xf;
    A.vp = &X->cam->vp_x;
    A.cs = cs;
    A.pep = X->pep;
    A.ldir = &X->ldir[0][0];
    A.nl = X->nl;
    A.em = X->S->em;
    A.nused = X->S->n;
    A.cid = X->cid;
    A.env = X->env;
    A.ns = ns;
#define PSLOT_FILL(P, v) (dcm_asm_pslot(&A, P, v), PROF(g_prof_xf++))
#define LIT_FILL(N, v) (dcm_asm_lslot(&A, N, v, 1), PROF(g_prof_lit++))
#define NSLOT_FILL(N, v) dcm_asm_lslot(&A, N, v, 0)
    if (FRONT && !ENV && !dcm_host_emit) {
        /* whole model in front: slots filled on the way, in submission order */
        emit_t E;
        E.st0 = st0;
        E.st1 = st1;
        E.verts = m->vertices;
        E.vref = m->vref;
        E.ps = ps;
        E.ns = ns;
        E.env = (const float(*)[2])X->env;
        E.sa = X->sa;
        E.flat = X->S->flat;
        E.ua = X->ua;
        E.ue = X->ue;
        E.ud = X->ud;
        E.uf = X->uf;
        E.near_w = X->near_w;
        E.front = 1;
        E.A = &A;
        E.wps = ps;
        E.wns = ns;
        E.wrote = 0;
        if (dcm_dc_capture)
            sent = LIT ? emit_zl_c(&E) : emit_zu_c(&E);
        else
            sent = LIT ? emit_zl(&E) : emit_zu(&E);
        D->pgen = cs;
        D->allin = 1;
        if (LIT) {
            if (E.wrote)
                lc->wgen++;
            D->lgen = lc->lgen;
            D->cid = X->cid;
            D->wgen = lc->wgen;
        }
        return sent;
    }
#else
#define PSLOT_FILL(P, v) pslot_fill(X, P, v)
#define LIT_FILL(N, v) lit_fill(X, N, v)
#define NSLOT_FILL(N, v) nslot_fill_x(X, N, v)
#endif
#if defined(_arch_dreamcast) && defined(DCM_PROFILE)
    uint64_t pt0 = timer_us_gettime64(), pt1, pt2;
#endif
    {
        /* positions (when the transforms changed) and lighting (when normal matrices, lights, the colour key or
         * shared slot colours changed), one pass; the draw is in front when every vertex is (as the per-strip
         * test would find) */
        int do_pos = D->pgen != cs;
        int do_lit = (LIT || ENV) && (D->lgen != lc->lgen || D->cid != X->cid || D->wgen != lc->wgen || !X->cid);
        if (do_pos) {
            float near_w = X->near_w;
            int allin = 1;
            for (st = st0; st < st1; st++) {
                const dcm_vertex_t *v = &m->vertices[st->first_vertex];
                const uint16_t *vr = m->vref + 2 * st->first_vertex, *ve = vr + 2 * st->count;
                const uint16_t *vc = vr + 2 * DCM_STRIP_MAX; /* the strip cap of the submission */
                for (; vr < ve; vr += 2, v++) {
                    pslot_t *P = &ps[vr[0]];
                    if (P->stamp != cs)
                        PSLOT_FILL(P, v);
                    if (P->c[3] < near_w && vr < vc)
                        allin = 0;
                }
            }
            D->pgen = cs;
            D->allin = allin;
        }
        if (do_lit) {
            uint32_t cid = X->cid, wrote = 0;
            const uint32_t *pep = X->pep;
            for (st = st0; st < st1; st++) {
                const dcm_vertex_t *v = &m->vertices[st->first_vertex];
                const uint16_t *vr = m->vref + 2 * st->first_vertex, *ve = vr + 2 * st->count;
                for (; vr < ve; vr += 2, v++) {
                    nslot_t *N = &ns[vr[1]];
                    if (LIT && (v->flags & DCM_VF_NORMAL)) {
                        if (N->cid != cid || !cid || N->ep != pep[v->palette]) {
                            LIT_FILL(N, v);
                            wrote = 1;
                        }
                    } else if (ENV && N->ep != pep[v->palette]) {
                        NSLOT_FILL(N, v);
                        wrote = 1;
                    }
                }
            }
            if (wrote)
                lc->wgen++;
            D->lgen = lc->lgen;
            D->cid = cid;
            D->wgen = lc->wgen;
        }
    }
#if defined(_arch_dreamcast) && defined(DCM_PROFILE)
    pt1 = timer_us_gettime64();
    g_prof_ph[0] += pt1 - pt0;
#endif
#if defined(_arch_dreamcast) && defined(DCM_PROFILE)
    pt2 = pt1;
#endif
#ifdef _arch_dreamcast
    if (!dcm_host_emit) {
        emit_t E;
        E.st0 = st0;
        E.st1 = st1;
        E.verts = m->vertices;
        E.vref = m->vref;
        E.ps = ps;
        E.ns = ns;
        E.env = (const float(*)[2])X->env;
        E.sa = X->sa;
        E.flat = X->S->flat;
        E.ua = X->ua;
        E.ue = X->ue;
        E.ud = X->ud;
        E.uf = X->uf;
        E.near_w = X->near_w;
        E.front = FRONT || D->allin;
        for (;;) {
            if (dcm_dc_capture)
                sent += LIT ? (ENV ? emit_le_c(&E) : emit_l_c(&E)) : (ENV ? emit_ue_c(&E) : emit_u_c(&E));
            else
                sent += LIT ? (ENV ? emit_le(&E) : emit_l(&E)) : (ENV ? emit_ue(&E) : emit_u(&E));
            if (E.st0 >= st1) {
#if defined(_arch_dreamcast) && defined(DCM_PROFILE)
                g_prof_ph[2] += timer_us_gettime64() - pt2;
#endif
                return sent;
            }
            sent += strip_out(X, E.st0, material, LIT, ENV, FRONT); /* crosses the near plane */
            E.st0++;
        }
    }
#endif
    for (st = st0; st < st1; st++)
        sent += strip_out(X, st, material, LIT, ENV, FRONT);
    return sent;
}

int dcm_draw(const dcm_model_t *m, const dcm_mtx_t *pal, const dcm_mtx_t *model_mtx, const dcm_camera_t *cam,
             const uint8_t *vis, const dcm_lights_t *lights, const dcm_draw_opts_t *opts) {
    return dcm_draw_ex(m, pal, model_mtx, cam, vis, lights, opts, NULL);
}

int dcm_draw_ex(const dcm_model_t *m, const dcm_mtx_t *pal, const dcm_mtx_t *model_mtx, const dcm_camera_t *cam,
                const uint8_t *vis, const dcm_lights_t *lights, const dcm_draw_opts_t *opts,
                const dcm_matstate_t *states) {
    static shade_t S;
    lcache_t *lc = (lcache_t *)m->lcache;
    dcm_lights_t deflights;
    float ldir[4][3];
    int sent = 0, built = 0, front = 0, i;
    uint32_t d;
    dctx_t X;
#if defined(_arch_dreamcast) && defined(DCM_PROFILE)
    uint64_t prof_t0 = timer_us_gettime64();
    uint32_t prof_xf0 = g_prof_xf;
#endif
    if (!m->vref || !lc)
        return 0; /* not loaded */
    if (!lights) {
        dcm_lights_stage(&deflights, 0);
        lights = &deflights;
    }
    X.nl = lights->count < 4 ? lights->count : 4;
    for (i = 0; i < X.nl; i++) {
        memcpy(ldir[i], lights->dir[i], sizeof(ldir[0]));
        norm3(ldir[i]);
    }
    X.m = m;
    X.lc = lc;
    X.xf = (const xform_t *)lc->xf;
    X.cam = cam;
    X.ps = lc->ps;
    X.ns = lc->ns;
    X.env = lc->env;
    X.pep = lc->pep;
    X.S = &S;
    X.ldir = (const float(*)[3])ldir;
    X.near_w = cam->near_w;
    for (d = 0; d < m->hdr->num_draws; d++) {
        const dcm_draw_t *dr = &m->draws[d];
        const dcm_material_t *mat = &m->materials[dr->material];
        dcm_matstate_t tmp;
        const dcm_matstate_t *ms;
        int cls, envmap;
        uint32_t alpha;
        if (dr->vis_node != 0xFFFF) {
            int visible = vis ? vis[dr->vis_node] : dr->vis_default;
            if (!visible)
                continue;
        }
        /* NNSi_G3dFuncSbc_MAT_InternalDefault: polygon alpha 0 -> the material's shapes are skipped */
        if ((states ? states[dr->material].alpha : mat->alpha) == 0)
            continue;
        cls = states ? state_class(m, mat, &states[dr->material]) : class_of(m, mat, mat->alpha, mat->texture);
#ifdef _arch_dreamcast
        {
            int want = cls == DCM_CLASS_OP ? PVR_LIST_OP_POLY : cls == DCM_CLASS_TR ? PVR_LIST_TR_POLY : opts->pt_list;
            if (want != opts->list)
                continue;
        }
#endif
        if (states) {
            ms = &states[dr->material];
        } else {
            mat_static(m, dr->material, &tmp);
            ms = &tmp;
        }
#ifdef _arch_dreamcast
        pvr_prim(material_header(m, mat, ms, opts->list), sizeof(pvr_poly_hdr_t));
#else
        (void)cls;
#endif
        /* palette * model * view * projection, when something is drawn and the inputs changed */
        if (!built) {
            int lchg, xchg, p, np = m->hdr->num_palette;
            xform_t *xf = (xform_t *)lc->xf;
#if defined(_arch_dreamcast) && defined(DCM_PROFILE)
            uint64_t st0 = timer_us_gettime64();
#endif
            xchg = !lc->kvalid || memcmp(lc->kpal, pal, sizeof(dcm_mtx_t) * np) ||
                   memcmp(&lc->kmodel, model_mtx, sizeof(dcm_mtx_t)) || memcmp(&lc->kview, &cam->view, sizeof(dcm_mtx_t)) ||
                   memcmp(lc->kproj, cam->proj, sizeof(lc->kproj)) || memcmp(lc->kvp, &cam->vp_x, sizeof(lc->kvp)) ||
                   memcmp(&lc->knear, &cam->near_w, sizeof(float));
            if (xchg) {
                build_xforms(m, pal, model_mtx, cam, xf);
                memcpy(lc->kpal, pal, sizeof(dcm_mtx_t) * np);
                lc->kmodel = *model_mtx;
                lc->kview = cam->view;
                memcpy(lc->kproj, cam->proj, sizeof(lc->kproj));
                memcpy(lc->kvp, &cam->vp_x, sizeof(lc->kvp));
                lc->knear = cam->near_w;
                lc->kvalid = 1;
                if (++lc->xgen == 0) { /* generation wrap: forget every position slot */
                    memset(lc->ps, 0, sizeof(pslot_t) * m->num_upos);
                    memset(lc->ds, 0, sizeof(*lc->ds) * m->hdr->num_draws);
                    lc->xgen = 1;
                }
            }
            if (xchg)
                lc->kfront = model_in_front(m, xf, X.near_w);
            front = lc->kfront;
            X.cs = lc->xgen;
            built = 1;
            /* lighting slots stay valid while the lights and a palette entry's normal matrix repeat bitwise */
            lchg = lc->nl != X.nl || memcmp(lc->ldir, lights->dir, sizeof(float) * 3 * X.nl) ||
                   memcmp(lc->lcol, lights->col, sizeof(float) * 3 * X.nl);
            if (lchg) {
                lc->nl = X.nl;
                memcpy(lc->ldir, lights->dir, sizeof(float) * 3 * X.nl);
                memcpy(lc->lcol, lights->col, sizeof(float) * 3 * X.nl);
                lc->nkeys = 0;
            }
            for (p = 0; (lchg || xchg) && p < np; p++)
                if (lchg || !lc->pep[p] || memcmp(lc->nmat[p], xf[p].n, sizeof(lc->nmat[0]))) {
                    memcpy(lc->nmat[p], xf[p].n, sizeof(lc->nmat[0]));
                    if (++lc->gen == 0) { /* epoch wrap: forget every slot */
                        memset(lc->ns, 0, sizeof(nslot_t) * m->num_unrm);
                        memset(lc->pep, 0, sizeof(uint32_t) * m->hdr->num_palette);
                        lc->gen = 1;
                        p = -1;
                        continue;
                    }
                    lc->pep[p] = lc->gen;
                    lc->lgen++;
                }
#if defined(_arch_dreamcast) && defined(DCM_PROFILE)
            g_prof_ph[3] += timer_us_gettime64() - st0;
#endif
        }
        envmap = states ? (ms->texgen == 2) : (mat->flags & DCM_MF_ENVMAP) != 0;
        dcm_matstate_uv(m, dr->material, states ? ms : NULL, &X.ua, &X.ud, &X.ue, &X.uf);
        alpha = (uint32_t)ms->alpha * 255u / 31u;
        if (opts)
            alpha = alpha * opts->alpha_scale / 255u;
        shade_setup(&S, mat, ms, lights, alpha);
        X.sa = S.alpha;
        /* lit colours are shared by draws with the same material colours (colour key, unique per model) */
        X.cid = 0;
        if (S.lit) {
            for (i = 0; i < (int)lc->nkeys; i++)
                if (lc->key[i].dif == ms->diffuse && lc->key[i].amb == ms->ambient && lc->key[i].em == ms->emission &&
                    lc->key[i].mask == mat->light_mask)
                    break;
            if (i == (int)lc->nkeys) {
                if (lc->nkeys == LC_KEYS)
                    lc->nkeys = i = 0;
                lc->key[i].dif = ms->diffuse;
                lc->key[i].amb = ms->ambient;
                lc->key[i].em = ms->emission;
                lc->key[i].mask = mat->light_mask;
                if (++lc->cgen == 0)
                    lc->cgen = 1;
                lc->key[i].id = lc->cgen;
                lc->nkeys++;
            }
            X.cid = lc->key[i].id;
        }
        if (front) {
            if (S.lit)
                sent += envmap ? draw_strips(&X, dr, 1, 1, 1) : draw_strips(&X, dr, 1, 0, 1);
            else
                sent += envmap ? draw_strips(&X, dr, 0, 1, 1) : draw_strips(&X, dr, 0, 0, 1);
        } else {
            if (S.lit)
                sent += envmap ? draw_strips(&X, dr, 1, 1, 0) : draw_strips(&X, dr, 1, 0, 0);
            else
                sent += envmap ? draw_strips(&X, dr, 0, 1, 0) : draw_strips(&X, dr, 0, 0, 0);
        }
    }
#if defined(_arch_dreamcast) && defined(DCM_PROFILE)
    prof_add(prof_t0, m, prof_xf0);
#endif
    return sent;
}

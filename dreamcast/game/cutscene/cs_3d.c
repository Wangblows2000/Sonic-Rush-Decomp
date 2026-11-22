/*
 * 3D for the cutscenes: Model_Init / Model_SetResource / Model_SetAnimation /
 * Model_Animate / Model_Draw of the DS, on the native model runtime
 * (dreamcast/game/model: .dcm meshes, .dca joint and .dcv visibility sets
 * from nsbmd2dc.py). A frame's instances are recorded during the logic and
 * submitted per PVR list by cutscene_draw.
 *
 * Camera: G3i_PerspectiveW(sin, cos of the half fovy, aspect, near, far) +
 * G3i_LookAt(eye, up, at), in model units (fx32 / 4096). The DS frame is
 * 256x192 at 4:3; the view is the full 640x480 frame.
 * Lights: NNS_G3dGlbLightColor / LightVector, the vectors are given in world
 * space and turned with the camera (NNS_G3dGlbFlushP sends them under the
 * camera matrix).
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "cs_internal.h"
#ifdef _arch_dreamcast
#include <dc/pvr.h>
#endif

#define MAX_INST 48
static CsInst s_inst[MAX_INST];
static int s_ninst;
static dcm_camera_t s_cam;
static dcm_lights_t s_lights;
static int s_cam_ok;

int cs_model_load(CsModel *m, const char *file) {
    char path[200];
    u32 size;
    memset(m, 0, sizeof(*m));
    if (!file || !file[0])
        return -1;
    snprintf(path, sizeof(path), "%s/%s", g_cs_model_dir, file);
    m->blob = (u8 *)cs_load_file(path, &size);
    if (!m->blob) {
        printf("cutscene: missing model %s\n", path);
        return -1;
    }
#ifdef CS_HOST
    if (dcm_model_load(&m->mdl, m->blob, size, 0) != 0) {
#else
    if (dcm_model_load(&m->mdl, m->blob, size, 1) != 0) {
#endif
        printf("cutscene: bad model %s\n", path);
        free(m->blob);
        m->blob = NULL;
        return -1;
    }
    m->ok = 1;
    return 0;
}

void cs_model_free(CsModel *m) {
    cs3d_forget_model(m);   /* drawn this frame before it was released (scene change) */
    if (m->ok)
        dcm_model_free(&m->mdl);
    free(m->blob);
    memset(m, 0, sizeof(*m));
}

int cs_anim_load(CsAnimSet *a, const char *file) {
    char path[200];
    u32 size;
    memset(a, 0, sizeof(*a));
    if (!file || !file[0])
        return -1;
    snprintf(path, sizeof(path), "%s/%s", g_cs_model_dir, file);
    a->blob = (u8 *)cs_load_file(path, &size);
    if (!a->blob || dca_set_load(&a->set, a->blob, size) != 0) {
        printf("cutscene: missing anim %s\n", path);
        free(a->blob);
        a->blob = NULL;
        return -1;
    }
    return 0;
}

void cs_anim_free(CsAnimSet *a) {
    free(a->blob);
    memset(a, 0, sizeof(*a));
}

static void norm3(float v[3]) {
    float l = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (l > 0) {
        v[0] /= l;
        v[1] /= l;
        v[2] /= l;
    }
}
static void cross3(const float a[3], const float b[3], float o[3]) {
    o[0] = a[1] * b[2] - a[2] * b[1];
    o[1] = a[2] * b[0] - a[0] * b[2];
    o[2] = a[0] * b[1] - a[1] * b[0];
}

static void build_camera(dcm_camera_t *c, const CsCam *cc) {
    float z[3], x[3], y[3], t, r, n = cc->near_, f = cc->far_;
    int i;
    memset(c, 0, sizeof(*c));
    for (i = 0; i < 3; i++)
        z[i] = cc->eye[i] - cc->at[i];
    norm3(z);
    cross3(cc->up, z, x);
    norm3(x);
    cross3(z, x, y);
    dcm_mtx_identity(&c->view);
    for (i = 0; i < 3; i++) {
        c->view.m[i][0] = x[i];
        c->view.m[i][1] = y[i];
        c->view.m[i][2] = z[i];
    }
    c->view.m[3][0] = -(x[0] * cc->eye[0] + x[1] * cc->eye[1] + x[2] * cc->eye[2]);
    c->view.m[3][1] = -(y[0] * cc->eye[0] + y[1] * cc->eye[1] + y[2] * cc->eye[2]);
    c->view.m[3][2] = -(z[0] * cc->eye[0] + z[1] * cc->eye[1] + z[2] * cc->eye[2]);
    /* G3_Perspective: top = near * sin / cos */
    t = n * (float)cs_sin(cc->fovy) / (float)(cs_cos(cc->fovy) ? cs_cos(cc->fovy) : 1);
    r = t * cc->aspect;
    c->proj[0][0] = 2 * n / (2 * r);
    c->proj[1][1] = 2 * n / (2 * t);
    c->proj[2][2] = (f + n) / (n - f);
    c->proj[2][3] = -1.0f;
    c->proj[3][2] = 2 * f * n / (n - f);
    c->vp_x = 0;
    c->vp_y = 0;
    c->vp_w = 640;
    c->vp_h = 480;
    c->near_w = n * 0.5f;
}

void cs_lights_from_ds(dcm_lights_t *l, const u16 col[3], const s16 vec[3][3], const CsCam *cam) {
    dcm_camera_t c;
    int i, k;
    build_camera(&c, cam);
    memset(l, 0, sizeof(*l));
    l->count = 3;
    for (i = 0; i < 3; i++) {
        float d[3] = {vec[i][0] / 4096.0f, vec[i][1] / 4096.0f, vec[i][2] / 4096.0f};
        for (k = 0; k < 3; k++)
            l->dir[i][k] = d[0] * c.view.m[0][k] + d[1] * c.view.m[1][k] + d[2] * c.view.m[2][k];
        l->col[i][0] = (col[i] & 31) / 31.0f;
        l->col[i][1] = (col[i] >> 5 & 31) / 31.0f;
        l->col[i][2] = (col[i] >> 10 & 31) / 31.0f;
    }
}

void cs3d_begin(const CsCam *cam, const dcm_lights_t *lights) {
    build_camera(&s_cam, cam);
    if (lights)
        s_lights = *lights;
    else
        dcm_lights_stage(&s_lights, 0);
    s_cam_ok = 1;
}

void cs3d_add(const CsInst *inst) {
    if (s_ninst < MAX_INST && inst->model && inst->model->ok && !inst->hidden)
        s_inst[s_ninst++] = *inst;
}

/* a model freed after cs3d_add in the same frame (PlayerDemoModel_NextScene releases the scene
 * after PlayerDemoModel_DrawModels): its instances are dropped from this frame's list */
void cs3d_forget_model(const CsModel *m) {
    int i, n = 0;
    for (i = 0; i < s_ninst; i++)
        if (s_inst[i].model != m)
            s_inst[n++] = s_inst[i];
    s_ninst = n;
}

void cs3d_reset(void) {
    s_ninst = 0;
    s_cam_ok = 0;
}
int cs3d_count(void) { return s_ninst; }

static void inst_matrix(const CsInst *in, dcm_mtx_t *m) {
    int r, c;
    /* NNS_G3dGlbSetBaseScale / BaseRot / BaseTrans: v * S * R + T */
    for (r = 0; r < 3; r++)
        for (c = 0; c < 3; c++)
            m->m[r][c] = in->scale[r] * in->rot[r * 3 + c];
    m->m[3][0] = in->trans[0];
    m->m[3][1] = in->trans[1];
    m->m[3][2] = in->trans[2];
}

static void pose(const CsInst *in, dcm_mtx_t *pal, u8 *vis) {
    static dcm_local_t loc[DCM_MAX_JOINTS];
    const dcm_model_t *md = &in->model->mdl;
    if (in->jnt && in->jnt->set.hdr && in->jnt_idx >= 0 && in->jnt_idx < in->jnt->set.hdr->num_anims)
        dca_sample(&in->jnt->set, in->jnt_idx, in->frame, md, loc);
    else
        dcm_bind_locals(md, loc);
    dcm_pose(md, loc, pal);
    if (vis) {
        if (in->vis && in->vis->set.hdr && in->vis_idx >= 0 && in->vis_idx < in->vis->set.hdr->num_anims)
            dca_visibility(&in->vis->set, in->vis_idx, in->frame, md, vis);
        else
            dca_visibility(NULL, -1, 0, md, vis);
    }
}

int cs3d_joint_pos(const CsInst *in, const char *name, float out[3]) {
    static dcm_local_t loc[DCM_MAX_JOINTS];
    static dcm_mtx_t world[DCM_MAX_JOINTS];
    dcm_mtx_t m;
    const dcm_model_t *md;
    float p[3];
    (void)name;
    if (!in->model || !in->model->ok)
        return 0;
    md = &in->model->mdl;
    if (in->jnt && in->jnt->set.hdr && in->jnt_idx >= 0 && in->jnt_idx < in->jnt->set.hdr->num_anims)
        dca_sample(&in->jnt->set, in->jnt_idx, in->frame, md, loc);
    else
        dcm_bind_locals(md, loc);
    dcm_world(md, loc, world);
    inst_matrix(in, &m);
    p[0] = world[0].m[3][0];
    p[1] = world[0].m[3][1];
    p[2] = world[0].m[3][2];
    out[0] = p[0] * m.m[0][0] + p[1] * m.m[1][0] + p[2] * m.m[2][0] + m.m[3][0];
    out[1] = p[0] * m.m[0][1] + p[1] * m.m[1][1] + p[2] * m.m[2][1] + m.m[3][1];
    out[2] = p[0] * m.m[0][2] + p[1] * m.m[1][2] + p[2] * m.m[2][2] + m.m[3][2];
    return 1;
}

/* the instance's material / pattern / SRT animations at its frame (looping over each set's length, as
 * the archive animations run with the loop flag); pattern and SRT need the model's textures
 * (NNS_G3dGetTex != NULL in PlayerDemoModel_LoadScene / TransformDemo_Load*) */
static int s_matanim_draws;   /* instances drawn with material animations (host tests) */
static const dcm_matstate_t *matstates(const CsInst *in) {
    static dcm_matstate_t st[128];
    const dcm_model_t *md = &in->model->mdl;
    int k, any = 0;
    if (!md->matx || md->hdr->num_materials > 128 || in->mat_idx < 0)
        return NULL;
    for (k = 0; k < 3; k++) {
        const CsAnimSet *a = in->mat[k];
        int nf, f;
        if (!a || !a->set.hdr || in->mat_idx >= a->set.hdr->num_anims || (k > 0 && md->hdr->num_textures == 0))
            continue;
        nf = a->set.anims[in->mat_idx].num_frames;
        f = (int)in->frame;
        if (nf > 0)
            f %= nf;
        if (!any) {
            dcm_matstate_init(md, st);
            any = 1;
        }
        dcm_matanim_apply(&a->set, in->mat_idx, f, md, st);
    }
    if (any)
        s_matanim_draws++;
    return any ? st : NULL;
}
int cs3d_matanim_draws(void) { return s_matanim_draws; }

void cs3d_draw_list(int list) {
    static dcm_mtx_t pal[DCM_MAX_PALETTE];
    static u8 vis[DCM_MAX_JOINTS];
    int i;
    if (!s_cam_ok)
        return;
    for (i = 0; i < s_ninst; i++) {
        const CsInst *in = &s_inst[i];
        dcm_mtx_t m;
        dcm_draw_opts_t o;
        memset(&o, 0, sizeof(o));
        pose(in, pal, vis);
        inst_matrix(in, &m);
        o.list = list;
#ifdef _arch_dreamcast
        o.pt_list = PVR_LIST_PT_POLY;
#else
        o.pt_list = 2;
#endif
        o.alpha_scale = in->alpha ? in->alpha : 255;
        dcm_draw_ex(&in->model->mdl, pal, &m, &s_cam, vis, &s_lights, &o, matstates(in));
    }
}

/* host tests: projected vertices of every recorded instance */
int cs3d_project_all(float *xyz, int max) {
    static dcm_mtx_t pal[DCM_MAX_PALETTE];
    int i, n = 0;
    if (!s_cam_ok)
        return 0;
    for (i = 0; i < s_ninst; i++) {
        const CsInst *in = &s_inst[i];
        dcm_mtx_t m;
        int nv = (int)in->model->mdl.hdr->num_vertices;
        if (n + nv > max)
            break;
        pose(in, pal, NULL);
        inst_matrix(in, &m);
        dcm_transform_vertices(&in->model->mdl, pal, &m, &s_cam, xyz + 3 * n);
        n += nv;
    }
    return n;
}

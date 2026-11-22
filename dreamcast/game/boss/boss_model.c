/*
 * Model runtime for the boss module: the parts of src/game/Graphics/Model/
 * the boss code drives (Model_Init, Model_SetResource, Model_SetAnimation,
 * Model_Animate/Model_AnimateMdl, Model_Draw's matrix ops) on top of the
 * converted G3D packages (dreamcast/game/model, .dcm/.dca).
 *
 * Model_AnimateMdl and the animation flag protocol (flags[i]: 1 paused,
 * 2 ended, 4 loop; speed @0x114 x speeds[i]) are kept as in the original,
 * because the boss state machines read them (unk334 & 2, unk30C->frame).
 * Model_Draw is split: boss_model_pose() runs where the DS draws (it builds
 * the matrix from the op list and poses the joints, so node-callback
 * positions such as the hammer's are available to the game logic), and
 * boss_model_draw() submits the stored pose to the PVR.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "boss_internal.h"

#ifdef _arch_dreamcast
#include <dc/pvr.h>
#endif

static char s_model_dir[96] = "/cd/models";
void boss_model_set_dir(const char *d) {
    strncpy(s_model_dir, d, sizeof(s_model_dir) - 1);
    s_model_dir[sizeof(s_model_dir) - 1] = 0;
}
int g_boss_upload_textures;
u8 *g_boss_debug_fb;     /* host only: boss_draw splats vertices here */
u32 g_boss_debug_color;

/* ------------------------------------------------------------ fx math */
fx32 FX_Div(fx32 n, fx32 d) {
    if (d == 0)
        return n >= 0 ? 0x7fffffff : (fx32)0x80000000;
    return (fx32)(((s64)n << 12) / d);
}
void MTX_Identity33(MtxFx33 *m) {
    memset(m, 0, sizeof(*m));
    m->m[0] = m->m[4] = m->m[8] = 0x1000;
}
void MTX_RotX33(MtxFx33 *m, fx32 s, fx32 c) {
    MTX_Identity33(m);
    m->m[4] = c;
    m->m[5] = s;
    m->m[7] = -s;
    m->m[8] = c;
}
void MTX_RotY33(MtxFx33 *m, fx32 s, fx32 c) {
    MTX_Identity33(m);
    m->m[0] = c;
    m->m[2] = -s;
    m->m[6] = s;
    m->m[8] = c;
}
void MTX_RotZ33(MtxFx33 *m, fx32 s, fx32 c) {
    MTX_Identity33(m);
    m->m[0] = c;
    m->m[1] = s;
    m->m[3] = -s;
    m->m[4] = c;
}
void MTX_Concat33(const MtxFx33 *a, const MtxFx33 *b, MtxFx33 *ab) {
    MtxFx33 t;
    int r, c;
    for (r = 0; r < 3; r++)
        for (c = 0; c < 3; c++)
            t.m[r * 3 + c] = FX_Mul(a->m[r * 3 + 0], b->m[0 * 3 + c]) + FX_Mul(a->m[r * 3 + 1], b->m[1 * 3 + c]) +
                             FX_Mul(a->m[r * 3 + 2], b->m[2 * 3 + c]);
    *ab = t;
}

/* ------------------------------------------------------ resources */
static void *slurp(const char *path, size_t *n) {
    FILE *f = fopen(path, "rb");
    void *b;
    long sz;
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    b = malloc((size_t)sz + 32);
    if (b == NULL || fread(b, 1, (size_t)sz, f) != (size_t)sz) {
        fclose(f);
        free(b);
        return NULL;
    }
    fclose(f);
    *n = (size_t)sz;
    return b;
}

int boss_res_load_model(BossModelRes *r, const char *file) {
    char path[192];
    size_t n;
    memset(r, 0, sizeof(*r));
    snprintf(path, sizeof(path), "%s/%s", s_model_dir, file);
    r->bytes = slurp(path, &n);
    if (r->bytes == NULL) {
        boss_log("missing", 0, 0);
        printf("boss: cannot load %s\n", path);
        return -1;
    }
    if (dcm_model_load(&r->mdl, r->bytes, n, g_boss_upload_textures) != 0) {
        printf("boss: bad model %s\n", path);
        free(r->bytes);
        r->bytes = NULL;
        return -1;
    }
    r->loaded = 1;
    return 0;
}

int boss_res_load_anim(BossAnimRes *r, const char *file) {
    char path[192];
    size_t n;
    memset(r, 0, sizeof(*r));
    snprintf(path, sizeof(path), "%s/%s", s_model_dir, file);
    r->bytes = slurp(path, &n);
    if (r->bytes == NULL) {
        printf("boss: cannot load %s\n", path);
        return -1;
    }
    if (dca_set_load(&r->set, r->bytes, n) != 0) {
        printf("boss: bad animation set %s\n", path);
        free(r->bytes);
        r->bytes = NULL;
        return -1;
    }
    r->loaded = 1;
    return 0;
}

void boss_res_free_model(BossModelRes *r) {
    if (r->loaded)
        dcm_model_free(&r->mdl);
    free(r->bytes);
    memset(r, 0, sizeof(*r));
}
void boss_res_free_anim(BossAnimRes *r) {
    free(r->bytes);
    memset(r, 0, sizeof(*r));
}

/* ----------------------------------------------------------- Model_* */
/* Model_Init (reused: same defaults) */
void Model_Init(BossModel *s, u32 a) {
    u32 i;
    free(s->palette);
    memset(s, 0, sizeof(*s));
    s->type = 0;
    s->flag = a;
    s->cb[0] = MOP_FLUSHP; /* s->unkA = 3 */
    s->scale[0] = s->scale[1] = s->scale[2] = 0x1000;
    MTX_Identity33(&s->rot);
    for (i = 0; i < 5; i++)
        s->speeds[i] = 0x1000;
    s->speed = 0x1000;
}

/* Model_SetResource: bind the converted model (the NNS render object) */
void Model_SetResource(BossModel *s, const BossModelRes *res) {
    s->res = res;
    free(s->palette);
    s->palette = NULL;
    if (res != NULL && res->loaded)
        s->palette = (dcm_mtx_t *)calloc(res->mdl.hdr->num_palette ? res->mdl.hdr->num_palette : 1, sizeof(dcm_mtx_t));
}

void boss_model_release(BossModel *m) { /* Model_Release */
    free(m->palette);
    m->palette = NULL;
    m->res = NULL;
}

/* Model_SetAnimation.  Only slot 0 (joint animation, .nsbca) is converted;
 * material (.nsbma), pattern (.nsbtp), SRT (.nsbta) and visibility slots are
 * accepted but stay empty (MODELS.md section 8).  The 0x40 cross-fade branch
 * is not used by any boss. */
void Model_SetAnimation(BossModel *o, u32 slot, const BossAnimRes *res, u32 idx) {
    BossAnmObj *a;
    if (slot >= 5)
        return;
    o->anm[slot] = NULL;
    if (slot != 0 || res == NULL || !res->loaded || idx >= res->set.hdr->num_anims) {
        o->flags[slot] &= ~3;
        return;
    }
    a = &o->anm_obj[slot];
    a->frame = 0;          /* NNS_G3dAnmObjInit */
    a->ratio = 0x1000;
    a->res = res;
    a->idx = (int)idx;
    a->numFrames = res->set.anims[idx].num_frames;
    o->anm[slot] = a;
    o->flags[slot] &= ~3;
}

/* Model_AnimateMdl (copied; d starts at 0 where the original reads an
 * uninitialised register for a paused slot) */
static void Model_AnimateMdl(BossModel *ctx, BossModel *o) {
    u32 i;
    fx32 d = 0;
    fx32 end;
    for (i = 0; i < 5; i++) {
        o->flags[i] &= ~2;
        if (o->anm[i] != NULL) {
            end = o->anm[i]->numFrames << 12;
            if (!(ctx->flag & 2) && !(o->flags[i] & 1)) {
                d = FX_Mul(o->speed, o->speeds[i]);
                o->anm[i]->frame += d;
            }
            if (d > 0) {
                if (o->anm[i]->frame >= end) {
                    o->flags[i] |= 2;
                    if (o->flags[i] & 4)
                        o->anm[i]->frame -= end;
                    else
                        o->anm[i]->frame = end - 1;
                }
            } else if (d < 0) {
                if (o->anm[i]->frame < 0) {
                    o->flags[i] |= 2;
                    if (o->flags[i] & 4)
                        o->anm[i]->frame += end;
                    else
                        o->anm[i]->frame = 0;
                }
            }
        }
    }
}

void Model_Animate(BossModel *s) {
    if (s->type == 0)
        Model_AnimateMdl(s, s);
    /* type 1 (shape, EffPebble): no animation */
}

/* ------------------------------------------------------------ posing */
static dcm_local_t s_locals[DCM_MAX_JOINTS];
static dcm_mtx_t s_world[DCM_MAX_JOINTS];

/* camera rotation for Model_MatrixOp_SetCameraRot (boss_camera.c) */
void boss_camera_view_rot(float r[9]);

void boss_model_pose(BossModel *m, const dcm_mtx_t *restored) {
    const dcm_model_t *md;
    int i, r, c, restore = 0, camrot = 0;
    float rot[9];
    if (m->res == NULL || !m->res->loaded)
        return;
    md = &m->res->mdl;
    for (i = 0; i < 8 && m->cb[i] != 0; i++) {
        if (m->cb[i] == MOP_RESTORE)
            restore = 1;
        if (m->cb[i] == MOP_SETCAMERAROT || m->cb[i] == MOP_SETCAMERAROT + 1) /* 0x14: billboard variant */
            camrot = 1;
    }
    if (restore && restored != NULL) {
        /* Model_MatrixOp_RestoreMtx: the position matrix is replaced by the stored one */
        m->mdl_mtx = *restored;
    } else {
        /* NNS_G3dGlbSetBaseScale/Rot/Trans + Flush: v * S * R * T (world, DS px) */
        if (camrot) {
            boss_camera_view_rot(rot); /* Model_TransposeRot(camera) */
        } else {
            for (i = 0; i < 9; i++)
                rot[i] = m->rot.m[i] / 4096.0f;
        }
        for (r = 0; r < 3; r++)
            for (c = 0; c < 3; c++)
                m->mdl_mtx.m[r][c] = rot[r * 3 + c] * (m->scale[r] / 4096.0f);
        for (c = 0; c < 3; c++)
            m->mdl_mtx.m[3][c] = m->trans[c] / 4096.0f;
    }
    /* joints: integer NNS frame (FX_Whole) of the slot-0 joint animation */
    if (m->anm[0] != NULL)
        dca_sample(&m->anm[0]->res->set, m->anm[0]->idx, (float)(m->anm[0]->frame >> 12), md, s_locals);
    else
        dcm_bind_locals(md, s_locals);
    dcm_world(md, s_locals, s_world);
    if (m->palette)
        dcm_pose(md, s_locals, m->palette);
    m->posed = 1;
}

/* NNS_G3dGetCurrentMtx after MTX_STORE in a timing-C NODEDESC callback:
 * joint world (model space) x model matrix. Valid right after boss_model_pose. */
void boss_model_joint_world(const BossModel *m, int joint, dcm_mtx_t *out) {
    if (m->res == NULL || joint < 0 || joint >= m->res->mdl.hdr->num_joints) {
        *out = m->mdl_mtx;
        return;
    }
    dcm_mtx_mul(out, &s_world[joint], &m->mdl_mtx);
}

int boss_model_draw(const BossModel *m, int list) {
    dcm_camera_t cam;
    dcm_draw_opts_t o;
    if (m->res == NULL || !m->res->loaded || !m->posed || (m->flag & 1))
        return 0;
    boss_camera(&cam);
#ifdef _arch_dreamcast
    o.list = list;
    o.pt_list = PVR_LIST_PT_POLY;
    o.alpha_scale = 255;
    return dcm_draw(&m->res->mdl, m->palette, &m->mdl_mtx, &cam, NULL, NULL, &o);
#else
    (void)o;
    if (list != 0)
        return 0;
    {
        static float xyz[3 * 4096];
        int n, i;
        if (m->res->mdl.hdr->num_vertices > 4096)
            return 0;
        n = dcm_transform_vertices(&m->res->mdl, m->palette, &m->mdl_mtx, &cam, xyz);
        if (g_boss_debug_fb) /* host debug view: splat the projected vertices (640x480 RGB) */
            for (i = 0; i < n; i++) {
                int x = (int)xyz[3 * i], y = (int)xyz[3 * i + 1];
                if (x >= 0 && x < 640 && y >= 0 && y < 480 && xyz[3 * i + 2] > 0) {
                    u8 *px = g_boss_debug_fb + 3 * (y * 640 + x);
                    px[0] = (u8)(g_boss_debug_color >> 16);
                    px[1] = (u8)(g_boss_debug_color >> 8);
                    px[2] = (u8)g_boss_debug_color;
                }
            }
        return n;
    }
#endif
}

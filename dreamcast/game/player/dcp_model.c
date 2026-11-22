/*
 * Player model presentation (Dreamcast port): Player_SetAnimation (model
 * part), Model_SetResource / Model_SetAnimation / Model_AnimateMdl,
 * StageObj_Draw3DEx for the player and Player_Draw (after-image).
 * See dcp_model.h and docs/dreamcast/translation/PLAYER.md section 8.
 */
#include "dcp_model.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _arch_dreamcast
#include <dc/pvr.h>
#endif

DcpModel g_dcp_model;

#define H g_dcp_hooks
#define W g_dcp_world

/* Player_Create: w->unk14C..154 = 0x34cc (model scale), w->unk18C = -0x10000
 * (Translate2 vector y, applied by Model_MatrixOp_RotateTranslate2Scale). */
#define PM_SCALE (0x34cc / 4096.0f)
#define PM_OFS_Y (-16.0f)

static void *slurp(const char *dir, const char *name, size_t *size) {
    char path[256];
    FILE *f;
    void *b;
    long n;
    snprintf(path, sizeof(path), "%s/%s", dir, name);
    f = fopen(path, "rb");
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    b = n > 0 ? malloc((size_t)n) : NULL;
    if (b && fread(b, 1, (size_t)n, f) != (size_t)n) {
        free(b);
        b = NULL;
    }
    fclose(f);
    *size = (size_t)n;
    return b;
}

void dcp_model_free(DcpModel *m) {
    int i;
    for (i = 0; i < 2; i++) {
        if (m->res[i].hdr)
            dcm_model_free(&m->res[i]);
        free(m->res_data[i]);
    }
    free(m->jnt_data);
    free(m->vis_data);
    memset(m, 0, sizeof(*m));
    m->jnt_anim = m->vis_anim = m->blend_anim = -1;
}

int dcp_model_load(DcpModel *m, int chara, const char *dir, int upload) {
    static const char *body[2] = {"son.dcm", "blz.dcm"};
    static const char *ball[2] = {"son_ball.dcm", "blz_spin.dcm"};
    size_t n;
    u8 *t;
    int i;
    dcp_model_free(m);
    m->chara = chara;
    m->res_data[0] = slurp(dir, body[chara ? 1 : 0], &n);
    if (!m->res_data[0] || dcm_model_load(&m->res[0], m->res_data[0], n, upload) != 0)
        return -1;
    m->res_data[1] = slurp(dir, ball[chara ? 1 : 0], &n);
    if (m->res_data[1] && dcm_model_load(&m->res[1], m->res_data[1], n, upload) != 0) {
        free(m->res_data[1]);
        m->res_data[1] = NULL;
        memset(&m->res[1], 0, sizeof(m->res[1]));
    }
    m->jnt_data = slurp(dir, "son.dca", &n);
    if (!m->jnt_data || dca_set_load(&m->jnt, m->jnt_data, n) != 0)
        return -2;
    m->vis_data = slurp(dir, "son.dcv", &n);
    if (m->vis_data && dca_set_load(&m->vis, m->vis_data, n) != 0) {
        free(m->vis_data);
        m->vis_data = NULL;
        memset(&m->vis, 0, sizeof(m->vis));
    }
    /* animtab.bin: the four Player_SetAnimation tables, ov000 0x02368bdc..0x02368d7c */
    t = slurp(dir, "animtab.bin", &n);
    if (t && n >= 3 * DCP_ANIMTAB_N) {
        memcpy(m->tab_jnt, t, DCP_ANIMTAB_N);
        memcpy(m->tab_res, t + DCP_ANIMTAB_N, DCP_ANIMTAB_N);
        memcpy(m->tab_vis, t + 2 * DCP_ANIMTAB_N, DCP_ANIMTAB_N);
    } else {
        /* older discs: animmap.bin holds the joint table only; derive the resource from the
         * animation's joint count (ball animations cover the ball skeleton) and use com_gen00 */
        free(t);
        t = slurp(dir, "animmap.bin", &n);
        if (!t || n < DCP_ANIMTAB_N) {
            free(t);
            return -3;
        }
        memcpy(m->tab_jnt, t, DCP_ANIMTAB_N);
        for (i = 0; i < DCP_ANIMTAB_N; i++) {
            int a = m->tab_jnt[i];
            int ball_anim = a < m->jnt.hdr->num_anims && m->jnt.anims[a].num_joints != m->res[0].hdr->num_joints;
            m->tab_res[i] = (u8)ball_anim;
            m->tab_vis[i] = ball_anim ? 0 : 1;
        }
    }
    free(t);
    m->speed = 0x1000;
    m->blend_speed = 0x200; /* Player_InitState: w->unk124->unk12C = 0x200 */
    m->slot_speed = 0x1000; /* Model_Init: unk118[i] = 0x1000 */
    m->ok = 1;
    return 0;
}

/* --------------------------------------------------- Player_SetAnimation */
static int anim_index(const DcpModel *m, const DcPlayer *p, s32 anim) {
    s32 a = anim;
    if (p->chara != 0 && a >= 0x30)
        a = (s16)(a + p->chara * 0x1b);
    if (a < 0 || a >= DCP_ANIMTAB_N)
        a = 0;
    (void)m;
    return a;
}

void dcp_model_set_anim(DcpModel *m, DcPlayer *p, s32 anim) {
    int a, res;
    if (!m->ok)
        return;
    m->p = p;
    m->set_count++;
    a = anim_index(m, p, anim);
    res = m->tab_res[a];
    if (res > 1 || !m->res[res].hdr)
        res = 0;
    if (res != m->cur_res) {
        /* Model_SetResource: NNS_G3dRenderObjInit drops every attached animation object
         * (the visibility animation too); the blend request is cancelled. */
        m->cur_res = res;
        m->vis_anim = -1;
        m->blend_anim = -1;
        p->disp_flags &= ~0x400;
    }
    /* model+0x10C bit 0x40 <- the 0x400 blend request of the player */
    if (p->disp_flags & 0x400)
        m->parts0 |= 0x40;
    else
        m->parts0 &= ~0x40;
    /* Model_SetAnimation slot 0 */
    if ((m->parts0 & 0x40) && m->jnt_anim >= 0) {
        m->parts0 &= ~0x80;
        m->blend_anim = m->jnt_anim; /* anm2 = anm (keeps its frame, frozen) */
        m->blend_frame = m->jnt_frame;
        m->ratio = 0;
    } else {
        m->blend_anim = -1;
    }
    m->jnt_anim = m->tab_jnt[a];
    m->jnt_frame = 0;
    m->parts0 &= ~3;
    /* slot 4 only when the table has an entry */
    if (m->tab_vis[a] != 0) {
        m->vis_anim = m->tab_vis[a];
        m->vis_frame = 0;
        m->parts4 &= ~3;
    }
    m->speed = 0x1000;
}

void dcp_model_set_speed(DcpModel *m, s32 s) { m->speed = (s << 12) >> 4; }
void dcp_model_set_slot_speed(DcpModel *m, s32 fx) { m->slot_speed = fx; }

static DcpModel *s_hooked;
static void (*s_prev_anim)(void *ctx, DcPlayer *p, s32 anim);
static void (*s_prev_speed)(void *ctx, DcPlayer *p, s32 s);
static void *s_prev_ctx;
static void hook_anim(void *ctx, DcPlayer *p, s32 anim) {
    if (s_hooked && (s_hooked->p == NULL || s_hooked->p == p))
        dcp_model_set_anim(s_hooked, p, anim);
    if (s_prev_anim)
        s_prev_anim(s_prev_ctx ? s_prev_ctx : ctx, p, anim);
}
static void hook_speed(void *ctx, DcPlayer *p, s32 s) {
    if (s_hooked && s_hooked->p == p)
        dcp_model_set_speed(s_hooked, s);
    if (s_prev_speed)
        s_prev_speed(s_prev_ctx ? s_prev_ctx : ctx, p, s);
}

void dcp_model_attach(DcpModel *m, const DcPlayer *p) {
    s_hooked = m;
    m->p = p;
    if (H.anim != hook_anim) {
        s_prev_anim = H.anim;
        s_prev_speed = H.anim_speed;
        s_prev_ctx = H.ctx;
        H.anim = hook_anim;
        H.anim_speed = hook_speed;
    }
    m->cur_res = 0;
    m->jnt_anim = m->vis_anim = m->blend_anim = -1;
    m->parts0 = m->parts4 = 0;
    m->posed = 0;
    if (p && m->ok)
        dcp_model_set_anim(m, (DcPlayer *)p, p->anim);
}

int dcp_model_cur_anim(const DcpModel *m) { return m->jnt_anim; }

/* ------------------------------------------------------ Model_AnimateMdl */
static s32 fx_mul(s32 a, s32 b) { return (s32)(((long long)a * b + 0x800) >> 12); }

static void advance(s32 *frame, s32 d, int nframes, u8 *flags) {
    s32 end = nframes << 12;
    if (d <= 0 || end <= 0)
        return;
    *frame += d;
    if (*frame >= end) {
        *flags |= 2;
        if (*flags & 4)
            *frame -= end;
        else
            *frame = end - 1;
        if (*frame >= end) /* more than one cycle in one frame */
            *frame %= end;
    }
}

static void blend_locals(dcm_local_t *dst, const dcm_local_t *old, int n, float w) {
    int j, k;
    for (j = 0; j < n; j++) {
        dcm_local_t *d = &dst[j];
        const dcm_local_t *o = &old[j];
        float *r = d->r, l;
        for (k = 0; k < 3; k++) {
            d->t[k] = o->t[k] + (d->t[k] - o->t[k]) * w;
            d->s[k] = o->s[k] + (d->s[k] - o->s[k]) * w;
        }
        for (k = 0; k < 9; k++)
            r[k] = o->r[k] + (r[k] - o->r[k]) * w;
        /* NNS blend: rows 0 and 1 normalised, row 2 = row0 x row1 */
        l = sqrtf(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]);
        if (l > 0) { r[0] /= l; r[1] /= l; r[2] /= l; }
        l = sqrtf(r[3] * r[3] + r[4] * r[4] + r[5] * r[5]);
        if (l > 0) { r[3] /= l; r[4] /= l; r[5] /= l; }
        r[6] = r[1] * r[5] - r[2] * r[4];
        r[7] = r[2] * r[3] - r[0] * r[5];
        r[8] = r[0] * r[4] - r[1] * r[3];
    }
}

void dcp_model_step(DcpModel *m, DcPlayer *p, int paused) {
    u16 flags;
    s32 d, ts = W.timescale ? W.timescale : 0x100;
    int frozen = (p->hitstop & 0xf) || paused;
    const dcm_model_t *mdl;
    if (!m->ok)
        return;
    m->p = p;
    if (m->jnt_anim < 0)
        dcp_model_set_anim(m, p, p->anim);
    flags = p->disp_flags;
    /* StageObj_Main: hit-stop or pause -> flag 0x10 (no frame advance); h5c is restored after the draw */
    if (frozen)
        flags |= 0x10;
    /* StageObj_Draw3DEx: parts[i] &= ~5, |= 4 when the object loops; flags &= ~8 */
    m->parts0 &= ~5;
    m->parts4 &= ~5;
    if (flags & 4) {
        m->parts0 |= 4;
        m->parts4 |= 4;
    }
    flags &= ~8;
    if (!(flags & 0x1000)) {
        /* Model_AnimateMdl (speed @0x114 and blend speed @0x12C scaled by data_0207cf7c) */
        s32 speed = (m->speed * ts) >> 8;
        s32 bspeed = (m->blend_speed * ts) >> 8;
        m->parts0 &= ~2;
        m->parts4 &= ~2;
        if ((m->parts0 & 0x40) && m->blend_anim >= 0) {
            if (!(m->parts0 & 0x80))
                m->ratio += fx_mul(bspeed, 0x1000);
            if (0x1000 - m->ratio <= 0) {
                m->ratio = 0x1000;
                m->parts0 &= ~0x40;
                m->parts0 |= 0x80;
                m->blend_anim = -1;
            }
        }
        d = (flags & 0x10) ? 0 : fx_mul(speed, m->slot_speed);
        if (m->jnt_anim >= 0 && m->jnt_anim < m->jnt.hdr->num_anims)
            advance(&m->jnt_frame, d, m->jnt.anims[m->jnt_anim].num_frames, &m->parts0);
        if (m->vis_anim >= 0 && m->vis.hdr && m->vis_anim < m->vis.hdr->num_anims)
            advance(&m->vis_frame, d, m->vis.anims[m->vis_anim].num_frames, &m->parts4);
    }
    /* end of StageObj_Draw3DEx: blend finished -> clear 0x400; joint animation ended -> 8 */
    if (m->parts0 & 0x80)
        flags &= ~0x400;
    if (m->parts0 & 2) {
        flags &= ~0x400;
        flags |= 8;
        m->done_count++;
    }
    if (!frozen)
        p->disp_flags = flags;

    /* pose: integer NNS frames (NNS_G3dAnmObj samples frame >> 12) */
    mdl = &m->res[m->cur_res];
    if (!mdl->hdr)
        return;
    dca_sample(&m->jnt, m->jnt_anim, (float)(m->jnt_frame >> 12), mdl, m->loc);
    if ((m->parts0 & 0x40) && m->blend_anim >= 0) {
        dca_sample(&m->jnt, m->blend_anim, (float)(m->blend_frame >> 12), mdl, m->loc2);
        blend_locals(m->loc, m->loc2, mdl->hdr->num_joints, (float)m->ratio / 4096.0f);
    }
    dcm_pose(mdl, m->loc, m->pal);
    if (m->cur_res == 0 && m->vis_anim >= 0 && m->vis.hdr)
        dca_visibility(&m->vis, m->vis_anim, (float)(m->vis_frame >> 12), mdl, m->visbits);
    else
        memset(m->visbits, 1, mdl->hdr->num_joints);
    m->posed = 1;
}

/* ------------------------------------------------------ model matrix */
#define DS_ANGLE(a) ((float)(a) * (6.28318530718f / 65536.0f))

static void rot_y(dcm_mtx_t *m, u16 a) {
    float s = sinf(DS_ANGLE(a)), c = cosf(DS_ANGLE(a));
    dcm_mtx_identity(m);
    m->m[0][0] = c;  m->m[0][2] = -s; /* MTX_RotY33_ */
    m->m[2][0] = s;  m->m[2][2] = c;
}
static void rot_x(dcm_mtx_t *m, u16 a) {
    float s = sinf(DS_ANGLE(a)), c = cosf(DS_ANGLE(a));
    dcm_mtx_identity(m);
    m->m[1][1] = c;  m->m[1][2] = s;  /* MTX_RotX33_ */
    m->m[2][1] = -s; m->m[2][2] = c;
}
static void rot_z(dcm_mtx_t *m, u16 a) {
    float s = sinf(DS_ANGLE(a)), c = cosf(DS_ANGLE(a));
    dcm_mtx_identity(m);
    m->m[0][0] = c;  m->m[0][1] = s;  /* MTX_RotZ33_ */
    m->m[1][0] = -s; m->m[1][1] = c;
}

/* v * S * T(0,-16,0) * R + t  ->  rows S*R, translation t + (0,-16,0) * R */
static void finish(const dcm_mtx_t *r, float tx, float ty, float tz, dcm_mtx_t *out) {
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            out->m[i][j] = r->m[i][j] * PM_SCALE;
    out->m[3][0] = tx + PM_OFS_Y * r->m[1][0];
    out->m[3][1] = ty + PM_OFS_Y * r->m[1][1];
    out->m[3][2] = tz + PM_OFS_Y * r->m[1][2];
}

void dcp_model_matrix(const DcPlayer *p, float tx, float ty, float tz, dcm_mtx_t *out) {
    dcm_mtx_t r, t;
    u8 rz = p->angle;
    /* StageObj_Draw3DEx(x, y, z, obj->unk1, obj, mdl): rotZ += obj->rotZ (@5) when non-zero */
    if (p->grav_rot != 0)
        rz = (u8)(rz + p->grav_rot);
    rot_y(&r, (p->disp_flags & 1) ? 0xc000 : 0x4000);
    rot_x(&t, (u16)-(p->rot2 << 8));
    dcm_mtx_mul(&r, &r, &t);
    rot_y(&t, (u16)-(p->rot3 << 8));
    dcm_mtx_mul(&r, &r, &t);
    rot_z(&t, (u16)(-rz << 8));
    dcm_mtx_mul(&r, &r, &t);
    /* shake: trans.x += ofsX (@6), trans.y -= ofsY (@7) */
    finish(&r, tx + p->shake_x, ty - p->shake_y, tz, out);
}

void dcp_model_matrix_ext(const dcm_mtx_t *ext, dcm_mtx_t *out) {
    finish(ext, ext->m[3][0], ext->m[3][1], ext->m[3][2], out);
}

void dcp_model_lights(const DcPlayer *p, int ghost, dcm_lights_t *l) {
    static const u16 ghost_col[2] = {0x6182, 0x1a98};          /* Player_Draw (Sonic, Blaze) */
    static const u16 water_col[3] = {0x7ef5, 0x4548, 0x2040};  /* Player_Draw, pf & 0x4000000 */
    int i;
    /* flag 0x800 (Player_Create): facing left keeps the stage vectors, facing right mirrors x */
    dcm_lights_stage(l, (p->disp_flags & 0x800) ? (p->disp_flags & 1) : 0);
    for (i = 0; i < l->count && i < 3; i++) {
        u16 c = 0;
        if (ghost)
            c = ghost_col[p->chara == 1];
        else if (p->pf & 0x4000000)
            c = water_col[i];
        else
            continue;
        l->col[i][0] = (float)(c & 31) / 31.0f;
        l->col[i][1] = (float)((c >> 5) & 31) / 31.0f;
        l->col[i][2] = (float)((c >> 10) & 31) / 31.0f;
    }
}

/* ------------------------------------------- StageTitleCardPlayer_Draw */
/* BossCamera_Create values (data_ov000_0236a444, copied to the title card work at Create) */
#define TC_FOVY 0x1555
#define TC_ASPECT (0x1555 / 4096.0f)
#define TC_NEAR 1.0f
#define TC_FAR 2048.0f

void dcp_model_titlecard_view(const DcPlayer *p, const s32 eye[3], const s32 at[3], s32 mx, s32 my, int zoom,
                              float z0, float z1, dcm_camera_t *cam, dcm_mtx_t *mdl) {
    float e[3], t[3], zx[3], xx[3], yy[3], l, h, w, top, bot, lft, rgt, tx = 128.0f, ty = -96.0f;
    float dist, wmin, k;
    int i, j;
    dcm_mtx_t r, q;
    for (i = 0; i < 3; i++) {
        e[i] = eye[i] / 4096.0f;
        t[i] = at[i] / 4096.0f;
    }
    /* G3i_LookAt_: z = normalize(eye - at), x = normalize(up x z) with up = (0, 1, 0), y = z x x */
    for (i = 0; i < 3; i++)
        zx[i] = e[i] - t[i];
    l = sqrtf(zx[0] * zx[0] + zx[1] * zx[1] + zx[2] * zx[2]);
    if (l <= 0.0f)
        l = 1.0f;
    for (i = 0; i < 3; i++)
        zx[i] /= l;
    xx[0] = zx[2];
    xx[1] = 0.0f;
    xx[2] = -zx[0];
    l = sqrtf(xx[0] * xx[0] + xx[2] * xx[2]);
    if (l <= 0.0f)
        l = 1.0f;
    xx[0] /= l;
    xx[2] /= l;
    yy[0] = zx[1] * xx[2] - zx[2] * xx[1];
    yy[1] = zx[2] * xx[0] - zx[0] * xx[2];
    yy[2] = zx[0] * xx[1] - zx[1] * xx[0];
    memset(cam, 0, sizeof(*cam));
    for (i = 0; i < 3; i++) {
        cam->view.m[i][0] = xx[i];
        cam->view.m[i][1] = yy[i];
        cam->view.m[i][2] = zx[i];
    }
    cam->view.m[3][0] = -(xx[0] * e[0] + xx[1] * e[1] + xx[2] * e[2]);
    cam->view.m[3][1] = -(yy[0] * e[0] + yy[1] * e[1] + yy[2] * e[2]);
    cam->view.m[3][2] = -(zx[0] * e[0] + zx[1] * e[1] + zx[2] * e[2]);
    cam->vp_w = 640.0f;
    cam->vp_h = 480.0f;
    /* G3i_PerspectiveW_ (angle, aspect, near, far of the title card's camera copy) */
    h = tanf(DS_ANGLE(TC_FOVY)) * TC_NEAR;
    w = h * TC_ASPECT;
    top = h;
    bot = -h;
    lft = -w;
    rgt = w;
    if (zoom) {
        /* flags2 & 0x400: G3i_FrustumW_ shifted by the model position, d = near / stage eye z */
        float d = TC_NEAR / (96.0f * cosf(DS_ANGLE(TC_FOVY)) / sinf(DS_ANGLE(TC_FOVY)));
        float a = d * (my / 4096.0f + 96.0f);
        float b = d * (128.0f - mx / 4096.0f);
        top = h - a;
        bot = -(h + a);
        lft = b - w;
        rgt = w + b;
    } else {
        tx = mx / 4096.0f;
        ty = my / 4096.0f;
    }
    cam->proj[0][0] = 2 * TC_NEAR / (rgt - lft);
    cam->proj[1][1] = 2 * TC_NEAR / (top - bot);
    cam->proj[2][0] = (rgt + lft) / (rgt - lft);
    cam->proj[2][1] = (top + bot) / (top - bot);
    cam->proj[2][2] = (TC_FAR + TC_NEAR) / (TC_NEAR - TC_FAR);
    cam->proj[2][3] = -1.0f;
    cam->proj[3][2] = 2 * TC_FAR * TC_NEAR / (TC_NEAR - TC_FAR);
    /* depth band: w of the nearest vertex (model about 16 px around its centre) -> 1/w = z1 */
    dist = sqrtf((e[0] - tx) * (e[0] - tx) + (e[1] - ty) * (e[1] - ty) + e[2] * e[2]);
    wmin = dist - 16.0f;
    if (wmin < 2.0f)
        wmin = 2.0f;
    k = 1.0f / (z1 * wmin);
    if (z0 * (dist + 16.0f) * k > 1.0f)
        k = 1.0f / (z0 * (dist + 16.0f)) * 0.999f;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            cam->proj[i][j] *= k;
    cam->near_w = 0.5f * k;
    /* rotation: RotY(facing) * RotX(-rotX) * RotY(-rotY) * RotZ(-rotZ) (obj @5 only, no ground angle) */
    rot_y(&r, (p->disp_flags & 1) ? 0xc000 : 0x4000);
    rot_x(&q, (u16)-(p->rot2 << 8));
    dcm_mtx_mul(&r, &r, &q);
    rot_y(&q, (u16)-(p->rot3 << 8));
    dcm_mtx_mul(&r, &r, &q);
    rot_z(&q, (u16)(-p->grav_rot << 8));
    dcm_mtx_mul(&r, &r, &q);
    finish(&r, tx, ty, 0.0f, mdl);
}

/* ------------------------------------------------------ Player_Draw */
int dcp_model_afterimage(const DcPlayer *p, s32 *x, s32 *y, s32 *z) {
    s32 dx, dy;
    if (!(p->air_lock != 0 || ((p->state == dcp_state_homing || (p->pf & 0x100)) && !(p->pf & 0x80))))
        return 0;
    /* gGameState.unk24 & 1 (frame skip) is never set in this port: the previous position @0x1C */
    dx = p->x - p->px;
    dy = p->y - p->py;
    if ((dx < 0 ? -dx : dx) < 0x200 && (dy < 0 ? -dy : dy) < 0x200)
        return 0;
    *x = p->px;
    *y = p->py;
    *z = p->pz;
    return 1;
}

int dcp_model_draw(const DcpModel *m, const DcPlayer *p, int list, const dcm_mtx_t *mdl, const dcm_camera_t *cam,
                   const dcm_lights_t *lights) {
    dcm_draw_opts_t o;
    const dcm_model_t *r;
    if (!m->ok || !m->posed || (p->disp_flags & 0x20))
        return 0;
    r = &m->res[m->cur_res];
    if (!r->hdr)
        return 0;
    o.list = list;
#ifdef _arch_dreamcast
    o.pt_list = PVR_LIST_PT_POLY;
#else
    o.pt_list = 1;
#endif
    o.alpha_scale = 255;
    return dcm_draw(r, m->pal, mdl, cam, m->visbits, lights, &o);
}

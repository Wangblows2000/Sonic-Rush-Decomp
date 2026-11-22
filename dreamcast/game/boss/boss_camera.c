/*
 * BossCamera (src/game/Boss/BossCamera_*.c): the 3D camera of every stage
 * act (Stage_Create: BossCamera_Create(act == 2 ? 2 : 0)) and of the boss
 * arenas.  The fixed-point state update is copied from the decompilation;
 * the G3D calls (G3i_PerspectiveW_, G3i_LookAt_) become a dcm_camera_t built
 * from the same eye / at / up / fovy / aspect / near / far.
 */
#include <math.h>
#include <string.h>
#include <stddef.h>
#include "boss_internal.h"

BossCamera data_ov000_0236a444;
#define CAM data_ov000_0236a444

static inline void SetVec(BossVec *v, s32 x, s32 y, s32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}
static inline void VEC_Add(const BossVec *a, const BossVec *b, BossVec *o) {
    o->x = a->x + b->x;
    o->y = a->y + b->y;
    o->z = a->z + b->z;
}
static inline void VEC_Subtract(const BossVec *a, const BossVec *b, BossVec *o) {
    o->x = a->x - b->x;
    o->y = a->y - b->y;
    o->z = a->z - b->z;
}
static fx32 VEC_Mag(const BossVec *v) {
    double l2 = (double)v->x * v->x + (double)v->y * v->y + (double)v->z * v->z;
    return (fx32)sqrt(l2);
}

/* BossCamera_Create (reused; the task is the BossCamera_Main call in boss_update) */
void BossCamera_Create(s32 mode) {
    BossCamera *c = &CAM;
    memset(c, 0, offsetof(BossCamera, unkE0)); /* MIi_CpuClear16(0, c, 0xE0) */
    c->active = 1;
    c->unkDC = (u32)mode;
    c->unk4 = 0x1555;
    c->unk8 = 0x1000;
    c->unkC = 0x800000;
    c->unk10 = 0x1555;
    c->unk14 = 0x1000;
    SetVec(&c->unk18, 0, 0x64000, 0xa000);
    SetVec(&c->unk24, 0, 0, 0);
    SetVec(&c->unk30, 0, 0x1000, 0);
    c->t[1].lerp = 0x200;         /* unkB4 */
    c->unkC0 = c->unkC4 = 0xc8000;
    c->unkC8 = 0x200;
    c->unkCC = c->unkD0 = 0x3c000;
    c->unkD4 = 0x200;
    c->unkD6 = c->unkD8 = 0;
    c->unkDA = 0x200;
    c->t[0].lerp = 0x200;         /* unk78 */
}

void BossCamera_Destructor(void) {
    boss_model_release(&CAM.unkE0); /* Model_Release(&data_ov000_0236a524) */
    CAM.active = 0;
}

/* BossCamera_UpdateFixed (copied) */
static void BossCamera_UpdateFixed(void) {
    CAM.unk24.x = 0x80000;
    CAM.unk24.y = -0x60000;
    CAM.unk24.z = 0;
    CAM.unk18.x = 0x80000;
    CAM.unk18.y = -0x60000;
    CAM.unk18.z = FX_Div(COS(CAM.unk4), SIN(CAM.unk4)) * 0x60;
}

/* BossCamera_UpdateTrack0 (copied; w = &t[0] at 0x48) */
static void BossCamera_UpdateTrack0(void) {
    BossCamTrack *w0 = &CAM.t[0], *w1 = &CAM.t[1];
    BossVec d;
    s32 mag;
    VEC_Subtract(&w0->tgt, &w0->cur, &d);
    d.x = FX_Mul(d.x, w0->lerp);
    d.y = FX_Mul(d.y, w0->lerp);
    d.z = FX_Mul(d.z, w0->lerp);
    if (w0->accel > 0) {
        mag = VEC_Mag(&d);
        if (mag <= 4) {
            d.x = d.y = d.z = 0;
        } else {
            d.x = FX_Div(d.x, mag);
            d.y = FX_Div(d.y, mag);
            d.z = FX_Div(d.z, mag);
        }
        if (w0->speed < mag)
            w0->speed += w0->accel;
        else
            w0->speed = mag;
        d.x = FX_Mul(d.x, w0->speed);
        d.y = FX_Mul(d.y, w0->speed);
        d.z = FX_Mul(d.z, w0->speed);
    }
    VEC_Add(&w0->cur, &d, &w0->cur);
    CAM.unk18 = w0->cur;
    VEC_Subtract(&w1->tgt, &w1->cur, &d);
    d.x = FX_Mul(d.x, w1->lerp);
    d.y = FX_Mul(d.y, w1->lerp);
    d.z = FX_Mul(d.z, w1->lerp);
    if (w1->accel > 0) {
        mag = VEC_Mag(&d);
        if (mag <= 4) {
            d.x = d.y = d.z = 0;
        } else {
            d.x = FX_Div(d.x, mag);
            d.y = FX_Div(d.y, mag);
            d.z = FX_Div(d.z, mag);
        }
        if (w1->speed < mag)
            w1->speed += w0->accel; /* original adds w->unk38 (the eye accel) here */
        else
            w1->speed = mag;
        d.x = FX_Mul(d.x, w1->speed);
        d.y = FX_Mul(d.y, w1->speed);
        d.z = FX_Mul(d.z, w1->speed);
    }
    VEC_Add(&w1->cur, &d, &w1->cur);
    CAM.unk24 = w1->cur;
}

/* BossCamera_UpdateTrack1 (copied): 'at' follows its target, the eye orbits it */
static void BossCamera_UpdateTrack1(void) {
    BossCamTrack *w0 = &CAM.t[0], *w1 = &CAM.t[1];
    BossVec d;
    s32 mag, diff, ad, step;
    VEC_Subtract(&w1->tgt, &w1->cur, &d);
    d.x = FX_Mul(d.x, w1->lerp);
    d.y = FX_Mul(d.y, w1->lerp);
    d.z = FX_Mul(d.z, w1->lerp);
    if (w1->accel > 0) {
        mag = VEC_Mag(&d);
        if (mag <= 4) {
            d.x = d.y = d.z = 0;
        } else {
            d.x = FX_Div(d.x, mag);
            d.y = FX_Div(d.y, mag);
            d.z = FX_Div(d.z, mag);
        }
        if (w1->speed < mag)
            w1->speed += w0->accel;
        else
            w1->speed = mag;
        d.x = FX_Mul(d.x, w1->speed);
        d.y = FX_Mul(d.y, w1->speed);
        d.z = FX_Mul(d.z, w1->speed);
    }
    VEC_Add(&w1->cur, &d, &w1->cur);
    diff = CAM.unkC0 - CAM.unkC4;
    ad = diff < 0 ? -diff : diff;
    if (CAM.unkC4 < CAM.unkC0) {
        step = FX_Mul(ad, CAM.unkC8);
        if (step < diff)
            CAM.unkC4 += step;
        else
            CAM.unkC4 = CAM.unkC0;
    } else if (CAM.unkC4 > CAM.unkC0) {
        step = FX_Mul(ad, CAM.unkC8);
        if (step < CAM.unkC4 - CAM.unkC0)
            CAM.unkC4 -= step;
        else
            CAM.unkC4 = CAM.unkC0;
    }
    diff = CAM.unkCC - CAM.unkD0;
    ad = diff < 0 ? -diff : diff;
    if (CAM.unkD0 < CAM.unkCC) {
        step = FX_Mul(ad, CAM.unkD4);
        if (step < diff)
            CAM.unkD0 += step;
        else
            CAM.unkD0 = CAM.unkCC;
    } else if (CAM.unkD0 > CAM.unkCC) {
        step = FX_Mul(ad, CAM.unkD4);
        if (step < CAM.unkD0 - CAM.unkCC)
            CAM.unkD0 -= step;
        else
            CAM.unkD0 = CAM.unkCC;
    }
    CAM.unkD8 += ((s16)(CAM.unkD6 - CAM.unkD8) * (s32)CAM.unkDA) >> 12;
    CAM.unk24 = w1->cur;
    w0->tgt.x = w1->cur.x + FX_Mul(CAM.unkC4, SIN(CAM.unkD8));
    w0->tgt.y = w1->cur.y + CAM.unkD0;
    w0->tgt.z = w1->cur.z + FX_Mul(CAM.unkC4, COS(CAM.unkD8));
    VEC_Subtract(&w0->tgt, &w0->cur, &d);
    d.x = FX_Mul(d.x, w0->lerp);
    d.y = FX_Mul(d.y, w0->lerp);
    d.z = FX_Mul(d.z, w0->lerp);
    if (w0->accel > 0) {
        mag = VEC_Mag(&d);
        if (mag <= 4) {
            d.x = d.y = d.z = 0;
        } else {
            d.x = FX_Div(d.x, mag);
            d.y = FX_Div(d.y, mag);
            d.z = FX_Div(d.z, mag);
        }
        if (w0->speed < mag)
            w0->speed += w0->accel;
        else
            w0->speed = mag;
        d.x = FX_Mul(d.x, w0->speed);
        d.y = FX_Mul(d.y, w0->speed);
        d.z = FX_Mul(d.z, w0->speed);
    }
    VEC_Add(&w0->cur, &d, &w0->cur);
    CAM.unk18 = w0->cur;
}

/* BossCamera_Main: Track (inline) + gBossCameraModeTable + sky model */
static inline void Track(BossCamTrack *t, u32 mask) {
    if (t != NULL) {
        if (CAM.unk0 & mask) {
            /* follow the object's model translation (unk124->unk48): not used by Boss1 */
            if (t->ox != NULL)
                t->tgt.x = (t->ox->x << 4) + t->offx;
            if (t->oy != NULL)
                t->tgt.y = t->offy - (t->oy->y << 4);
            if (t->oz != NULL)
                t->tgt.z = (t->oz->z << 4) + t->offz;
        } else {
            if (t->ox != NULL)
                t->tgt.x = (t->ox->x << 4) + t->offx;
            if (t->oy != NULL)
                t->tgt.y = t->offy - (t->oy->y << 4);
            if (t->oz != NULL)
                t->tgt.z = (t->oz->z << 4) + t->offz;
        }
    }
}

void BossCamera_Main(void) {
    if (!CAM.active)
        return;
    Track(&CAM.t[0], 2);
    Track(&CAM.t[1], 4);
    switch (CAM.unkDC) {
    case 0:
        BossCamera_UpdateFixed();
        break;
    case 1:
        BossCamera_UpdateTrack0();
        break;
    case 2:
        BossCamera_UpdateTrack1();
        break;
    }
    /* view built from these values (VEC_Add(.., &unk3C, ..) + LookAt) */
    {
        extern void boss_camera_commit(void);
        boss_camera_commit();
    }
    if (CAM.unk0 & 1) {
        CAM.unkE0.trans[0] = CAM.unk18.x; /* c->unk128 = c->unk18 */
        CAM.unkE0.trans[1] = CAM.unk18.y;
        CAM.unkE0.trans[2] = CAM.unk18.z;
        Model_Animate(&CAM.unkE0);
        boss_model_pose(&CAM.unkE0, NULL);
    }
    CAM.unk3C.x = CAM.unk3C.y = CAM.unk3C.z = 0;
}

/* --------------------------------------------- native camera output */
static float s_eye[3], s_at[3], s_up[3];
static float s_view_rot[9];
static int s_committed;

void boss_camera_commit(void) {
    s_eye[0] = (CAM.unk18.x + CAM.unk3C.x) / 4096.0f;
    s_eye[1] = (CAM.unk18.y + CAM.unk3C.y) / 4096.0f;
    s_eye[2] = (CAM.unk18.z + CAM.unk3C.z) / 4096.0f;
    s_at[0] = (CAM.unk24.x + CAM.unk3C.x) / 4096.0f;
    s_at[1] = (CAM.unk24.y + CAM.unk3C.y) / 4096.0f;
    s_at[2] = (CAM.unk24.z + CAM.unk3C.z) / 4096.0f;
    s_up[0] = CAM.unk30.x / 4096.0f;
    s_up[1] = CAM.unk30.y / 4096.0f;
    s_up[2] = CAM.unk30.z / 4096.0f;
    s_committed = 1;
}

static void norm3(float *v) {
    float l = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (l > 0) {
        v[0] /= l;
        v[1] /= l;
        v[2] /= l;
    }
}

/* G3i_LookAt_ + G3i_PerspectiveW_ as a dcm_camera_t (row vectors, world -> eye) */
void boss_camera(dcm_camera_t *c) {
    float x[3], y[3], z[3], cot, n, f, aspect;
    int i;
    memset(c, 0, sizeof(*c));
    if (!s_committed)
        boss_camera_commit();
    for (i = 0; i < 3; i++)
        z[i] = s_eye[i] - s_at[i];
    norm3(z);
    x[0] = s_up[1] * z[2] - s_up[2] * z[1];
    x[1] = s_up[2] * z[0] - s_up[0] * z[2];
    x[2] = s_up[0] * z[1] - s_up[1] * z[0];
    norm3(x);
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = z[2] * x[0] - z[0] * x[2];
    y[2] = z[0] * x[1] - z[1] * x[0];
    for (i = 0; i < 3; i++) {
        c->view.m[i][0] = x[i];
        c->view.m[i][1] = y[i];
        c->view.m[i][2] = z[i];
    }
    c->view.m[3][0] = -(x[0] * s_eye[0] + x[1] * s_eye[1] + x[2] * s_eye[2]);
    c->view.m[3][1] = -(y[0] * s_eye[0] + y[1] * s_eye[1] + y[2] * s_eye[2]);
    c->view.m[3][2] = -(z[0] * s_eye[0] + z[1] * s_eye[1] + z[2] * s_eye[2]);
    /* view rotation transposed = camera orientation (Model_TransposeRot of the camera matrix) */
    for (i = 0; i < 3; i++) {
        s_view_rot[0 * 3 + i] = c->view.m[i][0];
        s_view_rot[1 * 3 + i] = c->view.m[i][1];
        s_view_rot[2 * 3 + i] = c->view.m[i][2];
    }
    cot = (float)COS(CAM.unk4) / (float)SIN(CAM.unk4);
    n = CAM.unk8 / 4096.0f;
    f = CAM.unkC / 4096.0f;
    aspect = CAM.unk10 / 4096.0f;
    c->proj[0][0] = cot / aspect;
    c->proj[1][1] = cot;
    c->proj[2][2] = (f + n) / (n - f);
    c->proj[2][3] = -1.0f;
    c->proj[3][2] = 2 * f * n / (n - f);
    c->vp_x = 0;
    c->vp_y = 0;
    c->vp_w = 640.0f;
    c->vp_h = 480.0f;
    c->near_w = n * 0.5f;
}

void boss_camera_view_rot(float r[9]) {
    dcm_camera_t c;
    boss_camera(&c);
    memcpy(r, s_view_rot, sizeof(s_view_rot));
}

void boss_camera_eye(float eye[3], float at[3]) {
    if (!s_committed)
        boss_camera_commit();
    memcpy(eye, s_eye, sizeof(s_eye));
    memcpy(at, s_at, sizeof(s_at));
}

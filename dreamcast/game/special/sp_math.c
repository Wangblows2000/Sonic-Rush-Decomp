/*
 * sp_math.c - NitroSDK fixed-point maths the special stage uses, bit-exact:
 * FX_Div / FX_Sqrt / VEC_Mag / VEC_Distance / VEC_Normalize emulate the DS
 * hardware divider and square-root unit (src/libs/NitroSDK/fx/), the MTX_*
 * functions are the NitroSDK C/asm routines, FX_Atan2 uses the ROM atan table.
 */
#include <string.h>
#include "sp_internal.h"

const s16 *g_sp_sincos;
const s16 *g_sp_atan;
u32 gRandSeed = 0x12345678;

/* REG_DIV 64/32: (n << 32) / d, then (q + (1 << 19)) >> 20 (FX_GetDivResult) */
fx32 FX_Div(fx32 n, fx32 d) {
    s64 q;
    if (d == 0) /* hardware: quotient = -1 or +1 times all ones; never reached by the game */
        return n >= 0 ? (fx32)0x7fffffff : (fx32)0x80000000;
    q = ((s64)n << 32) / (s64)d;
    return (fx32)((q + (1 << 19)) >> 20);
}

static u32 isqrt64(u64 v) {
    u64 r = 0, bit = (u64)1 << 62;
    while (bit > v)
        bit >>= 2;
    while (bit) {
        if (v >= r + bit) {
            v -= r + bit;
            r = (r >> 1) + bit;
        } else
            r >>= 1;
        bit >>= 2;
    }
    return (u32)r;
}

u32 FX_Sqrt(s32 v) {
    if (v > 0)
        return (isqrt64((u64)v << 32) + 0x200) >> 10;
    return 0;
}

fx32 VEC_Mag(const VecFx32 *p) {
    s64 t = (s64)p->x * p->x + (s64)p->y * p->y + (s64)p->z * p->z;
    return (fx32)(((s32)isqrt64((u64)(t * 4)) + 1) >> 1);
}

fx32 VEC_Distance(const VecFx32 *a, const VecFx32 *b) {
    s64 t, d;
    d = a->x - b->x;
    t = d * d;
    d = a->y - b->y;
    t += d * d;
    d = a->z - b->z;
    t += d * d;
    return (fx32)(((s32)isqrt64((u64)(t * 4)) + 1) >> 1);
}

void VEC_Normalize(const VecFx32 *s, VecFx32 *d) {
    s64 t = (s64)s->x * s->x + (s64)s->y * s->y + (s64)s->z * s->z;
    s64 q;
    fx32 sq;
    if (t == 0) {
        d->x = d->y = d->z = 0;
        return;
    }
    q = (s64)(0x0100000000000000ULL / (u64)t);    /* DIVCNT 2: 64/64 */
    sq = (fx32)isqrt64((u64)(t * 4));
    q = q * sq;
    d->x = (fx32)((q * s->x + (1LL << 44)) >> 45);
    d->y = (fx32)((q * s->y + (1LL << 44)) >> 45);
    d->z = (fx32)((q * s->z + (1LL << 44)) >> 45);
}

void VEC_Add(const VecFx32 *a, const VecFx32 *b, VecFx32 *ab) {
    ab->x = a->x + b->x;
    ab->y = a->y + b->y;
    ab->z = a->z + b->z;
}
void VEC_Subtract(const VecFx32 *a, const VecFx32 *b, VecFx32 *ab) {
    ab->x = a->x - b->x;
    ab->y = a->y - b->y;
    ab->z = a->z - b->z;
}
void VEC_MultAdd(fx32 a, const VecFx32 *v1, const VecFx32 *v2, VecFx32 *d) {
    d->x = (fx32)(((s64)a * v1->x) >> 12) + v2->x;
    d->y = (fx32)(((s64)a * v1->y) >> 12) + v2->y;
    d->z = (fx32)(((s64)a * v1->z) >> 12) + v2->z;
}

u16 FX_Atan2(fx32 y, fx32 x) {
    fx32 a, b;
    s32 c;
    BOOL positive;
    if (y > 0) {
        if (x > 0) {
            if (x > y) { a = y; b = x; c = 0; positive = TRUE; }
            else if (x < y) { a = x; b = y; c = 16384; positive = FALSE; }
            else return 8192;
        } else if (x < 0) {
            x = -x;
            if (x < y) { a = x; b = y; c = 16384; positive = TRUE; }
            else if (x > y) { a = y; b = x; c = 32768; positive = FALSE; }
            else return 24576;
        } else
            return 16384;
    } else if (y < 0) {
        y = -y;
        if (x < 0) {
            x = -x;
            if (x > y) { a = y; b = x; c = -32768; positive = TRUE; }
            else if (x < y) { a = x; b = y; c = -16384; positive = FALSE; }
            else return 40960;
        } else if (x > 0) {
            if (x < y) { a = x; b = y; c = -16384; positive = TRUE; }
            else if (x > y) { a = y; b = x; c = 0; positive = FALSE; }
            else return 57344;
        } else
            return 49152;
    } else
        return x >= 0 ? 0 : 32768;
    if (b == 0)
        return 0;
    if (positive)
        return (u16)(c + g_sp_atan[FX_Div(a, b) >> 5]);
    return (u16)(c - g_sp_atan[FX_Div(a, b) >> 5]);
}

void MTX_Identity33_(MtxFx33 *m) {
    memset(m, 0, sizeof(*m));
    m->m[0] = m->m[4] = m->m[8] = 0x1000;
}
void MTX_Identity43_(MtxFx43 *m) {
    memset(m, 0, sizeof(*m));
    m->m[0] = m->m[4] = m->m[8] = 0x1000;
}
void MTX_RotX33_(MtxFx33 *m, fx32 s, fx32 c) {
    MTX_Identity33_(m);
    m->m[4] = c; m->m[5] = s; m->m[7] = -s; m->m[8] = c;
}
void MTX_RotY33_(MtxFx33 *m, fx32 s, fx32 c) {
    MTX_Identity33_(m);
    m->m[0] = c; m->m[2] = -s; m->m[6] = s; m->m[8] = c;
}
void MTX_RotZ33_(MtxFx33 *m, fx32 s, fx32 c) {
    MTX_Identity33_(m);
    m->m[0] = c; m->m[1] = s; m->m[3] = -s; m->m[4] = c;
}
void MTX_RotX43_(MtxFx43 *m, fx32 s, fx32 c) {
    MTX_Identity43_(m);
    m->m[4] = c; m->m[5] = s; m->m[7] = -s; m->m[8] = c;
}
void MTX_RotY43_(MtxFx43 *m, fx32 s, fx32 c) {
    MTX_Identity43_(m);
    m->m[0] = c; m->m[2] = -s; m->m[6] = s; m->m[8] = c;
}
void MTX_RotZ43_(MtxFx43 *m, fx32 s, fx32 c) {
    MTX_Identity43_(m);
    m->m[0] = c; m->m[1] = s; m->m[3] = -s; m->m[4] = c;
}

#define MM(x, y, z, b0, b1, b2) (fx32)(((s64)(x) * (b0) + (s64)(y) * (b1) + (s64)(z) * (b2)) >> 12)

void MTX_Concat33(const MtxFx33 *a, const MtxFx33 *b, MtxFx33 *ab) {
    MtxFx33 t;
    int r;
    for (r = 0; r < 3; r++) {
        fx32 x = a->m[r * 3], y = a->m[r * 3 + 1], z = a->m[r * 3 + 2];
        t.m[r * 3 + 0] = MM(x, y, z, b->m[0], b->m[3], b->m[6]);
        t.m[r * 3 + 1] = MM(x, y, z, b->m[1], b->m[4], b->m[7]);
        t.m[r * 3 + 2] = MM(x, y, z, b->m[2], b->m[5], b->m[8]);
    }
    *ab = t;
}

void MTX_Concat43(const MtxFx43 *a, const MtxFx43 *b, MtxFx43 *ab) {
    MtxFx43 t;
    int r;
    for (r = 0; r < 3; r++) {
        fx32 x = a->m[r * 3], y = a->m[r * 3 + 1], z = a->m[r * 3 + 2];
        t.m[r * 3 + 0] = MM(x, y, z, b->m[0], b->m[3], b->m[6]);
        t.m[r * 3 + 1] = MM(x, y, z, b->m[1], b->m[4], b->m[7]);
        t.m[r * 3 + 2] = MM(x, y, z, b->m[2], b->m[5], b->m[8]);
    }
    {
        fx32 x = a->m[9], y = a->m[10], z = a->m[11];
        t.m[9] = b->m[9] + MM(x, y, z, b->m[0], b->m[3], b->m[6]);
        t.m[10] = b->m[10] + MM(x, y, z, b->m[1], b->m[4], b->m[7]);
        t.m[11] = b->m[11] + MM(x, y, z, b->m[2], b->m[5], b->m[8]);
    }
    *ab = t;
}

void MTX_Copy33To43_(const MtxFx33 *a, MtxFx43 *b) {
    memcpy(b->m, a->m, 36);
    b->m[9] = b->m[10] = b->m[11] = 0;
}

void MTX_TransApply43(const MtxFx43 *s, MtxFx43 *d, fx32 x, fx32 y, fx32 z) {
    MtxFx43 t = *s;
    t.m[9] = MM(x, y, z, s->m[0], s->m[3], s->m[6]) + s->m[9];
    t.m[10] = MM(x, y, z, s->m[1], s->m[4], s->m[7]) + s->m[10];
    t.m[11] = MM(x, y, z, s->m[2], s->m[5], s->m[8]) + s->m[11];
    *d = t;
}

void MTX_MultVec43(const VecFx32 *v, const MtxFx43 *m, VecFx32 *d) {
    fx32 x = v->x, y = v->y, z = v->z;
    VecFx32 r;
    r.x = MM(x, y, z, m->m[0], m->m[3], m->m[6]) + m->m[9];
    r.y = MM(x, y, z, m->m[1], m->m[4], m->m[7]) + m->m[10];
    r.z = MM(x, y, z, m->m[2], m->m[5], m->m[8]) + m->m[11];
    *d = r;
}

void Model_TransposeRot(const MtxFx43 *src, MtxFx33 *dst) {
    int r, c;
    MtxFx33 t;
    for (r = 0; r < 3; r++)
        for (c = 0; c < 3; c++)
            t.m[r * 3 + c] = src->m[c * 3 + r];
    *dst = t;
}

static fx32 dot3(const VecFx32 *a, const VecFx32 *b) {
    return (fx32)(((s64)a->x * b->x + (s64)a->y * b->y + (s64)a->z * b->z + 0x800) >> 12);
}
static void cross3(const VecFx32 *a, const VecFx32 *b, VecFx32 *o) {
    VecFx32 r;
    r.x = (fx32)(((s64)a->y * b->z - (s64)a->z * b->y + 0x800) >> 12);
    r.y = (fx32)(((s64)a->z * b->x - (s64)a->x * b->z + 0x800) >> 12);
    r.z = (fx32)(((s64)a->x * b->y - (s64)a->y * b->x + 0x800) >> 12);
    *o = r;
}

/* G3i_LookAt_ (NitroSDK): z = normalize(pos - target), x = normalize(up x z), y = z x x;
 * camera matrix rows (x0 y0 z0 / x1 y1 z1 / x2 y2 z2 / -pos.x -pos.y -pos.z) */
void G3i_LookAt_(const VecFx32 *pos, const VecFx32 *up, const VecFx32 *target, MtxFx43 *out) {
    VecFx32 vx, vy, vz;
    VEC_Subtract(pos, target, &vz);
    VEC_Normalize(&vz, &vz);
    cross3(up, &vz, &vx);
    VEC_Normalize(&vx, &vx);
    cross3(&vz, &vx, &vy);
    out->m[0] = vx.x; out->m[1] = vy.x; out->m[2] = vz.x;
    out->m[3] = vx.y; out->m[4] = vy.y; out->m[5] = vz.y;
    out->m[6] = vx.z; out->m[7] = vy.z; out->m[8] = vz.z;
    out->m[9] = -dot3(pos, &vx);
    out->m[10] = -dot3(pos, &vy);
    out->m[11] = -dot3(pos, &vz);
}

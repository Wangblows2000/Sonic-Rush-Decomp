/*
 * Fixed-point helpers used by the objdata v2 objects.
 *
 * Reused from the decompilation (src/libs/NitroSDK/fx/, src/game/Stage/
 * ObjectSystem/): MTX_Identity33_, MTX_RotX33_, MTX_RotY33_, MTX_RotZ33_
 * (ARM/Thumb asm in the SDK, written here as the stores they perform),
 * MTX_Concat33 and MTX_MultVec33 (C, copied with the s64 >> 12 products),
 * FX_DivS32 (hardware divider -> C division, which truncates the same way),
 * ObjShift_Set and ObjShift_SetFromRef (copied).
 */
#include "obj_internal.h"

s16 obj_sin(u16 a) { return dcp_sin(a); } /* data_0206e470[(a >> 4) * 2]     */
s16 obj_cos(u16 a) { return dcp_cos(a); } /* data_0206e470[(a >> 4) * 2 + 1] */

/* FX_DivS32: DS divider (DIVCNT mode 0, 32/32) = truncating division */
s32 FX_DivS32(s32 n, s32 d) {
    if (d == 0)
        return n < 0 ? 1 : -1; /* hardware result for a zero divisor */
    return n / d;
}

/* MTX_Identity33_ */
void MTX_Identity33_(ObjMtx33 *m) {
    m->m[0][0] = 0x1000; m->m[0][1] = 0; m->m[0][2] = 0;
    m->m[1][0] = 0; m->m[1][1] = 0x1000; m->m[1][2] = 0;
    m->m[2][0] = 0; m->m[2][1] = 0; m->m[2][2] = 0x1000;
}
/* MTX_RotX33_(m, sin, cos) */
void MTX_RotX33_(ObjMtx33 *m, s32 sn, s32 cs) {
    m->m[0][0] = 0x1000; m->m[0][1] = 0; m->m[0][2] = 0;
    m->m[1][0] = 0; m->m[1][1] = cs; m->m[1][2] = sn;
    m->m[2][0] = 0; m->m[2][1] = -sn; m->m[2][2] = cs;
}
/* MTX_RotY33_(m, sin, cos) */
void MTX_RotY33_(ObjMtx33 *m, s32 sn, s32 cs) {
    m->m[0][0] = cs; m->m[0][1] = 0; m->m[0][2] = -sn;
    m->m[1][0] = 0; m->m[1][1] = 0x1000; m->m[1][2] = 0;
    m->m[2][0] = sn; m->m[2][1] = 0; m->m[2][2] = cs;
}
/* MTX_RotZ33_(m, sin, cos) */
void MTX_RotZ33_(ObjMtx33 *m, s32 sn, s32 cs) {
    m->m[0][0] = cs; m->m[0][1] = sn; m->m[0][2] = 0;
    m->m[1][0] = -sn; m->m[1][1] = cs; m->m[1][2] = 0;
    m->m[2][0] = 0; m->m[2][1] = 0; m->m[2][2] = 0x1000;
}

#define M(x, y, z, b0, b1, b2) (s32)(((s64)(x) * (b0) + (s64)(y) * (b1) + (s64)(z) * (b2)) >> 12)
/* MTX_Concat33 (copied; ab may alias a or b) */
void MTX_Concat33(const ObjMtx33 *a, const ObjMtx33 *b, ObjMtx33 *ab) {
    ObjMtx33 tmp, *p;
    s32 x, y, z, xx, yy, zz;
    p = (ab == b) ? &tmp : ab;
    x = a->m[0][0]; y = a->m[0][1]; z = a->m[0][2];
    p->m[0][0] = M(x, y, z, b->m[0][0], b->m[1][0], b->m[2][0]);
    p->m[0][1] = M(x, y, z, b->m[0][1], b->m[1][1], b->m[2][1]);
    xx = b->m[0][2]; yy = b->m[1][2]; zz = b->m[2][2];
    p->m[0][2] = M(x, y, z, xx, yy, zz);
    x = a->m[1][0]; y = a->m[1][1]; z = a->m[1][2];
    p->m[1][2] = M(x, y, z, xx, yy, zz);
    p->m[1][1] = M(x, y, z, b->m[0][1], b->m[1][1], b->m[2][1]);
    xx = b->m[0][0]; yy = b->m[1][0]; zz = b->m[2][0];
    p->m[1][0] = M(x, y, z, xx, yy, zz);
    x = a->m[2][0]; y = a->m[2][1]; z = a->m[2][2];
    p->m[2][0] = M(x, y, z, xx, yy, zz);
    p->m[2][1] = M(x, y, z, b->m[0][1], b->m[1][1], b->m[2][1]);
    xx = b->m[0][2]; yy = b->m[1][2]; zz = b->m[2][2];
    p->m[2][2] = M(x, y, z, xx, yy, zz);
    if (p == &tmp)
        *ab = tmp;
}
#undef M

/* MTX_MultVec33 (copied) */
void MTX_MultVec33(const ObjVec *v, const ObjMtx33 *m, ObjVec *dst) {
    s32 x = v->x, y = v->y, z = v->z;
    dst->x = (s32)(((s64)x * m->m[0][0] + (s64)y * m->m[1][0] + (s64)z * m->m[2][0]) >> 12);
    dst->y = (s32)(((s64)x * m->m[0][1] + (s64)y * m->m[1][1] + (s64)z * m->m[2][1]) >> 12);
    dst->z = (s32)(((s64)x * m->m[0][2] + (s64)y * m->m[1][2] + (s64)z * m->m[2][2]) >> 12);
}

/* ObjShift_Set (copied) */
s32 ObjShift_Set(s32 cur, s32 target, s32 shift, s32 maxStep, s32 minStep) {
    s32 d, diff;
    if (cur == target)
        return cur;
    diff = target - cur;
    if (minStep == 0)
        minStep = 1;
    d = diff >> shift;
    if (maxStep != 0) {
        if (d > maxStep)
            d = maxStep;
        if (d < -maxStep)
            d = -maxStep;
    }
    if (minStep != 0) {
        if (d > 0) {
            if (d < minStep)
                d = minStep;
        } else if (d < 0) {
            if (d > -minStep)
                d = -minStep;
        } else {
            if (diff > 0 && d < minStep)
                d = minStep;
            if (diff < 0 && d > -minStep)
                d = -minStep;
        }
    }
    cur += d;
    if (d > 0) {
        if (cur > target)
            cur = target;
    } else if (d < 0) {
        if (cur < target)
            cur = target;
    }
    return cur;
}

/* ObjShift_SetFromRef (copied) */
s32 ObjShift_SetFromRef(s32 cur, s32 target, s32 ref, s32 shift, u16 maxStep, s32 minStep) {
    s32 d, diff;
    if (cur == target)
        return cur;
    if (minStep == 0)
        minStep = 1;
    d = (cur - ref) >> shift;
    if (target > ref && d < 0)
        d = 0;
    if (target < ref && d > 0)
        d = 0;
    if (maxStep != 0) {
        if (d > maxStep)
            d = maxStep;
        if (d < -maxStep)
            d = -maxStep;
    }
    if (minStep != 0) {
        if (d > 0) {
            if (d < minStep)
                d = minStep;
        } else if (d < 0) {
            if (d > -minStep)
                d = -minStep;
        } else {
            diff = target - cur;
            if (diff > 0 && d < minStep)
                d = minStep;
            if (diff < 0 && d > -minStep)
                d = -minStep;
        }
    }
    cur += d;
    if (d > 0) {
        if (cur > target)
            cur = target;
    } else if (d < 0) {
        if (cur < target)
            cur = target;
    }
    return cur;
}

/* FX_Sqrt (NitroSDK): hardware 64-bit square root of v << 32, then
 * FX_GetSqrtResult (r + 0x200) >> 10 (objdata v4: Player_Action_Cannon) */
u32 FX_Sqrt(s32 v) {
    u64 n, r = 0, bit;
    if (v <= 0)
        return 0;
    n = (u64)(u32)v << 32;
    bit = (u64)1 << 62;
    while (bit > n)
        bit >>= 2;
    while (bit != 0) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return ((u32)r + 0x200) >> 10;
}

/* ObjAngle_MoveToward16 (objdata v4: Cannon_RotateBarrel) */
u16 ObjAngle_MoveToward16(u16 cur, u16 tgt, u16 step) {
    u16 d1, d2;
    if (tgt == cur)
        return tgt;
    d1 = (u16)(cur - tgt < 0 ? -(cur - tgt) : cur - tgt);
    if (cur > tgt)
        d2 = (u16)(0x10000 - cur + tgt);
    else
        d2 = (u16)(0x10000 - tgt + cur);
    if (d1 <= d2) {
        if (cur > tgt) {
            if (tgt > cur - step)
                cur = tgt;
            else
                cur = (u16)(cur - step);
        } else if (cur < tgt) {
            if (tgt < cur + step)
                cur = tgt;
            else
                cur = (u16)(cur + step);
        }
    } else {
        if (cur > tgt) {
            if (tgt + 0x10000 < cur + step)
                cur = tgt;
            else
                cur = (u16)(cur + step);
        } else if (cur < tgt) {
            if (tgt > cur - step + 0x10000)
                cur = tgt;
            else
                cur = (u16)(cur - step);
        }
    }
    return cur;
}

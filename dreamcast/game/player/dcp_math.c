/*
 * Fixed-point helpers used by the player physics.
 * Translations of: ObjSpd_DownSet, ObjSpd_UpSet, ObjAngle_MoveToward8,
 * ObjAngle_IsUpsideDown, Math_RotateVec2S16 (+ MTX_RotZ33_/MTX_MultVec33),
 * FX_Atan2 (+ FX_Div), the gRandSeed LCG and the data_0206e470 lookups.
 */
#include "dcp_player.h"

DcpWorld g_dcp_world = { 0, 0, 0, 1, 0, 0x18, 0, 0x7fffffff, 0x7fff, 0x100, 0, 0, 0, 3, 0x28, 0x78 };
DcpHooks g_dcp_hooks;

static const s16 *s_sincos; /* 8192 entries */
static const s16 *s_atan;   /* 129 entries */

void dcp_set_trig(const s16 *sincos8192, const s16 *atan129) {
    s_sincos = sincos8192;
    s_atan = atan129;
}

/* SIN_IDX(a) = data_0206e470[((a & 0xffff) >> 4) * 2] */
s16 dcp_sin(u16 a16) { return s_sincos[(a16 >> 4) * 2]; }
s16 dcp_cos(u16 a16) { return s_sincos[(a16 >> 4) * 2 + 1]; }

/* ObjSpd_DownSet: move towards zero by (b * timescale) >> 8, clamp at 0 */
s32 dcp_spd_down(s32 a, s32 b) {
    s32 ts = g_dcp_world.timescale;
    if (a > 0) {
        a -= (b * ts) >> 8;
        if (a < 0)
            a = 0;
    } else {
        a += (b * ts) >> 8;
        if (a > 0)
            a = 0;
    }
    return a;
}

/* ObjSpd_UpSet: a += (b*ts<<8)>>16, clamp to +-lim (lim 0 = no clamp) */
s32 dcp_spd_up(s32 a, s32 b, s32 lim) {
    s32 ts = g_dcp_world.timescale;
    a += (s32)((u32)(b * ts) << 8) >> 16;
    if (lim == 0)
        return a;
    if (b >= 0) {
        if (a > lim)
            a = lim;
        return a;
    }
    if (a < -lim)
        a = (s16)-lim;
    return a;
}

/* ObjAngle_MoveToward8 (u8 arithmetic exactly as the original) */
u8 dcp_angle_toward8(u8 cur, u8 tgt, u8 step) {
    u8 d1;
    u8 d2;
    if (tgt == cur)
        return tgt;
    d1 = cur - tgt < 0 ? -(cur - tgt) : cur - tgt;
    if (cur > tgt)
        d2 = 0x100 - cur + tgt;
    else
        d2 = 0x100 - tgt + cur;
    if (d1 <= d2) {
        if (cur > tgt) {
            if (tgt > cur - step)
                cur = tgt;
            else
                cur -= step;
        } else if (cur < tgt) {
            if (tgt < cur + step)
                cur = tgt;
            else
                cur += step;
        }
    } else {
        if (cur > tgt) {
            if (tgt + 0x100 < cur + step)
                cur = tgt;
            else
                cur += step;
        } else if (cur < tgt) {
            if (tgt > cur - step + 0x100)
                cur = tgt;
            else
                cur -= step;
        }
    }
    return cur;
}

dcp_bool dcp_angle_upside_down(u32 v) { return v > 0x60 && v < 0xa0; }

/* Math_RotateVec2S16: identity, RotZ(sin, cos), vector <<4, MultVec33, >>4 */
void dcp_rotate_vec(s16 *x, s16 *y, s32 angle) {
    s32 a = 0, b = 0;
    s32 s = dcp_sin((u16)(angle << 8));
    s32 c = dcp_cos((u16)(angle << 8));
    s32 vx, vy, rx, ry;
    if (x != NULL)
        a = *x;
    if (y != NULL)
        b = *y;
    vx = a << 4;
    vy = b << 4;
    /* MTX_RotZ33_: _00 = c, _01 = s, _10 = -s, _11 = c */
    rx = (s32)(((s64)vx * c + (s64)vy * -s) >> 12);
    ry = (s32)(((s64)vx * s + (s64)vy * c) >> 12);
    if (x != NULL)
        *x = (s16)(rx >> 4);
    if (y != NULL)
        *y = (s16)(ry >> 4);
}

u16 dcp_rand(void) {
    g_dcp_world.rand_seed = g_dcp_world.rand_seed * 0x196225 + 0x3c6ef35f;
    return (u16)(g_dcp_world.rand_seed >> 16);
}

/* (s32)(v * d) exactly as an IEEE double multiply (round to nearest even of
 * the 53-bit product) followed by truncation toward zero, for |v| < 2^16 and
 * 0 < d < 1. */
s32 dcp_mul_double(s32 v, DcpDouble d) {
    u64 av = (u64)(v < 0 ? -v : v);
    u64 a = av * (d.mant >> 32);          /* < 2^37 */
    u64 b = av * (d.mant & 0xffffffffu);  /* < 2^48 */
    u64 top = a + (b >> 32);              /* P = top * 2^32 + low */
    u64 low = b & 0xffffffffu;
    u64 r;
    u32 len = 0, s;
    u64 t;
    s32 res;
    for (t = top; t; t >>= 1)
        len++;
    if (top)
        len += 32;
    else
        for (t = low; t; t >>= 1)
            len++;
    if (len <= 53) {
        r = (top << 32) | low;
        s = 0;
    } else {
        u64 rem, half;
        s = len - 53; /* 1..16 */
        r = (top << (32 - s)) | (low >> s);
        rem = low & ((1ull << s) - 1);
        half = 1ull << (s - 1);
        if (rem > half || (rem == half && (r & 1)))
            r++;
    }
    res = (s32)(r >> (d.shift - s));
    return v < 0 ? -res : res;
}

/* FX_Div: hardware divider (numer << 32) / denom, result rounded to 20.12 */
static s32 fx_div(s32 a, s32 b) {
    s64 q = ((s64)a << 32) / (s64)b;
    return (s32)((q + (1 << 19)) >> 20);
}

/* FX_Atan2 (NitroSDK), 16-bit angle result */
u16 dcp_atan2(s32 y, s32 x) {
    s32 a, b, c;
    int positive;
    if (y > 0) {
        if (x > 0) {
            if (x > y) { a = y; b = x; c = 0; positive = 1; }
            else if (x < y) { a = x; b = y; c = 16384; positive = 0; }
            else return 8192;
        } else if (x < 0) {
            x = -x;
            if (x < y) { a = x; b = y; c = 16384; positive = 1; }
            else if (x > y) { a = y; b = x; c = 32768; positive = 0; }
            else return 24576;
        } else {
            return 16384;
        }
    } else if (y < 0) {
        y = -y;
        if (x < 0) {
            x = -x;
            if (x > y) { a = y; b = x; c = -32768; positive = 1; }
            else if (x < y) { a = x; b = y; c = -16384; positive = 0; }
            else return 40960;
        } else if (x > 0) {
            if (x < y) { a = x; b = y; c = -16384; positive = 1; }
            else if (x > y) { a = y; b = x; c = 0; positive = 0; }
            else return 57344;
        } else {
            return 49152;
        }
    } else {
        return x >= 0 ? 0 : 32768;
    }
    if (b == 0)
        return 0;
    if (positive)
        return (u16)(c + s_atan[fx_div(a, b) >> 5]);
    return (u16)(c - s_atan[fx_div(a, b) >> 5]);
}

/* ---------------------------------------------------- physics table */
static DcpPhysics s_phys[4];
static u32 s_phys_n;

int dcp_set_physics(const u8 *raw, u32 size) {
    u32 i, k;
    if (size < 0x30)
        return -1;
    s_phys_n = size / 0x30;
    if (s_phys_n > 4)
        s_phys_n = 4;
    for (i = 0; i < s_phys_n; i++) {
        s16 *dst = (s16 *)&s_phys[i];
        for (k = 0; k < 24; k++)
            dst[k] = (s16)(raw[i * 0x30 + k * 2] | (raw[i * 0x30 + k * 2 + 1] << 8));
    }
    return (int)s_phys_n;
}

const DcpPhysics *dcp_physics(u8 chara) {
    if (chara >= s_phys_n)
        chara = 0;
    return &s_phys[chara];
}

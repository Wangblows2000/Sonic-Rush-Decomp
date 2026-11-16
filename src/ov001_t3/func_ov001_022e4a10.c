#include <nitro/types.h>

s32 func_0200225c(s32 n, s32 d);

#pragma thumb on
static inline fx32 FX_Mul_022e4a10(fx32 a, fx32 b) {
    return (fx32)(((s64)a * b + 0x800) >> 12);
}

s32 func_ov001_022e4a10(s32 a, s32 b, s32 n, s32 m, s32 t) {
    fx32 r;
    fx32 v;

    if (t < 0) t = 0;
    if (t > 0x2000) t = 0x2000;
    if (m > n) m = n;
    r = func_0200225c(m << 12, n << 12);
    r = FX_Mul_022e4a10(r, t) + FX_Mul_022e4a10(FX_Mul_022e4a10(r, r), 0x1000 - t);
    return a + (FX_Mul_022e4a10((b - a) << 12, r) >> 12);
}
#pragma thumb reset

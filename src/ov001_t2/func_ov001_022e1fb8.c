#include <nitro/types.h>

fx32 func_0200225c(fx32 a, fx32 b);

#pragma thumb on
static inline fx32 FX_Mul_022e1fb8(fx32 a, fx32 b) {
    return (fx32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_022e1fb8(s32 n, s32 *out, s32 x0, s32 y0, s32 a, s32 b, s32 x1, s32 y1) {
    s32 i;
    fx32 t;

    for (i = 0; i < n; i++) {
        t = func_0200225c((i + 1) << 12, (n + 1) << 12);
        out[0] = (x0 + FX_Mul_022e1fb8(x1 - x0, t)) >> 12;
        out[1] = (y0 + FX_Mul_022e1fb8(y1 - y0, t)) >> 12;
        out += 2;
    }
}
#pragma thumb reset

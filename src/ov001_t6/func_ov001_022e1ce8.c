#include <nitro/types.h>

s32 func_0200225c(s32 n, s32 d);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

#pragma thumb on
void func_ov001_022e1ce8(s32 n, s32 *x, s32 *y, s32 *z) {
    s32 i;
    s32 t;
    s32 h[4];
    s32 d[4];

    i = 0;
    z[0] = 0;
    z[n - 1] = 0;
    for (; i < n - 1; i++) {
        h[i] = x[i + 1] - x[i];
        d[i + 1] = func_0200225c(y[i + 1] - y[i], h[i]);
    }
    z[1] = d[2] - d[1] - FX_Mul(h[0], z[0]);
    d[1] = (x[2] - x[0]) * 2;
    for (i = 1; i < n - 2; i++) {
        t = func_0200225c(h[i], d[i]);
        z[i + 1] = d[i + 2] - d[i + 1] - FX_Mul(z[i], t);
        d[i + 1] = (x[i + 2] - x[i]) * 2 - FX_Mul(h[i], t);
    }
    z[n - 2] -= FX_Mul(h[n - 2], z[n - 1]);
    for (i = n - 2; i > 0; i--) {
        z[i] = func_0200225c(z[i] - FX_Mul(h[i], z[i + 1]), d[i]);
    }
}
#pragma thumb reset

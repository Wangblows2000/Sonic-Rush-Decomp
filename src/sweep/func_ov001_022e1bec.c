#include <nitro/types.h>

s32 func_0200225c(s32 n, s32 d);

#pragma thumb on
static inline fx32 FX_Mul_022e1bec(fx32 a, fx32 b) {
    return (fx32)(((s64)a * b + 0x800) >> 12);
}

s32 func_ov001_022e1bec(s32 x, s32 n, s32 *xs, s32 *ds, s32 *ys) {
    s32 lo;
    s32 hi;
    s32 mid;
    s32 h;
    s32 y0;
    s32 y1;
    s32 t;
    s32 b;
    s32 c;

    lo = 0;
    hi = n - 1;
    while (lo < hi) {
        mid = (lo + hi) / 2;
        if (xs[mid] < x) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    if (lo > 0) lo--;
    h = xs[lo + 1] - xs[lo];
    y1 = *(s32 *)((u8 *)ys + (lo * 4 + 4));
    y0 = *(s32 *)((u8 *)ys + lo * 4);
    t = x - xs[lo];
    b = y0 + (y0 << 1) + func_0200225c(FX_Mul_022e1bec(y1 - y0, t), h);
    c = func_0200225c(ds[lo + 1] - ds[lo], h);
    return ds[lo] + FX_Mul_022e1bec(FX_Mul_022e1bec(b, t) + (c - FX_Mul_022e1bec(y0 + y0 + y1, h)), t);
}
#pragma thumb reset

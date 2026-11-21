#include <nitro/types.h>

fx32 FX_Div(fx32 a, fx32 b);


#pragma thumb on
static inline fx32 FX_Mul_022cf4f0(fx32 a, fx32 b) {
    return (fx32)(((s64)a * b + 0x800) >> 12);
}

s32 SegaLogo_Interpolate(s32 from, s32 to, s32 n, s32 i, fx32 ease) {
    fx32 t;

    if (ease < 0) ease = 0;
    if (ease > 0x2000) ease = 0x2000;
    if (i > n - 1) i = n - 1;
    if (i == 0) return from;
    if (i == n - 1) return to;
    if (n <= 1) return from;
    t = FX_Div(i << 12, (n - 1) << 12);
    t = FX_Mul_022cf4f0(t, ease) + FX_Mul_022cf4f0(FX_Mul_022cf4f0(t, t), 0x1000 - ease);
    return from + ((FX_Mul_022cf4f0((to - from) << 12, t) + 0x800) >> 12);
}
#pragma thumb reset

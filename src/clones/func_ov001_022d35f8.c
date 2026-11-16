#include <nitro/types.h>

s32 func_0200225c(s32 a, s32 b);

#pragma thumb on
static inline s32 FX_Mul_0204eedc(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

s32 func_ov001_022d35f8(s32 from, s32 to, s32 len, s32 cur, s32 e) {
    s32 t;

    if (e < 0)
        e = 0;
    if (e > 0x2000)
        e = 0x2000;
    if (cur > len)
        cur = len;
    t = func_0200225c(cur << 12, len << 12);
    t = FX_Mul_0204eedc(t, e) + FX_Mul_0204eedc(FX_Mul_0204eedc(t, t), 0x1000 - e);
    return from + ((FX_Mul_0204eedc((to - from) << 12, t) + 0x800) >> 12);
}
#pragma thumb reset

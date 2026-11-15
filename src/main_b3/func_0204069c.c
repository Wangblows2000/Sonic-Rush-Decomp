#include <nitro/types.h>

#pragma thumb on
static inline s32 FX_Mul_0204069c(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

s32 func_0204069c(s32 t, s32 e) {
    return FX_Mul_0204069c(t, e) + FX_Mul_0204069c(FX_Mul_0204069c(t, t), 0x1000 - e);
}
#pragma thumb reset

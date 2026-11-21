#include <nitro/types.h>

static inline s32 FX_Mul_0205de50(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

u16 StageTitleCard_LerpColor(s32 c0, s32 c1, s32 t) {
    s32 r0 = c0 & 0x1f;
    s32 g0 = (c0 >> 5) & 0x1f;
    s32 b0 = (c0 >> 10) & 0x1f;
    s32 r = FX_Mul_0205de50((((c1 & 0x1f) - r0) << 12), t) + (r0 << 12);
    s32 g = FX_Mul_0205de50(((((c1 >> 5) & 0x1f) - g0) << 12), t) + (g0 << 12);
    s32 b = FX_Mul_0205de50(((((c1 >> 10) & 0x1f) - b0) << 12), t) + (b0 << 12);
    return (r >> 12) | ((g >> 12) << 5) | ((b >> 12) << 10);
}

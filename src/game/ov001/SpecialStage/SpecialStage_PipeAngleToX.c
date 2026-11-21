#include <nitro/types.h>

extern s16 data_0206e470[];

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

s32 SpecialStage_PipeAngleToX(u16 ang) {
    return FX_Mul(SIN(ang), 0x1e000);
}

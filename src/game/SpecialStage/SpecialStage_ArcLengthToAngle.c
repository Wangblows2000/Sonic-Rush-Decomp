#include <nitro/types.h>

s32 FX_Div(s32 a, s32 b);

u16 SpecialStage_ArcLengthToAngle(s32 a, s32 b) {
    s32 t = (s32)(((s64)a * 0x3244 + 0x800) >> 12);
    s32 d = FX_Div(0x8000000, t);
    return (u16)((s32)(((s64)d * b + 0x800) >> 12) >> 12);
}

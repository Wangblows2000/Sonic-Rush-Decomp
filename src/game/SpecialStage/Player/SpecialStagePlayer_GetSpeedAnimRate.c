#include <nitro/types.h>

s32 FX_Div(s32 a, s32 b);

s32 SpecialStagePlayer_GetSpeedAnimRate(s32 a) {
    return FX_Div(a, 0x3000) * 4 + 0x1000;
}

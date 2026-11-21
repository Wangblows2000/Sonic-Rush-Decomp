#include <nitro/types.h>

s32 FX_Sqrt(s32 a);
u16 FX_Atan2(s32 a, s32 b);

u16 SpecialStage_PipeXToAngle(s32 x) {
    s32 r = FX_Sqrt(0x38400 - (s32)(((s64)(x >> 2) * (x >> 2) + 0x800) >> 12));
    if ((r << 2) == 0) {
        if (x > 0) {
            return 0x4000;
        }
        if (x < 0) {
            return 0xc000;
        }
        return 0;
    }
    return FX_Atan2(x, r << 2);
}

#include <nitro/types.h>

void MTX_TransApply43(s32 a, s32 b, s32 c, s32 d, s32 e);

void SpecialStageMap_Curve1PivotOffset(s32 a) {
    MTX_TransApply43(a, a, -0xbf000, 0, 0);
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 MTX_TransApply43();

void SpecialStageMap_StraightAdvance(s32 p0) {
    MTX_TransApply43(p0, p0, 0, 0, -0x12c000);
}

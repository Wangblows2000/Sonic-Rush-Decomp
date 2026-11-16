/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gSpecialStageMap[];
s32 MTX_TransApply43();

void SpecialStageMap_ScrollStraight(void) {
    MTX_TransApply43((*(u32 *)gSpecialStageMap + 0x8), (*(u32 *)gSpecialStageMap + 0x8), 0, 0, *(u32 *)*(u32 *)gSpecialStageMap);
}

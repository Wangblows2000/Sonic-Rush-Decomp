/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gSpecialStageMap[];
s32 SpecialStage_ArcLengthToAngle();
s32 SpecialStage_RotateMtxXAroundPivot();

void SpecialStageMap_ScrollCurve4(void) {
    s32 t0 = *(u32 *)gSpecialStageMap;
    s32 t1 = SpecialStage_ArcLengthToAngle(0xbf000, *(u32 *)*(u32 *)gSpecialStageMap);
    SpecialStage_RotateMtxXAroundPivot((t0 + 0x8), -0xbf000, t1);
}

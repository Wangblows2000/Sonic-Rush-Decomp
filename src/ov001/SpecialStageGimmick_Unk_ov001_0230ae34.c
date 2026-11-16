#include <nitro/types.h>

void SpecialStage_IsPointInCircle(s32 a, s32 b, s32 c, s32 d);

void SpecialStageGimmick_Unk_ov001_0230ae34(s32 a, s32 b, s32 *c) {
    SpecialStage_IsPointInCircle(a, b, c[0], c[1]);
}

#include <nitro/types.h>

extern s32 *gSpecialStageWork;

s32 SpecialStage_GetGimmickManager(void) {
    return gSpecialStageWork[7];
}

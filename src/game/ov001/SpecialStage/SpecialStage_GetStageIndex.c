#include <nitro/types.h>

extern s32 *gSpecialStageWork;

s32 SpecialStage_GetStageIndex(void) {
    return gSpecialStageWork[1];
}

#include <nitro/types.h>

extern s32 *gSpecialStageWork;

s32 SpecialStage_GetPlayer(void) {
    return gSpecialStageWork[6];
}

#include <nitro/types.h>

extern s32 *gSpecialStageWork;

s32 SpecialStage_GetEmeraldFlags(void) {
    return gSpecialStageWork[2];
}

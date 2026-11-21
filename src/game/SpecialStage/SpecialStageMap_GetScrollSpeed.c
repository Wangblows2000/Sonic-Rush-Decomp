#include <nitro/types.h>

extern s32 *gSpecialStageMap;

s32 SpecialStageMap_GetScrollSpeed(void) {
    return gSpecialStageMap[1];
}

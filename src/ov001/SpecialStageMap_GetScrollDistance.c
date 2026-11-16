#include <nitro/types.h>

extern s32 *gSpecialStageMap;

s32 SpecialStageMap_GetScrollDistance(void) {
    return gSpecialStageMap[0];
}

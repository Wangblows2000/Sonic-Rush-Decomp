#include <nitro/types.h>

extern s32 *gSpecialStageMap;

void SpecialStageMap_SetScrollSpeed(s32 v) {
    gSpecialStageMap[1] = v;
}

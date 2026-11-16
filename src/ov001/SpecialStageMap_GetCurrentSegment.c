#include <nitro/types.h>

extern u8 *gSpecialStageMap;

s32 SpecialStageMap_GetCurrentSegment(void) {
    return *(s32 *)(gSpecialStageMap + 0xa80);
}

#include <nitro/types.h>

extern u8 *gSpecialStageMap;

BOOL SpecialStageMap_IsSegmentListEmpty(void) {
    return *(u8 **)(gSpecialStageMap + 0xa80) == gSpecialStageMap + 0xa88;
}

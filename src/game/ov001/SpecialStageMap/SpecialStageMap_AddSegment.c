#include <nitro/types.h>

s32 *SpecialStageMap_AllocSegment(void);

void SpecialStageMap_AddSegment(s32 v) {
    *SpecialStageMap_AllocSegment() = v;
}

#include <nitro/types.h>

extern s32 *gSpecialStageMap;

s32 SpecialStageMap_GetCurveLength(void) {
    return gSpecialStageMap[14];
}

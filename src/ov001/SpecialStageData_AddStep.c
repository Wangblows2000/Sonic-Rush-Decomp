#include <nitro/types.h>

extern s32 *gSpecialStageData;

void SpecialStageData_AddStep(void) {
    gSpecialStageData[2]++;
    gSpecialStageData[3]++;
}

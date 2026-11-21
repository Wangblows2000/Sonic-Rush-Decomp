#include <nitro/types.h>

extern s32 gSpecialStageCamera[];

void SpecialStageCamera_SetMode(s32 v) {
    gSpecialStageCamera[0] = v;
    gSpecialStageCamera[1] = 1;
}

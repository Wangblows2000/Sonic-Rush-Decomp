#include <nitro/types.h>

extern s32 gStageEventFiles[];

s32 StageEvent_GetRingList(void) {
    return gStageEventFiles[1];
}

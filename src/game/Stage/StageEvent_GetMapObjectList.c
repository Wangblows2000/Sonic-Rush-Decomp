#include <nitro/types.h>

extern s32 gStageEventFiles[];

s32 StageEvent_GetMapObjectList(void) {
    return gStageEventFiles[0];
}

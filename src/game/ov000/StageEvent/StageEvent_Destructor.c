#include <nitro/types.h>

extern s32 gStageEventManagerTask;

void StageEvent_Destructor(void) {
    gStageEventManagerTask = 0;
}

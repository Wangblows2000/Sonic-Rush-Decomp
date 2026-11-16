#include <nitro/types.h>

void StageEvent_SpawnInView(void *p, s32 a, s32 b);

void StageEvent_UpdateBoss(void *p) {
    StageEvent_SpawnInView(p, 0, 5);
}

#include <nitro/types.h>

void ExtraBoss_CheckCollisionGroup(s32 a);

void ExtraBoss_CheckAllCollisions(void) {
    ExtraBoss_CheckCollisionGroup(1);
    ExtraBoss_CheckCollisionGroup(2);
    ExtraBoss_CheckCollisionGroup(0);
}

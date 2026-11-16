#include <nitro/types.h>

extern s32 gBoss2WaterLightTask;

void Boss2WaterLight_Destructor(void) {
    gBoss2WaterLightTask = 0;
}

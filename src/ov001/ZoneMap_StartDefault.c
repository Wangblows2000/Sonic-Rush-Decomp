#include <nitro/types.h>

void ZoneMap_Start(s32 a, s32 b);

#pragma thumb on
void ZoneMap_StartDefault(void) {
    ZoneMap_Start(0, 0);
}
#pragma thumb reset

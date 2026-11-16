#include <nitro/types.h>

void ZoneMap_Start(s32 a, s32 b);

#pragma thumb on
void ZoneMap_Unk_ov001_022e8b14(void) {
    ZoneMap_Start(1, 1);
}
#pragma thumb reset

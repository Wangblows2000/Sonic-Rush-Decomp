#include <nitro/types.h>

#pragma thumb on
void ZoneMap_SetMapBgIndex(u8 *p, s32 v) {
    *(s32 *)(p + 0x54) = v;
}
#pragma thumb reset

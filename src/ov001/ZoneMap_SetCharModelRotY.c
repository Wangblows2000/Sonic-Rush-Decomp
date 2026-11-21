#include <nitro/types.h>

#pragma thumb on
void ZoneMap_SetCharModelRotY(u8 *p, s32 v) {
    *(s32 *)(p + 0x10) = v;
}
#pragma thumb reset

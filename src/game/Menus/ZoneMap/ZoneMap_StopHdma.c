#include <nitro/types.h>

extern u32 gSystemFlags;

#pragma thumb on
void ZoneMap_StopHdma(void) {
    gSystemFlags &= ~0x400;
}
#pragma thumb reset

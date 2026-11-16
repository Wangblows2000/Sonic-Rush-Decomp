#include <nitro/types.h>

#pragma thumb on
void ZoneMap_EnableBlend(BOOL on) {
    *(vu16 *)0x04000050 = on ? 0x1841 : 0;
}
#pragma thumb reset

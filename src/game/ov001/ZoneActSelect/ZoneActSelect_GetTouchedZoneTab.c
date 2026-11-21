#include <nitro/types.h>

#pragma thumb on
s32 ZoneActSelect_GetTouchedZoneTab(s32 x, s32 y) {
    s32 i;
    s32 left;

    for (i = 0, left = 0x10; i < 7; left += 0x20, i++) {
        if (x >= left && x < left + 0x20 && y >= 0 && y < 0x26) {
            return i;
        }
    }
    return -1;
}
#pragma thumb reset

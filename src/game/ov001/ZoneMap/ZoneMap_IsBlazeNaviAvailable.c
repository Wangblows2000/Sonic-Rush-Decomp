#include <nitro/types.h>

#pragma thumb on
BOOL ZoneMap_IsBlazeNaviAvailable(s32 x) {
    if (x < 9) {
        return FALSE;
    }
    if (x >= 0x1e && x < 0x21) {
        return FALSE;
    }
    return TRUE;
}
#pragma thumb reset

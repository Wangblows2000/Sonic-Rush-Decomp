#include <nitro/types.h>

#pragma thumb on
BOOL ZoneMap_IsSonicNaviAvailable(s32 x) {
    if (x >= 2) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

#include <nitro/types.h>

extern s32 gZoneMapPauseWork;

#pragma thumb on
BOOL ZoneMapPause_IsClosed(void) {
    if (gZoneMapPauseWork == 0) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

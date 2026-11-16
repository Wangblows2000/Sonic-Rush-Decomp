#include <nitro/types.h>

extern s32 gZoneActSelectWork;

#pragma thumb on
BOOL ZoneActSelect_IsFinished(void) {
    if (gZoneActSelectWork == 0) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

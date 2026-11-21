#include <nitro/types.h>

extern s32 gZoneActSelectResult;
BOOL ZoneActSelect_IsFinished(void);

#pragma thumb on
s32 ZoneActSelect_GetResult(void) {
    if (ZoneActSelect_IsFinished()) {
        return gZoneActSelectResult;
    }
    return -1;
}
#pragma thumb reset

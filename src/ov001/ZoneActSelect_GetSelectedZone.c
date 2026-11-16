#include <nitro/types.h>

extern s32 data_ov001_0231c3ec;
BOOL ZoneActSelect_IsFinished(void);

#pragma thumb on
s32 ZoneActSelect_GetSelectedZone(void) {
    if (ZoneActSelect_IsFinished()) {
        return data_ov001_0231c3ec;
    }
    return -1;
}
#pragma thumb reset

#include <nitro/types.h>

extern u32 gSaveData[];

#pragma thumb on
BOOL Options_IsExtraStoryCleared(void) {
    if (gSaveData[2] & 0x1000) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

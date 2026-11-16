#include <nitro/types.h>

extern u32 gSaveData[];

#pragma thumb on
BOOL Options_IsSoundTestUnlocked(void) {
    if (gSaveData[2] & 0x200) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

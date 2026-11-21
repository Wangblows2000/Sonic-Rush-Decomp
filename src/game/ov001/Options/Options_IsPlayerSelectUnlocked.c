#include <nitro/types.h>

extern u32 gSaveData[];

#pragma thumb on
BOOL Options_IsPlayerSelectUnlocked(void) {
    if (gSaveData[2] & 0x100) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

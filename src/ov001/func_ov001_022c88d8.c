#include <nitro/types.h>

extern u32 gSaveData[];

#pragma thumb on
BOOL func_ov001_022c88d8(void) {
    if (gSaveData[2] & 0x1000) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

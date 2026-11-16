#include <nitro/types.h>

extern u32 gSaveData[];

#pragma thumb on
BOOL func_ov001_022c8b14(void) {
    if (gSaveData[2] & 0x100) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

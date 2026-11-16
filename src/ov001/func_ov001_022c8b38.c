#include <nitro/types.h>

extern u32 gSaveData[];

#pragma thumb on
BOOL func_ov001_022c8b38(void) {
    if (gSaveData[2] & 0x200) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

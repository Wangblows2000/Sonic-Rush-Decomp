#include <nitro/types.h>

#pragma thumb on
BOOL func_ov001_022d36ac(u8 *p) {
    if (*(s32 *)(p + 0x5d4) == 0) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

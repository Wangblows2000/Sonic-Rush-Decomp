#include <nitro/types.h>

#pragma thumb on
BOOL func_ov001_022e07d0(s32 x, s32 y) {
    if (x >= 0xd8 && x < 0xf8 && y >= 0x98 && y < 0xb8) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

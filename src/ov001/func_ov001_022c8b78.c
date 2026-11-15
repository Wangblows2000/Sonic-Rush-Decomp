#include <nitro/types.h>

#pragma thumb on
BOOL func_ov001_022c8b78(u32 x, u32 y) {
    if (x >= 0x38 && x < 0x80 && y >= 0x78 && y < 0x8e) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

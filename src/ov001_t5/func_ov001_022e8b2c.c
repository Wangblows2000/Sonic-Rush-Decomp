#include <nitro/types.h>

#pragma thumb on
BOOL func_ov001_022e8b2c(s32 x, s32 y, s32 mode, s32 left) {
    s32 top;

    switch (mode) {
    case 0:
        left = 8;
        top = 0x1c;
        break;
    case 1:
        left = 8;
        top = 0x30;
        break;
    }
    if (x >= left && x < left + 0xf0 && y >= top && y < top + 0xf) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

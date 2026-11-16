#include <nitro/types.h>

#pragma thumb on
s32 func_ov001_022d7a84(u16 x, u16 y, s32 topArg) {
    s32 i;
    u16 left;
    u16 top;
    s32 dx = 0;

    left = 0x40;
    top = topArg;
    for (i = 0; i < 4; i++) {
        if (x >= left && x < left + 0x80 && y >= top && y < top + 0x18) {
            return i;
        }
        left = left + dx;
        top += 0x20;
    }
    return -1;
}
#pragma thumb reset

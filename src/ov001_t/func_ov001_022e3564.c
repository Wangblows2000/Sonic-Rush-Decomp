#include <nitro/types.h>

#pragma thumb on
s32 func_ov001_022e3564(s32 x, s32 y) {
    if (x < 0 || x >= 0x100 || y < 0 || y >= 0xc0) {
        return 5;
    }
    if (x < 0x20) {
        if (y >= 0x50 && y < 0x70) return 0;
    } else if (x >= 0xe0) {
        if (y >= 0x50 && y < 0x70) return 2;
    } else if (y < 0x20) {
        if (x >= 0x70 && x < 0x90) return 1;
    } else if (y >= 0xa0) {
        if (x >= 0x70 && x < 0x90) return 3;
    }
    return 5;
}
#pragma thumb reset

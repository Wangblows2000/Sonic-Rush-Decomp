#include <nitro/types.h>

#pragma thumb on
s32 ZoneActSelect_Unk_ov001_022e07e8(s32 x, s32 y) {
    s32 i;
    s32 top;

    for (i = 0, top = 0x48; i < 3; top += 0x20, i++) {
        if (x >= 0x28 && x < 0xc8 && y >= top && y < top + 0x20) {
            return i;
        }
    }
    return -1;
}
#pragma thumb reset

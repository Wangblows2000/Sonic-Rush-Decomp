#include <nitro/types.h>

#pragma thumb on
void func_ov001_022cdbf8(u8 *p) {
    s32 i;
    for (i = 0; i < 12; i++) {
        *(s32 *)(p + 0x1b98) = -1;
        p += 4;
    }
}
#pragma thumb reset

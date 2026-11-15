#include <nitro/types.h>

#pragma thumb on
void func_ov001_022cdc28(u8 *p) {
    s32 i;
    u8 *q;
    for (i = 0, q = p; i < 0xe; i++) {
        *(s32 *)(q + 0xbd4) = -1;
        q += 4;
    }
    for (i = 0; i < 0x29; i++) {
        *(s32 *)(p + 0x1794) = -1;
        p += 4;
    }
}
#pragma thumb reset

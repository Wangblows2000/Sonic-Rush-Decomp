#include <nitro/types.h>

#pragma thumb on
void func_ov001_022cdc5c(u8 *p) {
    s32 i;
    for (i = 0; i < 5; i++) {
        *(s32 *)(p + 0x570) = -1;
        p += 4;
    }
}
#pragma thumb reset

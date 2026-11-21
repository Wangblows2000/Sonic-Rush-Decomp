#include <nitro/types.h>

#pragma thumb on
void Options_Unk_ov001_022cdc8c(u8 *p) {
    s32 i;
    for (i = 0; i < 8; i++) {
        *(s32 *)(p + 0x7c4) = -1;
        p += 4;
    }
}
#pragma thumb reset

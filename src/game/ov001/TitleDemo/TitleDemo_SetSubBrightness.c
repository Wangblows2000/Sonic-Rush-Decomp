#include <nitro/types.h>

extern u16 data_02087d44[];

#pragma thumb on
void TitleDemo_SetSubBrightness(u8 *p, s32 v) {
    if (v < -0x10) {
        v = -0x10;
    }
    if (v > 0x10) {
        v = 0x10;
    }
    if (*(s32 *)(p + 8) != 0) {
        data_02087d44[4] = *(s32 *)(p + 0xc);
        if (v >= *(s32 *)(p + 0xc)) {
            data_02087d44[4] = v;
        }
    } else {
        data_02087d44[4] = v;
    }
}
#pragma thumb reset

#include <nitro/types.h>

extern u16 data_0208926c[];

#pragma thumb on
void TitleDemo_SetMainBrightness(u8 *p, s32 v) {
    if (v < -0x10) {
        v = -0x10;
    }
    if (v > 0x10) {
        v = 0x10;
    }
    if (*(s32 *)(p + 8) != 0) {
        data_0208926c[4] = *(s32 *)(p + 0xc);
        if (v >= *(s32 *)(p + 0xc)) {
            data_0208926c[4] = v;
        }
    } else {
        data_0208926c[4] = v;
    }
}
#pragma thumb reset

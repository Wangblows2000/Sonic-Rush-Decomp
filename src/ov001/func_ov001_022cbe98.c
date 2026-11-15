#include <nitro/types.h>

extern u16 data_02087d04[];

#pragma thumb on
void func_ov001_022cbe98(void *unused, s32 x, s32 y) {
    if (x < -0x100) {
        x = -0x100;
    }
    if (x > 0x100) {
        x = 0x100;
    }
    data_02087d04[0] = -x;
    data_02087d04[1] = -y;
    data_02087d04[2] = -x;
    data_02087d04[3] = -y;
}
#pragma thumb reset

#include <nitro/types.h>

extern u8 *data_ov001_0231f6a0;

void func_ov001_023147c0(s32 a, s32 b) {
    u8 *p = data_ov001_0231f6a0;
    *(s32 *)(p + 0x10c) = a;
    *(s32 *)(p + 0x110) = b;
}

#include <nitro/types.h>

extern u32 data_02087cb8;

#pragma thumb on
void func_ov001_022e8328(void) {
    data_02087cb8 &= ~0x400;
}
#pragma thumb reset

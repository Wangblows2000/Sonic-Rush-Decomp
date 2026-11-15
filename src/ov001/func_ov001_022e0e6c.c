#include <nitro/types.h>

extern u32 *data_ov001_0231f05c;
void func_020533e0(s32 a, s32 b);
void func_020537b4(s32 a);

#pragma thumb on
void func_ov001_022e0e6c(void) {
    if (!(data_ov001_0231f05c[4] & 2)) {
        func_020533e0(4, 0xb4);
        func_020537b4(6);
    }
}
#pragma thumb reset

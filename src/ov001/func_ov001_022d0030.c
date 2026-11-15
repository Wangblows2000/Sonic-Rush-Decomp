#include <nitro/types.h>

extern u8 *data_ov001_0231f050;
extern u16 data_02087d44[];

#pragma thumb on
void func_ov001_022d0030(u16 v) {
    if (*(s32 *)(data_ov001_0231f050 + 0xbb4) == 0) {
        data_02087d44[4] = v;
    }
}
#pragma thumb reset

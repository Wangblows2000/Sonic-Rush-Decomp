#include <nitro/types.h>

extern u8 *data_ov001_0231f050;
extern u16 data_0208926c[];

#pragma thumb on
void func_ov001_022d0050(u16 v) {
    if (*(s32 *)(data_ov001_0231f050 + 0xbb4) == 0) {
        data_0208926c[4] = v;
    }
}
#pragma thumb reset

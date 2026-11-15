#include <nitro/types.h>

extern s32 data_ov001_0231be10;
extern void *data_022b4574;
BOOL func_02050e2c(void);
void func_020371c8(void *p);

#pragma thumb on
void func_ov001_022dabe4(void) {
    if (func_02050e2c()) {
        data_ov001_0231be10 = 0;
        func_020371c8(data_022b4574);
    }
}
#pragma thumb reset

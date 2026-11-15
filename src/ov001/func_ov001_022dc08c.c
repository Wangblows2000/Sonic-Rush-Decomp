#include <nitro/types.h>

extern s32 data_ov001_0231be10;
extern void *data_022b4574;
BOOL func_0205109c(void);
void func_020371c8(void *p);

#pragma thumb on
void func_ov001_022dc08c(void) {
    if (func_0205109c()) {
        data_ov001_0231be10 = 0;
        func_020371c8(data_022b4574);
    }
}
#pragma thumb reset

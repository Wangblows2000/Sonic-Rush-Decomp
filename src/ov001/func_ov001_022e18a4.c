#include <nitro/types.h>

extern s32 data_ov001_0231c3f0;
BOOL func_ov001_022e18ec(void);

#pragma thumb on
s32 func_ov001_022e18a4(void) {
    if (func_ov001_022e18ec()) {
        return data_ov001_0231c3f0;
    }
    return -1;
}
#pragma thumb reset

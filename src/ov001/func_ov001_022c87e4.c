#include <nitro/types.h>

extern s32 data_ov001_0231f040;

#pragma thumb on
BOOL func_ov001_022c87e4(void) {
    if (data_ov001_0231f040 == 0) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

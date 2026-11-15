#include <nitro/types.h>

extern s32 data_ov001_0231f058;

#pragma thumb on
BOOL func_ov001_022ddda8(void) {
    if (data_ov001_0231f058 == 0) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

#include <nitro/types.h>

extern s32 data_ov001_0231f05c;

#pragma thumb on
BOOL func_ov001_022e18ec(void) {
    if (data_ov001_0231f05c == 0) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

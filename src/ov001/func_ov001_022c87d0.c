#include <nitro/types.h>

extern s32 data_ov001_0231f044;

#pragma thumb on
BOOL func_ov001_022c87d0(void) {
    if (data_ov001_0231f044 != 0) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

#include <nitro/types.h>

extern s32 data_ov001_0231f060;

#pragma thumb on
BOOL func_ov001_022e9570(void) {
    if (data_ov001_0231f060 == 0) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

#include <nitro/types.h>

extern s32 data_ov001_0231c588;

#pragma thumb on
BOOL func_ov001_022e955c(void) {
    if (data_ov001_0231c588 == 0) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

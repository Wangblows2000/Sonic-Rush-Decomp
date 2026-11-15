#include <nitro/types.h>

extern u16 data_022503e0[];

#pragma thumb on
BOOL func_ov000_0235fa1c(void) {
    if (data_022503e0[2] & 2)
        return TRUE;
    return FALSE;
}
#pragma thumb reset

#include <nitro/types.h>

extern u16 gPad[];

#pragma thumb on
BOOL func_ov000_0235fa1c(void) {
    if (gPad[2] & 2)
        return TRUE;
    return FALSE;
}
#pragma thumb reset

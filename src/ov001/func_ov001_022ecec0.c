#include <nitro/types.h>

extern u16 gPad[];

#pragma thumb on
BOOL func_ov001_022ecec0(void) {
    if (gPad[2] & 2) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

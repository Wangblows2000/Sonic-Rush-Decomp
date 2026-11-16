#include <nitro/types.h>

extern u16 gPad[];

#pragma thumb on
BOOL func_ov001_022da4ec(void) {
    if (gPad[2] & 1) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

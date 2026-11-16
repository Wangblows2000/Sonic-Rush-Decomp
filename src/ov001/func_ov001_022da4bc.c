#include <nitro/types.h>

extern u16 gPad[];

#pragma thumb on
BOOL func_ov001_022da4bc(void) {
    if (gPad[2] & 8) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

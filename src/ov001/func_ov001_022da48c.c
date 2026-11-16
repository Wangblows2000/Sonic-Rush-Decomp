#include <nitro/types.h>

extern u16 gPad[];

#pragma thumb on
BOOL func_ov001_022da48c(void) {
    if (gPad[4] & 0x80) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

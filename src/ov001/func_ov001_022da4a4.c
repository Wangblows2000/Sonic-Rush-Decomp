#include <nitro/types.h>

extern u16 gPad[];

#pragma thumb on
BOOL func_ov001_022da4a4(void) {
    if (gPad[4] & 0x40) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

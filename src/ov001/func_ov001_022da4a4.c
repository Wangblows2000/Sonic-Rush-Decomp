#include <nitro/types.h>

extern u16 data_022503e0[];

#pragma thumb on
BOOL func_ov001_022da4a4(void) {
    if (data_022503e0[4] & 0x40) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

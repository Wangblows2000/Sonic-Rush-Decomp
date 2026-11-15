#include <nitro/types.h>

extern u16 data_022503e0[];

#pragma thumb on
BOOL func_ov001_022da4bc(void) {
    if (data_022503e0[2] & 8) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

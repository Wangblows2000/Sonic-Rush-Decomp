#include <nitro/types.h>

extern s32 data_022c4560[];

#pragma thumb on
BOOL func_ov000_0235f9d0(void) {
    switch (data_022c4560[4 / 4]) {
    case 0:
        return FALSE;
    case 1:
        return TRUE;
    default:
        return FALSE;
    }
}
#pragma thumb reset

#include <nitro/types.h>

extern u8 data_022c4560[];

#pragma thumb on
BOOL func_ov001_022d344c(void) {
    if (data_022c4560[0x1d] <= 1) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

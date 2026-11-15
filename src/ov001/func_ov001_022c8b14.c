#include <nitro/types.h>

extern u32 data_022c4680[];

#pragma thumb on
BOOL func_ov001_022c8b14(void) {
    if (data_022c4680[2] & 0x100) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

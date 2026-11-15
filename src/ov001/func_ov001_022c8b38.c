#include <nitro/types.h>

extern u32 data_022c4680[];

#pragma thumb on
BOOL func_ov001_022c8b38(void) {
    if (data_022c4680[2] & 0x200) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

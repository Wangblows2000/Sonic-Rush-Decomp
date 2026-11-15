#include <nitro/types.h>

extern u32 data_022b65b4[];
extern u16 data_022503e0[];

#pragma thumb on
BOOL func_ov001_022d5808(void) {
    if (data_022b65b4[4] & 0x40) {
        return TRUE;
    }
    if (data_022503e0[2] & 9) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

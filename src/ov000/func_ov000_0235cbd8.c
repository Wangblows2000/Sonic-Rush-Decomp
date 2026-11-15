#include <nitro/types.h>

extern s32 data_022c4560[];

#pragma thumb on
BOOL func_ov000_0235cbd8(void) {
    if (data_022c4560[0] == data_022c4560[1])
        return TRUE;
    return FALSE;
}
#pragma thumb reset

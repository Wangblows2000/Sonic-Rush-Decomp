#include <nitro/types.h>

extern s32 data_022c45fc;

#pragma thumb on
BOOL func_02050018(void) {
    if (data_022c45fc == 0) return TRUE;
    return FALSE;
}
#pragma thumb reset

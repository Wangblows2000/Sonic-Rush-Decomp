#include <nitro/types.h>

extern s32 gVsMenuTask;

#pragma thumb on
BOOL func_ov001_022ddda8(void) {
    if (gVsMenuTask == 0) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

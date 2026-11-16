#include <nitro/types.h>

extern s32 gZoneMapPauseWork;

#pragma thumb on
BOOL func_ov001_022e9570(void) {
    if (gZoneMapPauseWork == 0) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

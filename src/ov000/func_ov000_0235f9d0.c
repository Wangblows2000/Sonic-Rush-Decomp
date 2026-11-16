#include <nitro/types.h>

extern s32 gGameState[];

#pragma thumb on
BOOL func_ov000_0235f9d0(void) {
    switch (gGameState[4 / 4]) {
    case 0:
        return FALSE;
    case 1:
        return TRUE;
    default:
        return FALSE;
    }
}
#pragma thumb reset

#include <nitro/types.h>

extern s32 gGameState[];

#pragma thumb on
BOOL TimeSelectMenu_IsMode1Flag(void) {
    switch (gGameState[8 / 4]) {
    case 0:
        return FALSE;
    case 1:
        return TRUE;
    default:
        return TRUE;
    }
}
#pragma thumb reset

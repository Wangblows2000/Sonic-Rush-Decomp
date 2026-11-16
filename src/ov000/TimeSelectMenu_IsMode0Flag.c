#include <nitro/types.h>

extern s32 gGameState[];

#pragma thumb on
BOOL TimeSelectMenu_IsMode0Flag(void) {
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

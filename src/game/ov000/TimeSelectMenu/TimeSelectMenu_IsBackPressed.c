#include <nitro/types.h>

extern u16 gPad[];

#pragma thumb on
BOOL TimeSelectMenu_IsBackPressed(void) {
    if (gPad[2] & 2)
        return TRUE;
    return FALSE;
}
#pragma thumb reset

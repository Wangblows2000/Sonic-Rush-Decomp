#include <nitro/types.h>

extern u16 gPad[];

#pragma thumb on
BOOL VsMenu_IsAPressed(void) {
    if (gPad[2] & 1) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

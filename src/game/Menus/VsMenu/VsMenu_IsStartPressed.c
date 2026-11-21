#include <nitro/types.h>

extern u16 gPad[];

#pragma thumb on
BOOL VsMenu_IsStartPressed(void) {
    if (gPad[2] & 8) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

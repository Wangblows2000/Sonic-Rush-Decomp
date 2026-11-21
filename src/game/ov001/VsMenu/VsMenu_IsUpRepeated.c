#include <nitro/types.h>

extern u16 gPad[];

#pragma thumb on
BOOL VsMenu_IsUpRepeated(void) {
    if (gPad[4] & 0x40) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

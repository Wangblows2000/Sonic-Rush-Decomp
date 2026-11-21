#include <nitro/types.h>

extern s32 gVsMenuTask;

#pragma thumb on
BOOL VsMenu_IsClosedForMode1(void) {
    if (gVsMenuTask == 0) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset

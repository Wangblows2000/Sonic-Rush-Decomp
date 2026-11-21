#include <nitro/types.h>

extern s32 gVsMenuCancelled;

#pragma thumb on
s32 VsMenu_WasCancelledForMode1(void) {
    return gVsMenuCancelled;
}
#pragma thumb reset

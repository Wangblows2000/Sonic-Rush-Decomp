#include <nitro/types.h>

extern s32 gVsMenuCancelled;

#pragma thumb on
s32 VsMenu_WasCancelledForMode0(void) {
    return gVsMenuCancelled;
}
#pragma thumb reset

#include <nitro/types.h>

extern s32 gVsMenuCancelled;

#pragma thumb on
s32 VsMenu_WasCancelledForMode2(void) {
    return gVsMenuCancelled;
}
#pragma thumb reset

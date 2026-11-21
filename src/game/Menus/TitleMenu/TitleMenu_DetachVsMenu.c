#include <nitro/types.h>

extern void *gTitleMenuVsMenuOwner;

#pragma thumb on
void TitleMenu_DetachVsMenu(void) {
    *(vu16 *)0x0400000e = (*(vu16 *)0x0400000e & 0x43) | 0x288;
    gTitleMenuVsMenuOwner = NULL;
}
#pragma thumb reset

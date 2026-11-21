#include <nitro/types.h>

extern void *gTitleMenuVsMenuOwner;

#pragma thumb on
void TitleMenu_AttachVsMenu(void *p) {
    *(vu16 *)0x0400000e = (*(vu16 *)0x0400000e & 0x43) | 0x390;
    gTitleMenuVsMenuOwner = p;
}
#pragma thumb reset

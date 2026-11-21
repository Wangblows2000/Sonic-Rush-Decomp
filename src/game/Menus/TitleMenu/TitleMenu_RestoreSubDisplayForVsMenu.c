#include <nitro/types.h>

extern void *gTitleMenuVsMenuOwner;
void TitleMenu_RestoreSubDisplay(void *p);

#pragma thumb on
void TitleMenu_RestoreSubDisplayForVsMenu(void) {
    TitleMenu_RestoreSubDisplay(gTitleMenuVsMenuOwner);
}
#pragma thumb reset

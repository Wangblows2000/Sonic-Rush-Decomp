#include <nitro/types.h>

extern void *gTitleMenuVsMenuOwner;
void TitleMenu_SetupSubDisplayForMsgBox(void *p);

#pragma thumb on
void TitleMenu_SetupMsgBoxDisplayForVsMenu(void) {
    TitleMenu_SetupSubDisplayForMsgBox(gTitleMenuVsMenuOwner);
}
#pragma thumb reset

#include <nitro/types.h>

extern void *gTitleMenuVsMenuOwner;
void TitleMenu_CreateBgSlideTaskForward(void *p, s32 a, s32 b);

#pragma thumb on
void TitleMenu_StartVsBgSlideForward(void) {
    TitleMenu_CreateBgSlideTaskForward(gTitleMenuVsMenuOwner, 0, 0);
}
#pragma thumb reset

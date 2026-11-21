#include <nitro/types.h>

extern void *gTitleMenuVsMenuOwner;
void TitleMenu_CreateBgSlideTaskReverse(void *p, s32 a, s32 b);

#pragma thumb on
void TitleMenu_StartVsBgSlideReverse(void) {
    TitleMenu_CreateBgSlideTaskReverse(gTitleMenuVsMenuOwner, 0, 0);
}
#pragma thumb reset

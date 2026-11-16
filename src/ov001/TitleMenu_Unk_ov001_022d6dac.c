#include <nitro/types.h>

extern void *gTitleMenuVsMenuOwner;
void TitleMenu_Unk_ov001_022d75dc(void *p, s32 a, s32 b);

#pragma thumb on
void TitleMenu_Unk_ov001_022d6dac(void) {
    TitleMenu_Unk_ov001_022d75dc(gTitleMenuVsMenuOwner, 0, 0);
}
#pragma thumb reset

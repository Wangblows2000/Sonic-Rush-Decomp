#include <nitro/types.h>

extern void *gTitleMenuVsMenuOwner;
void TitleMenu_Unk_ov001_022d76c4(void *p, s32 a, s32 b);

#pragma thumb on
void TitleMenu_Unk_ov001_022d6dc0(void) {
    TitleMenu_Unk_ov001_022d76c4(gTitleMenuVsMenuOwner, 0, 0);
}
#pragma thumb reset

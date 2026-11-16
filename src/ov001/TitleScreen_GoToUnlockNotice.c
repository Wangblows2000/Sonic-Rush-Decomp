#include <nitro/types.h>

extern u8 gPad[];
void Pad_Reset(void *p, s32 a, s32 b);
void PostStaffRoll_ShowUnlockNotices(s32 a);

#pragma thumb on
void TitleScreen_GoToUnlockNotice(void) {
    Pad_Reset(gPad, 0, 0);
    PostStaffRoll_ShowUnlockNotices(1);
}
#pragma thumb reset

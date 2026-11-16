#include <nitro/types.h>

extern s32 gSaveDataBusy;
extern u32 gSystemFlags;
void PostStaffRoll_ShowPlayerSelectUnlockNotice(void);

#pragma thumb on
void PostStaffRoll_ShowUnlockNotices(void) {
    PostStaffRoll_ShowPlayerSelectUnlockNotice();
    gSaveDataBusy = 0;
    gSystemFlags &= ~0x10;
}
#pragma thumb reset

#include <nitro/types.h>

extern s32 gSaveDataBusy;
extern u32 gSystemFlags;
void PostStaffRoll_Begin(void);

#pragma thumb on
void PostStaffRoll_Start(void) {
    PostStaffRoll_Begin();
    gSaveDataBusy = 0;
    gSystemFlags &= ~0x10;
}
#pragma thumb reset

#include <nitro/types.h>

extern s32 gSaveDataBusy;
extern u32 gSystemFlags;
void func_ov001_022cf43c(void);

#pragma thumb on
void PostStaffRoll_Start(void) {
    func_ov001_022cf43c();
    gSaveDataBusy = 0;
    gSystemFlags &= ~0x10;
}
#pragma thumb reset

#include <nitro/types.h>

extern s32 gSaveDataBusy;
extern u32 gSystemFlags;
void func_ov001_022cf3b0(void);

#pragma thumb on
void func_ov001_022cf4a0(void) {
    func_ov001_022cf3b0();
    gSaveDataBusy = 0;
    gSystemFlags &= ~0x10;
}
#pragma thumb reset

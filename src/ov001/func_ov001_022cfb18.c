#include <nitro/types.h>

extern s32 gSaveDataBusy;
extern u32 gSystemFlags;
void func_ov001_022cf93c(void);
void func_ov001_022cfac8(void);

#pragma thumb on
void func_ov001_022cfb18(void) {
    func_ov001_022cf93c();
    func_ov001_022cfac8();
    gSaveDataBusy = 0;
    gSystemFlags &= ~0x10;
}
#pragma thumb reset

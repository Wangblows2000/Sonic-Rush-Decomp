#include <nitro/types.h>

extern s32 gSaveDataBusy;
extern u32 gSystemFlags;
void func_ov001_022d4f30(void);

#pragma thumb on
void func_ov001_022d4f78(void) {
    func_ov001_022d4f30();
    gSaveDataBusy = 0;
    gSystemFlags &= ~0x10;
}
#pragma thumb reset

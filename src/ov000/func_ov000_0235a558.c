#include <nitro/types.h>

extern s32 gSaveDataBusy;
extern u32 gSystemFlags;

void func_ov000_0235a2e8(s32 a);

#pragma thumb on
void func_ov000_0235a558(void) {
    func_ov000_0235a2e8(3);
    gSaveDataBusy = 0;
    gSystemFlags &= ~0x10;
}
#pragma thumb reset

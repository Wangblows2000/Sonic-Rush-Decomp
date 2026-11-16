#include <nitro/types.h>

extern s32 gSaveDataBusy;
extern u32 gSystemFlags;

void func_ov000_0235a2e8(s32 a);

#pragma thumb on
void func_ov000_0235a530(void) {
    func_ov000_0235a2e8(4);
    gSaveDataBusy = 0;
    gSystemFlags &= ~0x10;
}
#pragma thumb reset

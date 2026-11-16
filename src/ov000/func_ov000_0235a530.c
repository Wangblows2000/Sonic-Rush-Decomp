#include <nitro/types.h>

extern s32 gSaveDataBusy;
extern u32 gSystemFlags;

void PlayerSelectMenu_Create(s32 a);

#pragma thumb on
void func_ov000_0235a530(void) {
    PlayerSelectMenu_Create(4);
    gSaveDataBusy = 0;
    gSystemFlags &= ~0x10;
}
#pragma thumb reset

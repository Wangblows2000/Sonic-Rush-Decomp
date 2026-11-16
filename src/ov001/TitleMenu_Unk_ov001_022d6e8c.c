#include <nitro/types.h>

extern s32 gGameState[];
void VsConnect_CreateHostSync(void);

#pragma thumb on
void TitleMenu_Unk_ov001_022d6e8c(void) {
    gGameState[4] = 1;
    gGameState[6] |= 0x20;
    gGameState[6] &= ~0x10;
    VsConnect_CreateHostSync();
}
#pragma thumb reset

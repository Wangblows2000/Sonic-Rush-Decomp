#include <nitro/types.h>

extern s32 gGameState[];
void VsConnect_CreateJoinSync(void);

#pragma thumb on
void TitleMenu_Unk_ov001_022d6e6c(void) {
    gGameState[4] = 1;
    gGameState[6] |= 0x20;
    gGameState[6] &= ~0x10;
    VsConnect_CreateJoinSync();
}
#pragma thumb reset

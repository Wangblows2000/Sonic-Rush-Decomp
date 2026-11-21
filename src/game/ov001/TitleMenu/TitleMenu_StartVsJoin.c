#include <nitro/types.h>

extern s32 gGameState[];
void VsConnect_CreateJoinSync(void);

#pragma thumb on
void TitleMenu_StartVsJoin(void) {
    gGameState[4] = 1;
    gGameState[6] |= 0x20;
    gGameState[6] &= ~0x10;
    VsConnect_CreateJoinSync();
}
#pragma thumb reset

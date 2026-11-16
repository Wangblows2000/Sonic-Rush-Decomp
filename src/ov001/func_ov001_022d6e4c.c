#include <nitro/types.h>

extern s32 gGameState[];
void SceneChange_ToVsLobby(void);

#pragma thumb on
void func_ov001_022d6e4c(void) {
    gGameState[4] = 1;
    gGameState[6] |= 0x20;
    gGameState[6] |= 0x10;
    SceneChange_ToVsLobby();
}
#pragma thumb reset

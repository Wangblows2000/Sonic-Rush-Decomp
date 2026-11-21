/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gGameState[];
s32 Wireless_EnableErrorCallback();
s32 SceneChange_ToVsStage();

void Game_StartVsStage(s32 p0, s32 p1) {
    *(u32 *)((u8 *)gGameState + 0x10) = 1;
    *(u8 *)((u8 *)gGameState + 0x1c) = p0;
    *(u8 *)((u8 *)gGameState + 0x1d) = p1;
    Wireless_EnableErrorCallback(1, p1, gGameState, 1);
    SceneChange_ToVsStage();
}

#pragma thumb reset

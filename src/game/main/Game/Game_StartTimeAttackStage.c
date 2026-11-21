/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gGameState[];
s32 SceneChange_ToTimeAttackStage();

void Game_StartTimeAttackStage(s32 p0, s32 p1) {
    *(u32 *)((u8 *)gGameState + 0x10) = 2;
    s32 t0 = (*(u32 *)((u8 *)gGameState + 0x18) & ~0x20);
    *(u32 *)((u8 *)gGameState + 0x18) = t0;
    *(u32 *)((u8 *)gGameState + 0x18) = (t0 & ~0x10);
    if ((s32)p0 >= 0) {
        *(u8 *)((u8 *)gGameState + 0x1c) = p0;
    }
    s32 v1 = (s32)p0;
    if ((s32)p1 >= 0) {
        *(u8 *)((u8 *)gGameState + 0x1d) = p1;
        v1 = (s32)gGameState;
    }
    SceneChange_ToTimeAttackStage(v1, p1);
}

#pragma thumb reset

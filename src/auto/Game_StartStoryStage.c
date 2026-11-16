/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gGameState[];
s32 StoryProgress_GetZoneAt();
s32 func_0204e6ac();
s32 StoryProgress_RestoreFromSave();
s32 func_02064870();
s32 SceneChange_ToStage();
s32 DemoScript_StartScript0();

void Game_StartStoryStage(s32 p0, s32 p1) {
    *(u32 *)((u8 *)gGameState + 0x10) = 0;
    s32 t0 = (*(u32 *)((u8 *)gGameState + 0x18) & ~0x20);
    *(u32 *)((u8 *)gGameState + 0x18) = t0;
    *(u32 *)((u8 *)gGameState + 0x18) = (t0 & ~0x10);
    s32 t1 = StoryProgress_GetZoneAt(((u32)((s32)p0 << 0x18) >> 0x18), 0x10, gGameState, (t0 & ~0x10));
    *(u8 *)((u8 *)gGameState + 0x1c) = t1;
    s32 t2 = func_0204e6ac(((u32)((s32)p1 << 0x18) >> 0x18), gGameState);
    *(u8 *)((u8 *)gGameState + 0x1d) = t2;
    StoryProgress_RestoreFromSave(t2, gGameState);
    func_02064870();
    s32 t3 = *(u8 *)((u8 *)gGameState + 0x1d);
    if ((u32)t3 < (u32)2) {
        SceneChange_ToStage(t3);
        return;
    }
    DemoScript_StartScript0();
}

#pragma thumb reset

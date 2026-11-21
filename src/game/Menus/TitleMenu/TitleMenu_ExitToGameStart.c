/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gSaveData[];
extern u8 gGameState[];
s32 StoryProgress_RestoreFromSave();
s32 NaviM_ResetPending();
s32 SysSound_Release();
s32 Game_StartStoryStage();
s32 SceneChange_ToZoneActSelectMode();
s32 SceneChange_ToZoneMap();
s32 SceneChange_ToPlayerSelectMenuMode0();

void TitleMenu_ExitToGameStart(void) {
    s32 t0 = *(u16 *)((u8 *)gSaveData + 0xc);
    *(u32 *)gGameState = 0;
    StoryProgress_RestoreFromSave(gGameState, 0);
    NaviM_ResetPending();
    if (t0 == 0) {
        SysSound_Release();
        *(u32 *)gGameState = 0;
        Game_StartStoryStage(0, 0);
        return;
    }
    if (t0 == 1) {
        SceneChange_ToZoneActSelectMode();
        return;
    }
    if (t0 == 2) {
        SysSound_Release();
        *(u32 *)gGameState = 0;
        *(u32 *)((u8 *)gGameState + 0x10) = 0;
        s32 t1 = (*(u32 *)((u8 *)gGameState + 0x18) & ~0x20);
        *(u32 *)((u8 *)gGameState + 0x18) = t1;
        *(u32 *)((u8 *)gGameState + 0x18) = (t1 & ~0x10);
        SceneChange_ToZoneMap(0x10, gGameState, (t1 & ~0x10));
        return;
    }
    *(u32 *)((u8 *)gGameState + 0x10) = 0;
    s32 t2 = (*(u32 *)((u8 *)gGameState + 0x18) & ~0x20);
    *(u32 *)((u8 *)gGameState + 0x18) = t2;
    *(u32 *)((u8 *)gGameState + 0x18) = (t2 & ~0x10);
    SceneChange_ToPlayerSelectMenuMode0(0x10, gGameState, (t2 & ~0x10));
}

#pragma thumb reset

#include <nitro/types.h>

typedef struct {
    s32 unk0;
    u8 pad4[0xc];
    s32 unk10;
    u8 pad14[8];
    u8 unk1c;
    u8 unk1d;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0xc];
    u16 unkc;
} UnkStruct_022c4680;

extern UnkStruct_022c4560 gGameState;
extern u16 data_022c45f8;
extern UnkStruct_022c4680 gSaveData;

void StageExit_Create(void (*f)(void), s32 a);
void SceneChange_ToTryAgain(void);
void SceneChange_ToExtraEnding(void);
void SceneChange_ToEnding(void);
void Game_AdvanceAct(void);
void Game_AdvanceActWithDemo(void);
void NaviMsg_CloseFiles(void);
void NaviB_Release(void);
void DemoScript_StartScript4(void);
void SceneChange_ToZoneMap_1_0(void);
void SceneChange_ToZoneActSelectMode(void);
void SceneChange_ToZoneMap_1_1(void);

void ClearDemo_GoToNextScene(void) {
    u8 mode;

    if (gGameState.unk10 == 2) {
        StageExit_Create(SceneChange_ToTryAgain, 3);
        return;
    }
    mode = gGameState.unk1c;
    if (mode == 8) {
        SceneChange_ToExtraEnding();
        return;
    }
    if (mode == 7) {
        StageExit_Create(SceneChange_ToEnding, 3);
        return;
    }
    if (data_022c45f8 & 1) {
        data_022c45f8 &= ~1;
        switch (gGameState.unk1d) {
        case 0:
            StageExit_Create(Game_AdvanceAct, 3);
            return;
        case 1:
            StageExit_Create(Game_AdvanceActWithDemo, 3);
            return;
        case 2:
            if (mode == 6) {
                NaviMsg_CloseFiles();
                NaviB_Release();
                StageExit_Create(DemoScript_StartScript4, 1);
                return;
            }
            NaviMsg_CloseFiles();
            NaviB_Release();
            StageExit_Create(SceneChange_ToZoneMap_1_0, 1);
            return;
        }
    } else {
        switch (gGameState.unk1d) {
        case 0:
        case 1:
            if (gGameState.unk0 != 1 && mode == 0 && gSaveData.unkc < 2) {
                StageExit_Create(SceneChange_ToZoneActSelectMode, 3);
                return;
            }
            StageExit_Create(SceneChange_ToZoneMap_1_1, 3);
            return;
        case 2:
            NaviMsg_CloseFiles();
            NaviB_Release();
            StageExit_Create(SceneChange_ToZoneMap_1_1, 1);
            return;
        }
    }
}

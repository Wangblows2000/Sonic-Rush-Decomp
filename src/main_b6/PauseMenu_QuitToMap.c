#include <nitro/types.h>

typedef struct {
    s32 unk0;
    u8 pad4[0x18];
    u8 zone;
    u8 act;
} UnkState_020554dc;

typedef struct {
    u8 pad0[0xc];
    u16 unkc;
} UnkProg_020554dc;

extern UnkState_020554dc gGameState;
extern UnkProg_020554dc gSaveData;

void SaveData_StoreLives(void);
void SaveData_CommitOrShowError(void (*f)(void));
BOOL StoryProgress_IsStageNew(void);
void SceneChange_ToZoneActSelectMode(void);
void SceneChange_ToZoneMap_1_0(void);
void SceneChange_ToZoneMap_1_1(void);

void PauseMenu_QuitToMap(void) {
    SaveData_StoreLives();
    SaveData_CommitOrShowError(NULL);
    if (gGameState.unk0 != 1 && gGameState.zone == 0 && gSaveData.unkc < 2) {
        SceneChange_ToZoneActSelectMode();
        return;
    }
    if (StoryProgress_IsStageNew() || (gGameState.zone == 7 && gGameState.act == 2))
        SceneChange_ToZoneMap_1_0();
    else
        SceneChange_ToZoneMap_1_1();
}

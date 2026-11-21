#include <nitro/types.h>

typedef struct {
    s32 mode;
} UnkWork_022d2db8;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d2db8 *work;
} UnkTask_022d2db8;

extern UnkTask_022d2db8 *gTaskCurrent;
extern s32 gStaffRollWork;

void Task_DestroyByTag(s32 a);
void Touch_Disable(void);
void SaveData_SetSonicProgress(s32 a);
void SaveData_SetBlazeProgress(s32 a);
void SaveData_SetGameCleared(void);
BOOL SaveData_CommitOrShowError(s32 a);
void SceneChange_ToPostStaffRoll(void);

#pragma thumb on
void StaffRoll_State_Finish(void) {
    s32 mode;

    mode = gTaskCurrent->work->mode;
    gStaffRollWork = 0;
    Task_DestroyByTag(0x1173);
    Touch_Disable();
    switch (mode) {
    case 0:
        SaveData_SetSonicProgress(0x1a);
        break;
    case 1:
        SaveData_SetBlazeProgress(0x1c);
        break;
    case 2:
        SaveData_SetGameCleared();
        break;
    }
    if (SaveData_CommitOrShowError(0)) {
        SceneChange_ToPostStaffRoll();
    }
}
#pragma thumb reset

#include <nitro/types.h>

extern u32 gSaveData[];

void SaveData_CommitOrShowError(s32 a);

#pragma thumb on
void PlayerSelectMenu_MarkUnlockNoticeShown(void) {
    gSaveData[2] |= 0x200000;
    SaveData_CommitOrShowError(0);
}
#pragma thumb reset

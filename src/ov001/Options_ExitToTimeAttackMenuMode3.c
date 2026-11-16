#include <nitro/types.h>

void SaveData_CommitOrShowError(void (*f)(void));
void SceneChange_ToTimeAttackMenuMode3(void);

#pragma thumb on
void Options_ExitToTimeAttackMenuMode3(void) {
    SaveData_CommitOrShowError(SceneChange_ToTimeAttackMenuMode3);
}
#pragma thumb reset

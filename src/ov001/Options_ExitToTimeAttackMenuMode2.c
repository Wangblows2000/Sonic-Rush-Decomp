#include <nitro/types.h>

void SaveData_CommitOrShowError(void (*f)(void));
void SceneChange_ToTimeAttackMenuMode2(void);

#pragma thumb on
void Options_ExitToTimeAttackMenuMode2(void) {
    SaveData_CommitOrShowError(SceneChange_ToTimeAttackMenuMode2);
}
#pragma thumb reset

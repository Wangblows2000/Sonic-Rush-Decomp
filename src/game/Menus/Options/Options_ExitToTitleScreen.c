#include <nitro/types.h>

void SysSound_Release(void);
void SaveData_CommitOrShowError(void (*f)(void));
void SceneChange_ToTitleScreen(void);

#pragma thumb on
void Options_ExitToTitleScreen(void) {
    SysSound_Release();
    SaveData_CommitOrShowError(SceneChange_ToTitleScreen);
}
#pragma thumb reset

#include <nitro/types.h>

void SysSound_Release(void);
void func_0205a4ec(void (*f)(void));
void SceneChange_Unk_02054e68(void);

#pragma thumb on
void Options_ExitToTitleScreen(void) {
    SysSound_Release();
    func_0205a4ec(SceneChange_Unk_02054e68);
}
#pragma thumb reset

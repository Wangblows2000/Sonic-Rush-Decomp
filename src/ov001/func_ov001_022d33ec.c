#include <nitro/types.h>

void SysSound_Release(void);
void SceneChange_Unk_02054e68(void);

#pragma thumb on
void func_ov001_022d33ec(void) {
    SysSound_Release();
    SceneChange_Unk_02054e68();
}
#pragma thumb reset

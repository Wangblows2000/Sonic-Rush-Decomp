#include <nitro/types.h>

void Task_DestroyByTag(s32 a);
void Touch_Disable(void);
void SceneChange_Unk_02054ed8(void);

void func_ov001_022c8458(void) {
    Task_DestroyByTag(0x1173);
    Touch_Disable();
    SceneChange_Unk_02054ed8();
}

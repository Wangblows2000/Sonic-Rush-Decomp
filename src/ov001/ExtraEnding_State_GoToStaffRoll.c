#include <nitro/types.h>

void Task_DestroyByTag(s32 a);
void SceneChange_ToExtraStaffRoll(void);

void ExtraEnding_State_GoToStaffRoll(void) {
    Task_DestroyByTag(0x1173);
    SceneChange_ToExtraStaffRoll();
}

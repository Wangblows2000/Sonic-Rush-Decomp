#include <nitro/types.h>

void Task_DestroyByTag(s32 a);
void Touch_Disable(void);
void SceneChange_ToStaffRoll(void);

void Ending_State_GoToStaffRoll(void) {
    Task_DestroyByTag(0x1173);
    Touch_Disable();
    SceneChange_ToStaffRoll();
}

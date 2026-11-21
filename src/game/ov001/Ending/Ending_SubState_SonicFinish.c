#include <nitro/types.h>

extern void *gTaskCurrent;
extern u8 gDisplayMain[];
extern u8 gDisplaySub[];
void Ending_State_GoToStaffRoll(void);
void Ending_SetSubState(void *arg, s32 a1);

void Ending_SubState_SonicFinish(void *arg) {
    *(s16 *)(gDisplayMain + 0x48) = 16;
    *(s16 *)(gDisplaySub + 0x48) = 16;
    *(void (**)(void))((u8 *)gTaskCurrent + 8) = Ending_State_GoToStaffRoll;
    Ending_SetSubState(arg, 0);
}

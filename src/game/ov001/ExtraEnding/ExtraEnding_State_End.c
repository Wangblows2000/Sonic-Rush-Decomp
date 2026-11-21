#include <nitro/types.h>

extern void *gTaskCurrent;
extern u8 gDisplayMain[];
extern u8 gDisplaySub[];
void ExtraEnding_State_GoToStaffRoll(void);
void ExtraEnding_State_None(void);
void ExtraEnding_SetState(void *arg, void (*fn)(void));

void ExtraEnding_State_End(void *arg) {
    *(s16 *)(gDisplayMain + 0x48) = -16;
    *(s16 *)(gDisplaySub + 0x48) = -16;
    *(void (**)(void))((u8 *)gTaskCurrent + 8) = ExtraEnding_State_GoToStaffRoll;
    ExtraEnding_SetState(arg, ExtraEnding_State_None);
}

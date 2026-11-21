#include <nitro/types.h>

void Task_Create(void (*f)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void TimeAttackMenuBgScroll_Main(void);

#pragma thumb on
void TimeAttackMenu_CreateBgScrollTask(void) {
    Task_Create(TimeAttackMenuBgScroll_Main, 0, 0, 0x2000, 0x1000, 0);
}
#pragma thumb reset

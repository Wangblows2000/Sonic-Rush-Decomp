#include <nitro/types.h>

void Task_Create(void (*f)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void TryAgainBgScroll_Main(void);

#pragma thumb on
void TryAgain_CreateBgScrollTask(void) {
    Task_Create(TryAgainBgScroll_Main, 0, 0, 0x3000, 0x1000, 0);
}
#pragma thumb reset

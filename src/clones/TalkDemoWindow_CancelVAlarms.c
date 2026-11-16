#include <nitro/types.h>

void OS_CancelAllVAlarms(void);
void func_02007584(void);

#pragma thumb on
void TalkDemoWindow_CancelVAlarms(void) {
    OS_CancelAllVAlarms();
    func_02007584();
}
#pragma thumb reset

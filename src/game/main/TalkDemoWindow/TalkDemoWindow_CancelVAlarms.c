#include <nitro/types.h>

void OS_CancelAllVAlarms(void);
void OS_EndVAlarm(void);

#pragma thumb on
void TalkDemoWindow_CancelVAlarms(void) {
    OS_CancelAllVAlarms();
    OS_EndVAlarm();
}
#pragma thumb reset

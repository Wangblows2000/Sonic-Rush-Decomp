#include <nitro/types.h>

void OS_CancelAllVAlarms(void);
void OS_EndVAlarm(void);

void func_ov000_0233d538(void) {
    OS_CancelAllVAlarms();
    OS_EndVAlarm();
}

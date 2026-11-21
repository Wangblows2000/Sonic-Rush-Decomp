#include <nitro/types.h>

void OS_CancelAllVAlarms(void);
void OS_EndVAlarm(void);

void ZoneFx_Zone2_StopVAlarms(void) {
    OS_CancelAllVAlarms();
    OS_EndVAlarm();
}

#include <nitro/types.h>

void OS_InitVAlarm(u8 *p);
void OS_CreateVAlarm(void *p);

void ZoneFx_Zone2_InitVAlarms(u8 *p) {
    OS_InitVAlarm(p);
    OS_CreateVAlarm(p + 0xc);
    OS_CreateVAlarm(p + 0x34);
    OS_CreateVAlarm(p + 0x5c);
    OS_CreateVAlarm(p + 0x84);
}

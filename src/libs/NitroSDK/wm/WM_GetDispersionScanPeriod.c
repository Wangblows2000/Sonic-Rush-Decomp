#include <nitro/types.h>

void OS_GetMacAddress(u8 *mac);

u16 WM_GetDispersionScanPeriod(void)
{
    u8 mac[6];
    s32 i;
    u16 sum;

    OS_GetMacAddress(mac);
    for (i = 0, sum = 0; i < 6; i++) {
        sum += mac[i];
    }
    sum += *(vu32 *)0x027ffc3c;
    sum *= 13;
    return sum % 10 + 30;
}

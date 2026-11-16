#include <nitro/types.h>

void OS_GetMacAddress(u8 *mac);

u32 MBi_GetBeaconPeriodDispersion(void)
{
    u8 mac[6];
    s32 i;
    u32 sum;

    OS_GetMacAddress(mac);
    for (i = 0, sum = 0; i < 6; i++) {
        sum += mac[i];
    }
    sum = (sum + *(vu32 *)0x027ffc3c) * 7;
    return sum % 20;
}

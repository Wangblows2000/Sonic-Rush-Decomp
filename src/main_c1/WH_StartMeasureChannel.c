#include <nitro/types.h>

extern u32 data_022c1988;
extern u16 data_022c1960;
extern u16 data_022c1958;

void OS_GetMacAddress(u16 *mac);
void WH_ChangeSysState(s32 a);
void WH_SetError(s32 v);
s32 WH_StateInMeasureChannel(u16 ch);

BOOL WH_StartMeasureChannel(void)
{
    u16 mac[4];
    u32 seed;
    s32 r;

    OS_GetMacAddress(mac);
    seed = *(u32 *)0x027ffc3c;
    seed = mac[2] + (mac[1] + (mac[0] + seed));
    *(volatile u32 *)&data_022c1988 = seed;
    data_022c1988 = seed * 0x10dcd + 0x3039;
    data_022c1960 = 0;
    data_022c1958 = 0x65;
    WH_ChangeSysState(3);
    r = WH_StateInMeasureChannel(1);
    if (r == 0x18) {
        WH_SetError(0x18);
        WH_ChangeSysState(9);
        return FALSE;
    }
    if (r == 2)
        return TRUE;
    WH_SetError(r);
    WH_ChangeSysState(9);
    return FALSE;
}

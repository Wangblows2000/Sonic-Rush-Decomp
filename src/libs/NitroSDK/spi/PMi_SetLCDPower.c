#include <nitro/types.h>

extern u32 data_0207f094;

u32 PMi_SetLED(u32 a);
u32 PMi_SetLEDAsync(u32 a, u32 b, u32 c);

#define HW_VBLANK_COUNT (*(vu32 *)0x027ffc3c)
#define REG_POWCNT      (*(vu16 *)0x04000304)

BOOL PMi_SetLCDPower(u32 sw, u32 led, BOOL force, BOOL sync)
{
    switch (sw) {
    case 1:
        if (!force && HW_VBLANK_COUNT - data_0207f094 <= 7) {
            return FALSE;
        }
        if (led) {
            if (sync) {
                PMi_SetLED(led);
            } else {
                PMi_SetLEDAsync(led, 0, 0);
            }
        }
        REG_POWCNT |= 1;
        break;
    case 0:
        REG_POWCNT &= ~1;
        data_0207f094 = HW_VBLANK_COUNT;
        if (led) {
            if (sync) {
                PMi_SetLED(led);
            } else {
                PMi_SetLEDAsync(led, 0, 0);
            }
        }
        break;
    }
    return TRUE;
}

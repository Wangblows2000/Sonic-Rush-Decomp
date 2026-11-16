#include <nitro/types.h>

extern u32 data_0207f094;

u32 func_0200e4b0(u32 a);
u32 func_0200e4ec(u32 a, u32 b, u32 c);

#define HW_VBLANK_COUNT (*(vu32 *)0x027ffc3c)
#define REG_POWCNT      (*(vu16 *)0x04000304)

BOOL func_0200dfa8(u32 sw, u32 led, BOOL force, BOOL sync)
{
    switch (sw) {
    case 1:
        if (!force && HW_VBLANK_COUNT - data_0207f094 <= 7) {
            return FALSE;
        }
        if (led) {
            if (sync) {
                func_0200e4b0(led);
            } else {
                func_0200e4ec(led, 0, 0);
            }
        }
        REG_POWCNT |= 1;
        break;
    case 0:
        REG_POWCNT &= ~1;
        data_0207f094 = HW_VBLANK_COUNT;
        if (led) {
            if (sync) {
                func_0200e4b0(led);
            } else {
                func_0200e4ec(led, 0, 0);
            }
        }
        break;
    }
    return TRUE;
}

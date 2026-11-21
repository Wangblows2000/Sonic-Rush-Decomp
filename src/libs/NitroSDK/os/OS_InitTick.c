#include <nitro/types.h>

extern vu16 data_0207d36c;
extern volatile u64 data_0207d374;
extern u32 data_0207d370;

void OSi_SetTimerReserved(u32 timer);
void OSi_CountUpTick(void);
u32 OS_SetIrqFunction(u32 mask, void (*handler)(void));
u32 OS_EnableIrqMask(u32 mask);

#define REG_TM0CNT_L (*(vu16 *)0x04000100)
#define REG_TM0CNT_H (*(vu16 *)0x04000102)

void OS_InitTick(void)
{
    if (data_0207d36c) {
        return;
    }
    data_0207d36c = TRUE;
    OSi_SetTimerReserved(0);
    data_0207d374 = 0;
    REG_TM0CNT_H = 0;
    REG_TM0CNT_L = 0;
    REG_TM0CNT_H = 0xc1;
    OS_SetIrqFunction(8, OSi_CountUpTick);
    OS_EnableIrqMask(8);
    data_0207d370 = FALSE;
}

#include <nitro/types.h>

extern volatile u64 data_0207d374;
extern u32 data_0207d370;

void OSi_EnterTimerCallback(u32 timer, void (*callback)(void *), void *arg);

#define REG_TM0CNT_L (*(vu16 *)0x04000100)
#define REG_TM0CNT_H (*(vu16 *)0x04000102)

void OSi_CountUpTick(void)
{
    data_0207d374++;
    if (data_0207d370) {
        REG_TM0CNT_H = 0;
        REG_TM0CNT_L = 0;
        REG_TM0CNT_H = 0xc1;
        data_0207d370 = FALSE;
    }
    OSi_EnterTimerCallback(0, (void (*)(void *))OSi_CountUpTick, 0);
}

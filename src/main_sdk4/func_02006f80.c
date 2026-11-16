#include <nitro/types.h>

extern vu16 data_0207d36c;
extern volatile u64 data_0207d374;
extern u32 data_0207d370;

void func_02006edc(u32 timer);
void func_02006ef8(void);
u32 func_02005520(u32 mask, void (*handler)(void));
u32 OS_EnableIrqMask(u32 mask);

#define REG_TM0CNT_L (*(vu16 *)0x04000100)
#define REG_TM0CNT_H (*(vu16 *)0x04000102)

void func_02006f80(void)
{
    if (data_0207d36c) {
        return;
    }
    data_0207d36c = TRUE;
    func_02006edc(0);
    data_0207d374 = 0;
    REG_TM0CNT_H = 0;
    REG_TM0CNT_L = 0;
    REG_TM0CNT_H = 0xc1;
    func_02005520(8, func_02006ef8);
    OS_EnableIrqMask(8);
    data_0207d370 = FALSE;
}

#include <nitro/types.h>

extern volatile u64 data_0207d374;
extern u32 data_0207d370;

void func_020053fc(u32 timer, void (*callback)(void *), void *arg);

#define REG_TM0CNT_L (*(vu16 *)0x04000100)
#define REG_TM0CNT_H (*(vu16 *)0x04000102)

void func_02006ef8(void)
{
    data_0207d374++;
    if (data_0207d370) {
        REG_TM0CNT_H = 0;
        REG_TM0CNT_L = 0;
        REG_TM0CNT_H = 0xc1;
        data_0207d370 = FALSE;
    }
    func_020053fc(0, (void (*)(void *))func_02006ef8, 0);
}

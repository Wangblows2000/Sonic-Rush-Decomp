#include <nitro/types.h>

typedef void (*UnkExcpHandler_02006de0)(void *context, void *arg);

extern UnkExcpHandler_02006de0 data_0207d2dc;
extern void *data_0207d2e0;
extern u8 data_0207d2e8[];

void OS_EnableProtectionUnit(void);
void OS_DisableProtectionUnit(void);

void func_02006de0(void)
{
    if (data_0207d2dc == NULL) {
        return;
    }
    asm {
        mov r0, sp
        ldr r1, =0x9f
        msr cpsr_fsxc, r1
        mov sp, r0
    }
    OS_EnableProtectionUnit();
    data_0207d2dc(data_0207d2e8, data_0207d2e0);
    OS_DisableProtectionUnit();
}

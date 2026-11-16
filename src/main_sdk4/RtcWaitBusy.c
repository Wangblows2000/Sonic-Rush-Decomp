#include <nitro/types.h>

extern u32 data_0207f0e4;

asm void RtcWaitBusy(void)
{
    ldr r12, =data_0207f0e4
_4:
    ldr r0, [r12]
    cmp r0, #1
    beq _4
    bx lr
}

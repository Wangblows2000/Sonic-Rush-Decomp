#include <nitro/types.h>

asm void OS_GetProcMode(void)
{
    mrs r0, cpsr
    and r0, r0, #0x1f
    bx lr
}


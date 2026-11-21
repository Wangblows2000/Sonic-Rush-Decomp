#include <nitro/types.h>

asm void OS_SetProtectionRegion1(void)
{
    mcr p15, 0, r0, c6, c1, 0
    bx lr
}


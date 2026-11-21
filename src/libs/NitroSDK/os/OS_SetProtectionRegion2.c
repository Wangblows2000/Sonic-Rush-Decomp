#include <nitro/types.h>

asm void OS_SetProtectionRegion2(void)
{
    mcr p15, 0, r0, c6, c2, 0
    bx lr
}


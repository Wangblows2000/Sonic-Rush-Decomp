#include <nitro/types.h>

asm void OS_DisableProtectionUnit(void)
{
    mrc p15, 0, r0, c1, c0, 0
    bic r0, r0, #1
    mcr p15, 0, r0, c1, c0, 0
    bx lr
}


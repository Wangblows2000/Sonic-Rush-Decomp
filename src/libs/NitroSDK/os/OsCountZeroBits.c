#include <nitro/types.h>

asm void OsCountZeroBits(void)
{
    clz r0, r0
    bx lr
}


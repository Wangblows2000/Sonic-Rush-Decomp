#include <nitro/types.h>

asm void MI_SwapWord(void)
{
    swp r0, r0, [r1]
    bx lr
}


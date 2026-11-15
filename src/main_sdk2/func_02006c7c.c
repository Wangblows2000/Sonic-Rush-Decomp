#include <nitro/types.h>

asm void func_02006c7c(void)
{
    mrc p15, 0, r0, c9, c1, 0
    ldr r1, =0xfffff000
    and r0, r0, r1
    bx lr
}


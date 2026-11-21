#include <nitro/types.h>

asm void MIi_CpuCopy32(void)
{
    add r12, r1, r2
@1:
    cmp r1, r12
    ldmltia r0!, {r2}
    stmltia r1!, {r2}
    blt @1
    bx lr
}


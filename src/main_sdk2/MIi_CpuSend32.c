#include <nitro/types.h>

asm void MIi_CpuSend32(void)
{
    add r12, r0, r2
@1:
    cmp r0, r12
    ldmltia r0!, {r2}
    strlt r2, [r1]
    blt @1
    bx lr
}


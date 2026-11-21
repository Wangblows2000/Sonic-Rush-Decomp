#include <nitro/types.h>

asm void MIi_CpuCopy16(void)
{
    mov r12, #0
@1:
    cmp r12, r2
    ldrlth r3, [r0, r12]
    strlth r3, [r1, r12]
    addlt r12, r12, #2
    blt @1
    bx lr
}


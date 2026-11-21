#include <nitro/types.h>

asm void MIi_CpuClear16(void)
{
    mov r3, #0
@1:
    cmp r3, r2
    strlth r0, [r1, r3]
    addlt r3, r3, #2
    blt @1
    bx lr
}


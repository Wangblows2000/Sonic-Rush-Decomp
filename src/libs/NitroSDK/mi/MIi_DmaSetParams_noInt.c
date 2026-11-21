#include <nitro/types.h>

asm void MIi_DmaSetParams_noInt(void)
{
    stmdb sp!, {lr}
    sub sp, sp, #4
    mov r12, #0xc
    mul lr, r0, r12
    ldr r0, =0x40000b0
    add r12, lr, #0x4000000
    str r1, [r12, #0xb0]
    add r0, lr, r0
    str r2, [r0, #4]
    str r3, [r0, #8]
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
}


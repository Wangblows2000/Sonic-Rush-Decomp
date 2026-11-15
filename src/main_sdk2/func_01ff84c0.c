#include <nitro/types.h>

asm void func_01ff84c0(void)
{
    stmfd sp!, {r4, lr}
    mov r12, #0xc
    mul r4, r0, r12
    add r12, r4, #0x4000000
    ldr lr, =0x40000b0
    str r1, [r12, #0xb0]
    add r4, r4, lr
    str r2, [r4, #4]
    str r3, [r4, #8]
    ldr r1, [lr]
    cmp r0, #0
    ldr r0, [lr]
    moveq r1, #0
    streq r1, [r4]
    streq r1, [r4, #4]
    ldreq r0, =0x81400001
    ldr r1, =0x40000b0
    streq r0, [r4, #8]
    ldr r0, [r1]
    ldr r0, [r1]
    ldmfd sp!, {r4, lr}
    bx lr
}


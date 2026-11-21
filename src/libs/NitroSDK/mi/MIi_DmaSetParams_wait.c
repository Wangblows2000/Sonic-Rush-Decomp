#include <nitro/types.h>

void OS_DisableInterrupts(void);
void OS_RestoreInterrupts(void);

asm void MIi_DmaSetParams_wait(void)
{
    stmfd sp!, {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r7, r0
    mov r6, r1
    mov r5, r2
    mov r4, r3
    bl OS_DisableInterrupts
    mov r1, #0xc
    mul r3, r7, r1
    add r1, r3, #0x4000000
    ldr r2, =0x40000b0
    str r6, [r1, #0xb0]
    add r3, r3, r2
    str r5, [r3, #4]
    str r4, [r3, #8]
    ldr r1, [r2]
    cmp r7, #0
    ldr r1, [r2]
    moveq r2, #0
    streq r2, [r3]
    ldreq r1, =0x81400001
    streq r2, [r3, #4]
    streq r1, [r3, #8]
    bl OS_RestoreInterrupts
    add sp, sp, #4
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
}


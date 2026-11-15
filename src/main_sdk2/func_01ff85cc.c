#include <nitro/types.h>

void OS_DisableInterrupts(void);
void OS_RestoreInterrupts(void);

asm void func_01ff85cc(void)
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
    ldr r1, =0x40000b0
    add r2, r3, #0x4000000
    str r6, [r2, #0xb0]
    add r1, r3, r1
    str r5, [r1, #4]
    str r4, [r1, #8]
    bl OS_RestoreInterrupts
    add sp, sp, #4
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
}


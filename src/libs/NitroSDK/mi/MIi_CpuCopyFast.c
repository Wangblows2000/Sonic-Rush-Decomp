#include <nitro/types.h>

asm void MIi_CpuCopyFast(void)
{
    stmfd sp!, {r4, r5, r6, r7, r8, r9, r10}
    add r10, r1, r2
    mov r12, r2, lsr #5
    add r12, r1, r12, lsl #5
@1:
    cmp r1, r12
    ldmltia r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
    stmltia r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
    blt @1
@2:
    cmp r1, r10
    ldmltia r0!, {r2}
    stmltia r1!, {r2}
    blt @2
    ldmfd sp!, {r4, r5, r6, r7, r8, r9, r10}
    bx lr
}


#include <nitro/types.h>

asm void func_02000e20(void)
{
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    mov r2, #0
    str r2, [r1]
    str r2, [r1, #4]
    str r2, [r1, #8]
    bx lr
}


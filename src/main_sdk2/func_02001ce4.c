#include <nitro/types.h>

asm void func_02001ce4(void)
{
    ldmia r0!, {r2, r3, r12}
    add r0, r0, #4
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    add r0, r0, #4
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    add r0, r0, #4
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    add r0, r0, #4
    stmia r1!, {r2, r3, r12}
    bx lr
}


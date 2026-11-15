#include <nitro/types.h>

asm void func_020014b4(void)
{
    stmdb sp!, {r4}
    mov r12, #0
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, r12}
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, r12}
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, r12}
    mov r12, #0x1000
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, r12}
    ldmia sp!, {r4}
    bx lr
}


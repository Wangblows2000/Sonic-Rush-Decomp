#include <nitro/types.h>

asm void func_02000cb4(void)
{
    mov r1, #0
    mov r2, #0x1000
    mov r3, #0
    stmia r0!, {r2, r3}
    stmia r0!, {r1, r2}
    bx lr
}


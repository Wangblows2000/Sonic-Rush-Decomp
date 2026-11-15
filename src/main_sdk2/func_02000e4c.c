#include <nitro/types.h>

#pragma thumb on
asm void func_02000e4c(void)
{
    stmia r0!, {r1}
    mov r1, #0
    str r3, [r0, #0x1c]
    mov r3, #0
    stmia r0!, {r1, r3}
    stmia r0!, {r1, r2, r3}
    mov r2, #0
    stmia r0!, {r1, r3}
    bx lr
}
#pragma thumb reset


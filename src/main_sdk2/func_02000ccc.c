#include <nitro/types.h>

#pragma thumb on
asm void func_02000ccc(void)
{
    stmia r0!, {r1}
    mov r1, #0
    str r2, [r0, #8]
    mov r2, #0
    stmia r0!, {r1, r2}
    bx lr
}
#pragma thumb reset


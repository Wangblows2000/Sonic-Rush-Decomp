#include <nitro/types.h>

#pragma thumb on
asm void MTX_RotX43_(void)
{
    str r1, [r0, #0x14]
    neg r1, r1
    str r1, [r0, #0x1c]
    mov r1, #1
    lsl r1, r1, #0xc
    stmia r0!, {r1}
    mov r3, #0
    mov r1, #0
    stmia r0!, {r1, r3}
    stmia r0!, {r1, r2}
    str r1, [r0, #4]
    add r0, #0xc
    stmia r0!, {r2, r3}
    stmia r0!, {r1, r3}
    bx lr
}
#pragma thumb reset


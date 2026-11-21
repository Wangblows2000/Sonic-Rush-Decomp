#include <nitro/types.h>

#pragma thumb on
asm void MTX_RotX33_(void)
{
    mov r3, #1
    lsl r3, r3, #0xc
    str r3, [r0]
    mov r3, #0
    str r3, [r0, #4]
    str r3, [r0, #8]
    str r3, [r0, #0xc]
    str r2, [r0, #0x10]
    str r1, [r0, #0x14]
    str r3, [r0, #0x18]
    neg r1, r1
    str r1, [r0, #0x1c]
    str r2, [r0, #0x20]
    bx lr
}
#pragma thumb reset


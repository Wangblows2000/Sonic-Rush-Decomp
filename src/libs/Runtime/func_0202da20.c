#include <nitro/types.h>

asm void _dfltu(void)
{
    cmp r0, #0
    mov r1, #0
    bxeq lr
    mov r3, #0x400
    add r3, r3, #0x1e
    bmi _24
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
_24:
    mov r1, r0
    mov r0, r1, lsl #0x15
    add r1, r1, r1
    mov r1, r1, lsr #0xc
    orr r1, r1, r3, lsl #20
    bx lr
}

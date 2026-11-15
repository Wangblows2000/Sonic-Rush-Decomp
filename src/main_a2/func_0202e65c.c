#include <nitro/types.h>

asm void func_0202e65c(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    cmpcs r3, r1, lsl #1
    bcs _14
    bx lr
_14:
    orr r3, r0, r1
    movs r3, r3, lsl #1
    cmpeq r0, r0
    bxeq lr
    orrs r2, r0, r1
    bmi _34
    cmp r0, r1
    bx lr
_34:
    cmp r1, r0
    bx lr
}

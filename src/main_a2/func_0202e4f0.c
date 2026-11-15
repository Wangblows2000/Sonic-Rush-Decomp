#include <nitro/types.h>

asm void func_0202e4f0(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    bcs _14
    movs r2, #1
    bx lr
_14:
    cmp r3, r1, lsl #1
    bcs _24
    movs r2, #1
    bx lr
_24:
    orr r3, r0, r1
    movs r3, r3, lsl #1
    bxeq lr
    cmp r0, r1
    bx lr
}

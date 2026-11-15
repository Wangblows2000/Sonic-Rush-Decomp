#include <nitro/types.h>

asm void func_0202e478(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    movcc r0, #1
    bxcc lr
    cmp r3, r1, lsl #1
    movcc r0, #1
    bxcc lr
    orr r3, r0, r1
    movs r3, r3, lsl #1
    moveq r0, #0
    bxeq lr
    cmp r0, r1
    movne r0, #1
    moveq r0, #0
    bx lr
}

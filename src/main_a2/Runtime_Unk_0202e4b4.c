#include <nitro/types.h>

asm void Runtime_Unk_0202e4b4(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    movcc r0, #0
    bxcc lr
    cmp r3, r1, lsl #1
    movcc r0, #0
    bxcc lr
    orr r3, r0, r1
    movs r3, r3, lsl #1
    moveq r0, #1
    bxeq lr
    cmp r0, r1
    moveq r0, #1
    movne r0, #0
    bx lr
}

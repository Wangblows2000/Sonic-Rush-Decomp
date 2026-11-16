#include <nitro/types.h>

asm void _fcmple(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    cmpcs r3, r1, lsl #1
    bcs _1c
    mov r2, #0xff000000
    adds r2, r2, #0x10000000
    bx lr
_1c:
    orr r3, r0, r1
    movs r3, r3, lsl #1
    cmpeq r0, r0
    bxeq lr
    orrs r2, r0, r1
    bmi _3c
    cmp r0, r1
    bx lr
_3c:
    cmp r1, r0
    bx lr
}

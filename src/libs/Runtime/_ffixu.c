#include <nitro/types.h>

asm void _ffixu(void)
{
    tst r0, #0x80000000
    bne _24
    mov r1, #0x9e
    subs r1, r1, r0, lsr #23
    blt _38
    mov r2, r0, lsl #8
    orr r0, r2, #0x80000000
    mov r0, r0, lsr r1
    bx lr
_24:
    mov r2, #0xff000000
    cmp r2, r0, lsl #1
    movcs r0, #0
    mvncc r0, #0
    bx lr
_38:
    mvn r0, #0
    bx lr
}

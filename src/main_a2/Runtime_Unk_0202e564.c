#include <nitro/types.h>

asm void Runtime_Unk_0202e564(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    cmpcs r3, r1, lsl #1
    movcc r0, #0
    bxcc lr
    cmp r0, #0
    bicmi r0, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r1, #0
    bicmi r1, r1, #0x80000000
    rsbmi r1, r1, #0
    cmp r0, r1
    movle r0, #1
    movgt r0, #0
    bx lr
}

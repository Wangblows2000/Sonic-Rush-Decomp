#include <nitro/types.h>

asm void MI_CpuFill8(void)
{
    cmp r2, #0
    bxeq lr
    tst r0, #1
    beq _2c
    ldrh r12, [r0, #-1]
    and r12, r12, #0xff
    orr r3, r12, r1, lsl #8
    strh r3, [r0, #-1]
    add r0, r0, #1
    subs r2, r2, #1
    bxeq lr
_2c:
    cmp r2, #2
    blo _74
    orr r1, r1, r1, lsl #8
    tst r0, #2
    beq _4c
    strh r1, [r0], #2
    subs r2, r2, #2
    bxeq lr
_4c:
    orr r1, r1, r1, lsl #16
    bics r3, r2, #3
    beq _6c
    sub r2, r2, r3
    add r12, r3, r0
_60:
    str r1, [r0], #4
    cmp r0, r12
    blo _60
_6c:
    tst r2, #2
    strneh r1, [r0], #2
_74:
    tst r2, #1
    bxeq lr
    ldrh r3, [r0]
    and r3, r3, #0xff00
    and r1, r1, #0xff
    orr r1, r1, r3
    strh r1, [r0]
    bx lr
}

#include <nitro/types.h>

asm void MI_CpuCopy8(void)
{
    cmp r2, #0
    bxeq lr
    tst r1, #1
    beq _40
    ldrh r12, [r1, #-1]
    and r12, r12, #0xff
    tst r0, #1
    ldrneh r3, [r0, #-1]
    movne r3, r3, lsr #8
    ldreqh r3, [r0]
    orr r3, r12, r3, lsl #8
    strh r3, [r1, #-1]
    add r0, r0, #1
    add r1, r1, #1
    subs r2, r2, #1
    bxeq lr
_40:
    eor r12, r1, r0
    tst r12, #1
    beq _94
    bic r0, r0, #1
    ldrh r12, [r0], #2
    mov r3, r12, lsr #8
    subs r2, r2, #2
    blo _78
_60:
    ldrh r12, [r0], #2
    orr r12, r3, r12, lsl #8
    strh r12, [r1], #2
    mov r3, r12, lsr #0x10
    subs r2, r2, #2
    bhs _60
_78:
    tst r2, #1
    bxeq lr
    ldrh r12, [r1]
    and r12, r12, #0xff00
    orr r12, r12, r3
    strh r12, [r1]
    bx lr
_94:
    tst r12, #2
    beq _c0
    bics r3, r2, #1
    beq _10c
    sub r2, r2, r3
    add r12, r3, r1
_ac:
    ldrh r3, [r0], #2
    strh r3, [r1], #2
    cmp r1, r12
    blo _ac
    b _10c
_c0:
    cmp r2, #2
    blo _10c
    tst r1, #2
    beq _e0
    ldrh r3, [r0], #2
    strh r3, [r1], #2
    subs r2, r2, #2
    bxeq lr
_e0:
    bics r3, r2, #3
    beq _100
    sub r2, r2, r3
    add r12, r3, r1
_f0:
    ldr r3, [r0], #4
    str r3, [r1], #4
    cmp r1, r12
    blo _f0
_100:
    tst r2, #2
    ldrneh r3, [r0], #2
    strneh r3, [r1], #2
_10c:
    tst r2, #1
    bxeq lr
    ldrh r2, [r1]
    ldrh r0, [r0]
    and r2, r2, #0xff00
    and r0, r0, #0xff
    orr r0, r2, r0
    strh r0, [r1]
    bx lr
}

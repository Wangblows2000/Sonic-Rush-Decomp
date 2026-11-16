#include <nitro/types.h>

asm void func_0202e910(void)
{
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bcs _40
    cmn r12, r3, lsl #1
    bcs _64
_14:
    orrs r12, r3, r1
    bmi _28
    cmp r1, r3
    cmpeq r0, r2
    bx lr
_28:
    orr r12, r0, r12, lsl #1
    orrs r12, r12, r2
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    bx lr
_40:
    beq _4c
    movs r12, #1
    bx lr
_4c:
    cmp r0, #0
    bls _5c
    movs r12, #1
    bx lr
_5c:
    cmn r12, r3, lsl #1
    bcc _14
_64:
    beq _70
    movs r12, #1
    bx lr
_70:
    cmp r2, #0
    bls _14
    movs r12, #1
    bx lr
}

asm void func_0202e990(void)
{
    dcd 0xeaffffe3  // b func_0202e910+0x14 (mid-function target; raw encoding)
}

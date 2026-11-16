#include <nitro/types.h>

asm void func_0202eaa4(void)
{
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bcs _44
    cmn r12, r3, lsl #1
    bcs _70
_14:
    orrs r12, r3, r1
    bmi _28
    cmp r1, r3
    cmpeq r0, r2
    bx lr
_28:
    orr r12, r0, r12, lsl #1
    orrs r12, r12, r2
    cmpeq r1, r1
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    bx lr
_44:
    beq _54
    mov r12, #0xff000000
    adds r12, r12, #0x10000000
    bx lr
_54:
    cmp r0, #0
    bls _68
    mov r12, #0xff000000
    adds r12, r12, #0x10000000
    bx lr
_68:
    cmn r12, r3, lsl #1
    bcc _14
_70:
    beq _80
    mov r12, #0xff000000
    adds r12, r12, #0x10000000
    bx lr
_80:
    cmp r2, #0
    bls _14
    mov r12, #0xff000000
    adds r12, r12, #0x10000000
    bx lr
}

asm void func_0202eb38(void)
{
    dcd 0xeaffffde  // b func_0202eaa4+0x14 (mid-function target; raw encoding)
}

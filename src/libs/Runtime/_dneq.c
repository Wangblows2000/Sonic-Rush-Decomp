#include <nitro/types.h>

asm void _dneq(void)
{
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bcs _54
    cmn r12, r3, lsl #1
    bcs _70
_14:
    orrs r12, r3, r1
    bmi _30
    cmp r1, r3
    cmpeq r0, r2
    movne r0, #1
    moveq r0, #0
    bx lr
_30:
    orr r12, r0, r12, lsl #1
    orrs r12, r12, r2
    moveq r0, #0
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    movne r0, #1
    moveq r0, #0
    bx lr
_54:
    movne r0, #1
    bxne lr
    cmp r0, #0
    movhi r0, #1
    bxhi lr
    cmn r12, r3, lsl #1
    bcc _14
_70:
    movne r0, #1
    bxne lr
    cmp r2, #0
    movhi r0, #1
    bxhi lr
    b _14
}

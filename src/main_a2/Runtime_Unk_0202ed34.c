#include <nitro/types.h>

asm void Runtime_Unk_0202ed34(void)
{
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bcs _1c
    cmn r12, r3, lsl #1
    bcs _38
_14:
    mov r0, #1
    bx lr
_1c:
    movne r0, #0
    bxne lr
    cmp r0, #0
    movhi r0, #0
    bxhi lr
    cmn r12, r3, lsl #1
    bcc _14
_38:
    movne r0, #0
    bxne lr
    cmp r2, #0
    movhi r0, #0
    bxhi lr
    b _14
}

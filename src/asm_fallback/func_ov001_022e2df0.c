// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

#pragma thumb on
asm void func_ov001_022e2df0(void)
{
    cmp r0, #0xf
    bgt _38
    cmp r0, #0
    blt _32
    add r1, r0, r0
    add r1, pc
    ldrh r1, [r1, #4]
    add r1, pc
    bx r1
    lsl r1, r4, #0
    lsl r1, r4, #0
    lsl r1, r4, #0
    lsl r1, r4, #0
    lsl r1, r4, #0
    lsl r1, r4, #0
    lsl r5, r6, #0
    lsl r1, r7, #0
    lsl r1, r7, #0
    lsl r5, r6, #0
    lsl r1, r7, #0
    lsl r1, r7, #0
    lsl r5, r6, #0
    lsl r1, r7, #0
    lsl r1, r7, #0
    lsl r5, r6, #0
_32:
    cmp r0, #0
    beq _46
    b _4a
_38:
    cmp r0, #0x1c
    bgt _42
    cmp r0, #0x1c
    beq _46
    b _4a
_42:
    cmp r0, #0x1f
    bne _4a
_46:
    mov r0, #1
    bx lr
_4a:
    mov r0, #0
    bx lr
}
#pragma thumb reset

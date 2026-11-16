// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 gTalkDemoId;

#pragma thumb on
asm void func_ov001_022e1aac(void)
{
    cmp r0, #0x13
    bgt _40
    cmp r0, #0x13
    bge _7c
    cmp r0, #0
    bgt _12
    cmp r0, #0
    beq _54
    bx lr
_12:
    cmp r0, #0xf
    bhi _9a
    add r1, r0, r0
    add r1, pc
    ldrh r1, [r1, #4]
    add r1, pc
    bx r1
    lsl r3, r7, #1
    lsl r3, r7, #1
    lsl r3, r7, #1
    lsl r3, r7, #1
    lsl r3, r7, #1
    lsl r3, r7, #1
    lsl r5, r7, #0
    lsl r3, r7, #1
    lsl r3, r7, #1
    lsl r5, r0, #1
    lsl r3, r7, #1
    lsl r3, r7, #1
    lsl r5, r1, #1
    lsl r3, r7, #1
    lsl r3, r7, #1
    lsl r5, r2, #1
_40:
    cmp r0, #0x1c
    bgt _4e
    cmp r0, #0x1c
    bge _8c
    cmp r0, #0x18
    beq _84
    bx lr
_4e:
    cmp r0, #0x1f
    beq _94
    bx lr
_54:
    mov r1, #0x15
    ldr r0, [pc, #0x44]
    strh r1, [r0]
    bx lr
    mov r1, #0x16
    ldr r0, [pc, #0x3c]
    strh r1, [r0]
    bx lr
    mov r1, #0x17
    ldr r0, [pc, #0x34]
    strh r1, [r0]
    bx lr
    mov r1, #0x18
    ldr r0, [pc, #0x2c]
    strh r1, [r0]
    bx lr
    mov r1, #0x19
    ldr r0, [pc, #0x24]
    strh r1, [r0]
    bx lr
_7c:
    mov r1, #0x1a
    ldr r0, [pc, #0x1c]
    strh r1, [r0]
    bx lr
_84:
    mov r1, #0x1b
    ldr r0, [pc, #0x14]
    strh r1, [r0]
    bx lr
_8c:
    mov r1, #0x1c
    ldr r0, [pc, #0xc]
    strh r1, [r0]
    bx lr
_94:
    mov r1, #0x1d
    ldr r0, [pc, #0x4]
    strh r1, [r0]
_9a:
    bx lr
    dcd gTalkDemoId
}
#pragma thumb reset

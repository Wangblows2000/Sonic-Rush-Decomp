#include <nitro/types.h>

asm void MI_UncompressLZ16(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    mov r3, #0
    ldr r8, [r0], #4
    mov r10, r8, lsr #8
    mov r2, #0
_14:
    cmp r10, #0
    ble _d0
    ldrb r6, [r0], #1
    mov r7, #8
_24:
    subs r7, r7, #1
    blt _14
    tst r6, #0x80
    bne _50
    ldrb r9, [r0], #1
    orr r3, r3, r9, lsl r2
    sub r10, r10, #1
    eors r2, r2, #8
    streqh r3, [r1], #2
    moveq r3, #0
    b _c0
_50:
    ldrb r9, [r0]
    mov r8, #3
    add r5, r8, r9, asr #4
    ldrb r9, [r0], #1
    and r8, r9, #0xf
    mov r4, r8, lsl #8
    ldrb r9, [r0], #1
    orr r8, r9, r4
    add r4, r8, #1
    rsb r8, r2, #8
    and r9, r4, #1
    eor lr, r8, r9, lsl #3
    sub r10, r10, r5
_84:
    eor lr, lr, #8
    rsb r8, r2, #8
    add r8, r4, r8, lsr #3
    mov r8, r8, lsr #1
    mov r8, r8, lsl #1
    ldrh r9, [r1, -r8]
    mov r8, #0xff
    and r8, r9, r8, lsl lr
    mov r8, r8, asr lr
    orr r3, r3, r8, lsl r2
    eors r2, r2, #8
    streqh r3, [r1], #2
    moveq r3, #0
    subs r5, r5, #1
    bgt _84
_c0:
    cmp r10, #0
    movgt r6, r6, lsl #1
    bgt _24
    b _14
_d0:
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    bx lr
}

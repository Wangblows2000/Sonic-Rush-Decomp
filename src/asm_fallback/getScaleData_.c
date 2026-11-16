// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

asm void getScaleData_(void)
{
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    ldr r4, [r2, #4]
    ldr r12, [r2]
    mov r1, r1, asr #0xc
    add r3, r3, r4
    ands r2, r12, #0xc0000000
    beq _168
    ldr r2, [pc, #0x204]
    ands r4, r12, #0x40000000
    and r2, r12, r2
    mov r4, r2, lsr #0x10
    beq _5c
    ands r2, r1, #1
    beq _54
    cmp r1, r4
    movhi r1, r4, lsr #1
    addhi r1, r1, #1
    bhi _168
    mov r2, r1, lsr #1
    b _1b0
_54:
    mov r1, r1, lsr #1
    b _168
_5c:
    ands r2, r1, #3
    beq _164
    cmp r1, r4
    addhi r1, r2, r4, lsr #2
    bhi _168
    ands r2, r1, #1
    beq _15c
    ands r2, r1, #2
    movne r1, r1, lsr #2
    addne r2, r1, #1
    moveq r2, r1, lsr #2
    addeq r1, r2, #1
    ands r4, r12, #0x20000000
    beq _e0
    mov r5, r2, lsl #2
    mov r4, r1, lsl #2
    ldrsh r12, [r3, r5]
    ldrsh r5, [r3, r4]
    add r2, r3, r2, lsl #2
    add r4, r12, r12, lsl #1
    add r4, r5, r4
    mov r4, r4, asr #2
    str r4, [r0]
    add r1, r3, r1, lsl #2
    ldrsh r3, [r2, #2]
    ldrsh r2, [r1, #2]
    add sp, sp, #4
    add r1, r3, r3, lsl #1
    add r1, r2, r1
    mov r1, r1, asr #2
    str r1, [r0, #4]
    ldmia sp!, {r4, r5, lr}
    bx lr
_e0:
    ldr r5, [r3, r2, lsl #3]
    ldr r4, [r3, r1, lsl #3]
    mov r12, r5, asr #0x1f
    mov r12, r12, lsl #1
    mov lr, r5, lsl #1
    orr r12, r12, r5, lsr #31
    adds lr, lr, r5
    adc r12, r12, r5, asr #31
    adds lr, lr, r4
    adc r12, r12, r4, asr #31
    mov lr, lr, lsr #2
    orr lr, lr, r12, lsl #30
    str lr, [r0]
    add r2, r3, r2, lsl #3
    ldr r12, [r2, #4]
    add r1, r3, r1, lsl #3
    mov r2, r12, asr #0x1f
    mov r2, r2, lsl #1
    mov r3, r12, lsl #1
    ldr r1, [r1, #4]
    orr r2, r2, r12, lsr #31
    adds r3, r3, r12
    adc r2, r2, r12, asr #31
    adds r3, r3, r1
    adc r1, r2, r1, asr #31
    mov r2, r3, lsr #2
    orr r2, r2, r1, lsl #30
    str r2, [r0, #4]
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
_15c:
    mov r2, r1, lsr #2
    b _1b0
_164:
    mov r1, r1, lsr #2
_168:
    ands r2, r12, #0x20000000
    ldreq r2, [r3, r1, lsl #3]
    addeq r1, r3, r1, lsl #3
    streq r2, [r0]
    ldreq r1, [r1, #4]
    addeq sp, sp, #4
    streq r1, [r0, #4]
    ldmeqia sp!, {r4, r5, lr}
    bxeq lr
    mov r2, r1, lsl #2
    ldrsh r2, [r3, r2]
    add r1, r3, r1, lsl #2
    add sp, sp, #4
    str r2, [r0]
    ldrsh r1, [r1, #2]
    str r1, [r0, #4]
    ldmia sp!, {r4, r5, lr}
    bx lr
_1b0:
    ands r1, r12, #0x20000000
    beq _1f4
    mov r1, r2, lsl #2
    add r12, r3, r2, lsl #2
    ldrsh r2, [r3, r1]
    ldrsh r1, [r12, #4]
    add sp, sp, #4
    add r1, r2, r1
    mov r1, r1, asr #1
    str r1, [r0]
    ldrsh r2, [r12, #2]
    ldrsh r1, [r12, #6]
    add r1, r2, r1
    mov r1, r1, asr #1
    str r1, [r0, #4]
    ldmia sp!, {r4, r5, lr}
    bx lr
_1f4:
    add r12, r3, r2, lsl #3
    ldr r2, [r3, r2, lsl #3]
    ldr r1, [r12, #8]
    add r1, r2, r1
    mov r1, r1, asr #1
    str r1, [r0]
    ldr r2, [r12, #4]
    ldr r1, [r12, #0xc]
    add r1, r2, r1
    mov r1, r1, asr #1
    str r1, [r0, #4]
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
    dcd 0x1fff0000
}

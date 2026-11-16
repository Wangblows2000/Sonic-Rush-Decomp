#include <nitro/types.h>

extern void _s32_div_f(void);

asm void _ll_sdiv(void)
{
    stmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    eor r4, r1, r3
    mov r4, r4, asr #1
    mov r4, r4, lsl #1
    orrs r5, r3, r2
    bne _20
    ldmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
_20:
    mov r5, r0, lsr #0x1f
    add r5, r5, r1
    mov r6, r2, lsr #0x1f
    add r6, r6, r3
    orrs r6, r5, r6
    bne _54
    mov r1, r2
    bl _s32_div_f
    ands r4, r4, #1
    movne r0, r1
    mov r1, r0, asr #0x1f
    ldmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
_54:
    cmp r1, #0
    bge _64
    rsbs r0, r0, #0
    rsc r1, r1, #0
_64:
    cmp r3, #0
    bge _74
    rsbs r2, r2, #0
    rsc r3, r3, #0
_74:
    orrs r5, r1, r0
    beq _198
    mov r5, #0
    mov r6, #1
    cmp r3, #0
    bmi _a0
_8c:
    add r5, r5, #1
    adds r2, r2, r2
    adcs r3, r3, r3
    bpl _8c
    add r6, r6, r5
_a0:
    cmp r1, #0
    blt _c0
_a8:
    cmp r6, #1
    beq _c0
    sub r6, r6, #1
    adds r0, r0, r0
    adcs r1, r1, r1
    bpl _a8
_c0:
    mov r7, #0
    mov r12, #0
    mov r11, #0
    b _e8
_d0:
    orr r12, r12, #1
    subs r6, r6, #1
    beq _140
    adds r0, r0, r0
    adcs r1, r1, r1
    adcs r7, r7, r7
_e8:
    subs r0, r0, r2
    sbcs r1, r1, r3
    sbcs r7, r7, #0
    adds r12, r12, r12
    adc r11, r11, r11
    cmp r7, #0
    bge _d0
_104:
    subs r6, r6, #1
    beq _138
    adds r0, r0, r0
    adcs r1, r1, r1
    adc r7, r7, r7
    adds r0, r0, r2
    adcs r1, r1, r3
    adc r7, r7, #0
    adds r12, r12, r12
    adc r11, r11, r11
    cmp r7, #0
    bge _d0
    b _104
_138:
    adds r0, r0, r2
    adc r1, r1, r3
_140:
    ands r7, r4, #1
    moveq r0, r12
    moveq r1, r11
    beq _178
    subs r7, r5, #0x20
    movge r0, r1, lsr r7
    bge _19c
    rsb r7, r5, #0x20
    mov r0, r0, lsr r5
    orr r0, r0, r1, lsl r7
    mov r1, r1, lsr r5
    b _178
    mov r0, r1, lsr r7
    mov r1, #0
_178:
    cmp r4, #0
    blt _188
    ldmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
_188:
    rsbs r0, r0, #0
    rsc r1, r1, #0
    ldmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
_198:
    mov r0, #0
_19c:
    mov r1, #0
    cmp r4, #0
    blt _188
    ldmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
}

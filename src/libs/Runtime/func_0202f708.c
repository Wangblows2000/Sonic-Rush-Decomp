// asm: hand-written runtime library routine (split at mid-function entry points, see w_asm/split_plan.txt)
#include <nitro/types.h>

extern void _ll_sdiv_e74(void);
extern void _s32_div_f(void);
void _ll_sdiv(void);
void _ll_sdiv_e10(void);

asm void _ll_sdiv(void)
{
    stmdb sp!, {r4, r5, r6, r7, r11, r12, lr}
    eor r4, r1, r3
    mov r4, r4, asr #1
    mov r4, r4, lsl #1
}

asm void _ll_sdiv_e10(void)
{
    orrs r5, r3, r2
    bne _10
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
_10:
    mov r5, r0, lsr #0x1f
    add r5, r5, r1
    mov r6, r2, lsr #0x1f
    add r6, r6, r3
    orrs r6, r5, r6
    bne _44
    mov r1, r2
    bl _s32_div_f
    ands r4, r4, #1
    movne r0, r1
    mov r1, r0, asr #0x1f
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
_44:
    cmp r1, #0
    bge _54
    rsbs r0, r0, #0
    rsc r1, r1, #0
_54:
    cmp r3, #0
    bge _ll_sdiv_e74
    rsbs r2, r2, #0
    rsc r3, r3, #0
}

asm void _ll_sdiv_e74(void)
{
    orrs r5, r1, r0
    beq _124
    mov r5, #0
    mov r6, #1
    cmp r3, #0
    bmi _2c
_18:
    add r5, r5, #1
    adds r2, r2, r2
    adcs r3, r3, r3
    bpl _18
    add r6, r6, r5
_2c:
    cmp r1, #0
    blt _4c
_34:
    cmp r6, #1
    beq _4c
    sub r6, r6, #1
    adds r0, r0, r0
    adcs r1, r1, r1
    bpl _34
_4c:
    mov r7, #0
    mov r12, #0
    mov r11, #0
    b _74
_5c:
    orr r12, r12, #1
    subs r6, r6, #1
    beq _cc
    adds r0, r0, r0
    adcs r1, r1, r1
    adcs r7, r7, r7
_74:
    subs r0, r0, r2
    sbcs r1, r1, r3
    sbcs r7, r7, #0
    adds r12, r12, r12
    adc r11, r11, r11
    cmp r7, #0
    bge _5c
_90:
    subs r6, r6, #1
    beq _c4
    adds r0, r0, r0
    adcs r1, r1, r1
    adc r7, r7, r7
    adds r0, r0, r2
    adcs r1, r1, r3
    adc r7, r7, #0
    adds r12, r12, r12
    adc r11, r11, r11
    cmp r7, #0
    bge _5c
    b _90
_c4:
    adds r0, r0, r2
    adc r1, r1, r3
_cc:
    ands r7, r4, #1
    moveq r0, r12
    moveq r1, r11
    beq _104
    subs r7, r5, #0x20
    movge r0, r1, lsr r7
    bge _128
    rsb r7, r5, #0x20
    mov r0, r0, lsr r5
    orr r0, r0, r1, lsl r7
    mov r1, r1, lsr r5
    b _104
    mov r0, r1, lsr r7
    mov r1, #0
_104:
    cmp r4, #0
    blt _114
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
_114:
    rsbs r0, r0, #0
    rsc r1, r1, #0
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
_124:
    mov r0, #0
_128:
    mov r1, #0
    cmp r4, #0
    blt _114
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
}

// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 data_0206e470;
extern void FX_Sqrt(void);

asm void ExtraBoss_Unk_ov001_022fde54(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x124
    str r1, [sp, #4]
    str r0, [sp]
    ldr r2, [r0, #0x20]
    ldr r1, [r0, #0x14]
    ldr r0, [sp, #4]
    sub r6, r1, r2
    ldr r4, [r0, #0x20]
    ldr r3, [r0, #0x14]
    ldr r0, [sp]
    add r5, r1, r2
    ldr r7, [r0, #0x30]
    ldr r0, [sp, #4]
    sub r1, r3, r4
    ldr r2, [r0, #0x30]
    add r0, r3, r4
    add r3, r2, r1
    add r4, r7, r6
    cmp r4, r3
    add r1, r7, r5
    add r0, r2, r0
    bge _70
    cmp r1, r3
    addlt sp, sp, #0x124
    movlt r0, #0
    ldmltia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxlt lr
_70:
    cmp r3, r4
    bge _8c
    cmp r0, r4
    addlt sp, sp, #0x124
    movlt r0, #0
    ldmltia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxlt lr
_8c:
    ldr r0, [sp, #4]
    ldr r3, [r0, #0x1c]
    ldr r0, [r0, #0x18]
    smull r2, r1, r3, r3
    smull r4, r3, r0, r0
    mov r0, #0x800
    adds r4, r4, r0
    adc r3, r3, #0
    adds r2, r2, r0
    mov r4, r4, lsr #0xc
    adc r0, r1, #0
    mov r1, r2, lsr #0xc
    orr r4, r4, r3, lsl #20
    orr r1, r1, r0, lsl #20
    add r0, r4, r1
    bl FX_Sqrt
    ldr r1, [sp]
    ldr r4, [r1, #0x1c]
    ldr r1, [r1, #0x18]
    smull r3, r2, r4, r4
    smull r5, r4, r1, r1
    mov r1, #0x800
    adds r5, r5, r1
    adc r4, r4, #0
    adds r3, r3, r1
    mov r5, r5, lsr #0xc
    adc r1, r2, #0
    mov r2, r3, lsr #0xc
    orr r5, r5, r4, lsl #20
    orr r2, r2, r1, lsl #20
    mov r4, r0
    add r0, r5, r2
    bl FX_Sqrt
    ldr r1, [sp]
    add r4, r0, r4
    ldr r5, [r1, #0x28]
    ldr r3, [r1, #0xc]
    ldr r1, [sp, #4]
    add r3, r5, r3
    ldr r2, [r1, #0x28]
    ldr r1, [r1, #0xc]
    add r1, r2, r1
    subs r0, r3, r1
    rsbmi r0, r0, #0
    cmp r4, r0
    blt _174
    ldr r0, [sp]
    ldr r3, [r0, #0x2c]
    ldr r2, [r0, #0x10]
    ldr r0, [sp, #4]
    add r2, r3, r2
    ldr r1, [r0, #0x2c]
    ldr r0, [r0, #0x10]
    add r0, r1, r0
    subs r0, r2, r0
    rsbmi r0, r0, #0
    cmp r4, r0
    bge _184
_174:
    add sp, sp, #0x124
    mov r0, #0
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
_184:
    ldr r0, [sp]
    ldr lr, [pc, #0x564]
    ldrh r2, [r0, #0x24]
    ldrh r1, [r0, #0x34]
    mov r0, #0
    add r7, sp, #0x60
    add r1, r2, r1
    strh r1, [sp, #0x5c]
    ldr r1, [sp, #4]
    ldrh r2, [r1, #0x24]
    ldrh r1, [r1, #0x34]
    add r1, r2, r1
    strh r1, [sp, #0x5e]
    ldr r1, [sp]
    str r1, [sp, #0x54]
    ldr r1, [sp, #4]
    str r1, [sp, #0x58]
_1c8:
    mov r2, r0, lsl #1
    add r1, sp, #0x5c
    ldrh r3, [r1, r2]
    ldr r2, [pc, #0x51c]
    add r1, sp, #0x54
    and r2, r3, r2
    mov r2, r2, asr #4
    mov r3, r2, lsl #1
    mov r2, r3, lsl #1
    ldrsh r4, [lr, r2]
    add r2, r3, #1
    mov r3, r2, lsl #1
    add r2, sp, #0x24
    str r4, [r2, r0, lsl #2]
    ldrsh r4, [lr, r3]
    ldr r11, [r1, r0, lsl #2]
    ldr r5, [r2, r0, lsl #2]
    add r2, sp, #0x2c
    str r4, [r2, r0, lsl #2]
    ldr r6, [r2, r0, lsl #2]
    ldr r2, [r11, #0x18]
    mov r1, #0x800
    smull r8, r4, r2, r6
    adds r8, r8, r1
    adc r4, r4, #0
    mov r8, r8, lsr #0xc
    orr r8, r8, r4, lsl #20
    add r4, sp, #0x3c
    str r8, [r4, r0, lsl #2]
    smull r8, r4, r2, r5
    adds r8, r8, r1
    adc r2, r4, #0
    mov r4, r8, lsr #0xc
    orr r4, r4, r2, lsl #20
    add r2, sp, #0x34
    str r4, [r2, r0, lsl #2]
    ldr r4, [r11, #0x1c]
    add r2, sp, #0x3c
    smull r9, r8, r4, r6
    adds r9, r9, r1
    smull r6, r5, r4, r5
    adc r8, r8, #0
    adds r4, r6, r1
    mov r6, r9, lsr #0xc
    orr r6, r6, r8, lsl #20
    add r1, sp, #0x4c
    str r6, [r1, r0, lsl #2]
    adc r1, r5, #0
    mov r4, r4, lsr #0xc
    orr r4, r4, r1, lsl #20
    add r1, sp, #0x44
    str r4, [r1, r0, lsl #2]
    ldr r10, [r11, #0xc]
    ldr r1, [r11, #0x28]
    ldr r2, [r2, r0, lsl #2]
    add r4, r10, r1
    add r1, sp, #0x44
    ldr r12, [r1, r0, lsl #2]
    sub r1, r4, r2
    add r4, r4, r2
    ldr r9, [r11, #0x10]
    ldr r5, [r11, #0x2c]
    add r2, r1, r12
    str r2, [r7, r0, lsl #5]
    sub r2, r1, r12
    add r1, sp, #0x4c
    ldr r6, [r1, r0, lsl #2]
    add r1, sp, #0x34
    add r8, r9, r5
    ldr r5, [r1, r0, lsl #2]
    sub r1, r4, r12
    add r12, r4, r12
    sub r4, r8, r5
    add r5, r8, r5
    add r3, r7, r0, lsl #5
    sub r8, r4, r6
    str r8, [r3, #4]
    str r2, [r3, #8]
    add r2, r4, r6
    str r2, [r3, #0xc]
    str r1, [r3, #0x10]
    add r2, r5, r6
    str r2, [r3, #0x14]
    sub r1, r5, r6
    str r12, [r3, #0x18]
    str r1, [r3, #0x1c]
    ldrh r1, [r11, #0x34]
    cmp r1, #0
    beq _434
    cmp r10, #0
    bne _33c
    cmp r9, #0
    beq _434
_33c:
    ldrh r2, [r11, #0x34]
    ldr r1, [pc, #0x3b0]
    ldr r5, [r11, #0xc]
    and r1, r2, r1
    mov r1, r1, asr #4
    mov r2, r1, lsl #1
    add r1, r2, #1
    mov r1, r1, lsl #1
    ldrsh r1, [lr, r1]
    mov r2, r2, lsl #1
    ldrsh r4, [lr, r2]
    smull r10, r2, r5, r1
    mov r6, #0x800
    ldr r3, [r11, #0x10]
    adds r11, r10, r6
    adc r10, r2, #0
    mov r2, r11, lsr #0xc
    orr r2, r2, r10, lsl #20
    smull r11, r10, r3, r1
    smull r1, r12, r3, r4
    adds r1, r1, r6
    adc r12, r12, #0
    mov r1, r1, lsr #0xc
    orr r1, r1, r12, lsl #20
    sub r1, r2, r1
    smull r2, r4, r5, r4
    sub r1, r1, r5
    adds r2, r2, r6
    adc r5, r4, #0
    adds r4, r11, r6
    mov r6, r2, lsr #0xc
    adc r2, r10, #0
    mov r4, r4, lsr #0xc
    ldr r8, [r7, r0, lsl #5]
    orr r6, r6, r5, lsl #20
    orr r4, r4, r2, lsl #20
    add r2, r6, r4
    sub r2, r2, r3
    add r3, r8, r1
    add r9, r7, r0, lsl #5
    str r3, [r7, r0, lsl #5]
    ldr r3, [r9, #4]
    add r3, r3, r2
    str r3, [r9, #4]
    ldr r3, [r9, #8]
    add r3, r3, r1
    str r3, [r9, #8]
    ldr r3, [r9, #0xc]
    add r3, r3, r2
    str r3, [r9, #0xc]
    ldr r3, [r9, #0x10]
    add r3, r3, r1
    str r3, [r9, #0x10]
    ldr r3, [r9, #0x14]
    add r3, r3, r2
    str r3, [r9, #0x14]
    ldr r3, [r9, #0x18]
    add r1, r3, r1
    str r1, [r9, #0x18]
    ldr r1, [r9, #0x1c]
    add r1, r1, r2
    str r1, [r9, #0x1c]
_434:
    ldr r3, [r7, r0, lsl #5]
    add r2, r7, r0, lsl #5
    add r1, sp, #0xa0
    str r3, [r1, r0, lsl #6]
    add r4, sp, #0xa0
    add r6, r4, r0, lsl #6
    ldr r1, [r2, #4]
    ldr r5, [r2, #8]
    str r1, [r6, #4]
    ldr r4, [r2, #0xc]
    str r5, [r6, #8]
    str r4, [r6, #0xc]
    str r5, [r6, #0x10]
    str r4, [r6, #0x14]
    ldr r5, [r2, #0x10]
    ldr r4, [r2, #0x14]
    str r5, [r6, #0x18]
    str r4, [r6, #0x1c]
    str r5, [r6, #0x20]
    str r4, [r6, #0x24]
    ldr r4, [r2, #0x18]
    add r0, r0, #1
    ldr r2, [r2, #0x1c]
    str r4, [r6, #0x28]
    str r2, [r6, #0x2c]
    str r4, [r6, #0x30]
    str r2, [r6, #0x34]
    str r3, [r6, #0x38]
    str r1, [r6, #0x3c]
    cmp r0, #2
    blt _1c8
    mov r0, #0
    str r0, [sp, #0x10]
    str r0, [sp, #0x14]
_4bc:
    ldr r0, [sp, #0x10]
    add r1, sp, #0xa0
    add r3, r1, r0, lsl #4
    ldr r0, [r1, r0, lsl #4]
    ldr r2, [r3, #8]
    str r0, [sp, #8]
    ldr r0, [r3, #4]
    ldr r1, [r3, #0xc]
    str r0, [sp, #0xc]
    ldr r0, [sp, #8]
    ldr r10, [sp, #0x14]
    sub r8, r2, r0
    ldr r0, [sp, #0xc]
    sub r7, r1, r0
_4f4:
    add r0, sp, #0xa0
    add r1, r0, r10, lsl #4
    ldr r2, [r1, #0x40]
    ldr r0, [r1, #0x48]
    ldr r11, [r1, #0x44]
    ldr r1, [r1, #0x4c]
    sub r6, r2, r0
    sub r5, r11, r1
    smull r9, r3, r8, r5
    mov r0, r6, asr #0x1f
    str r0, [sp, #0x18]
    mov r0, #0x800
    adds r9, r9, r0
    adc r3, r3, #0
    mov r9, r9, lsr #0xc
    orr r9, r9, r3, lsl #20
    smull r3, r12, r7, r6
    adds r3, r3, r0
    adc r12, r12, #0
    mov r3, r3, lsr #0xc
    orr r3, r3, r12, lsl #20
    mov r4, r5, asr #0x1f
    mov r1, #0
    subs r9, r9, r3
    beq _628
    ldr r3, [sp, #0xc]
    sub lr, r11, r3
    ldr r3, [sp, #8]
    sub r12, r2, r3
    mov r2, lr, asr #0x1f
    str r2, [sp, #0x1c]
    umull r11, r2, r12, r5
    mla r2, r12, r4, r2
    mov r3, r12, asr #0x1f
    mla r2, r3, r5, r2
    adds r3, r11, r0
    ldr r11, [sp, #0x18]
    adc r2, r2, r1
    mov r5, r3, lsr #0xc
    orr r5, r5, r2, lsl #20
    smull r2, r3, r12, r7
    str r3, [sp, #0x20]
    umull r4, r3, lr, r6
    mla r3, lr, r11, r3
    ldr r11, [sp, #0x1c]
    adds r4, r4, r0
    mla r3, r11, r6, r3
    adc r3, r3, r1
    mov r4, r4, lsr #0xc
    orr r4, r4, r3, lsl #20
    sub r4, r5, r4
    smull r3, r5, lr, r8
    adds r3, r3, r0
    adc r5, r5, r1
    adds r2, r2, r0
    ldr r0, [sp, #0x20]
    mov r3, r3, lsr #0xc
    adc r0, r0, r1
    cmp r9, #0
    mov r1, r2, lsr #0xc
    orr r3, r3, r5, lsl #20
    orr r1, r1, r0, lsl #20
    sub r0, r3, r1
    rsblt r4, r4, #0
    rsblt r9, r9, #0
    rsblt r0, r0, #0
    cmp r4, #0
    blt _628
    cmp r9, r4
    blt _628
    cmp r0, #0
    blt _628
    cmp r9, r0
    addge sp, sp, #0x124
    movge r0, #1
    ldmgeia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxge lr
_628:
    add r10, r10, #1
    cmp r10, #4
    blt _4f4
    ldr r0, [sp, #0x10]
    add r0, r0, #1
    str r0, [sp, #0x10]
    cmp r0, #4
    blt _4bc
    ldr r0, [sp, #4]
    mov r3, #0
    str r0, [sp, #0x54]
    ldr r0, [sp]
    mov r1, r3
    str r0, [sp, #0x58]
    add r0, sp, #0x60
_664:
    ldr r6, [sp, #0x58]
    mov r2, r1
    ldr r4, [r6, #0x18]
    rsb r5, r4, #0
    add r10, r0, r3, lsl #5
_678:
    ldr r7, [r10, r2, lsl #3]
    mov r8, r2, lsl #3
    cmp r5, r7
    bgt _6bc
    cmp r7, r4
    bgt _6bc
    add r7, r8, r10
    ldr r8, [r6, #0x1c]
    ldr r9, [r7, #4]
    rsb r7, r8, #0
    cmp r7, r9
    bgt _6bc
    cmp r9, r8
    addle sp, sp, #0x124
    movle r0, #1
    ldmleia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxle lr
_6bc:
    add r2, r2, #1
    cmp r2, #4
    blt _678
    ldr r2, [sp]
    add r3, r3, #1
    str r2, [sp, #0x54]
    ldr r2, [sp, #4]
    cmp r3, #2
    str r2, [sp, #0x58]
    blt _664
    mov r0, #0
    add sp, sp, #0x124
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd data_0206e470
    dcd 0x0000ffff
}

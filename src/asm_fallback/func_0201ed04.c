// asm: fallback — C attempts did not match (best C draft: /home/claude/w_main_d2/ef_113.c)
#include <nitro/types.h>

extern void DC_FlushRange(void);
extern void MI_CpuCopy8(void);
extern void MI_CpuFill8(void);
extern void _u32_div_f(void);
extern u32 data_02072a3c;
extern u32 data_02072a4c;
extern u32 data_02083f4c;
extern u32 data_02083f5c;
extern void func_02006628(void);
extern void func_02006698(void);
extern void func_0200b930(void);
extern void func_0200b99c(void);
extern void func_0201f5b8(void);

asm void func_0201ed04(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x7c
    str r0, [sp, #8]
    ldr r0, [r0, #8]
    str r0, [sp, #0x78]
    ldr r0, [r0, #0x10c]
    mov r0, r0, lsl #0x1a
    movs r0, r0, asr #0x1f
    beq _44
    ldr r0, [sp, #0x78]
    ldr r0, [r0, #0x110]
    cmp r0, #0
    ldrgt r0, [sp, #0x78]
    ldrgt r0, [r0, #0x110]
    subgt r1, r0, #1
    ldrgt r0, [sp, #0x78]
    strgt r1, [r0, #0x110]
_44:
    ldr r0, [sp, #8]
    ldr r0, [r0, #0x2c]
    str r0, [sp, #0x14]
    mov r0, #0
    str r0, [sp, #0x10]
    ldr r0, [sp, #0x14]
    cmp r0, #0
    beq _7ec
    ldr r0, [sp, #0x10]
    mov r1, #0x8000
    str r0, [sp, #0x60]
    str r0, [sp, #0x6c]
    str r0, [sp, #0x4c]
    str r0, [sp, #0x54]
    str r0, [sp, #0x70]
    str r0, [sp, #0x74]
    str r0, [sp, #0x3c]
    mov r0, #1
    str r0, [sp, #0x44]
    ldr r0, [sp, #0x10]
    ldr r7, [pc, #0x804]
    rsb r8, r1, #0
    mov r6, #0x58
    str r0, [sp, #0x40]
    str r0, [sp, #0x50]
    str r0, [sp, #0x48]
_ac:
    ldr r0, [sp, #0x78]
    ldr r0, [r0, #0x10c]
    mov r0, r0, lsl #0x1a
    movs r0, r0, asr #0x1f
    beq _110
    ldr r0, [sp, #8]
    mov r4, #0
    ldr r0, [r0, #0x10]
    cmp r0, #0
    ble _7ec
    mov r5, r4
_d8:
    ldr r0, [sp, #8]
    ldr r2, [sp, #0x14]
    add r0, r0, r4, lsl #2
    ldr r3, [r0, #0x14]
    ldr r0, [sp, #0x10]
    mov r1, r5
    add r0, r3, r0
    bl MI_CpuFill8
    ldr r0, [sp, #8]
    add r4, r4, #1
    ldr r0, [r0, #0x10]
    cmp r4, r0
    blt _d8
    b _7ec
_110:
    ldr r0, [sp, #0x78]
    ldr r5, [r0, #0xd8]
    ldr r4, [r0, #0x158]
    mov r1, r5
    mov r0, r4
    bl _u32_div_f
    ldr r1, [sp, #0x78]
    mul r2, r0, r5
    ldr r1, [r1, #0xd0]
    sub r2, r4, r2
    sub r1, r1, #1
    cmp r0, r1
    ldrlo r1, [sp, #0x78]
    str r2, [sp, #0x1c]
    ldrlo r1, [r1, #0xd4]
    ldr r2, [sp, #0x14]
    strlo r1, [sp, #0x18]
    movlo r1, r5
    ldrhs r1, [sp, #0x78]
    str r2, [sp, #0x24]
    ldr r2, [sp, #0x78]
    ldrhs r1, [r1, #0xdc]
    ldrb r2, [r2, #0xbc]
    strhs r1, [sp, #0x18]
    ldrhs r1, [sp, #0x78]
    ldrhs r1, [r1, #0xe0]
    cmp r2, #0
    ldrne r2, [sp, #0x14]
    movne r2, r2, lsr #1
    strne r2, [sp, #0x24]
    ldr r2, [sp, #0x78]
    ldr r3, [r2, #0x10c]
    mov r2, r3, lsl #0x1b
    movs r2, r2, asr #0x1f
    beq _1bc
    ldr r2, [sp, #0x1c]
    cmp r2, #0
    ldreq r2, [sp, #0x78]
    biceq r3, r3, #0x10
    streq r3, [r2, #0x10c]
    strne r2, [sp, #0x24]
    ldrne r2, [sp, #0x3c]
    strne r2, [sp, #0x1c]
_1bc:
    ldr r2, [sp, #0x40]
    ldr r3, [sp, #0x1c]
    str r2, [sp, #0xc]
    ldr r2, [sp, #0x24]
    add r2, r3, r2
    cmp r2, r1
    blo _220
    mov r2, r3
    sub r1, r1, r2
    str r1, [sp, #0x24]
    ldr r1, [sp, #0x78]
    ldr r1, [r1, #0xd0]
    sub r1, r1, #1
    cmp r0, r1
    blo _220
    ldr r1, [sp, #0x78]
    ldrb r1, [r1, #0xbd]
    cmp r1, #0
    ldrne r1, [sp, #0x44]
    strne r1, [sp, #0xc]
    ldreq r1, [sp, #0x78]
    ldreq r1, [r1, #0x10c]
    orreq r2, r1, #0x20
    ldreq r1, [sp, #0x78]
    streq r2, [r1, #0x10c]
_220:
    ldr r2, [sp, #0x24]
    ldr r1, [sp, #0x1c]
    str r2, [sp, #0x28]
    ldr r2, [sp, #0x78]
    ldrb r2, [r2, #0xbc]
    cmp r2, #0
    beq _250
    cmp r2, #1
    beq _25c
    cmp r2, #2
    beq _274
    b _2b4
_250:
    ldr r2, [sp, #0x24]
    str r2, [sp, #0x2c]
    b _2b4
_25c:
    ldr r2, [sp, #0x24]
    mov r1, r1, lsl #1
    mov r2, r2, lsl #1
    str r2, [sp, #0x28]
    str r2, [sp, #0x2c]
    b _2b4
_274:
    ldr r3, [sp, #0x1c]
    ldr r2, [sp, #0x24]
    cmp r1, #0
    add r2, r3, r2
    add r2, r2, #1
    mov r3, r2, lsr #1
    ldr r2, [sp, #0x1c]
    mov r1, r1, lsr #1
    sub r2, r3, r2, lsr #1
    str r2, [sp, #0x2c]
    addeq r2, r2, #4
    streq r2, [sp, #0x2c]
    ldr r2, [sp, #0x28]
    addne r1, r1, #4
    mov r2, r2, lsl #1
    str r2, [sp, #0x28]
_2b4:
    ldr r2, [sp, #0x78]
    ldr r3, [r2, #0xd4]
    ldrb r4, [r2, #0xbe]
    mul r3, r0, r3
    mla r0, r4, r3, r1
    ldr r2, [r2, #0xcc]
    add r2, r0, r2
    ldr r0, [sp, #0x78]
    ldr r1, [r0, #0xa0]
    ldr r0, [sp, #0x48]
    str r0, [sp, #0x38]
    add r0, r2, r1
    str r0, [sp, #0x20]
    ldr r0, [sp, #8]
    ldr r0, [r0, #0x10]
    cmp r0, #0
    ble _744
    ldr r0, [sp, #0x78]
    ldr r1, [sp, #0x1c]
    add r4, r0, #0xf4
    ldr r0, [sp, #0x24]
    add r0, r1, r0
    str r0, [sp, #0x30]
    mov r0, r1
    and r0, r0, #1
    str r0, [sp, #0x34]
_31c:
    ldr r1, [sp, #8]
    ldr r0, [sp, #0x38]
    add r0, r1, r0, lsl #2
    ldr r2, [r0, #0x14]
    ldr r0, [sp, #0x78]
    ldrb r1, [r0, #0xbe]
    ldr r0, [sp, #0x10]
    add r9, r2, r0
    ldr r0, [sp, #0x38]
    mov r5, r9
    cmp r0, r1
    bge _6dc
    ldr r0, [sp, #0x78]
    ldrb r0, [r0, #0xbc]
    cmp r0, #2
    bne _36c
    ldr r0, [pc, #0x540]
    bl func_02006698
    ldr r0, [pc, #0x53c]
    ldr r9, [r0]
_36c:
    ldr r3, [sp, #0x38]
    ldr r2, [sp, #0x18]
    ldr r1, [sp, #0x20]
    ldr r0, [sp, #0x78]
    mla r1, r3, r2, r1
    ldr r2, [sp, #0x4c]
    add r0, r0, #0x5c
    bl func_0200b930
    ldr r0, [sp, #0x78]
    ldr r2, [sp, #0x2c]
    mov r1, r9
    add r0, r0, #0x5c
    bl func_0200b99c
    ldr r1, [sp, #0x2c]
    cmp r0, r1
    beq _3e8
    ldr r0, [sp, #0x50]
    str r0, [sp, #0x28]
    str r0, [sp, #0x24]
    str r0, [sp, #0xc]
    ldr r0, [sp, #0x78]
    ldr r0, [r0, #0x10c]
    orr r1, r0, #0x20
    ldr r0, [sp, #0x78]
    str r1, [r0, #0x10c]
    ldrb r0, [r0, #0xbc]
    cmp r0, #2
    bne _744
    ldr r0, [pc, #0x4c0]
    bl func_02006628
    b _744
_3e8:
    ldr r0, [sp, #0x78]
    ldrb r0, [r0, #0xbc]
    cmp r0, #2
    bne _720
    ldr r0, [sp, #0x1c]
    cmp r0, #0
    ldr r0, [pc, #0x4a0]
    ldr r0, [r0]
    mov r1, r0
    addeq r1, r0, #4
    ldreqh r2, [r0]
    ldreqh r0, [r0, #2]
    streqh r2, [r4]
    streqh r0, [r4, #2]
    ldr r0, [sp, #0x34]
    cmp r0, #0
    ldr r0, [sp, #0x1c]
    beq _4d0
    ldrb r2, [r1]
    ldrb r3, [r4, #2]
    ldrsh r9, [r4]
    mov r2, r2, asr #4
    and r11, r2, #0xf
    ands r2, r11, #4
    ldr r2, [pc, #0x45c]
    mov r10, r3, lsl #1
    ldrsh r10, [r2, r10]
    mov r2, r10, asr #3
    addne r2, r2, r10
    ands r12, r11, #2
    addne r2, r2, r10, asr #1
    ands r12, r11, #1
    addne r2, r2, r10, asr #2
    ands r10, r11, #8
    beq _48c
    sub r9, r9, r2
    mov r2, #0x8000
    rsb r2, r2, #0
    cmp r9, r2
    movlt r9, r8
    b _498
_48c:
    add r9, r9, r2
    cmp r9, r7
    movgt r9, r7
_498:
    ldr r2, [pc, #0x410]
    ldrsb r2, [r2, r11]
    adds r3, r3, r2
    ldrmi r3, [sp, #0x54]
    bmi _4b4
    cmp r3, #0x58
    movgt r3, r6
_4b4:
    mov r2, r9, lsl #0x10
    mov r2, r2, asr #0x10
    strh r2, [r4]
    strb r3, [r4, #2]
    strh r2, [r5], #2
    add r0, r0, #1
    add r1, r1, #1
_4d0:
    ldr r2, [sp, #0x30]
    bic r11, r2, #1
    cmp r0, r11
    bhs _630
_4e0:
    ldrb r2, [r1]
    ldrb r3, [r4, #2]
    ldrsh r9, [r4]
    and lr, r2, #0xf
    ands r2, lr, #4
    ldr r2, [pc, #0x3b0]
    mov r10, r3, lsl #1
    ldrsh r12, [r2, r10]
    mov r2, r12, asr #3
    addne r2, r2, r12
    ands r10, lr, #2
    str r10, [sp, #0x58]
    addne r2, r2, r12, asr #1
    ands r10, lr, #1
    str r10, [sp, #0x5c]
    addne r2, r2, r12, asr #2
    ands r10, lr, #8
    beq _540
    sub r9, r9, r2
    mov r2, #0x8000
    rsb r2, r2, #0
    cmp r9, r2
    movlt r9, r8
    b _54c
_540:
    add r9, r9, r2
    cmp r9, r7
    movgt r9, r7
_54c:
    ldr r2, [pc, #0x35c]
    ldrsb r2, [r2, lr]
    adds r3, r3, r2
    ldrmi r3, [sp, #0x60]
    bmi _568
    cmp r3, #0x58
    movgt r3, r6
_568:
    mov r2, r9, lsl #0x10
    mov r2, r2, asr #0x10
    strh r2, [r4]
    strb r3, [r4, #2]
    strh r2, [r5]
    ldrb r2, [r1]
    ldrb r3, [r4, #2]
    ldrsh r12, [r4]
    mov r2, r2, asr #4
    and r9, r2, #0xf
    ands r2, r9, #4
    ldr r2, [pc, #0x310]
    mov r10, r3, lsl #1
    ldrsh lr, [r2, r10]
    mov r2, lr, asr #3
    addne r2, r2, lr
    ands r10, r9, #2
    str r10, [sp, #0x64]
    addne r2, r2, lr, asr #1
    ands r10, r9, #1
    str r10, [sp, #0x68]
    addne r2, r2, lr, asr #2
    ands r10, r9, #8
    beq _5e0
    sub r12, r12, r2
    mov r2, #0x8000
    rsb r2, r2, #0
    cmp r12, r2
    movlt r12, r8
    b _5ec
_5e0:
    add r12, r12, r2
    cmp r12, r7
    movgt r12, r7
_5ec:
    ldr r2, [pc, #0x2bc]
    ldrsb r2, [r2, r9]
    adds r3, r3, r2
    ldrmi r3, [sp, #0x6c]
    bmi _608
    cmp r3, #0x58
    movgt r3, r6
_608:
    mov r2, r12, lsl #0x10
    mov r9, r2, asr #0x10
    strh r9, [r4]
    strb r3, [r4, #2]
    add r0, r0, #2
    strh r9, [r5, #2]
    add r5, r5, #4
    cmp r0, r11
    add r1, r1, #1
    blo _4e0
_630:
    ldr r2, [sp, #0x30]
    cmp r0, r2
    bhs _6d0
    ldrb r2, [r1]
    ldrb r0, [r4, #2]
    ldrsh r1, [r4]
    and r2, r2, #0xf
    ands r3, r2, #4
    ldr r3, [pc, #0x254]
    mov r9, r0, lsl #1
    ldrsh r10, [r3, r9]
    mov r9, r10, asr #3
    addne r9, r9, r10
    ands r3, r2, #2
    addne r9, r9, r10, asr #1
    ands r3, r2, #1
    addne r9, r9, r10, asr #2
    ands r3, r2, #8
    beq _694
    mov r3, #0x8000
    sub r1, r1, r9
    rsb r3, r3, #0
    cmp r1, r3
    movlt r1, r8
    b _6a0
_694:
    add r1, r1, r9
    cmp r1, r7
    movgt r1, r7
_6a0:
    ldr r3, [pc, #0x208]
    ldrsb r2, [r3, r2]
    adds r0, r0, r2
    ldrmi r0, [sp, #0x70]
    bmi _6bc
    cmp r0, #0x58
    movgt r0, r6
_6bc:
    mov r1, r1, lsl #0x10
    mov r1, r1, asr #0x10
    strh r1, [r4]
    strb r0, [r4, #2]
    strh r1, [r5]
_6d0:
    ldr r0, [pc, #0x1cc]
    bl func_02006628
    b _720
_6dc:
    ldr r0, [sp, #0x78]
    ldr r0, [r0, #0x10c]
    mov r0, r0, lsl #0x19
    movs r0, r0, asr #0x1f
    beq _704
    ldr r1, [sp, #0x74]
    ldr r2, [sp, #0x28]
    mov r0, r9
    bl MI_CpuFill8
    b _720
_704:
    ldr r0, [sp, #8]
    ldr r2, [sp, #0x28]
    ldr r3, [r0, #0x14]
    ldr r0, [sp, #0x10]
    mov r1, r9
    add r0, r3, r0
    bl MI_CpuCopy8
_720:
    ldr r0, [sp, #0x38]
    add r4, r4, #4
    add r0, r0, #1
    str r0, [sp, #0x38]
    ldr r0, [sp, #8]
    ldr r1, [r0, #0x10]
    ldr r0, [sp, #0x38]
    cmp r0, r1
    blt _31c
_744:
    ldr r0, [sp, #0x78]
    ldr r0, [r0, #0x10c]
    mov r1, r0, lsl #0x1b
    movs r1, r1, asr #0x1f
    bicne r1, r0, #0x10
    ldrne r0, [sp, #0x78]
    strne r1, [r0, #0x10c]
    bne _7e0
    ldr r0, [sp, #0xc]
    cmp r0, #0
    ldrne r0, [sp, #0x78]
    ldrne r1, [r0, #0xc4]
    strne r1, [r0, #0x158]
    bne _794
    ldr r0, [sp, #0x78]
    ldr r1, [r0, #0x158]
    ldr r0, [sp, #0x24]
    add r1, r1, r0
    ldr r0, [sp, #0x78]
    str r1, [r0, #0x158]
_794:
    ldr r1, [sp, #0x10]
    ldr r0, [sp, #0x28]
    add r0, r1, r0
    str r0, [sp, #0x10]
    ldr r1, [sp, #0x14]
    ldr r0, [sp, #0x28]
    sub r0, r1, r0
    str r0, [sp, #0x14]
    ldr r0, [sp, #0x78]
    ldr r0, [r0, #0x10c]
    mov r0, r0, lsl #0x1a
    movs r0, r0, asr #0x1f
    beq _7e0
    ldr r0, [sp, #0x78]
    ldr r0, [r0, #0x138]
    cmp r0, #0
    beq _7e0
    ldr r0, [sp, #0x78]
    bl func_0201f5b8
_7e0:
    ldr r0, [sp, #0x14]
    cmp r0, #0
    bne _ac
_7ec:
    ldr r0, [sp, #0x78]
    ldr r4, [r0, #0x130]
    cmp r4, #0
    beq _83c
    ldrb r0, [r0, #0xbc]
    ldr r3, [sp, #8]
    cmp r0, #0
    moveq r0, #0
    movne r0, #1
    str r0, [sp]
    ldr r0, [sp, #0x78]
    ldr r1, [r0, #0x134]
    ldr r0, [sp, #8]
    str r1, [sp, #4]
    ldr r1, [sp, #8]
    add r2, r0, #0x14
    ldr r0, [r0, #0xc]
    ldr r1, [r1, #0x10]
    ldr r3, [r3, #0x2c]
    blx r4
_83c:
    ldr r0, [sp, #8]
    mov r4, #0
    ldr r0, [r0, #0x10]
    cmp r0, #0
    ble _87c
_850:
    ldr r0, [sp, #8]
    ldr r1, [sp, #8]
    add r0, r0, r4, lsl #2
    ldr r0, [r0, #0x14]
    ldr r1, [r1, #0x2c]
    bl DC_FlushRange
    ldr r0, [sp, #8]
    add r4, r4, #1
    ldr r0, [r0, #0x10]
    cmp r4, r0
    blt _850
_87c:
    ldr r0, [sp, #8]
    ldr r0, [r0, #0xc]
    cmp r0, #0
    ldreq r0, [sp, #0x78]
    moveq r1, #1
    streq r1, [r0, #0x114]
    add sp, sp, #0x7c
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd 0x00007fff
    dcd data_02083f5c
    dcd data_02083f4c
    dcd data_02072a4c
    dcd data_02072a3c
}

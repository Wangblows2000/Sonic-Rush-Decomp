// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 data_0206e470;
extern u32 data_02072c70;
extern u32 data_02072c78;
extern u32 data_02072d60;
extern u32 data_02072da0;
extern u32 data_02078034;
extern void func_02000cd8(void);
extern void func_02000d98(void);
extern void func_02002240(void);
extern void func_02032d2c(void);
extern void func_02036aec(void);
extern void func_02036bfc(void);

asm void func_02032480(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0xd4
    mov r10, r0
    ldr r7, [r10, #0x10]
    mov r0, #0
    ldr r4, [r7, #0xc]
    str r0, [sp, #4]
    add r9, r7, r4
    ldr r0, [r10, #0x24]
    mov r8, r1
    ldr r1, [sp, #4]
    ldr r11, [r10, #0x2c]
    add r9, r0, r9
    ldr r6, [r10]
    ldr r5, [pc, #0x7a4]
    ands r0, r11, #0x80000
    ldr r0, [r5, r6, lsl #2]
    mov r7, r2
    mov r4, r3
    str r1, [sp, #8]
    str r0, [sp]
    addne sp, sp, #0xd4
    ldmneia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxne lr
    ldrh r0, [r9]
    cmp r0, #0
    addeq sp, sp, #0xd4
    ldmeqia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxeq lr
    cmp r8, #0
    rsblt r1, r8, #0
    movge r1, r8
    cmp r1, #0x22
    addlt sp, sp, #0xd4
    ldmltia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxlt lr
    cmp r7, #0
    rsblt r1, r7, #0
    movge r1, r7
    cmp r1, #0x22
    addlt sp, sp, #0xd4
    ldmltia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxlt lr
    ands r1, r11, #0x10000
    bne _f0
    ldr r1, [sp]
    cmp r0, #4
    add r1, r1, #0x800
    ldrh r2, [r1, #0xa0]
    ldrh r1, [r1, #0x9e]
    movhs r0, #4
    add r1, r2, r1
    add r0, r0, r1
    cmp r0, #0x20
    blt _f0
    mov r0, r10
    bl func_02032d2c
    add sp, sp, #0xd4
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
_f0:
    cmp r4, #0
    bne _108
    cmp r8, #0x1000
    bne _108
    cmp r7, #0x1000
    beq _134
_108:
    cmp r8, #0
    addgt r0, r8, #0xd0
    suble r0, r8, #0xd0
    bl func_02002240
    mov r6, r0
    cmp r7, #0
    addgt r0, r7, #0xd0
    suble r0, r7, #0xd0
    bl func_02002240
    mov r5, r0
    b _14c
_134:
    mov r0, r8
    bl func_02002240
    mov r6, r0
    mov r0, r7
    bl func_02002240
    mov r5, r0
_14c:
    ldr r0, [pc, #0x694]
    ldr r2, [pc, #0x694]
    and r0, r4, r0
    mov r0, r0, asr #4
    mov r1, r0, lsl #1
    add r0, r1, #1
    mov r1, r1, lsl #1
    mov r0, r0, lsl #1
    ldrsh r1, [r2, r1]
    ldrsh r2, [r2, r0]
    add r0, sp, #0x50
    bl func_02000cd8
    rsb r11, r6, #0
    add r0, sp, #0x50
    add r1, sp, #0xa0
    mov r2, r11
    mov r3, r5
    bl func_02000d98
    rsb r0, r8, #0
    str r0, [sp, #0x2c]
    ldr r2, [sp, #0x2c]
    add r0, sp, #0x50
    add r1, sp, #0x60
    mov r3, r7
    bl func_02000d98
    rsb r4, r5, #0
    add r0, sp, #0x50
    add r1, sp, #0xb0
    mov r2, r6
    mov r3, r4
    bl func_02000d98
    rsb r0, r7, #0
    str r0, [sp, #0x28]
    ldr r3, [sp, #0x28]
    add r0, sp, #0x50
    add r1, sp, #0x70
    mov r2, r8
    bl func_02000d98
    mov r2, r11
    mov r3, r4
    add r0, sp, #0x50
    add r1, sp, #0xc0
    bl func_02000d98
    ldr r2, [sp, #0x2c]
    ldr r3, [sp, #0x28]
    add r0, sp, #0x50
    add r1, sp, #0x80
    bl func_02000d98
    mov r2, r6
    mov r3, r5
    add r0, sp, #0x50
    add r1, sp, #0x90
    bl func_02000d98
    add r0, sp, #0x50
    mov r2, r8
    mov r3, r7
    mov r1, r0
    bl func_02000d98
    ldrsh r1, [r10, #4]
    ldrsh r2, [r10, #6]
    ldr r0, [r10, #0x2c]
    mov r1, r1, lsl #0xc
    str r1, [sp, #0x18]
    mov r1, r2, lsl #0xc
    str r1, [sp, #0x1c]
    ands r1, r0, #0x2000
    beq _280
    ldr r1, [sp]
    add r1, r1, #0x800
    ldrsh r3, [r1, #0xa2]
    ldrsh r2, [r1, #0xa4]
    ldr r1, [sp, #0x18]
    add r1, r1, r3, lsl #12
    str r1, [sp, #0x18]
    ldr r1, [sp, #0x1c]
    add r1, r1, r2, lsl #12
    str r1, [sp, #0x1c]
_280:
    ands r1, r0, #0x80
    ldrne r1, [sp, #8]
    and r2, r0, #0xc00
    orrne r1, r1, #0x1000
    movne r1, r1, lsl #0x10
    movne r1, r1, lsr #0x10
    strne r1, [sp, #8]
    ands r1, r0, #0x100
    ldrne r1, [sp, #8]
    orrne r1, r1, #0x2000
    movne r1, r1, lsl #0x10
    movne r1, r1, lsr #0x10
    strne r1, [sp, #8]
    ands r1, r0, #0x1000
    ldrne r1, [sp, #4]
    orrne r1, r1, #0x1000
    movne r1, r1, lsl #0x10
    movne r1, r1, lsr #0x10
    strne r1, [sp, #4]
    ldr r1, [sp, #4]
    ands r0, r0, #0x200
    orr r1, r1, r2
    mov r1, r1, lsl #0x10
    mov r1, r1, lsr #0x10
    movne r0, r1
    orrne r0, r0, #0x200
    movne r0, r0, lsl #0x10
    str r1, [sp, #4]
    movne r0, r0, lsr #0x10
    strne r0, [sp, #4]
    ldrh r0, [r9, #0x10]
    and r1, r0, #0xc00
    cmp r1, #0xc00
    bne _334
    ldr r2, [sp]
    ldr r0, [r10]
    ldrh r3, [r2, #0x96]
    ldr r4, [pc, #0x4d4]
    ldr r5, [r10, #0x34]
    ldr r4, [r4, r0, lsl #2]
    ldr r2, [pc, #0x4cc]
    sub r4, r5, r4
    add r3, r3, #7
    and r8, r2, r4, lsr r3
    b _35c
_334:
    ldr r2, [sp]
    ldr r0, [r10]
    ldrh r3, [r2, #0x94]
    ldr r4, [pc, #0x4a8]
    ldr r5, [r10, #0x34]
    ldr r4, [r4, r0, lsl #2]
    ldr r2, [pc, #0x4a0]
    sub r4, r5, r4
    add r3, r3, #5
    and r8, r2, r4, lsr r3
_35c:
    ldr r2, [pc, #0x494]
    cmp r1, #0xc00
    ldrh r5, [r2]
    ldrh r4, [r2, #2]
    ldrh r3, [r2, #4]
    ldrh r1, [r2, #6]
    strh r5, [sp, #0x48]
    strh r4, [sp, #0x4a]
    strh r3, [sp, #0x4c]
    strh r1, [sp, #0x4e]
    add r7, r9, #0x10
    bne _3a8
    ldr r2, [pc, #0x450]
    mov r1, #0
    ldr r0, [r2, r0, lsl #2]
    str r1, [sp, #0x10]
    ldrh r0, [r0, #0x96]
    str r0, [sp, #0xc]
    b _3e4
_3a8:
    ldr r3, [r10, #0x10]
    ldr r4, [pc, #0x430]
    ldr r2, [r3, #4]
    ldrh r1, [r10, #8]
    ldr r4, [r4, r0, lsl #2]
    add r0, r3, r2
    add r0, r0, r1, lsl #3
    ldrh r1, [r0, #8]
    ldrh r0, [r4, #0x94]
    cmp r1, #0
    str r0, [sp, #0xc]
    moveq r0, #0
    streq r0, [sp, #0x10]
    movne r0, #1
    strne r0, [sp, #0x10]
_3e4:
    ldrh r1, [r9]
    mov r0, #0
    str r0, [sp, #0x14]
    cmp r1, #0
    addls sp, sp, #0xd4
    ldmlsia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxls lr
    ldr r0, [sp, #0xc]
    mov r1, #1
    mov r0, r1, lsl r0
    str r0, [sp, #0x24]
    ldr r0, [sp, #4]
    and r0, r0, #0x200
    str r0, [sp, #0x20]
_41c:
    ldrh r2, [r7]
    ldrh r1, [r7, #2]
    mov r0, #0x800
    and r2, r2, #0xc000
    mov r2, r2, asr #0xc
    and r1, r1, #0xc000
    orr r11, r2, r1, asr #14
    ldr r1, [pc, #0x3bc]
    ldr r2, [sp, #8]
    ldr r1, [r1, r11, lsl #2]
    str r1, [sp, #0x44]
    ldrh r1, [r7, #2]
    ldrh r3, [r7]
    ldrsh r12, [r9, #0xe]
    eor r2, r2, r1
    and r2, r2, #0x3000
    mov r6, r2, asr #0xc
    and r2, r3, #0xff
    ldr r3, [pc, #0x394]
    ldrh r5, [sp, #0x46]
    and r4, r1, r3
    sub r2, r2, r12
    add r12, r2, r5, asr #1
    ldrsh r3, [r9, #0xc]
    ldrh r1, [sp, #0x44]
    mov r12, r12, lsl #0xc
    sub r2, r4, r3
    add r2, r2, r1, asr #1
    mov r4, r2, lsl #0xc
    mov r2, r1, lsl #0xb
    str r2, [sp, #0x34]
    ldr r2, [sp, #0x18]
    sub r1, r2, r1, lsl #11
    str r1, [sp, #0x38]
    add r1, sp, #0x50
    ldr lr, [r1, r6, lsl #4]
    add r1, r1, r6, lsl #4
    smull r3, r2, lr, r4
    adds r3, r3, r0
    adc r2, r2, #0
    mov r3, r3, lsr #0xc
    orr r3, r3, r2, lsl #20
    mov r2, r5, lsl #0xb
    str r2, [sp, #0x3c]
    ldr r2, [sp, #0x1c]
    ldr lr, [r1, #8]
    sub r2, r2, r5, lsl #11
    str r2, [sp, #0x40]
    smull r5, r2, lr, r12
    adds r5, r5, r0
    adc r2, r2, #0
    mov r5, r5, lsr #0xc
    orr r5, r5, r2, lsl #20
    ldr r2, [sp, #0x38]
    add r3, r3, r5
    add r5, r2, r3
    ldr r2, [r1, #4]
    ldr r1, [r1, #0xc]
    smull r4, r3, r2, r4
    smull r12, r2, r1, r12
    adds r1, r4, r0
    adc r4, r3, #0
    adds r3, r12, r0
    mov r12, r1, lsr #0xc
    adc r0, r2, #0
    mov r1, r3, lsr #0xc
    orr r1, r1, r0, lsl #20
    orr r12, r12, r4, lsl #20
    ldr r0, [sp, #0x40]
    add r1, r12, r1
    add r4, r0, r1
    ldr r0, [sp, #0x20]
    cmp r0, #0
    beq _568
    ldr r0, [sp, #0x34]
    sub r5, r5, r0
    ldr r0, [sp, #0x3c]
    sub r4, r4, r0
    add r0, sp, #0x44
    ldr r0, [r0]
    mov r1, r0, lsl #1
    add r0, sp, #0x44
    str r1, [r0]
_568:
    ldr r0, [r10, #0x2c]
    ands r0, r0, #0x4000
    beq _5dc
    ldrh r0, [sp, #0x44]
    add r0, r5, r0, lsl #12
    cmp r0, #0
    ble _5a4
    cmp r5, #0x100000
    bge _5a4
    ldrh r0, [sp, #0x46]
    add r0, r4, r0, lsl #12
    cmp r0, #0
    ble _5a4
    cmp r4, #0xc0000
    blt _5dc
_5a4:
    ldrh r0, [r9, #2]
    ands r0, r0, #1
    bne _7bc
    ldr r0, [pc, #0x24c]
    mov r1, r11, lsl #1
    ldrh r1, [r0, r1]
    ldr r0, [sp, #0x24]
    add r0, r0, r1
    sub r1, r0, #1
    ldr r0, [sp, #0xc]
    mov r1, r1, asr r0
    ldr r0, [sp, #0x10]
    add r8, r8, r1, lsl r0
    b _7bc
_5dc:
    mov r1, r6, lsl #1
    add r0, sp, #0x48
    ldrh r0, [r0, r1]
    str r0, [sp, #0x30]
    mov r0, r0, lsl #0x10
    movs r0, r0, asr #0x10
    bpl _6b4
    add r1, sp, #0x90
    ldr r0, [r10]
    add r1, r1, r6, lsl #4
    bl func_02036aec
    mov r2, r6, lsl #1
    add r1, sp, #0x48
    strh r0, [r1, r2]
    mov r0, r1
    ldrh r0, [r0, r2]
    str r0, [sp, #0x30]
    mov r0, r0, lsl #0x10
    movs r0, r0, asr #0x10
    bmi _67c
    ldr r1, [sp]
    ldr r0, [pc, #0x1d0]
    add r1, r1, r0
    ldr r0, [sp, #0x30]
    add r0, r1, r0, lsl #5
    add r1, sp, #0x90
    ldr r3, [r1, r6, lsl #4]
    add r2, r1, r6, lsl #4
    mov r1, r3, asr #4
    strh r1, [r0, #6]
    ldr r1, [r2, #4]
    mov r1, r1, asr #4
    strh r1, [r0, #0xe]
    ldr r1, [r2, #8]
    mov r1, r1, asr #4
    strh r1, [r0, #0x16]
    ldr r1, [r2, #0xc]
    mov r1, r1, asr #4
    strh r1, [r0, #0x1e]
    b _6b4
_67c:
    ldrh r0, [r9, #2]
    ands r0, r0, #1
    bne _7bc
    ldr r0, [pc, #0x174]
    mov r1, r11, lsl #1
    ldrh r1, [r0, r1]
    ldr r0, [sp, #0x24]
    add r0, r0, r1
    sub r1, r0, #1
    ldr r0, [sp, #0xc]
    mov r1, r1, asr r0
    ldr r0, [sp, #0x10]
    add r8, r8, r1, lsl r0
    b _7bc
_6b4:
    ldrh r1, [r10, #0x46]
    ldr r0, [r10]
    bl func_02036bfc
    mov r1, r4, asr #0xc
    and r4, r1, #0xff
    ldr r1, [pc, #0x130]
    and r3, r1, r5, asr #12
    mov r1, #0x200
    ldrh r5, [r7]
    rsb r2, r1, #0
    bic r1, r5, #0xff
    orr r1, r1, r4
    orr r1, r1, #0x100
    strh r1, [r0]
    ldrh r4, [r0]
    ldr r1, [sp, #4]
    orr r1, r4, r1
    strh r1, [r0]
    ldrh r1, [r7, #2]
    and r1, r1, r2
    orr r1, r1, r3
    strh r1, [r0, #2]
    ldrh r1, [r0, #2]
    bic r2, r1, #0x3e00
    ldr r1, [sp, #0x30]
    orr r1, r2, r1, lsl #9
    strh r1, [r0, #2]
    ldrh r1, [r9, #2]
    ands r1, r1, #1
    bne _788
    ldr r1, [pc, #0xc0]
    ldrh r3, [r7, #4]
    and r4, r8, r1
    ldr r1, [r10, #0x2c]
    mov r2, r11, lsl #1
    and r1, r1, #0x60000
    mov r1, r1, lsr #0x11
    orr r4, r4, r1, lsl #10
    ldrh r1, [r10, #0x44]
    add r1, r3, r1, lsl #12
    and r1, r1, #0xf000
    orr r1, r4, r1
    strh r1, [r0, #4]
    ldr r0, [pc, #0x9c]
    ldrh r1, [r0, r2]
    ldr r0, [sp, #0x24]
    add r0, r0, r1
    sub r1, r0, #1
    ldr r0, [sp, #0xc]
    mov r1, r1, asr r0
    ldr r0, [sp, #0x10]
    add r8, r8, r1, lsl r0
    b _7bc
_788:
    ldr r2, [r10, #0x2c]
    ldrh r3, [r7, #4]
    and r2, r2, #0x60000
    ldrh r1, [r10, #0x44]
    mov r4, r2, lsr #0x11
    add r2, r3, r1, lsl #12
    ldr r1, [pc, #0x4c]
    add r3, r3, r8
    and r1, r3, r1
    orr r3, r1, r4, lsl #10
    and r1, r2, #0xf000
    orr r1, r3, r1
    strh r1, [r0, #4]
_7bc:
    ldr r0, [sp, #0x14]
    ldrh r1, [r9]
    add r0, r0, #1
    str r0, [sp, #0x14]
    add r7, r7, #8
    cmp r0, r1
    blo _41c
    add sp, sp, #0xd4
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd data_02072c78
    dcd 0x0000ffff
    dcd data_0206e470
    dcd data_02072c70
    dcd 0x000003ff
    dcd data_02078034
    dcd data_02072da0
    dcd 0x000001ff
    dcd data_02072d60
    dcd 0x000008a8
}

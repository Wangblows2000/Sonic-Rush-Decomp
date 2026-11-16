// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void _s32_div_f(void);
extern u32 gObjVramBaseTable;
extern u32 gDisplayTable;
extern u32 data_02072da0;
extern void Oam_AllocObj(void);

asm void StageObj_DrawSpriteAlt(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x54
    mov r10, r0
    ldr r2, [r10, #0x10]
    mov r0, #0
    str r0, [sp, #8]
    ldr r0, [r2, #0xc]
    ldr r3, [r10, #0x24]
    add r2, r2, r0
    ldrh r8, [r3, r2]
    ldr r0, [sp, #8]
    ldr r4, [r10]
    ldr r5, [pc, #0x4d8]
    mov r9, r1
    ldr r5, [r5, r4, lsl #2]
    str r0, [sp, #0xc]
    cmp r8, #0
    add r7, r3, r2
    addeq sp, sp, #0x54
    ldmeqia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxeq lr
    ldrh r0, [r5, #0x94]
    ldrsh r3, [r7, #8]
    mov r1, #8
    ldrsh r2, [r7, #4]
    str r0, [sp]
    mov r1, r1, lsl r0
    sub r0, r3, r2
    ldrsh r2, [r10, #4]
    str r2, [sp, #0x28]
    ldrsh r2, [r10, #6]
    str r2, [sp, #0x2c]
    bl _s32_div_f
    ands r0, r0, #0xff
    str r0, [sp, #0x10]
    moveq r0, #1
    streq r0, [sp, #0x10]
    ldr r0, [r10, #0x2c]
    ands r1, r0, #0x2000
    beq _c4
    add r1, r5, #0x800
    ldrsh r3, [r1, #0xa2]
    ldrsh r2, [r1, #0xa4]
    ldr r1, [sp, #0x28]
    add r1, r1, r3
    str r1, [sp, #0x28]
    ldr r1, [sp, #0x2c]
    add r1, r1, r2
    str r1, [sp, #0x2c]
_c4:
    ands r1, r0, #0x80
    ldrne r1, [sp, #0xc]
    orrne r1, r1, #0x1000
    movne r1, r1, lsl #0x10
    movne r1, r1, lsr #0x10
    strne r1, [sp, #0xc]
    ands r1, r0, #0x100
    ldrne r1, [sp, #0xc]
    orrne r1, r1, #0x2000
    movne r1, r1, lsl #0x10
    movne r1, r1, lsr #0x10
    strne r1, [sp, #0xc]
    ands r0, r0, #0x1000
    ldrne r0, [sp, #8]
    orrne r0, r0, #0x1000
    movne r0, r0, lsl #0x10
    movne r0, r0, lsr #0x10
    strne r0, [sp, #8]
    ldrh r0, [r7, #0x10]
    and r0, r0, #0xc00
    cmp r0, #0xc00
    bne _144
    ldr r1, [pc, #0x3f0]
    ldrh r2, [r5, #0x96]
    ldr r3, [r10, #0x34]
    ldr r1, [r1, r4, lsl #2]
    add r2, r2, #7
    sub r3, r3, r1
    ldr r1, [pc, #0x3dc]
    and r1, r1, r3, lsr r2
    str r1, [sp, #4]
    b _168
_144:
    ldr r1, [pc, #0x3c8]
    ldrh r2, [r5, #0x94]
    ldr r3, [r10, #0x34]
    ldr r1, [r1, r4, lsl #2]
    add r2, r2, #5
    sub r3, r3, r1
    ldr r1, [pc, #0x3b4]
    and r1, r1, r3, lsr r2
    str r1, [sp, #4]
_168:
    cmp r0, #0xc00
    ldreqh r0, [r5, #0x96]
    add r6, r7, #0x10
    streq r0, [sp]
    mov r0, #0
    cmp r8, #0
    str r0, [sp, #0x1c]
    addls sp, sp, #0x54
    ldmlsia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxls lr
    ldr r0, [sp]
    mov r1, #1
    mov r0, r1, lsl r0
    str r0, [sp, #0x38]
    ldr r0, [sp, #0xc]
    str r1, [sp, #0x40]
    and r0, r0, #0x1000
    str r0, [sp, #0x30]
    ldr r0, [sp, #0xc]
    and r0, r0, #0x2000
    str r0, [sp, #0x34]
    mov r0, #8
    str r0, [sp, #0x44]
    ldr r0, [sp, #0x1c]
    str r0, [sp, #0x4c]
    str r0, [sp, #0x48]
_1d0:
    ldrh r2, [r6]
    ldrh r0, [r6, #2]
    ldr r1, [r10]
    and r2, r2, #0xc000
    mov r2, r2, asr #0xc
    and r0, r0, #0xc000
    orr r2, r2, r0, asr #14
    ldr r0, [pc, #0x31c]
    mov r3, r2, lsl #2
    ldr r1, [r0, r1, lsl #2]
    ldr r0, [pc, #0x31c]
    add r4, r0, r2, lsl #2
    ldrh r2, [r1, #0x94]
    ldr r1, [sp, #0x44]
    ldrh r0, [r0, r3]
    mov r1, r1, lsl r2
    bl _s32_div_f
    ands r0, r0, #0xff
    str r0, [sp, #0x20]
    ldrh r0, [r4, #2]
    ldr r4, [sp, #0x48]
    mov r0, r0, asr #3
    and r0, r0, #0xff
    str r0, [sp, #0x24]
    ldreq r0, [sp, #0x40]
    streq r0, [sp, #0x20]
    ldr r0, [sp, #0x24]
    cmp r0, #0
    ldreq r0, [sp, #0x40]
    streq r0, [sp, #0x24]
    ldr r0, [sp, #0x24]
    cmp r0, #0
    mov r0, r4
    str r0, [sp, #0x18]
    bls _4e8
    str r0, [sp, #0x3c]
_260:
    ldr r3, [r10]
    ldr r1, [pc, #0x2a4]
    ldrh r2, [r6]
    ldr r1, [r1, r3, lsl #2]
    ldr r0, [sp, #0x20]
    ldrh r3, [r1, #0x94]
    ldr r1, [sp, #0x18]
    mov r2, r2, lsl #0x17
    add r2, r1, r2, asr #26
    ldr r1, [sp, #0x10]
    cmp r0, #0
    ldrh r0, [r6, #2]
    mul r2, r1, r2
    mov r0, r0, lsl #0x17
    mov r0, r0, asr #0x1a
    mov r0, r0, asr r3
    and r1, r0, #0xff
    and r0, r2, #0xff
    add r5, r1, r0
    ldr r0, [sp, #0x4c]
    str r0, [sp, #0x14]
    bls _4c4
_2b8:
    ldr r0, [sp, #0x30]
    cmp r0, #0
    beq _314
    ldr r2, [r10]
    ldr r0, [pc, #0x240]
    mvn r1, #0
    ldr r3, [r0, r2, lsl #2]
    ldr r0, [sp, #0x14]
    ldrh r8, [r3, #0x94]
    sub r2, r1, r0
    mov r3, r5, lsl #2
    ldr r0, [sp, #0x44]
    ldrsh r11, [r9, r3]
    mov r0, r0, lsl r8
    ldr r3, [sp, #0x28]
    ldrsh r1, [r7, #0xc]
    add r3, r3, r11
    ldrh r8, [r6, #2]
    add r3, r1, r3
    mov r1, r8, lsl #0x17
    sub r1, r3, r1, asr #23
    mla r11, r0, r2, r1
    b _358
_314:
    ldr r2, [r10]
    ldr r0, [pc, #0x1f0]
    ldrh r1, [r6, #2]
    ldr r2, [r0, r2, lsl #2]
    mov r0, r5, lsl #2
    ldrsh r3, [r9, r0]
    ldrh r2, [r2, #0x94]
    ldr r0, [sp, #0x44]
    mov r1, r1, lsl #0x17
    mov r0, r0, lsl r2
    ldr r2, [sp, #0x28]
    add r3, r2, r3
    ldrsh r2, [r7, #0xc]
    sub r2, r3, r2
    add r2, r2, r1, asr #23
    ldr r1, [sp, #0x14]
    mla r11, r1, r0, r2
_358:
    ldr r1, [sp, #0x34]
    cmp r1, #0
    beq _398
    add r2, r9, r5, lsl #2
    ldrsh r8, [r2, #2]
    ldr r2, [sp, #0x2c]
    ldrsh r1, [r7, #0xe]
    add r2, r2, r8
    ldrh r3, [r6]
    add r2, r1, r2
    mov r1, r3, lsl #0x18
    sub r2, r2, r1, asr #24
    ldr r1, [sp, #0x3c]
    sub r1, r2, r1
    sub r8, r1, #8
    b _3c4
_398:
    ldrh r2, [r6]
    add r1, r9, r5, lsl #2
    ldrsh r8, [r1, #2]
    mov r1, r2, lsl #0x18
    ldr r2, [sp, #0x2c]
    ldrsh r3, [r7, #0xe]
    add r2, r2, r8
    sub r2, r2, r3
    add r2, r2, r1, asr #24
    ldr r1, [sp, #0x3c]
    add r8, r1, r2
_3c4:
    ldr r1, [r10, #0x2c]
    ands r1, r1, #0x4000
    beq _408
    add r0, r11, r0
    cmp r0, #0
    ble _3f8
    cmp r11, #0x100
    bge _3f8
    add r0, r8, #8
    cmp r0, #0
    ble _3f8
    cmp r8, #0xc0
    blt _408
_3f8:
    add r0, r4, #1
    and r4, r0, #0xff
    add r5, r5, #1
    b _4ac
_408:
    ldrh r1, [r10, #0x46]
    ldr r0, [r10]
    bl Oam_AllocObj
    and r1, r8, #0xff
    orr r1, r1, #0x4000
    strh r1, [r0]
    ldr r1, [pc, #0xf8]
    ldrh r8, [r0]
    ldr r2, [sp, #8]
    and r3, r11, r1
    eor r2, r8, r2
    strh r2, [r0]
    strh r3, [r0, #2]
    ldrh r3, [r0, #2]
    ldr r2, [sp, #0xc]
    ldr r8, [pc, #0xcc]
    eor r2, r3, r2
    strh r2, [r0, #2]
    ldrh r11, [r6, #4]
    ldrh r3, [r10, #0x44]
    ldr r2, [r10, #0x2c]
    and r11, r11, r8
    ldr r8, [sp, #0x38]
    add r1, r4, #1
    add r8, r11, r8
    sub r12, r8, #1
    and r2, r2, #0x60000
    mov r3, r3, lsl #0xc
    ldr r11, [sp, #4]
    ldr r8, [sp]
    mov r2, r2, lsr #0x11
    add r8, r11, r12, asr r8
    add r8, r4, r8
    ldr r4, [pc, #0x84]
    and r3, r3, #0xf000
    and r4, r8, r4
    orr r2, r4, r2, lsl #10
    orr r2, r3, r2
    add r5, r5, #1
    strh r2, [r0, #4]
    and r4, r1, #0xff
_4ac:
    ldr r0, [sp, #0x14]
    add r1, r0, #1
    ldr r0, [sp, #0x20]
    str r1, [sp, #0x14]
    cmp r1, r0
    blo _2b8
_4c4:
    ldr r0, [sp, #0x3c]
    add r0, r0, #8
    str r0, [sp, #0x3c]
    ldr r0, [sp, #0x18]
    add r1, r0, #1
    ldr r0, [sp, #0x24]
    str r1, [sp, #0x18]
    cmp r1, r0
    blo _260
_4e8:
    ldr r0, [sp, #0x1c]
    ldrh r1, [r7]
    add r0, r0, #1
    str r0, [sp, #0x1c]
    add r6, r6, #8
    cmp r0, r1
    blo _1d0
    add sp, sp, #0x54
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd gDisplayTable
    dcd gObjVramBaseTable
    dcd 0x000003ff
    dcd data_02072da0
    dcd 0x000001ff
}

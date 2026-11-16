// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 gObjVramBaseTable;
extern u32 gDisplayTable;
extern u32 data_02072d60;
extern u32 data_02072da0;
extern void Oam_AllocObj(void);

asm void AnimatorSprite_Draw(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x2c
    mov r10, r0
    ldr r4, [r10, #0x10]
    mov r0, #0
    ldr r1, [r4, #0xc]
    ldr r3, [r10, #0x2c]
    ldr r2, [r10, #0x24]
    add r1, r4, r1
    ldr r5, [r10]
    ldr r6, [pc, #0x484]
    str r0, [sp]
    str r0, [sp, #4]
    ands r0, r3, #0x80000
    add r7, r2, r1
    addne sp, sp, #0x2c
    ldr r0, [r6, r5, lsl #2]
    ldmneia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxne lr
    ldrh r1, [r7]
    str r1, [sp, #0x24]
    cmp r1, #0
    addeq sp, sp, #0x2c
    ldmeqia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxeq lr
    ldrsh r1, [r7, #6]
    ldrsh r6, [r7, #4]
    ldrsh r9, [r7, #8]
    str r1, [sp, #0x28]
    ands r1, r3, #0x2000
    ldrsh r1, [r10, #4]
    mov r12, r6
    ldr r11, [sp, #0x28]
    str r1, [sp, #0x10]
    ldrsh r1, [r10, #6]
    ldrsh r8, [r7, #0xa]
    str r1, [sp, #0x14]
    beq _bc
    add r1, r0, #0x800
    ldrsh r2, [r1, #0xa2]
    ldrsh lr, [r1, #0xa4]
    ldr r1, [sp, #0x10]
    add r1, r1, r2
    str r1, [sp, #0x10]
    ldr r1, [sp, #0x14]
    add r1, r1, lr
    str r1, [sp, #0x14]
_bc:
    ands r1, r3, #0x80
    beq _e0
    ldr r1, [sp, #4]
    rsb r12, r9, #0
    orr r1, r1, #0x1000
    mov r1, r1, lsl #0x10
    mov r1, r1, lsr #0x10
    str r1, [sp, #4]
    rsb r9, r6, #0
_e0:
    ands r1, r3, #0x100
    beq _108
    ldr r1, [sp, #4]
    rsb r11, r8, #0
    orr r1, r1, #0x2000
    mov r1, r1, lsl #0x10
    mov r1, r1, lsr #0x10
    str r1, [sp, #4]
    ldr r1, [sp, #0x28]
    rsb r8, r1, #0
_108:
    ands r1, r3, #0x1000
    ldrne r1, [sp]
    and r2, r3, #0xc00
    orrne r1, r1, #0x1000
    movne r1, r1, lsl #0x10
    movne r1, r1, lsr #0x10
    strne r1, [sp]
    ands r1, r3, #0x4000
    ldr r1, [sp]
    orr r1, r1, r2
    mov r1, r1, lsl #0x10
    mov r1, r1, lsr #0x10
    str r1, [sp]
    beq _1a0
    ldr r1, [sp, #0x10]
    add r1, r1, r9
    cmp r1, #0
    addle sp, sp, #0x2c
    ldmleia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxle lr
    ldr r1, [sp, #0x10]
    add r1, r1, r12
    cmp r1, #0x100
    addge sp, sp, #0x2c
    ldmgeia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxge lr
    ldr r1, [sp, #0x14]
    add r1, r1, r8
    cmp r1, #0
    addle sp, sp, #0x2c
    ldmleia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxle lr
    ldr r1, [sp, #0x14]
    add r1, r1, r11
    cmp r1, #0xc0
    addge sp, sp, #0x2c
    ldmgeia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxge lr
_1a0:
    ldrh r1, [r7, #0x10]
    and r1, r1, #0xc00
    cmp r1, #0xc00
    bne _1d4
    ldr r2, [pc, #0x300]
    ldrh r3, [r0, #0x96]
    ldr r6, [r10, #0x34]
    ldr r5, [r2, r5, lsl #2]
    ldr r2, [pc, #0x2f4]
    sub r5, r6, r5
    add r3, r3, #7
    and r6, r2, r5, lsr r3
    b _1f4
_1d4:
    ldr r2, [pc, #0x2dc]
    ldrh r3, [r0, #0x94]
    ldr r6, [r10, #0x34]
    ldr r5, [r2, r5, lsl #2]
    ldr r2, [pc, #0x2d0]
    sub r5, r6, r5
    add r3, r3, #5
    and r6, r2, r5, lsr r3
_1f4:
    cmp r1, #0xc00
    ldreqh r0, [r0, #0x96]
    add r5, r7, #0x10
    streq r0, [sp, #8]
    moveq r0, #0
    streq r0, [sp, #0xc]
    beq _240
    ldr r2, [r4, #4]
    ldrh r0, [r0, #0x94]
    ldrh r1, [r10, #8]
    add r2, r4, r2
    str r0, [sp, #8]
    add r0, r2, r1, lsl #3
    ldrh r0, [r0, #8]
    cmp r0, #0
    moveq r0, #0
    streq r0, [sp, #0xc]
    movne r0, #1
    strne r0, [sp, #0xc]
_240:
    ldr r0, [sp, #0x24]
    mov r4, #0
    cmp r0, #0
    addls sp, sp, #0x2c
    ldmlsia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxls lr
    ldr r0, [sp, #8]
    mov r1, #1
    mov r0, r1, lsl r0
    str r0, [sp, #0x20]
    ldr r0, [sp, #4]
    and r0, r0, #0x1000
    str r0, [sp, #0x18]
    ldr r0, [sp, #4]
    and r0, r0, #0x2000
    str r0, [sp, #0x1c]
_280:
    ldr r0, [sp, #0x18]
    ldrh r1, [r5]
    cmp r0, #0
    ldrh r0, [r5, #2]
    and r3, r1, #0xc000
    mov r3, r3, asr #0xc
    and r2, r0, #0xc000
    orr r11, r3, r2, asr #14
    ldr r2, [pc, #0x218]
    add r2, r2, r11, lsl #2
    beq _2d0
    ldr r3, [pc, #0x210]
    ldrsh r9, [r7, #0xc]
    and r8, r0, r3
    ldr r0, [sp, #0x10]
    ldrh r3, [r2]
    add r0, r0, r9
    sub r0, r0, r8
    sub r8, r0, r3
    b _2e8
_2d0:
    ldr r3, [pc, #0x1ec]
    and r8, r0, r3
    ldrsh r3, [r7, #0xc]
    ldr r0, [sp, #0x10]
    sub r0, r0, r3
    add r8, r8, r0
_2e8:
    ldr r0, [sp, #0x1c]
    cmp r0, #0
    andeq r3, r1, #0xff
    ldreqsh r1, [r7, #0xe]
    ldreq r0, [sp, #0x14]
    subeq r0, r0, r1
    addeq r9, r3, r0
    beq _324
    ldrsh r9, [r7, #0xe]
    ldr r0, [sp, #0x14]
    and r3, r1, #0xff
    add r0, r0, r9
    ldrh r1, [r2, #2]
    sub r0, r0, r3
    sub r9, r0, r1
_324:
    ldr r0, [r10, #0x2c]
    ands r0, r0, #0x4000
    beq _398
    ldrh r0, [r2]
    add r0, r8, r0
    cmp r0, #0
    ble _360
    cmp r8, #0x100
    bge _360
    ldrh r0, [r2, #2]
    add r0, r9, r0
    cmp r0, #0
    ble _360
    cmp r9, #0xc0
    blt _398
_360:
    ldrh r0, [r7, #2]
    ands r0, r0, #1
    bne _494
    ldr r0, [pc, #0x154]
    mov r1, r11, lsl #1
    ldrh r1, [r0, r1]
    ldr r0, [sp, #0x20]
    add r0, r0, r1
    sub r1, r0, #1
    ldr r0, [sp, #8]
    mov r1, r1, asr r0
    ldr r0, [sp, #0xc]
    add r6, r6, r1, lsl r0
    b _494
_398:
    ldrh r1, [r10, #0x46]
    ldr r0, [r10]
    bl Oam_AllocObj
    ldr r1, [pc, #0x118]
    and r9, r9, #0xff
    and r3, r8, r1
    mov r1, #0x200
    ldrh r8, [r5]
    rsb r2, r1, #0
    bic r1, r8, #0xff
    orr r1, r1, r9
    strh r1, [r0]
    ldrh r8, [r0]
    ldr r1, [sp]
    orr r1, r8, r1
    strh r1, [r0]
    ldrh r1, [r5, #2]
    and r1, r1, r2
    orr r1, r1, r3
    strh r1, [r0, #2]
    ldrh r2, [r0, #2]
    ldr r1, [sp, #4]
    eor r1, r2, r1
    strh r1, [r0, #2]
    ldrh r1, [r7, #2]
    ands r1, r1, #1
    bne _460
    ldr r1, [pc, #0xb0]
    ldrh r3, [r5, #4]
    and r8, r6, r1
    ldr r1, [r10, #0x2c]
    mov r2, r11, lsl #1
    and r1, r1, #0x60000
    mov r1, r1, lsr #0x11
    orr r8, r8, r1, lsl #10
    ldrh r1, [r10, #0x44]
    add r1, r3, r1, lsl #12
    and r1, r1, #0xf000
    orr r1, r8, r1
    strh r1, [r0, #4]
    ldr r0, [pc, #0x88]
    ldrh r1, [r0, r2]
    ldr r0, [sp, #0x20]
    add r0, r0, r1
    sub r1, r0, #1
    ldr r0, [sp, #8]
    mov r1, r1, asr r0
    ldr r0, [sp, #0xc]
    add r6, r6, r1, lsl r0
    b _494
_460:
    ldr r2, [r10, #0x2c]
    ldrh r3, [r5, #4]
    and r2, r2, #0x60000
    ldrh r1, [r10, #0x44]
    mov r8, r2, lsr #0x11
    add r2, r3, r1, lsl #12
    ldr r1, [pc, #0x3c]
    add r3, r3, r6
    and r1, r3, r1
    orr r3, r1, r8, lsl #10
    and r1, r2, #0xf000
    orr r1, r3, r1
    strh r1, [r0, #4]
_494:
    add r4, r4, #1
    add r5, r5, #8
    ldrh r0, [r7]
    cmp r4, r0
    blo _280
    add sp, sp, #0x2c
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd gDisplayTable
    dcd gObjVramBaseTable
    dcd 0x000003ff
    dcd data_02072da0
    dcd 0x000001ff
    dcd data_02072d60
}

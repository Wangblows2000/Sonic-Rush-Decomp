// asm: C-impossible: printf switch-table density rule, every compiler tested
#include <nitro/types.h>

extern u32 data_02077f20;
extern u32 data_02077f24;
extern void func_0202a098(void);
extern void func_0202a1e0(void);
extern void func_0202ac68(void);
extern void func_0202b3f0(void);
extern void func_0202b7b0(void);
extern void func_0202ba98(void);
extern void func_0202bcfc(void);
extern void func_0202c2d8(void);
extern void func_0202c5f0(void);

asm void func_0202a3e8(void)
{
    stmdb sp!, {r0, r1, r2, r3}
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x24c
    mov r11, sp
    mov r3, #0x20
    str r2, [r11, #0xc]
    mov r2, r3
    strb r2, [r11, #0x34]
    ldr r2, [r11, #0xc]
    mov r7, r0
    ldrb r2, [r2]
    mov r0, #0
    str r3, [r11, #0x20]
    mov r6, r1
    cmp r2, #0
    str r0, [r11, #0x10]
    beq _864
    mov r0, #0x200
    str r0, [r11, #0x28]
    mov r0, #0x25
    str r0, [r11, #0x24]
    ldr r0, [r11, #0x10]
    mov r8, #1
    str r0, [r11, #0x2c]
    mov r0, #0x30
    str r0, [r11, #0x30]
_68:
    ldr r0, [r11, #0xc]
    ldr r1, [r11, #0x24]
    bl func_0202c2d8
    str r0, [r11, #0x1c]
    cmp r0, #0
    bne _c4
    ldr r0, [r11, #0xc]
    bl func_0202c5f0
    movs r2, r0
    ldr r0, [r11, #0x10]
    add r0, r0, r2
    str r0, [r11, #0x10]
    beq _864
    ldr r1, [r11, #0xc]
    mov r0, r6
    blx r7
    cmp r0, #0
    bne _864
    add sp, r11, #0x24c
    mvn r0, #0
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    add sp, sp, #0x10
    bx lr
_c4:
    mov r1, r0
    ldr r0, [r11, #0xc]
    subs r2, r1, r0
    ldr r0, [r11, #0x10]
    add r0, r0, r2
    str r0, [r11, #0x10]
    beq _104
    ldr r1, [r11, #0xc]
    mov r0, r6
    blx r7
    cmp r0, #0
    addeq sp, r11, #0x24c
    mvneq r0, #0
    ldmeqia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    addeq sp, sp, #0x10
    bxeq lr
_104:
    ldr r0, [r11, #0x1c]
    add r1, r11, #0x27c
    add r2, r11, #0x38
    bl func_0202bcfc
    ldrb r1, [r11, #0x3d]
    str r0, [r11, #0xc]
    cmp r1, #0x61
    bgt _174
    cmp r1, #0x61
    bge _4cc
    cmp r1, #0x47
    bgt _168
    subs r0, r1, #0x41
    addpl pc, pc, r0, lsl #2
    b _15c
    b _4cc
    b _6b0
    b _6b0
    b _6b0
    b _454
    b _454
    b _454
_15c:
    cmp r1, #0x25
    beq _69c
    b _6b0
_168:
    cmp r1, #0x58
    beq _328
    b _6b0
_174:
    cmp r1, #0x75
    bgt _1dc
    subs r0, r1, #0x64
    addpl pc, pc, r0, lsl #2
    b _1d0
    b _1f8
    b _454
    b _454
    b _454
    b _6b0
    b _1f8
    b _6b0
    b _6b0
    b _6b0
    b _6b0
    b _604
    b _328
    b _6b0
    b _6b0
    b _6b0
    b _544
    b _6b0
    b _328
_1d0:
    cmp r1, #0x63
    beq _67c
    b _6b0
_1dc:
    cmp r1, #0x78
    bgt _1f0
    cmp r1, #0x78
    beq _328
    b _6b0
_1f0:
    cmp r1, #0xff
    b _6b0
_1f8:
    ldrb r0, [r11, #0x3c]
    cmp r0, #3
    ldreq r1, [r11, #0x27c]
    addeq r1, r1, #4
    streq r1, [r11, #0x27c]
    ldreq r9, [r1, #-4]
    beq _290
    cmp r0, #7
    bne _23c
    ldr r1, [r11, #0x27c]
    add r2, r1, #8
    str r2, [r11, #0x27c]
    ldr r1, [r2, #-8]
    str r1, [r11, #0x14]
    ldr r1, [r2, #-4]
    str r1, [r11, #0x18]
    b _290
_23c:
    cmp r0, #4
    ldreq r1, [r11, #0x27c]
    addeq r1, r1, #8
    streq r1, [r11, #0x27c]
    ldreq r9, [r1, #-8]
    beq _290
    cmp r0, #5
    ldreq r1, [r11, #0x27c]
    addeq r1, r1, #4
    streq r1, [r11, #0x27c]
    ldreq r9, [r1, #-4]
    beq _290
    cmp r0, #6
    ldreq r1, [r11, #0x27c]
    addeq r1, r1, #4
    streq r1, [r11, #0x27c]
    ldreq r9, [r1, #-4]
    ldrne r1, [r11, #0x27c]
    addne r1, r1, #4
    strne r1, [r11, #0x27c]
    ldrne r9, [r1, #-4]
_290:
    cmp r0, #2
    moveq r1, r9, lsl #0x10
    moveq r9, r1, asr #0x10
    cmp r0, #1
    moveq r1, r9, lsl #0x18
    moveq r9, r1, asr #0x18
    cmp r0, #7
    bne _2e8
    sub r4, sp, #4
    mov sp, r4
    add r0, r11, #0x38
    ldmia r0, {r0, r1, r2, r3}
    stmia r4, {r0, r1, r2, r3}
    ldmia r4, {r3}
    add sp, r4, #4
    ldr r0, [r11, #0x14]
    ldr r1, [r11, #0x18]
    add r2, r11, #0x248
    bl func_0202b7b0
    movs r4, r0
    bne _318
    b _6b0
_2e8:
    sub r4, sp, #8
    mov sp, r4
    add r0, r11, #0x38
    ldmia r0, {r0, r1, r2, r3}
    stmia r4, {r0, r1, r2, r3}
    ldmia r4, {r2, r3}
    add sp, r4, #8
    mov r0, r9
    add r1, r11, #0x248
    bl func_0202ba98
    movs r4, r0
    beq _6b0
_318:
    add r0, r11, #0x200
    add r0, r0, #0x47
    sub r10, r0, r4
    b _704
_328:
    ldrb r0, [r11, #0x3c]
    cmp r0, #3
    ldreq r1, [r11, #0x27c]
    addeq r1, r1, #4
    streq r1, [r11, #0x27c]
    ldreq r9, [r1, #-4]
    beq _3c0
    cmp r0, #7
    bne _36c
    ldr r1, [r11, #0x27c]
    add r2, r1, #8
    str r2, [r11, #0x27c]
    ldr r1, [r2, #-8]
    str r1, [r11, #0x14]
    ldr r1, [r2, #-4]
    str r1, [r11, #0x18]
    b _3c0
_36c:
    cmp r0, #4
    ldreq r1, [r11, #0x27c]
    addeq r1, r1, #8
    streq r1, [r11, #0x27c]
    ldreq r9, [r1, #-8]
    beq _3c0
    cmp r0, #5
    ldreq r1, [r11, #0x27c]
    addeq r1, r1, #4
    streq r1, [r11, #0x27c]
    ldreq r9, [r1, #-4]
    beq _3c0
    cmp r0, #6
    ldreq r1, [r11, #0x27c]
    addeq r1, r1, #4
    streq r1, [r11, #0x27c]
    ldreq r9, [r1, #-4]
    ldrne r1, [r11, #0x27c]
    addne r1, r1, #4
    strne r1, [r11, #0x27c]
    ldrne r9, [r1, #-4]
_3c0:
    cmp r0, #2
    moveq r1, r9, lsl #0x10
    moveq r9, r1, lsr #0x10
    cmp r0, #1
    andeq r9, r9, #0xff
    cmp r0, #7
    bne _414
    sub r4, sp, #4
    mov sp, r4
    add r0, r11, #0x38
    ldmia r0, {r0, r1, r2, r3}
    stmia r4, {r0, r1, r2, r3}
    ldmia r4, {r3}
    add sp, r4, #4
    ldr r0, [r11, #0x14]
    ldr r1, [r11, #0x18]
    add r2, r11, #0x248
    bl func_0202b7b0
    movs r4, r0
    bne _444
    b _6b0
_414:
    sub r4, sp, #8
    mov sp, r4
    add r0, r11, #0x38
    ldmia r0, {r0, r1, r2, r3}
    stmia r4, {r0, r1, r2, r3}
    ldmia r4, {r2, r3}
    add sp, r4, #8
    mov r0, r9
    add r1, r11, #0x248
    bl func_0202ba98
    movs r4, r0
    beq _6b0
_444:
    add r0, r11, #0x200
    add r0, r0, #0x47
    sub r10, r0, r4
    b _704
_454:
    ldrb r0, [r11, #0x3c]
    cmp r0, #8
    ldrne r0, [r11, #0x27c]
    addne r0, r0, #8
    strne r0, [r11, #0x27c]
    ldrne r5, [r0, #-8]
    ldrne r4, [r0, #-4]
    bne _488
    ldr r0, [r11, #0x27c]
    add r0, r0, #8
    str r0, [r11, #0x27c]
    ldr r5, [r0, #-8]
    ldr r4, [r0, #-4]
_488:
    sub r10, sp, #4
    mov sp, r10
    add r0, r11, #0x38
    ldmia r0, {r0, r1, r2, r3}
    stmia r10, {r0, r1, r2, r3}
    ldmia r10, {r3}
    add sp, r10, #4
    mov r0, r5
    mov r1, r4
    add r2, r11, #0x248
    bl func_0202ac68
    movs r4, r0
    beq _6b0
    add r0, r11, #0x200
    add r0, r0, #0x47
    sub r10, r0, r4
    b _704
_4cc:
    ldrb r0, [r11, #0x3c]
    cmp r0, #8
    ldrne r0, [r11, #0x27c]
    addne r0, r0, #8
    strne r0, [r11, #0x27c]
    ldrne r5, [r0, #-8]
    ldrne r4, [r0, #-4]
    bne _500
    ldr r0, [r11, #0x27c]
    add r0, r0, #8
    str r0, [r11, #0x27c]
    ldr r5, [r0, #-8]
    ldr r4, [r0, #-4]
_500:
    sub r10, sp, #4
    mov sp, r10
    add r0, r11, #0x38
    ldmia r0, {r0, r1, r2, r3}
    stmia r10, {r0, r1, r2, r3}
    ldmia r10, {r3}
    add sp, r10, #4
    mov r0, r5
    mov r1, r4
    add r2, r11, #0x248
    bl func_0202b3f0
    movs r4, r0
    beq _6b0
    add r0, r11, #0x200
    add r0, r0, #0x47
    sub r10, r0, r4
    b _704
_544:
    ldrb r0, [r11, #0x3c]
    cmp r0, #9
    bne _584
    ldr r0, [r11, #0x27c]
    ldr r2, [r11, #0x28]
    add r0, r0, #4
    str r0, [r11, #0x27c]
    ldr r1, [r0, #-4]
    add r0, r11, #0x48
    cmp r1, #0
    ldreq r1, [pc, #0x304]
    bl func_0202a098
    cmp r0, #0
    blt _6b0
    add r4, r11, #0x48
    b _594
_584:
    ldr r0, [r11, #0x27c]
    add r0, r0, #4
    str r0, [r11, #0x27c]
    ldr r4, [r0, #-4]
_594:
    ldrb r0, [r11, #0x3b]
    cmp r4, #0
    ldreq r4, [pc, #0x2d8]
    cmp r0, #0
    beq _5c8
    ldrb r10, [r4], #1
    ldrb r0, [r11, #0x3a]
    cmp r0, #0
    beq _704
    ldr r0, [r11, #0x44]
    cmp r10, r0
    movgt r10, r0
    b _704
_5c8:
    ldrb r0, [r11, #0x3a]
    cmp r0, #0
    beq _5f4
    ldr r10, [r11, #0x44]
    ldr r1, [r11, #0x2c]
    mov r0, r4
    mov r2, r10
    bl func_0202a1e0
    cmp r0, #0
    subne r10, r0, r4
    b _704
_5f4:
    mov r0, r4
    bl func_0202c5f0
    mov r10, r0
    b _704
_604:
    ldr r0, [r11, #0x27c]
    ldrb r1, [r11, #0x3c]
    add r0, r0, #4
    str r0, [r11, #0x27c]
    ldr r0, [r0, #-4]
    cmp r1, #7
    addls pc, pc, r1, lsl #2
    b _854
    b _644
    b _854
    b _650
    b _65c
    b _854
    b _854
    b _854
    b _668
_644:
    ldr r1, [r11, #0x10]
    str r1, [r0]
    b _854
_650:
    ldr r1, [r11, #0x10]
    strh r1, [r0]
    b _854
_65c:
    ldr r1, [r11, #0x10]
    str r1, [r0]
    b _854
_668:
    ldr r1, [r11, #0x10]
    str r1, [r0]
    mov r1, r1, asr #0x1f
    str r1, [r0, #4]
    b _854
_67c:
    ldr r0, [r11, #0x27c]
    add r4, r11, #0x48
    add r0, r0, #4
    str r0, [r11, #0x27c]
    ldr r0, [r0, #-4]
    mov r10, r8
    strb r0, [r11, #0x48]
    b _704
_69c:
    ldr r0, [r11, #0x24]
    add r4, r11, #0x48
    strb r0, [r11, #0x48]
    mov r10, r8
    b _704
_6b0:
    ldr r0, [r11, #0x1c]
    bl func_0202c5f0
    movs r2, r0
    ldr r0, [r11, #0x10]
    add r0, r0, r2
    str r0, [r11, #0x10]
    beq _6f0
    ldr r1, [r11, #0x1c]
    mov r0, r6
    blx r7
    cmp r0, #0
    addeq sp, r11, #0x24c
    mvneq r0, #0
    ldmeqia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    addeq sp, sp, #0x10
    bxeq lr
_6f0:
    add sp, r11, #0x24c
    ldr r0, [r11, #0x10]
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    add sp, sp, #0x10
    bx lr
_704:
    mov r5, r10
    ldrb r0, [r11, #0x38]
    cmp r0, #0
    beq _7c0
    cmp r0, #2
    ldreq r0, [r11, #0x30]
    ldrne r0, [r11, #0x20]
    strb r0, [r11, #0x34]
    ldrb r0, [r4]
    cmp r0, #0x2b
    beq _740
    cmp r0, #0x2d
    beq _740
    cmp r0, #0x20
    bne _77c
_740:
    ldrb r0, [r11, #0x34]
    cmp r0, #0x30
    bne _77c
    mov r0, r6
    mov r1, r4
    mov r2, r8
    blx r7
    cmp r0, #0
    addeq sp, r11, #0x24c
    mvneq r0, #0
    ldmeqia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    addeq sp, sp, #0x10
    bxeq lr
    add r4, r4, #1
    sub r10, r10, #1
_77c:
    ldr r0, [r11, #0x40]
    cmp r5, r0
    bge _7c0
_788:
    mov r0, r6
    add r1, r11, #0x34
    mov r2, r8
    blx r7
    cmp r0, #0
    addeq sp, r11, #0x24c
    mvneq r0, #0
    ldmeqia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    addeq sp, sp, #0x10
    bxeq lr
    add r5, r5, #1
    ldr r0, [r11, #0x40]
    cmp r5, r0
    blt _788
_7c0:
    cmp r10, #0
    beq _7f0
    mov r1, r4
    mov r2, r10
    mov r0, r6
    blx r7
    cmp r0, #0
    addeq sp, r11, #0x24c
    mvneq r0, #0
    ldmeqia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    addeq sp, sp, #0x10
    bxeq lr
_7f0:
    ldrb r0, [r11, #0x38]
    cmp r0, #0
    bne _848
    ldr r0, [r11, #0x40]
    cmp r5, r0
    bge _848
_808:
    ldr r0, [r11, #0x20]
    add r1, r11, #0x35
    strb r0, [r11, #0x35]
    mov r0, r6
    mov r2, r8
    blx r7
    cmp r0, #0
    addeq sp, r11, #0x24c
    mvneq r0, #0
    ldmeqia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    addeq sp, sp, #0x10
    bxeq lr
    add r5, r5, #1
    ldr r0, [r11, #0x40]
    cmp r5, r0
    blt _808
_848:
    ldr r0, [r11, #0x10]
    add r0, r0, r5
    str r0, [r11, #0x10]
_854:
    ldr r0, [r11, #0xc]
    ldrb r0, [r0]
    cmp r0, #0
    bne _68
_864:
    ldr r0, [r11, #0x10]
    add sp, r11, #0x24c
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    add sp, sp, #0x10
    bx lr
    dcd data_02077f20
    dcd data_02077f24
}

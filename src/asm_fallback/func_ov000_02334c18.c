// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 gMapObjectCreateTable;
extern u32 data_ov000_023686e0;
extern u32 data_ov000_0236aa78;
extern u32 data_ov000_0236abd8;

asm void func_ov000_02334c18(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x3c
    ldr r4, [pc, #0x278]
    mov r8, r3
    ldrh r5, [r4, #0xe]
    mov r4, r1, lsl #8
    ldr r0, [r0, #0x1c]
    sub r3, r5, #1
    mov r3, r3, asr #8
    add r3, r3, #1
    mla r1, r2, r3, r1
    mov r1, r1, lsl #0x10
    add r1, r0, r1, lsr #14
    ldr r3, [r1, #4]
    ldr r7, [sp, #0x60]
    ldrh r1, [r0, r3]
    add r0, r0, r3
    add r6, r0, #2
    movs r0, r1
    str r1, [sp]
    mov r1, #0
    ldr r5, [pc, #0x230]
    mov r11, r2, lsl #8
    ldr r0, [sp, #0x64]
    str r1, [sp, #4]
    addeq sp, sp, #0x3c
    ldmeqia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxeq lr
    and r1, r0, #8
    str r1, [sp, #8]
    and r1, r0, #0x10
    str r1, [sp, #0xc]
    and r1, r0, #0x20
    str r1, [sp, #0x10]
    and r1, r0, #0x40
    str r1, [sp, #0x14]
    and r1, r0, #0x80
    str r1, [sp, #0x18]
    and r1, r0, #1
    and r0, r0, #4
    str r0, [sp, #0x20]
    ldr r0, [sp, #4]
    str r1, [sp, #0x1c]
    str r0, [sp, #0x28]
    str r0, [sp, #0x2c]
    str r0, [sp, #0x30]
    str r0, [sp, #0x34]
    str r0, [sp, #0x38]
_c0:
    ldr r0, [sp, #8]
    cmp r0, #0
    beq _d8
    ldrh r0, [r6, #4]
    ands r0, r0, #0x4000
    beq _258
_d8:
    ldrb r1, [r6]
    cmp r1, #0xff
    beq _258
    ldr r0, [sp, #0xc]
    ldr r10, [sp, #0x1c]
    cmp r0, #0
    ldrb r0, [r6, #1]
    add r9, r1, r4
    str r0, [sp, #0x24]
    add lr, r0, r11
    ldrne r0, [sp, #0x28]
    ldreqh r0, [r6, #2]
    ldreqb r0, [r5, r0]
    mov r0, r0, lsl #0x10
    mov r12, r0, lsr #0x10
    ldr r0, [sp, #0x10]
    cmp r0, #0
    ldrne r0, [sp, #0x2c]
    ldreqh r0, [r6, #2]
    ldreqb r0, [r5, r0]
    mov r0, r0, lsl #0x10
    mov r3, r0, lsr #0x10
    ldr r0, [sp, #0x14]
    cmp r0, #0
    ldrne r0, [sp, #0x30]
    ldreqh r0, [r6, #2]
    ldreqb r0, [r5, r0]
    mov r0, r0, lsl #0x10
    mov r2, r0, lsr #0x10
    ldr r0, [sp, #0x18]
    cmp r0, #0
    ldrne r0, [sp, #0x34]
    ldreqh r0, [r6, #2]
    ldreqb r0, [r5, r0]
    cmp r10, #0
    mov r0, r0, lsl #0x10
    mov r0, r0, lsr #0x10
    beq _1b0
    ldr r10, [r8]
    add r10, r10, r12
    cmp r9, r10
    blo _258
    ldr r10, [r8, #8]
    sub r2, r10, r2
    cmp r9, r2
    bhi _258
    ldr r2, [r8, #4]
    add r2, r2, r3
    cmp lr, r2
    blo _258
    ldr r2, [r8, #0xc]
    sub r0, r2, r0
    cmp lr, r0
    bhi _258
_1b0:
    ldr r0, [sp, #0x20]
    cmp r0, #0
    beq _21c
    ldr r0, [pc, #0xcc]
    ldr r2, [r7]
    ldrh r3, [r0]
    ldrh r0, [r6, #2]
    ldrb r0, [r5, r0]
    sub r3, r3, r0
    ldr r0, [pc, #0xb8]
    smull r12, r10, r0, r3
    add r10, r10, r3, lsr #31
    sub r0, r2, r10
    cmp r9, r0
    bls _21c
    ldr r0, [r7, #8]
    add r0, r0, r10
    cmp r9, r0
    bhs _21c
    ldr r0, [r7, #4]
    sub r0, r0, r10
    cmp lr, r0
    bls _21c
    ldr r0, [r7, #0xc]
    add r0, r0, r10
    cmp lr, r0
    blo _258
_21c:
    ldrh r2, [r6, #2]
    cmp r2, #0xf2
    bhs _258
    ldr r0, [pc, #0x68]
    ldr r9, [r0, r2, lsl #2]
    cmp r9, #0
    beq _258
    add r0, r4, r1
    mov r1, r0, lsl #8
    ldr r0, [sp, #0x24]
    ldr r3, [sp, #0x38]
    add r0, r11, r0
    mov r2, r0, lsl #8
    mov r0, r6
    blx r9
_258:
    ldr r0, [sp, #4]
    add r6, r6, #0xc
    add r0, r0, #1
    mov r0, r0, lsl #0x10
    mov r1, r0, lsr #0x10
    ldr r0, [sp]
    str r1, [sp, #4]
    cmp r1, r0
    blo _c0
    add sp, sp, #0x3c
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd data_ov000_0236abd8
    dcd data_ov000_023686e0
    dcd data_ov000_0236aa78
    dcd 0x55555556
    dcd gMapObjectCreateTable
}

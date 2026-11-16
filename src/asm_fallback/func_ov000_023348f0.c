// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 gDecorationCreateTable;
extern u32 data_ov000_023685ec;
extern u32 data_ov000_0236abd8;

asm void func_ov000_023348f0(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x3c
    ldr r4, [pc, #0x228]
    mov r8, r3
    ldrh r4, [r4, #0xe]
    mov r5, r1, lsl #8
    ldr r0, [r0, #0x24]
    sub r3, r4, #1
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
    mov r0, #0
    str r1, [sp]
    ldr r11, [pc, #0x1e0]
    mov r4, r2, lsl #8
    ldr r1, [sp, #0x64]
    str r0, [sp, #4]
    addeq sp, sp, #0x3c
    ldmeqia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxeq lr
    and r0, r1, #0x10
    str r0, [sp, #8]
    and r0, r1, #0x20
    str r0, [sp, #0xc]
    and r0, r1, #0x40
    str r0, [sp, #0x10]
    and r0, r1, #0x80
    str r0, [sp, #0x14]
    and r0, r1, #1
    str r0, [sp, #0x18]
    and r0, r1, #4
    str r0, [sp, #0x1c]
    ldr r0, [sp, #4]
    str r0, [sp, #0x24]
    str r0, [sp, #0x28]
    str r0, [sp, #0x2c]
    str r0, [sp, #0x30]
    str r0, [sp, #0x34]
_b8:
    ldrb r1, [r6]
    cmp r1, #0xff
    beq _208
    ldr r0, [sp, #8]
    ldr r10, [sp, #0x18]
    cmp r0, #0
    ldrb r0, [r6, #1]
    add r9, r1, r5
    str r0, [sp, #0x20]
    add lr, r0, r4
    ldrne r0, [sp, #0x24]
    ldreqh r0, [r6, #2]
    ldreqb r0, [r11, r0]
    mov r0, r0, lsl #0x10
    mov r12, r0, lsr #0x10
    ldr r0, [sp, #0xc]
    cmp r0, #0
    ldrne r0, [sp, #0x28]
    ldreqh r0, [r6, #2]
    ldreqb r0, [r11, r0]
    mov r0, r0, lsl #0x10
    mov r3, r0, lsr #0x10
    ldr r0, [sp, #0x10]
    cmp r0, #0
    ldrne r0, [sp, #0x2c]
    ldreqh r0, [r6, #2]
    ldreqb r0, [r11, r0]
    mov r0, r0, lsl #0x10
    mov r2, r0, lsr #0x10
    ldr r0, [sp, #0x14]
    cmp r0, #0
    ldrne r0, [sp, #0x30]
    ldreqh r0, [r6, #2]
    ldreqb r0, [r11, r0]
    cmp r10, #0
    mov r0, r0, lsl #0x10
    mov r0, r0, lsr #0x10
    beq _190
    ldr r10, [r8]
    add r10, r10, r12
    cmp r9, r10
    blo _208
    ldr r10, [r8, #8]
    sub r2, r10, r2
    cmp r9, r2
    bhi _208
    ldr r2, [r8, #4]
    add r2, r2, r3
    cmp lr, r2
    blo _208
    ldr r2, [r8, #0xc]
    sub r0, r2, r0
    cmp lr, r0
    bhi _208
_190:
    ldr r0, [sp, #0x1c]
    cmp r0, #0
    beq _1cc
    ldr r0, [r7]
    cmp r9, r0
    bls _1cc
    ldr r0, [r7, #8]
    cmp r9, r0
    bhs _1cc
    ldr r0, [r7, #4]
    cmp lr, r0
    bls _1cc
    ldr r0, [r7, #0xc]
    cmp lr, r0
    blo _208
_1cc:
    ldrh r2, [r6, #2]
    cmp r2, #0xf2
    bhs _208
    ldr r0, [pc, #0x60]
    ldr r9, [r0, r2, lsl #2]
    cmp r9, #0
    beq _208
    add r0, r5, r1
    mov r1, r0, lsl #8
    ldr r0, [sp, #0x20]
    ldr r3, [sp, #0x34]
    add r0, r4, r0
    mov r2, r0, lsl #8
    mov r0, r6
    blx r9
_208:
    ldr r0, [sp, #4]
    add r6, r6, #4
    add r0, r0, #1
    mov r0, r0, lsl #0x10
    mov r1, r0, lsr #0x10
    ldr r0, [sp]
    str r1, [sp, #4]
    cmp r1, r0
    blo _b8
    add sp, sp, #0x3c
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd data_ov000_0236abd8
    dcd data_ov000_023685ec
    dcd gDecorationCreateTable
}

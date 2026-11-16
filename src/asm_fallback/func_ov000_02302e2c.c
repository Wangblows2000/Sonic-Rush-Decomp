// asm: fallback — C attempts did not match (best C draft: /home/claude/w_ov000q/func_ov000_02302e2c.c)
#include <nitro/types.h>

extern u32 data_ov000_0236ac60;

asm void func_ov000_02302e2c(void)
{
    stmdb sp!, {r4, r5, r6, lr}
    add r1, r0, r1
    add r1, r1, #0x200
    ldrsb r4, [r1, #0x3c]
    ldr r1, [r0, #0x1e0]
    ldr r2, [pc, #0xcc]
    ldrsb r3, [r1, #6]
    ldr r2, [r2, r4, lsl #2]
    ldr r4, [r0, #0x10]
    mov r12, #0
    ldr r5, [r2, #0x10]
    add lr, r4, r3, lsl #8
    mov r3, r12
    mov r6, r12
    cmp lr, r5
    bgt _50
    ldrb r4, [r1, #8]
    add r4, lr, r4, lsl #8
    cmp r4, r5
    movge r6, #1
_50:
    cmp r6, #0
    beq _8c
    ldrsb lr, [r1, #7]
    ldr r0, [r0, #0x14]
    ldr r4, [r2, #0x14]
    add lr, r0, lr, lsl #8
    cmp lr, r4
    mov r5, #0
    bgt _84
    ldrb r0, [r1, #9]
    add r0, lr, r0, lsl #8
    cmp r0, r4
    movge r5, #1
_84:
    cmp r5, #0
    movne r3, #1
_8c:
    cmp r3, #0
    beq _dc
    ldr r0, [r2, #0x54]
    mov r4, #1
    ands r2, r0, #1
    mov r3, #0
    beq _b4
    ldrh r0, [r1, #4]
    ands r0, r0, #0x20
    movne r3, r4
_b4:
    cmp r3, #0
    bne _d4
    cmp r2, #0
    bne _d0
    ldrh r0, [r1, #4]
    ands r0, r0, #0x10
    bne _d4
_d0:
    mov r4, #0
_d4:
    cmp r4, #0
    movne r12, #1
_dc:
    mov r0, r12
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
    dcd data_ov000_0236ac60
}

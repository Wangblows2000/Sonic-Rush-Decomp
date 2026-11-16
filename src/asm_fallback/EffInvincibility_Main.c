// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 data_0206e470;
extern void EffInvincibilitySparkle_Create(void);

asm void EffInvincibility_Main(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, lr}
    mov r6, r0
    ldr r5, [r6, #0x88]
    cmp r5, #0
    ldmeqia sp!, {r4, r5, r6, r7, r8, lr}
    bxeq lr
    ldr r2, [r6, #0xc]
    ands r0, r2, #7
    bne _d0
    ldr r1, [pc, #0xd0]
    mov r0, r2, lsl #0x1b
    and r1, r1, r0, lsr #16
    mov r1, r1, asr #4
    mov r2, r1, lsl #1
    add r1, r2, #1
    ldr r4, [pc, #0xbc]
    mov r1, r1, lsl #1
    mov r2, r2, lsl #1
    ldrsh r3, [r4, r1]
    mov r1, #0x18
    ldrsh r2, [r4, r2]
    smulbb r7, r3, r1
    smulbb r3, r2, r1
    ldr r2, [r5, #0x10]
    ldr r1, [r5, #0x14]
    mov r4, r0, lsr #0x10
    mov r7, r7, lsl #4
    mov r0, r2, asr #8
    add r0, r0, r7, asr #16
    mov r0, r0, lsl #0x10
    mov r8, r3, lsl #4
    mov r1, r1, asr #8
    add r1, r1, r8, asr #16
    mov r1, r1, lsl #0x10
    mov r2, r4
    mov r0, r0, asr #0x10
    mov r1, r1, asr #0x10
    bl EffInvincibilitySparkle_Create
    ldr r1, [r5, #0x10]
    ldr r0, [r5, #0x14]
    mov r1, r1, asr #8
    mov r0, r0, asr #8
    sub r3, r1, r7, asr #16
    sub r1, r0, r8, asr #16
    add r2, r4, #0x8000
    mov r0, r3, lsl #0x10
    mov r1, r1, lsl #0x10
    mov r2, r2, lsl #0x10
    mov r0, r0, asr #0x10
    mov r1, r1, asr #0x10
    mov r2, r2, lsr #0x10
    bl EffInvincibilitySparkle_Create
_d0:
    ldr r1, [r6, #0xc]
    add r0, r5, #0x300
    add r1, r1, #1
    str r1, [r6, #0xc]
    ldrsh r0, [r0, #0xb4]
    cmp r0, #0
    ldreq r0, [r6, #0x54]
    orreq r0, r0, #4
    streq r0, [r6, #0x54]
    ldmia sp!, {r4, r5, r6, r7, r8, lr}
    bx lr
    dcd 0x0000ffff
    dcd data_0206e470
}

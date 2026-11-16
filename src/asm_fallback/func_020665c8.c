// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 gMapCollisionMap;

asm void func_020665c8(void)
{
    stmdb sp!, {r4, r5, r6, lr}
    ldr r12, [pc, #0x60]
    mov r3, r1, asr #3
    ldr r1, [r12]
    mov r12, r3, asr #2
    ldrh lr, [r1, #0x18]
    add r12, r3, r12, lsr #29
    mov r4, r12, asr #3
    mul r12, lr, r4
    mov r6, r0, asr #3
    mov r0, r6, asr #2
    add r0, r6, r0, lsr #29
    add lr, r1, r2, lsl #2
    add r2, r12, r0, asr #3
    mov r5, r0, asr #3
    ldr r12, [lr, #0xc]
    mov r0, r2, lsl #1
    sub lr, r6, r5, lsl #3
    sub r2, r3, r4, lsl #3
    add r2, lr, r2, lsl #3
    ldrh r0, [r12, r0]
    mov r2, r2, lsl #1
    ldr r1, [r1, #8]
    add r0, r2, r0, lsl #7
    ldrh r0, [r1, r0]
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
    dcd gMapCollisionMap
}

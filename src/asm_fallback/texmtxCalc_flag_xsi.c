// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void FX_DivAsync(void);
extern void FX_GetDivResult(void);

asm void texmtxCalc_flag_xsi(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #4
    mov r6, r1
    ldrh r2, [r6, #0x2c]
    ldrh r1, [r6, #0x2e]
    mov r7, r0
    mov r10, r2, lsl #0xc
    mov r11, r1, lsl #0xc
    mov r0, r11
    mov r1, r10
    bl FX_DivAsync
    ldrsh r3, [r6, #0x22]
    ldr r0, [r6, #0x18]
    ldrsh r9, [r6, #0x20]
    smull r2, r1, r0, r3
    mov r2, r2, lsr #0xc
    ldr r8, [r6, #0x1c]
    orr r2, r2, r1, lsl #20
    str r2, [r7]
    smull r2, r1, r8, r3
    mov r4, r2, lsr #0xc
    orr r4, r4, r1, lsl #20
    smull r2, r1, r0, r9
    mov r5, r2, lsr #0xc
    orr r5, r5, r1, lsl #20
    str r4, [r7, #0x14]
    bl FX_GetDivResult
    smull r2, r1, r8, r9
    mov r2, r2, lsr #0xc
    orr r2, r2, r1, lsl #20
    mul r0, r2, r0
    mov r0, r0, asr #0xc
    str r0, [r7, #4]
    mov r0, r10
    mov r1, r11
    bl FX_DivAsync
    ldr lr, [r6, #0x1c]
    ldrsh r2, [r6, #0x20]
    ldr r8, [r6, #0x24]
    ldr r0, [r6, #0x28]
    smull r10, r9, r8, r2
    ldrh r1, [r6, #0x2c]
    smull r3, r2, r0, r2
    ldrsh r12, [r6, #0x22]
    str r1, [sp]
    mov r11, lr, asr #0x1f
    smlal r3, r2, r8, r12
    smull r8, r12, r0, r12
    subs r8, r10, r8
    sbc r0, r9, r12
    mov r9, r8, lsr #0xc
    mov r12, r3, lsr #0xc
    orr r9, r9, r0, lsl #20
    mov r3, r2, asr #0xc
    orr r12, r12, r2, lsl #20
    umull r10, r2, r9, lr
    mla r2, r9, r11, r2
    mov r8, r0, asr #0xc
    mla r2, r8, lr, r2
    ldr r1, [r6, #0x18]
    mov r8, r10, lsr #0xc
    orr r8, r8, r2, lsl #20
    add r2, r4, r8
    mov r0, r1, asr #0x1f
    umull r8, r4, r12, r1
    mla r4, r12, r0, r4
    mla r4, r3, r1, r4
    mov r0, r8, lsr #0xc
    orr r0, r0, r4, lsl #20
    sub r1, r5, r0
    ldr r0, [sp]
    sub r2, r2, #0x1000
    mul r1, r0, r1
    mov r0, r1, lsl #4
    str r0, [r7, #0x30]
    ldrh r0, [r6, #0x2e]
    rsb r0, r0, #0
    mul r1, r0, r2
    mov r0, r1, lsl #4
    str r0, [r7, #0x34]
    bl FX_GetDivResult
    rsb r1, r5, #0
    mul r0, r1, r0
    mov r0, r0, asr #0xc
    str r0, [r7, #0x10]
    add sp, sp, #4
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
}

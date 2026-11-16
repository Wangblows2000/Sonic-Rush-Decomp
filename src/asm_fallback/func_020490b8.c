// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

asm void func_020490b8(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, lr}
    sub sp, sp, #4
    ldr r6, [sp, #0x20]
    and r4, r2, #0x1f
    mul r5, r4, r6
    sub r4, r3, r6
    and r7, r1, #0x1f
    mla r8, r7, r4, r5
    ldr r12, [pc, #0x104]
    mov lr, #0
    ldr r7, [pc, #0x100]
    strh lr, [r12]
    ldr r5, [pc, #0xfc]
    str r8, [r7]
    str r3, [r5]
    str lr, [r5, #4]
_40:
    ldrh r5, [r12]
    ands r5, r5, #0x8000
    bne _40
    and r5, r2, #0x3e0
    mov r5, r5, asr #5
    mul r7, r5, r6
    and r5, r1, #0x3e0
    mov r5, r5, asr #5
    ldr r8, [pc, #0xd0]
    mla r9, r5, r4, r7
    ldrsh r7, [r8]
    mov r5, #0
    ldr r8, [pc, #0xb8]
    strh r5, [r12]
    mov r5, r7, lsl #0x10
    ldr r7, [pc, #0xb0]
    str r9, [r8]
    str r3, [r7]
    str lr, [r7, #4]
    mov r5, r5, lsr #0x10
    ldr r12, [pc, #0x94]
_94:
    ldrh r7, [r12]
    ands r7, r7, #0x8000
    bne _94
    and r2, r2, #0x7c00
    mov r2, r2, asr #0xa
    mul r6, r2, r6
    and r1, r1, #0x7c00
    mov r1, r1, asr #0xa
    ldr r2, [pc, #0x7c]
    mla r6, r1, r4, r6
    ldrsh r2, [r2]
    mov r1, #0
    ldr r4, [pc, #0x64]
    strh r1, [r12]
    mov r1, r2, lsl #0x10
    ldr r2, [pc, #0x5c]
    str r6, [r4]
    str r3, [r2]
    str lr, [r2, #4]
    mov r3, r1, lsr #0x10
    ldr r2, [pc, #0x40]
_e8:
    ldrh r1, [r2]
    ands r1, r1, #0x8000
    bne _e8
    ldr r2, [pc, #0x3c]
    mov r1, r3, lsl #5
    ldrsh r4, [r2]
    and r3, r5, #0x1f
    and r2, r1, #0x3e0
    mov r1, r4, lsl #0x10
    mov r1, r1, lsr #6
    and r4, r1, #0x7c00
    orr r1, r3, r2
    orr r1, r4, r1
    strh r1, [r0]
    add sp, sp, #4
    ldmia sp!, {r4, r5, r6, r7, r8, r9, lr}
    bx lr
    dcd 0x04000280
    dcd 0x04000290
    dcd 0x04000298
    dcd 0x040002a0
}

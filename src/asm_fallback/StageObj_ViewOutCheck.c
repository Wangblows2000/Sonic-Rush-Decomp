// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 data_0207cf80;
extern u32 gObjManagerFlags;
extern u32 gCamera;

asm void StageObj_ViewOutCheck(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, lr}
    sub sp, sp, #4
    ldr r4, [pc, #0x21c]
    mov r6, #0x100
    ldrsh r4, [r4]
    mov r9, #0xc0
    ldr r12, [sp, #0x20]
    cmp r4, #0x100
    rsbne r4, r4, #0x200
    movne r4, r4, lsl #0x10
    movne r6, r4, asr #0x10
    ldr r4, [pc, #0x1f8]
    ldrsh r4, [r4, #2]
    cmp r4, #0x100
    rsbne r5, r4, #0x200
    movne r4, #0xc0
    mulne r4, r5, r4
    movne r4, r4, lsl #8
    movne r9, r4, asr #0x10
    ldr r4, [pc, #0x1dc]
    cmp r2, #0
    ldr r5, [r4]
    moveq r2, #0x40
    ands r4, r5, #4
    addeq sp, sp, #4
    moveq r0, #0
    ldmeqia sp!, {r4, r5, r6, r7, r8, r9, lr}
    bxeq lr
    ands r4, r5, #0x200
    beq _140
    ldr r7, [pc, #0x1b4]
    ldr r5, [r7, #0x6c]
    ldr r4, [r7, #8]
    cmp r4, r5
    bge _b8
    mov r5, r5, asr #8
    ldr r7, [r7, #4]
    sub r5, r5, r4, asr #8
    mov r8, r7, asr #8
    mov r7, r4, asr #8
    add r4, r9, r5
    sub r8, r8, r2
    sub r7, r7, r2
    add r5, r6, r2, lsl #1
    add r4, r4, r2, lsl #1
    b _e0
_b8:
    mov r4, r4, asr #8
    ldr r7, [r7, #0x68]
    sub r4, r4, r5, asr #8
    mov r7, r7, asr #8
    mov r5, r5, asr #8
    add r4, r9, r4
    sub r8, r7, r2
    sub r7, r5, r2
    add r5, r6, r2, lsl #1
    add r4, r4, r2, lsl #1
_e0:
    ldrsh r9, [sp, #0x28]
    ldrsh r6, [sp, #0x24]
    add r2, r8, r3
    sub r8, r9, r12
    sub r6, r6, r3
    mov r3, r0, asr #8
    cmp r2, r3
    add r7, r7, r12
    add r4, r4, r8
    add r0, r5, r6
    bgt _21c
    add r0, r2, r0
    cmp r0, r3
    blt _21c
    mov r1, r1, asr #8
    cmp r7, r1
    bgt _21c
    add r0, r7, r4
    cmp r0, r1
    blt _21c
    add sp, sp, #4
    mov r0, #0
    ldmia sp!, {r4, r5, r6, r7, r8, r9, lr}
    bx lr
_140:
    ldr r5, [pc, #0xec]
    ldrsh r4, [sp, #0x28]
    ldr r8, [r5, #4]
    ldr r7, [r5, #8]
    ldrsh r5, [sp, #0x24]
    mov r8, r8, asr #8
    sub r8, r8, r2
    mov r7, r7, asr #8
    sub r7, r7, r2
    add r6, r6, r2, lsl #1
    sub lr, r5, r3
    add r5, r9, r2, lsl #1
    sub r9, r4, r12
    add r4, r8, r3
    mov r8, r0, asr #8
    cmp r4, r8
    add r0, r7, r12
    add r5, r5, r9
    add r6, r6, lr
    bgt _1c0
    add r4, r4, r6
    cmp r4, r8
    blt _1c0
    mov r4, r1, asr #8
    cmp r0, r4
    bgt _1c0
    add r0, r0, r5
    cmp r0, r4
    addge sp, sp, #4
    movge r0, #0
    ldmgeia sp!, {r4, r5, r6, r7, r8, r9, lr}
    bxge lr
_1c0:
    ldr r0, [pc, #0x6c]
    ldr r4, [r0, #0x68]
    ldr r0, [r0, #0x6c]
    mov r4, r4, asr #8
    sub r4, r4, r2
    mov r0, r0, asr #8
    sub r2, r0, r2
    add r0, r4, r3
    cmp r0, r8
    add r2, r2, r12
    bgt _21c
    add r0, r0, r6
    cmp r0, r8
    blt _21c
    mov r1, r1, asr #8
    cmp r2, r1
    bgt _21c
    add r0, r2, r5
    cmp r0, r1
    addge sp, sp, #4
    movge r0, #0
    ldmgeia sp!, {r4, r5, r6, r7, r8, r9, lr}
    bxge lr
_21c:
    mov r0, #1
    add sp, sp, #4
    ldmia sp!, {r4, r5, r6, r7, r8, r9, lr}
    bx lr
    dcd data_0207cf80
    dcd gObjManagerFlags
    dcd gCamera
}

// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

asm void ExtraBoss_Unk_ov001_022fe550(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #4
    ldr r4, [r0, #0x20]
    ldr r3, [r0, #0x14]
    ldr r5, [r1, #0x20]
    ldr r2, [r1, #0x14]
    ldr r8, [r0, #0x30]
    sub r7, r3, r4
    add r6, r3, r4
    ldr r4, [r1, #0x30]
    sub r3, r2, r5
    add r2, r2, r5
    add r7, r8, r7
    add r5, r4, r3
    cmp r7, r5
    add r3, r8, r6
    add r2, r4, r2
    bge _5c
    cmp r3, r5
    addlt sp, sp, #4
    movlt r0, #0
    ldmltia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxlt lr
_5c:
    cmp r5, r7
    bge _78
    cmp r2, r7
    addlt sp, sp, #4
    movlt r0, #0
    ldmltia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxlt lr
_78:
    ldr r5, [r0, #0x18]
    ldr r4, [r0, #0xc]
    ldr r3, [r1, #0x18]
    ldr r2, [r1, #0xc]
    ldr r8, [r0, #0x28]
    sub r7, r4, r5
    add r4, r4, r5
    ldr r11, [r0, #0x1c]
    ldr r10, [r0, #0x10]
    ldr lr, [r1, #0x28]
    sub r12, r2, r3
    add r9, r2, r3
    ldr r3, [r1, #0x1c]
    ldr r2, [r1, #0x10]
    ldr r6, [r0, #0x2c]
    sub r5, r10, r11
    add r0, r10, r11
    ldr r11, [r1, #0x2c]
    sub r10, r2, r3
    add r1, r2, r3
    add r7, r8, r7
    add r2, lr, r12
    cmp r7, r2
    add r3, r6, r5
    add r4, r8, r4
    add r8, r6, r0
    add r6, r11, r10
    add r5, lr, r9
    add r0, r11, r1
    bge _104
    cmp r4, r2
    addlt sp, sp, #4
    movlt r0, #0
    ldmltia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxlt lr
_104:
    cmp r2, r7
    bge _120
    cmp r5, r7
    addlt sp, sp, #4
    movlt r0, #0
    ldmltia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxlt lr
_120:
    cmp r3, r6
    bge _13c
    cmp r8, r6
    addlt sp, sp, #4
    movlt r0, #0
    ldmltia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxlt lr
_13c:
    cmp r6, r3
    bge _158
    cmp r0, r3
    addlt sp, sp, #4
    movlt r0, #0
    ldmltia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxlt lr
_158:
    mov r0, #1
    add sp, sp, #4
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
}

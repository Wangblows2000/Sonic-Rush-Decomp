// asm: fallback — C attempts did not match (best C draft: /home/claude/w_ov000r/xbest_func_ov000_023369bc.c)
#include <nitro/types.h>

extern u32 data_022c4560;
extern void func_02032c8c(void);
extern void func_020332c4(void);
extern void func_02033544(void);

asm void func_ov000_023369bc(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x14
    add r1, r0, #0x200
    ldrsh r3, [r1, #0x98]
    ldrsh r4, [r0, #4]
    ldrsh r2, [r0, #6]
    ldrsh r1, [r1, #0x9a]
    add r3, r4, r3
    add r3, r3, #0xa8
    add r1, r2, r1
    add r2, r1, #0x12
    mov r1, r3, lsl #0x10
    mov r1, r1, asr #0x10
    str r1, [sp]
    ldr r1, [r0, #0x10]
    ldr r4, [r0, #0x67c]
    ldr r3, [pc, #0x2e0]
    mov r2, r2, lsl #0x10
    cmp r4, r3
    add r6, r0, #0x298
    mov r4, r2, asr #0x10
    ldr r11, [r0, #0xc]
    str r1, [sp, #4]
    mov r5, #0
    blo _7c
    ldr r1, [pc, #0x2c0]
    ldr r1, [r1, #0x20]
    ands r1, r1, #8
    ldrne r11, [r0, #0x14]
    ldrne r0, [r0, #0x18]
    strne r0, [sp, #4]
_7c:
    mov r9, #0
    ldr r8, [sp]
    str r9, [sp, #8]
_88:
    mov r0, r9, lsl #0x12
    ldr r1, [r6, #0x3e8]
    mov r0, r0, lsr #0x10
    mov r0, r1, lsr r0
    and r0, r0, #0xf
    mov r0, r0, lsl #0x10
    mov r7, r0, lsr #0x10
    sub r0, r5, #0xa
    mov r1, r0, lsl #0x10
    add r3, r6, #0x1f4
    mov r0, #0x7c
    mla r10, r9, r0, r3
    mov r2, r7, lsl #0x10
    mov r5, r1, asr #0x10
    mov r0, r10
    mov r1, r2, lsr #0x10
    sub r8, r8, #0xa
    bl func_02033544
    add r0, r11, r7, lsl #7
    str r0, [r10, #0x5c]
    ldr r0, [sp, #4]
    ldr r1, [sp, #8]
    add r0, r0, r7, lsl #7
    str r0, [r10, #0x60]
    mov r0, r10
    mov r2, r1
    bl func_020332c4
    strh r8, [r10, #0x48]
    strh r4, [r10, #0x4a]
    ldrsh r1, [r10, #0x48]
    mov r0, r10
    strh r1, [r10, #0x4c]
    ldrsh r1, [r10, #0x4a]
    sub r1, r1, #0xc0
    strh r1, [r10, #0x4e]
    bl func_02032c8c
    add r0, r9, #1
    mov r0, r0, lsl #0x10
    mov r9, r0, lsr #0x10
    cmp r9, #2
    blo _88
    sub r0, r5, #8
    mov r2, r0, lsl #0x10
    ldr r1, [sp]
    add r0, r6, #0x2ec
    add r1, r1, r2, asr #16
    strh r1, [r0, #0x48]
    strh r4, [r0, #0x4a]
    ldrsh r1, [r0, #0x48]
    strh r1, [r0, #0x4c]
    ldrsh r1, [r0, #0x4a]
    sub r1, r1, #0xc0
    strh r1, [r0, #0x4e]
    bl func_02032c8c
    sub r0, r5, #6
    mov r1, r0, lsl #0x10
    ldr r0, [sp]
    mov r8, #0
    mov r5, r1, asr #0x10
    add r10, r0, r1, asr #16
    str r8, [sp, #0xc]
_17c:
    add r0, r8, #2
    mov r0, r0, lsl #0x12
    ldr r1, [r6, #0x3e8]
    mov r0, r0, lsr #0x10
    mov r0, r1, lsr r0
    and r0, r0, #0xf
    mov r0, r0, lsl #0x10
    mov r9, r0, lsr #0x10
    sub r0, r5, #0xa
    mov r1, r0, lsl #0x10
    add r3, r6, #0xfc
    mov r0, #0x7c
    mla r7, r8, r0, r3
    mov r2, r9, lsl #0x10
    mov r5, r1, asr #0x10
    mov r0, r7
    mov r1, r2, lsr #0x10
    sub r10, r10, #0xa
    bl func_02033544
    add r0, r11, r9, lsl #7
    str r0, [r7, #0x5c]
    ldr r0, [sp, #4]
    ldr r1, [sp, #0xc]
    add r0, r0, r9, lsl #7
    str r0, [r7, #0x60]
    mov r0, r7
    mov r2, r1
    bl func_020332c4
    strh r10, [r7, #0x48]
    strh r4, [r7, #0x4a]
    ldrsh r1, [r7, #0x48]
    mov r0, r7
    strh r1, [r7, #0x4c]
    ldrsh r1, [r7, #0x4a]
    sub r1, r1, #0xc0
    strh r1, [r7, #0x4e]
    bl func_02032c8c
    add r0, r8, #1
    mov r0, r0, lsl #0x10
    mov r8, r0, lsr #0x10
    cmp r8, #2
    blo _17c
    sub r0, r5, #8
    mov r2, r0, lsl #0x10
    ldr r1, [sp]
    add r0, r6, #0x368
    add r1, r1, r2, asr #16
    strh r1, [r0, #0x48]
    strh r4, [r0, #0x4a]
    ldrsh r1, [r0, #0x48]
    strh r1, [r0, #0x4c]
    ldrsh r1, [r0, #0x4a]
    sub r1, r1, #0xc0
    strh r1, [r0, #0x4e]
    bl func_02032c8c
    ldr r0, [r6, #0x3e8]
    sub r1, r5, #6
    mov r0, r0, lsr #0x10
    and r0, r0, #0xf
    mov r0, r0, lsl #0x10
    mov r8, r0, lsr #0x10
    mov r2, r1, lsl #0x10
    ldr r1, [sp]
    add r7, r6, #0x80
    mov r3, r8, lsl #0x10
    add r9, r1, r2, asr #16
    mov r0, r7
    mov r1, r3, lsr #0x10
    bl func_02033544
    ldr r0, [sp, #4]
    add r1, r11, r8, lsl #7
    str r1, [r7, #0x5c]
    add r0, r0, r8, lsl #7
    mov r1, #0
    str r0, [r7, #0x60]
    mov r0, r7
    mov r2, r1
    bl func_020332c4
    sub r0, r9, #0xa
    strh r0, [r7, #0x48]
    strh r4, [r7, #0x4a]
    ldrsh r1, [r7, #0x48]
    mov r0, r7
    strh r1, [r7, #0x4c]
    ldrsh r1, [r7, #0x4a]
    sub r1, r1, #0xc0
    strh r1, [r7, #0x4e]
    bl func_02032c8c
    sub r0, r5, #0x10
    mov r0, r0, lsl #0x10
    mov r0, r0, asr #0x10
    sub r0, r0, #0xe
    mov r2, r0, lsl #0x10
    ldr r1, [sp]
    add r0, r6, #4
    add r1, r1, r2, asr #16
    strh r1, [r0, #0x48]
    strh r4, [r0, #0x4a]
    ldrsh r1, [r0, #0x48]
    strh r1, [r0, #0x4c]
    ldrsh r1, [r0, #0x4a]
    sub r1, r1, #0xc0
    strh r1, [r0, #0x4e]
    bl func_02032c8c
    add sp, sp, #0x14
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd 0x00007e90
    dcd data_022c4560
}

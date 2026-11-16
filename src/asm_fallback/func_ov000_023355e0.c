// asm: fallback — C attempts did not match (best C draft: /home/claude/w_ov000t/e0p_peephole.c)
#include <nitro/types.h>

extern u32 data_ov000_023687d4;
extern u32 data_ov000_0236aa90;
extern u32 data_ov000_0236aa94;
extern u32 data_ov000_0236abd8;

asm void func_ov000_023355e0(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x1c
    ldr r5, [pc, #0x104]
    ldr r4, [pc, #0x104]
    ldrh r6, [r5, #0xe]
    ldr r4, [r4]
    ldrh r5, [r5, #0x10]
    sub r6, r6, #1
    ldr r10, [r4, #0x10]
    sub r4, r5, #1
    cmp r0, r6
    movgt r0, r6
    cmp r1, r4
    movgt r1, r4
    cmp r2, r6
    movle r6, r2
    cmp r3, r4
    movle r4, r3
    str r4, [sp, #0x14]
    mov r11, #0
    str r6, [sp, #0x10]
    ldr r4, [pc, #0xc0]
    str r0, [sp, #8]
    str r1, [sp, #0xc]
    add r5, sp, #8
    mov r7, r11
    mov r6, #0xf0
_6c:
    ldr r0, [pc, #0xac]
    ldr r1, [sp, #0xc]
    ldr r2, [r0, r11, lsl #2]
    mov r0, r1, lsl #8
    ldr r1, [sp, #0x14]
    mov r8, r0, lsr #0x10
    str r2, [r4]
    cmp r8, r1, asr #8
    bgt _f8
_90:
    ldr r0, [sp, #8]
    ldr r1, [sp, #0x10]
    mov r0, r0, lsl #8
    mov r9, r0, lsr #0x10
    cmp r9, r1, asr #8
    bgt _e0
_a8:
    str r7, [sp]
    str r6, [sp, #4]
    ldr r12, [r4]
    mov r0, r10
    mov r1, r9
    mov r2, r8
    mov r3, r5
    blx r12
    add r0, r9, #1
    mov r0, r0, lsl #0x10
    ldr r1, [sp, #0x10]
    mov r9, r0, lsr #0x10
    cmp r9, r1, asr #8
    ble _a8
_e0:
    add r0, r8, #1
    mov r0, r0, lsl #0x10
    ldr r1, [sp, #0x14]
    mov r8, r0, lsr #0x10
    cmp r8, r1, asr #8
    ble _90
_f8:
    add r0, r11, #1
    and r11, r0, #0xff
    cmp r11, #3
    blo _6c
    add sp, sp, #0x1c
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd data_ov000_0236abd8
    dcd data_ov000_0236aa90
    dcd data_ov000_0236aa94
    dcd data_ov000_023687d4
}

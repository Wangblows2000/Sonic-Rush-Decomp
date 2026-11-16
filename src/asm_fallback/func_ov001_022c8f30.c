// asm: fallback — C attempts did not match (best C draft: /home/claude/w_ov001j/src0/func_ov001_022c8f30.c)
#include <nitro/types.h>

extern void _s32_div_f(void);
extern void _u32_div_f(void);

#pragma thumb on
asm void func_ov001_022c8f30(void)
{
    push {r4, r5, r6, r7, lr}
    sub sp, #0x14
    str r2, [sp]
    ldr r5, [sp, #0x28]
    add r2, r5, #0
    str r2, [sp, #4]
    add r2, #0x28
    str r2, [sp, #4]
    add r2, r5, #0
    str r2, [sp, #8]
    add r2, #0x2c
    str r2, [sp, #8]
    cmp r3, #1
    beq _22
    ldr r2, [sp]
    lsr r2, r2, #1
    str r2, [sp]
_22:
    mov r4, #0
    add r6, r4, #0
    add r2, r4, #0
    ldr r3, [sp]
    cmp r3, #0
    bls _54
    str r4, [sp, #0xc]
    add r7, r4, #0
_32:
    ldrsh r3, [r0, r7]
    cmp r3, #0
    bge _3a
    neg r3, r3
_3a:
    add r6, r6, r3
    ldr r3, [sp, #0xc]
    ldrsh r3, [r1, r3]
    cmp r3, #0
    bge _46
    neg r3, r3
_46:
    add r4, r4, r3
    add r0, r0, #2
    add r1, r1, #2
    add r2, r2, #1
    ldr r3, [sp]
    cmp r2, r3
    blo _32
_54:
    add r0, r6, #0
    ldr r1, [sp]
    bl _u32_div_f
    add r6, r0, #0
    add r0, r4, #0
    ldr r1, [sp]
    bl _u32_div_f
    add r4, r0, #0
    ldr r0, [pc, #0xdc]
    add r1, r5, r0
    ldr r2, [r1]
    ldr r0, [pc, #0xdc]
    str r2, [r5, r0]
    ldr r0, [pc, #0xdc]
    add r2, r5, r0
    ldr r0, [r2]
    str r0, [r1]
    ldr r0, [pc, #0xd8]
    add r0, r5, r0
    ldr r1, [r0]
    str r1, [r2]
    str r6, [r0]
    ldr r0, [pc, #0xd0]
    add r1, r5, r0
    ldr r2, [r1]
    ldr r0, [pc, #0xd0]
    str r2, [r5, r0]
    ldr r0, [pc, #0xd0]
    add r0, r5, r0
    ldr r2, [r0]
    str r2, [r1]
    ldr r7, [pc, #0xcc]
    add r1, r5, r7
    ldr r2, [r1]
    str r2, [r0]
    str r4, [r1]
    mov r1, #0
    add r0, r1, #0
    add r3, r1, #0
_a6:
    ldr r2, [pc, #0xac]
    ldr r2, [r5, r2]
    cmp r2, #0
    bne _b0
    add r6, r0, #0
_b0:
    ldr r2, [r5, r7]
    cmp r2, #0
    bne _b8
    add r4, r3, #0
_b8:
    add r5, r5, #4
    add r1, r1, #1
    cmp r1, #4
    blt _a6
    ldr r0, [pc, #0xa4]
    cmp r6, r0
    bge _c8
    mov r6, #0
_c8:
    ldr r0, [pc, #0x9c]
    cmp r4, r0
    bge _d0
    mov r4, #0
_d0:
    ldr r0, [pc, #0x98]
    cmp r6, r0
    ble _d8
    add r6, r0, #0
_d8:
    ldr r0, [pc, #0x90]
    cmp r4, r0
    ble _e0
    add r4, r0, #0
_e0:
    lsl r0, r6, #0xc
    ldr r1, [pc, #0x88]
    bl _s32_div_f
    asr r2, r0, #0x1f
    lsr r1, r0, #0x16
    lsl r3, r2, #0xa
    orr r3, r1
    lsl r2, r0, #0xa
    mov r1, #0
    ldr r0, [pc, #0x78]
    add r2, r2, r0
    adc r3, r1
    lsl r0, r3, #0x14
    lsr r1, r2, #0xc
    orr r1, r0
    ldr r0, [pc, #0x70]
    add r5, r1, r0
    lsl r0, r4, #0xc
    ldr r1, [pc, #0x64]
    bl _s32_div_f
    asr r2, r0, #0x1f
    lsr r1, r0, #0x16
    lsl r3, r2, #0xa
    orr r3, r1
    lsl r2, r0, #0xa
    mov r1, #0
    ldr r0, [pc, #0x54]
    add r2, r2, r0
    adc r3, r1
    lsl r0, r3, #0x14
    lsr r1, r2, #0xc
    orr r1, r0
    ldr r0, [pc, #0x4c]
    add r0, r1, r0
    ldr r1, [sp, #4]
    ldr r1, [r1]
    cmp r5, r1
    ble _134
    ldr r1, [sp, #4]
    str r5, [r1]
_134:
    ldr r1, [sp, #8]
    ldr r1, [r1]
    cmp r0, r1
    ble _140
    ldr r1, [sp, #8]
    str r0, [r1]
_140:
    add sp, #0x14
    pop {r4, r5, r6, r7}
    pop {r3}
    bx r3
    dcd 0x0000018c
    dcd 0x00000190
    dcd 0x00000188
    dcd 0x00000184
    dcd 0x0000019c
    dcd 0x000001a0
    dcd 0x00000198
    dcd 0x00000194
    dcd 0x000003e8
    dcd 0x00003e80
    dcd 0x00000800
    dcd 0x00001000
}
#pragma thumb reset

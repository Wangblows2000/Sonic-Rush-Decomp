// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void _s32_div_f(void);
extern u32 data_ov000_0236aad8;
extern u32 gPlayers;

asm void func_ov000_02303c58(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #4
    mov r6, r0
    mov r4, #0
    ldr r11, [pc, #0x174]
    mov r0, #0x64
    str r0, [sp]
_1c:
    add r0, r6, r4
    add r2, r0, #0x200
    ldrsb r1, [r2, #0x1c]
    ldr r0, [r6, #0x1e0]
    ldr r8, [r11, r1, lsl #2]
    ldrsb r7, [r0, #6]
    ldrb r3, [r0, #8]
    ldr r1, [sp]
    ldr r0, [pc, #0x14c]
    mla r5, r4, r1, r0
    add r0, r7, r3
    ldr r1, [r6, #0x10]
    ldr r3, [r8, #0x10]
    add r0, r1, r0, lsl #8
    cmp r3, r0
    ble _88
    ldr r0, [r6, #0x210]
    str r0, [r5, #0x3c]
    ldr r0, [r6, #0x214]
    str r0, [r5, #0x40]
    ldr r0, [r5]
    bic r0, r0, #0x2000
    str r0, [r5]
    ldr r0, [r5]
    orr r0, r0, #0x1000
    str r0, [r5]
    b _170
_88:
    mov r0, r7, lsl #0x18
    add r0, r1, r0, asr #16
    cmp r3, r0
    bge _b4
    ldr r0, [r5]
    bic r0, r0, #0x2000
    str r0, [r5]
    ldr r0, [r5]
    bic r0, r0, #0x1000
    str r0, [r5]
    b _170
_b4:
    ldr r0, [r5]
    bic r0, r0, #0x1000
    str r0, [r5]
    ldrsb r0, [r2, #0x1c]
    ldr r8, [r6, #0x1e0]
    ldr r3, [r6, #0x14]
    ldrsb r7, [r8, #7]
    ldr r0, [r11, r0, lsl #2]
    mov r1, r7, lsl #0x18
    ldr r0, [r0, #0x14]
    add r1, r3, r1, asr #16
    cmp r0, r1
    blt _170
    ldrb r1, [r8, #9]
    add r1, r7, r1
    add r1, r3, r1, lsl #8
    cmp r0, r1
    bgt _170
    ldr r0, [r5]
    orr r0, r0, #0x2000
    str r0, [r5]
    ldrsb r1, [r2, #0x1c]
    ldr r0, [r6, #0x1e0]
    ldr r3, [r6, #0x10]
    ldr r1, [r11, r1, lsl #2]
    ldrsb r2, [r0, #6]
    ldr r7, [r1, #0x10]
    ldr r10, [r5, #0x48]
    ldr r1, [r6, #0x214]
    mov r7, r7, asr #8
    add r2, r2, r3, asr #8
    sub r9, r7, r2
    sub r1, r1, r10
    ldrb r8, [r0, #8]
    mul r0, r1, r9
    mov r1, r8
    bl _s32_div_f
    ldr r7, [r5, #0x44]
    ldr r1, [r6, #0x210]
    add r10, r10, r0
    sub r1, r1, r7
    mul r0, r1, r9
    mov r1, r8
    bl _s32_div_f
    add r0, r7, r0
    str r0, [r5, #0x3c]
    str r10, [r5, #0x40]
_170:
    add r0, r4, #1
    and r4, r0, #0xff
    cmp r4, #2
    blo _1c
    add sp, sp, #4
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd gPlayers
    dcd data_ov000_0236aad8
}

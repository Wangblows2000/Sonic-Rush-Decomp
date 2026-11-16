// asm: fallback — C attempts did not match (best C draft: /home/claude/w_main_c2/9900i.c)
#include <nitro/types.h>

asm void func_02009900(void)
{
    stmdb sp!, {lr}
    sub sp, sp, #4
    cmp r1, #0
    blt _18
    cmp r1, #0xf
    ble _28
_18:
    add sp, sp, #4
    mov r0, #0
    ldmia sp!, {lr}
    bx lr
_28:
    add r3, r0, #0x1000
    mov lr, #1
    mov r12, lr, lsl r1
    ldr r3, [r3, #0x1c4]
    ands r3, r3, r12
    ldr r12, [r2]
    moveq lr, #0
    and r3, lr, #1
    bic r12, r12, #2
    orr r3, r12, r3, lsl #1
    str r3, [r2]
    ldr r12, [r2]
    mov r3, r12, lsl #0x1e
    movs r3, r3, asr #0x1f
    beq _10c
    add r0, r0, r1, lsl #2
    add r0, r0, #0x1000
    ldr r0, [r0, #0x180]
    ldr r3, [r2]
    ands r1, r0, #0x80000000
    movne lr, #1
    moveq lr, #0
    bic r12, r3, #1
    and r3, lr, #1
    and r1, r0, #0x7f
    orr r3, r12, r3
    str r3, [r2]
    and r1, r1, #0xff
    strh r1, [r2, #4]
    ldrh r1, [r2, #4]
    and r3, r0, #0x300
    mov r12, r3, lsr #8
    mov r1, r1, lsl #4
    strh r1, [r2, #4]
    cmp r12, #3
    add r3, r2, #4
    addls pc, pc, r12, lsl #2
    b _fc
    b _fc
    b _d0
    b _e0
    b _f0
_d0:
    ldrh r1, [r3]
    mov r1, r1, asr #1
    strh r1, [r3]
    b _fc
_e0:
    ldrh r1, [r3]
    mov r1, r1, asr #2
    strh r1, [r3]
    b _fc
_f0:
    ldrh r1, [r3]
    mov r1, r1, asr #4
    strh r1, [r3]
_fc:
    and r0, r0, #0x7f0000
    mov r0, r0, lsr #0x10
    strb r0, [r2, #6]
    b _1a8
_10c:
    mov r3, #0x54
    mla r0, r1, r3, r0
    ldrb r1, [r0, #3]
    bic r12, r12, #1
    add r3, r2, #4
    mov r1, r1, lsl #0x1f
    mov r1, r1, lsr #0x1f
    and r1, r1, #1
    orr r1, r12, r1
    str r1, [r2]
    ldrb r1, [r0, #2]
    str r1, [r2, #8]
    ldrh r12, [r0, #0x24]
    and r1, r12, #0xff
    strh r1, [r2, #4]
    ldrh r1, [r2, #4]
    mov r12, r12, asr #8
    cmp r12, #3
    mov r1, r1, lsl #4
    strh r1, [r2, #4]
    addls pc, pc, r12, lsl #2
    b _1a0
    b _1a0
    b _174
    b _184
    b _194
_174:
    ldrh r1, [r3]
    mov r1, r1, asr #1
    strh r1, [r3]
    b _1a0
_184:
    ldrh r1, [r3]
    mov r1, r1, asr #2
    strh r1, [r3]
    b _1a0
_194:
    ldrh r1, [r3]
    mov r1, r1, asr #4
    strh r1, [r3]
_1a0:
    ldrb r0, [r0, #0x23]
    strb r0, [r2, #6]
_1a8:
    mov r0, #1
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
}

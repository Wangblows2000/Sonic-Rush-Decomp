#include <nitro/types.h>

asm void OSi_ReadCardRom32(void)
{
    stmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #4
    ldr r4, =0x27ffe60
    ldr r3, =0x1ff
    ldr r4, [r4]
    and r3, r0, r3
    bic r4, r4, #0x7000000
    orr r10, r4, #0xa1000000
    rsb r9, r3, #0
    ldr r4, =0x40001a4
@1:
    ldr r3, [r4]
    ands r3, r3, #0x80000000
    bne @1
    ldr r3, =0x40001a1
    mov r4, #0x80
    cmp r9, r2
    addge sp, sp, #4
    strb r4, [r3]
    add r0, r0, r9
    ldmgefd sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxge lr
    ldr r3, =0x4100010
    ldr r12, =0x40001a4
    ldr r5, =0x40001a8
    ldr r4, =0x40001a9
    mov r7, r0, lsr #8
    mov r6, #0xb7
    mov lr, #0
@2:
    strb r6, [r5]
    mov r8, r0, lsr #0x18
    strb r8, [r4]
    ldr r8, =0x40001aa
    mov r11, r0, lsr #0x10
    strb r11, [r8]
    ldr r8, =0x40001ab
    strb r7, [r8]
    ldr r8, =0x40001ac
    strb r0, [r8]
    ldr r8, =0x40001ad
    strb lr, [r8]
    ldr r8, =0x40001ae
    strb lr, [r8]
    ldr r8, =0x40001af
    strb lr, [r8]
    str r10, [r12]
@3:
    ldr r8, [r12]
    ands r11, r8, #0x800000
    beq @5
    cmp r9, #0
    ldr r11, [r3]
    blt @4
    cmp r9, r2
    strlt r11, [r1, r9]
@4:
    add r9, r9, #4
@5:
    ands r8, r8, #0x80000000
    bne @3
    cmp r9, r2
    add r7, r7, #2
    add r0, r0, #0x200
    blt @2
    add sp, sp, #4
    ldmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
}


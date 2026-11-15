#include <nitro/types.h>

asm void func_0202f24c(void)
{
    eor r2, r0, r1
    and r2, r2, #0x80000000
    mov r12, #0xff
    ands r3, r12, r0, lsr #23
    mov r0, r0, lsl #8
    cmpne r3, #0xff
    beq _7c
    orr r0, r0, #0x80000000
    ands r12, r12, r1, lsr #23
    mov r1, r1, lsl #8
    cmpne r12, #0xff
    beq _bc
    orr r1, r1, #0x80000000
_34:
    add r12, r3, r12
    umull r1, r3, r0, r1
    movs r0, r3
    addpl r0, r0, r0
    subpl r12, r12, #1
    subs r12, r12, #0x7f
    bmi _148
    cmp r12, #0xfe
    bge _1b4
    ands r3, r0, #0xff
    orr r0, r2, r0, lsr #8
    add r0, r0, r12, lsl #23
    tst r3, #0x80
    bxeq lr
    orrs r1, r1, r3, lsl #25
    andeqs r3, r0, #1
    addne r0, r0, #1
    bx lr
_7c:
    cmp r3, #0
    beq _d0
    movs r0, r0, lsl #1
    bne _1a4
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq _b0
    cmp r12, #0xff
    blt _198
    cmp r1, #0
    beq _198
    b _1a4
_b0:
    cmp r1, #0
    beq _1ac
    b _198
_bc:
    cmp r12, #0
    beq _12c
_c4:
    movs r1, r1, lsl #1
    bne _1a4
    b _198
_d0:
    movs r0, r0, lsl #1
    beq _108
    mov r0, r0, lsr #1
    clz r3, r0
    movs r0, r0, lsl r3
    rsb r3, r3, #1
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #8
    ands r12, r12, #0xff
    beq _12c
    cmp r12, #0xff
    beq _c4
    orr r1, r1, #0x80000000
    b _34
_108:
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq _1d8
    cmp r12, #0xff
    blt _1d8
    cmp r1, #0
    beq _1ac
    b _1a4
_12c:
    movs r1, r1, lsl #1
    beq _1d8
    mov r1, r1, lsr #1
    clz r12, r1
    movs r1, r1, lsl r12
    rsb r12, r12, #1
    b _34
_148:
    cmn r12, #0x18
    beq _190
    bmi _1d0
    cmp r1, #0
    orrne r0, r0, #1
    mov r3, r0
    mov r0, r0, lsr #8
    rsb r12, r12, #0
    orr r0, r2, r0, lsr r12
    rsb r12, r12, #0x18
    movs r1, r3, lsl r12
    bxeq lr
    tst r1, #0x80000000
    bxeq lr
    movs r1, r1, lsl #1
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
_190:
    mov r0, r0, lsl #1
    b _1c0
_198:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
_1a4:
    mvn r0, #0x80000000
    bx lr
_1ac:
    mvn r0, #0x80000000
    bx lr
_1b4:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
_1c0:
    movs r1, r0
    mov r0, r2
    addne r0, r0, #1
    bx lr
_1d0:
    mov r0, r2
    bx lr
_1d8:
    mov r0, r2
    bx lr
}

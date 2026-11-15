#include <nitro/types.h>

asm void func_0202f438(void)
{
    eors r2, r0, r1
    eormi r1, r1, #0x80000000
    dcd 0x4afffb64  // bmi func_0202e1cc+0xc (mid-function entry; no symbol, raw encoding)
    subs r12, r0, r1
    eorcc r12, r12, #0x80000000
    subcc r0, r0, r12
    addcc r1, r1, r12
    mov r2, #0x80000000
    mov r3, r0, lsr #0x17
    orr r0, r2, r0, lsl #8
    ands r12, r3, #0xff
    cmpne r12, #0xff
    beq _128
    mov r12, r1, lsr #0x17
    orr r1, r2, r1, lsl #8
    ands r2, r12, #0xff
    beq _168
_44:
    subs r12, r3, r12
    beq _8c
    rsb r2, r12, #0x20
    movs r2, r1, lsl r2
    mov r1, r1, lsr r12
    orrne r1, r1, #1
    subs r0, r0, r1
    bpl _d0
    ands r1, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    tst r1, #0x80
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
_8c:
    subs r0, r0, r1
    beq _234
    mov r2, r3, lsl #0x17
    and r2, r2, #0x80000000
    bic r3, r3, #0x100
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
    cmp r3, #0
    bgt _c0
    rsb r3, r3, #9
    orr r0, r2, r0, lsr r3
    bx lr
_c0:
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
_d0:
    mov r2, r3, lsl #0x17
    and r2, r2, #0x80000000
    bic r3, r3, #0x100
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
    cmp r3, #0
    bgt _fc
    rsb r3, r3, #9
    orr r0, r2, r0, lsr r3
    bx lr
_fc:
    ands r1, r0, #0xff
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bxeq lr
    tst r1, #0x80
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
_128:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    ands r3, r3, #0xff
    beq _190
    movs r0, r0, lsl #1
    bne _268
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq _25c
    cmp r12, #0xff
    blt _25c
    cmp r1, #0
    beq _270
    b _268
_168:
    cmp r12, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    and r3, r3, #0xff
    ands r12, r12, #0xff
    beq _1f8
_180:
    eor r2, r2, #0x80000000
    movs r1, r1, lsl #1
    bne _268
    b _25c
_190:
    movs r0, r0, lsl #1
    beq _1c8
    mov r0, r0, lsr #1
    mov r3, #1
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #8
    ands r12, r12, #0xff
    beq _1f8
    cmp r12, #0xff
    beq _180
    orr r1, r1, #0x80000000
    orr r3, r3, r2, lsr #23
    orr r12, r12, r2, lsr #23
    b _44
_1c8:
    mov r3, r1, lsr #0x17
    mov r0, r1, lsl #9
    ands r2, r3, #0xff
    beq _1ec
    cmp r2, #0xff
    blt _214
    cmp r0, #0
    bne _254
    b _25c
_1ec:
    cmp r0, #0
    beq _234
    b _214
_1f8:
    movs r1, r1, lsl #1
    beq _21c
    mov r1, r1, lsr #1
    mov r12, #1
    orr r12, r12, r2, lsr #23
    orr r3, r3, r2, lsr #23
    b _44
_214:
    mov r0, r1
    bx lr
_21c:
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
_234:
    mov r0, #0
    bx lr
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
_254:
    mvn r0, #0x80000000
    bx lr
_25c:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
_268:
    mvn r0, #0x80000000
    bx lr
_270:
    mvn r0, #0x80000000
    bx lr
}

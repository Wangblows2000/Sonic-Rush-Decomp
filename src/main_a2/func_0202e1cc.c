#include <nitro/types.h>

asm void func_0202e1cc(void)
{
    eors r2, r0, r1
    eormi r1, r1, #0x80000000
    dcd 0x4a00049a  // bmi func_0202f438+0xc (mid-function entry; no symbol, raw encoding)
    subs r12, r0, r1
    subcc r0, r0, r12
    addcc r1, r1, r12
    mov r2, #0x80000000
    mov r3, r0, lsr #0x17
    orr r0, r2, r0, lsl #8
    ands r12, r3, #0xff
    cmpne r12, #0xff
    beq _a0
    mov r12, r1, lsr #0x17
    orr r1, r2, r1, lsl #8
    ands r2, r12, #0xff
    beq _e0
_40:
    subs r12, r3, r12
    beq _58
    rsb r2, r12, #0x20
    movs r2, r1, lsl r2
    mov r1, r1, lsr r12
    orrne r1, r1, #1
_58:
    adds r0, r0, r1
    bcc _78
    and r1, r0, #1
    orr r0, r1, r0, rrx
    add r3, r3, #1
    and r2, r3, #0xff
    cmp r2, #0xff
    beq _1e8
_78:
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
_a0:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    ands r3, r3, #0xff
    beq _104
    movs r0, r0, lsl #1
    bne _214
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq _208
    cmp r12, #0xff
    blt _208
    cmp r1, #0
    beq _208
    b _214
_e0:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    and r3, r3, #0xff
    ands r12, r12, #0xff
    beq _160
_f8:
    movs r1, r1, lsl #1
    bne _214
    b _208
_104:
    movs r0, r0, lsl #1
    beq _13c
    mov r3, #1
    mov r0, r0, lsr #1
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #8
    ands r12, r12, #0xff
    beq _160
    cmp r12, #0xff
    beq _f8
    orr r1, r1, #0x80000000
    orr r3, r3, r2, lsr #23
    orr r12, r12, r2, lsr #23
    b _40
_13c:
    mov r3, r1, lsr #0x17
    mov r0, r1, lsl #9
    ands r3, r3, #0xff
    beq _1c8
    cmp r3, #0xff
    blt _1c8
    cmp r0, #0
    beq _208
    b _200
_160:
    movs r1, r1, lsl #1
    beq _1d0
    mov r1, r1, lsr #1
    mov r12, #1
    orr r3, r3, r2, lsr #23
    orr r12, r12, r2, lsr #23
    cmp r0, #0
    bmi _40
    adds r0, r0, r1
    bcc _194
    and r1, r0, #1
    orr r0, r1, r0, rrx
    add r12, r12, #1
_194:
    cmp r0, #0
    subge r12, r12, #1
    ands r1, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r12, lsl #23
    bxeq lr
    tst r1, #0x80
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
_1c8:
    mov r0, r1
    bx lr
_1d0:
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
_1e8:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
_200:
    mvn r0, #0x80000000
    bx lr
_208:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
_214:
    mvn r0, #0x80000000
    bx lr
}

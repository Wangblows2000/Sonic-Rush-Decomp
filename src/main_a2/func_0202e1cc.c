// asm: hand-written runtime library routine (split at mid-function entry points, see w_asm/split_plan.txt)
#include <nitro/types.h>

extern void _fsub_ec(void);
void _fadd(void);
void _fadd_ec(void);

asm void _fadd(void)
{
    eors r2, r0, r1
    eormi r1, r1, #0x80000000
    bmi _fsub_ec
}

asm void _fadd_ec(void)
{
    subs r12, r0, r1
    sublo r0, r0, r12
    addlo r1, r1, r12
    mov r2, #0x80000000
    mov r3, r0, lsr #0x17
    orr r0, r2, r0, lsl #8
    ands r12, r3, #0xff
    cmpne r12, #0xff
    beq _94
    mov r12, r1, lsr #0x17
    orr r1, r2, r1, lsl #8
    ands r2, r12, #0xff
    beq _d4
_34:
    subs r12, r3, r12
    beq _4c
    rsb r2, r12, #0x20
    movs r2, r1, lsl r2
    mov r1, r1, lsr r12
    orrne r1, r1, #1
_4c:
    adds r0, r0, r1
    blo _6c
    and r1, r0, #1
    orr r0, r1, r0, rrx
    add r3, r3, #1
    and r2, r3, #0xff
    cmp r2, #0xff
    beq _1dc
_6c:
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
_94:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    ands r3, r3, #0xff
    beq _f8
    movs r0, r0, lsl #1
    bne _208
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq _1fc
    cmp r12, #0xff
    blt _1fc
    cmp r1, #0
    beq _1fc
    b _208
_d4:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    and r3, r3, #0xff
    ands r12, r12, #0xff
    beq _154
_ec:
    movs r1, r1, lsl #1
    bne _208
    b _1fc
_f8:
    movs r0, r0, lsl #1
    beq _130
    mov r3, #1
    mov r0, r0, lsr #1
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #8
    ands r12, r12, #0xff
    beq _154
    cmp r12, #0xff
    beq _ec
    orr r1, r1, #0x80000000
    orr r3, r3, r2, lsr #23
    orr r12, r12, r2, lsr #23
    b _34
_130:
    mov r3, r1, lsr #0x17
    mov r0, r1, lsl #9
    ands r3, r3, #0xff
    beq _1bc
    cmp r3, #0xff
    blt _1bc
    cmp r0, #0
    beq _1fc
    b _1f4
_154:
    movs r1, r1, lsl #1
    beq _1c4
    mov r1, r1, lsr #1
    mov r12, #1
    orr r3, r3, r2, lsr #23
    orr r12, r12, r2, lsr #23
    cmp r0, #0
    bmi _34
    adds r0, r0, r1
    blo _188
    and r1, r0, #1
    orr r0, r1, r0, rrx
    add r12, r12, #1
_188:
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
_1bc:
    mov r0, r1
    bx lr
_1c4:
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
_1dc:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
_1f4:
    mvn r0, #0x80000000
    bx lr
_1fc:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
_208:
    mvn r0, #0x80000000
    bx lr
}

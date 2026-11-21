// asm: hand-written runtime library routine (split at mid-function entry points, see w_asm/split_plan.txt)
#include <nitro/types.h>

extern void _fadd_ec(void);
void _fsub(void);
void _fsub_ec(void);

asm void _fsub(void)
{
    eors r2, r0, r1
    eormi r1, r1, #0x80000000
    bmi _fadd_ec
}

asm void _fsub_ec(void)
{
    subs r12, r0, r1
    eorlo r12, r12, #0x80000000
    sublo r0, r0, r12
    addlo r1, r1, r12
    mov r2, #0x80000000
    mov r3, r0, lsr #0x17
    orr r0, r2, r0, lsl #8
    ands r12, r3, #0xff
    cmpne r12, #0xff
    beq _11c
    mov r12, r1, lsr #0x17
    orr r1, r2, r1, lsl #8
    ands r2, r12, #0xff
    beq _15c
_38:
    subs r12, r3, r12
    beq _80
    rsb r2, r12, #0x20
    movs r2, r1, lsl r2
    mov r1, r1, lsr r12
    orrne r1, r1, #1
    subs r0, r0, r1
    bpl _c4
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
_80:
    subs r0, r0, r1
    beq _228
    mov r2, r3, lsl #0x17
    and r2, r2, #0x80000000
    bic r3, r3, #0x100
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
    cmp r3, #0
    bgt _b4
    rsb r3, r3, #9
    orr r0, r2, r0, lsr r3
    bx lr
_b4:
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
_c4:
    mov r2, r3, lsl #0x17
    and r2, r2, #0x80000000
    bic r3, r3, #0x100
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
    cmp r3, #0
    bgt _f0
    rsb r3, r3, #9
    orr r0, r2, r0, lsr r3
    bx lr
_f0:
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
_11c:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    ands r3, r3, #0xff
    beq _184
    movs r0, r0, lsl #1
    bne _25c
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq _250
    cmp r12, #0xff
    blt _250
    cmp r1, #0
    beq _264
    b _25c
_15c:
    cmp r12, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    and r3, r3, #0xff
    ands r12, r12, #0xff
    beq _1ec
_174:
    eor r2, r2, #0x80000000
    movs r1, r1, lsl #1
    bne _25c
    b _250
_184:
    movs r0, r0, lsl #1
    beq _1bc
    mov r0, r0, lsr #1
    mov r3, #1
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #8
    ands r12, r12, #0xff
    beq _1ec
    cmp r12, #0xff
    beq _174
    orr r1, r1, #0x80000000
    orr r3, r3, r2, lsr #23
    orr r12, r12, r2, lsr #23
    b _38
_1bc:
    mov r3, r1, lsr #0x17
    mov r0, r1, lsl #9
    ands r2, r3, #0xff
    beq _1e0
    cmp r2, #0xff
    blt _208
    cmp r0, #0
    bne _248
    b _250
_1e0:
    cmp r0, #0
    beq _228
    b _208
_1ec:
    movs r1, r1, lsl #1
    beq _210
    mov r1, r1, lsr #1
    mov r12, #1
    orr r12, r12, r2, lsr #23
    orr r3, r3, r2, lsr #23
    b _38
_208:
    mov r0, r1
    bx lr
_210:
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
_228:
    mov r0, #0
    bx lr
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
_248:
    mvn r0, #0x80000000
    bx lr
_250:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
_25c:
    mvn r0, #0x80000000
    bx lr
_264:
    mvn r0, #0x80000000
    bx lr
}

// asm: hand-written runtime library routine (split at mid-function entry points, see w_asm/split_plan.txt)
#include <nitro/types.h>

extern void _dadd_e10(void);
void _ddiv(void);
void _ddiv_e10(void);

asm void _ddiv(void)
{
    stmdb sp!, {r4, lr}
    eors r12, r1, r3
    eormi r3, r3, #0x80000000
    bmi _dadd_e10
}

asm void _ddiv_e10(void)
{
    subs r12, r0, r2
    sbcs lr, r1, r3
    bhs _20
    eor lr, lr, #0x80000000
    adds r2, r2, r12
    adc r3, r3, lr
    subs r0, r0, r12
    sbc r1, r1, lr
_20:
    mov lr, #0x80000000
    mov r12, r1, lsr #0x14
    orr r1, lr, r1, lsl #11
    orr r1, r1, r0, lsr #21
    mov r0, r0, lsl #0xb
    movs r4, r12, lsl #0x15
    cmnne r4, #0x200000
    beq _224
    mov r4, r3, lsr #0x14
    orr r3, lr, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs lr, r4, lsl #0x15
    beq _26c
_58:
    subs r4, r12, r4
    beq _100
    cmp r4, #0x20
    ble _94
    cmp r4, #0x38
    movge r4, #0x3f
    sub r4, r4, #0x20
    rsb lr, r4, #0x20
    orrs lr, r2, r3, lsl lr
    mov r2, r3, lsr r4
    orrne r2, r2, #1
    subs r0, r0, r2
    sbcs r1, r1, #0
    bmi _bc
    b _1ac
_94:
    rsb lr, r4, #0x20
    movs lr, r2, lsl lr
    rsb lr, r4, #0x20
    mov r2, r2, lsr r4
    orr r2, r2, r3, lsl lr
    mov r3, r3, lsr r4
    orrne r2, r2, #1
    subs r0, r0, r2
    sbcs r1, r1, r3
    bpl _1ac
_bc:
    movs r2, r0, lsl #0x15
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    mov r1, r1, lsr #0xc
    orr r1, r1, r12, lsl #20
    tst r2, #0x80000000
    ldmeqia sp!, {r4, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqia sp!, {r4, lr}
    bxeq lr
    adds r0, r0, #1
    adc r1, r1, #0
    ldmia sp!, {r4, lr}
    bx lr
_100:
    subs r0, r0, r2
    sbc r1, r1, r3
    orrs lr, r1, r0
    beq _390
    mov lr, r12, lsl #0x14
    and lr, lr, #0x80000000
    bic r12, r12, #0x800
    cmp r1, #0
    bmi _188
    bne _138
    sub r12, r12, #0x20
    movs r1, r0
    mov r0, #0
    bmi _154
_138:
    clz r4, r1
    movs r1, r1, lsl r4
    rsb r4, r4, #0x20
    orr r1, r1, r0, lsr r4
    rsb r4, r4, #0x20
    mov r0, r0, lsl r4
    sub r12, r12, r4
_154:
    cmp r12, #0
    bgt _190
    rsb r12, r12, #0xc
    cmp r12, #0x20
    movge r0, r1
    movge r1, #0
    subge r12, r12, #0x20
    rsb r4, r12, #0x20
    mov r0, r0, lsr r12
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr r12
    ldmia sp!, {r4, lr}
    bx lr
_188:
    cmp r1, #0
    subges r12, r12, #1
_190:
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, r12, lsl #20
    ldmia sp!, {r4, lr}
    bx lr
_1ac:
    mov lr, r12, lsl #0x14
    and lr, lr, #0x80000000
    bic r12, r12, #0x800
    cmp r1, #0
    bne _1d0
    sub r12, r12, #0x20
    movs r1, r0
    mov r0, #0
    bmi _1ec
_1d0:
    clz r4, r1
    movs r1, r1, lsl r4
    rsb r4, r4, #0x20
    orr r1, r1, r0, lsr r4
    rsb r4, r4, #0x20
    mov r0, r0, lsl r4
    sub r12, r12, r4
_1ec:
    cmp r12, #0
    orrgt r12, r12, lr, lsr #20
    bgt _bc
    rsb r12, r12, #0xc
    cmp r12, #0x20
    movge r0, r1
    movge r1, #0
    subge r12, r12, #0x20
    rsb r4, r12, #0x20
    mov r0, r0, lsr r12
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr r12
    ldmia sp!, {r4, lr}
    bx lr
_224:
    cmp r12, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bics r12, r12, #0x800
    beq _290
    orrs r4, r0, r1, lsl #1
    bne _36c
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs r4, r4, lsl #0x15
    beq _358
    cmn r4, #0x200000
    bne _358
    orrs r4, r2, r3, lsl #1
    beq _380
    b _36c
_26c:
    cmp r4, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bic r12, r12, #0x800
    bics r4, r4, #0x800
    beq _308
    orrs r4, r2, r3, lsl #1
    bne _36c
    b _358
_290:
    orrs r4, r0, r1, lsl #1
    beq _2d0
    mov r12, #1
    bic r1, r1, #0x80000000
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs r4, r4, lsl #0x15
    cmnne r4, #0x200000
    mov r4, r4, lsr #0x15
    orr r4, r4, lr, lsr #20
    beq _26c
    orr r3, r3, #0x80000000
    orr r12, r12, lr, lsr #20
    b _58
_2d0:
    mov r12, r3, lsr #0x14
    mov r1, r3, lsl #0xb
    orr r1, r1, r2, lsr #21
    mov r0, r2, lsl #0xb
    movs r4, r12, lsl #0x15
    beq _2fc
    cmn r4, #0x200000
    bne _324
    orrs r4, r0, r1, lsl #1
    bne _370
    b _358
_2fc:
    orrs r4, r0, r1, lsl #1
    beq _390
    b _324
_308:
    orrs r4, r2, r3, lsl #1
    beq _334
    mov r4, #1
    bic r3, r3, #0x80000000
    orr r12, r12, lr, lsr #20
    orr r4, r4, lr, lsr #20
    b _58
_324:
    mov r1, r3
    mov r0, r2
    ldmia sp!, {r4, lr}
    bx lr
_334:
    cmp r1, #0
    subges r12, r12, #1
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, r12, lsl #20
    ldmia sp!, {r4, lr}
    bx lr
_358:
    ldr r1, [pc, #0x40]
    orr r1, lr, r1
    mov r0, #0
    ldmia sp!, {r4, lr}
    bx lr
_36c:
    mov r1, r3
_370:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmia sp!, {r4, lr}
    bx lr
_380:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmia sp!, {r4, lr}
    bx lr
_390:
    mov r1, #0
    mov r0, #0
    ldmia sp!, {r4, lr}
    bx lr
    dcd 0x7ff00000
}

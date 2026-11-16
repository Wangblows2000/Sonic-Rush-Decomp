// asm: hand-written runtime library routine (split at mid-function entry points, see w_asm/split_plan.txt)
#include <nitro/types.h>

extern void _ddiv_e10(void);
void _dadd(void);
void _dadd_e10(void);

asm void _dadd(void)
{
    stmdb sp!, {r4, lr}
    eors r12, r1, r3
    eormi r3, r3, #0x80000000
    bmi _ddiv_e10
}

asm void _dadd_e10(void)
{
    subs r12, r0, r2
    sbcs lr, r1, r3
    bhs _1c
    adds r2, r2, r12
    adc r3, r3, lr
    subs r0, r0, r12
    sbc r1, r1, lr
_1c:
    mov lr, #0x80000000
    mov r12, r1, lsr #0x14
    orr r1, lr, r1, lsl #11
    orr r1, r1, r0, lsr #21
    mov r0, r0, lsl #0xb
    movs r4, r12, lsl #0x15
    cmnne r4, #0x200000
    beq _118
    mov r4, r3, lsr #0x14
    orr r3, lr, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs lr, r4, lsl #0x15
    beq _160
_54:
    subs r4, r12, r4
    beq _ac
    cmp r4, #0x20
    ble _90
    cmp r4, #0x38
    movge r4, #0x3f
    sub r4, r4, #0x20
    rsb lr, r4, #0x20
    orrs lr, r2, r3, lsl lr
    mov r2, r3, lsr r4
    orrne r2, r2, #1
    adds r0, r0, r2
    adcs r1, r1, #0
    blo _d4
    b _b8
_90:
    rsb lr, r4, #0x20
    movs lr, r2, lsl lr
    rsb lr, r4, #0x20
    mov r2, r2, lsr r4
    orr r2, r2, r3, lsl lr
    mov r3, r3, lsr r4
    orrne r2, r2, #1
_ac:
    adds r0, r0, r2
    adcs r1, r1, r3
    blo _d4
_b8:
    add r12, r12, #1
    and r4, r0, #1
    movs r1, r1, rrx
    orr r0, r4, r0, rrx
    mov lr, r12, lsl #0x15
    cmn lr, #0x200000
    beq _2e4
_d4:
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
_118:
    cmp r12, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bics r12, r12, #0x800
    beq _184
    orrs r4, r0, r1, lsl #1
    bne _2c0
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs r4, r4, lsl #0x15
    beq _2ac
    cmn r4, #0x200000
    bne _2ac
    orrs r4, r2, r3, lsl #1
    beq _2ac
    b _2c0
_160:
    cmp r4, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bic r12, r12, #0x800
    bics r4, r4, #0x800
    beq _1f0
    orrs r4, r2, r3, lsl #1
    bne _2c0
    b _2ac
_184:
    orrs r4, r0, r1, lsl #1
    beq _1c4
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
    beq _160
    orr r3, r3, #0x80000000
    orr r12, r12, lr, lsr #20
    b _54
_1c4:
    mov r12, r3, lsr #0x14
    mov r1, r3, lsl #0xb
    orr r1, r1, r2, lsr #21
    mov r0, r2, lsl #0xb
    movs r4, r12, lsl #0x15
    beq _278
    cmn r4, #0x200000
    bne _278
    orrs r4, r0, r1, lsl #1
    beq _2ac
    b _2c4
_1f0:
    orrs r4, r2, r3, lsl #1
    beq _288
    mov r4, #1
    bic r3, r3, #0x80000000
    cmp r1, #0
    bpl _214
    orr r12, r12, lr, lsr #20
    orr r4, r4, lr, lsr #20
    b _54
_214:
    adds r0, r0, r2
    adcs r1, r1, r3
    blo _234
    add r12, r12, #1
    and r4, r0, #1
    movs r1, r1, rrx
    mov r0, r0, rrx
    orr r0, r0, r4
_234:
    cmp r1, #0
    subges r12, r12, #1
    movs r2, r0, lsl #0x15
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, r12, lsl #20
    ldmeqia sp!, {r4, lr}
    bxeq lr
    tst r2, #0x80000000
    ldmeqia sp!, {r4, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqia sp!, {r4, lr}
    bxeq lr
_278:
    mov r1, r3
    mov r0, r2
    ldmia sp!, {r4, lr}
    bx lr
_288:
    cmp r1, #0
    subges r12, r12, #1
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, r12, lsl #20
    ldmia sp!, {r4, lr}
    bx lr
_2ac:
    ldr r1, [pc, #0x50]
    orr r1, lr, r1
    mov r0, #0
    ldmia sp!, {r4, lr}
    bx lr
_2c0:
    mov r1, r3
_2c4:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmia sp!, {r4, lr}
    bx lr
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmia sp!, {r4, lr}
    bx lr
_2e4:
    cmp r12, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    ldr r1, [pc, #0xc]
    orr r1, lr, r1
    mov r0, #0
    ldmia sp!, {r4, lr}
    bx lr
    dcd 0x7ff00000
}

// asm: fallback — C attempts did not match (best C draft: /home/claude/w_main_d3/f6e0_i.c)
#include <nitro/types.h>

extern void MI_CpuFill8(void);
extern u32 data_0207f2c0;

asm void func_0200f6e0(void)
{
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    ldr r1, [pc, #0x1b8]
    mov r5, r0
    ldr r4, [r1]
    mov r1, #0
    add r0, r4, #0x18
    mov r2, #0x28
    bl MI_CpuFill8
    cmp r5, #0
    addeq sp, sp, #4
    str r5, [r4, #4]
    ldmeqia sp!, {r4, r5, lr}
    bxeq lr
    mov r0, r5, asr #8
    and r0, r0, #0xff
    mov r1, #1
    mov r2, r1, lsl r0
    and r0, r5, #0xff
    str r2, [r4, #0x18]
    cmp r0, #1
    bne _dc
    cmp r2, #0x200
    beq _74
    cmp r2, #0x2000
    beq _94
    cmp r2, #0x10000
    beq _b8
    b _1a0
_74:
    mov r0, #0x10
    str r0, [r4, #0x20]
    str r1, [r4, #0x24]
    mov r0, #0x4f
    add sp, sp, #4
    str r0, [r4, #0x28]
    ldmia sp!, {r4, r5, lr}
    bx lr
_94:
    mov r0, #0x20
    str r0, [r4, #0x20]
    mov r0, #2
    str r0, [r4, #0x24]
    mov r0, #0x4f
    add sp, sp, #4
    str r0, [r4, #0x28]
    ldmia sp!, {r4, r5, lr}
    bx lr
_b8:
    mov r0, #0x80
    str r0, [r4, #0x20]
    mov r0, #2
    str r0, [r4, #0x24]
    mov r0, #0x9e
    add sp, sp, #4
    str r0, [r4, #0x28]
    ldmia sp!, {r4, r5, lr}
    bx lr
_dc:
    cmp r0, #2
    bne _170
    cmp r2, #0x40000
    beq _100
    cmp r2, #0x80000
    beq _124
    cmp r2, #0x100000
    beq _124
    b _1a0
_100:
    ldr r0, [pc, #0xc4]
    ldr r1, [pc, #0xc4]
    str r0, [r4, #0x2c]
    str r1, [r4, #0x30]
    ldr r0, [pc, #0xbc]
    str r1, [r4, #0x3c]
    str r0, [r4, #0x38]
    str r0, [r4, #0x34]
    b _144
_124:
    ldr r1, [pc, #0xa0]
    mov r0, #0
    str r1, [r4, #0x2c]
    ldr r1, [pc, #0x98]
    str r0, [r4, #0x30]
    ldr r0, [pc, #0x94]
    str r1, [r4, #0x3c]
    str r0, [r4, #0x38]
_144:
    mov r0, #0x10000
    str r0, [r4, #0x1c]
    mov r0, #0x100
    str r0, [r4, #0x20]
    mov r0, #3
    str r0, [r4, #0x24]
    mov r0, #0x4f
    add sp, sp, #4
    str r0, [r4, #0x28]
    ldmia sp!, {r4, r5, lr}
    bx lr
_170:
    cmp r0, #3
    bne _1a0
    cmp r2, #0x2000
    beq _188
    cmp r2, #0x8000
    bne _1a0
_188:
    str r2, [r4, #0x20]
    mov r0, #2
    add sp, sp, #4
    str r0, [r4, #0x24]
    ldmia sp!, {r4, r5, lr}
    bx lr
_1a0:
    mov r1, #0
    str r1, [r4, #4]
    str r1, [r4, #0x18]
    ldr r0, [pc, #0x14]
    mov r1, #3
    ldr r0, [r0]
    str r1, [r0]
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
    dcd data_0207f2c0
    dcd 0x0000018b
    dcd 0x0000127f
    dcd 0x00013435
}

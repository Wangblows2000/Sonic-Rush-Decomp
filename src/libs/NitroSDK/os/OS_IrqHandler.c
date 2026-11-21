#include <nitro/types.h>

extern u32 OS_IRQTable;
void OS_IrqHandler_ThreadSwitch(void);

asm void OS_IrqHandler(void)
{
    stmdb sp!, {lr}
    mov r12, #0x4000000
    add r12, r12, #0x210
    ldr r1, [r12, #-8]
    cmp r1, #0
    ldmeqia sp!, {pc}
    ldmia r12, {r1, r2}
    ands r1, r1, r2
    ldmeqia sp!, {pc}
    mov r3, #0x80000000
@1:
    clz r0, r1
    bics r1, r1, r3, lsr r0
    bne @1
    mov r1, r3, lsr r0
    str r1, [r12, #4]
    rsbs r0, r0, #0x1f
    ldr r1, =OS_IRQTable
    ldr r0, [r1, r0, lsl #2]
    ldr lr, =OS_IrqHandler_ThreadSwitch
    bx r0
}


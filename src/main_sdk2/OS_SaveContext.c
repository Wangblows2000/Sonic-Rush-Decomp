#include <nitro/types.h>

void func_0200d104(void);

asm BOOL OS_SaveContext(register void *context)
{
    stmfd sp!, {r0, lr}
    add r0, r0, #0x48
    ldr r1, =func_0200d104
    blx r1
    ldmfd sp!, {r0, lr}
    add r1, r0, #0
    mrs r2, cpsr
    str r2, [r1], #4
    mov r0, #0xd3
    msr cpsr_c, r0
    str sp, [r1, #0x40]
    msr cpsr_c, r2
    mov r0, #1
    stmia r1, {r0-r14}
    add r0, pc, #8
    str r0, [r1, #0x3c]
    mov r0, #0
    bx lr
}

#include <nitro/types.h>

extern u32 data_0207d2e4;
void OSi_GetAndDisplayContext(void);

asm void OSi_ExceptionHandler(void)
{
    ldr r12, =data_0207d2e4
    ldr r12, [r12]
    cmp r12, #0
    movne lr, pc
    bxne r12
    ldr r12, =0x2000000
    stmdb r12!, {r0, r1, r2, r3, sp, lr}
    and r0, sp, #1
    mov sp, r12
    mrs r1, cpsr
    and r1, r1, #0x1f
    teq r1, #0x17
    bne @1
    bl OSi_GetAndDisplayContext
    b @2
@1:
    teq r1, #0x1b
    bne @2
    bl OSi_GetAndDisplayContext
@2:
    ldr r12, =data_0207d2e4
    ldr r12, [r12]
    cmp r12, #0
@3:
    beq @3
@4:
    mov r0, r0
    b @4
    ldmfd sp!, {r0, r1, r2, r3, r12, lr}
    mov sp, r12
    bx lr
}


#include <nitro/types.h>

extern u32 data_0207d3a0;
void func_01ff81ac(void);
void func_01ff83a0(void);

asm void func_01ff8480(void)
{
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r0, =data_0207d3a0
@1:
    ldrh r1, [r0]
    cmp r1, #0
    beq @1
    ldr r0, =0x4000208
    mov r1, #0
    strh r1, [r0]
    bl func_01ff83a0
    bl func_01ff81ac
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
}


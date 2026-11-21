#include <nitro/types.h>

extern u32 data_0207d3a0;
void OSi_DoBoot(void);
void OSi_ReloadRomData(void);

asm void OSi_DoResetSystem(void)
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
    bl OSi_ReloadRomData
    bl OSi_DoBoot
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
}


#include <nitro/types.h>

asm void DC_StoreAll(void)
{
    mov r1, #0
@1:
    mov r0, #0
@2:
    orr r2, r1, r0
    mcr p15, 0, r2, c7, c10, 2
    add r0, r0, #0x20
    cmp r0, #0x400
    blt @2
    add r1, r1, #0x40000000
    cmp r1, #0
    bne @1
    bx lr
}


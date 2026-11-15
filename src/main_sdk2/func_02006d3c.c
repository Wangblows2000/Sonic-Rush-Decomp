#include <nitro/types.h>

void OSi_SetExContext(void);
void func_02006de0(void);

asm void func_02006d3c(void)
{
    stmdb sp!, {lr}
    bl OSi_SetExContext
    bl func_02006de0
    ldmia sp!, {lr}
    bx lr
}


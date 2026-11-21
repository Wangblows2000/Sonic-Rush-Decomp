#include <nitro/types.h>

void OSi_SetExContext(void);
void OSi_DisplayExContext(void);

asm void OSi_GetAndDisplayContext(void)
{
    stmdb sp!, {lr}
    bl OSi_SetExContext
    bl OSi_DisplayExContext
    ldmia sp!, {lr}
    bx lr
}


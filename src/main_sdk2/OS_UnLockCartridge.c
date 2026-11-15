#include <nitro/types.h>

void func_020056f8(void);

asm void OS_UnLockCartridge(void)
{
    ldr r1, =func_020056f8
    bx r1
}


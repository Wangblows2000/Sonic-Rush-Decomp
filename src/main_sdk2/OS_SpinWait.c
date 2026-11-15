#include <nitro/types.h>

asm void OS_SpinWait(void)
{
@1:
    subs r0, r0, #4
    bhs @1
    bx lr
}


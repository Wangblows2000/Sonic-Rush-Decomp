#include <nitro/types.h>

#pragma thumb on
asm void func_02000cd8(void)
{
    str r2, [r0]
    str r1, [r0, #4]
    neg r1, r1
    str r1, [r0, #8]
    str r2, [r0, #0xc]
    bx lr
}
#pragma thumb reset


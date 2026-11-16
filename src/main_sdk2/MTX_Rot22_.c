#include <nitro/types.h>

#pragma thumb on
asm void MTX_Rot22_(void)
{
    str r2, [r0]
    str r1, [r0, #4]
    neg r1, r1
    str r1, [r0, #8]
    str r2, [r0, #0xc]
    bx lr
}
#pragma thumb reset


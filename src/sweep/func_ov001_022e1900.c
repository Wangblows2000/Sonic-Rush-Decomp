#include <nitro/types.h>

extern s32 gZoneActSelectWork;

#pragma thumb on
asm s32 func_ov001_022e1900(void)
{
    ldr r0, =gZoneActSelectWork
    ldr r0, [r0]
    cmp r0, #0
    bx lr
}
#pragma thumb reset

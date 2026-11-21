#include <nitro/types.h>

asm void MIi_CpuClear32(register u32 data, register void *dest, register u32 size)
{
    add ip, r1, r2
@1:
    cmp r1, ip
    stmltia r1!, {r0}
    blt @1
    bx lr
}

#include <nitro/types.h>

void GX_SetGraphicsMode(s32 a, s32 b, s32 c);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

#pragma thumb on
void ZoneActSelect_ResetDisplay(void) {
    vu32 z;

    GX_SetGraphicsMode(1, 5, 1);
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1900;
    z = 0;
    MIi_CpuClear32(z, (void *)0x6004000, 0xc000);
}
#pragma thumb reset

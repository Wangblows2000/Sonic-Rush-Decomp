#include <nitro/types.h>

extern u16 gDisplayMain[];
extern u16 gDisplaySub[];

void MIi_CpuClear32(u32 data, void *dest, u32 size);

#pragma thumb on
void ZoneActSelectMode_ResetDisplay(void) {
    vu32 z0;
    vu32 z1;

    gDisplayMain[0] = 0;
    gDisplayMain[1] = 0;
    gDisplayMain[2] = 0;
    gDisplayMain[3] = 0;
    gDisplayMain[4] = 0;
    gDisplayMain[5] = 0;
    gDisplayMain[6] = 0;
    gDisplayMain[7] = 0;
    gDisplaySub[0] = 0;
    gDisplaySub[1] = 0;
    gDisplaySub[2] = 0;
    gDisplaySub[3] = 0;
    gDisplaySub[4] = 0;
    gDisplaySub[5] = 0;
    gDisplaySub[6] = 0;
    gDisplaySub[7] = 0;
    z0 = 0;
    MIi_CpuClear32(z0, (void *)0x6000000, 0x80000);
    z1 = 0;
    MIi_CpuClear32(z1, (void *)0x6200000, 0x20000);
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0;
}
#pragma thumb reset

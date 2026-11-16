#include <nitro/types.h>

void MIi_CpuClear32(u32 data, void *dest, u32 size);

void SpecialStageIntroBg_Destructor(void) {
    vu32 zero;
    {
        u32 v = (*(vu32 *)0x4000000 & 0x1f00) >> 8;
        *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | ((v & ~6) << 8);
    }
    zero = 0;
    MIi_CpuClear32(zero, (void *)0x6000000, 0x1000);
}

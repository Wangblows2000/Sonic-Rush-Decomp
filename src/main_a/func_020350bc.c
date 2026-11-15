#include <nitro/types.h>

void MIi_CpuClear32(u32 data, void *dest, u32 size);
extern u8 data_0224ffa0[];
extern u8 data_022501a0[];
extern u8 data_0224ff44[];
extern u32 data_0224ff40;

void func_020350bc(void) {
    vu32 z0, z1, z2;
    z0 = 0;
    MIi_CpuClear32(z0, data_0224ffa0, 0x200);
    z1 = 0;
    MIi_CpuClear32(z1, data_022501a0, 0x240);
    z2 = 0;
    MIi_CpuClear32(z2, data_0224ff44, 0x40);
    data_0224ff40 = 0;
}

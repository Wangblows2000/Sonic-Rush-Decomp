#include <nitro/types.h>

extern u16 data_0208922c[];
extern u16 data_02087d04[];

void MIi_CpuClear32(u32 data, void *dest, u32 size);

#pragma thumb on
void func_ov001_022d3044(void) {
    vu32 z0;
    vu32 z1;
    vu32 z2;
    vu32 z3;
    vu32 z4;
    vu32 z5;

    data_0208922c[0] = 0;
    data_0208922c[1] = 0;
    data_0208922c[2] = 0;
    data_0208922c[3] = 0;
    data_0208922c[4] = 0;
    data_0208922c[5] = 0;
    data_0208922c[6] = 0;
    data_0208922c[7] = 0;
    data_02087d04[0] = 0;
    data_02087d04[1] = 0;
    data_02087d04[2] = 0;
    data_02087d04[3] = 0;
    data_02087d04[4] = 0;
    data_02087d04[5] = 0;
    data_02087d04[6] = 0;
    data_02087d04[7] = 0;
    z0 = 0;
    MIi_CpuClear32(z0, (void *)0x6000000, 0x80000);
    z1 = 0;
    MIi_CpuClear32(z1, (void *)0x6200000, 0x20000);
    z2 = 0;
    MIi_CpuClear32(z2, (void *)0x5000000, 0x200);
    z3 = 0;
    MIi_CpuClear32(z3, (void *)0x5000400, 0x200);
    z4 = 0;
    MIi_CpuClear32(z4, (void *)0x5000200, 0x200);
    z5 = 0;
    MIi_CpuClear32(z5, (void *)0x5000600, 0x200);
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0;
}
#pragma thumb reset

#include <nitro/types.h>

extern u16 data_0208922c[];
extern u16 data_02087d04[];

void MIi_CpuClear32(u32 data, void *dest, u32 size);

#pragma thumb on
void func_ov001_022ec41c(void) {
    vu32 z0;
    vu32 z1;

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
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0;
}
#pragma thumb reset

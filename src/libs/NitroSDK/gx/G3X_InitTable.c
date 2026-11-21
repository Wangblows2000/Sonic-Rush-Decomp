#include <nitro/types.h>

extern u32 data_02077964;

void MI_DmaFill32Async(u32 dmaNo, void *dest, u32 data, u32 size, void *callback, void *arg);
void MI_DmaFill32(u32 dmaNo, void *dest, u32 data, u32 size);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

void G3X_InitTable(void)
{
    s32 i;

    if (data_02077964 != (u32)-1) {
        MI_DmaFill32Async(data_02077964, (void *)0x04000330, 0, 16, NULL, NULL);
        MI_DmaFill32(data_02077964, (void *)0x04000360, 0, 96);
    } else {
        {
            vu32 z = 0;
            MIi_CpuClear32(z, (void *)0x04000330, 16);
        }
        {
            vu32 z = 0;
            MIi_CpuClear32(z, (void *)0x04000360, 96);
        }
    }
    for (i = 0; i < 32; i++) {
        *(vu32 *)0x040004d0 = 0;
    }
}

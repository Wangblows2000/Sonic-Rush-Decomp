#include <nitro/types.h>

extern u32 data_02077964;

void func_02007c54(u32 dmaNo, void *dest, u32 data, u32 size, void *callback, void *arg);
void func_02007e20(u32 dmaNo, void *dest, u32 data, u32 size);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

void func_02003f88(void)
{
    s32 i;

    if (data_02077964 != (u32)-1) {
        func_02007c54(data_02077964, (void *)0x04000330, 0, 16, NULL, NULL);
        func_02007e20(data_02077964, (void *)0x04000360, 0, 96);
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

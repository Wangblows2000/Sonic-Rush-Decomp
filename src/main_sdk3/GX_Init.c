#include <nitro/types.h>

extern u16 data_0207cfe0;
extern u32 data_02077964;

void GX_InitGXState(void);
s32 OS_GetLockID(void);
void OS_Terminate(void);
void MI_DmaFill32(u32 dmaNo, void *dest, u32 data, u32 size);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

#define REG(t, a) (*(volatile t *)(a))

void GX_Init(void)
{
    REG(u16, 0x04000304) |= 0x8000;
    REG(u16, 0x04000304) = (u16)((REG(u16, 0x04000304) & ~0x20e) | 0x20e);
    REG(u16, 0x04000304) |= 1;
    GX_InitGXState();
    while (data_0207cfe0 == 0) {
        s32 lockResult = OS_GetLockID();
        if (lockResult == -3) {
            OS_Terminate();
        }
        data_0207cfe0 = (u16)lockResult;
    }
    REG(u16, 0x04000004) = 0;
    REG(u32, 0x04000000) = 0;
    if (data_02077964 != (u32)-1) {
        MI_DmaFill32(data_02077964, (void *)0x04000008, 0, 0x60);
        REG(u16, 0x0400006c) = 0;
        MI_DmaFill32(data_02077964, (void *)0x04001000, 0, 0x70);
    } else {
        {
            vu32 z = 0;
            MIi_CpuClear32(z, (void *)0x04000008, 0x60);
        }
        REG(u16, 0x0400006c) = 0;
        {
            vu32 z = 0;
            MIi_CpuClear32(z, (void *)0x04001000, 0x70);
        }
    }
    REG(u16, 0x04000020) = 0x100;
    REG(u16, 0x04000026) = 0x100;
    REG(u16, 0x04000030) = 0x100;
    REG(u16, 0x04000036) = 0x100;
    REG(u16, 0x04001020) = 0x100;
    REG(u16, 0x04001026) = 0x100;
    REG(u16, 0x04001030) = 0x100;
    REG(u16, 0x04001036) = 0x100;
}

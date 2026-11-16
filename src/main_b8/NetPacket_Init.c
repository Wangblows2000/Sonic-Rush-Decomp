#include <nitro/types.h>

extern s32 data_022c4f18;
extern u16 data_022c4f1c[];
extern s32 data_022c4f14;
extern u8 data_0207cfb4[];
extern u8 data_0207cfb0;

void MIi_CpuClear16(u16 data, void *dest, u32 size);

void NetPacket_Init(void) {
    vu16 z;
    u8 i;

    z = 0;
    data_022c4f18 = 0;
    MIi_CpuClear16(z, data_022c4f1c, 0x300);
    data_022c4f14 = 0;
    for (i = 0; i < 0x10; i++)
        data_0207cfb4[i] = i;
    data_0207cfb0 = 0xff;
}

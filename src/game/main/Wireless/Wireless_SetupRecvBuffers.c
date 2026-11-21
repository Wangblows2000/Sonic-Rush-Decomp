#include <nitro/types.h>

void MIi_CpuClear32(u32 data, void *dest, u32 size);
extern u8 gWirelessSendBuffer[];
extern u8 data_022501a0[];
extern u8 *gWirelessRecvBuffers[];

void Wireless_SetupRecvBuffers(u32 size) {
    vu32 z0, z1, z2;
    u16 step;
    u16 i;
    u8 *p;
    z0 = 0;
    MIi_CpuClear32(z0, gWirelessSendBuffer, 0x200);
    z1 = 0;
    MIi_CpuClear32(z1, data_022501a0, 0x240);
    z2 = 0;
    MIi_CpuClear32(z2, gWirelessRecvBuffers, 0x40);
    step = (size + 3) & ~3;
    p = data_022501a0;
    for (i = 0; i < 16; i++) {
        gWirelessRecvBuffers[i] = p;
        p += step;
    }
}

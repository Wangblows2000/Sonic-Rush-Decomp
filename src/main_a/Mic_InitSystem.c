#include <nitro/types.h>

void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIC_Init(void);
extern u16 gMic[];

void Mic_InitSystem(void) {
    vu16 v;
    MIC_Init();
    v = 0;
    MIi_CpuClear16(v, gMic, 0x40);
}

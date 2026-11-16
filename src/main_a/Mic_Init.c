#include <nitro/types.h>

void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_0200dda8(void);
extern u16 gMic[];

void Mic_Init(void) {
    vu16 v;
    func_0200dda8();
    v = 0;
    MIi_CpuClear16(v, gMic, 0x40);
}

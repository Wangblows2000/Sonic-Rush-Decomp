#include <nitro/types.h>

void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_0200dda8(void);
extern u16 data_0224ff00[];

void func_02034e8c(void) {
    vu16 v;
    func_0200dda8();
    v = 0;
    MIi_CpuClear16(v, data_0224ff00, 0x40);
}

#include <nitro/types.h>

void MIi_CpuClear16(u16 data, void *dest, u32 size);
extern u16 data_0208f78e[];

void func_020336c8(void) {
    vu16 v = 0;
    MIi_CpuClear16(v, data_0208f78e, 0x40);
}

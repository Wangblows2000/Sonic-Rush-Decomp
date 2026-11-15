#include <nitro/types.h>

extern u8 data_0208af8a;
void MIi_CpuClear16(u16 data, void *dst, u32 size);
void func_020395f4(void) { vu16 v = 0; MIi_CpuClear16(v, &data_0208af8a, 0x4800); }

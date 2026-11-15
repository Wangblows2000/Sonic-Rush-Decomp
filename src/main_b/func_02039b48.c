#include <nitro/types.h>

extern u8 data_0208a77e;
extern u8 data_0208af7e;
extern u8 data_0208af8a;
void MIi_CpuClear16(u16 data, void *dst, u32 size);
void func_02039b48(void) { vu16 a = 0; vu16 b; vu16 c; MIi_CpuClear16(a, &data_0208a77e, 0x800); b = 0; MIi_CpuClear16(b, &data_0208af7e, 8); c = 0; MIi_CpuClear16(c, &data_0208af8a, 0x4800); }

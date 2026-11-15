#include <nitro/types.h>

extern u8 *data_02072c78[];
void MIi_CpuClear16(u16 data, void *dst, u32 size);
void func_02039d08(s32 i) { vu16 v = 0; MIi_CpuClear16(v, data_02072c78[i] + 0x98, 0x800); }

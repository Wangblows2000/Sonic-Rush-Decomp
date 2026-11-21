#include <nitro/types.h>

extern u8 data_022c19b4;
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void WH_SetSsid(const void *src, u32 size) { MIi_CpuCopy16(src, &data_022c19b4, size); }

#include <nitro/types.h>

void MI_CpuFill8(void *dst, u8 v, u32 size);
void MBi_InitCache(void *p) { MI_CpuFill8(p, 0, 0x70); }

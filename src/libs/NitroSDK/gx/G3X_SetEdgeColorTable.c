#include <nitro/types.h>

void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void G3X_SetEdgeColorTable(const void *src) { MIi_CpuCopy16(src, (void *)0x4000330, 0x10); }

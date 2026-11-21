#include <nitro/types.h>

void MI_CpuCopy8(const void *src, void *dst, u32 size);
s32 WFSi_ReadRomCallback(void *a, void *dst, const void *src, u32 size) { MI_CpuCopy8(src, dst, size); return 0; }

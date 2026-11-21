#include <nitro/types.h>

void MI_CpuCopy8(const void *src, void *dst, u32 size);
typedef struct { u8 pad[0x1c]; u8 *base; } UnkStruct_0200b878;
u32 FSi_WriteMemCallback(UnkStruct_0200b878 *p, const void *src, u32 off, u32 size) { MI_CpuCopy8(src, p->base + off, size); return 0; }

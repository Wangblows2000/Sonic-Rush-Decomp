#include <nitro/types.h>

void MI_CpuCopy8(const void *src, void *dst, u32 size);
typedef struct { u8 pad[0x1c]; u8 *base; } UnkStruct_0200b8a4;
u32 FSi_ReadMemCallback(UnkStruct_0200b8a4 *p, void *dst, u32 off, u32 size) { MI_CpuCopy8(p->base + off, dst, size); return 0; }

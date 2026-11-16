#include <nitro/types.h>

void DC_FlushRange(void *p, u32 size);
void DC_WaitWriteBufferEmpty(void);
void func_020183e8(void *c, u32 a, u32 b);
void MBi_BlockHeaderEnd(u32 a, u32 b, void *c) { DC_FlushRange(c, (a + 0x1f) & ~0x1f); DC_WaitWriteBufferEmpty(); func_020183e8(c, a, b); }

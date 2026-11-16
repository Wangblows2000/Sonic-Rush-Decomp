#include <nitro/types.h>

void DC_FlushRange(void *p, u32 size);
void DC_WaitWriteBufferEmpty(void);
void MBi_SendMP(void *c, u32 a, u32 b);
void MBi_BlockHeaderEnd(u32 a, u32 b, void *c) { DC_FlushRange(c, (a + 0x1f) & ~0x1f); DC_WaitWriteBufferEmpty(); MBi_SendMP(c, a, b); }

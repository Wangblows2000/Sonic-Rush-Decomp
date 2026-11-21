#include <nitro/types.h>

void GX_SendFifo48B(const void *src, void *dst);
void G3_MultMtx43(const void *m) { *(vu32 *)0x4000400 = 0x19; GX_SendFifo48B(m, (void *)0x4000400); }

#include <nitro/types.h>

void MI_CpuFill8(void *dst, u8 v, u32 size);
extern void *data_02080f44;
void MBi_SetParentPieceBuffer(void *p) { data_02080f44 = p; MI_CpuFill8(p, 0, 0x21c); }

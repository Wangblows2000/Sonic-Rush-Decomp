#include <nitro/types.h>

s32 PXI_SendWordByFifo(u32 tag, u32 data, u32 err);
void PMi_SendPxiData(u32 id) { while (PXI_SendWordByFifo(0x8, id, 0) != 0) {} }

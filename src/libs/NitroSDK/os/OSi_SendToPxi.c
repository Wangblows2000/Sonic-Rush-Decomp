#include <nitro/types.h>

s32 PXI_SendWordByFifo(u32 tag, u32 data, u32 err);
void OSi_SendToPxi(u32 id) { while (PXI_SendWordByFifo(0xc, id << 8, 0) != 0) {} }

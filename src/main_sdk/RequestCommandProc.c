#include <nitro/types.h>

s32 PXI_SendWordByFifo(u32 tag, u32 data, u32 err);
void RequestCommandProc(void) { while (PXI_SendWordByFifo(7, 0, 0) < 0) {} }

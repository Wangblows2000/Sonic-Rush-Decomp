#include <nitro/types.h>

s32 PXI_SendWordByFifo(u32 fifo, u32 data, BOOL err);
void WaitByLoop(s32 count);

void CTRDGi_SendtoPxi(u32 data)
{
    while (PXI_SendWordByFifo(13, data, FALSE) != 0) {
        WaitByLoop(1);
    }
}

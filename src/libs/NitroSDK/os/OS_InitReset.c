#include <nitro/types.h>

extern u16 data_0207d39c;

void PXI_Init(void);
BOOL PXI_IsCallbackReady(u32 fifotag, u32 proc);
void PXI_SetFifoRecvCallback(u32 tag, void (*fn)(u32, u32, BOOL));
void OSi_CommonCallback(u32 tag, u32 data, BOOL err);

void OS_InitReset(void)
{
    if (data_0207d39c) {
        return;
    }
    data_0207d39c = TRUE;
    PXI_Init();
    while (!PXI_IsCallbackReady(12, 1)) {
    }
    PXI_SetFifoRecvCallback(12, OSi_CommonCallback);
}

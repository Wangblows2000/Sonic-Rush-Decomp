#include <nitro/types.h>

extern u32 data_02080f5c;
extern u32 data_02080f60;

void CTRDGi_InitCommon(void);
void PXI_Init(void);
BOOL PXI_IsCallbackReady(u32 fifo, u32 proc);
void PXI_SetFifoRecvCallback(u32 fifo, void (*cb)(u32, u32, BOOL));
void CTRDGi_CallbackForInitModuleInfo(u32 tag, u32 data, BOOL err);
void CTRDGi_PulledOutCallback(u32 tag, u32 data, BOOL err);
void CTRDGi_InitModuleInfo(void);

void CTRDG_Init(void)
{
    if (data_02080f5c != 0) {
        return;
    }
    data_02080f5c = 1;
    CTRDGi_InitCommon();
    PXI_Init();
    while (!PXI_IsCallbackReady(13, 1)) {
    }
    PXI_SetFifoRecvCallback(13, CTRDGi_CallbackForInitModuleInfo);
    CTRDGi_InitModuleInfo();
    PXI_SetFifoRecvCallback(13, NULL);
    PXI_SetFifoRecvCallback(13, CTRDGi_PulledOutCallback);
    data_02080f60 = 0;
}

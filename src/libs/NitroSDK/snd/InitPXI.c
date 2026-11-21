#include <nitro/types.h>

extern void PXI_SetFifoRecvCallback(u32 fifotag, void (*callback)(u32, u32, BOOL));
extern void PxiFifoCallback(u32 tag, u32 data, BOOL err);
extern BOOL IsCommandAvailable(void);
extern BOOL PXI_IsCallbackReady(u32 fifotag, u32 proc);
extern void OS_SpinWait(u32 cycles);

void InitPXI(void)
{
    PXI_SetFifoRecvCallback(7, PxiFifoCallback);
    if (!IsCommandAvailable()) {
        return;
    }
    while (!PXI_IsCallbackReady(7, 1)) {
        OS_SpinWait(100);
    }
}

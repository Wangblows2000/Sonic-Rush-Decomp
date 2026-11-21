#include <nitro/types.h>
#include <main_sdk2/wmsys.h>

extern u16 data_0207fae8;

s32 WMi_CheckInitialized(void);
s32 WMi_CheckStateEx(s32 num, ...);
void WmClearFifoRecvFlag(void);
void PXI_SetFifoRecvCallback(u32 fifo, void *cb);

s32 WM_Finish(void)
{
    s32 r;
    if (WMi_CheckInitialized() != 0) {
        return 3;
    }
    r = WMi_CheckStateEx(1, 0);
    if (r != 0) {
        return r;
    }
    WmClearFifoRecvFlag();
    PXI_SetFifoRecvCallback(10, NULL);
    data_0207faec = NULL;
    data_0207fae8 = 0;
    return 0;
}

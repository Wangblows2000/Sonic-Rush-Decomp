#include <nitro/types.h>
#include <main_sdk2/wm.h>

s32 WM_Init(void *buf, u16 dma);

s32 WM_Initialize(void *buf, void *cb, u16 dma)
{
    UnkWmWork *w;
    s32 r = WM_Init(buf, dma);
    if (r != 0) {
        return r;
    }
    WMi_SetCallbackTable(0, cb);
    w = WMi_GetSystemWork();
    r = WMi_SendCommand(0, 3, w->unk0, w->unk4, w->unk10);
    if (r == 0) {
        r = 2;
    }
    return r;
}

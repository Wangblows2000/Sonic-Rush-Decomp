#include <nitro/types.h>
#include <main_sdk2/wm.h>

void DC_InvalidateRange(void *p, u32 size);

s32 WM_EndMP(void *cb)
{
    UnkWmWork *w = WMi_GetSystemWork();
    s32 r = WMi_CheckStateEx(2, 9, 10);
    if (r != 0) {
        return r;
    }
    DC_InvalidateRange(w->unk4 + 0xc, 4);
    if (*(u32 *)(w->unk4 + 0xc) == 0) {
        return 3;
    }
    WMi_SetCallbackTable(0x10, cb);
    r = WMi_SendCommand(0x10, 0);
    if (r == 0) {
        r = 2;
    }
    return r;
}

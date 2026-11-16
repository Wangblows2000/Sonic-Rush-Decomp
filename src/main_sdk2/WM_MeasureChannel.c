#include <nitro/types.h>
#include <main_sdk2/wm.h>

s32 WM_MeasureChannel(void *cb, u16 a, u16 b, u16 c, u16 d)
{
    UnkWmWork *w = WMi_GetSystemWork();
    u16 *buf;
    s32 r = WMi_CheckStateEx(1, 2);
    if (r != 0) {
        return r;
    }
    WMi_SetCallbackTable(0x1e, cb);
    buf = w->unkC;
    buf[0] = 0x1e;
    buf[1] = a;
    buf[2] = b;
    buf[3] = c;
    buf[4] = d;
    r = WMi_SendCommand(0x1e, 0);
    if (r == 0) {
        r = 2;
    }
    return r;
}

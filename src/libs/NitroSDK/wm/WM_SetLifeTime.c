#include <nitro/types.h>
#include <NitroSDK/wm.h>

s32 WM_SetLifeTime(void *cb, u32 a, u32 b, u32 c, u16 d)
{
    s32 r = WMi_CheckIdle();
    if (r != 0) {
        return r;
    }
    WMi_SetCallbackTable(0x1d, cb);
    r = WMi_SendCommand(0x1d, 4, a, b, c, d);
    if (r == 0) {
        r = 2;
    }
    return r;
}

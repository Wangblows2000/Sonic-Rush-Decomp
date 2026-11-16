#include <nitro/types.h>
#include <main_sdk2/wm.h>

s32 WMi_StartParentEx(void *cb, u32 a)
{
    s32 r = WMi_CheckStateEx(1, 2);
    if (r != 0) {
        return r;
    }
    WMi_SetCallbackTable(8, cb);
    r = WMi_SendCommand(8, 1, a);
    if (r == 0) {
        r = 2;
    }
    return r;
}

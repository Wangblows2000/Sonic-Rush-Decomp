#include <nitro/types.h>
#include <NitroSDK/wm.h>

s32 WM_SetBeaconIndication(void *cb, u32 a)
{
    s32 r = WMi_CheckIdle();
    if (r != 0) {
        return r;
    }
    if (a != 0 && a != 1) {
        return 6;
    }
    WMi_SetCallbackTable(0x19, cb);
    r = WMi_SendCommand(0x19, 1, a);
    if (r == 0) {
        r = 2;
    }
    return r;
}

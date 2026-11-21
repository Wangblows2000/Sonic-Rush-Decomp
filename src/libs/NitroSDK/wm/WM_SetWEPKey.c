#include <nitro/types.h>
#include <NitroSDK/wm.h>

void DC_StoreRange(void *p, u32 size);

s32 WM_SetWEPKey(void *cb, u32 n, void *p)
{
    s32 r = WMi_CheckIdle();
    if (r != 0) {
        return r;
    }
    if (n > 3) {
        return 6;
    }
    if (n != 0) {
        if (p == NULL) {
            return 6;
        }
        DC_StoreRange(p, 0x50);
    }
    WMi_SetCallbackTable(0x14, cb);
    r = WMi_SendCommand(0x14, 2, n, p);
    if (r == 0) {
        r = 2;
    }
    return r;
}

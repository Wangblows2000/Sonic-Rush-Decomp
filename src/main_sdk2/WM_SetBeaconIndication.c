#include <nitro/types.h>
#include <main_sdk2/wm.h>

s32 WM_SetBeaconIndication(void *cb, u32 a)
{
    s32 r = WMi_CheckIdle();
    if (r != 0) {
        return r;
    }
    if (a != 0 && a != 1) {
        return 6;
    }
    func_02010cd4(0x19, cb);
    r = func_02010b80(0x19, 1, a);
    if (r == 0) {
        r = 2;
    }
    return r;
}

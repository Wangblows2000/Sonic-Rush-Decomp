#include <nitro/types.h>
#include <NitroSDK/wm.h>
#include <NitroSDK/wh.h>

void DC_StoreRange(void *p, u32 size);
void WmCheckParentParameter(UnkParentParam_wh *pp);

s32 WM_SetParentParameter(void *cb, UnkParentParam_wh *pp)
{
    s32 r = WMi_CheckStateEx(1, 2);
    if (r != 0) {
        return r;
    }
    if (pp == NULL) {
        return 6;
    }
    if (pp->userGameInfoLength != 0 && pp->userGameInfo == NULL) {
        return 6;
    }
    if (pp->parentMaxSize + (pp->KS_Flag ? 0x2a : 0) > 0x200 || pp->childMaxSize + (pp->KS_Flag ? 6 : 0) > 0x200) {
        return 6;
    }
    WmCheckParentParameter(pp);
    WMi_SetCallbackTable(7, cb);
    DC_StoreRange(pp, sizeof(UnkParentParam_wh));
    if (pp->userGameInfoLength != 0) {
        DC_StoreRange(pp->userGameInfo, pp->userGameInfoLength);
    }
    r = WMi_SendCommand(7, 1, pp);
    if (r == 0) {
        r = 2;
    }
    return r;
}

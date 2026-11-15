#include <nitro/types.h>
#include <main_sdk2/wm.h>
#include <main_sdk2/wh.h>

void DC_StoreRange(void *p, u32 size);
void func_02011970(UnkParentParam_wh *pp);

s32 func_020119c0(void *cb, UnkParentParam_wh *pp)
{
    s32 r = func_02010a50(1, 2);
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
    func_02011970(pp);
    func_02010cd4(7, cb);
    DC_StoreRange(pp, sizeof(UnkParentParam_wh));
    if (pp->userGameInfoLength != 0) {
        DC_StoreRange(pp->userGameInfo, pp->userGameInfoLength);
    }
    r = func_02010b80(7, 1, pp);
    if (r == 0) {
        r = 2;
    }
    return r;
}

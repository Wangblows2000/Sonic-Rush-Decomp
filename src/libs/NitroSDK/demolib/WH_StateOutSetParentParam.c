/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 WH_SetError();
s32 WH_ChangeSysState();
extern u8 data_022c198c[];
s32 WH_StateInSetParentWEPKey();
s32 WH_StateInStartParent();

void WH_StateOutSetParentParam(u8 *p0) {
    s32 t0 = *(u16 *)((u8 *)p0 + 0x2);
    if (t0 != 0) {
        WH_SetError(t0);
        WH_ChangeSysState(9);
        return;
    }
    s32 t1 = *(u32 *)data_022c198c;
    if (t1 != 0) {
        s32 t2 = WH_StateInSetParentWEPKey(t1);
        if (t2 != 0) {
            return;
        }
        WH_ChangeSysState(9);
        return;
    }
    s32 t3 = WH_StateInStartParent();
    if (t3 != 0) {
        return;
    }
    WH_ChangeSysState(9);
}

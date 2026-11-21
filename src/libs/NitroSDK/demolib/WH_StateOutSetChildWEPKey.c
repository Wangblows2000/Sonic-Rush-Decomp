/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 WH_SetError();
s32 WH_ChangeSysState();
s32 WH_StateInStartChild();
extern u8 sTraceFunc[];
extern u8 data_020788e4[];

void WH_StateOutSetChildWEPKey(u8 *p0) {
    s32 t0 = *(u16 *)((u8 *)p0 + 0x2);
    if (t0 != 0) {
        WH_SetError(t0);
        WH_ChangeSysState(9);
        return;
    }
    s32 t1 = WH_StateInStartChild();
    if (t1 != 0) {
        return;
    }
    s32 t2 = *(u32 *)sTraceFunc;
    if (t2 != 0) {
        ((s32 (*)())t2)(data_020788e4, t2);
    }
    WH_ChangeSysState(9);
}

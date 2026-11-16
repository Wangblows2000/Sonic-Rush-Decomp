/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 WH_SetError();
s32 WH_ChangeSysState();
extern u8 data_022c1940[];
extern u8 data_022c1994[];
s32 WH_StateInSetChildWEPKey();
s32 WH_StateInStartChild();
extern u8 data_022c197c[];
extern u8 data_02078914[];

void WH_StateOutEndScan(u8 *p0) {
    s32 t0 = *(u16 *)((u8 *)p0 + 0x2);
    if (t0 != 0) {
        WH_SetError(t0);
        return;
    }
    WH_ChangeSysState(1);
    if (*(u16 *)data_022c1940 == 0) {
        return;
    }
    s32 t1 = *(u32 *)data_022c1994;
    if (t1 != 0) {
        s32 t2 = WH_StateInSetChildWEPKey(t1);
        if (t2 != 0) {
            return;
        }
        WH_ChangeSysState(9);
        return;
    }
    s32 t3 = WH_StateInStartChild();
    if (t3 != 0) {
        return;
    }
    s32 t4 = *(u32 *)data_022c197c;
    if (t4 != 0) {
        ((s32 (*)())t4)(data_02078914, t4);
    }
    WH_ChangeSysState(9);
}

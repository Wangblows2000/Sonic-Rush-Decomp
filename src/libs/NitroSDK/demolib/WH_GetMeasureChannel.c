/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 sSysState[];
s32 OS_Terminate();
s32 WH_ChangeSysState();
extern u8 data_022c1954[];
s32 SelectChannel();
extern u8 sTraceFunc[];
extern u8 data_022c1960[];
extern u8 data_02078780[];

s32 WH_GetMeasureChannel(void) {
    s32 t0 = *(u32 *)sSysState;
    if (t0 != 7) {
        OS_Terminate(t0);
    }
    WH_ChangeSysState(1);
    s32 t1 = SelectChannel(*(u16 *)data_022c1954);
    s32 t2 = *(u32 *)sTraceFunc;
    *(u16 *)data_022c1960 = t1;
    if (t2 != 0) {
        ((s32 (*)())t2)(data_02078780, *(u16 *)data_022c1960, data_022c1960, t2);
    }
    return *(u16 *)data_022c1960;
}

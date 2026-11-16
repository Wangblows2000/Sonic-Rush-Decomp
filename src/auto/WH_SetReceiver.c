/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c1970[];
void WH_PortReceiveCallback();
s32 func_020114f0();
s32 WH_ChangeSysState();
extern u8 sTraceFunc[];
extern u8 data_02078708[];

void WH_SetReceiver(s32 p0) {
    *(u32 *)data_022c1970 = p0;
    s32 t0 = func_020114f0(0xe, (u32)WH_PortReceiveCallback, 0);
    if (t0 == 0) {
        return;
    }
    WH_ChangeSysState(9);
    s32 t1 = *(u32 *)sTraceFunc;
    if (t1 == 0) {
        return;
    }
    ((s32 (*)())t1)(data_02078708, t1);
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 sSysState[];
extern u8 sTraceFunc[];
extern u8 data_020788b8[];
s32 WH_ChangeSysState();
extern u8 data_022c1994[];
extern u8 data_022c1ae0[];
extern u8 data_022c19b4[];
void WH_StateOutStartChild();
s32 WM_StartConnectEx();
s32 WH_SetError();

s32 WH_StateInStartChild(void) {
    if ((u32)(*(u32 *)sSysState - 0x4) <= (u32)2) {
        s32 t0 = *(u32 *)sTraceFunc;
        if (t0 != 0) {
            ((s32 (*)())t0)(data_020788b8, t0);
        }
        return 1;
    }
    WH_ChangeSysState(3);
    s32 t1 = WM_StartConnectEx((u32)WH_StateOutStartChild, data_022c1ae0, data_022c19b4, 1, ((u32)((*(u32 *)data_022c1994 != 0) << 0x10) >> 0x10));
    if (t1 == 2) {
        return 1;
    }
    WH_SetError(t1);
    return 0;
}

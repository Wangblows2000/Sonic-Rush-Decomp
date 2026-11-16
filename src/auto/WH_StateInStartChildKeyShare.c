/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 sSysState[];
s32 WH_ChangeSysState();
void func_0203ede0();
extern u8 data_022c1a40[];
s32 func_02012da0();
s32 WH_SetError();

s32 WH_StateInStartChildKeyShare(void) {
    s32 t0 = *(u32 *)sSysState;
    if (t0 == 6) {
        return 1;
    }
    if (t0 != 4) {
        return 0;
    }
    WH_ChangeSysState(6);
    s32 t1 = func_02012da0((u32)func_0203ede0, data_022c1a40);
    if (t1 == 2) {
        return 1;
    }
    WH_SetError(t1);
    return 0;
}

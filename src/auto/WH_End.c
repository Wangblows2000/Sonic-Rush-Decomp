/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 sSysState[];
s32 OS_Terminate();
s32 WH_ChangeSysState();
void WH_StateOutEnd();
s32 WM_End();

s32 WH_End(void) {
    s32 t0 = *(u32 *)sSysState;
    s32 v1 = 1;
    if (t0 != 1) {
        s32 v2 = v1;
        if (t0 != 7) {
            v2 = 0;
        }
        v1 = v2;
    }
    if (v1 == 0) {
        OS_Terminate(t0, v1);
    }
    WH_ChangeSysState(3);
    s32 t3 = WM_End((u32)WH_StateOutEnd);
    if (t3 == 2) {
        return 1;
    }
    WH_ChangeSysState(9);
    return 0;
}

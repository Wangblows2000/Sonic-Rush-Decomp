/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 WH_ChangeSysState();
extern u8 data_022c1994[];
extern u8 data_022c19a0[];
extern u8 data_022c1ae0[];
void WH_StateOutSetChildWEPKey();
s32 WM_SetWEPKey();
s32 WH_SetError();

s32 WH_StateInSetChildWEPKey(void) {
    WH_ChangeSysState(3);
    s32 t0 = ((s32 (*)())*(u32 *)data_022c1994)(data_022c19a0, data_022c1ae0, *(u32 *)data_022c1994);
    s32 t1 = WM_SetWEPKey((u32)WH_StateOutSetChildWEPKey, t0, data_022c19a0);
    if (t1 == 2) {
        return 1;
    }
    WH_SetError(t1);
    WH_ChangeSysState(9);
    return 0;
}

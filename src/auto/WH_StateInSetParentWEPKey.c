/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 WH_ChangeSysState();
extern u8 data_022c198c[];
extern u8 data_022c19a0[];
extern u8 sParentParam[];
void WH_StateOutSetParentWEPKey();
s32 func_02013064();
s32 WH_SetError();

s32 WH_StateInSetParentWEPKey(void) {
    WH_ChangeSysState(3);
    s32 t0 = ((s32 (*)())*(u32 *)data_022c198c)(data_022c19a0, sParentParam, *(u32 *)data_022c198c);
    s32 t1 = func_02013064((u32)WH_StateOutSetParentWEPKey, t0, data_022c19a0);
    if (t1 == 2) {
        return 1;
    }
    WH_SetError(t1);
    WH_ChangeSysState(9);
    return 0;
}

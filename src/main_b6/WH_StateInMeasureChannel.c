#include <nitro/types.h>

u32 func_020111e4(void);
void WH_SetError(s32 v);
void WH_ChangeSysState(s32 a);
u32 MeasureChannel(void (*cb)(void), u16 ch);
void WH_StateOutMeasureChannel(void);

u16 WH_StateInMeasureChannel(u16 ch) {
    u32 m = func_020111e4();

    if (m == 0x8000) {
        WH_SetError(3);
        WH_ChangeSysState(9);
        return 3;
    }
    if (m == 0) {
        WH_SetError(0x16);
        WH_ChangeSysState(9);
        return 0x18;
    }
    while (!((1 << (ch - 1)) & m)) {
        ch++;
        if (ch > 16)
            return 0x18;
    }
    return MeasureChannel(WH_StateOutMeasureChannel, ch);
}
